*
* $Id: sz_fitter.F,v 1.12 2002/09/24 22:55:51 dpp Exp $
*
* $Log: sz_fitter.F,v $
* Revision 1.12  2002/09/24 22:55:51  dpp
*      -> fix Z0 matrix element for condition of "no hits" (it was 0.2**3)
*
* Revision 1.11  2002/09/05 19:22:57  dpp
*      -> require STSZFT.GT.0 instead of .NE.0
*         now using STSZFT=-1 to designate orphaned hits
*
* Revision 1.10  2001/11/19 23:42:06  dpp
*      -> remove diagdoit (old duet diagnostics) routines
*      -> delete unused include file
*     **> change Z0_constraint_fit error matrix from .05 to .01 in d(Z0)
*
* Revision 1.9  2001/09/12 22:05:20  dpp
*      -> use variable, Z0BIAS_MIN, that was already set up
*
* Revision 1.8  2000/10/24 22:40:32  dpp
*      -> create error matrix values for all fits,
*         especially z0 constrained fits because the extrapolation
*         error will be used when assigning silicon hits
*
* Revision 1.7  2000/10/02 21:08:14  dpp
*      -> allow matrix invert for 2 entries if DET is significant
*
* Revision 1.6  2000/04/26 14:53:59  dpp
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*
* Revision 1.5  1999/11/23 23:28:01  dpp
* allow real (and simple)  z0=0 fit
*
* Revision 1.4  1999/11/03 22:59:54  dpp
* change compile switch for some print statements
*
* Revision 1.3  1999/09/12 22:37:11  dpp
* fill NHITLF and CHILF direcly; use Z0BIAS
*
* Revision 1.2  1999/08/03 21:54:23  dpp
* use "??" argument in XTS_TRAK call
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE SZ_FITTER
C.......................................................................
C.
C. SZ_FITTER - Do a CHI2 line fit. Results in tftrakcd
C.             Uses tftrakcd variables directly, fills PULFIT, ERMFIT
C.
C. COMMON    : ?
C. CALLS     :
C. CALLED    : ?
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 19-Dec-97
C.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfctlcde.inc"

      DOUBLE PRECISION S1,SX,SY,SXX,SXY,DET
      REAL WI,XI,YI
      REAL WPULL,CHI,ERRM(3),SIGE(3)
      INTEGER IFIT
      LOGICAL INVERT_OK
      REAL Z0BIAS_MAX,Z0BIAS_MIN
      LOGICAL Z0CONSTRAIN

      DATA Z0BIAS_MAX/ 1.     /
      DATA Z0BIAS_MIN/ 0.0001 /

* ----------Executable code starts here---------------------

      IF(NFIT.GE.0)THEN
         NHITLF= 0
         S1    = 0.0
         SX    = 0.0
         SY    = 0.0
         SXX   = 0.0
         SXY   = 0.0

#if defined(CLEO_SZFDIAG)
         PRINT 1001
 1001    FORMAT(' SZ_FITTER:seq IADFIT stat        weight',
     2          '        S         Z',
     3          '              S1              SX             SXX')
#endif


C create a fake point for reasonable Z0BIAS
C ignore Z0BIAS > Z0BIAS_MAX
C do a real Z0=0 fit (below) for Z0BIAS<Z0BIAS_MIN

         Z0CONSTRAIN=(Z0BIAS.LT. Z0BIAS_MIN)

         IF((Z0BIAS.LT.Z0BIAS_MAX).AND.(.NOT.Z0CONSTRAIN))THEN
            WI    = 1./Z0BIAS**2
            XI    = 0.
            YI    = 0.
            S1    = S1    + WI
            SX    = SX    + WI * XI
            SXX   = SXX   + WI * XI * XI
            SY    = SY    + WI * YI
            SXY   = SXY   + WI * XI * YI
            
#if defined(CLEO_SZFDIAG)
            PRINT 1006,WI,XI,YI,
     2           S1,SX,SXX
 1006       FORMAT(14X,' Z0BIAS     ',
     2             F14.3,F10.6,F10.6,F16.3,F16.4,F16.4)
#endif
         ENDIF

         DO 10 IFIT = 1, NFIT
            WI    = WGTFIT(IFIT)
            XI    = SFIT(IFIT)
            YI    = ZFIT(IFIT)
            IF((IADFIT(IFIT).NE.0).AND.(STSZFT(IFIT).GT.0))THEN

               NHITLF= NHITLF+ 1
               S1    = S1    + WI
               SX    = SX    + WI * XI
               SXX   = SXX   + WI * XI * XI
               SY    = SY    + WI * YI
               SXY   = SXY   + WI * XI * YI
            ENDIF
            
#if defined(CLEO_SZFDIAG)
            PRINT 1002,IFIT,IADFIT(IFIT),STSZFT(IFIT),WI,XI,YI,
     2           S1,SX,SXX
 1002       FORMAT(I14,I7,I5,
     2             F14.3,F10.6,F10.6,F16.3,F16.4,F16.4)
#endif

 10      CONTINUE


         IF(Z0CONSTRAIN)THEN
            IF(NHITLF.LT.1)THEN
               INVERT_OK=.FALSE.
            ELSE
               DET  = SXX
               IF( ABS(DET)/S1 .LT. 1.0E-6 )THEN
                  INVERT_OK=.FALSE.
               ELSE
                  INVERT_OK=.TRUE.
               ENDIF
            ENDIF

         ELSE
            IF(NHITLF.LT.2)THEN
               INVERT_OK=.FALSE.
            ELSE
               DET   = S1 * SXX - SX * SX
               IF( ABS(DET)/S1 .LT. 1.0E-6 )THEN
                  INVERT_OK=.FALSE.
               ELSE
                  INVERT_OK=.TRUE.
               ENDIF
            ENDIF
         ENDIF

         IF(INVERT_OK)THEN
            IF(Z0CONSTRAIN)THEN
               Y0LF=0.
               TANDLF= SXY  / DET
            ELSE
               Y0LF  = (  SXX * SY  -SX * SXY ) / DET
               TANDLF= ( -SX  * SY  +S1 * SXY ) / DET
            ENDIF

#if defined(CLEO_DPPDIAG)
            PRINT 1003,NHITLF,DET,Y0LF,TANDLF
 1003       FORMAT(' SZ_FITTER:INVERT:',
     2           ' NHITLF=',I3,'  DET=',F20.2,
     3           ' Y0LF,TANDLF=',2F10.6)
#endif
#if defined(CLEO_SZFDIAG)
            PRINT 1004
 1004       FORMAT(' SZ_FITTER:seq',
     2             ' stat         S         Z',
     3             '    PULFIT         WPULL       CHI2SUM')
#endif

            CHI = 0.0
            DO 20 IFIT = 1, NFIT
               PULFIT(IFIT)=ZFIT(IFIT)-(Y0LF+TANDLF*SFIT(IFIT))
               WPULL=WGTFIT(IFIT)*PULFIT(IFIT)**2
               IF(STSZFT(IFIT).GT.0)THEN
                  CHI=CHI+WPULL
               ENDIF
               
#if defined(CLEO_SZFDIAG)
                  PRINT 1005,IFIT,
     2                       STSZFT(IFIT),SFIT(IFIT),ZFIT(IFIT),
     3                       PULFIT(IFIT),WPULL,CHI
 1005             FORMAT(I14,I5,F10.6,F10.6,F10.6,F14.4,F14.4)
#endif

 20         CONTINUE

            IF(Z0CONSTRAIN)THEN
                  CHILF = CHI/FLOAT(NHITLF)
            ELSE
               IF(Z0BIAS.LT.1.)THEN
                  CHILF = (CHI + (Y0LF/Z0BIAS)**2)/(NHITLF-1)
               ELSE
                  CHILF = CHI/FLOAT(NHITLF-2)
               ENDIF
            ENDIF

C Load error matrix values
            IF(Z0CONSTRAIN)THEN
               ERRM(1)=0.01**2
               ERRM(2)=0.
               ERRM(3)=0.
            ELSE
               ERRM(1)= CHILF* SXX/DET
               ERRM(2)= CHILF* S1/DET
               ERRM(3)=-CHILF* SX/DET
            ENDIF

C Defaults for bad matrix
         ELSE
            CHILF = CHIDEF
            ERRM(1)=0.1**2
            ERRM(2)=0.
            ERRM(3)=0.
         ENDIF

#if defined(CLEO_XTSUBS)
         CALL XTS_TRAK('CF',0,'SZ_FITTER (end)')
#endif

C NO HITS
      ELSE
         ERRM(1)=0.2**2
         ERRM(2)=0.
         ERRM(3)=0.
      ENDIF

      CALL ERR_MATRIX_ELEMENT(1,P3DZ0,P3DZ0,ERRM(1))
      CALL ERR_MATRIX_ELEMENT(1,P3DCT,P3DCT,ERRM(2))
      CALL ERR_MATRIX_ELEMENT(1,P3DCT,P3DZ0,ERRM(3))
      
      RETURN
      END







