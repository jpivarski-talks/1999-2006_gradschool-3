*
* $Id: lfexer.F,v 1.1.1.1 1998/02/06 19:11:44 dpp Exp $
*
* $Log: lfexer.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.1  1996/06/24 21:15:08  dpp
* calls LFLERF and calculates extrapolation in s-z
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : A. Foland
      SUBROUTINE LFEXER(B,A,V11,V12,V22,D)
C.......................................................................
C.
C. LFERR - Do a CHI2 line fit, return errors
C.
C. COMMON    : ?
C. CALLS     : LFLERF
C. CALLED    : FTEXER
C. AUTHOR    :  A. Foland
C.
C. VERSION   : 1.0
C. CREATED   : 24-Jan-96 
C. LAST MOD  : 
C.
C.
C. Modification Log.
C. 25-Feb-88 : Began life as lflerf.F			     R. Namjoshi
C. 25-Feb-88 : Use +SEQ,TFTIME.                              R. Namjoshi
C. 24-Jan-96 : Change to LFEXER to return only errors        A. Foland
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

      REAL S1,SX,SY,SXX,SXY,WI,XI,YI,DET,A,B,CHI,CHISQ,CDUMMY,GDUMMY
      REAL V11,V12,V22,D
      INTEGER I

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      CALL LFLERF(CDUMMY,A,B,GDUMMY,CHISQ,S1,SX,SY,SXX,SXY,DET)

C Change from Sxx etc to the real error matrix entries

      V11=S1/DET
      V12=-SX/DET
      V22=SXX/DET
      D=DET

      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"





