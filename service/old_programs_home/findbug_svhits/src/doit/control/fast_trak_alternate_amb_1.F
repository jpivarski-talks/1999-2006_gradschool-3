* $Id: fast_trak_alternate_amb_1.F,v 1.2 2000/04/26 14:30:43 dpp Exp $
*
* $Log: fast_trak_alternate_amb_1.F,v $
* Revision 1.2  2000/04/26 14:30:43  dpp
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*
* Revision 1.1  1999/11/03 22:44:03  dpp
* new
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_ALTERNATE_AMB_1
C.......................................................................
C.
C. FAST_TRAK_ALTERNATE_AMB_1
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : split off of FAST_TRAK 28-Oct-99
C. LAST MOD  : 
C.
C. Modification Log.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfstakcd.inc"

#include "doit/duseq/fast_trak.inc"

      EXTERNAL JBIT
      INTEGER JBIT

C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

* INDX_GROUP...........counter of ambiguity group number
                       INTEGER INDX_GROUP
* INDX_SOL_SET.........index of the alternate solution set
                       INTEGER INDX_SOL_SET

C-----------------------------------------------------------------------
C other local variables
C-----------------------------------------------------------------------

C VALID_ALT_SOL_SET....=.TRUE. if alternate solution set is valid to use
                       LOGICAL VALID_ALT_SOL_SET(DIM_ALT_SOL_SET)

C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

      INDX_SOL_SET=0
 181  INDX_SOL_SET=INDX_SOL_SET+1
      IF(INDX_SOL_SET.LE.MAX_INDX_SOL_SET)THEN
            
#if defined(CLEO_FTDIAG)
         PRINT 1003
 1003    FORMAT(
     1' --------------------------------------------------------------')
         PRINT 1122,INDX_GROUP_SET,INDX_SOL_SET
 1122    FORMAT(' FAST_T: START AMB GROUPS, INDX_GROUP_SET=',I3,
     2          ' ,  INDX_SOL_SET=',I3)
#endif


         VALID_ALT_SOL_SET(INDX_SOL_SET)=.TRUE.
         INDX_GROUP=0
 182     INDX_GROUP=INDX_GROUP+1
         IF((INDX_GROUP.LE.NUMB_OF_GROUPS(INDX_GROUP_SET)).AND.
     2        VALID_ALT_SOL_SET(INDX_SOL_SET))THEN
            
            IF(INDX_GROUP.LE.NUMB_GR_ALTER)THEN
               INDX_SOL_USE(INDX_GROUP)
     2                     =JBIT(INDX_SOL_SET-1,INDX_GROUP)+1
            ELSE
               INDX_SOL_USE(INDX_GROUP)=1
            ENDIF
            
            IF(NUM_HITS_SAV_SOL(INDX_SOL_USE(INDX_GROUP),INDX_GROUP)
     2         .LE.0)THEN
               IF(INDX_SOL_USE(INDX_GROUP).EQ.1)THEN
                  INDX_SOL_USE(INDX_GROUP)=0
               ELSE
                  VALID_ALT_SOL_SET(INDX_SOL_SET)=.FALSE.
               ENDIF
            ENDIF
            
            GO TO 182
         ENDIF
            

         IF(VALID_ALT_SOL_SET(INDX_SOL_SET))THEN
            CALL FAST_TRAK_AMB_GRP_TO_TRAK
               
#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1121,INDX_GROUP_SET,INDX_SOL_SET,NFIT
 1121       FORMAT(' FAST_T:   DID AMB GROUPS, INDX_GROUP_SET=',I3,
     2           ' ,  INDX_SOL_SET=',I3,' , NFIT=',I3)
#endif

#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK('CF',0,
     2                    'FAST_TRAK alternate ambiguity groups')
#endif
            
            CALL FAST_TRAK_CLOSE_FIT_STORE
         ENDIF

         GO TO 181
      ENDIF

      RETURN
      END



