*
* $Id: xts_abortval.F,v 1.1 2001/11/19 23:13:54 dpp Exp $
*
* $Log: xts_abortval.F,v $
* Revision 1.1  2001/11/19 23:13:54  dpp
*      -> NEW
*      -> function to access xts_abort which is in common
*      -> must get rid of the entries someday
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
* function to extract the input value from XTS_RFVIEW
      INTEGER FUNCTION XTS_ABORTVAL(DIRECT)
      IMPLICIT NONE
      SAVE
#include "doit/sfseq/xts_cntl.inc"
      INTEGER DIRECT

      IF(DIRECT.EQ.0)XTS_ABORT=0

      XTS_ABORTVAL=XTS_ABORT

      RETURN
      END




