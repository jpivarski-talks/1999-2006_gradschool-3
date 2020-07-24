*
* $Id: fit_whatever.F,v 1.9 2002/09/05 19:24:07 dpp Exp $
*
* $Log: fit_whatever.F,v $
* Revision 1.9  2002/09/05 19:24:07  dpp
*      -> require STRFFT.GT.0 instead of .NE.0
*         now using STRFFT=-1 to designate orphaned hits
*
* Revision 1.8  2001/11/19 23:41:30  dpp
*      -> remove diagdoit (old duet diagnostics) routines
*      -> delete unused include file
*      -> add new control parameter include file
*      -> notes on NHITCF
*      -> set kinks to zero and out of range if not used
*      -> use NHIT3F,CHI3F to hold the last fitted values from any fitter
*      -> set NHITTF to NFIT for STAK operations
*      -> change some variable names
*
* Revision 1.7  2001/08/21 14:08:11  dpp
*      -> use "KILL" to disable acknowledge of divergence
*
* Revision 1.6  2001/03/30 00:00:33  dpp
*      -> acknowledge divergence
*
* Revision 1.5  2001/03/06 20:35:31  dpp
*      -> argument for CFDFIT
*      -> dispay divergence in diagnostic
*
* Revision 1.4  2000/12/19 23:38:46  dpp
*      -> counters of number of DR layers of types in fit
*
* Revision 1.3  2000/10/24 22:40:05  dpp
*      -> diagnostics
*
* Revision 1.2  2000/08/18 18:56:36  dpp
*      -> set LCFEMC false
*      -> new arguments for KABG2
*      -> diagnostics
*
* Revision 1.1  2000/04/26 14:54:58  dpp
*      -> NEW
*      -> access to all fitters
*      -> add access to CFDFIT with D0_CONS flag
*      -> add access to SZ_FITTER with SAV_FIT_TYP
*      -> use call to RESIDUALS instead of RESICF
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE FIT_WHATEVER
C.......................................................................
C.
C. FIT_WHATEVER - Refit CF of LF track in /TFTRAK/ and calculate the pulls
C.                use whatever fitter is required
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    : 
C. AUTHOR    :
C.
C. VERSION   : 1.01
C. CREATED   : 
C. LAST MOD  : 
C.
C.
C. Modification Log.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"

      INTEGER IFIT,  JFIT,  AFTERK, ILYR 
      REAL    XNOW,  YNOW
      REAL    RRK,   RRH
C== Dummy variables for KABG2
      REAL    KADUM, ALDUM, BEDUM, GADUM, XIDUM

      CHARACTER*6 MESS
      REAL CH2,CHTL,CHITMP,SUMWGT
      INTEGER NVAR

      LOGICAL LAYER_FOUND(KLYRTF)
      LOGICAL DIVERGE
#if defined(CLEO_DPPDIAG)
      INTEGER IGO,KILLA
      DATA KILLA/0/
#endif

* ----------Executable code starts here---------------------

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C-----------------------------------------------------------------------
C R-PHI fitting;  SAV_FIT_TYP is in tfgeomcd.inc
C-----------------------------------------------------------------------

      IF(SAV_FIT_TYP.EQ.FITTYP_CF)THEN

C-----------------------------------------------------------------------
C 2 parameter fit with D0 set to zero;  D0_CONS is in tftrakcd.inc
C   CFDFIT calculates pulls before fit, but does it recalculate after fit?
C   CFDFIT fills NHITCF
C   CFDFIT requires NHITCF>/=MNHTCF=4
C-----------------------------------------------------------------------

         IF(D0_CONS)THEN
            CALL CFDFIT(DIVERGE)
            NVAR=2

            PHI6CF=0.
            RKN6CF=8.9
            PHI7CF=0.
            RKN7CF=9.0

#if defined(CLEO_DPPDIAG)
            MESS='CFDFIT'
#endif

C-----------------------------------------------------------------------
C 4 parameter fit with kink;  KNK6CF is in tftrakcd.inc
C   first enable the transformation
C   CFFT4D fills NHITCF by calling CFFTX2 and passing  NHITCF as an argument
C-----------------------------------------------------------------------
         ELSE
            IF(.NOT.((KNK6CF.EQ.0).OR.(KNK6CF.EQ.KABG2_CLEAR)))THEN

               CALL KABG2(KNK6CF,
     1              FTK6CF,RKN6CF,SKN6CF,0,
     2              KAPCF,ALPCF,BETCF,GAMCF,XICF,0.,
     3              KADUM, ALDUM, BEDUM, GADUM, XIDUM)
               CALL CFFT4D

               DIVERGE=.FALSE.
               IF(FTK6CF.EQ.KABG2_FLOAT)THEN
                  NVAR=4
               ELSE
                  NVAR=3
               ENDIF

               PHI7CF=0.
               RKN7CF=9.0

#if defined(CLEO_DPPDIAG)
               MESS='CFCF4D'
#endif
C-----------------------------------------------------------------------
C standard 3 parameter fit 
C   CFCFIT sets XICF 
C   CFCFIT calls CFMTRX which fills NHITCF
C   CFCFIT requires NHITCF>/=MNHTCF=4
C-----------------------------------------------------------------------
            ELSE
               CALL KABG2(KABG2_CLEAR,
     1              0,0,0,0, 
     2              0.,0.,0.,0.,0., 0., 
     3              KADUM, ALDUM, BEDUM, GADUM, XIDUM)
               CALL CFCFIT
               DIVERGE=.FALSE.
               NVAR=3

               PHI6CF=0.
               RKN6CF=8.9
               PHI7CF=0.
               RKN7CF=9.0

#if defined(CLEO_DPPDIAG)
               MESS='CFCFIT'
#endif
            ENDIF
         ENDIF

C-----------------------------------------------------------------------
C then calculate the pulls for CFCFIT and CFFT4D and CFDFIT
C and label the error matrix elements "not current"
C-----------------------------------------------------------------------

C call KABG2; prepare to call RESIDUALS in loop over hits
C this call will fill PHI6CF for KABG2_ELOSS type kink
         IF((KNK6CF.NE.0).AND.(KNK6CF.NE.KABG2_CLEAR))THEN
            CALL KABG2(KABG2_CALC,
     1           0,0,0,KABG2_XI_RECALC,
     2           KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF, PHI6CF,
     3           KAPAFT6,ALPAFT6,BETAFT6,GAMAFT6,XIAFT6)
         ENDIF
         
         CH2=0.
         CHTL=0.
         SUMWGT=0.
         NLFDRA=0
         NLFDRU=0
         NLFDRV=0
         CALL VFILL(LAYER_FOUND, KLYRTF, .FALSE.)

         DO 10 IFIT = 1, NFIT
            IF((IADFIT(IFIT).NE.0).AND.(STRFFT(IFIT).GT.0))THEN
               ILYR=IPLFIT(IFIT)

               CALL RESIDUALS('FIT',0,IFIT,PULFIT(IFIT))
               CH2=CH2+PULFIT(IFIT)**2  *WGTFIT(IFIT)
               CHITMP=PULFIT(IFIT)
               IF(IADFIT(IFIT).LT.0)CHITMP=-CHITMP
               CHTL=CHTL+CHITMP
#if defined(CLEO_DPPDIAG)
               SUMWGT=SUMWGT+SQRT(WGTFIT(IFIT))*.000110
#endif

               IF(.NOT.LAYER_FOUND(ILYR))THEN
                  LAYER_FOUND(ILYR)=.TRUE.
                  IF(LUSE_LAYER(ILYR,LVLTYP_PAT_REC,FITTYP_CF))THEN
                     IF(LSTEREOLAYER(ILYR))THEN
                        IF(TANSTF(ILYR).GT.0.)THEN
                           NLFDRU=NLFDRU+1
                        ELSE
                           NLFDRV=NLFDRV+1
                        ENDIF
                     ELSE
                        NLFDRA=NLFDRA+1
                     ENDIF
                  ENDIF
               ENDIF

            ENDIF
 10      CONTINUE

C-----------------------------------------------------------------------
C note: NHITCF is used to calculate NDOFTF which is a STAK variable
C       NHITCF is used to calculate the time-like residuals and
C        the internally calculated chi square. This is consistant with
C        the use in CFCFIT and CFDFIT.
C-----------------------------------------------------------------------

         IF(NHITCF.GT.0)TLRESCF=CHTL/NHITCF
         NDOFTF=NHITCF-NVAR
         IF(NDOFTF.GE.1)THEN
            CH2RCCF=CH2/NDOFTF
         ELSE
            CH2RCCF=CHIDEF
         ENDIF


C-----------------------------------------------------------------------
C save the last fitted values in NHIT3F, CHI3F for use by the stak ordering
C mark the error matrtix elements false
C-----------------------------------------------------------------------

         NHIT3F=NHITCF
         CHI3F=CHICF
         LCFEMC=.FALSE.

#if defined(CLEO_DPPDIAG)
         PRINT 1003
 1003    FORMAT(
     1' ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
         PRINT 1001,MESS,NVAR,DIVERGE,NFIT,NHITCF,NDOFTF,SUMWGT,
     2        CH2RCCF,TLRESCF,NLFDRA,NLFDRU,NLFDRV
 1001    FORMAT(' FIT_WHATEVER: with ',A6,
     2        ' float',I2,' variable,',' diverg:',L1,
     3        ' NFIT=',I3,' NHITCF=',I3,' ndof=',I3,' SUMWGT=',F9.3,
     4        ' CH2=',F9.3,', ave tl=',F9.6,' NLFDRauv=',3I3)
         IF(DIVERGE)THEN
            PRINT 1005
 1005       FORMAT(
     1           ' FIT_WHATEVER: acknowledge diverge,',
     2           ' <cr>, or "K"ill')
            IF(KILLA.EQ.0)THEN
               CALL DSF_RFV_INPUT(IGO)
               IF(IGO.EQ.'K')THEN
                  KILLA=1
               ENDIF
            ENDIF
         ENDIF
         PRINT 1003
#endif

C-----------------------------------------------------------------------
C S-Z fitting
C    SZ_FITTER fills NHITLF
C-----------------------------------------------------------------------

      ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
         CALL SZ_FITTER

C save the last fitted values in NHIT3F, CHI3F for use by the stak ordering
         NHIT3F=NHITLF
         CHI3F=CHILF

      ENDIF

C-----------------------------------------------------------------------
C ALL FITS....
C save NHITTF because that is what is saved in the stak
C-----------------------------------------------------------------------
      NHITTF=NFIT

      RETURN
      END

