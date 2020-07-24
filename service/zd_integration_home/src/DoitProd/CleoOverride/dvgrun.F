*
* $Id: dvgrun.F,v 1.1.1.1 1999/09/08 18:53:34 lyon Exp $
*
* $Log: dvgrun.F,v $
* Revision 1.1.1.1  1999/09/08 18:53:34  lyon
* imported DoitProd sources
*
* Revision 1.1.1.1  1998/07/26 19:45:39  lyon
* DoitProcessor (Wrapped DOIT)
*
* Revision 1.1.1.1  1994/10/04 22:14:12  zfiles
* first version of dvlib in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  1.00/00 23/08/90  22.42.53  by  Paul Avery
*-- Author :
      SUBROUTINE DVGRUNALL(IRUN, IEVT)

C  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
C     Returns the run and event number to the caller
C
C *IRUN      integer variable (write)
C            Run number
C
C *IEVT      integer variable (write)
C            Event number
C  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
#include "dvlib/seq/dvstatus.inc"

C     Calling arguments
      INTEGER IRUN, IEVT

C     Local variables

C     External declarations
C  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

      IRUN = IRUNDV
      IEVT = IEVTDV

      RETURN
      END
