*
* $Id: cfxyht.F,v 1.9 2003/03/05 17:46:30 dpp Exp $
*
* $Log: cfxyht.F,v $
* Revision 1.9  2003/03/05 17:46:30  dpp
*      -> for TPC, use PWCD which has the cluster phi
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.8  2002/09/05 19:25:17  dpp
*     -> drift chamber / TPC dependent code
*
* Revision 1.7  2001/11/19 23:48:01  dpp
*      -> add new control parameter include file
*
* Revision 1.6  2000/11/15 16:52:38  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.5  2000/10/02 20:49:20  dpp
*      -> return error for large IDRFT drift distance so hit can
*         be given zero status and then not be added to the fit
*
* Revision 1.4  2000/08/18 18:54:51  dpp
*      -> new arguments for KABG2
*      -> uses new possible assignments of KNK6CF
*
* Revision 1.3  2000/05/16 19:04:24  dpp
* compatible with merged SV hit list
*
* Revision 1.2  2000/04/26 15:05:53  dpp
*      -> access svscratch.inc data for silicon x-y postion
*      -> ROADHITS no longer needs call to XYSVX1 for x-y
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE CFXYHT( IADR, ILYR, YSAG, XH, YH, IERROR )
C.......................................................................
C.
C. CFXYHT - Calculate X,Y values for axial wire hits
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 24-Jan-96
C.
C.
C. Modification Log.
C. 1996/01/24 call KABG2 and incorporate kink
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C=======================================================================
C Calculate the (X,Y) values of drift chamber hit for the axial wires
C
C Arguments:
C    IADR       Signed address of wire in /CDSCRT/
C    ILYR       Layer number
C    YSAG       Local wire sag
C    XH         X of hit
C    YH         Y of hit
C    IERROR     Error code
C=======================================================================

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/svscratch.inc"

      REAL TWOKAP,XW,YW,YSAG,DD,DELTAW,DENOM
      REAL XH,YH
      INTEGER ABS_ADR,IAMB,IADR,IERROR,ILYR
      REAL KA,AL,BE,GA,XI

C=======================================================================
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------


      IF(
     1     (KNK6CF.NE.0).AND.(KNK6CF.NE.KABG2_CLEAR).AND.
     2     (RTF(ILYR).GT.RKN6CF))THEN
        
        CALL KABG2(KABG2_CALC,
     1       0,0,0,KABG2_XI_OK,
     2       KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF, PHI6CF,
     3       KA,     AL,     BE,     GA,     XI)
        
      ELSE
        KA=KAPCF
        AL=ALPCF
        BE=BETCF
        GA=GAMCF
        XI=XICF
      ENDIF
      
      TWOKAP = 2.0*KA
      
      ABS_ADR= IADR
      IAMB   = 1
      IF(ABS_ADR.LT.0)THEN
        ABS_ADR=-ABS_ADR
        IAMB=-1
      ENDIF
      
      
C silicon in svscratch.inc
      
      IF(IDVCTF(ILYR).EQ.ISVX1)THEN
        XH=SV_X(ABS_ADR)
        YH=SV_Y(ABS_ADR)
        IERROR = 0
        
      ELSEIF(IDVCTF(ILYR).EQ.ISVZ1)THEN
        PRINT 9901,ILYR,IADR
 9901   FORMAT(' CFXYHT:',
     2       ' should not be called for layer ',I4,
     3       '  hit address=',I5)
        IERROR = 1
        
        
C wires in cdscrtch.inc
        
      ELSE

        IF(DRIFTDIR(ILYR).EQ.DRIFTDIR_PHI)THEN
          XW     = EvntHit_X(ABS_ADR)
          YW     = EvntHit_Y(ABS_ADR) + YSAG
          DD     = EvntHit_Drift(ABS_ADR)*IAMB
          DELTAW = KA*(XW**2 + YW**2) + AL*XW + BE*YW + GA
          
          DENOM  = 1.0 + TWOKAP*DELTAW
          
          XH = XW - DD*(AL + TWOKAP*XW)/DENOM
          YH = YW - DD*(BE + TWOKAP*YW)/DENOM
          
        ELSEIF(DRIFTDIR(ILYR).EQ.DRIFTDIR_Z)THEN
          XH=COS(EvntHit_corPhi(ABS_ADR))*RTF(ILYR)
          YH=SIN(EvntHit_corPhi(ABS_ADR))*RTF(ILYR)
c          XH=EvntHit_X(ABS_ADR)
c          YH=EvntHit_Y(ABS_ADR)
          
        ENDIF
        
        IF(
     1       (IDVCTF(ILYR).EQ.IDRFT).AND.
     2       (EvntHit_Drift(ABS_ADR).GT.(RTF(ILYR)*CELLTF(ILYR)))
     3       )THEN
          IERROR=2
        ELSE
          IERROR=0
        ENDIF
        
      ENDIF
      
      RETURN
      END
      



