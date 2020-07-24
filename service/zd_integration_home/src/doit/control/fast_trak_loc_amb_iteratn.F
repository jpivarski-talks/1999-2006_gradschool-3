* $Id: fast_trak_loc_amb_iteratn.F,v 1.1 2002/09/05 19:31:17 dpp Exp $
*
* $Log: fast_trak_loc_amb_iteratn.F,v $
* Revision 1.1  2002/09/05 19:31:17  dpp
*      -> NEW ROUTINE
*      -> split off from FAST_TRAK 19-Aug-2002
*      -> one iteration of local ambiguity resolution
*      -> this was split off to aid in processing
*          multiple representations from _fit_wire_
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_LOC_AMB_ITERATN
C.......................................................................
C.
C. FAST_TRAK_LOC_AMB_ITERATN - one iteration of local ambiguity resolution
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : slit off from FAST_TRAK 12-Aug-2002
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
C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------
      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C fill up the STAK with possible solutions
C clear the secondary stack for the local ambiguity representaions
C-----------------------------------------------------------------------

      CALL TFSTKC(STAK_SECONDARY)

C-----------------------------------------------------------------------
C set some status bits
C-----------------------------------------------------------------------
          
      IF(D0_CONS)THEN
        IF(ITERATION.EQ.1)THEN
          CALL STATUSBITS(kSTfillcf,kstFT1D0con,1)
        ELSE
          CALL STATUSBITS(kSTfillcf,kstFT2D0con,1)
        ENDIF
      ELSE
        IF(ITERATION.EQ.1)THEN
          CALL STATUSBITS(kSTfillcf,kstFT1D0con,0)
        ELSE
          CALL STATUSBITS(kSTfillcf,kstFT2D0con,0)
        ENDIF
      ENDIF

      IF(Z0BIAS.LE.Z0BIAS_FULL)THEN
        IF(ITERATION.EQ.1)THEN
          CALL STATUSBITS(kSTfillcf,kstFT1Z0con,1)
        ELSE
          CALL STATUSBITS(kSTfillcf,kstFT2Z0con,1)
        ENDIF
      ELSE
        IF(ITERATION.EQ.1)THEN
          CALL STATUSBITS(kSTfillcf,kstFT1Z0con,0)
        ELSE
          CALL STATUSBITS(kSTfillcf,kstFT2Z0con,0)
        ENDIF
      ENDIF
      
C=====================================================================
C  resolve L-R ambiguity with local half cell stagger
C=====================================================================
      
      CALL UCOPY(KAPCF,KAPHLD,NPARTF)
      
      IF(
     1     (EXECUTE_AMBIGUITY).AND.
     2     (NUMB_OF_GROUPS.GT.0)
     3     )THEN
        
        CALL FAST_TRAK_POPULAT_AMB_GRP
        CALL FAST_TRAK_AMB_GRP_TO_TRAK
        
#if defined(CLEO_FTDIAG)
        PRINT 1003
 1003   FORMAT(
     1       ' --------------------------------------',
     2       '---------------------------------------')
        PRINT 1120,NFIT
 1120   FORMAT(' FAST_TRAK_LOC_AMB_ITERATN:',
     2       ' did ambiguity groups, standard,  NFIT=',I3)
#if defined(CLEO_XTSUBS)
        CALL XTS_TRAK('CF',0,'F_T_loc_amb_iteratn AMB GROUPS')
#endif
#endif
        
C-----------------------------------------------------------------------
C loop back for a new T0
C-----------------------------------------------------------------------
        
#if defined(CLEO_DOITLOOPBACK)
#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
        PRINT 6603
 6603   FORMAT(' FAST_TRAK: START: ,LOOP BACK FOR NEW T0?')
        CALL XTS_RFV_INPUT
        IF(XTS_ANSWER_IY.EQ.'Y')THEN
          GO TO 101
        ENDIF
#endif
#endif
        
C-----------------------------------------------------------------------
C fill in the other hits
C-----------------------------------------------------------------------
        
        IF(ITERATION.EQ.1)THEN
          CALL STATUSBITS(kSTfillcf,kstFT1altamb,0)
          CALL STATUSBITS(kSTfillcf,kstFT1altambNUL,0)
          CALL STATUSBITS(kSTfillcf,kstFT1nhitlamb,NFIT)
        ELSE
          CALL STATUSBITS(kSTfillcf,kstFT2altamb,0)
          CALL STATUSBITS(kSTfillcf,kstFT2altambNUL,0)
          CALL STATUSBITS(kSTfillcf,kstFT2nhitlamb,NFIT)
        ENDIF
        
        CALL FAST_TRAK_CLOSE_FIT_STORE
        IF(ERROR_CODE.NE.0)GO TO 910
        
C-----------------------------------------------------------------------
C  load the alternate solutions
C-----------------------------------------------------------------------
        
        IF(EXECUTE_ALTERNATE)
     2       CALL FAST_TRAK_ALTERNATE_AMB_2
        
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
        
        IF(ITERATION.EQ.1)THEN
          CALL STATUSBITS(kSTfillcf,kstFT1altamb,0)
          CALL STATUSBITS(kSTfillcf,kstFT1altambNUL,0)
          CALL STATUSBITS(kSTfillcf,kstFT1nhitlamb,0)
        ELSE
          CALL STATUSBITS(kSTfillcf,kstFT2altamb,0)
          CALL STATUSBITS(kSTfillcf,kstFT2altambNUL,0)
          CALL STATUSBITS(kSTfillcf,kstFT2nhitlamb,0)
        ENDIF
        
        CALL FAST_TRAK_CLOSE_FIT_STORE
      ENDIF
      
C-----------------------------------------------------------------------
C error if there is nothing stored
C-----------------------------------------------------------------------
      
      IF(NSTKTF(STAK_SECONDARY).LE.0)THEN
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
 303  CONTINUE
      
C-----------------------------------------------------------------------
C  do not take from stack (do not reset UPTODATE_HIT) if only one in stack
C  but do call TFPOPF if CLEO_XTSUBS because FAST_TRAK_AMB_TO_TRAK
C    is called in FAST_TRACK_ALTERNAT_AMB_2 if CLEO_XTSUBS  
C-----------------------------------------------------------------------
#if defined(CLEO_XTSUBS)
      CALL TFPOPF(STAK_SECONDARY,FIRST_NEXT,0,STAK_LOC)
      FIRST_NEXT=STAK_PICK_NEXT
#else
      IF(NSTKTF(STAK_SECONDARY).GT.1)THEN
        CALL TFPOPF(STAK_SECONDARY,FIRST_NEXT,0,STAK_LOC)
        FIRST_NEXT=STAK_PICK_NEXT
      ELSE
        STAK_LOC=1
      ENDIF
#endif
C-----------------------------------------------------------------------
C  process the representation
C-----------------------------------------------------------------------
      
      IF (STAK_LOC.GT.0) THEN
        
#if defined(CLEO_FTDIAG)
        PRINT 1003
        PRINT 1031,STAK_LOC
 1031   FORMAT(' FAST_TRAK_LOC_AMB_ITERATN:'
     2       ' start of Z fitting for solution=',I3)
        PRINT 1003
        MESLOC='hits B4rzfit'
        PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif
        
        CALL WIRES_2_HITS(LVLTYP_PAT_REC,HITTYP_HIT,FITTYP_LF)
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
 1015   FORMAT(' FAST_TRAK_LOC_AMB_ITERATN:',
     2       '[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3       ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4       ' CH=',F9.3)
 1014   FORMAT(' FAST_TRAK_LOC_AMB_ITERATN:',
     2       '[',A12,'] Y0LF=',F6.3,
     3       ' T=',F6.3,'  NHITLF=',I4) 
        CFIX(110+ITERATION)=Y0LF
#endif
        
        CALL WIRES_2_HITS(LVLTYP_PAT_REC,HITTYP_HIT,FITTYP_CF)
        CALL FIT_WHATEVER
        
#if defined(CLEO_FTDIAG)
        PRINT 1003
        MESLOC='after RF fit'
        PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
        CALL XTS_TRAK('CF',0,'F_T_loc_amb_iteratn aft RF FIT')
#endif
#endif
        
C end of the (CLEO_SFZFIT) condition
#endif
        
C-----------------------------------------------------------------------
C  must refill XFIT and YFIT for fitting; not saved in stack
C  but maybe this is obsolete if the track is not coming from the stack
C-----------------------------------------------------------------------
C            CALL WIRES_2_HITS(LVLTYP_PAT_REC,HITTYP_HIT,FITTYP_CF)
        
C-----------------------------------------------------------------------
C  remove hits with large residuals
C  argument selects mode that remove hits first 
C-----------------------------------------------------------------------
            
        CALL FAST_TRAK_DEL_BAD_AND_ADD(
     2       FT_DEL_BAD,FT_USE_CELL_FRAC)
        
C-----------------------------------------------------------------------
C  change the "Z" fit
C-----------------------------------------------------------------------
#if defined(CLEO_SFZFIT)
#if defined(CLEO_FTDIAG)
        PRINT 1003
        MESLOC='hits B4rzfit'
        PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif
        
        CALL WIRES_2_HITS(LVLTYP_PAT_REC,HITTYP_HIT,FITTYP_LF)
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
        
        CALL WIRES_2_HITS(LVLTYP_PAT_REC,HITTYP_HIT,FITTYP_CF)
#endif
        
C-----------------------------------------------------------------------
C fill in the cfid parameters
C-----------------------------------------------------------------------
        
        CALL CDCFID(CURCF,PHI0CF,D0CF,
     2       KAPCF,ALPCF,BETCF,GAMCF,XICF)
        
C-----------------------------------------------------------------------
C update here if removing large residuals first
C   there can be multiple entries from different loc amb representations
C   and entries will be ordered and popped below
C   PROBLEM: this is NOT preceded by a CF fit, 
C      so it must reset NHIT3F,CHI3F
C-----------------------------------------------------------------------
        
        NHIT3F = NFIT
        CHI3F  = CHICF
        CALL TFUSTK(STAK_SECONDARY,0)
        
#if defined(CLEO_FTDIAG)
        PRINT 1003
        MESLOC='end of reprs'
        PRINT 1701,MESLOC,CURCS,PHI0CS,D0CS,NFIT
 1701   FORMAT(' FAST_TRAK_LOC_AMB_ITERATN:',
     2       '[',A12,'] C=',F6.3,' PHI=',F6.3,
     2       ' D0=',F6.3,' NFIT=',I3)
#if defined(CLEO_XTSUBS)
        CALL XTS_TRAK_INTERRUPT_VALUE('2')
        CALL XTS_TRAK('CF',0,'F_T_loc_amb_iteratn END OF REP')
c        CALL XTS_TRAK_INTERRUPT_VALUE('-')
        CALL XTS_TRAK_INTERRUPT_VALUE('1')
#endif
#endif
        
C-----------------------------------------------------------------------
C loop back here for next representation in the stack
C-----------------------------------------------------------------------
        
        IF(NSTKTF(STAK_SECONDARY).GT.1)GO TO 303
      ENDIF
      
C-----------------------------------------------------------------------
C before starting new iteration 
C  order the stack using standard stack criteria,
C  select best, clear stack for next iteration 
C-----------------------------------------------------------------------
      
      CALL TFOSTK(STAK_SECONDARY)
      CALL TFPOPF(STAK_SECONDARY,STAK_PICK_FIRST,0,STAK_LOC)
      HAVE_SOLUTION=(STAK_LOC.GT.0)
      CALL TFSTKC(STAK_SECONDARY)
      
      IF(HAVE_SOLUTION)THEN
        CALL TFPUSH(STAK_FT_LA_OUT,STAK_LOC)
      ENDIF
      
      CALL WIRES_2_HITS(LVLTYP_PAT_REC,HITTYP_HIT,FITTYP_CF)
      
#if defined(CLEO_FTDIAG)
      PRINT 1003
      MESLOC='end iteratn '
      PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK_INTERRUPT_VALUE('2')
      CALL XTS_TRAK('CF',0,'F_T_loc_amb_iteratn at END')
c      CALL XTS_TRAK_INTERRUPT_VALUE('-')
      CALL XTS_TRAK_INTERRUPT_VALUE('1')
#endif
#endif
      
 910  CONTINUE

      RETURN
      END




