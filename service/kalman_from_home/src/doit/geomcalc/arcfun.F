*
* $Id: arcfun.F,v 1.2 1999/11/03 22:54:48 dpp Exp $
*
* $Log: arcfun.F,v $
* Revision 1.2  1999/11/03 22:54:48  dpp
* no longer use the small angle approx when using the look-up table ATN2PI
* now protect against KAPPA=0, was never done before
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:28  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:51  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  14.40.48  by  Steve Schaffner
*CMZ :  5.54/03 03/05/91  16.45.15  by  Steve Schaffner
*CMZ :  5.54/00 05/02/91  15.35.47  by  Steve Schaffner
*CMZ :  5.52/00 27/02/90  10.57.51  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  13.16.52  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.50.22  by  R. Namjoshi
*-- Author :
      REAL FUNCTION ARCFUN( KAPPA, ALPHA, BETA, XX, YY, XI )
C.......................................................................
C.
C. ARCFUN - Calculate arc length of track from PCA to given X, Y
C.
C. COMMON    : ?
C. CALLS     : ATN2PI
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 01-Apr-87
C.
C. Modification Log.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
*
*     ARGUMENTS:
*        KAPPA,ALPHA,BETA,XI:    TRACK PARAMETERS
*        XX,YY:            POINT
*
      REAL ARGMIN
      PARAMETER (ARGMIN=0.3)
      REAL KAPMIN
      PARAMETER (KAPMIN=.01)

      REAL KAPPA,ALPHA,BETA,YY,XX,XI
      REAL R2KAP,AR2KAP,XINV,A1,A2,ARG2
      REAL CROSS,ATN2PI

* ----------Executable code starts here---------------------

      R2KAP  = 2.0 * KAPPA
      XINV   = 1./XI
      AR2KAP = ABS(R2KAP)
      CROSS  = ALPHA*YY - BETA*XX
      A1     = AR2KAP*CROSS*XINV
      A2     = R2KAP*(ALPHA*XX + BETA*YY)*XINV + XI



C no longer use the small angle approximation with
C the new look-up based ATN2PI

C      IF( A1.GE.0.0 .AND. A2.GT.0.0 .AND. A1.LT.ARGMIN ) THEN
C          ARG2   = A1*A1
C          ARCFUN = CROSS* (1. + ARG2 * (1./6. + ARG2 * (3./40.))) *XINV
C      ELSE
C          ARCFUN = ATN2PI(A1,A2)/AR2KAP
C      ENDIF

C protect against KAPPA=0, use GAMMA=0 approximation

      IF((KAPPA.LT.-KAPMIN).OR.(KAPPA.GT.KAPMIN))THEN
          ARCFUN = ATN2PI(A1,A2)/AR2KAP
       ELSE
          ARCFUN=SQRT(XX**2+YY**2)
       ENDIF

      RETURN
      END



