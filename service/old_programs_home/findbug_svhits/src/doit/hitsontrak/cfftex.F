*
* $Id: cfftex.F,v 1.3 2000/08/18 18:51:20 dpp Exp $
*
* $Log: cfftex.F,v $
* Revision 1.3  2000/08/18 18:51:20  dpp
*      -> save err matrix elements with ERR_MATRIX_ELEMENT
*      -> add kink angle and radius to argument list
*      -> new arguments for KABG2
*      -> error handling for PARXTR,INVERS, argument for PARXTR
*      -> new argument for INVERS
*      -> minimum granualarity for curvature
*
* Revision 1.2  1999/09/12 22:23:56  dpp
* add argument to call to CFFTX2
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.1  1996/06/24 21:16:12  dpp
* call PARXTR to calculate error matrix
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : A. Foland
      SUBROUTINE CFFTEX(
     1     KAARG, ALARG, BEARG, GAARG, KNARG, RADARG,
     2     D, PHI, C, ERR_RET)

C.......................................................................
C.
C. CFFTEX - Circle fit with kink in r-phi, non-linear, iterative
C.          to get the error matrix
C.
C. COMMON    : /FVARI/ /FVARR/ /FVARC/
C. CALLS     : CDCFID,CFFTX2,PARERR
C. CALLED    : FTEXER
C. AUTHOR    : A. Foland
C.
C. VERSION   : 1.01
C. CREATED   : 18-Dec-95
C. LAST MOD  : 
C.
C.
C. Modification Log.
C. 1-24-96 ADF Took from cfft4d.F and changed inputs to make CFFTEX
C. 1-29-96 ADF Changed commons to add ERRMTX
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"

C== Argument declarations
C INPUTS
C ======
      REAL KAARG, ALARG, BEARG, GAARG
      REAL KNARG, RADARG
C RETURNS
C =======
      REAL D, PHI, C
      INTEGER ERR_RET

C== local
      REAl CHISQ, CHISQA
      REAL XIFIT,CUFIT, FIFIT, D0FIT
      LOGICAL DUMP_PARXTR,DUMP_IN_CH2,DUMP_INITIAL_CH2
      INTEGER IER_PARXTR
      INTEGER NHIT_RET
      REAL ERR_MAT_TMP

C== variables
      INTEGER MVAR,MVARX
      PARAMETER(MVAR=6,MVARX=6)

      INTEGER     NVARF, NVARX
      REAL        CH2NOW
      REAL*8	  ERRMTX
      REAL        VARNOW,VARAXN,BINSZS,BNSZL1,BNSZL2
      CHARACTER*4 NMVARF,NMVARX
      COMMON/FVARI/
     1      NVARF,     NVARX
      COMMON/FVARR/
     1      CH2NOW,
     2      VARNOW(MVAR), VARAXN(MVARX),
     3      BINSZS(MVAR), BNSZL1(MVAR), BNSZL2(MVAR)
      COMMON/FVARC/
     1      NMVARF(MVAR), NMVARX(MVARX)
      COMMON/FVARER/
     1      ERRMTX(MVAR,MVAR)

C ----------Executable code starts here---------------------


      XIFIT= SQRT(ALARG**2+BEARG**2)


      CALL CDCFID(CUFIT,FIFIT,D0FIT,KAARG,ALARG,BEARG,GAARG,XIFIT)

C-----------------------------------------------------------------------
C set up the fitter
C-----------------------------------------------------------------------
      DUMP_INITIAL_CH2 = .FALSE.
      DUMP_PARXTR =      .FALSE.
      DUMP_IN_CH2 =      .FALSE.

      CALL CFFTX2( CUFIT, FIFIT, D0FIT, KNARG, RADARG, 
     2             NHIT_RET, CH2NOW, DUMP_INITIAL_CH2)

      NVARF     =  3

      NMVARF(1) =  'CUR '
      NMVARF(2) =  'PHI0'
      NMVARF(3) =  'D0'
      NMVARF(4) =  'KINK'

      BINSZS(1) =  .001000*CUFIT
      IF(ABS(BINSZS(1)).LT. 0.0005)BINSZS(1)=.000500
      BINSZS(2) =  .000500
      BINSZS(3) =  .000050
      BINSZS(4) =  .000500

      BNSZL1(1) =  .1*BINSZS(1)
      BNSZL1(2) =  .1*BINSZS(2)
      BNSZL1(3) =  .1*BINSZS(3)
      BNSZL1(4) =  .1*BINSZS(4)

      BNSZL2(1) =  .5*BNSZL1(1)
      BNSZL2(2) =  .2*BNSZL1(2)
      BNSZL2(3) =  .2*BNSZL1(3)
      BNSZL2(4) =  .2*BNSZL1(4)

      VARNOW(1) =  CUFIT
      VARNOW(2) =  FIFIT
      VARNOW(3) =  D0FIT
      VARNOW(4) =  KNARG

      NVARX     =  0
      NMVARX(1) =  'RADK'
      VARAXN(1) =  RADARG

C-----------------------------------------------------------------------
C call the fitter,
C  with 3rd argument FALSE to return error matrix without fitting
C-----------------------------------------------------------------------

      CALL PARXTR(DUMP_PARXTR,DUMP_IN_CH2,.FALSE.,IER_PARXTR)

#if defined(CLEO_TFXFDIAG)
      PRINT 9001,IER_PARXTR,CH2NOW
 9001 FORMAT(' CFFTEX: return from PARXTR, ERR=',I4,'  CH2NOW=',F12.4)
#endif

      IF(IER_PARXTR.NE.0)THEN
         ERR_RET=1
      ELSE

C-----------------------------------------------------------------------
C transfer
C from    parabolic extrapolation of chi-squared variables
C to      fitting routine and chi-squared calculation routine variables
C-----------------------------------------------------------------------

         CHISQ = CH2NOW
         CHISQA= CHISQ
         CUFIT = VARNOW(1)
         FIFIT = VARNOW(2)
         D0FIT = VARNOW(3)
         KNARG = VARNOW(4)


C load values to transfer to extrapolation error calculation

         D=D0FIT
         C=CUFIT
         PHI=FIFIT
         ERR_RET=0


C P3DCU,  P3DFI,  P3DDA,  P3DCT,  P3DZ0,  P3DK6,  P3DK7
C do not use ERRMTX as argument; it is real*8; convert to real*4

         ERR_MAT_TMP=ERRMTX(1,1)
         CALL ERR_MATRIX_ELEMENT(1,P3DCU,P3DCU,ERR_MAT_TMP)

         ERR_MAT_TMP=ERRMTX(2,2)
         CALL ERR_MATRIX_ELEMENT(1,P3DFI,P3DFI,ERR_MAT_TMP)

         ERR_MAT_TMP=ERRMTX(3,3)
         CALL ERR_MATRIX_ELEMENT(1,P3DDA,P3DDA,ERR_MAT_TMP)

         ERR_MAT_TMP=ERRMTX(2,1)
         CALL ERR_MATRIX_ELEMENT(1,P3DFI,P3DCU,ERR_MAT_TMP)
         ERR_MAT_TMP=ERRMTX(3,1)
         CALL ERR_MATRIX_ELEMENT(1,P3DDA,P3DCU,ERR_MAT_TMP)
         ERR_MAT_TMP=ERRMTX(3,2)
         CALL ERR_MATRIX_ELEMENT(1,P3DDA,P3DFI,ERR_MAT_TMP)
         
      ENDIF

      RETURN
      END











