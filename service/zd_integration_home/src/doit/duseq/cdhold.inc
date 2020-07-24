*
* $Id: cdhold.inc,v 1.3 2003/03/05 17:36:34 dpp Exp $
*
* $Log: cdhold.inc,v $
* Revision 1.3  2003/03/05 17:36:34  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.2  2001/11/19 23:34:38  dpp
*      ->clean up
*
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.6  1996/06/24 21:27:10  dpp
* add DVHTCX
*
* Revision 1.5  1996/04/26 15:02:08  zfiles
* Bring up to date with cdtrackcd.inc
*
* Revision 1.4  1996/04/23 19:18:17  dpp
* add more variables to store for multibunch comparison
*
* Revision 1.3  1996/01/25 16:29:59  zfiles
* 4 status bit words
*
* Revision 1.2  1995/11/27 19:57:19  nk
* Added Id and Log
*
*
*-- Author :
* Temporary hold for CDTRAKCD.INC
*
* cdhold.inc
*

C note: any subroutine including this file must pre-include cdscrtcd.inc
C cdscrtcd.inc is needed only to define parameter EvntHitMax


      INTEGER         NTRKIX,NTRKCX,LTRKCX,
     + IPNTCX        ,INXTCX        ,
     + KINCX         ,
     + NHTLVX        ,NHTAIX        ,NHTADX        ,
     + NHTSCX        ,NHTSDX        ,
     + IDRCX         ,STATCX,KBQDCX ,
     + NDOFCX        ,ISVTCX,
     + IPLRCX               ,
     + IHWRCX,        NFITCX, IADFCX,
     + DVHTCX

      COMMON /CDHLD1/ NTRKIX(5),NTRKCX(5),LTRKCX(5),
     + IPNTCX(MTRKCD,5),INXTCX(MTRKCD,5),
     + KINCX (MTRKCD,5),
     + NHTLVX(MTRKCD,5),NHTAIX(MTRKCD,5),NHTADX(MTRKCD,5),
     + NHTSCX(MTRKCD,5),NHTSDX(MTRKCD,5),
     + IDRCX (MTRKCD,5),STATCX(4,MTRKCD,5),KBQDCX(MTRKCD,5),
     + NDOFCX(MTRKCD,5),ISVTCX(MTRKCD,5),
     + IPLRCX(KLR2TF,MTRKCD,5),
     + IHWRCX(KLR2TF,MTRKCD,5),NFITCX(MTRKCD,5),
     + IADFCX(KLR2TF,MTRKCD,5),
     + DVHTCX(EvntHitMax,5)

      REAL
     + CURCX         ,PHI0CX        ,D0CX          ,
     + PHI6CX        ,PHI7CX        ,RKN7CX        ,
     + TANDCX        ,Z0CX          ,XYZ0CX        ,
     + CHI2CX        ,CHIZCX        ,CHI3CX        ,
     + RESICX        ,CHI3PX        ,
     + CHI3VX        ,CHI3DX        ,CHI3TX        ,
     + RESMCX        ,NN2CCX        ,ARC1CX        ,
     + ARC2CX        ,X0LHCX        ,Y0LHCX        ,
     + PULLCX        ,ERRMCX        ,SIG1CX        ,
     + SIG2CX        ,MEASCX        ,X0INCX        ,
     + Y0INCX

      COMMON /CDHLD2/
     + CURCX (MTRKCD,5),PHI0CX(MTRKCD,5),D0CX  (MTRKCD,5),
     + PHI6CX(MTRKCD,5),PHI7CX(MTRKCD,5),RKN7CX(MTRKCD,5),
     + TANDCX(MTRKCD,5),Z0CX  (MTRKCD,5),XYZ0CX(3,MTRKCD,5),
     + CHI2CX(MTRKCD,5),CHIZCX(MTRKCD,5),CHI3CX(MTRKCD,5),
     + RESICX(MTRKCD,5),CHI3PX(MTRKCD,5),
     + CHI3VX(MTRKCD,5),CHI3DX(MTRKCD,5),CHI3TX(MTRKCD,5),
     + RESMCX(MTRKCD,5),NN2CCX(MTRKCD,5),ARC1CX(MTRKCD,5),
     + ARC2CX(MTRKCD,5),X0LHCX(MTRKCD,5),Y0LHCX(MTRKCD,5),
     + X0INCX(MTRKCD,5),Y0INCX(MTRKCD,5),
     + PULLCX(KLR2TF,MTRKCD,5),ERRMCX(NERMTF,MTRKCD,5),
     + SIG1CX(KLR2TF,MTRKCD,5),SIG2CX(KLR2TF,MTRKCD,5),
     + MEASCX(KLR2TF,MTRKCD,5)




