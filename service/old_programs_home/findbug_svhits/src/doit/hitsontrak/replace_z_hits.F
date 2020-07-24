*
* $Id: replace_z_hits.F,v 1.6 2000/12/19 23:58:35 dpp Exp $
*
* $Log: replace_z_hits.F,v $
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
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfstakcd.inc"
#include "doit/duseq/fast_trak.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

C REPLACED...........=.TRUE. if new Z hits replace the input
                     LOGICAL REPLACED

C ISTKTF_SAVE........save the stack location of input representation
C                    this is the start of the generated  representations
                     INTEGER ISTKTF_SAVE

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

C clear the stack and
C save the input represenstation         

      CALL TFSTKC(SECONDARY_STAK)
      NHITCF = NFIT
      NHITTF = NHITCF
      CHITF  = CHICF
      PHI7CF = 0
      RKN7CF = 9.0
      CALL TFPUSH(SECONDARY_STAK)
      ISTKTF_SAVE=ISTKTF(SECONDARY_STAK)


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


      IF(LDELET)THEN
         NHITLF=0
         CALL COMPRESS_FIT(LUSETMP)
         CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)
         CALL FIT_WHATEVER
#if (defined(CLEO_DPPDIAG))&&(defined(CLEO_XTSUBS))
         CALL XTS_TRAK('CF',0,
     2        'REPLACE_Z_HITS fit aft delet Z hits')
#endif
      ENDIF

      DO 429 IROAD=1,3
         Y0LF=0.
         TANDLF=DZ_TEST(IROAD)
         
         NHITCF = NFIT
         NHITLF = 0
         NHITTF = NHITCF
         CHITF  = CHICF
         PHI7CF = 0
         RKN7CF = 9.0
         CALL TFPUSH(SECONDARY_STAK)
 429  CONTINUE

      ISTKTF(SECONDARY_STAK)=ISTKTF_SAVE
      DO 439 IROAD=1,3
         CALL TFPOPF(SECONDARY_STAK,STAK_PICK_NEXT,0)
         NFIT_SAV=NFIT
         CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
         CALL SCAN_Z_ROADS
         IF(NFIT.GT.NFIT_SAV)THEN
            NHITCF = NFIT
            NHITTF = NHITCF
            NHITLF = NFIT-NFIT_SAV
            CALL TFUSTK(SECONDARY_STAK,0)
         ENDIF
 439  CONTINUE
      
      CALL TFOSTK(SECONDARY_STAK)
      CALL TFPOPF(SECONDARY_STAK,STAK_PICK_FIRST,0)
      REPLACED=(ISTKTF(SECONDARY_STAK).NE.ISTKTF_SAVE)
      CALL TFSTKC(SECONDARY_STAK)
      CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
      CALL FIT_WHATEVER

#if defined(CLEO_DPPDIAG)
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      PRINT 1003
 1003 FORMAT(
     1' --------------------------------------------------------------')
 1015 FORMAT(
     2 ' REPLAC:[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3 ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4 ' CH=',F9.3)
 1014 FORMAT(
     2 ' REPLAC:[',A12,'] Y0LF=',F6.3,' T=',F6.3,'  NHITLF=',I4) 

      PRINT 1003
      MESLOC='hitsAFTrzfit'
      PRINT 1015,MESLOC,
     2     KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
      PRINT 1014,MESLOC,Y0LF,TANDLF,NHITLF
#endif
      
      CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)
      CALL FIT_WHATEVER

      
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

      CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
      RETURN
      END
      


