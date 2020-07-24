*
* $Id: duname.F,v 1.1 1998/02/24 18:49:54 dpp Exp $
*
* $Log: duname.F,v $
* Revision 1.1  1998/02/24 18:49:54  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 06/11/91  13.38.37  by  Steve Schaffner
*CMZ :  5.53/04 14/06/90  12.48.57  by  Steve Schaffner
*CMZ :  5.52/01 16/03/90  16.07.20  by  Steve Schaffner
*-- Author :    Steve Schaffner   15/03/90
      SUBROUTINE DUNAME
C.......................................................................
C.
C. DUNAME - Turn on and off reading of DUET.NML (called as KUIP command)
C.
C. Inputs    : ION -- .eq.0 => turn off use of namelist (defaults instead)
C.                    .ne.0 => turn on namelist
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
      PARAMETER(    CRNAME = 'DUNAME' )

      INTEGER ION
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      CALL KUGETI(ION)
      IF (ION.EQ.1) THEN
         LNAME = .TRUE.
      ELSE
         LNAME = .FALSE.
      END IF

      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
