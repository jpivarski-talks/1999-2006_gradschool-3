*
* $Id: tfstkl.F,v 1.3 2002/09/05 19:28:21 dpp Exp $
*
* $Log: tfstkl.F,v $
* Revision 1.3  2002/09/05 19:28:21  dpp
*      -> add names for substaks
*
* Revision 1.2  2002/05/23 14:32:34  dpp
*      -> stak is now structured as sub-staks in a global stak
*          rather than having the stak be multidimensioned
*
* Revision 1.1  2001/11/20 00:10:32  dpp
*      -> NEW ROUTINE
*      -> set internal value of stack location number
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE TFSTKL(SUBSTAK,INP_REL_LOC)
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
      PARAMETER(CHRCSR='$Revision: 1.3 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2002/09/05 19:28:21 $')
      PARAMETER(CHRCSA='$Author: dpp $')

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFSTKL' )

      INTEGER SUBSTAK,INP_REL_LOC

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      ISTKTF(SUBSTAK)=GSTKLOCL(SUBSTAK)-1+INP_REL_LOC
#if defined(CLEO_DPPDIAG)
      PRINT 1001,
     1     SUBSTAK,NAMSSTK(SUBSTAK),
     2     INP_REL_LOC,
     2     IFSTK(SUBSTAK),ISTKTF(SUBSTAK)
 1001 FORMAT(' TFSTKL:',
     1     ' SUBSTAK=',I2,3X,A15,
     2     ' (set pointer to input relative location=',I4,' )',
     2     '  IFSTK=',I4,'  ISTKTF=',I4)
#endif
      RETURN
      END


