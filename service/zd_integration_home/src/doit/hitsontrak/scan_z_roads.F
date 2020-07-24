* $Id: scan_z_roads.F,v 1.12 2003/03/05 18:01:27 dpp Exp $
*
* $Log: scan_z_roads.F,v $
* Revision 1.12  2003/03/05 18:01:27  dpp
*      -> changed all variable names in cdscrtcd to have common root
*      -> (part of the 06-Dec-2002 modification)
*          require SZ status for all hits tested, not just first in layer
*          this was a long standing error
*
* Revision 1.11  2002/12/06 16:59:08  dpp
*    -> test z-use status before attempting to divide by arc length
*       to avoid nasty divide-by-zero
*
* Revision 1.10  2002/09/05 18:57:46  dpp
*      -> require SZ status for all hits tested, not just first in layer
*         this was a long standing er
*
* Revision 1.9  2001/11/19 23:57:39  dpp
*     -> add new control parameter include file
*
* Revision 1.8  2000/10/24 22:48:54  dpp
*      -> use layer block to avoid double use of hits
*      -> block layers with hits remaining after the deletion
*      -> required for z layers with STSZFT=0 hits to avoid double count
*
* Revision 1.7  2000/10/02 20:47:14  dpp
*      -> require valid hit status
*
* Revision 1.6  2000/08/18 18:53:26  dpp
*      -> require hits to be in device IDRFT
*
* Revision 1.5  2000/03/20 22:22:07  dpp
*      -> fix error of stored DZ of best hit on wire
*
* Revision 1.4  2000/03/16 22:12:33  dpp
* remove the selection of BOTH signs of drift distance on same wire
*
* Revision 1.3  2000/03/01 23:01:09  dpp
*      -> protect array bounds: DIM_SOL
*
* Revision 1.2  2000/02/12 20:28:44  dpp
*      -> cut on z of wire
*
* Revision 1.1  2000/01/24 19:57:08  dpp
*      -> scan s-z space for solution
*      -> results put in TFTRAK
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE SCAN_Z_ROADS
C.......................................................................
C.
C. SCAN_Z_ROADS - look for road (defined by each hit) with maximum Z hits
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 6-JAN-2000
C. LAST MOD  : 
C.
C. Modification Log.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE


#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfstakcd.inc"

#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C DIMENSIONS
C-----------------------------------------------------------------------

C DIM_SOL..............dimension of the solution arrays
                       INTEGER DIM_SOL
                       PARAMETER (DIM_SOL=50)

C DIM_HITS_IN_SOL......dimension of the number of hits in a solution
                       INTEGER DIM_HITS_IN_SOL
                       PARAMETER (DIM_HITS_IN_SOL=100)

C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

C INDX_HIT_O...........counter of hit number, OUTER LOOP
                       INTEGER INDX_HIT_O
C LAYR_COUNTR_O........serial layer counter, OUTER LOOP
                       INTEGER LAYR_COUNTR_O
C INDX_HIT_I...........counter of hit number, INNER LOOP
                       INTEGER INDX_HIT_I
C LAYR_COUNTR_I........serial layer counter, INNER LOOP
                       INTEGER LAYR_COUNTR_I
C SOLUTION_SAV.........number of solutions stored
                       INTEGER SOLUTION_SAV
C SOL_TST..............solution number of the test solution
                       INTEGER SOL_TST
C INDX_SOL_HITS........counter of hits within a saved solution
                       INTEGER INDX_SOL_HITS

C-----------------------------------------------------------------------
C other local variables
C-----------------------------------------------------------------------

C NFIT_PREV............number of hits, previous
                       INTEGER NFIT_PREV

C TF_LAYR..............layer number found
                       INTEGER TF_LAYR

C TF_L_I...............layer number found in inner loop
                       INTEGER TF_L_I 

C LAYERS_TESTED........number of layers tested as the seed layer
                       INTEGER LAYERS_TESTED 

C WIRES_TESTED.........number of wires tested as the seed wire
                       INTEGER WIRES_TESTED 

C SMALL_DRIFT_LIM......cut on observed drift distance
                       REAL SMALL_DRIFT_LIM
                       PARAMETER (SMALL_DRIFT_LIM=.000010)

C DZ_CENTER............center of the current road
                       REAL DZ_CENTER

C DZ_WIDTH.............+/- width of the current road
                       REAL DZ_WIDTH 

C OVER_LENGTH_ALLOW....allowed extra Z beyond the length of wire
                       REAL OVER_LENGTH_ALLOW
                       PARAMETER (OVER_LENGTH_ALLOW=.1)

C DZ_HIT...............DZ of the current tested hit
                       REAL DZ_HIT

C RES_HIT...........residual of DZ_HIT relative to DZ_CENTER
                       REAL RES_HIT

C HIT_ON_WIRE..........count the occurance of hits on this wire
                       INTEGER HIT_ON_WIRE

C TEST_HIT.............=.TRUE. if this hit should be tested before
C                      possibly saving the best hit on the wire
                       LOGICAL TEST_HIT

C B_DZ_HIT.............DZ_HIT for the hit with best RES_HIT for a wire
                       REAL B_DZ_HIT

C B_RES_HIT............best RES_HIT for a wire
                       REAL B_RES_HIT

C INDX_HIT_S...........saved value of INDX_HIT_I; pick best solution on wire
                       INTEGER INDX_HIT_S

C Z_LIMIT..............limit of Z to exclude hits outside of chamber
                       REAL Z_LIMIT 

C SOL_HITS.............hit number within the solution
                       INTEGER SOL_HITS(DIM_SOL)

C SOL_TFHITS_N.........TFHITS hit number of a hit in a solution
                       INTEGER SOL_TFHITS_N(DIM_HITS_IN_SOL,DIM_SOL)

C SOL_AVE_DZ...........average DZ of solution
                       REAL SOL_AVE_DZ(DIM_SOL)

C SAV_HITS_MIN.........minimum number of hits to save a solution
                       INTEGER SAV_HITS_MIN
                       parameter (SAV_HITS_MIN=5)

C DZ_WIDTH_NOM.........nominal +/- width allowed
                       REAL DZ_WIDTH_NOM
                       PARAMETER (DZ_WIDTH_NOM=.20)

C SOL_LYRS.............number of layers represented in a solution
                       INTEGER SOL_LYRS(DIM_SOL)

C SOL_LYR_HIT...........NE.0 if a layer has a hit
                       INTEGER SOL_LYR_HIT(KLYRTF)

C TNSAV................save the TANDLF to restore at end
C Y0SAV................save the Y0LF to restore at end
                       REAL TNSAV,Y0SAV

C SOL_BEST.............solution number of the best solution
                       INTEGER SOL_BEST

C HITS_BEST_SOL........number of hits in the best solution
C LYRS_BEST_SOL........number of layers in the best solution
                       INTEGER HITS_BEST_SOL,LYRS_BEST_SOL

C NEW_BEST.............=.TRUE. if the current solution is a new best
                       LOGICAL NEW_BEST

C UNIQUE_BEST..........=.TRUE. if the BEST is "unique"
                       LOGICAL UNIQUE_BEST

C LYR_OVER_IGNORE......number of layers by which the BEST solution must exceed
C                      another solution to not consider the other solution
C                      as competition for the best
                       INTEGER LYR_OVER_IGNORE
                       PARAMETER (LYR_OVER_IGNORE=1)

C HIT_OVER_IGNORE......number of hits by which the BEST solution must exceed
C                      another solution to not consider the other solution
C                      as competition for the best
                       INTEGER HIT_OVER_IGNORE
                       PARAMETER (HIT_OVER_IGNORE=3)

C AVE_DIF_IGNORE.......differerence in average within which a competing
C                      solution is considered equivalent
                       REAL AVE_DIF_IGNORE
                       PARAMETER (AVE_DIF_IGNORE=2.5*DZ_WIDTH_NOM)

C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

                       REAL CFIX
                       COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts here
C-----------------------------------------------------------------------

#if (defined(CLEO_DPPDIAG))&&(defined(CLEO_XTSUBS))
                     CALL XTS_TRAK('CF',0,'SCAN_Z_ROADS, enter')
#endif
      Y0SAV=Y0LF
      TNSAV=TANDLF

      NFIT_PREV=NFIT
      IF(NLYRHT.GE.2)THEN

C-----------------------------------------------------------------------
C  loop over the layers in reverse order
C-----------------------------------------------------------------------
         SOLUTION_SAV=0
         SOL_TST=0
         LAYERS_TESTED=0
         WIRES_TESTED=0
         SOL_BEST=0
         LYRS_BEST_SOL=0
         HITS_BEST_SOL=0
 2       DO 240 LAYR_COUNTR_O=NLYRHT,1,-1
           TF_LAYR=IPLRLR(LAYR_COUNTR_O)
           IF(
     1          (IDVCTF(TF_LAYR).EQ.IDRFT).AND.
     2          (LAYER_BLOCK(TF_LAYR).EQ.0))THEN
             LAYERS_TESTED=LAYERS_TESTED+1

C-----------------------------------------------------------------------
C loop over hit WIRES in the layer
C require room for another solution and minimum drift di0stance
C require SZ status 
C-----------------------------------------------------------------------

             DO 235 INDX_HIT_O=
     2            LHITLR(LAYR_COUNTR_O),
     3            MHITLR(LAYR_COUNTR_O)
               IF(
     1               (SOLUTION_SAV.LT.DIM_SOL).AND.
     2               (ABS(DRFHIT(INDX_HIT_O)).GT.SMALL_DRIFT_LIM).AND.
     3               (STSZHT(INDX_HIT_O).GT.0)
     4               )THEN

C-----------------------------------------------------------------------
C define the road, and set up to evaluate road
C-----------------------------------------------------------------------

                 IF(
     1                 ZHIT(INDX_HIT_O).LT.
     2                 (ZENDTF(TF_LAYR)+OVER_LENGTH_ALLOW))THEN
                  
                   WIRES_TESTED=WIRES_TESTED+1
                   DZ_CENTER=ZHIT(INDX_HIT_O)/SHIT(INDX_HIT_O)
                   DZ_WIDTH=DZ_WIDTH_NOM
                   SOL_TST=SOLUTION_SAV+1
                   SOL_HITS(SOL_TST)=0
                   SOL_AVE_DZ(SOL_TST)=0.
                   SOL_LYRS(SOL_TST)=0
                   CALL VZERO(SOL_LYR_HIT,KLYRTF)
                   NFIT=NFIT_PREV

C-----------------------------------------------------------------------
C  inner loop over the layers
C-----------------------------------------------------------------------

                   DO 140 LAYR_COUNTR_I=1,NLYRHT
                     TF_L_I=IPLRLR(LAYR_COUNTR_I)
                     IF(
     1                    (IDVCTF(TF_L_I).EQ.IDRFT).AND.
     2                    (LAYER_BLOCK(TF_L_I).EQ.0))THEN
                     
C-----------------------------------------------------------------------
C  manual inner loop over hit WIRES in the layer
C-----------------------------------------------------------------------

                       INDX_HIT_I=LHITLR(LAYR_COUNTR_I)-1
 131                   INDX_HIT_I=INDX_HIT_I+1
                       IF(INDX_HIT_I.LE.MHITLR(LAYR_COUNTR_I))THEN

C-----------------------------------------------------------------------
C require hit to be not a ramp-down (or time to low)
C require SZ status 
C-----------------------------------------------------------------------

                         IF(
     2                         (ABS(DRFHIT(INDX_HIT_I)).GT.
     3                         SMALL_DRIFT_LIM).AND.
     3                         (STSZHT(INDX_HIT_I).GT.0))THEN

C-----------------------------------------------------------------------
C loop over the accessible hits on this wire
C NOTE: this increments the loop variable  
C-----------------------------------------------------------------------

                           INDX_HIT_S=0
                           B_RES_HIT=DZ_WIDTH

                           HIT_ON_WIRE=0
 133                       HIT_ON_WIRE=HIT_ON_WIRE+1

                           IF(HIT_ON_WIRE.EQ.1)THEN
                             TEST_HIT=.TRUE.
                           ELSEIF(HIT_ON_WIRE.EQ.2)THEN
                             TEST_HIT=.FALSE.
                             IF(
     2                            INDX_HIT_I.LT.
     3                            MHITLR(LAYR_COUNTR_I)
     4                            )THEN
                               IF(
     2                               IABS(IADRHT(INDX_HIT_I)).EQ.
     3                               IABS(IADRHT(INDX_HIT_I+1))
     4                               )THEN
                                  INDX_HIT_I=INDX_HIT_I+1
                                  TEST_HIT=.TRUE.
                               ENDIF
                             ENDIF
                           ELSE
                             TEST_HIT=.FALSE.
                           ENDIF

C-----------------------------------------------------------------------
C test if the hit is within the road and within the endplates
C require SZ status 
C save the best hit on a wire
C-----------------------------------------------------------------------

c                           IF(TEST_HIT)THEN
c                             DZ_HIT=ZHIT(INDX_HIT_I)
c     2                            /SHIT(INDX_HIT_I)
c                             RES_HIT=ABS(DZ_HIT-DZ_CENTER)
c                             Z_LIMIT=ZENDTF(TF_L_I)
c     2                            +OVER_LENGTH_ALLOW 
c                             IF(STSZHT(INDX_HIT_I).GT.0)THEN
c                               IF(ABS(ZHIT(INDX_HIT_I)).LT.Z_LIMIT)THEN
c                                 IF(RES_HIT.LE.B_RES_HIT)THEN
c                                   INDX_HIT_S=INDX_HIT_I
c                                   B_DZ_HIT=DZ_HIT
c                                   B_RES_HIT=RES_HIT
c                                 ENDIF
c                               ENDIF
c                             ENDIF
c                             GO TO 133
c                           ENDIF

                           IF(TEST_HIT)THEN
                             IF(STSZHT(INDX_HIT_I).GT.0)THEN
                               Z_LIMIT=ZENDTF(TF_L_I)
     2                              +OVER_LENGTH_ALLOW
                               IF(ABS(ZHIT(INDX_HIT_I)).LT.Z_LIMIT)THEN
                                 DZ_HIT=ZHIT(INDX_HIT_I)
     2                                /SHIT(INDX_HIT_I)
                                 RES_HIT=ABS(DZ_HIT-DZ_CENTER)
                                 IF(RES_HIT.LE.B_RES_HIT)THEN
                                   INDX_HIT_S=INDX_HIT_I
                                   B_DZ_HIT=DZ_HIT
                                   B_RES_HIT=RES_HIT
                                 ENDIF
                               ENDIF
                             ENDIF
                             GO TO 133
                           ENDIF

C-----------------------------------------------------------------------
C save the best hit on the wire in the solution, if there is one
C-----------------------------------------------------------------------

                           IF(INDX_HIT_S.NE.0)THEN
                             SOL_HITS(SOL_TST)=SOL_HITS(SOL_TST)+1

C-----------------------------------------------------------------------
C add hits to solution if there is room
C-----------------------------------------------------------------------

                             IF(SOL_HITS(SOL_TST).LE.
     2                            DIM_HITS_IN_SOL)THEN
                               SOL_TFHITS_N(SOL_HITS(SOL_TST),SOL_TST)
     3                               =INDX_HIT_S
                               SOL_AVE_DZ(SOL_TST)=
     2                              SOL_AVE_DZ(SOL_TST)+B_DZ_HIT
                               SOL_LYR_HIT(IPLHIT(INDX_HIT_S))=
     2                              SOL_LYR_HIT
     3                              (IPLHIT(INDX_HIT_S))
     4                              +1
                               IF(SOL_LYR_HIT(IPLHIT(INDX_HIT_S)).EQ.1)
     3                              SOL_LYRS(SOL_TST)=
     4                              SOL_LYRS(SOL_TST)+1

#if defined(CLEO_DPPDIAG)
                               CALL HIT_TO_FIT(H2F_NEW_ENTRY,
     2                              INDX_HIT_S,0)
#endif

C end room to store condition
                             ENDIF
C end found best hit on wire condition
                           ENDIF
C end of SMALL_DRIFT_LIM condition for inner LOOP
                         ENDIF
C end of inner loop over wires
                         GO TO 131
                       ENDIF
C end of inner loop over layers
                     ENDIF
 140               CONTINUE

C-----------------------------------------------------------------------
C evaluatate road
C save the best road
C-----------------------------------------------------------------------

                   IF(SOL_HITS(SOL_TST).GE.SAV_HITS_MIN)THEN
                     SOLUTION_SAV=SOL_TST
#if defined(CLEO_DPPDIAG)
                     PRINT 1003
#endif
                     IF(SOL_LYRS(SOL_TST).GT.LYRS_BEST_SOL)THEN
                        NEW_BEST=.TRUE.
                     ELSEIF(SOL_LYRS(SOL_TST).EQ.LYRS_BEST_SOL)THEN
                       IF(SOL_HITS(SOL_TST).GT.HITS_BEST_SOL)THEN
                         NEW_BEST=.TRUE.
                       ELSE
                         NEW_BEST=.FALSE.
                       ENDIF
                     ELSE
                       NEW_BEST=.FALSE.
                     ENDIF
                     IF(NEW_BEST)THEN
                       SOL_BEST=SOL_TST
                       LYRS_BEST_SOL=SOL_LYRS(SOL_TST)
                       HITS_BEST_SOL=SOL_HITS(SOL_TST)
#if defined(CLEO_DPPDIAG)
                       PRINT 2002
 2002                  FORMAT(' SCAN_Z_ROADS: NEW BEST')
#endif
                     ENDIF
                        
                     SOL_AVE_DZ(SOL_TST)=
     2                    SOL_AVE_DZ(SOL_TST)/SOL_HITS(SOL_TST)

#if defined(CLEO_DPPDIAG)
                     PRINT 2001,
     2                    LAYR_COUNTR_O,INDX_HIT_O,
     3                    SOL_HITS(SOL_TST),SOL_LYRS(SOL_TST),
     4                    SOL_AVE_DZ(SOL_TST)
 2001                FORMAT(
     2                    ' SCAN_Z_ROADS:L=',I3,' hitNUM=',I5,
     3                    ' number of hits=',I2,' hitLYRS=',I2,
     4                    ' ave DZ=',F8.3)
#if defined(CLEO_XTSUBS)
                     Y0LF=0.
                     TANDLF=SOL_AVE_DZ(SOL_TST)
                     CALL XTS_TRAK('CF',0,'SCAN_Z_ROADS, sav')
#endif
#endif
                   ENDIF

C end of test of Z of road seed hit
                 ENDIF
C end of SMALL_DRIFT_LIM condition for outer LOOP
               ENDIF
C end of outer loop over wires
 235         CONTINUE
C  end of outer loop over layers
           ENDIF
 240     CONTINUE

C-----------------------------------------------------------------------
C  store result in stack
C-----------------------------------------------------------------------

         IF(SOL_BEST.NE.0)THEN
            UNIQUE_BEST=.TRUE.
            DO 340 SOL_TST=1,SOLUTION_SAV
               IF(SOL_TST.NE.SOL_BEST)THEN
                  IF(
     1                 ((SOL_LYRS(SOL_BEST)-SOL_LYRS(SOL_TST))
     1                 .LT.LYR_OVER_IGNORE).AND.
     2                 ((SOL_HITS(SOL_BEST)-SOL_HITS(SOL_TST))
     2                 .LT.HIT_OVER_IGNORE).AND.
     3                 (ABS(SOL_AVE_DZ(SOL_BEST)-SOL_AVE_DZ(SOL_TST))
     3                 .GT.AVE_DIF_IGNORE))THEN
                     UNIQUE_BEST=.FALSE.
                  ENDIF
               ENDIF
 340        CONTINUE
         ELSE
            UNIQUE_BEST=.FALSE.
         ENDIF

C end of (NLYRHT.GT.1) condition
      ELSE
         UNIQUE_BEST=.FALSE.
      ENDIF

      NFIT=NFIT_PREV

      IF(UNIQUE_BEST)THEN
         DO 440 INDX_SOL_HITS=1,SOL_HITS(SOL_BEST)
            IF(INDX_SOL_HITS.LE.DIM_HITS_IN_SOL)THEN
               INDX_HIT_I=SOL_TFHITS_N(INDX_SOL_HITS,SOL_BEST)
               CALL HIT_TO_FIT(H2F_NEW_ENTRY,INDX_HIT_I,0)
            ENDIF
 440     CONTINUE

         Y0LF=0.
         TANDLF=SOL_AVE_DZ(SOL_BEST)
         
#if defined(CLEO_DPPDIAG)
         PRINT 1003
         PRINT 2003,
     2                    SOL_BEST,
     3                    SOL_HITS(SOL_BEST),SOL_LYRS(SOL_BEST),
     4                    SOL_AVE_DZ(SOL_BEST)
 2003    FORMAT(
     2                    ' BEST SOLUTION=',I5,
     3                    ' number of hits=',I2,' hitLYRS=',I2,
     4                    ' ave DZ=',F8.3)
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,'SCAN_Z_ROADS, best')
#endif
#endif

      ELSE
         Y0LF=  Y0SAV
         TANDLF=TNSAV
      ENDIF

C-----------------------------------------------------------------------
C  return
C-----------------------------------------------------------------------
 
      RETURN

 1003 FORMAT(
     1' --------------------------------------------------------------')

      END






