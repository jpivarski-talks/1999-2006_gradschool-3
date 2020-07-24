*
* $Id: boot_si_z_hits.F,v 1.10 2003/03/05 18:01:53 dpp Exp $
*
* $Log: boot_si_z_hits.F,v $
* Revision 1.10  2003/03/05 18:01:53  dpp
*      -> comment
*
* Revision 1.9  2002/05/23 14:23:36  dpp
*     -> consolidate unnecessary line
*
* Revision 1.8  2001/11/19 23:50:26  dpp
*      -> use argument in call of TFXFIT
*      -> delete unused include file
*      -> add new control parameter include file
*      -> require enough remaining r-phi hits after deleting z hits
*      -> remove filling NHITCF,PHI7CF,RKN7CF before STAK operation
*     **> store the input representation with CHIKEY=CHILF; not CHICF and
*         remove the funny +100 on CHITF (=CHILF) when storing intial roads
* 	I guess that the +100 was originally put in to give preference
* 	to the input representation in the case that no new representation
* 	had more hits. The selection was confused because the input
* 	representation was stored in the stack with chiCf while the
* 	other representations were stored with chiLf and with a
* 	Z0 bias of 0.02 .
*      -> use new argument for TFPOPF,TFPUSH
*     **> require layer to be silicon to be used as seed
*     **> do an r-phi fit, followed by another s-z fit for replaced tracks
*
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
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/statusbits.inc"
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
C STAK_LOC...........stack location index returned from TFPOPF
                     INTEGER STAK_LOC
C STAK_LOC_SAV.......save the stack location of input representation
C                    this is the start of the generated  representations
                     INTEGER STAK_LOC_SAV
C LDELET.............=.TRUE. if Z hits have been deleted
                     LOGICAL LDELET
C NDELET.............number of hits deleted from fit
                     INTEGER NDELET
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
C LAYO_MIN...........outer layer minimum
                     INTEGER LAYO_MIN
C ILAYO..............Outer loop layer counter
                     INTEGER ILAYO
C IPLRLRO............outer physical layer
                     INTEGER IPLRLRO
C LAYI_MAX...........inner layer maximum
                     INTEGER LAYI_MAX 
C ILAYI..............Inner loop layer counter
                     INTEGER ILAYI
C IPLRLRI............inner physical layer
                     INTEGER IPLRLRI
C IHITO..............Outer loop hit counter
                     INTEGER IHITO
C IHITI..............Inner loop hit counter
                     INTEGER IHITI
C LUSETMP............dummy argument to COMPRESS_FIT
                     LOGICAL LUSETMP(KLYRTF)
C NFIT_SAV...........save NFIT before adding Z hits 
                     INTEGER NFIT_SAV
C TFXFIT_ERR.........error returned from TFXFIT
                     INTEGER TFXFIT_ERR
C MESLOC.............diagnostic message giving location in program
                     CHARACTER*12 MESLOC

* ----------Executable code starts here---------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

                     
      PROGRAM_INDICATOR=P_I_BOOT_SI_Z_HITS

#if (defined(CLEO_DPPDIAG))&&(defined(CLEO_XTSUBS))
      CALL XTS_TRAK('CF',0,'BOOT_SI_Z_HITS enter')
#endif

     
C-----------------------------------------------------------------------
C save the current track as the first stack element
C it is expected that the last fit was LF
C
C note: TFOSTK orders the stack with the following variables, in this order
C (least significant)1: NHTKEY based on number of hits in last fit
C                    2: CHIKEY based on chisq in last fit
C                    3: TMPKEY based on NHTKEY, but saturated at 20
C                    4: LFHKEY based only on if NHITLF is non-zero 
C-----------------------------------------------------------------------
         
      CALL TFSTKC(STAK_SECONDARY)
      CALL TFPUSH(STAK_SECONDARY,STAK_LOC_SAV)
      REPLACED=.FALSE.

      LDELET=.FALSE.
      NDELET=0
      CALL VZERO(LAYER_BLOCK,KLYRTF)
      DO 111 IFITDRP=1,NFIT
        IF(STSZFT(IFITDRP).NE.0)THEN
          IADFIT(IFITDRP)=0
          LDELET=.TRUE.
          NDELET=NDELET+1
        ELSE
          LAYER_BLOCK(IABS(IPLFIT(IFITDRP)))=1
        ENDIF
 111  CONTINUE

C-----------------------------------------------------------------------
C protect against condition where not enough hits are remain for CF fit
C-----------------------------------------------------------------------
      IF((NFIT-NDELET).GE.MNHTCF)THEN

        IF(LDELET)THEN
          NHITLF=0
          CALL COMPRESS_FIT(LUSETMP)
          CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
          CALL FIT_WHATEVER
#if (defined(CLEO_DPPDIAG))&&(defined(CLEO_XTSUBS))
          CALL XTS_TRAK('CF',0,'BOOT_SI_Z_HITS del Z hits')
#endif
        ENDIF

C get a fresh set of z hits
        CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_LF)

C find the outermost and innermost layers with hits
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
 211      CONTINUE
         
         
          NFIT_SAV=NFIT
          Z0BIAS=.02

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C loop over assignment of the outer layer number
          LAYO_MIN=LAYMIN+1
          DO 249 ILAYO=LAYMAX,LAYO_MIN,-1
            IPLRLRO=IPLRLR(ILAYO)
            IF(
     1           (LAYER_BLOCK(IPLRLRO).EQ.0).AND.
     2           (IDVCTF(IPLRLRO).EQ.ISVZ1))THEN
C loop over assignment of the inner layer number
              LAYI_MAX=ILAYO-1
              DO 239 ILAYI=LAYMIN,LAYI_MAX
                IPLRLRI=IPLRLR(ILAYI)
                IF(
     1               (LAYER_BLOCK(IPLRLRI).EQ.0).AND.
     2               (IDVCTF(IPLRLRI).EQ.ISVZ1))THEN
C outer loop over outer layer hits
                  DO 229 IHITO=LHITLR(ILAYO),MHITLR(ILAYO)
                    IF(STSZHT(IHITO).GT.0)THEN
C inner loop over inner layer hits
                      DO 219 IHITI=LHITLR(ILAYI),MHITLR(ILAYI)
                        IF(STSZHT(IHITI).GT.0)THEN
                          IF(NSTKTF(STAK_SECONDARY).LT.NSTKDM)THEN
C proceed when there is room in the stack
                            NFIT=NFIT_SAV
                            CALL HIT_TO_FIT(H2F_NEW_ENTRY,IHITO,0)
                            CALL HIT_TO_FIT(H2F_NEW_ENTRY,IHITI,0)
                            CALL FIT_WHATEVER
                            CALL TFPUSH(STAK_SECONDARY,STAK_LOC)
                          ENDIF
                        ENDIF
 219                  CONTINUE
                    ENDIF
 229              CONTINUE
                ENDIF
 239          CONTINUE
            ENDIF
 249      CONTINUE
               

C loop over the stack entries (new entries only), use TFXFIT
          CALL TFSTKL(STAK_SECONDARY,STAK_LOC_SAV)
 321      CONTINUE
          CALL TFPOPF(STAK_SECONDARY,STAK_PICK_NEXT,0,STAK_LOC)
          IF (STAK_LOC.GT.0) THEN
            NFIT_SAV=NFIT
            CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_LF)
            CALL TFXFIT(TFXF_WIDE_STAND,TFXFIT_ERR)
            IF(NFIT.GT.NFIT_SAV)THEN
              CALL TFUSTK(STAK_SECONDARY,0)
            ENDIF
            GO TO 321
          ENDIF
            
C pop the best result         
          CALL TFOSTK(STAK_SECONDARY)
          CALL TFPOPF(STAK_SECONDARY,STAK_PICK_FIRST,0,STAK_LOC)
          REPLACED=(STAK_LOC.NE.STAK_LOC_SAV)
          IF(REPLACED)CALL STATUSBITS(kSTfillcf,kstBSZHboot,1)
          
#if defined(CLEO_XTSUBS)
          CALL XTS_TRAK_INTERRUPT_VALUE('6')
#endif

#if defined(CLEO_DPPDIAG)
          PRINT 1003
          MESLOC='BOOT selects'
          PRINT 1101,MESLOC,
     2         STAK_LOC,STAK_LOC_SAV,REPLACED
          PRINT 1015,MESLOC,
     2         KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
          PRINT 1014,MESLOC,
     2         Y0LF,TANDLF,NHITLF
#endif
          
C get a new r-phi fit followed by returning to the s-z fit
          IF(REPLACED)THEN
            CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_CF)
            CALL FIT_WHATEVER
            CALL ROADHITS(LVLTYP_TFXFIT,HITTYP_HIT,FITTYP_LF)
            CALL FIT_WHATEVER
          ENDIF
          
C save this as the only element in the stack
          CALL TFSTKC(STAK_SECONDARY)
          CALL TFPUSH(STAK_SECONDARY,STAK_LOC)
C end of condition that there are enough z layers
        ENDIF
C end of condition that there are enough r-phi hits
      ENDIF

#if defined(CLEO_DPPDIAG)
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      PRINT 1003
 1003 FORMAT(
     1' --------------------------------------------------------------')
 1015 FORMAT(
     2     ' BOOTSI:[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3     ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4     ' CH=',F9.3)
 1014 FORMAT(
     2 ' BOOTSI:[',A12,'] Y0LF=',F6.3,' T=',F6.3,'  NHITLF=',I4) 
 1101 FORMAT(
     2 ' BOOTSI:[',A12,'] STAK_LOC=',I3,'  "save"=',I3,'  REPLACED=',L1)

      PRINT 1003
      MESLOC='hitsAFTrzfit'
      PRINT 1015,MESLOC,
     2     KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
      PRINT 1014,MESLOC,Y0LF,TANDLF,NHITLF
#endif

      RETURN
      END






