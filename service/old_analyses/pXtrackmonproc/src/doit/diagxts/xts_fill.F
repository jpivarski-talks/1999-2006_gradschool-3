*
* $Id: xts_fill.F,v 1.4 1999/04/27 20:14:09 dpp Exp $
*
* $Log: xts_fill.F,v $
* Revision 1.4  1999/04/27 20:14:09  dpp
* use CLEO_XTSDEBUG
*
* Revision 1.3  1999/01/29 15:34:38  dpp
* force update
*
* Revision 1.2  1998/07/09 00:29:00  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.2  1997/02/25 21:36:29  dpp
* comments only
*
* Revision 1.1.1.1  1994/10/08 01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.13.31  by  Rob Kutschke
*CMZ :  6.00/01 04/03/93  13.19.08  by  Dan Peterson
*-- Author :dpp


      SUBROUTINE XTS_FILL(CALLER)
      IMPLICIT NONE

#include "cl3seq/cdgm3/cdgeompa.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

      INTEGER SET_FOREGROUND_C

      CHARACTER*(*)CALLER
      REAL X(6),Y(6)
      INTEGER I,J,IDUM
      INTEGER COLOLD
      REAL DMINPT,SCLMPT,DUSEPT

      DATA DMINPT,SCLMPT/.00629,1./

#if defined(CLEO_XTSDEBUG)
      PRINT 1001,NSEND,CALLER
1001  FORMAT(' XTS_FILL: ENTER: NSEND=',I4,
     1           '  CALLED BY ',A20)
#endif


      COLOLD=0
      DO 49 I=1,NSEND

C get geometry index for cell
      J = IDHIT(I)

C only display cells in zoom window
      IF (XCENTRS(J).LT.ZOOMXY(3,ZOOMU))GO TO 49
      IF (XCENTRS(J).GT.ZOOMXY(3,ZOOMU)+ZOOMXY(1,ZOOMU))GO TO 49
      IF (YCENTRS(J).LT.ZOOMXY(4,ZOOMU))GO TO 49
      IF (YCENTRS(J).GT.ZOOMXY(4,ZOOMU)+ZOOMXY(2,ZOOMU))GO TO 49

C select a color and save the color
      COLORH(J)=COLRHT(I)
      IF(COLRHT(I).EQ.COLOLD)GO TO 33
      COLOLD=COLRHT(I)
      IDUM=SET_FOREGROUND_C(COLOLD)
33    CONTINUE

C PT (round things)
      IF(YCORNR(1,J).LT.8.)GO TO 43
      DUSEPT=XCORNR(1,J)
      IF(ZOOMXY(1,ZOOMU).LT.SCLMPT)GO TO 41
      DUSEPT=DMINPT
41    CONTINUE
C      CALL ARCS_ARRAY_C(1,
      CALL ARCS_C(1,
     1       XCENTRS(J),YCENTRS(J),DUSEPT,DUSEPT,0.,360.,0,0,1)
      GO TO 49

C VD (hex things)
43    IF(YCORNR(5,J).GT.8.)GO TO 45
      CALL LINES_C(5,XCORNR(1,J),YCORNR(1,J),6)
      GO TO 49
C DR (square things)
45    CONTINUE
      CALL LINES_C(5,XCORNR(1,J),YCORNR(1,J),4)


49    CONTINUE

#if defined(CLEO_XTSDEBUG)
      PRINT 1003
1003  FORMAT(' XTS_FILL: EXIT')
#endif

      RETURN
      END

















