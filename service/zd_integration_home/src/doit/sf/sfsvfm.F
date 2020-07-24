*
* $Id: sfsvfm.F,v 1.3 2001/11/20 17:54:10 dpp Exp $
*
* $Log: sfsvfm.F,v $
* Revision 1.3  2001/11/20 17:54:10  dpp
*      -> delete stupid useless nonexistent include file
*
* Revision 1.2  1999/05/28 03:05:04  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.1  1997/02/25 22:03:28  dpp
* first version: fill SF arrays with SFX sparcifier hits
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :  DAN PETERSON
      SUBROUTINE SFSVFM(SHOWCL)
C.......................................................................
C.
C. SFSVGM - fill SVX hit map  as used in SF
C.
C. COMMON    :
C. CALLS     : GTSWHM
C. CALLED    : SFIND
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 
C. LAST MOD  : 
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE

#include "doit/duseq/tfindpar.inc"
#include "seq/clutil/mesrep.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"

C SHOWCL....SHOW WHERE CALLED FROM
C LENSC.....LENTH OF CHARATER STRING =SHOWCL

      CHARACTER*(*) SHOWCL
      INTEGER LENSC ,IOS

C TMPMAP....temporary map returned from  GTSWHM
C IFAIL.....fail code returned from GTSWHM

      INTEGER TMPMAP(16),IFAIL
      INTEGER FIRST,NCALL

      DATA FIRST/1/

C build the link list
      INTEGER I
      INTEGER LLAST(SFSVML)

C unscramble the bits from GTSWHM

C BITLAY....layer number for a BIT#
C BITCEL....relative CELL position for a BIT#
C ADDCEL....added CELL position per WE-WORD# for each bit
C WSTEST....1=WEST; 2=EAST
C WEWORD....WORD number with the WEST or the EAST

      INTEGER BITLAY(13)
      INTEGER BITCEL(13)
      INTEGER ADDCEL(13)
      INTEGER WSTEST(16)
      INTEGER WEWORD(16)

      DATA BITLAY/ 1,1,1,2,2,2,2,3,3,3,3,3,3 /
      DATA BITCEL/ 0,1,2,0,1,2,3,0,1,2,3,4,5 /
      DATA ADDCEL/ 3,3,3,4,4,4,4,6,6,6,6,6,6 / 
      DATA WSTEST/ 1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2 /
      DATA WEWORD/ 1,2,3,4,5,6,7,8,1,2,3,4,5,6,7,8 /

      INTEGER WE,WORD,BIT
      INTEGER LAYER,SFLAYR,CELL,INDEX
      INTEGER TMP1,TMP2,TMP3,HIT
      CHARACTER*1 DISPLA(13)

C ------------- executable code starts here ----------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      IF(FIRST.NE.0)THEN
            LENSC=LEN(SHOWCL)
            IF(LENSC.GT.10)LENSC=10
            WRITE(CHMESS,1101,IOSTAT=IOS)SHOWCL(1:LENSC)
1101        FORMAT(' entry: called from',A10)
            CALL MESLOG('SFSVFM',1,MSSUCC)
            FIRST=0
            NCALL=0
      ENDIF

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

C initialize the first elements

      DO 19 I=1,SFSVNL
 19   SFSV1S(I)=0


C make contact with the data

C-A Lyon-5-24-99      CALL GTSWHM(TMPMAP,IFAIL)
      IFAIL = 1

      NCALL=NCALL+1
      IF(NCALL.LT.10)THEN
            PRINT 2001,IFAIL
 2001       FORMAT(' SFSVFM: returned from GTSWHM: IFAIL=',I4)
      ENDIF      

      SFSVVA=0
      IF(IFAIL.NE.0)THEN
            DO 29 WORD=1,16
29          TMPMAP(WORD)=0
            TMPMAP(1)=530
            TMPMAP(5)=1042
C            GO TO 599
      ENDIF

      SFSVVA=1


C extract the bits

      DO 79 WORD=1,16
            TMP1=TMPMAP(WORD)
            DO 69 BIT=1,13
                  TMP2=TMP1/2
                  TMP3=TMP2*2
                  HIT=TMP1-TMP3
            
                              IF(NCALL.LT.10)THEN
                                    DISPLA(BIT)='.'
                                    IF(HIT.NE.0)DISPLA(BIT)='X'
                              ENDIF

                  LAYER=BITLAY(BIT)
                  CELL= BITCEL(BIT)+(WEWORD(WORD)-1)*ADDCEL(BIT)
                  INDEX=SFSVIN(LAYER,WSTEST(WORD))+CELL

C set the map element
                  SFSVMP(INDEX)=HIT

C set the link list elements, MIXES WEST and EAST
                  IF(HIT.NE.0)THEN
                        IF(SFSV1S(LAYER).EQ.0)THEN
                              SFSV1S(LAYER)=INDEX
                              IF(NCALL.LT.10)THEN
                                    PRINT 2011,LAYER,INDEX
2011                                FORMAT('         ',
     2                                     ' SFSV1S(',I6,')=',I6)
                              ENDIF
                        ELSE
                              SFSVNX(LLAST(LAYER))=INDEX
                              IF(NCALL.LT.10)THEN
                                    PRINT 2012,LAYER,LLAST(LAYER),INDEX
2012                                FORMAT(' LAYER:',I2,
     2                                     ' SFSVNX(',I6,')=',I6)
                              ENDIF
                        ENDIF
                        LLAST(LAYER)=INDEX
                        SFSVNX(INDEX)=0
                  ENDIF

C end of processing this bit
69          TMP1=TMP2
                              IF(NCALL.LT.10)THEN
                                    PRINT 2002,WORD,DISPLA
2002                                FORMAT(' WORD:',I3,'  [',13A1,']')
                              ENDIF
C end of loop over word
79    CONTINUE

599   CONTINUE
      RETURN
      END









