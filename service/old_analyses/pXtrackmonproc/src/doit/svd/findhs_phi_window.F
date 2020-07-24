*
* $Id: findhs_phi_window.F,v 1.1 2001/03/30 00:02:16 dpp Exp $
*
* $Log: findhs_phi_window.F,v $
* Revision 1.1  2001/03/30 00:02:16  dpp
*      -> NEW ROUTINE
*      -> determination of whether input phi matches hybrid
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      SUBROUTINE FINDHS_PHI_WINDOW(INP_HYBRID,INP_PHI,PHI_TOLER,
     2     KEEP_HIT)
C.......................................................................
C.
C. FINDHS_PHI_WINDOW - test if input phi matches phi of a hybrid
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FINDHS
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 27-mar-2001 adapted from FINDHS
C. LAST MOD  : 
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
*--
* INP_HYBRID.. hybrid number
* INP_PHI..... Expected PHI value
* PHI_TOLER....phi tolereance for edge of hybrid
* KEEP_HIT.... LOGICAL = .TRUE. IS input phi is within the hybrid 
* MISS........ miss distance in phi
*--
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/svgeometry.inc"

C ARGUMEMNTS

      INTEGER INP_HYBRID
      REAL INP_PHI
      REAL PHI_TOLER
      LOGICAL KEEP_HIT
      REAL MISS

C LOCAL VARIABLES

      REAL PHI_SMALL,PHI_LARGE,PHI_TESTI,PHI_ADDED,PHI_TEMPO,PHI_DIF_S

C DOIT FUNCTION
      REAL Z2PI

* ----------Executable code starts here---------------------
C test if input phi is within hybrid
C find the large and small phi edges of the hybrid
C the given boundaries can be in any order and can span phi=0 
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      PHI_LARGE=Z2PI(SVBOUNDARY_PHI(INP_HYBRID,1))
      PHI_SMALL=Z2PI(SVBOUNDARY_PHI(INP_HYBRID,2))
      IF(PHI_LARGE.LT.PHI_SMALL)THEN
         PHI_TEMPO=PHI_LARGE
         PHI_LARGE=PHI_SMALL
         PHI_SMALL=PHI_TEMPO
      ENDIF
C correction when the hybrid spans phi=0
      PHI_TEMPO=PHI_LARGE-PHI_SMALL
      IF(PHI_TEMPO.GT.PI)THEN
         PHI_TEMPO=PHI_LARGE
         PHI_LARGE=PHI_SMALL+TWOPI
         PHI_SMALL=PHI_TEMPO
      ENDIF
C difference of the input phi relative to low phi edge (minus tolerance)
C rotate the input phi until the "differece" is positive definite
      PHI_TESTI=INP_PHI
      PHI_ADDED=0.
 211  PHI_DIF_S=PHI_TESTI-(PHI_SMALL-PHI_TOLER)
      IF(PHI_DIF_S.LT.0.   )THEN
         PHI_TESTI=PHI_TESTI+TWOPI
         GO TO 211
      ENDIF
 212  PHI_DIF_S=PHI_TESTI-(PHI_SMALL-PHI_TOLER)
      IF(PHI_DIF_S.GE.TWOPI)THEN
         PHI_TESTI=PHI_TESTI-TWOPI
         GO TO 212
      ENDIF
C difference of the input phi relative to high phi edge (plus tolerance)
      MISS=PHI_TESTI-(PHI_LARGE+PHI_TOLER)
      KEEP_HIT=(MISS.LE.0.)

      RETURN
      END





