* $Id: wires_2_hits.F,v 1.13 2001/09/14 21:18:09 dpp Exp $
*
* $Log: wires_2_hits.F,v $
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
C. WIRES_2_HITS - converts from HTWADR wire list to TFHITSCD data sctructure
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
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/fast_trak.inc"

C---------------------------------------------------------------------
C LOCAL VARIABLE DOCUMENTATION
C---------------------------------------------------------------------
C LVL_TYP....input level,      USE_PAT_REC or USE_TFXFIT
C HIT_TYP....input hit type,   USE_WIRE or USE_HIT
C FIT_TYP....input fit use,    USE_CF of USE_LF
      INTEGER LVL_TYP,HIT_TYP,FIT_TYP

C COUNT_CALL.counter of number of calls
C            zeroed with input (USE_ACCESS,0,n)
C            output to "FIT_TYP" with input (USE_ACCESS,1,FIT_TYP)
      INTEGER COUNT_CALL
      DATA COUNT_CALL/0/

C IHIT.......COUNTER OF HIT NUMBER
C IADR.......TEMPORARY HOLD THE CURRENT WIRE HIT ADDRESS
C ILYR.......PHYSICAL LAYER NUMBER
C ILLYR......LOGICAL LAYER NUMBER
C ILYRL......LAYER NUMBER OF LAST HIT
C IAMB.......SIDE OF WIRE
C NEXTAMB....NEXT SIDE OF THE WIRE TO USE, 0: IF ONLY USE ONE SIDE
C IER........ERROR CODE
      INTEGER IHIT,IADR,ILYR,ILLYR,ILYRL,IAMB,NEXTAMB,IER

C ZLAYER.....Z AT THE CURRENT LAYER
C COSW.......COSINE PHI OF WIRE
C SINW.......SINE PHI OF WIRE
C MULT.......MULTIPLIER FOR DRIFT DISTANCE
C YSAG.......WIRE SAG
C XINT.......X   from intersection of track variables
C YINT.......Y   from intersection of track variables
C SINT.......S   from intersection of track variables
C ZINT.......Z   from intersection of track variables
C FINT.......PHI from intersection of track variables
C RESI.......residual of hit to the track variables
C VEL........drift velocity calculated from existing DWCD/TDWCD

      REAL ZLAYER,COSW,SINW,MULT,YSAG
      REAL XINT,YINT,SINT,ZINT,FINT,RESI,CORR,VEL

      REAL ARCFUN

C---------------------------------------------------------------------
C  Executable code starts here
C---------------------------------------------------------------------

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      IF( (LVL_TYP.EQ.USE_PAT_REC).OR.
     2        (LVL_TYP.EQ.USE_TFXFIT ))THEN

#if defined(CLEO_DPPDIAG)
         COUNT_CALL=COUNT_CALL+1
#endif

C---------------------------------------------------------------------
C check if the up-to-date status of the hits should be set =.FALSE.
C NOTE: FIT_TYP is not part of the condition so that the same set
C of hits can be alternately set up for either FIT_TYP value.
C store the input conditions for called subroutines
C---------------------------------------------------------------------
#if defined(CLEO_ROADHITSDIAG)
         PRINT 9911,UPTODATE_HIT
 9911    FORMAT(' WIRES_2_HITS: enter,                            ',
     2        ' UPTODATE_HIT=',L1)
#endif 
         IF(UPTODATE_HIT)THEN
            IF((LVL_TYP.NE.SAV_LVL_TYP).OR.
     2         (HIT_TYP.NE.SAV_HIT_TYP))UPTODATE_HIT=.FALSE. 
         ENDIF

#if defined(CLEO_ROADHITSDIAG)
         PRINT 9912,UPTODATE_HIT
 9912    FORMAT(' WIRES_2_HITS: after checking level and hit type,',
     2        ' UPTODATE_HIT=',L1)
#endif 
         
         SAV_LVL_TYP=LVL_TYP
         SAV_HIT_TYP=HIT_TYP
         SAV_FIT_TYP=FIT_TYP


C---------------------------------------------------------------------
C label the layers NOT up to date so that SLYRLR will be updated 
C under all circumstances (always assume new r-phi track parameters)
C---------------------------------------------------------------------

         CALL VFILL(UPTODATE_LYR,KLYRDM,.FALSE.)

C---------------------------------------------------------------------
C recalculate the hit structure only if there is a change
C---------------------------------------------------------------------

         IF(.NOT.UPTODATE_HIT)THEN

C---------------------------------------------------------------------
C zero out the pointers from hits to fit
C---------------------------------------------------------------------

            CALL HIT_TO_FIT(H2F_ZERO_POINTER,0,0)

#if defined(CLEO_ROADHITSDIAG)
            PRINT 9913
 9913       FORMAT(' WIRES_2_HITS: after ZEROing POINTER')
#endif 

C---------------------------------------------------------------------
C  correct wire position for "Z" at the layer radius
C    Y0LF and TANDLF must be set
C---------------------------------------------------------------------

            NHIT=0
            ILYRL=0
            NLYRHT=0
            NLYRLR=0
            CALL VZERO(ILLRLR,NLYRTF)

C manual loop over IHIT so it can jump out at saturation
            IHIT=0
 11         CONTINUE
            IF((IHIT.LT.NHTWIR).AND.(NHIT.LT.NHITDM))THEN
               IHIT=IHIT+1
               NHIT=NHIT+1
            
C---------------------------------------------------------------------
C so, this routine receives wire info from previously filled HTWADR
C---------------------------------------------------------------------

               IADR=HTWADR(IHIT)
               ILYR=ILCDTF(ICYLCD(IADR))

#if defined(CLEO_ROADHITSDIAG)
            PRINT 9914,IHIT,IADR,ILYR
 9914       FORMAT(' WIRES_2_HITS: processing HIT',I4,
     2           ' IADR=',I5,'  ILYR=',I3)
#endif 
         
               IF(ILYR.NE.ILYRL)THEN
                  ILYRL=ILYR
                  NLYRHT=NLYRHT+1
                  IPLRLR(NLYRHT)=ILYR
                  ILLRLR(ILYR)=NLYRHT
                  LHITLR(NLYRHT)=NHIT
                  EFFILR(NLYRHT)=1.

#if defined(CLEO_ROADHITSDIAG)
            PRINT 9915,NLYRHT,ILYR,KAPCF,ALPCF,BETCF,GAMCF
 9915       FORMAT(' WIRES_2_HITS: processing layer, sequential ',I2,
     2           '  ILYR=',I3,'  kabg=',4F10.6)
#endif 
               
C---------------------------------------------------------------------
C get the arc length to the intersection point on this layer
C !!!!!!! WARNING !!!!!!, TFPHTR is, but ARCFUN is not set up for kink
C---------------------------------------------------------------------

                  CALL TFPHTR(ALPCF,BETCF,KAPCF,GAMCF,PHI6CF,RKN6CF,
     2                        RSQTF(ILYR),+1,XINT,YINT,FINT,IER)

#if defined(CLEO_ROADHITSDIAG)
            PRINT 9916,NLYRHT,ILYR,FINT,IER
 9916       FORMAT(' WIRES_2_HITS: processing layer, sequential ',I2,
     2           '  ILYR=',I3,
     3           ' return from TFPHTR, phi=',F10.6,'  IER=',I3)
#endif 
                  IF(IER.EQ.0)THEN
                     SLYRLR(NLYRHT)=ARCFUN(KAPCF,
     2                    ALPCF,BETCF,XINT,YINT,XICF)
                     UPTODATE_LYR(NLYRHT)=.TRUE.
                  ENDIF
                   
               ENDIF
               
               COSW=XWCD(IADR)/RTF(ILYR)
               SINW=YWCD(IADR)/RTF(ILYR)
         
               IF    (SAV_HIT_TYP.EQ.USE_WIRE)THEN
                  IAMB=1
                  MULT=0.
                  NEXTAMB=0
               ELSEIF(SAV_HIT_TYP.EQ.USE_HIT )THEN
                  IAMB=-1
                  MULT=-1.
                  NEXTAMB=1
               ELSE
                  IAMB=0
               ENDIF

 21            CONTINUE
               IF(IAMB.NE.0)THEN
                  IADRHT(NHIT)=IADR*IAMB
                  IPLHIT(NHIT)=ILYR
                  OTHRHT(NHIT)=0
                  DRFHIT(NHIT)=DWCD(IADR)*MULT
                  IF(  (SAV_HIT_TYP.EQ.USE_WIRE).AND.
     2                 (ILYR.LE.MAX_LYR_OFFSET))
     3                 DRFHIT(NHIT)=DRFHIT(NHIT)+OFFSET_RES
                  STRFHT(NHIT)=SAV_HIT_TYP
                  STSZHT(NHIT)=0
                  XHIT(NHIT)=XWCD(IADR) + DRFHIT(NHIT)*SINW
                  YHIT(NHIT)=YWCD(IADR) + DRFHIT(NHIT)*COSW
                  SHIT(NHIT)=SLYRLR(NLYRHT)
                  ZHIT(NHIT)=Y0LF+TANDLF*SHIT(NHIT)
                  
                  IF((NEXTAMB.NE.0).AND.(NHIT.LT.NHITDM))THEN
                     IAMB=NEXTAMB
                     NEXTAMB=0
                     MULT=-MULT
                     NHIT=NHIT+1
                     GO TO 21
                  ENDIF

                  IF(IADRHT(NHIT).EQ.-IADRHT(NHIT-1))THEN
                     OTHRHT(NHIT)=NHIT-1
                     OTHRHT(NHIT-1)=NHIT
                  ENDIF

                  MHITLR(NLYRHT) = NHIT
                  GO TO 11
                  
               ENDIF
            ENDIF
         
C---------------------------------------------------------------------
C end of recalculating the hit structure
C---------------------------------------------------------------------
         ENDIF

         IF(SAV_HIT_TYP.EQ.USE_HIT)THEN
            CALL ROADHITS_COORD
         ENDIF
         
         UPTODATE_HIT=.TRUE.

C---------------------------------------------------------------------
C count the number of calls
C---------------------------------------------------------------------

#if defined(CLEO_DPPDIAG)
      ELSEIF(LVL_TYP.EQ.USE_ACCESS)THEN
         IF(HIT_TYP.EQ.0)THEN
            COUNT_CALL=0
         ELSEIF(HIT_TYP.EQ.1)THEN
            PRINT 1011,COUNT_CALL
 1011       FORMAT(' WIRES_2_HITS: called',I5,' times')
         ENDIF
#endif

C---------------------------------------------------------------------
C end of test on LVL_TYP
C---------------------------------------------------------------------
      ENDIF

      RETURN
      END
