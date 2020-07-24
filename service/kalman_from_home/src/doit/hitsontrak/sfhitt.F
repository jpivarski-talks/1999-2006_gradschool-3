*
* $Id: sfhitt.F,v 1.3 2000/02/12 20:30:49 dpp Exp $
*
* $Log: sfhitt.F,v $
* Revision 1.3  2000/02/12 20:30:49  dpp
*      -> extra drift distance from cut set in SFTCUT
*
* Revision 1.2  1999/11/22 21:02:52  dpp
*  apply multiplyer to the cut for drift distance
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :D. Peterson
      SUBROUTINE SFHITT(IADR,  LYRSF, ILYRL, LYRTF, ISER,  IWIRE, GOOD)

C.......................................................................
C.
C. SFHITT - test hits from SFHITS
C.
C. COMMON    : ?
C. CALLS     :
C. CALLED    : Only by SFHITS
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 11-Jan-96
C. LAST MOD  :
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C== Argument declarations
* IADR......TF address of hit to test
* LYRSF.....SF LAYER
* ILYRL.....PREVIOUS SF LAYER
* LYRTF.....TF LAYER
* ISER......SEQUENCE IN SEARCH
* IWIRE.....TESTED WIRE NUMBER
* GOOD......FLAG IF WIRE IS ACCEPTED

      INTEGER IADR,LYRSF,ILYRL,LYRTF,ISER,IWIRE,GOOD

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"


C== Local variables

C DLIMIT....DRIFT DISTANCE LIMIT

      REAL DLIMIT

C---------------------------------------------------------------------
C  Executable code starts here
C---------------------------------------------------------------------


C tests of hit quality and space

      GOOD=0
      IF( IADRHW(IADR).GT.0 )THEN
         DLIMIT=.55*CELLTF(LYRTF)*RTF(LYRTF)+DRIFT_EXTRA_HI

#if defined(CLEO_SFHITDIAG)
         PRINT 5004,ISER,IWIRE,IADR,IADRHW(IADR),
     1              DWCD(IADRHW(IADR)),DLIMIT
5004     FORMAT(' SFHITT: SER',I3,' WIR#',I3,' IADRHW(',I6,')=',I5,
     1          '  DWCD=',F9.6,' <?',F9.6)
#endif

         IF(DWCD(IADRHW(IADR)).LE.DLIMIT)THEN
            IF( NHTWIR.LT.NHITDM )THEN

C increment number of layers with hits 
               IF( LYRSF.NE.ILYRL )THEN
                  ILYRL  = LYRSF
                  NLYRWR = NLYRWR + 1
               ENDIF

               NHTWIR   = NHTWIR + 1
               HTWADR(NHTWIR) = IADRHW(IADR)
               GOOD=1
            ENDIF
         ENDIF

#if defined(CLEO_SFHITDIAG)
      ELSE
         PRINT 5006,ISER,IWIRE,IADR,IADRHW(IADR)
5006     FORMAT(' SFHITT: SER',I3,' WIR#',I3,' IADRHW(',I6,')=',I5)
#endif

      ENDIF
      END







