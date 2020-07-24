*
* $Id: dufind.F,v 1.33 2001/11/19 22:51:08 dpp Exp $
*
* $Log: dufind.F,v $
* Revision 1.33  2001/11/19 22:51:08  dpp
*      -> split off fixing of a few geometry problems
*      -> split off initializing some of the silicon data stucture
*      -> split off initializing some tf3fit control
*      -> split off initializing cdtrack counters for the event
*      -> remove diagdoit (old duet diagnostics) routines
*      -> remove direct access of the event control parameter
*      -> use argument to communicate number of chains with SFNEXT
*      -> LCFIND is now local
*      -> use argument in call of TFXFIT
*      -> use encapsulation for status bits
*      -> use encapsulation for fit variables
*      -> split off fast_trak track initialization
*      -> remove calls to CFLAST, CFCHOP, and the SFENDP switch
*      -> remove calls to CFEXTN; it could never be called because TFXFIT-LF
*         could not return an error (test is on NFIT (not NHITLF) >/=5)
*      -> delete unused include file
*      -> add new control parameter include file
*      -> remove filling NHITCF,PHI7CF,RKN7CF before STAK operation
*      -> add CF fit after final FAST_TRAK_DEL_BAD_AND_ADD for LF hits
*      -> do not use CHI3F so it can be used by FIT_WHATEVER again
*      -> no longer fill NHIT3F,CHI3F
*      -> remove call to COMPRESS_FIT: called in FAST_TRAK_DEL_BAD_AND_ADD
*      -> arguments for FAST_TRAK_CHECK_HIT_LAYRS
*     **> code change: LF fit is moved from immediately after TFXFIT-CF
*         to immediately before TFXFIT-LF. This means that TFXFIT-LF always
*         starts with a fresh LF fit; it had not if preceded by a call
*         to REPLACE_Z_HITS.  REPLACE_Z_HITS is still always preceded by an
*         LF fit because this fit is now inside that routine.
*      -> move diagnostic comments from DIAG_DUFIND back into DUFIND
*     **> set PURGE_SUBCHAIN FALSE when a track is killed after
*         the FAST_TRAK_CHECK_HIT_LAYRS call after the TFXFIT_LF
*      -> no longer set PURGE_SUBCHAIN FALSE after an error in TFXFIT_LF.
*      -> remove separate use of PURGE_SUCHAINS amd LCCDNEWT
*         now all control if through PROCESS_TRACK
*
* Revision 1.32  2001/09/12 21:57:44  dpp
*      -> add call to FAST_TRAK_DEL_BAD_AND_ADD at end
*      -> set up CF hits and fit in preparation for FAST_TRAK_DEL_BAD_AND_ADD
*      -> cut tracks in 1st pass with large CHICF and used F_T_RECALL_DZ_WINDOW
*      -> change statcf assignments for TFXFIT-LF
*
* Revision 1.31  2001/01/04 16:45:38  dpp
*      -> cut badly fit, large D0, tracks in first pass
*
* Revision 1.30  2000/12/19 23:47:38  dpp
*      -> use parameters to control TFPOPF
*      -> call FAST_TRAK_RECALL_DZ_WINDOW
*         to reCAll FAST_TRACK for marginal tracks
*      **(requires 64 bit off)
*      -> set flag to control window in tan(dip) in fast_trak
*
* Revision 1.29  2000/11/21 19:16:38  dpp
*      -> change acknowledge for dump at end, diagnostic only
*
* Revision 1.28  2000/11/15 16:51:09  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.27  2000/10/24 22:36:36  dpp
*      -> format
*      -> placement of DIAG_DUFIND call
*      -> move setting of status bit for boot_si_z_hits to that program
*      -> force correction of SVBOUNDARY_Z
*      -> default is to run REPLACE_Z_HITS; turn off with bit 64
*
* Revision 1.26  2000/10/02 20:56:22  dpp
*      -> use '2NDPASS' instead of 'SHORT' cut set of 2nd pass
*      -> new use of STAT words
*      -> calls BOOTSTRAP_SI_Z_HITS
*      -> adjust interupt values; make room/define value for BOOT_SI_Z_HITS
*      -> place track in primary stack after TFXFIT-CF
*      -> call TFXFIT-LF with and without z0 constraint for intermediate
*         extrapolated z0 range
*
* Revision 1.25  2000/08/28 17:40:06  dpp
*      -> new DIAG_DUFIND calls
*      -> new compile switch to isolate HBOOK calls
*
* Revision 1.24  2000/08/24 19:03:32  dpp
* -> new DCLEO_DIAG_DUFIND compile switch, dump w/o prompt
*
* Revision 1.23  2000/08/18 18:43:48  dpp
*      -> diagnostics
*      -> new arguments for KABG2
*      -> remove some obsolete variables
*      -> restore traditional use of CHI3F
*      -> reset D0_CONS=.FALSE. after fast_trak_check_hit_layrs
*      -> remove call to display if it will be called in SFIND
*
* Revision 1.22  2000/05/26 16:47:19  dpp
*      -> initialize the silicon available flag
*
* Revision 1.21  2000/04/26 14:30:01  dpp
*      -> remove argument from TFXFIT
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4
*
* Revision 1.20  2000/03/20 22:22:54  dpp
*      -> repeat TFXFIT (r-phi) if hits are added
*
* Revision 1.19  2000/03/01 23:00:18  dpp
*      -> compatible with argument change in STARTUP_T_D
*
* Revision 1.18  2000/02/18 21:20:15  dpp
*      -> now use compile flag DOITLOOPBACK for control of loop back and
*      -> now use NEXPT doit running level flag for:
*         DOIT vs DOITFAST
*         number of passes in DUFIND
*         SF cut set
*         chisq cut in preliminary passes
*         replace_z_hits
*
* Revision 1.17  2000/02/12 20:22:30  dpp
*      -> use NEXPT (the doit running level flag) to select Doit or DoitFast
*      -> use NEXPT to control call to REPLACE_Z_HITS
*
* Revision 1.16  2000/01/24 19:52:45  dpp
*      -> removed diagnostic code to change DWCD
*      -> use compile switch CLEO3STARTUP to control number of passes,
*         the SF cut set, and the cut on bad fits in preliminary passes
*      -> add control to loop back for another pass through FAST_TRAK
*         controlled by the compile switch CLEO3STARTUP
*      -> add call to REPLACE_Z_HITS, but commented out
*      -> do not use CHI3F so it can be used by FIT_WHATEVER 
*         for time-like residual
*
* Revision 1.15  2000/01/06 22:17:32  dpp
* delete tracks with bad fits in first SF pass
* clean up the control for DOFAST and IF(NFIT.GT.0)
* allow 3 passes
*  ignore tracks missing Z hits in preliminary passes
*
* Revision 1.14  1999/11/22 21:00:41  dpp
* in DoitFast: now skips to CDNEWT after FAST_TRAK
*
* Revision 1.13  1999/11/17 21:43:59  dpp
* changes to calls to DIAG_DUFIND
*
* Revision 1.12  1999/11/09 21:42:00  dpp
* add temporary assignment of cathode geometry,
* without compile switches so it won't crash in LFCTHT
*
* Revision 1.11  1999/11/03 22:40:03  dpp
* add compile switches for fast version
* call the cathode clustering (in diag only)
*
* Revision 1.10  1999/10/12 15:16:41  dpp
* change in reset of XTS_ABORT for "NEXT PASS"
*
* Revision 1.9  1999/09/12 22:40:12  dpp
* remove arguments from call to SZ_FITTER; set Z_BIAS; 
*   fix bug by calling TFSTKC(STAK_SECONDARY) before TFXFIT;
*   remove chisq argument from call to TFXFIT
*
* Revision 1.8  1999/08/03 21:50:17  dpp
* new calls to stack routines
*
* Revision 1.7  1999/05/28 03:04:43  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.6  1999/04/27 20:09:37  dpp
* remove CDSFSD; and aethetics
*
* Revision 1.5  1999/01/29 15:01:50  dpp
* switch to "CLEAN" SF cut set
*
* Revision 1.4  1999/01/11 03:43:53  lyon
* Remove annyoing print
*
* Revision 1.3  1998/11/23 19:30:46  lyon
* Now is called from C++
*
* Revision 1.2  1998/02/24 18:33:49  dpp
* eliminated obsolete external
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :
      SUBROUTINE DUFIND
C.......................................................................
C.
C. DUFIND - Driving subroutine for trackfinding
C.
C.
C. COMMON    : none
C. CALLS     :
C. CALLED    :
C. AUTHOR    :
C.
C. VERSION   : 1.0
C. CREATED   : March 1982
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/statusbits.inc"

C LFIRST_GRAPHICS.logical, =.true. if track graphics need to be initialized
                   LOGICAL LFIRST_GRAPHICS 
                   SAVE LFIRST_GRAPHICS
                   DATA LFIRST_GRAPHICS/.TRUE./
C L_DOIT_FULL.....logical, =.true. if DOIT_FULL event control bit is set
                   LOGICAL L_DOIT_FULL
C L_SF_STARTUP....logical, =.true. if SF_STARTUP event control bit is set
                   LOGICAL L_SF_STARTUP
C L_DOIT_NOCHI....logical, =.true. if DOIT_NOCHI event control bit is set
                   LOGICAL L_DOIT_NOCHI
C L_DOIT_NEW......logical, =.true. if DOIT_NEW event control bit is set
                   LOGICAL L_DOIT_NEW
C NUM_SF_LAYERS...number of layers in SF geometry
                   INTEGER NUM_SF_LAYERS
C NCHAIN..........number of SF chains
                   INTEGER NCHAIN
C SFNEXT_CONTROL..output number of chains found,
C                 input number of chains to flag used
                   INTEGER SFNEXT_CONTROL
C LMERGED.........logical output indicating that chains are merged
                   LOGICAL LMERGED 
C NFIT............number of hits on track
                   INTEGER NFIT
C CHISQ...........chi square of fit, usually, CHICF
                   REAL CHISQ
C DCA.............distance of closest approach, D0CF
                   REAL DCA
C ZINT............Z intercept
                   REAL ZINT
C NHITLF..........number of LF hits on track, and used in the fit
                   INTEGER NHITLF
C N_HIT3F.........number of hits in last fit done
                   INTEGER N_HIT3F

C PROCESS_TRACK...logical, =.TRUE. if track is
C                 good enough to continue processing
                  LOGICAL PROCESS_TRACK
C TFXFIT_ERR......err returned from TFXFIT
                   INTEGER TFXFIT_ERR
C LUTR3FIT........logical, if =.TRUE. use TF3FIT
                   LOGICAL LUTF3FIT
C SFPASS..........SF pass numper
                   INTEGER SFPASS
C MSFPASS.........Maximum SF pass numper
                   INTEGER MSFPASS
C LDOPASS.........logical, if =.TRUE. do pass
                   LOGICAL LDOPASS
C TRY_Z0_CONSTR...try Z0 constraint in TFXFIT_Z
                   LOGICAL TRY_Z0_CONSTR
C Z0BIAS_SAV......local temp value of Z0BIAS, used to restore old value
                   REAL Z0BIAS_SAV
C Z0BIAS_NEW......local value of Z0BIAS
                   REAL Z0BIAS_NEW
C PASS_Z_CRIT.....logical, =.TRUE. if Z criteria is passed 
C                 argument from FAST_TRAK_CHECK_HIT_LAYRS
                   LOGICAL PASS_Z_CRIT 
C FIRST_NEXT......argument to STAK routines
                   INTEGER FIRST_NEXT
C STAK_LOC........stack location index returned from TFPOPF
                   INTEGER STAK_LOC

C STAK_LOC_SAV....save the stack location of input representation
C                 this is the start of the generated  representations
                   INTEGER STAK_LOC_SAV


      INTEGER ITRK,ILYR
      LOGICAL LFOUND,LDIAG1,LDIAG2
      REAL RMSRET,RMS0,RMSLAS,DIRRMS,T0BOT
      REAL TFSAV,TDSAV,TCSAV
      REAL CHIRET
 
C NFIT_HOLD.......hold the value of NFIT to compare to TFXFIT output
                   INTEGER NFIT_HOLD
C N_CALL_TFXFIT...number of calls of TFXFIT in a loop
                   INTEGER N_CALL_TFXFIT
C LCTFXFIT........logical, if =.TRUE. process the TFXFIT sequence
                   LOGICAL LCTFXFIT
C DIDRECALL.......=.TRUE. if FAST_TRAK_RECALL_DZ_WINDO
C                 did the recall and exits with CF hits
                   LOGICAL DIDRECALL
C REPLACED........REPLACE_Z_HITS or BOOT_SI_Z_HITS replaced hits 
                   LOGICAL REPLACED

C XTS_INPUTVAL....function call to return input from XTS_RFVIEW
                   INTEGER XTS_INPUTVAL
C XTS_ABORTVAL....function call to return abort signal from XTS_RFVIEW
C                 argument of 0 zeros the abort signal, =1 return current 
                   INTEGER XTS_ABORTVAL
C NTRKCD...........number of distinct tracks in CDTRACK
                   INTEGER NTRKCD



C      EXTERNAL CFXYHT,LFXYHT

C== Dummy variables for KABG2
      REAL    KADUM, ALDUM, BEDUM, GADUM, XIDUM
      INTEGER IDUM1,IDUM2


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

C=======================================================================
C fix some problems in the CLEO3 geometry
C=======================================================================

      CALL INIT_GEOM_FIX

C=======================================================================
C call TFXFIN so that 
C the data base, or default, efficiencies can be overwritten
C NOT NORMAL !!!!!!!!!!!!!!!!!!!!!!!!
C=======================================================================

C      CALL TFXFIN

C=======================================================================
C initialize the Doit/DoitFast dependent variables for FAST_TRAK
C=======================================================================
      
      CALL FAST_TRAK_INITIALIZE_EVNT

C=======================================================================
C initialize the silicon available array
C=======================================================================

      CALL SVD_INITIALIZE_EVENT

C=======================================================================
C initialize TF3FIT control
C=======================================================================

      CALL TF3FIT_INITIALIZE_EVENT

C=======================================================================
C extract event control bits
C extract SF status
C=======================================================================
* EXB_DOIT_FULL -  NEXPT bit value to designate full production DOIT  
* 32, part of 38 

* EXB_SF_STARTUP - NEXPT bit value to designate using SF "STARTUP" cut set
* 4, part of 38

* EXB_DOIT_NOCHI - NEXPT bit value to designate NOT using DUFIND CHICF cut
* 8, not part of 38

* EXB_DOIT_NEW   - NEXPT bit value to designate NOT running some new stuff
* 64, not part of 38

      CALL EVENT_CONTROL_BITS(EXB_DOIT_FULL,L_DOIT_FULL)
      CALL EVENT_CONTROL_BITS(EXB_SF_STARTUP,L_SF_STARTUP)
      CALL EVENT_CONTROL_BITS(EXB_DOIT_NOCHI,L_DOIT_NOCHI)
      CALL EVENT_CONTROL_BITS(EXB_DOIT_NEW,L_DOIT_NEW)

      CALL SFACCESS(SFACCESS_NLAYSF,0,NUM_SF_LAYERS)

C=======================================================================
C initialize local control variables
C=======================================================================

      LUTF3FIT = .FALSE.
      
      IF(L_DOIT_FULL)THEN
        MSFPASS=3
      ELSE
        MSFPASS=1
      ENDIF

C=======================================================================
C initialize the kink control
C=======================================================================

      CALL KABG2(KABG2_CLEAR,
     1     0,0,0,0, 
     2     0.,0.,0.,0.,0., 0., 
     3     KADUM, ALDUM, BEDUM, GADUM, XIDUM)


C=======================================================================
C initialize to run without looking at seed tracks
C=======================================================================

      CALL CDTRACK_INITIALIZE_EVENT
      
#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1011,L_DOIT_FULL,L_SF_STARTUP,
     2                                   L_DOIT_NOCHI,L_DOIT_NEW
 1011                         FORMAT(' DUFIND: ENTER:',
     2                             ' full=',L1,' start=',L1,
     3                             ' nochi=',L1,' new=',L1)
#                             endif


C=======================================================================
C initialize graphics for higher level tracks
C=======================================================================

#                             if defined(CLEO_DIAG_DUFIND)
                              IF(LFIRST_GRAPHICS)THEN
                                 CALL INIT_DUFIND_GRAPHICS
                                 LFIRST_GRAPHICS=.FALSE.
                              ENDIF
#                             endif
                                   
C=======================================================================
C loop over the SF passes
C=======================================================================

      SFPASS=0
 211  SFPASS=SFPASS+1
      IF(SFPASS.GT.MSFPASS)THEN
        LDOPASS=.FALSE.
      ELSE
        IF(SFPASS.EQ.1)THEN
          IF(L_DOIT_FULL)THEN
            IF(L_SF_STARTUP)THEN
              CALL SFCUTS('DUFIND', 'STARTUP')
            ELSE 
              CALL SFCUTS('DUFIND', 'CLEAN')
            ENDIF
          ELSE
            CALL SFCUTS('DUFIND', 'STARTUP')
          ENDIF
          LDOPASS=(NUM_SF_LAYERS.GT.1)
        ELSEIF(SFPASS.EQ.2)THEN
          CALL SFCUTS('DUFIND', '2NDPASS')
          LDOPASS=(NUM_SF_LAYERS.GT.1)
        ELSEIF(SFPASS.GE.3)THEN
          CALL SFCUTS('DUFIND', 'SHORT')
          LDOPASS=(NUM_SF_LAYERS.GT.1)
        ENDIF
      ENDIF
      IF(LDOPASS)THEN
        
C=======================================================================
C do the SF pattern recognition
C=======================================================================

#                             if defined(CLEO_XTSUBS)
                              IDUM1=XTS_ABORTVAL(0)
#                             endif
#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1031
 1031                         FORMAT(' DUFIND: will call SFIND')
#                             endif
        CALL SFIND
#                             if defined(CLEO_XTSUBS)
                              IF(XTS_ABORTVAL(1).EQ.1)GO TO 999
#                             endif

#                             if defined(CLEO_DIAG_DUFIND)
                              CALL SFACCESS(SFACCESS_NLAYSF,0,NCHAIN)
                              PRINT 1041,NCHAIN
 1041                         FORMAT(' DUFIND: back from SFIND:',
     2                             ' NCHAIN=',I4)
                              CALL AFTER_SFIND(LDIAG1)
                              IF(.NOT.LDIAG1)GO TO 999
#                             endif
                                   
        SFNEXT_CONTROL=0

C return point for next SF track
 311    CONTINUE
        CALL SFNEXT(SFNEXT_CONTROL,LMERGED)
        IF(SFNEXT_CONTROL.GE.1)THEN
          CALL STATUSBITS(kSTfillcs,kstSFseedind,1)
          CALL STATUSBITS(kSTfillcs,kstSFpassnum,SFPASS)

C return point for testing T0 values
 312      CONTINUE
            
C clear the stack
          CALL TFSTKC(0)

C transfer data from SF representation
C into TFHITSCD wire array HTWADR
#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1061
 1061                         FORMAT(' DUFIND: will call SFHITS'
     2                             '  and reset XTS_TRAK display')
#                             if defined(CLEO_XTSUBS)
                              IF(XTS_ABORTVAL(1).EQ.0)
     2                             CALL XTS_TRAK_INTERRUPT_THRESH('0')
                              CALL XTS_TRAK_INTERRUPT_VALUE('0')
#                             endif
#                             endif
#if defined(CLEO_DOITLOOPBACK)
#if (defined(CLEO_DIAG_DUFIND))&&(defined(CLEO_XTSUBS))
C                             CALL STARTUP_T_D(3,0)
                              CALL STARTUP_T_D(2,0)
#endif
#endif
          CALL SFHITS

C=======================================================================
C try to fit hits in pattern recognition
C to track parameters 
C=======================================================================

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1071
 1071                         FORMAT(' DUFIND: will call FAST_TRAK',
     3                             ' (and set xts_trak INTERRUPT)')
#                             if defined(CLEO_XTSUBS)
                              CALL XTS_TRAK_INTERRUPT_VALUE('1')
#endif
C                             this is for zooming
                              CALL CONTROL_ACCESS(
     2                             CONTROL_ACCESS_SET_TFXS,0,1)
C                             this clears the counter in WIRES_2_HITS
                              CALL WIRES_2_HITS(LVLTYP_ACCESS,0,0)
#                             endif

          CALL FAST_TRAK_INITIALIZE_TRAK
          CALL FAST_TRAK
          CALL FIT_ACCESS(FIT_ACCESS_NFIT,0,NFIT)

#                             if defined(CLEO_DIAG_DUFIND)
C                             this prints the counter in WIRES_2_HITS
                              CALL WIRES_2_HITS(LVLTYP_ACCESS,1,0)
                              PRINT 1081,NFIT
 1081                         FORMAT(' DUFIND: return from FAST_TRAK,',
     2                             ' NFIT=',I4)
#                             endif
#                             if defined(CLEO_XTSUBS)
                              IF(XTS_ABORTVAL(1).EQ.1)GO TO 999
#                             endif

C=======================================================================
C  after FAST_TRACK, test NFIT
C                  NFIT may be zero when a track is not found
C  IF(PROCESS_TRACK)"   ( #0 )
C=======================================================================

          PROCESS_TRACK=(NFIT.GT.0)
          IF(PROCESS_TRACK)THEN
            CALL CDTRACK_ACCESS(CDT_INCR_SEED,IDUM1,IDUM2)
          ENDIF
          
C=======================================================================
C this is for full DOIT;   NOT DoitFast
C  IF(PROCESS_TRACK)"   ( #1 )
C=======================================================================

          IF(L_DOIT_FULL)THEN
            IF(PROCESS_TRACK)THEN

C order the stack and restore the best track from the stack
              CALL TFOSTK(STAK_PRIMARY)
              CALL TFPOPF(STAK_PRIMARY,STAK_PICK_FIRST,0,STAK_LOC)

C cut tracks with bad chi square
              IF(.NOT.L_DOIT_NOCHI)THEN
                CALL FIT_ACCESS(FIT_ACCESS_CHICF,0,CHISQ)
                CALL FIT_ACCESS(FIT_ACCESS_D0CF,0,DCA)

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1091,CHISQ,DCA,NFIT
 1091                         FORMAT(' DUFIND: possibly cut on',
     1                             ' chisq=',F8.3,' dca=',F9.6,' ....',
     2                             ' NFIT=',I4)
#                             endif

                IF(
     1               (SFPASS .LT.MSFPASS).AND.
     2               (CHISQ  .GT. 50.   ) )PROCESS_TRACK=.FALSE.
                IF( 
     1               (SFPASS .EQ.1      ).AND.
     2               (CHISQ  .GT. 20.   ).AND.
     3               (ABS(DCA).GT. 0.1  ) )PROCESS_TRACK=.FALSE.
              ENDIF

C end of "IF(PROCESS_TRACK)"   ( #1 )
            ENDIF

C=======================================================================
C  after tests of the track, test if processing should be continued
C  IF(PROCESS_TRACK)"   ( #2 )
C=======================================================================

            IF(PROCESS_TRACK)THEN

C=======================================================================
C decide if TFXFIT CF should be called
C     use 2nd pass through TFXFIT if there are hits added
C=======================================================================
              N_CALL_TFXFIT=0                 
 401          N_CALL_TFXFIT=N_CALL_TFXFIT+1
              CALL FIT_ACCESS(FIT_ACCESS_NFIT,0,NFIT)
              
              IF(N_CALL_TFXFIT.EQ.1)THEN
                LCTFXFIT=.TRUE.
              ELSEIF(
     1               (N_CALL_TFXFIT.LE.2).AND.
     2               (NFIT.GT.NFIT_HOLD))THEN
                LCTFXFIT=.TRUE.
              ELSE
                LCTFXFIT=.FALSE.
              ENDIF
              
              IF(LCTFXFIT)THEN
                NFIT_HOLD=NFIT

C=======================================================================
C  fill in r-phi hits with TFXFIT
C=======================================================================

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1101,N_CALL_TFXFIT,NFIT
 1101                         FORMAT(' DUFIND: will call ROADHITS',
     2                             ' for CF before TFXFIT(',I2,')',
     3                             ', NFIT=',I4)
#                             endif

                CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1111
 1111                         FORMAT(' DUFIND: after call to ROADHITS')
#                             endif

C=======================================================================
C test if the newly defined road indicates there exists
C a much better track solution within the road
C=======================================================================

                IF(N_CALL_TFXFIT.EQ.1)THEN

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1121
 1121                         FORMAT(' DUFIND: will call',
     2                             ' FAST_TRAK_RECALL_DZ_WINDO',
     3                             ' to test before TFXFIT')
#                             endif

                  CALL FAST_TRAK_RECALL_DZ_WINDO
     2                             (FTR_BEFORE_TFXFIT,DIDRECALL)
                ENDIF

C=======================================================================
C update the representation in the primary stack with all the status bits
C this saves the initial FAST_TRAK output to compare to FAST_TRAK output
C from FAST_TRAK_RECALL_DZ_WINDO
C
C   there will be multiple entries because FAST_TRAK can be recalled
C   entries will be ordered and popped in FAST_TRAK_RECALL_DZ_WINDO 
C   this is properly preceded by a CF fit from FAST_TRAK
C 
C=======================================================================

                CALL TFUSTK(STAK_PRIMARY,0)

C=======================================================================
C fill in r-phi hits with TFXFIT, finally
C=======================================================================

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1131,NFIT
 1131                         FORMAT(' DUFIND: will call TFXFIT for CF',
     3                             '(set xts_trak INTERRUPT)',
     3                             ', NFIT=',I4)
#                             if defined(CLEO_XTSUBS)
                              CALL XTS_TRAK_INTERRUPT_VALUE('3')
#                             endif
#                             endif
                IF(N_CALL_TFXFIT.EQ.1)THEN
                  CALL STATUSBITS(kSTfillcf,kstDUtfxfitCF1,1)
                ELSE
                  CALL STATUSBITS(kSTfillcf,kstDUtfxfitCF2,1)
                ENDIF
                
                CALL TFXFIT(TFXF_NORMAL_STAND,TFXFIT_ERR)

C=======================================================================
C DIAGNOSTIC FOR ITERATING DRIFT FUNCTION
C=======================================================================

#if defined (CLEO_DOITLOOPBACK)
#if (defined(CLEO_DIAG_DUFIND))&&(defined(CLEO_XTSUBS))
                     PRINT 6603
 6603                FORMAT(' DUFIND: ,LOOP BACK FOR NEW T0?')
                     IF(XTS_INPUTVAL(IDUM1).EQ.'Y')THEN
                        GO TO 312
                     ENDIF
#endif
#endif

C=======================================================================
C hits and fit should be CF when entering here
C=======================================================================
C test if there is a better z way off somewhere else
C REPLACE_Z_HITS starts with ROADHITS(LF) and fit
C REPLACE_Z_HITS will end with ROADHITS(CF) and fit
C REPLACE_Z_HITS uses the secondary stak
C=======================================================================

                CALL FIT_ACCESS(FIT_ACCESS_NHITLF,0,NHITLF)

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1171,NHITLF
 1171                         FORMAT(' DUFIND: will call',
     2                             ' REPLACE_Z_HITS',
     3                             ' if NHITLF=',I3,' < 8')
#                             endif

                IF(NHITLF.LT.8)THEN
                  CALL REPLACE_Z_HITS(REPLACED)
                  IF(REPLACED)THEN
                    IF(N_CALL_TFXFIT.EQ.1)THEN
                      CALL STATUSBITS(kSTfillcf,kstDUrepZhaCF1,1)
                    ELSE
                      CALL STATUSBITS(kSTfillcf,kstDUrepZhaCF2,1)
                    ENDIF
                  ENDIF
                ENDIF

C=======================================================================
C hits and fit should be CF when entering here
C=======================================================================
C test if the newly defined road indicates there exists
C a much better track solution within the road
C if so, re-run FAST_TRAK for multiple well defined z projections
C FAST_TRAK_RECALL_DZ_WINDO will end with ROADHITS(CF)
C=======================================================================

                IF(
     1               (N_CALL_TFXFIT.EQ.1).AND.
     2               (.NOT.L_DOIT_NEW))THEN
                  
#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1181
 1181                         FORMAT(' DUFIND: will call',
     2                             ' FAST_TRAK_RECALL_DZ_WINDO',
     3                             ' after TFXFIT',
     4                             ' (may set xts_trak INTERRUPT',
     5                             ' inside _RECALL_DZ_WINDO)')
#                             endif

                  CALL FAST_TRAK_RECALL_DZ_WINDO
     2                             (FTR_AFTER_TFXFIT,DIDRECALL)
                ENDIF


C=======================================================================
C this is the new placement of the z fit
C do a Z fit before the next iteration
C refresh the z parameters with ROADHITS
C refit in Z and get the number of Z hits with SZ_FITTER
C=======================================================================

                CALL FIT_ACCESS(FIT_ACCESS_NFIT,0,NFIT)

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1161,NFIT
 1161                         FORMAT(' DUFIND: will call ROADHITS',
     2                             ' for LF fit, NFIT=',I4)
#                                  endif
                CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_LF)
                CALL FIT_WHATEVER

C=======================================================================
C loop back; process TFXFIT (r-phi) again if number of hits has increased
C=======================================================================

                GO TO 401
              ENDIF

C=======================================================================
C hits and fit should be LF when entering here
C=======================================================================
C if enough hits, use FAST_TRAK_CHECK_HIT_LAYRS to remove Z0 constraint
C do not increase the bias; do not reduce the value of Z0BIAS 
C do not allow D0 constraint to be set by FAST_TRAK_CHECK_HIT_LAYRS
C=======================================================================

              CALL FIT_ACCESS(FIT_ACCESS_Z0BIAS,0,Z0BIAS_SAV)
              CALL FAST_TRAK_CHECK_HIT_LAYRS(
     1             FT_NO_D0CON,FT_NO_MORE_Z0BIAS,PASS_Z_CRIT)
              CALL FIT_ACCESS(FIT_ACCESS_Z0BIAS,0,Z0BIAS_NEW)
              IF(Z0BIAS_NEW.GT.Z0BIAS_SAV)THEN
                CALL STATUSBITS(kSTfillcf,kstDUrZ0biasaCF,1)
              ENDIF

C=======================================================================
C fill in S-Z hits with TFXFIT
C
C   this section stores multiple entries in the primary stak
C   with differing Z0 constraints
C   both entries are popped below so ordering does not matter
C
C   the last fit may not be a proper LF fit
C   DUFIND after TFXFIT and DUFIND after FAST_TRAK_RECALL_DZ_WINDOS
C   end with LF fit, but not REPLACE_Z_HITS 
C
C   this section also stores multiple entries in the secondary stak
C   these are the results after TFXFIT. why TFUSTK is not used to
C   update the results in the primary stak is unknown
C   The results are then ordered and the first one popped
C   this is properly preceded by a LF fit from FAST_TRAK
C
C=======================================================================

              CALL FIT_ACCESS(FIT_ACCESS_NFIT,0,NFIT)

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1211,NFIT
 1211                         FORMAT(' DUFIND: clear primary stak',
     3                             ' and push in r-phi result',
     3                             ', NFIT=',I4)
#                             endif

              CALL TFSTKC(0)
              CALL STATUSBITS(kSTfillcf,kstDUtfxfitLFnt,1)
              CALL TFPUSH(STAK_PRIMARY,STAK_LOC)
            
              CALL FIT_ACCESS(FIT_ACCESS_Y0LF,0,ZINT)
              CALL FIT_ACCESS(FIT_ACCESS_NHITLF,0,NHITLF)
              TRY_Z0_CONSTR=
     1             (ABS(ZINT).GT.0.01).AND.
     2             (ABS(ZINT).LT.0.05).AND.
     3             (NHITLF.GT.10)
              IF(TRY_Z0_CONSTR)THEN
              
#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1221
 1221                         FORMAT(' DUFIND:',
     2                             ' REFIT in Z with Z CONSTRAINT',
     3                             ' and push that into the stak')
#                             endif

                CALL FIT_ACCESS(FIT_ACCESS_Z0BIAS,0,Z0BIAS_SAV)
                CALL FIT_ACCESS(FIT_ACCESS_Z0BIAS_SET,0,0.)
                CALL FIT_WHATEVER
                CALL FIT_ACCESS(FIT_ACCESS_Z0BIAS_SET,0,Z0BIAS_SAV)
                CALL STATUSBITS(kSTfillcf,kstDUtfxfitLFzt,1)
                CALL TFPUSH(STAK_PRIMARY,STAK_LOC)
              ENDIF

              FIRST_NEXT=STAK_PICK_FIRST
 502          CALL TFPOPF(STAK_PRIMARY,FIRST_NEXT,0,STAK_LOC)
              IF(STAK_LOC.GT.0) THEN

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1231,STAK_LOC
 1231                         FORMAT(' DUFIND: will call TFXFIT for LF',
     2                             ' (rep',I3,')',
     2                             ' (set xts_trak INTERRUPT)')
#                             if defined(CLEO_XTSUBS)
                              CALL XTS_TRAK_INTERRUPT_VALUE('4')
#                             endif
#                             endif

                CALL TFXFIT(TFXF_NORMAL_STAND,TFXFIT_ERR)
                CALL TFPUSH(STAK_SECONDARY,STAK_LOC)

#                             if defined(CLEO_XTSUBS)
                              IF(XTS_ABORTVAL(1).EQ.1)GO TO 999
#                             endif

                FIRST_NEXT=STAK_PICK_NEXT
                GO TO 502
              ENDIF
              CALL TFOSTK(STAK_SECONDARY)
              CALL TFPOPF(STAK_SECONDARY,STAK_PICK_FIRST,0,STAK_LOC)
              IF(STAK_LOC.NE.1)
     2             CALL STATUSBITS(kSTfillcf,kstDUtfxfitLFzu,1)

C=======================================================================
C hits and fit should be LF when entering here
C=======================================================================
C when there are no S-Z hits already, fill in S-Z hits with BOOT_SI_Z_HITS
C note: boot_si_z_hits will delete the stereo hits. Yet, it is unclear
C       what is the criteria for the number of remaining CF hits.
C       FAST_TRAK demands .ge.(NHIT_MIN_TRAK=7) hits and the code below
C       demands .lt.3  LF hits so the minimum number of 
C       remaining CF hits is 5.
C=======================================================================

              CALL FIT_ACCESS(FIT_ACCESS_NHITLF,0,NHITLF)

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1271,NHITLF
 1271                         FORMAT(' DUFIND: will call',
     2                             ' CALL BOOT_SI_Z_HITS',
     3                             ' if NHITLF=',I3,' < 3')
#                             endif

              IF(NHITLF.LT.3)THEN

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1281
 1281                         FORMAT(' DUFIND: will call ',
     2                             'BOOT_SI_Z_HITS',
     2                             '(set xts_trak INTERRUPT)'
     4                             '(set xts_trak INTERRUPT',
     5                             ' again at end)')
#                             if defined(CLEO_XTSUBS)
                              CALL XTS_TRAK_INTERRUPT_VALUE('5')
#                             endif
#                             endif

                CALL BOOT_SI_Z_HITS(REPLACED)
              ENDIF

C=======================================================================
C cut tracks with few Z hits
C do not allow D0 constraint to be set by FAST_TRAK_CHECK_HIT_LAYRS
C do not allow Z0BIAS constraint to be set by FAST_TRAK_CHECK_HIT_LAYRS
C cut tracks that used FAST_TRAK_RECALL_DZ_WINDO and still have bad fit
C note: 11/15/2001, a separate logical,PURGE_SUBCHAIN, had been used to
C indicate when the subchain should be purged from further use. In the past,
C this logical was not changed to FALSE when PROCESS_TRACK was set false here.
C This caused the subchains to nopt be tested for a killed track.
C Testing 11/15/2001 showed that subchains should be tested. 
C=======================================================================

              IF(SFPASS .LT.MSFPASS)THEN
                CALL FAST_TRAK_CHECK_HIT_LAYRS(
     1               FT_NO_D0CON,FT_SET_Z0BIAS_USE,PASS_Z_CRIT)
                IF(.NOT.PASS_Z_CRIT)PROCESS_TRACK=.FALSE.
                
                CALL FIT_ACCESS(FIT_ACCESS_CHICF,0,CHISQ)
                IF( 
     1               (SFPASS .EQ.1      ).AND.
     2               (CHISQ  .GT. 20.   ).AND.
     3               (DIDRECALL         ) )PROCESS_TRACK=.FALSE.
              ENDIF

C=======================================================================
C end of "IF(PROCESS_TRACK)"   ( #2 )
C=======================================================================

            ENDIF

C=======================================================================
C  after TFXFIT, test PROCESS_TRACK again
C                  PROCESS_TRACK may be .FALSE. for badly fit tracks
C  call the 3D fitter or
C  use FAST_TRAK_DEL_BAD_AND_ADD to replace bad hits
C  "IF(PROCESS_TRACK)"   ( #3 )
C=======================================================================

            IF(PROCESS_TRACK) THEN

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1331
 1331                         FORMAT(' DUFIND: order the STAK',
     2                             ' and call TFPOPF',
     3                             ' in preparation for TF3FIT')
                              CALL XTS_TRAK('CF',0,'DUFIND, temp')
#                                  endif

C order the stack
              CALL TFOSTK(STAK_SECONDARY)
              CALL TFPOPF(STAK_SECONDARY,STAK_PICK_FIRST,0,STAK_LOC)
C 3D track fit

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1351
 1351                         FORMAT(' DUFIND:',
     2                             ' will call TF3FIT',
     3                             ' (or F_T_DEL_BAD_AND_ADD)',
     4                             ' (set xts_trak INTERRUPT)')
#                             if defined(CLEO_XTSUBS)
                              CALL XTS_TRAK_INTERRUPT_VALUE('7')
#                             endif
                              CALL XTS_TRAK('CF',0,'DUFIND, temp')
#                             endif

              IF(LUTF3FIT)THEN
                CALL TF3FIT    
              ELSE
                CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
                CALL FIT_WHATEVER
                CALL FIT_ACCESS(FIT_ACCESS_NFIT,0,NFIT)

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1361,NFIT
 1361                         FORMAT(' DUFIND: will call',
     2                             ' FAST_TRAK_DEL_BAD_AND_ADD',
     3                             ' after CF hits and fit',
     4                             ', NFIT=',I4)
                              CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')
#                             endif

                CALL FAST_TRAK_DEL_BAD_AND_ADD
     2                             (FT_NO_DELETE,FT_USE_SIGMAS)

                CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_LF)
                CALL FIT_WHATEVER
                CALL FIT_ACCESS(FIT_ACCESS_NFIT,0,NFIT)

#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1371,NFIT
 1371                         FORMAT(' DUFIND: will call',
     2                             ' FAST_TRAK_DEL_BAD_AND_ADD',
     3                             ' after LF hits and fit',
     4                             ', NFIT=',I4)
                              CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')
#                             endif

                CALL FAST_TRAK_DEL_BAD_AND_ADD
     2                    (FT_NO_DELETE,FT_USE_SIGMAS)

                CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
                CALL FIT_WHATEVER
                CALL FIT_ACCESS(FIT_ACCESS_NFIT,0,NFIT)
              
              ENDIF
            
#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1381,NFIT
 1381                         FORMAT(' DUFIND: will call',
     2                             ' TFUSTK and TFPOPF',
     3                             ' after TF3FIT',
     4                             ' (or F_T_DEL_BAD_AND_ADD)',
     5                             ', NFIT=',I4)
                              CALL XTS_TRAK('CF',0,'DIAG_DUFIND, temp')
#                             endif

C=======================================================================
C update the stack after the fit
C   this is properly preceded by a CF fit
C Pop first TFIND data off the stack
C=======================================================================

              CALL FIT_ACCESS(FIT_ACCESS_NHITLF,0,N_HIT3F)
              IF (N_HIT3F .GT. 0) THEN
                
                CALL TFUSTK(STAK_SECONDARY,0)
                CALL TFPOPF(STAK_SECONDARY,STAK_PICK_FIRST,0,STAK_LOC)
                IF (STAK_LOC.LE.0) THEN
                  PROCESS_TRACK=.FALSE.
                ENDIF
              ENDIF

C=======================================================================
C end of "IF(PROCESS_TRACK)"   ( #3 )C Pop first TFIND data off the stack
C=======================================================================

            ENDIF

C=======================================================================
C that was the end of the full DOIT condition
C=======================================================================
C=======================================================================
C this is for DoitFast
C=======================================================================

          ELSE
            
          ENDIF

C=======================================================================
C store the track
C=======================================================================

          IF(PROCESS_TRACK)THEN
            CALL FIT_ACCESS(FIT_ACCESS_NFIT,0,NFIT)
  
#                             if defined(CLEO_DIAG_DUFIND)
                              PRINT 1401,NFIT
 1401                         FORMAT(' DUFIND: will call CDNEWT',
     3                             ' (set xts_trak INTERRUPT)',
     3                             ', NFIT=',I4)
#if                           defined(CLEO_XTSUBS)
C                             refresh the CF hits for the display
                              CALL ROADHITS(LVLTYP_TFXFIT,
     2                             HITTYP_HIT,FITTYP_CF)
                              CALL XTS_TRAK_INTERRUPT_VALUE('8')
                              CALL XTS_TRAK('CF',0,'DUFIND, temp')
#                             endif
#                             endif

C add a quicky plot
#                             if defined(CLEO_DUFIND_HBOOK)
                              CALL PLOTS_DUFIND
#                             endif

            CALL CDNEWT
          ENDIF

C*****   Smoke 'em if you got 'em  - we're done for this track!
          CALL KABG2(KABG2_CLEAR,
     1         0,0,0,0, 
     2         0.,0.,0.,0.,0., 0., 
     3         KADUM, ALDUM, BEDUM, GADUM, XIDUM)

          IF(.NOT.PROCESS_TRACK)SFNEXT_CONTROL=0
          CALL CDTRACK_ACCESS(CDT_NTRKCD,IDUM1,NTRKCD)
          IF(NTRKCD.LT.MTRAK2)GO TO 311
       
C=======================================================================
C end
C=======================================================================

C break point when SF seeds are exhaused
        ENDIF
C end of SF pass
        GO TO 211
      ENDIF
#                                  if defined(CLEO_DUFIND_HIT_DUMP)
                                   CALL DUMP_DUFIND
#                                  endif
C abort
 999  CONTINUE
      RETURN
      END















