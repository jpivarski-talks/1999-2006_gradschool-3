*
* $Id: incsrr.F,v 1.2 2001/11/20 00:15:23 dpp Exp $
*
* $Log: incsrr.F,v $
* Revision 1.2  2001/11/20 00:15:23  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:44  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:42  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  14.40.53  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  13.17.05  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.50.35  by  R. Namjoshi
*-- Author :
      SUBROUTINE INCSRR( IFIRST, INEXT, RKEY )
C.......................................................................
C.
C. INCSRR - Perform ascending REAL sort using RKEY
C.
C. COMMON    : None
C. CALLS     : None
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 11-May-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
C== Argument declarations
C
      INTEGER IFIRST, INEXT(*)
      REAL             RKEY(*)
C
C== Local variables
C
      INTEGER I, J, K, L, M

* ----------Executable code starts here---------------------

      L = IFIRST
      M = IFIRST
      I = INEXT(M)
C
10    IF( I .GT. 0 ) THEN
          IF( RKEY(I) .GE. RKEY(M) ) THEN
              M = I
          ELSE
              INEXT(M) = INEXT(I)
              IF( RKEY(I) .LT. RKEY(L) ) THEN
                  INEXT(I) = L
                  L = I
              ELSE
                  K = L
                  J = INEXT(K)
20                IF( RKEY(I) .GE. RKEY(J) ) THEN
                      K = J
                      J = INEXT(K)
                      GOTO 20
                  ENDIF
                  INEXT(K) = I
                  INEXT(I) = J
              ENDIF
          ENDIF
          I = INEXT(M)
          GOTO 10
      ENDIF

      IFIRST = L

      RETURN
      END
