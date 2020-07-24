*
* $Id: sfaccess.F,v 1.1 2001/11/20 00:04:11 dpp Exp $
*
* $Log: sfaccess.F,v $
* Revision 1.1  2001/11/20 00:04:11  dpp
*      -> NEW ROUTINE
*      -> access information from sf
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE SFACCESS(ACTION,INDEX,IVAL)
C......................................................................
C.
C. SFACCESS- access status information from SF
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 15-Oct-2001
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/controlpar.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"

      INTEGER ACTION,INDEX,IVAL
      INTEGER VAL_INT
      REAL VAL_REAL
      EQUIVALENCE (VAL_INT,VAL_REAL)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C-----------------------------------------------------------------------
C executable code starts here
C-----------------------------------------------------------------------

C-----------------------------------------------------------------------
C number of layers in SF geometry
C-----------------------------------------------------------------------
      IF(ACTION.EQ.SFACCESS_NLAYSF)THEN
         IVAL=NLAYSF

C-----------------------------------------------------------------------
C number of chains found
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.SFACCESS_NCHAIN)THEN
         IVAL=NCHAIN

C-----------------------------------------------------------------------
      ENDIF
      RETURN
      END




