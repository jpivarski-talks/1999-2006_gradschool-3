*
* $Id: sf_tpc_cluster.F,v 1.1 2003/03/05 18:30:16 dpp Exp $
*
* $Log: sf_tpc_cluster.F,v $
* Revision 1.1  2003/03/05 18:30:16  dpp
*      -> NEW ROUTINE
*      -> addresses EvntHit arrays through sf_data_access
*      -> maximum: finds maximum of all hits in layer,
*      -> multi-hit: uses pre-determined (from sfind_hit_quality) best on cell
*      -> other hit: uses map from EvntHit arrays
*      -> use EvntHit_OpnHit to indicate usable hits
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE SF_TPC_CLUSTER(LYRSF)
C.......................................................................
C.
C. SF_TPC_CLUSTER - cluster TPC pads in sf
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson, adapted from cdclus
C. VERSION   : 1.00
C. CREATED   : 05-Nov-2002
C. LAST MOD  : 
C.......................................................................
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

#include "doit/duseq/tfgeomcd.inc"

      LOGICAL FIRST_LOOP_FOR_PEAK
      INTEGER LYRSF
      INTEGER LYRCD
      INTEGER LYRTF
      
      INTEGER HIT_ADR,     HIT_ADR_PEAK,     HIT_ADR_LAST
      INTEGER HIT_ADR_LOW
      INTEGER OPEN_HIT

      INTEGER SF_WIRE,     SF_WIRE_PEAK,     SF_WIRE_LAST
      INTEGER SF_CELL_ID,  SF_CELL_ID_PEAK,  SF_CELL_ID_HOLD_ADJ
      REAL    QMAX
      INTEGER PULSEH,      PULSEH_PEAK
      REAL    PHFRAC,      PHFRAC_HOLD_ADJ
      INTEGER PULSEH_SAT,  PULSEH_LOW
      REAL    PHFRAC_DIP
      INTEGER IDIR
      INTEGER W_DIFF

      LOGICAL FULLY_WRAPPED
      LOGICAL ADJACENT
      LOGICAL SPLIT_AT_DIP

      LOGICAL DUMPIT


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

C-----------------------------------------------------------------------
C layer numbers
C-----------------------------------------------------------------------

      LYRCD=CDLASF(LYRSF)
      LYRTF=ILCDTF(LYRCD)
      FIRST_LOOP_FOR_PEAK=.TRUE.

C-----------------------------------------------------------------------
C start of searching for peak cathode pad in the phi sector
C-----------------------------------------------------------------------

 111  HIT_ADR_PEAK=0

C QMAX: pulse height of the maximum ph pad
C       (initialize to the minimum allowed)

      QMAX=CLS_Q_P_MIN(LYRTF)

C-----------------------------------------------------------------------
C loop through pads in the layers looking for a new maximum
C-----------------------------------------------------------------------

C get the first hit

      sf_data_input_layr_sf=LYRSF
#define SF_DATA_1STH
#include "doit/sf_data_access/sf_data_access.inc"
      HIT_ADR=sf_data_output_hit_adr

C test there is a cell with hit

 131  CONTINUE
      IF(HIT_ADR.GT.0)THEN
        
C test if hit is still open with hit level flag
        
        sf_data_input_hit_adr=HIT_ADR
#define SF_DATA_OPEN_HIT
#include "doit/sf_data_access/sf_data_access.inc"
        OPEN_HIT=sf_data_output_open_hit
        
        IF(OPEN_HIT.GE.0)THEN
          
          IF(FIRST_LOOP_FOR_PEAK)THEN
            
            sf_data_input_hit_adr=HIT_ADR
            sf_data_input_primary=0
#define SF_DATA_PUT_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"
            
            sf_data_input_hit_adr=HIT_ADR
            sf_data_input_nxt_clus=0
#define SF_DATA_PUT_NEXT_HIT_CLUSTER
#include "doit/sf_data_access/sf_data_access.inc"
            
          ENDIF
          
          sf_data_input_hit_adr=HIT_ADR
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
          SF_WIRE=sf_data_output_wire
          
          
C get the SF address
          
          SF_CELL_ID=INSFSF(LYRSF)+SF_WIRE
          
C use INUCHN={-9} to denote valid non-clustered hits still under consideration
C INUCHN={-7:-8} includes hits that are on, or next to, previous tracks
C INUCHN={-2:-6} will include hits out of the time window or ph window 
C INUCHN={-1} denotes hits hits already clustered, not leading hit
C INUCHN={ 0} denotes hits hits already clustered, leading hit
          
          IF(INUCHN(SF_CELL_ID).EQ.-9)THEN
            
C select highest pulse height in the layer
            
            sf_data_input_hit_adr=HIT_ADR
#define SF_DATA_ADCH
#include "doit/sf_data_access/sf_data_access.inc"
            PULSEH=sf_data_output_ph_happy
            
            IF(PULSEH.GT.QMAX) THEN
              
C make no cuts on time correlation, that is done with the window
C all hits within the window are considered in-time
              
              HIT_ADR_PEAK=    HIT_ADR
              SF_WIRE_PEAK=    SF_WIRE
              SF_CELL_ID_PEAK= SF_CELL_ID
              QMAX=            PULSEH
              PULSEH_PEAK=     PULSEH
              
C end of condition of new high pulse height
            ENDIF
C end condition, valid hit in INUCHN
          ENDIF
          
C end of open hit condition
        ENDIF

C process next hit
          
        sf_data_input_hit_adr=HIT_ADR
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
        HIT_ADR=sf_data_output_hit_adr
        
        GO TO 131
        
C end of valid hit condition
      ENDIF
      
C stop processing the input section if there is no valid peak
      
      IF(HIT_ADR_PEAK.GT.0)THEN

C set INUCHN for the peak of a cluster
        
        INUCHN(SF_CELL_ID_PEAK)=0

C set the primary hit number for the peak of the cluster

          sf_data_input_hit_adr=HIT_ADR_PEAK
          sf_data_input_primary=HIT_ADR_PEAK
#define SF_DATA_PUT_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"


            DUMPIT=.FALSE.
            IF(
     1           (SF_Cell_ID_Peak.eq.8154).or.
     1           (SF_Cell_ID_Peak.eq.8155).or.
     1           (SF_Cell_ID_Peak.eq.8156))DUMPIT=.TRUE.



C-----------------------------------------------------------------------
C FILL IN THE CLUSTER
C-----------------------------------------------------------------------

C this version counts down through the geometry 
C rather than an input hit list
C there is no guarantee that hits are in order in the hit wire list

        PHFRAC_HOLD_ADJ=0.
        IDIR=0

C-----------------------------------------------------------------------
C start point for new direction
C first, skip down to the first connected strip in the cluster
C-----------------------------------------------------------------------

 201    CONTINUE
        IF(IDIR.EQ.0)THEN
          IDIR=-1
          HIT_ADR_LAST=HIT_ADR_PEAK
C Do reset SF_WIRE_LAST to the peak value, it determines the path

C-----------------------------------------------------------------------
C then, skip up to the last connected strip in the cluster
C-----------------------------------------------------------------------

        ELSEIF(IDIR.EQ.-1)THEN
          IDIR=1
C Do not change the HIT_ADR_LAST, it is the last hit address from 
C the downward search, or is still the peak, it is needed for nxt_clus.
C Do reset SF_WIRE_LAST to the peak value, it determines the path
          
        ENDIF

C-----------------------------------------------------------------------
C common initialization for the start of either direction
C-----------------------------------------------------------------------

        SF_WIRE_LAST=SF_WIRE_PEAK
        PULSEH_LOW=PULSEH_PEAK
        HIT_ADR_LOW= HIT_ADR_PEAK            
        

          IF(dumpit)print 9911,IDIR,sf_cell_id_peak,
     2         sf_wire_last,hit_adr_last
 9911     format(' sf_tpc_cluster: skip ',i4,', cell_id_peak=',I6,
     2         ' sf_wire_last=',I6,' hit_adr_last=',I6)

C-----------------------------------------------------------------------
C start point for a testing new hit in the current direction 
C-----------------------------------------------------------------------

 241    CONTINUE
        
C test that it is not wrapped all the way around
        
        W_DIFF=SF_WIRE_LAST-SF_WIRE_PEAK
        IF(W_DIFF.GE.NWRSF(LYRSF))W_DIFF=W_DIFF-NWRSF(LYRSF)
        IF(W_DIFF.LT.0           )W_DIFF=W_DIFF+NWRSF(LYRSF)
        ADJACENT=W_DIFF.EQ.0
        
        IF(IDIR.EQ.-1)THEN
          FULLY_WRAPPED=W_DIFF.EQ.(1)
        ELSEIF(IDIR.EQ.1)THEN
          FULLY_WRAPPED=W_DIFF.EQ.(NWRSF(LYRSF)-1)
        ELSE
          FULLY_WRAPPED=.TRUE.
        ENDIF

        IF(.NOT.FULLY_WRAPPED)THEN
          
C get the SF address
          
          SF_WIRE=SF_WIRE_LAST+IDIR
          IF(SF_WIRE.GE.NWRSF(LYRSF))SF_WIRE=SF_WIRE-NWRSF(LYRSF)
          IF(SF_WIRE.LT.0           )SF_WIRE=SF_WIRE+NWRSF(LYRSF)
          SF_CELL_ID=INSFSF(LYRSF)+SF_WIRE
          

          IF(dumpit)print 9913,sf_cell_id_peak,
     2        sf_wire,sf_cell_id
 9913     format(' sf_tpc_cluster:locate cell,cell_id_peak=',I6,
     2         ' TEST sf_wire=',I6,' ..sf_cell_id=',I6)

C test the map to see if there are any hits in on this pad
          
          sf_data_input_sf_adr=SF_CELL_ID
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
          HIT_ADR=sf_data_output_hit_adr

          IF(dumpit)print 9914,sf_cell_id_peak,
     2        sf_wire,sf_cell_id,
     3         hit_adr
 9914     format(' sf_tpc_cluster: testing,   cell_id_peak=',I6,
     2         ' TEST sf_wire=',I6,' ..sf_cell_id=',I6,
     3         ' hit_adr=',I6)
          
          IF(HIT_ADR.GT.0)THEN

C test if hit is still open with hit level flag
            
            sf_data_input_hit_adr=HIT_ADR
#define SF_DATA_OPEN_HIT
#include "doit/sf_data_access/sf_data_access.inc"
            OPEN_HIT=sf_data_output_open_hit
            
            IF(OPEN_HIT.GE.0)THEN
              
C use INUCHN={-9} to denote valid non-clustered hits still under consideration
C INUCHN={-2:-8} will include hits out of the time window 
C INUCHN={-1} denotes hits hits already clustered, not leading hit
C INUCHN={ 0} denotes hits hits already clustered, leading hit
              
              
              IF(dumpit)print 9915,sf_cell_id_peak,
     2             sf_wire,sf_cell_id,
     3             hit_adr,INUCHN(SF_CELL_ID)
 9915         format(' sf_tpc_cluster: testing,   cell_id_peak=',I6,
     2             ' TEST sf_wire=',I6,' ..sf_cell_id=',I6,
     3             ' hit_adr=',I6,' inuchn=',I3)
              
              IF(INUCHN(SF_CELL_ID).EQ.-9)THEN              
                
C pulse height cut                  
                
                sf_data_input_hit_adr=HIT_ADR
#define SF_DATA_ADCH
#include "doit/sf_data_access/sf_data_access.inc"
                PULSEH=sf_data_output_ph_happy
              
                
                IF(dumpit)print 9916,sf_cell_id_peak,
     2               sf_wire,sf_cell_id,
     3               hit_adr,PULSEH,CLS_Q_A_MIN(LYRTF)
 9916           format(' sf_tpc_cluster: testing,   cell_id_peak=',I6,
     2               ' TEST sf_wire=',I6,' ..sf_cell_id=',I6,
     3               ' hit_adr=',I6,' PULSEH=',I6,'Q_MIN=',F9.1)
                
                IF(PULSEH.GT.CLS_Q_A_MIN(LYRTF))THEN
                  
                  sf_data_input_hit_adr=HIT_ADR_LAST
                  sf_data_input_nxt_clus=HIT_ADR
#define SF_DATA_PUT_NEXT_HIT_CLUSTER
#include "doit/sf_data_access/sf_data_access.inc"
                  
                  
                  sf_data_input_hit_adr=HIT_ADR
                  sf_data_input_primary=HIT_ADR_PEAK
#define SF_DATA_PUT_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"
                  
                  
                  INUCHN(SF_CELL_ID)=-1
                  
                  
                  PHFRAC=(1.*PULSEH)/(1.*PULSEH_PEAK)
                  
                  IF(ADJACENT)THEN
                    IF(PHFRAC.GT.PHFRAC_HOLD_ADJ)THEN
                      PHFRAC_HOLD_ADJ=PHFRAC
                      SF_CELL_ID_HOLD_ADJ=SF_CELL_ID
                    ENDIF
                  ENDIF
                  
                  
                  IF(dumpit)print 9917,sf_cell_id_peak,
     2                 sf_wire,sf_cell_id,
     3                 hit_adr,PHFRAC,ADJACENT
 9917             format(' sf_tpc_cluster: keep hit,  cell_id_peak=',I6,
     2                 ' TEST sf_wire=',I6,' ..sf_cell_id=',I6,
     3                 ' hit_adr=',I6,' PHFRAC=',F9.4,'adjacent=',L1)
                  
                  
                  HIT_ADR_LAST=HIT_ADR
                  SF_WIRE_LAST=SF_WIRE
                  
C determine if cluster should be split at a dip
C funny use of PULSEH_SAT (saturated) is to avaoid divide by zero
                  PULSEH_SAT=PULSEH
                  IF(PULSEH_SAT.LT.1)PULSEH_SAT=1
                  IF(PULSEH_SAT.LT.PULSEH_LOW)THEN
                    PULSEH_LOW=PULSEH_SAT
                    HIT_ADR_LOW=HIT_ADR
                    SPLIT_AT_DIP=.FALSE.
                  ELSE
                    
C-----------------------------------------------------------------------
C test for split if current location is not a new low
                    PHFRAC_DIP=1.*PULSEH_LOW/PULSEH_SAT
                    IF(PHFRAC_DIP.LT.CLS_F_C_MAX(LYRTF))THEN
                      
                      IF(dumpit)print 9918,
     1                     sf_cell_id_peak,
     2                     sf_wire,sf_cell_id,
     3                     hit_adr,hit_adr_low
 9918                 format(' sf_tpc_cluster:',
     1                     ' split@dip, cell_id_peak=',I6,
     2                     ' TEST sf_wire=',I6,' ..sf_cell_id=',I6,
     3                     ' hit_adr=',I6,' hit_adr_LOW=',I6)
                      
                      
C HIT_ADR_LOW stays with cluster, everything else is clipped
C delete the link list and re-initialize INUCHN
                      HIT_ADR_LAST=HIT_ADR_LOW
 261                  CONTINUE
                      
                      sf_data_input_hit_adr=HIT_ADR_LAST
#define SF_DATA_GET_NEXT_HIT_CLUSTER
#include "doit/sf_data_access/sf_data_access.inc"
                      HIT_ADR=sf_data_output_nxt_clus
                      
                      IF(HIT_ADR.NE.0)THEN
                        
                        sf_data_input_hit_adr=HIT_ADR_LAST
                        sf_data_input_nxt_clus=0
#define SF_DATA_PUT_NEXT_HIT_CLUSTER
#include "doit/sf_data_access/sf_data_access.inc"
                        
                        sf_data_input_hit_adr=HIT_ADR
                        sf_data_input_primary=0
#define SF_DATA_PUT_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"
                        
                        sf_data_input_hit_adr=HIT_ADR
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
                        SF_WIRE=sf_data_output_wire
                        
C get the SF address
                        SF_CELL_ID=INSFSF(LYRSF)+SF_WIRE
                        INUCHN(SF_CELL_ID)=-9
                        
                        HIT_ADR_LAST=HIT_ADR
                        GO TO 261
                      ENDIF
                      SPLIT_AT_DIP=.TRUE.
                    ELSE
                      SPLIT_AT_DIP=.FALSE.
                    ENDIF
                  ENDIF
                  
                  IF(.NOT.SPLIT_AT_DIP)GO TO 241
                  
C end condition (PULESH.GT.CLS_Q_A_MIN(ILYR)); pulse height
                ENDIF
C end condition, valid hit in INUCHN
              ENDIF
C end of condition; not previously used
            ENDIF
C end condition (HIT_ADR.GT.0); tested pad exists
          ENDIF
C end condition (.NOT.FULLY_WRAPPED); have not wrapped all the way around
        ENDIF
        
        IF(IDIR.NE.1)GO TO 201
        
        
C this places the most significant secondary hit into
C  the list available for SFIND, and therefore into 
C   the hit list for the "wire position fit"
        
C in SFHITS, the lessor hits in the cluster are added to 
C   the hit list for the purpose of future clutering,   
        
        IF(PHFRAC_HOLD_ADJ.GT.CLS_F_E_MIN(LYRTF))THEN
          INUCHN(SF_CELL_ID_HOLD_ADJ)=0
        ENDIF
        
C-----------------------------------------------------------------------
C loop back to do the next peak ELEMENT
C-----------------------------------------------------------------------
        FIRST_LOOP_FOR_PEAK=.FALSE.
        GO TO 111
        
C end of condition that peak is found
      ENDIF
      
      RETURN
      END







