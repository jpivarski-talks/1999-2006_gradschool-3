      SUBROUTINE SVLCWC(NWAF,XW,XL,IERR)
C...................................................................
C.
C. SVLCWC - CLEO-2.1 Silicon Vertex Detector convert local
C.                   coordinates to world coordinates
C.
C. Inputs   : NWAF Standard Wafer Number (0-95)
C.            XL  3 dimension real*8 vector, of coordinates
C.                  in local system of wafer NWAF
C.
C. Outputs  : XW  3 dimension real*8 vector, of coordinates
C.                  in world system
C.            IERR -1 if NWAF out of bounds
C.                  0 if normal return
C.
C. COMMONS  :
#include "seq/svseq/svgeompa.inc"
#include "seq/svseq/svgseq.inc"
#include "seq/svseq/svlaseq.inc"
      INTEGER NWAF,IERR
      DOUBLE PRECISION XW,XL,VWRK1,VWRK2
      DIMENSION XW(3),XL(3),VWRK1(3),VWRK2(3)
C >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

      PRINT*, ' !!!!! DANGER  DUMMY ROUTINE FOR SVLCWC '
      RETURN 
      END
