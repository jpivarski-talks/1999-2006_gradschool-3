*
* $Id: fit_access.F,v 1.2 2002/05/23 14:33:18 dpp Exp $
*
* $Log: fit_access.F,v $
* Revision 1.2  2002/05/23 14:33:18  dpp
*      -> expanded info
*
* Revision 1.1  2001/11/19 23:39:47  dpp
*      -> NEW ROUTINE
*      -> access values in the currect fit
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FIT_ACCESS(ACTION,INDEX,IVAL)
C.......................................................................
C.
C. FIT_ACCESS - access values internal to the fit
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.
C. CREATED   : 22-Oct-2001
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"

      INTEGER ACTION,INDEX,IVAL
      INTEGER VAL_INT
      REAL VAL_REAL
      EQUIVALENCE (VAL_INT,VAL_REAL)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------


C-----------------------------------------------------------------------
C number of hits in the fit
C-----------------------------------------------------------------------
      IF(ACTION.EQ.FIT_ACCESS_NFIT)THEN
         IVAL=NFIT

C-----------------------------------------------------------------------
C number of LF hits
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_NHITLF)THEN
         IVAL=NHITLF

C-----------------------------------------------------------------------
C number of DR Axial
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_NLFDRA)THEN
         IVAL=NLFDRA

C-----------------------------------------------------------------------
C number of DR stereo U
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_NLFDRU)THEN
         IVAL=NLFDRU

C-----------------------------------------------------------------------
C number of DR stereo V
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_NLFDRV)THEN
         IVAL=NLFDRV

C-----------------------------------------------------------------------
C number of hits used in last fit done, NHIT3F
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_NHIT3F)THEN
         IVAL=NHIT3F

C-----------------------------------------------------------------------
C chisquare of the r-phi fit
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_CHICF)THEN
         VAL_REAL=CHICF
         IVAL=VAL_INT

C-----------------------------------------------------------------------
C curvature, KAPCF
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_KAPCF)THEN
         VAL_REAL=KAPCF
         IVAL=VAL_INT


C-----------------------------------------------------------------------
C distance of closest approach, D0
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_D0CF)THEN
         VAL_REAL=D0CF
         IVAL=VAL_INT


C-----------------------------------------------------------------------
C Z intercept, Y0LF
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_Y0LF)THEN
         VAL_REAL=Y0LF
         IVAL=VAL_INT


C-----------------------------------------------------------------------
C Z0 bias in fit
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_Z0BIAS)THEN
         VAL_REAL=Z0BIAS
         IVAL=VAL_INT

C-----------------------------------------------------------------------
C set the Z0 bias in fit
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_Z0BIAS_SET)THEN
         VAL_INT=IVAL
         Z0BIAS=VAL_REAL

C-----------------------------------------------------------------------
C set the radius of pattern recognition limit in fit
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.FIT_ACCESS_RPRLIM_SET)THEN
         VAL_INT=IVAL
         RPRLIM=VAL_REAL

C-----------------------------------------------------------------------
      ENDIF
      RETURN
      END








