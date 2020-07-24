* $Id: fast_trak.F,v 1.27 2001/09/12 22:01:50 dpp Exp $
*
* $Log: fast_trak.F,v $
* Revision 1.27  2001/09/12 22:01:50  dpp
*      -> new arguments for fast_trak_del_bad_and_add
*      -> call fitter before fast_trak_del_bad_and_add
*
* Revision 1.26  2000/12/19 23:41:41  dpp
*      -> use parameters to control TFPOPF
*      -> now clear secondary stack internally, at end
*      -> new interrupt at end of representation
*      -> do not zero z fit when in mode to limit width of window in tan(dip)
*      -> actively insert zeros in status words for compatibility with recall
*
* Revision 1.25  2000/12/04 15:58:56  dpp
*      -> FIX ERROR; refresh with WIRES_2_HITS after
*                    the TFPOPF at the end of the iteration
*
* Revision 1.24  2000/11/21 19:17:34  dpp
*      -> On 2000/10/2 a second call to fast_trak_del_bad_and_add was
*         installed near the end of fast_track to replace any hit with a
*         better one in the same layer. This is not compatible with DoitFast
*         because DoitFast can have two hits on the same layer (in contrast,
*         at this point, Doit does not). Of the two hits on the same layer
*         delivered by DoitFast, one of them has a smaller residual. The 2nd
*         call to fast_trak_del_bad_and_add replaces the larger_residual_hit
*         with the lower_residual_hit and also leaves the original instance
*         of the lower_residual_hit in place; the lower_residual_hit appears
*         twice. Remove 2nd call to fast_trak_del_bad_and_add in DoitFast.
*
* Revision 1.23  2000/11/15 16:51:45  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.22  2000/10/24 22:35:34  dpp
*      -> set program indicator with parameter
*      -> set UPTODATE_LYR flag .FALSE.
*
* Revision 1.21  2000/10/02 20:56:39  dpp
*      -> new use of STAT words
*      -> set PROGRAM_INDICATOR
*      -> eliminate Z0_CONSTRAIN variable; use Z0BIAS
*      -> adjust interupt values; make room/define value for BOOT_SI_Z_HITS
*
* Revision 1.20  2000/08/18 18:47:44  dpp
*      -> reset D0_CONSTRAIN=.FALSE. after FAST_TRAK_CHECK_HIT_LAYRS
*
* Revision 1.19  2000/04/26 14:38:58  dpp
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*
* Revision 1.18  2000/03/13 19:51:51  dpp
* fix bug: TFPOPF call at end of iteration was inside of diag switch
*
* Revision 1.17  2000/03/01 23:00:42  dpp
*      -> compatible with argument change in STARTUP_T_D
*
* Revision 1.16  2000/02/18 21:23:31  dpp
*  -> now use compile flag DOITLOOPBACK for control of loop back
*
* Revision 1.15  2000/02/12 20:21:25  dpp
*      -> use NEXPT (the doit running level flag) to select Doit or DoitFast
*
* Revision 1.14  2000/01/24 19:49:57  dpp
*      -> loop for manually investigating T0 for startup data
*         controlled by the compile switch CLEO3STARTUP
*      -> Z0 constraint for startup data
*         controlled by the compile switch CLEO3STARTUP
*
* Revision 1.13  2000/01/06 22:16:33  dpp
* fix error handing
* controls and loop to test offsets in inner layers
* use FAST_TRAK_CHECK_HIT_LAYERS to decide to constrain Z
* ZERO the Z projection when NHITLF=0
*
* Revision 1.12  1999/12/09 21:05:14  dpp
*  change call to hit_to_fit to use parameter directive
*
* Revision 1.11  1999/11/23 23:25:48  dpp
* fix diagnostic, must call TFPOPF after FAST_TRAK_AMB_GRP_TO_TRAK
*
* Revision 1.10  1999/11/22 21:02:24  dpp
* DoitFast: now skips to TFPUSH after fast_trak_fit_wire_positn
*
* Revision 1.9  1999/11/14 18:16:45  dpp
* change to calling fast_trak_alternate_amb_2
* use KAPHLD to save fit for consistant start on each representation
*
* Revision 1.8  1999/11/03 22:45:38  dpp
* break-up into several smaller subroutines
* this is now the main control
* add switches for fast version
*
* Revision 1.7  1999/09/12 22:41:10  dpp
* remove arguments from call to SZ_FITTER, CFDFIT, CFRFT4;
*  use COMPRESS_FIT instead of local compress
*
* Revision 1.6  1999/08/03 21:51:01  dpp
* major changes to amb group solutions; "best"
*
* Revision 1.5  1999/04/28 14:01:46  dpp
* add switch to this diagnostic also
*
* Revision 1.4  1999/04/27 19:56:51  dpp
* redundant amb group sets; remove SFMERC
*
* Revision 1.3  1999/01/29 15:04:35  dpp
* more useful comment brackets
*
* Revision 1.2  1998/10/07 15:06:24  lyon
* All layers are now available to fast_trak
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK
C.......................................................................
C.
C. FAST_TRAK - Find a track in the using the hit list
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 
C.
C. Modification Log.
C. 21-March-95 updated to cleo3 version that runs on stereo wires
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE


C     Find a SF track by:
C     1) searching in a road about the SFIND track for minimum hit multiplicity
C     2) fitting those hits to a first track
C     3) using that track to determine the L-R points
C     4) fitting those points to a "final" track

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfstakcd.inc"

#include "doit/sfseq/xts_cntl.inc"

#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

* INDX_GROUP...........counter of ambiguity group number
                       INTEGER INDX_GROUP
C OFFSET_MODE..........index to identify the standard run from 
C                      run in which the inner layers are offset
                       INTEGER OFFSET_MODE
C INDX_OFFSET..........counter of the loop over offsets of the inner layers
                       INTEGER INDX_OFFSET

C-----------------------------------------------------------------------
C other local variables
C-----------------------------------------------------------------------

C MESLOC...............message giving location in program
                       CHARACTER*12 MESLOC
C IOPENV...............flag that opening code is run
                       INTEGER IOPENV
C ITERATION............iteration number for ambiguity resolution
                       INTEGER ITERATION
C FIRST_NEXT...........select the "first" (or "best") solution, or the next
                       INTEGER FIRST_NEXT 
C T0_TDC...............T0, in TDC counts, for startup data
                       INTEGER T0_TDC
c                       DATA T0_TDC/11000/
                       DATA T0_TDC/0/

C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C DATA
C-----------------------------------------------------------------------

      DATA IOPENV/0/


      integer ifit

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

#if defined(CLEO_DOITLOOPBACK)
#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
 101  CONTINUE
      PRINT 6601,T0_TDC
 6601 FORMAT(' FAST_TRAK: START: , ENTER NEW T0? current=',I8)
      CALL XTS_RFV_INPUT
      IF(XTS_ANSWER_IY.EQ.'Y')THEN
         PRINT 6602,T0_TDC
 6602    FORMAT('  FAST_TRAK: current=',I8,' ENTER T0 (like 9575)')
         CALL XTS_RFV_GETINT
         T0_TDC=XTS_ANSWER_IY
      ENDIF
      PRINT 6604,T0_TDC
 6604 FORMAT(' FAST_TRAK:T0_TDC=',F8.1)  
      CALL STARTUP_T_D(1,T0_TDC)
#endif
#endif

      ERROR_CODE=0

      IF(IOPENV.EQ.0)THEN
         IOPENV=1
         CALL FAST_TRAK_INITIALIZE_CUTS
      ENDIF

C-----------------------------------------------------------------------
C set the program indicator to flag hits in fit
C-----------------------------------------------------------------------

      PROGRAM_INDICATOR=P_I_FAST_TRAK

C-----------------------------------------------------------------------
C set the flag to require full execution of the hit finding routines.
C when =.TRUE., this flag allows skipping certain redundant parts
C and allows using the pointers from the HITs to the FITs to update the
C FIT variables.
C it is reset =.FALSE. several places, notably...
C HERE, when the WIRES_2_HITS inputs change, and in TFPOPF, and ROADHITS. 
C-----------------------------------------------------------------------

      UPTODATE_HIT=.FALSE.
      CALL VFILL(UPTODATE_LYR,KLYRDM,.FALSE.)

C-----------------------------------------------------------------------
C loop over the group sets
C-----------------------------------------------------------------------
c      DO 799 INDX_GROUP_SET=1,DIM_GROUP_SET

      OFFSET_MODE=1
 106  CONTINUE
      INDX_OFFSET=FIRST_INDX_OFFSET(OFFSET_MODE)-1
 108  INDX_OFFSET=INDX_OFFSET+1
      IF(INDX_OFFSET.LE.LAST_INDX_OFFSET(OFFSET_MODE))THEN

      INDX_GROUP_SET=1
      OFFSET_RES=OFFSET_INNER(INDX_OFFSET)

      IF(NUMB_OF_GROUPS(INDX_GROUP_SET).LT.1)GO TO 799
      CALL TFSTKC(SECONDARY_STAK)

C-----------------------------------------------------------------------
C make tests on number of available hits
C fit to the wire positions
C _FIT_WIRE_POSITN calls _CHECK_HIT_LAYRS which sets Z0BIAS but no
C  z fits are made before Z0BIAS is set below 
C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG)
      IF(NEXPT.LT.EXB_DOIT_FULL)THEN
         PRINT 1003
         PRINT 1004
 1004    FORMAT(' FAST_TRAK: ############### DOIT FAST ############')
         PRINT 1003
      ENDIF
#endif
      CALL FAST_TRAK_FIT_WIRE_POSITN
      IF(ERROR_CODE.NE.0)GO TO 910
      OFFSET_RES=0.

C-----------------------------------------------------------------------
C  FULL PRODUCTION DOIT; NOT DOITFAST
C-----------------------------------------------------------------------

      IF(NEXPT.GE.EXB_DOIT_FULL)THEN

C-----------------------------------------------------------------------
C  found start track from wire positions; now determine L-R points
C  initialize the iteration number
C-----------------------------------------------------------------------

         ITERATION=1

C-----------------------------------------------------------------------
C  transfer into the /TFHITSCD/ arrays; using drift distances
C  WIRES_2_HITS will correct wire position for "Z" at the layer radius
C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG)
         PRINT 1003
 1003    FORMAT(
     1' --------------------------------------------------------------')
         MESLOC='hits w/time '
         PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
 1015    FORMAT(
     2        ' FAST_T:[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3        ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4        ' CH=',F9.3)
 1014    FORMAT(
     2        ' FAST_T:[',A12,'] Y0LF=',F6.3,' T=',F6.3,'  NHITLF=',I4) 
#endif

         CALL WIRES_2_HITS(USE_PAT_REC,USE_HIT,USE_CF)

C-----------------------------------------------------------------------
C  start of new iteration
C  for first iteration, fill up the TRAKSTAK with possible solutions
C  for further iterations start with the best solution from previous iteration
C  (best solution  was selected at the end of the previous iteration)
C  and clear the stack for this iteration
C  if no best, go to end of loop over group set
C-----------------------------------------------------------------------

 111     CONTINUE

         IF(ITERATION.GE.2)THEN
            IF (ISTKTF(SECONDARY_STAK).LE.0)GO TO 799
            CALL TFSTKC(SECONDARY_STAK)
         ENDIF

         IF(ITERATION.EQ.1)THEN
            Z0BIAS=Z0BIAS_1
         ELSE
            Z0BIAS=Z0BIAS_2
         ENDIF
         Z0BIAS_USE=Z0BIAS

C-----------------------------------------------------------------------
C check hit layers to determine if Z0 constraint is required
C _CHECK_HIT_LAYRS sets Z0BIAS directly, will set to either 
C    Z0BIAS_USE as selected above or Z0BIAS_FULL (ie 0.)
C do not allow D0 constraint to be set by FAST_TRAK_CHECK_HIT_LAYRS
C-----------------------------------------------------------------------
         CALL FAST_TRAK_CHECK_HIT_LAYRS
         IF(ITERATION.GE.2)THEN
            D0_CONSTRAIN=.FALSE.
#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1119
 1119       FORMAT(' FAST_T: removing D0 constraint')
#endif
         ENDIF


         STAT_BIT_BASE=(ITERATION-1)*10 
         IF(D0_CONSTRAIN)THEN
            CALL SBYT(1,STATCF(2),STAT_BIT_BASE+1,1)
         ELSE
            CALL SBYT(0,STATCF(2),STAT_BIT_BASE+1,1)
         ENDIF
         
         IF(Z0BIAS.LE.Z0BIAS_FULL)THEN
            CALL SBYT(1,STATCF(2),STAT_BIT_BASE+2,1)
         ELSE
            CALL SBYT(0,STATCF(2),STAT_BIT_BASE+2,1)
         ENDIF
         
C=====================================================================
C  resolve L-R ambiguity with local half cell stagger
C=====================================================================

         CALL UCOPY(KAPCF,KAPHLD,NPARTF)

         IF( EXECUTE_AMBIGUITY.AND.
     2        (NUMB_OF_GROUPS(INDX_GROUP_SET).GT.0))THEN
            
            CALL FAST_TRAK_POPULAT_AMB_GRP
            CALL FAST_TRAK_AMB_GRP_TO_TRAK

#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1120,NFIT
 1120       FORMAT(' FAST_T: DID AMBIGUITY GROUPS, STANDARD,',
     2           ' NFIT=',I3)
#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK('CF',0,
     2           'FAST_TRAK ambiguity groups')
#endif
#endif

C-----------------------------------------------------------------------
C loop back for a new T0
C-----------------------------------------------------------------------

#if defined(CLEO_DOITLOOPBACK)
#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
            PRINT 6603
 6603       FORMAT(' FAST_TRAK: START: ,LOOP BACK FOR NEW T0?')
            CALL XTS_RFV_INPUT
            IF(XTS_ANSWER_IY.EQ.'Y')THEN
               GO TO 101
            ENDIF
#endif
#endif

C-----------------------------------------------------------------------
C fill in the other hits
C-----------------------------------------------------------------------

            CALL SBYT(0,STATCF(2),STAT_BIT_BASE+3,2)
            CALL SBYT(NFIT,STATCF(2),STAT_BIT_BASE+5,6)

            CALL FAST_TRAK_CLOSE_FIT_STORE
            IF(ERROR_CODE.NE.0)GO TO 910

C-----------------------------------------------------------------------
C  load the alternate solutions
C-----------------------------------------------------------------------

            IF(EXECUTE_ALTERNATE)
     2           CALL FAST_TRAK_ALTERNATE_AMB_2
            
C-----------------------------------------------------------------------
C  branch point if there are no groups in the group set
C  or if local ambiguity is disabled
C-----------------------------------------------------------------------
         ELSE
         
C-----------------------------------------------------------------------
C  when local ambiguity is not used,
C  zero NFIT for new iteration in HIT_TO_FIT 
C  mark all layers as not resolved
C  and fill the track with the closest hit  
C-----------------------------------------------------------------------

            CALL HIT_TO_FIT(H2F_ZERO_FIT,0,0)
            HITBSC=0
            HITASC=0
            TF_LAYR_MIN=999
            TF_LAYR_MAX=0
            CALL VZERO(FLAG_LYR_USED,NLYRHT)
            CALL SBYT(0,STATCF(2),STAT_BIT_BASE+3,2)
            CALL SBYT(0,STATCF(2),STAT_BIT_BASE+5,6)

            CALL FAST_TRAK_CLOSE_FIT_STORE
         ENDIF

C-----------------------------------------------------------------------
C error if there is nothing stored
C-----------------------------------------------------------------------

         IF(NSTKTF(SECONDARY_STAK).LE.0)THEN
            ERROR_CODE=5
            GO TO 910
         ENDIF

C-----------------------------------------------------------------------
C  loop over representations
C  refit in "Z" , followed by refit in "r-phi"
C  loop through the stack, update the stack
C-----------------------------------------------------------------------

#if defined(CLEO_SFZFIT)
         FIRST_NEXT=STAK_PICK_FIRST
 303     CONTINUE

C-----------------------------------------------------------------------
C  do not take from stack (do not reset UPTODATE_HIT) if only one in stack
C  but do call TFPOPF if CLEO_XTSUBS because FAST_TRAK_AMB_TO_TRAK
C    is called in FAST_TRACK_ALTERNAT_AMB_2 if CLEO_XTSUBS  
C-----------------------------------------------------------------------
#if defined(CLEO_XTSUBS)
         CALL TFPOPF(SECONDARY_STAK,FIRST_NEXT,0)
         FIRST_NEXT=STAK_PICK_NEXT
#else
         IF(NSTKTF(SECONDARY_STAK).GT.1)THEN
            CALL TFPOPF(SECONDARY_STAK,FIRST_NEXT,0)
            FIRST_NEXT=STAK_PICK_NEXT
         ELSE
            ISTKTF(SECONDARY_STAK)=1
         ENDIF
#endif
C-----------------------------------------------------------------------
C  process the representation
C-----------------------------------------------------------------------

         IF (ISTKTF(SECONDARY_STAK).GT.0) THEN
         
#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1031,ISTKTF(SECONDARY_STAK)
 1031       FORMAT(' FAST_T: start of Z fitting for solution=',I3)
            PRINT 1003
            MESLOC='hits B4rzfit'
            PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif

            CALL WIRES_2_HITS(USE_PAT_REC,USE_HIT,USE_LF)
            CALL FIT_WHATEVER
            IF((.NOT.F_T_DZ_WINDOWS).AND.(NHITLF.EQ.0))THEN
               TANDLF=0
               Y0LF=0
            ENDIF

#if defined(CLEO_FTDIAG)
            PRINT 1003
            MESLOC='hitsAFTrzfit'
            PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
            PRINT 1014,MESLOC,Y0LF,TANDLF,NHITLF
            CFIX(110+ITERATION)=Y0LF
#endif

            CALL WIRES_2_HITS(USE_PAT_REC,USE_HIT,USE_CF)
            CALL FIT_WHATEVER

#if defined(CLEO_FTDIAG)
            PRINT 1003
            MESLOC='after RF fit'
            PRINT 1015,MESLOC,
     2           KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK('CF',0,'FAST_TRAK after RF FIT')
#endif
#endif

C end of the (CLEO_SFZFIT) condition
#endif

C-----------------------------------------------------------------------
C  must refill XFIT and YFIT for fitting; not saved in stack
C  but maybe this is obsolete if the track is not coming from the stack
C-----------------------------------------------------------------------
C            CALL WIRES_2_HITS(USE_PAT_REC,USE_HIT,USE_CF)

C-----------------------------------------------------------------------
C  remove hits with large residuals
C  argument selects mode that remove hits first 
C-----------------------------------------------------------------------

            CALL FAST_TRAK_DEL_BAD_AND_ADD(FT_DEL_BAD,FT_USE_CELL_FRAC)

C-----------------------------------------------------------------------
C  change the "Z" fit
C-----------------------------------------------------------------------
#if defined(CLEO_SFZFIT)
#if defined(CLEO_FTDIAG)
            PRINT 1003
            MESLOC='hits B4rzfit'
            PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif

            CALL WIRES_2_HITS(USE_PAT_REC,USE_HIT,USE_LF)
            CALL FIT_WHATEVER
            IF((.NOT.F_T_DZ_WINDOWS).AND.(NHITLF.EQ.0))THEN
               TANDLF=0
               Y0LF=0
            ENDIF

#if defined(CLEO_FTDIAG)
            PRINT 1003
            MESLOC='hitsAFTrzfit'
            PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
            PRINT 1014,MESLOC,Y0LF,TANDLF,NHITLF
            CFIX(113)=Y0LF
            CFIX(121)=ABS(KAPCF)
            CFIX(122)=ZTMP-(Y0LF+.3*TANDLF)
#endif

            CALL WIRES_2_HITS(USE_PAT_REC,USE_HIT,USE_CF)
#endif

C-----------------------------------------------------------------------
C fill in the cfid parameters
C-----------------------------------------------------------------------
            
            CALL CDCFID(CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)
            
C-----------------------------------------------------------------------
C  compress the hit list
C-----------------------------------------------------------------------

            CALL COMPRESS_FIT(LUSETF)

C-----------------------------------------------------------------------
C update here if removing large residuals first
C-----------------------------------------------------------------------

            NHITCF = NFIT
            NHITTF = NHITCF
            CHITF  = CHICF
            PHI7CF = 0
            RKN7CF = 9.0+.01*      INDX_GROUP_SET
            CALL TFUSTK(SECONDARY_STAK,0)

#if defined(CLEO_FTDIAG)
            PRINT 1003
            MESLOC='end of reprs'
            PRINT 1701,MESLOC,CURCS,PHI0CS,D0CS,NFIT
 1701       FORMAT(' FAST_T:[',A12,'] C=',F6.3,' PHI=',F6.3,
     2                        ' D0=',F6.3,' NFIT=',I3)
#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK_INTERRUPT_VALUE('2')
            CALL XTS_TRAK('CF',0,'FAST_TRAK end of representatio')
            CALL XTS_TRAK_INTERRUPT_VALUE('-')
#endif
#endif

C-----------------------------------------------------------------------
C loop back here for next representation in the stack
C-----------------------------------------------------------------------

            IF(NSTKTF(SECONDARY_STAK).GT.1)GO TO 303
         ENDIF

C-----------------------------------------------------------------------
C  try more iterations?
C  order the stack before starting new iteration 
C  try more iterations here if removing large residuals first
C-----------------------------------------------------------------------

         CALL TFOSTK(SECONDARY_STAK)
         CALL TFPOPF(SECONDARY_STAK,STAK_PICK_FIRST,0)

         CALL WIRES_2_HITS(USE_PAT_REC,USE_HIT,USE_CF)

#if defined(CLEO_FTDIAG)
         PRINT 1003
         MESLOC='end iteratn '
         PRINT 1015,MESLOC,
     2           KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('2')
         CALL XTS_TRAK('CF',0,'FAST_TRAK end of iteration')
         CALL XTS_TRAK_INTERRUPT_VALUE('-')
#endif
#endif

         ITERATION=ITERATION+1
         IF(ITERATION.LE.MAX_ITERATE)GO TO 111

C-----------------------------------------------------------------------
C end of full production DOIT, remainder is common code  
C-----------------------------------------------------------------------

      ENDIF

C-----------------------------------------------------------------------
C  compare the hit list to that provided by the SFIND chains
C-----------------------------------------------------------------------

c not for now      CALL SFMERC(IADFIT,NFIT)

      IF(NFIT.EQ.0)THEN
         ERROR_CODE=6
         GO TO 910
      ENDIF

C-----------------------------------------------------------------------
C  replace hits when there is a hit on the layer with a better residual
C  argument selects mode to NOT delete all bad hits first
C
C  NOTE: iteration ended with sz fit, tfpop, and then filling r-phi hits
C    requires an r-phi fit to fill PULFIT required by f_t_del_bad_and_add
C
C  and then
C  compress the hit list
C-----------------------------------------------------------------------


      IF(NEXPT.GE.EXB_DOIT_FULL)THEN
         CALL FIT_WHATEVER
         
         CALL FAST_TRAK_DEL_BAD_AND_ADD(FT_NO_DELETE,FT_USE_CELL_FRAC)
         CALL COMPRESS_FIT(LUSETF)

#if defined(CLEO_FTDIAG)
         PRINT 1003
         MESLOC='aft compress'
         PRINT 1015,MESLOC,
     2           KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('2')
         CALL XTS_TRAK('CF',0,'FAST_TRAK aft replace,comp')
         CALL XTS_TRAK_INTERRUPT_VALUE('-')
#endif
#endif
      ENDIF

C-----------------------------------------------------------------------
C  load it into the stak
C-----------------------------------------------------------------------

      NHITCF = NFIT
      NHITTF = NHITCF
      CHITF  = CHICF
      PHI7CF = 0
      RKN7CF = 9.0+.01*      INDX_GROUP_SET

      CALL TFPUSH(PRIMARY_STAK)


C-----------------------------------------------------------------------
C end loop over offset
C-----------------------------------------------------------------------

#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
      PRINT 1003
      PRINT 1021,OFFSET_MODE,INDX_OFFSET,OFFSET_INNER(INDX_OFFSET),
     2           CHICF,D0CF
 1021 FORMAT(' FAST_TRAK: offset_MODE=',I2,'  offset(',I2,')=',F8.4,
     2       '   CHICF=',F9.3,'  D0CF=',F9.5)
C      CALL XTS_RFV_INPUT
#endif

      GO TO 108
      ENDIF
      
C-----------------------------------------------------------------------
C possibly test the offsets 
C-----------------------------------------------------------------------

      IF(OFFSET_MODE.EQ.1)THEN
         IF(  (CHICF.GT.30.).AND.
     1        (ABS(D0CF).GT.0.01))THEN
            OFFSET_MODE=2
C            GO TO 106
         ENDIF
      ENDIF

C-----------------------------------------------------------------------
C  save hits and the status hits for possible restart of the seed
C-----------------------------------------------------------------------

      NFITSV = NFIT
      CALL UCOPY( IADFIT, IADFSV, NFIT )
      CALL UCOPY( IPLFIT, IPLFSV, NFIT )
      CALL UCOPY( STATCS, STATSV, 4)

C-----------------------------------------------------------------------
C  end of loop over INDX_GROUP_SET
C-----------------------------------------------------------------------

 799  CONTINUE
      CALL TFSTKC(SECONDARY_STAK)

C-----------------------------------------------------------------------
C  errors and normal end
C-----------------------------------------------------------------------

 910  IF(ERROR_CODE.NE.0)THEN
         CALL HIT_TO_FIT(H2F_ZERO_FIT,0,0)

#if defined(CLEO_FTDIAG)
         IF(ERROR_CODE.EQ.1)THEN
            PRINT 9901
 9901       FORMAT(' FAST_TRAK: ',
     2           'DIED IN _FIT_WIRE_POSITN: NOT ENOUGH LAYERS')
         ELSEIF(ERROR_CODE.EQ.2)THEN
            PRINT 9902
 9902       FORMAT(' FAST_TRAK: ',
     2           'DIED IN _FIT_WIRE_POSITN: ',
     3           'NOT ENOUGH LAYERS FILLING TFHITSCD')
         ELSEIF(ERROR_CODE.EQ.3)THEN
            PRINT 9903
 9903       FORMAT(' FAST_TRAK: ',
     2           'DIED IN _FIT_WIRE_POSITN: ',
     3           'NOT ENOUGH ONE HIT LAYERS FOR WIRE FIT')
         ELSEIF(ERROR_CODE.EQ.4)THEN
            PRINT 9904
 9904       FORMAT(' FAST_TRAK: ',
     2           'DIED IN _CLOSE_FIT_STORE: NOT ENOUGH HITS IN HIT FIT')
         ELSEIF(ERROR_CODE.EQ.5)THEN
            PRINT 9905
 9905       FORMAT(' FAST_TRAK: ',
     2           'FAILURE TO CREATE ANY TRACK REPRESENTATIONS ')

         ELSEIF(ERROR_CODE.EQ.6)THEN
            PRINT 9906
 9906       FORMAT(' FAST_TRAK: ',
     2           'KILLED BY SFMERC; COMPARISON WITH SF CHAIN')
         ENDIF
#endif

      ENDIF

      RETURN
      END






