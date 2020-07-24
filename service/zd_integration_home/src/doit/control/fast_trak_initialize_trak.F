* $Id: fast_trak_initialize_trak.F,v 1.2 2002/05/23 14:16:00 dpp Exp $
*
* $Log: fast_trak_initialize_trak.F,v $
* Revision 1.2  2002/05/23 14:16:00  dpp
*      -> set the radius limit
*
* Revision 1.1  2001/11/19 22:56:16  dpp
*     -> NEW ROUTINE
*      -> access control variables
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_INITIALIZE_TRAK
C.......................................................................
C.
C. FAST_TRAK_INITIALIZE_EVNT - things that need to be initialized for 
C.                             each  TRACK
C. COMMON    : 
C. CALLS     : 
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split from DUFIND 18-Oct-01
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C  Executable code starts here
C-----------------------------------------------------------------------

      F_T_DZ_WINDOWS=.FALSE.
      RADIUS_LIMIT=9.99

      RETURN
      END






