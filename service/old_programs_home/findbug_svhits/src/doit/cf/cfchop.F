*
* $Id: cfchop.F,v 1.3 2000/04/26 14:47:12 dpp Exp $
*
* $Log: cfchop.F,v $
* Revision 1.3  2000/04/26 14:47:12  dpp
*     -> use FIT_WHATEVER instead of CFDFIT or CFRFT4, or SZ_FITTER
*
* Revision 1.2  1999/09/12 22:37:40  dpp
* remove argument in call to CFRFT4
*
* Revision 1.1  1998/02/24 18:29:20  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE CFCHOP
C.......................................................................
C.
C. CFXFIT - Drop hits that are beyond the last reasonable (CFLAST) layer.
C.
C. COMMON    : ?
C. CALLED    : ?
C. AUTHOR    : S. Schafner
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

C=======================================================================
C CFCHOP is called after CFLAST has been run; it deletes hits from
C TFTRAK that are beyond the endplate escape point (or where we
C think the escape point is).  The hit array is then repacked.
C
C=======================================================================

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tftrkscd.inc"
#include "seq/clutil/mesrep.inc"

C     RCCHOP -- radial distance beyond nominal escape point that we'll accept
C     REPACK -- .TRUE. => repack hits before leaving
C     CHOP_OK -- .TRUE. if it is reasonable to chop a few hits

      LOGICAL REPACK,CHOP_OK
      INTEGER IHIT,NFITS,ILYR,NKILLU
      REAL RESC
      LOGICAL HAVEHIT(KLYRTF)

C     Some day there will be a real message logging utility.
      INTEGER NMES
      SAVE    NMES
      DATA    NMES / 0 /

* ----------Executable code starts here---------------------

C Save number of hits

      NFITS = NFIT
      IF (ILRESC(2) .GT. 0) THEN
         RESC = RENDTF(ILRESC(2))

C Test version: loop over hits, check for LUSE layers to be killed, and
C quit if there will be too many of them

         NKILLU = 0
         DO 50 IHIT = 1,NFIT
            ILYR = IPLFIT(IHIT)
            IF (RENDTF(ILYR) .GT. RESC+RCCHOP)NKILLU = NKILLU + 1
 50      CONTINUE
         
         CHOP_OK=.TRUE.
         IF (NFIT-NKILLU .LT. MNHTCF) THEN
            CHOP_OK=.FALSE.
            NMES = NMES + 1
            IF ( NMES .LE. 10 ) THEN
               CHMESS = 'Lost too many layers'
               CALL MESLOG('CFCHOP',1,MSWARN)
            ENDIF
         END IF
         
         IF(CHOP_OK)THEN

C Loop over the hits

            REPACK = .FALSE.
            DO 100 IHIT = 1,NFIT
               ILYR = IPLFIT(IHIT)
               IF (RENDTF(ILYR) .GT. RESC+RCCHOP) THEN
                  IADFIT(IHIT) = 0
                  REPACK = .TRUE.
               ENDIF
 100        CONTINUE
            
C Repack hit array, if necessary
            
            IF (REPACK)THEN
               CALL COMPRESS_FIT(HAVEHIT)
C Refit
               CALL FIT_WHATEVER
               NHITTF = NFIT
               CHITF  = CHICF
            ENDIF

         ENDIF
      ENDIF
      RETURN
      END


