*
* $Id: cdcanh.F,v 1.3 2001/11/19 23:49:18 dpp Exp $
*
* $Log: cdcanh.F,v $
* Revision 1.3  2001/11/19 23:49:18  dpp
*      -> delete unused include file
*
* Revision 1.2  1999/05/28 03:04:54  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/01/24 22:38:51  zfiles
* change argument in call to TFPHTR to
* included a passed value of kink=0; both kink6 and kink7 are treated locally
*
* Revision 1.1.1.1  1994/10/08 01:00:51  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/14 16/07/92  16.57.34  by  Dan Peterson
*CMZ :  5.54/03 27/02/91  13.05.35  by  Steve Schaffner
*CMZ :  5.54/00 27/10/90  16.48.31  by  Steve Schaffner
*CMZ :  5.53/07 11/10/90  22.58.28  by  Steve Schaffner
*CMZ :  5.53/06 05/08/90  22.09.06  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.46.37  by  Steve Schaffner
*CMZ :  5.50/00 14/09/88  19.23.49  by  Dan Riley
*-- Author :
      SUBROUTINE CDCANH
C.......................................................................
C.
C. CDCANH - Cancel hits on current track, even if not used in fit
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 04-Sep-88
C.
C. Modification Log.
C.
C. 10-Apr-87 : Protect against bad IWIRE. This is TEMP. R. Namjoshi
C. 29-Aug-88 : Extend over all CF layers.  -D. Riley
C.             Does all this TEMP stuff ever go away???
C. 04-Sep-88 : Complicated CDCANH considerably by adding the kink,
C.             searching a range of wires, and checking the residuals.
C.             Made the TEMP stuff go away.   -D. Riley
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfhitscd.inc"

      REAL HARPO, ZEPPO, CHICO, GROCHO, GUMMO
      REAL DPHI, D0, PHI0, RKINK6,RKINK7
      REAL XT, YT, PHIT,Z2PI,YSAG,XHT,YHT,RES,RESMAX,DD,WGT,CDWGTD
      LOGICAL LPASS6,LPASS7
      INTEGER IBRNCH, ILYR, IER, IWIRE, IADR,NSERF,IWIRE0,IWIRF,IADRW
* ----------Executable code starts here---------------------


      HARPO  = ALPCF
      ZEPPO  = BETCF
      CHICO  = KAPCF
      GROCHO = GAMCF
      LPASS6 = .FALSE.
      LPASS7 = .FALSE.

      IF (LKN6TF)  THEN
          RKINK6  = RSCATF(IKN6TF)
      ELSE
          RKINK6  = 1.0E10
      ENDIF
      IF (LKN7TF)  THEN
          RKINK7  = RSCATF(IKN7TF)
      ELSE
          RKINK7  = 1.0E10
      ENDIF

      DO 10 ILYR = LLYRCF, MLYRCF
          NSERF  = NSRCTF(ILYR)

C--> Deal with the kink.
          IF (.NOT.LPASS6 .AND. RTF(ILYR).GT.RKINK6) THEN
              DPHI  = PHI6CF
              D0    = D0CF - RKINK6*DPHI
              PHI0  = PHI0CF + DPHI
              CALL CDKABG(CURCF, PHI0, D0,
     +                    CHICO, HARPO, ZEPPO, GROCHO, GUMMO)
              LPASS6 = .TRUE.
          ENDIF
          IF (.NOT.LPASS7 .AND. RTF(ILYR).GT.RKINK7) THEN
              DPHI  = PHI7CF
              D0    = D0CF - RKINK7*DPHI
              PHI0  = PHI0CF + DPHI
              CALL CDKABG(CURCF, PHI0, D0,
     +                    CHICO, HARPO, ZEPPO, GROCHO, GUMMO)
              LPASS7 = .TRUE.
          ENDIF

          IBRNCH = 1
          CALL TFPHTR(HARPO, ZEPPO, CHICO, GROCHO, 0.,0.,
     2                RSQTF(ILYR), IBRNCH, XT, YT, PHIT, IER)

          IF (IER.EQ.0) THEN
              IWIRE0 = NINT(Z2PI(PHIT-PHIFTF(ILYR))/CELLTF(ILYR))
              DO 20 IWIRF = IWIRE0-NSERF, IWIRE0+NSERF
                  IWIRE = IWIRF
                  IF(IWIRE.LT.0)            IWIRE = IWIRE + NWIRTF(ILYR)
                  IF(IWIRE.GE.NWIRTF(ILYR)) IWIRE = IWIRE - NWIRTF(ILYR)
                  IADR = INDXTF(ILYR) + IWIRE
                  IADRW = IADRHW(IADR)
                  IF (IADRW.GT.0) THEN
                      YSAG = 0.0
                      CALL CFXYHT(IADRW,ILYR,YSAG,XHT,YHT,IER)
                      IF (IER.EQ.0) THEN
                          RES    = ABS(CHICO*(XHT**2 + YHT**2)
     +                           + HARPO*XHT+ZEPPO*YHT + GROCHO)
                          RESMAX = ERRCAN*SIGLCF*SIGFTF(ILYR)

C-> Simple test to see if it's worth our while to calculate the full
C   drift-distance-dependent resolution.  I haven't checked that this
C   is really reasonable...

C-A Lyon-5-24-99                          IF (RES.LT.4.0*RESMAX) THEN
C-A Lyon-5-24-99                              DD = DWCD(IADRW)
C-A Lyon-5-24-99                              WGT = CDWGTD(ILYR, DD)
C-A Lyon-5-24-99                              IF (WGT.GT.0.0) THEN
C-A Lyon-5-24-99                                  RESMAX = ERRCAN*SIGLCF/SQRT(WGT)
C-A Lyon-5-24-99                              ENDIF
C-A Lyon-5-24-99                              IF (RES.LT.RESMAX) IADRHW(IADR) = -IADRW
C-A Lyon-5-24-99                          ENDIF
                      ENDIF
                  ENDIF
20            CONTINUE
          ENDIF
10    CONTINUE
C
      END
