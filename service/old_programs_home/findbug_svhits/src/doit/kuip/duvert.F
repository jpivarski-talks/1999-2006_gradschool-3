*
* $Id: duvert.F,v 1.1 1998/02/24 18:49:56 dpp Exp $
*
* $Log: duvert.F,v $
* Revision 1.1  1998/02/24 18:49:56  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 23/09/91  11.41.15  by  Steve Schaffner
*CMZ :  5.53/04 14/06/90  12.48.57  by  Steve Schaffner
*CMZ :  5.52/01 16/03/90  16.07.20  by  Steve Schaffner
*-- Author :    Steve Schaffner   15/03/90
      SUBROUTINE DUVERT
C.......................................................................
C.
C. DUVERT - Turn on and off use of DUVFND (called as KUIP command)
C.
C. Inputs    : ION -- .eq.0 => turn off use of DUVFND (average instead)
C.                    .ne.0 => turn on (default)
C.
C. Author    : Steve Schaffner  15/03/90  15.48.22
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
      SAVE /TFCTLL/
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DUVERT' )

      INTEGER ION
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      CALL KUGETI(ION)
      IF (ION.EQ.0) THEN
         LVERT = .FALSE.
      ELSE
         LVERT = .TRUE.
      END IF

      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
