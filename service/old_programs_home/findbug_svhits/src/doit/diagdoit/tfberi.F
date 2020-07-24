*
* $Id: tfberi.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: tfberi.F,v $
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
*CMZ :  5.54/07 21/05/91  13.51.34  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.22.51  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.35  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFBERI
C.......................................................................
C.
C. TFBERI - Initialize the error buffer
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : ?
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

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tferbfcd.inc"
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      NTFEBF           = 0
      ITFEBF(1,NERBDM) = NRUN
      ITFEBF(2,NERBDM) = NEVENT
      ITFEBF(3,NERBDM) = NRECRD
      ITFEBF(4,NERBDM) = 0
      ITFEBF(5,NERBDM) = 0
      ITFEBF(6,NERBDM) = 0

      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
