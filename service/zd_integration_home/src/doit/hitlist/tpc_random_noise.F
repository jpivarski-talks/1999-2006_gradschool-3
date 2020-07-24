*
* $Id: tpc_random_noise.F,v 1.3 2003/05/12 16:08:05 dpp Exp $
*
* $Log: tpc_random_noise.F,v $
* Revision 1.3  2003/05/12 16:08:05  dpp
*      -> use CLEO_DOITTPC to dummy this routine in CLEO production
*
* Revision 1.2  2003/03/06 17:00:00  dpp
*      -> changed compile switch from CLEO_SUNOS to CLEO_SunOS
*
* Revision 1.1  2003/03/05 17:53:12  dpp
*      -> NEW ROUTINE
*      -> tpc specific hit source
*      -> use compile switch for the declaration of RAN
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author : D. Peterson jan 14,2002
      SUBROUTINE TPC_RANDOM_NOISE
#if defined(CLEO_DOITTPC)

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"

#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfextra.inc"


      INTEGER IDUM
      INTEGER ILCD
      REAL    AREA
      REAL    AREAPERHIT
      INTEGER NHITSADD
      INTEGER ITRY
      REAL    Z_HIT
      REAL    Z_WIDTH
      REAL    PH_HIT
      REAL    PHITMP
      REAL    PHIDIF
      REAL    CELLCALC_R
      REAL    CELLCALC_DIF
      INTEGER CELLCALC
      INTEGER IADR

C to compile in solaris, must be deleted in OSF
#if defined(CLEO_SunOS)
      REAL RAN(2)
#endif

C note: will require stripping out the added hits,
C before CDTRACK, to allow running more than 1 event at a time


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


      DO 495 ILCD=1,NLYRCD
        print 1011,ilcd,idvccd(ilcd),EvntHit_Num
 1011   format(' TPC_RANDOM_HIT: layer=',I5,
     2       ' idvccd=',I4,' EvntHit_Num=',I8)

        IF(IDVCCD(ILCD).EQ.ITPC1)THEN

C surface area of a layer (*2 to account for two ends)
          AREA=twopi*RCD(ILCD)*ZENDCD(ILCD)*2.

C area per random noise hit: width*length / occupancy
c          AREAPERHIT=.01*.02 / .0004
c          AREAPERHIT=.01*.02 / .001
          AREAPERHIT=.01*.02 / .003

          NHITSADD=AREA/AREAPERHIT
          IF(NHITSADD.GE.1)THEN

C loop over hits
            DO 99 ITRY=1,NHITSADD

C select z position
              Z_HIT=(1.-2.*RAN(IDUM))*ZENDCD(ILCD)
              Z_WIDTH=.02

C select pulse height (1000 is the ph for min ionizing)
              PH_HIT=2.*1000.*RAN(IDUM)

C select phi and wire address
              PHITMP=RAN(IDUM)*TWOPI

              PHIDIF=PHITMP-PHIFCD(ILCD)
              IF(PHIDIF.LT.0.)   PHIDIF=PHIDIF+twopi
              IF(PHIDIF.GE.twopi)PHIDIF=PHIDIF-twopi
              
              CELLCALC_R=PHIDIF/CELLCD(ILCD)
              CELLCALC=CELLCALC_R+.5
              CELLCALC_DIF=CELLCALC_R-CELLCALC
              IF(CELLCALC.GT.NWIRCD(ILCD))CELLCALC=
     2             CELLCALC-NWIRCD(ILCD)
              IADR=CELLCALC+INDXCD(ILCD)
              


C add the hit
              CALL TPC_ADD_HIT(ILCD,IADR,Z_HIT,Z_WIDTH,PH_HIT)
 99         CONTINUE
          ENDIF
        ENDIF
 495  CONTINUE

C end of compliation flag CLEO_DOITTPC
#endif

      RETURN
      END









