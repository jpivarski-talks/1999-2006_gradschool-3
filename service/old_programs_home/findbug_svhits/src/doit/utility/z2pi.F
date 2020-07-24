*
* $Id: z2pi.F,v 1.2 1998/02/24 19:11:57 dpp Exp $
*
* $Log: z2pi.F,v $
* Revision 1.2  1998/02/24 19:11:57  dpp
* multipass
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
*CMZ :  5.54/07 21/05/91  14.40.55  by  Steve Schaffner
*CMZ :  5.52/01 22/03/90  20.47.31  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  13.17.09  by  Steve Schaffner
*CMZ :  5.50/00 24/04/89  13.26.15  by  Steve Schaffner
*-- Author :
      REAL FUNCTION Z2PI( ARG )
C.......................................................................
C.
C. Z2PI   - Return ARG in range 0 to 2pi
C.
C. COMMON    : ?
C. CALLS     :
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 06-Apr-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
      REAL ARG

#include "doit/duseq/tfconspa.inc"

* ----------Executable code starts here---------------------

      Z2PI = ARG

 11   IF (Z2PI .LT.  0.0 ) THEN
         Z2PI = Z2PI + TWOPI
         GO TO 11
      ENDIF

 21   IF (Z2PI .GE. TWOPI)THEN
         Z2PI = Z2PI - TWOPI
         GO TO 21
      ENDIF

      RETURN
      END
