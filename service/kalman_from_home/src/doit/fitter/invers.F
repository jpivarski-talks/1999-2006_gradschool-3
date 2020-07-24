*
* $Id: invers.F,v 1.2 2000/08/18 18:57:49 dpp Exp $
*
* $Log: invers.F,v $
* Revision 1.2  2000/08/18 18:57:49  dpp
*      -> error handling for INVERS
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.1  1996/01/24 21:50:26  zfiles
* initial version: general matrix inversion
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE INVERS(MATSQR,MATINV,MVAR,NVUSE,DET,IERR)
C.......................................................................
C.
C. INVMLT    : invert: MATINV=(MATSQR)**-1
C.
C. COMMON    : none
C. CALLS     : none
C. CALLED    : none
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


C ARGUMENTS
      REAL*8  MATSQR(*),MATINV(*)
      REAL*8  DET
      INTEGER MVAR,NVUSE,IERR
C INTERNAL VALUES
      REAL*8  TMPS(30,30),WORK(30,30)
      REAL*8  XNORM,XIS2,X,RAT
      INTEGER I,IX,IM1,J,JX,K
      INTEGER INDEX
C STATEMENT FUNCTION
      INDEX(I,J)=I+(J-1)*MVAR
C EXECUTABLE
      IERR=1
      IF(NVUSE.GT.MVAR)GO TO 95
      IERR=2
      IF(NVUSE.GT.30)GO TO 95
      IERR=3
      IF(NVUSE.LT.1)GO TO 95
      IERR=4
      DET=1.
      XNORM=0.
      XIS2=0.
      DO 19 I=1,NVUSE
      DO 19 J=1,NVUSE
      TMPS(I,J)=MATSQR(INDEX(I,J))
      WORK(I,J)=0.
      IF(I.EQ.J)WORK(I,I)=1.
      X=TMPS(I,J)
      X=DABS(X)
      IF(X.LT.  1.D-20)GO TO 19
      X=DLOG(X)
      XNORM=XNORM+X
      XIS2=XIS2+1.
19    CONTINUE
      XNORM=XNORM/XIS2
      XNORM=DEXP(XNORM)
      DO 29 I=1,NVUSE
      DO 29 J=1,NVUSE
29    TMPS(I,J)=TMPS(I,J)/XNORM
      DO 79 I=1,NVUSE
      X=TMPS(I,I)
      IF(X.NE.0.)GO TO 51
      DO 37 J=I,NVUSE
      JX=J
      X=TMPS(J,I)
      IF(X.NE.0.)GO TO 41
37    CONTINUE
      IERR=5
      GO TO 95
41    IERR=6
      DO 47 J=1,NVUSE
      X=TMPS(I,J)
      TMPS(I,J)=TMPS(JX,J)
      TMPS(JX,J)=X
      X=WORK(I,J)
      WORK(I,J)=WORK(JX,J)
47    WORK(JX,J)=X
      DET=-DET
51    DO 59 J=I,NVUSE
      X=TMPS(J,I)
      IF(X.EQ.0.)GO TO 59
      DET=DET*X
      DO 57 K=1,NVUSE
      TMPS(J,K)=TMPS(J,K)/X
57    WORK(J,K)=WORK(J,K)/X
59    CONTINUE
      IF(I.EQ.NVUSE)GO TO 79
      IX=I+1
      DO 67 J=IX,NVUSE
      X=TMPS(J,I)
      IF(X.EQ.0.)GO TO 67
      DO 65 K=1,NVUSE
      TMPS(J,K)=TMPS(J,K)-TMPS(I,K)
65    WORK(J,K)=WORK(J,K)-WORK(I,K)
67    CONTINUE
79    CONTINUE
      IF(NVUSE.EQ.1)GO TO 89
      DO 87 IX=2,NVUSE
      I=NVUSE-IX+2
      IM1=I-1
      DO 85 J=1,IM1
      RAT=TMPS(J,I)/TMPS(I,I)
      DO 83 K=1,NVUSE
      TMPS(J,K)=TMPS(J,K)-RAT*TMPS(I,K)
83    WORK(J,K)=WORK(J,K)-RAT*WORK(I,K)
85    CONTINUE
87    CONTINUE
89    CONTINUE
      DO 93 I=1,NVUSE
      DO 93 J=1,NVUSE
93    MATINV(INDEX(I,J))=WORK(I,J)/XNORM
      DET=DET*XNORM**NVUSE
      IERR=0
95    RETURN
      END








