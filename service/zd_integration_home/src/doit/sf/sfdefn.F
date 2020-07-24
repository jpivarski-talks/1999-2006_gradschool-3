*
* $Id: sfdefn.F,v 1.6 2003/03/05 18:35:13 dpp Exp $
*
* $Log: sfdefn.F,v $
* Revision 1.6  2003/03/05 18:35:13  dpp
*      -> remove (null) cuts on number of hits in individual devices in SFNEXT
*      -> clean up some use of compile-time switches
*
* Revision 1.5  2001/11/20 17:53:34  dpp
*     -> delete stupid useless nonexistent include file
*
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

C SHOWCL....SHOW WHERE CALLED FROM
      CHARACTER*(*) SHOWCL,WHICH

C ISF.........SF INDEX
C ICD.........CD INDEX
C ISV.........SV index
C ISF2........INDEX FOR SECONDARY LOOP OVER SF LAYERS
C IUSE........index of layers used in SF
      INTEGER ISF,ISV,ICD,ISF2,IUSE
C NSMAX.......MAXIMUM NUMBER IN THE SUBSET FROM WHICH TO CHOOSE SF LAYERS
      INTEGER NSMAX
C LENSC.......LENTH OF CHARATER STRING =SHOWCL
      INTEGER LENSC,LENSW,IOS

      INTEGER FIRST,COUNT,COUNTM
      INTEGER NWANT
      CHARACTER*2 DLABL
      LOGICAL GETMORE

C CDLASF_TMP..temporary for ordering
      INTEGER CDLASF_TMP

      DATA FIRST,COUNT,COUNTM/1,0,10/

C ------------- executable code starts here ----------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      IF(FIRST.EQ.1)THEN

        LENSC=LEN(SHOWCL)
        IF(LENSC.GT.10)LENSC=10
        LENSW=LEN(WHICH)
        IF(LENSW.GT.8)LENSC=8
        WRITE(CHMESS,1001,IOSTAT=IOS)SHOWCL(1:LENSC),WHICH(1:LENSW)
 1001   FORMAT(' entry: called from',A10, ' specify  "',A8,'"')
        CALL MESLOG('SFDEFN',1,MSSUCC)

        WRITE(CHMESS,1002,IOSTAT=IOS)NCANCD
 1002   FORMAT(' entry: NCANCD=',I6)
        CALL MESLOG('SFDEFN',1,MSSUCC)


        IF(COUNT.LT.COUNTM)THEN
          COUNT=COUNT+1
        ELSE
          FIRST=0
        ENDIF
      ENDIF


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCC                                                                       CC
CCC NOTE: THE SF LAYERS DEFINED AS ONE OF THE FOLLOWING                   CC
CCC       CLEO2:           SF LAYERS ARE THOSE DEFINED AS AXIAL IN CDGEOM CC
CCC       CLEO2:(inc. SVX) SF LAYERS ARE THOSE DEFINED AS AXIAL IN CDGEOM CC
CCC                        plus the svx sparsifiers
CCC       CLEO3:           SF LAYERS ARE THOSE DEFINED AS ANODE IN CDGEOM CC
CCC                                                                       CC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
#if defined(CLEO_CLEO3)
      SFSVNL=0
      NSMAX=NCANCD
#else
      SFSVNL=0
      IF(WHICH.EQ.'SFBWAL')CALL SFSVGM('SFDEFN') 
      NSMAX=NAXLCD
#endif
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


      CALL VZERO(CDLASF,MLAYSF)
      CALL VZERO(SFLACD,MCDLSF)

C maximum SF and SSF layer for devices
      MLSFSV=0
      MLSFPT=0
      MLSFVD=0
      MLSFZD=0
      MLSFDR=0
      MLSSFSV=0
      MLSSFPT=0
      MLSSFVD=0
      MLSSFZD=0
      MLSSFDR=0

C start to build up number of SF layers
      NLAYSF=0


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C loop through the (non CD) SVX layers adding SF layers
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      IF(SFSVNL.GE.1)THEN

        ISV=0
 11     ISV=ISV+1
        IF(ISV.LE.SFSVNL)THEN
          
          IF(NLAYSF.LT.MLAYSF)THEN
            NLAYSF=NLAYSF+1
            CDLASF(NLAYSF)=-ISV
            SFLASV(ISV)  =NLAYSF
            
            MLSFSV=NLAYSF
            GETMORE=.TRUE.
            DLABL='SV'
            
#if defined(CLEO_SFDIAG)
            PRINT 1101,NLAYSF,CDLASF(NLAYSF),DLABL
 1101       FORMAT(' SFDEFN: CDLASF(',I3,')=',I6,2X,A2)
#endif
            
C end of condition: NLAYSF.LT.MLAYSF
          ELSE
            GETMORE=.FALSE.
            WRITE(CHMESS,1113,IOSTAT=IOS)SFSVNL,NLAYSF
 1113       FORMAT(' although [available source layers]=',I4,
     2           ' have reached [SF limit]: NLAYSF=',I4)
          ENDIF
          
C end of loop; end of condition:ISV.LE.SFSVNL
        ELSE
          GETMORE=.FALSE.
          WRITE(CHMESS,1112,IOSTAT=IOS)SFSVNL,NLAYSF
 1112     FORMAT(' stopped creating SF layers after ',
     2         '[maximum source layers]=',I4,' reached: LAYSF=',I4)
        ENDIF
        IF(GETMORE)GO TO 11
        CALL MESLOG('SFDEFN',1,MSSUCC)
C end of condition: SFSVNL.GE.1
      ENDIF


      
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C loop through the CD layers adding SF layers
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      IF(NSMAX.GE.1)THEN
        ICD=0
 21     ICD=ICD+1
        IF(ICD.LE.NLYRCD)THEN

C                 you can even put in lines to skip layers, ie,
C                 IF(ICD.something or other)GO TO 21 or something
          
#if !defined(CLEO_CLEO3)
          IUSE=IAXLCD(ICD)
#endif
#if defined(CLEO_CLEO3)
          IUSE=ICANCD(ICD)
#endif
          
          IF((IUSE.LE.MCDLSF).AND.(IUSE.GE.1))THEN
            IF(NLAYSF.LT.MLAYSF)THEN
              NLAYSF=NLAYSF+1
              CDLASF(NLAYSF)=ICD
              SFLACD(ICD)   =NLAYSF
              GETMORE=.TRUE.
              
              IF    (IDVCCD(ICD).EQ.IIVDT)THEN
                MLSFPT=NLAYSF
                DLABL='IV'
              ELSEIF(IDVCCD(ICD).EQ.IPTLA)THEN
                MLSFPT=NLAYSF
                DLABL='PT'
              ELSEIF(IDVCCD(ICD).EQ.IVDET)THEN
                MLSFVD=NLAYSF
                DLABL='VD'
              ELSEIF(IDVCCD(ICD).EQ.IZDA)THEN
                MLSFZD=NLAYSF
                DLABL='ZD'
              ELSEIF(IDVCCD(ICD).EQ.IDRFT)THEN
                MLSFDR=NLAYSF
                DLABL='DR'
              ENDIF
              
#if defined(CLEO_SFDIAG)
              PRINT 1201,NLAYSF,ICD,DLABL
 1201         FORMAT(' SFDEFN: CDLASF(',I3,')=',I9,2X,A2)
#endif
              
C end of condition: NLAYSF.LT.MLAYSF
            ELSE
              GETMORE=.FALSE.
              WRITE(CHMESS,1213,IOSTAT=IOS)NSMAX,NLAYSF
 1213         FORMAT(' although [available source layers]=',I4,
     2             ' have reached [SF limit]: NLAYSF=',I4)
            ENDIF
            
C end of condition: (IUSE.LE.MCDLSF).AND.(IUSE.GE.1)
          ELSE
            GETMORE=.TRUE.

#if defined(CLEO_SFDIAG)
              PRINT 1202,ICD,IUSE
 1202         FORMAT(' SFDEFN: CDlayer ',I3,' non valid SF layer,',
     2             '  pointer=',I9)
#endif
          ENDIF

C end of loop; end of condition: ICD.LE.NLYRCD
        ELSE
          GETMORE=.FALSE.
          WRITE(CHMESS,1212,IOSTAT=IOS)NLYRCD,NLAYSF
 1212     FORMAT(' stopped creating SF layers after ',
     2         '[maximum source layers]=',I4,' reached: LAYSF=',I4)
        ENDIF
        IF(GETMORE)GO TO 21
        CALL MESLOG('SFDEFN',1,MSSUCC)

C end of condition: NSMAX.GE.1
      ENDIF

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C reorder layers by radius (CD layers only)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      IF(NLAYSF.GE.1)THEN
        DO 59 ISF=1,NLAYSF
          IF(CDLASF(ISF).GT.0)THEN
            IF(ISF.LT.NLAYSF)THEN
              DO 55 ISF2=(ISF+1),NLAYSF
                IF(CDLASF(ISF2).GT.0)THEN
                  IF(RCD(CDLASF(ISF2)).LT.RCD(CDLASF(ISF)))THEN
                    CDLASF_TMP=CDLASF(ISF)
                    CDLASF(ISF)=CDLASF(ISF2)
                    CDLASF(ISF2)=CDLASF_TMP

                    SFLACD(CDLASF(ISF))=ISF
                    SFLACD(CDLASF(ISF2))=ISF2
                  
                  ENDIF
                ENDIF
 55           CONTINUE
            ENDIF
#if defined(CLEO_SFDIAG)
            PRINT 1505,ISF,CDLASF(ISF)
 1505       FORMAT(' SFDEFN: reorder: CDLASF(',I3,')=',I4)
#endif
          
          ENDIF
 59     CONTINUE
      ENDIF

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC NOTE: CODE WILL CREATE "SSF" SUPER LAYERS                  CCCCCCCC
CCCCCCCC         from hard-wired code                               CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


#if defined(CLEO_SFSSF)  
      NLASSF=0
      ISF=0
 61   ISF=ISF+1
      IF(ISF.LE.NLAYSF)THEN
        ICD=CDLASF(ISF)
        
C non-CD layers
        IF(ICD.LT.0)THEN
          NLASSF=NLASSF+1
          LAYSSF(ISF)=NLASSF
        ELSE
          
C CD layers
          IF   (IDVCCD(ICD).EQ.IIVDT)THEN
            NLASSF=NLASSF+1
            MLSSFPT=NLASSF
            LAYSSF(ISF)=NLASSF

C note: only PT is set up for non-trivial SSF layers
          ELSEIF(IDVCCD(ICD).EQ.IPTLA)THEN
            NLASSF=NLASSF+1
            MLSSFPT=NLASSF
            LAYSSF(ISF)=NLASSF
            ISF=ISF+1
            LAYSSF(ISF)=NLASSF

          ELSEIF(IDVCCD(ICD).EQ.IVDET)THEN
            NLASSF=NLASSF+1
            MLSSFVD=NLASSF
            LAYSSF(ISF)=NLASSF
          ELSEIF(IDVCCD(ICD).EQ.IZDA)THEN
            NLASSF=NLASSF+1
            MLSSFZD=NLASSF
            LAYSSF(ISF)=NLASSF
          ELSEIF(IDVCCD(ICD).EQ.IDRFT)THEN
            NLASSF=NLASSF+1
            MLSSFDR=NLASSF
            LAYSSF(ISF)=NLASSF
          ENDIF
        ENDIF
        GO TO 61
      ENDIF
      
      
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCC NOTE: CODE WILL CREATE STANDARD SINGLE LAYER SSF LAYERS    CCCCCCCC
CCCCCCCC                                                            CCCCCCCC
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

#else
      NLASSF=0
      DO 101 ISF=1,NLAYSF
        NLASSF=ISF
        ICD=CDLASF(ISF)
        LAYSSF(ISF)=ISF

        IF(ICD.GT.0)THEN
          IF   (IDVCCD(ICD).EQ.IIVDT)THEN
            MLSSFPT=NLASSF
          ELSEIF(IDVCCD(ICD).EQ.IPTLA)THEN
            MLSSFPT=NLASSF
          ELSEIF(IDVCCD(ICD).EQ.IVDET)THEN
            MLSSFVD=NLASSF
          ELSEIF(IDVCCD(ICD).EQ.IZDA)THEN
            MLSSFZD=NLASSF
          ELSEIF(IDVCCD(ICD).EQ.IDRFT)THEN
            MLSSFDR=NLASSF
          ENDIF
        ENDIF

 101  CONTINUE
#endif

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

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

















