*
* $Id: tfunin.F,v 1.2 2001/11/20 00:00:45 dpp Exp $
*
* $Log: tfunin.F,v $
* Revision 1.2  2001/11/20 00:00:45  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:30  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:55  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 10/10/91  15.05.59  by  Jon Urheim
* add UNIX flag to +SELF statements.
*CMZ :  5.54/04 21/05/91  13.51.42  by  Steve Schaffner
*CMZ :  5.53/07 15/06/90  16.47.44  by  D. Riley
*CMZ :  5.53/03 16/05/90  17.45.54  by  Steve Schaffner
*CMZ :  5.51/03 27/09/89  12.00.00  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.37.12  by  Steve Schaffner
*CMZ :          26/05/89  14.05.09  by  Steve Schaffner
* Hard-wire unit numbers for life in post-CLUNS era
*CMZ :  5.50/00 25/02/88  19.51.52  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFUNIN
C.......................................................................
C.
C. TFUNIN - Initialize DUET Logical Unit Numbers
C.
C. COMMON    : ?
C. CALLS     : ?
C. CALLED    : ?
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

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfunitcd.inc"
#include "doit/duseq/tfctlcde.inc"
C
C== Local variables
C
      CHARACTER*10 FDGN,FORMTD
      INTEGER LRECL
#if defined(CLEO_VAX)||defined(CLEO_PDP10)||defined(CLEO_DECS)||defined(CLEO_UNIX)
      DATA    FDGN /'DUDIAG.DGN'/
#endif
#if defined(CLEO_IBM)
      DATA    FDGN/'DUDIAG'/
#endif
      DATA    FORMTD/'FORMATTED'/
      DATA    LRECL/263/

* ----------Executable code starts here---------------------

      IF (LOPEN) THEN
          IUNDGN = 22
          CALL GETLUN(IUNDGN,'DUET')
#if defined(CLEO_VAX)
          OPEN(UNIT=IUNDGN,FILE=FDGN,FORM=FORMTD,STATUS='NEW',
     +            RECL=LRECL)
#endif

#if defined(CLEO_IBM)
          OPEN(UNIT=IUNDGN,FILE=FDGN,FORM=FORMTD,STATUS='NEW')
#endif

#if defined(CLEO_DECS)||defined(CLEO_UNIX)
          OPEN(UNIT=IUNDGN,FILE=FDGN,FORM=FORMTD,STATUS='UNKNOWN')
#endif

          IUNLPT = IUNDGN
      ENDIF
      IUNTTR = 5
      IUNTTW = 6

      RETURN
      END
