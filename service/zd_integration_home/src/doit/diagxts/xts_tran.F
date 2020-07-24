*
* $Id: xts_tran.F,v 1.4 2002/09/05 19:09:43 dpp Exp $
*
* $Log: xts_tran.F,v $
* Revision 1.4  2002/09/05 19:09:43  dpp
*      -> maximum radius
*
* Revision 1.3  1999/01/29 15:34:44  dpp
* force update
*
* Revision 1.2  1998/05/08 14:13:52  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.3  1997/02/25 21:41:42  dpp
* loop over XTS cells rather than CD
*
* Revision 1.2  1995/09/25 19:26:40  zfiles
* switch XTSDEBUG to control print statements
*
* Revision 1.1.1.1  1994/10/08  01:01:00  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/14 01/03/94  17.19.40  by  Rob Kutschke
*CMZ :  6.00/07 31/08/93  14.24.44  by  Rob Kutschke
*CMZ :  6.00/01 29/01/92  09.50.04  by  Dan Peterson
*-- Author :dpp
      SUBROUTINE XTS_TRAN(XSIZE,YSIZE,XORG,YORG,RADSIZ,CALLER)
      IMPLICIT NONE
      SAVE

#include "cl3seq/zfiles/usbank.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

      REAL XMIN,XMAX,YMIN,YMAX,RADSIZ
      REAL FEDGE,EDGE
      REAL DX,DY,XMIN1,YMIN1
      REAL XSIZE,YSIZE,XORG,YORG
      CHARACTER*20 CALLER
      INTEGER I

      DATA FEDGE/.025/

#if defined(CLEO_XTSDEBUG)
      PRINT 2000,CALLER
2000  FORMAT(' XTS_TRAN: ENTER; CALLED FROM...',A20)
#endif

      XMIN=XCENTRS(1)
      XMAX=XCENTRS(1)
      YMIN=YCENTRS(1)
      YMAX=YCENTRS(1)
      DO 101 I=1,NCDXTS
        XMIN = MIN(XMIN,XCENTRS(I))
        XMAX = MAX(XMAX,XCENTRS(I))
        YMIN = MIN(YMIN,YCENTRS(I))
        YMAX = MAX(YMAX,YCENTRS(I))
 101  CONTINUE
      RADSIZ=MAX(ABS(XMIN),ABS(XMAX))
      RADSIZ=MAX(ABS(YMAX),RADSIZ)
      RADSIZ=MAX(ABS(YMIN),RADSIZ)
      EDGE = FEDGE*(XMAX-XMIN)
      
      XMIN = XMIN-EDGE
      XMAX = XMAX+EDGE
      EDGE = FEDGE*(YMAX-YMIN)
      YMIN = YMIN-EDGE
      YMAX = YMAX+EDGE
      
#if defined(CLEO_XTSDEBUG)
      PRINT 2013,XMIN,XMAX,YMIN,YMAX
2013  FORMAT(' XTS_TRAN: Xmin,Xmax,Ymin,Ymax=',4F9.5)
#endif

      DX=XMAX-XMIN
      DY=YMAX-YMIN
      IF(DX.LT.DY)DX=DY
      IF(DY.LT.DX)DY=DX
      XMIN1=MIN(XMIN,YMIN)
      YMIN1=MIN(XMIN,YMIN)

      XSIZE=DX
      YSIZE=DY
      XORG=XMIN1
      YORG=YMIN1

#if defined(CLEO_XTSDEBUG)
      PRINT 2097
2097  FORMAT(' XTS_TRAN: ENDED WITH SUCCESS')
#endif

      RETURN
      END












