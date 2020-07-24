*
* $Id: smxvec.F,v 1.1 2003/03/05 17:44:48 dpp Exp $
*
* $Log: smxvec.F,v $
* Revision 1.1  2003/03/05 17:44:48  dpp
*      -> NEW ROUTINE, moved from "fitter/."
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.2  2001/11/19 23:44:15  dpp
*     -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:28  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:49  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 10/10/91  14.53.06  by  Jon Urheim
* add UNIX flag to +SELF statement.
*CMZ :  5.54/04 21/05/91  14.40.54  by  Steve Schaffner
*CMZ :  5.53/03 17/04/90  12.14.02  by  D. Riley
*CMZ :  5.51/00 09/08/89  13.17.07  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.50.39  by  R. Namjoshi
*-- Author :
      SUBROUTINE SMXVEC( A, B, C, N )
C.......................................................................
C.
C. SMXVEC - Multiply column vector by symmetric matrix
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 01-Apr-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
C=======================================================================
C
C  MULTIPLY A COLUMN VECTOR BY A SYMMETRIC MATRIX:
C  C(I) = A(IJ)*B(J)
C
C  MATRIX A IS STORED IN PACKED LOWER TRIANGULAR FORM:
C
C  A  =  A(1)
C        A(2)  A(3)
C        A(4)  A(5)  A(6)
C        ............A(N(N+1)/2)
C
C  N IS THE ORDER OF THE MATRIX: ==> A HAS N(N+1)/2 ELEMENTS
C  DOUBLE PRECISION IS USED INTERNALLY ON THE VAX & IBM
C
C=======================================================================
C
      REAL  A(*),B(*),C(*)
      INTEGER IROW,N,I,J,JROW
C
#if defined(CLEO_PDP10)
      REAL  CC
#endif
#if defined(CLEO_VAX)||defined(CLEO_IBM)||defined(CLEO_DECS)||defined(CLEO_UNIX)
      DOUBLE PRECISION  CC
#endif

* ----------Executable code starts here---------------------

      IF( N.LE.0 ) RETURN
      IROW   = 0

      DO 10 I = 1, N
          CC     = 0
          IROW   = IROW + I - 1
          DO 20 J = 1, I
              CC = CC + A(IROW+J)*B(J)
20        CONTINUE
          IF( I.LT.N ) THEN
              JROW   = IROW + I
              DO 30 J = I+1, N
                  JROW   = JROW + J - 1
                  CC     = CC + A(JROW)*B(J)
30            CONTINUE
          ENDIF
          C(I)   = CC
10    CONTINUE

      RETURN
      END
