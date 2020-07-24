*
* $Id: invmlt.F,v 1.2 2000/08/18 19:00:34 dpp Exp $
*
* $Log: invmlt.F,v $
* Revision 1.2  2000/08/18 19:00:34  dpp
*      -> new argument for INVERS
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.1  1996/01/24 21:48:55  zfiles
* initial version: invert square matrix and multiply column
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE INVMLT(MATSQR,MATLIN,ANSWER,MVAR,NVARF,IERR)
C.......................................................................
C.
C. INVMLT    : invert and multiply: ANSWER=(MATSQR)**-1 * MATLIN
C.
C. COMMON    : none
C. CALLS     : none
C. CALLED    : PARXTR
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 18-Dec-95
C. LAST MOD  : 
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE


C arguments

      REAL MATSQR(*),MATLIN(*),ANSWER(*)
      INTEGER MVAR,NVARF,IERR

C internal values; reals are double precision

      REAL*8 TMPSQR(30,30),TMPLIN(30),TMPANS(30)
      REAL*8 WORK  (30,30)
      REAL*8 DET,DET_RET
      INTEGER ERR_INVERS
      INTEGER II,JJ,INDEX

      INDEX(II,JJ)=II+(JJ-1)*MVAR

      IERR=11
      IF(NVARF.LT.1 )GO TO 900
      IERR=12
      IF(NVARF.GT.30)GO TO 900

      DO 9 II=1,NVARF
      TMPLIN(II)   =MATLIN(II)
      DO 7 JJ=1,NVARF
7     TMPSQR(II,JJ)=MATSQR(INDEX(II,JJ))
9     CONTINUE



      IF(NVARF.NE.1)GO TO 200
      WORK(1,1)=1./ TMPSQR(1,1)
      GO TO 500



200   IF(NVARF.NE.2)GO TO 300

      DET= TMPSQR(1,1)*TMPSQR(2,2)
     2    -TMPSQR(1,2)*TMPSQR(2,1)
      IF(DET.LT. 1.E-19)THEN
         IERR=21
         GO TO 900
      ENDIF
      WORK(1,1)= TMPSQR(2,2)/DET
      WORK(1,2)=-TMPSQR(1,2)/DET
      WORK(2,1)=-TMPSQR(2,1)/DET
      WORK(2,2)= TMPSQR(1,1)/DET
      GO TO 500


300   IF(NVARF.NE.3)GO TO 400

      DET= TMPSQR(1,1)*TMPSQR(2,2)*TMPSQR(3,3)
     2    +TMPSQR(1,2)*TMPSQR(2,3)*TMPSQR(3,1)
     3    +TMPSQR(1,3)*TMPSQR(2,1)*TMPSQR(3,2)
     4    -TMPSQR(1,1)*TMPSQR(2,3)*TMPSQR(3,2)
     5    -TMPSQR(1,2)*TMPSQR(2,1)*TMPSQR(3,3)
     6    -TMPSQR(1,3)*TMPSQR(2,2)*TMPSQR(3,1)
      IF(ABS(DET).LT. 1.E-19)THEN
         IERR=31
         GO TO 900
      ENDIF

      WORK(1,1)=( TMPSQR(2,2)*TMPSQR(3,3)-TMPSQR(2,3)*TMPSQR(3,2) )/DET
      WORK(1,2)=(-TMPSQR(1,2)*TMPSQR(3,3)+TMPSQR(1,3)*TMPSQR(3,2) )/DET
      WORK(1,3)=( TMPSQR(1,2)*TMPSQR(2,3)-TMPSQR(1,3)*TMPSQR(2,2) )/DET
      WORK(2,1)=(-TMPSQR(2,1)*TMPSQR(3,3)+TMPSQR(2,3)*TMPSQR(3,1) )/DET
      WORK(2,2)=( TMPSQR(1,1)*TMPSQR(3,3)-TMPSQR(1,3)*TMPSQR(3,1) )/DET
      WORK(2,3)=(-TMPSQR(1,1)*TMPSQR(2,3)+TMPSQR(1,3)*TMPSQR(2,1) )/DET
      WORK(3,1)=( TMPSQR(2,1)*TMPSQR(3,2)-TMPSQR(2,2)*TMPSQR(3,1) )/DET
      WORK(3,2)=(-TMPSQR(1,1)*TMPSQR(3,2)+TMPSQR(1,2)*TMPSQR(3,1) )/DET
      WORK(3,3)=( TMPSQR(1,1)*TMPSQR(2,2)-TMPSQR(1,2)*TMPSQR(2,1) )/DET

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

400   IF(NVARF.GT.30)THEN
         IERR=40
         GO TO 900
      ENDIF
      CALL INVERS(TMPSQR,WORK,30,NVARF,DET_RET,ERR_INVERS)
      IF(ERR_INVERS.NE.0)THEN
         IERR=ERR_INVERS+50
         GO TO 900
      ENDIF

500   CONTINUE
      DO 529 II=1,NVARF
      TMPANS(II)=0.
      DO 527 JJ=1,NVARF
527   TMPANS(II)=TMPANS(II)+WORK(II,JJ)*TMPLIN(JJ)
529   CONTINUE

      DO 539 II=1,NVARF
      ANSWER(II)          =TMPANS(II)
      DO 537 JJ=1,NVARF
537   MATSQR(INDEX(II,JJ))=WORK(II,JJ)
539   CONTINUE

      IERR=0

900   CONTINUE
      RETURN
      END










