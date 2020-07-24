*      Include file runev
*
* begin_doc
*
*  $Id: runev.inc,v 1.1 1999/05/28 03:06:54 lyon Exp $
*
*  $Log: runev.inc,v $
*  Revision 1.1  1999/05/28 03:06:54  lyon
*  Needed to pass run and event number
*
*
*  Documentation for include file runev
*
*  Purpose:  Holds current run and event number for DOIT routines
*  --------
*
*  Used in routines:
*  ----------------
*
*  Notes:
*  ------
*
*  Author:   Adam Lyon      Created:  Fri May 21 10:57:23 EDT 1999
*  -------
*
*  Major revisions:
*  ----------------
*
* end_doc
*
*
*  Remember NO NESTED INCLUDES (except for parameters)
*
*  COMMON BLOCK NAME should be the same as include file name!
*
* begin_var
*  variables for include file runev:
*  -----------------------------------
*
*----6----------------------------------------------------------------72
*  
*  Variables contained in runev common block
*  --------------------------------------------
      INTEGER current_run, current_event

*
* end_var
*
       COMMON/RUNEV/ current_run, current_event




