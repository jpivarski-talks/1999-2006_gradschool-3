*
* $Id: tfxytr.F,v 1.2 2001/11/19 23:45:35 dpp Exp $
*
* $Log: tfxytr.F,v $
* Revision 1.2  2001/11/19 23:45:35  dpp
*      -> delete unused include file
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
*CMZ :  5.54/07 21/05/91  13.51.42  by  Steve Schaffner
*CMZ :  5.53/07 24/09/90  16.56.13  by  D. Riley
*CMZ :  5.52/00 22/01/90  13.32.54  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.23.06  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.52  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFXYTR(ALP,BET,KAP,GAM,RSQ,IBRNCH,XTR,YTR,PHITR,IER)
C
C     Given the track parameters, calculates the co-ordinates
C     and azimuth of the intersection with a surface of radius**2 RSQ.
C     Arguments:
C        ALP,BET,KAP,GAM      Track parameters
C        RSQ         Radius squared
C        IBRNCH         Branch: +1 (outward) -1 (inward)
C        IER         < 0 if track doesn't reach layer
C        XTR,YTR        Co-ordinates
C        PHITR       Track azimuth
C
C     TFXYTR only calculates (XTR,YTR)
C
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      REAL     KAP,XI2,ALP,BET,COEFF1,RSQ,GAM,COEFF2,XI2INV,XTR,YTR
      REAL     PHITR,ATN2PI
      INTEGER  IER,IBRNCH
C
* ----------Executable code starts here---------------------

      IER    = -1
      XI2    = ALP**2 + BET**2
      COEFF1 = KAP*RSQ + GAM
      COEFF2 = RSQ*XI2 - COEFF1**2
      IF(COEFF2.LT.0.)  RETURN
C
      IER    = 0
      COEFF2 = IBRNCH*SQRT(COEFF2)
      XI2INV = 1./XI2
      XTR    = (-ALP*COEFF1 - BET*COEFF2)*XI2INV
      YTR    = (-BET*COEFF1 + ALP*COEFF2)*XI2INV
      RETURN
      END
