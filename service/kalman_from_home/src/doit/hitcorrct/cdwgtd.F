*
* $Id: cdwgtd.F,v 1.1 1998/02/24 18:53:45 dpp Exp $
*
* $Log: cdwgtd.F,v $
* Revision 1.1  1998/02/24 18:53:45  dpp
* not entered 2/9/98
*
*
      REAL FUNCTION CDWGTD( ILYR, DRIFT)
C.......................................................................
C.
C. CDWGTD - Return weight as a function of drift distance
C.
C. COMMON    : ?
C. CALLS     : WGTDCD
C. CALLED    : TF3FIT
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C.
C. Modification Log.
C. conv to f77, 2/12/98, dpp
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C Arguments:
C   ILYR    +- layer number (-ve = qdiv; +ve = normal)
C   DRIFT   signed drift distance.

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
      INTEGER ILYR,IDEV
      REAL DRIFT,WGTDCD
      EXTERNAL WGTDCD

* ----------Executable code starts here---------------------

      CDWGTD = 0.

      IF( ILYR.GT.0 .AND. ILYR.LE.QDIVAD )THEN
         IDEV = IDVCTF(ILYR)

         IF( IDEV.NE.IPROP )THEN
            CDWGTD = WGTDCD( ILTFCD(ILYR), DRIFT )
         ENDIF

      ENDIF
      RETURN
      END



