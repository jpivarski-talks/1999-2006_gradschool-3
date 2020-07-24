*
* $Id: xts_sensitize.F,v 1.3 1999/01/29 15:34:43 dpp Exp $
*
* $Log: xts_sensitize.F,v $
* Revision 1.3  1999/01/29 15:34:43  dpp
* force update
*
* Revision 1.2  1998/05/08 14:13:51  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :dpp (a sensitive guy)

      SUBROUTINE XTS_SENSITIZE(MODE,INDEX,VALUE,HOLD_INDEX)
      IMPLICIT NONE
      SAVE

#include "cl3seq/zfiles/usbank.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

      INTEGER ADD_CHOICE_C
      INTEGER START_PULLDOWN_MENU_C
      INTEGER MODE,INDEX,VALUE,HOLD_INDEX
      INTEGER MODE1,MODE10,SET,ICHOIC,JCHOIC,LT,MT


C MODE: 0,  restore the set in HOLD,
C                  CURR put into TEMP ; TEMP is left as the old CURR
C                  HOLD put into CURR ; HOLD can be released
C
C       1,  put TEMP into CURR,
C                  CURR put into HOLD ; HOLD is left as the old CURR
C                  TEMP put into CURR ; TEMP is left unchanged  
C
C       2,  add this index to a NEW set in TEMP which is zeroed
C       3,  add this index to a NEW set in TEMP which is based on CURR
C       4,  add this index to the existing set in TEMP 
C     +10,  and set all downward dependencies

C SENSI_TEMP..sensitizing control array
C SENSI_HOLD..sensitizing control array
C SENSI_CURR..sensitizing control array
      INTEGER MSHOLD,MSHTOT
      PARAMETER (MSHOLD=10, MSHTOT=MSHOLD*MAXCHO)
      INTEGER SENSI_TEMP(MAXCHO),SENSI_CURR(MAXCHO),SENSI_TEST(MAXCHO)
      INTEGER SENSI_HOLD(MAXCHO,MSHOLD)
      INTEGER IH,JH,USEHOLD(MSHOLD)
      DATA USEHOLD /MSHOLD*0/
      DATA SENSI_HOLD /MSHTOT*0/
      DATA SENSI_CURR /MAXCHO*1/
C-------executable code starts here-------------------------------------

      MODE10=MODE/10
      MODE1=MODE-MODE10*10
      SET=0
      IF(VALUE.NE.0)SET=1
      IH=1

      IF(MODE1.EQ.0)THEN
         IH=HOLD_INDEX
         IF((IH.LT.1).OR.(IH.GT.MSHOLD))GO TO 94
         IF(USEHOLD(IH).EQ.0)GO TO 95
         CALL UCOPY(SENSI_CURR(1   ),SENSI_TEST(1   ),NCHOIC)
         CALL UCOPY(SENSI_CURR(1   ),SENSI_TEMP(1   ),NCHOIC)
         CALL UCOPY(SENSI_HOLD(1,IH),SENSI_CURR(1   ),NCHOIC)
         USEHOLD(IH)=0
         GO TO 40
      ELSEIF(MODE1.EQ.1)THEN
         DO 15 JH=1,MSHOLD
            IF(USEHOLD(JH).EQ.1)GO TO 15
            IH=JH
            GO TO 17 
 15      CONTINUE
         GO TO 93
 17      HOLD_INDEX=IH
         USEHOLD(IH)=1
         CALL UCOPY(SENSI_CURR(1   ),SENSI_TEST(1   ),NCHOIC)
         CALL UCOPY(SENSI_CURR(1   ),SENSI_HOLD(1,IH),NCHOIC)
         CALL UCOPY(SENSI_TEMP(1   ),SENSI_CURR(1   ),NCHOIC)
         GO TO 40
      ELSEIF(MODE1.EQ.2)THEN
         CALL VZERO(SENSI_TEMP,NCHOIC)
         IH=0
         GO TO 25
      ELSEIF(MODE1.EQ.3)THEN
         CALL UCOPY(SENSI_CURR(1   ),SENSI_TEMP(1   ),NCHOIC)
         IH=0
         GO TO 25
      ELSEIF(MODE1.EQ.4)THEN
         IH=0
         GO TO 25
      ELSE
         IH=0
         GO TO 91
      ENDIF

 25   IF(INDEX.EQ.0)GO TO 99
      SENSI_TEMP(INDEX)=SET
      IF(MODE10.EQ.1)THEN
C set the downward dependencies
C by testing in the upward dependency of each choice
         DO 39 ICHOIC=1,NCHOIC
            IF(ICHOIC.EQ.INDEX)GO TO 39
            IF(SENSI_TEMP(ICHOIC).EQ.SET)GO TO 39
            JCHOIC=ICHOIC
 31         JCHOIC=CHDEPEND(JCHOIC)
            IF(JCHOIC.EQ.INDEX)THEN
               SENSI_TEMP(ICHOIC)=SET
               GO TO 39
            ENDIF
            IF(JCHOIC.EQ.0)GO TO 39
            GO TO 31
 39      CONTINUE
      ENDIF
      GO TO 99

 40   CONTINUE
C fill in missing upward dependencies
      DO 49 ICHOIC=1,NCHOIC
         IF(SENSI_CURR(ICHOIC).EQ.0)GO TO 49
         JCHOIC=ICHOIC
 41      JCHOIC=CHDEPEND(JCHOIC)
         IF(JCHOIC.EQ.0)GO TO 49
         IF(SENSI_CURR(JCHOIC).NE.0)GO TO 41
         SENSI_CURR(JCHOIC)=1
         GO TO 41
 49      CONTINUE

C and set the values

         DO 59 ICHOIC=1,NCHOIC
         IF(SENSI_CURR(ICHOIC).EQ.0)THEN
            IF(SENSI_TEST(ICHOIC).NE.0)
     1      CALL UNSENSITIZE_CHOICE_C(LABCHO(ICHOIC))
         ELSE
            IF(SENSI_TEST(ICHOIC).EQ.0)
     1      CALL RESENSITIZE_CHOICE_C(LABCHO(ICHOIC))
         ENDIF
 59   CONTINUE
      GO TO 99


 91   CONTINUE
      PRINT 1001,MODE,MODE1,MODE10
 1001 FORMAT(' XTS_SENSITIZE: MODE=',I3,' MODE1=',I3,' MODE10=',I3)
      GO TO 99

 93   CONTINUE
      PRINT 1003,MODE
 1003 FORMAT(' XTS_SENSITIZE: MODE=',I3,' no available HOLD')
      GO TO 99

 94   CONTINUE
      PRINT 1004,MODE,HOLD_INDEX
 1004 FORMAT(' XTS_SENSITIZE: MODE=',I3,' input HOLD_INDEX=',I3)
      GO TO 99

 95   CONTINUE
      PRINT 1005,MODE,HOLD_INDEX
 1005 FORMAT(' XTS_SENSITIZE: MODE=',I3,' missing HOLD_INDEX=',I3)
      GO TO 99


 99   CONTINUE

c      TYPE 1101,MODE
c 1101 FORMAT(' XTS_SENSITIVE: MODE=',I3)
c      LT=1
c 101  MT=NCHOIC
c      IF(MT-LT.GT.49)MT=LT+49
c      TYPE 1011,LT,MT,(SENSI_CURR(ICHOIC   ),ICHOIC=1,MT)
c      IF(IH.NE.0)
c     2TYPE 1012,IH,   (SENSI_HOLD(ICHOIC,IH),ICHOIC=1,MT)
c      TYPE 1013,      (SENSI_TEMP(ICHOIC   ),ICHOIC=1,MT)
c 1011 FORMAT(' SENSI_CURR',2I4        ,X,50I1)
c 1012 FORMAT(' SENSI_HOLD',' #',I2,4X ,X,50I1)
c 1013 FORMAT(' SENSI_TEMP',8X         ,X,50I1)
c      IF(MT.LT.NCHOIC)THEN
c         LT=MT+1
c         GO TO 101
c      ENDIF

      RETURN
      END

