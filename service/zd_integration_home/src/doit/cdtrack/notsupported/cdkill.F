*
* $Id: cdkill.F,v 1.1 2003/03/05 17:02:02 dpp Exp $
*
* $Log: cdkill.F,v $
* Revision 1.1  2003/03/05 17:02:02  dpp
*      -> NEW ROUTINE, moved from "cdtrack/."
*
* Revision 1.2  2001/11/19 22:45:47  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:20  clib
* Developmental version of DUET.
*
* Revision 1.3  1996/03/12 14:46:59  zfiles
* Deleted tracks now have KINCD=-20
*
* Revision 1.2  1995/06/19 20:43:45  zfiles
* Added Kalman filter stuff from Rob
*
* Revision 1.1.1.1  1994/10/08  01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 28/05/91  13.39.10  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.15  by  Steve Schaffner
*CMZ :  5.50/00 13/05/89  16.51.38  by  Steve Schaffner
*-- Author :
      SUBROUTINE CDKILL
C.......................................................................
C.
C. CDKILL - Kill any crazy tracks
C.
C. COMMON    : ?
C. CALLS     : CDCMPR VFILL
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 14-Apr-87
C.
C. Modification Log.
C.
C. Mar 15/95 - R. Kutschke
C.           - Do not compress list.  Just flag with KINCD.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duklseq/dutmp1.inc"
C
      INTEGER ITRK
      REAL D0MAX
      DATA    D0MAX /0.005/

* ----------Executable code starts here---------------------

      IF(NTRKCD.LE.0) RETURN
      IF(NLYRVD.LE.0) RETURN
C
      DO 10 ITRK = 1, NTRKCD
          IF ( KINCD(ITRK) .LT. -3 ) GOTO 10
          IF (   (ABS(D0CD(ITRK)).GT.D0MAX .OR. KINCD(ITRK).EQ.-1)
     +          .AND.   NHTADR(ITRK).EQ.0
     +          .AND.   NHTLVD(ITRK).LT.NLMNVD ) THEN
                KINCD (ITRK) = KINCD(ITRK) - 20
                MAPO2N(ITRK) = -998
          ENDIF
10    CONTINUE

      RETURN
      END
