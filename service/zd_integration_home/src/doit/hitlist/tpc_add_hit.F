
*
* $Id: tpc_add_hit.F,v 1.1 2003/03/05 17:51:08 dpp Exp $
*
* $Log: tpc_add_hit.F,v $
* Revision 1.1  2003/03/05 17:51:08  dpp
*      -> NEW ROUTINE
*      -> add one hit to the hit list, called from hit sources
*      -> use EvntHit_OpnHit to indicate usable hits
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TPC_ADD_HIT(ILCD,IADR,Z_HIT,Z_WIDTH,PH_HIT)
C.......................................................................
C.
C. TPC_ADD_HIT - add one hit to the hit list
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 16-Dec-2002
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE


#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfindpar.inc"
C which includes 
C               #include "cl3seq/cdgm3/cdgeompa.inc"
C               #include "doit/duseq/tfgeompa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"
#include "doit/duseq/runev.inc"

C ARGUMENTS
      INTEGER ILCD
      INTEGER IADR
      REAL    Z_HIT
      REAL    Z_WIDTH
      REAL    PH_HIT

C LOCAL
      LOGICAL ALLOW_HIT
      INTEGER ILTF
      INTEGER LastHitMlt
      INTEGER nMlt
      REAL    PHINOW
      REAL    VEL





c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


      ALLOW_HIT=
     1     (IADR.GE.(INDXCD(ILCD))).AND.
     2     (IADR.LT.(INDXCD(ILCD)+NWIRCD(ILCD)))
      
      IF(ALLOW_HIT)THEN
        
        IF(EvntHit_MapDet(IADR).EQ.0)THEN
          ALLOW_HIT=.TRUE.
        ELSE
          ILTF=ILCDTF(ILCD)
          ALLOW_HIT=MultiHitLayer(ILTF)
        ENDIF
        
      ENDIF
      
      IF(ALLOW_HIT)THEN  
        ALLOW_HIT=(EvntHit_Num.LT.EvntHitMax)
      ENDIF
      
      IF(ALLOW_HIT)THEN  
          
        EvntHit_Num=EvntHit_Num+1
        EvntHit_OpnHit(EvntHit_Num)=7
        EvntHit_CDlayr(EvntHit_Num)=ILCD
        EvntHit_laWire(EvntHit_Num)=IADR-INDXCD(ILCD)
        
                
        IF(EvntHit_MapDet(IADR).EQ.0)THEN
          
          EvntHit_MapDet(IADR)=EvntHit_Num
          
C insert into the front on the layer link list 
C  as in  Doit/Fortran/doitloaddrhits.F
          
          EvntHit_NxtLyr(EvntHit_Num)=
     1         EvntHit_1stLyr(ilcd)
          EvntHit_1stLyr(ilcd)=EvntHit_Num
          
          EvntHit_nMult(EvntHit_Num)=1
        ELSE
          
C many arrays are locked into having EvntHit_MapDet unchanged 
C so it is dificult to insert into the front on the link list as 
C in  Doit/Fortran/doitloaddrhits.F
C bounce up to the last hit in the multi-hit link-list
          
          LastHitMlt=EvntHit_MapDet(IADR)
          nMLT=EvntHit_nMult(LastHitMlt)+1
 151      EvntHit_nMult(LastHitMlt)=nMLT
          IF(EvntHit_NxtMlt(LastHitMlt).NE.0)THEN
            LastHitMlt=EvntHit_NxtMlt(LastHitMlt)
            GO TO 151
          ENDIF
          EvntHit_NxtMlt(LastHitMlt)=EvntHit_Num
          EvntHit_nMult(EvntHit_Num)=nMLT
        ENDIF
        
        EvntHit_1stMlt(EvntHit_Num)=EvntHit_MapDet(IADR)
        EvntHit_NxtMlt(EvntHit_Num)=0
        EvntHit_PrimMlt(EvntHit_Num)=EvntHit_Num
        EvntHit_TimEarly(EvntHit_Num)=0
        EvntHit_TimLate(EvntHit_Num)=0
        
        
        PHINOW=PHIFCD(ILCD)
     2       +CELLCD(ILCD)*EvntHit_laWire(EvntHit_Num)
        IF(PHINOW.LT.0.)   PHINOW=PHINOW+2.*PI
        IF(PHINOW.GE.2.*PI)PHINOW=PHINOW-2.*PI
          
        EvntHit_X(EvntHit_Num)=COS(PHINOW)*RCD(ILCD)
        EvntHit_Y(EvntHit_Num)=SIN(PHINOW)*RCD(ILCD)
        EvntHit_Z(EvntHit_Num)=Z_HIT
        
        EvntHit_PhiWir(EvntHit_Num)=PHINOW
        
        EvntHit_Drift(EvntHit_Num)=0.
        
        EvntHit_NxtCls(EvntHit_Num)=0
        EvntHit_PrimCls(EvntHit_Num)=0
        
        EvntHit_NHitDev(EvntHit_Num)=EvntHit_Num
        
        
        
C velocity in meters/ps            
        VEL=50. *1.E-9
C time of the hit in ps
        EvntHit_rawTIM(EvntHit_Num)=Z_HIT/VEL
C TDC
        EvntHit_rawTDC(EvntHit_Num)=Z_HIT/3.0*4096
C now using this variable for the time duration of the hit, in ps,
C specifically, the duration due to z propogation of the track
        EvntHit_corTIM(EvntHit_Num)= ABS(Z_WIDTH)/VEL
          
C EvntHit_Charge is "uncorrected" charge and available in sf_data_access
C EvntHit_rawADC is the raw adc and is available in sf_data_access
C PWCD used be the "corrected charge" and is not available
C do not fill PWCD; this location will be used by EvntHit_corPhi
        
        EvntHit_Charge(EvntHit_Num)=PH_HIT
        EvntHit_rawADC(EvntHit_Num)=.5*PH_HIT
        
        
        
        
          
c                    print 1102,
c     1                   IADR,
c     2                   EvntHit_MapDet(IADR),
c     3                   EvntHit_CDlayr(EvntHit_Num),
c     4                   EvntHit_laWire(EvntHit_Num),
c     5                   EvntHit_Num,
c     6                   EvntHit_1stMlt(EvntHit_Num),
c     7                   EvntHit_NxtMlt(EvntHit_1stMlt(EvntHit_Num)),
c     8                   EvntHit_Charge(EvntHit_Num)
        
        
 1102   format(' TPC_HITLIST:',
     1       '  IADR=',I6,
     2       '  map=',I6,
     3       '  Layr=',I3,
     4       '  Wire=',I5,
     5       ' this hit num=',I6,
     6       ' 1st hit for cell=',I6,
     7       ' next hit for 1st hit=',I6,
     8       ' this PH=',F6.1)
        
        
      ENDIF
      RETURN
      END
      




