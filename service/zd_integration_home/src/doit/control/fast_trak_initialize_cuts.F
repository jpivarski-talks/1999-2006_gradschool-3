* $Id: fast_trak_initialize_cuts.F,v 1.14 2003/03/05 17:09:20 dpp Exp $
*
* $Log: fast_trak_initialize_cuts.F,v $
* Revision 1.14  2003/03/05 17:09:20  dpp
*      -> now calls one of the detector specific set-up routines
*
* Revision 1.13  2002/09/05 19:37:45  dpp
*      -> move more cuts out of the code
*      -> remove any variables involving "loop over group set"
*      -> remove any variables involving the "offset residual"
*      -> add _check_hits_layers argument
*         to indicate that D0 is probably established
*
* Revision 1.12  2002/05/23 14:19:26  dpp
*      -> require 0 (instead of 2) stereo groups when expecting 0
*         require 1 (instead of 2) stereo groups when expecting 1
*         (but DUFIND will require other stereo layer use in first pass)
*
* Revision 1.11  2001/11/19 22:52:32  dpp
*     -> remove direct access of the event control parameter
*
* Revision 1.10  2001/09/12 21:55:42  dpp
*      -> comments
*      -> reorder and rationalize some of the conditions
*      -> variable name change
*
* Revision 1.9  2000/12/19 23:45:13  dpp
*      -> set variable for limit of Z outside endplate
*      -> set residual maximum to limit width of window in tan(dip)
*      -> set variables to control windows in tan(dip)
*      -> set cuts for separate "u" and "v" stereo layers requirements to
*         "pass z criteria"
*
* Revision 1.8  2000/10/02 20:58:09  dpp
*      -> fix names of Z0BIAS_ and Z0BIAS_USE
*
* Revision 1.7  2000/08/18 18:47:23  dpp
*      -> add D0 constraint based on (lack of) stereo hits
*         to address problem of tracks with only two stereo super layers
*         fitting to a larger curvature rather than a larger Z
*      -> require representation from 2 "Z" superlayers in z criteria
*      -> saturate TANDLF
*
* Revision 1.6  2000/02/12 20:20:50  dpp
*      -> use NEXPT (the doit running level flag) to select Doit or DoitFast
*      -> require more stereo amb groups to allow Z0 to float
*
* Revision 1.5  2000/01/24 19:49:04  dpp
*      -> cosmetic changes only
*
* Revision 1.4  2000/01/06 22:14:01  dpp
* constants for tests of offsets in inner layers
* reduce cut for "delete bad hits" from .24 cells to .14
* increase allowed dZ in alt_amb_2 from .14 to .17
* constants to define large slant loc_amb solutions
* constants for evaluating hit consentration
*
* Revision 1.3  1999/11/23 23:26:18  dpp
* Z0BIAS=0 for first pass, DEWEIGHT_CLOSE_ENUF=100,
* SIN_ENTR_CLOSE=0.90, SIN_ENTR_LOC_AMB=0.95
*
* Revision 1.2  1999/11/17 21:45:17  dpp
* implement cuts and array sizes for _alternat_amb_2
*
* Revision 1.1  1999/11/03 22:43:19  dpp
* new; and add switches for fast version
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_INITIALIZE_CUTS
C.......................................................................
C.
C. FAST_TRAK_INITIALIZE_CUTS - initilize cuts and ambiguity group structure
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split from FAST_TRAK 26-Oct-99
C. LAST MOD  : 15-Sept-88
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/fast_trak.inc"
        
C-----------------------------------------------------------------------
C  Executable code starts here
C-----------------------------------------------------------------------

      CALL EVENT_CONTROL_BITS(EXB_DOIT_FULL,L_FT_DOIT_FULL)
      CALL EVENT_CONTROL_BITS(EXB_DOIT_TPC,L_FT_DOIT_TPC)

C-----------------------------------------------------------------------
C now calls one of the detector specific set-up routine
C-----------------------------------------------------------------------
      IF(L_FT_DOIT_TPC)THEN
        CALL FAST_TRAK_INITIALIZE_TPC1
      ELSE
        IF (IXLRTF(1,IZDA).GT.0) THEN
          CALL FAST_TRAK_INITIALIZ_DR3ZD
        ELSE
          CALL FAST_TRAK_INITIALIZ_DR3Si
        ENDIF
      ENDIF

      RETURN
      END






