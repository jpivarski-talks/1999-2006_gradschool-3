//////////////////////////////////////////////////////////////////////////
// Buffer base class used for serializing objects.                      //
//////////////////////////////////////////////////////////////////////////

// this :
#include <Rio/Core/Buffer.h>
#include <Rio/Core/Platform.h>

#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IObject.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Core/tobuf.h>
#include <Rio/Core/Lib.h>
#include <Rio/Core/Map.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Out.h>
#include <Rio/Core/Arguments.h>

#include <string.h> //memcpy

#define Rio_assert(e) \
  if (!(e)) {\
    Out out(fPrinter);\
    out << "Rio_assert : " << #e << " violated at line " \
        << __LINE__ << " of \"" << __FILE__ << "\"." << endl;\
    return;\
  }

#define Rio_assert_return(e,r) \
  if (!(e)) {\
    Out out(fPrinter);\
    out << "Rio_assert : " << #e << " violated at line " \
        << __LINE__ << " of \"" << __FILE__ << "\"." << endl;\
    return r;\
  }

#define MAXIMUM(a,b) ((a)>(b)?a:b) 
#define MINIMUM(a,b) ((a)<(b)?a:b) 

const unsigned int Rio_kNullTag = 0;
const int Rio_kExtraSpace = 8;
const int Rio_kMaxInt = 2147483647;       
const unsigned int Rio_kByteCountMask = 0x40000000; 
const short Rio_kByteCountVMask = 0x4000;
const short Rio_kMaxVersion = 0x3FFF;
// last valid fMapCount and byte count
const unsigned int Rio_kMaxMapCount = 0x3FFFFFFE;  
const unsigned int Rio_kNewClassTag = 0xFFFFFFFF;
// OR the class index with this
const unsigned int Rio_kClassMask = 0x80000000;  

// first 2 map entries are taken by null obj and self obj :
const int Rio_kMapOffset = 2;   
const int Rio_kMapSize = 503;

//////////////////////////////////////////////////////////////////////////////
Rio::Buffer::Buffer(
 const Buffer& aBuffer
)
:IBuffer()
,fDictionary(aBuffer.fDictionary)
,fPrinter(aBuffer.fPrinter)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::operator=(
 const Buffer&
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::Buffer::Buffer(
 IDictionary& aDictionary
,IPrinter& aPrinter
,Mode aMode
,int aBufferSize
,char* aBuffer
)
:fDictionary(aDictionary)
,fPrinter(aPrinter)
,fMode(aMode)
,fVersion(0)
,fBufSize(0)
,fBuffer(0)
,fBufCur(0)
,fBufMax(0)
,fMapCount(0)
,fMapSize(Rio_kMapSize)
,fDisplacement(0)
,fReadMap(0)
//////////////////////////////////////////////////////////////////////////////
// Create an I/O buffer object. Mode should be either TBuffer::kRead or
// TBuffer::kWrite. By default the I/O buffer has a size of
// TBuffer::kInitialSize (1024) bytes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Debug::increment();
  // Before using the buffer make sure some assumptions are true
  Rio_assert(sizeof(short) == 2);
  Rio_assert(sizeof(int) == 4);
// commented lines below in view of support for longlong on 32 bits machines
//#ifdef R__B64
//   Rio_assert(sizeof(long) == 8);
//#else
//   Rio_assert(sizeof(long) == 4);
//#endif
  Rio_assert(sizeof(float) == 4);
  Rio_assert(sizeof(double) == 8);

  if (aBuffer) {
    fBufSize  = aBufferSize;
    fBuffer = aBuffer;
  } else {
    fBufSize  = (aBufferSize < MINIMAL) ? MINIMAL : aBufferSize;
    fBuffer = new char[fBufSize + Rio_kExtraSpace];
    if(!fBuffer) {
      Out out(fPrinter);
      out << "Rio::Buffer::Buffer : can't alloc " 
	  << fBufSize + Rio_kExtraSpace << " bytes." << endl;
      fBufSize = 0;
    }
  }

  fBufCur = fBuffer;
  fBufMax = fBuffer + fBufSize;
}
//////////////////////////////////////////////////////////////////////////////
Rio::Buffer::~Buffer(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  delete [] fBuffer;
  if(isReading()) delete fReadMap;
  else delete fWriteMap;
  fReadMap = 0;
  Debug::decrement();
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::isReading(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return (fMode & WRITE) == 0; 
}
//////////////////////////////////////////////////////////////////////////////
bool Rio::Buffer::isWriting(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return (fMode & WRITE) != 0; 
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setBuffer(
 void* aBuffer
,unsigned int aNewSize
)
//////////////////////////////////////////////////////////////////////////////
// Set buffer address
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fBuffer = (char *)aBuffer;
  fBufCur = fBuffer;
  if (aNewSize > 0) fBufSize = aNewSize;
  fBufMax = fBuffer + fBufSize;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setReadMode(
)
//////////////////////////////////////////////////////////////////////////////
// Set buffer in read mode.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fMode = READ;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setWriteMode(
)
//////////////////////////////////////////////////////////////////////////////
// Set buffer in write mode.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fMode = WRITE;
}
//////////////////////////////////////////////////////////////////////////////
char* Rio::Buffer::buffer(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fBuffer;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setDisplacement(
 int aSkipped
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fDisplacement = (int)(length() - aSkipped); 
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setDisplacement(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fDisplacement = 0;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::displacement(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fDisplacement;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::bufferSize(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return fBufSize;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::length(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return (int)(fBufCur - fBuffer); 
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setBufferOffset(
 int aOffset
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  fBufCur = fBuffer+aOffset;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::expand(
 int aNewSize
)
//////////////////////////////////////////////////////////////////////////////
// Expand the I/O buffer to newsize bytes.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  int len = length();

  fBuffer = (char*) Rio::realloc(fBuffer,
				(aNewSize+Rio_kExtraSpace) * sizeof(char),
				(fBufSize+Rio_kExtraSpace) * sizeof(char));
  if(!fBuffer) {
    Out out(fPrinter);
    out << "Rio::Buffer::expand : can't realloc "
	<< (aNewSize+Rio_kExtraSpace) * sizeof(char) << " bytes." << endl;
    fBufSize = 0;
    fBufCur = 0;
    fBufMax = 0;
    return;
  }
  fBufSize = aNewSize;
  fBufCur  = fBuffer + len;
  fBufMax  = fBuffer + fBufSize;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::read(unsigned char& x){
  Rio_frombuf(fBufCur,&x);
}
void Rio::Buffer::read(unsigned short& x){
  Rio_frombuf(fBufCur,&x);
}
void Rio::Buffer::read(unsigned int& x){
  Rio_frombuf(fBufCur,&x);
}
void Rio::Buffer::read(unsigned long& x){
  Rio_frombuf(fBufCur,&x);
}
void Rio::Buffer::read(float& x){
  Rio_frombuf(fBufCur,&x);
}
void Rio::Buffer::read(double& x){
  Rio_frombuf(fBufCur,&x);
}
void Rio::Buffer::read(char& x){ 
  Rio_frombuf(fBufCur,(unsigned char*)&x); 
}
void Rio::Buffer::read(bool& x){ 
  Rio_frombuf(fBufCur,(unsigned char*)&x); 
}
void Rio::Buffer::read(short& x){ 
  Rio_frombuf(fBufCur,(unsigned short*)&x); 
}
void Rio::Buffer::read(int& x){ 
  Rio_frombuf(fBufCur,(unsigned int*)&x); 
}
void Rio::Buffer::read(long& x){ 
  Rio_frombuf(fBufCur,(unsigned long*)&x); 
}
void Rio::Buffer::read(std::string& x) {
  Rio_frombuf(fBufCur,x);
}
void Rio::Buffer::write(unsigned char x) {
  if (fBufCur + sizeof(unsigned char) > fBufMax) expand(2*fBufSize);
  Rio_tobuf(fBufCur,x);
}
void Rio::Buffer::write(unsigned short x){
  if (fBufCur + sizeof(unsigned short) > fBufMax) expand(2*fBufSize);
  Rio_tobuf(fBufCur,x);
}
void Rio::Buffer::write(unsigned int x){
  if (fBufCur + sizeof(unsigned int) > fBufMax) expand(2*fBufSize);
  Rio_tobuf(fBufCur,x);
}
void Rio::Buffer::write(unsigned long x){
  if (fBufCur + sizeof(unsigned long) > fBufMax) expand(2*fBufSize);
  Rio_tobuf(fBufCur,x);
}
void Rio::Buffer::write(float x){
  if (fBufCur + sizeof(float) > fBufMax) expand(2*fBufSize);
  Rio_tobuf(fBufCur,x);
}
void Rio::Buffer::write(double x) {
  if (fBufCur + sizeof(double) > fBufMax) expand(2*fBufSize);
  Rio_tobuf(fBufCur,x);
}
void Rio::Buffer::write(char x){ 
  write((unsigned char)x); 
}
void Rio::Buffer::write(bool x){ 
  write((unsigned char)x); 
}
void Rio::Buffer::write(short x){ 
  write((unsigned short)x);
}
void Rio::Buffer::write(int x){ 
  write((unsigned int)x); 
}
void Rio::Buffer::write(long x){ 
  write((unsigned long)x); 
}
void Rio::Buffer::write(const std::string& x) {
  if (fBufCur + x.size() > fBufMax) expand(2*fBufSize);
  Rio_tobuf(fBufCur,x);
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::readArray(
 char*& c
)
//////////////////////////////////////////////////////////////////////////////
// Read array of characters from the I/O buffer. Returns the number of
// characters read. If argument is a 0 pointer then space will be
// allocated for the array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(isReading(),0);

  int n;
  read(n);

  if (!n) return n;

  if (!c) c = new char[n];
  
  int l = n * sizeof(char);
  ::memcpy(c, fBufCur, l);
  fBufCur += l;

  return n;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::readArray(
 int*& ii
)
//////////////////////////////////////////////////////////////////////////////
// Read array of ints from the I/O buffer. Returns the number of ints
// read. If argument is a 0 pointer then space will be allocated for the
// array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(isReading(),0);

  int n;
  read(n);

  if (!n) return n;
  
  if (!ii) ii = new int[n];
  
#ifdef RIO_BYTESWAP
# ifdef USE_BSWAPCPY
  bswapcpy32(ii, fBufCur, n);
  fBufCur += n * sizeof(int);
# else
  for (int i = 0; i < n; i++)
    Rio_frombuf(fBufCur, &ii[i]);
# endif
#else
  int l = n * sizeof(int);
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
  return n;
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::readArray(
 double*& d
)
//////////////////////////////////////////////////////////////////////////////
// Read array of doubles from the I/O buffer. Returns the number of doubles
// read. If argument is a 0 pointer then space will be allocated for the
// array.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(isReading(),0);

  int n;
  read(n);

  if (!n) return n;

  if (!d) d = new double[n];

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++)
    Rio_frombuf(fBufCur, &d[i]);
#else
  int l = n * sizeof(double);
  ::memcpy(d, fBufCur, l);
  fBufCur += l;
#endif

  return n;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeArray(
 const int* ii
,int n
) 
//////////////////////////////////////////////////////////////////////////////
// Write array of n ints into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert(isWriting());
  
  write(n);
  
  if (!n) return;
  
  Rio_assert(ii);

  int l = n * sizeof(int);
  if (fBufCur + l > fBufMax) expand(MAXIMUM(2*fBufSize, fBufSize+l));
  
#ifdef RIO_BYTESWAP
# ifdef USE_BSWAPCPY
  bswapcpy32(fBufCur, ii, n);
  fBufCur += l;
# else
  for (int i = 0; i < n; i++)
    Rio_tobuf(fBufCur, ii[i]);
# endif
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeArray(
 const double* d
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n doubles into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert(isWriting());

  write(n);

  if (!n) return;

  Rio_assert(d);

  int l = n * sizeof(double);
  if (fBufCur + l > fBufMax) expand(MAXIMUM(2*fBufSize, fBufSize+l));

#ifdef RIO_BYTESWAP
  for (int i = 0; i < n; i++)
    Rio_tobuf(fBufCur, d[i]);
#else
  ::memcpy(fBufCur, d, l);
  fBufCur += l;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::readFastArray(
 char* c
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n characters from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return;
  int l = n * sizeof(char);
  ::memcpy(c, fBufCur, l);
  fBufCur += l;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::readFastArray(
 int* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n ints from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return;

#ifdef RIO_BYTESWAP
# ifdef USE_BSWAPCPY
  bswapcpy32(ii, fBufCur, n);
  fBufCur += sizeof(Int_t)*n;
# else
  //char *sw = (char*)ii;
  for (int i = 0; i < n; i++) {
    Rio_frombuf(fBufCur, &ii[i]);
    //sw[0] = fBufCur[3];
    //sw[1] = fBufCur[2];
    //sw[2] = fBufCur[1];
    //sw[3] = fBufCur[0];
    //fBufCur += 4;
    //sw += 4;
  }
# endif
#else
  int l = n * sizeof(int);
  ::memcpy(ii, fBufCur, l);
  fBufCur += l;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::readFastArray(
 float* f
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Read array of n floats from the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return;

#ifdef RIO_BYTESWAP
# ifdef USE_BSWAPCPY
  bswapcpy32(f, fBufCur, n);
  fBufCur += sizeof(Float_t)*n;
# else
//   char *sw = (char*)f;
  for (int i = 0; i < n; i++) {
    Rio_frombuf(fBufCur, &f[i]);
    //sw[0] = fBufCur[3];
    //sw[1] = fBufCur[2];
    //sw[2] = fBufCur[1];
    //sw[3] = fBufCur[0];
    //fBufCur += 4;
    //sw += 4;
  }
# endif
#else
  int l = n * sizeof(float);
  ::memcpy(f, fBufCur, l);
  fBufCur += l;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeFastArray(
 const char* c
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n characters into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return;
  int l = n * sizeof(char);
  if (fBufCur + l > fBufMax) expand(MAXIMUM(2*fBufSize, fBufSize+l));
  ::memcpy(fBufCur, c, l);
  fBufCur += l;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeFastArray(
 const int* ii
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n ints into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return;

  int l = n * sizeof(int);
  if (fBufCur + l > fBufMax) expand(MAXIMUM(2*fBufSize, fBufSize+l));

#ifdef RIO_BYTESWAP
# ifdef USE_BSWAPCPY
  bswapcpy32(fBufCur, ii, n);
  fBufCur += l;
# else
  for (int i = 0; i < n; i++)
    Rio_tobuf(fBufCur, ii[i]);
# endif
#else
  ::memcpy(fBufCur, ii, l);
  fBufCur += l;
#endif
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeFastArray(
 const float* f
,int n
)
//////////////////////////////////////////////////////////////////////////////
// Write array of n floats into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (n <= 0) return;

  int l = n * sizeof(float);
  if (fBufCur + l > fBufMax) expand(MAXIMUM(2*fBufSize, fBufSize+l));
  
#ifdef RIO_BYTESWAP
# ifdef USE_BSWAPCPY
  bswapcpy32(fBufCur, f, n);
  fBufCur += l;
# else
  for (int i = 0; i < n; i++)
    Rio_tobuf(fBufCur, f[i]);
# endif
#else
  ::memcpy(fBufCur, f, l);
  fBufCur += l;
#endif
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
char* Rio::Buffer::readString(
 char* aString
,int aMax
)
//////////////////////////////////////////////////////////////////////////////
// Read string from I/O buffer. String is read till 0 character is
// found or till max-1 characters are read (i.e. string s has max
// bytes allocated). If max = -1 no check on number of character is
// made, reading continues till 0 character is found.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!isReading()) return 0;
  if (aMax == -1) aMax = Rio_kMaxInt;
  char  ch;
  int nr = 0;
  while (nr < aMax-1) {
    read(ch);
    // stop when 0 read
    if (ch == 0) break;
    aString[nr++] = ch;
  }
  aString[nr] = 0;
  return aString;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeString(
 const char* aString
)
//////////////////////////////////////////////////////////////////////////////
// Write string to I/O buffer. Writes string upto and including the
// terminating 0.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  writeBuffer(aString,(::strlen(aString)+1)*sizeof(char));
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::readBuffer(
 void* aBuffer
,int aMax
)
//////////////////////////////////////////////////////////////////////////////
// Read max bytes from the I/O buffer. The function returns
// the actual number of bytes read.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!isReading()) return 0;
  if (aMax <= 0) return 0;
  int n = MINIMUM(aMax, (int)(fBufMax - fBufCur));
  ::memcpy(aBuffer, fBufCur, n);
  fBufCur += n;
  return n;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeBuffer(
 const void* aBuffer
,int aMax
)
//////////////////////////////////////////////////////////////////////////////
// Write max bytes from buf into the I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!isWriting()) return;
  if (aMax <= 0) return;
  if (fBufCur + aMax > fBufMax) expand(MAXIMUM(2*fBufSize, fBufSize+aMax));
  ::memcpy(fBufCur,aBuffer,aMax);
  fBufCur += aMax;
}
//////////////////////////////////////////////////////////////////////////////
short Rio::Buffer::readVersion(
 unsigned int* aStartPosition
,unsigned int* aByteCount
)
//////////////////////////////////////////////////////////////////////////////
// Read class version from I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  short version;
  if (aStartPosition && aByteCount) {
    // before reading object save start position
    *aStartPosition = (unsigned int)(fBufCur-fBuffer);
    
    // read byte count (older files don't have byte count)
    // byte count is packed in two individual shorts, this to be
    // backward compatible with old files that have at this location
    // only a single short (i.e. the version)
    union {
      unsigned int cnt;
      short vers[2];
    } v;
#ifdef RIO_BYTESWAP
    read(v.vers[1]);
    read(v.vers[0]);
#else
    read(v.vers[0]);
    read(v.vers[1]);
#endif
    
    // no bytecount, backup and read version
    if (!(v.cnt & Rio_kByteCountMask)) {
      fBufCur -= sizeof(unsigned int);
      v.cnt = 0;
    }
    *aByteCount = (v.cnt & ~Rio_kByteCountMask);
    read(version);
    //printf("Reading version=%d at pos=%d, bytecount=%d\n",version,*startpos,*bcnt);
    
  } else {
    
    // not interested in byte count
    read(version);
    
    // if this is a byte count, then skip next short and read version
    if (version & Rio_kByteCountVMask) {
      read(version);
      read(version);
    }
    //printf("Reading version=%d at pos=%d\n",version,startpos);
  }
  
  return version;
}
//////////////////////////////////////////////////////////////////////////////
unsigned int Rio::Buffer::writeVersion(
 short aVersion
,bool aUseBcnt
)
//////////////////////////////////////////////////////////////////////////////
// Write class version to I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int cntpos = 0;
  if (aUseBcnt) {
    // reserve space for leading byte count
    cntpos = (unsigned int)(fBufCur-fBuffer);
    fBufCur += sizeof(unsigned int);
  }

  if (aVersion > Rio_kMaxVersion) {
    Out out(fPrinter);
    out << "Rio::Buffer::writeVersion : "
	<< "version number cannot be larger than " << Rio_kMaxVersion << endl;
    aVersion = Rio_kMaxVersion;
  }

  write(aVersion);
  
  // return position where to store possible byte count
  return cntpos;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::setByteCount(
 unsigned int aCountPos
,bool aPackInVersion
)
//////////////////////////////////////////////////////////////////////////////
// Set byte count at position cntpos in the buffer. Generate warning if
// count larger than kMaxMapCount. The count is excluded its own size.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  unsigned int cnt = 
    (unsigned int)(fBufCur - fBuffer) - aCountPos - sizeof(unsigned int);
  char  *buf = (char *)(fBuffer + aCountPos);

  // if true, pack byte count in two consecutive shorts, so it can
  // be read by ReadVersion()
  if (aPackInVersion) {
    union {
      unsigned int cnt;
      short vers[2];
    } v;
    v.cnt = cnt;
#ifdef RIO_BYTESWAP
    Rio_tobuf(buf, short(v.vers[1] | Rio_kByteCountVMask));
    Rio_tobuf(buf, v.vers[0]);
#else
    Rio_tobuf(buf, short(v.vers[0] | Rio_kByteCountVMask));
    Rio_tobuf(buf, v.vers[1]);
#endif
  } else {
    Rio_tobuf(buf, cnt | Rio_kByteCountMask);
  }

  if (cnt >= Rio_kMaxMapCount) {
    Out out(fPrinter);
    out << "Rio::Buffer::setByteCount : bytecount too large (more than "
	<< Rio_kMaxMapCount << ")." << endl;
    // exception
  }
}
//////////////////////////////////////////////////////////////////////////////
int Rio::Buffer::checkByteCount(
 unsigned int aStartPos
,unsigned int aByteCount
,const std::string& aClassName
)
//////////////////////////////////////////////////////////////////////////////
// Check byte count with current buffer position. They should
// match. If not print warning and position buffer in correct
// place determined by the byte count. Startpos is position of
// first byte where the byte count is written in buffer.
// Returns 0 if everything is ok, otherwise the bytecount offset
// (< 0 when read too little, >0 when read too much).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
   if (!aByteCount) return 0;

   int offset = 0;

   long endpos = long(fBuffer) + aStartPos + aByteCount + sizeof(unsigned int);

   if (long(fBufCur) != endpos) {
     if (long(fBufCur) < endpos) {
       Out out(fPrinter);
       out << "Rio::Buffer::checkByteCount : object of class \""
	   << aClassName << "\" read too few bytes (" 
	   << endpos-(long)fBufCur << " missing)." << endl;
     }
     if (long(fBufCur) > endpos) {
       Out out(fPrinter);
       out << "Rio::Buffer::checkByteCount : object of class \""
	   << aClassName << "\" read too many bytes ("
	   << (long)fBufCur-endpos << " in excess)." << endl;
     }
     Out out(fPrinter);
     out << "Rio::Buffer::checkByteCount : \"" << aClassName 
	 << "\"::Streamer() not in sync with data on file, fix Streamer()"
	 << endl;

     offset = int(long(fBufCur) - endpos);
     
     fBufCur = (char*)endpos;
   }
   return offset;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::checkCount(
 unsigned int aOffset
)
//////////////////////////////////////////////////////////////////////////////
// Check if offset is not too large (< kMaxMapCount) when writing.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (isWriting()) {
    if (aOffset >= Rio_kMaxMapCount) {
      Out out(fPrinter);
      out << "Rio::Buffer::checkCount : " 
	  << "buffer offset too large (larger than " 
	  << Rio_kMaxMapCount << ")." << endl;
      // exception
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::initMap(
)
//////////////////////////////////////////////////////////////////////////////
// Create the fWriteMap or fReadMap containers and initialize them
// with the null object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (isWriting()) {
    if (!fWriteMap) {
      fWriteMap = new Map(fPrinter,fMapSize);
      fMapCount = 0;
    }
  } else {
    if (!fReadMap) {
      fReadMap = new Map(fPrinter,fMapSize);
      fReadMap->add(0,Rio_kNullTag);      // put kNullTag in slot 0
      fMapCount = 1;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::resetMap(
)
//////////////////////////////////////////////////////////////////////////////
// Delete existing fWriteMap or fReadMap and reset map counter.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (isWriting()) {
    delete fWriteMap;
    fWriteMap = 0;
  } else {
    delete fReadMap;
    fReadMap = 0;
  }
  fMapCount = 0;
  fDisplacement = 0;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::mapObject(
 const void* aObject
,unsigned int aOffset
)
//////////////////////////////////////////////////////////////////////////////
// Add object to the fWriteMap or fReadMap containers (depending on the mode).
// If obj is not 0 add object to the map (in read mode also add 0 objects to
// the map). This method may only be called outside this class just before
// calling obj->Streamer() to prevent self reference of obj, in case obj
// contains (via via) a pointer to itself. In that case offset must be 1
// (default value for offset).
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  //Out out(fPrinter);
  //out << "Rio::Buffer::mapObject : debug obj " << aObject 
  //  << " offset : " << aOffset << endl;

  if (isWriting()) {
    if (!fWriteMap) initMap();
    if (aObject) {
      checkCount(aOffset);
      const void* ptr = aObject;
      fWriteMap->add(hash(&ptr,sizeof(void*)),
		     (long)aObject,
		     aOffset);
      fMapCount++;
    }
  } else {
    if (!fReadMap) initMap();
    fReadMap->add(aOffset,(long)aObject);
    fMapCount++;
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeClass(
 const IClass& aClass
)
//////////////////////////////////////////////////////////////////////////////
// Write class description to I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert(isWriting());

  const void* ptr = &aClass;
  unsigned long idx;

  if ((idx = (unsigned long)
	      fWriteMap->findValue(hash(&ptr,sizeof(void*)),(long)&aClass)
	      ) != 0) {
    
    // truncation is OK the value we did put in the map is an 30-bit offset
    // and not a pointer
    unsigned int clIdx = (unsigned int)idx;
    
    // save index of already stored class
    write((clIdx | Rio_kClassMask));

  } else {

    // offset in buffer where class info is written
    unsigned int offset = (unsigned int)(fBufCur-fBuffer);
    
    // save new class tag
    write(Rio_kNewClassTag);
    
    // write class name
    //cl->Store(*this);
    
    writeString(aClass.name().c_str());
    
    // store new class reference in fWriteMap (+Rio_kMapOffset so it's != kNullTag)
    mapObject(&aClass, offset + Rio_kMapOffset);
    
  }
}
//////////////////////////////////////////////////////////////////////////////
unsigned int Rio::Buffer::checkObject(
 unsigned int aOffset
,const IClass* aClass
,bool aReadClass //default : false
)
//////////////////////////////////////////////////////////////////////////////
// Check for object in the read map. If the object is 0 it still has to be
// read. Try to read it from the buffer starting at location offset. If the
// object is -1 then it really does not exist and we return 0. If the object
// exists just return the offset.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // in position 0 we always have the reference to the null object
  if (!aOffset) return aOffset;

  long cli;

  if (aReadClass) {
    if ((cli = fReadMap->findValue(aOffset)) == 0) {
      Out out(fPrinter);
      out << "Rio::Buffer::checkObject : debug : no class found !" << endl; 

      // No class found at this location in map. It might have been skipped
      // as part of a skipped object. Try to explicitely read the class.
      
      // save fBufCur and set to place specified 
      // by aOffset (-kMapOffset-sizeof(bytecount))
      char* bufsav = fBufCur;
      fBufCur = 
	(char*)(fBuffer + aOffset - Rio_kMapOffset-sizeof(unsigned int));
      
      unsigned int tag;
      IClass* c = readClass(aClass,tag);
      if (c == (IClass*)-1) {
	// mark class as really not available
	fReadMap->remove(aOffset);
	fReadMap->add(aOffset, -1);
	aOffset = 0;
	Out out(fPrinter);
	out << "Rio::Buffer::checkObject : " 
	    << "reference to unavailable class \"" 
	    << (aClass ? aClass->name() : "")
	    << "\", pointers of this type will be 0." << endl;
      }
      
      fBufCur = bufsav;
      
    } else if (cli == -1) {
      
      Out out(fPrinter);
      out << "Rio::Buffer::checkObject : class does not exist." << endl;
      // class really does not exist
      return 0;
    } else {
    }
    
  } else {
    
    if ((cli = fReadMap->findValue(aOffset)) == 0) {

      Out out(fPrinter);
      out << "Rio::Buffer::checkObject : try to read object" << endl;

      // No object found at this location in map. It might have been skipped
      // as part of a skipped object. Try to explicitely read the object.
      
      // save fBufCur and set to place specified by aOffset (-kMapOffset)
      char *bufsav = fBufCur;
      fBufCur = (char *)(fBuffer + aOffset - Rio_kMapOffset);
      
      //FIXME IObject* obj = readObject(cl);
      IObject* obj = readObject(Arguments());
      //IObject* obj = 0;
      if (!obj) {
	// mark object as really not available
	fReadMap->remove(aOffset);
	fReadMap->add(aOffset, -1);
	aOffset = 0;
	out << "Rio::Buffer::checkObject : "
	    << "reference to object of unavailable class \"" 
	    << (aClass ? aClass->name() : "")
	    << "\", pointer will be 0." << endl;
      }
      
      fBufCur = bufsav;
      
    } else if (cli == -1) {
      
      Out out(fPrinter);
      out << "Rio::Buffer::checkObject : object does not exist."<< endl;
      // object really does not exist

      return 0;

    } else {

      //Out out(fPrinter);
      //out << "Rio::Buffer::checkObject : ok."<< endl;

    }
    
  }
  
  return aOffset;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IClass* Rio::Buffer::readClass(
 const IClass* aRequiredClass
,unsigned int& aObjectTag
)
//////////////////////////////////////////////////////////////////////////////
// Read class definition from I/O buffer. clReq can be used to cross check
// if the actually read object is of the requested class. objTag is
// set in case the object is a reference to an already read object.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(isReading(),0);

  // read byte count and/or tag (older files don't have byte count)
  unsigned int bcnt;
  unsigned int tag;
  unsigned int startpos = 0;
  read(bcnt);
  if (!(bcnt & Rio_kByteCountMask) || bcnt == Rio_kNewClassTag) {
    tag  = bcnt;
    bcnt = 0;
  } else {
    fVersion = 1;
    startpos = (unsigned int)(fBufCur-fBuffer);
    read(tag);
  }
  
  // in case tag is object tag return tag
  if (!(tag & Rio_kClassMask)) {
    aObjectTag = tag;
    return 0;
  }
  
  IClass* cl = 0;
  if (tag == Rio_kNewClassTag) {
    
    char s[80];
    readString(s, 80);
    std::string className = s;

    // got a new class description followed by a new object
    // (class can be 0 if class dictionary is not found, in that
    // case object of this class must be skipped)
    // ROOT code : cl = TClass::Load(*this);
    cl = fDictionary.findClass(className);
    if(!cl) {
      Out out(fPrinter);
      out << "Rio::Buffer::readClass : could not create Class of name \""
	  << className << "\"." << endl;
      return 0;
    }

    // add class to fReadMap for later reference
    if (fVersion > 0) {
      // check if class was already read
      IClass* cl1 = (IClass*)fReadMap->findValue(startpos + Rio_kMapOffset);
      if (cl1 != cl) {
	mapObject(cl ? cl : (void*)-1, startpos + Rio_kMapOffset);
      }
    } else {
      mapObject(cl, fMapCount);
    }

  } else {
    
    // got a tag to an already seen class
    unsigned int clTag = (tag & ~Rio_kClassMask);
    
    if (fVersion > 0) {
      clTag += fDisplacement;
      Out out(fPrinter);
      clTag = checkObject(clTag, aRequiredClass,true);
    } else {
      if (clTag == 0 || clTag > (unsigned int)fReadMap->size()) {
	Out out(fPrinter);
	out << "Rio::Buffer::readClass : illegal class tag="
	    << clTag << " (0<tag<=" << fReadMap->size() 
	    << "), I/O buffer corrupted." << endl;
	// exception
      }
    }
    
    // class can be 0 if dictionary was not found
    cl = (IClass*)fReadMap->findValue(clTag);
  }
  
  /*FIXME
  if (cl && clReq && !cl->InheritsFrom(clReq)) {
    Error("ReadClass", "got wrong class: %s",cl->GetName());
    // exception
  }
  */

  // return bytecount in objTag
  aObjectTag = (bcnt & ~Rio_kByteCountMask);
  
  // case of unknown class
  if (!cl) cl = (IClass*)-1;
  
  return cl;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::Buffer::readObject(
 const Arguments& aArgs
)
//////////////////////////////////////////////////////////////////////////////
// Read object from I/O buffer. clReq can be used to cross check
// if the actually read object is of the requested class.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert_return(isReading(),0);

  // make sure fReadMap is initialized
  initMap();
  
  // before reading object save start position
  unsigned int startpos = (unsigned int)(fBufCur-fBuffer);

  // attempt to load next object as TClass clReq
  unsigned int tag;       // either tag or byte count
  //FIXME TClass *clRef = ReadClass(clReq, &tag);

  IClass* clRef = readClass(0,tag);

  // check if object has not already been read
  // (this can only happen when called via checkObject())
  IObject* obj = 0;
  if (fVersion > 0) {
    obj = (IObject*)fReadMap->findValue(startpos + Rio_kMapOffset);
    if (obj) {
      Out out(fPrinter);
      out << "Rio::Buffer::readObject : object already read." << endl;
      if (obj == (IObject*) -1) {
	obj = 0;
      /*FIXME
      } else if (clReq && !obj->IsA()->InheritsFrom(clReq)) {
	out << "Rio::Buffer::readObject : got object of wrong class." << endl;
	// exception
      */
      }
      checkByteCount(startpos, tag, 0);
      return obj;
    }
  }
  
  // unknown class, skip to next object and return 0 obj
  if (clRef == (IClass*) -1) {
    if (fVersion > 0)
      mapObject((void*)-1, startpos + Rio_kMapOffset);
    else
      mapObject(0, fMapCount);
    checkByteCount(startpos, tag, 0);
    return 0;
  }
  
  if (!clRef) {

    // got a reference to an already read object
    if (fVersion > 0) {
      tag += fDisplacement;
      //FIXME tag = checkObject(tag, clReq);
      tag = checkObject(tag, 0);
    } else {
      if (tag > (unsigned int)fReadMap->size()) {
	Out out(fPrinter);
	out << "Rio::Buffer::readObject : " 
	    << "object tag " << tag 
	    << " too large (>" << (unsigned int)fReadMap->size()
	    << "), I/O buffer corrupted." << endl;
	// exception
	return 0;
      }
    }
    
    obj = (IObject*)fReadMap->findValue(tag);

    /*FIXME
    if (obj && clReq && !obj->IsA()->InheritsFrom(clReq)) {
      Error("ReadObject", "got object of wrong class");
      // exception
    }
    */
    
  } else {
    
    // allocate a new object based on the class found
    /* obj = (TObject *)clRef->New(); */
    obj = clRef->create(aArgs); 
    if(!obj) {
      Out out(fPrinter);
      out << "Rio::Buffer::readObject : could not create object of class \"" 
	  << clRef->name() << "\"." << endl;
      return 0;
    }
   
    // add to fReadMap before reading rest of object
    if (fVersion > 0) {
      mapObject(obj, startpos + Rio_kMapOffset);
    } else {
      mapObject(obj, fMapCount);
    }

    obj->stream(*this);
  
    checkByteCount(startpos, tag, clRef->name());

  }
  
  return obj;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::Buffer::writeObject(
 IObject* aObject
)
//////////////////////////////////////////////////////////////////////////////
// Write object to I/O buffer.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Rio_assert(isWriting());

  // make sure fWriteMap is initialized
  initMap();

  const void* ptr = aObject;
  unsigned long idx;

  if (!aObject) {

    // save kNullTag to represent NULL pointer
    write(Rio_kNullTag);

  } else if ((idx = (unsigned long)
	      fWriteMap->findValue(hash(&ptr,sizeof(void*)),(long)aObject)
	      ) != 0) {
    
    // truncation is OK the value we did put in the map is an 30-bit offset
    // and not a pointer
    unsigned int objIdx = (unsigned int)idx;
    
    // save index of already stored object
    write(objIdx);
    
  } else {

    // reserve space for leading byte count
    unsigned int cntpos = (unsigned int)(fBufCur-fBuffer);
    fBufCur += sizeof(unsigned int);
    
    // write class of object first
    writeClass(aObject->isA());
    
    // add to map before writing rest of object (to handle self reference)
    // (+Rio_kMapOffset so it's != kNullTag)
    mapObject(aObject, cntpos + Rio_kMapOffset);
    
    // let the object write itself :
    aObject->stream(*this);
    
    // write byte count
    setByteCount(cntpos);
  
  }
}
