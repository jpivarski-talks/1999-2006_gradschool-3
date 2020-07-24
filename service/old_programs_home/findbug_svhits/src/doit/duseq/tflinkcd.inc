*
* $Id: tflinkcd.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tflinkcd.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:09  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/07 17/05/91  19.19.41  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.33.48  by  Steve Schaffner
*-- Author :
*
* tflinkcd.inc
*
*IF=-NOCOMDOC.
#include "doit/duseq/tflinkdo.inc"
C
      INTEGER         NLNK, LLNKTP        , MLNKTP        ,
     +                      IIHLNK        , IOHLNK        ,
     +                      ITPLNK        ,
     +                      IFRUT         , ILRUT         ,
     +                      IFBRN         , ILBRN         ,
     +                NLMB, NXTLMB        , ILKLMB
      REAL                  ALNK          , BLNK
*
      COMMON /TFLINK/ NLNK, LLNKTP(NTYPDM), MLNKTP(NTYPDM),
     +                      IIHLNK(NLNKDM), IOHLNK(NLNKDM),
     +                      ITPLNK(NLNKDM),
     +                      ALNK  (NLNKDM), BLNK  (NLNKDM),
     +                      IFRUT (NLNKDM), ILRUT (NLNKDM),
     +                      IFBRN (NLNKDM), ILBRN (NLNKDM),
     +                NLMB, NXTLMB(NLMBDM), ILKLMB(NLMBDM)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
