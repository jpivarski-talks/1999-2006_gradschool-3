*
* $Id: zbitcd.F,v 1.3 2001/11/20 17:54:55 dpp Exp $
*
* $Log: zbitcd.F,v $
* Revision 1.3  2001/11/20 17:54:55  dpp
*      -> delete stupid useless nonexistent include file
*
* Revision 1.2  2001/11/19 22:44:30  dpp
*      -> delete unused include file
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
*CMZ :  5.54/07 21/05/91  14.40.55  by  Steve Schaffner
*CMZ :  5.53/04 10/07/90  19.03.47  by  Steve Schaffner
*CMZ :  5.51/03 31/10/89  12.45.49  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  13.17.09  by  Steve Schaffner
*CMZ :  5.50/00 16/05/89  14.18.44  by  Steve Schaffner
*-- Author :
      SUBROUTINE ZBITCD( ITRK, JDEV )
C.......................................................................
C.
C. ZBITCD - Zero the bits for device JDEV, track ITRK
C.
C. COMMON    : ?
C. CALLS     : IBITCD
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 06-Apr-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C   Arguments:
C     ITRK  track number  (IF <= 0, then do all tracks
C     JDEV  device number (if <= 0, then do all devices)

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/anltmp.inc"
#include "doit/duseq/tfbitcom.inc"

      INTEGER IBIT,MBIT,LBIT,I,J,IWORD,MTRK,LTRK,JDEV,ITRK,JBIT,JNUT
      LOGICAL LINIT
      DATA    LINIT/.TRUE./

C ------------- executable code starts here ----------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      IF( LINIT ) THEN
          CALL IBITCD
          LINIT  = .FALSE.
      ENDIF

      IF(ITRK.GT.MTRKDR)     RETURN
      IF(JDEV.GT.MXDVCD)     RETURN
      LTRK   = 1
      MTRK   = MTRKDR
      IF( ITRK.GT.0 ) THEN
          LTRK   = ITRK
          MTRK   = LTRK
      ENDIF
      IF (JDEV.GT.0) THEN
         IF (IADR(JDEV,1).EQ.0)      RETURN
      END IF

      DO 10 I = LTRK, MTRK
          IF( JDEV.LE.0 ) THEN
              DO 20 J = 1, MBITDR
                  KBITDR(I,J) = 0
20            CONTINUE

          ELSE
              DO 30 JNUT = 1, 2
                  LBIT   = IADR(JDEV,JNUT) + 1
                  MBIT   = IADR(JDEV,JNUT) + NLYRDR(JDEV)
                  DO 40 IBIT = LBIT, MBIT
                      IWORD  = (IBIT-1)/32 + 1
                      JBIT   = MOD(IBIT-1,32)
                      KBITDR(I,IWORD) = IBCLR(KBITDR(I,IWORD),JBIT)
40                CONTINUE
30            CONTINUE
          ENDIF

10    CONTINUE

      RETURN
      END
