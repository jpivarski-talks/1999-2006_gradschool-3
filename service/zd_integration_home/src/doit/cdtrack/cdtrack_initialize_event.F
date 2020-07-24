*
* $Id: cdtrack_initialize_event.F,v 1.1 2001/11/19 22:42:48 dpp Exp $
*
* $Log: cdtrack_initialize_event.F,v $
* Revision 1.1  2001/11/19 22:42:48  dpp
*      -> NEW ROUTINE
*      -> initialize cdtrack counters for the event
*      -> split off from DUFIND
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE CDTRACK_INITIALIZE_EVENT
C.......................................................................
C.
C. CDTRACK_INITIALIZE_EVENT - initialize cdtrack variables for each event
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
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cdtrakcd.inc"

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      NTRKIN=0
      NTRACK = NTRKIN
      NTRKCD = 0

      RETURN
      END




