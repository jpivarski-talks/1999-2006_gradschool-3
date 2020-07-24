*
* $Id: sfxtsc.inc,v 1.11 2003/03/05 18:38:14 dpp Exp $
*
* $Log: sfxtsc.inc,v $
* Revision 1.11  2003/03/05 18:38:14  dpp
*      -> move the TYP_BOUND to an integer only common block
*      -> define color for clustered hit
*
* Revision 1.10  2002/09/05 19:20:55  dpp
*      -> ZOOMU for sz plot,for now so sz plot size can be defined in xts_prep
*
* Revision 1.9  2000/12/19 23:31:00  dpp
*      -> new answer: other2
*
* Revision 1.8  2000/10/02 21:14:24  dpp
*      -> add axis locations of TFXF plot
*
* Revision 1.7  2000/08/18 18:37:34  dpp
*      -> implement new choice: multi-hit
*
* Revision 1.6  2000/04/26 14:28:31  dpp
* sfxtsp     -> add xts_rfview entry numbers
*            -> add parameters for saved {residual,radius} location
* sfxtsc     -> add saved {residual,radius} location of displayed hits
*
* Revision 1.5  2000/02/12 20:35:22  dpp
*      -> define new color for superlayer borders and border typ
*
* Revision 1.4  1999/10/12 15:13:59  dpp
* add "next pass" into button structure
*
* Revision 1.3  1999/10/08 13:41:00  lyon
* Fixed SunOS directive
*
* Revision 1.2  1999/01/11 02:50:31  lyon
* Added CLEO_SUNOS
*
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:42  clib
* Developmental version of DUET.
*
* Revision 1.5  1997/02/25 22:08:06  dpp
* add index's for XTS layer numbering
*
* Revision 1.4  1996/02/14 21:18:00  zfiles
* add SFLSFA,SFWSFA to sfcom.inc,  move SFLACD from sfxtsc.inc to sfcom.inc
*
* Revision 1.3  1995/09/25 19:47:25  zfiles
* add variable ENTRFV that had been local in XTS_RFVIEW
*
* Revision 1.2  1995/04/20  22:29:06  zfiles
* Dan's change. See TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:01:12  zfiles
* first version of doit in CVS
*
*
* COMMON BLOCKS FOR SF XTS ROUTINES
*
* sfxtsc.inc
*
#if !defined(CLEO_NOCOMDOC)
* SFXTSC DUCUMENTATION

C colors

C CLZERO...BACKGROUND OF THE PICTURE
C CLBORD...CHAMBER BORDERS
C CLSUBB...CHAMBER SUB-BORDERS
C CLLET1...1ST CHOISE OF LETTER COLOR
C CLWHIT...ALTERNATE CHOISE OF LETTER COLOR
C CLDUMB...
C CLHITM...HIGH TIME HITS
C CLNORM...NORMAL GOOD HITS THAT ARE AVAILABLE FOR NEW CHAINS
C CLCHAN...NORMAL HITS THAT ARE IN THE CURRENT DISPLAYED CHAIN
C CLUSED...USED IN PREVIOUS CHAINS, NOT AVAILABLE
C CLSTRO...STEREO
C CLHADC...HIGH ADC (MEANING LOW CHARGE)
C CLDUSE...ALREADY USED BY DUET
C CLDUCH...ALREADY USED BY DUET BUT ASSIGNED TO THE CURRENT CHAIN
C CLDADJ...ADJACENT TO A HIT USED BY DUET
C CLCLUS...CLUSTERED HIT: hit is NOT the primary hit of the cluster
C COLRHT...color of hit sent, hits specified by IDHIT
C COLORH...current color of specific hit, specified by XCENTRS etc

C other variables

C ENTRFV......entry staus of XTS_RFVIEW
C NSEND
C IDHIT
C WKID
C MAXSIZ
C DISPLAY
C BANNER
C INXSCD......index, in the xts geometry, of a  CD layer
C INXSSF......index, in the xts geometry, of an SF layer
C RBOUNDS.....radius of boundry circles
C TYP_BOUND...boundry type
C XYBSEG......({Y or Y},{1st or 2nd point}, {n})endpoints of boundry segments
C XCENTRS
C YCENTRS
C SIZHIT
C XCORNR
C YCORNR
C LEFTSF......FLAG=0;  OR 1 IF SF XTS HAS BEEN LEFT THROUGH XTS_LEAV
C NCHOIC......NUMBER OF CHOICES DEFINED
C LABCHO......returned address of choise
C NAMCHO......user name of choise which is displayed
C CHDEPEND....index of other choice, on which this is dependent
C LAZOOM_DRAW,LAZOOM,LAFORWARD,LABACKUP,LAFULLSIZE
C LAREDRAW,LANEW_DRAW, LAINFO,LA,LAID_TDC_ADC,LAID_MULTIH,LAIC_CCC
C LADISTANCE,LAWIR_DUET,LANEXT_HIT,LAINUCHN,LAISTFOR,LADSFDCH,LADSFDMP
C LACOLOR,LALOOP_1,LALOOP_2,LASKIP,LANEXT_EVENT,LANEXT_PASS
C LAANSWER,LAYES,LANO,LAALWAYS,LAOTHER,LAOTHER2,
C LAKILL,LAMORE,LACR,LAZHIT,LARESI
C      .......these are all indices for choices
C SHOCH
C NIDHLD
C IDHLD
C CLHLD
C ZOOMU.......CURRENT ZOOM NUMBER
C ZOOMIT
C ZOOMXY......COORDINATES
C ZOOMU_RPHI..CURRENT ZOOM NUMBER FOR R-PHI VIEW
C ZOOMU_TFXF..current ZOOM NUMBER for TFXFIT view
C ETYPE
C TRKHLD
C NCDXTS...number of cells displayed in XTS routines
C NBOUNDS..number of boundry circles
C NBNDSEQ..number of boundry straight line segments
C RESMIN......minimum residual on the TFXFIT plot
C RESMAX......maximum residual on the TFXFIT plot
C RESDIF......difference RESMAX-RESMIN
C RADMIN......minimum radius on the TFXFIT plot
C RADMAX......maximum radius on the TFXFIT plot
C RADDIF......difference RADMAX-RADMIN
C XAPOSL......x axis position left on the TFXFIT plot
C XAPOSR......x axis position right on the TFXFIT plot
C XAPOSB......x axis position bottom on the TFXFIT plot
C XAPOST......x axis position top on the TFXFIT plot
C YAPOSL......y axis position left on the TFXFIT plot
C YAPOSR......y axis position right on the TFXFIT plot
C YAPOSB......y axis position bottom on the TFXFIT plot
C YAPOST......y axis position top on the TFXFIT plot
C RESSAV......saved residual in TFXFIT diaplay
C RADSAV......saved radius in TFXFIT dispaly
#endif

#if defined(CLEO_ULTRIX)
      INTEGER   CLZERO,CLSTRO,CLHITM,CLNORM,CLCHAN,CLUSED
      INTEGER   CLDUMB,CLLET1,CLHADC,CLDUSE,CLDUCH,CLDADJ,CLCLUS
      INTEGER   CLLET2,CLBORD,CLSUBB
      INTEGER   COLRHT,COLORH,LABCHO,CLHLD
#else
C*****These are good for both Alpha and SunOS
      INTEGER*8 CLZERO,CLSTRO,CLHITM,CLNORM,CLCHAN,CLUSED
      INTEGER*8 CLDUMB,CLLET1,CLHADC,CLDUSE,CLDUCH,CLDADJ,CLCLUS
      INTEGER*8 CLLET2,CLBORD,CLSUBB
      INTEGER*8 COLRHT,COLORH,LABCHO,CLHLD
#endif
      INTEGER ENTRFV
      INTEGER NSEND,IDHIT
      INTEGER WKID
      CHARACTER*10 DISPLAY
      CHARACTER*10 BANNER
      REAL MAXSIZ
      INTEGER INXSCD,INXSSF
      REAL RBOUNDS,XYBSEG
      INTEGER TYP_BOUND
      REAL XCENTRS,YCENTRS,SIZHIT
      REAL XCORNR,YCORNR
      INTEGER LEFTSF
      INTEGER NCHOIC,CHDEPEND
      INTEGER LAZOOM_DRAW,LAZOOM,LAFORWARD,LABACKUP,LAFULLSIZE
      INTEGER LAREDRAW,LANEW_DRAW, LAINFO,LA,LAID_TDC_ADC,LAID_MULTIH
      INTEGER LAIC_CCC
      INTEGER LADISTANCE,LAWIR_DUET,LANEXT_HIT
      INTEGER LAINUCHN,LAISTFOR,LADSFDCH,LADSFDMP
      INTEGER LACOLOR,LALOOP_1,LALOOP_2
      INTEGER LASKIP,LANEXT_EVENT,LANEXT_PASS
      INTEGER LAANSWER,LAYES,LANO,LAALWAYS
      INTEGER LAOTHER,LAOTHER2,LAKILL,LAMORE,LACR,LAZHIT,LARESI
      CHARACTER*10 NAMCHO
      INTEGER SHOWCH,NIDHLD,IDHLD
      INTEGER ZOOMIT,ZOOMU,ZOOMU_RPHI,ZOOMU_TFXF,ZOOMU_SZ
      REAL ZOOMXY
      INTEGER ETYPE,TRKHLD
      INTEGER NCDXTS,NBOUNDS,NBNDSEG
      REAL RESMIN,RESMAX,RESDIF
      REAL RADMIN,RADMAX,RADDIF
      REAL XAPOSL,XAPOSR,XAPOSB,XAPOST
      REAL YAPOSL,YAPOSR,YAPOSB,YAPOST
      REAL RESSAV,RADSAV



      COMMON/XTS_STATUS /ENTRFV
      COMMON/XTS_ADRESS /CLZERO,CLSTRO,CLHITM,CLNORM,CLCHAN,
     2                   CLUSED,CLDUMB,CLLET1,CLHADC,CLDUSE,
     3                   CLDUCH,CLDADJ,CLCLUS,
     4                   CLLET2,CLBORD,CLSUBB,
     5                   COLRHT(MSEND),COLORH(MCD),LABCHO(MAXCHO),
     6                   CLHLD(MIDHLD)
      COMMON/XTS_SENDI  /NSEND,IDHIT(MSEND)
      COMMON/XTS_NAMEI  /WKID
      COMMON/XTS_NAMER  /MAXSIZ
      COMMON/XTS_NAMEC  /DISPLAY,BANNER
      COMMON/XTS_GEOMTR /INXSCD(MLAY_CD),INXSSF(MLAY_SF)
      COMMON/XTS_CNTRSR /
     1                   RBOUNDS(MBOUNDS),
     2                   XYBSEG(2,2,MBNDSEG),
     3                   XCENTRS(MCD),YCENTRS(MCD),
     4                   SIZHIT(MCD),XCORNR(6,MCD),YCORNR(6,MCD)
      COMMON/XTS_CNTRSI /
     1     TYP_BOUND(MBOUNDS)
      COMMON/XTS_OUTSIDI/LEFTSF
      COMMON/XTS_CHOICEI/NCHOIC,CHDEPEND(MAXCHO),
     2   LAZOOM_DRAW,LAZOOM,LAFORWARD,LABACKUP,LAFULLSIZE,
     3   LAREDRAW,LANEW_DRAW, LAINFO,LA,LAID_TDC_ADC,LAID_MULTIH,
     4   LAIC_CCC,
     5   LADISTANCE,LAWIR_DUET,LANEXT_HIT,
     6   LAINUCHN,LAISTFOR,LADSFDCH,LADSFDMP,
     7   LACOLOR,LALOOP_1,LALOOP_2,
     8   LASKIP,LANEXT_EVENT,LANEXT_PASS,
     9   LAANSWER,LAYES,LANO,LAALWAYS,
     x   LAOTHER,LAOTHER2,LAKILL,LAMORE,LACR,LAZHIT,LARESI
      COMMON/XTS_CHOICEC/NAMCHO(MAXCHO)
      COMMON/XTS_HISTI  /SHOWCH(MCHAIN),NIDHLD,IDHLD(MIDHLD)
      COMMON/XTS_ZOOMI  /ZOOMU,ZOOMIT(3,MZOOM),
     2                   ZOOMU_RPHI,ZOOMU_TFXF(2),ZOOMU_SZ
      COMMON/XTS_ZOOMR  /ZOOMXY(4,MZOOM)
      COMMON/XTS_TRAKI  /ETYPE,TRKHLD
      COMMON/XTS_CDGEOMI/NCDXTS,NBOUNDS,NBNDSEG
      COMMON/XTS_TFXF_R/ RESMIN,RESMAX,RESDIF,
     2                   RADMIN,RADMAX,RADDIF,
     3                   XAPOSL,XAPOSR,XAPOSB,XAPOST,
     4                   YAPOSL,YAPOSR,YAPOSB,YAPOST,
     5                   RESSAV(M_XTS_HITS),RADSAV(M_XTS_HITS)




