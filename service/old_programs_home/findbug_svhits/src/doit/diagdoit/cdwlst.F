*
* $Id: cdwlst.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: cdwlst.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:45  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:57  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 28/05/91  13.39.15  by  Steve Schaffner
*CMZ :  5.51/04 05/12/89  00.36.40  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.25  by  Steve Schaffner
*CMZ :  5.50/00 22/04/89  13.35.31  by  Steve Schaffner
*-- Author :
      SUBROUTINE CDWLST( IDEV )
C.......................................................................
C.
C. CDWLST - List hit wires for CD device IDEV
C.
C. COMMON    : ?
C. CALLS     : TFPSUB
C. CALLED    : DUET3
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 04-Jul-87
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
C== Argument declarations
C
      INTEGER IDEV

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfunitcd.inc"
C
C== Local variables
C
      INTEGER NPRINT, IHW, ILTF, IW, IWADR

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      NPRINT = 0
C
C== Loop over all hit wires, print if hit is from device IDEV
C
      DO 10 IHW = 1, NHTWCD
          ILTF  = ILCDTF(ICYLCD(IHW))
          IF( IDVCTF(ILTF).EQ.IDEV ) THEN

              IF( MOD(NPRINT,50).EQ.0 ) THEN
                  WRITE(IUNDGN,100)
                  CALL TFPSUB(' $', 'CDWLST', 'PRINT   ')
                  WRITE(IUNDGN,105) DEVNAM(IDEV)
              ENDIF

              NPRINT = NPRINT + 1
              IW     = IWIRCD(IHW)
              IWADR  = INDXTF(ILTF) + IW
              WRITE (IUNDGN,110) IHW,ILTF,IW,
     +               PHIWCD(IHW),XWCD (IHW),YWCD(IHW),ZWCD(IHW),
     +               TDWCD(IHW),DWCD(IHW),QWCD(IHW),PWCD(IHW),
     +               INXHCD(IHW),IADRHW(IWADR)

          ENDIF
10    CONTINUE
*
      RETURN
*
  100 FORMAT('1')
  105 FORMAT('0',10X,'PRINTS THE LIST OF ',A6,' HIT WIRES:'/
     + /'   IHW ILYR IWIR      PHIW        XW        YW        ZW',
     +                  '       TDW        DW        QW        PW',
     +                  '  INXHCD  IADRHW')
  110 FORMAT( ' ',3I5,4F10.5,0P,F10.1,6P,F10.1,0P,2F10.1,2I8)
*
      END
* ========================================================
#include "doit/duseq/dupatchd.inc"
