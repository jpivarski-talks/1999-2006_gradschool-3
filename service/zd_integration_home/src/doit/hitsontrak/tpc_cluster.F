*
* $Id: tpc_cluster.F,v 1.1 2003/03/05 17:57:58 dpp Exp $
*
* $Log: tpc_cluster.F,v $
* Revision 1.1  2003/03/05 17:57:58  dpp
*      -> NEW ROUTINE
*      -> cluster pads within the tfhitscd arrays
*      -> changed all variable names in cdscrtcd to have common root
*      -> fully modified for multi-hit
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TPC_CLUSTER(RoadSeqLayer)
C.......................................................................
C.
C. TPC_CLUSTER - cluster TPC pads
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson, adapted from sf_tpc_cluster
C. VERSION   : 1.00
C. CREATED   : 06-Nov-2002
C. LAST MOD  : 
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"

      LOGICAL FIRST_LOOP_FOR_PEAK

      INTEGER RoadSeqLayer
      INTEGER RoadSeqHit

      INTEGER LYRTF

      INTEGER HIT_ADR,       HIT_ADR_PEAK,     HIT_ADR_LAST
      INTEGER HIT_ADR_LOW

      INTEGER CD_WIRE,       CD_WIRE_PEAK,     CD_WIRE_LAST

      REAL    QMAX
      REAL    PULSEH,        PULSEH_PEAK

      REAL    TIME,          TIME_PEAK

      REAL    PULSEH_ABV,    PULSEH_ABV_LOW
      REAL    PULSEH_ABV_SUM,PULSEH_ABV_PEAK
      REAL    PHFRAC_DIP

      INTEGER HITS_FOUND_ON_WIRE

      INTEGER ISEARCH,IDIR
      INTEGER W_DIFF

      LOGICAL FULLY_WRAPPED
      LOGICAL CONT_IN_DIRECTION

      REAL PH_sum
      REAL PHweighPhiWsm
      REAL PhiW,PhiW_PEAK,PHI_DIF

      INTEGER LocalMHIT

      LOGICAL DUMPIT

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

C-----------------------------------------------------------------------
C layer numbers
C-----------------------------------------------------------------------

      LYRTF=IPLRLR(RoadSeqLayer)
      FIRST_LOOP_FOR_PEAK=.TRUE.

#if defined(CLEO_ROADHITSDIAG)
      PRINT 9911,
     1     RoadSeqLayer,
     2     LHITLR(RoadSeqLayer),MHITLR(RoadSeqLayer),
     3     LYRTF,CLS_Q_P_MIN(LYRTF),CLS_Q_A_MIN(LYRTF)
 9911 FORMAT(' tpc_cluster:   enter:',
     1     ' seq=',I3,
     2     ' range in hit list={',I6,':',I6'}',
     3     ' TF layer:',I3,' Qpeak,Qadd=',2F6.1)
#endif

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

      RoadSeqHit=LHITLR(RoadSeqLayer)

 131  CONTINUE
      HIT_ADR=IADRHT(RoadSeqHit)

      IF(
     1     (RoadSeqHit.LE.MHITLR(RoadSeqLayer)).AND.
     2     (HIT_ADR.GT.0))THEN
        
        IF(FIRST_LOOP_FOR_PEAK)THEN
          EvntHit_PrimCls(HIT_ADR)=0
          EvntHit_NxtCls(HIT_ADR)=0
        ENDIF
        
C use  EvntHit_PrimCls(HIT_ADR)=0 to denote 
C   valid non-clustered hits still under consideration
        
        IF(EvntHit_PrimCls(HIT_ADR).EQ.0)THEN
          
C select highest pulse height in the layer
          
          PULSEH=EvntHit_Charge(HIT_ADR)
          IF(PULSEH.GT.QMAX) THEN
            
            CD_WIRE=EvntHit_laWire(HIT_ADR)
            
C make no cuts on time correlation, that is done with the window
C all hits within the window are considered in-time
            
            HIT_ADR_PEAK=    HIT_ADR
            CD_WIRE_PEAK=    CD_WIRE
            QMAX=            PULSEH

C end of condition of new high pulse height
          ENDIF
C end condition, valid hit in EvntHit_PrimCls
        ENDIF
        
C process next hit
        RoadSeqHit=RoadSeqHit+1
        GO TO 131
        
C end of valid hit condition
      ENDIF
      
C stop processing the input section if there is no valid peak
      
      IF(HIT_ADR_PEAK.GT.0)THEN

C set the primary hit number for the peak of the cluster

        EvntHit_PrimCls(HIT_ADR_PEAK)=HIT_ADR_PEAK

C fill in other quantities for the peak

        PULSEH_PEAK=       QMAX
        TIME_PEAK=         EvntHit_rawTIM(HIT_ADR_PEAK)

        PhiW_PEAK=         EvntHit_PhiWir(HIT_ADR_PEAK)
        PH_sum=            PULSEH_PEAK
        PHweighPhiWsm=     PULSEH_PEAK*PhiW_PEAK

        HITS_FOUND_ON_WIRE=1
        PULSEH_ABV_SUM=    PULSEH_PEAK-CLS_Q_A_MIN(LYRTF)
        PULSEH_ABV_PEAK=PULSEH_ABV_SUM

        DUMPIT=.FALSE.
        IF(
     1       (HIT_ADR_PEAK.eq.16632).or.
     1       (HIT_ADR_PEAK.eq.34776).or.
     1       (HIT_ADR_PEAK.eq.10949)
     1       )DUMPIT=.TRUE.


#if defined(CLEO_ROADHITSDIAG)
        IF(DUMPIT)THEN
          PRINT 9912,
     1         CD_WIRE_PEAK,HIT_ADR_PEAK,
     2         PhiW_PEAK,PULSEH_PEAK,
     3         PHweighPhiWsm,PH_sum
        ENDIF
 9912   FORMAT(' tpc_cluster:    peak:',
     1       '      wire=',I6,'  HitAdr=',I6,
     2       '  Phi=',F6.2,'  Q=',F8.2,
     3       '  PHweighPhiWsm=',F8.2,'  PH_sum=',F8.2)
#endif


        
C-----------------------------------------------------------------------
C FILL IN THE CLUSTER
C-----------------------------------------------------------------------

C this version counts down through the geometry 
C rather than an input hit list
C there is no guarantee that hits are in order in the hit list
C ISEARCH {1,2,3} counts the directions searched {0,-1,1}
C direction=0 is used to pick up multi-hits in the peak cell
 
        ISEARCH=0

C-----------------------------------------------------------------------
C start point for new direction
C-----------------------------------------------------------------------

 201    ISEARCH=ISEARCH+1
        IF(ISEARCH.LE.3)THEN

C-----------------------------------------------------------------------
C look for other hits on the same cell of multi-hit cells
C-----------------------------------------------------------------------

          IF(ISEARCH.EQ.1)THEN
            IDIR=0
            HIT_ADR_LAST=HIT_ADR_PEAK
C Do reset CD_WIRE_LAST to the peak value, it determines the path

C-----------------------------------------------------------------------
C skip down to the first connected strip in the cluster
C-----------------------------------------------------------------------

          ELSEIF(ISEARCH.EQ.2)THEN
            IDIR=-1
C Do not change the HIT_ADR_LAST, it is the last hit address from 
C the downward search, or is still the peak, it is needed for EvntHit_NxtCls.
C Do reset CD_WIRE_LAST to the peak value, it determines the path
            
C-----------------------------------------------------------------------
C skip up to the last connected strip in the cluster
C-----------------------------------------------------------------------
            
          ELSEIF(ISEARCH.EQ.3)THEN
            IDIR=1
C Do not change the HIT_ADR_LAST, it is the last hit address from 
C the downward search, or is still the peak, it is needed for EvntHit_NxtCls.
C Do reset CD_WIRE_LAST to the peak value, it determines the path
            
          ENDIF
          
C-----------------------------------------------------------------------
C common initialization for the start of any direction
C-----------------------------------------------------------------------
          
          CD_WIRE_LAST=CD_WIRE_PEAK
          PULSEH_ABV_LOW=PULSEH_ABV_PEAK
          
C-----------------------------------------------------------------------
C start point for a testing new hit in the current direction 
C-----------------------------------------------------------------------
          
 241      CONTINUE
          
C test that it is not wrapped all the way around
          
          W_DIFF=CD_WIRE_LAST-CD_WIRE_PEAK
          IF(W_DIFF.GE.NWIRTF(LYRTF))W_DIFF=W_DIFF-NWIRTF(LYRTF)
          IF(W_DIFF.LT.0            )W_DIFF=W_DIFF+NWIRTF(LYRTF)
          
          IF(IDIR.EQ.0)THEN
            FULLY_WRAPPED=.FALSE.
          ELSEIF(IDIR.EQ.-1)THEN
            FULLY_WRAPPED=W_DIFF.EQ.(1)
          ELSEIF(IDIR.EQ.1)THEN
            FULLY_WRAPPED=W_DIFF.EQ.(NWIRTF(LYRTF)-1)
          ENDIF
          
          IF(.NOT.FULLY_WRAPPED)THEN
            
C-----------------------------------------------------------------------
C get a new CD wire
C (this is a bit of a fake, mixing TF and CD geometry)
C initialize cell sum  quantities
C-----------------------------------------------------------------------
            
            CD_WIRE=CD_WIRE_LAST+IDIR
            IF(CD_WIRE.GE.NWIRTF(LYRTF))CD_WIRE=CD_WIRE-NWIRTF(LYRTF)
            IF(CD_WIRE.LT.0            )CD_WIRE=CD_WIRE+NWIRTF(LYRTF)

            IF(IDIR.NE.0)THEN
              HITS_FOUND_ON_WIRE=0
              PULSEH_ABV_SUM=0.
            ENDIF

C-----------------------------------------------------------------------
C loop through pads in this layers looking for the target pad; 
C-----------------------------------------------------------------------
            
            RoadSeqHit=LHITLR(RoadSeqLayer)

#if defined(CLEO_ROADHITSDIAG)
            IF(DUMPIT)THEN
              PRINT 9913,
     1             CD_WIRE,IDIR
            ENDIF
 9913       FORMAT(' tpc_cluster: looking:',
     1           '  for wire=',I6,' direction',I4)
#endif
            
            
 251        CONTINUE
            HIT_ADR=IADRHT(RoadSeqHit)
            IF(RoadSeqHit.LE.MHITLR(RoadSeqLayer))THEN
              
C require usable hit
              IF(HIT_ADR.GT.0)THEN
                
                
#if defined(CLEO_ROADHITSDIAG)
                IF(DUMPIT)THEN
                  PRINT 9914,
     1                 CD_WIRE,IDIR,
     2                 HIT_ADR,EvntHit_laWire(HIT_ADR),
     3                 EvntHit_PrimCls(HIT_ADR),
     4                 EvntHit_Charge(HIT_ADR)
                ENDIF
 9914             FORMAT(' tpc_cluster: looking:',
     1                 '  for wire=',I6,' direction',I4,
     2                 '  this is HitAdr=',I6,'  wire',I4,
     3                 '  attach:',I6,
     4                 '  Q=',F6.2)
#endif
                  
                IF(EvntHit_laWire(HIT_ADR).EQ.CD_WIRE)THEN
                                    
C use  EvntHit_PrimCls(HIT_ADR)=0 to denote 
C valid non-clustered hits still under consideration
                  
                  IF(EvntHit_PrimCls(HIT_ADR).EQ.0)THEN
                    
C pulse height cut                  
                    
                    PULSEH=EvntHit_Charge(HIT_ADR)
                    PULSEH_ABV=PULSEH-CLS_Q_A_MIN(LYRTF)
                    IF(PULSEH_ABV.GT.0.)THEN
C time difference cut
                      TIME= EvntHit_rawTIM(HIT_ADR)
                      IF(ABS(TIME-TIME_PEAK).LT.CLS_TC_MAX(LYRTF))THEN
                        
C add all hits, on this CD_WIRE, satisfying criteria, to the cluster
                        
                        EvntHit_NxtCls(HIT_ADR_LAST)=HIT_ADR
                        EvntHit_PrimCls(HIT_ADR)=HIT_ADR_PEAK
                        
                        HIT_ADR_LAST=HIT_ADR
                        CD_WIRE_LAST=CD_WIRE
                        
                        PhiW=EvntHit_PhiWir(HIT_ADR)
                        PHI_DIF=PhiW-PhiW_PEAK
                        IF(PHI_DIF.GT. PI)PhiW=PhiW-TWOPI
                        IF(PHI_DIF.LE.-PI)PhiW=PhiW+TWOPI
                        PH_sum=PH_sum+PULSEH
                        PHweighPhiWsm=PHweighPhiWsm+PULSEH*PhiW
                        
                        HITS_FOUND_ON_WIRE=HITS_FOUND_ON_WIRE+1
                        PULSEH_ABV_SUM=PULSEH_ABV_SUM+PULSEH_ABV
                        
                        
#if defined(CLEO_ROADHITSDIAG)
                        IF(DUMPIT)THEN
                          PRINT 9915,
     1                         CD_WIRE,HIT_ADR,
     2                         PhiW,PULSEH,
     3                         PHweighPhiWsm,PH_sum
                        ENDIF
 9915                   FORMAT(' tpc_cluster: found it:',
     1                       '      wire=',I6,'  HitAdr=',I6,
     2                       '  Phi=',F6.2,'  Q=',F8.2,
     3                       '  PHweighPhiWsm=',F8.2,
     4                       '  PH_sum=',F8.2)
#endif
                        
C end of condition (ABS(TIME-TIME_PEAK).LT.CLS_TC_MAX(LYRTF)); time cut
                      ENDIF
C end condition (PULSEH.GT.CLS_Q_A_MIN(ILYR)); pulse height
                    ENDIF
C end condition, valid hit in EvntHit_PrimCls
                  ENDIF
C end condition (EvntHit_laWire(HIT_ADR).EQ.CD_WIRE)
                ENDIF
C end condition that the hit is usable
              ENDIF
C end of condition that hit is on required layer; return with next hit
              RoadSeqHit=RoadSeqHit+1
              GO TO 251
            ENDIF
C return to test next cell if something found on last cell
            IF(HITS_FOUND_ON_WIRE.GE.1)THEN
              
C-----------------------------------------------------------------------
C splitting is now based on pad sum
C This version (unlike SF) uses the PH ABoVe the minimum for adding a pad
C  for the dip criteria. This natuarally avoids divide by zero.
C-----------------------------------------------------------------------
              
#if defined(CLEO_ROADHITSDIAG)
              IF(DUMPIT)THEN
                PRINT 9916,
     1               CD_WIRE,HIT_ADR_LAST,
     2               IDIR,
     3               PULSEH_ABV_SUM,PULSEH_ABV_LOW
              ENDIF
 9916         FORMAT(' tpc_cluster: at end of',
     1             '     wire=',I6,'  HitAdr=',I6,
     2             '  IDIR=',I2,
     3             '  PH sum_ABV=',F8.2,
     4             '  PH _LOW=',F8.2)
#endif

              IF(IDIR.EQ.0)THEN
                PULSEH_ABV_PEAK=PULSEH_ABV_SUM
                HIT_ADR_LOW=HIT_ADR_LAST
                CONT_IN_DIRECTION=.FALSE.
                
              ELSE
                IF(PULSEH_ABV_SUM.LT.PULSEH_ABV_LOW)THEN
                  PULSEH_ABV_LOW=PULSEH_ABV_SUM
                  HIT_ADR_LOW=HIT_ADR_LAST
                  CONT_IN_DIRECTION=.TRUE.
                ELSE
                  
C test for split if current location is not a new low
                  
                  PHFRAC_DIP=PULSEH_ABV_LOW/PULSEH_ABV_SUM
                  IF(PHFRAC_DIP.GT.CLS_F_C_MAX(LYRTF))THEN
                    CONT_IN_DIRECTION=.TRUE.
                    
                  ELSE

#if defined(CLEO_ROADHITSDIAG)
                    IF(DUMPIT)THEN
                      PRINT 9917,
     1                     CD_WIRE,HIT_ADR_LAST,
     2                     IDIR,
     3                     HIT_ADR_LOW
                    ENDIF
 9917               FORMAT(' tpc_cluster: at end of',
     1                   '     wire=',I6,'  HitAdr=',I6,
     2                   '  IDIR=',I2,
     3                   '  split after hit addres=',I6)
#endif
                    
C HIT_ADR_LOW stays with cluster, everything else is clipped
C delete the link list
C remove contribution to average phi
                    
                    HIT_ADR_LAST=HIT_ADR_LOW
 271                CONTINUE
                    
                    HIT_ADR=EvntHit_NxtCls(HIT_ADR_LAST)
                    IF(HIT_ADR.NE.0)THEN
                      
                      EvntHit_NxtCls(HIT_ADR_LAST)=0
                      EvntHit_PrimCls(HIT_ADR)=0
                      
                      PhiW=EvntHit_PhiWir(HIT_ADR)
                      PHI_DIF=PhiW-PhiW_PEAK
                      IF(PHI_DIF.GT. PI)PhiW=PhiW-TWOPI
                      IF(PHI_DIF.LE.-PI)PhiW=PhiW+TWOPI
                      PULSEH=EvntHit_Charge(HIT_ADR)
                      PH_sum=PH_sum-PULSEH
                      PHweighPhiWsm=PHweighPhiWsm-PULSEH*PhiW
                      
                      HIT_ADR_LAST=HIT_ADR
                      GO TO 271
                    ENDIF
                    CONT_IN_DIRECTION=.FALSE.
                  ENDIF
                ENDIF

C end of condition that (IDIR.EQ.0), or not                
              ENDIF
              
C-----------------------------------------------------------------------
C return to test next cell if something found on last cell, and not split
C-----------------------------------------------------------------------

              IF(CONT_IN_DIRECTION)GO TO 241
            ENDIF

C end condition (.NOT.FULLY_WRAPPED); have not wrapped all the way around
          ENDIF
C end of loop over search "directions"
          GO TO 201
        ENDIF
        
#if defined(CLEO_ROADHITSDIAG)
        IF(DUMPIT)THEN
          PRINT 9918,
     1         CD_WIRE_PEAK,HIT_ADR_PEAK,
     2         PHweighPhiWsm,PH_sum
        ENDIF
 9918   FORMAT(' tpc_cluster: cluster:',
     1       ' peak wire=',I6,'  HitAdr=',I6,
     2       '  phw=',F8.2,' phsum=',F8.2)
#endif

        IF(PH_sum.GT.0.)THEN
          PHweighPhiWsm=PHweighPhiWsm/PH_sum
          IF(PHweighPhiWsm.GE.TWOPI)PHweighPhiWsm=PHweighPhiWsm-TWOPI
          IF(PHweighPhiWsm.LT.0.   )PHweighPhiWsm=PHweighPhiWsm+TWOPI
          EvntHit_corPhi(HIT_ADR_PEAK)=PHweighPhiWsm
        ENDIF
        
C-----------------------------------------------------------------------
C loop back to do the next peak ELEMENT
C-----------------------------------------------------------------------
        FIRST_LOOP_FOR_PEAK=.FALSE.
        GO TO 111
        
C end of condition that peak is found
      ENDIF

C-----------------------------------------------------------------------
C PURGE
C-----------------------------------------------------------------------
      RoadSeqHit=LHITLR(RoadSeqLayer)
      LocalMHIT=LHITLR(RoadSeqLayer)-1
 301  CONTINUE
      HIT_ADR=IADRHT(RoadSeqHit)
      
      IF(
     1     (RoadSeqHit.LE.MHITLR(RoadSeqLayer)).AND.
     2     (HIT_ADR.GT.0))THEN
        IF(EvntHit_PrimCls(HIT_ADR).EQ.HIT_ADR)THEN
          LocalMHIT=LocalMHIT+1
          IPLHIT(LocalMHIT)=IPLHIT(RoadSeqHit)
          IADRHT(LocalMHIT)=IADRHT(RoadSeqHit)
          OTHRHT(LocalMHIT)=OTHRHT(RoadSeqHit)
          STWRHT(LocalMHIT)=STWRHT(RoadSeqHit)
          DRFHIT(LocalMHIT)=DRFHIT(RoadSeqHit)
          STRFHT(LocalMHIT)=STRFHT(RoadSeqHit)
          STSZHT(LocalMHIT)=STSZHT(RoadSeqHit)
          XHIT(LocalMHIT)=  XHIT(RoadSeqHit)
          YHIT(LocalMHIT)=  YHIT(RoadSeqHit)
          SHIT(LocalMHIT)=  SHIT(RoadSeqHit)
          ZHIT(LocalMHIT)=  ZHIT(RoadSeqHit)
        ENDIF
        
C process next hit
        RoadSeqHit=RoadSeqHit+1
        GO TO 301
      ENDIF
      MHITLR(RoadSeqLayer)=LocalMHIT

#if defined(CLEO_ROADHITSDIAG)
      PRINT 9919,
     1     RoadSeqLayer,
     2     LHITLR(RoadSeqLayer),MHITLR(RoadSeqLayer)
 9919 FORMAT(' tpc_cluster: purged:',
     1     ' seq=',I3,
     2     ' range in hit list={',I6,':',I6'}')
#endif

      RETURN
      END

