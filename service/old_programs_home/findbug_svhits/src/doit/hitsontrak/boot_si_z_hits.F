*
* $Id: boot_si_z_hits.F,v 1.7 2001/09/12 22:03:08 dpp Exp $
*
* $Log: boot_si_z_hits.F,v $
* Revision 1.7  2001/09/12 22:03:08  dpp
*      -> change statcf assignments
*
* Revision 1.6  2000/12/19 23:58:57  dpp
*      -> use parameters to control TFPOPF
*
* Revision 1.5  2000/11/15 16:54:26  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.4  2000/10/24 22:47:54  dpp
*      -> reload selected track into stak at end
*      -> set program indicator with parameter
*      -> set status bit within this program
*      -> diagnostics
*
* Revision 1.3  2000/10/05 21:57:36  dpp
*      -> block layers with hits remaining after the deletion
*      -> required for z layers with STSZFT=0 hits
*
* Revision 1.2  2000/10/03 13:27:41  dpp
* new routine submitted Oct 2, 2000 - but CVS header lines were missing $
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :
      SUBROUTINE BOOT_SI_Z_HITS(REPLACED)
C.......................................................................
C.
C. BOOTS_SI_Z_HITS - create roads for looking for z projections in 
C.                       the silicon
C.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    :
C.
C. VERSION   : 1.0
C. CREATED   : 15-Sept-2000
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

C ILLYR..............logical layer number
                     INTEGER ILLYR

C LAYMIN.............minimum layer with hits
                     INTEGER LAYMIN

C LAYMAX.............maximum layer with hits
                     INTEGER LAYMAX

C SLAYMIN............S of logical layer LAYMIN
                     REAL SLAYMIN

C SLAYMAX............S of logical layer LAYMAX
                     REAL SLAYMAX

C ILAYO..............Outer loop layer counter
                     INTEGER ILAYO

C LAYO_MIN...........outer layer minimum
                     INTEGER LAYO_MIN

C ILAYI..............Inner loop layer couter
                     INTEGER ILAYI

C LAYI_MAX...........inner layer maximum
                     INTEGER LAYI_MAX 

C IHITO..............Outer loop hit counter
                     INTEGER IHITO

C IHITI..............Inner loop hit counter
                     INTEGER IHITI

C LUSETMP............dummy argument to COMPRESS_FIT
                     LOGICAL LUSETMP(KLYRTF)

C NFIT_SAV...........save NFIT before adding Z hits 
                     INTEGER NFIT_SAV

C MESLOC.............diagnostic message giving location in program
                     CHARACTER*12 MESLOC

* ----------Executable code starts here---------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

                     
      PROGRAM_INDICATOR=P_I_BOOT_SI_Z_HITS

#if (defined(CLEO_DPPDIAG))&&(defined(CLEO_XTSUBS))
         CALL XTS_TRAK('CF',0,
     2        'BOOT_SI_Z_HITS enter')
#endif

     
C save the current track as the first stack element
C note: TFOSTK orders the stack with the following variables, in this order
C (least significant)1: NHTKEY based on NHITTF
C                    2: CHIKEY based on CHITF 
C                    3: TMPKEY based on NHTKEY, but saturated at 20
C                    4: LFHKEY based only on if NHITLF is non-zero 
         
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
      DO 111 IFITDRP=1,NFIT
         IF(STSZFT(IFITDRP).NE.0)THEN
            IADFIT(IFITDRP)=0
            LDELET=.TRUE.
         ELSE
            LAYER_BLOCK(IABS(IPLFIT(IFITDRP)))=1
         ENDIF
 111  CONTINUE


      IF(LDELET)THEN
         NHITLF=0
         CALL COMPRESS_FIT(LUSETMP)
         CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_CF)
         CALL FIT_WHATEVER
#if (defined(CLEO_DPPDIAG))&&(defined(CLEO_XTSUBS))
         CALL XTS_TRAK('CF',0,
     2        'BOOT_SI_Z_HITS del Z hits')
#endif
      ENDIF

C get a fresh set of z hits
      CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)

C for now, find the outermost and innermost layers with hits
      IF(NLYRHT.GE.2)THEN
         LAYMIN=0
         SLAYMIN=999.
         LAYMAX=0
         SLAYMAX=0.
         DO 211 ILLYR=1,NLYRHT
            IF(SLYRLR(ILLYR).LT.SLAYMIN)THEN
               LAYMIN=ILLYR
               SLAYMIN=SLYRLR(ILLYR)
            ENDIF
            IF(SLYRLR(ILLYR).GT.SLAYMAX)THEN
               LAYMAX=ILLYR
               SLAYMAX=SLYRLR(ILLYR)
            ENDIF
 211     CONTINUE
         
         
         NFIT_SAV=NFIT
         Z0BIAS=.02

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C loop over assignment of the outer layer number
         LAYO_MIN=LAYMIN+1
         DO 249 ILAYO=LAYMAX,LAYO_MIN,-1
           IF(LAYER_BLOCK(IPLRLR(ILAYO)).EQ.0)THEN

C loop over assignment of the inner layer number
            LAYI_MAX=ILAYO-1
            DO 239 ILAYI=LAYMIN,LAYI_MAX
              IF(LAYER_BLOCK(IPLRLR(ILAYI)).EQ.0)THEN
C outer loop over outer layer hits
               DO 229 IHITO=LHITLR(ILAYO),MHITLR(ILAYO)
                  IF(STSZHT(IHITO).GT.0)THEN
C inner loop over inner layer hits
                     DO 219 IHITI=LHITLR(ILAYI),MHITLR(ILAYI)
                        IF(STSZHT(IHITI).GT.0)THEN
C demand that there is room in the stack
                           IF(NSTKTF(SECONDARY_STAK).LT.NSTKDM)THEN
                              
                              NFIT=NFIT_SAV
                              CALL HIT_TO_FIT(H2F_NEW_ENTRY,IHITO,0)
                              CALL HIT_TO_FIT(H2F_NEW_ENTRY,IHITI,0)
                              CALL FIT_WHATEVER
                              NHITTF = NFIT
                              NHITCF = NFIT_SAV
                              NHITLF = 2
                              CHITF  = 100.+CHILF
                              PHI7CF = 0
                              RKN7CF = 9.0
                              CALL TFPUSH(SECONDARY_STAK)
                              
                           ENDIF
                        ENDIF
 219                 CONTINUE
                  ENDIF
 229           CONTINUE
              ENDIF
 239        CONTINUE
           ENDIF
 249     CONTINUE
               

C loop over the stack entries (new entries only), use TFXFIT
         ISTKTF(SECONDARY_STAK)=ISTKTF_SAVE
 321     CONTINUE
         CALL TFPOPF(SECONDARY_STAK,STAK_PICK_NEXT,0)
         IF (ISTKTF(SECONDARY_STAK).GT.0) THEN
            NFIT_SAV=NFIT
            CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
            CALL TFXFIT
            IF(NFIT.GT.NFIT_SAV)THEN
               NHITTF = NFIT
               NHITCF = NFIT_SAV
               NHITLF = NFIT-NFIT_SAV
               CHITF  = CHILF
               CALL TFUSTK(SECONDARY_STAK,0)
            ENDIF
            GO TO 321
         ENDIF

         
         CALL TFOSTK(SECONDARY_STAK)
         CALL TFPOPF(SECONDARY_STAK,STAK_PICK_FIRST,0)
         REPLACED=(ISTKTF(SECONDARY_STAK).NE.ISTKTF_SAVE)
         IF(REPLACED)CALL SBYT(1,STATCF(3),4,1)

#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK_INTERRUPT_VALUE('6')
#endif
#if defined(CLEO_DPPDIAG)
         PRINT 1003
         MESLOC='BOOT selects'
         PRINT 1101,MESLOC,
     2        ISTKTF(SECONDARY_STAK),ISTKTF_SAVE,REPLACED
         PRINT 1015,MESLOC,
     2        KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
         PRINT 1014,MESLOC,
     2        Y0LF,TANDLF,NHITLF
#endif

         CALL ROADHITS(USE_TFXFIT,USE_HIT,USE_LF)
         CALL FIT_WHATEVER

         CALL TFSTKC(SECONDARY_STAK)
         NHITTF = NFIT
         NHITCF = NFIT_SAV
         CHITF  = CHICF
         PHI7CF = 0
         RKN7CF = 9.0
         CALL TFPUSH(SECONDARY_STAK)
      ENDIF

#if defined(CLEO_DPPDIAG)
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
       PRINT 1003
 1003  FORMAT(
     1' --------------------------------------------------------------')
 1015 FORMAT(
     2 ' BOOTSI:[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3 ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4 ' CH=',F9.3)
 1014 FORMAT(
     2 ' BOOTSI:[',A12,'] Y0LF=',F6.3,' T=',F6.3,'  NHITLF=',I4) 
 1101 FORMAT(
     2 ' BOOTSI:[',A12,'] ISTKTF=',I3,'  "save"=',I3,'  REPLACED=',L1)

      PRINT 1003
      MESLOC='hitsAFTrzfit'
      PRINT 1015,MESLOC,
     2     KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
      PRINT 1014,MESLOC,Y0LF,TANDLF,NHITLF
#endif

      RETURN
      END
      


