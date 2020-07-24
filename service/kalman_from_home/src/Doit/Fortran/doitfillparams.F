*
* $Id: doitfillparams.F,v 1.2 2000/02/08 18:33:48 lyon Exp $
*
* $Log: doitfillparams.F,v $
* Revision 1.2  2000/02/08 18:33:48  lyon
* Allow for no bunch finder running and DoitModeNumber
*
* Revision 1.1  1999/05/28 03:15:28  lyon
* Passed run, event number and bfield
*
*
      SUBROUTINE DOITFILLPARAMS(run, event, bfield, imode)
*
* begin_doc
*
*  Documentation for subroutine doitfillparams
*
*  Purpose: Fill some doit parameters
*  --------
*
*  Input Parameters:  (Name - Type - Meaning)
*  ----------------
*        run = current run number
*        event = current event number
*        bfield = bfield (signed)      
*        imode  = Doit mode number (e.g. Doit, DoitFast, etc)
*
*  Output Parameters:  (Name - Type - Meaning)
*  -----------------
*
*  Other routines:
*  ---------------
*
*  Notes:
*  ------

*
*  Author:   Adam Lyon      Created:  Fri May 21 11:23:09 EDT 1999
*  -------
*
*  Major revisions:
*  ----------------
*     
*
* end_doc
*
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
#include "doit/duseq/runev.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"      
#include "doit/duseq/tfctlcde.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
*
      INTEGER run, event, imode
      REAL bfield

*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitfillparams.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.2 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/02/08 18:33:48 $')
      PARAMETER(CHRCSA='$Author: lyon $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITFILLPARAMS' )
*
*  Local User defined variables:
*  -----------------------------
*
      INTEGER NWRITE
*
* end_var
*
*  Data statements:
*  ----------------
*
      SAVE RCSINI
      SAVE NWRITE
*
      DATA RCSINI/.TRUE./
      DATA NWRITE/0/
*
*  executable code for routine doitfillparams:
#if WRITE_RCS_INFO
      IF(RCSINI)THEN
        RCSINI = .FALSE.
        WRITE(6,*)'-------- CVSinfo --------'
        WRITE(6,*)CHRCSS
        WRITE(6,*)CHRCSR//' '//CHRCST
        WRITE(6,*)CHRCSD//' '//CHRCSA
      ENDIF
#endif
*----6----------------------------------------------------------------72

C*****In runev.inc
      current_run = run
      current_event = event

C*****In tfctlcde.inc
      bmagtf = bfield
      nexpt  = imode

C*****In tfgeomcd.inc
C*****Fill Beam Position - IS ZERO FOR NOW
      call vzero(xxbmtf, 3)

      RETURN
      END



