//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  The Key class includes functions to book space on a file,           //
//   to create I/O buffers, to fill these buffers                       //
//   to compress/uncompress data buffers.                               //
//                                                                      //
//  Before saving (making persistent) an object on a file, a key must   //
//  be created. The key structure contains all the information to       //
//  uniquely identify a persistent object on a file.                    //
//     fNbytes    = number of bytes for the compressed object+key       //
//     version of the Key class                                         //
//     fObjlen    = Length of uncompressed object                       //
//     fDatime    = Date/Time when the object was written               //
//     fKeylen    = number of bytes for the key structure               //
//     fCycle     = cycle number of the object                          //
//     fSeekKey   = Address of the object on file (points to fNbytes)   //
//                  This is a redundant information used to cross-check //
//                  the data base integrity.                            //
//     fSeekPdir  = Pointer to the directory supporting this object     //
//     fClassName = Object class name                                   //
//     fName      = name of the object                                  //
//     fTitle     = title of the object                                 //
//                                                                      //
//  The Key class is used by ROOT to:                                   //
//    - to write an object in the Current Directory                     //
//    - to write a new ntuple buffer                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// this :
#include <Rio/Core/Key.h>

#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>

#include <Rio/Core/tobuf.h>
#include <Rio/Core/File.h>
#include <Rio/Core/FreeSegment.h>
#include <Rio/Core/Buffer.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Directory.h>
#include <Rio/Core/Out.h>

#include <string.h> //memcpy

#define Rio_Key_Class_Version 2
#define MAXIMUM(a,b) ((a)>(b)?a:b)
const int Rio_kMAXBUF = 0xffffff;

extern "C" {
  void Rio_zip(int,int*,char*,int*,char*,int*);
  void Rio_unzip(int*,unsigned char*,int*,unsigned char*,int*);
}

//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 File& aFile
)
:fFile(aFile)
,fLeft(0)
,fBuffer(0)
,fBufferRef(0)
// Record
,fNumberOfBytes(0)
,fVersion(0)
,fObjectSize(0)
,fKeyLength(0)
,fCycle(0)
,fSeekKey(0)
,fSeekParentDirectory(0)
//////////////////////////////////////////////////////////////////////////////
// Used by Basket.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
  fKeyLength = findRecordSize();
  fDate.setDate(0);
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 File& aFile
,Seek aSeekParentDirectory
,const std::string& aObjectName
,const std::string& aObjectTitle
,const std::string& aObjectClassName
,int aNumberOfBytes
)
:fFile(aFile)
,fLeft(0)
,fBuffer(0)
,fBufferRef(0)
// Record
,fNumberOfBytes(0)
,fVersion(0)
,fObjectSize(aNumberOfBytes)
,fKeyLength(0)
,fCycle(0)
,fSeekKey(0)
,fSeekParentDirectory(0)
,fObjectClassName(aObjectClassName)
,fObjectName(aObjectName)
,fObjectTitle(aObjectTitle)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
  fKeyLength = findRecordSize();
  initialize(aNumberOfBytes);
  fSeekParentDirectory = aSeekParentDirectory;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 File& aFile
,Seek aPointer
,int aNumberOfBytes
)
:fFile(aFile)
,fBuffer(0)
,fBufferRef(0)
// Record :
,fNumberOfBytes(aNumberOfBytes)
,fVersion(0)
,fObjectSize(0)
,fKeyLength(0)
,fCycle(0)
,fSeekKey(aPointer)
,fSeekParentDirectory(0)
//////////////////////////////////////////////////////////////////////////////
// Create a TKey object to read keys :
//  Constructor called by TDirectory::ReadKeys and by TFile::TFile
//  A TKey object is created to read the keys structure itself
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
  fBuffer = new char[aNumberOfBytes];
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 File& aFile
,Directory& aDirectory
,IObject& aObject
,const std::string& aName
,int aBufferSize
)
:fFile(aFile)
,fBuffer(0)
,fBufferRef(0)
// Record :
,fNumberOfBytes(0)
,fVersion(0)
,fObjectSize(0)
,fKeyLength(0)
,fSeekKey(0)
,fSeekParentDirectory(0)
,fObjectName(aName)
,fObjectTitle(aObject.title())
//////////////////////////////////////////////////////////////////////////////
// Create a Key object and fill output buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
  fObjectClassName = aObject.isA().name();
  fBufferRef = new Buffer(fFile.dictionary(),fFile.printer(),
			  Buffer::WRITE,aBufferSize);
  fCycle = aDirectory.appendKey(this);
  stream(*fBufferRef); //write key itself
  fKeyLength = fBufferRef->length();
  //FIXME register obj in map in case of self reference
  fBufferRef->mapObject(&aObject); 
  aObject.stream(*fBufferRef); //write object
  int lbuf = fBufferRef->length();
  fObjectSize = lbuf - fKeyLength;

  int cxlevel = fFile.compressionLevel();
  if (cxlevel && fObjectSize > 256) {
    if (cxlevel == 2) cxlevel--;
    int nbuffers = fObjectSize/Rio_kMAXBUF;
    //add 8 bytes in case object is placed in a deleted gap
    int buflen = 
      MAXIMUM(512,fKeyLength + fObjectSize + 9*nbuffers + 8); 
    fBuffer = new char[buflen];
    char* objbuf = fBufferRef->buffer() + fKeyLength;
    char* bufcur = &fBuffer[fKeyLength];
    int noutot = 0;
    int nout;
    int nzip = 0;
    int bufmax;
    for (int i=0;i<=nbuffers;i++) {
      bufmax = ((i == nbuffers) ? fObjectSize - nzip : Rio_kMAXBUF);
      Rio_zip(cxlevel, &bufmax, objbuf, &bufmax, bufcur, &nout);
      if (nout == 0) { //this happens when the buffer cannot be compressed
	fBuffer = fBufferRef->buffer();
	initialize(fObjectSize);
	fSeekParentDirectory = aDirectory.seekDirectory();
	fBufferRef->setBufferOffset(0);
	stream(*fBufferRef);         //write key itself again
	return;
      }
      bufcur += nout;
      noutot += nout;
      objbuf += Rio_kMAXBUF;
      nzip   += Rio_kMAXBUF;
    }
    initialize(noutot);
    fSeekParentDirectory = aDirectory.seekDirectory();
    fBufferRef->setBufferOffset(0);
    stream(*fBufferRef);         //write key itself again
    ::memcpy(fBuffer,fBufferRef->buffer(),fKeyLength);
    delete fBufferRef; 
    fBufferRef = 0;
  } else {
    fBuffer = fBufferRef->buffer();
    initialize(fObjectSize);
    fSeekParentDirectory = aDirectory.seekDirectory();
    fBufferRef->setBufferOffset(0);
    stream(*fBufferRef);         //write key itself again
  }
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::Key(
 const Key& aKey
)
:fFile(aKey.fFile)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::operator=(
 const Key&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Key::~Key(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  deleteBuffer();
  Debug::decrement();
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Key::className(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fObjectClassName;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Key::name(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fObjectName;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::Key::title(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fObjectTitle;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Buffer* Rio::Key::bufferRef(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBufferRef;
}
//////////////////////////////////////////////////////////////////////////////
short Rio::Key::cycle(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCycle;
}
int Rio::Key::numberOfBytes() const  {return fNumberOfBytes;}
int Rio::Key::objectSize() const  {return fObjectSize;}
int Rio::Key::version() const {return fVersion;}
Rio::Seek Rio::Key::seekKey() const  {return fSeekKey;}
Rio::Seek Rio::Key::seekParentDirectory() const {return fSeekParentDirectory;}
//////////////////////////////////////////////////////////////////////////////
char* Rio::Key::buffer(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fBuffer + fKeyLength;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::initialize(
 int aNumberOfBytes
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Find free segment :
  int nsize = aNumberOfBytes + fKeyLength;
  FreeSegment* bestfree  = fFile.bestFreeSegment(nsize);
  if (bestfree == 0) {
    Out out(fFile.printer());
    out << "Rio::Key::initialize : cannot allocate " 
	<< nsize << " bytes for ID = \"" 
	<< fObjectName << "\" Title = \"" << fObjectTitle << "\"" << endl;
    return;
  }


  fDate.set();
  fSeekKey  = bestfree->first();
  // Case Add at the end of the file :
  if (fSeekKey == fFile.end()) {
    fFile.setEnd(fSeekKey+nsize);
    bestfree->setFirst(fSeekKey+nsize);
    fLeft   = -1;
    if (!fBuffer) fBuffer = new char[nsize];
  } else {
    fLeft = int(bestfree->last() - fSeekKey - nsize + 1);
  }
  // Case where new object fills exactly a deleted gap :
  fNumberOfBytes = nsize;
  if (fLeft == 0) {
    if (!fBuffer) {
      fBuffer = new char[nsize];
    }
    fFile.freeSegments().remove(bestfree);
    delete bestfree;
  }
  // Case where new object is placed in a deleted gap larger than itself :
  if (fLeft > 0) {    // found a bigger segment
    if (!fBuffer) {
      fBuffer = new char[nsize+sizeof(int)];
    }
    char *buffer  = fBuffer+nsize;
    int nbytesleft = -fLeft;  // set header of remaining record
    Rio_tobuf(buffer, nbytesleft);
    bestfree->setFirst(fSeekKey+nsize);
  }

  // Done elsewhere : fSeekParentDirectory = fDirectory.seekDirectory();
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Key::keyLength(
) const  
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fKeyLength;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Key::findRecordSize(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Return the size in bytes of the key header structure.
  int nbytes = sizeof fNumberOfBytes;
  nbytes += sizeof(short);
  nbytes += sizeof fObjectSize;
  nbytes += fDate.recordSize();
  nbytes += sizeof fKeyLength;
  nbytes += sizeof fCycle;
  nbytes += sizeof fSeekKey;
  nbytes += sizeof fSeekParentDirectory;
  nbytes += Rio_std_string_recordSize(fObjectClassName);
  nbytes += Rio_std_string_recordSize(fObjectName);
  nbytes += Rio_std_string_recordSize(fObjectTitle);
  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::fillBuffer(
 char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Encode key header into output buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_tobuf(aBuffer, fNumberOfBytes);
  short version = Rio_Key_Class_Version;
  Rio_tobuf(aBuffer, version);
  Rio_tobuf(aBuffer, fObjectSize);
  Rio_tobuf(aBuffer, fDate.date());
  Rio_tobuf(aBuffer, fKeyLength);
  Rio_tobuf(aBuffer, fCycle);
  Rio_tobuf(aBuffer, fSeekKey);
  Rio_tobuf(aBuffer, fSeekParentDirectory);
  Rio_tobuf(aBuffer, fObjectClassName);
  Rio_tobuf(aBuffer, fObjectName);
  Rio_tobuf(aBuffer, fObjectTitle);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::readBuffer(
 char*& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Decode input buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_frombuf(aBuffer, &fNumberOfBytes);
  short version;
  Rio_frombuf(aBuffer,&version);
  fVersion = version;
  Rio_frombuf(aBuffer, &fObjectSize);
  unsigned int date;
  Rio_frombuf(aBuffer, &date);
  fDate.setDate(date);
  Rio_frombuf(aBuffer, &fKeyLength);
  Rio_frombuf(aBuffer, &fCycle);
  Rio_frombuf(aBuffer, &fSeekKey);
  Rio_frombuf(aBuffer, &fSeekParentDirectory);
  Rio_frombuf(aBuffer, fObjectClassName);
  Rio_frombuf(aBuffer, fObjectName);
  Rio_frombuf(aBuffer, fObjectTitle);
  /*FIXME : TBranch does a gROOT->SetReadingObject.
  if (!gROOT->ReadingObject()) {
    if (fSeekPdir != gDirectory->GetSeekDir()) gDirectory->AppendKey(this);
  }
  */
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
// Stream a class object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    aBuffer.read(fNumberOfBytes);
    aBuffer.readVersion();
    aBuffer.read(fObjectSize);
    unsigned int date;
    aBuffer.read(date);
    fDate.setDate(date);
    aBuffer.read(fKeyLength);
    aBuffer.read(fCycle);
    aBuffer.read(fSeekKey);
    aBuffer.read(fSeekParentDirectory);
    aBuffer.read(fObjectClassName);
    aBuffer.read(fObjectName);
    aBuffer.read(fObjectTitle);
  } else {
    aBuffer.write(fNumberOfBytes);
    aBuffer.writeVersion(Rio_Key_Class_Version);
    aBuffer.write(fObjectSize);
    if (fDate.date() == 0) fDate.set();
    aBuffer.write(fDate.date());
    aBuffer.write(fKeyLength);
    aBuffer.write(fCycle);
    aBuffer.write(fSeekKey);
    aBuffer.write(fSeekParentDirectory);
    aBuffer.write(fObjectClassName);
    aBuffer.write(fObjectName);
    aBuffer.write(fObjectTitle);
  }
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Key::writeFile(
 int aCycle
)
//////////////////////////////////////////////////////////////////////////////
// Write the encoded object supported by this key.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nsize  = fNumberOfBytes;
  char* buffer = fBuffer;
  if (aCycle) {
    fCycle = aCycle;
    fillBuffer(buffer);
    buffer = fBuffer;
  }

  if (fLeft > 0) nsize += sizeof(int);

  fFile.setPosition(fSeekKey);

#if 0
  for (Int_t i=0;i<nsize;i+=kMAXFILEBUFFER) {
     Int_t nb = kMAXFILEBUFFER;
     if (i+nb > nsize) nb = nsize - i;
     gFile->WriteBuffer(buffer,nb);
     buffer += nb;
  }
#else
  fFile.writeBuffer(buffer,nsize);
#endif
  //  gFile->Flush(); Flushing takes too much time.
  //                  Let user flush the file when he wants.
  if (fFile.verboseLevel()) {
    Out out(fFile.printer());
    out << "Rio::Key::writeFile : writing " 
	<< nsize << " bytes at address "
	<< fSeekKey << " for ID=\"" 
	<< fObjectName << "\" Title=\"" << fObjectTitle << "\"." << endl;
  }

  deleteBuffer();
  return nsize;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::readFile(
)
//////////////////////////////////////////////////////////////////////////////
// Read the key structure from the file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int nsize = fNumberOfBytes;
  fFile.setPosition(fSeekKey);
  fFile.readBuffer(fBuffer,nsize);
  if(fFile.verboseLevel()) {
    Out out(fFile.printer());
    out << "Rio::Key::readFile : Reading " 
	<< nsize << " bytes at address " << fSeekKey << "." << endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::deleteBuffer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fBufferRef) {
    // fBuffer may come from a : fBuffer = fBufferRef->buffer();
    delete fBufferRef;
    fBufferRef = 0;
  } else {
    if (fBuffer) delete [] fBuffer;
  }
  fBuffer = 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::Key::readObject(
 Directory& aDirectory
)
//////////////////////////////////////////////////////////////////////////////
// To read an object from the file :
//  The object associated to this key is read from the file into memory
//  Once the key structure is read (via Streamer) the class identifier
//  of the object is known.
//  Using the class identifier we find the TClass object for this class.
//  A TClass object contains a full description (i.e. dictionary) of the
//  associated class. In particular the TClass object can create a new
//  object of the class type it describes. This new object now calls its
//  Streamer function to rebuilt itself.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBufferRef = new Buffer(fFile.dictionary(),fFile.printer(),
			  Buffer::READ,fObjectSize+fKeyLength);
  if(!fBufferRef) {
    Out out(fFile.printer());
    out << "Rio::Key::readObject : cannot allocate buffer: " 
	<< "fObjectSize = " << fObjectSize << endl;
    return 0;
  }
  if (fObjectSize > fNumberOfBytes-fKeyLength) {
    fBuffer = new char[fNumberOfBytes];
    readFile(); //read object structure from file
    ::memcpy(fBufferRef->buffer(),fBuffer,fKeyLength);
  } else {
    fBuffer = fBufferRef->buffer();
    readFile(); //Read object structure from file
   }
  // get version of key
  fBufferRef->setBufferOffset(sizeof(fNumberOfBytes));
  short kvers = fBufferRef->readVersion();
  
  fBufferRef->setBufferOffset(fKeyLength);

  Arguments args;
  Args_insertDirectory(args,(IDirectory*)&aDirectory);

  IClass* cl = fFile.dictionary().findClass(fObjectClassName);
  if (!cl) {
    Out out(fFile.printer());
    out << "Rio::Key::readObject : unknown class \"" 
	<< fObjectClassName << "\"." << endl;
    return 0;
  }

  IObject* obj = cl->create(args);
  if (!obj) {
    Out out(fFile.printer());
    out << "Rio::Key::readObject : cannot create new object of class \"" 
	<< fObjectClassName << "\"." << endl;
    return 0;
  }

  if (kvers > 1)
    fBufferRef->mapObject(obj);  //register obj in map to handle self reference

  if (fObjectSize > fNumberOfBytes-fKeyLength) {
    unsigned char* objbuf = 
      (unsigned char*)(fBufferRef->buffer() + fKeyLength);
    unsigned char* bufcur = (unsigned char*)&fBuffer[fKeyLength];
    int nin, nout, nbuf;
    int noutot = 0;
    while (1) {
      nin  = 
	9 + ((int)bufcur[3] | ((int)bufcur[4] << 8) | ((int)bufcur[5] << 16));
      nbuf = (int)bufcur[6] | ((int)bufcur[7] << 8) | ((int)bufcur[8] << 16);
      Rio_unzip(&nin, bufcur, &nbuf, objbuf, &nout);
      if (!nout) break;
      noutot += nout;
      if (noutot >= fObjectSize) break;
      bufcur += nin;
      objbuf += nout;
    }
    if (nout) {
      obj->stream(*fBufferRef);
      delete [] fBuffer;
    } else {
      delete [] fBuffer;
      delete obj;
      delete fBufferRef;
      fBufferRef = 0;
      fBuffer = 0;
      return 0;
    }
  } else {
    obj->stream(*fBufferRef);
  }
  
  //FIXME if (gROOT->GetForceStyle()) obj->UseCurrentStyle();
  
  Directory* dir = Rio::cast<Directory>(obj);
  if(dir) {
    if(fFile.verboseLevel()) {
      Out out(fFile.printer());
      out << "Rio::Key::readObject : directory : \"" 
	  << name() << "\"" << endl;
    }
    dir->setName(name());
    dir->setTitle(title());
  }

  delete fBufferRef;
  fBufferRef = 0;
  fBuffer = 0;
  
  return obj;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Key::destroy(
)
//////////////////////////////////////////////////////////////////////////////
// Delete an object from the file :
// Note: the key is not deleted. You still have to call "delete key".
// This is different from the behaviour of TObject::Delete()!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fFile.verboseLevel()) {
    Out out(fFile.printer());
    out << "Rio::Key::destroy : " 
	<< fObjectName << " at address " 
	<< fSeekKey << ", nbytes = " << fNumberOfBytes << endl;
  }

  Seek first = fSeekKey;
  Seek last  = fSeekKey + fNumberOfBytes - 1;
  fFile.makeFreeSegment(first, last);  // release space used by this key
  //Done elsewhere : if(fDirectory) fDirectory->removeKey(this);
}

