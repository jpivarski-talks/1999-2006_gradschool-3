* $Id: compress_fit.F,v 1.6 2001/11/19 23:57:18 dpp Exp $
*
* $Log: compress_fit.F,v $
* Revision 1.6  2001/11/19 23:57:18  dpp
*      -> add new control parameter include file
*      -> fills LUSETF directly, LUSETF not passed in argument
*      -> set NHITTF to NFIT for STAK operations
*
* Revision 1.5  2000/10/02 20:45:25  dpp
*      -> transfer program indicator
*
* Revision 1.4  1999/12/10 22:21:53  dpp
* add DWTFIT
*
* Revision 1.3  1999/12/09 21:35:20  dpp
* change call to hit_to_fit to use parameter directive, really
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE COMPRESS_FIT
C.......................................................................
C.
C. COMPRESS_FIT - Compress the Fit list in tftrakcd.inc
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 3-Sept-99
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
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"

C-----------------------------------------------------------------------
C LOCAL VARIABLE DOCUMENTATION
C-----------------------------------------------------------------------

C IFIT.....INDEX FOR FIT LIST
C NFITS....save NFIT for compressing
      INTEGER IFIT,NFITS

C executable code starts here

C this array is returned to identify TF layers with hits

      CALL VFILL(LUSETF,KLYRTF, .FALSE.)

C save the old number of hits for the loop
C zero NFIT and HITTRK

      NFITS = NFIT
      CALL HIT_TO_FIT(H2F_ZERO_FIT,0,0)

C loop to rebuild the arrays

      DO 789 IFIT = 1, NFITS
      IF(IADFIT(IFIT).NE.0)THEN

         NFIT=NFIT+1
         IADFIT(NFIT) = IADFIT(IFIT)
         IPLFIT(NFIT) = IPLFIT(IFIT)
         DWTFIT(NFIT) = DWTFIT(IFIT)
         SRCFIT(NFIT) = SRCFIT(IFIT)
         XFIT(NFIT)   = XFIT(IFIT)
         YFIT(NFIT)   = YFIT(IFIT)
         SFIT(NFIT)   = SFIT(IFIT)
         ZFIT(NFIT)   = ZFIT(IFIT)
         PULFIT(NFIT) = PULFIT(IFIT)
         DFIT(NFIT)   = DFIT(IFIT) 
         MESFIT(NFIT) = MESFIT(IFIT)
         STRFFT(NFIT) = STRFFT(IFIT) 
         STSZFT(NFIT) = STSZFT(IFIT)
         TRKHIT(NFIT) = TRKHIT(IFIT) 

         HITTRK(TRKHIT(NFIT)) = NFIT 

         LUSETF(IPLFIT(NFIT))=.TRUE.

      ENDIF
 789  CONTINUE

C change value of NHITTF set in last FIT
      NHITTF=NFIT

      RETURN
      END






