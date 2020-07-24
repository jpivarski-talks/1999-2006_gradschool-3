* $Id: fast_trak.F,v 1.31 2002/09/24 22:54:06 dpp Exp $
*
* $Log: fast_trak.F,v $
* Revision 1.31  2002/09/24 22:54:06  dpp
*     -> new control constant names for existing do contraint control
*      -> do not change d0 constraint condition from that set in
*         fast_trak_fit_wire_positn for the first pass
*      -> get new hit list after selecting fast_trak_fit_wire_positn rep.
*
* Revision 1.30  2002/09/05 19:30:07  dpp
*      -> remove any code involving "loop over group set"
*      -> remove any code involving the "offset residual"
*      -> add _check_hits_layers argument
*         that indicates that D0 if probably established
*      -> retrieve _fit_wire_positn result from stack
*      -> split off local abiguity resolution iteration
*      -> change names of wire-level arrays to avoid confusion
*      -> test multiple representations from _fit_wire_positn
*
* Revision 1.29  2002/05/23 14:15:23  dpp
*      -> remove unused variable
*
* Revision 1.28  2001/11/19 22:51:25  dpp
*      -> remove direct access of the event control parameter
*      -> use status bit encapsulation
*      -> add new control parameter include file
*      -> remove filling NHITCF,PHI7CF,RKN7CF before STAK operation
*      -> remove call to COMPRESS_FIT: called in FAST_TRAK_DEL_BAD_AND_ADD
*      -> arguments for FAST_TRAK_CHECK_HIT_LAYRS
*      -> use new argument for TFPOPF,TFPUSH
*
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
*      -> reset D0_CONS=.FALSE. after FAST_TRAK_CHECK_HIT_LAYRS
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
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/statusbits.inc"
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
C STAK_LOC.............stack location index returned from TFPOPF
                       INTEGER STAK_LOC
C-----------------------------------------------------------------------
C other local variables
C-----------------------------------------------------------------------
C HAVE_SOLUTION........=.TRUE. if solution was found from last step
                       LOGICAL HAVE_SOLUTION
C MESLOC...............message giving location in program
                       CHARACTER*12 MESLOC
C FIRST_NEXT...........select the "first" (or "best") solution, or the next
                       INTEGER FIRST_NEXT 
C IOPENV...............flag that opening code is run
                       INTEGER IOPENV
C T0_TDC...............T0, in TDC counts, for startup data
                       INTEGER T0_TDC
c                       DATA T0_TDC/11000/
                       DATA T0_TDC/0/
C D0_CRIT..............D0 criteria returned from _CHECK_HIT_LAYRS
                       LOGICAL D0_CRIT 
C Z_CRIT...............Z criteria returned from _CHECK_HIT_LAYRS
                       LOGICAL Z_CRIT 
C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------
      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C DATA
C-----------------------------------------------------------------------

      DATA IOPENV/0/

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C enter the T0 for manual setting of T0
C-----------------------------------------------------------------------
#if defined(CLEO_DOITLOOPBACK)
#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
 101  CONTINUE
      PRINT 6601,T0_TDC
 6601 FORMAT(' FAST_TRAK: START: , ENTER NEW T0? current=',I8)
      CALL XTS_RFV_INPUT
      IF(XTS_ANSWER_IY.EQ.'Y')THEN
        PRINT 6602,T0_TDC
 6602   FORMAT('  FAST_TRAK: current=',I8,' ENTER T0 (like 9575)')
        CALL XTS_RFV_GETINT
        T0_TDC=XTS_ANSWER_IY
      ENDIF
      PRINT 6604,T0_TDC
 6604 FORMAT(' FAST_TRAK:T0_TDC=',F8.1)  
      CALL STARTUP_T_D(1,T0_TDC)
#endif
#endif

C-----------------------------------------------------------------------
C initialization
C-----------------------------------------------------------------------

      ERROR_CODE=0
      
      IF(IOPENV.EQ.0)THEN
        IOPENV=1
        CALL FAST_TRAK_INITIALIZE_CUTS
      ENDIF
      
C-----------------------------------------------------------------------
C set the program indicator to flag hits in fit
C check event control bits
C-----------------------------------------------------------------------
      
      PROGRAM_INDICATOR=P_I_FAST_TRAK
      
C-----------------------------------------------------------------------
C notify user when this is DOIT FAST
C-----------------------------------------------------------------------
#if defined(CLEO_FTDIAG)
      IF(.NOT.L_FT_DOIT_FULL)THEN
        PRINT 1003
 1003   FORMAT(
     1       ' --------------------------------------',
     2       '---------------------------------------')
        PRINT 1004
 1004   FORMAT(' FAST_TRAK: ############### DOIT FAST ############')
        PRINT 1003
      ENDIF
#endif

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
C fit to the wire positions
C _FIT_WIRE_POSITN calls _CHECK_HIT_LAYRS which sets Z0BIAS but no
C  z fits are made before Z0BIAS is set below 
C-----------------------------------------------------------------------

      CALL FAST_TRAK_FIT_WIRE_POSITN
      IF(ERROR_CODE.NE.0)GO TO 910

      
C-----------------------------------------------------------------------
C  FULL PRODUCTION DOIT; NOT DOITFAST
C-----------------------------------------------------------------------
      
      IF(L_FT_DOIT_FULL)THEN
         
C-----------------------------------------------------------------------
C loop over iteration number
C-----------------------------------------------------------------------
        
        ITERATION=0
 111    ITERATION=ITERATION+1

C-----------------------------------------------------------------------
C  start of new iteration
C-----------------------------------------------------------------------
          
        FIRST_NEXT=STAK_PICK_FIRST

C-----------------------------------------------------------------------
C need the HAVE_SOLUTION logical even for the wrap-up
C-----------------------------------------------------------------------

        IF(ITERATION.EQ.1)THEN
          CALL TFPOPF(STAK_FT_WF_OUT,FIRST_NEXT,0,STAK_LOC)
        ELSE
          CALL TFOSTK(STAK_FT_LA_OUT)
          CALL TFPOPF(STAK_FT_LA_OUT,STAK_PICK_FIRST,0,STAK_LOC)
        ENDIF
        
        HAVE_SOLUTION=(STAK_LOC.GT.0)
          
C-----------------------------------------------------------------------
C fill up the STAK with possible solutions
C-----------------------------------------------------------------------

        CALL TFSTKC(STAK_FT_LA_OUT)

C-----------------------------------------------------------------------
C entry point to test another representation from _FIT_WIRE_POSITN 
C-----------------------------------------------------------------------

 211    CONTINUE

C-----------------------------------------------------------------------
C Prepare to do the first iteration to determine L-R points.
C The input of the first iteration is the track found from wire positions.
C Note: WIRES_2_HITS call is required here to provide refresh for
C   alternate wire-position-fit representations  
C-----------------------------------------------------------------------
C-----------------------------------------------------------------------
C For later iterations, 
C   the starting point is the best solution from the previous iteration.
C While WIRES_2_HITS was called at end of the iteration in 
C   fast_trak_loc_amb_iteratn, it also needs to be called here to refresh if 
C   the chosen representation from posible wire-position-fit representations 
C   is different from the ending representation from the LAST call to 
C   fast_trak_loc_amb_iteratn.
C-----------------------------------------------------------------------
          
        IF(ITERATION.EQ.1)THEN

#if defined(CLEO_FTDIAG)
          PRINT 1003
          MESLOC='hits w/time '
          PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
 1015     FORMAT(' FAST_TRAK:',
     2         '[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3         ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4         ' CH=',F9.3)
 1014     FORMAT(' FAST_TRAK:',
     2         '[',A12,'] Y0LF=',F6.3,
     3         ' T=',F6.3,'  NHITLF=',I4) 
#endif
          
        ENDIF

        CALL WIRES_2_HITS(LVLTYP_PAT_REC,HITTYP_HIT,FITTYP_CF)

C-----------------------------------------------------------------------
C start iteration condition,
C   if there is no best solution, exit without solution.
C-----------------------------------------------------------------------

        IF(
     1       (ITERATION.LE.MAX_ITERATE).AND.
     2       (HAVE_SOLUTION)
     3       )THEN

C-----------------------------------------------------------------------
C check hit layers to determine if Z0 constraint is required
C _CHECK_HIT_LAYRS sets Z0BIAS directly, will set to either 
C    Z0BIAS_USE as selected above or Z0BIAS_FULL (ie 0.)
C do not allow D0 constraint to be set by FAST_TRAK_CHECK_HIT_LAYRS
C in the second iteration
C-----------------------------------------------------------------------


          IF(ITERATION.EQ.1)THEN
            Z0BIAS_USE=Z0BIAS_1
            CALL FAST_TRAK_CHECK_HIT_LAYRS(
     2           FT_D0CON_HOLD,FT_SET_Z0BIAS,D0_CRIT,Z_CRIT)

#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1118
 1118       FORMAT(' FAST_TRAK:',
     2           ' D0 constraint is taken from input wire fit')
#endif
            
          ELSE
            Z0BIAS_USE=Z0BIAS_2
            CALL FAST_TRAK_CHECK_HIT_LAYRS(
     2           FT_D0CON_OFF,FT_SET_Z0BIAS,D0_CRIT,Z_CRIT)

#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1119
 1119       FORMAT(' FAST_TRAK:',
     2           ' D0 constraint is not used in later iterations')
#endif
            
          ENDIF
          
C-----------------------------------------------------------------------
C local ambiguity resolution iteration 
C-----------------------------------------------------------------------
          
          CALL FAST_TRAK_LOC_AMB_ITERATN
          IF(ERROR_CODE.NE.0)GO TO 910


C-----------------------------------------------------------------------
C try the other representation on the first iteration 
C-----------------------------------------------------------------------

          IF(ITERATION.EQ.1)THEN
            FIRST_NEXT=STAK_PICK_NEXT
            CALL TFPOPF(STAK_FT_WF_OUT,FIRST_NEXT,0,STAK_LOC)
            HAVE_SOLUTION=(STAK_LOC.GT.0)
            IF(HAVE_SOLUTION)THEN
              GO TO 211
            ENDIF
          ENDIF
          
C-----------------------------------------------------------------------
C  loop back for next iteration
C-----------------------------------------------------------------------
          GO TO 111
C-----------------------------------------------------------------------
C end of iteration condition
C-----------------------------------------------------------------------
        ENDIF

C-----------------------------------------------------------------------
C  DOITFAST ; NOT FULL PRODUCTION DOIT
C-----------------------------------------------------------------------
      ELSE
        CALL TFPOPF(STAK_FT_WF_OUT,STAK_PICK_FIRST,0,STAK_LOC)
        HAVE_SOLUTION=(STAK_LOC.GT.0)

C-----------------------------------------------------------------------
C end of full DOIT / DOITFAST differences, remainder is common code  
C-----------------------------------------------------------------------
      ENDIF
      
C-----------------------------------------------------------------------
C CONTINUE ONLY IF A SOLUTION WAS FOUND
C-----------------------------------------------------------------------
      
      IF(HAVE_SOLUTION)THEN
        
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
        
        
        IF(L_FT_DOIT_FULL)THEN
          CALL FIT_WHATEVER
          
          CALL FAST_TRAK_DEL_BAD_AND_ADD(
     2         FT_NO_DELETE,FT_USE_CELL_FRAC)
          
#if defined(CLEO_FTDIAG)
          PRINT 1003
          MESLOC='aft compress'
          PRINT 1015,MESLOC,
     2         KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
          CALL XTS_TRAK_INTERRUPT_VALUE('2')
          CALL XTS_TRAK('CF',0,'FAST_TRAK aft replace,comp')
c          CALL XTS_TRAK_INTERRUPT_VALUE('-')
          CALL XTS_TRAK_INTERRUPT_VALUE('1')
#endif
#endif
        ENDIF
        
C-----------------------------------------------------------------------
C  load it into the primary stak
C   there can be multiple entries from different offsets
C   and entries will be ordered and popped in DUFIND
C   this is properly preceded by a CF fit in either full doit or doit_fast
C-----------------------------------------------------------------------
        
        CALL TFPUSH(STAK_PRIMARY,STAK_LOC)
        
C-----------------------------------------------------------------------
C  save hits and the status hits for possible restart of the seed
C-----------------------------------------------------------------------
        
        NFITSV = NFIT
        CALL UCOPY( IADFIT, IADFSV, NFIT )
        CALL UCOPY( IPLFIT, IPLFSV, NFIT )
        CALL UCOPY( STATCS, STATSV, 4)
        
C-----------------------------------------------------------------------
C  break point if the wrap-up is to be skipped
C-----------------------------------------------------------------------

      ENDIF
      
C-----------------------------------------------------------------------
C  errors and normal end
C-----------------------------------------------------------------------
      
 910  IF(ERROR_CODE.NE.0)THEN
        CALL HIT_TO_FIT(H2F_ZERO_FIT,0,0)
        
#if defined(CLEO_FTDIAG)
        IF(ERROR_CODE.EQ.1)THEN
          PRINT 9901
 9901     FORMAT(' FAST_TRAK: ',
     2         'DIED IN _FIT_WIRE_POSITN: NOT ENOUGH LAYERS')
        ELSEIF(ERROR_CODE.EQ.2)THEN
          PRINT 9902
 9902     FORMAT(' FAST_TRAK: ',
     2         'DIED IN _FIT_WIRE_POSITN: ',
     3         'NOT ENOUGH LAYERS FILLING TFHITSCD')
        ELSEIF(ERROR_CODE.EQ.3)THEN
          PRINT 9903
 9903     FORMAT(' FAST_TRAK: ',
     2         'DIED IN _FIT_WIRE_POSITN: ',
     3         'NOT ENOUGH ONE HIT LAYERS FOR WIRE FIT')
        ELSEIF(ERROR_CODE.EQ.4)THEN
          PRINT 9904
 9904     FORMAT(' FAST_TRAK: ',
     2         'DIED IN _CLOSE_FIT_STORE: NOT ENOUGH HITS IN HIT FIT')
        ELSEIF(ERROR_CODE.EQ.5)THEN
          PRINT 9905
 9905     FORMAT(' FAST_TRAK: ',
     2         'FAILURE TO CREATE ANY TRACK REPRESENTATIONS ')
          
        ELSEIF(ERROR_CODE.EQ.6)THEN
          PRINT 9906
 9906     FORMAT(' FAST_TRAK: ',
     2         'KILLED BY SFMERC; COMPARISON WITH SF CHAIN')
        ENDIF
#endif
        
      ENDIF
      
      RETURN
      END




