*
* $Id: defch2.F,v 1.2 1999/09/12 22:33:47 dpp Exp $
*
* $Log: defch2.F,v $
* Revision 1.2  1999/09/12 22:33:47  dpp
* add argument for noumber of hits returned from CFFTX2
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.1  1996/01/24 21:47:12  zfiles
* initial version: buffer from PARXTR to CFFTX2
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE DEFCH2(CH2,IERR,DUMP_IN_CH2,DELETE_IN_CH2)
C.......................................................................
C.
C. DEFCH2    : defines chi squared
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 18-Dec-95
C. LAST MOD  : 
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C COMMONs used in parabolic extrapolation of chi-squared

      INTEGER MVAR,MVARX
      PARAMETER(MVAR=6,MVARX=6)

      INTEGER     NVARF, NVARX
      REAL        CH2NOW
      REAL        VARNOW,VARAXN,BINSZS,BNSZL1,BNSZL2
      CHARACTER*4 NMVARF,NMVARX
      COMMON/FVARI/
     1      NVARF,     NVARX
      COMMON/FVARR/
     1      CH2NOW,
     2      VARNOW(MVAR), VARAXN(MVARX),
     3      BINSZS(MVAR) ,BNSZL1(MVAR), BNSZL2(MVAR)
      COMMON/FVARC/
     1      NMVARF(MVAR), NMVARX(MVARX)

C LOCAL VARIABLES

      INTEGER IERR
      LOGICAL DUMP_IN_CH2,DELETE_IN_CH2
      REAL CH2

C CHI SQUARED DETERMINING VARIABLES, and other DOIT variables

      REAL CU,FI,D0,KN,RADKN
      INTEGER NHIT_RET

C EXECUTABLE

C-----------------------------------------------------------------------
C transfer
C from    parabolic extrapolation of chi-squared variables
C to      fitting routine and chi-squared calculation routine variables
C-----------------------------------------------------------------------

      CU=VARNOW(1)
      FI=VARNOW(2)
      D0=VARNOW(3)
      KN=VARNOW(4)
      RADKN=VARAXN(1)

C-----------------------------------------------------------------------
C call the user defined chi squared calculations
C-----------------------------------------------------------------------

      CALL CFFTX2(CU,FI,D0,KN,RADKN,NHIT_RET,CH2,DUMP_IN_CH2)

      RETURN
      END








