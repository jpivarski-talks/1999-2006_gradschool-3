*
* $Id: atn2pi.F,v 1.1 1999/11/03 22:52:52 dpp Exp $
*
* $Log: atn2pi.F,v $
* Revision 1.1  1999/11/03 22:52:52  dpp
* new look-up based ATAN
* now in geomcalc; old Taylor based is in cleoks_util (renamed)
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      REAL FUNCTION ATN2PI(Y_INPUT,X_INPUT)
C.......................................................................
C.
C. ATN2PI - acr tangent, returns 0-2pi, look-up
C.
C. COMMON    : 
C. CALLS     : None
C. CALLED    : Various places
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 01-11-99
C. LAST MOD  :
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

      REAL X_INPUT,Y_INPUT,XA,YA,ANGLE

      REAL ONEPI,TWOPI,HAFPI
      PARAMETER (ONEPI= 3.1415926536)
      PARAMETER (TWOPI= 2.*ONEPI)
      PARAMETER (HAFPI= .5*ONEPI)

      LOGICAL OPEN
      DATA OPEN/.FALSE./

      INTEGER BINS,TABLE_SIZE
      PARAMETER (BINS=500)
      REAL VAL(0:BINS),DIFF(0:BINS)

      INTEGER IBIN
      LOGICAL VALID
      INTEGER QUADRANT
      REAL ARGUMENT,EXPAND,EXCESS
      LOGICAL FLIPPED

C----------Executable code starts here---------------------

C fill in the table

      IF(.NOT.OPEN)THEN
         OPEN=.TRUE.
         DO 10 IBIN=0,BINS
            ARGUMENT=(1./BINS)*IBIN
            VAL(IBIN)=ATAN(ARGUMENT)
            IF(IBIN.GT.0)DIFF(IBIN-1)=VAL(IBIN)-VAL(IBIN-1)
 10      CONTINUE
         DIFF(BINS)=0.
      ENDIF

C find the octant and get the argument      

      IF    (X_INPUT.GT.0.)THEN
         XA=  X_INPUT
         VALID=.TRUE.
         IF    (Y_INPUT.GE.0.)THEN
            YA=  Y_INPUT
            QUADRANT=1
         ELSE
            YA= -Y_INPUT
            QUADRANT=4
         ENDIF
      ELSEIF(X_INPUT.LT.0.)THEN
         XA= -X_INPUT
         VALID=.TRUE.
         IF    (Y_INPUT.GT.0.)THEN
            YA=  Y_INPUT
            QUADRANT=2
         ELSE
            YA= -Y_INPUT
            QUADRANT=3
         ENDIF
      ELSE
         XA=  X_INPUT
         IF    (Y_INPUT.GT.0.)THEN
            VALID=.TRUE.
            YA=  Y_INPUT
            QUADRANT=1
         ELSEIF(Y_INPUT.LT.0.)THEN
            VALID=.TRUE.
            YA= -Y_INPUT
            QUADRANT=4
         ELSE
            VALID=.FALSE.
            ATN2PI=0.
         ENDIF
      ENDIF

      IF(VALID)THEN

         IF(XA.GT.YA)THEN
            ARGUMENT=YA/XA
            FLIPPED=.FALSE.
         ELSE
            ARGUMENT=XA/YA
            FLIPPED=.TRUE.
         ENDIF

         EXPAND=ARGUMENT*BINS
         IBIN=EXPAND
         EXCESS=EXPAND-IBIN

         ANGLE=VAL(IBIN)+EXCESS*DIFF(IBIN)
         IF(FLIPPED)ANGLE=HAFPI-ANGLE

         IF    (QUADRANT.EQ.2)THEN
            ANGLE=ONEPI-ANGLE
         ELSEIF(QUADRANT.EQ.3)THEN
            ANGLE=ONEPI+ANGLE
         ELSEIF(QUADRANT.EQ.4)THEN
            ANGLE=TWOPI-ANGLE
         ENDIF

         ATN2PI= ANGLE
      ENDIF

      RETURN
      END



