* $Id: fast_trak_amb_grp_to_trak.F,v 1.8 2002/09/05 19:33:56 dpp Exp $
*
* $Log: fast_trak_amb_grp_to_trak.F,v $
* Revision 1.8  2002/09/05 19:33:56  dpp
*      -> remove any code involving "loop over group set"
*
* Revision 1.7  2001/11/19 22:55:24  dpp
*     -> add new control parameter include file
*
* Revision 1.6  2001/09/12 21:56:24  dpp
*      -> expand use of program indicator
*
* Revision 1.5  1999/12/09 21:07:29  dpp
* change call to hit_to_fit to use parameter directive
* zero the third argument to HIT_TO_FIT(H2F_NEW_ENTRY...
*
* Revision 1.4  1999/11/23 23:23:31  dpp
*  diagnostic
*
* Revision 1.3  1999/11/14 18:20:24  dpp
* uses KAPHLD to start each representation with consistant track parameters
*
* Revision 1.2  1999/11/04 16:55:50  dpp
* put switch on new print statement
*
* Revision 1.1  1999/11/03 22:42:41  dpp
* new
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_AMB_GRP_TO_TRAK
C.......................................................................
C.
C. FAST_TRAK_AMB_GRP_TO_TRAK - transfer from ambiguity group data structure
C.                             to /TFTRAK/
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split off from FAST_TRAK 26-Oct-99
C. LAST MOD  : 
C.
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
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfstakcd.inc"

#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

* INDX_GROUP...........counter of ambiguity group number
                       INTEGER INDX_GROUP
* INDX_HIT.............counter of hit number
                       INTEGER INDX_HIT

C-----------------------------------------------------------------------
C temp values
C-----------------------------------------------------------------------

* LYR_NUMBER...........TF layer number
                       INTEGER LYR_NUMBER
* LOGICAL_LYR_IN_HITS..logical layer number in "road" data structure
                       INTEGER LOGICAL_LYR_IN_HITS
* IHITSV...............save the hit number that is to be fitted
                       INTEGER IHITSV
* INDX_SOL.............temp value of INDX_SOL_USE
                       INTEGER INDX_SOL

C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

      PROGRAM_INDICATOR=P_I_F_T_AMB_GRP_TO_TRAK

      CALL UCOPY(KAPHLD,KAPCF,NPARTF)

      CALL HIT_TO_FIT(H2F_ZERO_FIT,0,0)
      HITBSC=0
      HITASC=0
      TF_LAYR_MIN=999
      TF_LAYR_MAX=0
      CALL VZERO(FLAG_LYR_USED,NLYRHT)

#if defined(CLEO_FTDIAG)
      PRINT 9881,(
     2     (INDX_GROUP),
     3     INDX_GROUP=1,NUMB_OF_GROUPS)
      PRINT 9882,(INDX_SOL_USE
     2     (INDX_GROUP),
     3     INDX_GROUP=1,NUMB_OF_GROUPS)
      PRINT 9883,(NUM_HITS_SAV_SOL
     2     (1,INDX_GROUP),
     3     INDX_GROUP=1,NUMB_OF_GROUPS)
      PRINT 9883,(NUM_HITS_SAV_SOL
     2     (2,INDX_GROUP),
     3     INDX_GROUP=1,NUMB_OF_GROUPS)
 9881 FORMAT(' FAST_TRAK:                I=',20I4)
 9882 FORMAT(' FAST_TRAK:     INDX_SOL_USE=',20I4)
 9883 FORMAT(' FAST_TRAK: NUM_HITS_SAV_SOL=',20I4)
#endif

      DO 185 INDX_GROUP=1,NUMB_OF_GROUPS
         INDX_SOL=INDX_SOL_USE(INDX_GROUP)
         IF(INDX_SOL.GT.0)THEN
            DO 184 INDX_HIT=1,NUM_HITS_SAV_SOL
     2           (INDX_SOL,INDX_GROUP)
               LYR_NUMBER=LYR_SAV_SOL
     2              (INDX_HIT,INDX_SOL,INDX_GROUP)
               IHITSV=HITS_SAV_SOL
     2              (INDX_HIT,INDX_SOL,INDX_GROUP)
               
               LOGICAL_LYR_IN_HITS=ILLRLR(LYR_NUMBER)
               FLAG_LYR_USED(LOGICAL_LYR_IN_HITS)=2
               IF(RTF(LYR_NUMBER).LT.RKN6CF)THEN
                  HITBSC=HITBSC+1
               ELSE
                  HITASC=HITASC+1
               ENDIF
               CALL HIT_TO_FIT(H2F_NEW_ENTRY,IHITSV,0)
               IF(LYR_NUMBER.LT.TF_LAYR_MIN)TF_LAYR_MIN=LYR_NUMBER
               IF(LYR_NUMBER.GT.TF_LAYR_MAX)TF_LAYR_MAX=LYR_NUMBER
 184        CONTINUE
         ENDIF

#if defined(CLEO_XTSUBS)
C        CALL XTS_TRAK('FT',0,'FAST_TRAK just another group')
#endif

 185  CONTINUE
      RETURN
      END





