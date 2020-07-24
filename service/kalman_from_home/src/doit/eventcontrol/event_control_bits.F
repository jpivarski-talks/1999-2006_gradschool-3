*
* $Id: event_control_bits.F,v 1.1 2001/11/19 23:38:10 dpp Exp $
*
* $Log: event_control_bits.F,v $
* Revision 1.1  2001/11/19 23:38:10  dpp
*      -> NEW ROUTINE
*      -> access the event control parameter set for the process/producer
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :
      SUBROUTINE EVENT_CONTROL_BITS(BIT,L_GOT_IT)
C.......................................................................
C.
C. EVENT_CONTROL_BITS - test if designated bits are 
C.                      in the run control parameter
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    :
C.
C. VERSION   : 1.0
C. CREATED   : 15-Oct-01
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
      INTEGER BIT
      LOGICAL L_GOT_IT

      IF(IAND(NEXPT,BIT).EQ.BIT)THEN
         L_GOT_IT=.TRUE.
      ELSE
         L_GOT_IT=.FALSE.
      ENDIF 

      RETURN
      END








