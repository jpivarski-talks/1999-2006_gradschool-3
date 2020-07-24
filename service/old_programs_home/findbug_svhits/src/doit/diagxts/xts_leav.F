*
* $Id: xts_leav.F,v 1.3 1999/01/29 15:34:40 dpp Exp $
*
* $Log: xts_leav.F,v $
* Revision 1.3  1999/01/29 15:34:40  dpp
* force update
*
* Revision 1.2  1998/07/09 00:34:33  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/01/25 15:10:13  zfiles
* remove cdplot.inc
*
* Revision 1.1.1.1  1994/10/08 01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.16.27  by  Rob Kutschke
*CMZ :  6.00/01 29/01/92  09.54.37  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.39  by  Dan Peterson
*-- Author :dpp
      SUBROUTINE XTS_LEAV(CALLER)
C PREPARE XTS FOR RETURN TO DUET
      IMPLICIT NONE
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"
      CHARACTER*(*) CALLER

C      PRINT 2001,LEFTSF,CALLER
2001  FORMAT(' XTS_LEAV: ENTER, LEFTSF=',I3,' CALLED FROM ',A20)

      IF(LEFTSF.EQ.0)GO TO 101

C      PRINT 2030
2030  FORMAT(' XTS_LEAV: QUICKLY EXIT WITHOUT EXECUTING')

      GO TO 147
101   CONTINUE

147   LEFTSF=1
C      PRINT 2025,LEFTSF
2025  FORMAT(' XTS_LEAV: EXIT,  LEFTSF=',I3)

      RETURN
      END






