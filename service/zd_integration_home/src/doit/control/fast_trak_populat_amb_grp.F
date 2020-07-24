* $Id: fast_trak_populat_amb_grp.F,v 1.11 2003/03/05 17:10:17 dpp Exp $
*
* $Log: fast_trak_populat_amb_grp.F,v $
* Revision 1.11  2003/03/05 17:10:17  dpp
*      -> diagnostics
*      -> add maximum average residual for local amb solution
*      -> add variable extra z requirement in ambiguity solution
*
* Revision 1.10  2002/09/05 19:33:23  dpp
*      -> remove any code involving "loop over group set"
*
* Revision 1.9  2002/05/23 14:16:31  dpp
*     -> use the radius limit when selecting hits
*
* Revision 1.8  2000/12/19 23:42:23  dpp
*      -> limit of Z outside endplate
*      -> residual maximum when in mode to limit width of window in tan(dip)
*
* Revision 1.7  2000/10/02 20:59:03  dpp
*      -> require valid hit status
*
* Revision 1.6  2000/04/26 14:32:23  dpp
*      -> use call to RESIDUALS instead of RESICF
*
* Revision 1.5  2000/01/06 22:12:01  dpp
* allow large slant solutions
* (that are consistant with large d(res)/d(r) )
*
* Revision 1.4  1999/11/23 23:24:58  dpp
* allow cut in entrance angle
*
* Revision 1.3  1999/11/17 21:47:16  dpp
* fix error in index deep inside the compare of hit list
*    from CUR and SAV
*
* Revision 1.2  1999/11/14 18:21:23  dpp
* new section for group soltion overlaps; eliminates subsets
*
* Revision 1.1  1999/11/03 22:42:29  dpp
* new
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_POPULAT_AMB_GRP
C.......................................................................
C.
C. FAST_TRAK_POPULAT_AMB_GRP - populate the ambiguity groups and
C.                             save the best solutions
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split of of FAST_TRAK 26-Oct-99
C. LAST MOD  : 15-Sept-88
C.
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

#include "doit/sfseq/xts_cntl.inc"

#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

* INDX_GROUP...........counter of ambiguity group number
                       INTEGER INDX_GROUP
* HIT_AMB_GR...........sequental hit number in ambiguity group
                       INTEGER HIT_AMB_GR 
* HIT2_AMB_GR..........another sequental hit number in ambiguity group
                       INTEGER HIT2_AMB_GR 
* HIT_C_AMB_GR.........seq hit number in the current ambiguity group
                       INTEGER HIT_C_AMB_GR
* HIT_S_AMB_GR.........seq hit number in the saved ambiguity group
                       INTEGER HIT_S_AMB_GR 

C-----------------------------------------------------------------------
C temp values
C-----------------------------------------------------------------------

* LYR_NUMBER...........TF layer number
                       INTEGER LYR_NUMBER
* LOGICAL_LYR_IN_HITS..logical layer number in "road" data structure
                       INTEGER LOGICAL_LYR_IN_HITS
* AMB_LYR_NUM..........layer number within the ambiguity group
                       INTEGER AMB_LYR_NUM
                       
C-----------------------------------------------------------------------
C local varables for the local ambiguity resolution
C-----------------------------------------------------------------------
                       INTEGER SELECT_FROM_STAK 
* HITS_USEDIN_AMB_GR...number of hits found in an ambiguity group
                       INTEGER HITS_USEDIN_AMB_GR
* LYRS_USEDIN_AMB_GR...number of layers found in an ambiguity group
                       INTEGER LYRS_USEDIN_AMB_GR
* SIN_ENTR_GR..........sine of entrance angle for the group
                       REAL SIN_ENTR_GR
* LYR_NUMBER_PREV......previous TF layer number
                       INTEGER LYR_NUMBER_PREV
* SEQ_HIT_NUM_IN_HITS..sequential hit number in the "road" data structure
                       INTEGER SEQ_HIT_NUM_IN_HITS
* HIT_RESID............{point - fitted track}
                       REAL HIT_RESID 
* Z_HIT_RESID..........{point - fitted track} in Z used in FITTYP_CF mode
                       REAL Z_HIT_RESID 
* SAV_HIT_NUM_AMB_GR...saved {SEQ_HIT_NUM_IN_HITS} for ambiguity group
                       INTEGER SAV_HIT_NUM_AMB_GR(DIM_HITS_IN_GROUP)
* SAV_LYR_AMB_GR.......saved TF layer number for ambiguity group
                       INTEGER SAV_LYR_AMB_GR(DIM_HITS_IN_GROUP)
* SAV_RESID_AMB_GR.....saved {HIT_RESID} for ambiguity group
                       REAL SAV_RESID_AMB_GR(DIM_HITS_IN_GROUP)
* SAV_Z_RESID_AMB_GR...saved {HIT_RESID} in Z for ambiguity group
                       REAL SAV_Z_RESID_AMB_GR(DIM_HITS_IN_GROUP)
* MIN_RES_DIF_LYR......minimum residual difference for hits in tested layer 
                       REAL MIN_RES_DIF_LYR
* USED_ROAD_CUR_SOL....used road of hits in CURRENT local ambiguity solution
*                      ie, maxiumum MIN_RES_DIF_LYR for layers in tested group 
                       REAL USED_ROAD_CUR_SOL
* NUM_HITS_CUR_SOL.....number of hits in CURRENT local ambiguity solution
                       INTEGER NUM_HITS_CUR_SOL
* NUM_HITS_SATURATE....=.TRUE. if NUM_HITS_CUR_SOL is saturated
                       LOGICAL NUM_HITS_SATURATE
* AVE_RES_CUR_SOL......average residual
*                      of hits in CURRENT local ambiguity solution
                       REAL AVE_RES_CUR_SOL 
* RES_DIF_HIT..........difference of SAV_RESID_AMB_GR between two hits
                       REAL RES_DIF_HIT
* Z_RES_DIF_HIT........difference of SAV_Z_RESID_AMB_GR between two hits
                       REAL Z_RES_DIF_HIT
* HIT_IN_ROAD..........=.TRUE. when hit passed ambiguity road requirements
                       LOGICAL HIT_IN_ROAD
* END_OF_LAYER.........=.TRUE. after last hit in layer is processed
                       LOGICAL END_OF_LAYER
* FOUND_HIT_IN_LAYER...=.TRUE. if a good hit has been found for a layer
                       LOGICAL FOUND_HIT_IN_LAYER 
* SAVEHIT..............=.TRUE. if hit is to be saved in loc amb solution
                       LOGICAL SAVEHIT
* SAVEHIT_NEW_LAYER....=.TRUE. if this is the first hit found in current layer
                       LOGICAL SAVEHIT_NEW_LAYER 
* HITS_CUR_SOL.........seq hit number in "road" data structure 
*                      of hits in CURRENT local ambiguity solution
                       INTEGER HITS_CUR_SOL(DIM_HITS_SOL)
* LYR_CUR_SOL..........layer num of hits in CURRENT local ambiguity solution
                       INTEGER LYR_CUR_SOL(DIM_HITS_SOL)
* L2H_CUR_SOL..........map from layer-within-group to
*                      seq hit number in "road" data structure
*                      used for comparing hits lists of solutions
                       INTEGER L2H_CUR_SOL(DIM_HITS_SOL)
* RES_CUR_SOL..........residual of hits in CURRENT local ambiguity solution
                       REAL RES_CUR_SOL(DIM_HITS_SOL)
* MIN_LYR_CUR_SOL......minimum TF layer in CURRENT local ambiguity solution
                       INTEGER MIN_LYR_CUR_SOL
* MAX_LYR_CUR_SOL......maximum TF layer in CURRENT local ambiguity solution
                       INTEGER MAX_LYR_CUR_SOL
* RES_MIN_LYR_CUR_SOL..residual if hit in minimum TF layer 
                       REAL RES_MIN_LYR_CUR_SOL
* RES_MAX_LYR_CUR_SOL..residual if hit in maximum TF layer
                       REAL RES_MAX_LYR_CUR_SOL
* SLOPE_MAX_MIN........slope, d(res)/d(radius) from min to max layer
                       REAL SLOPE_MAX_MIN
* DEV_LINE.............deviation of a hit from the line from min to max
                       REAL DEV_LINE
* MAX_DEV_LINE.........maximum deviation of hits from the line from min to max
                       REAL MAX_DEV_LINE
* GOOD_SOLUTION_1......=.TRUE. if the solution to local ambiguity is
*                      good and could be saved
                       LOGICAL GOOD_SOLUTION_1 
* GOOD_SOLUTION_2......=.TRUE. if the solution to local ambiguity is 
*                      still good and could be saved
                       LOGICAL GOOD_SOLUTION_2
* SHIFT_INTO_1ST.......=1 if current solution is to be shifted into best
*                      =-1 if decision is made to not shift into best
*                      =0 is decision is not made
                       INTEGER SHIFT_INTO_1ST
* SHIFT_INTO_2ND.......=1 if current solution is to be shifted into 2ND best
*                      =-1 if decision is made to not shift into 2ND best
*                      =0 is decision is not made
                       INTEGER SHIFT_INTO_2ND
* BUMP_1ST_INTO_2ND....=.TRUE. best solution should be dropped into 2nd best
                       LOGICAL BUMP_1ST_INTO_2ND 
* DIFFERENT_HIT_LIST...=.TRUE. is hit list of current and best are different
                       LOGICAL DIFFERENT_HIT_LIST 
* N_DIFF_HIT...........number of different hits when comparing two solutions
                       INTEGER N_DIFF_HIT
* N_UNIQUE_CUR.........number of layers that are unique to current solution
                       INTEGER N_UNIQUE_CUR
* N_UNIQUE_SAV.........number of layers that are unique to saved solution
                       INTEGER N_UNIQUE_SAV
* INCR_C...............=.TRUE. if the index for current is to be incremented
                       LOGICAL INCR_C
* INCR_S...............=.TRUE. if the index for SAVED is to be incremented
                       LOGICAL INCR_S
* INCR_SOMETHING.......=.TRUE. if somethinbg was incremented
                       LOGICAL INCR_SOMETHING 
* ADR_CUR_SOL..........hit address (IADRHT(HITS...)) 
*                      of hits in CURRENT local ambiguity solution
                       INTEGER ADR_CUR_SOL
* ADR_SAV_SOL..........temporary hit address (IADRHT(HITS...)) 
*                      of hits in SAVED local ambiguity solution
                       INTEGER ADR_SAV_SOL
* LOAD_BEST............=.TRUE. if the hits from best solution should be added
                       LOGICAL LOAD_BEST
* LEN_DIF_SOL..........length difference between two solutions
                       INTEGER LEN_DIF_SOL
* WID_DIF_SOL..........width (used road) difference between two solutions
                       REAL WID_DIF_SOL
* RES_DIF_SOL..........residual difference between two solutions
                       REAL RES_DIF_SOL

                       INTEGER IGO
C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

 1003 FORMAT(
     1' --------------------------------------------------------------')

C-----------------------------------------------------------------------
C  loop over the AMBIGUITY GROUPS
C-----------------------------------------------------------------------
      DO 176 INDX_GROUP=1,NUMB_OF_GROUPS
C-----------------------------------------------------------------------
C  clear the local ambiguity group solution by
C    zeroing the number of hits in the saved solutions
C  set the used index to zero
C  zero hits and layers found in the group
C-----------------------------------------------------------------------
        CALL VZERO(NUM_HITS_SAV_SOL(1,INDX_GROUP),2)
        INDX_SOL_USE(INDX_GROUP)=0
        
        HITS_USEDIN_AMB_GR=0
        LYRS_USEDIN_AMB_GR=0
        
        LYR_NUMBER=LYR_LAST_IN_GR(INDX_GROUP)
        SIN_ENTR_GR=RTF(LYR_NUMBER)*KAPCF
        
#if defined(CLEO_FTDIAG)
        PRINT 1003
        PRINT 1103,INDX_GROUP,
     2       LYR_FIRST_IN_GR(INDX_GROUP),
     3       LYR_LAST_IN_GR(INDX_GROUP),
     4       MIN_LYRS_ACCEPT,
     5       SIN_ENTR_GR,SIN_ENTR_LOC_AMB,
     6       RTF(LYR_NUMBER),RADIUS_LIMIT
 1103   FORMAT(' FAST_T:  GROUP:',I3,'  TF-layers ',I4,' to',I4,
     1       ', req',I3,' lyrs w/hits',' sENT=',F5.2,' cut',F5.2,
     1       ' req rad [',F6.2,']< ',F6.2/
     2       16X,' group        road  event                '/
     3       16X,'  seq   TF     hit   hit    residual       residual'/
     4       16X,'  hit  layer (IHIT) (ADR) (fit-meas_hit)     Z     ')     
#endif
        
        
C-----------------------------------------------------------------------
C  loop over the LAYERS in this group
C  loop over the HITS in the LAYERS
C  delete hits that have no time information
C  and delete hits that have no status
C ***skip entire group if the entrance angle cut fails at the last layer 
C-----------------------------------------------------------------------
        IF(
     1       (ABS(SIN_ENTR_GR).LE. SIN_ENTR_LOC_AMB).AND.
     2       (RTF(LYR_NUMBER).LT.RADIUS_LIMIT)
     3       )THEN
          
          DO 127 LYR_NUMBER=LYR_FIRST_IN_GR(INDX_GROUP),
     2         LYR_LAST_IN_GR(INDX_GROUP)
            LOGICAL_LYR_IN_HITS=ILLRLR(LYR_NUMBER)
            
            IF(LOGICAL_LYR_IN_HITS.GT.0)THEN
              LYR_NUMBER_PREV=0
              
              DO 125 SEQ_HIT_NUM_IN_HITS=LHITLR(LOGICAL_LYR_IN_HITS),
     2             MHITLR(LOGICAL_LYR_IN_HITS)
                
                IF(
     1               (ABS(DRFHIT(SEQ_HIT_NUM_IN_HITS)).GE.MIN_DRIFT)
     2               .AND.
     2               (STRFHT(SEQ_HIT_NUM_IN_HITS).GT.0)
     3               .AND.
     3               (ZHIT(SEQ_HIT_NUM_IN_HITS).LT.
     3               (ZENDTF(LYR_NUMBER)+Z_OVER_LENGTH_FT))
     5               )THEN
                  
                  CALL RESIDUALS('HIT',0,
     2                 SEQ_HIT_NUM_IN_HITS,HIT_RESID)

                  IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                    CALL RESIDUALS('HIT',FITTYP_LF,
     2                   SEQ_HIT_NUM_IN_HITS,Z_HIT_RESID)
                  ENDIF

                  IF(HITS_USEDIN_AMB_GR.LT.DIM_HITS_IN_GROUP)THEN

                    IF(
     1                   (.NOT.F_T_DZ_WINDOWS).OR.
     2                   (ABS(HIT_RESID).LT.MAX_RES_DZ_WINDOW))THEN

                      HITS_USEDIN_AMB_GR=HITS_USEDIN_AMB_GR+1
                      SAV_HIT_NUM_AMB_GR(HITS_USEDIN_AMB_GR)=
     2                     SEQ_HIT_NUM_IN_HITS
                      SAV_LYR_AMB_GR(HITS_USEDIN_AMB_GR)=
     2                     LYR_NUMBER
                      SAV_RESID_AMB_GR(HITS_USEDIN_AMB_GR)=
     2                     HIT_RESID

                      IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                        SAV_Z_RESID_AMB_GR(HITS_USEDIN_AMB_GR)=
     2                       Z_HIT_RESID
                      ENDIF

                      LYR_NUMBER_PREV=LYR_NUMBER
                    ENDIF
                  ENDIF
                  
#if defined(CLEO_FTDIAG)
                  IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                    PRINT 1104,HITS_USEDIN_AMB_GR,
     2                   LYR_NUMBER,
     3                   SEQ_HIT_NUM_IN_HITS,
     4                   IADRHT(SEQ_HIT_NUM_IN_HITS),
     5                   SAV_RESID_AMB_GR(HITS_USEDIN_AMB_GR),
     6                   SAV_Z_RESID_AMB_GR(HITS_USEDIN_AMB_GR)
                  ELSE
                    PRINT 1104,HITS_USEDIN_AMB_GR,
     2                   LYR_NUMBER,
     3                   SEQ_HIT_NUM_IN_HITS,
     4                   IADRHT(SEQ_HIT_NUM_IN_HITS),
     5                   SAV_RESID_AMB_GR(HITS_USEDIN_AMB_GR)
                  ENDIF
 1104             FORMAT(16X,I5,I6,I6,I7,   F15.6,   F15.6)
#endif
                  
C-----------------------------------------------------------------------
C end of loop over HITS in single layer
C-----------------------------------------------------------------------
                ENDIF
 125          CONTINUE
              IF(LYR_NUMBER_PREV.NE.0)
     2             LYRS_USEDIN_AMB_GR=LYRS_USEDIN_AMB_GR+1
              
C-----------------------------------------------------------------------
C  end of test of hits in layer
C  end of loop over LAYERS in an ambiguity group
C  end of test of entrance angle
C-----------------------------------------------------------------------
            ELSE
#if defined(CLEO_FTDIAG)
              PRINT 1122
 1122         FORMAT(16X,' reject layer, LOGICAL_LYR_IN_HITS=0')
#endif
            ENDIF
 127      CONTINUE
        ELSE
#if defined(CLEO_FTDIAG)
          PRINT 1121
 1121     FORMAT(16X,'reject group on entrance angle or radius limit') 
#endif
        ENDIF
        
C-----------------------------------------------------------------------
C  continue up to the point of testing NUM_HITS_SAV_SOL
C   only if there are  enough layers
C-----------------------------------------------------------------------
        IF(LYRS_USEDIN_AMB_GR.GE.MIN_LYRS_ACCEPT)THEN
          
C-----------------------------------------------------------------------
C  test a path for each hit
C  look for similar ============>>>>>>>>> {POINT - FITTED TRACK}
C-----------------------------------------------------------------------
          
C-----------------------------------------------------------------------
C  loop over the hits in the ambiguity group, defines low edge of road
C  second loop is again over all hits in the group;
C  look for match in SAV_RESID_AMB_GR
C  double counting is avoided because road is positive only
C-----------------------------------------------------------------------
          
#if defined(CLEO_FTDIAG)
          PRINT 1003
          PRINT 1105,INDX_GROUP,
     2         HITS_USEDIN_AMB_GR,
     3         LYRS_USEDIN_AMB_GR,
     4         MIN_LYRS_ACCEPT,
     5         ROAD_AMB
 1105     FORMAT(' FAST_T: GROUP:',I3,
     1         ' have',I4,' hits on',I4,' layers, ',
     2         ' require',I4,' hits within',F9.6/
     3         16X,'           road  event ',
     4         '           road  event           '/
     5         16X,'  seq   TF  hit address',
     6         '  seq   TF  hit address  residual   residual '/
     7         16X,' numbr LAY numbr numbr ',
     8         ' numbr LAY numbr numbr difference            ')     
#endif
          
          DO 171 HIT_AMB_GR=1,HITS_USEDIN_AMB_GR
            LYR_NUMBER_PREV=0
            MIN_RES_DIF_LYR=0.
            USED_ROAD_CUR_SOL=0.
            NUM_HITS_CUR_SOL=0
            AVE_RES_CUR_SOL=0.
            
            MIN_LYR_CUR_SOL=999
            MAX_LYR_CUR_SOL=0
            
            CALL VZERO(L2H_CUR_SOL,DIM_HITS_SOL)
            
C-----------------------------------------------------------------------
C  2nd loop is manual to allow doing the end of layer stuff in one location
C   end of layer is found at end of hit list or when layer number changes
C-----------------------------------------------------------------------
            HIT2_AMB_GR=0
            FOUND_HIT_IN_LAYER=.FALSE.
 141        HIT2_AMB_GR=HIT2_AMB_GR+1
            
            END_OF_LAYER=.FALSE.
            IF(HIT2_AMB_GR.GT.HITS_USEDIN_AMB_GR)THEN
              END_OF_LAYER=.TRUE.
            ELSEIF(SAV_LYR_AMB_GR(HIT2_AMB_GR).NE.
     2             LYR_NUMBER_PREV)THEN
              IF(LYR_NUMBER_PREV.NE.0)END_OF_LAYER=.TRUE.
              LYR_NUMBER_PREV=SAV_LYR_AMB_GR(HIT2_AMB_GR)
            ENDIF
            
            IF(END_OF_LAYER)THEN
              IF(FOUND_HIT_IN_LAYER)THEN
                IF(MIN_RES_DIF_LYR.GT.USED_ROAD_CUR_SOL)
     2               USED_ROAD_CUR_SOL=MIN_RES_DIF_LYR
                
                IF(LYR_CUR_SOL(NUM_HITS_CUR_SOL).LT.
     2               MIN_LYR_CUR_SOL)THEN
                  MIN_LYR_CUR_SOL=LYR_CUR_SOL(NUM_HITS_CUR_SOL)
                  RES_MIN_LYR_CUR_SOL=RES_CUR_SOL(NUM_HITS_CUR_SOL)
                ENDIF
                
                IF(LYR_CUR_SOL(NUM_HITS_CUR_SOL).GT.
     2               MAX_LYR_CUR_SOL)THEN
                  MAX_LYR_CUR_SOL=LYR_CUR_SOL(NUM_HITS_CUR_SOL)
                  RES_MAX_LYR_CUR_SOL=RES_CUR_SOL(NUM_HITS_CUR_SOL)
                ENDIF
                
                FOUND_HIT_IN_LAYER=.FALSE.
              ENDIF
            ENDIF
            
            IF(HIT2_AMB_GR.LE.HITS_USEDIN_AMB_GR)THEN
              
C-----------------------------------------------------------------------
C  alway choose self; never use other hit in same layer
C-----------------------------------------------------------------------
              IF(SAV_LYR_AMB_GR(HIT2_AMB_GR).EQ.
     2             SAV_LYR_AMB_GR(HIT_AMB_GR))THEN
                IF(HIT2_AMB_GR.EQ.HIT_AMB_GR)THEN
                  RES_DIF_HIT=0.
                  SAVEHIT=.TRUE.
                  SAVEHIT_NEW_LAYER=.TRUE.
                  
#if defined(CLEO_FTDIAG)
                  PRINT 1106,HIT_AMB_GR,
     2                 SAV_LYR_AMB_GR(HIT_AMB_GR),
     3                 SAV_HIT_NUM_AMB_GR(HIT_AMB_GR),
     4                 IADRHT(SAV_HIT_NUM_AMB_GR(HIT_AMB_GR)),
     5                 RES_DIF_HIT,
     6                 SAV_RESID_AMB_GR(HIT_AMB_GR)
 1106             FORMAT(16X,I5,I5,I6,I6,8X,'-self-',9X,F11.6,F11.6)
#endif
                ELSE
                  SAVEHIT=.FALSE.
                ENDIF
                
C-----------------------------------------------------------------------
C  process layers other than self; obtain...
C  MIN_RES_DIF_LYR....minimum residual difference for hits in tested layer
C  USED_ROAD_CUR_SOL....maxiumum MIN_RES_DIF_LYR for layers in tested group
C-----------------------------------------------------------------------
              ELSE
                RES_DIF_HIT=SAV_RESID_AMB_GR(HIT2_AMB_GR)-
     2               SAV_RESID_AMB_GR(HIT_AMB_GR)

                IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                  Z_RES_DIF_HIT=SAV_Z_RESID_AMB_GR(HIT2_AMB_GR)-
     2                 SAV_Z_RESID_AMB_GR(HIT_AMB_GR)
                ENDIF


C=======================================================================
C one sided cut on the r-phi road, ROAD_AMB
C also, extra cut in z residual difference for CF
C=======================================================================

                HIT_IN_ROAD=
     1               (RES_DIF_HIT.GE.0.).AND.
     2               (RES_DIF_HIT.LE.ROAD_AMB)

                IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN
                  HIT_IN_ROAD=HIT_IN_ROAD.AND.
     3                 (ABS(Z_RES_DIF_HIT).LT.ROAD_AMB_EXTRA_Z)
                ENDIF

                IF(HIT_IN_ROAD)THEN
                  
#if defined(CLEO_FTDIAG)
                  PRINT 1107,HIT_AMB_GR,
     2                 SAV_LYR_AMB_GR(HIT_AMB_GR),
     3                 SAV_HIT_NUM_AMB_GR(HIT_AMB_GR),
     4                 IADRHT(SAV_HIT_NUM_AMB_GR(HIT_AMB_GR)),
     5                 HIT2_AMB_GR,
     6                 SAV_LYR_AMB_GR(HIT2_AMB_GR),
     7                 SAV_HIT_NUM_AMB_GR(HIT2_AMB_GR),
     8                 IADRHT(SAV_HIT_NUM_AMB_GR(HIT2_AMB_GR)),
     9                 RES_DIF_HIT,
     6                 SAV_RESID_AMB_GR(HIT2_AMB_GR)
 1107             FORMAT(16X,I5,I5,I6,I6, I6,I5,I6,I6, F11.6,F11.6)
#endif
                  
                  IF(.NOT.FOUND_HIT_IN_LAYER)THEN
C-----------------------------------------------------------------------
C  first good hit of a NEW LAYER
C-----------------------------------------------------------------------
                    SAVEHIT=.TRUE.
                    SAVEHIT_NEW_LAYER=.TRUE.
                    
C-----------------------------------------------------------------------
C  this is another good hit in the same layer; is it better?
C-----------------------------------------------------------------------
                  ELSE
                    IF(RES_DIF_HIT.LT.MIN_RES_DIF_LYR)THEN
                      SAVEHIT=.TRUE.
                      SAVEHIT_NEW_LAYER=.FALSE.
                    ELSE
                      SAVEHIT=.FALSE.
                    ENDIF
                  ENDIF
C-----------------------------------------------------------------------
C  hit out if range
C-----------------------------------------------------------------------
                ELSE
                  SAVEHIT=.FALSE.
                ENDIF
              ENDIF
C-----------------------------------------------------------------------
C  if better, or if first hit, save the hit
C-----------------------------------------------------------------------
              IF(SAVEHIT)THEN
                FOUND_HIT_IN_LAYER=.TRUE.
                MIN_RES_DIF_LYR=RES_DIF_HIT
                
                IF(SAVEHIT_NEW_LAYER)THEN
                  IF(NUM_HITS_CUR_SOL.LT.DIM_HITS_SOL)THEN
                    NUM_HITS_CUR_SOL=NUM_HITS_CUR_SOL+1
                    NUM_HITS_SATURATE=.FALSE.
                  ELSE
                    NUM_HITS_SATURATE=.TRUE.
                  ENDIF
                ELSE
                  
                  AVE_RES_CUR_SOL=AVE_RES_CUR_SOL-
     2                 RES_CUR_SOL(NUM_HITS_CUR_SOL)
                  NUM_HITS_SATURATE=.FALSE.
                ENDIF
                
                IF(.NOT.NUM_HITS_SATURATE)THEN
                  
                  HITS_CUR_SOL(NUM_HITS_CUR_SOL)=
     2                 SAV_HIT_NUM_AMB_GR(HIT2_AMB_GR)
                  LYR_CUR_SOL(NUM_HITS_CUR_SOL)=
     2                 SAV_LYR_AMB_GR(HIT2_AMB_GR)
                  AMB_LYR_NUM=        1+SAV_LYR_AMB_GR(HIT2_AMB_GR)
     2                 -LYR_FIRST_IN_GR(INDX_GROUP)
                  L2H_CUR_SOL(AMB_LYR_NUM)=
     2                 SAV_HIT_NUM_AMB_GR(HIT2_AMB_GR)
                  RES_CUR_SOL(NUM_HITS_CUR_SOL)=
     2                 SAV_RESID_AMB_GR(HIT2_AMB_GR)
                  AVE_RES_CUR_SOL=AVE_RES_CUR_SOL+
     2                 RES_CUR_SOL(NUM_HITS_CUR_SOL)
                ENDIF
              ENDIF
              
C-----------------------------------------------------------------------
C  end of loop over HIT2_AMB_GR
C-----------------------------------------------------------------------
              GO TO 141
            ENDIF
            
C-----------------------------------------------------------------------
C  have ended the loop over HIT2_AMB_GR, inner loop over hits
C  test if there are enough correlated hits found in the ambiguity group
C  finish calculation of average residual
C-----------------------------------------------------------------------
            GOOD_SOLUTION_1=NUM_HITS_CUR_SOL.GE.MIN_LYRS_ACCEPT
            IF(GOOD_SOLUTION_1)THEN
              AVE_RES_CUR_SOL=AVE_RES_CUR_SOL/NUM_HITS_CUR_SOL
              
C-----------------------------------------------------------------------
C  calculate deviation from line between minimum and maximum layer
C-----------------------------------------------------------------------
              SLOPE_MAX_MIN=(RES_MAX_LYR_CUR_SOL-RES_MIN_LYR_CUR_SOL)
     2             /(RTF(MAX_LYR_CUR_SOL)-RTF(MIN_LYR_CUR_SOL))
              MAX_DEV_LINE=0.
              DO 145 HIT2_AMB_GR=1,NUM_HITS_CUR_SOL
                DEV_LINE=ABS(RES_CUR_SOL(HIT2_AMB_GR)-
     2               (RES_MIN_LYR_CUR_SOL
     3               +(RTF(LYR_CUR_SOL(HIT2_AMB_GR))
     4               -RTF(MIN_LYR_CUR_SOL))
     5               *SLOPE_MAX_MIN))
                IF(DEV_LINE.GT.MAX_DEV_LINE)MAX_DEV_LINE=DEV_LINE
 145          CONTINUE
              
#if defined(CLEO_FTDIAG)
              PRINT 1108,NUM_HITS_CUR_SOL,
     2             USED_ROAD_CUR_SOL,AVE_RES_CUR_SOL,
     3             MAX_DEV_LINE,
     4             (HITS_CUR_SOL(HIT2_AMB_GR),
     5             HIT2_AMB_GR=1,NUM_HITS_CUR_SOL)
 1108         FORMAT(11X,'solution, len',I3,
     2             '; used road',F9.6,'; ave res',F9.6
     3             '; maxdev',f9.6,
     4             ' HITS:',16(I5,','))
#endif
              
              IF(
     1             (USED_ROAD_CUR_SOL.LE.ROAD_AMB_NO_ALIGN))THEN
                IF(ABS(AVE_RES_CUR_SOL).LT.MAX_AVE_RES_SOL)THEN
#if defined(CLEO_FTDIAG)
                  PRINT 1131
 1131             FORMAT(11X,
     1                 'solution ACCEPTED in parallel road')
#endif
                  GOOD_SOLUTION_2=.TRUE.
                ELSE
                  GOOD_SOLUTION_2=.FALSE.
#if defined(CLEO_FTDIAG)
                  PRINT 1132
 1132             FORMAT(11X,
     1                 'solution accepted in parallel road',
     2                 ', but REJECTED for average residual')
#endif
                ENDIF
              ELSEIF(
     4               (NUM_HITS_CUR_SOL.GE.MIN_LYRS_ALIGN).AND.
     3               (MAX_DEV_LINE.LE.ALIGN_AMB_FULL_ROAD))THEN
                IF(ABS(AVE_RES_CUR_SOL).LT.MAX_AVE_RES_SOL)THEN
#if defined(CLEO_FTDIAG)
                  PRINT 1133
 1133             FORMAT(11X,
     1                 'solution ACCEPTED in slanted road')
#endif
                  GOOD_SOLUTION_2=.TRUE.
                ELSE
                  GOOD_SOLUTION_2=.FALSE.
#if defined(CLEO_FTDIAG)
                  PRINT 1134
 1134             FORMAT(11X,
     1                 'solution accepted in slanted road',
     2                 ', but REJECTED for average residual')
#endif
                ENDIF
              ELSE
                GOOD_SOLUTION_2=.FALSE.
#if defined(CLEO_FTDIAG)
                PRINT 1135
 1135           FORMAT(11X,
     1               'solution REJECTED for used road width')
#endif
              ENDIF
              
              IF(GOOD_SOLUTION_2)THEN
C-----------------------------------------------------------------------
C  if there is a previously found best hit list 
C  are these similar wire lists; difference being only small residuals?
C  this also takes into account when one is a subset of the other 
C-----------------------------------------------------------------------
                
                DIFFERENT_HIT_LIST=.FALSE.
                IF(NUM_HITS_SAV_SOL(1,INDX_GROUP).GT.0)THEN
                  N_DIFF_HIT=0
                  N_UNIQUE_CUR=0
                  N_UNIQUE_SAV=0
                  
                  HIT_C_AMB_GR=0
                  HIT_S_AMB_GR=0
                  INCR_C=.TRUE.
                  INCR_S=.TRUE.
                  
 151              IF(.NOT.DIFFERENT_HIT_LIST)THEN
                    INCR_SOMETHING=.FALSE.
                    
                    IF(INCR_C)THEN
                      IF(HIT_C_AMB_GR.LT.NUM_HITS_CUR_SOL)THEN
                        HIT_C_AMB_GR=HIT_C_AMB_GR+1
                        INCR_SOMETHING=.TRUE.
                      ENDIF
                    ENDIF
                    IF(INCR_S)THEN
                      IF(HIT_S_AMB_GR.LT.
     2                     NUM_HITS_SAV_SOL(1,INDX_GROUP))THEN
                        HIT_S_AMB_GR=HIT_S_AMB_GR+1
                        INCR_SOMETHING=.TRUE.
                      ENDIF
                    ENDIF
                    
                    IF(INCR_SOMETHING)THEN
                      
C CUR layer smaller; it has been passed up and is unique 
                      IF(  LYR_CUR_SOL(HIT_C_AMB_GR).LT.
     2                     LYR_SAV_SOL(HIT_S_AMB_GR,1,INDX_GROUP)
     3                     )THEN
                        N_UNIQUE_CUR=N_UNIQUE_CUR+1
                        INCR_C=.TRUE.
                        INCR_S=.FALSE.
                        
C SAV layer smaller; it has been passed up and is unique 
                      ELSEIF(LYR_CUR_SOL(HIT_C_AMB_GR).GT.
     2                       LYR_SAV_SOL(HIT_S_AMB_GR,1,INDX_GROUP)
     3                       )THEN
                        N_UNIQUE_SAV=N_UNIQUE_SAV+1
                        INCR_C=.FALSE.
                        INCR_S=.TRUE.
                        
C same layer found; not unique; test if exactly the same address
C now test in layer only if there is a hit in the layer for both
C the CUR and SAV solutions. This treats a subset of a solution to be
C equivalent and not to be saved as unique. 
                      ELSE
                        ADR_CUR_SOL=IADRHT(HITS_CUR_SOL
     2                       (HIT_C_AMB_GR))
                        ADR_SAV_SOL=IADRHT(HITS_SAV_SOL
     2                       (HIT_S_AMB_GR,1,INDX_GROUP))
                        IF(ADR_CUR_SOL.NE.ADR_SAV_SOL)THEN
                          N_DIFF_HIT=N_DIFF_HIT+1
                          
C now test on significant residual rather than different wire 
                          IF(ABS(
     2                         RES_CUR_SOL(HIT_C_AMB_GR)-
     3                         RES_SAV_SOL(HIT_S_AMB_GR,
     3                         1,INDX_GROUP)
     4                         ).GT. RES_DIF_SIM_HITS) 
     5                         DIFFERENT_HIT_LIST=.TRUE.
                        ENDIF
                        INCR_C=.TRUE.
                        INCR_S=.TRUE.
                      ENDIF
                      GO TO 151
                    ENDIF
                  ENDIF
                  
                  
#if defined(CLEO_FTDIAG)
                  PRINT 1118,N_UNIQUE_CUR,N_UNIQUE_SAV,N_DIFF_HIT,
     2                 DIFFERENT_HIT_LIST
 1118             FORMAT(11X,' unique cur=',I2,' unique sav=',I2,
     2                 ' different hits=',I2,
     3                 ' prelim_dif_hit_lis=',L1)
#endif
                  
                  IF((N_UNIQUE_SAV.NE.0).AND.(N_UNIQUE_CUR.NE.0))
     2                 DIFFERENT_HIT_LIST=.TRUE.
                  IF(N_DIFF_HIT.GT.MAX_DIFF_HIT)
     2                 DIFFERENT_HIT_LIST=.TRUE.
                ENDIF
                
                
C-----------------------------------------------------------------------
C  compare this ambiguity solution with the previously found best
C-----------------------------------------------------------------------
                
                IF(NUM_HITS_CUR_SOL.GT.
     2               NUM_HITS_SAV_SOL(1,INDX_GROUP))THEN
                  SHIFT_INTO_1ST=1
                  BUMP_1ST_INTO_2ND=DIFFERENT_HIT_LIST
                  SHIFT_INTO_2ND=-1
                  
                ELSEIF(NUM_HITS_CUR_SOL.LT.
     2                 NUM_HITS_SAV_SOL(1,INDX_GROUP))THEN
                  SHIFT_INTO_1ST=-1
                  SHIFT_INTO_2ND=0
                  
C-----------------------------------------------------------------------
C  whether to shift current into 1st is ambiguous; test USED_ROAD_CUR_SOL
C-----------------------------------------------------------------------
                ELSE
                  IF(USED_ROAD_CUR_SOL.LT.
     2                 USED_ROAD_SAV_SOL(1,INDX_GROUP))THEN
                    SHIFT_INTO_1ST=1
                  ELSE
                    SHIFT_INTO_1ST=-1
                  ENDIF
                ENDIF
                
C-----------------------------------------------------------------------
C  allow only unique solutions into 2nd
C-----------------------------------------------------------------------
                
                IF(SHIFT_INTO_1ST.EQ. 1)THEN
                  BUMP_1ST_INTO_2ND=DIFFERENT_HIT_LIST
                  SHIFT_INTO_2ND=-1
                ELSEIF(SHIFT_INTO_1ST.EQ.-1)THEN
                  IF(DIFFERENT_HIT_LIST)THEN
                    SHIFT_INTO_2ND=0
                  ELSE
                    SHIFT_INTO_2ND=-1
                  ENDIF
                ENDIF
                
#if defined(CLEO_FTDIAG)
                IF(( (SHIFT_INTO_1ST.EQ.1).AND.
     2               (.NOT.BUMP_1ST_INTO_2ND))
     3               .OR.(SHIFT_INTO_2ND.EQ.-1))THEN
                  PRINT 1115
 1115             FORMAT(11X,' same wire list,',
     2                 ' shift neither to 2nd place')
                ENDIF
#endif
                
C-----------------------------------------------------------------------
C  new solution has a same number of hits but a different hit list and
C  a better correlation than the old solution,
C  shift the old best (if it exists) into the second best
C  before saving the new solution as the best
C-----------------------------------------------------------------------
                IF(SHIFT_INTO_1ST.EQ.1)THEN
                  IF((NUM_HITS_SAV_SOL(1,INDX_GROUP).GT.0).AND.
     2                 BUMP_1ST_INTO_2ND)THEN
                    NUM_HITS_SAV_SOL(2,INDX_GROUP)=
     2                   NUM_HITS_SAV_SOL(1,INDX_GROUP)
                    CALL UCOPY(HITS_SAV_SOL(1,1,INDX_GROUP),
     2                   HITS_SAV_SOL(1,2,INDX_GROUP),
     3                   NUM_HITS_SAV_SOL(1,INDX_GROUP))
                    CALL UCOPY(LYR_SAV_SOL(1,1,INDX_GROUP),
     2                   LYR_SAV_SOL(1,2,INDX_GROUP),
     3                   NUM_HITS_SAV_SOL(1,INDX_GROUP))
                    CALL UCOPY(L2H_SAV_SOL(1,1,INDX_GROUP),
     2                   L2H_SAV_SOL(1,2,INDX_GROUP),
     3                   DIM_HITS_SOL)
                    CALL UCOPY(RES_SAV_SOL(1,1,INDX_GROUP),
     2                   RES_SAV_SOL(1,2,INDX_GROUP),
     3                   NUM_HITS_SAV_SOL(1,INDX_GROUP))
                    USED_ROAD_SAV_SOL(2,INDX_GROUP)=
     2                   USED_ROAD_SAV_SOL(1,INDX_GROUP)
                    AVE_RES_SAV_SOL(2,INDX_GROUP)=
     2                   AVE_RES_SAV_SOL(1,INDX_GROUP)
                    
#if defined(CLEO_FTDIAG)
                    PRINT 1109
 1109               FORMAT(11X,'bump previous best to 2nd place')
#endif
                    
                  ENDIF
                  
C-----------------------------------------------------------------------
C  save the new solution as the best
C-----------------------------------------------------------------------
                  
                  NUM_HITS_SAV_SOL(1,INDX_GROUP)=
     2                 NUM_HITS_CUR_SOL
                  CALL UCOPY(HITS_CUR_SOL(1),
     2                 HITS_SAV_SOL(1,1,INDX_GROUP),
     3                 NUM_HITS_CUR_SOL)
                  CALL UCOPY(LYR_CUR_SOL(1),
     2                 LYR_SAV_SOL(1,1,INDX_GROUP),
     3                 NUM_HITS_CUR_SOL)
                  CALL UCOPY(L2H_CUR_SOL(1),
     2                 L2H_SAV_SOL(1,1,INDX_GROUP),
     3                 DIM_HITS_SOL)
                  CALL UCOPY(RES_CUR_SOL(1),
     2                 RES_SAV_SOL(1,1,INDX_GROUP),
     3                 NUM_HITS_CUR_SOL)
                  USED_ROAD_SAV_SOL(1,INDX_GROUP)=
     2                 USED_ROAD_CUR_SOL
                  AVE_RES_SAV_SOL(1,INDX_GROUP)=
     2                 AVE_RES_CUR_SOL
                  
#if defined(CLEO_FTDIAG)
                  PRINT 1110
 1110             FORMAT(11X,'save current solution as BEST')
#endif
                ENDIF
                
C-----------------------------------------------------------------------
C  compare this ambiguity resolution with the previous SECOND  best
C  don't care about identical wires, just choose best correlation
C-----------------------------------------------------------------------
                
                IF(SHIFT_INTO_2ND.EQ.0)THEN
                  IF(NUM_HITS_CUR_SOL.GT.
     2                 NUM_HITS_SAV_SOL(2,INDX_GROUP))THEN
                    SHIFT_INTO_2ND=1
                  ELSEIF(NUM_HITS_CUR_SOL.LT.
     2                   NUM_HITS_SAV_SOL(2,INDX_GROUP))THEN
                    SHIFT_INTO_2ND=-1
                  ENDIF
                ENDIF
                IF(SHIFT_INTO_2ND.EQ.0)THEN
                  IF(USED_ROAD_CUR_SOL.LT.
     2                 USED_ROAD_SAV_SOL(2,INDX_GROUP))THEN
                    SHIFT_INTO_2ND=1
                  ELSE
                    SHIFT_INTO_2ND=-1
                  ENDIF
                ENDIF
                
C-----------------------------------------------------------------------
C  save the new solution as the second best
C-----------------------------------------------------------------------
                
                IF(SHIFT_INTO_2ND.EQ.1)THEN
                  NUM_HITS_SAV_SOL(2,INDX_GROUP)=
     2                 NUM_HITS_CUR_SOL
                  CALL UCOPY(HITS_CUR_SOL(1),
     2                 HITS_SAV_SOL(1,2,INDX_GROUP),
     3                 NUM_HITS_CUR_SOL)
                  CALL UCOPY(LYR_CUR_SOL(1),
     2                 LYR_SAV_SOL(1,2,INDX_GROUP),
     3                 NUM_HITS_CUR_SOL)
                  CALL UCOPY(L2H_CUR_SOL(1),
     2                 L2H_SAV_SOL(1,2,INDX_GROUP),
     3                 DIM_HITS_SOL)
                  CALL UCOPY(RES_CUR_SOL(1),
     2                 RES_SAV_SOL(1,2,INDX_GROUP),
     3                 NUM_HITS_CUR_SOL)
                  USED_ROAD_SAV_SOL(2,INDX_GROUP)=
     2                 USED_ROAD_CUR_SOL
                  AVE_RES_SAV_SOL(2,INDX_GROUP)=
     2                 AVE_RES_CUR_SOL
                  
#if defined(CLEO_FTDIAG)
                  PRINT 1111
 1111             FORMAT(11X,'save current solution as 2nd place')
#endif
                  
                ENDIF
C-----------------------------------------------------------------------
C large used road and bad alignment, solution rejected
C-----------------------------------------------------------------------
                
#if defined(CLEO_FTDIAG)
              ELSE
                PRINT 1119
 1119           FORMAT(11X,'REJECT current solution',
     2               ' on used road and alignment ')
#endif
              ENDIF
              
C-----------------------------------------------------------------------
C  end test if there are enough correlated hits found in the ambiguity group
C-----------------------------------------------------------------------
              
#if defined(CLEO_FTDIAG)
            ELSE
              PRINT 1120
 1120         FORMAT(11X,'REJECT current solution',
     2             ' on length')
#endif
            ENDIF
            
C-----------------------------------------------------------------------
C  end loop over hits in group/(solutions to the ambiguity)
C-----------------------------------------------------------------------
 171      CONTINUE
          
C-----------------------------------------------------------------------
C  branch point if there are not enough layers in the group
C-----------------------------------------------------------------------
        ENDIF
        
C-----------------------------------------------------------------------
C  load up the best solution only if it is unique;
C  do not use if first and second best solututions have same number of
C  hits and similar widths 
C-----------------------------------------------------------------------
        LOAD_BEST=.FALSE.
        IF(NUM_HITS_SAV_SOL(1,INDX_GROUP).GT.0)THEN
          
          IF(.NOT.LOAD_BEST)THEN
            LEN_DIF_SOL=NUM_HITS_SAV_SOL(1,INDX_GROUP)
     2           -NUM_HITS_SAV_SOL(2,INDX_GROUP)
            IF(LEN_DIF_SOL.GE.LEN_DIF_UNIQUE)LOAD_BEST=.TRUE.
            
#if defined(CLEO_FTDIAG)
            PRINT 1112,NUM_HITS_SAV_SOL(1,INDX_GROUP),
     2           NUM_HITS_SAV_SOL(2,INDX_GROUP),LEN_DIF_UNIQUE
 1112       FORMAT(11X,'comparing solution lengths: load best if',
     2           I4,' - ',I4,'  >/= (LEN_DIF_UNIQUE=)',I3)
#endif
          ENDIF
          
          IF(.NOT.LOAD_BEST)THEN
            WID_DIF_SOL=USED_ROAD_SAV_SOL(2,INDX_GROUP)
     2           -USED_ROAD_SAV_SOL(1,INDX_GROUP)
            IF(WID_DIF_SOL.GT.WID_DIF_UNIQUE)LOAD_BEST=.TRUE.
            
#if defined(CLEO_FTDIAG)
            PRINT 1113,USED_ROAD_SAV_SOL(2,INDX_GROUP),
     2           USED_ROAD_SAV_SOL(1,INDX_GROUP),
     3           WID_DIF_UNIQUE
 1113       FORMAT(11X,'comparing used widths:   load best if ',
     2           F9.6,' - ',F9.6,' > (WID_DIF_UNIQUE=)',F9.6)
#endif
          ENDIF
          
          
          IF(.NOT.LOAD_BEST)THEN
            RES_DIF_SOL=AVE_RES_SAV_SOL(2,INDX_GROUP)
     2           -AVE_RES_SAV_SOL(1,INDX_GROUP)
            RES_DIF_SOL=ABS(RES_DIF_SOL)
            IF(RES_DIF_SOL.LT.RES_DIF_UNIQUE)LOAD_BEST=.TRUE.
            
#if defined(CLEO_FTDIAG)
            PRINT 1116,AVE_RES_SAV_SOL(2,INDX_GROUP),
     2           AVE_RES_SAV_SOL(1,INDX_GROUP),
     3           RES_DIF_UNIQUE
 1116       FORMAT(11X,'comparing ave residuals:  load best if',
     2           F9.6,' - ',F9.6,'  < (RES_DIF_UNIQUE=)',F9.6)
#endif
          ENDIF
        ENDIF 
        
C-----------------------------------------------------------------------
C  authorize loading the best solution into /TFTRAKCD/
C-----------------------------------------------------------------------
        
        IF(LOAD_BEST)THEN
          INDX_SOL_USE(INDX_GROUP)=1
          
#if defined(CLEO_FTDIAG)
          PRINT 1114,INDX_GROUP
 1114     FORMAT(11X,'will STORE solution for group number',I3)
#endif
          
        ELSE
          INDX_SOL_USE(INDX_GROUP)=0
          
#if defined(CLEO_FTDIAG)
          PRINT 1117,INDX_GROUP
 1117     FORMAT(11X,'DO NOT STORE solution for group number',I3)
#endif
          
        ENDIF
        
        
c#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
c      PRINT 1136
c 1136 FORMAT(' FAST_TRAk_POPULAT_AMB_GRP: waiting')
c      CALL XTS_RFV_INPUT
c#endif 
        
C-----------------------------------------------------------------------
C  end of loop over ambiguity groups
C-----------------------------------------------------------------------

 176  CONTINUE
      
      RETURN
      END
      



