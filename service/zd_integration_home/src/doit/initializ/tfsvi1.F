*
* $Id: tfsvi1.F,v 1.1.1.1 1998/02/06 19:11:44 dpp Exp $
*
* $Log: tfsvi1.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:30  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/04/23 23:44:52  zfiles
* Comments in previous version are wrong!
* True comment:
*      First release.
*
* Revision 1.1  1996/04/23 07:32:40  zfiles
* Initialize LUHTTF.
*
* Revision 1.1.1.1  1996/03/28 17:14:18  zfiles
* New util area created
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TFSVI1
* 
* begin_doc
*
*  Documentation for subroutine tfsvi1
*
*  Purpose: Disable use of SVD layers for DUET pattern recognition.
*  -------- This needs to be done, for example, when the Illinois pattern 
*           recognition is run.
*
*  Input Parameters:  (Name - Type - Meaning)
*  ----------------
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
*  Author:   Rob Kutschke      Created:  Mon Apr 22 04:31:32 EDT 1996
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
      SAVE
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: tfsvi1.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.1.1.1 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 1998/02/06 19:11:44 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFSVI1' )
*
*  Local User defined variables:
*  -----------------------------
*
      INTEGER ILYR
*
* end_var
*
*  Data statements:
*  ----------------
*
*
*  executable code for routine tfsvi1:
*----6----------------------------------------------------------------72
*
      DO 100 ILYR = 1, NLYRTF
        IF ( IDVCTF(ILYR) .EQ. ISVX1 .OR.
     +       IDVCTF(ILYR) .EQ. ISVZ1     ) LUHTTF(ILYR) = .FALSE.
 100  CONTINUE

      END



