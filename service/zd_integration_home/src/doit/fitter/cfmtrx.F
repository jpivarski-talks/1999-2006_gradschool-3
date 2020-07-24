*
* $Id: cfmtrx.F,v 1.4 2002/09/05 19:23:49 dpp Exp $
*
* $Log: cfmtrx.F,v $
* Revision 1.4  2002/09/05 19:23:49  dpp
*      -> require STRFFT.GT.0 instead of .NE.0
*         now using STRFFT=-1 to designate orphaned hits
*
* Revision 1.3  2001/11/19 23:44:31  dpp
*      -> just remove some blanks that make it difficult to search
*
* Revision 1.2  1999/09/12 22:36:28  dpp
* fills NHITCF; returns error if NHITCF<MNHTCF
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
*CMZ :  5.54/07 10/10/91  15.13.26  by  Jon Urheim
* add UNIX flag to +SELF statement.
*CMZ :  5.54/04 20/05/91  09.57.20  by  Steve Schaffner
*CMZ :  5.53/07 05/10/90  19.11.16  by  Steve Schaffner
*CMZ :  5.53/03 17/04/90  12.14.03  by  D. Riley
*CMZ :  5.51/00 10/08/89  15.38.32  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.01  by  R. Namjoshi
*-- Author :
      SUBROUTINE CFMTRX(IERR)
C.......................................................................
C.
C. CFMTRX - Calculates elements of the H matrix.
C.
C. COMMON    : None
C. CALLS     : None
C. CALLED    : ?
C. AUTHOR    : D. Cassel. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 22-Jul-87
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE


#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"

#if defined(CLEO_IBM)
      DOUBLE PRECISION XAV   ,YAV   ,WSUM  ,RSCALE,COSROT,SINROT
      DOUBLE PRECISION XXAV  ,XYAV  ,YYAV  ,XRRAV ,YRRAV ,RRRRAV
#endif
      COMMON /CIRHIJ/  XAV   ,YAV   ,WSUM  ,RSCALE,COSROT,SINROT,
     +                 XXAV  ,XYAV  ,YYAV  ,XRRAV ,YRRAV ,RRRRAV

#if defined(CLEO_VAX)||defined(CLEO_DECS)||defined(CLEO_UNIX)
      REAL XAV   ,YAV   ,WSUM  ,RSCALE,COSROT,SINROT
      REAL XXAV  ,XYAV  ,YYAV  ,XRRAV ,YRRAV ,RRRRAV
#endif
C      LOGICAL         LDEBUG
C      COMMON /LDEBUG/ LDEBUG

      REAL WI,XI,YI,WIXI,WIYI,A,B,ASQPB
      REAL RASQPB,SPLUS,SMINUS,RRAV
      REAL XOLD,YOLD,YIYI,RIRI,XIXI,WIRIRI
      INTEGER IFIT,IERR

C----------------------------------------------------------------------
C  D. G. CASSEL 08.02.80
C
C CFMTRX calculates the elements of the H matrix used in CIRFIT
C
C  The matrix can be ill-conditioned in the sense that the
C  determinant is badly determined. Since the chisquare is the
C  determinant, when properly normalized, the calculation is done
C  in DOUBLE PRECISION.
C
C H is calculated in the coordinate system where :
C
C  <X>    = 0
C  <Y>    = 0
C  <XY>   = 0
C  <R**2> = 1
C
C  In this coordinate system only 5 elements of H are neither 0 or 1
C
C  they are : <X**2>, <Y**2>, <X R**2>, <Y R**2> & <R**4>
C
C  <X> and <Y> before translation are needed to get back to the lab
C
C  COSROT & SINROT are needed to get back to the lab
C
C  SIGN(Q) can be determined with the signs chosen for CROT & SINROT
C
C  WSUM   is required to scale the CHISQUARE
C  RSCALE is required to scale GAMMA & KAPPA
C
C First calculate <X> and <Y> in the original coordinate system
C----------------------------------------------------------------------

* ----------Executable code starts here---------------------

      WSUM   = 0.00
      XAV    = 0.00
      YAV    = 0.00
      NHITCF= 0

      DO 10 IFIT = 1, NFIT
         WI     = WGTFIT(IFIT)
         XI     = XFIT  (IFIT)
         YI     = YFIT  (IFIT)
         IF((IADFIT(IFIT).NE.0).AND.(STRFFT(IFIT).GT.0))THEN
            NHITCF=  NHITCF + 1
            WSUM   = WSUM   + WI
            XAV    = XAV    + WI*XI
            YAV    = YAV    + WI*YI
         ENDIF
 10   CONTINUE

      IF(NHITCF.GE.MNHTCF)THEN

         IERR=0
         XAV    = XAV    / WSUM
         YAV    = YAV    / WSUM

C Calculate <X**2>, <XY>, and <Y**2> with <X> = 0, & <Y> = 0

         XXAV   = 0.00
         XYAV   = 0.00
         YYAV   = 0.00

         DO 20 IFIT = 1, NFIT
            IF((IADFIT(IFIT).NE.0).AND.(STRFFT(IFIT).GT.0))THEN
               WI     = WGTFIT(IFIT)
               XI     = XFIT  (IFIT)
               YI     = YFIT  (IFIT)
               XI     = XI - XAV
               YI     = YI - YAV
               WIXI   = WI*XI
               WIYI   = WI*YI
               XXAV   = XXAV   + WIXI*XI
               XYAV   = XYAV   + WIXI*YI
               YYAV   = YYAV   + WIYI*YI
            ENDIF
 20      CONTINUE
         
         XXAV   = XXAV   / WSUM
         XYAV   = XYAV   / WSUM
         YYAV   = YYAV   / WSUM
         
C----------------------------------------------------------------------
C Rotate coordinates so that <XY> = 0
C  SIGN(C**2 - S**2) = SIGN(XXAV - YYAV) >
C  &                                     > ==> New : (XXAV-YYAV) > 0
C  SIGN(S) = SIGN(XYAV)                  >
C----------------------------------------------------------------------

         A      = ABS( XXAV - YYAV )
         B      = 4.0*XYAV*XYAV
         
         ASQPB  = A**2 + B
         RASQPB = SQRT(ASQPB)
         
         SPLUS  = 1.00 + A / RASQPB
         SMINUS = B / (ASQPB*SPLUS)
         
         SPLUS  = SQRT(0.5*SPLUS )
         SMINUS = SQRT(0.5*SMINUS)

C First require : SIGN(C**2 - S**2) = SIGN(XXAV - YYAV)

         IF( XXAV .LE. YYAV ) THEN
            COSROT = SMINUS
            SINROT = SPLUS
         ELSE
            COSROT = SPLUS
            SINROT = SMINUS
         ENDIF

C Require : SIGN(S) = SIGN(XYAV)*SIGN(C) (Assuming SIGN(C) > 0)

         IF( XYAV  .LT. 0.00 ) SINROT = - SINROT

C----------------------------------------------------------------------
C We now have the smallest angle that guarantees <X**2> > <Y**2>
C
C  To get the SIGN of the charge right, the new X-AXIS must point
C  outward from the orgin.  We are free to change signs of both
C  COSROT and SINROT simultaneously to accomplish this.
C
C  Choose SIGN of C wisely to be able to get the sign of the charge
C----------------------------------------------------------------------

         IF( (COSROT*XAV + SINROT*YAV) .LE. 0.00 ) THEN
            COSROT = - COSROT
            SINROT = - SINROT
         ENDIF
         
C        IF(LDEBUG)WRITE(IUNLPT,130)XAV,YAV,XXAV,XYAV,YYAV,COSROT,SINROT

C Now get <R**2> and RSCALE = SQRT(<R**2>)

         RRAV   = XXAV  + YYAV
         RSCALE = SQRT(RRAV)
         
         XXAV   = 0.00
         YYAV   = 0.00
         XRRAV  = 0.00
         YRRAV  = 0.00
         RRRRAV = 0.00
         XYAV   = 0.00

         DO 30 IFIT = 1, NFIT
            IF((IADFIT(IFIT).NE.0).AND.(STRFFT(IFIT).GT.0))THEN
               WI     = 0.00
               XOLD   = 0.00
               YOLD   = 0.00
               WI     = WGTFIT(IFIT)
               XOLD   = XFIT  (IFIT)
               YOLD   = YFIT  (IFIT)
               XOLD   = XOLD - XAV
               YOLD   = YOLD - YAV
            
C Rotate so that <XY> = 0 & divide by RSCALE so that <R**2> = 1

               XI     = (   COSROT*XOLD  + SINROT*YOLD ) / RSCALE
               YI     = ( - SINROT*XOLD  + COSROT*YOLD ) / RSCALE
               
               XIXI   = XI  *XI
               YIYI   = YI  *YI
               RIRI   = XIXI + YIYI

               WIRIRI = WI  *RIRI

               XYAV   = XYAV   + WI    *XI*YI
               XXAV   = XXAV   + WI    *XIXI
               YYAV   = YYAV   + WI    *YIYI
            
               XRRAV  = XRRAV  + WIRIRI*XI
               YRRAV  = YRRAV  + WIRIRI*YI
               RRRRAV = RRRRAV + WIRIRI*RIRI
            ENDIF
 30      CONTINUE

C Divide by WSUM to make averages

         XXAV   = XXAV   / WSUM
         YYAV   = YYAV   / WSUM
         XRRAV  = XRRAV  / WSUM
         YRRAV  = YRRAV  / WSUM
         RRRRAV = RRRRAV / WSUM
         XYAV   = XYAV   / WSUM

C        LDEBUG = LDEBUG.OR.(XXAV.LT.YYAV)
C        IF (.NOT.LDEBUG) RETURN
C        CHECK  = XXAV + YYAV - 1.00
C        WRITE(IUNLPT,140) XXAV,XYAV,YYAV,CHECK,XRRAV,YRRAV,RRRRAV

      ELSE
         IERR=1
      ENDIF

      RETURN

C 130 FORMAT(/' TRANS :        XAV         YAV        XXAV        XYAV',
C   +        '        YYAV           C           S'//' ',6X,1P8E12.4)
C 140 FORMAT(/' ROTAT :       XXAV        XYAV        YYAV       CHECK',
C   +        '       XRRAV       YRRAV      RRRRAV'//' ',6X,1P8E12.4)

      END

