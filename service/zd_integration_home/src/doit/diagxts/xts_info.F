*
* $Id: xts_info.F,v 1.14 2003/05/12 16:06:31 dpp Exp $
*
* $Log: xts_info.F,v $
* Revision 1.14  2003/05/12 16:06:31  dpp
*      -> remove use of CLEO_SunOS
*
* Revision 1.13  2003/03/06 16:59:25  dpp
*      -> changed compile switch from CLEO_SUNOS to CLEO_SunOS
*
* Revision 1.12  2003/03/05 17:27:36  dpp
*      -> remove zfiles stuff and use of SFNOZFILES switch
*      -> increase field width for cell id, doit hit,
*      -> define info option for 'next hit'
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.11  2000/08/18 18:40:59  dpp
*      -> implement new choice: multi-hit
*
* Revision 1.10  2000/05/26 16:50:24  dpp
*      -> get pulse height in happy units from sf_data_access
*         instead of EvntHit_Charge
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
* Developmental version of DOIT.
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

C----------
C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"
C----------

#include "doit/duseq/controlpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"


      INTEGER SET_FOREGROUND_C

      CHARACTER*(*) CALLER
      INTEGER NINFO

      INTEGER XJUST0,YJUST0,XJUST,YJUST,YJDIFF

      REAL YHUSER/.0015/
      REAL    YH_USE_TEMP, YHU
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
      CHARACTER*7 STRING7
      CHARACTER*29 STRING29
      CHARACTER*1 FLAG
      INTEGER IHITD,IHITX,IHIT_LOOP
      INTEGER ITIME,IPULS
      INTEGER IPREAM,ICRATE,ICARD,ITB,JTB
      REAL TIME,PHINOW
      REAL RDRIFT
      INTEGER IDRIFT,IPULSE,LOGPHI,IEXPL
      INTEGER IEARLY,ILATE,NHSAMECELL
      INTEGER NXTHCLUS,PRIHCLUS,NXTHMULT
      LOGICAL USE_CLLET2
      LOGICAL VALID_HIT

      INTEGER NEXPL
      PARAMETER (NEXPL=15)
      INTEGER NEXPLP1
      PARAMETER (NEXPLP1=NEXPL+1)
      
#if defined(CLEO_ULTRIX)
      INTEGER COLREQ(NEXPL)
#else
      INTEGER*8 COLREQ(NEXPL)
#endif

      EQUIVALENCE (COLREQ(1),CLZERO)
      CHARACTER*6 COLORE(2,NEXPLP1)

      DATA COLORE/
     1     'ZERO  ','      ',
     2     'STEREO','      ',
     4     'HIGH  ','TIME  ',
     2     'NORMAL','USABLE',
     5     'CURRNT','CHAIN ',
     6     'USED  ','CHAIN ',
     7     'DUMB, ','TRAK  ',
     8     'LET1  ','      ',
     9     'HIGH  ','ADC   ',
     x     'USEDBY','DOIT  ',
     1     'UDOIT,','TFHITS',
     2     'ADJCNT','DOIT  ',
     3     'clustr','w/usbl',
     4     'LET2  ','      ',
     5     'BORDER','      ',
     6     'SOURCE','UNKNON'/

      LOGICAL L_XTS_DOIT_TPC

      DATA PI/3.14159265/


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C-----------------------------------------------------------------------
C executable starts here
C-----------------------------------------------------------------------

      CALL EVENT_CONTROL_BITS(EXB_DOIT_TPC,L_XTS_DOIT_TPC)


#if defined(CLEO_XTSDEBUG)
      PRINT 1005,NINFO,NSEND,CALLER
 1005 FORMAT(' XTS_INFO: INFO#',I4,'  HAVE',I4,' HITS..',
     1     ' CALLED BY ',A20)
#endif
      
C text position will specify center the text horizontally
C and the                    center the text vertically

      XJUST0=0
#if defined(CLEO_ULTRIX)
      YJDIFF=3
      YJUST0=-4
#else
      YJDIFF=3
      YJUST0=-5
#endif
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      
C-----------------------------------------------------------------------
C set the character height, may be reset later in the loop
C-----------------------------------------------------------------------
      
      YH_USE_TEMP=YHUSER*MAXSIZ/ZOOMXY(2,ZOOMU)
      
C this is the start of the loop over the passed cells
      DO 999 I=1,NSEND
        
C get geometry index for cell
        ADRXT = IDHIT(I)
        
C only display INFO for cells in the window
        
        IF(
     1       (XCENTRS(ADRXT).GE.ZOOMXY(3,ZOOMU)).AND.
     2       (XCENTRS(ADRXT).LE.ZOOMXY(3,ZOOMU)+ZOOMXY(1,ZOOMU)).AND.
     3       (YCENTRS(ADRXT).GE.ZOOMXY(4,ZOOMU)).AND.
     4       (YCENTRS(ADRXT).LE.ZOOMXY(4,ZOOMU)+ZOOMXY(2,ZOOMU))
     5       )THEN
          
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
 51       CONTINUE
          
C now loop over SF layer looking for not CD layers
C CDLASF(for these layers) will be negative
          
          IF(NLAYSF.GT.0)THEN
            DO 53 L=1,NLAYSF
              IF(CDLASF(L).LE.0)THEN
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
              ENDIF
 53         CONTINUE
          ENDIF
          
          WIRE=ADRXT-IADLOW
          
C     ADRXT is input
          ADRSF=0
          IF(LAYRSF.GT.0)ADRSF=WIRE+INSFSF(LAYRSF)
          ADRCD=0
          IF(LAYRCD.GT.0)ADRCD=WIRE+INDXCD(LAYRCD)
          ADRXX=0
          IF(LAYRXX.GT.0)ADRXX=WIRE+SFSVIN(LAYRXX,1)
          
          NRDOUT=(IADRHI-IADLOW+NWIRE-1)/NWIRE
          RDOUT=1
 57       CONTINUE
          IF(WIRE.GE.NWIRE)THEN
            WIRE=WIRE-NWIRE
            RDOUT=RDOUT+1
            GO TO 57
          ENDIF
          
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
          
        
C adjustable YH for the small cells
          
          IF(LAYRCD.GT.0)SCALE=CELLCD(LAYRCD)*RCD(LAYRCD)/.014
          IF(LAYRXX.GT.0)SCALE=2.*PI/SFSVCL(LAYRXX)*SFSVRD(LAYRXX)/.014
          YHU=YH_USE_TEMP*SCALE
          
C set the text color index for dark background or...
C check if this ADRSF is on one of the light colored chains
          
          USE_CLLET2=.FALSE.
          
          IF(ADRSF.GT.0)THEN
C loop over chains
            ICH=1
 71         CONTINUE
            IF(ICH.LE.MCHAIN)THEN
              IF(
     1             (SHOWCH(ICH).EQ.0).OR.
     2             (SHOWCH(ICH).EQ.3))THEN
                
C loop over the elements of the chain link-list
                IADRI=LINCHN(ICH)
 72             TSTCEL=IADRI
C restart to test double hits
 73             CONTINUE
                IF(TSTCEL.NE.0)THEN
                  IF(ADRSF.EQ.TSTCEL)THEN
                    USE_CLLET2=.TRUE.
                  ENDIF
                  IF((.NOT.USE_CLLET2).AND.(DOUBPT(TSTCEL).NE.0))THEN
                    TSTCEL=DOUBPT(TSTCEL)
                    GO TO 73
                  ENDIF
                  IF((.NOT.USE_CLLET2).AND.(IADRI.NE.MINCHN(ICH)))THEN
                    IADRI=ISTFOR(IADRI)
                    GO TO 72
                  ENDIF
                ENDIF
              ENDIF
              IF((.NOT.USE_CLLET2))THEN
                ICH=ICH+1
                GO TO 71
              ENDIF
            ENDIF
          ENDIF
          
C check if this ADRCD is on a displayed  doit track
          
          IF((.NOT.USE_CLLET2))THEN
            IF(ADRCD.GT.0)THEN
              L=1
 81           CONTINUE
              IF((.NOT.USE_CLLET2).AND.(L.LE.NIDHLD))THEN
                IF(ADRXT.EQ.IDHLD(L))THEN
                  USE_CLLET2=.TRUE.
                ENDIF
                L=L+1
                GO TO 81
              ENDIF
            ENDIF
          ENDIF
          
          IF(USE_CLLET2)THEN
            IDUM=SET_FOREGROUND_C(CLLET2)
          ELSE
            IDUM=SET_FOREGROUND_C(CLLET1)
          ENDIF
          
C x center and y base of cell
          
          XC = XCENTRS(ADRXT)
          YC = YCENTRS(ADRXT)
          XJUST=XJUST0
          YJUST=YJUST0
          
C-----------------------------------------------------------------------
C write the cell's ID
C-----------------------------------------------------------------------
          
          IF(ADRCD.GT.0)THEN
            WRITE(STRING7,1001,IOSTAT=IERR) ADRCD
 1001       FORMAT('c',I6)
          ELSEIF(ADRXX.GT.0)THEN
            WRITE(STRING7,1002,IOSTAT=IERR) ADRXX
 1002       FORMAT('x',I5)
          ENDIF
          L=LEN(STRING7)
          CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING7(1:L),0.)
          YJUST=YJUST+YJDIFF
            
C-----------------------------------------------------------------------
C END OF WRITING THE CELL ID, THIS PART WAS COMMON TO ALL INFO
C-----------------------------------------------------------------------
          
C-----------------------------------------------------------------------
C get the hit number from sf_data_access
C-----------------------------------------------------------------------
          
          IF(ADRCD.GT.0)THEN
            IHITX=0
            
            sf_data_input_cd_adr=ADRCD
#define SF_DATA_MAPV_CD
#include "doit/sf_data_access/sf_data_access.inc"
            IHITD=sf_data_output_hit_adr

            IF(
     1           (IHITD.GE.1).AND.
     2           (IHITD.LE.EvntHit_Num))THEN
              VALID_HIT=.TRUE.
            ELSE

              WRITE(STRING,1009,IOSTAT=IERR)IHITD
 1009         FORMAT('h',I5)
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              VALID_HIT=.FALSE.
            ENDIF
            
          ELSEIF(ADRXX.GT.0)THEN
            IHITD=0
            IHITX=SFSVMP(ADRXX)
            VALID_HIT=.TRUE.
          ELSE
            IHITD=0
            IHITX=0
            VALID_HIT=.FALSE.
          ENDIF

            
C-----------------------------------------------------------------------
C display the choice-specific information
C-----------------------------------------------------------------------

          IF(VALID_HIT)THEN
            
            
            IF(NINFO.EQ.1)THEN
C write the cell's raw TDC
C write the cell's raw ADC
              
              
C write the cell's raw TDC
              IF(IHITD.LE.0)THEN
                WRITE(STRING,1003,IOSTAT=IERR)
 1003           FORMAT('NO TDC')
              ELSE
                
                sf_data_input_hit_adr=IHITD
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
                ITIME=sf_data_output_tdc_raw
                
                WRITE(STRING,1008,IOSTAT=IERR) ITIME
 1008           FORMAT('T',I5)
              ENDIF
              
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              
C write the cell's raw ADC
              IF(IHITD.LE.0)THEN
                WRITE(STRING,1004,IOSTAT=IERR)
 1004           FORMAT('NO ADC')
              ELSE
                
                sf_data_input_hit_adr=IHITD
#define SF_DATA_RADC
#include "doit/sf_data_access/sf_data_access.inc"
                IPULS=sf_data_output_adc_raw
                
                WRITE(STRING,1007,IOSTAT=IERR) IPULS
 1007           FORMAT('A',I5)
              ENDIF
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              
              
              
              
              
            ELSEIF(NINFO.EQ.2)THEN
C write the cell's CRATE ADDRESS
              
#if defined(CLEO_SFDIAG)
              IF(LAYRCD.LE.LCANDR)THEN
                WRITE(STRING,2003,IOSTAT=IERR)
 2003           FORMAT('NOCC')
              ELSE
                LAYRDR=LAYRCD-LCANDR+1
                IPREAM=(WIRE+24)/24
                CALL SF_PREAMP_TO_CRATE(LAYRDR,IPREAM,ICRATE,ICARD,ITB)
                WRITE(STRING,2001,IOSTAT=IERR)ICRATE
 2001           FORMAT('"',I2)
                L=LEN(STRING)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
                YJUST=YJUST+YJDIFF
                
                JTB='T'
                IF(ITB.EQ.2)JTB='B'
                WRITE(STRING,2002,IOSTAT=IERR)ICARD,JTB
 2002           FORMAT(I2,'_',A1)
              ENDIF
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
#endif
              
              
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
              
            ELSEIF(NINFO.EQ.3)THEN
C write the SF ADDRESS
C write the cell's INUCHN
C write the cell's DOUBPT
              
C write the SF ADDRESS
              IF(ADRSF.EQ.0)THEN
                WRITE(STRING,3004,IOSTAT=IERR)
 3004           FORMAT('NOT SF')
              ELSE         
                WRITE(STRING,3001,IOSTAT=IERR)ADRSF
 3001           FORMAT('s',I5)
                L=LEN(STRING)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's INUCHN
                WRITE(STRING,3002,IOSTAT=IERR)INUCHN(ADRSF)
 3002           FORMAT('U ',I3)
                L=LEN(STRING)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's DOUBPT
                IF((DOUBPT(ADRSF).GT.9999).OR.
     2               (DOUBPT(ADRSF).LT.-999))THEN
                  WRITE(STRING,3013,IOSTAT=IERR)DOUBPT(ADRSF)
 3013             FORMAT(I6)
                ELSE
                  WRITE(STRING,3003,IOSTAT=IERR)DOUBPT(ADRSF)
 3003             FORMAT('D',I4)
                ENDIF
              ENDIF
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              
              
              
            ELSEIF(NINFO.EQ.4)THEN
C write the cell's TIME in ns
C write the DISTANCE in microns
C write the cell's PULSE HEIGHT in happy units
              
              IF(ADRCD.EQ.0)THEN
                WRITE(STRING,4005,IOSTAT=IERR)
 4005           FORMAT('NOT CD')
              ELSE
                
                sf_data_input_hit_adr=IHITD
#define SF_DATA_TMPS
#include "doit/sf_data_access/sf_data_access.inc"
                TIME=sf_data_output_r_tim_ps
                
                ITIME=TIME/1000
C               ITIME=ITIME/10
                IF(ITIME.LT.-99999)ITIME=-99999
                IF(ITIME.GT.999999)ITIME=999999
                WRITE(STRING7,4001,IOSTAT=IERR) ITIME
 4001           FORMAT('n', I6)
                L=LEN(STRING7)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING7(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's DRIFT DISTANCE in microns
                IF(IHITD.EQ.0)THEN
                  WRITE(STRING,4004,IOSTAT=IERR)
 4004             FORMAT('NODOIT')
                ELSE

                  sf_data_input_hit_adr=IHITD
#define SF_DATA_DRIFT_DISTANCE
#include "doit/sf_data_access/sf_data_access.inc"
                  RDRIFT=sf_data_output_drift
                  
                  IDRIFT=RDRIFT*1000000.
                  IF(IDRIFT.LT.-999)IDRIFT=-999
                  IF(IDRIFT.GT.9999)IDRIFT=9999
                  WRITE(STRING,4002,IOSTAT=IERR)IDRIFT
 4002             FORMAT('D ',I4)
                ENDIF
                L=LEN(STRING)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's PULSE HEIGHT
                
                sf_data_input_hit_adr=IHITD
#define SF_DATA_ADCH
#include "doit/sf_data_access/sf_data_access.inc"
                IPULSE=sf_data_output_ph_happy
                
                
                IF(IPULSE.LT.-999)IPULSE=-999
                IF(IPULSE.GT.9999)IPULSE=9999
                WRITE(STRING,4003,IOSTAT=IERR)IPULSE
 4003           FORMAT('P ',I4)
              ENDIF
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              
              
              
              
            ELSEIF(NINFO.EQ.5)THEN
C write the SF ADDRESS
C write the cell's ISTFOR
C write the cell's ISTBAK
              
C write the SF ADDRESS
              IF(ADRSF.EQ.0)THEN
                WRITE(STRING,5004,IOSTAT=IERR)
 5004           FORMAT('NOT SF')
              ELSE
                WRITE(STRING,5001,IOSTAT=IERR)ADRSF
 5001           FORMAT('s',I5)
                L=LEN(STRING)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's ISTFOR
                WRITE(STRING,5002,IOSTAT=IERR)ISTFOR(ADRSF)
 5002           FORMAT('F',I5)
                L=LEN(STRING)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's ISTBAK
                WRITE(STRING,5003,IOSTAT=IERR)ISTBAK(ADRSF)
 5003           FORMAT('B',I5)
              ENDIF
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              
              
              
              
            ELSEIF(NINFO.EQ.6)THEN
C write the cell's LAYER NUMBER
C write the cell's WIRE NUMBER
C write the cell's DOIT HIT NUMBER
              
              IF(ADRCD.GT.0)THEN
                WRITE(STRING7,6006,IOSTAT=IERR)LAYRCD
 6006           FORMAT('Lcd',I4)
              ELSEIF(ADRXX.GT.0)THEN
                WRITE(STRING7,6008,IOSTAT=IERR)LAYRXX
 6008           FORMAT('Lxx',I4)
              ENDIF
              L=LEN(STRING7)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING7(1:L),0.)
              YJUST=YJUST+YJDIFF
              
C write the cell's WIRE NUMBER
              IF(NRDOUT.EQ.1)THEN
                WRITE(STRING,6003,IOSTAT=IERR)WIRE
 6003           FORMAT('WR',I4)
              ELSE
                WRITE(STRING,6004,IOSTAT=IERR)RDOUT,WIRE
 6004           FORMAT('W',I1,I4)
              ENDIF
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              
C write the cell's DOIT HIT NUMBER
              IF(IHITD.EQ.0)THEN
                WRITE(STRING7,6007,IOSTAT=IERR)
 6007           FORMAT('notDOIT')
              ELSE
                WRITE(STRING7,6001,IOSTAT=IERR)IHITD
 6001           FORMAT('H',I6)
              ENDIF
              L=LEN(STRING7)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING7(1:L),0.)
              YJUST=YJUST+YJDIFF
              
              
              
              
            ELSEIF(NINFO.EQ.7)THEN
C  explain the cell's COLOR
              
              IEXPL=0
 721          IEXPL=IEXPL+1
              IF(IEXPL.LE.NEXPL)THEN
                IF(COLORH(ADRXT).NE.COLREQ(IEXPL))THEN
                  GO TO 721
                ENDIF
              ENDIF
              
              WRITE(STRING,7001,IOSTAT=IERR)COLORE(1,IEXPL)
 7001         FORMAT(A6)
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              
              WRITE(STRING,7001,IOSTAT=IERR)COLORE(2,IEXPL)
              L=LEN(STRING)
              CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              YJUST=YJUST+YJDIFF
              
              
              
            ELSEIF(NINFO.EQ.8)THEN
C write the number of hits in the cell
C (not tpc)
C write the time in ns to the next previous hit
C write the time in ns to the next later hit
C (tpc, loop over multi-hits in cell)
C write the hit's DOIT HIT NUMBER
C write the hit's TIME in ns
C write the hit's PULSE HEIGHT in happy units
C write the hit's NEXT HIT in clustering
              
              IF(ADRCD.EQ.0)THEN
                WRITE(STRING,8005,IOSTAT=IERR)
 8005           FORMAT('NOT CD')
                L=LEN(STRING)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
              ELSE
                
C write the number of hits in the cell
                IF(IHITD.EQ.0)THEN
                  WRITE(STRING,8004,IOSTAT=IERR)
 8004             FORMAT('NODOIT')
                ELSE
                  
                  sf_data_input_hit_adr=IHITD
#define SF_DATA_NHITS_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
                  NHSAMECELL=sf_data_output_hits_cell
                  
                  WRITE(STRING,8001,IOSTAT=IERR)NHSAMECELL
 8001             FORMAT('h'  ,I5)
                ENDIF
                L=LEN(STRING)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING(1:L),0.)
                YJUST=YJUST+YJDIFF
                
                IF(.NOT.L_XTS_DOIT_TPC)THEN

C write the time in ns to the next previous hit
                  IF(EvntHit_TimEarly(IHITD).EQ.-1)THEN
                    WRITE(STRING,8006,IOSTAT=IERR)
 8006               FORMAT('e:none')
                  ELSE
                    IEARLY=EvntHit_TimEarly(IHITD)/1000
                    IF(IEARLY.LT.-9999)IEARLY=-9999
                    IF(IEARLY.GT.99999)IEARLY=99999
                    WRITE(STRING,8002,IOSTAT=IERR)IEARLY
 8002               FORMAT('e'  ,I5)
                  ENDIF
                  L=LEN(STRING)
                  CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,
     2                 STRING(1:L),0.)
                  YJUST=YJUST+YJDIFF
                  
C write the time in ns to the next later hit
                  IF(EvntHit_TimLate(IHITD).EQ.-1)THEN
                    WRITE(STRING,8007,IOSTAT=IERR)
 8007               FORMAT('l:none')
                  ELSE
                    ILATE=EvntHit_TimLate(IHITD)/1000
                    IF(ILATE.LT.-9999)ILATE=-9999
                    IF(ILATE.GT.99999)ILATE=99999
                    WRITE(STRING,8003,IOSTAT=IERR)ILATE
 8003               FORMAT('l'  ,I5)
                  ENDIF
                  L=LEN(STRING)
                  CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,
     2                 STRING(1:L),0.)
                  YJUST=YJUST+YJDIFF
                ELSE
                  
#define SF_DATA_USE_ADDRESSED_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
                  
                  sf_data_input_cd_adr=ADRCD
#define SF_DATA_MAPV_CD
#include "doit/sf_data_access/sf_data_access.inc"
                  IHIT_LOOP=sf_data_output_hit_adr


                  WRITE(STRING29,8012)
 8012             FORMAT('  -hit- ','-time- ',' --ph- ','-next- ')
                  
                  L=LEN(STRING29)
                  CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU/3.,
     2                 STRING29(1:L),0.)
                    YJUST=YJUST+YJDIFF
                  
                  
 821              CONTINUE
                  IF(IHIT_LOOP.NE.0)THEN
                    
                    sf_data_input_hit_adr=IHIT_LOOP
#define SF_DATA_TMPS
#include "doit/sf_data_access/sf_data_access.inc"
                    TIME=sf_data_output_r_tim_ps
                    ITIME=TIME/1000
                    IF(ITIME.LT.-99999)ITIME=-99999
                    IF(ITIME.GT.999999)ITIME=999999
                    
                    sf_data_input_hit_adr=IHIT_LOOP
#define SF_DATA_ADCH
#include "doit/sf_data_access/sf_data_access.inc"
                    IPULSE=sf_data_output_ph_happy
                                        
                    IF(IPULSE.LT.-999)IPULSE=-999
                    IF(IPULSE.GT.9999)IPULSE=9999

                    sf_data_input_hit_adr=IHIT_LOOP
#define SF_DATA_GET_NEXT_HIT_CLUSTER
#include "doit/sf_data_access/sf_data_access.inc"
                    NXTHCLUS=sf_data_output_nxt_clus


                    sf_data_input_hit_adr=IHIT_LOOP
#define SF_DATA_GET_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"
                    PRIHCLUS=sf_data_output_primary
                    
                    FLAG=' '
                    IF(PRIHCLUS.EQ.IHIT_LOOP)FLAG='*'

                    
                    WRITE(STRING29,8011)FLAG,IHIT_LOOP,
     2                   ITIME,IPULSE,NXTHCLUS 
 8011               FORMAT(A1,I7,I7,I7,I7)
                    
                    L=LEN(STRING29)
                    CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU/3.,
     2                   STRING29(1:L),0.)
                    YJUST=YJUST+YJDIFF
                    
                    sf_data_input_hit_adr=IHIT_LOOP
#define SF_DATA_NEXTHIT_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
                    IHIT_LOOP=sf_data_output_next_mult
                    GO TO 821
                  ENDIF
                  
#define SF_DATA_USE_MULTIHIT_RESTORE
#include "doit/sf_data_access/sf_data_access.inc"
                  
                ENDIF
              ENDIF
              
              
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
              
            ELSEIF(NINFO.EQ.9)THEN
C write the cell's hit number from the map
C write the cell's primary hit in clustering
C write the cell's next hit in clustering
C write the cell's next hit in multihit
              
              IF(ADRCD.EQ.0)THEN
                WRITE(STRING,9005,IOSTAT=IERR)
 9005           FORMAT('NOT CD')
              ELSE
                
                WRITE(STRING7,9004,IOSTAT=IERR) IHITD
 9004           FORMAT('H', I6)
                L=LEN(STRING7)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING7(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's primary hit in clustering
                
                sf_data_input_hit_adr=IHITD
#define SF_DATA_GET_CLUSTER_PRIMARY
#include "doit/sf_data_access/sf_data_access.inc"
                PRIHCLUS=sf_data_output_primary
                
                WRITE(STRING7,9001,IOSTAT=IERR) PRIHCLUS
 9001           FORMAT('p', I6)
                L=LEN(STRING7)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING7(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's next hit in clustering
                
                sf_data_input_hit_adr=IHITD
#define SF_DATA_GET_NEXT_HIT_CLUSTER
#include "doit/sf_data_access/sf_data_access.inc"
                NXTHCLUS=sf_data_output_nxt_clus
                
                WRITE(STRING7,9002,IOSTAT=IERR) NXTHCLUS
 9002           FORMAT('C', I6)
                L=LEN(STRING7)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING7(1:L),0.)
                YJUST=YJUST+YJDIFF
                
C write the cell's next hit in multihit
              
                sf_data_input_hit_adr=IHITD
#define SF_DATA_NEXTHIT_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
                NXTHMULT=sf_data_output_next_mult
                
                WRITE(STRING7,9003,IOSTAT=IERR) NXTHMULT
 9003           FORMAT('m', I6)
                L=LEN(STRING7)
                CALL VEC_STRING_C(XC,YC,XJUST,YJUST,YHU,STRING7(1:L),0.)
                YJUST=YJUST+YJDIFF
              ENDIF
              
              
              
C end of the NINFO selection
            ENDIF
            
C end of VALID_HIT condition
          ENDIF
          
C end of position in bounds condition
        ENDIF
        
C END OF LOOP OVER DISPLAYED CELLS
 999  CONTINUE
      
#if defined(CLEO_XTSDEBUG)
      PRINT 1006
 1006 FORMAT(' XTS_INFO: EXIT')
#endif
      
      RETURN
      END
