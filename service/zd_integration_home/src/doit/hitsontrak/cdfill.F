*
* $Id: cdfill.F,v 1.2 2003/03/05 18:00:34 dpp Exp $
*
* $Log: cdfill.F,v $
* Revision 1.2  2003/03/05 18:00:34  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/05/10 12:50:24  zfiles
* Skip any QDIV hits that are in the list.  They will be added by TF3FIT.
*
* Revision 1.1.1.1  1994/10/08  01:00:52  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 28/05/91  13.39.07  by  Steve Schaffner
*CMZ :  5.53/07 18/09/90  22.45.27  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.11  by  Steve Schaffner
*CMZ :  5.50/00 12/10/88  22.02.40  by  Dan Riley
*-- Author :    Dan Riley   12/10/88
      SUBROUTINE CDFILL(ITRK, IBRNCH)
C.......................................................................
C.
C. CDFILL - Fills TFTRAK with the hits for track ITRK
C.
C. COMMON    : ?
C. CALLS     : ?
C. CALLED    : DUET
C. AUTHOR    : M. Ogg, rewritten by D. Riley
C.
C. VERSION   : 1.10
C. CREATED   : ?
C. LAST MOD  : 12-Oct-88
C.
C. Modification Log.
C.
C. 12-Oct-88 : Rewritten in F77 and extensively revised.  -D. Riley
C.
C. 5-May-96 : Rob Kutschke.
C.            Until now, QDIV hits were not filled into the
C.            the CDTRAKCD hit lists after TF3FIT/KLMN.
C.            Now they are.
C.            However TF3FIT still expects to add the QDIV hits
C.            on its own; so, if there are any in the list, drop them.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
      INTEGER IFIT,IWIRE,ITRK,IAMB,ILYR,IADR,JLYR,IBRNCH,IER
      REAL    XTR,YTR,PHITR,ARCFUN
* ----------Executable code starts here---------------------

      DO 10 IFIT=1,NLYRTF
          IWIRE  = IHWRCD(IFIT,ITRK)
          IF (IWIRE.NE.0.AND.NFIT.LT.NLR2TF) THEN
              IAMB   = ISIGN(1,IWIRE)
              IWIRE  = IABS(IWIRE)
              ILYR   = IWIRE/IWRPTF
              IF ( ILYR .GT. QDIVAD ) GOTO 10
              IWIRE  = MOD(IWIRE,IWRPTF)
              IADR   = IABS(EvntHit_MapDet(INDXTF(ILYR) + IWIRE))

              IF (IDVCTF(ILYR).EQ.IVDCT.OR.IDVCTF(ILYR).EQ.IDRCT) THEN
                  JLYR = ILCRTF(ILYR)
              ELSE
                  JLYR = ILYR
              ENDIF

              CALL TFXYTR(ALPCF,BETCF,KAPCF,GAMCF,RSQTF(JLYR),
     +                    IBRNCH,XTR,YTR,PHITR,IER)
              NFIT   = NFIT + 1
              SFIT  (NFIT) = ARCFUN(KAPCF,ALPCF,BETCF,XTR,YTR,XICF)
              IADFIT(NFIT) = ISIGN(IADR,IAMB)
              IPLFIT(NFIT) = ILYR
              WGTFIT(NFIT) = WGTFTF(ILYR)

              IF (ILYR.GE.LLYRCF .AND. ILYR.LE.MLYRCF) THEN
                  NHITCF = NHITCF + 1
              ELSE
                  NHITLF = NHITLF + 1
              ENDIF

              IF (LQDVTF(ILYR).AND.EvntHit_NxtCls(IADR).GT.0) THEN
                  NHTQLF = NHTQLF + 1
              ENDIF
          ENDIF
10    CONTINUE

      END
