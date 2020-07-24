* $Id: fast_trak_initialize_evnt.F,v 1.1 2000/02/18 21:21:48 dpp Exp $
*
* $Log: fast_trak_initialize_evnt.F,v $
* Revision 1.1  2000/02/18 21:21:48  dpp
*      -> NEW
*      -> reset Doit/DoitFast dependent variables for each event
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_INITIALIZE_EVNT
C.......................................................................
C.
C. FAST_TRAK_INITIALIZE_EVNT - things that need to be initialized for 
C.                             each  event to avoid interference 
C.                             between DOIT and DOITFAST
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split from FAST_TRAK 26-Oct-99
C. LAST MOD  : 15-Sept-88
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfstakcd.inc"

#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C  Executable code starts here
C-----------------------------------------------------------------------
C-----------------------------------------------------------------------
C cuts for local ambiguity
C-----------------------------------------------------------------------

C MAX_ITERATE..........maximum iterations through local ambiguity
         IF(NEXPT.GE.EXB_DOIT_FULL)THEN
            MAX_ITERATE=2
         ELSE
            MAX_ITERATE=1
         ENDIF

C EXECUTE_AMBIGUITY....flag that the ambiguity part should be executed
         EXECUTE_AMBIGUITY=.TRUE.

C EXECUTE_ALTERNATE....flag that alternate ambiguity solutiuns should be tested
         IF(NEXPT.GE.EXB_DOIT_FULL)THEN
            EXECUTE_ALTERNATE=.TRUE.
         ELSE
            EXECUTE_ALTERNATE=.FALSE.
         ENDIF



      RETURN
      END






