*
* $Id: cdmiss.F,v 1.1 2003/03/05 17:02:00 dpp Exp $
*
* $Log: cdmiss.F,v $
* Revision 1.1  2003/03/05 17:02:00  dpp
*      -> NEW ROUTINE, moved from "cdtrack/."
*
* Revision 1.2  2001/11/19 22:44:46  dpp
*     -> delete unused include fil
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:21  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 09/09/91  12.31.33  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.15  by  Steve Schaffner
*CMZ :  5.50/00 15/05/89  12.56.15  by  Steve Schaffner
*CMZ :          04/05/89  19.04.06  by  Steve Schaffner
* All ANLCDR and ANLCDS variables replaced with CDTRAKCD equivalents
*-- Author :    Dan Riley   24/10/88
      SUBROUTINE CDMISS(ITRK,MISDR)
C.......................................................................
C.
C. CDSTOR - Return MISDR (number of missing hits in track).
C.
C. COMMON    : ?
C. CALLS     :
C. CALLED    : CDSTOR
C. AUTHOR    : D. Riley
C.
C. VERSION   : 1.00
C. CREATED   : 24-Oct-88
C. LAST MOD  : 24-Oct-88
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/cdcorrcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
      INTEGER MISDR,IRAE,ITRK,ILYR
      REAL RMIN,RMX1,RMX2,RMAX,CT,Z0,ZEND

* ----------Executable code starts here---------------------

      RMIN =     ABS(D0CD(ITRK))
      RMX1 = 1.0/ABS(CURCD(ITRK)) - D0CD(ITRK)*SIGN(1.,CURCD(ITRK))

      IRAE = 0

      CT   = TANDCD(ITRK)
      Z0   = Z0CD(ITRK)

      DO 22 ILYR = 1, NLYRTF
          IF (RTF(ILYR).GE.RMIN) THEN
              IF (CT.NE.0.0) THEN
                  ZEND = ZENDTF(ILYR)
                  RMX2 = (SIGN(ZEND,CT) - Z0) / CT
                  RMAX = AMIN1(RMX1, RMX2)
              ELSE
                  RMAX = RMX1
              ENDIF
              IF (RTF(ILYR).LE.RMAX) THEN
                  IRAE = IRAE + 1
              ENDIF
          ENDIF
22    CONTINUE

      IRAE = IRAE - NHTLVD(ITRK) - NHTAIZ(ITRK)
     +            - NHTADR(ITRK) - NHTSDR(ITRK)
     +            - NHTSCT(ITRK)

      MISDR = IRAE

      END
