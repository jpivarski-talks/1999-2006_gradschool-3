*
* $Id: lfctlcde.inc,v 1.2 2000/10/02 21:16:22 dpp Exp $
*
* $Log: lfctlcde.inc,v $
* Revision 1.2  2000/10/02 21:16:22  dpp
*     -> move Z0BIAS to tfctlcde
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:07  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/14 14/01/92  12.09.12  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.48.23  by  Steve Schaffner
*-- Author :
* LF cuts sequence
*
* lfctlcde.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/lfctldoc.inc"
#endif
C
      INTEGER         IFLGLF, MLRLLF, NDRPLF
      COMMON /LFCTLI/ IFLGLF, MLRLLF, NDRPLF
*
      REAL            GMINLF, SIGLLF,
     +                YHTCUT, SGYSLF, DIFSLF, YMXCUT,
     +                ERRHLF, ERRDLF,
     +                Y0MIN , Y0MIN1, Y0MIN2,
     +                Y0MAX , Y0MAX1, Y0MAX2, RLCHOP,
     +                CH2LF1, CH2LF2, CH2LFD, TCRLCT, QPKMCT, QMINCT,
     +                                        TCRLDC, QPKMDC, QMINDC
      COMMON /LFCTLR/ GMINLF, SIGLLF,
     +                YHTCUT, SGYSLF, DIFSLF, YMXCUT,
     +                ERRHLF, ERRDLF,
     +                Y0MIN , Y0MIN1, Y0MIN2,
     +                Y0MAX , Y0MAX1, Y0MAX2, RLCHOP,
     +                CH2LF1, CH2LF2, CH2LFD, TCRLCT, QPKMCT, QMINCT,
     +                                        TCRLDC, QPKMDC, QMINDC

      LOGICAL         LLFIND
      COMMON /LFCTLL/ LLFIND

* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"




