*
*CMZ :          31/01/94  12.27.45  by  Unknown
*CMZ :  2.01/11 13/11/89  11.31.12  by  Nobu Katayama
*-- Author :
C
C      INCLUDE 'CLINC:CDGEOM.PAR'
C
C.......................................................................
C.
C. CDGEOM - PARAMETERS Defining CLEO-II Central Detector Geometry.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    : R. Namjoshi, N. Katayama
C.
C. VERSION   : 2.00
C. CREATED   : 02-Sep-86
C. LAST MOD  : 06-Jan-89
C.
CRYD     Started to put in SVX 29-jul-93
C.
C. Modification Log.
C.
C. 16-Nov-86 : R. Namjoshi     Add control flag parameters
C. 25-Oct-87 : R. Namjoshi     Make completely F77 compatible
C. 06-Jan-89 : N. Katayama     Preliminary version for CLEO-II
#if defined(CLEO_CDGDOC)
C.                             All new variables are marked *V2.0 and *END V2.0
C.                             Major changes are the following.
C.   (1) The ordering of the device is not assumed at the compile time.
C.   (2) There are minimul number of PARAMETER statements just enough for
C.       all the declarations. Use KDEVCD, MXDVCD, KLYRCD, KCANCD, KCATCD
C.       KCSTCD, KCAXCD, and KWIRCD only to declare dimensions.
C.       In particular, dinamic allocation of arrays of size MWIRCD
C.       (actual number of channels) is recommanded.
C.   (3) There are two different kind of device ordering. One is indexed by
C.       the device definition parameters (IDRFT, IPROP, IPTLA, etc.) The
C.       other is indexed by the device configuration, which is done at the
C.       beginning of the execution. The ordering as well as inclusion of the
C.       devices are totally configurable. Although
C.       there is one assumption you can make. Since at this moment all
C.       layers are cylindrical, the layers are ordered by their radii except
C.       that all cathodes come after all anodes.
C.   (4) Many new handy-dandy indices and variables are added. They may be
C.       more than necessary. In particular, variables related to the device
C.       have two forms differently ordered, the configuration order and the
C.       definition order. For example, NWDVCD is the number of wires of each
C.       device ordered by the device definition parameters, NDVWCD by the
C.       configuration ordering. Use which ever you like, but do not confuse.
C.       if the dimension of an array is KDEVCD, the ordering is the
C.       configuration ordering and MXDVCD the device definition ordering.
C.   (5) Most of the disapeared parameters are now defined as variables in
C.       the common block CDGEOP (LXXXXX and MXXXXX are EQUIVALENCed  to
C.       an array.) Try to compile your program with new definitions.
C.......................................................................
C
C    ******************************************************************
C    *                                                                *
C    *    Layers are configured at the execution time.                *
C    *    Basic constraints are anode layers in radial order,         *
C    *    and then cathodes in radial order. New type of device       *
C    *    such as a pixel device should come after the cathodes...    *
C    *                                                                *
C    ******************************************************************
C
C== Device definitions
C
* KDEVCD - Number of maximum different CD devices for a job in
*          the CDGEOM. You need to re compile all the code when you
*          increase this number.
#endif
      INTEGER    KDEVCD
      PARAMETER( KDEVCD =   16 )
#if defined(CLEO_CDGDOC)
*
* MXDVCD - The maximum possible number of devices defined in
*          the CDGEOM. You need to re compile all the code when you
*          increase this number.
#endif
      INTEGER MXDVCD
      PARAMETER( MXDVCD =   16 )
#if defined(CLEO_CDGDOC)
*
* MXDTCD - The maximum possible number of device types defined in
*          the CDGEOM. You need to re compile all the code when you
*          increase this number.
#endif
      INTEGER MXDTCD
      PARAMETER( MXDTCD =   4 )
#if defined(CLEO_CDGDOC)
*
* These parameters are called the device definition parameters
* IDRFT  - Drift  chamber   device
* IDRCT  - DR cathode strip device
* ISVX1  - Silicon vertex detector first generation R-Phi
* ISVZ1  - Silicon vertex detector first generation Z
* IIVDT  - the "IV" ; the 3 layer Ohio straw device 
* IVDET  - the "VD" ; the 10 layer Ohio small cell device
* IVDCT  - "VD" cathodes 
* IPROP  - the "IZ" ; the 3 layer Syracuse MWPC device
* IPTLA  - the "PT" ; the 6 layer Ohio straw device 
* IPTLC  - "PT" cathodes, although it didi not have any 
* IZDA   - the "ZD" ; the 6 layer Cornell open cell device
* ITPC1  - Dan's imaginary TPC device
 
#endif

#include "cl3seq/cdgm3/devicecodes.inc"
*23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      INTEGER     IDRFT,           IDRCT
      PARAMETER(  IDRFT = DEVICE_DRFT,       IDRCT = DEVICE_DRCT )
      CHARACTER*4 CRDRFT,          CRDRCT
      PARAMETER(  CRDRFT = 'DRAN', CRDRCT = 'DRCT' )

      INTEGER     ISVX1,           ISVZ1
      PARAMETER ( ISVX1 = DEVICE_SVX1,       ISVZ1 = DEVICE_SVZ1 )
      CHARACTER*4 CRSVX1,          CRSVZ1
      PARAMETER(  CRSVX1 = 'SVX1', CRSVZ1 = 'SVZ1' )

      INTEGER     IIVDT
      PARAMETER(  IIVDT = DEVICE_IVDT )
      CHARACTER*4 CRIVDT
      PARAMETER(  CRIVDT = 'IVDT')

      INTEGER     IVDET,           IVDCT
      PARAMETER(  IVDET = DEVICE_VDET,       IVDCT = DEVICE_VDCT )
      CHARACTER*4 CRVDET,          CRVDCT
      PARAMETER(  CRVDET = 'VDAN', CRVDCT = 'VDCT' )

      INTEGER     IPROP
      PARAMETER(  IPROP = DEVICE_PROP )
      CHARACTER*4 CRPROP
      PARAMETER(  CRPROP = 'PROP' )

      INTEGER     IPTLA,           IPTLC
      PARAMETER(  IPTLA = DEVICE_PTLA,       IPTLC = DEVICE_PTLC )
      CHARACTER*4 CRPTLA,          CRPTLC
      PARAMETER(  CRPTLA = 'PTLA', CRPTLC = 'PTLC' )

      INTEGER     IZDA 
      PARAMETER(  IZDA = DEVICE_ZDA )
      CHARACTER*4 CRZDA 
      PARAMETER(  CRZDA = 'ZD  ' )

      INTEGER     ITPC1
      PARAMETER(  ITPC1=  DEVICE_TPC1 )
      CHARACTER*4 CRTPC1
      PARAMETER(  CRTPC1 = 'TPC1' )




#if defined(CLEO_CDGDOC)
*
* Device type definition. This also defines the order of device types
* in CDGEOM. The order is (1) anodes (2) cathodes (3) Silicon strips
* (4) SVZ strips
*
*
#endif
      INTEGER    ANODCD,     CATHCD
      PARAMETER (ANODCD = 1, CATHCD = 2)
      CHARACTER*4 CRANOD,    CRCATH
      PARAMETER (CRANOD = 'ANOD', CRCATH = 'CATH')

      INTEGER     SSTRCD
      PARAMETER ( SSTRCD = 3 )
      CHARACTER*4 CRSSTR
      PARAMETER ( CRSSTR = 'SSTR' )

#if defined(CLEO_CDGDOC)
C KWIRCD - (was the) Maximum possible number of wires in the CDGEOM.
C        not it appears that KWIRCD is used only to build KWS1CD
C        which is used as the dimension declaration of IADRHW in cdscrtcd
C
C
C When possible, avoid using this parameter. Use dinamic creation of
C arrays using ZEBRA. It is easy. Here is an example. Assuming you have
C +SEQ, USBANK. statement, declare ISXXXX as an INTEGER and SAVE it.
C (replace XXXX with meaningful name.)
C get pointer by calling USGSLK at the beginnig of the job.
*     CALL USGSLK(ISXXXX, CRNAME)
C (An error occured if ISXXXX is null.)
C and if you want to allocate an array permanently, do exactly like,
*     CALL MZBOOK(IUSCON, PZU(ISXXXX), PZU(ISXXXX), 1, 'XXXX', 0, 0,
*    1            MWIRCD, 2, 0)
C and you can use space between IZU(PZU(ISXXXX)+1) and
C IZU(PZU(ISXXXX)+MWIRCD).
C Note, if REAL use ZU instead of IZU. MWIRCD is a variable in one
C of CDGEOM COMMON BLOCKS specifying the number of wires of current
C CD configuration.
C If you want allocate an array event by event, do exactly like,
*     CALL MZBOOK(IUSEVT, PZU(ISXXXX), PZU(ISXXXX), 1, 'XXXX', 0, 0,
*    1            MWIRCD, 2, 0)
C instead. In this case, your BANK is wiped at the end of your event
C and you have to call MZBOOK again at the beginning of the next event.
C Before you put some values into space between IZU(PZU(ISXXXX)+1) and
C IZU(PZU(ISXXXX)+MWIRCD), it is a good idea to check if PZU(ISXXXX) is
C not zero.
#endif
      INTEGER    KWIRCD

#if defined(CLEO_DOITTPC)
      PARAMETER( KWIRCD = 560000 )
#else
      PARAMETER( KWIRCD = 20480 )
#endif

#if defined(CLEO_CDGDOC)

C
C== Layer definitions of the device. Use only for the declarations
C
* NCANDR - Number of DR anode   layers
* NCATDR - Number of DR cathode layers
* NCAXDR - Number of DR axial   layers
* NCSTDR - Number of DR stereo  layers
* NSSVX1 - Number of SVX1 strip layers
*
#endif
      INTEGER    NCAXDR
      PARAMETER( NCAXDR = 16 )
      INTEGER    NCSTDR,      NCANDR,      NCATDR,	NCTIDR
      PARAMETER( NCSTDR = 31, NCANDR = 47, NCATDR =  1,	NCTIDR=0)
      INTEGER     NSSVX1
      PARAMETER( NSSVX1 =  4 )

      INTEGER    NCANIZ,      NCANIV,      NCANVD
      PARAMETER( NCANIZ =  0, NCANIV =  3, NCANVD = 10)
      INTEGER    NCATVD
      PARAMETER( NCATVD =  2)
      INTEGER    NCANPT,      NCATPT
      PARAMETER( NCANPT =  6, NCATPT =  0 )
      INTEGER    NCANZD,      NCATZD
      PARAMETER( NCANZD =  6, NCATZD =  0 )
      INTEGER    NRDTPC
      PARAMETER( NRDTPC = 144 )


#if defined(CLEO_CDGDOC)
C
C== Wire  definitions of the device. Use only for declarations
C
* KWIRDX - Number of DR axial   wires
* KWIRDS - Number of DR stereo  wires
* KWIRDR - Number of DR         wires
* KSTPDR - Number of DR cathode strips
* KSSVX1 - Number of SVX1 strips (R-Phi)
* KSSVZ1 - Number of SVZ1 strips Z
#endif


      INTEGER    KWIRDX,        KWIRDS,        KWIRDR
      PARAMETER( KWIRDX = 1696, KWIRDS = 8100, KWIRDR = KWIRDX+KWIRDS)
      INTEGER    KSTPDR
      PARAMETER( KSTPDR = 3744)
      INTEGER    KSSVX1,        KSSVZ1

#if defined(CLEO_DOITTPC)
      PARAMETER( KSSVX1 =    0, KSSVZ1 =    0)
#else
      PARAMETER( KSSVX1 =13104, KSSVZ1 =57456)
#endif

      INTEGER    KWS1CD
      PARAMETER( KWS1CD = KWIRCD + KSSVX1 + KSSVZ1 )

C dpp 10/7/97 space fillers
      INTEGER    KWIRIZ,      KWIRIV,        KWIRVD
      PARAMETER( KWIRIZ =  0, KWIRIV =  192, KWIRVD = 800)
      INTEGER    KWR2VD,       KWIRPT
      PARAMETER( KWR2VD = 2*KWIRVD, KWIRPT = 384 )
      INTEGER    KSTPPT,        KSTPVD
      PARAMETER( KSTPPT =  0,   KSTPVD = 1280)
C end dpp 10/7/97

#if defined(CLEO_CDGDOC)
C

C-----------------------------------------------------------------------
C
C== These PARAMETERS are derived from the geometry
C
#endif
#if defined(CLEO_CDGDOC)
* NCANCD, NCATCD, NCAPCD as well as NLYRCD became FORTRAN VARIABLES rather
* than PARAMETERS. Maximum possible NCANCD, NCATCD, NCAPCD and NLYRCD are
* defined here as KCANCD, KCATCD, KCAPCD and KLYRCD.
* These are only to be used for the declarations.
*
* KCAXCD - Maximum possible number of CD   axial anode   layers
* KCSTCD - Maximum possible number of CD  stereo anode   layers
* KCANCD - Maximum possible number of CD         anode   layers
* KCATCD - Maximum possible number of CD         cathode layers
* KLYRCD - Maximum possible number of CD                 layers
* KQDVCD - Maximum possible number of CD charge division layers
* KSTXCD - Maximum possible number of CD axial   strip   layers
* KSTZCD - Maximum possible number of CD Z       strip   layers
* KSTRCD - MAximum possible number of CD          strip  layers
#endif
      INTEGER    KCAXCD,        KCSTCD,      KCATCD
#if defined(CLEO_DOITTPC)
      PARAMETER( KCAXCD = 150,  KCSTCD =  0, KCATCD =  0 )
#else
      PARAMETER( KCAXCD =  20,  KCSTCD = 70, KCATCD = 10 )
#endif

      INTEGER    KCANCD
      INTEGER    KLYRCD,        KQDVCD
      PARAMETER( KCANCD = KCAXCD + KCSTCD,   KQDVCD = 20  )
      INTEGER    KSTXCD
      PARAMETER( KSTXCD = 4 )
      INTEGER    KSTZCD
      PARAMETER( KSTZCD = 4 )
      INTEGER    KSTRCD
      PARAMETER (KSTRCD = KSTXCD + KSTZCD )
C     KLYRCD is large enough to hold svd1 so we do not chenge it.
      PARAMETER( KLYRCD = KCANCD + KCATCD)
#if defined(CLEO_CDGDOC)
C
C== PARAMETERs for cell codes
C
* ICLTUB - Tubular cell
* ICLSQ1 - Square  cell DR1
* ICLSQ2 - Square  cell DR2
* ICLSQZ - Square  cell DR2 stereo
* ICLHEX - Hex     cell VD
* ICLS1A - Square  cell DR1 cathodes above (outer cathodes)
* ICLS1B - Square  cell DR1 cathodes below (inner cathodes)
* ICLS2A - Square  cell DR2 cathodes above
* ICLS2B - Square  cell DR2 cathodes below
* ICLHXA - Hex     cell VD  cathodes above
* ICLHXB - Hex     cell VD  cathodes below
* ICLHXB - Hex     cell VD  cathodes below
* ICLSTR - Cathode strip.
* ICLSST - Silicon strip
* ICLTPC - TPC
*
#endif
      INTEGER    ICLTUB,      ICLSQ1,      ICLSQ2,      ICLSQZ
      PARAMETER( ICLTUB =  1, ICLSQ1 =  2, ICLSQ2 =  3, ICLSQZ =  4 )
      INTEGER    ICLHEX,      ICLS1A,      ICLS1B,      ICLS2A
      PARAMETER( ICLHEX =  5, ICLS1A =  6, ICLS1B =  7, ICLS2A =  8 )
      INTEGER    ICLS2B,      ICLHXA,      ICLHXB,      ICLSTR
      PARAMETER( ICLS2B =  9, ICLHXA = 10, ICLHXB = 11, ICLSTR = 12 )
      INTEGER    ICLSST
      PARAMETER( ICLSST = 13 )
      INTEGER    ICLTPC
      PARAMETER( ICLTPC = 14 )

      INTEGER MXTYCD
      PARAMETER (MXTYCD = 14)

      CHARACTER*(*)    CRLTUB,      CRLSQ1,      CRLSQ2,      CRLSQZ
      PARAMETER( CRLTUB =  'TUBE', CRLSQ1 =  'SQ1 ', CRLSQ2 =  'SQ2 ',
     *CRLSQZ =  'SQZ ' )
      CHARACTER*(*)    CRLHEX,      CRLS1A,      CRLS1B,      CRLS2A
      PARAMETER( CRLHEX =  'HEX ', CRLS1A =  'SQ1A', CRLS1B =  'SQ1B',
     *CRLS2A =  'SQ2A' )
      CHARACTER*(*)    CRLS2B,      CRLHXA,      CRLHXB,      CRLSTR
      PARAMETER( CRLS2B =  'SQ2B', CRLHXA = 'HEXA',  CRLHXB = 'HEXB',
     *CRLSTR =  'STRP' )
      CHARACTER*(*)    CRLSST
      PARAMETER( CRLSST= 'SSTR' )
#if defined(CLEO_CDGDOC)
C
C== Flags for checking correct calling sequence of routines
C
#endif
      INTEGER    KCBDCD
      PARAMETER( KCBDCD = 10101010 )
      INTEGER    KCINCD
      PARAMETER( KCINCD = 20202020 )
      INTEGER    KCRUCD
      PARAMETER( KCRUCD = 30303030 )
#if defined(CLEO_CDGDOC)
*
* NSCACD - Max. number of discreet   scatterers (including beam pipe)
* NSCCDD - Max. number of continuous scatterers
*
#endif
      INTEGER    NSCACD,     NSCCCD
      PARAMETER( NSCACD = 4, NSCCCD = 4 )
C
#if defined(CLEO_CDGDOC)
*
* KSCTCD - Max. number of scatterers
* KSCTVA - Max. number of variables in scattering data base
*
#endif
      INTEGER    KSCTCD,      KSCTVA
      PARAMETER( KSCTCD = 20, KSCTVA = 12)
C
#if defined(CLEO_CDGDOC)
C   Parameters dimensioning the device parameter list
* KPARVA - max number of variables in device parameter list
C
* KPADVA - max number of variables in PT anode
* KPCDVA - max number of variables in PT cathode
* KVADVA - max number of variables in VD anode
* KVCDVA - max number of variables in VD cathode
* KDADVA - max number of variables in DR anode
* KDCDVA - max number of variables in DR cathode
* KSIDVA - max number of variables in silicon
*
CRYD Not put in by me! Don't know what it is used for!
* KSIDPL - max number of planes per layer in silicon
C
#endif
      INTEGER KPARVA
      PARAMETER (KPARVA = 15)
      INTEGER KDADVA, KDCDVA, KSIDVA
      PARAMETER (KDADVA = 12, KDCDVA = 10)
      PARAMETER (KSIDVA = 16)
      INTEGER KSIDPL
      PARAMETER (KSIDPL = 20)
















