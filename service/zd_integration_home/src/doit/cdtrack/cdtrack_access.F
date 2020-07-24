*
* $Id: cdtrack_access.F,v 1.1 2001/11/19 22:43:14 dpp Exp $
*
* $Log: cdtrack_access.F,v $
* Revision 1.1  2001/11/19 22:43:14  dpp
*      -> NEW ROUTINE
*      -> access values in CDTRACK commons
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE CDTRACK_ACCESS(ACTION,INDEX,IVAL)
C.......................................................................
C.
C. CDTRACK_ACCESS - access CDTRACK variables
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 22-Oct-2001
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cdtrakcd.inc"

      INTEGER ACTION,INDEX,IVAL
      INTEGER VAL_INT
      REAL VAL_REAL
      EQUIVALENCE (VAL_INT,VAL_REAL)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


C-----------------------------------------------------------------------
C increment the seed track number
C-----------------------------------------------------------------------
      IF(ACTION.EQ.CDT_INCR_SEED)THEN
         NTRACK=NTRACK+1
         IVAL=NTRACK

C-----------------------------------------------------------------------
C increment the seed track number
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.CDT_NTRKCD)THEN
         IVAL=NTRKCD

C-----------------------------------------------------------------------
      ENDIF
      RETURN
      END


