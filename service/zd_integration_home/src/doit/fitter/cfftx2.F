*
* $Id: cfftx2.F,v 1.6 2002/09/05 19:23:19 dpp Exp $
*
* $Log: cfftx2.F,v $
* Revision 1.6  2002/09/05 19:23:19  dpp
*      -> require STRFFT.GT.0 instead of .NE.0
*         now using STRFFT=-1 to designate orphaned hits
*
* Revision 1.5  2001/11/19 23:43:12  dpp
*      -> add new control parameter include file
*
* Revision 1.4  2000/08/18 18:57:16  dpp
*      -> change comment
*      -> new arguments for KABG2
*
* Revision 1.3  2000/04/26 14:55:47  dpp
*      -> NO CHANGE
*      -> MUST use RESIDUALS, but be careful with the kink
*         maybe require a flag that the kabg2 has been called after the fit
*         or call kabg2 as part ofthe fit
*         maybe require using kapaft6 instead of a local variable
*
* Revision 1.2  1999/09/12 22:33:05  dpp
* require fit status flag STRFFT; add argument for number of hits found
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.1  1996/01/24 21:42:29  zfiles
* initial version: calculate chi squared for CFFT4D using tftrakcd
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE CFFTX2(CUARG,FIARG,D0ARG,KNARG,RADKN,
     2                  NHIT_RET,CHISQ,DUMP_LOCAL)
C.......................................................................
C.
C. CFFTM2    : calculates chi2 for input track parameters
C.
C. COMMON    : TFTRAKCD
C. CALLS     : SORTZV
C. CALLED    : CFFT4D,DEFCH2
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 12-Dec-95
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

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tftrakcd.inc"

      REAL    CUARG, FIARG, D0ARG, KNARG, RADKN, CHISQ
      INTEGER NHIT_RET
      LOGICAL DUMP_LOCAL
      REAL    KA,    AL,    BE,    GA,    XI
      REAL    CU,    FI,    D0
      INTEGER POINT(KLR2TF)
      INTEGER IFIT,  JFIT,  AFTERK 
      REAL    XNOW,  YNOW,  WNOW,  WSUM,  RESID
      REAL    RRK,   RRH

* ----------Executable code starts here---------------------
      AFTERK=0
      RRK=RADKN**2
      CU=CUARG
      FI=FIARG
      D0=D0ARG
      CALL CDKABG(CU,FI,D0,KA,AL,BE,GA,XI)
      IF(DUMP_LOCAL)PRINT 1001,KA,AL,BE,GA,XI,KNARG,RADKN
      CALL SORTZV(SFIT, POINT, NFIT, 1,0,0)
      CHISQ  = 0.
      WSUM   = 0.
      NHIT_RET=0

c could call RESIDUALS but that does not have the status check

      DO 49 IFIT = 1, NFIT
         JFIT=POINT(IFIT)
         IF((IADFIT(JFIT).NE.0).AND.(STRFFT(JFIT).GT.0))THEN
            NHIT_RET=NHIT_RET+1
            XNOW = XFIT(JFIT)
            YNOW = YFIT(JFIT)
            RRH= XNOW**2+YNOW**2

C change the track parameters at the kink
C only done once because the hits are ordered
C XI is fresh from call to CDKABG
            IF((AFTERK.EQ.0).AND.(RRH.GT.RRK))THEN
               AFTERK=1
               CALL KABG2(KABG2_CALC,
     1              0,0,0,KABG2_XI_OK,              
     2              KA, AL, BE, GA, XI, KNARG,
     3              KA, AL, BE, GA, XI)
               IF(DUMP_LOCAL)PRINT 1002,KA,AL,BE,GA,XI
            ENDIF

C continue with calculation of chi squared
            WNOW   = WGTFIT(JFIT)
            WSUM   = WSUM   + WNOW
            RESID  = RRH*KA + XNOW*AL + YNOW*BE + GA 
            CHISQ  = CHISQ + RESID**2 * WNOW
            IF(DUMP_LOCAL)PRINT 1003,IFIT,JFIT,IPLFIT(JFIT),RESID,CHISQ
         ENDIF
 49   CONTINUE

 1001 FORMAT(' CFFTX2:ENTER KABGXkr=',7F9.6)
 1002 FORMAT(' CFFTX2:SHIFT KABGX=  ',5F9.6)
 1003 FORMAT(' CFFTX2:',I4,' LAY(',I3')=',I3,
     2                '  RESID=',F10.6,' CHISQ=',F16.3)

      RETURN
      END




