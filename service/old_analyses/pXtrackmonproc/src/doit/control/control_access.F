*
* $Id: control_access.F,v 1.1 2001/11/19 22:56:50 dpp Exp $
*
* $Log: control_access.F,v $
* Revision 1.1  2001/11/19 22:56:50  dpp
*      -> NEW ROUTINE
*      -> access control variables
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE CONTROL_ACCESS(ACTION,INDEX,IVAL)
C.......................................................................
C.
C. CONTROL_ACCESS - access various values internal to various commons
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.
C. CREATED   : 22-Oct-2001
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfxfitcd.inc"

      INTEGER ACTION,INDEX,IVAL
      INTEGER VAL_INT
      REAL VAL_REAL
      EQUIVALENCE (VAL_INT,VAL_REAL)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------


C-----------------------------------------------------------------------
C set the TFXFIT view switch
C-----------------------------------------------------------------------
      IF(ACTION.EQ.CONTROL_ACCESS_SET_TFXS)THEN
         TFXS=IVAL

C-----------------------------------------------------------------------
      ENDIF
      RETURN
      END
