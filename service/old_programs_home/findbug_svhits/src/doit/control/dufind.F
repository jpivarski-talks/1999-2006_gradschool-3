*
* $Id: dufind.F,v 1.32 2001/09/12 21:57:44 dpp Exp $
*
* $Log: dufind.F,v $
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
*      -> move setting of staus bit for boot_si_z_hits to that program
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
*      -> reset D0_CONSTRAIN=.FALSE. after fast_trak_check_hit_layrs
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
*   fix bug by calling TFSTKC(SECONDARY_STAK) before TFXFIT;
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
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    :
C.
C. VERSION   : 1.0
C. CREATED   : xx-Mar-82
C. LAST MOD  : 20-Apr-90
C.
C. Modification Log.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tfhistpa.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdcorrcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfstakcd.inc"
#include "doit/duseq/fast_trak.inc"
#include "doit/duseq/tftime.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tffitctl.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/duseq/svscratch.inc"
#include "doit/duseq/svgeometry.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif


C LUTR3FIT........logical, if =.TRUE. use TF3FIT
C LUCDCORH........logical, if =.TRUE. use CDCORH
C LCCDNEWT........logical, if =.TRUE. Call CDNEWT
C SFPASS..........SF pass numper
C MSFPASS.........Maximum SF pass numper
C LDOPASS.........logical, if =.TRUE. do pass
C TRY_Z0_CONSTR...try Z0 constraint in TFXFIT_Z

      LOGICAL LUTF3FIT,LUCDCORH,LCCDNEWT,LDOPASS
      LOGICAL TRY_Z0_CONSTR

      REAL Z0BIAS_SAV

      INTEGER SFPASS,MSFPASS
      INTEGER FIRST_NEXT

      INTEGER ITRK,IFSTAT,NLDROP,IDUM,ILYR
      LOGICAL LFOUND,LDIAG1,LDIAG2
      REAL RMSRET,RMS0,RMSLAS,DIRRMS,T0BOT
      REAL TFSAV,TDSAV,TCSAV
      REAL CHIRET
      integer ihyb
      real svwafer

C NFIT_HOLD.......hold the value of NFIT to compare to TFXFIT output
C N_CALL_TFXFIT...number of calls of TFXFIT in a loop
C LCTFXFIT........logical, if =.TRUE. process the TFXFIT sequence
C DIDRECALL.......=.TRUE. if FAST_TRAK_RECALL_DZ_WINDO
C                         did the recall and exits with CF hits

      INTEGER NFIT_HOLD,N_CALL_TFXFIT
      LOGICAL LCTFXFIT,REPLACED,DIDRECALL


      INTEGER JBIT
      EXTERNAL JBIT

      EXTERNAL CFXYHT,LFXYHT

C== Dummy variables for KABG2
      REAL    KADUM, ALDUM, BEDUM, GADUM, XIDUM

* ----------Executable code starts here---------------------


C DUDE! this is necessary until the geometry is fixed
C       this is (only slightly more than) the minimum required for FINDHW 

      nwirtf(56)=1536
      nwrztf(56)=192
      phiftf(56)=3.14159265/8.
      celltf(56)=3.14159265/4.
      celztf(56)=.0116
      rtf(56)=rtf(55)
      rsqtf(56)=rsqtf(55)
C so, the above can be deleted when the geometry is verified



C DUDE! this necesssary because the silicon geomtry is wrong
C SVBOUNDARY_Z(hybrid,2)..... Z coordinate of boundary of wafer
      svwafer=0.053
      do 41 ihyb=1,7
         svboundary_z(   ihyb,1)=  1.5  *svwafer
         svboundary_z(61+ihyb,2)= -1.5  *svwafer
         if(ihyb.le.3)then
            svboundary_z(   ihyb,2)= -0.5  *svwafer
         else
            svboundary_z(   ihyb,2)=  0.5  *svwafer
         endif
         svboundary_z(61+ihyb,1)=svboundary_z(   ihyb,2)
 41   continue
      do 42 ihyb=8,17
         svboundary_z(   ihyb,1)=  2.0  *svwafer
         svboundary_z(61+ihyb,2)= -2.0  *svwafer
         svboundary_z(   ihyb,2)=  0.0
         svboundary_z(61+ihyb,1)=svboundary_z(   ihyb,2)
 42      continue
      do 43 ihyb=18,35
         svboundary_z(   ihyb,1)=  3.5  *svwafer
         svboundary_z(61+ihyb,2)= -3.5  *svwafer
         idum=ihyb/2
         if(2*idum.ne.ihyb)then
            svboundary_z(   ihyb,2)= -0.5  *svwafer
         else
            svboundary_z(   ihyb,2)=  0.5  *svwafer
         endif
         svboundary_z(61+ihyb,1)=svboundary_z(   ihyb,2)
 43      continue
      do 44 ihyb=36,61
         svboundary_z(   ihyb,1)=  5.0  *svwafer
         svboundary_z(61+ihyb,2)= -5.0  *svwafer
         svboundary_z(   ihyb,2)=  0.0
         svboundary_z(61+ihyb,1)=svboundary_z(   ihyb,2)
 44   continue
C so, the above can be deleted when the geometry is verified

C call TFXFIN so that 
C the data base, or default, efficiencies can be overwritten
C NOT NORMAL !!!!!!!!!!!!!!!!!!!!!!!!

C      CALL TFXFIN

C initialize the Doit/DoitFast dependent variables for FAST_TRAK
      
      CALL FAST_TRAK_INITIALIZE_EVNT

C initialize the silicon available array
      CALL VFILL(SV_AVAIL,SV_NHITS,1)

C initialise the error buffer

      CALL TFBERI
      NEVUSE = NEVUSE + 1

C set-up for SF track finding

      LSFCMP   = .FALSE.
      LCFXLA   = .TRUE.
      LCFXPR   = .TRUE.
      LCALIB   = .FALSE.
      DUALTR   = .FALSE.
      LUTF3FIT = .FALSE.
      LUCDCORH = .FALSE.

      IF(NEXPT.GE.EXB_DOIT_FULL)THEN
         MSFPASS=3
      ELSE
         MSFPASS=1
      ENDIF

      CALL KABG2(KABG2_CLEAR,
     1     0,0,0,0, 
     2     0.,0.,0.,0.,0., 0., 
     3     KADUM, ALDUM, BEDUM, GADUM, XIDUM)


C initializ without looking at seed tracks
      NTRKIN=0
      NTRACK = NTRKIN
      NTRKCD = 0
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(1,IDUM,LDIAG1)
#                                  endif

C                                  general XTS plotting call
#                                  if !defined(CLEO_SFDUET)
#                                  if defined(CLEO_XTSUBS)
                                   XTS_ABORT=0
C                                   IF (LXTSUB) CALL DSF_RFVIEW('DUFIND')
                                   IF(XTS_ABORT.EQ.1)GO TO 999
#                                  endif
#                                  endif
                                   
#            if defined(CLEO_DIAG_DUFIND)
             DO 119 ILYR=1,NLYRTF
                IF(LCLSTR_LAYER(ILYR))THEN
                   CALL SIMPLE_CATHODE_CLUSTER(ILYR)
                ENDIF
 119         CONTINUE
#            endif

      SFPASS=0
 211  SFPASS=SFPASS+1
      IF(SFPASS.GT.MSFPASS)THEN
         LDOPASS=.FALSE.
      ELSE
         IF(SFPASS.EQ.1)THEN

            IF(NEXPT.GE.EXB_DOIT_FULL)THEN
               IF(IAND(NEXPT,EXB_SF_STARTUP).EQ.EXB_SF_STARTUP)THEN
                  CALL SFCUTS('DUFIND', 'STARTUP')
               ELSE 
                  CALL SFCUTS('DUFIND', 'CLEAN')
               ENDIF
            ELSE
               CALL SFCUTS('DUFIND', 'STARTUP')
            ENDIF

            LDOPASS=(NLAYSF.GT.1)
         ELSEIF(SFPASS.EQ.2)THEN
            CALL SFCUTS('DUFIND', '2NDPASS')
            LDOPASS=(NLAYSF.GT.1)
         ELSEIF(SFPASS.GE.3)THEN
            CALL SFCUTS('DUFIND', 'SHORT')
            LDOPASS=(NLAYSF.GT.1)
         ENDIF
      ENDIF

      IF(LDOPASS)THEN
#                                  if defined(CLEO_XTSUBS)
                                   XTS_ABORT=0
#                                  endif
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(3,IDUM,LDIAG1)
#                                  endif
         CALL SFIND
#                                  if defined(CLEO_XTSUBS)
                                   IF(XTS_ABORT.EQ.1)GO TO 999
#                                  endif

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(4,IDUM,LDIAG1)
                                   IF(.NOT.LDIAG1)GO TO 999
#                                  endif
 311     CONTINUE
         CALL SFNEXT
         CALL SBYT(1,STATCS(1),1,3)
         CALL SBYT(SFPASS,STATCS(1),4,3)
         IF (NCHANM.GE.1)THEN
 312        CONTINUE
            
C clear the stack and flag
            CALL TFSTKC(0)
            LCFIND = .FALSE.

C transfer data from SF representation
C into TFHITSCD wire array HTWADR
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(6,IDUM,LDIAG1)
#                                  endif
#if defined(CLEO_DOITLOOPBACK)
#if (defined(CLEO_DIAG_DUFIND))&&(defined(CLEO_XTSUBS))
C       CALL STARTUP_T_D(3,0)
       CALL STARTUP_T_D(2,0)
#endif
#endif
            CALL SFHITS

C try to fit hits in pattern recognition
C to track parameters 
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(7,IDUM,LDIAG1)
#                                  endif
            F_T_DZ_WINDOWS=.FALSE.
            CALL FAST_TRAK
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(8,IDUM,LDIAG1)
#                                  endif
#                                  if defined(CLEO_XTSUBS)
                                   IF(XTS_ABORT.EQ.1)GO TO 999
#                                  endif
            LCFIND = .FALSE.
            LCCDNEWT=.FALSE.

C=======================================================================
C this is for full DOIT;   NOT DoitFast
C  after FAST_TRACK, test NFIT
C                  NFIT may be zero when a track is not found
C=======================================================================

            IF(NEXPT.GE.EXB_DOIT_FULL)THEN
               IF(NFIT.GT.0) THEN
                  NTRACK = NTRACK + 1
                  IFSTAT = IFDRDG
                  KTIMES = KTIMDG

C order the stack
                  CALL TFOSTK(PRIMARY_STAK)
C restore the track from the stack
                  CALL TFPOPF(PRIMARY_STAK,STAK_PICK_FIRST,0)

C cut tracks with bad chi square
                  IF(IAND(NEXPT,EXB_DOIT_NOCHI).NE.EXB_DOIT_NOCHI)THEN
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(9,IDUM,LDIAG1)
#                                  endif
                     IF(
     1                    (SFPASS .LT.MSFPASS).AND.
     2                    (CHICF  .GT. 50.   ) )NFIT=0
                     IF( 
     1                    (SFPASS .EQ.1      ).AND.
     2                    (CHICF  .GT. 20.   ).AND.
     3                    (ABS(D0CF).GT. 0.1 ) )NFIT=0
                  ENDIF

C end of "IF(NFIT.GT.0....   (  #1 )
               ENDIF

C=======================================================================
C  after tests of the track, test NFIT again
C                  NFIT may be zero for badly fit tracks
C=======================================================================

               IF(NFIT.GT.0) THEN

#if defined(CLEO_SFFCMP)
                  LSFCMP = .TRUE.
#endif

C=======================================================================
C decide if TFXFIT CF should be called
C     use 2nd pass through TFXFIT if there are hits aded
C=======================================================================


                  N_CALL_TFXFIT=0                 
 401              N_CALL_TFXFIT=N_CALL_TFXFIT+1

                  IF(N_CALL_TFXFIT.EQ.1)THEN
                     LCTFXFIT=.TRUE.
                  ELSEIF(
     2                    (N_CALL_TFXFIT.LE.2).AND.
     3                    (NFIT.GT.NFIT_HOLD))THEN
                     LCTFXFIT=.TRUE.
                  ELSE
                     LCTFXFIT=.FALSE.
                  ENDIF

                  IF(LCTFXFIT)THEN
                     NFIT_HOLD=NFIT

C=======================================================================
C  fill in r-phi hits with TFXFIT
C=======================================================================

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(10,IDUM,LDIAG1)
#                                  endif
                     CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(11,IDUM,LDIAG1)
#                                  endif

C=======================================================================
C test if the newly defined road indicates there exists
C a much better track solution within the road
C=======================================================================
                     IF(N_CALL_TFXFIT.EQ.1)THEN

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(12,IDUM,LDIAG1)
#                                  endif
                        CALL FAST_TRAK_RECALL_DZ_WINDO
     2                                  (FTR_BEFORE_TFXFIT,DIDRECALL)
                     ENDIF

C=======================================================================
C update the representation in the primary stack with all the status bits
C this saves the initial FAST_TRAK output to compare to FAST_TRAK output
C from FAST_TRAK_RECALL_DZ_WINDO 
C=======================================================================

                     NHITCF = NFIT
                     NHITTF = NHITCF
                     CHITF  = CHICF
                     PHI7CF = 0
                     RKN7CF = 9.0
                     CALL TFUSTK(PRIMARY_STAK,0)

C=======================================================================
C fill in r-phi hits with TFXFIT
C=======================================================================

C CDCORH called in DU3DTR after CFHITS (which calls ROADHITS)
C                    IF(LUCDCORH)CALL CDCORH(CFXYHT)
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(13,IDUM,LDIAG1)
#                                  endif
                     CALL SBYT(1,STATCF(2),(21+(N_CALL_TFXFIT-1)*6),1)
                     CALL TFXFIT
                     LCFIND=.TRUE.

C=======================================================================
C DIAGNOSTIC FOR ITERATING DRIFT FUNCTION
C=======================================================================

#if defined (CLEO_DOITLOOPBACK)
#if (defined(CLEO_DIAG_DUFIND))&&(defined(CLEO_XTSUBS))
                     PRINT 6603
 6603                FORMAT(' DUFIND: ,LOOP BACK FOR NEW T0?')
                     CALL XTS_RFV_INPUT
                     IF(XTS_ANSWER_IY.EQ.'Y')THEN
                        GO TO 312
                     ENDIF
#endif
#endif

C=======================================================================
C CFLAST and CFCHOP section
C  CFCHOP requires previous call to CFHITS(,,USE_CF)
C=======================================================================
#if defined(CLEO_SFENDP)
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(14,IDUM,LDIAG1)
#                                  endif
                     CALL CFLAST(KAPCF,ALPCF,BETCF,GAMCF,PHI6CF,RKN6CF)
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(15,ILRESC(2),LDIAG1)
#                                  endif
                     IF(ILRESC(2).NE.0)CALL CFCHOP
#endif

C=======================================================================
C fill in Z hits........
C refresh the z parameters with ROADHITS
C refit in Z and get the number of Z hits with SZ_FITTER
C=======================================================================
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(16,IDUM,LDIAG1)
#                                  endif
                     CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
                     CALL FIT_WHATEVER

C=======================================================================
C test if there is a better z way off somewhere else
C have called ROADHITS(LF); REPLACE_Z_HITS will end with ROADHITS(LF) 
C=======================================================================
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(17,8,LDIAG1)
#                                  endif
                     IF(NHITLF.LT.8)THEN
                        CALL REPLACE_Z_HITS(REPLACED)
                        IF(REPLACED)CALL SBYT(1,STATCF(2),
     2                       (22+(N_CALL_TFXFIT-1)*6),1)
                     ENDIF

C=======================================================================
C test if the newly defined road indicates there exists
C a much better track solution within the road
C if so, re-run FAST_TRAK for multiple well defined z projections
C FAST_TRAK_RECALL_DZ_WINDO will end with ROADHITS(CF)
C=======================================================================

                     IF(N_CALL_TFXFIT.EQ.1)THEN
                        IF(IAND(NEXPT,EXB_DOIT_NEW).NE.EXB_DOIT_NEW)THEN
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(18,IDUM,LDIAG1)
#                                  endif
                           CALL FAST_TRAK_RECALL_DZ_WINDO
     2                                  (FTR_AFTER_TFXFIT,DIDRECALL)
                           IF(DIDRECALL)THEN
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(16,IDUM,LDIAG1)
#                                  endif
                              CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
                              CALL FIT_WHATEVER
                           ENDIF
                        ENDIF
                     ENDIF

C=======================================================================
C loop back; process TFXFIT (r-phi) again if number of hits has increased
C=======================================================================
                     GO TO 401
                  ENDIF


C=======================================================================
C if enough hits, use FAST_TRAK_CHECK_HIT_LAYRS to remove Z0 constraint
C do not increase the bias; do not reduce Z0BIAS 
C do not allow D0 constraint to be set by FAST_TRAK_CHECK_HIT_LAYRS
C=======================================================================
                  Z0BIAS_SAV=Z0BIAS
                  CALL FAST_TRAK_CHECK_HIT_LAYRS
                  D0_CONSTRAIN=.FALSE.
                  IF(Z0BIAS.LT.Z0BIAS_SAV)THEN
                     Z0BIAS=Z0BIAS_SAV
                  ELSEIF(Z0BIAS.GT.Z0BIAS_SAV)THEN
                     CALL SBYT(1,STATCF(2),29,1)
                  ENDIF

C=======================================================================
C fill in S-Z hits with TFXFIT
C=======================================================================

C CDCORH called in DULFTR after LFHITS (which calls ROADHITS)
C                 IF(LUCDCORH)CALL CDCORH(LFXYHT)

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(21,IDUM,LDIAG1)
#                                  endif

                  TRY_Z0_CONSTR=
     1                 (ABS(Y0LF).GT.0.01).AND.
     2                 (ABS(Y0LF).LT.0.05).AND.
     3                 (NHITLF.GT.10)

                  CALL TFSTKC(0)

                  NHITCF = NFIT
                  NHITTF = NHITCF
                  CHITF  = CHICF
                  PHI7CF = 0
                  RKN7CF = 9.0+.01*      INDX_GROUP_SET
                  CALL SBYT(1,STATCF(3),1,1)
                  CALL TFPUSH(PRIMARY_STAK)

                  IF(TRY_Z0_CONSTR)THEN
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(22,IDUM,LDIAG1)
#                                  endif
                     Z0BIAS=0.
                     CALL FIT_WHATEVER
                     Z0BIAS=Z0BIAS_USE
                     CALL SBYT(1,STATCF(3),2,1)
                     CALL TFPUSH(PRIMARY_STAK)
                  ENDIF

                  FIRST_NEXT=STAK_PICK_FIRST
 502              CALL TFPOPF(PRIMARY_STAK,FIRST_NEXT,0)
                  IF(ISTKTF(PRIMARY_STAK).GT.0) THEN

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(23,
     2                                 ISTKTF(PRIMARY_STAK),LDIAG1)
#                                  endif
                     CALL TFXFIT
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(24,IDUM,LDIAG1)
#                                  endif
                     NHITCF=NFIT
                     NHITTF=NFIT
                     CHITF  = CHICF
                     CALL TFPUSH(SECONDARY_STAK)
#                                  if defined(CLEO_XTSUBS)
                                   IF(XTS_ABORT.EQ.1)GO TO 999
#                                  endif

                     FIRST_NEXT=STAK_PICK_NEXT
                     GO TO 502
                  ENDIF
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(25,IDUM,LDIAG1)
#                                  endif
                  CALL TFOSTK(SECONDARY_STAK)
                  CALL TFPOPF(SECONDARY_STAK,STAK_PICK_FIRST,0)
                  IF(ISTKTF(PRIMARY_STAK).NE.1)
     2                 CALL SBYT(1,STATCF(3),3,1)
 
                  LSFCMP = .FALSE.

C=======================================================================
C when there are no S-Z hits already, fill in S-Z hits with BOOT_SI_Z_HITS
C=======================================================================
                  IF(NHITLF.LT.3)THEN
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(28,IDUM,LDIAG1)
#                                  endif
                     CALL BOOT_SI_Z_HITS(REPLACED)
                  ENDIF
C=======================================================================
C possibly search for track by extending seed track segment
C hits must be set up again because
C LFIND call could have overwritten CFIND info
C=======================================================================
                  IF (.NOT.LCFIND) THEN
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(31,IDUM,LDIAG1)
#                                  endif
                     CALL CDSFSD
C clear the stack
                     CALL TFSTKC(SECONDARY_STAK)
C set the search width
                     CALL UCOPY(NSRWTF(LLYRCF),NSRCTF(LLYRCF),NLYRCF)
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(32,IDUM,LDIAG1)
#                                  endif
                     CALL CFEXTN
                     LCFIND = NFIT.GT.0
                  ENDIF

C end of "IF(NFIT.GT.0....   (  #2 )
               ENDIF

C=======================================================================
C cut tracks with few Z hits
C do not allow D0 constraint to be set by FAST_TRAK_CHECK_HIT_LAYRS
C do not allow Z0BIAS constraint to be set by FAST_TRAK_CHECK_HIT_LAYRS
C cut tracks that used FAST_TRAK_RECALL_DZ_WINDO and stiil have bad fit
C=======================================================================
               IF(SFPASS .LT.MSFPASS)THEN
                  CALL FAST_TRAK_CHECK_HIT_LAYRS
                  D0_CONSTRAIN=.FALSE.
                  Z0BIAS=Z0BIAS_USE
                  IF(.NOT.PASS_Z_CRIT)NFIT=0
                  IF( 
     1                 (SFPASS .EQ.1      ).AND.
     2                 (CHICF  .GT. 20.   ).AND.
     3                 (DIDRECALL         ) )NFIT=0
               ENDIF

C=======================================================================
C  after TFXFIT and CFEXTN, test NFIT again
C                  NFIT may be zero for badly fit tracks
C  call the 3D fitter or
C  use FAST_TRAK_DEL_BAD_AND_ADD to replace bad hits
C=======================================================================
               IF(NFIT.GT.0) THEN
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(33,IDUM,LDIAG1)
#                                  endif
C order the stack
                  CALL TFOSTK(SECONDARY_STAK)
                  CALL TFPOPF(SECONDARY_STAK,STAK_PICK_FIRST,0)
C 3D track fit

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(35,IDUM,LDIAG1)
#                                  endif

                  LKALMN=.FALSE.
                  IF(LUTF3FIT)THEN
                     CALL TF3FIT    
                  ELSE
                     CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)
                     CALL FIT_WHATEVER

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(36,IDUM,LDIAG1)
#                                  endif

                     CALL FAST_TRAK_DEL_BAD_AND_ADD
     2                    (FT_NO_DELETE,FT_USE_SIGMAS)
                     CALL COMPRESS_FIT(LUSETF)

                     CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
                     CALL FIT_WHATEVER

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(37,IDUM,LDIAG1)
#                                  endif

                     CALL FAST_TRAK_DEL_BAD_AND_ADD
     2                    (FT_NO_DELETE,FT_USE_SIGMAS)
                     CALL COMPRESS_FIT(LUSETF)


                     NHIT3F = NHITTF
                     CHI3F  = CHITF
                  ENDIF

                  IF (NHIT3F .GT. 0) THEN
                     NTRT3F(0) = NTRT3F(0) + 1

#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(38,IDUM,LDIAG1)
#                                  endif

C update the stack after the fit
C Pop first TFIND data off the stack
                     NHITTF = NHIT3F
                     CHITF  = CHI3F
                     CALL TFUSTK(SECONDARY_STAK,0)
                     CALL TFPOPF(SECONDARY_STAK,STAK_PICK_FIRST,0)
                     IF (ISTKTF(SECONDARY_STAK).GT.0) THEN
                        LCCDNEWT=.TRUE.
                     ENDIF
                  ENDIF

C end of "IF(NFIT.GT.0....   (  #3 )C Pop first TFIND data off the stack

               ENDIF

C=======================================================================
C this is the end of the full DOIT condition
C this is for DoitFast
C=======================================================================

            ELSE
               IF(NFIT.GT.0) THEN
                  NTRACK = NTRACK + 1
                  LCCDNEWT=.TRUE.
               ENDIF
            ENDIF

C=======================================================================
C store the track
C=======================================================================
            IF(LCCDNEWT)THEN
C set the interrupt value and set up CF hits
  
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(40,IDUM,LDIAG1)
#                                  endif

C add a quicky plot

#                                  if defined(CLEO_DUFIND_HBOOK)
                                   CALL DIAG_DUFIND(71,IDUM,LDIAG1)
#                                  endif

               CALL CDNEWT
            ENDIF

C*****   Smoke 'em if you got 'em  - we're done for this track!
            CALL KABG2(KABG2_CLEAR,
     1           0,0,0,0, 
     2           0.,0.,0.,0.,0., 0., 
     3           KADUM, ALDUM, BEDUM, GADUM, XIDUM)

            IF ( NTRKCD .LT. MTRAK2 ) GO TO 311
       
C=======================================================================
C end
C=======================================================================

C break point when SF seeds are exhaused
         ENDIF
C end of SF pass
         GO TO 211
      ENDIF
#                                  if defined(CLEO_DUFIND_HIT_DUMP)
                                   CALL DIAG_DUFIND(72,IDUM,LDIAG1)
#                                  endif
C abort
 999  CONTINUE
#                                  if defined(CLEO_DIAG_DUFIND)
                                   CALL DIAG_DUFIND(99,IDUM,LDIAG1)
#                                  endif
      RETURN
      END
      




