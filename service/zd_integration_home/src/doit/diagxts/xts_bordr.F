*
* $Id: xts_bordr.F,v 1.7 2003/03/05 17:24:30 dpp Exp $
*
* $Log: xts_bordr.F,v $
* Revision 1.7  2003/03/05 17:24:30  dpp
*      -> diagnostic format
*
* Revision 1.6  2002/09/05 19:07:32  dpp
*      -> diagnostics
*
* Revision 1.5  2000/02/12 20:25:45  dpp
*      -> display r-phi view superlayer borders with different color
*
* Revision 1.4  1999/01/29 15:34:37  dpp
* force update
*
* Revision 1.3  1999/01/11 02:46:49  lyon
* SunOS does not allow CALLing functions
*
* Revision 1.2  1998/07/09 00:27:57  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.2  1997/02/25 21:34:41  dpp
* change loop limit to NBOUNDS
*
* Revision 1.1.1.1  1994/10/08 01:00:59  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.12.41  by  Rob Kutschke
*CMZ :  6.00/01 29/01/92  09.51.17  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.40  by  Dan Peterson
*-- Author :dpp


      SUBROUTINE XTS_BORDR(CALLER)
      IMPLICIT NONE

#include "cl3seq/cdgm3/cdgeompa.inc"
#include "doit/duseq/runev.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"

      INTEGER SET_BACKGROUND_C
      INTEGER SET_FOREGROUND_C
      INTEGER SET_BACKING_C
      INTEGER LINES_C

      REAL PI
      PARAMETER (PI=3.14159265)
      REAL XX(100),YY(100)
      CHARACTER*(*)CALLER
      INTEGER IDUM
      INTEGER I,ITHETA
      REAL THETA
      INTEGER OLD_TYPE
      CHARACTER*9 STRING
      INTEGER L,IERR
      REAL X_RUN,Y_RUN,SIZE_RUN

#if defined(CLEO_XTSDEBUG)
      PRINT 1001,CALLER
 1001 FORMAT(' XTS_BORDR: ENTER: CALLED BY ',A20)
#endif

      IDUM=SET_BACKGROUND_C(CLZERO)
      IDUM=SET_BACKING_C(1)
      CALL ERASE_C
      OLD_TYPE=0
      

#if defined(CLEO_XTSDEBUG)
      PRINT 1002,NBOUNDS
 1002 FORMAT(' XTS_BORDR: found NBOUNDS=',I4)
#endif

      DO 49 I=1,NBOUNDS

#if defined(CLEO_XTSDEBUG)
        PRINT 1003,I,TYP_BOUND(I),OLD_TYPE
 1003   FORMAT(' XTS_BORDR: TYP_BOUND(',I2,')=',I3,'   old=',I3)
#endif

         IF(TYP_BOUND(I).NE.OLD_TYPE)THEN
            IF(TYP_BOUND(I).EQ.1)THEN
               IDUM=SET_FOREGROUND_C(CLBORD)
            ELSEIF(TYP_BOUND(I).EQ.2)THEN
               IDUM=SET_FOREGROUND_C(CLSUBB)
            ENDIF
            OLD_TYPE=TYP_BOUND(I)
         ENDIF
         DO 39 ITHETA=1,100
            THETA=2.*PI*(ITHETA-1)/99.
            XX(ITHETA)=RBOUNDS(I)*COS(THETA)
            YY(ITHETA)=RBOUNDS(I)*SIN(THETA)
 39      CONTINUE

#if defined(CLEO_XTSDEBUG)
        PRINT 1004,I
 1004   FORMAT(' XTS_BORDR: calling LINES_C for border number ',I4)
#endif

         IDUM=LINES_C(0,XX,YY,100)
 49   CONTINUE

      IDUM=SET_FOREGROUND_C(CLLET1)
      ZOOMU=ZOOMU_RPHI
      X_RUN=ZOOMXY(3,ZOOMU)+  .02 * ZOOMXY(1,ZOOMU)
      Y_RUN=ZOOMXY(4,ZOOMU)+  .96 * ZOOMXY(2,ZOOMU)
      SIZE_RUN=2.6

      WRITE(STRING,1011,IOSTAT=IERR)CURRENT_RUN
 1011 FORMAT('r',I8)
      L=LEN(STRING)
      CALL VEC_STRING_C(X_RUN,Y_RUN,-1,-1,SIZE_RUN,STRING(1:L),0.)

      WRITE(STRING,1012,IOSTAT=IERR)CURRENT_EVENT
 1012 FORMAT('e',I8)
      L=LEN(STRING)
      CALL VEC_STRING_C(X_RUN,Y_RUN,-1, 2,SIZE_RUN,STRING(1:L),0.)

      CALL UPDATE_C

#if defined(CLEO_XTSDEBUG)
      PRINT 1029
 1029 FORMAT(' XTS_BORDR: EXIT')
#endif

      RETURN
      END













