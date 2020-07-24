*
* $Id: tfcov.F,v 1.1 2003/03/05 17:44:48 dpp Exp $
*
* $Log: tfcov.F,v $
* Revision 1.1  2003/03/05 17:44:48  dpp
*      -> NEW ROUTINE, moved from "fitter/."
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.2  2001/11/19 23:40:57  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:27  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:49  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/14 17/07/92  13.34.38  by  Dan Peterson
*CMZ :  5.53/07 13/09/90  13.35.46  by  D. Riley
*-- Author :
      SUBROUTINE TFCOV(GYINV, ALPHA, BETA, KAPPA, GAMMA, XI, TAND)
C. TFCOV - Compute covariance matrix of hits

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"

      REAL GYINV(KGYMTF)
      REAL ALPHA, BETA, KAPPA, GAMMA, XI, TAND
      REAL STOLSQ,STOL,STOLCU,XXT,YYT,PHIT,ARCFUN,TERM1,DELSI,DELSJ
      REAL DELAM,TERM1A,TERM1B,T,TERM2A,TERM2B,EXPTS,TERM2,EXPTA

      REAL DSCTC (2*NSCCTF), DSCTF (2*NSCATF)
      REAL SSCATC(2*NSCCTF), SSCATF(2*NSCATF)
      INTEGER IPNTR(KLR2TF),NFCOVP,IFC,IDIAG,IBRNCH,INDX,ISCT,IER
      INTEGER NSCATM,IPXI,JFC,IPXJ,M,KCV,ISC

      NFCOVP = NFIT

C-> Conversion factor from S to trackLength constant for a helix,
C-> assuming no energy loss.

      STOLSQ = 1 + TAND**2
      STOL   = SQRT(STOLSQ)
      STOLCU = STOLSQ * STOL
      CALL VZERO(GYINV, KGYMTF)
C
C-> MEASUREMENT ERROR
C
C
C-> Contribution from measurement error
C
      DO 20 IFC = 1, NFCOVP
          IF (IPLFIT(IFC).GT.0) THEN
              IDIAG = (IFC*(IFC+1))/2
              GYINV(IDIAG) = 1.0/WGTFIT(IFC)
          ENDIF
 20   CONTINUE
C
C-> CONTINUOUS SCATTERING MEDIA
C
C
C-> Compute track length in each continuous scattering medium
C-> (in SSCCTF) on outward branch.
C
      IBRNCH = 1
      INDX   = 0
C
      DO 100 ISCT = LSCCTF, MSCCTF
          CALL TFXYTR( ALPHA, BETA, KAPPA, GAMMA, RSCCTF(ISCT)**2,
     +                 IBRNCH, XXT, YYT, PHIT, IER)
          IF (IER.EQ.0) THEN
              INDX         = INDX + 1
              SSCATC(INDX) = ARCFUN(KAPPA, ALPHA, BETA, XXT, YYT, XI)
              DSCTC(INDX)  = XSCCTF(ISCT) * (SCTGEV*EETF/P2TF)**2
          ENDIF
 100  CONTINUE
C
C-> Do we encounter any of them on an inward branch?
C
      CALL TFXYTR( ALPHA, BETA, KAPPA, GAMMA, RSQTF(MCAXDR),
     +             IBRNCH, XXT, YYT, PHIT, IER)
      IF (IER.NE.0) THEN
          IBRNCH = -1
          DO 110 ISCT = MSCCTF, LSCCTF, -1
              CALL TFXYTR( ALPHA, BETA, KAPPA, GAMMA, RSCCTF(ISCT)**2,
     +                     IBRNCH, XXT, YYT, PHIT, IER)
              IF (IER.EQ.0) THEN
                  INDX         = INDX + 1
                  SSCATC(INDX) = ARCFUN(KAPPA, ALPHA, BETA,
     +                                  XXT, YYT, XI)
                  DSCTC(INDX)  = XSCCTF(ISCT) * (SCTGEV*EETF/P2TF)**2
              ENDIF
 110      CONTINUE
      ENDIF
C
      NSCATM = INDX
      IF (NSCATM.LE.0) GOTO 133
C
C-> Set up array pointing from a hit back to the last scattering
C-> surface inside of it.
C
      CALL VZERO(IPNTR, KLR2TF)
      IPXI = 0
      DO 121 IFC = 1, NFCOVP
 120      IF (IPXI.LT.NSCATM .AND. SFIT(IFC).GT.SSCATC(IPXI+1)) THEN
              IPXI = IPXI+1
              GOTO 120
          ENDIF
          IPNTR(IFC) = IPXI
 121  CONTINUE
C
C-> Contribution from multiple scattering in continuous medium.
C
      DO 132 IFC = 1, NFCOVP
          IPXI = IPNTR(IFC)
C-> Do not include q div
          IF (IPLFIT(IFC).GT.0.AND.
     +        IPLFIT(IFC).LT.QDIVAD.AND.IPXI.GT.0) THEN
              IDIAG = (IFC*(IFC+1))/2
              DO 131 JFC = 1, IFC
                  IPXJ = IPNTR(JFC)
                  IF (IPLFIT(JFC).GT.0.AND.
     +                IPLFIT(JFC).LT.QDIVAD.AND.IPXJ.GT.0) THEN
                      TERM1 = 0.0
                      DO 130 M = 1, IPXJ-1
                          DELSI  = SFIT(IFC)   - SSCATC(M)
                          DELSJ  = SFIT(JFC)   - SSCATC(M)
                          DELAM  = SSCATC(M+1) - SSCATC(M)
                          TERM1A = DELSI * DELSJ
                          TERM1B = DELAM/3.0 - 0.5*(DELSI + DELSJ)
                          T = DELAM * (TERM1A + DELAM * TERM1B)
                          TERM1  = TERM1 + STOLCU * DSCTC(M) * T
 130                  CONTINUE
C
                      DELSI  = SFIT(IFC) - SSCATC(IPXJ)
                      DELSJ  = SFIT(JFC) - SSCATC(IPXJ)
                      TERM2A = 0.5 * DELSJ**2
                      TERM2B = DELSI - DELSJ/3.0
                      TERM2  = DSCTC(IPXJ) * STOLCU * TERM2A * TERM2B
C
                      KCV = IDIAG - (IFC-JFC)
                      GYINV(KCV) = GYINV(KCV) + TERM1 + TERM2
C
                  ENDIF
 131          CONTINUE
          ENDIF
 132  CONTINUE
C
C-> DISCRETE SCATTERING MEDIA
C
C
C-> Compute distance along track to each discrete scattering
C-> surface (in SSCATF) on outward branch
C-> does not scatter on surfaces that have a kink
 133  IBRNCH = 1
      INDX   = 0
C
      DO 140 ISCT = LSCATF, MSCATF
          IF(    (ISCT.NE.IKN6TF .OR. .NOT. LKN6TF)
     +      .AND.(ISCT.NE.IKN7TF .OR. .NOT. LKN7TF)) THEN
              CALL TFXYTR( ALPHA, BETA, KAPPA, GAMMA, RSCATF(ISCT)**2,
     +                     IBRNCH, XXT, YYT, PHIT, IER)
              IF (IER.EQ.0) THEN
                  INDX = INDX + 1
                  SSCATF(INDX) = ARCFUN(KAPPA, ALPHA, BETA,
     +                                  XXT, YYT, XI)
                  DSCTF(INDX) = XSCATF(ISCT)*STOL*(SCTGEV*EETF/P2TF)**2
              ENDIF
          ENDIF
 140  CONTINUE
C
C-> Do we encounter any of them on an inward branch, ignoring the
C-> beampipe?
C
      CALL TFXYTR( ALPHA, BETA, KAPPA, GAMMA, RSQTF(MCAXDR),
     +             IBRNCH, XXT, YYT, PHIT, IER)
      IF (IER.NE.0) THEN
          IBRNCH = -1
          DO 150 ISCT = MSCATF, LSCATF+1, -1
              IF(    (ISCT.NE.IKN6TF .OR. .NOT. LKN6TF)
     +          .AND.(ISCT.NE.IKN7TF .OR. .NOT. LKN7TF)) THEN
                  CALL TFXYTR( ALPHA, BETA, KAPPA, GAMMA,
     +                     RSCATF(ISCT)**2,
     +                     IBRNCH, XXT, YYT, PHIT, IER)
                  IF (IER.EQ.0) THEN
                      INDX = INDX + 1
                      SSCATF(INDX) = ARCFUN(KAPPA, ALPHA, BETA,
     +                                      XXT, YYT, XI)
                      DSCTF(INDX) = XSCATF(ISCT)*STOL*
     +                              (SCTGEV*EETF/P2TF)**2
                  ENDIF
              ENDIF
 150      CONTINUE
      ENDIF
C
      NSCATM = INDX
      IF (NSCATM.LE.0) GOTO 173
C
C-> Set up array pointing from a hit back to the last scattering
C-> surface inside of it.
C
      CALL VZERO(IPNTR, KLR2TF)
      IPXI = 0
      DO 161 IFC = 1, NFCOVP
 160      IF (IPXI.LT.NSCATM .AND. SFIT(IFC).GT.SSCATF(IPXI+1)) THEN
              IPXI = IPXI+1
              GOTO 160
          ENDIF
          IPNTR(IFC) = IPXI
 161  CONTINUE
C
C-> NOW ADD CONTRIBUTIONS FROM DISCREET SCATTERING LAYERS
C
      DO 172 IFC = 1, NFCOVP
          IPXI = IPNTR(IFC)
          IF (IPLFIT(IFC).GT.0.AND.
     +        IPLFIT(IFC).LT.QDIVAD.AND.IPXI.GT.0) THEN
C
              IDIAG = (IFC*(IFC+1))/2
              DO 171 JFC = 1, IFC
                  IPXJ = IPNTR(JFC)
                  IF (IPLFIT(JFC).GT.0.AND.
     +                IPLFIT(JFC).LT.QDIVAD.AND.IPXJ.GT.0) THEN
C
                      EXPTS  = 0.0
                      DO 170 ISC = 1, MIN(IPXI, IPXJ)
                          EXPTA = (SFIT(JFC)-SSCATF(ISC)) *
     +                            (SFIT(IFC)-SSCATF(ISC))
                          EXPTS = EXPTS + EXPTA*STOLSQ*DSCTF(ISC)
 170                  CONTINUE
C
                      KCV = IDIAG - (IFC-JFC)
                      GYINV(KCV) = GYINV(KCV) + EXPTS
C
                  ENDIF
 171          CONTINUE
          ENDIF
 172  CONTINUE
 173  CONTINUE
      END
