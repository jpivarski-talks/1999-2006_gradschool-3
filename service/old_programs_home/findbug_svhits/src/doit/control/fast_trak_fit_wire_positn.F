* $Id: fast_trak_fit_wire_positn.F,v 1.11 2001/09/12 21:57:19 dpp Exp $
*
* $Log: fast_trak_fit_wire_positn.F,v $
* Revision 1.11  2001/09/12 21:57:19  dpp
*      -> expand use of program indicator
*
* Revision 1.10  2000/12/19 23:44:29  dpp
*      -> actively insert zeros in status words for compatibility with recall
*      -> no longer insert wire fit chisq into status words
*
* Revision 1.9  2000/10/02 20:56:55  dpp
*      -> new use of STAT words
*
* Revision 1.8  2000/08/18 18:44:53  dpp
*      -> change kink in seed track to use DOIT scattering definitions
*      -> uses new possible assignments of KNK6CF; add FTK6CF,SKN6CF
*      -> call FAST_TRAK_CHECK_HIT_LAYRS to determine D0_CONSTRAIN
*         to address problem of tracks with only two stereo super layers
*         fitting to a larger curvature rather than a larger Z
*      -> saturate TANDLF
*
* Revision 1.7  2000/05/17 19:46:49  dpp
*      -> remove unprotected print statement
*
* Revision 1.6  2000/04/26 14:38:26  dpp
*      -> set value of D0_CONSTRAIN
*      -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*
* Revision 1.5  2000/02/12 20:20:02  dpp
*      -> format statements
*
* Revision 1.4  2000/01/24 19:53:16  dpp
*      -> remove unneeded comments
*
* Revision 1.3  2000/01/06 22:12:55  dpp
*  fix error handing
*
* Revision 1.2  1999/12/09 21:06:27  dpp
* change call to hit_to_fit to use parameter directive
* zero the third argument to HIT_TO_FIT(H2F_NEW_ENTRY...
*
* Revision 1.1  1999/11/03 22:43:45  dpp
* new
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_FIT_WIRE_POSITN
C.......................................................................
C.
C. FAST_TRAK_FIT_WIRE_POSITN - fit to the wire position with time information
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split off of FAST_TRAK 27-Oct-99
C. LAST MOD  : 
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfstakcd.inc"

#include "doit/duseq/fast_trak.inc"


C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

C INDX_HIT.............counter of hit number
                       INTEGER INDX_HIT
C LAYR_COUNTR..........serial layer counter
                       INTEGER LAYR_COUNTR

C-----------------------------------------------------------------------
C other local variables
C-----------------------------------------------------------------------

C TF_LAYR..............layer number found
                       INTEGER TF_LAYR
C MTEMP................temporary variable for displaying cut limit
                       INTEGER MTEMP
C MESLOC...............message giving location in program
                       CHARACTER*12 MESLOC
C IY...................accepted variable for pauses
                       INTEGER IY
C ICHICF...............integerized value of CHICF
                       INTEGER ICHICF

C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

      PROGRAM_INDICATOR=P_I_F_T_FIT_WIRE_POSITN

C-----------------------------------------------------------------------
C test on number of layers with hits
C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG)
      MTEMP=NHIT_MIN_TRAK
      PRINT 1003
 1003 FORMAT(
     1' --------------------------------------------------------------')
      PRINT 1004,NLYRWR,MTEMP,NHTWIR
 1004 FORMAT(
     2 ' FT_F_W_P:NLYRWR=',I2,' (hit layers) req',I3,' NHTWIR=',I3)
c      CALL DSF_RFV_INPUT(IY)
#endif

      IF(NLYRWR.LT.NHIT_MIN_TRAK)THEN
         ERROR_CODE=1
         GO TO 990
      ENDIF

C-----------------------------------------------------------------------
C  found enough hit layers to fit a track with wire positions
C  transfer into the /TFHITSCD/ arrays; use wire positions ONLY
C  HWSTAT will be valid for hits if no wires are eliminated in this
C         routine and if there is a 1-to-1 correlation from wires to hits
C  HWSTAT - hit status, 0=isolated, 1=isolated doublet, 2=not isolated 
C  WIRES_2_HITS will correct wire position for "Z" at the layer radius
C
C  transfer into the /TFTRAKCD/ arrays, use wire positions
C    Y0LS and TANDLS must be set in SFNEXT
C  saturate the TANDLF at the maximum allowed for particles from origin
C  /TFHITSCD/ versions of the track parameters; xxxxCS, are no longer
C     used beyond this point
C
C  test on number of layers with hits
C-----------------------------------------------------------------------

      CALL UCOPY(KAPCS, KAPCF, NPARTF )
      IF(ABS(TANDLF).GT.DZRAIL)TANDLF=SIGN(DZRAIL,TANDLF)
      CALL WIRES_2_HITS(USE_PAT_REC,USE_WIRE,USE_CF)



#if defined(CLEO_FTDIAG)
      MTEMP=MAX0(MNHTCF,NHIT_MIN_TRAK)
      PRINT 1003
      PRINT 1005,NLYRHT,MTEMP,NHIT
 1005 FORMAT(
     2 ' FT_F_W_P:NLYRHT=',I2,' (hit layers) req',I3,'   NHIT=',I3)
      CFIX(110)=Y0LF
      ZTMP=Y0LF+.3*TANDLF
#endif

      IF(NLYRHT.LT.MAX0(MNHTCF,NHIT_MIN_TRAK))THEN
         ERROR_CODE=2
         GO TO 990
      ENDIF

C-----------------------------------------------------------------------
C  formerly select hits in layers with only one hit
C  now select hits that are isolated or isolated doublets
C  test on number of layers with only one hit
C  require RKN6CF set to evaluate number of hits before/after potential kink
C-----------------------------------------------------------------------

      RKN6CF=RAD_SCAT(2)

      CALL HIT_TO_FIT(H2F_ZERO_FIT,0,0)
      HITBSC=0
      HITASC=0
      DO 79 LAYR_COUNTR=1,NLYRHT
         TF_LAYR=IPLRLR(LAYR_COUNTR)
         
         DO 77 INDX_HIT=LHITLR(LAYR_COUNTR),MHITLR(LAYR_COUNTR)

C select isolated hits and isolated doublets
            IF(HWSTAT(INDX_HIT).LE.1)THEN
C select isolated hits only
C           IF(HWSTAT(INDX_HIT).LE.0)THEN

               IF(RTF(TF_LAYR).LT.RKN6CF)THEN
                  HITBSC=HITBSC+1
               ELSE
                  HITASC=HITASC+1
               ENDIF
               CALL HIT_TO_FIT(H2F_NEW_ENTRY,INDX_HIT,0)
            ENDIF
            
 77      CONTINUE
 79   CONTINUE

#if defined(CLEO_FTDIAG)
      MTEMP=MNHTCF
      PRINT 1003
      PRINT 1006,NFIT,MTEMP
 1006 FORMAT(
     2 ' FT_F_W_P:  NFIT=',I3,' (1 hit lays) req',I3)
c      CALL DSF_RFV_INPUT(IY)
#endif

      IF(NFIT.LT.MNHTCF)THEN
         ERROR_CODE=3
         GO TO 990
      ENDIF

C-----------------------------------------------------------------------
C  CU/PHI/DA & KABG filled in CS in SFNEXT, move from CS to CF above
C  fit to the wire positions; weight, WGTFIT, is cell/root12
C  CFDFIT works direcly in CF variables
C  CFDFIT constrains D0CF (or GAMCF) to be =0, hence XICF=1.
C  if the curvature is small use CFCFIT, instead of CFDFIT, to allow
C    DADR to change. KAPCF, etc, will be recalculated, not iterative
C  CFRFIT calls CFCFIT and calculates PULFIT, CFDFIT calculates PULFIT itself
C
C  NOTE: REGARDING STEREO WIRE FITTING, 
C        CFDFIT fits to {XWCD,YWCD} & DFIT, not {XFIT,YFIT}
C
C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG)
      MESLOC='  start     '
      PRINT 1003
      PRINT 1013,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF
      PRINT 1014,MESLOC,Y0LF,TANDLF
 1013 FORMAT(
     2 ' FT_F_W_P:[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3 ' G=',F9.6,' X=',F6.3,' F6=',F9.6)
 1015 FORMAT(
     2 ' FT_F_W_P:[',A12,'] KCF=',F6.3,' A=',F6.3,' B=',F6.3,
     3 ' G=',F9.6,' X=',F6.3,' F6=',F9.6,
     4 ' CH=',F9.3)
 1014 FORMAT(
     2 ' FT_F_W_P:[',A12,'] Y0LF=',F6.3,' T=',F6.3) 
#endif

      D0_CONSTRAIN=.TRUE.
      CALL FIT_WHATEVER
      CALL SBYT(NFIT,STATCF(1),18,6)
     
#if defined(CLEO_FTDIAG)
      PRINT 1003
      MESLOC='D0 constrain'
      PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,' FAST_TRAK aft D0 constrn fit')
#endif
#endif


C-----------------------------------------------------------------------
C test if D0 constraint can be removed
C and, if allowed, refit with floating D0
C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG)
      PRINT 1003
      PRINT 1016,KAPCF,NFIT
 1016 FORMAT(' FT_F_W_P: float D0 fit; require'/
     1       '           KAPCF =',F6.3,   '(?<1.5)',
     2                '  NFIT  =',I4,  2X,'( ?>/=10  )')
#endif

      D0_CONSTRAIN=
     1     (NFIT.LT.        10).OR.
     2     (ABS(KAPCF).GT. 1.5)

      IF(.NOT.D0_CONSTRAIN)THEN

#if defined(CLEO_FTDIAG)
         PRINT 1018
 1018    FORMAT(' FT_F_W_P:',
     2        ' so far,so good to float D0, check hit layers') 
#endif

         CALL FAST_TRAK_CHECK_HIT_LAYRS

      ENDIF

C remove the D0 constraint
      IF(.NOT.D0_CONSTRAIN)THEN
         CALL SBYT(1,STATCF(1),24,1)

#if defined(CLEO_FTDIAG)
      PRINT 1019
 1019 FORMAT(' FT_F_W_P: procede with floating D0 fit') 
#endif
C-----------------------------------------------------------------------
C test if there is a scatter (still using the wire position fit
C if the scatter is small, remove it; return to 3 parameter fit
C-----------------------------------------------------------------------

cccccccccccccccccccccc temp ccccccccccccccccccccccccccc
         LKN6TF=.TRUE.
         LKNPRC=.FALSE.
ccccccccccccccccccccccccccccccccccccccccccccccccccccccc

#if defined(CLEO_FTDIAG)
         PRINT 1003
         PRINT 1017,KAPCF,LKN6TF,LKNPRC,HITBSC,HITASC
 1017    FORMAT(
     1        ' FT_F_W_P: test of scatter; require'/
     2        '           KAPCF =',F6.3,   '( .1<k<1.5)'/
     3        '           LKN6TF=',L1,  5X,'( ?=T     )',
     4                  ' LKNPRC=',L1,  5X,'( ?=T     )'/
     5        '           HITBSC=',I4,  2X,'( ?>/=4   )',
     6                  ' HITASC=',I4,  2X,'( ?>/=4   )')
#endif

         IF(
     1        LKN6TF .AND. 
     2        LKNPRC .AND. 
     3        (HITBSC.GE.       4).AND. 
     4        (HITASC.GE.       4).AND. 
     5        (ABS(KAPCF).GT. 0.1))THEN

C kink in kappa:KNK6CF=KABG2_CURV,  kink in  r-phi:KNK6CF=KABG2_PHI
            KNK6CF=KABG2_PHI
            FTK6CF=KABG2_FLOAT
            SKN6CF=R_LEN_SCAT(2)

            CALL FIT_WHATEVER
            CALL SBYT(1,STATCF(1),25,1)

#if defined(CLEO_FTDIAG)
            PRINT 1003
            MESLOC='4FIT W/ KINK'
            PRINT 1015,MESLOC,
     2           KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif

C-----------------------------------------------------------------------
C CLEO3: if kink is less than 500 micron shift  at 1 meter distance
C CLEO2                  was 5000 micron at 1 meter
C then skip it, it is insignificant
C-----------------------------------------------------------------------

            IF(ABS(PHI6CF).LT.0.005)THEN
               KNK6CF=KABG2_CLEAR
               CALL FIT_WHATEVER
               CALL SBYT(0,STATCF(1),26,1)

#if defined(CLEO_FTDIAG)
               PRINT 1003
               MESLOC='3F; del kink'
               PRINT 1015,MESLOC,
     2              KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif

            ELSE
               CALL SBYT(1,STATCF(1),26,1)
            ENDIF

C-----------------------------------------------------------------------
C case where kink is not tested but
C KAPCF and number of hits still warrants fit with D0 floating
C-----------------------------------------------------------------------
         ELSE
            KNK6CF=KABG2_CLEAR
            CALL FIT_WHATEVER
            CALL SBYT(0,STATCF(1),25,1)

#if defined(CLEO_FTDIAG)
            PRINT 1003
            MESLOC='D0 FLOAT FIT'
            PRINT 1015,MESLOC,
     2           KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
#endif
         ENDIF



C do not remove D0 constraint
      ELSE
         CALL SBYT(0,STATCF(1),24,1)
C-----------------------------------------------------------------------
C end of condition that there are enough hits for D0 float fit
C now remove the D0 constraint for all remaining fits
C-----------------------------------------------------------------------
      ENDIF
      D0_CONSTRAIN=.FALSE.
      ICHICF=CHICF
      IF(ICHICF.LT.0)ICHICF=0
      IF(ICHICF.GT.31)ICHICF=31
C      CALL SBYT(ICHICF,STATCF(word),bit,5)
               
C-----------------------------------------------------------------------
C NOTE: if there has been no call to a fitter with D0 floating
C the transformation in KABG2 has not been set and we are relying
C on the RESET in DUFIND to have set the transformation to 0
C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG)
      PRINT 1003
      MESLOC='   WIRE FIT '
      PRINT 1015,MESLOC,KAPCF,ALPCF,BETCF,GAMCF,XICF,PHI6CF,CHICF
      MESLOC='after kink  '
      PRINT 1013,MESLOC,KAPAFT6,ALPAFT6,BETAFT6,GAMAFT6,XIAFT6
#endif

#if (defined(CLEO_FTDIAG))&&(defined(CLEO_XTSUBS))
      CALL XTS_TRAK('CF',0,' FAST_TRAK after fit to wires')
#endif

C-----------------------------------------------------------------------
C branch point for error
C-----------------------------------------------------------------------

990   CONTINUE

      RETURN
      END






