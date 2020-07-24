*
* $Id: replace_z_hits.F,v 1.9 2002/09/05 18:57:26 dpp Exp $
*
* $Log: replace_z_hits.F,v $
* Revision 1.9  2002/09/05 18:57:26  dpp
*      -> Save "deleted z hits" representation in temporary stack
*         instead of saving the three starting points in the secondary stack.
*         This is required after removing the LF hit ordering in the stack;
*         without LF ordering, the best representation could be
*         a "deleted z hits" representation.
*
* Revision 1.8  2002/05/23 14:23:09  dpp
*      -> consolidate unnecessary line
*
* Revision 1.7  2001/11/19 23:50:41  dpp
*      -> delete unused include file
*      -> add new control parameter include file
*      -> remove filling NHITCF,PHI7CF,RKN7CF before STAK operation
*      -> do an SZ fit on input track representation (moved from DUFIND)
*      -> store the chisq for ordering as CHILF
*      -> do an SZ fit on each of the 3 best solutions
*      -> use new argument for TFPOPF,TFPUSH
*      -> now exits with CF hits rather than LF
*      -> remove extra Z roadhits and fit that had come after popping best
*
* Revision 1.6  2000/12/19 23:58:35  dpp
*      -> use parameters to control TFPOPF
*      -> now clear secondary stack internally
*
* Revision 1.5  2000/11/15 16:58:05  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.4  2000/10/24 22:48:26  dpp
*      -> set program indicator with parameter
*      -> block layers with hits remaining after the deletion
*      -> required for z layers with STSZFT=0 hits to avoid double count
*
* Revision 1.3  2000/10/02 20:45:40  dpp
*      -> new argument identifies if hits have been replaced
*      -> set PROGRAM_INDICATOR
*
* Revision 1.2  2000/04/26 14:57:36  dpp
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*
* Revision 1.1  2000/01/24 19:57:37  dpp
*      -> save the input z solution
*      -> call scan_z_roads to generate posssible alternative solutions
*      -> select best solution from stack
*      -> uses LOCAL_USE_STACK which it leaves clear and expects to find clear
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :
      SUBROUTINE REPLACE_Z_HITS(REPLACED)
C.......................................................................
C.
C. REPLACE_Z_HITS - replce z hits in track if better solution found
C.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    :
C.
C. VERSION   : 1.0
C. CREATED   : 14-Jan-2000
C. LAST MOD  : 
C.
C. Modification Log.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
c#include "doit/duseq/tfstakcd.inc"
#include "doit/duseq/fast_trak.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

C REPLACED...........=.TRUE. if new Z hits replace the input
                     LOGICAL REPLACED

C STAK_LOC...........stack location index returned from TFPOPF
                     INTEGER STAK_LOC

C STAK_LOC_SAV.......save the stack location of input representation
C                    this is the start of the generated  representations
                     INTEGER STAK_LOC_SAV

C LDELET.............=.TRUE. if Z hits have been deleted
                     LOGICAL LDELET

C IFITDRP............index to loop over the FIT
                     INTEGER IFITDRP

C IROAD..............index to loop over roads
                     INTEGER IROAD

C LUSETMP............dummy argument to COMPRESS_FIT
                     LOGICAL LUSETMP(KLYRTF)

C NFIT_SAV...........save NFIT before adding Z hits in SCAN_Z_ROADS 
                     INTEGER NFIT_SAV

C MESLOC.............diagnostic message giving location in program
                     CHARACTER*12 MESLOC

C DZ_TEST............input TANDLF to use in ROADHITS
                     REAL DZ_TEST(3)
                     DATA DZ_TEST/-1.7,0.,1.7/


* ----------Executable code starts here---------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

                     
      PROGRAM_INDICATOR=P_I_REPLACE_Z_HITS

C-----------------------------------------------------------------------
C clear the stack and
C save the input represenstation         
C last fit was an LF fit in DUFIND so ...
C  the number of hits and chi2 reflect that LF fit
C-----------------------------------------------------------------------

      CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_LF)
      CALL FIT_WHATEVER
      CALL TFSTKC(STAK_SECONDARY)
      CALL TFPUSH(STAK_SECONDARY,STAK_LOC_SAV)


      LDELET=.FALSE.
      CALL VZERO(LAYER_BLOCK,KLYRTF)
      DO 411 IFITDRP=1,NFIT
         IF(STSZFT(IFITDRP).NE.0)THEN
            IADFIT(IFITDRP)=0
            LDELET=.TRUE.
         ELSE
            LAYER_BLOCK(IABS(IPLFIT(IFITDRP)))=1
         ENDIF
 411  CONTINUE

C-----------------------------------------------------------------------
C NOTE: this could drop the number of CF hits below the limit required
C    for a CF fit; MNHTCF=4. In this case, the fitter will not change
C    the track parameters but will update NHITCF. Must rely on limits
C    placed in SCAN_Z_ROADS to provide enough hits to fit.
C    SCAN_Z_ROADS requires 5 hits in drift chamber 
C    so MNHTCF requirement will be satisfied.
C
C NOTE: TFOSTK no longer used the old number of LF hits ordering; must 
C    force the number of hits used in orderinfg to be =0 in initial set-up
C OR, do not save the starting representations which do not yield a fit
C-----------------------------------------------------------------------

      IF(LDELET)THEN
         NHITLF=0
         CALL COMPRESS_FIT(LUSETMP)
         CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
         CALL FIT_WHATEVER
#if (defined(CLEO_DPPDIAG))&&(defined(CLEO_XTSUBS))
         CALL XTS_TRAK('CF',0,
     2        'REPLACE_Z_HITS fit aft delet Z hits')
#endif
      ENDIF

      CALL TFSTKC(STAK_TMP1)
      CALL TFPUSH(STAK_TMP1,STAK_LOC)


      DO 439 IROAD=1,3
         CALL TFPOPF(STAK_TMP1,STAK_PICK_FIRST,0,STAK_LOC)
         NFIT_SAV=NFIT
         Y0LF=0.
         TANDLF=DZ_TEST(IROAD)
         CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_LF)
         CALL SCAN_Z_ROADS
         IF(NFIT.GT.NFIT_SAV)THEN
            CALL FIT_WHATEVER
            CALL TFPUSH(STAK_SECONDARY,STAK_LOC)
         ENDIF
 439  CONTINUE
      
      CALL TFOSTK(STAK_SECONDARY)
      CALL TFPOPF(STAK_SECONDARY,STAK_PICK_FIRST,0,STAK_LOC)
      REPLACED=(STAK_LOC.NE.STAK_LOC_SAV)
      CALL TFSTKC(STAK_SECONDARY)
      CALL TFSTKC(STAK_TMP1)

#if defined(CLEO_DPPDIAG)
      PRINT 1003
      MESLOC='hitsAFTselect'
      PRINT 1015,MESLOC,
     2     KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
      PRINT 1014,MESLOC,Y0LF,TANDLF,NHITLF
#endif
      
      CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
      CALL FIT_WHATEVER
C NOTE: there will be another LF fit before the end of the loop

      
#if defined(CLEO_DPPDIAG)
      PRINT 1003
      MESLOC='after RF fit'
      PRINT 1015,MESLOC,
     2     KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,
     2     'REPLACE_Z_HITS after RF FIT')
#endif
#endif
      RETURN

#if defined(CLEO_DPPDIAG)
 1003 FORMAT(
     1' --------------------------------------------------------------')
 1015 FORMAT(
     2 ' REPLAC:[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3 ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4 ' CH=',F9.3)
 1014 FORMAT(
     2 ' REPLAC:[',A12,'] Y0LF=',F6.3,' T=',F6.3,'  NHITLF=',I4) 
#endif

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      END




