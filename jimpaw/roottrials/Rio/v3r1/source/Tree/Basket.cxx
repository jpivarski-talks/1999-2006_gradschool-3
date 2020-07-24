// this :
#include <Rio/Tree/Basket.h>

// Core :
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/File.h> //Hard to avoid.
#include <Rio/Core/Buffer.h>
#include <Rio/Core/Lib.h>
#include <Rio/Core/Out.h>

// Tree :
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/Tree.h>

extern "C" {
  void Rio_zip(int,int*,char*,int*,char*,int*);
  void Rio_unzip(int*,unsigned char*,int*,unsigned char*,int*);
}

#define MAXIMUM(a,b) ((a)>(b)?a:b) 

const unsigned int Rio_kDisplacementMask = 0xFF000000;
const int Rio_kMAXBUF = 0xffffff;

RIO_META_SOURCE(Rio::Basket)

//////////////////////////////////////////////////////////////////////////////
Rio::Basket::Basket(
 IDirectory& aDirectory
,Branch& aBranch
)
:Key(*((Rio::File*)aDirectory.file().cast(File::is())))
,fDirectory(aDirectory)
,fBranch(aBranch)
,fBufferSize(0)
,fNevBufSize(0)
,fNevBuf(0)
,fLast(0)
,fHeaderOnly(false)
,fDisplacement(0)
,fEntryOffset(0)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Basket::Basket(
 IDirectory& aDirectory
,Branch& aBranch
,const std::string& aName
,const std::string& aTitle
)
:Key(*((Rio::File*)aDirectory.file().cast(File::is())))
,fDirectory(aDirectory)
,fBranch(aBranch)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fObjectName = aName;
  fObjectTitle = aTitle;
  fObjectClassName = sClass->name();
  fBufferSize  = fBranch.basketSize();
  fNevBufSize  = fBranch.entryOffsetLength();
  fNevBuf      = 0;
  fEntryOffset = 0;  //Must be set to 0 before calling Sizeof
  fDisplacement= 0;  //Must be set to 0 before calling Sizeof
  fBuffer      = 0;  //Must be set to 0 before calling Sizeof
  fBufferRef   = new Buffer(fFile.dictionary(),aDirectory.printer(),
			    Buffer::WRITE, fBufferSize);
  fHeaderOnly  = true;
  fLast        = 0; // RDK: Must initialize before calling Streamer()
  stream(*fBufferRef);
  fKeyLength   = fBufferRef->length();
  fObjectSize  = fBufferSize - fKeyLength;
  fLast        = fKeyLength;
  fBuffer      = 0;
  fHeaderOnly  = false;
  if (fNevBufSize) fEntryOffset = new int[fNevBufSize];
  fBranch.tree().incrementTotalBuffers(fBufferSize);
}
//////////////////////////////////////////////////////////////////////////////
Rio::Basket::~Basket(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fDisplacement) delete [] fDisplacement;
  if (fEntryOffset)  delete [] fEntryOffset;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::setReadMode(
)
//////////////////////////////////////////////////////////////////////////////
// Set read mode of basket.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fLast = fBufferRef->length();
  fBufferRef->setReadMode();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::setWriteMode(
)
//////////////////////////////////////////////////////////////////////////////
// Set write mode of basket.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBufferRef->setWriteMode();
  fBufferRef->setBufferOffset(fLast);
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Basket::nevBuf(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fNevBuf;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::setNevBufSize(
 int aNumber
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fNevBufSize = aNumber;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Basket::nevBufSize(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fNevBufSize;
}
//////////////////////////////////////////////////////////////////////////////
int* Rio::Basket::entryOffset(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fEntryOffset;
}
//////////////////////////////////////////////////////////////////////////////
int* Rio::Basket::displacement(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDisplacement;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void* Rio::Basket::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (Basket*)this;
  else return Object::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    Key::stream(aBuffer); //this must be first
    short v = aBuffer.readVersion();
    aBuffer.read(fBufferSize);
    aBuffer.read(fNevBufSize);
    aBuffer.read(fNevBuf);
    aBuffer.read(fLast);
    char flag;
    aBuffer.read(flag);
    if (fLast > fBufferSize) fBufferSize = fLast;
    if (!flag) return;
    if (flag%10 != 2) {
      delete [] fEntryOffset;
      fEntryOffset = new int[fNevBufSize];
      if (fNevBuf) aBuffer.readArray(fEntryOffset);
      if (20<flag && flag<40) {
	for(int i=0; i<fNevBuf; i++){
	  fEntryOffset[i] &= ~Rio_kDisplacementMask;
	}
      }
      if (flag>40) {
	fDisplacement = new int[fNevBufSize];
	aBuffer.readArray(fDisplacement);
      }
    }
    if (flag == 1 || flag > 10) {
      fBufferRef = new Buffer(fFile.dictionary(),fFile.printer(),
			      Buffer::READ,fBufferSize);
      char *buf  = fBufferRef->buffer();
      if (v > 1) aBuffer.readFastArray(buf,fLast);
      else       aBuffer.readArray(buf);
      fBufferRef->setBufferOffset(fLast);
      fBranch.tree().incrementTotalBuffers(fBufferSize);
    }
  } else {
    Key::stream(aBuffer);   //this must be first
    aBuffer.writeVersion(sClass->version());
    int curLast = fBufferRef->length();
    if (fBufferRef && !fHeaderOnly && !fSeekKey && curLast > fLast) 
      fLast = curLast;
    if (fLast > fBufferSize) fBufferSize = fLast;
    
    aBuffer.write(fBufferSize);
    aBuffer.write(fNevBufSize);
    aBuffer.write(fNevBuf);
    aBuffer.write(fLast);
    char flag = 1;
    if (!fEntryOffset)  flag  = 2;
    if (fBufferRef)     flag += 10;
    if (fDisplacement)  flag += 40;
    if (fHeaderOnly)    flag  = 0;
    aBuffer.write(flag);
    if (fHeaderOnly) return;
    if (fEntryOffset && fNevBuf) {
      aBuffer.writeArray(fEntryOffset, fNevBuf);
      if (fDisplacement) aBuffer.writeArray(fDisplacement, fNevBuf);
    }
    if (fBufferRef) {
      char* buf = fBufferRef->buffer();
      aBuffer.writeFastArray(buf, fLast);
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Basket::update(
 int offset
,int skipped
)
//////////////////////////////////////////////////////////////////////////////
// Update basket header and EntryOffset table.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (fEntryOffset) {
    if (fNevBuf+1 >= fNevBufSize) {
      int newsize = MAXIMUM(10,2*fNevBufSize);
      fEntryOffset  = (int*)Rio::realloc(fEntryOffset, 
					 newsize * sizeof(int),
					 fNevBufSize * sizeof(int));
      if (fDisplacement) {
	fDisplacement = (int*)Rio::realloc(fDisplacement, 
					   newsize * sizeof(int),
					   fNevBufSize * sizeof(int));
      }
      fNevBufSize   = newsize;
      // Update branch only for the first 10 baskets
      if (fBranch.writeBasket() < 10) 
	fBranch.setEntryOffsetLength(newsize);
      
    }
    fEntryOffset[fNevBuf] = offset;
    
    if (skipped!=offset && !fDisplacement){
      fDisplacement = new int[fNevBufSize];
      for (int i = 0; i<fNevBufSize; i++) fDisplacement[i] = fEntryOffset[i];
    }
    if (fDisplacement) {
      fDisplacement[fNevBuf] = skipped;
      fBufferRef->setDisplacement(skipped);
    }
  }
  
  fNevBuf++;
}
#include <string.h> //memcpy
//////////////////////////////////////////////////////////////////////////////
int Rio::Basket::writeBuffer(
)
//////////////////////////////////////////////////////////////////////////////
// Write buffer of this basket on the current file
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fFile.isWritable()) return 0;
  // Transfer fEntryOffset table at the end of fBuffer. Offsets to fBuffer
  // are transformed in entry length to optimize compression algorithm.
  fLast = fBufferRef->length();
  if (fEntryOffset) {
    fBufferRef->writeArray(fEntryOffset,fNevBuf+1);
    delete [] fEntryOffset; 
    fEntryOffset = 0;
    if (fDisplacement) {
      fBufferRef->writeArray(fDisplacement,fNevBuf+1);
      delete [] fDisplacement; 
      fDisplacement = 0;
    }
  }
  
  int lbuf = fBufferRef->length();
  fObjectSize = lbuf - fKeyLength;

  int nout = 0;
  fHeaderOnly = true;
  fCycle = fBranch.writeBasket();
  int cxlevel = fBranch.compressionLevel();
  if (cxlevel > 0) {
    if (cxlevel == 2) cxlevel--;
    int nbuffers = fObjectSize/Rio_kMAXBUF;
    //add 28 bytes in case object is placed in a deleted gap
    int buflen = fKeyLength + fObjectSize + 28; 
    fBuffer = new char[buflen];
    char* objbuf = fBufferRef->buffer() + fKeyLength;
    char* bufcur = &fBuffer[fKeyLength];
    int noutot = 0;
    int nzip   = 0;
    for (int i=0;i<=nbuffers;i++) {
      int bufmax;
      if (i == nbuffers) bufmax = fObjectSize -nzip;
      else               bufmax = Rio_kMAXBUF;
      Rio_zip(cxlevel, &bufmax, objbuf, &bufmax, bufcur, &nout);
      if (nout == 0) { //this happens when the buffer cannot be compressed
	fBuffer = fBufferRef->buffer();
	initialize(fObjectSize);
	fSeekParentDirectory = fDirectory.seekDirectory();
	fBufferRef->setBufferOffset(0);
	stream(*fBufferRef);         //write key itself again
	Key::writeFile(0);
	fHeaderOnly = false;
	return fKeyLength + nout;
      }
      bufcur += nout;
      noutot += nout;
      objbuf += Rio_kMAXBUF;
      nzip   += Rio_kMAXBUF;
    }
    nout = noutot;
    initialize(noutot);
    fSeekParentDirectory = fDirectory.seekDirectory();
    fBufferRef->setBufferOffset(0);
    stream(*fBufferRef);         //write key itself again
    ::memcpy(fBuffer,fBufferRef->buffer(),fKeyLength);
    delete fBufferRef; 
    fBufferRef = 0;
  } else {
    fBuffer = fBufferRef->buffer();
    initialize(fObjectSize);
    fSeekParentDirectory = fDirectory.seekDirectory();
    fBufferRef->setBufferOffset(0);
    stream(*fBufferRef);         //write key itself again
    nout = fObjectSize;
  }
  
  //  TKey::WriteFile calls FillBuffer. TBasket inherits from TKey, hence
  //  TBasket::FillBuffer is called.
  Key::writeFile(0);
  fHeaderOnly = false;
  
  return fKeyLength + nout;

}
//////////////////////////////////////////////////////////////////////////////
int Rio::Basket::readBasketBytes(
 Seek aPosition
)
//////////////////////////////////////////////////////////////////////////////
// Read basket buffers in memory and cleanup :
//       Read first bytes of a logical record starting at position pos
//       return record length (first 4 bytes of record)
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  const int len = 128;
  char buffer[len];
  int keylen;
  fFile.getRecordHeader(buffer, aPosition,len, 
			fNumberOfBytes, fObjectSize, keylen);
  fKeyLength = keylen;
  return fNumberOfBytes;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Basket::readBasketBuffers(
 Seek aPosition
,int aLength
)
//////////////////////////////////////////////////////////////////////////////
// Read basket buffers in memory and cleanup :
//  Read a basket buffer. Check if buffers of previous ReadBasket
//  should not be dropped. Remember, we keep buffers
//  in memory up to fMaxVirtualSize.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int badread = 0;

  //FIXME if (gBranch->GetTree()->MemoryFull(fBufferSize)) 
  //FIXME   gBranch->DropBaskets();
  
  fBufferRef = new Buffer(fFile.dictionary(),fFile.printer(),
			  Buffer::READ,aLength);
  char* buffer = fBufferRef->buffer();
  fFile.setPosition(aPosition);
  fFile.readBuffer(buffer,aLength);
  stream(*fBufferRef);
  if (fObjectSize > fNumberOfBytes-fKeyLength) {
    fBuffer = new char[fObjectSize+fKeyLength];
    ::memcpy(fBuffer,buffer,fKeyLength);
    unsigned char *objbuf = (unsigned char*)(fBuffer + fKeyLength);
    int nin = fNumberOfBytes-fKeyLength;
    int nout;
    Rio_unzip(&nin, (unsigned char*)&buffer[fKeyLength], &fObjectSize, objbuf, &nout);
    if (nout != fObjectSize) {
      Out out(fBranch.printer());
      out << "Rio::Basket::readBasketBuffers : fObjlen = " 
	  << fObjectSize << ", nout = " << nout << endl;
      badread = 1;
    }
    delete [] buffer;
    fBufferRef->setBuffer(fBuffer, fObjectSize+fKeyLength );
  } else {
    fBuffer = fBufferRef->buffer();
  }

  fBranch.tree().incrementTotalBuffers(fBufferSize);
  
  // read offsets table
  if (!fBranch.entryOffsetLength()) return badread;
  delete [] fEntryOffset;
  fEntryOffset = 0;
  fBufferRef->setBufferOffset(fLast);
  fBufferRef->readArray(fEntryOffset);
  delete [] fDisplacement;
  fDisplacement = 0;
  if (fBufferRef->length() != fBufferRef->bufferSize()) {
    // There is more data in the buffer!  It is the diplacement
    // array.
    fBufferRef->readArray(fDisplacement);
  }

  return badread;
}
