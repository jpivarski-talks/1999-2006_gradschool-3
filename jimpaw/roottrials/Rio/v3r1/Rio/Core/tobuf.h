#ifndef Rio_tobuf_h
#define Rio_tobuf_h

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// A set of inline byte handling routines.                              //
//                                                                      //
// The set of Rio_tobuf() and Rio_frombuf() routines take care of       //
// packing a basic type value into a buffer in network byte order       //
// (i.e. they perform byte swapping when needed). The buffer does not   //
// have to start on a machine (long) word boundary.                     //
//                                                                      //
// For __GNUC__ on linux on i486 processors and up                      //
// use the `bswap' opcode provided by the GNU C Library.                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <Rio/Core/ByteSwap.h>
#include <Rio/Core/Platform.h>

#ifndef RIO_BYTESWAP
#include <string.h> //For memcpy.
#endif

inline void Rio_tobuf(char *&buf, unsigned char x) {
  *buf++ = x;
}

inline void Rio_tobuf(char *&buf, unsigned short x) {
#ifdef RIO_BYTESWAP
#if defined(__linux) && defined(__i386__)
  *((unsigned short*)buf) = Rio_bswap_16(x);
#else
  char *sw = (char *)&x;
  buf[0] = sw[1];
  buf[1] = sw[0];
#endif
#else
  ::memcpy(buf, &x, sizeof(unsigned short));
#endif
  buf += sizeof(unsigned short);
}

inline void Rio_tobuf(char *&buf, unsigned int x) {
#ifdef RIO_BYTESWAP
# if defined(__linux) && defined(__i386__)
   *((unsigned int*)buf) = Rio_bswap_32(x);
# else
   char *sw = (char *)&x;
   buf[0] = sw[3];
   buf[1] = sw[2];
   buf[2] = sw[1];
   buf[3] = sw[0];
# endif
#else
   ::memcpy(buf, &x, sizeof(unsigned int));
#endif
   buf += sizeof(unsigned int);
}

inline void Rio_tobuf(char *&buf, unsigned long x){
#ifdef RIO_BYTESWAP
   char *sw = (char *)&x;
   if (sizeof(unsigned long) == 8) {
      buf[0] = sw[7];
      buf[1] = sw[6];
      buf[2] = sw[5];
      buf[3] = sw[4];
      buf[4] = sw[3];
      buf[5] = sw[2];
      buf[6] = sw[1];
      buf[7] = sw[0];
   } else {
      buf[0] = 0;
      buf[1] = 0;
      buf[2] = 0;
      buf[3] = 0;
      buf[4] = sw[3];
      buf[5] = sw[2];
      buf[6] = sw[1];
      buf[7] = sw[0];
   }
#else
   if (sizeof(unsigned long) == 8) {
      ::memcpy(buf, &x, 8);
   } else {
      buf[0] = 0;
      buf[1] = 0;
      buf[2] = 0;
      buf[3] = 0;
      ::memcpy(buf+4, &x, 4);
   }
#endif
   buf += 8;
}

inline void Rio_tobuf(char *&buf, float x) {
#ifdef RIO_BYTESWAP
# if defined(__linux) && defined(__i386__)
   *((unsigned int*)buf) = Rio_bswap_32(*((unsigned int*)&x));
# else
   char *sw = (char *)&x;
   buf[0] = sw[3];
   buf[1] = sw[2];
   buf[2] = sw[1];
   buf[3] = sw[0];
# endif
#else
   ::memcpy(buf, &x, sizeof(float));
#endif
   buf += sizeof(float);
}

inline void Rio_tobuf(char *&buf, double x) {
#ifdef RIO_BYTESWAP
# if defined(__linux) && defined(__i386__) && defined __GNUC__ && __GNUC__ >= 2 && !defined(__STRICT_ANSI__)
  *((unsigned long long *)buf) = Rio_bswap_64(*((unsigned long long *)&x));
# else
   char *sw = (char *)&x;
   buf[0] = sw[7];
   buf[1] = sw[6];
   buf[2] = sw[5];
   buf[3] = sw[4];
   buf[4] = sw[3];
   buf[5] = sw[2];
   buf[6] = sw[1];
   buf[7] = sw[0];
# endif
#else
   ::memcpy(buf, &x, sizeof(double));
#endif
   buf += sizeof(double);
}

inline void Rio_frombuf(char *&buf, unsigned char* x) {
  *x = *buf++;
}

inline void Rio_frombuf(char *&buf, unsigned short* x) {
#ifdef RIO_BYTESWAP
# if defined(__linux) && defined(__i386__)
   *x = Rio_bswap_16(*((unsigned short*)buf));
# else
   char *sw = (char *)x;
   sw[0] = buf[1];
   sw[1] = buf[0];
# endif
#else
   ::memcpy(x, buf, sizeof(unsigned short));
#endif
   buf += sizeof(unsigned short);
}

inline void Rio_frombuf(char *&buf, unsigned int* x) {
#ifdef RIO_BYTESWAP
# if defined(__linux) && defined(__i386__)
   *x = Rio_bswap_32(*((unsigned int*)buf));
# else
   char *sw = (char *)x;
   sw[0] = buf[3];
   sw[1] = buf[2];
   sw[2] = buf[1];
   sw[3] = buf[0];
# endif
#else
   ::memcpy(x, buf, sizeof(unsigned int));
#endif
   buf += sizeof(unsigned int);
}

inline void Rio_frombuf(char *&buf, unsigned long *x){
#ifdef RIO_BYTESWAP
   char *sw = (char *)x;
   if (sizeof(unsigned long) == 8) {
      sw[0] = buf[7];
      sw[1] = buf[6];
      sw[2] = buf[5];
      sw[3] = buf[4];
      sw[4] = buf[3];
      sw[5] = buf[2];
      sw[6] = buf[1];
      sw[7] = buf[0];
   } else {
      sw[0] = buf[7];
      sw[1] = buf[6];
      sw[2] = buf[5];
      sw[3] = buf[4];
   }
#else
   if (sizeof(unsigned long) == 8) {
      ::memcpy(x, buf, 8);
   } else {
      ::memcpy(x, buf+4, 4);
   }
#endif
   buf += 8;
}

inline void Rio_frombuf(char *&buf, float* x) {
#ifdef RIO_BYTESWAP
# if defined(__linux) && defined(__i386__)
   *((unsigned int*)x) = Rio_bswap_32(*((unsigned int*)buf));
# else
   char *sw = (char *)x;
   sw[0] = buf[3];
   sw[1] = buf[2];
   sw[2] = buf[1];
   sw[3] = buf[0];
# endif
#else
   ::memcpy(x, buf, sizeof(float));
#endif
   buf += sizeof(float);
}

inline void Rio_frombuf(char *&buf, double* x) {
#ifdef RIO_BYTESWAP
# if defined(__linux) && defined(__i386__) && defined __GNUC__ && __GNUC__ >= 2 && !defined(__STRICT_ANSI__)
   *((unsigned long long*)x) = Rio_bswap_64(*((unsigned long long *)buf));
# else
   char *sw = (char *)x;
   sw[0] = buf[7];
   sw[1] = buf[6];
   sw[2] = buf[5];
   sw[3] = buf[4];
   sw[4] = buf[3];
   sw[5] = buf[2];
   sw[6] = buf[1];
   sw[7] = buf[0];
# endif
#else
   ::memcpy(x, buf, sizeof(double));
#endif
   buf += sizeof(double);
}

inline void Rio_tobuf(char *&buf, char x)  { 
  Rio_tobuf(buf, (unsigned char) x); }
inline void Rio_tobuf(char *&buf, short x) { 
  Rio_tobuf(buf, (unsigned short) x); }
inline void Rio_tobuf(char *&buf, int x)   { 
  Rio_tobuf(buf, (unsigned int) x); }
inline void Rio_tobuf(char *&buf, long x)   { 
  Rio_tobuf(buf, (unsigned long) x); }

inline void Rio_frombuf(char *&buf, char* x)  { 
  Rio_frombuf(buf,(unsigned char*) x); }
inline void Rio_frombuf(char *&buf, short* x) { 
  Rio_frombuf(buf,(unsigned short*) x); }
inline void Rio_frombuf(char *&buf, int* x) { 
  Rio_frombuf(buf,(unsigned int*) x); }
inline void Rio_frombuf(char *&buf, long* x) { 
  Rio_frombuf(buf,(unsigned long*) x); }

#include <string>
inline void Rio_tobuf(char*& buf, const std::string& x) {
  unsigned char nwh;
  int nchars = x.size();
  if (nchars > 254) {
    nwh = 255;
    Rio_tobuf(buf,nwh);
    Rio_tobuf(buf,nchars);
  } else {
    nwh = (unsigned char)nchars;
    Rio_tobuf(buf,nwh);
  }
  for (int i = 0; i < nchars; i++) buf[i] = x[i];
  buf += nchars;
}
inline void Rio_frombuf(char *&buf,std::string& x) {
  unsigned char nwh;
  int nchars;
  Rio_frombuf(buf,&nwh);
  if (nwh == 255)
    Rio_frombuf(buf,&nchars);
  else
    nchars = nwh;
  //FIXME : is there a way to put directly things into x ?
  char* s = new char[nchars+1];
  char* p = s;
  for (int i = 0; i < nchars; i++,p++) Rio_frombuf(buf,p);
  s[nchars] = '\0';
  x = std::string(s);
  delete [] s;
}
inline int Rio_std_string_recordSize(const std::string& x) {
  // Returns size string will occupy on I/O buffer.
  if (x.size() > 254)
    return x.size()+sizeof(unsigned char)+sizeof(int);
  else
    return x.size()+sizeof(unsigned char);
}


#endif
