*
* $Id: cdplin.F,v 1.1.1.1 1998/02/06 19:11:47 dpp Exp $
*
* $Log: cdplin.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:47  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:44  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:42  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 28/05/91  13.39.11  by  Steve Schaffner
*CMZ :  5.52/00 09/01/90  17.16.04  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.17  by  Steve Schaffner
*CMZ :  5.50/00 24/10/88  13.31.26  by  Dan Riley
*CMZ :          22/07/88  14.59.10  by  Dan Riley
* write line to IUNDGN
*-- Author :
      SUBROUTINE CDPLIN( CHAR )
C.......................................................................
C.
C. CDPLIN - Print a 128 column line of character CHAR
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 22-Jul-88
C.
C. Modification Log.
C.
C. 22-Jul-88 : send line to IUNDGN--that's usually where it's wanted.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
#include "doit/duseq/tfunitcd.inc"

      CHARACTER*1 CHAR
      INTEGER I

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      WRITE(IUNDGN,1) (CHAR,I=1,128)
C
      RETURN
*
1     FORMAT(/1X,128(A1))
*
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
