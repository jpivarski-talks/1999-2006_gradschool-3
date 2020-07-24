*
* $Id: tfpber.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: tfpber.F,v $
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
*CMZ :  5.54/07 21/05/91  13.51.37  by  Steve Schaffner
*CMZ :  5.54/00 04/12/90  13.50.56  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.22.57  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.41  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFPBER
C.......................................................................
C.
C. TFPBER - Print the error buffer
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ??-Apr-82
C. LAST MOD  : 24-Jul-87
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
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tferbfcd.inc"
#include "doit/duseq/tfunitcd.inc"

      INTEGER NERR,IERR,ISUB,ICOD,I

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      IF( .NOT.LPRERR ) RETURN
      IF( NTFEBF.LE.0 ) RETURN

      WRITE(IUNDGN,100)
      IF( NTFEBF.GE.NERBDM ) THEN
          NERR   = NERBDM + ITFEBF(6,NERBDM) - 1
          WRITE(IUNDGN,200) NRUN,NEVENT,NRECRD,NERR
      ENDIF

      NERR   = MIN0(NTFEBF,NERBDM-1)
      DO 10 IERR = 1, NERR
          ISUB   = ITFEBF(5,IERR)/IERPAC
          ICOD   = MOD(ITFEBF(5,IERR),IERPAC)
          WRITE(IUNDGN,300) (ITFEBF(I,IERR),I=1,4),
     +                      ISUB,ICOD,ITFEBF(6,IERR)
10    CONTINUE

      RETURN
*
100   FORMAT(' ')
200   FORMAT(' %%  RUN:',I6,'  EVENT:',I6,'  RECORD:',I6,
     +       '.   ERROR BUFFER OVERFLOW:',I6,'  ERRORS ON THIS EVENT')
300   FORMAT(' % ',I6,'/',I6,'  RECORD:',I6,'  NTRACK:',I6,
     +       '  SUBROUTINE:',I4,'  ERROR:',I4,'  PARAMETER:',I7)
*
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
