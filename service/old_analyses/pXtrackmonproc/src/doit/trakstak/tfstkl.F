*
* $Id: tfstkl.F,v 1.1 2001/11/20 00:10:32 dpp Exp $
*
* $Log: tfstkl.F,v $
* Revision 1.1  2001/11/20 00:10:32  dpp
*      -> NEW ROUTINE
*      -> set internal value of stack location number
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE TFSTKL(ISTAK,ISTKTFLOC)
C.......................................................................
C.
C. TFSTKL - sets the location number  /TFSTAK/
C.
C. COMMON    :
C.
C. Calls     :
C. Called    :
C. Author    : D. Peterson
C.
C. ISTKTF = ordinal number of track in /TFSTAK/ just popped
C.        = 0 => all tracks have been popped
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- SEQUENCE declarations ---------------
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfstakcd.inc"
* -------------------- Local    declarations ---------------
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: tfstkl.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.1 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2001/11/20 00:10:32 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFSTKL' )

      INTEGER ISTAK,ISTKTFLOC

* ----------Executable code starts here---------------------

         ISTKTF(ISTAK)=ISTKTFLOC
#if defined(CLEO_DPPDIAG)
      PRINT 1001,ISTAK,
     2     IFSTK(ISTAK),ISTKTF(ISTAK)
 1001 FORMAT(' TFSTKL: ISTAK=',I2,
     2     '  ISFTK=',I4,'  ISTKTF=',I4)
#endif
         RETURN
         END

