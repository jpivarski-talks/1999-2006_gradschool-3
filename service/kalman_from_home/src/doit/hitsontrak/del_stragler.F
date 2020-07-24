* $Id: del_stragler.F,v 1.1 2002/05/23 14:24:11 dpp Exp $
*
* $Log: del_stragler.F,v $
* Revision 1.1  2002/05/23 14:24:11  dpp
*      -> NEW ROUTINE
*      -> delete stragler hits from TFTRAK, compress, but do not refit
*
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE DEL_STRAGLER(REMAIN,RET_ERR)
C.......................................................................
C.
C. DEL_STRAGLER - delete hits at a larger radius than the stragler radius
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 21-March-2002
C. LAST MOD  : 
C.
C. Modification Log.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"

C-----------------------------------------------------------------------
C ARGUMENT DOCUMENTATION
C-----------------------------------------------------------------------

C REMAIN......number of hits that must remain
      INTEGER REMAIN
C RET_ERR.....return non-zero if the number of hits would drop below min
      INTEGER RET_ERR

C-----------------------------------------------------------------------
C LOCAL VARIABLE DOCUMENTATION
C-----------------------------------------------------------------------

C IFIT........INDEX FOR FIT LIST
      INTEGER IFIT
C TYLYR.......TF layer
      INTEGER TFLYR
C NFIT_OK.....number of hits in the fit that are still OK
      INTEGER NFIT_OK
C DEL_HIT.....flag to delete hit
      LOGICAL DEL_HIT(KLR2TF)
      

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      NFIT_OK=NFIT
      RET_ERR=0

      DO 119 IFIT=1,NFIT
         IF(RTF(IPLFIT(IFIT)).GT.RSTRAG)THEN
            DEL_HIT(IFIT)=.TRUE.
            NFIT_OK=NFIT_OK-1
         ELSE
            DEL_HIT(IFIT)=.FALSE.
         ENDIF
 119  CONTINUE


      IF(NFIT_OK.LT.NFIT)THEN
         IF(NFIT_OK.GE.REMAIN)THEN
            DO 129 IFIT=1,NFIT
               IF(DEL_HIT(IFIT))THEN
                  IADFIT(IFIT)=0
               ENDIF
 129        CONTINUE
            CALL COMPRESS_FIT

         ELSE
            RET_ERR=1
         ENDIF
      ENDIF

      IF(RET_ERR.EQ.0)THEN
         RPRLIM=RSTRAG
      ENDIF

      RETURN
      END






