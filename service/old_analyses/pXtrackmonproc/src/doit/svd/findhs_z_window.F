*
* $Id: findhs_z_window.F,v 1.1 2001/03/30 00:02:38 dpp Exp $
*
* $Log: findhs_z_window.F,v $
* Revision 1.1  2001/03/30 00:02:38  dpp
*      -> NEW ROUTINE
*      -> determination of whether input Z matches hybrid
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      SUBROUTINE FINDHS_Z_WINDOW(INP_HYBRID,INP_Z,INP_Z0,Z_TOLER,
     2     KEEP_HIT,MISSZ)
C.......................................................................
C.
C. FINDHS_Z_WINDOW - test if input z matches z of hybrid
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
* INP_Z....... Expected Z value
* INP_Z0...... = Z0 of fit, added to window in Z for phi hybrids
* Z_TOLER......z tolereance for edge of hybrid
* KEEP_HIT.... LOGICAL = .TRUE. IS input phi is within the hybrid 
* MISSZ....... miss distance in Z
*--
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/svgeometry.inc"
#include "doit/duseq/svscratch.inc"

C ARGUMEMNTS

      INTEGER INP_HYBRID
      REAL INP_Z,INP_Z0,Z_TOLER
      LOGICAL KEEP_HIT
      REAL MISSZ

C LOCAL VARIABLES

      REAL Z_SMALL,Z_LARGE,Z_TEMPO

* ----------Executable code starts here---------------------

C CF: test if the input Z is within the hybrid
C find the large and small Z edges of the hybrid
C the given boundaries can be in any order
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      Z_LARGE=SVBOUNDARY_Z(INP_HYBRID,1)
      Z_SMALL=SVBOUNDARY_Z(INP_HYBRID,2)
      IF(Z_LARGE.LT.Z_SMALL)THEN
         Z_TEMPO=Z_LARGE
         Z_LARGE=Z_SMALL
         Z_SMALL=Z_TEMPO
      ENDIF
      KEEP_HIT=.TRUE.

C difference of the input Z relative to low Z edge (minus tolerance)
C widen tolerance to accept hybrid if track is shifted to Z0=0
      IF(KEEP_HIT)THEN
         MISSZ=INP_Z-(Z_SMALL-Z_TOLER)
         IF(INP_Z0.LT.0.)MISSZ=MISSZ-INP_Z0
         KEEP_HIT=(MISSZ.GE.0.)
      ENDIF

      IF(KEEP_HIT)THEN
         MISSZ=INP_Z-(Z_LARGE+Z_TOLER)
         IF(INP_Z0.GT.0.)MISSZ=MISSZ-INP_Z0
         KEEP_HIT=(MISSZ.LE.0.)
      ENDIF
      
      RETURN
      END
