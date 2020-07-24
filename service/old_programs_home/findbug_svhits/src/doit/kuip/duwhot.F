*
* $Id: duwhot.F,v 1.1 1998/02/24 18:49:57 dpp Exp $
*
* $Log: duwhot.F,v $
* Revision 1.1  1998/02/24 18:49:57  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE DUWHOT
C.......................................................................
C.
C. DUNAME - Turn on and off writing out of htis on tracks.
C.
C. Inputs    : ION -- .ne.1 => write out of hits on tracks
C.                    .eq.1 => do not write out hits on tracks
C.
C. Author    : Rob Kutschke, 17/05/96
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DUWHOT' )

      INTEGER ION

      CALL KUGETI(ION)
      IF (ION.EQ.1) THEN
         LWHITS = .TRUE.
      ELSE
         LWHITS = .FALSE.
      END IF

      RETURN
      END
