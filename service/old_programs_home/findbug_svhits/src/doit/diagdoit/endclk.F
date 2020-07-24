*
* $Id: endclk.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: endclk.F,v $
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
*CMZ :  5.54/07 21/05/91  14.40.56  by  Steve Schaffner
*CMZ :  5.53/07 25/09/90  16.11.12  by  D. Riley
*CMZ :  5.53/04 18/06/90  11.38.21  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.57.31  by  Steve Schaffner
*CMZ :  5.51/06 29/12/89  12.50.22  by  Steve Schaffner
*CMZ :  5.51/01 08/09/89  14.57.18  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  13.16.52  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  23.50.28  by  R. Namjoshi
*-- Author :
      SUBROUTINE ENDCLK( ICLOCK )
C.......................................................................
C.
C. ENDCLK - Stop  timer number ICLOCK
C.
C. COMMON    : ?
C. CALLS     : TIMEX
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 25-Feb-88
C.
C. Modification Log.
C.
C. 25-Feb-88 : Use +SEQ,TFTIME.                              R. Namjoshi
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
C
#include "doit/duseq/tftime.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
      INTEGER ICLOCK,ISTOP
      REAL T0
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      IF (.NOT.LWTIME) RETURN
      CALL TIMEX(T0)
      ISTOP  = T0*1000.0
      ITIMES(ICLOCK) = ITIMES(ICLOCK) + ISTOP
C
      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
