* $Id: fast_trak_check_hit_layrs.F,v 1.7 2001/11/19 22:54:20 dpp Exp $
*
* $Log: fast_trak_check_hit_layrs.F,v $
* Revision 1.7  2001/11/19 22:54:20  dpp
*      -> add new control parameter include file
*      -> options for control of setting D0 and Z0 contraints
*
* Revision 1.6  2000/12/19 23:45:54  dpp
*      -> apply separate "u" and "v" stereo layer requirements to
*         "pass z criteria"
*
* Revision 1.5  2000/10/02 20:58:40  dpp
*      -> set Z0BIAS directly, eliminate Z0_CONSTRAIN
*
* Revision 1.4  2000/08/18 18:46:56  dpp
*      -> add D0 constraint based on (lack of) stereo hits
*      -> for the first time, allow D0_CONS=.TRUE.
*      -> fix error in expected number of layers for large NEGATIVE TANSLF
*
* Revision 1.3  2000/04/26 14:29:30  dpp
*      -> remove setting D0_CONS=.TRUE. for now
*         it had not been being used after the initila wire fit anyway
*
* Revision 1.2  2000/01/24 19:48:33  dpp
*      -> fixed error: had no ABS on Z test
*         NOTE: This would set a Z constraint on any -Z track with less than
*               5 super layers in stereo. It also eliminates, until the last
*               pass, any -Z track also with less than 5 super layers in Z.
*      -> more diagnostics
*
* Revision 1.1  2000/01/06 22:15:15  dpp
* evaluate number of hits  in axial and stereo
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_CHECK_HIT_LAYRS(
     2     SET_D0CON,SET_Z0_BIAS,PASS_Z_CRIT)
C.......................................................................
C.
C. FAST_TRAK_CHECK_HIT_LAYRS - check if enough layers are used to
C.                          allow D0 and Z0 to float
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : ?
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

#include "doit/sfseq/xts_cntl.inc"

#include "doit/duseq/fast_trak.inc"

C-----------------------------------------------------------------------
C arguments
C-----------------------------------------------------------------------

* SET_D0CON............control to allow setting the D0 constraint
                       INTEGER SET_D0CON

* SET_Z0_BIAS..........control to allow setting the Z0 bias
                       INTEGER SET_Z0_BIAS

* PASS_Z_CRIT..........=.TRUE. if pass Z criteria
                       LOGICAL PASS_Z_CRIT

C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------


* INDX_GROUP...........counter of ambiguity group number
                       INTEGER INDX_GROUP

* IFIT.................counter of hits in fit
                       INTEGER IFIT

C ILYR.................serial TF layer counter
                       INTEGER ILYR


C-----------------------------------------------------------------------
C other variables
C-----------------------------------------------------------------------

C LYR_USED_LOCAL.......=.TRUE. if the layer is already used,
C                      avoid double counting
                       LOGICAL LYR_USED_LOCAL(KLYRTF)

C ACURV................ABS(curvature)
                       REAL ACURV

C R_CURL...............radius ofthe curl over
                       REAL R_CURL

C R_Z_EXIT.............radius of z exit out of endplate
                       REAL R_Z_EXIT

C Z0BIAS_TMP...........temporary value of Z0BIAS
                       REAL Z0BIAS_TMP

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------

      AX_LYR_HIT=0
      ST_LYR_HIT=0
      ST_U_LYR_HIT=0
      ST_V_LYR_HIT=0
      CALL VZERO(HITS_IN_AMB_GR,DIM_GROUP)      
      AX_AMB_GR_HIT=0
      ST_AMB_GR_HIT=0

      AX_LYR_HIT_EXPCT=0
      ST_LYR_HIT_EXPCT=0
      ST_U_LYR_HIT_EXPCT=0
      ST_V_LYR_HIT_EXPCT=0
      AX_AMB_GR_HIT_EXPCT=0
      ST_AMB_GR_HIT_EXPCT=0
C-----------------------------------------------------------------------
C count the layers with hits    and hits in the ambiguity groups
C-----------------------------------------------------------------------
      
      CALL VFILL(LYR_USED_LOCAL,NLYRTF,.FALSE.)
      DO 159 IFIT = 1, NFIT
         ILYR=IPLFIT(IFIT)
         IF(.NOT.LYR_USED_LOCAL(ILYR))THEN
            
            IF(  
     2           (ILYR.GE.FIRST_AXIAL_LYR).AND.
     3           (ILYR.LE.LAST_AXIAL_LYR))THEN
               AX_LYR_HIT=AX_LYR_HIT+1
            ELSEIF(
     2           (ILYR.GE.FIRST_STEREO_LYR).AND.
     3           (ILYR.LE.LAST_STEREO_LYR))THEN
               ST_LYR_HIT=ST_LYR_HIT+1
               IF(TANSTF(ILYR).GT. 0.005)THEN
                  ST_U_LYR_HIT=ST_U_LYR_HIT+1
               ELSEIF(TANSTF(ILYR).LT.-0.005)THEN
                  ST_V_LYR_HIT=ST_V_LYR_HIT+1
               ENDIF
            ENDIF

            DO 149 INDX_GROUP=1,NUMB_OF_GROUPS(INDX_GROUP_SET)

               IF(  
     2              (ILYR.GE.
     2              LYR_FIRST_IN_GR(INDX_GROUP,INDX_GROUP_SET)).AND.
     3              (ILYR.LE.
     3              LYR_LAST_IN_GR(INDX_GROUP,INDX_GROUP_SET)))THEN
                  HITS_IN_AMB_GR(INDX_GROUP)=
     2                 HITS_IN_AMB_GR(INDX_GROUP)+1
               ENDIF
 149        CONTINUE
            
            LYR_USED_LOCAL(ILYR)=.TRUE.
         ENDIF
 159  CONTINUE

C-----------------------------------------------------------------------
C count the groups with significant hits
C-----------------------------------------------------------------------

      DO 179 INDX_GROUP=1,NUMB_OF_GROUPS(INDX_GROUP_SET)
         IF(HITS_IN_AMB_GR(INDX_GROUP).GE.SIGNIF_HITS_AMB_GR)THEN
            IF(
     2           (INDX_GROUP.GE.FIRST_AXIAL_AMB_GR).AND.
     3           (INDX_GROUP.LE.LAST_AXIAL_AMB_GR))THEN
               AX_AMB_GR_HIT=AX_AMB_GR_HIT+1
            ELSEIF(
     2           (INDX_GROUP.GE.FIRST_STEREO_AMB_GR).AND.
     3           (INDX_GROUP.LE.LAST_STEREO_AMB_GR))THEN
               ST_AMB_GR_HIT=ST_AMB_GR_HIT+1
            ENDIF
         ENDIF
 179  CONTINUE

C-----------------------------------------------------------------------
C find the last reasonable layers to expect hits
C-----------------------------------------------------------------------

      ACURV=ABS(CURCF)
      IF(ACURV.GT. .8)THEN
         R_CURL=ABS(1./CURCF+D0CF)
      ELSE
         R_CURL=999.
      ENDIF

C-----------------------------------------------------------------------
C loop through the layers counting the expected layers with hits
C
C  (it is a fake to use RTF instead of arc length with YANDLF )
C
C-----------------------------------------------------------------------

      DO 259 ILYR=1,NLYRTF

         IF(
     2        (RTF(ILYR).LE.R_CURL).AND.
     3        ( ABS(Y0LF+TANDLF*RTF(ILYR)).LE.ZENDTF(ILYR)))THEN

            IF(  
     2           (ILYR.GE.FIRST_AXIAL_LYR).AND.
     3           (ILYR.LE.LAST_AXIAL_LYR))THEN
               AX_LYR_HIT_EXPCT=AX_LYR_HIT_EXPCT+1
            ELSEIF(
     2           (ILYR.GE.FIRST_STEREO_LYR).AND.
     3           (ILYR.LE.LAST_STEREO_LYR))THEN
               ST_LYR_HIT_EXPCT=ST_LYR_HIT_EXPCT+1
               IF(TANSTF(ILYR).GT. 0.005)THEN
                  ST_U_LYR_HIT_EXPCT=ST_U_LYR_HIT_EXPCT+1
               ELSEIF(TANSTF(ILYR).LT.-0.005)THEN
                  ST_V_LYR_HIT_EXPCT=ST_V_LYR_HIT_EXPCT+1
               ENDIF
            ENDIF

         ENDIF
 259  CONTINUE

C-----------------------------------------------------------------------
C loop through the ambiguity groups counting the expected groups with hits
C   count a group if it can possibly have SIGNIF_HITS_AMB_GR+1
C-----------------------------------------------------------------------

      DO 279 INDX_GROUP=1,NUMB_OF_GROUPS(INDX_GROUP_SET)
         ILYR=LYR_FIRST_IN_GR(INDX_GROUP,INDX_GROUP_SET)
     2        +SIGNIF_HITS_AMB_GR

         IF(
     2        (RTF(ILYR).LE.R_CURL).AND.
     3        ( ABS(Y0LF+TANDLF*RTF(ILYR)).LE.ZENDTF(ILYR)))THEN

            IF(  
     2           (INDX_GROUP.GE.FIRST_AXIAL_AMB_GR).AND.
     3           (INDX_GROUP.LE.LAST_AXIAL_AMB_GR))THEN
               AX_AMB_GR_HIT_EXPCT=AX_AMB_GR_HIT_EXPCT+1
            ELSEIF(
     2           (INDX_GROUP.GE.FIRST_STEREO_AMB_GR).AND.
     3           (INDX_GROUP.LE.LAST_STEREO_AMB_GR))THEN
               ST_AMB_GR_HIT_EXPCT=ST_AMB_GR_HIT_EXPCT+1
            ENDIF

         ENDIF
 279  CONTINUE

C-----------------------------------------------------------------------
C assign the constraints
C-----------------------------------------------------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

      IF(SET_D0CON.EQ.FT_SET_D0CON)THEN
         IF(
     1        (AX_LYR_HIT    
     1        .GE. F_AX_LYR_HT_REQ_D0*AX_LYR_HIT_EXPCT)
     2        .AND.
     2        (AX_AMB_GR_HIT 
     2        .GE. AX_AMB_GR_HT_REQ_D0(AX_AMB_GR_HIT_EXPCT))
     3        .AND.
     3        (ST_LYR_HIT    
     3        .GE. F_ST_LYR_HT_REQ_D0*AX_LYR_HIT_EXPCT)
     4        .AND.
     4        (ST_AMB_GR_HIT 
     4        .GE. ST_AMB_GR_HT_REQ_D0(AX_AMB_GR_HIT_EXPCT))
     9        )THEN
            D0_CONS=.FALSE.
         ELSE
            D0_CONS=.TRUE.
         ENDIF
      ELSEIF(SET_D0CON.EQ.FT_NO_D0CON)THEN
         D0_CONS=.FALSE.
      ELSE
#if defined(CLEO_FTDIAG)
         PRINT 1011,SET_D0CON
 1011    FORMAT(' FAST_TRAK_CHECK_HIT_LAYRS: invalid SET_D0CON=',I5) 
#endif
      ENDIF

      IF(SET_Z0_BIAS.EQ.FT_SET_Z0BIAS_USE)THEN
         Z0BIAS=Z0BIAS_USE
      ELSE
         IF( 
     1        (ST_LYR_HIT    
     1        .GE. F_ST_LYR_HT_REQ_Z0*ST_LYR_HIT_EXPCT)
     2        .AND.
     2        (ST_AMB_GR_HIT 
     2        .GE. ST_AMB_GR_HT_REQ_Z0(ST_AMB_GR_HIT_EXPCT))
     9        )THEN
            Z0BIAS_TMP=Z0BIAS_USE
         ELSE
            Z0BIAS_TMP=Z0BIAS_FULL
         ENDIF
         IF(SET_Z0_BIAS.EQ.FT_SET_Z0BIAS)THEN
            Z0BIAS=Z0BIAS_TMP
         ELSEIF(SET_Z0_BIAS.EQ.FT_NO_MORE_Z0BIAS)THEN
            IF(Z0BIAS_TMP.GT.Z0BIAS)Z0BIAS=Z0BIAS_TMP
         ELSE
#if defined(CLEO_FTDIAG)
            PRINT 1012,SET_Z0_BIAS
 1012       FORMAT(' FAST_TRAK_CHECK_HIT_LAYRS:invalid SET_Z0_BIAS=',I5) 
#endif
         ENDIF
      ENDIF

      IF( 
     1     (ST_LYR_HIT    .GE. F_ST_LYR_HT_REQ_PZ*ST_LYR_HIT_EXPCT)
     2     .AND.
     1     (ST_U_LYR_HIT  .GE. F_ST_U_LYR_HT_REQ_PZ*ST_U_LYR_HIT_EXPCT)
     2     .AND.
     1     (ST_V_LYR_HIT  .GE. F_ST_V_LYR_HT_REQ_PZ*ST_V_LYR_HIT_EXPCT)
     2     .AND.
     2     (ST_AMB_GR_HIT .GE. ST_AMB_GR_HT_REQ_PZ(ST_AMB_GR_HIT_EXPCT))
     9     )THEN
         PASS_Z_CRIT=.TRUE.
      ELSE
         PASS_Z_CRIT=.FALSE.
      ENDIF

#if defined(CLEO_FTDIAG)
      PRINT 1003
      PRINT 1001,
     2     CURCF,D0CF,TANDLF,Y0LF,
     3     AX_LYR_HIT,F_AX_LYR_HT_REQ_D0,AX_LYR_HIT_EXPCT,
     4     AX_AMB_GR_HIT,AX_AMB_GR_HT_REQ_D0(AX_AMB_GR_HIT_EXPCT),
     5     AX_AMB_GR_HIT_EXPCT,
     3     ST_LYR_HIT,F_ST_LYR_HT_REQ_D0,ST_LYR_HIT_EXPCT,
     4     ST_AMB_GR_HIT,ST_AMB_GR_HT_REQ_D0(ST_AMB_GR_HIT_EXPCT),
     5     ST_AMB_GR_HIT_EXPCT,
     6     D0_CONS,
     3     ST_LYR_HIT,F_ST_LYR_HT_REQ_Z0,ST_LYR_HIT_EXPCT,
     4     ST_AMB_GR_HIT,ST_AMB_GR_HT_REQ_Z0(ST_AMB_GR_HIT_EXPCT),
     5     ST_AMB_GR_HIT_EXPCT,
     6     Z0BIAS,
     3     ST_LYR_HIT,F_ST_LYR_HT_REQ_PZ,ST_LYR_HIT_EXPCT,
     4     ST_AMB_GR_HIT,ST_AMB_GR_HT_REQ_PZ(ST_AMB_GR_HIT_EXPCT),
     5     ST_AMB_GR_HIT_EXPCT,
     3     ST_U_LYR_HIT,F_ST_U_LYR_HT_REQ_PZ,ST_U_LYR_HIT_EXPCT,
     3     ST_V_LYR_HIT,F_ST_V_LYR_HT_REQ_PZ,ST_V_LYR_HIT_EXPCT,
     6     PASS_Z_CRIT
 1003 FORMAT(
     1' --------------------------------------------------------------')
 1001 FORMAT(
     1     ' FAST_TRAK_CHECK_HIT_LAYRS:',
     2     ' CUR=',F7.2,' D0=',F6.3,' DZ=',F6.3,' Z0=',F6.3/
     3     '     axial hits=',I2,' require ',F4.2,'*',I3,
     4     '     ax_amb_grp=',I2,' require ',I2,' out of ',I3/
     3     '    stereo hits=',I2,' require ',F4.2,'*',I3,
     4     '     st_amb_grp=',I2,' require ',I2,' out of ',I3,
     5     '  D0_CONSTRAIN=',L1/
     3     '    stereo hits=',I2,' require ',F4.2,'*',I3,
     4     '     st_amb_grp=',I2,' require ',I2,' out of ',I3,
     5     '  Z0BIAS=',F6.4/
     3     '    stereo hits=',I2,' require ',F4.2,'*',I3,
     4     '     st_amb_grp=',I2,' require ',I2,' out of ',I3/
     3     '  U stereo hits=',I2,' require ',F4.2,'*',I3,
     3     '  V stereo hits=',I2,' require ',F4.2,'*',I3,5X,
     5     '  PASS_Z_CRIT =',L1)
#endif

      RETURN
      END






