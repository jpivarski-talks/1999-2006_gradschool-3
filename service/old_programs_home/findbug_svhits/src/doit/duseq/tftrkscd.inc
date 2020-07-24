*
* $Id: tftrkscd.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tftrkscd.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:09  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/14 06/01/92  22.02.03  by  Steve Schaffner
*CMZ :  5.53/06 30/07/90  17.59.34  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  19.01.16  by  Steve Schaffner
*-- Author :
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/tftrksdo.inc"
#endif
C
      INTEGER         NTRK,IFTRK         ,ILTRK         ,
     +                     NXTTRK        ,
     +                     LLKTRK        ,MLKTRK        ,
     +                NLKT,ILKTRK
*
      REAL  ATFTRK        ,BTFTRK        ,GTFTRK        ,CTFTRK        ,
     +                     CHITRK        ,CHATRK
*
      COMMON /TFTRKS/ NTRK,IFTRK (KLYRDM),ILTRK (KLYRDM),
     +                     NXTTRK(NTRKDM),
     +                     LLKTRK(NTRKDM),MLKTRK(NTRKDM),
     +                NLKT,ILKTRK(NLKTDM),
     +      ATFTRK(NTRKDM),BTFTRK(NTRKDM),GTFTRK(NTRKDM),CTFTRK(NTRKDM),
     +                     CHITRK(NTRKDM),CHATRK(NTRKDM)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
