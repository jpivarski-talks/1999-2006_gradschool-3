*
* $Id: dubnfl.F,v 1.2 1999/11/29 21:43:49 lyon Exp $
*
* $Log: dubnfl.F,v $
* Revision 1.2  1999/11/29 21:43:49  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1  1998/02/24 18:49:52  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE DUBNFL
C.......................................................................
C.
C. DUNAME - Turn on and off DUET bunch finder
C.
C. Inputs    : ION -- .ne.1 => turn off DUET bunchfinder
C.                    .eq.1 => turn on DUET bunchfinder
C.
C. Author    : Peter Kim    13/2/97
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/utilseq/cdbnc100.inc"

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DUBNFL' )

      INTEGER ION

      CALL KUGETI(ION)
      IF (ION.EQ.1) THEN
         IFLDBN = 1
      ELSE
         IFLDBN = 0
      END IF

      RETURN
      END
