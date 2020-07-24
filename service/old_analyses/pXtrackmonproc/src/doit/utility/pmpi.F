*
* $Id: pmpi.F,v 1.1 1998/02/24 19:09:24 dpp Exp $
*
* $Log: pmpi.F,v $
* Revision 1.1  1998/02/24 19:09:24  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      REAL FUNCTION PMPI(ARG)

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfconspa.inc"
      REAL ARG


* ----------Executable code starts here---------------------

      PMPI = ARG

 11   IF (PMPI .LT.  -PI ) THEN
         PMPI = PMPI + TWOPI
         GO TO 11
      ENDIF

 21   IF (PMPI .GE.   PI )THEN
         PMPI = PMPI - TWOPI
         GO TO 21
      ENDIF

      RETURN
      END
