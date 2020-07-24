*
* $Id: cfgrow.F,v 1.4 2000/08/18 19:07:13 dpp Exp $
*
* $Log: cfgrow.F,v $
* Revision 1.4  2000/08/18 19:07:13  dpp
*      -> change argument in call to TFPHTR to RKN6CF
*
* Revision 1.3  2000/04/26 14:11:39  dpp
*    -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*
* Revision 1.2  1999/09/12 22:30:05  dpp
* remove argument in call to CFRFT4
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/01/24 22:18:11  zfiles
* argument in call to TFPHTR
* call CFRFT4 instead of CFRFIT; do not zero PHI6CF
*
* Revision 1.1.1.1  1994/10/08 01:00:54  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/10 18/12/91  20.10.38  by  Dan Peterson
*CMZ :  5.53/07 15/08/90  18.40.53  by  D. Riley
*-- Author :
      SUBROUTINE CFGROW(ILYR,LLYR,MLYR,
     1                  IBRNCH,NINBND,NGAPT,NFSV,NGAPT1,NGAPT2)
C
C  TO GROW-THE-TRACK-THROUGH-THE-DETECTOR-ADDING-HITS
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

      INTEGER ILYR,LLYR,MLYR,IBRNCH,NINBND,NGAPT
      INTEGER NFSV,NGAPT1,NGAPT2
      INTEGER NSER,MINBND
      INTEGER NGAP,MGAP,MGAPT,MGAPO,MGAPTO,MGAPI,MGAPTI
      INTEGER NFITL,NHW,IER,NWIR,IWIRE,ISER,IWADR
      INTEGER ISAVE,IHIT,IADR,IERROR
      REAL XT,YT,PHIT,ARCFUN,DELMAX,DEL,DELI

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      NSER   = 1
      MGAPO  = 1
      MGAPTO = 3
      MGAPI  = 0
      MGAPTI = 1
      MINBND = 3

      IF(IBRNCH.EQ. 1)GO TO 15
      IF(IBRNCH.EQ.-1)GO TO 16
      GO TO 90

15    MGAP   = MGAPO
      MGAPT  = MGAPTO
      GO TO 17
16    MGAP   = MGAPI
      MGAPT  = MGAPTI

17    NHIT   = 0
      NLYRHT = 0
      NGAP   = 0

20    CONTINUE

C test that it is valid to look for a hit in this layer

      IF(ILYR  .LT.LLYR  )GO TO 90
      IF(ILYR  .GT.MLYR  )GO TO 90
      IF(NFIT  .GE.NLYRCF)GO TO 90
      IF(NGAP  .GT.MGAP  )GO TO 90
      IF(NGAPT .GT.MGAPT )GO TO 90
      IF(NINBND.GE.MINBND)GO TO 90
      NFITL  = NFIT
      NHW    = 0

C locate projected crossing point and check for "top of curl"

      CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     +            RSQTF(ILYR),IBRNCH,XT,YT,PHIT,IER)
      IF(IER.EQ.0)GO TO 23

C transfer to the inbound branch; do not increment NGAP as in old

      IBRNCH = -1
      MGAP   = MGAPI
      MGAPT  = MGAPTI
      GO TO 61

C find the hit wires in this layer within the specified road

23    NWIR   = NWIRTF(ILYR)
      IWIRE  = NINT((PHIT-PHIFTF(ILYR))/CELLTF(ILYR)) - NSER - 1

      DO 25 ISER=-NSER,NSER
      IWIRE  = IWIRE + 1
      IF(IWIRE.LT.0)      IWIRE = IWIRE + NWIR
      IF(IWIRE.GE.NWIR)   IWIRE = IWIRE - NWIR
      IWADR  = INDXTF(ILYR) + IWIRE
      IF(IADRHW(IWADR).LE.0)GO TO 25
      IF(NHIT.GE.NHITDM)GO TO 25
      NHW    = NHW  + 1
      NHIT   = NHIT + 1
      IADRHT(NHIT) = IADRHW(IWADR)
25    CONTINUE

      IF(NHW.LE.0)GO TO 45

C found a hit wire; set up to look for acceptable hits

      NLYRHT = NLYRHT + 1
      IPLRLR(NLYRHT) = ILYR
      LHITLR(NLYRHT) = NHIT - NHW + 1
      MHITLR(NLYRHT) = NHIT
      SLYRLR(NLYRHT) = ARCFUN(KAPCF,ALPCF,BETCF,XT,YT,XICF)

C find the hit closest to the track

      DELMAX = CELLTF(ILYR)*RTF(ILYR)
      ISAVE  = 0
      DO 40 IHIT=LHITLR(NLYRHT),MHITLR(NLYRHT)
      IADR   = IABS(IADRHT(IHIT))
      DEL    = KAPCF*RSQTF(ILYR)+ALPCF*XWCD(IADR)
     +                +BETCF*YWCD(IADR)+GAMCF
      DELI   = ABS(ABS(DEL) - DWCD(IADR))
      IF (DELI.GE.DELMAX)GO TO 40
      DELMAX = DELI
      ISAVE  = IADR
      IF(DEL.LT.0) ISAVE = -ISAVE
40    CONTINUE

C put the best (if it exists) hit into "FIT" arrays

      IF(ISAVE.EQ.0)GO TO 45
      NFIT   = NFIT + 1
      IADR   = IABS(ISAVE)
      IADFIT(NFIT) = ISAVE
      IPLFIT(NFIT) = ILYR
      WGTFIT(NFIT) = WGTHTF(ILYR)
      DFIT  (NFIT) = DWCD(IADR)*ISIGN(1,ISAVE)
      SFIT  (NFIT) = SLYRLR(NLYRHT)
      CALL CFXYHT(ISAVE,ILYR,0.,XFIT(NFIT),YFIT(NFIT),IERROR)

C branch point for no wires or good hits found

45    IF (NFIT.LE.NFITL)GO TO 55

C refit the track if hits were added

      CALL FIT_WHATEVER
      NFITL  = NFIT
      NGAP   = 0
      IF((NFIT-NFSV).EQ.1)NGAPT1=NGAPT
      IF((NFIT-NFSV).EQ.2)NGAPT2=NGAPT
      IF(IBRNCH.EQ.1)GO TO 47
      NINBND=NINBND+1
47    CONTINUE
      GO TO 61

C record gap length if no hit added

55    NGAP = NGAP + 1
      NGAPT= NGAPT+ 1

C go to next layer and continue

61    ILYR   = ILYR + IBRNCH
      GO TO 20

90    CONTINUE
      RETURN
      END







