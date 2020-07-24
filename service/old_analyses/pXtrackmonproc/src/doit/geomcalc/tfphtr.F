*
* $Id: tfphtr.F,v 1.4 2001/11/19 23:46:31 dpp Exp $
*
* $Log: tfphtr.F,v $
* Revision 1.4  2001/11/19 23:46:31  dpp
*      -> add new control parameter include file
*
* Revision 1.3  2000/10/24 22:51:27  dpp
*     -> TO PREVENT HISTORY DEPENDENT RESULTS
*         return {x,y} on layer radius for non-intersecting curlers
*
* Revision 1.2  2000/08/18 19:04:03  dpp
*      -> new arguments for KABG2
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:28  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/01/24 22:13:41  zfiles
* change argument to include kink and radius
* incorporates kink into calculation of predicted phi
*
* Revision 1.1.1.1  1994/10/08 01:00:51  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  13.51.45  by  Steve Schaffner
*CMZ :  5.53/07 24/09/90  16.56.13  by  D. Riley
*CMZ :  5.52/00 22/01/90  13.32.54  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.23.06  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.52  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFPHTR(ALPHA,BETA,KAPPA,GAMMA,KN,RK,
     2                  RSQ,IBRNCH,XTR,YTR,PHITR,IER)

C     Given the track parameters, calculates the co-ordinates
C     and azimuth of the intersection with a surface of radius**2 RSQ.
C     Arguments:
C        ALPHA,BETA,KAPPA,GAMMA      Track parameters
C        RSQ                         Radius squared
C        IBRNCH                      Branch: +1 (outward) -1 (inward)
C        IER                         < 0 if track doesn't reach layer
C        XTR,YTR                     Co-ordinates
C        PHITR                       Track azimuth
C
C     TFPHTR calculates PHITR as well as (XTR,YTR)

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"

      REAL     KAPPA,ALPHA,BETA,GAMMA
      REAL     KN,RK,RKSQ
      REAL     COEFF1,RSQ,COEFF2,XI2_INV,XTR,YTR
      REAL     KA,AL,BE,GA,  XIBEF, XI2,XI
      REAL     PHITR,ATN2PI
      INTEGER  IER,IBRNCH
C
* ----------Executable code starts here---------------------

      RKSQ=RK**2
      XI2= ALPHA **2  +  BETA **2
      XI=SQRT(XI2)

C use parameters after kink if radius is beyond kink .OR. 
C                     on second half of loop and track passes kink radius
 
      IF(  (RSQ.GT.RKSQ).OR.
     2     ( (IBRNCH.EQ.-1).AND.(((XI+1)/(2.*KAPPA))**2 .GT.RKSQ) )
     4     )THEN
         CALL KABG2(KABG2_CALC,
     1        0,0,0,KABG2_XI_RECALC,
     2        KAPPA,ALPHA,BETA, GAMMA,XIBEF,  KN,
     3        KA   ,AL   ,BE   ,GA   ,XI)
         XI2= AL **2  +  BE **2
         XI=SQRT(XI2)
      ELSE
         KA=KAPPA
         AL=ALPHA
         BE=BETA
         GA=GAMMA
      ENDIF
      
C calculation checked as the solution of 

C    2                           2   2    2
C K r  +A x  +B y  +G =0   and  x  +y   =r        (dpp, 10/11/00)


      COEFF1= KA*RSQ + GA
      COEFF2= RSQ*XI2 - COEFF1**2
      IF(COEFF2.GE.0.)THEN

         COEFF2=   IBRNCH*SQRT(COEFF2)
         XI2_INV=  1./XI2
         XTR=      (-AL  *COEFF1 - BE  *COEFF2)*XI2_INV
         YTR=      (-BE  *COEFF1 + AL  *COEFF2)*XI2_INV
         IER=      0
         

C if no intersection , give point on layer inline with top of curler
      ELSE
         COEFF1=SQRT(RSQ)/XI

         XTR=      (-AL*COEFF1                )
         YTR=      (-BE*COEFF1                )
         IER=      -1
      ENDIF

      PHITR=       ATN2PI(YTR,XTR)

      RETURN
      END




