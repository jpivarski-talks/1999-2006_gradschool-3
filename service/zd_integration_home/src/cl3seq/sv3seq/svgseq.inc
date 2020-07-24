*
* $Id: svgseq.inc,v 1.3 1998/03/27 17:37:33 pg Exp $
*
* $Log: svgseq.inc,v $
* Revision 1.3  1998/03/27 17:37:33  pg
*    Forgot to commit svgseq.inc which caused SVGM3 compilation to bomb.
*
* Revision 1.2  1996/09/30 05:48:10  jjo
* Modify svgeompa.inc to use longer variable names per CLEO3 standards
*        SVNSTR -> NSTRIP_SV3, SVLYRS -> NLAYER_SV3, SVMWFR -> NWAFER_SV3
*        NSVWP? -> NWAF_PHI?_SV3, NSVWZ? -> NWAF_Z?_SV3
*        SV_CHAN_ACTIVE  -> NCHAN_ACTIVE_SV3, SV_CHAN_TOTAL  -> NCHAN_TOTAL_SV3
*
* Revision 1.1.1.1  1995/11/01 21:18:36  lkg
* Import renamed CLEO3 include repository: cl3seq
*
* Revision 1.2  1994/10/12  00:14:55  zfiles
* Changed the name of KSVPAR common block due to collision with 94a kuip
*
* Revision 1.1.1.1  1994/10/04  20:59:04  zfiles
* first version of seq in CVS
*
*
*
* svgseq.inc
*
*----------------------------------------------------------------------
*
*  surveying constants for SVD
*
*
*  SVDOFF - x,y,z and 3 rotation angles for entire SVD
*  SVWFOF - same for each wafer
*
*  SVSET - detector set name. (filled in SVGINI)
*  SVLDET - sv layer detector name (filled in SVGINI)
*  SVDET - detector name. (filled in SVGINI)
*  SPDET - support structure name. (filled in SVGINI)
*  RCDET - receiver chip name. (filled in SVGINI)
      CHARACTER*4     SVSET, SVDET, SPDET, RCDET, SVLDET

      COMMON /SVDNAM/ SVSET, SVDET, SPDET(4), RCDET, SVLDET(4)
      SAVE /SVDNAM/
*
*  ISVST - SVD set number.
*  ISVDT - detector number.
*
      INTEGER         ISVST, ISVDT

      COMMON /ISVNUM/ ISVST, ISVDT
      SAVE /ISVNUM/
*
*  NSVWFP - Number of wafers in r-phi for each layer
*  NSVWFZ - Number of wafers in z for each layer
*
      INTEGER NSVWFP, NSVWFZ
      COMMON / SVSTRC / NSVWFP(NLAYER_SV3), NSVWFZ(NLAYER_SV3)
*
*
*  SVTHCK - Thickness of the silicon wafers
*  SVWFWD - Width of silicon wafers
*  SVWFLN - Length of silicon wafers
*  SVWDOF - Width of dead region along width
*  SVLNOF - Width of dead region along length
*  SVPITX - Implant pitch on N-side (r-phi)
*  SVPITZ - Implant pitch on P-side (z)
*  SVEDGE - fraction of active area at the edges
*
      DOUBLE PRECISION            SVTHCK
      DOUBLE PRECISION            SVWFWD,         SVWFLN
      DOUBLE PRECISION            SVWDOF,         SVLNOF
      DOUBLE PRECISION            SVPITX,         SVPITZ
      DOUBLE PRECISION            SVEDGE

      COMMON /RSVPAR/ SVTHCK,
     &                SVWFWD,    SVWFLN,
     &                SVWDOF(2), SVLNOF(2),
     &                SVPITX,   SVPITZ,
     &                SVEDGE(2,0:1)
      SAVE /RSVPAR/

* SVWFTL - tilt in wafer needed to get proper pinwheel effect
* SVWFXO - local x offset (ie, along width direction) to get pinwheel
* SVWFRA - radius to move the strip after tilt + shift
* SVWFCP - phi of each center of wafer
* SVWFCZ - z position of center of wafer
* SVZROT - to transform local <--> world, need a rotation about local
*          y = 0 axis
* SVCRDR - distance from nominal layer radius to actual wafers in 
*          "crennalated" (sp?) layers
* SVLPIN - .TRUE. if layer is pinwheel, .FALSE. if not

      DOUBLE PRECISION SVWFTL(4), SVWFXO(4), SVWFRA(4), SVCRDR(4)
      DOUBLE PRECISION SVWFCP(SVMXWP,NLAYER_SV3),
     &                 SVWFCZ(SVMXWZ,NLAYER_SV3)
      LOGICAL SVYROT(SVMXWZ,NLAYER_SV3),SVZROT(NLAYER_SV3),SVLPIN(4)
      COMMON / SVPINW / SVWFTL, SVWFXO, SVWFRA, SVWFCP, SVWFCZ,
     $                  SVCRDR,
     $                  SVYROT,SVZROT,SVLPIN
      SAVE /SVPINW/

* SPTHCK - support structure thickness
* SPLENG - support structure length for each layer
* SPWIDT - support structure width
* RCTHCK - receiver chip thickness
* RCLENG - receiver chip length
* RCWIDT - receiver chip width
      DOUBLE PRECISION SPTHCK, SPLENG, SPWIDT
      DOUBLE PRECISION RCTHCK, RCLENG, RCWIDT
      COMMON / SVSPPR / SPTHCK, SPLENG(4), SPWIDT,
     $                  RCTHCK, RCLENG,    RCWIDT

      SAVE /SVSPPR/


