//
//  A ROOT file is a suite of consecutive data records with the following
//    format (see also the TKey class);
// TKey ---------------------
//      byte 1->4  Nbytes    = Length of compressed object (in bytes)
//           5->6  Version   = TKey version identifier
//           7->10 ObjLen    = Length of uncompressed object
//          11->14 Datime    = Date and time when object was written to file
//          15->16 KeyLen    = Length of the key structure (in bytes)
//          17->18 Cycle     = Cycle of key
//          19->22 SeekKey   = Pointer to record itself (consistency check)
//          23->26 SeekPdir  = Pointer to directory header
//          27->27 lname     = Number of bytes in the class name
//          28->.. ClassName = Object Class Name
//          ..->.. lname     = Number of bytes in the object name
//          ..->.. Name      = lName bytes with the name of the object
//          ..->.. lTitle    = Number of bytes in the object title
//          ..->.. Title     = Title of the object
//          -----> DATA      = Data bytes associated to the object
//
//  The first data record starts at byte fBEGIN (currently set to kBegin)
//  Bytes 1->kBegin contain the file description:
//       byte  1->4  "root"      = Root file identifier
//             5->8  fVersion    = File format version
//             9->12 fBEGIN      = Pointer to first data record
//            13->16 fEND        = Pointer to first free word at the EOF
//            17->20 fSeekFree   = Pointer to FREE data record
//            21->24 fNbytesFree = Number of bytes in FREE data record
//            25->28 nfree       = Number of free data records
//            29->32 fNbytesName = Number of bytes in TNamed at creation time
//            33->33 fUnits      = Number of bytes for file pointers
//            34->37 fCompress   = Zip compression level
//

// this :
#include <Rio/Core/File.h>

#include <Rio/Interfaces/IDictionary.h>

#include <Rio/Core/Key.h>
#include <Rio/Core/tobuf.h>
#include <Rio/Core/Directory.h>
#include <Rio/Core/FreeSegment.h>
#include <Rio/Core/Buffer.h>
#include <Rio/Core/Platform.h>
#include <Rio/Core/smanip.h>
#include <Rio/Core/List.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Out.h>

#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

// Various mess :
#ifdef WIN32
#include <direct.h>
#define ssize_t int
#include <io.h>
#include <sys/types.h>
#else
#include <unistd.h>
#endif
#if defined(__alpha) && !defined(linux) // OSF1 & cxx.
#define EINTR		4		/* Interrupted system call */
#endif

static const int kBegin = 64;
static const char kUnits = 4;
enum EAccessMode {
  kFileExists        = 0,
  kExecutePermission = 1,
  kWritePermission   = 2,
  kReadPermission    = 4
};

static bool Rio_fileStatus(const std::string&,long&,long&,long&,long&);
static bool Rio_accessPathName(const std::string&,EAccessMode);
static bool Rio_unlink(const std::string&);

#define NOT_OPEN (-1)

RIO_META_SOURCE(Rio::File)
//////////////////////////////////////////////////////////////////////////////
void* Rio::File::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (File*)this;
  else return 0;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
Rio::File::File(
 const std::string& aPath
,Mode aMode
,IDictionary& aDictionary
,IPrinter& aPrinter
)
:fName(aPath)
,fTitle("")
,fMode(aMode)
,fDictionary(aDictionary)
,fPrinter(aPrinter)
,fRootDirectory(0)
,fVerboseLevel(0)
,fWritable(false)
,fFileDescriptor(NOT_OPEN)
,fBytesWrite(0)
,fBytesRead(0)
,fWritten(0)
,fSumBuffer(0)
,fSum2Buffer(0)
// Begin of record :
,fVersion(0)
,fBEGIN(0)
,fEND(0)
,fSeekFree(0)
,fNbytesFree(0)
,fNbytesName(0)
,fUnits(kUnits)
,fCompress(1)
,fSeekInfo(0)
,fNbytesInfo(0)
//////////////////////////////////////////////////////////////////////////////
//  If Mode   = CREATE          create a new file and open it for writing,
//                              if the file already exists the file is
//                              not opened.
//            = RECREATE        create a new file, if the file already
//                              exists it will be overwritten.
//            = UPDATE          open an existing file for writing.
//                              if no file exists, it is created.
//            = READ            open an existing file for reading.
//  If the constructor failed in any way IsZombie() will return true.
//  Use IsOpen() to check if the file is (still) open.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
  fVersion = version();
  // Root directory (should have the same name, title than the file) :

  // Extract file name :
  std::string directory;
  std::string name;
  std::string suffix;
  smanip smanip;
  smanip.pathNameSuffix(fName,directory,name,suffix);

  fRootDirectory = new Directory(*this,0,name,fTitle);

  bool create = (aMode==CREATE ? true : false);
  bool recreate = (aMode==RECREATE ? true : false);
  bool update = (aMode==UPDATE ? true : false);
  bool read = (aMode==READ ? true : false);

  if (!create && !recreate && !update && !read) {
    read = true;
  }

  if(recreate) {
    if(Rio_accessPathName(fName,kFileExists)) Rio_unlink(fName);
    recreate = false;
    create = true;
  }
   
  if(create && Rio_accessPathName(fName, kFileExists)) {
    Out out(fPrinter);
    out << "Rio::File::File : file \"" << fName << "\" already exists." << endl;
    return;
  }

  if(update) {
    if(!Rio_accessPathName(fName, kFileExists)) {
      update = false;
      create = true;
    }
    if(update && !Rio_accessPathName(fName,kWritePermission)) {
      Out out(fPrinter);
      out << "Rio::File::File : no write permission, could not open file \""
	  << fName << "\"." << endl;
      return;
    }
  }
  if(read) {
    if(!Rio_accessPathName(fName,kFileExists)) {
      Out out(fPrinter);
      out << "Rio::File::File : file \"" << fName << "\" does not exist." << endl;
      return;
    }
    if(!Rio_accessPathName(fName,kReadPermission)) {
      Out out(fPrinter);
      out << "Rio::File::File : no read permission, could not open file \"" << fName << "\"." << endl;
      return;
    }
  }


  if(create || update) {
#ifdef WIN32
    fFileDescriptor = ::open(fName.c_str(),
			     O_RDWR | O_CREAT | O_BINARY,
			     S_IREAD | S_IWRITE);
#else
    fFileDescriptor = ::open(fName.c_str(), 
			     O_RDWR | O_CREAT, 
			     0644);
#endif
    if(fFileDescriptor == NOT_OPEN) {
      Out out(fPrinter);
      out << "Rio::File::File : file \"" << fName 
	  << "\" can not be opened." << endl;
      return;
    }
    fWritable = true;
  } else {
#ifdef WIN32
    fFileDescriptor = ::open(fName.c_str(),
			     O_RDONLY | O_BINARY, 
			     S_IREAD | S_IWRITE);
#else
    fFileDescriptor = ::open(fName.c_str(),
			     O_RDONLY, 
			     0644);
#endif
    if(fFileDescriptor == NOT_OPEN) {
      Out out(fPrinter);
      out << "Rio::File::File : file \"" << fName
	  << "\" can not be opened for reading." << endl;
      return;
    }
    fWritable = false;
  }

  if(!(create ? initializeInCreateMode():initializeInUpdateMode())) {
    ::close(fFileDescriptor);
    fFileDescriptor = NOT_OPEN;
    return;
  }

  // Create StreamerInfo index
  /*FIXME
  int lenIndex = gROOT->GetListOfStreamerInfo()->GetSize()+1;
  if (lenIndex < 5000) lenIndex = 5000;
  fClassIndex = new TArrayC(lenIndex);
  if (fSeekFree > fBEGIN) readStreamerInfo();
  */

}
//////////////////////////////////////////////////////////////////////////////
Rio::File::File(
 const File& aFile
)
:fDictionary(aFile.fDictionary)
,fPrinter(aFile.fPrinter)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::operator=(
 const File&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::File::~File(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  close();
  delete fRootDirectory;
  if(verboseLevel()) {
    Out out(fPrinter);
    out << "Rio::File::~File : called for \"" << fName << "\"." << endl;
  }
  Debug::decrement();
}
//////////////////////////////////////////////////////////////////////////////
Rio::IPrinter& Rio::File::printer(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fPrinter;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDictionary& Rio::File::dictionary(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fDictionary;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IDirectory& Rio::File::directory(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *fRootDirectory;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::isOpen(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFileDescriptor == NOT_OPEN ? false : true;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::verboseLevel(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fVerboseLevel;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::setVerboseLevel(
 int aVerboseLevel
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fVerboseLevel = aVerboseLevel;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::File::name(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fName;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::isWritable(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fWritable; 
}
//////////////////////////////////////////////////////////////////////////////
Rio::Seek Rio::File::end(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fEND; 
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::setEnd(
 Seek aLast
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fEND = aLast; 
}
//////////////////////////////////////////////////////////////////////////////
std::list<Rio::FreeSegment*>& Rio::File::freeSegments(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fFreeSegments;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::compressionLevel(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fCompress;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::setCompressionLevel(
 int aLevel
)
//////////////////////////////////////////////////////////////////////////////
// Set level of compression for this file
// ======================================
//
//  level = 0 objects written to this file will not be compressed.
//  level = 1 minimal compression level but fast.
//  ....
//  level = 9 maximal compression level but slow.
//
//  Note that the compression level may be changed at any time.
//  The new compression level will only apply to newly written objects.
//  The function TFile::Map shows the compression factor
//  for each object written to this file.
//  The function TFile::GetCompressionFactor returns the global
//  compression factor for this file.
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
   if (aLevel < 0) aLevel = 0;
   if (aLevel > 9) aLevel = 9;
   fCompress = aLevel;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::initializeInCreateMode(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int max_file_size = 2000000000;  // should rather check disk quota

  fBEGIN = kBegin;  // First used word in file following the file header.
  fEND = fBEGIN;    // Pointer to end of file.

  fFreeSegments.push_back(new FreeSegment(fBEGIN,max_file_size));

  // Write Directory info :
  int namelen = 
    Rio_std_string_recordSize(fName) + 
    Rio_std_string_recordSize(fTitle);
  int nbytes = namelen + fRootDirectory->recordSize();
  Key key(*this,fRootDirectory->seekDirectory(),
	  fName,fTitle,"TFile",nbytes);
  // fNbytesName = start point of directory info from key head.
  fNbytesName = key.keyLength() + namelen; 
  fRootDirectory->setNbytesName(fNbytesName);
  fRootDirectory->setSeekDirectory(key.seekKey());
  fSeekFree = 0;
  fNbytesFree = 0;

  if(!writeHeader()) {
    Out out(fPrinter);
    out << "Rio::File::initialize : can't write file header." << endl;
    return false;
  }

  char* buffer = key.buffer();
  Rio_tobuf(buffer,fName);
  Rio_tobuf(buffer,fTitle);
  fRootDirectory->fillBuffer(buffer);

  if(verboseLevel()) {
    Out out(fPrinter);
    out << "Rio::File::initialize : write key (" << namelen 
	<< ", " << fRootDirectory->recordSize() << ", " 
	<< nbytes << ", " << fNbytesName << ", " << key.seekKey() 
	<< "):" << endl;
  }

  key.writeFile();

  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::initializeInUpdateMode(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!readHeader()) return false;
  fRootDirectory->setSeekDirectory(fBEGIN);
  // Read Free segments structure if file is writable :
  if (fWritable) {
    if (fSeekFree > fBEGIN) {
      readFreeSegments();
    } else {
      Out out(fPrinter);
      out << "Rio::File::initialize : file \"" << fName 
	  << "\" probably not closed, cannot read free segments" << endl;
    }
  }
  // Read Directory info :
  int nbytes = fNbytesName + fRootDirectory->recordSize();
  char* header = new char[nbytes];
  char* buffer = header;
  setPosition(fBEGIN);
  readBuffer(buffer,nbytes);
  buffer = header+fNbytesName;
  fRootDirectory->readBuffer(buffer);
  int nk = 
    sizeof(int) +
    sizeof(short) +
    2*sizeof(int) + 
    2*sizeof(short) +
    2*sizeof(Seek);
  buffer = header+nk;
  std::string cname;
  Rio_frombuf(buffer,cname); // Should be "TFile".
  Rio_frombuf(buffer,cname);
  Rio_frombuf(buffer,fTitle);
  delete [] header;
  int dirNbytesName = fRootDirectory->nBytesName();
  if (dirNbytesName < 10 || dirNbytesName > 1000) {
    Out out(fPrinter);
    out << "Rio::File::initalize : Cannot read directory info" << endl;
    return false;
  }
  // Read keys of the top directory :
  if (fRootDirectory->seekKeys() > fBEGIN) {
    fRootDirectory->readKeys();
  } else {
    Out out(fPrinter);
    out << "Rio::File::initialize : file \"" << fName 
	<< "\" probably not closed, trying to recover" << endl;
    recover();
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::close(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!isOpen()) return;

  if (isWritable()) writeStreamerInfo();
  /*FIXME
  delete fClassIndex;
  fClassIndex = 0;
  */
  
  //FIXME : TCollection::StartGarbageCollection();

  fRootDirectory->close();

  if (isWritable()) {
    if(!fFreeSegments.empty()) {
      FreeSegment* f1 = *(fFreeSegments.begin());
      if(f1) {
	if(verboseLevel()) {
	  Out out(fPrinter);
	  out << "Rio::File::close : writeFreeSegments :" << endl;
	}

	writeFreeSegments();       // Write free segments linked list

	if(!writeHeader())  { // Now write file header
	  Out out(fPrinter);
	  out << "Rio::File::close : can't write file header." << endl;
	}
      }
    }
    //FIXME if (fCache) fCache->Flush();
  }

  // Delete free segments from free list.
  std::list<FreeSegment*>::iterator it;
  for(it=fFreeSegments.begin();
      it!=fFreeSegments.end();
      it = fFreeSegments.erase(it)) 
    delete (*it);

  ::close(fFileDescriptor);
  fFileDescriptor = NOT_OPEN;
  
  fWritable = false;

  //FIXME TCollection::EmptyGarbageCollection();
}
#include <string.h> //memcpy
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::writeHeader(
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  FreeSegment* lastfree = (!fFreeSegments.empty() ? fFreeSegments.back() : 0);
  if (lastfree) fEND  = lastfree->first();
  const char* root = "root";
  char psave[kBegin];
  char* buffer = psave;
  ::memcpy(buffer, root, 4); buffer += 4;
  Rio_tobuf(buffer, fVersion);
  Rio_tobuf(buffer, fBEGIN);
  Rio_tobuf(buffer, fEND);
  Rio_tobuf(buffer, fSeekFree);
  Rio_tobuf(buffer, fNbytesFree);
  int nfree  = fFreeSegments.size();
  Rio_tobuf(buffer, nfree);
  Rio_tobuf(buffer, fNbytesName);
  Rio_tobuf(buffer, fUnits);
  Rio_tobuf(buffer, fCompress);
  Rio_tobuf(buffer, fSeekInfo);
  Rio_tobuf(buffer, fNbytesInfo);
  int nbytes  = buffer - psave;
  if(!setPosition()) return false;
  if(!writeBuffer(psave, nbytes)) {
    return false;
  }
  if(!synchronize()) return false;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::readHeader(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  char header[kBegin];
  setPosition();
  readBuffer(header,kBegin);
  // make sure this is a root file
  if (strncmp(header, "root", 4)) {
    Out out(fPrinter);
    out << "Rio::File::readHeader : \"" << fName 
	<< "\" not a ROOT file" << endl;
    return false;
  }
  char *buffer = header + 4;    // skip the "root" file identifier
  Rio_frombuf(buffer, &fVersion);
  Rio_frombuf(buffer, &fBEGIN);
  Rio_frombuf(buffer, &fEND);
  Rio_frombuf(buffer, &fSeekFree);
  Rio_frombuf(buffer, &fNbytesFree);
  int nfree = 0;
  Rio_frombuf(buffer, &nfree);
  Rio_frombuf(buffer, &fNbytesName);
  Rio_frombuf(buffer, &fUnits );
  Rio_frombuf(buffer, &fCompress);
  Rio_frombuf(buffer, &fSeekInfo);
  Rio_frombuf(buffer, &fNbytesInfo);
  return true;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::version(
)
//////////////////////////////////////////////////////////////////////////////
// Return version id as an integer, i.e. "2.22/04" -> 22204.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
//#define ROOT_RELEASE "3.00/06"   
#define ROOT_MAJOR_VERSION 3
#define ROOT_MINOR_VERSION 0
#define ROOT_PATCH_VERSION 6

  return 
    10000 * ROOT_MAJOR_VERSION + 
    100 * ROOT_MINOR_VERSION + 
    ROOT_PATCH_VERSION;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::setPosition(
 Seek aOffset
,From aFrom
)
//////////////////////////////////////////////////////////////////////////////
// Seek to a specific position in the file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int whence = 0;
  switch (aFrom) {
  case BEGIN:
    whence = SEEK_SET;
    break;
  case CURRENT:
    whence = SEEK_CUR;
    break;
  case END:
    whence = SEEK_END;
    break;
  }
  if (::lseek(fFileDescriptor, aOffset, whence) < 0) {
    Out out(fPrinter);
    out << "Rio::File::setPosition : cannot set position " 
	<< aOffset << " in file \"" << fName << "\"." << endl;
    return false;
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::writeBuffer(
 const char* aBuffer
,int aLength
)
//////////////////////////////////////////////////////////////////////////////
// Write a buffer to the file. This is the basic low level write operation.
// Returns false in case of failure.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!isOpen() || !fWritable) return false;
  ssize_t siz;
  //FIXME : gSystem->IgnoreInterrupt();
  while ((siz = ::write(fFileDescriptor,aBuffer,aLength)) < 0 && 
	 errorNumber() == EINTR)
    resetErrorNumber();
  //FIXME : gSystem->IgnoreInterrupt(kFALSE);
  if (siz < 0) {
    Out out(fPrinter);
    out << "Rio::File::writeBuffer : error writing to file \"" 
	<< fName << "\"." << endl;
    return false;
  }
  if (siz != aLength) {
    Out out(fPrinter);
    out << "Rio::File::writeBuffer : " 
	<< "error writing all requested bytes to file \"" << fName 
	<< "\", wrote " << siz << " of " << aLength << endl;
    return false;
  }
  fBytesWrite  += siz;
  return true;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::readBuffer(
 char* aBuffer
,int aLength
)
//////////////////////////////////////////////////////////////////////////////
// Read a buffer from the file. This is the basic low level read operation.
// Returns kTRUE in case of failure.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(isOpen()) {
    ssize_t siz;
    while ((siz = ::read(fFileDescriptor, aBuffer, aLength)) < 0 && 
	   errorNumber() == EINTR)
      resetErrorNumber();
    if (siz < 0) {
      Out out(fPrinter);
      out << "Rio::File::readBuffer : error reading from file \"" 
	  << fName << "\"." << endl;
      return false;
    }
    if (siz != aLength) {
      Out out(fPrinter);
      out << "Rio::File::readBuffer : " 
	  << "error reading all requested bytes from file " 
	  << fName << ", got " << siz << " of " << aLength << endl;
      return false;
    }
    fBytesRead  += siz;
    return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::getRecordHeader(
 char* aBuffer
,Seek aFirst
,int aMaxbytes
,int& aNbytes
,int& aObjectSize
,int& aKeyLength
)
//////////////////////////////////////////////////////////////////////////////
// Read the logical record header starting at position first :
// maxbytes bytes are read into buf
// the function reads nread bytes where nread is the minimum of maxbytes
// and the number of bytes before the end of file.
// the function returns nread.
// In output arguments:
//    nbytes : number of bytes in record
//             if negative, this is a deleted record
//             if 0, cannot read record, wrong value of argument first
//    objlen : uncompressed object size
//    keylen : length of logical record header
// Note that the arguments objlen and keylen are returned only if maxbytes >=16
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Out out(fPrinter);
  if (aFirst < fBEGIN) {
    out << "Rio::File::getRecordHeader : error : " 
	<< aFirst << " < fBEGIN." << endl;
    return 0;
  }
  if (aFirst > fEND) {
    out << "Rio::File::getRecordHeader : error : " 
	<< aFirst << " > fEND." << endl;
    return 0;
  }
  setPosition(aFirst);
  int nread = aMaxbytes;
  if (aFirst+aMaxbytes > fEND) nread = fEND-aMaxbytes;
  if (nread < 4) {
    out << "Rio::File::getRecordHeader : parameter maxbytes=" 
	<< nread << " must be >= 4" << endl;
    return nread;
  }
  readBuffer(aBuffer,nread);
  short versionkey;
  short klen;
  unsigned int datime;
  int nb,olen;
  char* buffer = aBuffer;
  Rio_frombuf(buffer,&nb);
  aNbytes = nb;
  if (nb < 0) return nread;
//   const Int_t headerSize = Int_t(sizeof(nb) +sizeof(versionkey) +sizeof(olen) +sizeof(datime) +sizeof(klen));
  const int headerSize = 16;
  if (nread < headerSize) return nread;
  Rio_frombuf(buffer, &versionkey);
  Rio_frombuf(buffer, &olen);
  Rio_frombuf(buffer, &datime);
  Rio_frombuf(buffer, &klen);
  if (!olen) olen = aNbytes-klen;
  aObjectSize = olen;
  aKeyLength = klen;
  return nread;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::errorNumber(
)
//////////////////////////////////////////////////////////////////////////////
// Return system error number.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_SOLARIS_CC50
  return ::errno;
#else
  return errno;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::resetErrorNumber(
)
//////////////////////////////////////////////////////////////////////////////
// Reset system error number.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef RIO_SOLARIS_CC50
   ::errno = 0;
#else
   errno = 0;
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::File::synchronize(
)
//////////////////////////////////////////////////////////////////////////////
// Synchornize a file's in-core and on-disk states.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!isOpen() || !fWritable) return false;
#ifdef WIN32
  return true;
#else
  if (::fsync(fFileDescriptor) < 0) {
    Out out(fPrinter);
    out << "Rio::File::synchronize : error flushing file \"" 
	<< fName << "\"." << endl;
    return false;
  } 
  return true;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::writeFreeSegments(
)
//////////////////////////////////////////////////////////////////////////////
//  The linked list of FREE segments (fFree) is written as a single data
//  record
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Delete old record if it exists :
  if (fSeekFree != 0){
    makeFreeSegment(fSeekFree, fSeekFree + fNbytesFree -1);
  }

  int nbytes = 0;
 {std::list<FreeSegment*>::iterator it;
  for(it=fFreeSegments.begin();it!=fFreeSegments.end();++it) {
    nbytes += (*it)->recordSize();
  }}
  if (!nbytes) return;

  Key key(*this,fRootDirectory->seekDirectory(),fName,fTitle,"TFile",nbytes);
  if (key.seekKey() == 0) {
    return;
  }
  char* buffer = key.buffer();
  
 {std::list<FreeSegment*>::iterator it;
  for(it=fFreeSegments.begin();it!=fFreeSegments.end();++it) {
    (*it)->fillBuffer(buffer);
  }}

  fNbytesFree = key.numberOfBytes();
  fSeekFree   = key.seekKey();
  if(verboseLevel()) {
    Out out(fPrinter);
    out << "Rio::File::writeFreeSegments : write key :" << endl;
  }

  key.writeFile();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::readFreeSegments(
)
//////////////////////////////////////////////////////////////////////////////
// Read the FREE list.
//  Every file has a linked list (fFree) of free segments
//  This linked list has been written on the file via WriteFree
//  as a single data record
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Key headerfree(*this,fSeekFree,fNbytesFree);
  headerfree.readFile();
  char* buffer = headerfree.buffer();
  headerfree.readBuffer(buffer);
  buffer = headerfree.buffer();
  while(1) {
    FreeSegment* afree = new FreeSegment();
    afree->readBuffer(buffer);
    fFreeSegments.push_back(afree);
    if (afree->last() > fEND) break;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::makeFreeSegment(
 Seek aFirst
,Seek aLast
)
//////////////////////////////////////////////////////////////////////////////
// Mark unused bytes on the file :
//  The list of free segments is in the fFree list
//  When an object is deleted from the file, the freed space is added
//  into the FREE linked list (fFree). The FREE list consists of a chain
//  of consecutive free segments on the file. At the same time, the first
//  4 bytes of the freed record on the file are overwritten by GAPSIZE
//  where GAPSIZE = -(Number of bytes occupied by the record).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(fFreeSegments.empty()) return;
  FreeSegment* newfree = new FreeSegment(aFirst,aLast);
  if(!newfree) return;
  fFreeSegments.insert(fFreeSegments.begin(),newfree);
  Seek nfirst = newfree->first();
  Seek nlast = newfree->last();
  int nbytes = (int)(nfirst - nlast -1);
  int nb = sizeof(int);
  char* buffer = new char[nb];
  char* psave = buffer;
  Rio_tobuf(buffer,nbytes);
  if (nlast == fEND-1) fEND = nfirst;
  setPosition(nfirst);
  writeBuffer(psave, nb);
  synchronize();
  delete [] psave;
}
//////////////////////////////////////////////////////////////////////////////
Rio::FreeSegment* Rio::File::bestFreeSegment(
 int aNumberOfBytes
)
//////////////////////////////////////////////////////////////////////////////
// Return the best free segment where to store nbytes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  FreeSegment* idcur1 = 0;
  std::list<FreeSegment*>::iterator it;
  for(it=fFreeSegments.begin();it!=fFreeSegments.end();++it) {
    FreeSegment* idcur = *it;
    int nleft = int(idcur->last() - idcur->first() +1);
    if (nleft == aNumberOfBytes) return idcur;  // Found an exact match.
    if(nleft > aNumberOfBytes+3) 
      if (idcur1 == 0) 
	idcur1 = idcur;
  }
  return idcur1; // Return first segment >nbytes.
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::recover(
)
//////////////////////////////////////////////////////////////////////////////
// Attempt to recover file if not correctly closed.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  long id, size, flags, modtime;
  if (!Rio_fileStatus(fName,id,size,flags,modtime)) {
    Out out(fPrinter);
    out << "Rio::File::recover : cannot stat the file \"" 
	<< fName << "\"." << endl;
    return;
  }

  fEND = Seek(size);

  char header[kBegin];

  int nrecov = 0;
  int nwheader = 64;
  int nread = nwheader;
  Seek idcur = fBEGIN;
  while (idcur < fEND) {
    setPosition(idcur);
    if (idcur+nread >= fEND) nread = fEND-idcur-1;
    readBuffer(header, nread);
    char* buffer = header;
    char* bufread = header;
    int nbytes;
    Rio_frombuf(buffer, &nbytes);
    if (!nbytes) {
      Out out(fPrinter);
      out << "Rio::File::recover : Address = " << idcur << tab 
	  << "Nbytes = " << nbytes << tab << "=====E R R O R=======" << endl;
      break;
    }
    if (nbytes < 0) {
      idcur -= nbytes;
      if (fWritable) {
	fFreeSegments.push_back(new FreeSegment(idcur,idcur-nbytes-1));
      }
      setPosition(idcur);
      continue;
    }
    short versionkey;
    int objlen;
    unsigned int datime;
    short keylen,cycle;
    Seek seekkey,seekpdir;
    char nwhc;
    Rio_frombuf(buffer, &versionkey);
    Rio_frombuf(buffer, &objlen);
    Rio_frombuf(buffer, &datime);
    Rio_frombuf(buffer, &keylen);
    Rio_frombuf(buffer, &cycle);
    Rio_frombuf(buffer, &seekkey);
    Rio_frombuf(buffer, &seekpdir);
    Rio_frombuf(buffer, &nwhc);
    char *classname = new char[nwhc+1];
    int i;
    for (i = 0;i < nwhc; i++) Rio_frombuf(buffer, &classname[i]);
    classname[nwhc] = '\0';
    if (!strcmp(classname,"TBasket")) {idcur += nbytes; continue;}
    if (seekpdir != fRootDirectory->seekDirectory()) {
      idcur += nbytes; continue;
    }
    Key* key = new Key(*this);
    key->readBuffer(bufread);
    fRootDirectory->appendKey(key);
    nrecov++;
    delete [] classname;
    idcur += nbytes;
  }
  if (fWritable) {
    fFreeSegments.push_back(new FreeSegment(fEND,2000000000));
    if (nrecov) this->write();
  }
  Out out(fPrinter);
  if (nrecov) {
    out << "Rio::File::recover : successfully recovered " 
	<< nrecov << " keys." << endl;
  } else {
    out << "Fio::File::recover : no keys recovered." << endl;
  }
}
//////////////////////////////////////////////////////////////////////////////
int Rio::File::write(
)
//////////////////////////////////////////////////////////////////////////////
// Write memory objects to this file :
//  Loop on all objects in memory (including subdirectories).
//  A new key is created in the KEYS linked list for each object.
//  The list of keys is then saved on the file (via WriteKeys)
//  as a single data record.
//  The directory header info is rewritten on the directory header record.
//  The linked list of FREE segments is written.
//  The file header is written (bytes 1->fBEGIN).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Out out(fPrinter);
  if (!isWritable()) {
    out << "Rio::File::write : file not opened in write mode." << endl;
    return 0;
  }

  if(verboseLevel()) {
    out << "Rio::File::write : writing Name=\"" 
	<< fName << "\" Title=\"" << fTitle << "\"." << endl;
  }

  int nbytes = fRootDirectory->write(); // Write directory tree

  writeStreamerInfo();

  if(verboseLevel()) {
    out << "Rio::File::write : writeFreeSegments :" << endl;
  }

  writeFreeSegments(); // Write free segments.

  if(!writeHeader()) { // Now write file header
    out << "Rio::File::write : can't write file header." << endl;
  }

  return nbytes;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::writeStreamerInfo(
)
//////////////////////////////////////////////////////////////////////////////
//  Write the list of TStreamerInfo as a single object in this file
//  The class Streamer description for all classes written to this file
//  is saved.
//  see class TStreamerInfo
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fWritable) return;

  /*FIXME
  if (!fClassIndex) return;
  //no need to update the index if no new classes added to the file
  if (fClassIndex->fArray[0] == 0) return;
  if (gDebug > 0) printf("Calling WriteStreamerInfo for file: %s\n",GetName());
  */

  // build a temporary list with the marked files
  List list;

  // ROOT code : TIter next(gROOT->GetListOfStreamerInfo());
  const std::list<IObject*>& infos = fDictionary.streamerInfos();
  std::list<IObject*>::const_iterator it;
  for(it=infos.begin();it!=infos.end();++it) {
    /*FIXME
    if (info->IsA() != TStreamerInfo::Class()) {
      Warning("WriteStreamerInfo"," not a TStreamerInfo object");
      continue;
    }
    */
    //int uid = (*it)->identifier();
    int uid = 0; //FIXME
    //FIXME if (fClassIndex->fArray[uid]) list.Add(info);
    list.push_back(*it);
    if(verboseLevel()) {
      Out out(fPrinter);
      out << "Rio::File::writeStreamerInfo : -class: \""
	  << (*it)->name() << "\" info number " << uid << " saved." << endl;
    }
  }
  if (list.size() == 0) return;
  //FIXME fClassIndex->fArray[0] = 0;

  // always write with compression on
  int compress = fCompress;
  fCompress = 1;


  //free previous StreamerInfo record
  if(fSeekInfo) makeFreeSegment(fSeekInfo,fSeekInfo+fNbytesInfo-1);
  //Create new key
  //FIXME : &list will be put in the buffer map !
  Key key(*this,*fRootDirectory,list,"StreamerInfo",bestBuffer());
  fRootDirectory->removeKey(&key);
  fSeekInfo = key.seekKey();
  fNbytesInfo = key.numberOfBytes();
  sumBuffer(key.objectSize());
  key.writeFile(0);

  fCompress = compress;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::readStreamerInfo(
)
//////////////////////////////////////////////////////////////////////////////
// Read the list of StreamerInfo from this file
// The key with name holding the list of TStreamerInfo objects is read.
// The corresponding TClass objects are updated.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // FIXME : dummy for the moment...
  /*
  if (fSeekInfo > 0 && fSeekInfo < fEND) {
    Key key(fFile);
    char* buffer = new char[fNbytesInfo+1];
    char* buf = buffer;
    setPosition(fSeekInfo);
    readBuffer(buf,fNbytesInfo);
    key.readBuffer(buf);
    //FIXME list = (TList*)key->ReadObj();
    delete [] buffer;
  }
  */
}
//////////////////////////////////////////////////////////////////////////////
void Rio::File::sumBuffer(
 int aBufferSize
)
//////////////////////////////////////////////////////////////////////////////
// Increment statistics for buffer sizes of objects in this file.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fWritten++;
  fSumBuffer  += aBufferSize;
  fSum2Buffer += aBufferSize * aBufferSize;
}
#include <math.h> //fabs,sqrt
//////////////////////////////////////////////////////////////////////////////
int Rio::File::bestBuffer(
) const
//////////////////////////////////////////////////////////////////////////////
// Return the best buffer size of objects on this file.
//  The best buffer size is estimated based on the current mean value
//  and standard deviation of all objects written so far to this file.
//  Returns mean value + one standard deviation.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (!fWritten) return Buffer::INITIAL;
  double mean = fSumBuffer/fWritten;
  double rms2 = ::fabs(fSum2Buffer/fSumBuffer -mean*mean);
  return (int)(mean + ::sqrt(rms2));
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
bool Rio_fileStatus(
 const std::string& aPath
,long& aID
,long& aSize
,long& aFlags
,long& aModtime
)
//////////////////////////////////////////////////////////////////////////////
// Get info about a file: id, size, flags, modification time.
// Id      is (statbuf.st_dev << 24) + statbuf.st_ino
// Size    is the file size
// Flags   is file type: 0 is regular file, bit 0 set executable,
//                       bit 1 set directory, bit 2 set special file
//                       (socket, fifo, pipe, etc.)
// Modtime is modification time
// The function returns 0 in case of success and 1 if the file could
// not be stat'ed.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  aID = 0;
  aSize = 0;
  aFlags = 0;
  aModtime = 0;
  struct stat statbuf;
  if(::stat(aPath.c_str(),&statbuf) >= 0) {
#ifdef WIN32
    aID = (statbuf.st_dev << 24) + statbuf.st_ino;
#else
#if defined(RIO_KCC)
    aID = (statbuf.st_dev.__val[0] << 24) + statbuf.st_ino;
#else
    aID = (statbuf.st_dev << 24) + statbuf.st_ino;
#endif
#endif
    aSize = statbuf.st_size;
    aModtime = statbuf.st_mtime;
    if (statbuf.st_mode & ((S_IEXEC)|(S_IEXEC>>3)|(S_IEXEC>>6))) aFlags |= 1;
    if ((statbuf.st_mode & S_IFMT) == S_IFDIR) aFlags |= 2;
    if ((statbuf.st_mode & S_IFMT) != S_IFREG &&
	(statbuf.st_mode & S_IFMT) != S_IFDIR) aFlags |= 4;
    return true;
  }
  return false;
}
//////////////////////////////////////////////////////////////////////////////
bool Rio_accessPathName(
 const std::string& aPath
,EAccessMode aMode
)
//////////////////////////////////////////////////////////////////////////////
// Returns true if one can access a file using the specified access mode.
// Mode is the same as for the WinNT access(2) function.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
#ifdef WIN32
  return (::_access(aPath.c_str(), aMode) == 0) ? true : false;
#else
  return (::access(aPath.c_str(), aMode) == 0) ? true : false;
#endif
}
//////////////////////////////////////////////////////////////////////////////
bool Rio_unlink(
 const std::string& aPath
)
//////////////////////////////////////////////////////////////////////////////
// Unlink, i.e. remove, a file or directory. Returns true when succesfull,
// false in case of failure.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  struct stat finfo;
  if (::stat(aPath.c_str(), &finfo) < 0) return false;
#ifdef WIN32 
  if (finfo.st_mode & S_IFDIR)
    return (::_rmdir(aPath.c_str())==-1 ? false : true);
  else
    return (::unlink(aPath.c_str())==-1 ? false : true);
#else
  if (S_ISDIR(finfo.st_mode))
    return (::rmdir(aPath.c_str())==-1 ? false : true);
  else
    return (::unlink(aPath.c_str())==-1 ? false : true);
#endif
}
