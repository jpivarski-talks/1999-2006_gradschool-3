*
* $Id: dubn0.F,v 1.2 1999/11/29 21:43:51 lyon Exp $
*
* $Log: dubn0.F,v $
* Revision 1.2  1999/11/29 21:43:51  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1.1.1  1998/02/06 19:11:45  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:36  clib
* Developmental version of DUET.
*
* Revision 1.3  1997/04/15 04:25:33  clib
* dubnch -> cdbnc100.
*
* Revision 1.2  1995/11/30 23:49:47  zfiles
* Minor updates from Peter Kim
*
* Revision 1.1  1995/11/27 19:39:40  nk
* initialize dubnch.inc
*
*
C-----------------------------------------------------------
      SUBROUTINE DUBN0
C
C   Initialize the common block  cdbnc100.inc
C
C-----------------------------------------------------------
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/utilseq/cdbnc100.inc"

      INTEGER IX

      IBNDUF = 1
      IBNTD  = 1

      DUEVLF = 0.0

      DO 100 IX = 1, 5
          IBNTFD(IX) = 0
          IBNDU(IX)  = 0
          NTGDBN(IX) = 0
          IBNHIS(IX) = 0

          TFBDLR(IX) = 0.0
          TFBDCF(IX) = 0.0
          DUEVAL(IX) = 0.0
 100  CONTINUE

      RETURN
      END

