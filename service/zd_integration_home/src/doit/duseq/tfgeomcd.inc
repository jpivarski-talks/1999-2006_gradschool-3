*
* $Id: tfgeomcd.inc,v 1.6 2003/03/05 17:35:27 dpp Exp $
*
* $Log: tfgeomcd.inc,v $
* Revision 1.6  2003/03/05 17:35:27  dpp
*      -> add new clustering controls
*      -> add flag for multi-hit electronics
*      -> changed all variable names in cdscrtc4d to have common root
*
* Revision 1.5  2002/09/05 19:15:00  dpp
*      -> remove unused NSRVTF, search width using VD seed
*      -> remove NFETLF (link-tree)
*      -> change DEVNAM to CHAR*4 for compatibility with cdgeompa labels
*      -> remove unused sigmas, weights, search widths
*      -> add DRIFTDIR to distinguish between drift chamber and TPC
*
* Revision 1.4  2000/12/19 23:28:59  dpp
*      -> add flag for stereo layer
*      -> remove all references to LCSTDR,MCSTDR,MCTICD,LCTOCD
*
* Revision 1.3  2000/11/15 17:01:16  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.2  2000/08/18 18:34:34  dpp
*      -> add new DOIT scatters
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
*
*
*-- Author :
*
* tfgeomcd.inc
*
#if !defined(CLEO_NOCOMDOC)
*
* TFGEOMCD Documentation

* NLYRVD - Number of (IV or PT) + VD layers
* NLYRCF - Number of layers used in CFIND
* LLYRCF - First CF layer number
* MLYRCF - Last  CF layer number
* NLYRLF - Number of layers used in LFIND
* LLYRLF - First LF layer number
* MLYRLF - Last  LF layer number
*
* LLYRVD - First VD layer
* MLYRVD - Last  VD layer
*
* LCAXDR - First axial DR layer
* MCAXDR - Last  axial DR layer


* ILYR runs from 1 to NLYRTF for the NLYRTF layers
*
* IDVCTF(ILYR) - Name of the device for layer # ILYR
*                = IPROP for proportional wire chamber layers
*                = IDRFT for drift  chamber  layers
*                = IVDET for vertex detector layers
*                = IIVDT for vertex detector insert layers
*                = IVDCT for vertex detector cathode strip layers
*                = IDRCT for drift  chamber  cathode strip layers
*                = IPTLA for PT anode layers
*
* LUSETF(ILYR) - .TRUE.  if this layer is  available for TFORST
* LSAVTF(ILYR) - Save area for LUSETF.
* LPRFTF(ILYR) - Preferred layers for ROADHITS to make LUSETF.

* LU3DTF(ILYR) - .TRUE.  if this layer is  available for TF3FIT
* LQDVTF(ILYR) - .TRUE.  if this layer has charge division
* LUHTTF(ILYR) - .TRUE. this layer is available in ROADHITS.

* LUSE_LAYER(layer,use_level,fit_typ) this layer is available in ROADHITS
*                                    for PAT-REC/TFXFIT , r-phi/sz

*
* RENDTF(ILYR) - Radius         of layer ILYR at Z = ZENDTF(ILYR)
* RTF   (ILYR) - Radius         of layer ILYR at Z = 0
* RSQTF (ILYR) - Radius-squared of layer ILYR at Z = 0
* DIFSTF(ILYR) - Number of cells between wire location at each end
* ANGSTF(ILYR) - Stereo angle of layer ILYR
* TANSTF(ILYR) - Tangent of the stereo angle of layer ILYR

* note: these are used by CDCANH and CDFILL but do not seamed to be assigned
* SIGFTF(ILYR) - Sigma used in the CFIND & LFIND fits
* WGTFTF(ILYR) - Weight ( = 1/SIGFTF**2)
* NSRCTF(ILYR) - Number of (R-PHI) cells used to search for hits

* note: this is still used in SFHITS
* NSRWTF(ILYR) - Search width using wire position only

* SIGQTF(ILYR) - Sigma Z for charge division only
* WGTQTF(ILYR) - Weight ( = 1/SIGQTF**2)

* SIGMA_LAYER (layer,hit_typ,fit_typ) - Sigma, Weight and Search width
* WEIGHT_LAYER(layer,hit_typ,fit_typ) - of a layer for wires and hits
* SEARCH_LAYER(layer,hit_typ,fit_typ) - and for CF and LF use
* LCLSTR_LAYER(layer) - flag if clustering is to be done for this layer
* MultiHitLayer(layer) - flag if layer utilizes multi-hit structure in cdscrtcd
* LAMB_D_LAYER(layer) - flag if there is ambiguous drift distance in layer
* LSTEREOLAYER(layer) - flag if layer is treated as stereo
* DRIFTDIR(layer) - flag that drift direction is phi(drift chamber) or Z(TPC)
* CLS_TC_MAX(layer) - maximum time difference w/correlated anode in clustering
* CLS_Q_A_MIN(layer) - minimum charge for an element to be added to a cluster
* CLS_Q_P_MIN(layer) - minimum charge to be a peak of a cluster
* CLS_F_E_MIN(layer) - minimum fractional charge to enable a secondary pad
* CLS_F_C_MAX(layer) - maximum fractional charge to cut a cluster at a dip

* SAV_LVL_TYP - save level type in higher routine for use in called routines
* SAV_HIT_TYP - save hit type in higher routine for use in called routines
* SAV_FIT_TYP - save fit type in higher routine for use in called routines

* IXSFTF(ILYR) - Device layer # of TFIND layer # ILYR (x-reference)
* INDXTF(ILYR) - address of wire # 0 of TF layer # ILYR
* INDVTF(ILYR) - Relative address in device of wire 0, TF layer ILYR
* NWIRTF(ILYR) - # of wires in cylinder
* CELLTF(ILYR) - Width of cell in PHI (radians)
* PHIFTF(ILYR) - Actual  PHI of wire 0 at Z = 0 in cylinder
* NWRZTF(ILYR) - # of Z divisions (strips or charge division)
* ILCRTF(ILYR) - Correlation from layer ILYR (0 if none)
* CELZTF(ILYR) - Width of cell in Z (meters)
* ZEEFTF(ILYR) - Z of centre of strip # 0
* ZENDTF(ILYR) - Z at end of layer ILYR in CD
*
* XX0TF(I,ILYR) - (X,Y,Z) offset of layer ILYR
* UU0TF(I,ILYR) - (X,Y,Z) direction cosine of axis of layer ILYR
*
**** BSAGTF(ILYR) - Sag at centre of wire: Y=YW + BSAGTF*(1 - (Z/ZMAX)**2)
* WENDTF(ILYR) = Which end wire read out from: +1.0 => +ve Z
*
* LSCATF - First  effective discreet scattering surface
* MSCATF - Last   effective discreet scattering surface
* IKN6TF - Index of scatterer where kink is fitted
* IKN7TF - Index of scatterer where 2nd kink is fitted
* RSCATF - Radii      of the discreet scattering surfaces (beam pipe=1)
* XSCATF - Thickness  of the discreet scattering surfaces in rad. len.
* LSCCTF - First  continuous scattering medium
* MSCCTF - Last   continuous scattering medium
* RSCCTF - Radii of start of continuous scattering media
* XSCCTF - Inverse X0     of continuous scattering media

* new DOIT scatters
* RAD_SCAT     - radius of scatter
* R_LEN_SCAT   - radiation lengthS of scatter
* ACTIV_SCAT   - =F/=T if scatter is activated
*
* DEVNAM - Name of device in characters - for printing
* XXBMTF - Nominal beam position (X,Y,Z)
* IXLRTF - XREF from device and local layer number to ILYR
* ----------------------------------------------------------
#endif

C Parameters moved from PAR to CDE

      INTEGER NCANTF, NCATTF, NLYRTF, NLR2TF, NLYRVD, NLYRCF
      INTEGER LLYRCF, MLYRCF, NLYRLF, LLYRLF, MLYRLF, LLYRVD, MLYRVD
      INTEGER LCAXDR, MCAXDR

      COMMON/TFGEII/ NCANTF, NCATTF, NLYRTF, NLR2TF, NLYRVD, NLYRCF,
     +       LLYRCF, MLYRCF, NLYRLF, LLYRLF, MLYRLF, LLYRVD, MLYRVD,
     +       LCAXDR, MCAXDR


      INTEGER         LSCATF, MSCATF,
     +                LSCCTF, MSCCTF,
     +                IKN6TF, IKN7TF,
     +                IDVCTF        ,
     +                NSRCTF        , NSRWTF        ,
     +                SEARCH_LAYER  ,
     +                DRIFTDIR      ,
     +                SAV_LVL_TYP   ,SAV_HIT_TYP    ,SAV_FIT_TYP    ,   
     +                IXSFTF        ,INDXTF         , INDVTF        ,
     +                NWIRTF        ,NWRZTF         ,
     +                ILCRTF        ,
     +                IXLRTF        ,
     +                ILCDTF        ,ILTFCD

      COMMON /TFGEOI/ LSCATF, MSCATF,
     +                LSCCTF, MSCCTF,
     +                IKN6TF        , IKN7TF,
     +                IDVCTF(KLYRTF),
     +                NSRCTF(KLYRTF), NSRWTF(KLYRTF),
     +                SEARCH_LAYER(KLYRTF,NFITTYP,NHITTYP),
     +                DRIFTDIR(KLYRTF),
     +                SAV_LVL_TYP   ,SAV_HIT_TYP    ,SAV_FIT_TYP    ,   
     +                IXSFTF(KLYRTF), INDXTF(KLYRTF), INDVTF(KLYRTF),
     +                NWIRTF(KLYRTF), NWRZTF(KLYRTF),
     +                ILCRTF(KLYRTF),
     +                IXLRTF(KLYRTF,MXDVCD),
     +                ILCDTF(KLYRTF), ILTFCD(KLYRTF)

      LOGICAL         LUSETF,         LU3DTF,         LQDVTF,
     +                LSAVTF,         LPRFTF,         LUHTTF,
     +                LUSE_LAYER,     LCLSTR_LAYER,   MultiHitLayer,
     +                LAMB_D_LAYER,   LSTEREOLAYER,   ACTIV_SCAT


      COMMON /TFGEOL/ LUSETF(KLYRTF), LU3DTF(KLYRTF), LQDVTF(KLYRTF),
     +                LSAVTF(KLYRTF), LPRFTF(KLYRTF), LUHTTF(KLYRTF),
     +                LUSE_LAYER(KLYRTF,NLVLTYP,NHITTYP),
     +                LCLSTR_LAYER(KLYRTF),MultiHitLayer(KLYRTF),
     +                LAMB_D_LAYER(KLYRTF),LSTEREOLAYER(KLYRTF),
     +                ACTIV_SCAT(M_SCAT)

      REAL            RTF           , RSQTF         ,
     +                TANSTF        ,
     +                RENDTF        , DIFSTF        , ANGSTF        ,
     +                SIGFTF        , WGTFTF        ,
     +                SIGQTF        , WGTQTF        ,
     +                SIGMA_LAYER   , WEIGHT_LAYER  ,
     +                CLS_TC_MAX    ,
     +                CLS_Q_A_MIN   , CLS_Q_P_MIN   ,
     +                CLS_F_E_MIN   , CLS_F_C_MAX   ,
     +                PHIFTF        , CELLTF        ,
     +                ZEEFTF        , CELZTF        ,
     +                ZENDTF        ,
     +                RSCATF        , XSCATF        ,
     +                RSCCTF        , XSCCTF        ,
     +                XX0TF         , UU0TF         ,
     +                WENDTF        ,
     +                XXBMTF        ,
     +                RAD_SCAT      , R_LEN_SCAT

      COMMON /TFGEOR/ RTF   (KLYRTF), RSQTF (KLYRTF),
     +                TANSTF(KLYRTF),
     +                RENDTF(KLYRTF), DIFSTF(KLYRTF), ANGSTF(KLYRTF),
     +                SIGFTF(KLYRTF), WGTFTF(KLYRTF),
     +                SIGQTF(KLYRTF), WGTQTF(KLYRTF),
     +                SIGMA_LAYER (KLYRTF,NFITTYP,NHITTYP),
     +                WEIGHT_LAYER(KLYRTF,NFITTYP,NHITTYP),
     +                CLS_TC_MAX(KLYRTF),
     +                CLS_Q_A_MIN(KLYRTF),CLS_Q_P_MIN(KLYRTF),
     +                CLS_F_E_MIN(KLYRTF),CLS_F_C_MAX(KLYRTF),
     +                PHIFTF(KLYRTF), CELLTF(KLYRTF),
     +                ZEEFTF(KLYRTF), CELZTF(KLYRTF),
     +                ZENDTF(KLYRTF),
     +                RSCATF(NSCATF), XSCATF(NSCATF),
     +                RSCCTF(NSCCTF), XSCCTF(NSCCTF),
     +                XX0TF(3,KLYRTF), UU0TF(3,KLYRTF),
     +                WENDTF(KLYRTF),
     +                XXBMTF(3),
     +                RAD_SCAT(M_SCAT), R_LEN_SCAT(M_SCAT)

      CHARACTER*4     DEVNAM
      COMMON /TFGEOC/ DEVNAM(MXDVCD)
* ----------------------------------------------------------
