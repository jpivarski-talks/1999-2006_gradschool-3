*
* $Id: duhold.F,v 1.2 2003/03/05 18:07:53 dpp Exp $
*
* $Log: duhold.F,v $
* Revision 1.2  2003/03/05 18:07:53  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.1.1.1  1998/02/06 19:11:45  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:36  clib
* Developmental version of DUET.
*
* Revision 1.5  1996/06/24 21:25:04  dpp
* add code to copy EvntHit_NHitDev <-> DVHTCX
* add cdscrtcd.inc; now needed to define EvntHitMax
*
* Revision 1.4  1996/04/26 15:04:14  zfiles
* Bring up to date with cdtrakcd.inc
*
* Revision 1.3  1996/04/23 19:23:01  dpp
* add more variables to save for multibunch comparison
*
* Revision 1.2  1996/01/25 16:27:54  zfiles
* 4 status bit words
*
* Revision 1.1  1995/11/27 19:43:04  nk
* copy between /CDTRAKCD/ and /CDHOLD/
*
*
C-------------------------------------------------------------
      SUBROUTINE DUHOLD(IBN,CHDIR)
C
C  Copy cdtrakcd.inc  <--> cdhold.inc
C
C       IBN = bunch number
C       CHDIR = 'HOLD' or 'RSTR'
C                                              11/13/95  PCK
C-------------------------------------------------------------

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdtrakcd.inc"
C cdscrtcd.inc is needed only to define parameter EvntHitMax
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdhold.inc"

      INTEGER IBN,IX,IY
      CHARACTER*4 CHDIR

C---------------------------
C    CDTRAKCD --> CDHOLD
C---------------------------

      IF(CHDIR.EQ.'HOLD') THEN

          NTRKIX(IBN) = NTRKIN
          NTRKCX(IBN) = NTRKCD
          LTRKCX(IBN) = LTRKCD
C ADF 6/9/96 Add lines to save EvntHit_NHitDev
	DO IX=1,EvntHitMax
	   DVHTCX(IX,IBN)=EvntHit_NHitDev(IX)
	ENDDO
        DO 100 IX=1,MTRKCD
          IPNTCX(IX,IBN) = IPNTCD(IX)
          INXTCX(IX,IBN) = INXTCD(IX)
           KINCX(IX,IBN) =  KINCD(IX)
          NHTLVX(IX,IBN) = NHTLVD(IX)
          NHTAIX(IX,IBN) = NHTAIZ(IX)
          NHTADX(IX,IBN) = NHTADR(IX)
          NHTSCX(IX,IBN) = NHTSCT(IX)
          NHTSDX(IX,IBN) = NHTSDR(IX)
           IDRCX(IX,IBN) =  IDRCD(IX)
          CALL UCOPY(STATCD(1,IX),STATCX(1,IX,IBN),4)
          KBQDCX(IX,IBN) = KBQDCD(IX)
          NDOFCX(IX,IBN) = NDOFCD(IX)
          ISVTCX(IX,IBN) = ISVTCD(IX)
          NFITCX(IX,IBN) = NFITCD(IX)
          DO 110 IY=1,KLR2TF
            IPLRCX(IY,IX,IBN) = IPLRCD(IY,IX)
            IHWRCX(IY,IX,IBN) = IHWRCD(IY,IX)
            IADFCX(IY,IX,IBN) = IADFCD(IY,IX)
 110      CONTINUE
           CURCX(IX,IBN) =  CURCD(IX)
          PHI0CX(IX,IBN) = PHI0CD(IX)
            D0CX(IX,IBN) =   D0CD(IX)
          PHI6CX(IX,IBN) = PHI6CD(IX)
          PHI7CX(IX,IBN) = PHI7CD(IX)
          RKN7CX(IX,IBN) = RKN7CD(IX)
          TANDCX(IX,IBN) = TANDCD(IX)
            Z0CX(IX,IBN) =   Z0CD(IX)
          XYZ0CX(1,IX,IBN) = XYZ0CD(1,IX)
          XYZ0CX(2,IX,IBN) = XYZ0CD(2,IX)
          XYZ0CX(3,IX,IBN) = XYZ0CD(3,IX)
          CHI2CX(IX,IBN) = CHI2CD(IX)
          CHIZCX(IX,IBN) = CHIZCD(IX)
          CHI3CX(IX,IBN) = CHI3CD(IX)
          RESICX(IX,IBN) = RESICD(IX)
          CHI3PX(IX,IBN) = CHI3PT(IX)
          CHI3VX(IX,IBN) = CHI3VD(IX)
          CHI3DX(IX,IBN) = CHI3DR(IX)
          CHI3TX(IX,IBN) = CHI3CT(IX)
          RESMCX(IX,IBN) = RESMCD(IX)
          NN2CCX(IX,IBN) = NN2CCD(IX)
          ARC1CX(IX,IBN) = ARC1CD(IX)
          ARC2CX(IX,IBN) = ARC2CD(IX)
          X0LHCX(IX,IBN) = X0LHCD(IX)
          Y0LHCX(IX,IBN) = Y0LHCD(IX)
          X0INCX(IX,IBN) = X0INCD(IX)
          Y0INCX(IX,IBN) = Y0INCD(IX)
        DO 120 IY=1,KLR2TF
          PULLCX(IY,IX,IBN) = PULLCD(IY,IX)
          SIG1CX(IY,IX,IBN) = SIG1CD(IY,IX)
          SIG2CX(IY,IX,IBN) = SIG2CD(IY,IX)
          MEASCX(IY,IX,IBN) = MEASCD(IY,IX)
 120    CONTINUE
        DO 130 IY=1,NERMTF
 130      ERRMCX(IY,IX,IBN) = ERRMCD(IY,IX)

 100    CONTINUE

      ENDIF


C---------------------------
C  CDHOLD --> CDTRAKCD
C---------------------------

      IF(CHDIR.EQ.'RSTR') THEN

          NTRKIN = NTRKIX(IBN)
          NTRKCD = NTRKCX(IBN)
          LTRKCD = LTRKCX(IBN)
C ADF 6/9/96 Add lines to save EvntHit_NHitDev.
	DO IX=1,EvntHitMax
	   EvntHit_NHitDev(IX)=DVHTCX(IX,IBN)
	ENDDO
        DO 200 IX=1,MTRKCD
          IPNTCD(IX) = IPNTCX(IX,IBN)
          INXTCD(IX) = INXTCX(IX,IBN)
           KINCD(IX) = KINCX(IX,IBN)
          NHTLVD(IX) = NHTLVX(IX,IBN)
          NHTAIZ(IX) = NHTAIX(IX,IBN)
          NHTADR(IX) = NHTADX(IX,IBN)
          NHTSCT(IX) = NHTSCX(IX,IBN)
          NHTSDR(IX) = NHTSDX(IX,IBN)
           IDRCD(IX) = IDRCX(IX,IBN)
          CALL UCOPY(STATCX(1,IX,IBN),STATCD(1,IX),4)
          KBQDCD(IX) = KBQDCX(IX,IBN)
          NDOFCD(IX) = NDOFCX(IX,IBN)
          ISVTCD(IX) = ISVTCX(IX,IBN)
          NFITCD(IX) = NFITCX(IX,IBN)
        DO 210 IY=1,KLR2TF
          IPLRCD(IY,IX) = IPLRCX(IY,IX,IBN)
          IHWRCD(IY,IX) = IHWRCX(IY,IX,IBN)
          IADFCD(IY,IX) = IADFCX(IY,IX,IBN)
 210    CONTINUE
           CURCD(IX) = CURCX(IX,IBN)
          PHI0CD(IX) = PHI0CX(IX,IBN)
            D0CD(IX) = D0CX(IX,IBN)
          PHI6CD(IX) = PHI6CX(IX,IBN)
          PHI7CD(IX) = PHI7CX(IX,IBN)
          RKN7CD(IX) = RKN7CX(IX,IBN)
          TANDCD(IX) = TANDCX(IX,IBN)
            Z0CD(IX) = Z0CX(IX,IBN)
          XYZ0CD(1,IX) = XYZ0CX(1,IX,IBN)
          XYZ0CD(2,IX) = XYZ0CX(2,IX,IBN)
          XYZ0CD(3,IX) = XYZ0CX(3,IX,IBN)
          CHI2CD(IX) = CHI2CX(IX,IBN)
          CHIZCD(IX) = CHIZCX(IX,IBN)
          CHI3CD(IX) = CHI3CX(IX,IBN)
          RESICD(IX) = RESICX(IX,IBN)
          CHI3PT(IX) = CHI3PX(IX,IBN)
          CHI3VD(IX) = CHI3VX(IX,IBN)
          CHI3DR(IX) = CHI3DX(IX,IBN)
          CHI3CT(IX) = CHI3TX(IX,IBN)
          RESMCD(IX) = RESMCX(IX,IBN)
          NN2CCD(IX) = NN2CCX(IX,IBN)
          ARC1CD(IX) = ARC1CX(IX,IBN)
          ARC2CD(IX) = ARC2CX(IX,IBN)
          X0LHCD(IX) = X0LHCX(IX,IBN)
          Y0LHCD(IX) = Y0LHCX(IX,IBN)
          X0INCD(IX) = X0INCX(IX,IBN)
          Y0INCD(IX) = Y0INCX(IX,IBN)
        DO 220 IY=1,KLR2TF
          PULLCD(IY,IX) = PULLCX(IY,IX,IBN)
          SIG1CD(IY,IX) = SIG1CX(IY,IX,IBN)
          SIG2CD(IY,IX) = SIG2CX(IY,IX,IBN)
          MEASCD(IY,IX) = MEASCX(IY,IX,IBN)
 220    CONTINUE
        DO 230 IY=1,NERMTF
 230      ERRMCD(IY,IX) = ERRMCX(IY,IX,IBN)

 200    CONTINUE

      ENDIF

      RETURN
      END


