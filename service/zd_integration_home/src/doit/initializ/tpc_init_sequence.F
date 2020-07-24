*
* $Id: tpc_init_sequence.F,v 1.1 2003/03/05 18:04:35 dpp Exp $
*
* $Log: tpc_init_sequence.F,v $
* Revision 1.1  2003/03/05 18:04:35  dpp
*      -> NEW ROUTINE
*      -> call the other doit initialization normally called from C++
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TPC_INIT_SEQUENCE
C.......................................................................
C.
C. TPC_INIT_SEQUENCE - Initialize quantities for TPC trackfinding
C.
C. COMMON    : 
C. CALLS     : TPC_INIT_CDGEOM DOITNL TFINIT TFGMI2 TFXFIN CALL SFINIT
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 22-Oct-2002
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"

#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfunitcd.inc"
#include "doit/duseq/runev.inc"

      integer i

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

C this sets the doit status bits
      NEXPT=1056

      current_run=123456
      current_event=0

      CALL TPC_INIT_CDGEOM

      CALL DOITNL
      CALL TFINIT
      CALL TFGMI2
c      CALL TFXFIN
      CALL SFINIT('tpc...','SHORT')

      RETURN
      END






