*
* $Id: tpc_time_response.F,v 1.1 2003/03/05 17:52:15 dpp Exp $
*
* $Log: tpc_time_response.F,v $
* Revision 1.1  2003/03/05 17:52:15  dpp
*      -> NEW ROUTINE
*      -> eliminate hits that are not allowed by previous hits
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TPC_TIME_RESPONSE
C.......................................................................
C.
C. TPC_TIME_RESPONSE - merge/delete hits that are close in time
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 08-Dec-2002
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE


#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfindpar.inc"
C which includes 
C               #include "cl3seq/cdgm3/cdgeompa.inc"
C               #include "doit/duseq/tfgeompa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"
#include "doit/duseq/runev.inc"

      INTEGER ILCD,ILTF
      INTEGER IHIT,JHIT,KHIT
      REAL TIME_DIFF1,TIME_DIFF2
      REAL PED_CONTRIBUTE,PED_SUM(MHWCD)

C TIME_INHIBIT, applied inhibit in picosec
C 200 000 ps  at 50 E-9 meter /pico sec = 10 mm = 1 cm
      REAL TIME_INHIBIT
      PARAMETER (TIME_INHIBIT=200 000.)

C TIME_AMP, amplifier shaping time in picosec
C  400 ns, see Dean Karlens Nov 26, 2002 talk
      REAL TIME_AMP 
      PARAMETER (TIME_AMP=400 000.) 

C PED_FRAC, upper limit of pedistal, compared to hit pulse height
C that is allowed to observe hit
      REAL PED_FRAC
      PARAMETER (PED_FRAC=.3)

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      print 1003,EvntHit_Num

 1003 FORMAT(' TPC_TIME_RESPONSE: number of hits=',I7)

C-----------------------------------------------------------------------
C loop over hits that are on pads that have multiple hits
C-----------------------------------------------------------------------

      IF(EvntHit_Num.GT.0)THEN
        DO 290 IHIT=1,EvntHit_Num
          ILCD=EvntHit_CDlayr(IHIT)
          IF(IDVCCD(ILCD).EQ.ITPC1)THEN
            ILTF=ILCDTF(ILCD)
            IF(ILTF.GT.0)THEN
              IF(MultiHitLayer(ILTF))THEN
                IF(EvntHit_nMult(IHIT).GT.1)THEN

C-----------------------------------------------------------------------
C primary loop over all of the hits on the multihit pad
C this loop is to calculate a pedistal for each hit, JHIT
C-----------------------------------------------------------------------

                  JHIT=EvntHit_1stMlt(IHIT)
 210              IF(JHIT.GT.0)THEN
                    PED_SUM(JHIT)=0.
                    
C-----------------------------------------------------------------------
C secondary loop over the hits on the multihit pad
C calculate the contribution to the pedistal, for JHIT, from KHIT
C-----------------------------------------------------------------------
                    
                    KHIT=EvntHit_1stMlt(IHIT)
 220                IF(KHIT.GT.0)THEN
                      IF(KHIT.NE.JHIT)THEN
C-----------------------------------------------------------------------
C test if K precedes J
C test if (K plus the pulse length) precedes J
C use an exponetially decaying amp pulse
C-----------------------------------------------------------------------
                        TIME_DIFF1=
     1                       EvntHit_rawTIM(JHIT)-
     2                       EvntHit_rawTIM(KHIT)
                        IF(TIME_DIFF1.GT.0.)THEN

                          TIME_DIFF2=
     1                         EvntHit_rawTIM(JHIT)-(
     2                         EvntHit_rawTIM(KHIT)+
     3                         EvntHit_corTIM(KHIT))

                          IF(TIME_DIFF2.LE.0.)THEN
                            PED_CONTRIBUTE=EvntHit_Charge(KHIT)
                          ELSE
                            PED_CONTRIBUTE=EvntHit_Charge(KHIT)
     2                           *EXP(-TIME_DIFF2/TIME_AMP)
                          ENDIF
                          
                          PED_SUM(JHIT)=PED_SUM(JHIT)+PED_CONTRIBUTE

                        ENDIF
                      ENDIF
C-----------------------------------------------------------------------
C end of secondary loop over the hits on the multihit pad
C-----------------------------------------------------------------------
                      
                      KHIT=EvntHit_NxtMlt(KHIT)
                      GO TO 220
                    ENDIF

C-----------------------------------------------------------------------
C end of primary loop over the hits on the multihit pad
C-----------------------------------------------------------------------
                    
                    JHIT=EvntHit_NxtMlt(JHIT)
                    GO TO 210
                  ENDIF


C-----------------------------------------------------------------------
C primary loop over all of the hits on the multihit pad
C this loop is to compare the hit ph to the pedistal, for each hit, JHIT
C-----------------------------------------------------------------------

                  JHIT=EvntHit_1stMlt(IHIT)
 310              IF(JHIT.GT.0)THEN
                    
C-----------------------------------------------------------------------
C test if the pedistal for JHIT is too large; possibly kill the hit
C-----------------------------------------------------------------------

                    IF(
     1                   (PED_SUM(JHIT).GT.0.).AND.
     2                   (PED_SUM(JHIT).GT.
     3                   PED_FRAC*EvntHit_Charge(JHIT))
     4                   )THEN

                      EvntHit_Charge(JHIT)=-90.

                    ENDIF

C-----------------------------------------------------------------------
C end of primary loop over the hits on the multihit pad
C-----------------------------------------------------------------------
                    
                    JHIT=EvntHit_NxtMlt(JHIT)
                    GO TO 310
                  ENDIF
                  
C end of condition (EvntHit_nMult(IHIT).GT.1)
                 ENDIF
C end of condition (MultiHitLayer(ILTF))
              ENDIF
C end of condition (ILTF.GT.0)
            ENDIF
C end of condition (IDVCCD(ILCD).EQ.ITPC1)
          ENDIF
C end of loop over hit cells          
 290    CONTINUE
C end of condition (EvntHit_Num.GT.0)
      ENDIF
      RETURN
      END







