*
*CMZ :          12/01/94  21.44.39  by  Unknown
*CMZ :          09/10/91  22.57.46  by  Jon Urheim
* minor fix to facillitate compilations on ibm rs6000.
*CMZ :  1.01/73 27/11/90  11.30.13  by  Brian Geiser
*CMZ :  1.01/48 05/07/90  11.25.42  by  Brian Geiser
*CMZ :  1.01/41 16/05/90  16.32.16  by  Maurice Garcia-Sciveres
*-- Author :
C
C      INCLUDE 'CLINC:CDGEOM.CDE'
C
C.......................................................................
C.
C. CDGEOM - INCLUDE file for CLEO-II Central Detector Geometry COMMON
C.
C. COMMON    : CDCTLI CDGEOC CDGEOI CDGEOL CDGEOR
C. CALLS     :
C. CALLED    :
C. AUTHOR    : R. Namjoshi, based on M. Ogg's version
C.           : N. Katayama, for the CLEO-II.
C. VERSION   : 2.00
C. CREATED   : 02-Sep-86
C. LAST MOD  : 06-Jan-89
C.
C.
C. Modification Log.
C.
C. 16-Nov-86 : R. Namjoshi.     Add control flag variables and fix
C.                              some small errors.
C. 25-Oct-87 : R. Namjoshi      Make completely F77 compatible
C.
C. 28-Jan-88 : R. Namjoshi      Add array ERBMCD for beam pos. errors
C.
C. 06-Jan-89 : N. Katayama     Preliminary version for CLEO-II
C
CRYD   28-jun-93  Started to put in SVX        A.RYD
C
*  Selection flag CDGDOC is added to write out comments.
****** To get comments back SELECT CDGDOC in CMZ session.
C
C.......................................................................
#if defined(CLEO_CDGDOC)
C
C== CDCTLI - CD control  common
C
* LCNFCD - Flags if CD configuration is changed.
* LPOSCD - Flags if CD positions are changed.
*
#endif
      LOGICAL          LCNFCD, LPOSCD, LCPACD, LCVACD, LCDACD,
     1                 LCVCCD, LCDCCD
      COMMON /CDCTLL / LCNFCD, LPOSCD, LCPACD, LCVACD, LCDACD,
     1                 LCVCCD, LCDCCD
      SAVE   /CDCTLL/
#if defined(CLEO_CDGDOC)
*V2.00
C
C CDGEOP - CD Geometry PARAMETER COMMON
C
C Most of parameters are moved from CDGEOM.PAR into this common block
C so that we can configure at the execution time.
C
C The following conventions are used when naming arrays:
C    *DV*CD is used for arrays which are indexed by the current CD
C           device ordering (changed at run time)
C    **DVCD is used for arrays which are indexed by the device definition
C           ordering (must recompile)
C
C If a quantity has only one ordering, it is the device definition ordering.

* IDFRCD - Device configuration table
*          IDFRCD(1) - IDFRCD(MDEVCD) describe current CD geometry
*          configuration. If IDFRCD(1) is IPTLA, this means that the
*          first device in CD geometry is IPTLA, and so on.
*          This associates CD device with DEVICE definition.
*          This ordering of the devices is called the CD device ordering.
* IDTOCD - an inverse array of IDFRCD. It associates the device definition
*          parameters with CD device ordering. 0 if the device is not
*          used in the current configuration.
*          For example if IDTOCD(IPTLA) is 1, IPTLA is the first device in
*          the current CD device configuration.
*          Identity : IDFRCD(IDTOCD(IPTLA)) = IPTLA, it IPTLA is used.
*          Or       : IDTOCD(IDFRCD(I)) = I if I <= MDEVCD.
*
* NDVLCD - the number of layers of each CD device. The ordering is
*          the CD device ordering.
* NLDVCD - the number of layers of each CD device. The ordering is
*          the device definition ordering.
*          NLDVCD(IDRFT) is 51, which is equal to NCANDR (PARAMETER).
*
* IDVLCD - The first and last layer of each CD device. The ordering is
*          the CD device ordering.
*          This array is equivalenced to XCAYZZ, where
*          X is either L (first) or M (last), Y is either N (anode) or
*          T (cathode) and ZZ is one of the device names. See equivalence
*          statements after the COMMON statement.
* ILDVCD - First and last layers of the device. The ordering is the device
*          definition ordering.
*          order.
*
* NDVWCD - the number of wires (strips) of each CD device. The ordering is
*          the CD device ordering.
* NWDVCD - the number of wires (strips) of each CD device. The ordering is
*          the device definition ordering.
*          NWDVCD(IDRFT) is 12240, which is equal to MWIRDR (PARAMETER).
*
* LDVCCD - Flag for the charge division for each CD device. The ordering is
*          the CD device ordering.
* LCDVCD - Flag for the charge division for each CD device. The ordering is
*          the device definition ordering.
*
* IDVTCD - gives the device type (indexed by CD device order)
* ITDVCD - gives the device type (indexed by device definition order)
*
* ILDTCD - First and last layers of the device type.
*          This array is equivalenced to XCAYCD, where
*          X is either L (first) or M (last), Y is either N (anode) or
*          T (cathode). See equivalence statements after the COMMON statement.
* NWDTCD - the number of wires (strips) of each CD device type.
* NLDTCD - the number of layers of each CD device type.
*          This array is equivalenced to NCAYCD, where
*          Y is either N (anode) or T (cathode), or P (pixel). See equivalence
*          statements after the COMMON statement.
*
* IDBQCD - the offset in CDLA for the secondary channel of the charge division
*          device. The ordering is the device definition ordering.
*
* NAXLCD - Number of CD      axial   layers
* NSTRCD - Number of CD      stereo  layers
* NLYRCD - Number of CD              layers
* NQDVCD - Number of CD layers with charge division
* MDEVCD - Number of different CD devices
*          See KDEVCD and MXDVCD in CDGEOMPA for declarations
*
* MWIRCD = Total # of wires in CD
*     See KWIRCD in CDGEOMPA for declarations.
*          # VD       wires :   800*2
*          # VD      insert :   192 / or PTL 384
*          # VD    cathodes :  1280
*          # DR       wires : 12240
*          # DR    cathodes :  3072
*          # SVX     strips :                       13104
*                             -----       ------
*          # CD       wires : 18384        18576    31296
*
* ILBDCD - An offset to CDLA from DLA for given DVN.
#endif
      INTEGER IDFRCD, IDVLCD, NDVWCD, NDVLCD, IDTOCD, ILDVCD, NWDVCD
      INTEGER NLDVCD, NAXLCD, NSTRCD, NLYRCD, NQDVCD, IDBQCD
      INTEGER ILDTCD, NWDTCD, NLDTCD, MDEVCD, MWIRCD, ILBDCD
      INTEGER IDVTCD, ITDVCD
      LOGICAL LCDVCD, LDVCCD
#if defined(CLEO_CDGDOC)
* The first and last word in the common block
*
#endif
      INTEGER CDFIRP, CDLASP
      COMMON /CDGEOP / CDFIRP,
     1 IDFRCD(KDEVCD), IDVLCD(2, KDEVCD),
     1 NDVLCD(KDEVCD), NDVWCD(KDEVCD), LDVCCD(KDEVCD),
     1 IDTOCD(MXDVCD), ILDVCD(2, MXDVCD), NLDVCD(MXDVCD),
     1 NWDVCD(MXDVCD), LCDVCD(MXDVCD), IDBQCD(MXDVCD),
     1 ILDTCD(2, MXDTCD), NLDTCD(MXDTCD), NWDTCD(MXDTCD),
     1 NAXLCD, NSTRCD, MDEVCD, MWIRCD, NQDVCD, NLYRCD,
     1 ILBDCD(MXDVCD),
     1 IDVTCD(KDEVCD), ITDVCD(MXDVCD),
     1 CDLASP
      SAVE /CDGEOP/
#if defined(CLEO_CDGDOC)
C
C Equivalence for MCANIZ and so on.
*
* NCANCD - Number of CD anode layers
* LCANCD - The first layer of CD anodes
* MCANCD - The last layer of CD anodes
#endif
      INTEGER NCANCD, LCANCD, MCANCD
      EQUIVALENCE (LCANCD, ILDTCD(1, ANODCD))
      EQUIVALENCE (MCANCD, ILDTCD(2, ANODCD))
      EQUIVALENCE (NCANCD, NLDTCD(   ANODCD))
#if defined(CLEO_CDGDOC)
* NCATCD - Number of CD cathode layers
* LCATCD - The first layer of CD cathodes
* MCATCD - The last  layer of CD cathodes
#endif
      INTEGER NCATCD, LCATCD, MCATCD
      EQUIVALENCE (LCATCD, ILDTCD(1, CATHCD))
      EQUIVALENCE (MCATCD, ILDTCD(2, CATHCD))
      EQUIVALENCE (NCATCD, NLDTCD(   CATHCD))
#if defined(CLEO_CDGDOC)
* NSSTCD - Number of CD silicon strip layers
* LSSTCD - The first layer of CD silicon strip
* MSSTCD - The last layer of CD silicon strip
#endif
      INTEGER NSSTCD, LSSTCD, MSSTCD
      EQUIVALENCE (LSSTCD, ILDTCD(1, SSTRCD))
      EQUIVALENCE (MSSTCD, ILDTCD(2, SSTRCD))
      EQUIVALENCE (NSSTCD, NLDTCD(   SSTRCD))
#if defined(CLEO_CDGDOC)
*
* LCANIZ - First IZ anode layer
* MCANIZ - Last  IZ anode layer
*
#endif
      INTEGER    LCANIZ,      MCANIZ
      EQUIVALENCE (LCANIZ,ILDVCD(1,IPROP)),
     *            (MCANIZ,ILDVCD(2,IPROP))
#if defined(CLEO_CDGDOC)
*
* LCANIV - First IV anode layer
* MCANIV - Last  IV anode layer
*
#endif
      INTEGER    LCANIV,              MCANIV
      EQUIVALENCE (LCANIV,ILDVCD(1,IIVDT)),
     *            (MCANIV,ILDVCD(2,IIVDT))
#if defined(CLEO_CDGDOC)
*
* LCANPT - First PTL anode layer
* MCANPT - Last  PTL anode layer
*
#endif
      INTEGER    LCANPT,              MCANPT
      EQUIVALENCE (LCANPT,ILDVCD(1,IPTLA)),
     *            (MCANPT,ILDVCD(2,IPTLA))
#if defined(CLEO_CDGDOC)
*
* LCANVD - First VD anode layer
* MCANVD - Last  VD anode layer
*
#endif
      INTEGER    LCANVD,              MCANVD
      EQUIVALENCE (LCANVD,ILDVCD(1,IVDET)),
     *            (MCANVD,ILDVCD(2,IVDET))
#if defined(CLEO_CDGDOC)
*
* LCANDR - First DR anode layer
* MCANDR - Last  DR anode layer
*
#endif
      INTEGER    LCANDR,              MCANDR
      EQUIVALENCE (LCANDR,ILDVCD(1,IDRFT)),
     *            (MCANDR,ILDVCD(2,IDRFT))
#if defined(CLEO_CDGDOC)
*
* LCATPT - First PTL cathode layer
* MCATDR - Last  PTL cathode layer
*
#endif
      INTEGER    LCATPT,              MCATPT
      EQUIVALENCE (LCATPT,ILDVCD(1,IPTLC)),
     *            (MCATPT,ILDVCD(2,IPTLC))
#if defined(CLEO_CDGDOC)

*
* LCATVD - First VD cathode layer
* MCATVD - Last  VD cathode layer
*
#endif
      INTEGER    LCATVD,              MCATVD
      EQUIVALENCE (LCATVD,ILDVCD(1,IVDCT)),
     *            (MCATVD,ILDVCD(2,IVDCT))
#if defined(CLEO_CDGDOC)
*
* LCATDR - First DR cathode layer
* MCATDR - Last  DR cathode layer
*
#endif
      INTEGER    LCATDR,              MCATDR
      EQUIVALENCE (LCATDR,ILDVCD(1,IDRCT)),
     *            (MCATDR,ILDVCD(2,IDRCT))
#if defined(CLEO_CDGDOC)
*
* LCSVX1 - First SI R-Phi layer
* MCSVX1 - Last  SI R-Phi layer
*
#endif
      INTEGER    LCSVX1,              MCSVX1
      EQUIVALENCE (LCSVX1,ILDVCD(1,ISVX1)),
     *            (MCSVX1,ILDVCD(2,ISVX1))
#if defined(CLEO_CDGDOC)
*
* LCSVZ1 - First SI Z layer
* MCSVZ1 - Last  SI Z layer
*
#endif
      INTEGER    LCSVZ1,              MCSVZ1
      EQUIVALENCE (LCSVZ1,ILDVCD(1,ISVZ1)),
     *            (MCSVZ1,ILDVCD(2,ISVZ1))
#if defined(CLEO_CDGDOC)
C
C
C
C== CDGEOC - CD Geometry CHARACTER COMMON
C
* DVNMCD - CD device name (Four letters for the moment.)
* DTNMCD - CD device type name (Four letters for the moment.)
* CLTYCD - CD CELL TYPE names
*
#endif
      CHARACTER*4     DVNMCD,         DTNMCD,         CLTYCD
      COMMON /CDGEOC/ DVNMCD(MXDVCD), DTNMCD(MXDTCD), CLTYCD(MXTYCD)
      SAVE   /CDGEOC/
#if defined(CLEO_CDGDOC)
*
*END V2.00
C
C== CDGEOI - CD Geometry INTEGER COMMON
C
* IDVCCD - Device code for this layer
* IXSFCD - Device layer # from CD layer #
* INDXCD - IADRHW address of wire # 0 of CD layer #
* INDVCD - Relative address in device of wire 0
* NWIRCD - # of wires in cylinder
* NWRZCD - # of Z divisions (strips or charge division)
* IXLRCD - XREF from device and local layer num to ILYR
*          This is indexd by the device definition parameters
* ICELCD - Cell type code for each layer
*V2.00
* ICATCD - CD layer number of I-th cathode layer (I is inbetween 1 and NCATCD)
* IAXLCD - CD layer number of I-th axial   wire  (I is inbetween 1 and NAXLCD)
* ISTRCD - CD layer number of I-th stereo  wire  (I is inbetween 1 and NSTRCD)
* ICANCD - CD layer number of I-th anode   wire  (I is inbetween 1 and NCANCD)
* ISSLCD - CD layer number of I-th silicon layer (I is inbetween 1 and NSSTCD)
* ATOCCD - CD layer number of a cathode associated with the I-th anode wire.
* CTOACD - CD layer number of an anode wire associated with the I-th cathod.
* ILBSCD - CD base address for device logical address
* NFLDCD - # of field wires
*END V2.00
*
#endif
      INTEGER IDVCCD, IXSFCD, INDXCD, INDVCD, NWIRCD, NWRZCD, IXLRCD
      INTEGER NLOFCD, ICELCD, ICATCD, IAXLCD, ISTRCD, ICANCD, ATOCCD
      INTEGER CTOACD, ILBSCD, NFLDCD, ISSTCD
#if defined(CLEO_CDGDOC)
* The first and last word in the common block
C
#endif
      INTEGER CDFIRI, CDLASI
      COMMON /CDGEOI/ CDFIRI,
     1                IDVCCD(KLYRCD), IXSFCD(KLYRCD), INDXCD(KLYRCD),
     1                INDVCD(KLYRCD), NWIRCD(KLYRCD), NWRZCD(KLYRCD),
     1                IXLRCD(KLYRCD,MXDVCD), ICELCD(KLYRCD),
     1 ICATCD(KLYRCD), IAXLCD(KLYRCD), ISTRCD(KLYRCD), ICANCD(KLYRCD),
     1 ATOCCD(KLYRCD), CTOACD(KLYRCD), ILBSCD(KLYRCD), NFLDCD(KLYRCD),
     * ISSTCD(KLYRCD),
     1 CDLASI
      SAVE   /CDGEOI/
#if defined(CLEO_CDGDOC)
C
C== CDGEOL - CD Geometry LOGICAL COMMON
C
* LDVPCD - .TRUE. If corresponding device is present
* LQDVCD - .TRUE. If this layer has charge division
* LSTLCD - .TRUE. If this layer is stereo
*V2.00
* LAXLCD - .TRUE. If this layer is axial
* LCTLCD - .TRUE. If this layer is cathode
*END V2.00
*
#endif
      LOGICAL LDVPCD, LQDVCD, LSTLCD, LAXLCD, LCTLCD, LSSLCD
#if defined(CLEO_CDGDOC)
* The first and last word in the common block
#endif
      LOGICAL CDFIRL, CDLASL
      COMMON /CDGEOL/ CDFIRL,
     *LDVPCD(KLYRCD), LQDVCD(KLYRCD), LSTLCD(KLYRCD),
     +                LAXLCD(KLYRCD), LCTLCD(KLYRCD),
     *                LSSLCD(KLYRCD),
     * CDLASL
      SAVE   /CDGEOL/
#if defined(CLEO_CDGDOC)
C
C== CDGEOR - CD Geometry REAL COMMON
C
* RCD    - Radius         of layer at Z = 0
* RSQCD  - Radius-squared of layer at Z = 0
* REFFCD - Displacement of effec. cath. radius from assoc. anode
* TANSCD - Tangent of the stereo angle of layer
* RENDCD - Radius of layer at Z = ZENDCD(ILYR)
* DIFSCD - Number of cells between wire location at each end
* ANGSCD - Stereo angle of layer
*** PHI0CD - Nominal PHI of wire 0 at Z = 0 in cylinder
* PHIFCD - Actual  PHI of wire 0 at Z = 0 in cylinder
* CELLCD - Width of cell in PHI (radians)
* ZEEFCD - Z of centre of strip # 0
* CELZCD - Width of cell in Z (meters)
* ZENDCD - Z at end of layer
* XX0CD  - (X,Y,Z)  Offset of layer
* UU0CD  - (X,Y,Z)  Direction cosine of axis of layer
*** DPHICD - Add to PHI0CD to get PHIFCD
* BSAGCD - Max Y sag of wires.  Returned to array BSAGCD(KLYR) 11/26/90 BPG
*** BSAGCD - Made into a function BSAGCD(IR,Z) in DUMMYCDCAL.OLB 5/16/90 MGS
* WENDCD - Which end wire read out from: +1.0 => +VE Z
*** CELOCD - Cell offset of wire 0 (units of cell width)
* CELRCD - Cell radial height (meters)
* RDLFCD - RaDiation Length of Field wires in meter
* WIDFCD - Diameter of Field wires in meter
*
#endif
      REAL RCD,    RSQCD,  TANSCD
      REAL RENDCD, DIFSCD, ANGSCD
***      REAL PHI0CD, PHIFCD, CELLCD
      REAL PHIFCD, CELLCD, REFFCD
      REAL ZEEFCD, CELZCD, ZENDCD
      REAL XX0CD,  UU0CD
      REAL WENDCD
***      REAL CELOCD, CELRCD
      REAL CELRCD, RDLFCD, WIDFCD
      REAL BSAGCD
#if defined(CLEO_CDGDOC)
C
* The first and last word in the common block
#endif
      REAL CDFIRR, CDLASR
      COMMON /CDGEOR/ CDFIRR,
     1                RCD   (KLYRCD), RSQCD (KLYRCD), TANSCD(KLYRCD),
     1                RENDCD(KLYRCD), DIFSCD(KLYRCD), ANGSCD(KLYRCD),
     1                PHIFCD(KLYRCD), CELLCD(KLYRCD),
     1                ZENDCD(KLYRCD), ZEEFCD(KLYRCD), CELZCD(KLYRCD),
     1                XX0CD(3,KLYRCD),UU0CD(3,KLYRCD),
     1                WENDCD(KLYRCD),
     1                CELRCD(2, KLYRCD), RDLFCD(KLYRCD), WIDFCD(KLYRCD),
     1                REFFCD(KLYRCD), BSAGCD(KLYRCD), CDLASR
      SAVE   /CDGEOR/
#if defined(CLEO_CDGDOC)
C
C Beam position
C
* XXBMCD Nominal beam position
* ERBMCD Error on XXBMCD
#endif
      REAL XXBMCD, ERBMCD
      COMMON / CDGMBM / XXBMCD(3), ERBMCD(3)
      SAVE   /CDGMBM/
#if defined(CLEO_CDGDOC)
*
C
*V2.00
C ZFILES pointers to logical addresses. To use these pointers you
C need to include +SEQ, USBANK.
C CD Logical Address is defined as follows. CDLA of 1 is assigned to the
C device logical address (DLA) of 1 (See reference CSN 89-xx) of the first
C CD device. All addresses are assigned consectively and CDLA of
C (NDVWCD(1) + 1) is assigned to DLA of 1 of the second devicce.
C
C
* IDOFCD - converts CDLA to the device definition parameter.
* IROFCD - converts CDLA to the CD layer number.
* IWOFCD - converts CDLA to the CD wire number in the layer. (0 to NWIRCD-1)
*          It is 0 for cathodes.
* IPOFCD - converts CDLA to the CD phi section number. It is 0 for anodes.
* ISOFCD - converts CDLA to the CD strip number. It is 0 for cathodes.
* ISCDPS - detector position banks
* IMOFCD - quicker ( not tested ) conversion for CD layer number.
*          IR = IZU(PZU(IMOFCD)+1+(ICDLA-1)/MMOFCD)
*          IF(IR.LT.0) IR = IZU(PZU(ILOFCD)+ICDLA)
*
* The following are defined by PAH, Refer to his manual of EA to CDLA
* conversion.
*
* ICOFCD - converts CDLA to Crate number
* IAOFCD - converts CDLA to cArd number
* IHOFCD - converts CDLA to cHannel number
*
* ILFOCD - converts Electronic address to Layer number
* IPFOCD - converts Ea to Phi
* IWFOCD - converts Ea to Wire number
* Let's assume ICDLA is the CD Logical Address. The CD layer number is
* obtained by IRCD = IZU(PZU(IROFCD)+ICDLA).
* Of course, these conversion should be done in the lowest level routines.


#endif
#if !defined(CLEO_RS6000)
      INTEGER         MSOFCD    , MMOFCD
      PARAMETER     ( MSOFCD = 5, MMOFCD = 2**MSOFCD )
#endif
#if defined(CLEO_RS6000)
      INTEGER         MSOFCD    , MMOFCD
      PARAMETER     ( MSOFCD = 5, MMOFCD = 32 )
#endif

      INTEGER         IDOFCD, ILOFCD, IWOFCD, IPOFCD, ISOFCD, ISCDPS
      INTEGER         ICOFCD, IAOFCD, IHOFCD, ILFOCD, IPFOCD, IWFOCD
      INTEGER         IMOFCD
      COMMON/ CDGEOZ/ IDOFCD, ILOFCD, IWOFCD, IPOFCD, ISOFCD, ISCDPS,
     1                ICOFCD, IAOFCD, IHOFCD, ILFOCD, IPFOCD, IWFOCD,
     1                IMOFCD
      SAVE   /CDGEOZ/
C
#if defined(CLEO_CDGDOC)
C  Offsets, tilts, etc. for each device. These quantities are added to
C  the ones measured per layer.
C
C  XXDVCD - (x,y,z) offset of each device.
C           Indexed by device definition order.
C  UUDVCD - Direction cosines of each device.
C           Indexed by device definition order.
C  RTDVCD - rotation of each device around axis of symmetry
C           Indexed by device definition order.
#endif
C
      REAL XXDVCD, UUDVCD, RTDVCD
      COMMON /CDGOFF/ XXDVCD(3,KDEVCD), UUDVCD(3,KDEVCD),
     *                RTDVCD(KDEVCD)
#if defined(CLEO_CDGDOC)
C
C  Parameters for all the devices (different for each device)
C  It is indexed by (variable, device)
C  PARCD (IPARCD) - parameters for each CD device
C
C  The following data bases are device specific
C  They are indexed by (variable, device layer)
C  DADCD (IDRDCD) - data base for DR anode
C  DCDCD (IDCDCD) - data base for DR cathode
C  SIDCD (ISIDCD) - data base for silicon
C ISIBCD          - number of sides per silicon layer
#endif
C      REAL     PARCD
C      REAL     DADCD,  DCDCD,  SIDCD
C      INTEGER ISIBCD
C
C      COMMON /CDDV1R/ PARCD(KPARVA,KDEVCD),
C     *                DADCD(KDADVA,NCANDR),  DCDCD(KDCDVA,NCATDR),
C     *                SIDCD(KSIDVA,KSIDPL,NCANSI), ISIBCD(NCANSI)
C
C      INTEGER        IPARCD(KPARVA,KDEVCD)
C      INTEGER        IDADCD(KDADVA,NCANDR), IDCDCD(KDCDVA,NCATDR)
C      INTEGER        ISIDCD(KSIDVA,KSIDPL,NCANSI)
CC
C      EQUIVALENCE (DADCD, IDADCD), (DCDCD, IDCDCD)
C      EQUIVALENCE (SIDCD, ISIDCD)
C
#if defined(CLEO_CDGDOC)
C   Scattering data base
C
* NSCTCD   number of scatterers
*  SCTCD   scattering data base
* ISCTCD   equivalenced to SCTCD
#endif
      INTEGER NSCTCD
      REAL SCTCD
      COMMON /CDSC1R/ NSCTCD, SCTCD(KSCTVA,KSCTCD)
      INTEGER ISCTCD(KSCTVA,KSCTCD)
      EQUIVALENCE (SCTCD, ISCTCD)
C
#if defined(CLEO_CDGDOC)
C
C
C Guide for converting one index to another.
C
C Definition if the indices. To specify a wire or strip there are 5 ways to
C index it.
C
C (1) Central Detector Logical Addressing (CDLA) is the most essential
C     addressing scheme. It starts from 1 and ends at MWIRCD. It is continguous
C     and dense. But this address is not determined until the actual job starts.
C     Because we have a device with charge division, some complecation can not
C     be avoided.
C (2) Central Detector Layer number and Wire number (for anodes), Layer number
C     and phi number and strip number. (CDL, CDW) or (CDL, CDP, CDS)
C CDW = DVW is wire number in the layer. It is inbetween 0 and NWIRCD(CDL)-1
C For charge division wire, it is inbetween 0 and 2*nwircd(CDL)-1.
C CDL is CD layer number. It is inbetween 1 and MLYRCD.
C CDP is phi section number. between 0 and NWIRCD(CDL)/NWRZCD(CDL)-1.
C CDS is strip number. between 0 and NWRZCD(CDL)-1.
C
C (3) Device logical address and device configuration number. (DVN,DLA)
C DVN is the device number. This is defined by parameter statements in
C CDGEOMPA.
C (4) Device Layer number and device wire number (DVN,DVL,CDW) or
C     (DVN,DVL,CDP,CDS), wire, phi and strip numbers are same for CD numbering.
C DVL is Device layer number. Conversion between CDL and DVL is.
C       DVL = IXSFCD(cdl)
C       CDL = IXLRCD(DVL,DVN)
C (5) electronics address (EA)(CRATE,SLOT,CHANNEL) There is two kinds. Designed
C     and actual. We will come back to this point later.
C
C     xx out of 5*4 conversions are easily performed using either a function
C     or an array in CDGEOM. (all indices are integer.)
C
C ( 12) CDLA to (CDL,CDW) or (CDL,CDP,CDS)
C       CDL = IZU(PZU(ILOFCD)+CDLA)
C       CDW = IZU(PZU(IWOFCD)+CDLA) (between 0 and NWIRCD-1, -1 for cathodes )
C       CDW will be between NWIRCD and 2*NWIRCD-1 for a secondary channel
C       of the charge division device.
C       CDP = IZU(PZU(IPOFCD)+CDLA), -1 for anodes
C       CDS = IZU(PZU(ISOFCD)+CDLA), -1 for anodes
C ( 13) CDLA to (DVN,DLA)
C       DVN = IZU(PZU(IDOFCD)+CDLA)
C       DLA = CDLA - ILBSCD(ILDVCD(1,DVN))
C       DLA = CDLA - ILBDCD(DVN)
C ( 14) CDLA to (DVN,DVL,CDW) or (DVN,DVL,CDP,CDS)
C       DVN = IZU(PZU(IDOFCD)+CDLA)
C       cdl = IZU(PZU(ILOFCD)+CDLA), then
C       DVL = IXSFCD(cdl)
C       CDW = IZU(PZU(IWOFCD)+CDLA), -1 for cathodes
C       CDW will be between NWIRCD and 2*NWIRCD-1 for a secondary channel
C       of the charge division device.
C       CDP = IZU(PZU(IPOFCD)+CDLA), -1 for anodes
C       CDS = IZU(PZU(ISOFCD)+CDLA), -1 for anodes
C ( 15) CDLA to EA
C       No direct path is provided.
C       (14) and (45)
C
C ( 21) (CDL,CDW) or (CDL,CDP,CDS) to CDLA
C       CDLA = INDXCD(CDL) + CDW for anodes
C       CDW is in between 0 and 2*NWIRCD(CDL)-1 for charge div. dev.
C       CDLA = INDCCD(CDL,CDP) + CDS (not yet implemented) for cathodes
C       INDCCD is a function at this point.
C ( 23) (CDL,CDW) or (CDL,CDP,CDS) to (DVN,DLA)
C       DVN = IDVCCD(CDL)
C       For DLA, go through CDLA. (21) and (13)
C       DLA = INDXCD(CDL) + MOD(CDW,NWIRCD(CDL)) - ILBSCD(CDL) for anodes
C       DLA = INDCCD(CDL,CDP) + CDS - ILBSCD(CDL) (not yet implemented) for
C       cathodes.
C ( 24) (CDL,CDW) or (CDL,CDP,CDS) to (DVN,DVL,CDW) or (DVN,DVL,CDP,CDS)
C       DVN = IDVCCD(CDL)
C       DVL = IXSFCD(CDL)
C ( 25) (CDL,CDW) or (CDL,CDP,CDS) to EA
C       No direct path is provided.
C       (24) and (45)
C
C ( 31) (DVN,DLA) to CDLA
C       CDLA = DLA + ILBSCD(ILDVCD(1,DVN))
C       CDLA = DLA + ILBDCD(DVN)
C
C ( 32) (DVN,DLA) to (CDL,CDW) or (CDL,CDP,CDS)
C       go through CDLA. (31) and (12)
C ( 34) (DVN,DLA) to (DVN,DVL,CDW) or (DVN,DVL,CDP,CDS)
C
C       CDLA = DLA + ILBSCD(ILDVCD(1,DVN))
C       CDLA = DLA + ILBDCD(DVN)
C       CDL = IZU(PZU(ILOFCD)+CDLA)
C       DVL = IXSFCD(CDL)
C       CDW = IZU(PZU(IWOFCD)+CDLA)
C       CDP = IZU(PZU(IPOFCD)+CDLA)
C       CDS = IZU(PZU(ISOFCD)+CDLA)
C
C ( 35) (DVN,DLA)to EA
C       No direct path is provided.
C       (31), (14) and (45)
C
C ( 41) (DVN,DVL,CDW) or (DVN,DVL,CDP,CDS) to CDLA
C       go through CDL,,,. (42) and (21)
C
C ( 42) (DVN,DVL,CDW) or (DVN,DVL,CDP,CDS) to (CDL,CDW) or (CDL,CDP,CDS)
C       CDL = IXLRCD(DVL, DVN)
C
C ( 43) (DVN,DVL,CDW) or (DVN,DVL,CDP,CDS) to (DVN,DLA)
C       go through (42), (21) and (13)
C       DLA = INDXCD(IXLRCD(DVL,DVN)) + MOD(CDW,NWIRCD(CDL))
C             - ILBSCD(CDL)
C
C ( 45) (DVN,DVL,CDW) or (DVN,DVL,CDP,CDS) to EA
C       Use EOFLCD(DVN,DVL,CDW,IQDV,CR,SL,CH)
C
C ( 51) EA to CDLA
C       go through (54), (42) then (21)
C ( 52) EA to (CDL,CDW) or (CDL,CDP,CDS)
C       go through (54) and (42)
C ( 53) EA to (DVN,DLA)
C       (54) and (42), (21) and (13)
C ( 54) EA to (DVN,DVL,CDW) or (DVN,DVL,CDP,CDS)
C       Use LOFECD(DVN,DVL,CDW,IQDV,CR,SL,CH)
*END V2.00
C Scattering materials.
* LSCACD - First  effective scattering surface
* MSCACD - Last   effective scattering surface
* LSCCCD - First  continuous scattering medium
* MSCCCD - Last   continuous scattering medium
* IKNKCD - Index of scatterer where kink is fitted
#endif
      INTEGER         LSCACD, MSCACD, LSCCCD, MSCCCD, IKNKCD
      COMMON /CDSCAI/ LSCACD, MSCACD, LSCCCD, MSCCCD, IKNKCD
      SAVE   /CDSCAI/

      REAL            RSCACD, XSCACD, RSCCCD, XSCCCD
      COMMON /CDSCAR/
     *                RSCACD(NSCACD), XSCACD(NSCACD),
     *                RSCCCD(NSCCCD), XSCCCD(NSCCCD)
      SAVE   /CDSCAR/






