*
* $Id: tffitctl.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tffitctl.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:10  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/14 28/02/94  13.25.14  by  Dan Peterson
*-- Author :
* TF final fitter control
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/tffitcdo.inc"
#endif
C
      LOGICAL         LCALIB,DUALTR,LKN6FI,LRMVHT,LKN7CT,LADVDE
      LOGICAL         FITFLT
      INTEGER         USEFIT
C
      COMMON /TFFITC/ LCALIB,DUALTR,LRMVHT,LKN6FI,LKN7CT,LADVDE,
     +                FITFLT(NP3DTF),
     +                USEFIT(KLR2TF)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
