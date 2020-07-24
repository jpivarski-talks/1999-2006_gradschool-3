/* @(#)root/zip:$Name: OpenScientist-v10r0p6 $:$Id: Bits.c,v 1.5 2002/09/02 14:18:36 barrand Exp $ */
/* Author: */
/*

 Copyright (C) 1990-1993 Mark Adler, Richard B. Wales, Jean-loup Gailly,
 Kai Uwe Rommel and Igor Mandrichenko.
 Permission is granted to any individual or institution to use, copy, or
 redistribute this software so long as all of the original files are included,
 that it is not sold for profit, and that this copyright notice is retained.

*/

/*
 *  bits.c by Jean-loup Gailly and Kai Uwe Rommel.
 *
 *  This is a new version of im_bits.c originally written by Richard B. Wales
 *
 *  PURPOSE
 *
 *      Output variable-length bit strings. Compression can be done
 *      to a file or to memory.
 *
 *  DISCUSSION
 *
 *      The PKZIP "deflate" file format interprets compressed file data
 *      as a sequence of bits.  Multi-bit strings in the file may cross
 *      byte boundaries without restriction.
 *
 *      The first bit of each byte is the low-order bit.
 *
 *      The routines in this file allow a variable-length bit value to
 *      be output right-to-left (useful for literal values). For
 *      left-to-right output (useful for code strings from the tree routines),
 *      the bits must have been reversed first with R__bi_reverse().
 *
 *      For in-memory compression, the compressed bit stream goes directly
 *      into the requested output buffer. The input data is read in blocks
 *      by the R__mem_read() function. The buffer is limited to 64K on 16 bit
 *      machines.
 *
 *  INTERFACE
 *
 *      void R__bi_init (FILE *zipfile)
 *          Initialize the bit string routines.
 *
 *      void R__send_bits (int value, int length)
 *          Write out a bit string, taking the source bits right to
 *          left.
 *
 *      int R__bi_reverse (int value, int length)
 *          Reverse the bits of a bit string, taking the source bits left to
 *          right and emitting them right to left.
 *
 *      void R__bi_windup (void)
 *          Write out any remaining bits in an incomplete byte.
 *
 *      void R__copy_block(char far *buf, unsigned len, int header)
 *          Copy a stored block to the zip file, storing first the length and
 *          its one's complement if requested.
 *
 *      int R__seekable(void)
 *          Return true if the zip file can be seeked.
 *
 *      ulg R__memcompress (char *tgt, ulg tgtsize, char *src, ulg srcsize);
 *          Compress the source buffer src into the target buffer tgt.
 */

/* #include "zip.h" */
#include <Rio/zip/ZIP.h>

extern ulg Rio_window_size; /* size of sliding window */


/* ===========================================================================
 *  Prototypes for local functions
 */
local int  Rio_mem_read     OF((char *buf, unsigned size));
local void Rio_flush_outbuf OF((unsigned w, unsigned size));


/* ===========================================================================
 * Local data used by the "bit string" routines.
 */
local FILE *zfile; /* output zip file */

local unsigned short bi_buf;
/* Output buffer. bits are inserted starting at the bottom (least significant
 * bits).
 */

#define Buf_size (8 * 2*sizeof(char))
/* Number of bits used within bi_buf. (bi_buf might be implemented on
 * more than 16 bits on some systems.)
 */

local int bi_valid;
/* Number of valid bits in bi_buf.  All bits above the last valid bit
 * are always zero.
 */

local char *in_buf, *out_buf;
/* Current input and output buffers. in_buf is used only for in-memory
 * compression.
 */

local unsigned in_offset, out_offset;
/* Current offset in input and output buffers. in_offset is used only for
 * in-memory compression. On 16 bit machiens, the buffer is limited to 64K.
 */

local unsigned in_size, out_size;
/* Size of current input and output buffers */

int (*Rio_read_buf) OF((char *buf, unsigned size)) = Rio_mem_read;
/* Current input function. Set to Rio_mem_read for in-memory compression */

#ifdef DEBUG
ulg Rio_bits_sent;   /* bit length of the compressed data */
#endif

/* Output a 16 bit value to the bit stream, lower (oldest) byte first */
#define PUTSHORT(w) \
{ if (out_offset < out_size-1) { \
    out_buf[out_offset++] = (char) ((w) & 0xff); \
    out_buf[out_offset++] = (char) ((ush)(w) >> 8); \
  } else { \
    Rio_flush_outbuf((w),2); \
  } \
}

#define PUTBYTE(b) \
{ if (out_offset < out_size) { \
    out_buf[out_offset++] = (char) (b); \
  } else { \
    Rio_flush_outbuf((b),1); \
  } \
}


/* ===========================================================================
 *  Prototypes for local functions
 */
local int  Rio_mem_read     OF((char *b,    unsigned bsize));
local void Rio_flush_outbuf OF((unsigned w, unsigned bytes));

/* ===========================================================================
 * Initialize the bit string routines.
 */
void Rio_bi_init (FILE *zipfile)
    /* FILE *zipfile;   output zip file, NULL for in-memory compression */
{
    zfile  = zipfile;
    bi_buf = 0;
    bi_valid = 0;
#ifdef DEBUG
    Rio_bits_sent = 0L;
#endif
}

/* ===========================================================================
 * Send a value on a given number of bits.
 * IN assertion: length <= 16 and value fits in length bits.
 */
void Rio_send_bits(int value, int length)
    /* int value;   value to send */
    /* int length;  number of bits */
{
#ifdef DEBUG
    Tracevv((stderr," l %2d v %4x ", length, value));
    Assert(length > 0 && length <= 15, "invalid length");
    Rio_bits_sent += (ulg)length;
#endif
    /* If not enough room in bi_buf, use (valid) bits from bi_buf and
     * (16 - bi_valid) bits from value, leaving (width - (16-bi_valid))
     * unused bits in value.
     */
    if (bi_valid > (int)Buf_size - length) {
        bi_buf |= (value << bi_valid);
        PUTSHORT(bi_buf);
        bi_buf = (ush)value >> (Buf_size - bi_valid);
        bi_valid += length - Buf_size;
    } else {
        bi_buf |= value << bi_valid;
        bi_valid += length;
    }
}

/* ===========================================================================
 * Reverse the first len bits of a code, using straightforward code (a faster
 * method would use a table)
 * IN assertion: 1 <= len <= 15
 */
unsigned Rio_bi_reverse(unsigned code, int len)
    /* unsigned code;  the value to invert */
    /* int len;        its bit length */
{
    register unsigned res = 0;
    do {
        res |= code & 1;
        code >>= 1, res <<= 1;
    } while (--len > 0);
    return res >> 1;
}

/* ===========================================================================
 * Flush the current output buffer.
 */
local void Rio_flush_outbuf(unsigned w, unsigned bytes)
    /* unsigned w;      value to flush */
    /* unsigned bytes;  number of bytes to flush (0, 1 or 2) */
{
    Rio_error("output buffer too small for in-memory compression");

    /* Encrypt and write the output buffer: */
    out_offset = 0;
    if (bytes == 2) {
        PUTSHORT(w);
    } else if (bytes == 1) {
        out_buf[out_offset++] = (char) (w & 0xff);
    }
}

/* ===========================================================================
 * Write out any remaining bits in an incomplete byte.
 */
void Rio_bi_windup()
{
    if (bi_valid > 8) {
        PUTSHORT(bi_buf);
    } else if (bi_valid > 0) {
        PUTBYTE(bi_buf);
    }
    if (zfile != (FILE *) NULL) {
        Rio_flush_outbuf(0, 0);
    }
    bi_buf = 0;
    bi_valid = 0;
#ifdef DEBUG
    Rio_bits_sent = (Rio_bits_sent+7) & ~7;
#endif
}

/* ===========================================================================
 * Copy a stored block to the zip file, storing first the length and its
 * one's complement if requested.
 */
void Rio_copy_block(char far *buf, unsigned len, int header)
    /* char far *buf;  the input data */
    /* unsigned len;   its length */
    /* int header;     true if block header must be written */
{
    Rio_bi_windup();              /* align on byte boundary */

    if (header) {
        PUTSHORT((ush)len);
        PUTSHORT((ush)~len);
#ifdef DEBUG
        Rio_bits_sent += 2*16;
#endif
    }
    if (out_offset + len > out_size) {
printf("out_offset=%d, len=%d, out_size=%d\n",out_offset,len,out_size);
        Rio_error("output buffer too small for in-memory compression");
    } else {
        memcpy(out_buf + out_offset, buf, len);
        out_offset += len;
    }
#ifdef DEBUG
    Rio_bits_sent += (ulg)len<<3;
#endif
}


/* ===========================================================================
 * Return true if the zip file can be seeked. This is used to check if
 * the local header can be re-rewritten. This function always returns
 * true for in-memory compression.
 * IN assertion: the local header has already been written (ftell() > 0).
 */
int Rio_seekable()
{
#if 0
    return (zfile == NULL ||
            (fseek(zfile, -1L, SEEK_CUR) == 0 &&
             fseek(zfile,  1L, SEEK_CUR) == 0));
#endif

    return (0);
}

/* ===========================================================================
 * In-memory compression. This version can be used only if the entire input
 * fits in one memory buffer. The compression is then done in a single
 * call of Rio_memcompress(). (An extension to allow repeated calls would be
 * possible but is not needed here.)
 * The first two bytes of the compressed output are set to a short with the
 * method used (DEFLATE or STORE). The following four bytes contain the CRC.
 * The values are stored in little-endian order on all machines.
 * This function returns the byte size of the compressed output, including
 * the first six bytes (method and crc).
 */

ulg Rio_memcompress(char *tgt, ulg tgtsize, char *src, ulg srcsize)
    /* char *tgt, *src;        target and source buffers */
    /* ulg tgtsize, srcsize;   target and source sizes */
{
    ush att      = (ush)UNKNOWN;
    ush flags    = 0;
    ulg crc      = 0;
    int method   = DEFLATE;

    if (tgtsize <= 6L) Rio_error("target buffer too small");
#if 0
    crc = updcrc((char *)NULL, 0);
    crc = updcrc(src, (extent) srcsize);
#endif
    Rio_read_buf  = Rio_mem_read;
    in_buf    = src;
    in_size   = (unsigned)srcsize;
    in_offset = 0;

    out_buf    = tgt;
    out_size   = (unsigned)tgtsize;
    out_offset = 2 + 4;
    Rio_window_size = 0L;

    Rio_bi_init((FILE *)NULL);
    Rio_ct_init(&att, &method);
    Rio_lm_init((level != 0 ? level : 1), &flags);
    Rio_Deflate();
    Rio_window_size = 0L; /* was updated by lm_init() */

    /* For portability, force little-endian order on all machines: */
    tgt[0] = (char)(method & 0xff);
    tgt[1] = (char)((method >> 8) & 0xff);
    tgt[2] = (char)(crc & 0xff);
    tgt[3] = (char)((crc >> 8) & 0xff);
    tgt[4] = (char)((crc >> 16) & 0xff);
    tgt[5] = (char)((crc >> 24) & 0xff);

    return (ulg)out_offset;
}

/* ===========================================================================
 * In-memory read function. As opposed to file_read(), this function
 * does not perform end-of-line translation, and does not update the
 * crc and input size.
 *    Note that the size of the entire input buffer is an unsigned long,
 * but the size used in Rio_mem_read() is only an unsigned int. This makes a
 * difference on 16 bit machines. Rio_mem_read() may be called several
 * times for an in-memory compression.
 */
local int Rio_mem_read(char *b, unsigned bsize)
{
    if (in_offset < in_size) {
        ulg block_size = in_size - in_offset;
        if (block_size > (ulg)bsize) block_size = (ulg)bsize;
        memcpy(b, in_buf + in_offset, (unsigned)block_size);
        in_offset += (unsigned)block_size;
        return (int)block_size;
    } else {
        return 0; /* end of input */
    }
}

/***********************************************************************
 *                                                                     *
 * Name: Rio_zip                                      Date:    20.01.95 *
 * Author: E.Chernyaev (IHEP/Protvino)               Revised:          *
 *                                                                     *
 * Function: In memory ZIP compression                                 *
 *           It's a variant of Rio_memcompress adopted to be issued from*
 *           FORTRAN. Written for DELPHI collaboration (CERN)          *
 *                                                                     *
 * Input: cxlevel - compression level                                  *
 *        scrsize - size of input buffer                               *
 *        src     - input buffer                                       *
 *        tgtsize - size of target buffer                              *
 *                                                                     *
 * Output: tgt - target buffer (compressed)                            *
 *         irep - size of compressed data (0 - if error)               *
 *                                                                     *
 ***********************************************************************/
#define HDRSIZE 9
static  int error_flag;

#ifdef __cplusplus
extern "C" { void Rio_zip(int,int*,char*,int*,char*,int*);}
#endif

void Rio_zip(int cxlevel, int *srcsize, char *src, int *tgtsize, char *tgt, int *irep)
     /* int cxlevel;                      compression level */
     /* int  *srcsize, *tgtsize, *irep;   source and target sizes, replay */
     /* char *tgt, *src;                  source and target buffers */

{
  ush att      = (ush)UNKNOWN;
  ush flags    = 0;
  int method   = DEFLATE;
  level        = cxlevel;

  *irep        = 0;
  error_flag   = 0;
  if (*tgtsize <= HDRSIZE) Rio_error("target buffer too small");
  if (error_flag != 0) return;
  if (*srcsize > 0xffffff) Rio_error("source buffer too big");
  if (error_flag != 0) return;

  Rio_read_buf  = Rio_mem_read;
  in_buf    = src;
  in_size   = (unsigned) (*srcsize);
  in_offset = 0;

  out_buf     = tgt;
  out_size    = (unsigned) (*tgtsize);
  out_offset  = HDRSIZE;
  Rio_window_size = 0L;

  Rio_bi_init((FILE *)NULL);      /* initialize bit routines */
  if (error_flag != 0) return;
  Rio_ct_init(&att, &method);     /* initialize tree routines */
  if (error_flag != 0) return;
  Rio_lm_init(level, &flags);     /* initialize compression */
  if (error_flag != 0) return;
  Rio_Deflate();                  /* compress data */
  if (error_flag != 0) return;

  tgt[0] = 'C';               /* Signature 'C'-Chernyaev, 'S'-Smirnov */
  tgt[1] = 'S';
  tgt[2] = (char) method;

  out_size  = out_offset - HDRSIZE;         /* compressed size */
  tgt[3] = (char)(out_size & 0xff);
  tgt[4] = (char)((out_size >> 8) & 0xff);
  tgt[5] = (char)((out_size >> 16) & 0xff);

  tgt[6] = (char)(in_size & 0xff);         /* decompressed size */
  tgt[7] = (char)((in_size >> 8) & 0xff);
  tgt[8] = (char)((in_size >> 16) & 0xff);

  *irep     = out_offset;
  return;
}

void Rio_error(char *msg)
{
  fprintf(stderr,"Rio_zip: %s\n",msg);
  error_flag = 1;
  if (verbose) { }  /*use verbose to avoid warning*/
}

