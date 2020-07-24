      SUBROUTINE SVLCNX(NWAF,ISID,NSTR,XLOC,IERR)
C...................................................................
C.
C. SVLCNX - CLEO-2.1 Silicon Vertex Detector Channel to Coordinate
C.                   Conversion in Local (Wafer) Coordinates.
C.
C. Inputs   : NWAF Standard Wafer Number (0-95)
C.            ISID Silicon Side Number (0=rphi, 1=z)
C.            NSTR Local Strip Number: (0-188, rphi, ISID=0, Layer 1,3)
C.                                     (0-251, rphi, ISID=0, Layer 2  )
C.                                     (0-566,  z  , ISID=1, Layer 1  )
C.                                     (0-755,  z  , ISID=1, Layer 2  )
C.                                     (0-566,  z  , ISID=1, Layer 3  )
C.                 NOTE: smaller strip number defined LOCALLY; convention
C.                       agrees with hardware numbering (with pads facing
C.                       you, channel 1 is on LEFT); except hardware
C.                       numbering starts at 1 rather than 0.
C.                       on phi side this means 0 is at large phi on +z=west;
C.                       0 is at small phi on -z=east; in both cases
C.                       on z side 0 is toward interaction point.
C.
C. Outputs  : XLOC Local coordinate, measured from center;
C.                 ISID=0: for +-z wafers, XLOC<0 for small phi
C.                 ISID=1: XLOC<0 nearer to IP, far from CAMEX.
C.            IERR -2 illegal wafer/side inputs: XLOC=0. returned.
C.                 -1 NSTR outside bounds; nearest XLOC returned.
C.                  0 successful return
C.
C. COMMONS  :
#include "seq/svseq/svgeompa.inc"
#include "seq/svseq/svgseq.inc"
#include "seq/svseq/svlaseq.inc"
      INTEGER NWAF,ISID,NSTR,IERR
C,IWFTYP,NSTRMX
      DOUBLE PRECISION XLOC
C,PITCH,XLOCMN,XLOCMX
      DOUBLE PRECISION RSTR

      PRINT*, '!!!!!!! DANGER - SVLCNX IS DUMMIED !!!!!!!!!'
      RETURN
      END
