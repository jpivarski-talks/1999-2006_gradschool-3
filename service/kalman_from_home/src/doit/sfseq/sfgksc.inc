*
* $Id: sfgksc.inc,v 1.2 2001/11/20 17:51:37 dpp Exp $
*
* $Log: sfgksc.inc,v $
* Revision 1.2  2001/11/20 17:51:37  dpp
*      -> delete stupid useless nonexistent include file
*      -> merge in documentation
*
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:11  zfiles
* first version of doit in CVS
*
*
* COMMON BLOCKS FOR SF GKS ROUTINES
*
* sfgksc.inc
*
#if !defined(CLEO_NOCOMDOC)
C CLZERO
C CLBLAK...THIS IS WHITE SINCE I LIKE REVERSE
C CLWHIT...THIS IS BLACK SINCE I LIKE REVERSE
C CLDUMB...
C CLHITM...HIGH TIME HITS
C CLNORM...NORMAL GOOD HITS THAT ARE AVAILABLE FOR NEW CHAINS
C CLCHAN...NORMAL HITS THAT ARE IN THE CURRENT DISPLAYED CHAIN
C CLUSED...USED IN PREVIOUS CHAINS, NOT AVAILABLE
C CLSTRO...STEREO
C CLHADC...HIGH ADC (MEANING LOW CHARGE)
C CLDUSE...ALREADY USED BY DUET
C CLDUCH...ALREADY USED BY DUET BUT ASSIGNED TO THE CURRENT CHAIN
C CLDIAG...USED TO DISPLAY HITS RELEVENT TO DIAGNOSTIC
C..
C MAPSEG...ELEMENT NUMBER (USER ELEMENT)  GIVES GKS SEG NUMBER
C JWASAC...LIST OF WORK STATIONS FOUND ACTIVE, TERMINATE WITH ZERO
C         . WILL BE NEGATIVE IF OPEN BUT NOT ACTIVE
C SEGOPN...0;  OR name IF THERE WAS AN OPEN SEGMENT ON ENTERING GKS
C SEGOPR...PRIORITY OF THAT OLD OPEN SEGMENT
C LEFTSF...FLAG=0;  OR 1 IF SF GKS HAS BEEN LEFT THROUGH GKS_LEAV
C ITRFND...TRANSFORMATION NUMBER FOUND IN USE, LEFT FROM DUET
C ITRNSF...TRANSFORMATION NUMBER TO BE USED BY SF
#endif
      INTEGER COLRHT,IDHIT
      INTEGER GRID,DFALT,INFO,CRRNT
      INTEGER CHANON,JCHANO
      REAL PGRID,PDFALT,PINFO,PCRRNT
      REAL PCHANO
      INTEGER CLZERO,CLSTRO,CLHITM,CLNORM,CLCHAN,CLUSED
      INTEGER CLDUMB,CLBLAK,CLHADC,CLDUSE,CLDUCH,CLDIAG
      INTEGER CLWHIT
      INTEGER WKID
      CHARACTER*6 DSPNOD
      CHARACTER*10 TITLE
      REAL XHI,XLO,YHI,YLO
      REAL RBOUNDS,XCENTRS,YCENTRS,SIZHIT
      INTEGER MAPSEG,JWASAC
      INTEGER SEGOPN,LEFTSF,TRNFND,TRANSF
      REAL SEGOPR
      COMMON/CD_COLOR/CLZERO,CLSTRO,CLHITM,CLNORM,CLCHAN,
     1                CLUSED,CLDUMB,CLBLAK,CLHADC,CLDUSE,
     2                CLDUCH,CLDIAG,CLWHIT,
     3                COLRHT(MSEND),IDHIT(MSEND)
      COMMON/CD_SEGMT/GRID,DFALT,INFO,CRRNT,
     2                CHANON(MCHAIN,2),JCHANO(MCHAIN),
     3                PGRID,PDFALT,PINFO,PCRRNT,
     4                PCHANO(MCHAIN,2)
      COMMON/CD_SSIZE/WKID,DSPNOD,TITLE,XLO,XHI,YLO,YHI
      COMMON/CD_CNTRS/RBOUNDS(6),XCENTRS(MCD),YCENTRS(MCD),
     1                SIZHIT(MCD)
      COMMON/CD_STORD/MAPSEG(256),JWASAC(10),SEGOPN,SEGOPR,LEFTSF,
     1                TRNFND,TRANSF
CVAX      INCLUDE 'SYS$LIBRARY:GKSDEFS.BND'
      INCLUDE '/usr/include/GKS3D/GKS3Dforbnd.f'















