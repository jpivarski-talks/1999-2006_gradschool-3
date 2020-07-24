*
* $Id: svd_initialize_event.F,v 1.1 2001/11/20 00:07:05 dpp Exp $
*
* $Log: svd_initialize_event.F,v $
* Revision 1.1  2001/11/20 00:07:05  dpp
*      -> NEW ROUTINE
*      -> initialize silicon data structure for the event
*      -> split off from DUFIND
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE SVD_INITIALIZE_EVENT
C.......................................................................
C.
C. SVD_INITIALIZE_EVENT - initialize silicon variables for each event
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 10-Oct-2001
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/svscratch.inc"

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      CALL VFILL(SV_AVAIL,SV_NHITS,1)

      RETURN
      END





