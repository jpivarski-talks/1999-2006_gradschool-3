*
* $Id: tfberr.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: tfberr.F,v $
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
*CMZ :  5.54/07 21/05/91  13.51.35  by  Steve Schaffner
*CMZ :  5.54/00 04/12/90  13.38.30  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.22.51  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.35  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFBERR( ISUB, ICOD, IPAR )
C.......................................................................
C.
C. TFBERR - Write an error into /TFERBF/
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : Many places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ??-Apr-82
C. LAST MOD  : 23-Jul-87
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C=======================================================================
C TFBERR writes an error into /TFERBF/
C
C Arguments:
C    ISUB      Subroutine number
C    ICOD      Error code
C    IPAR      Error parameter
C=======================================================================

#include "doit/duseq/tfindpar.inc"

#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tferbfcd.inc"
C
C== Argument declarations
C
      INTEGER ISUB, ICOD, IPAR

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"
C
C== Add the error if there is space in the buffer
C
      IF( NTFEBF.LT.(NERBDM-1) ) THEN
          NTFEBF           = NTFEBF + 1
          ITFEBF(1,NTFEBF) = NRUN
          ITFEBF(2,NTFEBF) = NEVENT
          ITFEBF(3,NTFEBF) = NRECRD
          ITFEBF(4,NTFEBF) = NTRACK
          ITFEBF(5,NTFEBF) = ISUB*IERPAC + ICOD
          ITFEBF(6,NTFEBF) = IPAR
      ELSE
          NTFEBF           = NERBDM
          ITFEBF(6,NTFEBF) = ITFEBF(6,NTFEBF) + 1
      ENDIF

      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
