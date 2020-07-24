*
* $Id: sfdefn.F,v 1.4 1999/10/06 12:33:24 dpp Exp $
*
* $Log: sfdefn.F,v $
* Revision 1.4  1999/10/06 12:33:24  dpp
* removed assignment SFNEXT maximum start layer
*
* Revision 1.3  1999/04/27 19:39:17  dpp
* in SFNEXT, select chains starting as far out as layer 12
*
* Revision 1.2  1998/07/09 01:00:48  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.6  1997/02/25 21:48:35  dpp
* call the new SFSVGM geometry routine and create SF layers fot SV layers
*
* Revision 1.5  1996/02/29 17:34:09  zfiles
* FORMAT's only
*
* Revision 1.4  1996/02/14 21:04:16  zfiles
* remove call to SFLWF; use SFLSFA etc
*
* Revision 1.3  1996/01/30 19:59:21  zfiles
* Fixed Id and Log
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :  DAN PETERSON
      SUBROUTINE SFDEFN(SHOWCL,WHICH)
C.......................................................................
C.
C. SFDEFN - Initialization configuration dependent varaiables for SFIND
C.
C. COMMON    : /STEPCn/ and others
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 27-Jul-88
C. LAST MOD  : 27-Jul-88
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/sfseq/sfpar.inc"
#include "seq/clutil/mesrep.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"

C ISF.......SF INDEX
C ICD.......CD INDEX
C ITMP......TEMP INDEX
C NSMAX.....MAXIMUM NUMBER IN THE SUBSET FROM WHICH TO CHOOSE SF LAYERS
C SHOWCL....SHOW WHERE CALLED FROM
C LENSC.....LENTH OF CHARATER STRING =SHOWCL

      INTEGER ISF,ICD,ITMP
      INTEGER NWANT,NSMAX
      CHARACTER*(*) SHOWCL,WHICH
      INTEGER LENSC,LENSW,IOS,FIRST,COUNT,COUNTM
      CHARACTER*2 DLABL

      DATA FIRST,COUNT,COUNTM/1,0,10/

C=========================================================================
#include "doit/duseq/duexed.inc"

      IF(FIRST.NE.1)GO TO 5
      LENSC=LEN(SHOWCL)
      IF(LENSC.GT.10)LENSC=10
      LENSW=LEN(WHICH)
      IF(LENSW.GT.8)LENSC=8
      WRITE(CHMESS,1001,IOSTAT=IOS)SHOWCL(1:LENSC),WHICH(1:LENSW)
1001  FORMAT(' entry: called from',A10, ' specify  "',A8,'"')
      CALL MESLOG('SFDEFN',1,MSSUCC)
      IF(COUNT.LT.COUNTM)THEN
         COUNT=COUNT+1
      ELSE
         FIRST=0
      ENDIF
5     CONTINUE


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC                                                                       CC
CCC NOTE: THE SF LAYERS DEFINED AS ONE OF THE FOLLOWING                   CC
CCC       CLEO2:           SF LAYERS ARE THOSE DEFINED AS AXIAL IN CDGEOM CC
CCC       CLEO2:(inc. SVX) SF LAYERS ARE THOSE DEFINED AS AXIAL IN CDGEOM CC
CCC                        plus the svx sparsifiers
CCC       CLEO3:           SF LAYERS ARE THOSE DEFINED AS ANODE IN CDGEOM CC
CCC                                                                       CC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
#if !defined(CLEO_CLEO3)
      SFSVNL=0
      IF(WHICH.EQ.'SFBWAL')CALL SFSVGM('SFDEFN') 
      NSMAX=NAXLCD
#endif
#if defined(CLEO_CLEO3)
      SFSVNL=0
      NSMAX=NCANCD
#endif
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


      CALL VZERO(CDLASF,MLAYSF)
      CALL VZERO(SFLACD,MCDLSF)

C maximum SF layer for devices
      MLSFSV=0
      MLSFPT=0
      MLSFVD=0
      MLSFDR=0

C start to build up number of SF layers
      NLAYSF=0

C loop through the SVX layers adding SF layers
      IF(SFSVNL.LT.1)GO TO 19
      DO 11 ITMP=1,SFSVNL

      IF(NLAYSF.GE.MLAYSF)GO TO 15
      NLAYSF=NLAYSF+1
      CDLASF(NLAYSF)=-ITMP
      SFLASV(ITMP)  =NLAYSF

      MLSFSV=NLAYSF
      DLABL='SV'

#if defined(CLEO_SFDIAG)
      PRINT 1101,NLAYSF,CDLASF(NLAYSF),DLABL
1101  FORMAT(' SFDEFN: CDLASF(',I3,')=',I6,2X,A2)
#endif

11    CONTINUE
C END OF loop through the SVX layers adding SF layers

      WRITE(CHMESS,1112,IOSTAT=IOS)SFSVNL,NLAYSF
1112  FORMAT(' stopped creating SF layers after ',
     2 '[maximum source layers]=',I4,' reached: LAYSF=',I4)
      GO TO 18

15    CONTINUE
      WRITE(CHMESS,1113,IOSTAT=IOS)SFSVNL,NLAYSF
1113  FORMAT(' although [available source layers]=',I4,
     2 ' have reached [SF limit]: NLAYSF=',I4)


18    CONTINUE
      CALL MESLOG('SFDEFN',1,MSSUCC)
19    CONTINUE

C loop through the CD layers adding SF layers
      IF(NSMAX.LT.1)GO TO 29
      DO 21 ITMP=1,NSMAX
C                 you can even put in lines to skip layers, ie,
C                 IF(ITMP.something or other)GO TO 21 or something

#if !defined(CLEO_CLEO3)
      ICD=IAXLCD(ITMP)
#endif
#if defined(CLEO_CLEO3)
      ICD=ICANCD(ITMP)
#endif

      IF((ICD.GT.MCDLSF).OR.(ICD.LE.0))THEN
          WRITE(CHMESS,1215,IOSTAT=IOS)ICD
1215      FORMAT(' bad ICD value=',I5)
          CALL MESLOG('SFDEFN',1,MSERRO)
          GO TO 21
      ENDIF

      IF(NLAYSF.GE.MLAYSF)GO TO 25
      NLAYSF=NLAYSF+1
      CDLASF(NLAYSF)=ICD
      SFLACD(ICD)   =NLAYSF

      IF    (IDVCCD(ICD).EQ.IIVDT)THEN
      MLSFPT=NLAYSF
      DLABL='IV'
      ELSEIF(IDVCCD(ICD).EQ.IPTLA)THEN
      MLSFPT=NLAYSF
      DLABL='PT'
      ELSEIF(IDVCCD(ICD).EQ.IVDET)THEN
      MLSFVD=NLAYSF
      DLABL='VD'
      ELSEIF(IDVCCD(ICD).EQ.IDRFT)THEN
      MLSFDR=NLAYSF
      DLABL='DR'
      ENDIF

#if defined(CLEO_SFDIAG)
      PRINT 1201,NLAYSF,ITMP,ICANCD(ITMP),DLABL
1201  FORMAT(' SFDEFN: CDLASF(',I3,')=ICANCD(',I3,')=',I6,2X,A2)
#endif

21    CONTINUE
C END OF loop through the CD layers adding SF layers

      WRITE(CHMESS,1212,IOSTAT=IOS)NSMAX,NLAYSF
1212  FORMAT(' stopped creating SF layers after ',
     2 '[maximum source layers]=',I4,' reached: LAYSF=',I4)
      GO TO 28

25    CONTINUE
      WRITE(CHMESS,1213,IOSTAT=IOS)NSMAX,NLAYSF
1213  FORMAT(' although [available source layers]=',I4,
     2 ' have reached [SF limit]: NLAYSF=',I4)


28    CONTINUE
      CALL MESLOG('SFDEFN',1,MSSUCC)
29    CONTINUE

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC NOTE: CODE WILL CREATE "SSF" SUPER LAYERS FROM LAYFIL      CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


#if defined(CLEO_SFSSF)  
      NLASSF=0
      DO 101 ISF=1,NLAYSF
      ICD=CDLASF(ISF)

C non-CD layers
      IF(ICD.LT.0)THEN
                  NLASSF=NLASSF+1
                  LAYSSF(ISF)=NLASSF
      ELSE

C CD layers
            IF   (IDVCCD(ICD).EQ.IIVDT)THEN
                  NLASSF=NLASSF+1
                  LAYSSF(ISF)=NLASSF
            ELSEIF(IDVCCD(ICD).EQ.IPTLA)THEN
                  NLASSF=NLASSF+1
                  LAYSSF(ISF)=NLASSF
                  ISF=ISF+1
                  LAYSSF(ISF)=NLASSF
            ELSEIF(IDVCCD(ICD).EQ.IVDET)THEN
                  NLASSF=NLASSF+1
                  LAYSSF(ISF)=NLASSF
            ELSEIF(IDVCCD(ICD).EQ.IDRFT)THEN
                  NLASSF=NLASSF+1
                  LAYSSF(ISF)=NLASSF
            ENDIF
      ENDIF
101   CONTINUE
#endif


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC NOTE: CODE WILL CREATE STANDARD SINGLE LAYER SSF LAYERS    CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

#if !defined(CLEO_SFSSF)
      NLASSF=NLAYSF
      DO 101 ISF=1,NLAYSF
101   LAYSSF(ISF)=ISF
#endif

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

C Minumum number of hits required in SFNEXT for each if the devices

        MNULSV=0
        MNULPT=0
        MNULVD=0
        MNULDR=0

C LALTRD defines use of alternate readout on dual readout layers

#if defined(CLEO_CLEO3)
        LALTRD = .FALSE.
#endif
#if !defined(CLEO_CLEO3)
        LALTRD = LUSVDE
#endif

      LENSC=LEN(SHOWCL)
      WRITE(CHMESS,1909,IOSTAT=IOS)NLAYSF,NLASSF
1909  FORMAT(' exit: NLAYSF=',I4,',   NLASSF=',I4)
      CALL MESLOG('SFDEFN',1,MSSUCC)
      RETURN
      END

















