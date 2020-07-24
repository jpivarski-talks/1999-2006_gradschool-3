*
* $Id: cdpntr.F,v 1.2 2001/11/20 00:11:39 dpp Exp $
*
* $Log: cdpntr.F,v $
* Revision 1.2  2001/11/20 00:11:39  dpp
*      -> delete unused include fil
*
* Revision 1.1.1.1  1998/02/06 19:11:47  dpp
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
*CMZ :  5.54/07 28/05/91  13.39.12  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.19  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.50.51  by  R. Namjoshi
*-- Author :
      SUBROUTINE CDPNTR( IFIRST, INEXT, INDEL )
C.......................................................................
C.
C. CDPNTR - Delete a pointer from a list of pointers
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
      INTEGER IFIRST, INDEL, INEXT(*)
C
C== Local variables
C
      INTEGER IND, INDX

* ----------Executable code starts here---------------------

C Take the list of pointers, INEXT, beginning with IFIRST and delete
C the pointer INDEL

      IF( IFIRST.EQ.INDEL ) IFIRST = INEXT(IFIRST)
      INDX   = IFIRST

10    IF( INDX.GT.0 ) THEN
          IND    = INDX
          INDX   = INEXT(INDX)
          IF( INDX.EQ.INDEL ) INDX = INEXT(INDX)
          INEXT(IND) = INDX
          GOTO 10
      ENDIF

      RETURN
      END


