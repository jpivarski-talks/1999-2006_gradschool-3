*
* $Id: sfind_hit_quality.F,v 1.1 2003/03/05 18:29:44 dpp Exp $
*
* $Log: sfind_hit_quality.F,v $
* Revision 1.1  2003/03/05 18:29:44  dpp
*      -> NEW ROUTINE
*      -> split off from sfind
*      -> new assignments for negative INUCHN
*      -> assign INUCHN hit quality based on PH, time and previous use
*      -> call clustering
*      -> test SF flag to use TPC specific code
*      -> remove special (faster?) reset for sfbwal
*      -> use EvntHit_OpnHit to indicate usable hits
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : DAN PETERSON
      SUBROUTINE SFIND_HIT_QUALITY(PRE_POST_SFCHAN)
C......................................................................
C.
C. SFIND_HIT_QUALITY
C.
C. COMMON    : sfcom, sf_data_access
C. CALLS     : 
C. CALLED    : SFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 19-Nov-02
C. LAST MOD  : 
C.
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE

#endif
      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom, and cdscrtcd (if needed)
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------

#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"


      INTEGER      PRE_POST_SFCHAN
      INTEGER      LYRSF,LYRCD
      INTEGER      IH,IH_layer,IH_Best
      INTEGER      Wire0 ,WireP ,WireM
      INTEGER      SFCellID_0,SFCellID_P,SFCellID_M
      LOGICAL      ALTRNT,SO_FAR_SO_GOOD,BETTER_HIT
      INTEGER      IADCD
      INTEGER      TIMEI,PULSEH,PULSEH_Best
      REAL         TIMER,DIST
      INTEGER      IHSAME
      INTEGER      OPEN_HIT

      REAL ZPRED,ZCALC

      LOGICAL DUMPIT

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C***********************************************************************
C executable code starts here
C***********************************************************************

C-----------------------------------------------------------------------
C ADDRESSING OF INUCHN ETC. IS SF
C ELIMINATE HITS WITH A TIME CUT
C ELIMINATE HITS WITH A PH CUT
C ELIMINATE HITS THAT ARE IN A DUET TRACK
C SFCHAN IGNORES ALL HITS WITH INUCHN(SFCellID_0)<0
C  BOTH AS DESIRED HIT OR ADJACENT
C-----------------------------------------------------------------------

      IF(PRE_POST_SFCHAN.EQ.1)THEN
        
C-----------------------------------------------------------------------
C loop over the SF layers
C-----------------------------------------------------------------------
        
        DO 79 LYRSF=1,NLAYSF
          LYRCD=CDLASF(LYRSF)
          ZPRED=RCD(LYRCD)*SFZSLCNTR
          
C-----------------------------------------------------------------------
C activate using the addressed cell directly
C-----------------------------------------------------------------------
        
#define SF_DATA_USE_ADDRESSED_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"

C-----------------------------------------------------------------------
C loop over the hits in the layer
C-----------------------------------------------------------------------
          
          sf_data_input_layr_sf=LYRSF
#define SF_DATA_1STH
#include "doit/sf_data_access/sf_data_access.inc"
          IH=sf_data_output_hit_adr
          
 61       CONTINUE
          IF(IH.GT.0)THEN
            IH_Layer=IH
            
C-----------------------------------------------------------------------
C addresses of the wire and the neighbors of the wire
C remember, SFSVWE=1
C-----------------------------------------------------------------------
            
            sf_data_input_hit_adr=IH
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
            Wire0=sf_data_output_wire
            
            ALTRNT=Wire0.GT.NWRSF(LYRSF)
            WireM=Wire0-1
            WireP=Wire0+1
            
            IF(.NOT.ALTRNT)THEN
              IF(WireM.LT.  0           )WireM=WireM+NWRSF(LYRSF)
              IF(WireP.GE.  NWRSF(LYRSF))WireP=WireP-NWRSF(LYRSF)
            ELSE
              IF(WireM.LT.  NWRSF(LYRSF))WireM=WireM+NWRSF(LYRSF)
              IF(WireP.GE.2*NWRSF(LYRSF))WireP=WireP-NWRSF(LYRSF)
            ENDIF
            
            SFCellID_0=INSFSF(LYRSF)+Wire0
            SFCellID_M=INSFSF(LYRSF)+WireM
            SFCellID_P=INSFSF(LYRSF)+WireP
            
C INCDSF points to either the CD of SV geometry, depending on layer
            IADCD=INCDSF(LYRSF)+Wire0

C dump
            DUMPIT=.FALSE.
            IF(
     1           (SFCellID_0.eq.697).or.
     1           (SFCellID_0.eq.497).or.
     1           (SFCellID_0.eq.317).or.
     1           (SFCellID_0.eq.165).or.
     1           (SFCellID_0.eq. 33).or.
     1           (SFCellID_M.eq.697).or.
     1           (SFCellID_M.eq.497).or.
     1           (SFCellID_M.eq.317).or.
     1           (SFCellID_M.eq.165).or.
     1           (SFCellID_M.eq. 33).or.
     1           (SFCellID_P.eq.697).or.
     1           (SFCellID_P.eq.497).or.
     1           (SFCellID_P.eq.317).or.
     1           (SFCellID_P.eq.165).or.
     1           (SFCellID_P.eq. 33)
     1           )DUMPIT=.TRUE.
            DUMPIT=.FALSE.


C-----------------------------------------------------------------------
C distance and time cuts
C-----------------------------------------------------------------------
            
            SO_FAR_SO_GOOD=.TRUE.
            IF(LYRCD.GT.0)THEN
              
              IH_Best=0
              PULSEH_Best=0
              
 63           CONTINUE
              
C-----------------------------------------------------------------------
C cut in the raw TDC
C-----------------------------------------------------------------------
              IF(SO_FAR_SO_GOOD)THEN
                
                sf_data_input_hit_adr=IH
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
                TIMEI=sf_data_output_tdc_raw
                
C RAW_TDC_L etc. is set in SFTCUT 
                IF( 
     1               (TIMEI .LT.RAW_TDC_L(LYRSF)).OR.
     1               (TIMEI .GT.RAW_TDC_H(LYRSF)))THEN
                  INUCHN(SFCellID_0)=-2
                  SO_FAR_SO_GOOD=.FALSE.
                ENDIF
              ENDIF

              if(dumpit)print 8001,sfcellid_0,ih,
     2             timei,SO_FAR_SO_GOOD
 8001         format(' '/
     1             ' sfind_hit_quality: cell=',I6,' hit=',I6,
     2             ' timeI=',I6,' SFSgood=',L1)
              
C-----------------------------------------------------------------------
C cut in the drift time in picoseconds
C-----------------------------------------------------------------------
              
              IF(SO_FAR_SO_GOOD)THEN
                
                sf_data_input_hit_adr=IH
#define SF_DATA_TMPS
#include "doit/sf_data_access/sf_data_access.inc"
                TIMER=sf_data_output_r_tim_ps
                
C TIME_PS_L etc. is set in SFTCUT
                IF(
     1               (TIMER.LT.TIME_PS_L(LYRSF)).OR.
     1               (TIMER.GT.TIME_PS_H(LYRSF)))THEN
                  INUCHN(SFCellID_0)=-3
                  SO_FAR_SO_GOOD=.FALSE.
                ENDIF
              ENDIF

              if(dumpit)print 8002,sfcellid_0,ih,
     2             timer,SO_FAR_SO_GOOD
 8002         format(
     1             ' sfind_hit_quality: cell=',I6,' hit=',I6,
     2             ' timeR=',F12.1,' SFSgood=',L1)
              
C-----------------------------------------------------------------------
C cut on the z window
C-----------------------------------------------------------------------
              
              IF((L_SF_DOIT_TPC).AND.(SO_FAR_SO_GOOD))THEN
                ZCALC=TIMER*SFZSLVEL
                
c                  print 9911,timer,SFZSLVEL,zcalc,zpred,SFZSLWID
c 9911             format(' SFIND:',
c     1                 '  time=',e11.3,' VEL=',e11.3,
c     1                 '  zcalc=',F7.3,'  zpred=',f7.3,
c     2                 '  ZWID=',f7.3)
                
                IF(ABS(ZCALC-ZPRED).GT.SFZSLWID)THEN
                  INUCHN(SFCellID_0)=-4
                  SO_FAR_SO_GOOD=.FALSE.
                ENDIF
              ENDIF
              
                
                
                if(dumpit)print 8003,sfcellid_0,ih,
     2               zcalc,SO_FAR_SO_GOOD
 8003           format(
     1               ' sfind_hit_quality: cell=',I6,' hit=',I6,
     2               ' zcalc=',F9.3,' SFSgood=',L1)
                
                
C-----------------------------------------------------------------------
C          cut on the drift distance stored in CDSCRTCD
C-----------------------------------------------------------------------
              
              IF(SO_FAR_SO_GOOD)THEN
                
                sf_data_input_hit_adr=IH
#define SF_DATA_DRIFT_DISTANCE
#include "doit/sf_data_access/sf_data_access.inc"
                DIST=sf_data_output_drift
                
                DIST=DIST*1000000.
                
C DRIFT_D_L etc. is set in SFTCUT
                IF(
     1               (DIST .LT.DRIFT_D_L(LYRSF)).OR.
     1               (DIST .GT.DRIFT_D_H(LYRSF)))THEN
                  INUCHN(SFCellID_0)=-5
                  SO_FAR_SO_GOOD=.FALSE.
                ENDIF
              ENDIF
              

              if(dumpit)print 8004,sfcellid_0,ih,
     2             dist,SO_FAR_SO_GOOD
 8004         format(
     1             ' sfind_hit_quality: cell=',I6,' hit=',I6,
     2             ' dist=',F9.1,' SFSgood=',L1)
              
C-----------------------------------------------------------------------
C pulse height cut
C-----------------------------------------------------------------------
              IF(SO_FAR_SO_GOOD)THEN
                
C      sf_data_input_hit_adr=IH
C#define SF_DATA_RADC
C#include "doit/sf_data_access/sf_data_access.inc"
C      PULSEH=sf_data_output_adc_raw
                
                sf_data_input_hit_adr=IH
#define SF_DATA_ADCH
#include "doit/sf_data_access/sf_data_access.inc"
                PULSEH=sf_data_output_ph_happy
                
                IF(
     1               (PULSEH.LT.PULS_HT_L(LYRSF)).OR.
     1               (PULSEH.GT.PULS_HT_H(LYRSF)))THEN
                  INUCHN(SFCellID_0)=-6
                  SO_FAR_SO_GOOD=.FALSE.
                ENDIF
              ENDIF


              if(dumpit)print 8005,sfcellid_0,ih,
     2             pulseh,SO_FAR_SO_GOOD
 8005         format(
     1             ' sfind_hit_quality: cell=',I6,' hit=',I6,
     2             ' pulseh=',I6,' SFSgood=',L1)

              
C-----------------------------------------------------------------------
C test if this is the best hit found for this cell
C-----------------------------------------------------------------------
              
              IF(SO_FAR_SO_GOOD)THEN
                
                IF(IH_Best.EQ.0)THEN
                  BETTER_HIT=.TRUE.
                ELSE
                  IF(PULSEH.GT.PULSEH_Best)THEN
                    BETTER_HIT=.TRUE.
                  ELSE
                    BETTER_HIT=.FALSE.
                  ENDIF
                ENDIF
                
                IF(BETTER_HIT)THEN
                  IH_Best=IH
                  PULSEH_Best=PULSEH

              if(dumpit)print 8006,sfcellid_0,ih,
     2             PULSEH_Best
 8006         format(
     1             ' sfind_hit_quality: cell=',I6,' hit=',I6,
     2             ' pulseh=',I6,'   ---- new best hit')


                ENDIF
                
              ENDIF
              
C-----------------------------------------------------------------------
C loop back with another hit in the same cell
C-----------------------------------------------------------------------
              
              sf_data_input_hit_adr=IH
#define SF_DATA_NEXTHIT_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
              IHSAME=sf_data_output_next_mult
              
              IF(IHSAME.NE.0)THEN
                IH=IHSAME
                SO_FAR_SO_GOOD=.TRUE.
C problem::::::this interferes with "next-to" hit assignment
                INUCHN(SFCellID_0)=0
                GO TO 63
              ENDIF
              
C-----------------------------------------------------------------------
C set INUCHN if a good hit was found, otherwise, leave it as the last bad
C do not overwrite the next-to-previous information
C set the prime hit on a multi-hit cell to the best hit
C-----------------------------------------------------------------------
              
              IF(IH_Best.NE.0)THEN
                SO_FAR_SO_GOOD=.TRUE.

                IF(INUCHN(SFCellID_0).NE.-8)THEN
                  INUCHN(SFCellID_0)=0
                ENDIF
                
                sf_data_input_hit_adr=IH_Best
#define SF_DATA_SET_PRIME_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"



                if(dumpit)print 8007,sfcellid_0,ih_layer,
     2               IH_Best,
     3               EvntHit_1stMlt(IH_Best),
     4               EvntHit_PrimMlt(
     4               EvntHit_1stMlt(SF_DATA_INPUT_HIT_ADR))
 8007           format(
     1               ' sfind_hit_quality: cell=',I6,' hit=',I6,
     2               ' hit_best=',I6,
     3               ' 1stmut(best)=',I6,
     4               ' prime(1stmlt(best))=',I6)


              ELSE
                SO_FAR_SO_GOOD=.FALSE.
              ENDIF
            
C-----------------------------------------------------------------------
C cut on hits used by previous DOIT tracks
C not beam wall
C-----------------------------------------------------------------------
#if defined(CLEO_SFDUET)
              IF(SO_FAR_SO_GOOD)THEN
                IF(CNTLSF.NE.5)THEN 
                  
                  if(dumpit)print 8008,
     2                 IADCD,EvntHit_MapDet(IADCD),
     3                 sfcellid_0,INUCHN(SFCellID_0),
     4                 sfcellid_M,INUCHN(SFCellID_M),
     5                 sfcellid_P,INUCHN(SFCellID_P)
 8008             format(
     1                 ' sfind_hit_quality:',
     2                 ' MapDet(',I6,')=',I7,
     3                 ' (0)INUCHN(',I6,')=',I3,
     4                 ' (M)INUCHN(',I6,')=',I3,
     5                 ' (P)INUCHN(',I6,')=',I3)
                  
C potential problem, this test should probably be inside the multihit loop
                  
c                  IF(EvntHit_MapDet(IADCD).LT.0)THEN

C this is now tested at the hit level, 
C but should still be inside the multihit loop

                  sf_data_input_hit_adr=IH_Best
#define SF_DATA_OPEN_HIT
#include "doit/sf_data_access/sf_data_access.inc"
                  OPEN_HIT=sf_data_output_open_hit
                  
                  IF(OPEN_HIT.LT.0)THEN
                    INUCHN(SFCellID_0)=-7
                    IF(INUCHN(SFCellID_M).EQ.0)INUCHN(SFCellID_M)=-8
                    IF(INUCHN(SFCellID_P).EQ.0)INUCHN(SFCellID_P)=-8
                    SO_FAR_SO_GOOD=.FALSE.
                  ENDIF
                  
                ENDIF
              ENDIF
#endif
              
C-----------------------------------------------------------------------
C end of the LYRCD condition
C-----------------------------------------------------------------------
            ENDIF
            
            
C-----------------------------------------------------------------------
C set all remaining good hits to the non-clustered status
C-----------------------------------------------------------------------
            
            IF((L_SF_DOIT_TPC))THEN
              IF(INUCHN(SFCellID_0).EQ.0)INUCHN(SFCellID_0)=-9
            ENDIF
            
C-----------------------------------------------------------------------
C process next hit in the layer
C-----------------------------------------------------------------------
            
            sf_data_input_hit_adr=IH_Layer
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
            IH=sf_data_output_hit_adr
            
            GO TO 61
            
C end of valid hit condition
          ENDIF
          
        
C-----------------------------------------------------------------------
C activate using the PRIME  cell of multi-hit electronics before clustering
C-----------------------------------------------------------------------

          IF((L_SF_DOIT_TPC))THEN
            
#define SF_DATA_USE_PRIME_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
            
C-----------------------------------------------------------------------
C cluster the hits in this layer after all other cuts
C-----------------------------------------------------------------------
            
            CALL SF_TPC_CLUSTER(LYRSF+0)
          ENDIF
                   
C-----------------------------------------------------------------------
C end of loop over layers
C-----------------------------------------------------------------------

 79     CONTINUE
        
C-----------------------------------------------------------------------
C this is the end of the first entry, before the first pass through SFCHAN
C fot TPC, sf_data_access will be left in a state of "USE_PRIME" 
C meaning that the prime hit will be delivered
C sf_data_access is initialized to the "USE ADDRESSED" state in TFINIT  
C-----------------------------------------------------------------------
        
        
C-----------------------------------------------------------------------
C this is the entry used after the first pass through SFCHAN
C SET THE INUCHN=-8 (adjacent to previous doit tracks) HITS BACK T0 0
C-----------------------------------------------------------------------
        
      ELSE
        
        DO 279 LYRSF=1,NLAYSF
          LYRCD=CDLASF(LYRSF)
          IF(LYRCD.GT.0)THEN
            
            sf_data_input_layr_sf=LYRSF
#define SF_DATA_1STH
#include "doit/sf_data_access/sf_data_access.inc"
            IH=sf_data_output_hit_adr
            
 261        IF(IH.GT.0)THEN
            
              sf_data_input_hit_adr=IH
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
              Wire0=sf_data_output_wire
              
              SFCellID_0=INSFSF(LYRSF)+Wire0
              IF(INUCHN(SFCellID_0).EQ.-8)INUCHN(SFCellID_0)=0
              
              sf_data_input_hit_adr=IH
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
              IH=sf_data_output_hit_adr
              
              GO TO 261
            ENDIF
          ENDIF
 279    CONTINUE
        
      ENDIF
      
C-----------------------------------------------------------------------
C the end
C-----------------------------------------------------------------------
      RETURN
      END
      








