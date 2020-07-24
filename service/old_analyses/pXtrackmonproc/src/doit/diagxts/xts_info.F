*
* $Id: xts_info.F,v 1.11 2000/08/18 18:40:59 dpp Exp $
*
* $Log: xts_info.F,v $
* Revision 1.11  2000/08/18 18:40:59  dpp
*      -> implement new choice: multi-hit
*
* Revision 1.10  2000/05/26 16:50:24  dpp
*      -> get pulse height in happy units from sf_data_access instead of QWCD
*
* Revision 1.9  1999/11/19 22:59:19  dpp
* remove ZFILES pointer requirement
*
* Revision 1.8  1999/10/08 13:41:36  lyon
* Fixed SunOS directives and fixed CALLs to functions
*
* Revision 1.7  1999/01/29 15:34:40  dpp
* force update
*
* Revision 1.6  1999/01/11 02:46:50  lyon
* SunOS does not allow CALLing functions
*
* Revision 1.5  1998/07/21 15:02:36  lyon
* Protect against including cdscrtcd.inc twice
*
* Revision 1.4  1998/07/09 13:01:35  lyon
* Added include cdscrtcd.inc
*
* Revision 1.3  1998/07/09 01:23:29  lyon
* Retrofitted for sf_data_access.inc
*
* Revision 1.2  1998/05/08 14:13:48  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.3  1997/02/25 21:39:38  dpp
* also display SF layers that are not CD layers
*
* Revision 1.2  1995/04/20 22:38:57  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/18 22/03/94  14.46.00  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.40  by  Dan Peterson
*-- Author :dpp

      SUBROUTINE XTS_INFO(NINFO,CALLER)
      IMPLICIT NONE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------

#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

#ifndef CLEO_SFNOZFILES
#include "doit/duseq/cdscrtcd.inc"
#endif

#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

      INTEGER SET_FOREGROUND_C

      CHARACTER*(*) CALLER
      INTEGER NINFO

      INTEGER XJUST0,YJUST0,XJUST,YJUST,YJDIFF

      REAL YHUSER/.0015/
      REAL    YH_USE_TEMP, YH_USE
      INTEGER IDUM
      INTEGER I
      INTEGER ADRXT,LAYRXT
      INTEGER ADRCD,LAYRCD,IADLOW,IADRHI,L,WIRE,NWIRE,RDOUT,NRDOUT
      INTEGER ADRSF,LAYRSF
      INTEGER ADRXX,LAYRXX
      INTEGER       LAYRDR
      REAL PI,SCALE
      INTEGER ICH,IADRI,TSTCEL
      REAL XC,YC
      INTEGER IERR
      CHARACTER*6 STRING
      INTEGER IHITZ,IHITX,IHITD
      INTEGER ITIME,IPULS
      INTEGER IPREAM,ICRATE,ICARD,ITB,JTB
      REAL TIME,PHINOW
      INTEGER IDRIFT,IPULSE,LOGPHI,IEXPL
      INTEGER IEARLY,ILATE

#if defined(CLEO_ULTRIX)
      INTEGER COLREQ(14)
#else
C*****Alpha and SunOS      
      INTEGER*8 COLREQ(14)
#endif

      EQUIVALENCE (COLREQ(1),CLZERO)
      CHARACTER*6 COLORE(2,15)

      DATA COLORE/
     1 'ZERO  ','      ','STEREO','      ','HIGH  ','TIME  ',
     2 'NORMAL','USABLE','CURRNT','CHAIN ','USED  ','CHAIN ',
     3 'DUMB, ','TRAK  ','LET1  ','      ','HIGH  ','ADC   ',
     4 'USEDBY','DUET  ','UDUET,','TFHITS','ADJCNT','DUET  ',
     5 'LET2  ','      ','BORDER','      ','SOURCE','UNKNON'/

      DATA PI/3.14159265/



#if defined(CLEO_XTSDEBUG)
      PRINT 1005,NINFO,NSEND,CALLER
1005  FORMAT(' XTS_INFO: INFO#',I4,'  HAVE',I4,' HITS..',
     1       ' CALLED BY ',A20)
#endif

C     text position will specify center the text horizontally
C     and the                    center the text vertically

      XJUST0=0
#if defined(CLEO_ULTRIX)
c      YJDIFF=2
c      YJUST0=-2
      YJDIFF=3
      YJUST0=-4
#endif
#if defined(CLEO_AXPOSF)
      YJDIFF=3
      YJUST0=-5
#endif
#if defined(CLEO_SUNOS)
      YJDIFF=3
      YJUST0=-5
#endif

C set the character height, may be reset later in the loop
      YH_USE_TEMP=YHUSER*MAXSIZ/ZOOMXY(2,ZOOMU)

C this is the start of the loop over the passed cells
      DO 999 I=1,NSEND

C get geometry index for cell
      ADRXT = IDHIT(I)

C only display INFO for cells in the window

      IF (XCENTRS(ADRXT).LT.ZOOMXY(3,ZOOMU))GO TO 994
      IF (XCENTRS(ADRXT).GT.ZOOMXY(3,ZOOMU)+ZOOMXY(1,ZOOMU))GO TO 994
      IF (YCENTRS(ADRXT).LT.ZOOMXY(4,ZOOMU))GO TO 994
      IF (YCENTRS(ADRXT).GT.ZOOMXY(4,ZOOMU)+ZOOMXY(2,ZOOMU))GO TO 994

C find the highest index address </= than the input address
C also find the lowest index address > than the input address
C loop to find CD layer number

      LAYRCD=0
      LAYRSF=0
      LAYRXX=0
      IADLOW=0
      IADRHI=NCDXTS+1
      DO 51 L=LCANCD,MCANCD
      IF(INXSCD(L).LE.ADRXT)THEN
         IF(INXSCD(L).GE.IADLOW)THEN
            IADLOW=INXSCD(L)
            LAYRCD=L
            NWIRE=NWIRCD(LAYRCD)
            LAYRSF=SFLACD(L)
         ENDIF
      ELSE
         IF(INXSCD(L).LT.IADRHI)THEN
            IADRHI=INXSCD(L)
         ENDIF
      ENDIF
51    CONTINUE

C now loop over SF layer looking for not CD layers
C CDLASF(for these layers) will be negative
      IF(NLAYSF.LE.0)GO TO 55
      DO 53 L=1,NLAYSF
      IF(CDLASF(L).GT.0)GO TO 53
      IF(INXSSF(L).LE.ADRXT)THEN
         IF(INXSSF(L).GE.IADLOW)THEN
            IADLOW=INXSSF(L)
            LAYRSF=L
            NWIRE=NWRSF(LAYRSF)
            LAYRCD=CDLASF(L)
            LAYRXX=-CDLASF(L)
         ENDIF
      ELSE
         IF(INXSSF(L).LT.IADRHI)THEN
            IADRHI=INXSSF(L)
         ENDIF
      ENDIF
53    CONTINUE

55    WIRE=ADRXT-IADLOW

C     ADRXT is input
      ADRSF=0
      IF(LAYRSF.GT.0)ADRSF=WIRE+INSFSF(LAYRSF)
      ADRCD=0
      IF(LAYRCD.GT.0)ADRCD=WIRE+INDXCD(LAYRCD)
      ADRXX=0
      IF(LAYRXX.GT.0)ADRXX=WIRE+SFSVIN(LAYRXX,1)

      NRDOUT=(IADRHI-IADLOW+NWIRE-1)/NWIRE
      RDOUT=1
57    CONTINUE
      IF(WIRE.GE.NWIRE)THEN
         WIRE=WIRE-NWIRE
         RDOUT=RDOUT+1
         GO TO 57
      ENDIF

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      print 9901,i,ADRXT,LAYRSF,ADRSF,LAYRCD,ADRCD,LAYRXX,ADRXX
 9901 format(' cell ',i4,' fiducial',
     2 ' Axts=',i5,          ' Lsf=',i2,' Asf=',i5,
     3 ' Lcd=',i2,' Acd=',i5,' Lxx=',i2,' Axx=',i5)


C adjustable YH for the small cells
      IF(LAYRCD.GT.0)SCALE=CELLCD(LAYRCD)*RCD(LAYRCD)/.014
      IF(LAYRXX.GT.0)SCALE=2.*PI/SFSVCL(LAYRXX)*SFSVRD(LAYRXX)/.014
      YH_USE=YH_USE_TEMP*SCALE

C set the text color index for dark background or...
C check if this ADRCD is on one of the light colored chains
      IF(ADRSF.LE.0)GO TO 78
      DO 77 ICH=1,MCHAIN
      IF(SHOWCH(ICH).EQ.-1)GO TO 77
      IF(SHOWCH(ICH).EQ.0)GO TO 71
      IF(SHOWCH(ICH).EQ.3)GO TO 71
      GO TO 77

71    IADRI=LINCHN(ICH)
72    TSTCEL=IADRI
73    IF(TSTCEL.EQ.0)GO TO 77
      IF(ADRSF.EQ.TSTCEL)GO TO 88
      IF(DOUBPT(TSTCEL).EQ.0)GO TO 75
      TSTCEL=DOUBPT(TSTCEL)
      GO TO 73
75    IF(IADRI.EQ.MINCHN(ICH))GO TO 77
      IADRI=ISTFOR(IADRI)
      GO TO 72
77    CONTINUE

78    CONTINUE

C check if this ADRCD is on a displayed  doit track

      IF(ADRCD.LE.0)GO TO 82
      IF(NIDHLD.LE.0)GO TO 82
      DO 81 L=1,NIDHLD
      IF(ADRXT.EQ.IDHLD(L))GO TO 88
81    CONTINUE
82    CONTINUE

87    IDUM=SET_FOREGROUND_C(CLLET1)
      GO TO 95

88    IDUM=SET_FOREGROUND_C(CLLET2)

C x center and y base of cell

95    XC = XCENTRS(ADRXT)
      YC = YCENTRS(ADRXT)
      XJUST=XJUST0
      YJUST=YJUST0

C write the cell's ID

      IF(ADRCD.GT.0)THEN
            WRITE(STRING,1001,IOSTAT=IERR) ADRCD
1001        FORMAT('c',I5)
      ELSEIF(ADRXX.GT.0)THEN
            WRITE(STRING,1002,IOSTAT=IERR) ADRXX
1002        FORMAT('x',I5)
      ENDIF
      L=LEN(STRING)
      CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)

C get the hit number in ZFILES

      IHITZ=0
      IF(ADRCD.GT.0)THEN
         IHITX=0

         sf_data_input_cd_adr=ADRCD
#define SF_DATA_MAPV_CD
#include "doit/sf_data_access/sf_data_access.inc"
         IHITZ=sf_data_output_hit_adr

         IF(
     1        (IHITZ.LT.1).OR.
     2        (IHITZ.GT.NHTWCD))THEN
            YJUST=YJUST+YJDIFF
            WRITE(STRING,1009,IOSTAT=IERR)IHITZ
 1009       FORMAT('h',I5)
            L=LEN(STRING)
            CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
            GO TO 995
         ENDIF

      ELSEIF(ADRXX.GT.0)THEN
         IHITX=SFSVMP(ADRXX)
      ENDIF

C get the hit number in DUET
      IHITD=0
      IHITD=IABS(IADRHW(ADRCD))

C END OF WRITING THE CELL ID, THIS PART WAS COMMON TO ALL INFO

      IF(NINFO.EQ.1)GO TO 111
      IF(NINFO.EQ.2)GO TO 211
      IF(NINFO.EQ.3)GO TO 311
      IF(NINFO.EQ.4)GO TO 411
      IF(NINFO.EQ.5)GO TO 511
      IF(NINFO.EQ.6)GO TO 611
      IF(NINFO.EQ.7)GO TO 711
      IF(NINFO.EQ.8)GO TO 811


C THIS IS NINFO=1
C write the cell's raw TDC
C write the cell's raw ADC

111      YJUST=YJUST+YJDIFF

#ifdef CLEO_SFNOZFILES
         IF(IHITZ.LE.0)THEN
#else
         IF((IHITZ.LE.0).OR.(IPTDCD.LE.0))THEN
#endif

               WRITE(STRING,1003,IOSTAT=IERR)
         ELSE

      sf_data_input_hit_adr=IHITZ
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      ITIME=sf_data_output_tdc_raw

               WRITE(STRING,1008,IOSTAT=IERR) ITIME
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
C write the cell's PH,  RAW ADC
         YJUST=YJUST+YJDIFF

#ifdef CLEO_SFNOZFILES
         IF(IHITZ.LE.0)THEN
#else
         IF((IHITZ.LE.0).OR.(IPTDCD.LE.0))THEN
#endif

               WRITE(STRING,1004,IOSTAT=IERR)
         ELSE

      sf_data_input_hit_adr=IHITZ
#define SF_DATA_RADC
#include "doit/sf_data_access/sf_data_access.inc"
      IPULS=sf_data_output_adc_raw

               WRITE(STRING,1007,IOSTAT=IERR) IPULS
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
      GO TO 999
1003  FORMAT('NO TDC')
1004  FORMAT('NO ADC')
1008  FORMAT('T',I5)
1007  FORMAT('A',I5)

C THIS IS NINFO=2
C write the cell's CRATE ADDRESS

211   YJUST=YJUST+YJDIFF
#if defined(CLEO_SFDIAG)
      IF(LAYRCD.LE.LCANDR)THEN
            WRITE(STRING,2003,IOSTAT=IERR)
      ELSE
            LAYRDR=LAYRCD-LCANDR+1
            IPREAM=(WIRE+24)/24
            CALL SF_PREAMP_TO_CRATE(LAYRDR,IPREAM,ICRATE,ICARD,ITB)
            WRITE(STRING,2001,IOSTAT=IERR)ICRATE
            L=LEN(STRING)
            CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
            YJUST=YJUST+YJDIFF
            JTB='T'
            IF(ITB.EQ.2)JTB='B'
            WRITE(STRING,2002,IOSTAT=IERR)ICARD,JTB
      ENDIF
      L=LEN(STRING)
      CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
#endif
295   CONTINUE
      GO TO 999
2001  FORMAT('"',I2)
2002  FORMAT(I2,'_',A1)
2003  FORMAT('NOCC')


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C THIS IS NINFO=3
C write the SF ADDRESS
C write the cell's INUCHN
C write the cell's DOUBPT

311      YJUST=YJUST+YJDIFF
         IF(ADRSF.EQ.0)THEN
               WRITE(STRING,3004,IOSTAT=IERR)
         ELSE         
               WRITE(STRING,3001,IOSTAT=IERR)ADRSF
               L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
               YJUST=YJUST+YJDIFF
               WRITE(STRING,3002,IOSTAT=IERR)INUCHN(ADRSF)
               L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
               YJUST=YJUST+YJDIFF
               IF((DOUBPT(ADRSF).GT.9999).OR.
     2            (DOUBPT(ADRSF).LT.-999))THEN
                     WRITE(STRING,3013,IOSTAT=IERR)DOUBPT(ADRSF)
               ELSE
                     WRITE(STRING,3003,IOSTAT=IERR)DOUBPT(ADRSF)
               ENDIF
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
      GO TO 999
3001  FORMAT('s',I5)
3002  FORMAT('U ',I3)
3003  FORMAT('D',I4)
3013  FORMAT(I6)
3004  FORMAT('NOT SF')

C THIS IS NINFO=4
C write the cell's TIME in ns
C write the DISTANCE in microns
C write the cell's PULSE HEIGHT in happy units

411      YJUST=YJUST+YJDIFF
         IF(ADRCD.EQ.0)THEN
               WRITE(STRING,4005,IOSTAT=IERR)
         ELSE

      sf_data_input_hit_adr=IHITZ
#define SF_DATA_TMPS
#include "doit/sf_data_access/sf_data_access.inc"
      TIME=sf_data_output_r_tim_ps

               ITIME=TIME/1000
C               ITIME=ITIME/10
               IF(ITIME.LT.-9999)ITIME=-9999
               IF(ITIME.GT.99999)ITIME=99999
               WRITE(STRING,4001,IOSTAT=IERR) ITIME
               L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)

C write the cell's DRIFT DISTANCE
               YJUST=YJUST+YJDIFF
               IF(IHITD.EQ.0)THEN
                     WRITE(STRING,4004,IOSTAT=IERR)
               ELSE
                     IDRIFT=DWCD(IHITD)*1000000.
                     IF(IDRIFT.LT.-999)IDRIFT=-999
                     IF(IDRIFT.GT.9999)IDRIFT=9999
                     WRITE(STRING,4002,IOSTAT=IERR)IDRIFT
               ENDIF
               L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
C write the cell's PULSE HEIGHT
               YJUST=YJUST+YJDIFF


C      sf_data_input_hit_adr=IHITZ
C#define SF_DATA_RADC
C#include "doit/sf_data_access/sf_data_access.inc"
C      IPULSE=sf_data_output_adc_raw

C               IPULSE=QWCD(IHITD)

      sf_data_input_hit_adr=IHITZ
#define SF_DATA_ADCH
#include "doit/sf_data_access/sf_data_access.inc"
      IPULSE=sf_data_output_ph_happy


               IF(IPULSE.LT.-999)IPULSE=-999
               IF(IPULSE.GT.9999)IPULSE=9999
               WRITE(STRING,4003,IOSTAT=IERR)IPULSE
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
      GO TO 999
4001  FORMAT('n', I5)
4002  FORMAT('D ',I4)
4003  FORMAT('P ',I4)
4004  FORMAT('NODUET')
4005  FORMAT('NOT CD')

C THIS IS NINFO=5
C write the SF ADDRESS
C write the cell's ISTFOR
C write the cell's ISTFOR

511      YJUST=YJUST+YJDIFF
         IF(ADRSF.EQ.0)THEN
               WRITE(STRING,5004,IOSTAT=IERR)
         ELSE
               WRITE(STRING,5001,IOSTAT=IERR)ADRSF
               L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
               YJUST=YJUST+YJDIFF
               WRITE(STRING,5002,IOSTAT=IERR)ISTFOR(ADRSF)
               L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
               YJUST=YJUST+YJDIFF
               WRITE(STRING,5003,IOSTAT=IERR)ISTBAK(ADRSF)
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
      GO TO 999
5001  FORMAT('s',I5)
5002  FORMAT('F',I5)
5003  FORMAT('B',I5)
5004  FORMAT('NOT SF')

C THIS IS NINFO=6
C write the cell's LAYER NUMBER
C write the cell's WIRE NUMBER
C write the cell's DUET HIT NUMBER

611      YJUST=YJUST+YJDIFF
         IF(ADRCD.GT.0)THEN
               WRITE(STRING,6006,IOSTAT=IERR)LAYRCD
         ELSEIF(ADRXX.GT.0)THEN
               WRITE(STRING,6008,IOSTAT=IERR)LAYRXX
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
C write the cell's WIRE NUMBER
         YJUST=YJUST+YJDIFF
         IF(NRDOUT.EQ.1)THEN
            WRITE(STRING,6003,IOSTAT=IERR)WIRE
         ELSE
            WRITE(STRING,6004,IOSTAT=IERR)RDOUT,WIRE
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
C write the cell's DUET HIT NUMBER
         YJUST=YJUST+YJDIFF
         IF(IHITD.EQ.0)THEN
               WRITE(STRING,6007,IOSTAT=IERR)
         ELSE
               WRITE(STRING,6001,IOSTAT=IERR)IHITD
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
      GO TO 999
6006  FORMAT('Lcd',I3)
6008  FORMAT('Lxx',I3)
6003  FORMAT('WR',I4)
6004  FORMAT('W',I1,I4)
6001  FORMAT('DH',I4)
6007  FORMAT('NODUET')

C THIS IS NINFO=7
C  explain the cell's COLOR

711      YJUST=YJUST+YJDIFF
         IEXPL=0
721      IEXPL=IEXPL+1
         IF(IEXPL.EQ.15)GO TO 725
         IF(COLORH(ADRXT).EQ.COLREQ(IEXPL))GO TO 725
         GO TO 721
725      WRITE(STRING,7001,IOSTAT=IERR)COLORE(1,IEXPL)
7001  FORMAT(A6)
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
         YJUST=YJUST+YJDIFF
         WRITE(STRING,7001,IOSTAT=IERR)COLORE(2,IEXPL)
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
      GO TO 999

C THIS IS NINFO=8
C write the number of hits in the cell
C write the time in ns to the next previous hit
C write the time in ns to the next later hit

 811  YJUST=YJUST+YJDIFF
      IF(ADRCD.EQ.0)THEN
         WRITE(STRING,8005,IOSTAT=IERR)
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
      ELSE
         
C write the number of hits in the cell
         IF(IHITD.EQ.0)THEN
            WRITE(STRING,8004,IOSTAT=IERR)
         ELSE
            WRITE(STRING,8001,IOSTAT=IERR)NTDCCD(IHITD)
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)
         
C write the time in ns to the next previous hit
         YJUST=YJUST+YJDIFF
         IF(EARLYCD(IHITD).EQ.-1)THEN
            WRITE(STRING,8006,IOSTAT=IERR)
         ELSE
            IEARLY=EARLYCD(IHITD)/1000
            IF(IEARLY.LT.-9999)IEARLY=-9999
            IF(IEARLY.GT.99999)IEARLY=99999
            WRITE(STRING,8002,IOSTAT=IERR)IEARLY
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)

C write the time in ns to the next later hit
         YJUST=YJUST+YJDIFF
         IF(LATECD(IHITD).EQ.-1)THEN
            WRITE(STRING,8007,IOSTAT=IERR)
         ELSE
            ILATE=LATECD(IHITD)/1000
            IF(ILATE.LT.-9999)ILATE=-9999
            IF(ILATE.GT.99999)ILATE=99999
            WRITE(STRING,8003,IOSTAT=IERR)ILATE
         ENDIF
         L=LEN(STRING)
         CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YH_USE,STRING(1:L),0.)

      ENDIF
      GO TO 999
 8001 FORMAT('h'  ,I5)
 8006 FORMAT('e:none')
 8002 FORMAT('e'  ,I5)
 8007 FORMAT('l:none')
 8003 FORMAT('l'  ,I5)
 8004 FORMAT('NODUET')
 8005 FORMAT('NOT CD')


C EXIT FOR POSITION OUT OF BOUNDS
994   CONTINUE
      GO TO 999

C EXIT FOR ILLEGAL HIT NUMBER
995   CONTINUE
      GO TO 999

C END OF LOOP OVER DISPLAYED CELLS
999   CONTINUE

#if defined(CLEO_XTSDEBUG)
      PRINT 1006
1006  FORMAT(' XTS_INFO: EXIT')
#endif

      RETURN
      END
