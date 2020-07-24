*
* $Id: tf3fit_initialize_event.F,v 1.1 2001/11/19 23:40:14 dpp Exp $
*
* $Log: tf3fit_initialize_event.F,v $
* Revision 1.1  2001/11/19 23:40:14  dpp
*      -> NEW ROUTINE
*      -> initialize tf3fit control variables
*      -> split off from DUFIND
*      -> remove call to kalman fitter and the logical switc
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TF3FIT_INITIALIZE_EVENT
C.......................................................................
C.
C. TF3FIT_INITIALIZE_EVENT - initialize TF3FIT variables for each event
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
#include "doit/duseq/tffitctl.inc"

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      LCALIB   = .FALSE.
      DUALTR   = .FALSE.

      RETURN
      END
