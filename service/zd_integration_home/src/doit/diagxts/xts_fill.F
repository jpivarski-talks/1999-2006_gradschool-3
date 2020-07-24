*
* $Id: xts_fill.F,v 1.6 2003/03/05 17:26:02 dpp Exp $
*
* $Log: xts_fill.F,v $
* Revision 1.6  2003/03/05 17:26:02  dpp
*      -> protect against NSEND=0 input
*
* Revision 1.5  2002/09/05 19:10:21  dpp
*     -> some F77
*
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
      REAL DMINPT,SCLMPT,DUSEPT

#if defined(CLEO_ULTRIX)
      INTEGER COLOLD
#else
      INTEGER*8 COLOLD
#endif

      DATA DMINPT,SCLMPT/.00629,1./

#if defined(CLEO_XTSDEBUG)
      PRINT 1001,NSEND,CALLER
1001  FORMAT(' XTS_FILL: ENTER: NSEND=',I4,
     1           '  CALLED BY ',A20)
#endif

      IF(NSEND.GT.0)THEN
      COLOLD=0
      DO 49 I=1,NSEND

C get geometry index for cell
        J = IDHIT(I)

C only display cells in zoom window
        IF(
     1       (XCENTRS(J).GT.ZOOMXY(3,ZOOMU)).AND.
     2       (XCENTRS(J).LT.ZOOMXY(3,ZOOMU)+ZOOMXY(1,ZOOMU)).AND.
     3       (YCENTRS(J).GT.ZOOMXY(4,ZOOMU)).AND.
     4       (YCENTRS(J).LT.ZOOMXY(4,ZOOMU)+ZOOMXY(2,ZOOMU)))THEN
          
C select a color and save the color
          COLORH(J)=COLRHT(I)
          IF(COLRHT(I).NE.COLOLD)THEN
            COLOLD=COLRHT(I)
            IDUM=SET_FOREGROUND_C(COLOLD)
          ENDIF

C PT (round things)
          IF(YCORNR(1,J).GE.8.)THEN
            DUSEPT=XCORNR(1,J)
            IF(ZOOMXY(1,ZOOMU).LT.SCLMPT)GO TO 41
            DUSEPT=DMINPT
 41         CONTINUE
            CALL ARCS_C(1,
     1           XCENTRS(J),YCENTRS(J),DUSEPT,DUSEPT,0.,360.,0,0,1)
            
C VD (hex things)
          ELSEIF(YCORNR(5,J).LE.8.)THEN
            CALL LINES_C(5,XCORNR(1,J),YCORNR(1,J),6)

C DR (square things)
          ELSE
            CALL LINES_C(5,XCORNR(1,J),YCORNR(1,J),4)
          ENDIF

        ENDIF
 49   CONTINUE
        ENDIF

#if defined(CLEO_XTSDEBUG)
      PRINT 1003
1003  FORMAT(' XTS_FILL: EXIT')
#endif

      RETURN
      END

















