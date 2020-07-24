*
* $Id: duklmn.F,v 1.2 1999/05/28 03:04:59 lyon Exp $
*
* $Log: duklmn.F,v $
* Revision 1.2  1999/05/28 03:04:59  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.1  1998/02/24 18:49:53  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE DUKLMN
C.......................................................................
C.
C. DUNAME - Turn on and off Kalman filter.
C.
C. Inputs    : ION -- .ne.1 => turn off Kalman filter
C.                    .eq.1 => turn on Kalman filter.
C.
C. Author    : Rob Kutschke, 15/06/95
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DUKLMN' )

      INTEGER ION

      CALL KUGETI(ION)
      IF (ION.EQ.1) THEN
         LKALMN = .TRUE.
         PRINT*, 'DANGER - KALMAN IS NO LONGER A PART OF DOIT'
         PRINT*, '         KALMAN WILL BE TURNED OFF'
         LKALMN = .FALSE.
      ELSE
         LKALMN = .FALSE.
      END IF

      RETURN
      END
