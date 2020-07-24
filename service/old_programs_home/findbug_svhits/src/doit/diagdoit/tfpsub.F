*
* $Id: tfpsub.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: tfpsub.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:45  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:58  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  13.51.39  by  Steve Schaffner
*CMZ :  5.54/03 27/02/91  14.02.18  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.23.01  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.46  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFPSUB( IWHAT, SUBUSE, WHAT )
C.......................................................................
C.
C. TFPSUB - Identify run,event... number and calling routine
C.
C. COMMON    : ?
C. CALLS     : TFPSUB
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ??-Apr-82
C. LAST MOD  : 27-Jul-87
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
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"

#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfunitcd.inc"
C
C== Argument declarations
C
      CHARACTER*(*) IWHAT, SUBUSE, WHAT
C
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      WRITE (IUNDGN,100) IWHAT,SUBUSE,WHAT,NRUN  ,NEVENT,NRECRD,NTRACK

      RETURN
*
  100 FORMAT(/A2,' ',A6,' ',A8,' : RUN',I6,' : EVENT',I6,
     +                         ' : REC',I6,' : ITRK',I3)
*
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
