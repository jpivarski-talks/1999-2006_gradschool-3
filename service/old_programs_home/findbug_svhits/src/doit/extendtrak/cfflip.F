*
* $Id: cfflip.F,v 1.3 2000/04/26 14:12:08 dpp Exp $
*
* $Log: cfflip.F,v $
* Revision 1.3  2000/04/26 14:12:08  dpp
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*
* Revision 1.2  1999/09/12 22:29:57  dpp
* remove argument in call to CFRFT4
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.4  1996/02/06 00:08:41  zfiles
* move duplicated finish-up code to cfexfn
*
* Revision 1.3  1996/01/30 15:10:49  zfiles
* change in argument for CFLAST
*
* Revision 1.2  1996/01/24 22:21:38  zfiles
* call CFRFT4 instead of CFCFIT; do not zero PHI6CF
*
* Revision 1.1.1.1  1994/10/08 01:00:54  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/16 08/10/92  12.45.45  by  Rob Kutschke
*CMZ :  5.54/14 28/09/92  20.32.12  by  Rob Kutschke
*CMZ :  5.53/07 15/08/90  16.33.03  by  D. Riley
*CMZ :  5.53/00 02/04/90  14.06.06  by  Steve Schaffner
*CMZ :  5.52/01 11/03/90  22.38.13  by  Steve Schaffner
*CMZ :  5.51/00 10/08/89  14.43.28  by  Steve Schaffner
*CMZ :  5.50/00 08/08/88  18.08.00  by  D. Peterson
*-- Author :
      SUBROUTINE CFFLIP(ILYR,LLYR,MLYR,
     1                  IBRNCH,NINBND,NGAPT,NFSV,NGAPT1,NGAPT2)
C
C  TO FLIP-THE-LAST-HIT-AND-GROW-TRACK-AGAIN
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

      INTEGER ILYR,IADR,K,LLYR,MLYR
      INTEGER IBRNCH,NINBND,NGAPT,NFSV,NGAPT1,NGAPT2
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      ILYR   = IPLFIT(NFIT) + IBRNCH

C-> FLIP-THE-AMBIGUITY-OF-HIT-NFIT-AND-REFIT
      IADR   = IABS(IADFIT(NFIT))
      IADFIT(NFIT) = -IADFIT(NFIT)
      DFIT  (NFIT) = -DFIT(NFIT)
      CALL CFXYHT(IADFIT(NFIT),IPLFIT(NFIT),0.,XFIT(NFIT),YFIT(NFIT),K)
      CALL FIT_WHATEVER

C-> GROW-THE-TRACK-THROUGH-THE-DETECTOR-ADDING-HITS
      CALL CFGROW(ILYR,LLYR,MLYR,
     1            IBRNCH,NINBND,NGAPT,NFSV,NGAPT1,NGAPT2)

C-> PUSH-THE-TRACK-ONTO-THE-STACK
      CALL CFEXFN(3)

      RETURN
      END

