*
* $Id: tfstkc.F,v 1.2 1999/08/03 21:59:55 dpp Exp $
*
* $Log: tfstkc.F,v $
* Revision 1.2  1999/08/03 21:59:55  dpp
* add index
*
* Revision 1.1.1.1  1998/02/06 19:11:47  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:43  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:56  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/36 21/04/93  16.07.54  by  Dan Peterson
*-- Author :
      SUBROUTINE TFSTKC(ISTAK)
C.......................................................................
C.
C. TFPUSH - clears the stack by zeroing NSTKTF
C.
C. COMMON    :
C.
C. Calls     :
C. Called    :
C. Author    : D. Peterson
C.
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- SEQUENCE declarations ---------------
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfstakcd.inc"
* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: tfstkc.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.2 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 1999/08/03 21:59:55 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFSTKC' )
      INTEGER ISTAK

* ----------Executable code starts here---------------------

      IF(ISTAK.EQ.0)THEN
         CALL VZERO(NSTKTF,NSTKTYP)
      ELSE
         NSTKTF(ISTAK) = 0
      ENDIF

      RETURN
      END



