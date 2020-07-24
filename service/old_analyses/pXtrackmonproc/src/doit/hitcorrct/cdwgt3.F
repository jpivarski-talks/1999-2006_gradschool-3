*
* $Id: cdwgt3.F,v 1.2 1998/02/24 18:23:05 dpp Exp $
*
* $Log: cdwgt3.F,v $
* Revision 1.2  1998/02/24 18:23:05  dpp
* restore the pilot.h
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      REAL FUNCTION CDWGT3( ILYR, DRIFT,ETAIN,PHI)
C.......................................................................
C.
C. CDWGT3 - Return weight as a function of drift distance
C.
C. COMMON    : ?
C. CALLS     : WGT3CD
C. CALLED    : TF3FIT
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
C Arguments:
C   ILYR    +- layer number (-ve = qdiv; +ve = normal)
C   DRIFT   signed drift distance.
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
      INTEGER ILYR,IDEV
      REAL DRIFT,ETAIN,WGT3CD,PHI
* ----------Executable code starts here---------------------

      CDWGT3 = 0.
      IF( ILYR.LE.0 .OR. ILYR.GT.QDIVAD )     RETURN
      IDEV = IDVCTF(ILYR)
      IF( IDEV.EQ.IPROP ) RETURN

C     CDWGT3 = WGT3CD( ILTFCD(ILYR), DRIFT ,ETAIN,PHI)
      CDWGT3=1./(.000100)**2

      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>








