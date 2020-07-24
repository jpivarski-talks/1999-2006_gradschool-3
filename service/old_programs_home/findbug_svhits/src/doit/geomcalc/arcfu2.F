*
* $Id: arcfu2.F,v 1.1 1998/02/24 18:56:39 dpp Exp $
*
* $Log: arcfu2.F,v $
* Revision 1.1  1998/02/24 18:56:39  dpp
* not entered 2/9/98
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      REAL*4 FUNCTION ARCFU2 ( ETA, XW, YW )
*
* begin_doc
*
*  Documentation for subroutine arcfu2
*
*  Purpose: 
*  -------- 
*  Compute arclength along a helix from PCAO to the PCA of some other 
*  specified point, such as a wire position.  There is a special treatment
*  of very stiff tracks.  For normal tracks, the routine computes
*  a turning angle in the range [0,2pi] and then mulitplies by the
*  radius of curvature; in this case the arclength will always be
*  positive.  In the case of very stiff tracks it is possible to
*  get a negative arc length, which means that the specified point
*  is "behind" the PCAO.  The sense of "behind" is defined by
*  the direction of the track at the PCAO.
*
*  Input Parameters: 
*  ----------------
*  REAL ETA(5) - Helical track parameters
*  REAL XW     - X of wire position
*  REAL YW     - Y of wire position
*
*  Output Parameters: 
*  -----------------
*  Function value 
*
*  Other routines:
*  ---------------
*
*  Notes:
*  ------
*
*  Author:   Rob Kutschke      Created:  Fri May 10 02:15:44 EDT 1996
*  -------
*
*  Major revisions:
*  ----------------
*     
*
* end_doc
*
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
      SAVE
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------

C     Track parameters.
      REAL*4 ETA(5)

C     Wire Position.
      REAL XW, YW

*  Local User defined variables:
*  -----------------------------
*
C     Charge, radius of curvature.
      REAL*8 Q, RHO

C     Sine of the polar angle.
      REAL STH

C     Center of curvature and its signed distance from origin.
      REAL*8 XC, YC, RC

C     Turning angle from PCAO to point closest to wire. 
C     COS and SIN of this angle.
      REAL*8 PSI, COSPSI, SINPSI

C     Vector from center of curvature to origin.
      REAL*8 CO(2), RCO

C     Vector from center of curvature to the wire.
      REAL*8 CW(2), RCW

C     A point on the track and the direction at that point.
      REAL*8 T0(2), T(2)

C     Stuff.
      REAL*8 TWOPI
      PARAMETER ( TWOPI = 3.141592654D0 * 2.D0 )

*
* end_var
*
*  Data statements:
*  ----------------
*

*  executable code for routine arcfu2:
*----6----------------------------------------------------------------72
*

C     Is the track too straight?
      IF ( ABS(ETA(1)) .LE. 0.00001 ) GOTO 1000

C     Quantities derived from track parameters.
      Q    = SIGN(1., ETA(1) )
      RHO  = DABS( 0.5D0/DBLE(ETA(1)) )
      RC   =  DBLE(ETA(3)) + 0.5D0/DBLE(ETA(1))
      XC   = -RC*DSIN(DBLE(ETA(2)))
      YC   =  RC*DCOS(DBLE(ETA(2)))
      STH  = 1.D0 / DSQRT ( 1.D0 + DBLE(ETA(4))**2 )

      CO(1)  =    - XC
      CO(2)  =    - YC
      CW(1)  = XW - XC
      CW(2)  = YW - YC
      RCO    = ABS(RC)
      RCW    = SQRT( CW(1)**2 + CW(2)**2 )

      SINPSI = Q * ( CO(1)*CW(2) - CO(2)*CW(1) )/RCO/RCW
      COSPSI =     ( CO(1)*CW(1) + CO(2)*CW(2) )/RCO/RCW
      PSI    = DATAN2( SINPSI, COSPSI)

      IF ( PSI .LT. 0.0 ) PSI = PSI + TWOPI

      ARCFU2 = RHO*PSI/STH

      RETURN

C     The track is straight or almost straight.
 1000 CONTINUE

C     Quantities derived from track parameters.
      T (1) =  DCOS(DBLE(ETA(2)))
      T (2) =  DSIN(DBLE(ETA(2)))
      T0(1) = - ETA(3)*T(2)
      T0(2) =   ETA(3)*T(1)
      STH   = 1.D0 / DSQRT ( 1.D0 + DBLE(ETA(4))**2 )

      ARCFU2 = ( (DBLE(XW)-T0(1))*T(1) + (DBLE(YW)-T0(2))*T(2) )
      ARCFU2 = ARCFU2/STH

      RETURN
      END




