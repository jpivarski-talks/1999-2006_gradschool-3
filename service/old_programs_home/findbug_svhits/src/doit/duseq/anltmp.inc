*
* $Id: anltmp.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: anltmp.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:06  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/07 21/05/91  14.42.17  by  Steve Schaffner
*CMZ :  5.53/07 12/10/90  17.46.15  by  Steve Schaffner
*CMZ :  5.53/06 01/08/90  11.01.08  by  Steve Schaffner
*CMZ :  5.52/00 24/01/90  14.41.42  by  Steve Schaffner
*-- Author :
*  Temp. common for analyzed data.
*DBDR(MTRKDR)  Signed impact parameter w/r/t beam spot
C*IPLRDR(IHIT,ITRKDR) -- Physical layer of IHIT'th hit - removed 7/30/90 sfs
C*IHWRDR -- (+-) address of used hit -removed 7/30/90 sfs
*    the preceeding two are included in case PUTDH is ever activated

      INTEGER KBITDR
C     INTEGER IPLRDR,IHWRDR
      REAL DBDR
      COMMON/ANLTMP/KBITDR(MTRKDR,MBITDR),DBDR(MTRKDR)
C     + IPLRDR(KLR2TF,MTRKDR),IHWRDR(KLR2TF,MTRKDR)

* ------------------------------------------
#include "doit/duseq/duseqd.inc"