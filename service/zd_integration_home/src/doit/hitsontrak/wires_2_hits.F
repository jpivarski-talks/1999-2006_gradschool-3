* $Id: wires_2_hits.F,v 1.16 2003/03/05 17:59:20 dpp Exp $
*
* $Log: wires_2_hits.F,v $
* Revision 1.16  2003/03/05 17:59:20  dpp
*      -> diagnostic format
*      -> install location for end-of-layer procedures
*      -> changed all variable names in cdscrtcd to have common root
*      -> change the name of the hit level arrays again, to RoadWirLst_
*      -> loop over hits on multi-hit layer, put all in list
*
* Revision 1.15  2002/09/05 19:04:18  dpp
*      -> format
*      -> drift chamber / TPC dependent code
*      -> protect against overflow of layer lists
*      -> change names of wire-level arrays to avoid confusion
*      -> fill the hit level copy of the wire status
*
* Revision 1.14  2001/11/19 23:55:32  dpp
*      -> add new control parameter include file
*
* Revision 1.13  2001/09/14 21:18:09  dpp
*      -> fill pointer to hit coming from same wire
*
* Revision 1.12  2001/03/30 00:04:29  dpp
*      -> assign EFFILR to default value: =1.
*
* Revision 1.11  2000/12/19 23:56:57  dpp
*      -> added diagnostics
*
* Revision 1.10  2000/11/15 16:58:31  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.9  2000/10/24 22:45:22  dpp
*      -> set UPTODATE_LYR flag
*
* Revision 1.8  2000/01/06 22:06:46  dpp
* apply offset controlled in fast_trak
*
* Revision 1.7  1999/12/09 21:13:07  dpp
* fill STRFHT and STSZHT with HIT_TYP rather than LVL_TYP
* change call to hit_to_fit to use parameter directive
*
* Revision 1.6  1999/11/17 21:41:33  dpp
* add couter for number of calls
*
* Revision 1.5  1999/11/03 22:23:41  dpp
* set UPTODATE_HIT =FALSE when input conditions change; =TRUE on exit
* use UPTODATE_HIT flag to skip uneeded code
*
* Revision 1.4  1999/09/12 22:23:29  dpp
* sets STRFHT; calls HIT_TO_FIT
*
* Revision 1.3  1999/08/03 21:55:53  dpp
* hit buffer saturation
*
* Revision 1.2  1998/02/24 18:28:01  dpp
* fit the syntax for the Id and Log
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE WIRES_2_HITS(LVL_TYP,HIT_TYP,FIT_TYP)
C.......................................................................
C.
C. WIRES_2_HITS - converts from RoadWirLst_ wire list 
C.                 to TFHITSCD data structure
C.
C. COMMON    : TFHITSCD
C. CALLS     : 
C. CALLED    : DUFIND, FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 30-Oct-97
C. LAST MOD  : 30-Oct-97
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
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/fast_trak.inc"

#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"


C---------------------------------------------------------------------
C LOCAL VARIABLE DOCUMENTATION
C---------------------------------------------------------------------
C LVL_TYP.........input level,      LVLTYP_PAT_REC or LVLTYP_TFXFIT
C HIT_TYP.........input hit type,   HITTYP_WIRE or HITTYP_HIT
C FIT_TYP.........input fit use,    FITTYP_CF of FITTYP_LF
      INTEGER LVL_TYP,HIT_TYP,FIT_TYP

C COUNT_CALL......counter of number of calls
C                 zeroed with input (LVLTYP_ACCESS,0,n)
C                 output to "FIT_TYP" with input (LVLTYP_ACCESS,1,FIT_TYP)
      INTEGER COUNT_CALL
      DATA COUNT_CALL/0/

C IHIT_WIRE.......COUNTER OF HIT NUMBER
C ADR_PASS........current hit address in EvntHit passed through the wire list
C ADR.............current hit address in EvntHit to put into the hit list
C                 may differ from adr_pass when looping through multi-hits
C ILYR............PHYSICAL LAYER NUMBER
C ILLYR...........LOGICAL LAYER NUMBER
C ILYRL...........LAYER NUMBER OF LAST HIT
C IAMB............SIDE OF WIRE
C NEXTAMB.........NEXT SIDE OF THE WIRE TO USE, 0: IF ONLY USE ONE SIDE
C IER.............ERROR CODE
      INTEGER IHIT_WIRE,ADR_PASS,ADR,ILYR,ILLYR,ILYRL,IAMB,NEXTAMB,IER

C ZLAYER..........Z AT THE CURRENT LAYER
C COSW............COSINE PHI OF WIRE
C SINW............SINE PHI OF WIRE
C MULT............MULTIPLIER FOR DRIFT DISTANCE
C YSAG............WIRE SAG
C XINT............X   from intersection of track variables
C YINT............Y   from intersection of track variables
C SINT............S   from intersection of track variables
C ZINT............Z   from intersection of track variables
C FINT........... PHI from intersection of track variables
C RESI............residual of hit to the track variables
C VEL.............drift velocity calculated from 
C                 existing EvntHit_Drift/EvntHit_rawTIM

      REAL ZLAYER,COSW,SINW,MULT,YSAG
      REAL XINT,YINT,SINT,ZINT,FINT,RESI,CORR,VEL
      REAL ARCFUN

C HIT_STILL_OK....no problem, continue processing the current hit
      LOGICAL HIT_STILL_OK
C FOUND_NEW_LAYER.must finish current layer before continuing
      LOGICAL FOUND_NEW_LAYER

      LOGICAL DUMPIT


C-----------------------------------------------------------------------
C  Executable code starts here
C-----------------------------------------------------------------------

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      
      IF( (LVL_TYP.EQ.LVLTYP_PAT_REC).OR.
     2     (LVL_TYP.EQ.LVLTYP_TFXFIT ))THEN

#if defined(CLEO_DPPDIAG)
        COUNT_CALL=COUNT_CALL+1
#endif

C-----------------------------------------------------------------------
C check if the up-to-date status of the hits should be set =.FALSE.
C NOTE: FIT_TYP is not part of the condition so that the same set
C of hits can be alternately set up for either FIT_TYP value.
C store the input conditions for called subroutines
C-----------------------------------------------------------------------
#if defined(CLEO_ROADHITSDIAG)
        PRINT 9911,UPTODATE_HIT
 9911   FORMAT(' WIRES_2_HITS: enter,                            ',
     2       ' UPTODATE_HIT=',L1)
#endif 
        IF(UPTODATE_HIT)THEN
          IF((LVL_TYP.NE.SAV_LVL_TYP).OR.
     2         (HIT_TYP.NE.SAV_HIT_TYP))UPTODATE_HIT=.FALSE. 
        ENDIF

#if defined(CLEO_ROADHITSDIAG)
        PRINT 9912,UPTODATE_HIT
 9912   FORMAT(' WIRES_2_HITS: after checking level and hit type,',
     2       ' UPTODATE_HIT=',L1)
#endif 
        
        SAV_LVL_TYP=LVL_TYP
        SAV_HIT_TYP=HIT_TYP
        SAV_FIT_TYP=FIT_TYP
        
        
C-----------------------------------------------------------------------
C label the layers NOT up to date so that SLYRLR will be updated 
C under all circumstances (always assume new r-phi track parameters)
C-----------------------------------------------------------------------
        
        CALL VFILL(UPTODATE_LYR,KLYRDM,.FALSE.)
        
C-----------------------------------------------------------------------
C recalculate the hit structure only if there is a change
C-----------------------------------------------------------------------

        IF(.NOT.UPTODATE_HIT)THEN
          
C-----------------------------------------------------------------------
C zero out the pointers from hits to fit
C-----------------------------------------------------------------------
          
          CALL HIT_TO_FIT(H2F_ZERO_POINTER,0,0)

#if defined(CLEO_ROADHITSDIAG)
          PRINT 9913
 9913     FORMAT(' WIRES_2_HITS: after ZEROing POINTER')
#endif 

C-----------------------------------------------------------------------
C  correct wire position for "Z" at the layer radius
C    Y0LF and TANDLF must be set
C-----------------------------------------------------------------------

          NHIT=0
          ILYRL=0
          NLYRHT=0
          NLYRLR=0
          CALL VZERO(ILLRLR,NLYRTF)
          
C-----------------------------------------------------------------------
C manual loop over IHIT_WIRE so it can jump out at saturation
C-----------------------------------------------------------------------

          IHIT_WIRE=0
 11       CONTINUE
          IF((IHIT_WIRE.LT.RoadWirLst_Nhit).AND.(NHIT.LT.NHITDM))THEN
            HIT_STILL_OK=   .TRUE.
            FOUND_NEW_LAYER=.FALSE.
            IHIT_WIRE=IHIT_WIRE+1
            
C-----------------------------------------------------------------------
C this routine receives wire info from previously filled RoadWirLst_
C-----------------------------------------------------------------------
            
            ADR_PASS=RoadWirLst_adrs(IHIT_WIRE)
            ADR=ADR_PASS

C-----------------------------------------------------------------------
C RoadWirLst_adrs() will provide a specific hit which may be one of 
C multiple hits on a cell of a multi-hit capable cell
C move to the beginning of the multi-hit list in preparation 
C to loop through all hits in the multi-hit list
C-----------------------------------------------------------------------

C whoa, the pass address, ADR_PASS, was checked for being a valid hit
C must check each of these hits separately

            IF(
     1           (LCLSTR_LAYER(ILYRL)).AND.
     2           (SAV_HIT_TYP.EQ.HITTYP_HIT))THEN
              ADR=EvntHit_1stMlt(ADR_PASS)
            ENDIF

C-----------------------------------------------------------------------
C continue with the current hit
C-----------------------------------------------------------------------

            ILYR=ILCDTF(EvntHit_CDlayr(ADR))
            
#if defined(CLEO_ROADHITSDIAG)
            PRINT 9914,IHIT_WIRE,
     2           ADR,ILYR,
     3           EvntHit_CDlayr(ADR),EvntHit_laWire(ADR),
     4           INXSCD(EvntHit_CDlayr(ADR)),
     5           EvntHit_X(ADR),EvntHit_Y(ADR)
 9914       FORMAT(' WIRES_2_HITS: processing HIT',I5,
     2           ' ADR=',I7,'  ILYR=',I3,
     3           '  CDlayr=',I5,'  laWire=',I6,
     4           '  INXSCD=',I7
     5           '  x=',F7.3,' y=',F7.3)
#endif 

C-----------------------------------------------------------------------
C reached end of input list, or saturated output list; finish current layer
C-----------------------------------------------------------------------
            
          ELSE
            HIT_STILL_OK=   .FALSE.
            FOUND_NEW_LAYER=.TRUE.
          ENDIF
          
C-----------------------------------------------------------------------
C recognize new layer
C-----------------------------------------------------------------------
          
          IF((HIT_STILL_OK).AND.(ILYR.NE.ILYRL))THEN
            FOUND_NEW_LAYER=.TRUE.
          ENDIF
          
C-----------------------------------------------------------------------
C finish off previous layer; 
C note, TPC_CLUSTER is protected against MHITLR < LHITLR 
C-----------------------------------------------------------------------
          IF(
     1         (FOUND_NEW_LAYER).AND.(ILYRL.NE.0).AND.
     2         (NLYRHT.NE.0))THEN
            
            IF(
     1           (LCLSTR_LAYER(ILYRL)).AND.
     2           (SAV_HIT_TYP.EQ.HITTYP_HIT))THEN
              
              CALL TPC_CLUSTER(NLYRHT+0)
            ENDIF
            
            IF(MHITLR(NLYRHT).LT.LHITLR(NLYRHT))THEN
              NLYRHT=NLYRHT-1
            ENDIF
            
            IF(NLYRHT.NE.0)THEN
              NHIT=MHITLR(NLYRHT)
            ELSE
              NHIT=0
            ENDIF
            
          ENDIF
          
C-----------------------------------------------------------------------
C create the new layer
C-----------------------------------------------------------------------
          
          IF((FOUND_NEW_LAYER).AND.(HIT_STILL_OK))THEN
            ILYRL=ILYR
            IF(NLYRHT.LT.KLYRDM)THEN
              NLYRHT=NLYRHT+1
              IPLRLR(NLYRHT)=ILYR
              ILLRLR(ILYR)=NLYRHT
              LHITLR(NLYRHT)=NHIT+1
              MHITLR(NLYRHT)=NHIT
              EFFILR(NLYRHT)=1.
              
#if defined(CLEO_ROADHITSDIAG)
              PRINT 9915,NLYRHT,ILYR,KAPCF,ALPCF,BETCF,GAMCF
 9915         FORMAT(' WIRES_2_HITS:',
     1             '   creating layer, sequential ',I3,
     2             '  ILYR=',I3,'  kabg=',4F10.6)
#endif 
              
C-----------------------------------------------------------------------
C get the arc length to the intersection point on this layer
C !!!!!!! WARNING !!!!!!, TFPHTR is, but ARCFUN is not set up for kink
C-----------------------------------------------------------------------
              
              CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     2             RSQTF(ILYR),+1,XINT,YINT,FINT,IER)
              
#if defined(CLEO_ROADHITSDIAG)
              PRINT 9916,NLYRHT,ILYR,FINT,IER
 9916         FORMAT(' WIRES_2_HITS:',
     1             ' continuing layer, sequential ',I3,
     2             '  ILYR=',I3,
     3             ' return from TFPHTR, phi=',F10.6,'  IER=',I3)
#endif 
              IF(IER.EQ.0)THEN
                SLYRLR(NLYRHT)=ARCFUN(KAPCF,
     2               ALPCF,BETCF,XINT,YINT,XICF)
                UPTODATE_LYR(NLYRHT)=.TRUE.
              ENDIF
              
            ELSE
              HIT_STILL_OK=.FALSE.
            ENDIF
          ENDIF

C-----------------------------------------------------------------------
C continue with current hit wire in the (possibly new) layer
C-----------------------------------------------------------------------
          
          IF(HIT_STILL_OK)THEN
            
            COSW=EvntHit_X(ADR)/RTF(ILYR)
            SINW=EvntHit_Y(ADR)/RTF(ILYR)
            
            IF    (SAV_HIT_TYP.EQ.HITTYP_WIRE)THEN
              IAMB=1
              MULT=0.
              NEXTAMB=0
            ELSEIF(SAV_HIT_TYP.EQ.HITTYP_HIT )THEN
              IF(LAMB_D_LAYER(ILYR))THEN
                IAMB=-1
                MULT=-1.
                NEXTAMB=1
              ELSE
                IAMB=1
                MULT=0.
                NEXTAMB=0
              ENDIF
            ELSE
              IAMB=0
            ENDIF
            
 21         CONTINUE
            IF(IAMB.NE.0)THEN
              NHIT=NHIT+1
              IADRHT(NHIT)=ADR*IAMB
              IPLHIT(NHIT)=ILYR
              STWRHT(NHIT)=RoadWirLst_stat(IHIT_WIRE)

              OTHRHT(NHIT)=0
              IF(IADRHT(NHIT).EQ.-IADRHT(NHIT-1))THEN
                OTHRHT(NHIT)=NHIT-1
                OTHRHT(NHIT-1)=NHIT
              ENDIF
              
              IF(DRIFTDIR(ILYR).EQ.DRIFTDIR_PHI)THEN
                DRFHIT(NHIT)=EvntHit_Drift(ADR)*MULT
                STRFHT(NHIT)=SAV_HIT_TYP
                STSZHT(NHIT)=0
                XHIT(NHIT)=EvntHit_X(ADR)+DRFHIT(NHIT)*SINW
                YHIT(NHIT)=EvntHit_Y(ADR)+DRFHIT(NHIT)*COSW
                SHIT(NHIT)=SLYRLR(NLYRHT)
                ZHIT(NHIT)=Y0LF+TANDLF*SHIT(NHIT)
                
              ELSEIF(DRIFTDIR(ILYR).EQ.DRIFTDIR_Z)THEN
                DRFHIT(NHIT)=EvntHit_Drift(ADR)*MULT
                STRFHT(NHIT)=SAV_HIT_TYP
                STSZHT(NHIT)=SAV_HIT_TYP
                XHIT(NHIT)=EvntHit_X(ADR)
                YHIT(NHIT)=EvntHit_Y(ADR)
                SHIT(NHIT)=SLYRLR(NLYRHT)
                ZHIT(NHIT)=EvntHit_Z(ADR)
                EvntHit_corPhi(ADR)=EvntHit_PhiWir(ADR)
              ELSE
              ENDIF
              
C-----------------------------------------------------------------------
C load another hit on the same wire with the same hit address for 
C ambiguous drift sign layers
C-----------------------------------------------------------------------

              IF((NEXTAMB.NE.0).AND.(NHIT.LT.NHITDM))THEN
                IAMB=NEXTAMB
                NEXTAMB=0
                MULT=-MULT
                GO TO 21
              ENDIF
              
C-----------------------------------------------------------------------
C load another hit on the same wire with a different hit address for 
C multi hit layers
C-----------------------------------------------------------------------

C whoa, the pass address, ADR_PASS, was checked for being a valid hit
C must check each of these hits separately

              IF(
     1             (LCLSTR_LAYER(ILYRL)).AND.
     2             (SAV_HIT_TYP.EQ.HITTYP_HIT).AND.
     3             (NHIT.LT.NHITDM))THEN
                ADR=EvntHit_NxtMlt(ADR)
                IF(ADR.GT.0)THEN
                  GO TO 21
                ENDIF
              ENDIF

C-----------------------------------------------------------------------
C loop back for new passed hit wire
C-----------------------------------------------------------------------
              
              MHITLR(NLYRHT) = NHIT
              GO TO 11
              
C-----------------------------------------------------------------------
C end of testing for valid hit
C-----------------------------------------------------------------------
            ENDIF
C-----------------------------------------------------------------------
C end of processing current passed hit wire
C-----------------------------------------------------------------------
          ENDIF
C-----------------------------------------------------------------------
C end of recalculating the hit structure
C-----------------------------------------------------------------------
        ENDIF
        
        IF(SAV_HIT_TYP.EQ.HITTYP_HIT)THEN
          
#if defined(CLEO_ROADHITSDIAG)
          PRINT 9917,NLYRHT,NHIT
 9917     FORMAT(' WIRES_2_HITS:',
     1         ' NHITLR=',I4,'  NHIT=',I6,'  calling ROADHITS_COORD')
#endif
          
          CALL ROADHITS_COORD
        ENDIF
        
        UPTODATE_HIT=.TRUE.
        
#if defined(CLEO_ROADHITSDIAG)
        PRINT 9918,NLYRHT,NHIT
 9918   FORMAT(' WIRES_2_HITS:',
     1       ' NHITLR=',I4,'  NHIT=',I6,'  exit')
#endif
C-----------------------------------------------------------------------
C still in test on LVL_TYP;  count the number of calls
C-----------------------------------------------------------------------
        
#if defined(CLEO_DPPDIAG)
      ELSEIF(LVL_TYP.EQ.LVLTYP_ACCESS)THEN
        IF(HIT_TYP.EQ.0)THEN
          COUNT_CALL=0
        ELSEIF(HIT_TYP.EQ.1)THEN
          PRINT 1011,COUNT_CALL
 1011     FORMAT(' WIRES_2_HITS: called',I5,' times')
        ENDIF
#endif
        
C-----------------------------------------------------------------------
C end of test on LVL_TYP
C-----------------------------------------------------------------------
      ENDIF
      
      RETURN
      END

