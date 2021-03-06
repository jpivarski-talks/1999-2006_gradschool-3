*
* $Id: dsfhit.F,v 1.13 2000/10/02 20:52:45 dpp Exp $
*
* $Log: dsfhit.F,v $
* Revision 1.13  2000/10/02 20:52:45  dpp
*      -> remove_z_test messages
*      -> display number of VELOCITY_USE layers since z change
*
* Revision 1.12  2000/08/18 18:42:28  dpp
*     -> messages in "more" for removal of  Z test near top of curler
*
* Revision 1.11  2000/02/21 23:45:56  dpp
*  -> fix error in flag value for one of the width correction statements
*
* Revision 1.10  2000/02/12 20:27:14  dpp
*      -> display more info from... RFX and velocity shift,
*         WIDHIS from the "_MORE" input
*      -> renumber most of the FORMATS
*
* Revision 1.9  2000/01/06 22:20:01  dpp
* include more width calculation history
*
* Revision 1.8  1999/12/09 21:28:31  dpp
* change standard output of hit finding history
*
* Revision 1.7  1999/10/06 12:36:53  dpp
* new info on search width determination
*
* Revision 1.6  1999/08/03 21:51:33  dpp
* SFCHAN status
*
* Revision 1.5  1999/04/27 20:11:24  dpp
* display new info; new entry
*
* Revision 1.4  1999/01/29 14:56:58  dpp
* more info in "MORE"
*
* Revision 1.3  1998/07/09 21:27:14  lyon
* Retrofitted for sf_data_access (Dan missed this one)
*
* Revision 1.2  1998/07/09 00:26:11  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.4  1997/02/25 21:30:34  dpp
* allow use of SVX layer numbers
*
* Revision 1.3  1996/08/08 16:04:49  dpp
* fix array declaration; VSTORF->MVSTOR
*
* Revision 1.2  1995/09/25 19:18:47  zfiles
* changes for CLEO3 pattern recognition
*
* Revision 1.1.1.1  1994/10/08  01:01:03  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/18 22/03/94  14.47.47  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.29  by  Dan Peterson
*CMZ :  2.00/00 08/05/89  15.54.35  by  Dan Peterson
*-- Author : D. Peterson

      SUBROUTINE DSFHIT(IADR,IBRITE,IKEEP,
     2  ISFTYP,ILYRL,ILYRT,
     3  IWRNRX,IWSHSM,IWRNPX,IWRNPU,
     4  IVPRLM,IVPRED,WIDHIS,
     5  IDIFTN,IZNOWX,IWSHFZS,IWSHFZC,
     5  WIDADP,WIDADL,WIDADS,WIDADC,NHI,NLO,
     6  IWRNTU,IWRNFX,IHQLER,
     7  VELOCITY_USE, ZCALC_USE, STOPBK, NONSTRAGLER)

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom, and cdscrtcd (if needed)
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------

#include "seq/clutil/mesrep.inc"

#ifndef CLEO_SFNOZFILES
#include "doit/duseq/cdscrtcd.inc"
#endif

#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"


#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

      INTEGER    MAXSTR,MXLIST,NLIST,WDLIST
      PARAMETER (MAXSTR=2000)
      PARAMETER (WDLIST=5,NLIST=2*WDLIST+1,MXLIST=NLIST+1)

      INTEGER LSTORE(MHITSF),MSTORE(MHITSF)
      INTEGER ISTORE(43,MAXSTR),IHSTOR(MXLIST,MAXSTR)
      INTEGER IVRFAS(5,MVSTOR,MAXSTR),IVRFVS(4,2,MAXSTR),IVTMP(MVSTOR)
      INTEGER WIDHIS(MWIDH)
      INTEGER IWSTOR(MWIDHP,MAXSTR),CNSTOR(3,MAXSTR)
      INTEGER IY

C     MISCELANEOUS

      INTEGER IADR, IBRITE, IKEEP, MSTORS, ILYRT, ILYRL, IWRNPX
      INTEGER IWSHSM, IWRNPU, IVPRLM, IVPRED, IWRNTU
      INTEGER IDIFTN,IZNOWX,IWSHFZS,IWSHFZC,IWRNFX, IHQLER
      INTEGER VELOCITY_USE, ZCALC_USE, STOPBK, NONSTRAGLER
      INTEGER NEW_VEL_OR_Z,NEW_VEL
      INTEGER Z_TEST_ACTIVE,COUNTER_Z_CHANGE,LN_SINCE_Z_CHANGE
      INTEGER VELU_SINCE_Z_CH,VELU_PREV_Z_CH
      INTEGER LEN_TOT_NOW
      INTEGER LVL_RESTOR,DISPLAYLVL
      INTEGER FND_ST_THRS
      INTEGER LN_GOOD_STR,LN_GOOD_VEL,LN_GOOD_Z
      INTEGER ZTEMP,DISPLAYVZ,ZNEWX
      INTEGER WIDADP,WIDADL,WIDADS,WIDADC,NHI,NLO
      INTEGER IW, ISFTYP, IV, IV2, ISFSSF, LLYRSF
      INTEGER LLYRCD, IH, IWCD, IWDIFF, IADRT
      INTEGER IWDISP, IMORE, JSTORI, JSSTOP, KSTORI, IRETRN, IWRNRX
      INTEGER IADRI, JSTORM, KSTORM, KJDIFF, I, ISKIP, ISUM, IRELX
      INTEGER JSTORE, KSTORE, ICAL5
      REAL    FRAC1, FRAC2, FRAC3, SUM
      REAL    CAL1, CAL2, CAL3, CAL4, CAL6, CAL7
      CHARACTER*4 V_Z

C NSTORE IS THE INDEX WITHIN A CHAIN
C NSTORG IS THE GLOBAL INDEX, IT MUST BE ZEROED, @ EVENT BEGIN
C NSTORU IS THE ROTATING INDEX
C IADRS IS THE ADDRESS OF THE INITIAL HIT

      INTEGER NSTORE, NSTORG, NSTORU, KILLA, IADRS
      DATA NSTORE,NSTORG/0,0/
      DATA KILLA/0/

      CALL DSFHTP(IADR,IBRITE)

      IF(IKEEP.EQ.1)THEN
         NSTORE=NSTORE+1
         NSTORG=NSTORG+1

         NSTORU=NSTORG
 61      CONTINUE
         IF(NSTORU.GT.MAXSTR)THEN
            NSTORU=NSTORU-MAXSTR
            GO TO 61
         ENDIF

         IF(NSTORE.EQ.1)THEN
            LSTORE(IADR)=NSTORU
            IADRS=IADR
         ENDIF
         
         MSTORE(IADRS)=NSTORU
         MSTORS=NSTORU
         ISTORE(1,NSTORU)=1
         ISTORE(2,NSTORU)=ILYRL
         ISTORE(3,NSTORU)=ILYRT
         ISTORE(4,NSTORU)=RFX_N(1)
         ISTORE(5,NSTORU)=IWRNRX
         ISTORE(6,NSTORU)=IWRNPX
         ISTORE(14,NSTORU)=IWSHSM
         ISTORE(15,NSTORU)=RFX_L(1)
         ISTORE(7,NSTORU)=IWRNPU
C ISTORV IS IN COMMON
         ISTORE(8,NSTORU)=ISTORV
         ISTORE(9,NSTORU)=IVPRLM
         ISTORE(10,NSTORU)=IVPRED
         ISTORE(11,NSTORU)=IWRNTU
         ISTORE(12,NSTORU)=IWRNFX
         ISTORE(13,NSTORU)=IHQLER
         ISTORE(16,NSTORU)=IDIFTN
         ISTORE(17,NSTORU)=IZNOWX
         ISTORE(18,NSTORU)=IWSHFZS
         ISTORE(19,NSTORU)=IWSHFZC
         ISTORE(20,NSTORU)=WIDADP
         ISTORE(21,NSTORU)=WIDADL
         ISTORE(22,NSTORU)=WIDADS
         ISTORE(23,NSTORU)=WIDADC
         ISTORE(24,NSTORU)=NHI
         ISTORE(25,NSTORU)=NLO
         ISTORE(26,NSTORU)=VELOCITY_USE
         ISTORE(27,NSTORU)=ZCALC_USE
         ISTORE(28,NSTORU)=STOPBK
         ISTORE(29,NSTORU)=NONSTRAGLER
         ISTORE(30,NSTORU)=-1
         ISTORE(31,NSTORU)=-1
         ISTORE(32,NSTORU)=-1
         ISTORE(33,NSTORU)=-1
         ISTORE(34,NSTORU)=-1
         ISTORE(35,NSTORU)=-1
         ISTORE(36,NSTORU)=-1
         ISTORE(37,NSTORU)=-1
         ISTORE(38,NSTORU)=-1
         ISTORE(39,NSTORU)=-1
         ISTORE(40,NSTORU)=-1
         ISTORE(41,NSTORU)=-1
         ISTORE(42,NSTORU)=-1
         ISTORE(43,NSTORU)=-1
         
         DO 64 IW=1,MWIDH
            IWSTOR(IW,NSTORU)=WIDHIS(IW)
 64      CONTINUE
         IWSTOR(MWIDHP,NSTORU)=ISFTYP
            
C STORE IN UNROTATED ORDER, VSTORF IS NEWEST, 1 IS OLDEST
C IV IS LOCATION IN UNROTATED ARRAY, IV2 IN ROTATED
         DO 65 IV=1,VSTORF
            IV2=IV+VLLOC1
            IF(IV2.GT.VSTORF)IV2=IV2-VSTORF
            IVRFAS(1,IV,NSTORU)=RFX_S(1,IV2)
            IVRFAS(2,IV,NSTORU)=RFX_S(2,IV2)
            IVRFAS(3,IV,NSTORU)=VELADD(IV2)
            IVRFAS(4,IV,NSTORU)=RFX_S(4,IV2)
            IVRFAS(5,IV,NSTORU)=RFX_S(5,IV2)
 65      CONTINUE

         DO 66 IV=1,2
            IVRFVS(1,IV,NSTORU)=RFX_SS(1,IV)
            IVRFVS(2,IV,NSTORU)=RFX_SS(2,IV)
            IVRFVS(3,IV,NSTORU)=IVTEMP(IV)
 66      CONTINUE

         IVRFVS(4,1,NSTORU)=EXTUV1
         IVRFVS(4,2,NSTORU)=EXTUV2
         DO 67 IV=1,3
            IF(IV.LE.MUTSTC)THEN
               CNSTOR(IV,NSTORU)=-1
               IF(IV.LE.NUTSTD)THEN
                  CNSTOR(IV,NSTORU)=IUCNTR(IV)
               ENDIF
            ENDIF
 67      CONTINUE
      ENDIF
      RETURN

C MORE information after velocity calculation

      ENTRY DSFHIT_MORE(
     1     NEW_VEL_OR_Z,NEW_VEL,
     2     LN_GOOD_STR,LN_GOOD_VEL,LN_GOOD_Z,
     3     LVL_RESTOR,
     4     ZTEMP,Z_TEST_ACTIVE,COUNTER_Z_CHANGE,LN_SINCE_Z_CHANGE,
     5     VELU_SINCE_Z_CH,VELU_PREV_Z_CH,
     6     ZNEWX,
     7     LEN_TOT_NOW,WIDHIS)
      ISTORE(30,NSTORU)=NEW_VEL_OR_Z
      ISTORE(31,NSTORU)=NEW_VEL
      ISTORE(32,NSTORU)=LN_GOOD_STR
      ISTORE(33,NSTORU)=LN_GOOD_VEL
      ISTORE(34,NSTORU)=LN_GOOD_Z
      ISTORE(35,NSTORU)=LVL_RESTOR
      ISTORE(36,NSTORU)=ZTEMP
      ISTORE(37,NSTORU)=Z_TEST_ACTIVE
      ISTORE(38,NSTORU)=COUNTER_Z_CHANGE
      ISTORE(39,NSTORU)=LN_SINCE_Z_CHANGE
      ISTORE(40,NSTORU)=VELU_SINCE_Z_CH
      ISTORE(41,NSTORU)=VELU_PREV_Z_CH
      ISTORE(42,NSTORU)=ZNEWX
      ISTORE(43,NSTORU)=LEN_TOT_NOW
         DO 69 IW=1,MWIDH
            IWSTOR(IW,NSTORU)=WIDHIS(IW)
 69      CONTINUE
      RETURN


C HIT_FAIL_SEED

      ENTRY DSFHFS(IADR,ILYRT,IWRNTU,IHQLER)

      NSTORE=NSTORE+1
      NSTORG=NSTORG+1

      NSTORU=NSTORG
71    IF(NSTORU.LE.MAXSTR)GO TO 72
      NSTORU=NSTORU-MAXSTR
      GO TO 71
72    IF(NSTORE.EQ.1)GO TO 73
      PRINT 1021,IADR
1021  FORMAT(' DSFHFS: FATAL ERROR, NSTORE.NE.1, IADR=',I6)
73    LSTORE(IADR)=NSTORU
      IADRS=IADR
      MSTORE(IADRS)=NSTORU
      ISTORE(1,NSTORU)=2
      ISTORE(3,NSTORU)=ILYRT
      ISTORE(4,NSTORU)=RFX_N(1)
      ISTORE(11,NSTORU)=IWRNTU
      ISTORE(13,NSTORU)=IHQLER
      NSTORE=0
      RETURN

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C HIT_FAIL

      ENTRY DSFHTF(IADR,
     2  ISFTYP,ILYRL,ILYRT,
     3  IWRNRX,IWSHSM,IWRNPX,IWRNPU,
     4  IVPRLM,IVPRED,WIDHIS,
     5  IDIFTN,IZNOWX,IWSHFZS,IWSHFZC,
     6  WIDADP,WIDADL,WIDADS,WIDADC,NHI,NLO,
     7  IWRNTU,       IHQLER)

      NSTORE=NSTORE+1
      NSTORG=NSTORG+1

      NSTORU=NSTORG
74    IF(NSTORU.LE.MAXSTR)GO TO 75
      NSTORU=NSTORU-MAXSTR
      GO TO 74
75    IF(NSTORE.NE.1)GO TO 76
      PRINT 1011,IADR
1011  FORMAT(' DSFHTF: ODD: STARTING CHAIN ON FAIL, IADR=',I6)
      LSTORE(IADR)=NSTORU
      IADRS=IADR

76    MSTORE(IADRS)=NSTORU
      MSTORS=NSTORU

      ISTORE(1,NSTORU)=0
      ISTORE(2,NSTORU)=ILYRL
      ISTORE(3,NSTORU)=ILYRT
      ISTORE(4,NSTORU)=RFX_N(1)
      ISTORE(5,NSTORU)=IWRNRX
      ISTORE(6,NSTORU)=IWRNPX
      ISTORE(14,NSTORU)=IWSHSM
      ISTORE(15,NSTORU)=RFX_L(1)
      ISTORE(7,NSTORU)=IWRNPU
      ISTORE(8,NSTORU)=ISTORV
      ISTORE(9,NSTORU)=IVPRLM
      ISTORE(10,NSTORU)=IVPRED
      ISTORE(11,NSTORU)=IWRNTU
C      ISTORE(12,NSTORU)=IWRNFX
      ISTORE(13,NSTORU)=IHQLER
      ISTORE(16,NSTORU)=IDIFTN
      ISTORE(17,NSTORU)=IZNOWX
      ISTORE(18,NSTORU)=IWSHFZS
      ISTORE(19,NSTORU)=IWSHFZC
      ISTORE(20,NSTORU)=WIDADP
      ISTORE(21,NSTORU)=WIDADL
      ISTORE(22,NSTORU)=WIDADS
      ISTORE(23,NSTORU)=WIDADC
      ISTORE(24,NSTORU)=NHI
      ISTORE(25,NSTORU)=NLO
      ISTORE(26,NSTORU)=0
      ISTORE(27,NSTORU)=0
      ISTORE(28,NSTORU)=0
      ISTORE(29,NSTORU)=0
      ISTORE(30,NSTORU)=0
      ISTORE(31,NSTORU)=0
      ISTORE(32,NSTORU)=0
      ISTORE(33,NSTORU)=0
      ISTORE(34,NSTORU)=0
      ISTORE(35,NSTORU)=0
      ISTORE(36,NSTORU)=0
      ISTORE(37,NSTORU)=0
      ISTORE(38,NSTORU)=0
      ISTORE(39,NSTORU)=0
      ISTORE(40,NSTORU)=0
      ISTORE(41,NSTORU)=0
      ISTORE(42,NSTORU)=0
      ISTORE(43,NSTORU)=0

      DO 77 IW=1,MWIDH
77    IWSTOR(IW,NSTORU)=WIDHIS(IW)
      IWSTOR(MWIDHP,NSTORU)=ISFTYP
      DO 78 IV=1,VSTORF
         IV2=IV+VLLOC1
         IF(IV2.GT.VSTORF)IV2=IV2-VSTORF
         IVRFAS(1,IV,NSTORU)=RFX_S(1,IV2)
         IVRFAS(2,IV,NSTORU)=RFX_S(2,IV2)
         IVRFAS(3,IV,NSTORU)=VELADD(IV2)
         IVRFAS(4,IV,NSTORU)=RFX_S(4,IV2)
         IVRFAS(5,IV,NSTORU)=RFX_S(5,IV2)
 78      CONTINUE
      DO 79 IV=1,2
      IVRFVS(1,IV,NSTORU)=RFX_SS(1,IV)
      IVRFVS(2,IV,NSTORU)=RFX_SS(2,IV)
79    IVRFVS(3,IV,NSTORU)=IVTEMP(IV)
      IVRFVS(4,1,NSTORU)=EXTUV1
      IVRFVS(4,2,NSTORU)=EXTUV2
      DO 80 IV=1,3
      IF(IV.GT.MUTSTC)GO TO 80
      CNSTOR(IV,NSTORU)=-1
      IF(IV.GT.NUTSTD)GO TO 80
      CNSTOR(IV,NSTORU)=IUCNTR(IV)
80    CONTINUE

      IHSTOR(1,NSTORU)=0
C LOOP OVER THE SUB LAYERS OF SUPER LAYER ILYRT
C FIND THE OTHER HITS IN THE LAYER
      ISFSSF=0
81    ISFSSF=ISFSSF+1
      IF(ISFSSF.GT.MSFSSF)GO TO 89
      
      LLYRSF=LAYSF(ISFSSF,ILYRT)
      IF(LLYRSF.EQ.0)GO TO 89
      LLYRCD=CDLASF(LLYRSF)

      sf_data_input_layr_cd = llyrcd
#define SF_DATA_1STH_CD
#include "doit/sf_data_access/sf_data_access.inc"
      IH = sf_data_output_hit_adr

85    IF(IH.EQ.0)GO TO 81
      
      sf_data_input_hit_adr = IH
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
      IWCD = sf_data_output_wire

      IF(IWCD.LT.0            )GO TO 87
      IF(IWCD.GE.NWRSF(LLYRSF))GO TO 87

      IADRT=INSFSF(LAYSF(ISFSSF,ILYRT))+IWCD
      IWDISP=WIRSSF(IADRT)
      IWDIFF=IWDISP-IWRNTU
      IF(IWDIFF.GT. NWRSF(LLYRSF)/2)IWDIFF=IWDIFF-NWRSF(LLYRSF)
      IF(IWDIFF.LE.-NWRSF(LLYRSF)/2)IWDIFF=IWDIFF+NWRSF(LLYRSF)
      IF(IWDIFF.LT.0)IWDIFF=-IWDIFF
      IF(IWDIFF.GT.WDLIST)GO TO 87
      IHSTOR(1,NSTORU)=IHSTOR(1,NSTORU)+1
      IHSTOR(IHSTOR(1,NSTORU)+1,NSTORU)=IWDISP
      IF(INUCHN(IADRT).LT.0)IHSTOR(IHSTOR(1,NSTORU)+1,NSTORU)
     1                    =-IHSTOR(IHSTOR(1,NSTORU)+1,NSTORU)
      IF(IHSTOR(1,NSTORU).EQ.NLIST)GO TO 89

87    CONTINUE
      
      sf_data_input_hit_adr = IH
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
      IH = sf_data_output_hit_adr
      GO TO 85
89    CONTINUE
      RETURN

      ENTRY DSFDM0
      NSTORE=0
      RETURN

      ENTRY DSFDM00
      NSTORE=0
      NSTORG=0
      CALL VZERO(LSTORE,MHITSF)
      CALL VZERO(MSTORE,MHITSF)
      RETURN

      ENTRY DSFDMP
      IF(KILLA.EQ.1)GO TO 789
      IMORE=0
101   PRINT 2001,NSTORE

#if !defined(CLEO_XTSUBS)
2001  FORMAT(' DSFDMP: DUMP HITS ON THIS CHAIN?, NSTORE=',I5/
     1  '  ,"Y", --> "N", "K"ILL, "O"THER ADDRESS,',
     2  ' "D"ISPLAYMENU, "M"OREINFO')
#endif

#if defined(CLEO_XTSUBS)
2001  FORMAT(' DSFDMP: DUMP HITS ON THIS CHAIN?, NSTORE=',I5/
     1  '  ,"Y", --> "N", "K"ILL, "O"THER ADDRESS,',
     2  '  "M"OREINFO')
#endif

      IF(NSTORE.EQ.0)GO TO 789

#if !defined(CLEO_XTSUBS)
      READ 2002,IY
2002  FORMAT(A1)
#endif

#if defined(CLEO_XTSUBS)
      CALL XTS_RFV_INPUT
      IY=XTS_ANSWER_IY
#endif

      IF(IY.NE.'Y')GO TO 102
      JSTORI=NSTORE
      JSSTOP=0
      KSTORI=NSTORU
      IRETRN=1
      GO TO 160
102   IF(IY.EQ.' ')GO TO 789
      IF(IY.EQ.'null')GO TO 789
      IF(IY.EQ.'N')GO TO 789
      IF(IY.NE.'K')GO TO 104
      KILLA=1
      GO TO 789
104   IF(IY.NE.'O')GO TO 121
106   PRINT 2011
2011  FORMAT(' DSFDMP: ENTER CELL-ID OF HIT TO DUMP, I,',
     1       ' ENTER <CR> OR <zero> TO SKIP IT')

#if !defined(CLEO_XTSUBS)
      READ 2012,IADRI
2012  FORMAT(I)
#endif

#if defined(CLEO_XTSUBS)
      CALL XTS_RFV_GETINT
      IADRI=XTS_ANSWER_IY
      PRINT 2017,IADRI
2017  FORMAT(' DSFDMP: RECEIVED IADRI=',I10)
#endif

      IF(IADRI.EQ.0)GO TO 101
      IF((IADRI.GE.1).AND.(IADRI.LE.MHITSF))GO TO 107
      PRINT 2013,IADRI
2013  FORMAT(' DSFDMP: INPUT ADDRESS =',I10,' IS GARBAGE')
      GO TO 106
107   IF((LSTORE(IADRI).NE.0).AND.(MSTORE(IADRI).NE.0))GO TO 108
      PRINT 2014,IADRI
2014  FORMAT(' DSFDMP: REQUESTED ADDRESS  (',I6,')  HAS NO RECORD')
      GO TO 101
108   KSTORI=MSTORE(IADRI)
      JSTORI=MSTORE(IADRI)-LSTORE(IADRI)+1
      JSSTOP=0
      IF(KSTORI.LE.0)KSTORI=KSTORI+MAXSTR
      IRETRN=2
      GO TO 160
C
121   IF(IY.NE.'D')GO TO 131
      PRINT 2016
2016  FORMAT(' CONTROL IS ON EVENT DISPLAY MENU')
      CALL DSF_SCREEN_D
      GO TO 101
C
131   IF(IY.NE.'M')GO TO 101
      IF(IMORE.EQ.1)GO TO 133
      PRINT 2023
2023  FORMAT(' YOU HAD BETTER GET THE FIRST DUMP OF HITS',
     1  ' BEFORE ASKING FOR ADDITIONAL INFO')
      GO TO 101
133   PRINT 2021
2021  FORMAT(' ENTER VALUE UNDER "#" FOR ADITIONAL INFO',
     1       ' <CR> OR <zero> TO SKIP IT')

#if !defined(CLEO_XTSUBS)
      READ 2022,JSTORM
2022  FORMAT(I)
#endif

#if defined(CLEO_XTSUBS)
      CALL XTS_RFV_GETINT
      JSTORM=XTS_ANSWER_IY
#endif

      IF(JSTORM.EQ.0)GO TO 101
      IF(JSTORM.LT.1)GO TO 133
      IF(JSTORM.GT.NSTORE)GO TO 133
      KSTORM=JSTORM+KJDIFF
      IF(KSTORM.GT.MAXSTR)KSTORM=KSTORM-MAXSTR

      JSTORI=JSTORM+4
      IF(JSTORI.GT.NSTORE)JSTORI=NSTORE
      JSSTOP=JSTORM-4
      IF(JSSTOP.LT.0)JSSTOP=0
      KSTORI=JSTORI+KJDIFF
      IF(KSTORI.GT.MAXSTR)KSTORI=KSTORI-MAXSTR
      IRETRN=3
      GO TO 160
135   CONTINUE

      PRINT 2070,ISTORE(3,KSTORM)

      DO 136 I=1,VSTORF
      IVTMP(I)=LAYSSF(SFLSFA( IVRFAS(3,I,KSTORM) ))
136   CONTINUE

      PRINT 2078,(         I        ,I=1,VSTORF)
      PRINT 2080,(IVRFAS(3,I,KSTORM),I=1,VSTORF)
      PRINT 2082,(IVTMP(I)          ,I=1,VSTORF)
      PRINT 2084,(IVRFAS(1,I,KSTORM),I=1,VSTORF)
      PRINT 2086,(IVRFAS(2,I,KSTORM),I=1,VSTORF)
      PRINT 2088,(IVRFAS(4,I,KSTORM),I=1,VSTORF)
      PRINT 2090,(IVRFAS(5,I,KSTORM),I=1,VSTORF)

      ISKIP=6*(VSTORH-1)
      PRINT 2106
      PRINT 2108,(IVRFVS(1,I,KSTORM),I=2,1,-1)
      PRINT 2110,(IVRFVS(2,I,KSTORM),I=2,1,-1)
      PRINT 2112,(IVRFVS(3,I,KSTORM),I=2,1,-1)

      FRAC1=1.*EXTPV1/EXTPDM
      FRAC2=1.*EXTPV2/EXTPDM
      SUM=FRAC1*IVRFVS(3,1,KSTORM)+FRAC2*IVRFVS(3,2,KSTORM)+.5
      ISUM=SUM
      PRINT 2114,FRAC1,IVRFVS(3,1,KSTORM),FRAC2,IVRFVS(3,2,KSTORM),ISUM

      FRAC1=1.*EXTPRL/EXTODM
      FRAC2=1.*EXTOLD/EXTODM
      PRINT 2116,FRAC1,FRAC2

      FRAC3=1.*IVRFVS(4,1,KSTORM)+1.*IVRFVS(4,2,KSTORM)
      FRAC1=1.*IVRFVS(4,1,KSTORM)/FRAC3
      FRAC2=1.*IVRFVS(4,2,KSTORM)/FRAC3
      SUM=FRAC1*IVRFVS(3,1,KSTORM)+FRAC2*IVRFVS(3,2,KSTORM)+.5
      ISUM=SUM
      PRINT 2118,FRAC1,IVRFVS(3,1,KSTORM),FRAC2,IVRFVS(3,2,KSTORM),
     1  ISUM,ISTORE(9,KSTORM)

      FRAC1=1.*EXTPRL/EXTODM
      FRAC2=1.*EXTOLD/EXTODM
      PRINT 2120,FRAC1,FRAC2,ISTORE(10,KSTORM)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      FRAC1=1.* ISTORE(10,KSTORM)/RPWSSF(ISTORE(3,KSTORM))
      FRAC2=1.* ISTORE(4,KSTORM)/MULST
      FRAC3=FRAC1*FRAC2
      CAL1=(1.*ISTORE(5,KSTORM) )/1024.
      CAL2=(1.*ISTORE(6,KSTORM) )/1024.
      IRELX=IBITS((ISTORE(6,KSTORM)+512),0,10)
      CAL4=(1.*ISTORE(14,KSTORM) )/1024.
      ICAL5=(1+GAPWD1(IWSTOR(MWIDHP,KSTORM)))*RPWSSF(ISTORE(3,KSTORM))
      CAL6=(1.*ISTORE(18,KSTORM) )/1024.
      CAL7=(1.*ISTORE(19,KSTORM) )/1024.
      PRINT 2144,ISTORE(3,KSTORM),FRAC1
      PRINT 2146,ISTORE(3,KSTORM),FRAC2
      PRINT 2148,FRAC3

      IF(    IWSTOR(WH_S_SMO,KSTORM).EQ.1)THEN
         PRINT 2150,ISTORE(15,KSTORM),GAPWD1(IWSTOR(MWIDHP,KSTORM)),
     2        RPWSSF(ISTORE(3,KSTORM)),ICAL5
      ELSEIF(IWSTOR(WH_S_SMO,KSTORM).EQ.2)THEN
         PRINT 2151,ISTORE(15,KSTORM),GAPWD1(IWSTOR(MWIDHP,KSTORM)),
     2        RPWSSF(ISTORE(3,KSTORM)),ICAL5,ISTORE(39,KSTORM)
      ELSEIF(IWSTOR(WH_S_SMO,KSTORM).EQ.3)THEN
         PRINT 2152,ISTORE(15,KSTORM),GAPWD1(IWSTOR(MWIDHP,KSTORM)),
     2        RPWSSF(ISTORE(3,KSTORM)),ICAL5
         PRINT 2153,CAL4
      ENDIF
      
      PRINT 2154,ISTORE(16,KSTORM),ISTORE(17,KSTORM)
      PRINT 2156,CAL6,CAL7          
      PRINT 2158,CAL1
      PRINT 2160,CAL2,CCTRLO(IWSTOR(MWIDHP,KSTORM)),
     2           IRELX,CCTRHI(IWSTOR(MWIDHP,KSTORM))

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      IF(IWSTOR(WH_W_LEN,KSTORM).EQ.1)
     1     PRINT 2186,WIDAD2(IWSTOR(MWIDHP,KSTORM))
      IF(IWSTOR(WH_W_LEN,KSTORM).EQ.2)
     1     PRINT 2188,WIDAD1(IWSTOR(MWIDHP,KSTORM))
      IF(IWSTOR(WH_W_LEN,KSTORM).EQ.3)
     1     PRINT 2190,WIDAD0(IWSTOR(MWIDHP,KSTORM))
      IF(IWSTOR(WH_W_GAP,KSTORM).EQ.0)
     1     PRINT 2192
      IF(IWSTOR(WH_W_GAP,KSTORM).EQ.1)
     1     PRINT 2194
      IF(IWSTOR(WH_W_GAP,KSTORM).EQ.2)
     1     PRINT 2196,WIDAD1(IWSTOR(MWIDHP,KSTORM))
      IF(IWSTOR(WH_W_GAP,KSTORM).EQ.3)
     1     PRINT 2198,WIDAD2(IWSTOR(MWIDHP,KSTORM))
      PRINT 2200,ISTORE(20,KSTORM)
      IF(IWSTOR(WH_W_GAP,KSTORM).EQ.4)
     1     PRINT 2202,WIDAD2(IWSTOR(MWIDHP,KSTORM))
      IF(IWSTOR(WH_W_GAP,KSTORM).EQ.5)
     1     PRINT 2204,WIDAD2(IWSTOR(MWIDHP,KSTORM))
      IF(IWSTOR(WH_W_NAR,KSTORM).EQ.0)
     1     PRINT 2206
      IF(IWSTOR(WH_W_NAR,KSTORM).EQ.1)
     1     PRINT 2208
      IF(IWSTOR(WH_W_NAR,KSTORM).EQ.2)
     1     PRINT 2210,WIDAD2(IWSTOR(MWIDHP,KSTORM))

      IF(IWSTOR(WH_W_ST_CH,KSTORM).EQ.0)
     1     PRINT 2212
      IF(IWSTOR(WH_W_ST_CH,KSTORM).EQ.1)
     1     PRINT 2214
      IF(IWSTOR(WH_W_ST_CH,KSTORM).EQ.2)
     1     PRINT 2216,ISTORE(22,KSTORM)
      IF(IWSTOR(WH_W_ST_CH,KSTORM).GT.2)THEN
         FND_ST_THRS=IWSTOR(4,KSTORM)-2
         PRINT 2218,FND_ST_THRS,
     2        THRS_ST_MEAS(FND_ST_THRS,IWSTOR(MWIDHP,KSTORM)),
     3        ISTORE(22,KSTORM)
      ENDIF

      IF(IWSTOR(WH_W_CNTR,KSTORM).EQ.0)
     1     PRINT 2228
      IF(IWSTOR(WH_W_CNTR,KSTORM).EQ.1)
     1     PRINT 2230
      IF(IWSTOR(WH_W_CNTR,KSTORM).EQ.2)
     1     PRINT 2232,WIDAD1(IWSTOR(MWIDHP,KSTORM))
      PRINT 2234,ISTORE(21,KSTORM)
      IF(IWSTOR(WH_W_ST_SH,KSTORM).EQ.0)
     1     PRINT 2236
      IF(IWSTOR(WH_W_ST_SH,KSTORM).EQ.1)
     1     PRINT 2238
      IF(IWSTOR(WH_W_ST_SH,KSTORM).EQ.2)
     1     PRINT 2240,ISTORE(23,KSTORM)
      IF(IWSTOR(WH_W_ST_SH,KSTORM).EQ.3)
     1     PRINT 2242,ISTORE(23,KSTORM)
      IF(IWSTOR(WH_W_VEL,KSTORM).EQ.0)
     1     PRINT 2244
      IF(IWSTOR(WH_W_VEL,KSTORM).EQ.1)
     1     PRINT 2246
      IF(IWSTOR(WH_W_VEL,KSTORM).EQ.2)
     1     PRINT 2248
      IF(IWSTOR(WH_W_IG_DOUB,KSTORM).EQ.0)
     1     PRINT 2250
      IF(IWSTOR(WH_W_IG_DOUB,KSTORM).EQ.1)
     1     PRINT 2252
      IF(IWSTOR(WH_W_IG_DOUB,KSTORM).EQ.2)
     1     PRINT 2254
      IF(IWSTOR(WH_W_IG_DOUB,KSTORM).EQ.3)
     1     PRINT 2256
      IF(IWSTOR(WH_W_PHICOR,KSTORM).EQ.0)
     1     PRINT 2258
      IF(IWSTOR(WH_W_PHICOR,KSTORM).EQ.1)
     1     PRINT 2260
      IF(IWSTOR(WH_W_PHICOR,KSTORM).EQ.2)
     1     PRINT 2262
      IF(IWSTOR(WH_W_RAD_OUT,KSTORM).EQ.0)
     1     PRINT 2264,ISTORE(8,NSTORU)
      IF(IWSTOR(WH_W_RAD_OUT,KSTORM).EQ.1)
     1     PRINT 2266
      IF(IWSTOR(WH_W_RAD_OUT,KSTORM).EQ.2)
     1     PRINT 2268
      IF(IWSTOR(WH_W_RAD_OUT,KSTORM).EQ.3)
     1     PRINT 2270
      PRINT 2272,ISTORE(24,KSTORM),ISTORE(25,KSTORM)
      PRINT 2274,
     1     (CNSTOR(I,KSTORM), 
     2     LTOPAS(I,IWSTOR(MWIDHP,KSTORM)),
     2     LOUTOF(I,IWSTOR(MWIDHP,KSTORM)),I=1,3),JUTSTC
      IF(ISTORE(13,KSTORM).EQ.-5)
     1     PRINT 2278
      IF(ISTORE(13,KSTORM).EQ.-4)
     1     PRINT 2280
      IF(ISTORE(13,KSTORM).EQ.-3)
     1     PRINT 2282
      IF(ISTORE(13,KSTORM).EQ.-2)
     1     PRINT 2284
      IF(ISTORE(13,KSTORM).EQ.-1)
     1     PRINT 2286
      IF(ISTORE(13,KSTORM).EQ. 0)
     1     PRINT 2288
      IF(ISTORE(13,KSTORM).EQ. 1)
     1     PRINT 2290
      IF(ISTORE(13,KSTORM).EQ. 2)
     1     PRINT 2292
      IF(ISTORE(13,KSTORM).GT. 2)
     1     PRINT 2294,ISTORE(13,KSTORM)
      IF(ISTORE(13,KSTORM).LT.-5)
     1     PRINT 2294,ISTORE(13,KSTORM)
      IF(IWSTOR(WH_V_Z_STR,KSTORM).EQ.0)
     1     PRINT 2300
      IF(IWSTOR(WH_V_Z_STR,KSTORM).EQ.1)
     1     PRINT 2302
      IF(IWSTOR(WH_V_Z_STR,KSTORM).EQ.2)
     1     PRINT 2304,ISTORE(39,KSTORM)
      IF(IWSTOR(WH_V_Z_STR,KSTORM).EQ.3)
     1     PRINT 2306,ISTORE(39,KSTORM)


      IF(IWSTOR(WH_V_Z_RZT_R,KSTORM).EQ.0)
     1     PRINT 2308
      IF(IWSTOR(WH_V_Z_RZT_R,KSTORM).EQ.1)
     1     PRINT 2309,ISTORE(38,KSTORM)
      IF(IWSTOR(WH_V_Z_RZT_R,KSTORM).EQ.2)
     1     PRINT 2310
      IF(IWSTOR(WH_V_Z_RZT_R,KSTORM).EQ.3)
     1     PRINT 2311
      IF(IWSTOR(WH_V_Z_RZT_Z,KSTORM).EQ.1)
     1     PRINT 2312,ISTORE(42,KSTORM),ISTORE(38,KSTORM)
      IF(IWSTOR(WH_V_Z_RZT_Z,KSTORM).EQ.2)
     1     PRINT 2313,ISTORE(42,KSTORM),ISTORE(38,KSTORM)
      IF(IWSTOR(WH_V_Z_RZT_Z,KSTORM).EQ.3)
     1     PRINT 2314,ISTORE(38,KSTORM),ISTORE(39,KSTORM)
      IF(IWSTOR(WH_V_Z_RZT_Z,KSTORM).EQ.4)
     1     PRINT 2315,ISTORE(38,KSTORM),ISTORE(39,KSTORM)
      IF(IWSTOR(WH_V_Z_RZT_C,KSTORM).EQ.1)
     1     PRINT 2317,ISTORE(10,KSTORM),ISTORE(38,KSTORM)
      IF(IWSTOR(WH_V_Z_RZT_C,KSTORM).EQ.2)
     1     PRINT 2318,ISTORE(10,KSTORM),ISTORE(43,KSTORM)
      IF(IWSTOR(WH_V_Z_RZT_C,KSTORM).EQ.3)
     1     PRINT 2318,ISTORE(10,KSTORM),ISTORE(43,KSTORM)
      IF(IWSTOR(WH_V_Z_RZT_R,KSTORM).EQ.11)
     1     PRINT 2324
      IF(IWSTOR(WH_V_Z_RZT_R,KSTORM).EQ.12)
     1     PRINT 2325
      IF(IWSTOR(WH_V_Z_RZT_R,KSTORM).EQ.13)
     1     PRINT 2326
      IF(IWSTOR(WH_V_Z_RZT_R,KSTORM).EQ.14)
     1     PRINT 2327

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

 2070 FORMAT(' DUMP OF THE VELOCITY CALCULATION DATA,',
     1  ' MOST RECENT IS AT RIGHT, LAYER',I4)
 2078 FORMAT('          ENTRY=> ',19I6)
 2080 FORMAT(' CELL ID          ',19I6)
 2082 FORMAT(' SSF LAYER        ',19I6)
 2084 FORMAT(' RADIUS CHANGE    ',19I6)
 2086 FORMAT('    PHI CHANGE    ',19I6)
 2088 FORMAT(' raw rad change   ',19I6)
 2090 FORMAT(' raw phi change   ',19I6)
 2106 FORMAT(' THESE ARE THE SUMS')
 2108 FORMAT(' RADIUS CHANGE    ',<ISKIP>X,I6,<ISKIP>X,I6)
 2110 FORMAT('    PHI CHANGE    ',<ISKIP>X,I6,<ISKIP>X,I6)
 2112 FORMAT(' VELOCITY, ST CELL',<ISKIP>X,I6,<ISKIP>X,I6)
 2114 FORMAT(' (INITIAL) PRLIMINARY VELOCITY:  ',F6.3,' * ',I5,
     1       ' + ',F6.3,' * ',I5,' = ',I5)
 2116 FORMAT(' (INITIAL) PREDICTED  VELOCITY:  ',F6.3,' *PRELIM',
     1       ' + ',F6.3,' OLD   ',' = ?')
 2118 FORMAT(' ( USED  ) PRLIMINARY VELOCITY:  ',F6.3,' * ',I5,
     1       ' + ',F6.3,' * ',I5,' = ',I5,'  VLPRLM=',I5)
 2120 FORMAT(' ( FOUND ) PREDICTED  VELOCITY:  ',F6.3,' *PRELIM',
     1       ' + ',F6.3,' OLD   ',' = ',5X,'   VLPRED=',I5)

 2144 FORMAT(' PREDICTED VELOCITY INTO LAYER',I3,'            =',F7.3,
     1                                          ' CELLS/(CM OF DR)')

 2146 FORMAT(' RADIUS CHANGE TO LAYER',I3,'           IS       ',F7.3,
     2                                          ' CM')
 2148 FORMAT(' THIS THEN IS A PREDICTED SHIFT OF            ',F7.3,
     2                                           ' CELLS')

 2150 FORMAT(' NO SMOOTHING:',
     2 ' RFX_L(1)[=',I6,'] >/= (1+GAPWD1[=',I2,'])*',
     2 ' (RPWSSF[=',I6,'])..=[',I6,']')
 2151 FORMAT(' NO SMOOTHING:',
     2 ' RFX_L(1)[=',I6,'] < (1+GAPWD1[=',I2,'])*',
     2 ' (RPWSSF[=',I6,'])..=[',I6,'] but LN_SINCE_Z_CHANGE=',I2)
 2152 FORMAT(' SMOOTHING ALLOWED:',
     2 ' RFX_L(1)[=',I6,'] < (1+GAPWD1[=',I2,'])*',
     2 ' (RPWSSF[=',I6,'])..=[',I6,']')
 2153 FORMAT(' SHIFT IN PREDICTED WIRE NUMBER FROM SMOOTHING:',F7.3,
     2                                           ' CELLS')

 2154 FORMAT(' ( DIFTN=',I6,'  ZNOWX=',I6,' )')
 2156 FORMAT(' SHIFT IN PREDICTED WIRE NUMBER FROM "Z" SHIFT:',F7.3,
     2   ' CELLS; (complement is ',F7.3,' cells)')
 2158 FORMAT(' STORED VALUE OF RADIAL PROJECTED WIRE NUMBER:',F7.3)
 2160 FORMAT(' STORED VALUE OF        PREDICTED WIRE NUMBER:',F7.3,
     2 '        (CEL CNTR TEST: ',I4,'</=(IRELX=)',I4,'</=',I4,' ?)')
 2186 FORMAT(' CHAIN IS SHORT, NOT ESTABLISHED,  WIDTH=WIDAD2=',I2)
 2188 FORMAT(' CHAIN IS MEDIUM LENGTH,           WIDTH=WIDAD1=',I2)
 2190 FORMAT(' CHAIN IS LONG ENOUGH TO SET       WIDTH=WIDAD0=',I2)
 2192 FORMAT(' no gap; width does not increase')
 2194 FORMAT(' gap is not significant; width does not increase')
 2196 FORMAT(' GAP CAUSES WIDTH TO INCREASE TO         WIDAD1=',I2)
 2198 FORMAT(' GAP CAUSES WIDTH TO INCREASE TO         WIDAD2=',I2)
 2200 FORMAT(' SEARCH WIDTH BASED ON PREVIOUS FOUND HITS=',I2)
 2202 FORMAT(' GAP, INCLUDING INACTIVE REGION,   WIDTH=WIDAD1=',I2)
 2204 FORMAT(' GAP, INCLUDING INACTIVE REGION,   WIDTH=WIDAD2=',I2)
 2206 FORMAT(' do not test "narrow cells following wide" (width large)')
 2208 FORMAT(' did not find "narrow cells following wide"')
 2210 FORMAT(' NARROW CELLS FOLLOW WIDE CELLS, INCR TO WIDAD2=',I2)
 2212 FORMAT(' no change from stereo shift; not a stereo transition')
 2214 FORMAT(' no change from stereo shift, velocity not established')
 2216 FORMAT(' STEREO SHIFT; MAX PRED SCALING, MIN WIDTH INC = ',I2)
 2218 FORMAT(' STEREO SHIFT; threshold(',I2,')=',I2,' WID INC= ',I2)
 2228 FORMAT(' do not test "too close to cell center" (width large)')
 2230 FORMAT(' did not find "too close to cell center"')
 2232 FORMAT(' TOO CLOSE TO CELL CENTER;   INCREASE TO WIDAD1=',I2)
 2234 FORMAT(' PRELIMINARY SEARCH WIDTH FOR THIS LAYER=',I2) 
 2236 FORMAT(' search width not increased; stereo shift is full') 
 2238 FORMAT(' NON-EXPECTED --- WSHFZC=0 ---') 
 2240 FORMAT(' SCALED STEREO SHIFT; INCREASE "HI" SIDE BY', I3) 
 2242 FORMAT(' SCALED STEREO SHIFT; INCREASE "LO" SIDE BY', I3) 
 2244 FORMAT(' did not find "large velocity"')
 2246 FORMAT(' LARGE VELOCITY; INCREASE "HI" SIDE')
 2248 FORMAT(' LARGE VELOCITY; INCREASE "LO" SIDE')
 2250 FORMAT(' did not find "ignoring implausible doublet"')
 2252 FORMAT(' did not find significant "ignoring implausible doublet"')
 2254 FORMAT(' IGNORING IMPLAUSABLE DOUBLET; INCREASE "HI" SIDE')
 2256 FORMAT(' IGNORING IMPLAUSABLE DOUBLET; INCREASE "LO" SIDE')
 2258 FORMAT(' did not find "large PHICOR ("excess curvature")')
 2260 FORMAT(' PHICOR (EXCESS CURVATURE)  , INCREASE "HI" SIDE')
 2262 FORMAT(' PHICOR (EXCESS CURVATURE)  , INCREASE "LO" SIDE')
 2264 FORMAT(' did not check if radially-out included, ISTORV=',I3)
 2266 FORMAT(' radially-out included, do not change NLO;NHI')
 2268 FORMAT(' correct NHI to include testing radially-out')
 2270 FORMAT(' correct NLO to include testing radially-out')
 2272 FORMAT(' FINAL SEARCH WIDTH:  NHI=',I4,'     NLO=',I4)
 2274 FORMAT(' success history:',3(I4,'/',I2,':',I2),
     2       '  (stragler:',I2,')')
 2278 FORMAT(' >>>>>> HIT NOT FOUND IN CDRAW1IN')
 2280 FORMAT(' >>>>>> HIT LABELED "BAD" IN SFIND')
 2282 FORMAT(' >>>>>> HIT ALREADY IN USE BY ANOTHER SFIND CHAIN')
 2284 FORMAT(' >>>>>> HIT NOT ISOLATED; NOT USED; FOLLOWS NOT ISOLATED')
 2286 FORMAT(' >>>>>> IHQLER=-1 IS NOT SUPPOSED TO BE IMPLIMENTED')
 2288 FORMAT(' >>>>>> GOOD SINGLE HIT')
 2290 FORMAT(' >>>>>> DOUBLE HIT, MAYBE NOT USED IN VELOCITY')
 2292 FORMAT(' >>>>>> UNISOLATED HIT; USE IT ANYWAY')
 2294 FORMAT(' >>>>>> IHQLER=',I5,' IS NOT IMPLIMENTED')
 2300 FORMAT(' DO NOT select trial of LN_GOOD_Z')
 2302 FORMAT(' select trial of LN_GOOD_Z:',
     2     ' Z change while Z test not active')



 2304 FORMAT(' select trial of LN_GOOD_Z:',
     2     ' sufficient length= ',I3)
 2306 FORMAT(' select trial of LN_GOOD_Z:',
     2     ' in axial, sufficient length= ',I3)

 2308 FORMAT(' Z TEST NOT ACTIVE,',
     2     ' DO NOT remove Z TEST based on RADIUS')
 2309 FORMAT(' DO NOT remove Z TEST based on RADIUS:',
     2     ' countZchang=',I3)
 2310 FORMAT(' REMOVE Z TEST: outer radius window set very large')
 2311 FORMAT(' REMOVE Z TEST: at outer radius of chamber')
 2312 FORMAT(' DO NOT remove Z TEST based on Z escape:',
     2     ' Z= ',I6,'  countZchang=',I3)
 2313 FORMAT(' REMOVE Z TEST: Z escape:',
     2     ' Z= ',I6,'  countZchang=',I3)
 2314 FORMAT(' REMOVE Z TEST: Z escape:',
     2     ' suffcnt Z meas=',I3,'  and len since=',I3)
 2315 FORMAT(' DO NOT remove Z TEST:',
     2     ' has Z escape, but Z meas=',I3,'  and len since=',I3)
 2317 FORMAT(' DO NOT remove Z TEST based on Curler:',
     2     ' vel= ',I6,'  Z meas=',I3)
 2318 FORMAT(' REMOVE Z TEST: Curler:',
     2     ' vel= ',I6,' suffcnt length=',I4)
 2319 FORMAT(' DO NOT remove Z TEST',
     2     ' CURLER: vel= ',I6,' BUT INSUFFICIENT length=',I4)
 2324 FORMAT(' DO NOT remove Z TEST:',
     2     ' based on inner radius of chamber')
 2325 FORMAT(' REMOVE Z TEST:',
     2     ' at inner radius of chamber')
 2326 FORMAT(' DO NOT remove Z TEST:',
     2     ' based on sufficient length into axial')
 2327 FORMAT(' remove Z TEST:',
     2     ' extends sufficient length into axial')

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12


      GO TO 101

C JSTORI APPEARS TO BE AN INITIAL LOCAL COUNTER
C KSTORI APPEARS TO BE AN INITIAL ACTUAL STORAGE LOCATION
160   CONTINUE
      PRINT 2015,KSTORI
      KJDIFF=KSTORI-JSTORI
      IF(KJDIFF.LT.0)KJDIFF=KJDIFF+MAXSTR
      JSTORE=JSTORI+1
      KSTORE=KSTORI+1
161   JSTORE=JSTORE-1
      KSTORE=KSTORE-1
C YO, WHEN JSTORE HAS GONE DOWN TO JSSTOP(ZERO?),
C     THE FIRST HIT OF THE CHAIN IS DISPLAYED
C AND WHEN KSTORE HAS GONE DOWN TO ZERO (AND JSTORE DIDN'T) MUST WRAP AROUND
      IF(JSTORE.EQ.JSSTOP)GO TO 189
      IF(KSTORE.EQ.0)KSTORE=MAXSTR

      IF(ISTORE(1,KSTORE).EQ.2)THEN
         PRINT 2004,JSTORE,
     1        ISTORE(3,KSTORE),
     3        ISTORE(11,KSTORE),
     4        ISTORE(13,KSTORE)

      ELSE

         FRAC2=1.* ISTORE(4,KSTORE)/MULST
         CAL1=(1.*ISTORE(5,KSTORE) )/1024.
         CAL2=(1.*ISTORE(6,KSTORE) )/1024.

         IF(ISTORE(1,KSTORE).EQ.0)THEN
            PRINT 2005,JSTORE,
     1        ISTORE( 3,KSTORE),FRAC2,            ISTORE( 2,KSTORE),
     2        CAL1,             CAL2,             ISTORE( 7,KSTORE),
     3        ISTORE( 8,KSTORE),ISTORE(10,KSTORE),ISTORE(11,KSTORE),
     4        ISTORE(13,KSTORE),
     5        (IHSTOR(I,KSTORE),I=1,(IHSTOR(1,KSTORE)+1))
            
         ELSE
            CAL3=(1.*ISTORE(12,KSTORE) )/1024.
            IF(ISTORE(30,KSTORE).EQ.0)THEN
               V_Z='none'
               DISPLAYVZ=0
               DISPLAYLVL=0
            ELSEIF(ISTORE(30,KSTORE).EQ.1)THEN
               V_Z='  V='
               DISPLAYVZ=ISTORE(31,KSTORE)
               DISPLAYLVL=ISTORE(35,KSTORE)
            ELSEIF(ISTORE(30,KSTORE).EQ.2)THEN
               V_Z='  Z='
               DISPLAYVZ=ISTORE(36,KSTORE)
               DISPLAYLVL=0
            ENDIF      

            PRINT 2003,JSTORE,
     1        ISTORE( 3,KSTORE),FRAC2,            ISTORE( 2,KSTORE),
     2        CAL1,             CAL2,             ISTORE( 7,KSTORE),
     3        ISTORE( 8,KSTORE),ISTORE(10,KSTORE),ISTORE(11,KSTORE),
     4        CAL3             ,ISTORE(13,KSTORE),
     5        ISTORE(26,KSTORE),ISTORE(27,KSTORE),
     6        ISTORE(28,KSTORE),ISTORE(29,KSTORE),
     7        V_Z,DISPLAYVZ,DISPLAYLVL,
     8        ISTORE(32,KSTORE),ISTORE(33,KSTORE),ISTORE(34,KSTORE),
     9        ISTORE(37,KSTORE),ISTORE(38,KSTORE),ISTORE(39,KSTORE),
     x        ISTORE(40,KSTORE),ISTORE(41,KSTORE)

         ENDIF
      ENDIF
      
 2015 FORMAT(' DISPLAY FROM STORAGE NUMBER ',I6/
     1     '   # LT CH_R LV RADIAL PRDICT....'
     2     ' STRV VPRED   TEST   FOUND QUAL OTHR',
     3     8X,' vu zu sb ns  V/Z=value Vrestor Ls Lv Lz ac zC LS VS PS')
 2003 FORMAT(I4,I3,F5.1,I3,F7.2,F7.2,I4,I5,I6,I7,
     1     '  (',F5.1,')',I4,5X,
     2     8X,I3,I3,I3,I3,X,A4,I6,I8,I3,I3,I3,I3,I3,I3,I3,I3)
 2005 FORMAT(I4,I3,F5.1,I3,F7.2,F7.2,I4,I5,I6,I7,
     1     ' -FAILED-'   ,I4,I5,
     2     2X,14I4)
 2004 FORMAT(I4,I5,'    TRIAL SEED      ',20X,I7,' -FAILED-',I4)
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C THIS IS TO STOP IF PROGRAM GOES CRAZY
      IF(KSTORE.EQ.NSTORU+1)GO TO 189 ! IT HAS WRAPPED ALL THE WAY AROUND
      IF((KSTORE.EQ.1).AND.(NSTORU.EQ.MAXSTR))GO TO 189 ! ALSO FULL WRAP
      IMORE=1
      GO TO 161

189   CONTINUE
C THIS IS TO RETURN TO CALLING SEQUENCE
      IF(IRETRN.EQ.1)GO TO 101
      IF(IRETRN.EQ.2)GO TO 101
      IF(IRETRN.EQ.3)GO TO 135
      GO TO 101

789   NSTORE=0
      RETURN

#if defined(CLEO_SFDIAG)
      ENTRY DSFHIT0
      KILLA=0
      RETURN
#endif

      END
