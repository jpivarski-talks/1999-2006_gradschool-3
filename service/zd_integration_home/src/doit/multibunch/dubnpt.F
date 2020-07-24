*
* $Id: dubnpt.F,v 1.3 1999/11/29 21:43:53 lyon Exp $
*
* $Log: dubnpt.F,v $
* Revision 1.3  1999/11/29 21:43:53  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.2  1998/05/08 14:14:03  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:45  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:36  clib
* Developmental version of DUET.
*
* Revision 1.5  1997/04/15 04:33:16  clib
* New DUBN zebra bank; fill it with contents of cdbnc100.inc - not dubnch.inc.
*
* Revision 1.4  1996/05/22 19:52:39  clib
* pck: Bug fix.  IBNTF is now properly saved for the first event.
*
* Revision 1.3  1996/04/23 07:36:15  zfiles
* Comments forthcoming...
*
* Revision 1.2  1995/11/30 23:49:50  zfiles
* Minor updates from Peter Kim
*
* Revision 1.1  1995/11/27 19:42:18  nk
* store bunch info in Zebra bank
*
C----------------------------------------------------------------------
      SUBROUTINE DUBNPT
C.......................................................................
C. DUBNPT  - Loads CDBNC100 into ZEBRA bank
C. Inputs    : none
C. Outputs   : none
C. COMMON    : cdbnc100.inc
C. Calls     : ZPUT
C. Called    : DUETEV
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif

#include "doit/utilseq/cdbnc100.inc"
#include "cl3seq/zfiles/usbank.inc"
#include "cl3seq/zfiles/quest.inc"
#include "seq/clutil/mesrep.inc"

      INTEGER IT,IDDUBI,IDDUBR
      LOGICAL LFIRST
      SAVE IDDUBI,IDDUBR,LFIRST
      DATA LFIRST/.TRUE./
C
C---Initialize
      IF(LFIRST)THEN
        LFIRST=.FALSE.

        CALL ZDGUID(1,IDDUBI,'DUBI','WO','C','DUBN output','DUBNPT')
        IF(ICQUST(JZFERR).NE.0)GO TO 19

        CALL ZDGUID(1,IDDUBR,'DUBR','WO','C','DUBN output','DUBNPT')
        IF(ICQUST(JZFERR).NE.0)GO TO 19
      ENDIF

C---Load banks

      CALL ZDPVAL(IDDUBI,IBNTD,23,1,1,0,0,0)
      IF(ICQUST(JZFERR).NE.0)GO TO 19

      CALL ZDPVAL(IDDUBR,TFBDLR(1),16,1,1,0,0,0)
      IF(ICQUST(JZFERR).NE.0)GO TO 19

      RETURN
19    WRITE(CHMESS,1001)
1001  FORMAT(' Zbank ERROR')
      CALL MESLOG('DUBNPT',1,MSERRO)
      END
