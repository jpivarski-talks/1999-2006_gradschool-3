*
* $Id: findhw.F,v 1.9 2003/03/05 17:59:54 dpp Exp $
*
* $Log: findhw.F,v $
* Revision 1.9  2003/03/05 17:59:54  dpp
*      -> use device, rather than LCLSTR_LAYER, to specify cathodes
*      -> changed all variable names in cdscrtcd to have common root
*      -> loop over hits on multi-hit layer, put all in list
*
* Revision 1.8  2002/10/25 13:35:36  dpp
* Restricted the widening of the search road
* (road is widened for large entrance angle)
* to .not.cathodes. When the road was widened
* for cathodes, multiple panels were allowed.
*
* Revision 1.7  2000/11/15 16:57:51  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.6  2000/10/02 20:44:27  dpp
*      -> diagnostics
*      -> broaden search road with large entrance angle
*
* Revision 1.5  2000/08/18 18:49:26  dpp
*      -> change compilation switch to ROADHITSDIAG to limit outout
*
* Revision 1.4  2000/04/26 14:58:02  dpp
*      -> print statement
*
* Revision 1.3  1999/11/03 22:17:06  dpp
* remove ad-hoc block of layer 56
*
* Revision 1.2  1999/06/08 21:20:03  lyon
* Turn off projecting into cathodes until have geometry
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE FINDHW(LYRTF,PHI_IN,Z_IN,KAPRAD,ADR_RET,NHWMAX,NHW )
C.......................................................................
C.
C. FINDHW - Search for hit wires near PHI_IN, Z_IN
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : ROADHITS
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 01-Apr-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
*--
* LYRTF     = Layer number
* PHI_IN    = Expected PHI value
* Z_IN      = Expected ZEE value
* RAPRAD    = Kappa * Radius used to broaden search road
*

* ADR_RET   = Addresses of hit wires found
* NHWMAX = Maximum # of hit wires calling program will accept
* NHW    = # of hit wires found
*--
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"

#include "CathodeFortranAccess/cathclus.inc"
#include "CathodeFortranAccess/clustopad.inc"
#include "CathodeFortranAccess/cathpads.inc"
#include "CathodeFortranAccess/padtoclusll.inc"


      INTEGER ADR_RET(*)
      INTEGER LYRTF,NHWMAX,NHW,NSERF,IWIRF0,NWIRF,NWIRZ
      INTEGER ISERF,IWIRF,IWIRZ
      INTEGER CellID,HitAdr
      REAL PHI_IN,Z_IN,KAPRAD,AKAPRAD,Z2PI
      LOGICAL GOOD_HIT
      INTEGER IGO

c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


      NSERF  = SEARCH_LAYER(LYRTF,SAV_HIT_TYP,SAV_FIT_TYP)

c NINT is nearest integer

      IWIRF0 = NINT(Z2PI(PHI_IN-PHIFTF(LYRTF))/CELLTF(LYRTF))
      NWIRF  = NWIRTF(LYRTF)/NWRZTF(LYRTF)
      NWIRZ  = NWRZTF(LYRTF)

c expand the seach width for large entrance angle , not for cathodes

      IF( 
     1     (IDVCTF(LYRTF).EQ.IDRFT).OR.
     2     (IDVCTF(LYRTF).EQ.ITPC1))THEN
         AKAPRAD=ABS(KAPRAD)
        IF(AKAPRAD.GT. 0.90)THEN
          NSERF=NSERF+2
        ELSEIF(AKAPRAD.GT. 0.71)THEN
          NSERF=NSERF+1
        ENDIF
      ENDIF
      
#if defined(CLEO_ROADHITSDIAG)
      print 8883,LYRTF,kaprad,iwirf0,nserf,indxtf(LYRTF)
 8883 format(' findhw: LYRTF=',I3,' sine ent ang=',F6.2,
     2     ' search about wire',I5,' +/-',I3,
     3     ' INDXTF=',I6)
#endif

      
      NHW = 0

C R-Phi loop

      DO 49 ISERF=IWIRF0-NSERF,IWIRF0+NSERF
        IWIRF=ISERF
        IF(IWIRF.LT.0    )IWIRF=IWIRF+NWIRF
        IF(IWIRF.GE.NWIRF)IWIRF=IWIRF-NWIRF

C Z loop
        
        DO 39 IWIRZ=0,NWIRZ-1
          CellID=INDXTF(LYRTF)+IWIRF*NWIRZ+IWIRZ
          HitAdr=EvntHit_MapDet(CellID)

C multi-hit loop          
          
 21       CONTINUE
          IF(HitAdr.GT.0) THEN
            
            IF(IDVCTF(LYRTF).EQ.IDRCT)THEN
              GOOD_HIT=.TRUE.
            ELSE
              GOOD_HIT=(EvntHit_OpnHit(HitAdr).GE.0)
            ENDIF
            IF(GOOD_HIT)THEN
              
              
              NHW=MIN0(NHW+1,NHWMAX)
              ADR_RET(NHW)=HitAdr
              
#if defined(CLEO_ROADHITSDIAG)
              IF(IDVCTF(LYRTF).EQ.IDRCT)THEN
                print 8885,LYRTF,IWIRF,IWIRZ,
     2               INDXTF(LYRTF),CellID,HitAdr,
     3               CLUSPADPTR(ABS(HitAdr)),
     4               PADID(PADPTR(
     4               CLUSPADPTR(ABS(HitAdr))+1))
                
 8885           format(' findhw:  ','LYRTF=',I3,
     2               ' iwirf=',I3,' iwirz=',I4,
     3               ' indxtf=',I6,
     4               '  _MapDet(',I6,')=',I4,
     5               ' CLUSPADPTR=',I5,' PADID=',I5) 
              ENDIF
#endif
              
              IF(MultiHitLayer(LYRTF))THEN
                HitAdr=EvntHit_NxtMlt(HitAdr)
                GO TO 21
              ENDIF
            ENDIF
            
c23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
          ENDIF
 39     CONTINUE
        
 49   CONTINUE
      
      RETURN
      END
      


