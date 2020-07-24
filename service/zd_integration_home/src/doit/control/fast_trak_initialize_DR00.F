* $Id: fast_trak_initialize_DR00.F,v 1.1 2003/03/05 17:08:13 dpp Exp $
*
* $Log: fast_trak_initialize_DR00.F,v $
* Revision 1.1  2003/03/05 17:08:13  dpp
*      -> NEW ROUTINE
*      -> DR3 only specific fast track initialization
*      -> add max ave resid for loc amb solution; set to .99 for no effect
*      -> changed all variable names in cdscrtcd to have common root
*      -> variable extra z requirement in ambiguity solution, 999. no effect
*      -> variable z match requirement in close hit, 999. no effect
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_INITIALIZE_DR00
C.......................................................................
C.
C. FAST_TRAK_INITIALIZE_DR00 - initilize cuts and ambiguity group structure
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split from FAST_TRAK_INITIALIZE_CUTS 11-Nov-2002
C. LAST MOD  : 
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
C local index
C-----------------------------------------------------------------------
C IGRPCR...............counter
                       INTEGER IGRPCR

C-----------------------------------------------------------------------
C DATA  ,  use data statement for local variable, then transfer into common
C-----------------------------------------------------------------------

C DIM_GROUP............number of groups to do local ambiguity, dimension
C NUMB_OF_GROUPS.......number of ambiguity groups created
C LYR_FIRST_IN_GR......gives first TF layer in a group
C LYR_LAST_IN_GR.......gives last TF layer in a group

      INTEGER DIM_GROUP_dr
      PARAMETER (DIM_GROUP_dr=12)
      INTEGER NUMB_OF_GROUPS_dr
      INTEGER LYR_FIRST_IN_GR_dr(DIM_GROUP_dr)
      INTEGER LYR_LAST_IN_GR_dr(DIM_GROUP_dr)
      
      DATA NUMB_OF_GROUPS_dr,
     2     (LYR_FIRST_IN_GR_dr(IGRPCR),IGRPCR=1,12),
     3     (LYR_LAST_IN_GR_dr(IGRPCR),IGRPCR=1,12)/
     1     12, 
     2     5, 9,13,17, 25,29,33,37,41,45,49,53,
     3     8,12,16,20, 28,32,36,40,44,48,52,56/

        
C-----------------------------------------------------------------------
C  Executable code starts here
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C variables required to transfer from ambiguity group data structure
C    to /TFTRAK/
C-----------------------------------------------------------------------
C-----------------------------------------------------------------------
C other variables required for ambiguity groups
C-----------------------------------------------------------------------
      NUMB_OF_GROUPS=NUMB_OF_GROUPS_dr
      IF(NUMB_OF_GROUPS.GT.DIM_GROUP)NUMB_OF_GROUPS=DIM_GROUP

#if defined(CLEO_FTDIAG)
      PRINT 1001,NUMB_OF_GROUPS
 1001 FORMAT(' FAST_T: created ',I3,' local ambiguity groups') 
#endif

      
      IF(NUMB_OF_GROUPS.GT.0)THEN
        DO 35 IGRPCR=1,NUMB_OF_GROUPS
          LYR_FIRST_IN_GR(IGRPCR)=LYR_FIRST_IN_GR_dr(IGRPCR)
          LYR_LAST_IN_GR(IGRPCR)=LYR_LAST_IN_GR_dr(IGRPCR)
          
#if defined(CLEO_FTDIAG)
          PRINT 1002,IGRPCR,
     2         LYR_FIRST_IN_GR(IGRPCR),
     2         LYR_LAST_IN_GR(IGRPCR)
 1002     FORMAT(' FAST_T: group',I4,' TF layers',I4,' to',I4)
#endif
          
 35     CONTINUE
      ENDIF

C-----------------------------------------------------------------------
C bias in Z0 projection
C-----------------------------------------------------------------------

C Z0BIAS_1............Z0BIAS used for first iteration
C         Z0BIAS_1=.04
C         Z0BIAS_1=.02
         Z0BIAS_1=0.
C Z0BIAS_2.............Z0BIAS used for later iterations
         Z0BIAS_2=.1
C Z0BIAS_FULL..........Z0BIAS used for full constraint
         Z0BIAS_FULL=0.
C DZRAIL...............maximum TANDLF to saturate result from SF
         DZRAIL=2.53
C-----------------------------------------------------------------------
C control of running with fixed tan(dip) windows
C-----------------------------------------------------------------------

C N_DZ_FIX.............number of tan(dip) values, defined
         N_DZ_FIX=11
         IF(N_DZ_FIX.GT.DIM_DZ_FIX)N_DZ_FIX=DIM_DZ_FIX
C TRY_DZ_FIX...........the tan(dip) values
         DO 49 INDX_DZ_FIX=1,N_DZ_FIX
            TRY_DZ_FIX(INDX_DZ_FIX)=-2.0 +(INDX_DZ_FIX-1) * 0.4
 49      CONTINUE
C-----------------------------------------------------------------------
C cuts for the wire fit
C-----------------------------------------------------------------------

C NHIT_MIN_TRAK........number of hits; minimal track
         NHIT_MIN_TRAK=7
C NHIT_LIM_D0_CONS.....minimum number of hits to remove D0 constraint
         NHIT_LIM_D0_CONS=10
C CURV_LIM_D0_CONS.....maximum curvature to remove D0 constraint
         CURV_LIM_D0_CONS=1.5

C-----------------------------------------------------------------------
C cuts for both close hit and local ambiguity
C-----------------------------------------------------------------------

C Z_OVER_LENGTH_FT.....limit of z of hit beyond wire length
         Z_OVER_LENGTH_FT=.13

C-----------------------------------------------------------------------
C cuts for close hit
C-----------------------------------------------------------------------

C N_NOTAIL.............number of layers previously found to use TF_LAYR_MIN/MAX
         N_NOTAIL=10
C SIN_ENTR_CLOSE.......sine of entrance angle cut for close hit
         SIN_ENTR_CLOSE=0.90
C         SIN_ENTR_CLOSE=9.99
C DEWEIGHT_CLOSE_ENUF..local ambiguity weight over close hit weight
C                      when there are enough hits found in loc-amb
C                      deweight=100 is sigma of .001 compared to .0001
         DEWEIGHT_CLOSE_ENUF=100.

C ROAD_CLOSE_ENUF......cut on allowed deviation from fit for close found hits
C                      when there are enough hits found in loc-amb
#if defined(CLEO_CLEO3)
         ROAD_CLOSE_ENUF=0.002500
#else
         ROAD_CLOSE_ENUF=0.001250
#endif


C ROAD_Z_CLOSE_ENUF....cut on allowed deviation from z fit for FITTYP_CF mode
C                      when there are enough hits found in loc-amb
         ROAD_Z_CLOSE_ENUF=999.

C TIME_LIM_CLOSE_ENUF..cut on observed time
C                      when there are enough hits found in loc-amb
#if defined(CLEO_CLEO3)
         TIME_LIM_CLOSE_ENUF=0.
#else
         TIME_LIM_CLOSE_ENUF=0.000020
#endif
C-----------------------------------------------------------------------
C cuts for local ambiguity
C-----------------------------------------------------------------------

C MAX_ITERATE..........maximum iterations through local ambiguity
         IF(L_FT_DOIT_FULL)THEN
            MAX_ITERATE=2
         ELSE
            MAX_ITERATE=1
         ENDIF

C EXECUTE_AMBIGUITY....flag that the ambiguity part should be executed
         EXECUTE_AMBIGUITY=.TRUE.

C EXECUTE_ALTERNATE....flag that alternate ambiguity solutiuns should be tested
         IF(L_FT_DOIT_FULL)THEN
            EXECUTE_ALTERNATE=.TRUE.
         ELSE
            EXECUTE_ALTERNATE=.FALSE.
         ENDIF

C SIN_ENTR_LOC_AMB.......sine of entrance angle cut for local ambiguity
         SIN_ENTR_LOC_AMB=0.95
C         SIN_ENTR_LOC_AMB=9.99

C MAX_RES_DZ_WINDOW....maximum residual allowed when using tan(dip) window
         MAX_RES_DZ_WINDOW=.015

C MIN_LYRS_ACCEPT......min number of layers found to accept ambiguity group
         MIN_LYRS_ACCEPT=3

C MIN_DRIFT............minimum drift distance to remove "ramp-downs"
         MIN_DRIFT=.000020

C ROAD_AMB.............road for ambiguity, full width, in meters ;
C                      absolute width of road containing hits in solution
         ROAD_AMB=0.004500

C ROAD_AMB_EXTRA_Z.....road for extra z requirement for solution, half width
         ROAD_AMB_EXTRA_Z=999.

C MAX_AVE_RES_SOL......maximum average residual for solution
         MAX_AVE_RES_SOL=.99

C ROAD_AMB_NO_ALIGN....secondary road for ambiguity, full width, in meters ;
C                      no alignment cut applied
         ROAD_AMB_NO_ALIGN=0.002500

C MIN_LYRS_ALIGN.......min number layers to accept ambiguity group on align
         MIN_LYRS_ALIGN=4

C ALIGN_AMB_FULL_ROAD..alignment cut to accept full road
         ALIGN_AMB_FULL_ROAD=0.000600

C RES_DIF_SIM_HITS.....minimum residual difference between two hits
C                      to be considered unique for bumping 1st into 2nd  
         RES_DIF_SIM_HITS=0.003

C MAX_DIFF_HIT.........maximum number of "different wire" hits for two
C                      solutions to be considered similar
         MAX_DIFF_HIT=1

C LEN_DIF_UNIQUE.......minimum length difference between two solutions
C                      for the 1st best to be considered unique
         LEN_DIF_UNIQUE=1

C WID_DIF_UNIQUE.......minimum width difference between two solutions
C                      for the 1st best to be considered unique
         WID_DIF_UNIQUE=0.00125

C RES_DIF_UNIQUE.......minimum residual difference between two solutions
C                      for the 1st best to be considered unique
         RES_DIF_UNIQUE=0.00125


C-----------------------------------------------------------------------
C cuts for evaluating if there are enough axial and stereo layers
C-----------------------------------------------------------------------

C AX_LYR_HIT_EXPCT.....number of hits in axial layers expected
C        assigned in code for each track

C ST_LYR_HIT_EXPCT.....number of hits in stereo layers expected
C        assigned in code for each track

C SIGNIF_HITS_AMB_GR...significant number of hits in ambiguity group
C                      to count in AX/ST_AMB_GR_HIT
         SIGNIF_HITS_AMB_GR=2

C AX_AMB_GR_HIT_EXPCT..number significant hit axial ambiguity groups expected 
C        assigned in code for each track

C ST_AMB_GR_HIT_EXPCT..numbr significant hit stereo ambiguity groups expected 
C        assigned in code for each track

C F_AX_LYR_HT_REQ_D0...fraction of expected number of axial hits required
C                      to allow D0 to float
         F_AX_LYR_HT_REQ_D0=.3

C F_ST_LYR_HT_REQ_D0...fraction of expected number of stereo hits required
C                      to allow D0 to float
         F_ST_LYR_HT_REQ_D0=.5

C F_ST_LYR_HT_REQ_Z0...fraction of expected number of stereo hits required
C                      to allow Z0 to float
         F_ST_LYR_HT_REQ_Z0=0.

C F_ST_LYR_HT_REQ_PZ...fraction of expected number of stereo hits required
C                      to pass z criteria for saving track
         F_ST_LYR_HT_REQ_PZ=.3

C F_ST_U_LYR_HT_REQ_PZ.fraction of expected number of +stereo hits required
C                      to pass z criteria for saving track
         F_ST_U_LYR_HT_REQ_PZ=.1

C F_ST_V_LYR_HT_REQ_PZ.fraction of expected number of -stereo hits required
C                      to pass z criteria for saving track
         F_ST_V_LYR_HT_REQ_PZ=.1

C AX_AMB_GR_HT_REQ_D0..number of axial ambiguity groups required (indexed
C                      by number expected) to allow D0 to float
         AX_AMB_GR_HT_REQ_D0(0)=0
         AX_AMB_GR_HT_REQ_D0(1)=0
         AX_AMB_GR_HT_REQ_D0(2)=0
         AX_AMB_GR_HT_REQ_D0(3)=0
         AX_AMB_GR_HT_REQ_D0(4)=0

C ST_AMB_GR_HT_REQ_D0..number of stereo ambiguity groups required (indexed
C                      by number expected) to allow D0 to float

         ST_AMB_GR_HT_REQ_D0(0)=3
         ST_AMB_GR_HT_REQ_D0(1)=3
         ST_AMB_GR_HT_REQ_D0(2)=3
         ST_AMB_GR_HT_REQ_D0(3)=3
         ST_AMB_GR_HT_REQ_D0(4)=3
         ST_AMB_GR_HT_REQ_D0(5)=3
         ST_AMB_GR_HT_REQ_D0(6)=4
         ST_AMB_GR_HT_REQ_D0(7)=5
         ST_AMB_GR_HT_REQ_D0(8)=6

C ST_AMB_GR_HT_REQ_Z0..number of stereo ambiguity groups required (indexed
C                      by number expected) to allow Z0 to float
C         ST_AMB_GR_HT_REQ_Z0(0)=3 BEFORE FEB 11, 2000
C         ST_AMB_GR_HT_REQ_Z0(1)=3
C         ST_AMB_GR_HT_REQ_Z0(2)=3
C         ST_AMB_GR_HT_REQ_Z0(3)=3
C         ST_AMB_GR_HT_REQ_Z0(4)=3
C         ST_AMB_GR_HT_REQ_Z0(5)=3
C         ST_AMB_GR_HT_REQ_Z0(6)=3
C         ST_AMB_GR_HT_REQ_Z0(7)=4
C         ST_AMB_GR_HT_REQ_Z0(8)=5

         ST_AMB_GR_HT_REQ_Z0(0)=4
         ST_AMB_GR_HT_REQ_Z0(1)=4
         ST_AMB_GR_HT_REQ_Z0(2)=4
         ST_AMB_GR_HT_REQ_Z0(3)=4
         ST_AMB_GR_HT_REQ_Z0(4)=4
         ST_AMB_GR_HT_REQ_Z0(5)=4
         ST_AMB_GR_HT_REQ_Z0(6)=4
         ST_AMB_GR_HT_REQ_Z0(7)=4
         ST_AMB_GR_HT_REQ_Z0(8)=5

C ST_AMB_GR_HT_REQ_PZ..number of stereo ambiguity groups required (indexed
C                    by number expected) to pass z criteria for saving track
C         ST_AMB_GR_HT_REQ_PZ(0)=0
C         ST_AMB_GR_HT_REQ_PZ(1)=0
C         ST_AMB_GR_HT_REQ_PZ(2)=0
C         ST_AMB_GR_HT_REQ_PZ(3)=0
C         ST_AMB_GR_HT_REQ_PZ(4)=1

c         ST_AMB_GR_HT_REQ_PZ(0)=2
c         ST_AMB_GR_HT_REQ_PZ(1)=2

         ST_AMB_GR_HT_REQ_PZ(0)=0
         ST_AMB_GR_HT_REQ_PZ(1)=1

         ST_AMB_GR_HT_REQ_PZ(2)=2
         ST_AMB_GR_HT_REQ_PZ(3)=2
         ST_AMB_GR_HT_REQ_PZ(4)=2

         ST_AMB_GR_HT_REQ_PZ(5)=2
         ST_AMB_GR_HT_REQ_PZ(6)=3
         ST_AMB_GR_HT_REQ_PZ(7)=4
         ST_AMB_GR_HT_REQ_PZ(8)=5

C AX_LYR_HT_REQ_PD0....number of axial layers required
C                      to pass D0 criteria
         AX_LYR_HT_REQ_PD0=9



C FIRST_AXIAL_LYR......first axial layer (TF layer) for counting 
         FIRST_AXIAL_LYR=5

C LAST_AXIAL_LYR.......last axial layer (TF layer) for counting
         LAST_AXIAL_LYR=20

C FIRST_AXIAL_AMB_GR...first axial ambiguity group for counting
         FIRST_AXIAL_AMB_GR=1

C LAST_AXIAL_AMB_GR....last axial ambiguity group for counting
         LAST_AXIAL_AMB_GR=4

C FIRST_STEREO_LYR.....first stereo layer (TF layer) for counting 
         FIRST_STEREO_LYR=25

C LAST_STEREO_LYR......last stereo layer (TF layer) for counting
         LAST_STEREO_LYR=55

C FIRST_STEREO_AMB_GR..first stereo ambiguity group for counting
         FIRST_STEREO_AMB_GR=5

C LAST_STEREO_AMB_GR...last stereo ambiguity group for counting
         LAST_STEREO_AMB_GR=12

C-----------------------------------------------------------------------
C set-up for _ALTERNATE_AMB_GRP_1
C-----------------------------------------------------------------------

C NUMB_GR_ALTER........number of groups to be alternated to make up the
C                      alternate solutions
C                      YOU ARE RESPONSIBLE to make sure that this does not
C                      lead to a MAX_INDX_SOL_SET greater than 
C                      DIM_ALT_SOL_SET (a parameter)
         NUMB_GR_ALTER=4

C MAX_INDX_SOL_SET.....maximum of INDX_SOL_SET; 2**NUMB_GR_ALTER
         MAX_INDX_SOL_SET=2**NUMB_GR_ALTER


C-----------------------------------------------------------------------
C cuts for _ALTERNATE_AMB_GRP_2
C-----------------------------------------------------------------------
C DIF_SHIFT_CUT........maximum allowed shift in r-phi
C                      between two consecutive group solutions
         DIF_SHIFT_CUT= 0.002
C DIF_ZMEAS_CUT........maximum allowed z measurement
C                      between two consecutive group solutions
C  12/15/99         DIF_ZMEAS_CUT= 0.140
         DIF_ZMEAS_CUT= 0.170
C DIF_SHIFT_SIGMA......sigma used to calculate quality from shift in r-phi
C                      between two consecutive group solutions
         DIF_SHIFT_SIGMA= 0.001
C DIF_ZMEAS_SIGMA......sigma used to calculate quality from z measurement
C                      between two consecutive group solutions
         DIF_ZMEAS_SIGMA= 0.030
C-----------------------------------------------------------------------
C cuts for HIT_DROPPING/ADDING_OTHER  SECTION
C-----------------------------------------------------------------------

C CELCT1...............lower limit cut (in cells) on PULFIT to
C                      drop hits, dropping up to MAX_DROP
C  12/15/99         CELCT1=.24
         CELCT1=.14

C MAX_DROP.............maximum number of hits allowed to be dropped with CELCT1
C     5/19/99     MAX_DROP=3
         MAX_DROP=10

C CELCT2...............lower limit cut (in cells) on PULFIT to
C                      continue testing to drop more hits; 
C                      below this cut, dropping will end after refit 
         CELCT2=1.6

C DROP_REQ_REFIT.......threshold of NDROP_NOT_REFIT to require refit
C                      before dropping more hits
         DROP_REQ_REFIT=3

C SHORT_REQ_REFIT......force refit if number of hits drop below this amount
         SHORT_REQ_REFIT=10



      RETURN
      END






