*
* $Id: xts_inputval.F,v 1.1 2001/11/19 23:13:21 dpp Exp $
*
* $Log: xts_inputval.F,v $
* Revision 1.1  2001/11/19 23:13:21  dpp
*      -> NEW
*      -> function to access xts_rfv_input which is an entry to xts_rfview
*      -> must get rid of the entries someday
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
* function to extract the input value from XTS_RFVIEW
      INTEGER FUNCTION XTS_INPUTVAL(DUM)
      IMPLICIT NONE
      SAVE
#include "doit/sfseq/xts_cntl.inc"
      INTEGER DUM

      CALL XTS_RFV_INPUT
      XTS_INPUTVAL=XTS_ANSWER_IY

      RETURN
      END



