* $Id: fast_trak_initialize_evnt.F,v 1.3 2003/03/05 17:09:42 dpp Exp $
*
* $Log: fast_trak_initialize_evnt.F,v $
* Revision 1.3  2003/03/05 17:09:42  dpp
*      -> comment
*
* Revision 1.2  2001/11/19 22:52:54  dpp
*      -> remove direct access of the event control parameter
*      -> fill fast_trak variable for event control
*
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
#include "doit/duseq/controlpar.inc"
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

C Reset of L_FT_DOIT_FULL is required here; 
C it might be changed after call to fast_trak_initialize_cuts. 

      CALL EVENT_CONTROL_BITS(EXB_DOIT_FULL,L_FT_DOIT_FULL)

C-----------------------------------------------------------------------
C cuts for local ambiguity
C-----------------------------------------------------------------------

C MAX_ITERATE..........maximum iterations through local ambiguity
      IF(L_FT_DOIT_FULL)THEN
         MAX_ITERATE=2
      ELSE
         MAX_ITERATE=1
      ENDIF

C EXECUTE_AMBIGUITY....flag that the ambiguity part should be executed
      EXECUTE_AMBIGUITY=.TRUE.

C EXECUTE_ALTERNATE....flag that alternate ambiguity solutiuns should be tested
      IF(L_FT_DOIT_FULL)THEN
         EXECUTE_ALTERNATE=.TRUE.
      ELSE
         EXECUTE_ALTERNATE=.FALSE.
      ENDIF

C-----------------------------------------------------------------------
      RETURN
      END






