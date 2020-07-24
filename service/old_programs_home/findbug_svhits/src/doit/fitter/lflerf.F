*
* $Id: lflerf.F,v 1.1.1.1 1998/02/06 19:11:42 dpp Exp $
*
* $Log: lflerf.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:49  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/14 20/05/91  09.59.41  by  Steve Schaffner
*CMZ :  5.54/03 24/04/91  15.30.02  by  Steve Schaffner
*CMZ :  5.53/07 18/09/90  11.34.05  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  12.57.43  by  Steve Schaffner
*CMZ :  5.50/00 26/02/88  00.20.28  by  R. Namjoshi
*-- Author :
      SUBROUTINE LFLERF(CDUMMY,A,B,GDUMMY,CHISQ,S1,SX,SY,SXX,SXY,DET)
C.......................................................................
C.
C. LFLERF - Do a CHI2 line fit, return errors
C.
C. COMMON    : ?
C. CALLS     : BEGCLK ENDCLK
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 25-Feb-88
C.
C.
C. Modification Log.
C.
C. 25-Feb-88 : Use +SEQ,TFTIME.                              R. Namjoshi
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftime.inc"
#include "doit/duseq/tftrakcd.inc"

      REAL S1,SX,SY,SXX,SXY,WI,XI,YI,DET,A,B,CHI,CHISQ,CDUMMY,GDUMMY
      INTEGER I

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"
C
      CALL BEGCLK(KTIMES+ITFIT )

      S1    = 0.0
      SX    = 0.0
      SY    = 0.0
      SXX   = 0.0
      SXY   = 0.0
C
      DO 10 I = 1, NFIT
          WI    = WGTFIT(I)
          XI    = XFIT(I)
          YI    = YFIT(I)
C
          S1    = S1  + WI
          SX    = SX  + WI * XI
          SY    = SY  + WI * YI
          SXX   = SXX + WI * XI * XI
          SXY   = SXY + WI * XI * YI
10    CONTINUE
C
      DET   = S1 * SXX - SX * SX
      IF( ABS(DET) .GT. 1.0E-20 ) THEN
          A     = ( S1 * SXY - SX  * SY ) / DET
          B     = ( SY * SXX - SXY * SX ) / DET

          CHI   = 0.0
          DO 20 I = 1, NFIT
              CHI = CHI + WGTFIT(I)*(YFIT(I) - A*XFIT(I) - B)**2
20        CONTINUE

          CHISQ = CHI/FLOAT(NFIT-2)

      ELSE
          CHISQ = CHIDEF
      ENDIF

      CALL ENDCLK(KTIMES+ITFIT )
C
      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
