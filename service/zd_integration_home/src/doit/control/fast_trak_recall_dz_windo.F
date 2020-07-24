* $Id: fast_trak_recall_dz_windo.F,v 1.4 2002/09/05 19:36:11 dpp Exp $
*
* $Log: fast_trak_recall_dz_windo.F,v $
* Revision 1.4  2002/09/05 19:36:11  dpp
*      -> changed order of logic in testing condition
*         for better diagnostics
*      -> added a  criteria for activating
*
* Revision 1.3  2001/11/19 22:52:00  dpp
*      -> use status bit encapsulation
*      -> remove filling NHITCF,PHI7CF,RKN7CF before STAK operation
*      -> use argument in call of TFXFIT
*      -> add new control parameter include file
*      -> use new argument for TFPOPF,TFPUSH
*      -> now exits with CF hits rather than LF
*      -> add second activation criteria with
*         looser hit criteria combined with large CHICF
*
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
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/statusbits.inc"
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

C STAK_LOC.............stack location index returned from TFPOPF
                       INTEGER STAK_LOC

C-----------------------------------------------------------------------
C temp values
C-----------------------------------------------------------------------

C NLDA............numb layers difference, road-fit, axial
C NLDU............numb layers difference, road-fit, +stereo
C NLDV............numb layers difference, road-fit, -stereo
C NLDAP...........numb layers difference, road-fit, axial  ,previous
C NLDUP...........numb layers difference, road-fit, +stereo,previous
C NLDVP...........numb layers difference, road-fit, -stereo,previous
C SUMRUV..........sum of number of ihts in road in UV
C SUMRAUV.........sum of number of ihts in road in AUV
C SUMRAUVP........sum of number of ihts in road in AUV,previous
C SUMNLD..........sum of numb layers difference, road-fit
C FRACDUV.........fraction of the difference,road-fit, in UV 
C DZ_FIX_ATTEMPT..iteration number, attempting to find a new fit with dz fix
C TESTDZWINDOWS...=.TRUE. if fast_trak is to be recalled  
C TESTDZWINDOWSP..=.TRUE. if it was found that 
C                            fast_trak should have been recalled  
C TFXFIT_ERR......error returned from TFXFIT

      INTEGER NLDA,NLDU,NLDV
      INTEGER NLDAP,NLDUP,NLDVP
      INTEGER SUMRUV,SUMRAUV,SUMRAUVP,SUMNLD,DZ_FIX_ATTEMPT
      REAL FRACDUV
      INTEGER TEST1,TEST2,TEST3,TESTUV
      REAL TESTFRAC
      LOGICAL TESTDZWINDOWS
      LOGICAL TESTDZWINDOWSP
      LOGICAL PASSD0,TESTTEMP
      INTEGER TFXFIT_ERR
      

      SAVE TESTDZWINDOWSP
C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

      IF(WHAT.EQ.FTR_BEFORE_TFXFIT)THEN
        NLDA=NLRDRA-NLFDRA
        NLDU=NLRDRU-NLFDRU
        NLDV=NLRDRV-NLFDRV
        SUMRUV=NLRDRU+NLRDRV         
        SUMRAUV=NLRDRA+NLRDRU+NLRDRV
        SUMNLD=NLDA+NLDU+NLDV
        FRACDUV=0.
        IF(SUMRUV.GE.1)FRACDUV=1.*(NLDU+NLDV)/SUMRUV
        
        
#if defined(CLEO_FTDIAG)
        PRINT 1003
 1003   FORMAT(
     1       ' ---------------------------------------------------'
     2       '----------------------------------------------------')
        
        PRINT 1011,NLFDRA,NLRDRA,NLFDRU,NLRDRU,NLFDRV,NLRDRV,
     2       D0CF,Y0LF,CHICF
 1011   FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(PRELIM)  ',
     2       ' layers FIT/ROAD',
     3       ': a',I3,'/',I2,
     4       ', u',I3,'/',I2,
     5       ', v',I3,'/',I2,
     6       '    D0CF=',F9.4,' Y0LF=',F9.4,
     7       ' CHICF=',F9.4)
#endif
        
        PASSD0=(ABS(D0CF).LT. 0.10)
        
#if defined(CLEO_FTDIAG)
        PRINT 1012,PASSD0
 1012   FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(PRELIM)  ',
     2       16X,9X,9X,9X,
     6       'req D0CF<   0.1000',
     7       5X,9X,6X,9X,' result=',L1)
#endif
        
        IF(PASSD0)THEN
          
          IF(CHICF.LT.25.)THEN
            TEST1= 9
            TEST2= 6
            TEST3=15
            TESTUV=8
            TESTFRAC=.5
          ELSE
            TEST1= 8
            TEST2= 5
            TEST3=12
            TESTUV=8
            TESTFRAC=.5
          ENDIF
          
          TESTTEMP=.FALSE.
          
          IF(.NOT.TESTTEMP)THEN
            TESTTEMP=(
     1           ((NLDA.GE.TEST1)).OR.
     2           ((NLDU.GE.TEST1)).OR.
     3           ((NLDV.GE.TEST1))
     4           )
            
#if defined(CLEO_FTDIAG)
            PRINT 1014,TEST1,TESTTEMP
 1014       FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(PRELIM)  ',
     2           16X,'    single type missing ',I3,
     6           9X,9X,  5X,9X,6X,9X,' result=',L1)
#endif
          ENDIF
          
          IF(.NOT.TESTTEMP)THEN
            TESTTEMP=(
     1           ((NLDA.GE.TEST2).AND.(NLDU.GE.TEST2)).OR.
     2           ((NLDU.GE.TEST2).AND.(NLDV.GE.TEST2)).OR.
     3           ((NLDV.GE.TEST2).AND.(NLDA.GE.TEST2))
     4           )
            
#if defined(CLEO_FTDIAG)
            PRINT 1015,TEST2,TESTTEMP
 1015       FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(PRELIM)  ',
     2           16X,' two types each missing ',I3,
     6           9X,9X,  5X,9X,6X,9X,' result=',L1)
#endif
          ENDIF
          
          IF(.NOT.TESTTEMP)THEN
            TESTTEMP=(
     1           ((SUMNLD.GE.TEST3))
     4           )
            
#if defined(CLEO_FTDIAG)
            PRINT 1016,TEST3,TESTTEMP
 1016       FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(PRELIM)  ',
     2           16X,'   sum of types missing ',I3,
     6           9X,9X,  5X,9X,6X,9X,' result=',L1)
#endif
          ENDIF
        
          IF(.NOT.TESTTEMP)THEN
            
            TESTTEMP=(
     1           ((SUMRUV.GE.TESTUV)).AND.
     2           ((FRACDUV.GE.TESTFRAC))
     4           )
            
#if defined(CLEO_FTDIAG)
            PRINT 1017,TESTUV,TESTFRAC,TESTTEMP
 1017       FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(PRELIM)  ',
     2           16X,'   total in U and V >/= ',I3,
     2           ' and fraction missing >/= ',F6.3,
     6           6X,9X,' result=',L1)
#endif
          ENDIF

C end of the D0 condition
        ENDIF

        TESTDZWINDOWSP=PASSD0.AND.TESTTEMP
        IF(TESTDZWINDOWSP)THEN
          CALL STATUSBITS(kSTfillcf,kstFTRneedrec,1)
        ENDIF
        EXIT_STATUS=.FALSE.
        
#if defined(CLEO_FTDIAG)
        PRINT 1003
#endif
        
C-----------------------------------------------------------------------
C 2nd entry
C-----------------------------------------------------------------------
      ELSEIF(WHAT.EQ.FTR_AFTER_TFXFIT)THEN                              
        IF(TESTDZWINDOWSP)THEN
          
          EXIT_STATUS=.FALSE.
          CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
          
          DZ_FIX_ATTEMPT=0
 421      DZ_FIX_ATTEMPT=DZ_FIX_ATTEMPT+1
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
     1         ((NLDA.GE.4)).OR.
     2         ((NLDU.GE.4)).OR.
     3         ((NLDV.GE.4))
     4         )
          IF(TESTDZWINDOWS.AND.(DZ_FIX_ATTEMPT.GE.2))THEN
            TESTDZWINDOWS=SUMRAUV.GT.SUMRAUVP
          ENDIF
          
#if defined(CLEO_FTDIAG)
          PRINT 1003
          PRINT 1031,NLFDRA,NLRDRA,NLFDRU,NLRDRU,NLFDRV,NLRDRV,
     2         SUMRAUVP,SUMRAUV,TESTDZWINDOWS
 1031     FORMAT(' FAST_TRAK_RECALL_DZ_WINDO(FINAL)   ',
     2         ' layers FIT/ROAD',
     3         ': a',I3,'/',I2,
     4         ', u',I3,'/',I2,
     5         ', v',I3,'/',I2,
     3         '    sumROAD:prev/now=',I2,',',I2,
     8         '  decision=',L1)
          PRINT 1003
#endif
          
          IF(TESTDZWINDOWS)THEN
#if defined(CLEO_FTDIAG)
            PRINT 1035,DZ_FIX_ATTEMPT
 1035       FORMAT(' FAST_TRAK_RECALL_DZ_WINDO:',
     2           'starting iteration:',I2) 

#endif
            EXIT_STATUS=.TRUE.
            
C=======================================================================
C restore the track from the stack and replace it as only element
C the representation coming back from DUFIND will be the original
C FAST_TRAK representation but with some  status flags set
C the representation coming from DUFIND is popped back 
C this this is properly preceded by a CF fit from FAST_TRAK
C because the representation had not been updated after the Z fit
C also, the new representations from FAST_TRAK end with a CF fit 
C=======================================================================
            
            CALL TFPOPF(STAK_PRIMARY,STAK_PICK_FIRST,0,STAK_LOC)
            CALL TFSTKC(STAK_PRIMARY)
            CALL TFPUSH(STAK_PRIMARY,STAK_LOC)
            
            IF(DZ_FIX_ATTEMPT.EQ.1)THEN
              CALL STATUSBITS(kSTfillcf,kstFTRrecFT1,1)
            ELSE
              CALL STATUSBITS(kSTfillcf,kstFTRrecFT2,1)
            ENDIF
            
            F_T_DZ_WINDOWS=.TRUE.
            CALL UCOPY(KAPCF, KAPCS, NPARTF )
            
#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK_INTERRUPT_VALUE('1')
            CALL XTS_TRAK_INTERRUPT_THRESH('1')
#endif
            INDX_DZ_FIX=0
 423        INDX_DZ_FIX=INDX_DZ_FIX+1
            IF(INDX_DZ_FIX.LE.N_DZ_FIX)THEN
#if defined(CLEO_FTDIAG)
              PRINT 1003
              PRINT 1034,INDX_DZ_FIX
 1034         FORMAT(' FAST_TRAK_RECALL_DZ_WINDO:',
     2             'calling FAST_TRAK,index:',I3) 

#if defined(CLEO_XTSUBS)
              CALL XTS_TRAK_INTERRUPT_VALUE('1')
              CALL XTS_TRAK_INTERRUPT_THRESH('-')
#endif
#endif
              Y0LS=0.
              TANDLS=TRY_DZ_FIX(INDX_DZ_FIX)

c            PRINT 1003
c            PRINT 1037
c 1037       FORMAT(' FAST_TRAK_RECALL_DZ_WINDO:',
c     2           ' calling SFHITS before FAST_TRAK')

c              call sfhits(SFHITS_FULL)

              CALL FAST_TRAK
              GO TO 423
            ENDIF
            
            CALL TFOSTK(STAK_PRIMARY)
            CALL TFPOPF(STAK_PRIMARY,STAK_PICK_FIRST,0,STAK_LOC)
            IF(STAK_LOC.NE.1)
     2           CALL STATUSBITS(kSTfillcf,kstFTRnewrep,1)
            CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
            
#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1036
 1036       FORMAT(' FAST_TRAK_RECALL_DZ_WINDO: calling TFXFIT') 
#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK_INTERRUPT_VALUE('3')
#endif
#endif
            CALL TFXFIT(TFXF_NORMAL_STAND,TFXFIT_ERR)
            
            IF(DZ_FIX_ATTEMPT.LT.2)THEN
              GO TO 421
            ENDIF
            
          ENDIF
C end of condition that recall was activated
        ELSE
          EXIT_STATUS=.FALSE.
        ENDIF
C end of 2nd entry
      ENDIF
C-----------------------------------------------------------------------
      RETURN
      END
C-----------------------------------------------------------------------
