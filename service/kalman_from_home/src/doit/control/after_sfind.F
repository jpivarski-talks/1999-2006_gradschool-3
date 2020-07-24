*
* $Id: after_sfind.F,v 1.1 2001/11/19 23:00:10 dpp Exp $
*
* $Log: after_sfind.F,v $
* Revision 1.1  2001/11/19 23:00:10  dpp
*      -> NEW ROUTINE
*      -> control of further processing after SFIND
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D Peterson
      SUBROUTINE AFTER_SFIND(LRETRN)
C.......................................................................
C.
C. AFTER_SFIND - decision for DUFIND to continue after SFIND
C.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.0
C. CREATED   : 16-11.2001
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

      LOGICAL LRETRN
      INTEGER IY
      INTEGER AFTRSF/0/

* ----------Executable code starts here---------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

#if defined(CLEO_SFDIAG)
         IF(AFTRSF.EQ.-1)THEN
            LRETRN=.FALSE.
         ELSEIF(AFTRSF.EQ. 1)THEN
            LRETRN=.TRUE.
         ELSE
            PRINT 1042
            CALL DSF_RFV_INPUT(IY)
#if defined(CLEO_XTSUBS)
            IF(XTS_ABORT.EQ.1)IY='N'
#endif
            IF(IY.EQ.'Y')THEN
               LRETRN=.TRUE.
            ELSEIF(IY.EQ.'A')THEN
               LRETRN=.TRUE.
               AFTRSF= 1
            ELSEIF(IY.EQ.'N')THEN
               LRETRN=.FALSE.
            ELSEIF(IY.EQ.'K')THEN
               LRETRN=.FALSE.
               AFTRSF=-1
            ELSE
               LRETRN=.TRUE.
            ENDIF
         ENDIF
#else
         LRETRN=.TRUE.
#endif
         IF(LRETRN)THEN
            PRINT 1044
         ELSE
            PRINT 1043
         ENDIF

 1042    FORMAT(' AFTER_SFIND: do you want continue after SFIND ?'/
     2        ' "-->Y"es, "N"o, "A"lways, or  "K"ill')
 1043    FORMAT(' AFTER_SFIND: DUFIND will return after SFIND')
 1044    FORMAT(' AFTER_SFIND: DUFIND will continue after SFIND')


      RETURN
      END



