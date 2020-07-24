*
* $Id: tpc_pad_response.F,v 1.2 2003/05/12 16:08:55 dpp Exp $
*
* $Log: tpc_pad_response.F,v $
* Revision 1.2  2003/05/12 16:08:55  dpp
*      -> use CLEO_DOITTPC to dummy this routine in CLEO production
*
* Revision 1.1  2003/03/05 17:51:42  dpp
*      -> NEW ROUTINE
*      -> provide pads and realative pulse heights for
*         input layer number and phi of crossing
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TPC_PAD_RESPONSE(MODE,ILCD,PHI_INNER,PHI_OUTER,
     2     IADR,PH_FRAC,NEXT_CELL)
C.......................................................................
C.
C. TPC_PAD_RESPONSE - provide pad for input layer and phi
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 04-Nov-2002
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C Arguments

      INTEGER MODE
      INTEGER ILCD
      REAL PHI_INNER
      REAL PHI_OUTER
      INTEGER IADR
      REAL PH_FRAC
      INTEGER NEXT_CELL

#if defined(CLEO_DOITTPC)

#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfindpar.inc"
C which includes 
C               #include "cl3seq/cdgm3/cdgeompa.inc"
C               #include "doit/duseq/tfgeompa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"

#include "/home/axp/dpp/lcd_simulation/cornell/hep/lcd/io/fortran/lcdevt.inc"


      REAL PHIDIF,CELLCALC_R,CELLCALC_DIF
      INTEGER CELLCALC

      INTEGER REL_CELL(20)
      REAL TEST_PH,TEST_FRAC
      REAL REL_PH(20),TOT_PH
      INTEGER NCELL_FIND
      INTEGER ICELL

      INTEGER DIREC,ADD,REL_LOC
      LOGICAL KEEP_HIT

      REAL SIGNAL_SIGMA_PADS
      INTEGER CUTOFF_TOT_PADS
      REAL CUTOFF_FRAC_HT



      PARAMETER ( SIGNAL_SIGMA_PADS= 0.7 )
c      PARAMETER ( SIGNAL_SIGMA_PADS= .5 )
c      PARAMETER ( CUTOFF_TOT_PADS=    3 )
      PARAMETER ( CUTOFF_TOT_PADS=    9 )
      PARAMETER ( CUTOFF_FRAC_HT=    .002 )


c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

c      print 1000,MODE,ILCD,PHI_INNER,PHI_OUTER
 1000 format(' TPC_PAD_RESPONSE: enter, MODE=',I2,
     2     '  ILCD=',I4,'  phi=',2f8.3)


      IF(MODE.EQ.0)THEN
        
        PHIDIF=PHI_INNER-PHIFCD(ILCD)
        IF(PHIDIF.LT.0.)   PHIDIF=PHIDIF+twopi
        IF(PHIDIF.GE.twopi)PHIDIF=PHIDIF-twopi
        
        CELLCALC_R=PHIDIF/CELLCD(ILCD)
        CELLCALC=CELLCALC_R+.5
        CELLCALC_DIF=CELLCALC_R-CELLCALC

        
        NCELL_FIND=0
        TOT_PH=0.
        DIREC=1
        IF(CELLCALC_DIF.LT.0.)DIREC=-1
        ADD=1
        REL_LOC=0

 11     CONTINUE

        TEST_PH=EXP(-(
     2       (CELLCALC_R-(CELLCALC+REL_LOC))/
     3       SIGNAL_SIGMA_PADS
     4       )**2)

        KEEP_HIT=.FALSE.
        IF(REL_LOC.EQ.0)KEEP_HIT=.TRUE.
        IF(.NOT.KEEP_HIT)THEN
          TEST_FRAC=TEST_PH/REL_PH(1)
          IF(TEST_FRAC.GE.CUTOFF_FRAC_HT)THEN
            KEEP_HIT=.TRUE.
          ENDIF
        ENDIF

c        print 1011,cellcalc_r,cellcalc,cellcalc_dif,
c     2       ncell_find,direc,add,rel_loc,test_ph,keep_hit
 1011   format(' TPC_PAD_RESPONSE:',
     1       ' cell R,I,dif=',F7.2,I5,F7.2,
     2       '  ncell_find=',I2,
     3       '  direc=',I2,'  add=',I3,'  rel_loc=',I3,
     4       '  test_ph=',F9.6,'  keep_hit=',L1)


        IF(KEEP_HIT)THEN
          NCELL_FIND=NCELL_FIND+1
          REL_CELL(NCELL_FIND)=REL_LOC
          REL_PH(NCELL_FIND)=TEST_PH
          TOT_PH=TOT_PH+REL_PH(NCELL_FIND)

          IF(NCELL_FIND.LT.CUTOFF_TOT_PADS)THEN
            REL_LOC=REL_LOC+DIREC*ADD
            DIREC=-DIREC
            ADD=ADD+1
            GO TO 11
          ENDIF
        ENDIF

        IF(NCELL_FIND.GE.1)THEN
          DO 23 ICELL=1,NCELL_FIND
            REL_PH(ICELL)=REL_PH(ICELL)/TOT_PH
 23       CONTINUE
          NEXT_CELL=1

        ELSE
          NEXT_CELL=0
        ENDIF
        
      ELSEIF(MODE.EQ.1)THEN

        IF(NEXT_CELL.NE.0)THEN
          IADR=CELLCALC+REL_CELL(NEXT_CELL)
          IF(IADR.GT.NWIRCD(ILCD))THEN
            IADR=IADR-NWIRCD(ILCD)
          ENDIF
          IADR=IADR+INDXCD(ILCD)
          PH_FRAC=REL_PH(NEXT_CELL)

          IF(NEXT_CELL.LT.NCELL_FIND)THEN
            NEXT_CELL=NEXT_CELL+1
          ELSE
            NEXT_CELL=0
          ENDIF

        ENDIF

      ENDIF

C end of compliation flag CLEO_DOITTPC
#endif

      RETURN
      END




