#ifndef Rio_Platform_h
#define Rio_Platform_h

#if defined(__alpha) && !defined(__linux__)
#   ifndef __VMS
#      define RIO_BYTESWAP
#   endif
#endif

#if defined(__sun) && !defined(__linux__)
#   ifdef __SVR4
#      ifdef __i386
#         define RIO_BYTESWAP
#      endif
#   endif
#   if __SUNPRO_CC > 0x420
#      define RIO_SOLARIS_CC50
#   endif
#endif

#if defined(__linux__) && defined(__i386__)
#   define RIO_BYTESWAP
#endif

#if defined(__linux__) && defined(__ia64__)
#   define RIO_BYTESWAP
#endif

#if defined(__linux__) && defined(__alpha__)
#   define RIO_BYTESWAP
#endif

#if defined(__FreeBSD__)
#   define RIO_BYTESWAP
#endif

#ifdef _WIN32
#   define RIO_BYTESWAP
#endif

#ifdef BORLAND
#   define RIO_BYTESWAP
#endif

#ifdef __KCC
#   define RIO_KCC
#endif

#endif

