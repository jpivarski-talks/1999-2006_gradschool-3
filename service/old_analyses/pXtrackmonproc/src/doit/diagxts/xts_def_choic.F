*
* $Id: xts_def_choic.F,v 1.3 1999/01/29 15:34:38 dpp Exp $
*
* $Log: xts_def_choic.F,v $
* Revision 1.3  1999/01/29 15:34:38  dpp
* force update
*
* Revision 1.2  1998/05/08 14:13:47  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author :dpp

      SUBROUTINE XTS_DEF_CHOIC(ZERO,NAME,INDEX,WANT,DEPEND,PULLSTART,
     2                         SENSI)
      IMPLICIT NONE
      SAVE

#include "cl3seq/zfiles/usbank.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

      INTEGER ADD_CHOICE_C
      INTEGER START_PULLDOWN_MENU_C
      INTEGER ZERO,INDEX,WANT,DEPEND,PULLSTART,SENSI
      
      CHARACTER*(*) NAME
C ZERO...... ZERO THE STACK OF CHOICES
C INDEX..... RETURNED INDEX NUMBER
C WANT...... FLAG FOR DEPENDENCIES


C-------executable code starts here-------------------------------------

      IF(ZERO.EQ.0)THEN
         NCHOIC=0
         CALL XTS_SENSITIZE(2,0,0,0)

      ELSE
         NCHOIC=NCHOIC+1
         INDEX=NCHOIC
         IF((WANT.NE.0).AND.(DEPEND.EQ.0))GO TO 91
         CHDEPEND(INDEX)=DEPEND
         NAMCHO(INDEX)=NAME
         IF(SENSI.NE.0)CALL XTS_SENSITIZE(4,INDEX,1,0)


         IF(PULLSTART.NE.0)THEN
            LABCHO(INDEX)=START_PULLDOWN_MENU_C(0,NAMCHO(INDEX))
         ELSE
            IF(DEPEND.EQ.0)THEN
               LABCHO(INDEX)=ADD_CHOICE_C(0             ,NAMCHO(INDEX))
            ELSE
               LABCHO(INDEX)=ADD_CHOICE_C(LABCHO(DEPEND),NAMCHO(INDEX))
            ENDIF
         ENDIF
      ENDIF

      GO TO 99
         
 91   CONTINUE
      PRINT 1001,NAME
 1001 FORMAT(' XTS_DEF_CHOIC: error, missing DEPEND, name=',A10)
      GO TO 99

 99   CONTINUE
      RETURN
      END







