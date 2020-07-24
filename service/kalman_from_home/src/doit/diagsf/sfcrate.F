*
* $Id: sfcrate.F,v 1.1.1.1 1998/02/06 19:11:41 dpp Exp $
*
* $Log: sfcrate.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:03  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.34.57  by  Rob Kutschke
*CMZ :  6.00/03 08/08/93  22.20.22  by  Rob Kutschke
*CMZ :  6.00/01 18/03/93  17.31.31  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.32  by  Dan Peterson
*-- Author :
      SUBROUTINE SFCRATE
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif
C CRATE NAMES
      INTEGER NAMCR(0:16),NAMCRI(0:99)
C
C
C DATA READ IN FROM LNS62::UD:[DPP.CLEO2]CRATOU.___
C        CRATE#("40),CARD,  DRLAY/PREAMP,  TOP/BOT
      INTEGER INPUT1(16,  20,2,2),INTEMP(20,2,2)
      INTEGER INPUTC
C
C  CRATE#, CARD#, 2HALVES  --> LAYER,PREAMP
      INTEGER LAYPRE(16,20,2,  2)
C
C  LAYER#, PREAMP#{ (WIRE+24)/24 }
C           --> CRATE#{1:16},CARD,2HALVESC
      INTEGER ICRATE(51,16,   3)
C
C  MISCELLANEOUS
      INTEGER I, IREAD, IOPEN, IUNIT, NCRATE, NCARDL, ICARD
      INTEGER ILP, ITB, LAY, IPREAM, KLAY, KCRT, KCRD, KTB, KPRE
      INTEGER LAYTST, IPREMT
      CHARACTER*4 ISTAR
      DATA IUNIT/16/
C
      PRINT 3006
3006  FORMAT(' SFCRATE: LETS JUST FORGET IT, OK? ...<CR> or "N"O')
#if !defined(CLEO_XTSUBS)
      READ 3007,INPUTC
3007  FORMAT(A2)
#endif
#if defined(CLEO_XTSUBS)
      CALL XTS_RFV_INPUT
      INPUTC=XTS_ANSWER_IY
#endif
      IF(INPUTC.NE.'N')GO TO 195
C LOAD UP THE NAME CRATE INVERSE ARRAY
      DO 12 I=0,99
12    NAMCRI(I)=0
      NAMCR(0)=-1
C
      IREAD=0
      IOPEN=1
      PRINT 1005
1005  FORMAT(' SFCRATE: WILL OPEN .www')
      OPEN(UNIT=IUNIT,
     1     FILE='input/cratou.www',
     2     STATUS='OLD',READONLY)
      GO TO 16
15    IOPEN=2
      PRINT 1006
1006  FORMAT(' SFCRATE: WILL OPEN .eee')
      OPEN(UNIT=IUNIT,
     1     FILE='input/cratou.eee',
     2     STATUS='OLD',READONLY)
16    CONTINUE
      PRINT 1007
1007  FORMAT(' SFCRATE: START TO READ')
18    CONTINUE
      READ(IUNIT,1001,END=95,ERR=91)
     1 INPUTC,INTEMP
1001  FORMAT(1X             /
     2       3X,I2,7X,20I3  /
     3       12X,     20I3  /
     4       1X             /
     5       12X,     20I3  /
     6       12X,     20I3  )
C     PRINT 1002,INPUTC,INTEMP
C1002  FORMAT(' <<<',1X           ,'>>>'  /
C     2       ' <<<',3X,I2,7X,20I3,'>>>'  /
C     3       ' <<<',12X,     20I3,'>>>'  /
C     4       ' <<<',1X           ,'>>>'  /
C     5       ' <<<',12X,     20I3,'>>>'  /
C     6       ' <<<',12X,     20I3,'>>>'  )
      IREAD=IREAD+1
      NAMCR(IREAD)=INPUTC
      NAMCRI(INPUTC)=IREAD
C
C STORE THE DATA,  COMPRESSED
      NCRATE=NAMCRI(INPUTC)
      NCARDL=0
      DO 23 ICARD=1,20
      IF(INTEMP(ICARD,2,1).NE.0)GO TO 20
      IF(INTEMP(ICARD,2,2).NE.0)GO TO 20
      GO TO 23
20    NCARDL=NCARDL+1
      DO 21 ILP=1,2
      DO 21 ITB=1,2
      INPUT1(NCRATE,NCARDL,ILP,ITB)=
     1 INTEMP(      ICARD ,ILP,ITB)
21    CONTINUE
23    CONTINUE
C      PRINT 1003,IREAD,INPUTC,
C     1 (((INPUT1(NCRATE,ICARD,ILP,ITB),ICARD=1,20)
C     2                                   ,ILP=1,2)
C     3                                   ,ITB=1,2)
1003  FORMAT(1X,I2,'   INPUT1'/
     2       3X,I2,7X,20I3  /
     3       12X,     20I3  /
     4       1X             /
     5       12X,     20I3  /
     6       12X,     20I3  )
C
C
      DO 35 ICARD=1,NCARDL
      DO 34 ITB=1,2
C
C  LAYER#, PREAMP#{ (WIRE+24)/24 }
C           --> CRATE#{1:16},CARD,2HALVESC
C     DIMENSION ICRATE(51,16,   3)
      LAY=    INPUT1(NCRATE,ICARD,1,ITB)
      IPREAM= INPUT1(NCRATE,ICARD,2,ITB)
      IF(IPREAM.EQ.0)GO TO 34
C
      ICRATE(LAY,IPREAM,1)=NCRATE
      ICRATE(LAY,IPREAM,2)=ICARD
      ICRATE(LAY,IPREAM,3)=ITB
C
      DO 33 ILP=1,2
C
C  CRATE#, CARD#, 2HALVES  --> LAYER,PREAMP
C     DIMENSION LAYPRE(16,20,2,  2)
       LAYPRE(NCRATE,ICARD,ITB,ILP)=
     1 INPUT1(NCRATE,ICARD,ILP,ITB)
C
33    CONTINUE
34    CONTINUE
35    CONTINUE
C
C
      GO TO 18
C
C
C ERROR IN FILE
91    CONTINUE
      PRINT 3001
3001  FORMAT(' SFCRATE:ERROR DURING READ')
      GO TO 190
C END OF FILE
95    CONTINUE
      PRINT 3002
3002  FORMAT(' SFCRATE: END OF FILE DURING READ')
C CLOSE AND EXIT
97    CONTINUE
      CLOSE(UNIT=IUNIT)
      IF(IOPEN.EQ.1)GO TO 15
C
      DO 119 LAY=1,51
      DO 117 IPREAM=1,16
      IF(ICRATE(LAY,IPREAM,1).EQ.0)GO TO 119
      LAYTST=LAYPRE(ICRATE(LAY,IPREAM,1),
     1              ICRATE(LAY,IPREAM,2),
     2              ICRATE(LAY,IPREAM,3),1)
      IPREMT=LAYPRE(ICRATE(LAY,IPREAM,1),
     1              ICRATE(LAY,IPREAM,2),
     2              ICRATE(LAY,IPREAM,3),2)
      ISTAR='    '
      IF(LAYTST.NE.LAY)ISTAR='****'
      IF(IPREMT.NE.IPREAM)ISTAR='****'
C      PRINT 4001,LAY,IPREAM,NAMCR(ICRATE(LAY,IPREAM,1)),
C     1                           ICRATE(LAY,IPREAM,2),
C     2                           ICRATE(LAY,IPREAM,3),
C     3  LAYTST,IPREMT,ISTAR
4001  FORMAT(' L/C:',I2,'/',I2,'  ---> CC "',I2,I3,I2,
     1  ' -->',I3,'/',I2,2X,A4)
117   CONTINUE
119   CONTINUE
190   CONTINUE
      PRINT 4002
4002  FORMAT(' SFCRATE: EXIT')
195   RETURN
C
C
      ENTRY SF_CRATE_TO_PREAMP(KCRT,KCRD,KTB,KLAY,KPRE)
      KLAY=LAYPRE(NAMCRI(KCRT),KCRD,KTB,1)
      KPRE=LAYPRE(NAMCRI(KCRT),KCRD,KTB,2)
      RETURN
      ENTRY SF_PREAMP_TO_CRATE(KLAY,KPRE,KCRT,KCRD,KTB)
      KCRT=NAMCR(ICRATE(KLAY,KPRE,1))
      KCRD=      ICRATE(KLAY,KPRE,2)
      KTB=       ICRATE(KLAY,KPRE,3)
      RETURN
C
      END










