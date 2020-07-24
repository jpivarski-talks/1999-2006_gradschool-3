*
* $Id: lfxyht.F,v 1.12 2003/03/05 17:46:13 dpp Exp $
*
* $Log: lfxyht.F,v $
* Revision 1.12  2003/03/05 17:46:13  dpp
*      -> use device, rather than LCLSTR_LAYER, to specify cathodes
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.11  2002/09/24 00:35:03  dpp
*      -> replaced call to lfctht which was accidentally deleted sept 5, 02
*
* Revision 1.10  2002/09/05 19:25:41  dpp
*  -> drift chamber / TPC dependent code
*
* Revision 1.9  2001/11/19 23:48:17  dpp
*      -> add new control parameter include file
*      -> delete unused include file
*
* Revision 1.8  2000/11/15 16:53:03  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.7  2000/10/02 20:49:37  dpp
*      -> return error for large IDRFT drift distance so hit can
*         be given zero status and then not be added to the fit
*
* Revision 1.6  2000/08/18 18:55:07  dpp
*      -> new arguments for KABG2
*      -> uses new possible assignments of KNK6CF
*
* Revision 1.5  2000/05/16 19:04:25  dpp
* compatible with merged SV hit list
*
* Revision 1.4  2000/04/26 15:05:29  dpp
*      -> access svscratch.inc data for silicon s-z postion
*      -> ROADHITS no longer needs call to XYSVX1 for s-z
*
* Revision 1.3  1999/11/09 21:45:17  dpp
* fix the crash in LFCTHT by over-writing errors in the cathode geometry
*
* Revision 1.2  1999/11/03 22:50:07  dpp
* test for cathode layer using flag rather than layer number
* F77 updates
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE LFXYHT( IADR, ILYR, YSAG, SH, ZH, IER )
C.......................................................................
C.
C. LFXYHT - Calculate S,Z values for the LF hits
C.
C. COMMON    : ?
C. CALLS     : ARCFUN LFCTHT LFQDIV
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ??-Apr-82
C. LAST MOD  : 23-Jul-87
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C=======================================================================
C Calculate the (S,Z) values for LF hits
C
C Arguments:
C    IADR      Signed address of wire in /CDSCRT/
C    ILYR      Layer number
C    YSAG      Local wire sag
C    SH        S of hit on track
C    ZH        Z of hit on track
C    IER       IER = 0 => Found possible S-Z
C              IER > 0 => Impossible S-Z
C=======================================================================
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/svscratch.inc"

C== Argument declarations

      INTEGER IADR, ILYR, IER
      REAL    YSAG, SH, ZH

C== Local variables

      INTEGER IDEV,ABS_ADR,IAMB
      REAL RR,RRSQ,TANTAU,TWOKAP,XW,YW,DD,CPHIS,SPHIS
      REAL THETAW,DELTAW,B,C,DISC,DENOM,XU,YU,XH,YH,ARCFUN
      REAL KA,AL,BE,GA,XI

      INTEGER ITMP1,ITMP2

* ----------Executable code starts here---------------------

C Branch on device/type of hit


      IDEV = IDVCTF(ILYR)
      
      IF(
     1     (IDEV.EQ.IVDCT).OR.
     2     (IDEV.EQ.IDRCT))THEN
        CALL LFCTHT( IADR, ILYR, YSAG, SH, ZH, IER )
      
  
      ELSEIF(LQDVTF(ILYR))THEN
        CALL LFQDIV( IADR, ILYR, YSAG, SH, ZH, IER )
        
      ELSE
        RR     = RTF   (ILYR)
        RRSQ   = RSQTF (ILYR)
        
        IF(
     1       (KNK6CF.NE.0).AND.(KNK6CF.NE.KABG2_CLEAR).AND.
     2       (RTF(ILYR).GT.RKN6CF))THEN
          
          CALL KABG2(KABG2_CALC,
     1         0,0,0,KABG2_XI_RECALC,
     2         KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF, PHI6CF,
     3         KA,     AL,     BE,     GA,     XI)
          
        ELSE
          KA=KAPCF
          AL=ALPCF
          BE=BETCF
          GA=GAMCF
          XI=XICF
        ENDIF
        
        TANTAU = TANSTF(ILYR)
        TWOKAP = 2.0*KA
        
        ABS_ADR= IADR
        IAMB   = 1
        IF(ABS_ADR.LT.0)THEN
          ABS_ADR=-ABS_ADR
          IAMB=-1
        ENDIF
        
        IF(IDVCTF(ILYR).EQ.ISVX1)THEN
          PRINT 9901,ILYR
 9901     FORMAT(' LFXYHT:',
     2         ' should not be called for layer ',I4,
     3         '  hit address=',I5)
          IER=5
          
        ELSEIF(IDVCTF(ILYR).EQ.ISVZ1) THEN
          ZH=SV_Z(ABS_ADR)
          SH=ARCFUN(KA,AL,BE,SV_X(ABS_ADR),SV_Y(ABS_ADR),XI )
          IER=0
          
C wires in cdscrtch.inc
        ELSE
          IER=0
          
          IF(
     1         (IDVCTF(ILYR).EQ.IDRFT).AND.
     2         (EvntHit_Drift(ABS_ADR).GT.(RTF(ILYR)*CELLTF(ILYR)))
     3         )THEN
            IER=1
          ENDIF
          
          IF(DRIFTDIR(ILYR).EQ.DRIFTDIR_PHI)THEN
            IF(IER.EQ.0)THEN
              XW     = EvntHit_X(ABS_ADR)
              YW     = EvntHit_Y(ABS_ADR)
              DD     = EvntHit_Drift(ABS_ADR)*IAMB
              CPHIS  = XW/RR
              SPHIS  = YW/RR
              THETAW =           AL*SPHIS - BE*CPHIS
              DELTAW = KA*RRSQ + AL*XW    + BE*YW    + GA
              
              B = THETAW - TWOKAP*YSAG*CPHIS
              C = DELTAW - DD - KA*DD*DD + YSAG*(BE + KA*(2.0*YW+YSAG))
              
              DISC = B**2 - 4.0*KA*C
              IF( DISC.LT.0.0)IER=2
            ENDIF
            
            IF(IER.EQ.0)THEN
              ZH   = - 2.0*C / (ABS(B) + SQRT(DISC)) / TANTAU
              IF( ABS(ZH).GT.YHTCUT )IER=3
            ENDIF
            
            IF(IER.EQ.0)THEN
              IF( B.LT.0.0 ) ZH = - ZH
              DENOM = 1.0 + TWOKAP*DD
              XU = XW + ZH*TANTAU*SPHIS
              YU = YW - ZH*TANTAU*CPHIS + YSAG
              XH = XU - DD*(AL + TWOKAP*XU)/DENOM
              YH = YU - DD*(BE + TWOKAP*YU)/DENOM
              SH     = ARCFUN( KA, AL, BE, XH, YH, XI )
            ENDIF
            
          ELSEIF(DRIFTDIR(ILYR).EQ.DRIFTDIR_Z)THEN
            IF(IER.EQ.0)THEN
              XH     = EvntHit_X(ABS_ADR)
              YH     = EvntHit_Y(ABS_ADR)
              ZH     = EvntHit_Z(ABS_ADR)
              SH     = ARCFUN( KA, AL, BE, XH, YH, XI )            
            ENDIF

C end of device dependence
          ENDIF
C end of wires in cdscrtch.inc
        ENDIF
      ENDIF
      RETURN
      END







