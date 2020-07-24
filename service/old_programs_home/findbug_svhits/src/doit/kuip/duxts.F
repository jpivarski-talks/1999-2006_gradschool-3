*
* $Id: duxts.F,v 1.1 1998/02/24 18:49:58 dpp Exp $
*
* $Log: duxts.F,v $
* Revision 1.1  1998/02/24 18:49:58  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/14 05/05/92  12.46.04  by  Steve Schaffner
*CMZ :  5.53/04 14/06/90  12.48.57  by  Steve Schaffner
*CMZ :  5.52/01 16/03/90  16.07.20  by  Steve Schaffner
*-- Author :    Steve Schaffner   15/03/90
      SUBROUTINE DUXTS
C.......................................................................
C.
C. DUNAME - Turn on and off reading of XTSUBS plotting (called as KUIP command)
C.
C. Inputs    : ION -- .ne.1 => turn off plotting
C.                    .eq.1 => turn on plotting
C.
C. Author    : Steve Schaffner  15/03/90  15.48.22
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
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
         LXTSUB = .TRUE.
      ELSE
         LXTSUB = .FALSE.
      END IF

#if defined(CLEO_XTSUBS)
C     Initialize XTSUBS plotting
      IF (LXTSUB) THEN
         CALL XTS_TRAK_INIT('TF',1,1,'DUETBR')
         CALL XTS_TRAK_INIT('TF',2,1,'DUETBR')
         CALL XTS_TRAK_INIT('TF',3,1,'DUETBR')
         CALL XTS_TRAK_INIT('CD',1,1,'DUETBR')
         CALL XTS_TRAK_INIT('CD',2,1,'DUETBR')
         CALL XTS_TRAK_INIT('CD',3,1,'DUETBR')
      END IF
#endif

      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
