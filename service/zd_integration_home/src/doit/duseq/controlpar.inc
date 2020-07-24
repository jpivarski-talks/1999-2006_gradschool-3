*
* $Id: controlpar.inc,v 1.5 2003/03/05 17:35:51 dpp Exp $
*
* $Log: controlpar.inc,v $
* Revision 1.5  2003/03/05 17:35:51  dpp
*      -> TPC in status bits
*
* Revision 1.4  2002/09/24 22:56:14  dpp
*      -> new control constant names for existing do contraint control
*      -> new d0 constraint control to allow holding input value
*
* Revision 1.3  2002/09/05 19:15:26  dpp
*      -> add values for DRIFTDIR
*      -> define new "wire fit" and "loc amb" stacks
*      -> add names for substaks
*      -> change name of the TMP2 stack to LIM_RAD_O
*      -> add control of SFHITS
*
* Revision 1.2  2002/05/23 14:26:02  dpp
*      -> parameters to define sub-staks in a global stak
*          rather than having the stak be multidimensioned
*      -> more parameters to access values in TFTRAK
*      -> new flag to fetch status bits from storage
*
* Revision 1.1  2001/11/19 23:19:08  dpp
*      -> NEW
*      -> collect all the control parameters here
*
*
#if !defined(CLEO_NOCOMDOC)

* LVLTYP_PAT_REC   - control of hit finding LVLTYP: pattern rec requested
* LVLTYP_TFXFIT    - control of hit finding LVLTYP: TFXFIT requested
* LVLTYP_ACCESS    - control of hit finding LVLTYP: access internals
* HITTYP_WIRE      - control of hit finding HITTYP: WIREs are requested
* HITTYP_HIT       - control of hit finding HITTYP: HITs are requested
* FITTYP_CF        - control of hit finding FITTYP: CF use
* FITTYP_LF        - control of hit finding FITTYP: LF use

* DRIFTDIR_PHI     - indicates layer has drift in phi, like a drift chamber
* DRIFTDIR_Z       - indicates layer has drift in Z, like a TPC 
* DRIFTDIR_NONE    - indicates layer has no drift measurement

* H2F_ZERO_FIT     - HIT_TO_FIT: zero the fit and HITTRK
* H2F_ZERO_POINTER - HIT_TO_FIT: zero HITTRK and TRKFIT
* H2F_TEST_POINTER - HIT_TO_FIT: test either HITTRK and TRKFIT
* H2F_NEW_ENTRY    - HIT_TO_FIT: make new entry in fit
* H2F_UPDATE_ENTRY - HIT_TO_FIT: update existing entry in fit
* H2F_DEWEIGHT     - HIT_TO_FIT: deweight existing entry in fit
* H2F_FULLWEIGHT   - HIT_TO_FIT: reset all entries to full weight
* H2F_CHANGE_P_I   - HIT_TO_FIT: change program indicator
* H2F_REMOVE_ENTRY - HIT_TO_FIT: remove , by fit index

* SFHITS_ONLY_SF   - SFHITS: only use the input sfchan wires
* SFHITS_FULL      - SFHITS: traditional full execution, fill in other hits

* FIT_ACCESS_NFIT       - FIT_ACCESS: provide NFIT
* FIT_ACCESS_NHITLF     - FIT_ACCESS: provide NHITLF
* FIT_ACCESS_NHIT3F     - FIT_ACCESS: provide NHIT3F
* FIT_ACCESS_CHICF      - FIT_ACCESS: provide CHICF
* FIT_ACCESS_KAPCF      - FIT_ACCESS: provide KAPCF
* FIT_ACCESS_D0CF       - FIT_ACCESS: provide D0CF
* FIT_ACCESS_Y0LF       - FIT_ACCESS: provide Y0LF
* FIT_ACCESS_Z0BIAS     - FIT_ACCESS: provide Z0BIAS
* FIT_ACCESS_Z0BIAS_SET - FIT_ACCESS: fill new value of Z0BIAS
* FIT_ACCESS_NLFDRA     - FIT_ACCESS: provide Num Layers Fit DR Axial
* FIT_ACCESS_NLFDRU     - FIT_ACCESS: provide Num Layers Fit DR stereo U
* FIT_ACCESS_NLFDRV     - FIT_ACCESS: provide Num Layers Fit DR stereo V
* FIT_ACCESS_RPRLIM_SET - FIT_ACCESS: fill new value of RPRLIM
 


* KABG2_CLEAR      - KABG2 - clear all kinks
* KABG2_PHI        - KABG2 - create kink in phi
* KABG2_CURV       - KABG2 - create kink in curvature
* KABG2_ELOSS      - KABG2 - create energy loss
* KABG2_CALC       - KABG2 - calculate transformation
* KABG2_FLOAT      - KABG2 - kink variable floats in fit
* KABG2_FIX        - KABG2 - kink variable is fixed 
* KABG2_XI_RECALC  - KABG2 - recalculate XI 
* KABG2_XI_OK      - KABG2 - XI is OK, do not recalculate 

* FT_DEL_BAD       - FAST_TRAK_DEL_BAD_AND_ADD - delete bad hits
* FT_NO_DELETE     - FAST_TRAK_DEL_BAD_AND_ADD - do not delete bad hits
* FT_USE_CELL_FRAC - FAST_TRAK_DEL_BAD_AND_ADD - use cell fraction roads
* FT_USE_SIGMAS    - FAST_TRAK_DEL_BAD_AND_ADD - use layer sigma roads
* FT_D0CON_SET     - FAST_TRAK_CHECK_HIT_LAYRS - allow D0 constraint
* FT_D0CON_OFF     - FAST_TRAK_CHECK_HIT_LAYRS - do not allow D0 constraint
* FT_D0CON_HOLD    - FAST_TRAK_CHECK_HIT_LAYRS - hold input value of DO_CONS
* FT_SET_Z0BIAS_USE- FAST_TRAK_CHECK_HIT_LAYRS - set Z0BIAS to Z0BIAS_USE 
* FT_SET_Z0BIAS    - FAST_TRAK_CHECK_HIT_LAYRS - set new value using criteria
* FT_NO_MORE_Z0BIAS- FAST_TRAK_CHECK_HIT_LAYRS - do not allow more Z0 bias
* FTR_BEFORE_TFXFIT- FAST_TRAK_RECALL_DZ_WINDO - processing before TFXFIT
* FTR_AFTER_TFXFIT - FAST_TRAK_RECALL_DZ_WINDO - processing after TFXFIT

*   NOTE: must set up new sub-staks in tfstkc.F
*   NOTE: array length must be suffucient for the sum of the stack lengths   
* STAK_PRIMARY     - Primary stack index (not simply a matching number) 
*                      FAST_TRAK uses this for the loop over GROUP SET 
*                      and to tranfer the output.
* STAK_SECONDARY   - Secondary stack (not simply a matching number)
*                      FAST_TRAK uses this stack number for the 
*                      resprentation in the inner loop
*                      Used in other routines for a local stack  number
* STAK_TMP1        - temporary stack 1 (not simply a matching number)
*                      can be used in various places to store a representation
* STAK_LIM_RAD_O   - stack for the limit radius output
* STAK_FT_WF_OUT   - stack for FAST_TRAK wire fit output
* STAK_FT_LA_OUT   - stack for FAST_TRAK local ambiguity resolution output
* LEN_STAK_PRIM    - length of stak_primary, 
* LEN_STAK_SECO    - length of stak_secondary, 
* LEN_STAK_TMP1    - length of stak_tmp1, 
* LEN_STAK_L_R_O   - length of stak_lim_rad_o
* LEN_STAK_WF_O    - length of stak_ft_wf_out
* LEN_STAK_LA_O    - length of stak_ft_la_out
* NAM_STAK_PRIM    - NAME of stak_primary, 
* NAM_STAK_SECO    - NAME of stak_secondary, 
* NAM_STAK_TMP1    - NAME of stak_tmp1, 
* NAM_STAK_L_R_O   - NAME of stak_lim_rad_o
* NAM_STAK_WF_O    - NAME of stak_ft_wf_out
* NAM_STAK_LA_O    - NAME of stak_ft_la_out

* STAK_PICK_FIRST  - pick the first element off the stack
* STAK_PICK_NEXT   - pick the next  element off the stack

* CDT_INCR_SEED    - increment the seed track number stored in CDTRACK
* CDT_NTRKCD       - provide NTRKCD number of distinct tracks

* P_I_FAST_TRAK            - PROGRAM INDICATOR for FAST_TRAK
* P_I_F_T_AMB_GRP_TO_TRAK  - PROGRAM INDICATOR for FAST_TRAK_AMB_GRP_TO_TRAK 
* P_I_F_T_CLOSE_FIT_STORE  - PROGRAM INDICATOR for FAST_TRAK_CLOSE_FIT_STORE
* P_I_F_T_DEL_BAD_AND_ADD  - PROGRAM INDICATOR for FAST_TRAK_DEL_BAD_AND_ADD
* P_I_F_T_FIT_WIRE_POSITN  - PROGRAM INDICATOR for FAST_TRAK_FIT_WIRE_POSITN
* P_I_TFXFIT               - PROGRAM INDICATOR for TFXFIT
* P_I_TFXFIT_REDUCE        - PROGRAM INDICATOR for TFXFIT, but reduced drift
* P_I_REPLACE_Z_HITS       - PROGRAM INDICATOR for REPLACE_Z_HITS
* P_I_BOOT_SI_Z_HITS       - PROGRAM INDICATOR for BOOT_SI_Z_HITS

* EXB_DOIT_FAST -  NEXPT bit value to designate DoitFast
* EXB_DOIT_FULL -  NEXPT bit value to designate full production DOIT  
* EXB_DOIT_NEW   - NEXPT bit value to designate NOT running some new stuff
* EXB_SF_DATA_CUT- NEXPT bit value to designate using SF "data" time cuts
* EXB_SF_STARTUP - NEXPT bit value to designate using SF "STARTUP" cut set
* EXB_DOIT_NOCHI - NEXPT bit value to designate NOT using DUFIND CHICF cut
* EXB_CALIB      - NEXPT bit value to designate special calibration settings
* EXB_DOIT_TPC   - NEXPT bit value to designate TPC code changes 

* SFACCESS_NLAYSF     - SF provide NLAYSF
* SFACCESS_NCHAIN     - SF provide NCHAIN

* TFXF_NORMAL_STAND     - TFXFIT - use normal road in standard
* TFXF_WIDE_STAND       - TFXFIT - use wider road in standard

* CONTROL_ACCESS_SET_TFXS  -  set the TFXFIT view switch
#endif


      INTEGER   LVLTYP_PAT_REC   , LVLTYP_TFXFIT   , LVLTYP_ACCESS
      INTEGER   HITTYP_WIRE      , HITTYP_HIT
      INTEGER   FITTYP_CF        , FITTYP_LF

      PARAMETER (LVLTYP_PAT_REC  = 1)
      PARAMETER (LVLTYP_TFXFIT   = 2)
      PARAMETER (LVLTYP_ACCESS   = 3)
      PARAMETER (HITTYP_WIRE     = 1)
      PARAMETER (HITTYP_HIT      = 2)
      PARAMETER (FITTYP_CF       = 1)
      PARAMETER (FITTYP_LF       = 2)

      INTEGER DRIFTDIR_PHI       , DRIFTDIR_Z       , DRIFTDIR_NONE

      PARAMETER (DRIFTDIR_PHI    = 1)
      PARAMETER (DRIFTDIR_Z      = 2)
      PARAMETER (DRIFTDIR_NONE   = 0)

      INTEGER H2F_ZERO_FIT,H2F_ZERO_POINTER,H2F_TEST_POINTER
      INTEGER H2F_NEW_ENTRY,H2F_UPDATE_ENTRY,H2F_REMOVE_ENTRY
      INTEGER H2F_DEWEIGHT,H2F_FULLWEIGHT
      INTEGER H2F_CHANGE_P_I

      PARAMETER (H2F_ZERO_FIT    = 0)
      PARAMETER (H2F_ZERO_POINTER= 1)
      PARAMETER (H2F_TEST_POINTER= 2)
      PARAMETER (H2F_NEW_ENTRY   = 5)
      PARAMETER (H2F_UPDATE_ENTRY= 6)
      PARAMETER (H2F_DEWEIGHT    = 7)
      PARAMETER (H2F_CHANGE_P_I  = 8)
      PARAMETER (H2F_FULLWEIGHT  = 9)
      PARAMETER (H2F_REMOVE_ENTRY=10)

      INTEGER SFHITS_ONLY_SF,SFHITS_FULL
      PARAMETER (SFHITS_ONLY_SF  =0)
      PARAMETER (SFHITS_FULL     =1)

      INTEGER FIT_ACCESS_NFIT,  FIT_ACCESS_CHICF
      INTEGER FIT_ACCESS_KAPCF
      INTEGER FIT_ACCESS_D0CF,  FIT_ACCESS_Y0LF
      INTEGER FIT_ACCESS_NHITLF,FIT_ACCESS_NHIT3F
      INTEGER FIT_ACCESS_Z0BIAS,FIT_ACCESS_Z0BIAS_SET  
      INTEGER FIT_ACCESS_NLFDRA,FIT_ACCESS_NLFDRU
      INTEGER FIT_ACCESS_NLFDRV,FIT_ACCESS_RPRLIM_SET



      PARAMETER (FIT_ACCESS_NFIT      =101)
      PARAMETER (FIT_ACCESS_CHICF     =102)
      PARAMETER (FIT_ACCESS_KAPCF     =103)
      PARAMETER (FIT_ACCESS_D0CF      =104)
      PARAMETER (FIT_ACCESS_Y0LF      =105)
      PARAMETER (FIT_ACCESS_NHITLF    =106)
      PARAMETER (FIT_ACCESS_NHIT3F    =107)
      PARAMETER (FIT_ACCESS_Z0BIAS    =108)
      PARAMETER (FIT_ACCESS_Z0BIAS_SET=109)
      PARAMETER (FIT_ACCESS_NLFDRA    =110)
      PARAMETER (FIT_ACCESS_NLFDRU    =111)
      PARAMETER (FIT_ACCESS_NLFDRV    =112)
      PARAMETER (FIT_ACCESS_RPRLIM_SET=113)


      INTEGER 
     + 	KABG2_CLEAR ,KABG2_PHI   ,KABG2_CURV   ,KABG2_ELOSS ,
     +  KABG2_CALC  ,KABG2_FLOAT ,KABG2_FIX    ,
     +  KABG2_XI_RECALC          ,KABG2_XI_OK         

      PARAMETER (KABG2_CLEAR     = 1)
      PARAMETER (KABG2_PHI       = 2)
      PARAMETER (KABG2_CURV      = 3)
      PARAMETER (KABG2_ELOSS     = 4)
      PARAMETER (KABG2_CALC      = 5)
      PARAMETER (KABG2_FLOAT     = 6)
      PARAMETER (KABG2_FIX       = 7)
      PARAMETER (KABG2_XI_RECALC = 8)
      PARAMETER (KABG2_XI_OK     = 9)



      INTEGER 
     +  FT_DEL_BAD         ,FT_NO_DELETE       ,
     +  FT_USE_CELL_FRAC   ,FT_USE_SIGMAS      ,
     +  FT_D0CON_SET       ,FT_D0CON_OFF       ,FT_D0CON_HOLD      ,
     +  FT_SET_Z0BIAS_USE  ,FT_SET_Z0BIAS      ,FT_NO_MORE_Z0BIAS  ,
     +  FTR_BEFORE_TFXFIT  ,FTR_AFTER_TFXFIT

      PARAMETER (FT_DEL_BAD        =31)
      PARAMETER (FT_NO_DELETE      =32)
      PARAMETER (FT_USE_CELL_FRAC  =33)
      PARAMETER (FT_USE_SIGMAS     =34)
      PARAMETER (FT_D0CON_SET      =35)
      PARAMETER (FT_D0CON_OFF      =36)
      PARAMETER (FT_D0CON_HOLD     =37)
      PARAMETER (FT_SET_Z0BIAS_USE =38)
      PARAMETER (FT_SET_Z0BIAS     =39)
      PARAMETER (FT_NO_MORE_Z0BIAS =40)
      PARAMETER (FTR_BEFORE_TFXFIT =41)
      PARAMETER (FTR_AFTER_TFXFIT  =42)

      INTEGER STAK_PRIMARY,LEN_STAK_PRIM
      PARAMETER (STAK_PRIMARY      = 1)
      PARAMETER (LEN_STAK_PRIM     =50)
      CHARACTER*15 NAM_STAK_PRIM
      PARAMETER (NAM_STAK_PRIM='primary        ')

      INTEGER STAK_SECONDARY,LEN_STAK_SECO
      PARAMETER (STAK_SECONDARY    = 2)
      PARAMETER (LEN_STAK_SECO     =50)
      CHARACTER*15 NAM_STAK_SECO
      PARAMETER (NAM_STAK_SECO='secondary      ')

      INTEGER STAK_TMP1,LEN_STAK_TMP1
      PARAMETER (STAK_TMP1         = 3)
      PARAMETER (LEN_STAK_TMP1     = 1)
      CHARACTER*15 NAM_STAK_TMP1
      PARAMETER (NAM_STAK_TMP1='temporary 1    ')

      INTEGER STAK_LIM_RAD_O,LEN_STAK_L_R_O
      PARAMETER (STAK_LIM_RAD_O     = 4)
      PARAMETER (LEN_STAK_L_R_O     = 1)
      CHARACTER*15 NAM_STAK_L_R_O
      PARAMETER (NAM_STAK_L_R_O='ltd rad output ')

      INTEGER STAK_FT_WF_OUT,LEN_STAK_WF_O
      PARAMETER (STAK_FT_WF_OUT    = 5)
      PARAMETER (LEN_STAK_WF_O     = 2)
      CHARACTER*15 NAM_STAK_WF_O
      PARAMETER (NAM_STAK_WF_O='wire fit output')

      INTEGER STAK_FT_LA_OUT,LEN_STAK_LA_O
      PARAMETER (STAK_FT_LA_OUT    = 6)
      PARAMETER (LEN_STAK_LA_O     = 2)
      CHARACTER*15 NAM_STAK_LA_O
      PARAMETER (NAM_STAK_LA_O='loc amb output ')


      INTEGER STAK_PICK_FIRST
      PARAMETER (STAK_PICK_FIRST = 1)
      INTEGER STAK_PICK_NEXT
      PARAMETER (STAK_PICK_NEXT  = 2)

      INTEGER CDT_INCR_SEED  ,CDT_NTRKCD
      PARAMETER (CDT_INCR_SEED   = 1)
      PARAMETER (CDT_NTRKCD      = 2)


      INTEGER P_I_FAST_TRAK
      INTEGER P_I_F_T_AMB_GRP_TO_TRAK, P_I_F_T_CLOSE_FIT_STORE
      INTEGER P_I_F_T_DEL_BAD_AND_ADD, P_I_F_T_FIT_WIRE_POSITN
      INTEGER P_I_TFXFIT,              P_I_TFXFIT_REDUCE
      INTEGER P_I_REPLACE_Z_HITS,      P_I_BOOT_SI_Z_HITS

      PARAMETER (P_I_FAST_TRAK                =10)
      PARAMETER (P_I_F_T_AMB_GRP_TO_TRAK      =11)
      PARAMETER (P_I_F_T_CLOSE_FIT_STORE      =12)
      PARAMETER (P_I_F_T_DEL_BAD_AND_ADD      =13)
      PARAMETER (P_I_F_T_FIT_WIRE_POSITN      =14)
      PARAMETER (P_I_TFXFIT                   =21)
      PARAMETER (P_I_TFXFIT_REDUCE            =22)
      PARAMETER (P_I_REPLACE_Z_HITS           =31)
      PARAMETER (P_I_BOOT_SI_Z_HITS           =41)

      INTEGER EXB_DOIT_FAST,   EXB_DOIT_FULL,    EXB_DOIT_NEW
      INTEGER EXB_SF_DATA_CUT, EXB_SF_STARTUP,   EXB_DOIT_NOCHI
      INTEGER EXB_CALIB,       EXB_DOIT_TPC

      PARAMETER (EXB_DOIT_FAST=   1)
      PARAMETER (EXB_DOIT_FULL=  32)

      PARAMETER (EXB_DOIT_NEW=   64)
      PARAMETER (EXB_SF_DATA_CUT= 2)
      PARAMETER (EXB_SF_STARTUP=  4)
      PARAMETER (EXB_DOIT_NOCHI=  8)
      PARAMETER (EXB_CALIB=     256)
      PARAMETER (EXB_DOIT_TPC= 1024)

      INTEGER SFACCESS_NLAYSF,  SFACCESS_NCHAIN
      PARAMETER (SFACCESS_NLAYSF=    1)
      PARAMETER (SFACCESS_NCHAIN=    2)

      INTEGER TFXF_NORMAL_STAND, TFXF_WIDE_STAND
      PARAMETER (TFXF_NORMAL_STAND   =1)
      PARAMETER (TFXF_WIDE_STAND     =2)


      INTEGER CONTROL_ACCESS_SET_TFXS
      PARAMETER (CONTROL_ACCESS_SET_TFXS=   1)
