#ifndef Rio_ZIP_h
#define Rio_ZIP_h

/*

 Copyright (C) 1990-1993 Mark Adler, Richard B. Wales, Jean-loup Gailly,
 Kai Uwe Rommel and Igor Mandrichenko.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as all of the original files are included,
 that it is not sold for profit, and that this copyright notice is retained.

*/

/*
 *  zip.h by Mark Adler.
 */


#define ZIP   /* for crypt.c:  include zip password functions, not unzip */

/* Set up portability */
/* #include "tailor.h" */
#include <Rio/zip/Tailor.h>

#define MIN_MATCH  3
#define MAX_MATCH  258
/* The minimum and maximum match lengths */

#ifndef WSIZE
#  define WSIZE  ((unsigned)32768)
#endif
/* Maximum window size = 32K. If you are really short of memory, compile
 * with a smaller WSIZE but this reduces the compression ratio for files
 * of size > WSIZE. WSIZE must be a power of two in the current implementation.
 */

#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
/* Minimum amount of lookahead, except at the end of the input file.
 * See deflate.c for comments about the MIN_MATCH+1.
 */

#define MAX_DIST  (WSIZE-MIN_LOOKAHEAD)
/* In order to simplify the code, particularly on 16 bit machines, match
 * distances are limited to MAX_DIST instead of WSIZE.
 */


/* Define fseek() commands */
#ifndef SEEK_SET
#  define SEEK_SET 0
#endif /* !SEEK_SET */

#ifndef SEEK_CUR
#  define SEEK_CUR 1
#endif /* !SEEK_CUR */

/* Types centralized here for easy modification */
#define local static            /* More meaningful outside functions */
typedef unsigned char uch;      /* unsigned 8-bit value */
typedef unsigned short ush;     /* unsigned 16-bit value */
typedef unsigned long ulg;      /* unsigned 32-bit value */

/* internal file attribute */
#define UNKNOWN (-1)
#define BINARY  0
#define ASCII   1

#define BEST -1                 /* Use best method (deflation or store) */
#define STORE 0                 /* Store method */
#define DEFLATE 8               /* Deflation method*/

static int verbose=0;           /* Report oddities in zip file structure */
static int level=6;             /* Compression level */

/* Diagnostic functions */
#ifdef DEBUG
# ifdef MSDOS
#  undef  stderr
#  define stderr stdout
# endif
#  define diag(where) fprintf(stderr, "zip diagnostic: %s\n", where)
#  define Assert(cond,msg) {if(!(cond)) error(msg);}
#  define Trace(x) fprintf x
#  define Tracev(x) {if (verbose) fprintf x ;}
#  define Tracevv(x) {if (verbose>1) fprintf x ;}
#  define Tracec(c,x) {if (verbose && (c)) fprintf x ;}
#  define Tracecv(c,x) {if (verbose>1 && (c)) fprintf x ;}
#else
#  define diag(where)
#  define Assert(cond,msg)
#  define Trace(x)
#  define Tracev(x)
#  define Tracevv(x)
#  define Tracec(c,x)
#  define Tracecv(c,x)
#endif

#ifndef UTIL
        /* in deflate.c */
void Rio_lm_init OF((int pack_level, ush *flags));
void Rio_lm_free OF((void));
ulg  Rio_Deflate OF((void));

        /* in trees.c */
void Rio_ct_init     OF((ush *attr, int *method));
int  Rio_ct_tally    OF((int dist, int lc));
ulg  Rio_flush_block OF((char far *buf, ulg stored_len, int eof));

        /* in bits.c */
void     Rio_bi_init    OF((FILE *zipfile));
void     Rio_send_bits  OF((int value, int length));
unsigned Rio_bi_reverse OF((unsigned value, int length));
void     Rio_bi_windup  OF((void));
void     Rio_copy_block OF((char far *buf, unsigned len, int header));
int      Rio_seekable   OF((void));
extern   int (*Rio_read_buf) OF((char *buf, unsigned size));
ulg      Rio_memcompress OF((char *tgt, ulg tgtsize, char *src, ulg srcsize));
void     Rio_error      OF((char *h));

#endif /* !UTIL */

/* end of zip.h */

#endif
