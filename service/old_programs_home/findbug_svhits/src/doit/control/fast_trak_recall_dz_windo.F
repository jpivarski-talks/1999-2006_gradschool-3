* $Id: fast_trak_recall_dz_windo.F,v 1.2 2001/01/04 16:45:21 dpp Exp $
*
* $Log: fast_trak_recall_dz_windo.F,v $
* Revision 1.2  2001/01/04 16:45:21  dpp
*      -> allow triggering recall based on total number missing
*
* Revision 1.1  2000/12/19 23:46:36  dpp
*      -> NEW
*      -> reCAll FAST_TRACK for marginal tracks
*         use a series of windows in tan(dip)
*         to reduce ambiguity of overlapping tracks
*      -> set flag to control window in tan(dip) in fast_trak
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_RECALL_DZ_WINDO(WHAT,EXIT_STATUS)
C.......................................................................
C.
C. FAST_TRAK_RECALL_DZ_WINDO - loop to recall FAST_TRAK with 
C.                             a series of DZ windows
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 06-12-2000
C. LAST MOD  : 
C.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfstakcd.inc"

#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C argument
C-----------------------------------------------------------------------
C WHAT............distinguish between the call before and after TFXFIT
      INTEGER WHAT
C EXIT_STATUS.....=.TRUE. if recall done and exiting with CF hits
      LOGICAL EXIT_STATUS 
C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C temp values
C-----------------------------------------------------------------------

C NLDA............numb layers difference, road-fit, axial
C NLDU............numb layers difference, road-fit, +stereo
C NLDV............numb layers difference, road-fit, -stereo
C NLDAP...........numb layers difference, road-fit, axial  ,previous
C NLDUP...........numb layers difference, road-fit, +stereo,previous
C NLDVP...........numb layers difference, road-fit, -stereo,previous
C SUMRAUV.........sum of number of ihts in road in AUV
C SUMRAUVP........sum of number of ihts in road in AUV,previous
C SUMNLD..........sum of numb layers difference, road-fit
C DZ_FIX_ATTEMPT..iteration number, attempting to find a new fit with dz fix
C TESTDZWINDOWS...=.TRUE. if fast_trak is to be recalled  
C TESTDZWINDOWSP..=.TRUE. if it was found that 
C                            fast_trak should have been recalled  

      INTEGER NLDA,NLDU,NLDV,NLDAP,NLDUP,NLDVP
      INTEGER SUMRAUV,SUMRAUVP,SUMNLD,DZ_FIX_ATTEMPT
      LOGICAL TESTDZWINDOWS,TESTDZWINDOWSP

      SAVE TESTDZWINDOWSP
C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

      IF(WHAT.EQ.FTR_BEFORE_TFXFIT)THEN
         NLDA=NLRDRA-NLFDRA
         NLDU=NLRDRU-NLFDRU
         NLDV=NLRDRV-NLFDRV
         SUMRAUV=NLRDRA+NLRDRU+NLRDRV
         SUMNLD=NLDA+NLDU+NLDV

         TESTDZWINDOWSP=(ABS(D0CF).LT. 0.10)

         IF(TESTDZWINDOWSP)THEN
            TESTDZWINDOWSP=(
     1           ((NLDA.GE.9)).OR.
     2           ((NLDU.GE.9)).OR.
     3           ((NLDV.GE.9)).OR.
     1           ((NLDA.GE.6).AND.(NLDU.GE.6)).OR.
     2           ((NLDU.GE.6).AND.(NLDV.GE.6)).OR.
     3           ((NLDV.GE.6).AND.(NLDA.GE.6)).OR.
     1           ((SUMNLD.GE.15))
     4           )
         ENDIF

#if defined(CLEO_FTDIAG)
         PRINT 1003
         PRINT 1011,NLFDRA,NLRDRA,NLFDRU,NLRDRU,NLFDRV,NLRDRV,
     2           D0CF,TESTDZWINDOWSP
#endif

         IF(TESTDZWINDOWSP)THEN
            CALL SBYT(1,STATCF(2),23,1)
         ENDIF
         EXIT_STATUS=.FALSE.

C-----------------------------------------------------------------------
C 2nd entry
C-----------------------------------------------------------------------
      ELSEIF(WHAT.EQ.FTR_AFTER_TFXFIT)THEN
         IF(TESTDZWINDOWSP)THEN

            EXIT_STATUS=.FALSE.
            CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)

            DZ_FIX_ATTEMPT=0
 421        DZ_FIX_ATTEMPT=DZ_FIX_ATTEMPT+1
            IF(DZ_FIX_ATTEMPT.GE.2)THEN
               NLDAP=NLDA
               NLDUP=NLDU
               NLDVP=NLDV
               SUMRAUVP=SUMRAUV
            ENDIF
            NLDA=NLRDRA-NLFDRA
            NLDU=NLRDRU-NLFDRU
            NLDV=NLRDRV-NLFDRV
            SUMRAUV=NLRDRA+NLRDRU+NLRDRV
            
            TESTDZWINDOWS=(
     1           ((NLDA.GE.4)).OR.
     2           ((NLDU.GE.4)).OR.
     3           ((NLDV.GE.4))
     4           )
            IF(TESTDZWINDOWS.AND.(DZ_FIX_ATTEMPT.GE.2))THEN
               TESTDZWINDOWS=SUMRAUV.GT.SUMRAUVP
            ENDIF

#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1012,NLFDRA,NLRDRA,NLFDRU,NLRDRU,NLFDRV,NLRDRV,
     2           SUMRAUVP,SUMRAUV,TESTDZWINDOWS
#endif
            
            IF(TESTDZWINDOWS)THEN
#if defined(CLEO_FTDIAG)
               PRINT 1015,DZ_FIX_ATTEMPT
#endif
               EXIT_STATUS=.TRUE.

C restore the track from the stack and replac it as only element
               CALL TFPOPF(PRIMARY_STAK,STAK_PICK_FIRST,0)
               CALL TFSTKC(PRIMARY_STAK)
               NHITCF = NFIT
               NHITTF = NHITCF
               CHITF  = CHICF
               PHI7CF = 0
               RKN7CF = 9.0
               CALL TFPUSH(PRIMARY_STAK)
               
               CALL SBYT(1,STATCF(2),23+DZ_FIX_ATTEMPT,1)
               F_T_DZ_WINDOWS=.TRUE.
               CALL UCOPY(KAPCF, KAPCS, NPARTF )

#if defined(CLEO_XTSUBS)
               CALL XTS_TRAK_INTERRUPT_VALUE('1')
               CALL XTS_TRAK_INTERRUPT_THRESH('1')
#endif
               INDX_DZ_FIX=0
 423           INDX_DZ_FIX=INDX_DZ_FIX+1
               IF(INDX_DZ_FIX.LE.N_DZ_FIX)THEN
#if defined(CLEO_FTDIAG)
                  PRINT 1003
                  PRINT 1014,INDX_DZ_FIX
#if defined(CLEO_XTSUBS)
                  CALL XTS_TRAK_INTERRUPT_VALUE('1')
                  CALL XTS_TRAK_INTERRUPT_THRESH('-')
#endif
#endif
                  Y0LS=0.
                  TANDLS=TRY_DZ_FIX(INDX_DZ_FIX)
                  CALL FAST_TRAK
                  GO TO 423
               ENDIF

               CALL TFOSTK(PRIMARY_STAK)
               CALL TFPOPF(PRIMARY_STAK,STAK_PICK_FIRST,0)
               IF(ISTKTF(PRIMARY_STAK).NE.1)CALL SBYT(1,STATCF(2),26,1)
               CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)
                  
#if defined(CLEO_FTDIAG)
               PRINT 1003
               PRINT 1016
#if defined(CLEO_XTSUBS)
               CALL XTS_TRAK_INTERRUPT_VALUE('3')
#endif
#endif
               CALL TFXFIT

               IF(DZ_FIX_ATTEMPT.LT.2)THEN
                  GO TO 421
               ENDIF
               
            ENDIF

            IF(.NOT.EXIT_STATUS)THEN
               CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
            ENDIF

         ELSE
            EXIT_STATUS=.FALSE.
         ENDIF
      ENDIF
C-----------------------------------------------------------------------
      RETURN
C-----------------------------------------------------------------------
         
#if defined(CLEO_FTDIAG)
 1003 FORMAT(
     1' --------------------------------------------------------------')
 1011 FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(PRELIM)  ',
     2     ' layers in FIT/ROAD, auv=',3(I3,'/',I2,',   '),
     3     ' D0CF=',F9.4,'  decision=',L1)
 1012 FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(FINAL)   ',
     2     ' layers in FIT/ROAD, auv=',3(I3,'/',I2,',   '),
     3     ' sumROAD:prev/now=',I2,',',I2,'  decision=',L1)
 1014 FORMAT(' FAST_TRAK_RECALL_DZ_WINDO:calling FAST_TRAK,index:',I3) 
 1015 FORMAT(' FAST_TRAK_RECALL_DZ_WINDO:starting iteration:',I2) 
 1016 FORMAT(' FAST_TRAK_RECALL_DZ_WINDO: calling TFXFIT') 
#endif

      END













