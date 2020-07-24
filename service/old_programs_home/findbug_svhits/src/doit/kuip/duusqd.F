*
* $Id: duusqd.F,v 1.1 1998/02/24 18:49:55 dpp Exp $
*
* $Log: duusqd.F,v $
* Revision 1.1  1998/02/24 18:49:55  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/15 29/09/92  19.47.49  by  Rob Kutschke
*CMZ :  5.54/10 19/12/91  17.27.15  by  Steve Schaffner
*CMZ :  5.53/04 14/06/90  12.48.57  by  Steve Schaffner
*CMZ :  5.52/01 16/03/90  16.07.20  by  Steve Schaffner
*-- Author :    Steve Schaffner   15/03/90
      SUBROUTINE DUUSQD
C.......................................................................
C.
C. DUNAME - Turn on and off charge division (called as KUIP command)
C.
C. Inputs    : ION -- .ne.1 => turn off charge division
C.                    .eq.1 => turn on charge division
C.
C. Author    : Steve Schaffner  15/03/90  15.48.22
C.
C. Modfied   : Rob Kutscke, 29/09/92
C.             In order than one can have mixed before and after
C.             QDIV runs in one file, we need two variables.
C.             This routine might be called before any data is read.
C.             In that case the run number is not defined; therefore
C.             LUSEQD cannot be set .TRUE. in this routine.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DUUSQD' )

      INTEGER ION
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      CALL KUGETI(ION)
      IF (ION.EQ.1) THEN
         LUSQD1 = .TRUE.
      ELSE
         LUSQD1 = .FALSE.
         LUSEQD = .FALSE.
      END IF


      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
