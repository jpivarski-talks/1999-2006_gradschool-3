*
* $Id: fast_trak.inc,v 1.14 2003/03/05 17:36:12 dpp Exp $
*
* $Log: fast_trak.inc,v $
* Revision 1.14  2003/03/05 17:36:12  dpp
*      -> TPC flag in status bits transfered to fast_trak common
*      -> add maximum average residual for local amb solution
*      -> changed all variable names in cdscrtcd to have common root
*      -> add variable extra z requirement in ambiguity solution
*      -> add variable z match requirement in close hit
*
* Revision 1.13  2002/09/05 19:16:54  dpp
*      -> move more cuts into common
*      -> remove "offset residual" and "group sets"
*      -> add _check_hits_layers argument
*         to indicate that D0 is probably established
*
* Revision 1.12  2002/05/23 14:25:23  dpp
*      -> add radius limit for adding hits to track
*      -> add largest radius of occupied layer in amb group
*
* Revision 1.11  2001/11/19 23:18:29  dpp
*      -> new logical for program control
*      -> move some switches to controlpar.inc
*
* Revision 1.10  2001/09/12 22:13:28  dpp
*      -> deweight hits added in STANDARD that have large entrance angle
*
* Revision 1.9  2000/12/19 23:29:56  dpp
*      -> define parameters to control TFPOPF
*      -> define variable for limit of Z outside endplate
*      -> define flag to control window in tan(dip) in fast_trak
*      -> define residual maximum to limit width of window in tan(dip)
*      -> define parameters and variables to control windows in tan(dip)
*      -> variables and cuts for separate "u" and "v" stereo layers
*         requirements to "pass z criteria"
*
* Revision 1.8  2000/10/02 21:16:38  dpp
*      -> change names to Z0BIAS_1 and Z0BIAS_2
*      -> add Z0BIAS_FULL and Z0BIAS_USE
*
* Revision 1.7  2000/08/18 18:34:54  dpp
*      -> add D0 constraint based on (lack of) stereo hits
*         to address problem of tracks with only two stereo super layers
*         fitting to a larger curvature rather than a larger Z
*      -> retain D0 constraint through the first iteration
*      -> saturate TANDLF
*
* Revision 1.6  2000/04/26 14:25:03  dpp
*      -> move D0_CONSTRAIN  and Z0_CONSTRAIN to TFCTLCDE
*      -> CLEAN UP
*
* Revision 1.5  2000/01/06 22:25:26  dpp
* -> variables to control offset
* -> error code
* -> variables for large slant solutions
* -> variables for evaluating hit consentration
*
* Revision 1.4  1999/11/23 23:27:21  dpp
* add variables for entrance angle cuts
*
* Revision 1.3  1999/11/17 21:43:01  dpp
* add more cuts for FAST_TRAK_ALTERNAT_AMB_2
*
* Revision 1.2  1999/11/14 18:15:11  dpp
* added L2H_SAV_SOL
*
* Revision 1.1  1999/11/03 22:57:11  dpp
* new
*
*
* PARAMETERS AND COMMON FOR FAST_TRAK and subparts 
*
* fast_trak.inc
*
#if !defined(CLEO_NOCOMDOC)

C-----------------------------------------------------------------------
C dimensions of arrays
C-----------------------------------------------------------------------

C DIM_GROUP............number of groups to do local ambiguity, dimension
C DIM_HITS_SOL.........number of hits in a loc amb group solution, dimension
C DIM_HITS_IN_GROUP....number of usable hits in an ambiguity group
C DIM_ALT_SOL_SET......number of possible alternate solution sets (in _1)
C DIM_SAV_SOL_SET......number of saved alternate solution sets (in _2)
C DIM_DZ_FIX...........number of tan(dip) values, possible

C-----------------------------------------------------------------------
C logical extracted from the event control bits
C-----------------------------------------------------------------------

C L_FT_DOIT_FULL.......=.true. if DOIT_FULL event control bit is set
C L_FT_DOIT_TPC........=.true. if DOIT_TPC event control bit is set

C-----------------------------------------------------------------------
C variables to allow comparison of results from different parts of the
C program
C-----------------------------------------------------------------------

C ZTMP.................value of "z" held from early fit
C ERROR_CODE...........error code transferred from subroutines

C-----------------------------------------------------------------------
C variables common to "local ambiguity", "close", and the main program
C-----------------------------------------------------------------------

C FLAG_LYR_USED........flag of ambiguity resolution, 0:not, 1:close, 2:local
C HITBSC...............number of layers with hits before the scatter
C HITASC...............number of layers with hits after  the scatter  
C TF_LAYR_MIN..........minimum layer found in local ambiguity
C TF_LAYR_MAX..........maximum layer found in local ambiguity
C AX_LYR_HIT...........number of axial layers with hits
C ST_LYR_HIT...........number of stereo layers with hits
C ST_U_LYR_HIT.........number of +stereo layers with hits
C ST_V_LYR_HIT.........number of -stereo layers with hits
C HITS_IN_AMB_GR.......number of hits in each of the ambiguity groups 
C                      (indexed by ambiguity group)  
C RAD_IN_AMB_GR........maximum radius occupied in each ambiguity group
C                      (indexed by ambiguity group)  
C AX_AMB_GR_HIT........number of axial ambiguity groups with significant hits
C ST_AMB_GR_HIT........number of stereo ambiguity groups with significant hits

C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

C ITERATION............iteration number for ambiguity resolution
C INDX_DZ_FIX..........counter of tan(dip) values

C-----------------------------------------------------------------------
C variables required to transfer from ambiguity group solutions to /TFTRAK/
C-----------------------------------------------------------------------

C NUMB_OF_GROUPS.......number of ambiguity groups created
C INDX_SOL_USE.........index of the solution to a group to use

C-----------------------------------------------------------------------
C ambiguity group solutions
C-----------------------------------------------------------------------
C required to transfer to /TFTRAK/
C NUM_HITS_SAV_SOL.....number of hits in SAVED local ambiguity solution
C LYR_SAV_SOL..........layer number for hits in HITS_SAV_SOL
C HITS_SAV_SOL.........seq hit number in "road" data structure 
C                      of hits in for SAVED local ambiguity solution

C NOT required to transfer to /TFTRAK/
C L2H_SAV_SOL..........map from layer-within-group to
C                      seq hit number in "road" data structure
C                      used for comparing hits lists of solutions
C USED_ROAD_SAV_SOL....USED_ROAD_CUR_SOL in SAVED local ambiguity solution
C RES_SAV_SOL..........residuals for hits in HITS_SAV_SOL
C AVE_RES_SAV_SOL......average residual

C-----------------------------------------------------------------------
C other variables required to describe the ambiguity groups
C-----------------------------------------------------------------------

C LYR_FIRST_IN_GR......gives first TF layer in a group
C LYR_LAST_IN_GR.......gives last TF layer in a group
 
C-------------------------------------------------------------------
C bias in projection, and flag to comunicate with DUFIND
C-----------------------------------------------------------------------

C Z0BIAS_1.............Z0BIAS used for first iteration
C Z0BIAS_2.............Z0BIAS used for later iterations
C Z0BIAS_FULL..........Z0BIAS used for full constraint
C Z0BIAS_USE...........Z0BIAS currently used
C 
C DZRAIL...............maximum TANDLF to saturate result from SF
C F_T_DZ_WINDOWS.......=.TRUE. to put FAST_TRAK in mode to search
C                      near an input tan(dip)  

C-----------------------------------------------------------------------
C control of running with fixed tan(dip) windows
C-----------------------------------------------------------------------

C N_DZ_FIX.............number of tan(dip) values, defined
C TRY_DZ_FIX...........the tan(dip) values

C-----------------------------------------------------------------------
C cuts for the wire fit
C-----------------------------------------------------------------------

C NHIT_MIN_TRAK........number of hits; minimal track
C NHIT_LIM_D0_CONS.....minimum number of hits to remove D0 constraint
C CURV_LIM_D0_CONS.....maximum curvature to remove D0 constraint

C-----------------------------------------------------------------------
C cuts for both close hit and local ambiguity
C-----------------------------------------------------------------------

C Z_OVER_LENGTH_FT.....limit of z of hit beyond wire length
C RADIUS_LIMIT.........limit of radius for adding hits

C-----------------------------------------------------------------------
C cuts for close hit
C-----------------------------------------------------------------------

C N_NOTAIL.............number of layers previously found to use TF_LAYR_MIN/MAX
C SIN_ENTR_CLOSE.......sine of entrance angle cut for close hit
C DEWEIGHT_CLOSE_ENUF..local ambiguity weight over close hit weight
C                      when there are enough hits found in loc-amb
C ROAD_CLOSE_ENUF......cut on allowed deviation from fit for close found hits
C                      when there are enough hits found in loc-amb
C ROAD_Z_CLOSE_ENUF....cut on allowed deviation from z fit for FITTYP_CF mode
C                      when there are enough hits found in loc-amb
C TIME_LIM_CLOSE_ENUF..cut on observed time
C                      when there are enough hits found in loc-amb
        
C-----------------------------------------------------------------------
C cuts for local ambiguity
C-----------------------------------------------------------------------

C MAX_ITERATE..........maximum iterations through local ambiguity
C EXECUTE_AMBIGUITY....flag that the ambiguity part should be executed
C EXECUTE_ALTERNATE....flag that alternate ambiguity solutiuns should be tested
C SIN_ENTR_LOC_AMB.....sine of entrance angle cut for local ambiguity
C MAX_RES_DZ_WINDOW....maximum residual allowed when using tan(dip) window
C MIN_LYRS_ACCEPT......min number of layers found to accept ambiguity group
C MIN_DRIFT............minimum drift distance to remove "ramp-downs"
C ROAD_AMB.............road for ambiguity, full width, in meters ;
C                      absolute width of road containing hits in solution
C ROAD_AMB_EXTRA_Z.....road for extra z requirement for solution, half width
C MAX_AVE_RES_SOL......maximum average residual for solution
C ROAD_AMB_NO_ALIGN....secondary road for ambiguity, full width, in meters ;
C                      no alignment cut applied
C MIN_LYRS_ALIGN.......min number layers to accept ambiguity group on align
C ALIGN_AMB_FULL_ROAD..alignment cut to accept full road
C
C RES_DIF_SIM_HITS.....minimum residual difference between two hits
C                      to be considered unique for bumping 1st into 2nd  
C MAX_DIFF_HIT.........maximum number of "different wire" hits for two
C                      solutions to be considered similar
C LEN_DIF_UNIQUE.......minimum length difference between two solutions
C                      for the 1st best to be considered unique
C WID_DIF_UNIQUE.......minimum width difference between two solutions
C                      for the 1st best to be considered unique
C RES_DIF_UNIQUE.......minimum residual difference between two solutions
C                      for the 1st best to be considered unique

C-----------------------------------------------------------------------
C cuts for evaluating if there are enough axial and stereo layers
C-----------------------------------------------------------------------

C AX_LYR_HIT_EXPCT.....number of hits in axial layers expected
C ST_LYR_HIT_EXPCT.....number of hits in stereo layers expected
C ST_U_LYR_HIT_EXPCT...number of hits in +stereo layers expected
C ST_V_LYR_HIT_EXPCT...number of hits in -stereo layers expected
C SIGNIF_HITS_AMB_GR...significant number of hits in ambiguity group
C                      to count in AX/ST_AMB_GR_HIT
C AX_AMB_GR_HIT_EXPCT..number significant hit axial ambiguity groups expected 
C ST_AMB_GR_HIT_EXPCT..numbr significant hit stereo ambiguity groups expected 
C F_AX_LYR_HT_REQ_D0...fraction of expected number of axial hits required
C                      to allow D0 to float
C F_ST_LYR_HT_REQ_D0...fraction of expected number of stereo hits required
C                      to allow D0 to float
C F_ST_LYR_HT_REQ_Z0...fraction of expected number of stereo hits required
C                      to allow Z0 to float
C F_ST_LYR_HT_REQ_PZ...fraction of expected number of stereo hits required
C                      to pass z criteria
C F_ST_U_LYR_HT_REQ_PZ.fraction of expected number of +stereo hits required
C                      to pass z criteria
C F_ST_V_LYR_HT_REQ_PZ.fraction of expected number of -stereo hits required
C                      to pass z criteria
C AX_AMB_GR_HT_REQ_D0..number of axial ambiguity groups required (indexed
C                      by number expected) to allow D0 to float
C ST_AMB_GR_HT_REQ_D0..number of stereo ambiguity groups required (indexed
C                      by number expected) to allow D0 to float
C ST_AMB_GR_HT_REQ_Z0..number of stereo ambiguity groups required (indexed
C                      by number expected) to allow Z0 to float
C ST_AMB_GR_HT_REQ_PZ..number of stereo ambiguity groups required (indexed
C                      by number expected) to pass z criteria
C AX_LYR_HT_REQ_PD0....number of axial layers required
C                      to pass D0 criteria
C FIRST_AXIAL_LYR......first axial layer (TF layer) for counting 
C LAST_AXIAL_LYR.......last axial layer (TF layer) for counting
C FIRST_AXIAL_AMB_GR...first axial ambiguity group for counting
C LAST_AXIAL_AMB_GR....last axial ambiguity group for counting
C FIRST_STEREO_LYR.....first stereo layer (TF layer) for counting 
C LAST_STEREO_LYR......last stereo layer (TF layer) for counting
C FIRST_STEREO_AMB_GR..first stereo ambiguity group for counting
C LAST_STEREO_AMB_GR...last stereo ambiguity group for counting

C-----------------------------------------------------------------------
C set-up for _ALTERNATE_AMB_GRP_1
C-----------------------------------------------------------------------

C NUMB_GR_ALTER........number of groups to be alternated to make up the
C                      alternate solutions
C                      YOU ARE RESPONSIBLE to make sure that this does not
C                      lead to a MAX_INDX_SOL_SET greater than 
C                      DIM_ALT_SOL_SET (a parameter)
C MAX_INDX_SOL_SET.....maximum of INDX_SOL_SET; 2**NUMB_GR_ALTER

C-----------------------------------------------------------------------
C cuts for _ALTERNATE_AMB_GRP_2
C-----------------------------------------------------------------------

C DIF_SHIFT_CUT........maximum allowed shift in r-phi
C                      between two consecutive group solutions
C DIF_ZMEAS_CUT........maximum allowed z measurement
C                      between two consecutive group solutions
C DIF_SHIFT_SIGMA......sigma used to calculate quality from shift in r-phi
C                      between two consecutive group solutions
C DIF_ZMEAS_SIGMA......sigma used to calculate quality from z measurement
C                      between two consecutive group solutions


C-----------------------------------------------------------------------
C cuts for HIT_DROPPING/ADDING_OTHER  SECTION
C-----------------------------------------------------------------------

C CELCT1...............lower limit cut (in cells) on PULFIT to
C                      drop hits, dropping up to MAX_DROP
C MAX_DROP.............maximum number of hits allowed to be dropped with CELCT1
C CELCT2...............lower limit cut (in cells) on PULFIT to
C                      continue testing to drop more hits; 
C                      below this cut, dropping will end after refit 
C DROP_REQ_REFIT.......threshold of NDROP_NOT_REFIT to require refit
C                      before dropping more hits
C SHORT_REQ_REFIT......force refit if number of hits drop below this amount

C-----------------------------------------------------------------------

#endif
C-----------------------------------------------------------------------

                       INTEGER DIM_GROUP
                       PARAMETER (DIM_GROUP=20)

                       INTEGER DIM_HITS_SOL 
                       PARAMETER (DIM_HITS_SOL=10)

                       INTEGER DIM_HITS_IN_GROUP
                       PARAMETER (DIM_HITS_IN_GROUP=30)

                       INTEGER DIM_ALT_SOL_SET
                       PARAMETER (DIM_ALT_SOL_SET=200)

                       INTEGER DIM_SAV_SOL_SET
                       PARAMETER (DIM_SAV_SOL_SET=1000)

                       INTEGER DIM_DZ_FIX
                       PARAMETER (DIM_DZ_FIX=15)

C-----------------------------------------------------------------------
                       LOGICAL L_FT_DOIT_FULL
                       LOGICAL L_FT_DOIT_TPC
C-----------------------------------------------------------------------
                       REAL ZTMP
                       INTEGER ERROR_CODE
C-----------------------------------------------------------------------
                       INTEGER FLAG_LYR_USED
                       INTEGER HITBSC,HITASC
                       INTEGER TF_LAYR_MIN,TF_LAYR_MAX
                       INTEGER AX_LYR_HIT
                       INTEGER ST_LYR_HIT,ST_U_LYR_HIT,ST_V_LYR_HIT
                       INTEGER HITS_IN_AMB_GR
                       REAL RAD_IN_AMB_GR
                       INTEGER AX_AMB_GR_HIT,ST_AMB_GR_HIT
C-----------------------------------------------------------------------
                       INTEGER ITERATION
                       INTEGER INDX_DZ_FIX
C-----------------------------------------------------------------------
                       INTEGER NUMB_OF_GROUPS
                       INTEGER INDX_SOL_USE
C-----------------------------------------------------------------------
                       INTEGER NUM_HITS_SAV_SOL
                       INTEGER LYR_SAV_SOL
                       INTEGER HITS_SAV_SOL
                       INTEGER L2H_SAV_SOL
                       REAL USED_ROAD_SAV_SOL
                       REAL RES_SAV_SOL
                       REAL AVE_RES_SAV_SOL 
C-----------------------------------------------------------------------
                       INTEGER LYR_FIRST_IN_GR
                       INTEGER LYR_LAST_IN_GR
C-----------------------------------------------------------------------
                       REAL Z0BIAS_1,Z0BIAS_2,Z0BIAS_FULL,Z0BIAS_USE
                       REAL DZRAIL
                       LOGICAL F_T_DZ_WINDOWS
                       INTEGER N_DZ_FIX
                       REAL TRY_DZ_FIX
C-----------------------------------------------------------------------
                       INTEGER NHIT_MIN_TRAK
                       INTEGER NHIT_LIM_D0_CONS
                       REAL CURV_LIM_D0_CONS
C-----------------------------------------------------------------------
                       REAL Z_OVER_LENGTH_FT
                       REAL RADIUS_LIMIT
C-----------------------------------------------------------------------
                       INTEGER N_NOTAIL
                       REAL SIN_ENTR_CLOSE
                       REAL DEWEIGHT_CLOSE_ENUF
                       REAL ROAD_CLOSE_ENUF
                       REAL ROAD_Z_CLOSE_ENUF
                       REAL TIME_LIM_CLOSE_ENUF 
C-----------------------------------------------------------------------
                       INTEGER MAX_ITERATE
                       LOGICAL EXECUTE_AMBIGUITY
                       LOGICAL EXECUTE_ALTERNATE
                       REAL SIN_ENTR_LOC_AMB
                       REAL MAX_RES_DZ_WINDOW
                       INTEGER MIN_LYRS_ACCEPT
                       REAL MIN_DRIFT
                       REAL ROAD_AMB
                       REAL ROAD_AMB_EXTRA_Z
                       REAL MAX_AVE_RES_SOL
                       REAL ROAD_AMB_NO_ALIGN
                       INTEGER MIN_LYRS_ALIGN
                       REAL ALIGN_AMB_FULL_ROAD
                       REAL RES_DIF_SIM_HITS
                       INTEGER MAX_DIFF_HIT
                       INTEGER LEN_DIF_UNIQUE
                       REAL WID_DIF_UNIQUE
                       REAL RES_DIF_UNIQUE
C-----------------------------------------------------------------------
                       INTEGER AX_LYR_HIT_EXPCT
                       INTEGER ST_LYR_HIT_EXPCT
                       INTEGER ST_U_LYR_HIT_EXPCT
                       INTEGER ST_V_LYR_HIT_EXPCT
                       INTEGER SIGNIF_HITS_AMB_GR
                       INTEGER AX_AMB_GR_HIT_EXPCT
                       INTEGER ST_AMB_GR_HIT_EXPCT
                       REAL F_AX_LYR_HT_REQ_D0
                       REAL F_ST_LYR_HT_REQ_D0
                       REAL F_ST_LYR_HT_REQ_Z0
                       REAL F_ST_LYR_HT_REQ_PZ
                       REAL F_ST_U_LYR_HT_REQ_PZ
                       REAL F_ST_V_LYR_HT_REQ_PZ
                       INTEGER AX_AMB_GR_HT_REQ_D0
                       INTEGER ST_AMB_GR_HT_REQ_D0
                       INTEGER ST_AMB_GR_HT_REQ_Z0
                       INTEGER ST_AMB_GR_HT_REQ_PZ
                       INTEGER AX_LYR_HT_REQ_PD0
                       INTEGER FIRST_AXIAL_LYR
                       INTEGER LAST_AXIAL_LYR
                       INTEGER FIRST_AXIAL_AMB_GR
                       INTEGER LAST_AXIAL_AMB_GR
                       INTEGER FIRST_STEREO_LYR
                       INTEGER LAST_STEREO_LYR
                       INTEGER FIRST_STEREO_AMB_GR
                       INTEGER LAST_STEREO_AMB_GR

C-----------------------------------------------------------------------
                       INTEGER NUMB_GR_ALTER
                       INTEGER MAX_INDX_SOL_SET
C-----------------------------------------------------------------------
                       REAL DIF_SHIFT_CUT
                       REAL DIF_ZMEAS_CUT
                       REAL DIF_SHIFT_SIGMA
                       REAL DIF_ZMEAS_SIGMA
C-----------------------------------------------------------------------
                       REAL CELCT1
                       INTEGER MAX_DROP
                       REAL CELCT2
                       INTEGER DROP_REQ_REFIT
                       INTEGER SHORT_REQ_REFIT
C-----------------------------------------------------------------------
	COMMON/FAST_TRAK_COM_0/
     1                      L_FT_DOIT_FULL,
     2                      L_FT_DOIT_TPC

	COMMON/FAST_TRAK_COM_1/
     1                      ZTMP,
     2                      ERROR_CODE

	COMMON/FAST_TRAK_COM_2/
     1                      FLAG_LYR_USED(KLYRDM),
     2                      HITBSC,HITASC,
     3                      TF_LAYR_MIN,TF_LAYR_MAX,
     1                      AX_LYR_HIT,
     2                      ST_LYR_HIT,
     3                      HITS_IN_AMB_GR(DIM_GROUP),
     4                      RAD_IN_AMB_GR(DIM_GROUP),
     5                      AX_AMB_GR_HIT,ST_AMB_GR_HIT

	COMMON/FAST_TRAK_COM_3/
     1                      ITERATION,
     1                      INDX_DZ_FIX
	COMMON/FAST_TRAK_COM_4/
     1                      NUMB_OF_GROUPS,
     2                      INDX_SOL_USE(DIM_GROUP)
	COMMON/FAST_TRAK_COM_5/
     1                      NUM_HITS_SAV_SOL         (2,DIM_GROUP),
     2                      LYR_SAV_SOL (DIM_HITS_SOL,2,DIM_GROUP),
     3                      HITS_SAV_SOL(DIM_HITS_SOL,2,DIM_GROUP),
     4                      L2H_SAV_SOL (DIM_HITS_SOL,2,DIM_GROUP),
     5                      USED_ROAD_SAV_SOL        (2,DIM_GROUP),
     6                      RES_SAV_SOL (DIM_HITS_SOL,2,DIM_GROUP),
     7                      AVE_RES_SAV_SOL          (2,DIM_GROUP) 

	COMMON/FAST_TRAK_COM_6/
     1                      LYR_FIRST_IN_GR(DIM_GROUP),
     2                      LYR_LAST_IN_GR(DIM_GROUP)
	COMMON/FAST_TRAK_COM_7/
     1                      Z0BIAS_1,Z0BIAS_2,Z0BIAS_FULL,Z0BIAS_USE,
     1                      DZRAIL,
     1                      F_T_DZ_WINDOWS,
     1                      N_DZ_FIX,TRY_DZ_FIX(DIM_DZ_FIX),
     1                      NHIT_MIN_TRAK,
     1                      NHIT_LIM_D0_CONS,
     1                      CURV_LIM_D0_CONS,
     1                      Z_OVER_LENGTH_FT,
     1                      RADIUS_LIMIT,
     1                      N_NOTAIL,
     1                      SIN_ENTR_CLOSE,
     1                      DEWEIGHT_CLOSE_ENUF,
     1                      ROAD_CLOSE_ENUF,
     1                      ROAD_Z_CLOSE_ENUF,
     1                      TIME_LIM_CLOSE_ENUF, 
     1                      MAX_ITERATE,
     1                      EXECUTE_AMBIGUITY,
     1                      EXECUTE_ALTERNATE,
     1                      SIN_ENTR_LOC_AMB,
     1                      MAX_RES_DZ_WINDOW,
     1                      MIN_LYRS_ACCEPT,
     1                      MIN_DRIFT,
     1                      ROAD_AMB,
     1                      ROAD_AMB_EXTRA_Z,
     1                      MAX_AVE_RES_SOL,
     1                      ROAD_AMB_NO_ALIGN,
     1                      MIN_LYRS_ALIGN,
     1                      ALIGN_AMB_FULL_ROAD,
     1                      RES_DIF_SIM_HITS,
     1                      MAX_DIFF_HIT,
     1                      LEN_DIF_UNIQUE,
     1                      WID_DIF_UNIQUE,
     1                      RES_DIF_UNIQUE,
     1                      AX_LYR_HIT_EXPCT,
     1                      ST_LYR_HIT_EXPCT,
     1                      ST_U_LYR_HIT_EXPCT,ST_V_LYR_HIT_EXPCT,
     1                      SIGNIF_HITS_AMB_GR,
     1                      AX_AMB_GR_HIT_EXPCT,
     1                      ST_AMB_GR_HIT_EXPCT,
     1                      F_AX_LYR_HT_REQ_D0,
     1                      F_ST_LYR_HT_REQ_D0,
     1                      F_ST_LYR_HT_REQ_Z0,
     1                      F_ST_LYR_HT_REQ_PZ,
     1                      F_ST_U_LYR_HT_REQ_PZ,F_ST_V_LYR_HT_REQ_PZ,
     1                      AX_AMB_GR_HT_REQ_D0(0:DIM_GROUP),
     1                      ST_AMB_GR_HT_REQ_D0(0:DIM_GROUP),
     1                      ST_AMB_GR_HT_REQ_Z0(0:DIM_GROUP),
     1                      ST_AMB_GR_HT_REQ_PZ(0:DIM_GROUP),
     1                      AX_LYR_HT_REQ_PD0,
     1                      FIRST_AXIAL_LYR,
     1                      LAST_AXIAL_LYR,
     1                      FIRST_AXIAL_AMB_GR,
     1                      LAST_AXIAL_AMB_GR,
     1                      FIRST_STEREO_LYR,
     1                      LAST_STEREO_LYR,
     1                      FIRST_STEREO_AMB_GR,
     1                      LAST_STEREO_AMB_GR,
     1                      NUMB_GR_ALTER,
     1                      MAX_INDX_SOL_SET,
     1                      DIF_SHIFT_CUT,
     1                      DIF_ZMEAS_CUT,
     1                      DIF_SHIFT_SIGMA,
     1                      DIF_ZMEAS_SIGMA,
     1                      CELCT1,
     1                      MAX_DROP,
     1                      CELCT2,
     1                      DROP_REQ_REFIT,
     1                      SHORT_REQ_REFIT

C-----------------------------------------------------------------------

	SAVE /FAST_TRAK_COM_0/,/FAST_TRAK_COM_1/,/FAST_TRAK_COM_2/
	SAVE /FAST_TRAK_COM_3/,/FAST_TRAK_COM_4/,/FAST_TRAK_COM_5/
	SAVE /FAST_TRAK_COM_6/,/FAST_TRAK_COM_7/






