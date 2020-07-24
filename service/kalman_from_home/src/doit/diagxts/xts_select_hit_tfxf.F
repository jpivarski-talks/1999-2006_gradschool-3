*
* $Id: xts_select_hit_tfxf.F,v 1.3 2000/10/02 20:55:08 dpp Exp $
*
* $Log: xts_select_hit_tfxf.F,v $
* Revision 1.3  2000/10/02 20:55:08  dpp
*      -> do not select hit for coordinates below the lower axis
*
* Revision 1.2  2000/05/26 16:52:56  dpp
*      -> remove unnecessary common
*
* Revision 1.1  2000/04/26 14:51:29  dpp
*      -> NEW
*      -> select a hit in TFXFIT display
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :dpp
      SUBROUTINE XTS_SELECT_HIT_TFXF
      IMPLICIT NONE
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfxfitcd.inc"
C#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"
#include "doit/sfseq/xts_cntl.inc"

      INTEGER SET_BACKING_C
      INTEGER SET_FOREGROUND_C


      INTEGER ID,TYPE,IDUM,YJUST
      REAL QX1,QY1
      INTEGER IHIT
      REAL DIST,MIN_DIST
      LOGICAL SAVE_THIS
      INTEGER ITRACE

C=========================================================================


#if defined(CLEO_XTSDEBUG)
      PRINT 1001
1001  FORMAT(' XTS_SELECT_HIT_TFXF:','enter')
#endif

C request input
      IDUM=SET_BACKING_C(0)
      IDUM=SET_FOREGROUND_C(CLBORD)
      YJUST=1
      YJUST=YJUST+3
      CALL VEC_STRING_C(
     2     RESMIN+RESDIF*.2,
     3     RADMIN+RADDIF*.98,
     3     -1,YJUST,2.0,
     4     'maybe you have to click again',0.)
      CALL UPDATE_C

C request 1st coord
31    CONTINUE

#if defined(CLEO_XTSDEBUG)
      PRINT 1002
1002  FORMAT(' XTS_SELECT_HIT_TFXF: REQUESTING HIT')
#endif

      CALL NEXT_EVENT_C(ID,TYPE,QX1,QY1)

#if defined(CLEO_XTSDEBUG)
      PRINT 1003,ID,TYPE,QX1,QY1
 1003 FORMAT(' XTS_SELECT_HIT_TFXF:',
     1     ' NEXT_EVENT_C returned ID,TYPE=',2I10,
     2     ' {x,y}=',2F12.6)
#endif

      YJUST=YJUST+3
      CALL VEC_STRING_C(
     2     RESMIN+RESDIF*.2,
     3     RADMIN+RADDIF*.98,
     3     -1,YJUST,2.0,
     4     'continuing with screen input',0.)
      IF(TYPE.EQ.0)THEN
         PRINT 1011
 1011    FORMAT(' XTS_SELECT_HIT_TFXF:',
     2        ' main window exposed during click')
         GO TO 31
      ELSEIF((TYPE.NE.1).AND.(TYPE.NE.-1))THEN
         GO TO 31
      ELSEIF(ID.NE.0)THEN
         GO TO 31
      ENDIF

      PRINT 2001,QX1,QY1
 2001 FORMAT(' XTS_SELECT_HIT_TFXF:',
     2     ' found coordinates:',2F12.6)

      HIT_DISPLA=0
      IF(QY1.GT.XAPOSB)THEN
         IF(N_HIT_TFXF.GT.0)THEN
            DO 55 IHIT=1,N_HIT_TFXF
               DIST=SQRT(
     1              ((RESSAV(IHIT)-QX1)/.001)**2+
     2              ((RADSAV(IHIT)-QY1)/.014)**2)
               IF(HIT_DISPLA.EQ.0)THEN
                  SAVE_THIS=.TRUE.
               ELSEIF(DIST.LT.MIN_DIST)THEN
                  SAVE_THIS=.TRUE.
               ELSE
                  SAVE_THIS=.FALSE.
               ENDIF
               IF(SAVE_THIS)THEN
                  HIT_DISPLA=IHIT
                  MIN_DIST=DIST
               ENDIF
 55         CONTINUE
         ENDIF
      ENDIF

      PRINT 2002,RESSAV(HIT_DISPLA),RADSAV(HIT_DISPLA),HIT_DISPLA
 2002 FORMAT(' XTS_SELECT_HIT_TFXF:',
     2     ' MATCH coordinates:',2F12.6,'  HIT=',I4)
      
#if defined(CLEO_XTSDEBUG)
      IF(HIT_DISPLA.EQ.0)THEN
         PRINT 2011
 2011    FORMAT(' XTS_SELECT_HIT_TFXF:',' match no hit')
      ELSE
         IF(N_TRACER(HIT_DISPLA).EQ.0)THEN
            PRINT 2012,HIT_DISPLA
 2012       FORMAT(' XTS_SELECT_HIT_TFXF:',' hit',I3,'  has no log')
         ELSE
            DO 65 ITRACE=1,N_TRACER(HIT_DISPLA)
               PRINT 2013,HIT_DISPLA,TRACER(ITRACE,HIT_DISPLA),
     2              TRACER_NUM(1,ITRACE,HIT_DISPLA),
     3              TRACER_NUM(2,ITRACE,HIT_DISPLA)
 2013          FORMAT(' XTS_SELECT_HIT_TFXF:',' hit',I3,'->',
     2              A30,X,I6,X,I6)
 65         CONTINUE
         ENDIF
      ENDIF
#endif

#if defined(CLEO_XTSDEBUG)
      PRINT 4001
4001  FORMAT(' XTS_SELECT_HIT_TFXF: EXIT')
      RETURN
#endif

      END



