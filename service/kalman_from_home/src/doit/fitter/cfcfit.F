*
* $Id: cfcfit.F,v 1.3 2001/11/19 23:41:11 dpp Exp $
*
* $Log: cfcfit.F,v $
* Revision 1.3  2001/11/19 23:41:11  dpp
*      -> remove diagdoit (old duet diagnostics) routines
*      -> delete unused include file
*      -> remove setting PHI6CF; now in fit_whatever or other calling routine
*
* Revision 1.2  1999/09/12 22:34:41  dpp
* remove arguments, use CF directly; use NHITCF not NFIT in chisq
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:49  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/33 30/04/93  15.41.23  by  Rob Kutschke
*CMZ :  5.54/14 06/01/92  16.33.10  by  Steve Schaffner
*CMZ :          10/10/91  15.11.32  by  Jon Urheim
* add UNIX to +SELF statements.
*CMZ :  5.54/04 20/05/91  09.57.18  by  Steve Schaffner
*CMZ :  5.53/07 05/10/90  19.16.16  by  Steve Schaffner
*CMZ :  5.53/03 17/04/90  12.14.03  by  D. Riley
*CMZ :  5.51/00 10/08/89  15.43.21  by  Steve Schaffner
*CMZ :  5.50/00 26/02/88  00.07.54  by  R. Namjoshi
*-- Author :
      SUBROUTINE CFCFIT
C.......................................................................
C.
C. CFCFIT - Circle fit, linear least sqares with quadratic constraint
C.
C. COMMON    : ?
C. CALLS     : CFMTRX
C. CALLED    : ?
C. AUTHOR    : D. Cassel. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 25-Feb-88
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

      REAL XRRXRR,YRRYRR,RRRRM1,XXYY,C0
      REAL C1,C2,C4,C2D,C4D,CHISCL,DLAMAX,DLAMDA,H11,H14
      REAL H22,H24,H34,ROOTSQ,RATIO,ALPAR,BETAR
      INTEGER ITRY,NTRY,IERR

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfunitcd.inc"

#if defined(CLEO_IBM)
      DOUBLE PRECISION XAV   ,YAV   ,WSUM  ,RSCALE,COSROT,SINROT
      DOUBLE PRECISION XXAV  ,XYAV  ,YYAV  ,XRRAV ,YRRAV ,RRRRAV
#endif
#if defined(CLEO_VAX)||defined(CLEO_DECS)||defined(CLEO_UNIX)
      REAL XAV   ,YAV   ,WSUM  ,RSCALE,COSROT,SINROT
      REAL XXAV,XYAV,YYAV  ,XRRAV ,YRRAV ,RRRRAV
#endif

      COMMON /CIRHIJ/  XAV   ,YAV   ,WSUM  ,RSCALE,COSROT,SINROT,
     +                 XXAV  ,XYAV  ,YYAV  ,XRRAV ,YRRAV ,RRRRAV
C
C== Local variables
C
#if defined(CLEO_PDP10)||defined(CLEO_VAX)||defined(CLEO_DECS)||defined(CLEO_UNIX)
      REAL             KAPPA ,ALPHA ,BETA  ,GAMMA
      REAL             LAMDA ,CPOLY ,DCPOLY
#endif
#if defined(CLEO_IBM)
      DOUBLE PRECISION KAPPA ,ALPHA ,BETA  ,GAMMA
      DOUBLE PRECISION LAMDA ,CPOLY ,DCPOLY
      DOUBLE PRECISION C0,C1,C2,C4,DC1,DC2,DC4
#endif

C      LOGICAL         LDEBUG
C      COMMON /LDEBUG/ LDEBUG

      DATA NTRY /5/

C-->  D. G. CASSEL 08.02.80

C-->  FITS A CIRCLE TO THE POINTS SPECIFIED IN THE XFIT & YFIT ARRAYS

C-->  THE FIT MINIMIZES THE DISTANCE FROM
C-->  THE CIRCLE TO THE POINTS.  THIS DISTANCE IS MEASURED ALONG
C-->  THE RADIUS OF THE CIRCLE, NOT ALONG THE CHAMBER SURFACE.

C-->  FIRST CALCULATE THE H MATRIX AND OTHER THINGS

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12+++++++

* ----------Executable code starts here---------------------

      CALL CFMTRX(IERR)
      IF(IERR.EQ.0)THEN
C use these to get the coefficients of the 4-th order characteristic polynimial
C don't panic - the third order term is zero !

         XRRXRR = XRRAV*XRRAV
         YRRYRR = YRRAV*YRRAV
         RRRRM1 = RRRRAV - 1.0
         XXYY   = XXAV*YYAV
         C0 =        RRRRM1*XXYY - XRRXRR*YYAV - YRRYRR*XXAV
         C1 =      - RRRRM1      + XRRXRR      + YRRYRR      - 4.0*XXYY
         C2 =  4.0 + RRRRM1                                  - 4.0*XXYY
         C4 = -4.0

C coefficients of the derivative - used in Newton-Raphson iterations

         C2D =   2.0*C2
         C4D =   4.0*C4

C starting value of LAMDA =
C  linear interpolation between CPOLY(0) & CPOLY(YYAV)

C        IF (LDEBUG) WRITE(IUNLPT,120) C0,C1,C2,C4
C        LAMDA  = YYAV*C0/(C0 + YRRYRR*(XXAV-YYAV))
         LAMDA  = 0.00

C        IF (LDEBUG) WRITE(IUNLPT,125) LAMDA,DLAMDA

         CHISCL = WSUM*RSCALE**2
         DLAMAX = 0.001/CHISCL

         ITRY   = 0
         DLAMDA = DLAMAX
         
 10      IF( (ITRY.LE.NTRY) .AND. (ABS(DLAMDA).GE.DLAMAX) ) THEN
            CPOLY  = C0 + LAMDA*(C1 + LAMDA*(C2  + LAMDA*LAMDA*C4 ))
            DCPOLY =            (C1 + LAMDA*(C2D + LAMDA*LAMDA*C4D))
            DLAMDA = -CPOLY/DCPOLY
            LAMDA  = LAMDA + DLAMDA
            
C           IF (LDEBUG) WRITE(IUNLPT,125) LAMDA,DLAMDA

            ITRY   = ITRY + 1
            GOTO 10
         ENDIF

         IF( LAMDA.LT.0.0 ) LAMDA = 0.0
         CHICF  = CHISCL*LAMDA/FLOAT(NHITCF-3)
         
         IF( CHICF.GE.0.0 ) THEN

C           DCHISQ = CHISCL*DLAMDA
C           LDEBUG = LDEBUG.OR.(LAMDA .LT. 0.0)
C           IF (LDEBUG) WRITE(IUNLPT,135) ITRY,CHICF,DCHISQ

C now calculate matrix elements for ALPHA , BETA , GAMMA  & KAPPA

            H11    = XXAV  -     LAMDA
            H14    = XRRAV
            H22    = YYAV  -     LAMDA
            H24    = YRRAV
            H34    = 1.0   + 2.0*LAMDA
            
C           IF (LDEBUG) WRITE(IUNLPT,140) H11,H14,H22,H24,H34

            ROOTSQ = (H14/H11)**2 + 4.0*H34

C First check for the case of a perfectly straight track.
            IF ( H24 .EQ. 0.0 .AND. H22 .EQ. 0.0 ) THEN
               KAPPA =  0.
               BETA  = -1.
               ALPHA =  0.
               GAMMA =  0.
               
C Normal case, not a straight track.
            ELSE
               IF( ABS(H22).GT.ABS(H24) ) THEN
                  RATIO  = H24/H22
                  ROOTSQ =        RATIO**2  + ROOTSQ
                  KAPPA  = 1.0/SQRT(ROOTSQ)
                  BETA   = - RATIO*KAPPA
                 
C                 LDEBUG = LDEBUG.OR.(ROOTSQ.LT. 0.0)
C                 IF (LDEBUG) WRITE(IUNLPT,145) KAPPA ,BETA,LAMDA

               ELSE
                  RATIO  = H22/H24
                  ROOTSQ = 1.0 + (RATIO**2)*ROOTSQ
                  BETA   = 1.0/SQRT(ROOTSQ)
                  IF( H24.GT.0.0) BETA   = - BETA
                  KAPPA  = - RATIO*BETA

C                 LDEBUG = LDEBUG.OR.(ROOTSQ.LT. 0.0)
C                 IF (LDEBUG) WRITE(IUNLPT,150) KAPPA ,BETA,LAMDA
               ENDIF

               ALPHA  = - (H14/H11)*KAPPA
               GAMMA  = -  H34     *KAPPA
            ENDIF

C           CHECK  = ALPHA**2 + BETA**2 - 4.0*KAPPA*GAMMA  - 1.0
C           IF(LDEBUG)WRITE(IUNLPT,160)KAPPA,ALPHA,BETA,GAMMA,CHECK,CHICF

C transform these into the lab coordinate system

C first get KAPPA  and GAMMA  back to real dimensions

            KAPPA  = KAPPA/RSCALE
            GAMMA  = GAMMA*RSCALE

C next rotate ALPHA  and BETA

            ALPAR  =     COSROT*ALPHA  - SINROT*BETA
            BETAR  =     SINROT*ALPHA  + COSROT*BETA

C then translate by (XAV,YAV)

            ALPHA  =   ALPAR - 2.0*KAPPA*XAV
            BETA   =   BETAR - 2.0*KAPPA*YAV
            GAMMA  =   GAMMA
     2             + KAPPA*(XAV**2 + YAV**2) - ALPAR*XAV - BETAR*YAV

C           CHECK  = ALPHA**2 + BETA**2 - 4.0*KAPPA*GAMMA  - 1.0
C           IF(LDEBUG)WRITE(IUNLPT,160)KAPPA,ALPHA,BETA,GAMMA,CHECK,CHICF

            IF( YRRAV.GE.0.0 ) THEN
               ALPCF =   ALPHA
               BETCF =   BETA
               GAMCF =   GAMMA
               KAPCF =   KAPPA
            ELSE
               ALPCF = - ALPHA
               BETCF = - BETA
               GAMCF = - GAMMA
               KAPCF = - KAPPA
            ENDIF

            XICF  = SQRT(1.0+4.0*KAPCF*GAMCF)

C CHICF < 0
         ELSE
            IERR=2
         ENDIF
      ENDIF
         
      IF(IERR.NE.0)THEN
         CHICF  = CHIDEF
      ENDIF
         
      RETURN

C 120 FORMAT(/' C0 =',1PE12.4,', C1 =',E12.4,
C    &       ', C2 =',  E12.4,', C4 =',E12.4)
C 125 FORMAT(/' LAMDA =',1PE12.4,' DLAMDA =',1PE12.4)
C 135 FORMAT(/' ITRY = ',I5,' CHISQ =',0PF12.5,' DCHISQ =',1PE12.3)
C 140 FORMAT(/'          H11         H14         H22         H24',
C    &         '         H34'//' ',1P10E12.4)
C 145 FORMAT(/' H24/H22 ==> KAPPA  =',1PE12.4,' BETA ='1PE12.4,E12.4)
C 150 FORMAT(/' H22/H24 ==> KAPPA  =',1PE12.4,' BETA ='1PE12.4,E12.4)
C 160 FORMAT(  /' CIRCLF : KAPPA        ALPA        BETA       GAMMA',
C    & '       CHECK      CHISQP'//'   ',1P6E12.4)

      END


