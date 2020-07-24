*
* $Id: cfextn.F,v 1.3 2000/04/26 14:12:31 dpp Exp $
*
* $Log: cfextn.F,v $
* Revision 1.3  2000/04/26 14:12:31  dpp
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*
* Revision 1.2  1999/09/12 22:29:43  dpp
* remove argument in call to CFRFT4
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.4  1996/02/06 00:09:17  zfiles
* move duplicated finish-up code to cfexfn
*
* Revision 1.3  1996/01/30 15:10:51  zfiles
* change in argument for CFLAST
*
* Revision 1.2  1996/01/24 22:20:24  zfiles
* success is now indicated by NFIT>0
* call CFRFT4, instead of CFCFIT; do not zero PHI6CF
*
* Revision 1.1.1.1  1994/10/08 01:00:54  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/16 08/10/92  12.45.14  by  Rob Kutschke
*CMZ :  5.54/14 28/09/92  20.31.53  by  Rob Kutschke
*CMZ :  5.53/07 15/08/90  18.40.53  by  D. Riley
*CMZ :  5.53/00 02/04/90  14.06.06  by  Steve Schaffner
*CMZ :  5.52/01 11/03/90  22.38.13  by  Steve Schaffner
*CMZ :  5.51/00 10/08/89  14.43.28  by  Steve Schaffner
*CMZ :  5.50/00 08/08/88  18.08.00  by  D. Peterson
*-- Author :
      SUBROUTINE CFEXTN

C     Extends the current track by growing it out layer
C     by layer into the rest of the detector.
C     This is used when things get desperate, and all other
C     possibilities have been used up.
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"

      INTEGER ILMAX,I,IAMB,IADR,IERROR,NFSV
      INTEGER NFTHLD,IBRHLD,NINHLD
      INTEGER ILYR,LLYR,MLYR,IBRNCH,NINBND,NGAPT,NGAPT1,NGAPT2

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

C-> RESTORE (X,Y) VALUES OF CURRENT HITS, AND FIND WHERE WE ARE SOFAR

      ITRKTF = 0
      ILMAX  = 0
      IF(NFIT.LE.0) GO TO 90
      DO 10 I=1,NFIT
      ILYR   = IPLFIT(I)
      IF (ILYR.GT.ILMAX) ILMAX = ILYR
      IAMB   = IADFIT(I)
      IADR   = IABS(IAMB  )
      DFIT  (I) = DWCD(IADR)*ISIGN(1,IAMB)
C roughly my eye, this is a yucko twist of reality
      SFIT  (I) = RTF(ILYR)
      WGTFIT(I) = WGTHTF(ILYR)
      CALL CFXYHT(IAMB,ILYR,0.,XFIT(I),YFIT(I),IERROR)
 10   CONTINUE

C-> ESTABLISH MINIMUM AND MAXIMUM LAYERS FOR SEARCH, SO GET EXTRA HITS
      LLYR   = MAX0(ILMAX+1,LCAXDR)
      MLYR   = MLYRCF
      IF (LLYR.GT.MLYR)GO TO 85

C-> SAVE NUMBER OF FITTED HITS,INITIALIZE LAYER NUMBER AND INBOUND INFO
      NFSV   = NFIT
      ILYR   = LLYR
      IBRNCH = 1
      NINBND = 0
      NGAPT  = 0
      NGAPT1 = 0
      NGAPT2 = 0

C-> GROW-THE-TRACK-THROUGH-THE-DETECTOR-ADDING-HITS
      CALL CFGROW(ILYR,LLYR,MLYR,
     1            IBRNCH,NINBND,NGAPT,NFSV,NGAPT1,NGAPT2)
      IF (NFIT.LE.NFSV)GO TO 85

C-> PUSH-THE-TRACK-ONTO-THE-STACK
      CALL CFEXFN(2)

C-> CHANGE AMBIGUITY ON SECOND NEW HIT AND GROW FROM THERE
C-> FLIP-THE-LAST-HIT-AND-GROW-TRACK-AGAIN
      NFTHLD = NFIT
      IBRHLD = IBRNCH
      NINHLD = NINBND

      IF((NFIT-NFSV).LT.2)GO TO 31
      NFIT   = NFSV + 2
      NGAPT  = NGAPT2
      IF(IBRHLD.EQ.1)GO TO 25
      NINBND = NINHLD - (NFTHLD-NFIT)
      IF(NINBND.GT.0)GO TO 25
      NINBND = 0
      IBRNCH = 1
25    CONTINUE
      CALL CFFLIP(ILYR,LLYR,MLYR,
     1            IBRNCH,NINBND,NGAPT,NFSV,NGAPT1,NGAPT2)

C-> CHANGE AMBIGUITY ON FIRST  NEW HIT AND GROW FROM THERE
C-> FLIP-THE-LAST-HIT-AND-GROW-TRACK-AGAIN
31    CONTINUE
      NFIT   = NFSV + 1
      NGAPT  = NGAPT1
      IF(IBRHLD.EQ.1)GO TO 35
      NINBND = NINHLD - (NFTHLD-NFIT)
      IF(NINBND.GT.0)GO TO 35
      NINBND = 0
      IBRNCH = 1
35    CONTINUE
      CALL CFFLIP(ILYR,LLYR,MLYR,
     1            IBRNCH,NINBND,NGAPT,NFSV,NGAPT1,NGAPT2)

C-> CHANGE AMBIGUITY ON SECOND NEW HIT AND GROW FROM THERE
C-> THIS IS THE SECOND NEW HIT WHEN THE FIRST NEW HIT WAS FLIPPED
C->  AND NOTHING WILL COME AFTER THIS SO REFILL NINBND AND NFIT
C-> FLIP-THE-LAST-HIT-AND-GROW-TRACK-AGAIN
      NFTHLD = NFIT
      IBRHLD = IBRNCH
      NINHLD = NINBND

      IF((NFIT-NFSV).LT.2)GO TO 51
      NFIT   = NFSV + 2
      NGAPT  = NGAPT2
      IF(IBRHLD.EQ.1)GO TO 45
      NINBND = NINHLD - (NFTHLD-NFIT)
      IF(NINBND.GT.0)GO TO 45
      NINBND = 0
      IBRNCH = 1
45    CONTINUE
      CALL CFFLIP(ILYR,LLYR,MLYR,
     1            IBRNCH,NINBND,NGAPT,NFSV,NGAPT1,NGAPT2)

51    CONTINUE
      GOTO 90

C-> Restore the seed track and return
85    CONTINUE
      CALL FIT_WHATEVER

C-> PUSH-THE-TRACK-ONTO-THE-STACK
      CALL CFEXFN(1)

C-> by definition, CFEXTN never fails.
90    RETURN
      END







