*
* $Id: cfft4d.F,v 1.6 2001/11/19 23:42:44 dpp Exp $
*
* $Log: cfft4d.F,v $
* Revision 1.6  2001/11/19 23:42:44  dpp
*      -> add new control parameter include file
*
* Revision 1.5  2000/10/02 21:09:07  dpp
*       -> change CU granularity to .001 (fixed; not scaled)
*
* Revision 1.4  2000/08/18 18:58:11  dpp
*      -> error handling for PARXTR,INVERS, argument for PARXTR
*      -> minimum granualarity for curvature
*      -> reduce chi2 by ndf
*
* Revision 1.3  1999/09/12 22:35:49  dpp
* remove arguments, use CF directly;add argument in call to CFFTX2
*
* Revision 1.2  1998/07/10 23:43:54  lyon
* Removed annoying prints
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.3  1996/06/24 21:19:58  dpp
* include new argument for PARXTR
*
* Revision 1.2  1996/04/03 16:18:44  dpp
* bug fix; was BINSZS(24); now (4)
*
* Revision 1.1  1996/01/24 21:14:06  zfiles
* Initial version. CF fitter with kink, called like CFCFIT
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE CFFT4D
C.......................................................................
C.
C. CFFT4D - Circle fit with kink in r-phi, non-linear, iterative
C.          XIFIT is not needed as input
C.
C. COMMON    : /FVARI/ /FVARR/ /FVARC/
C. CALLS     : CDCFID,CFFTX2,PARXTR
C. CALLED    : TFXFIT,SFFTRK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.01
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

C== local
      LOGICAL DUMP_PARXTR,DUMP_IN_CH2,DUMP_INITIAL_CH2
      LOGICAL PASS_FIT
      INTEGER IER_PARXTR

C== PARXTR variables
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
     3      BINSZS(MVAR), BNSZL1(MVAR), BNSZL2(MVAR)
      COMMON/FVARC/
     1      NMVARF(MVAR), NMVARX(MVARX)

C== DOIT variables
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tftrakcd.inc"


C ----------Executable code starts here---------------------

      XICF= SQRT(ALPCF**2+BETCF**2)
      CALL CDCFID( CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)

C-----------------------------------------------------------------------
C set up the fitter
C-----------------------------------------------------------------------
      DUMP_INITIAL_CH2 = .FALSE.
      DUMP_PARXTR =      .FALSE.
      DUMP_IN_CH2 =      .FALSE.

      CALL CFFTX2( CURCF,PHI0CF,D0CF,PHI6CF,RKN6CF,
     2             NHITCF,CH2NOW,DUMP_INITIAL_CH2)

      IF(FTK6CF.EQ.KABG2_FLOAT)THEN
         NVARF  =  4
      ELSE
         NVARF  =  3
      ENDIF
      
      IF(NFIT.GT.NVARF)THEN

         NMVARF(1) =  'CUR '
         NMVARF(2) =  'PHI0'
         NMVARF(3) =  'D0'
         NMVARF(4) =  'KINK'

c         BINSZS(1) =  .001000*CURCF
c         IF(ABS(BINSZS(1)).LT. 0.0005)BINSZS(1)=.000500
         BINSZS(1) =  .001000

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
         
         VARNOW(1) =  CURCF
         VARNOW(2) =  PHI0CF
         VARNOW(3) =  D0CF
         VARNOW(4) =  PHI6CF

         NVARX     =  1
         NMVARX(1) =  'RADK'
         VARAXN(1) =  RKN6CF

C-----------------------------------------------------------------------
C call the fitter
C-----------------------------------------------------------------------

         CALL PARXTR(DUMP_PARXTR,DUMP_IN_CH2,.TRUE.,IER_PARXTR)

#if defined(CLEO_DPPDIAG)
         PRINT 9001,IER_PARXTR,CH2NOW
 9001    FORMAT(' CFFT4D: return from PARXTR, ERR=',I4,
     2        '  CH2NOW=',F12.4)
#endif

         IF(IER_PARXTR.EQ.0)THEN
            PASS_FIT=.TRUE.
         ELSE
            PASS_FIT=.FALSE.
         ENDIF
      ELSE
         PASS_FIT=.FALSE.
      ENDIF

C-----------------------------------------------------------------------
C transfer
C from    parabolic extrapolation of chi-squared variables
C to      fitting routine and chi-squared calculation routine variables
C-----------------------------------------------------------------------

      IF(PASS_FIT)THEN
         CHICF = CH2NOW/FLOAT(NFIT-NVARF)
         CURCF = VARNOW(1)
         PHI0CF= VARNOW(2)
         D0CF  = VARNOW(3)
         PHI6CF= VARNOW(4)

         CALL CDKABG( CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)
         
      ELSE
         CHICF = CHIDEF
      ENDIF


      RETURN
      END
