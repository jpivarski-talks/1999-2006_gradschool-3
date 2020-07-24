* $Id: fast_trak_alternate_amb_2.F,v 1.13 2002/09/05 19:34:27 dpp Exp $
*
* $Log: fast_trak_alternate_amb_2.F,v $
* Revision 1.13  2002/09/05 19:34:27  dpp
*      -> remove any code involving "loop over group set"
*
* Revision 1.12  2001/11/19 22:55:01  dpp
*      -> use status bit encapsulation
*
* Revision 1.11  2000/10/02 20:57:20  dpp
*      -> new use of STAT words
*
* Revision 1.10  2000/08/18 18:46:32  dpp
*      -> use compile switches FTDIAG_ALT and FTDIAG_ALT_MORE to limit output
*
* Revision 1.9  2000/04/26 14:31:16  dpp
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*
* Revision 1.8  2000/01/06 22:10:09  dpp
* change some of the diagnostic compile switches
*
* Revision 1.7  1999/12/09 21:10:22  dpp
* reduce number of diagnostic messages CLEO_FTDIAG_MORE
* remove error (introduced 11/23/99) that
*    stops solution set at first group with no solution
*
* Revision 1.6  1999/11/30 23:29:43  dpp
* fixed wayward compile switch
*
* Revision 1.5  1999/11/29 22:15:21  dpp
* add loop to drop groups
* new structure eliminating the _BASE arrays and some assignments
*
* Revision 1.4  1999/11/23 23:24:19  dpp
*  added diagnostic to kill specified groups
*
* Revision 1.3  1999/11/17 21:46:26  dpp
* fix exponent in "quality", implement cuts from _INITIALIZE_CUTS
*
* Revision 1.2  1999/11/14 18:19:34  dpp
* now complete; sorts and loads solutions
*
* Revision 1.1  1999/11/11 16:48:12  dpp
* first version of the new alternate_amb
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE FAST_TRAK_ALTERNATE_AMB_2
C.......................................................................
C.
C. FAST_TRAK_ALTERNATE_AMB_2
C.                 -new version of generator of alternate ambiguity solutions
C. COMMON    : 
C. CALLS     : 
C. CALLED    : FAST_TRAK
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 28-Oct-99
C. LAST MOD  : 
C.
C. Modification Log.
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/statusbits.inc"
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
C limit
C-----------------------------------------------------------------------

C MAX_GROUP_NUM........maximum group number to use based on 
C                      the last group number with solutions
                       INTEGER MAX_GROUP_NUM 

C MAX_INDX_SOL.........maximum allowed value of INDX_SOL_USE
                       INTEGER MAX_INDX_SOL
                       PARAMETER (MAX_INDX_SOL=2)
C MAX_NULL_SOL.........maximum of groups with null solutions
C                      these are only groups that have solutions but 
C                      are set to null
                       INTEGER MAX_NULL_SOL


C-----------------------------------------------------------------------
C indices
C-----------------------------------------------------------------------

C WANT_NULL_SOL........number of null solutions for this pass
                       INTEGER WANT_NULL_SOL
C INDX_GROUP...........counter of ambiguity group number
                       INTEGER INDX_GROUP
C INDX_GROUP_OLD.......temp value of INDEX_GROUP used during changes
                       INTEGER INDX_GROUP_OLD
C INDX_SOL.............index of the solution number within a group
C                      temporary value of INDX_SOL_USE
                       INTEGER INDX_SOL 
C IPICK_BEST...........index of the loop of picking the best solutions
                       INTEGER IPICK_BEST
C INDX_ELEMENT.........index over elements in the stack
                       INTEGER INDX_ELEMENT 
C ITEMP................counter of ambiguity group; 2nd loop within main loop 
                       INTEGER ITEMP
C N_TOP................counter of number of times starting at "top of loop"
                       INTEGER N_TOP

C-----------------------------------------------------------------------
C sets of ambiguity group solutions
C-----------------------------------------------------------------------

C N_HITS_INCL_TST_GRP
                       INTEGER N_HITS_INCL_TST_GRP(0:DIM_GROUP)
C N_ZMEAS_INCL_TST_GRP
                       INTEGER N_ZMEAS_INCL_TST_GRP(0:DIM_GROUP)
C NUMB_NULL_SOL........number of null solutions created
                       INTEGER NUMB_NULL_SOL(0:DIM_GROUP)
C AVE_RES_TEST.........average residual for the test group
                       REAL AVE_RES_TEST(0:DIM_GROUP)
C INDX_BASE............index of the base for the current group
                       INTEGER INDX_BASE(DIM_GROUP)


C REF_LYR_TEST.........layer to use to pick up TF values for the test group
                       INTEGER REF_LYR_TEST(DIM_GROUP)
C VALID_ZMEAS
                       LOGICAL VALID_ZMEAS(DIM_GROUP)
C SHIFT_ADDING_TST_GRP.residual difference between test and base
                       REAL SHIFT_ADDING_TST_GRP(DIM_GROUP)
C TAN_DIF..............TANSTF difference between test and base
                       REAL TAN_DIF
C ZMEAS_ADDING_TST_GRP
                       REAL ZMEAS_ADDING_TST_GRP(DIM_GROUP)
C MIN_SHIFT............minimum residual between base and test
C                      when there is no valid Z measurement
C MAX_SHIFT............maximum residual between base and test
C                      when there is no valid Z measurement
C DIF_SHIFT............difference (max-min) residual between base and test
                       REAL MIN_SHIFT,MAX_SHIFT,DIF_SHIFT
C MIN_ZMEAS............minimum Z measurement between base and test
C                      when there is a valid Z measurement
C MAX_ZMEAS............maximum Z measurement between base and test
C                      when there is a valid Z measurement
C DIF_ZMEAS............difference (max-min) Z meas between base and test
                       REAL MIN_ZMEAS,MAX_ZMEAS,DIF_ZMEAS
C SUM_DZMEAS...........sum of (z measure)/(radius) in INDX_SOL_USE 
                       REAL SUM_DZMEAS
C NUM_DZMEAS...........number of measurements in AVE_DZMEAS
                       INTEGER NUM_DZMEAS
C NUM_INDEX_SOL_HOLD...number of stored elements in INDX_SOL_HOLD
                       INTEGER NUM_INDEX_SOL_HOLD
C INDX_SOL_HOLD........stack of values of INDX_SOL_USE
                       INTEGER INDX_SOL_HOLD(DIM_GROUP,DIM_SAV_SOL_SET)
C QUAL_INDEX_SOL_HOLD..qualtity of the elements of INDX_SOL_HOLD
                       REAL QUAL_INDEX_SOL_HOLD(DIM_SAV_SOL_SET)
C AVE_DZMEAS_HOLD......average of (z measure)/(radius) in INDX_SOL_USE 
                       REAL AVE_DZMEAS_HOLD(DIM_SAV_SOL_SET)
C PICKED_HOLD..........flag that an element had been picked
                       LOGICAL PICKED_HOLD(DIM_SAV_SOL_SET)
C BEST_FOUND_QUAL......best found quality when seaching the stack
                       REAL BEST_FOUND_QUAL
C BEST_ELEMENT.........element number of the best in the stack
                       INTEGER BEST_ELEMENT 
C USE_NULL.............=1 if WANT_NULL_SOL is non-zero
                       INTEGER USE_NULL

C-----------------------------------------------------------------------
C other local variables
C-----------------------------------------------------------------------

C TEST_MORE........... =.TRUE. if more ambiguity group solutions from the
C                      current group (INDX_GROUP), what that may be,
C                      are to be tested
                       LOGICAL TEST_MORE 

C PRE_GOOD............ =.TRUE. if the preliminary test is passed
                       LOGICAL PRE_GOOD

C IS_NEW_GROUP........ =.TRUE. if this is a new group and should be initiated
                       LOGICAL IS_NEW_GROUP

C NEXT_GROUP.......... =.TRUE. to go to the next ambiguity group
C                      with the current test as the base
                       LOGICAL NEXT_GROUP

C PREV_GROUP.......... =.TRUE. to go to the previous ambiguity group
C                      after the current group is exhausted
                       LOGICAL PREV_GROUP

C COMPLETE_SET......... =.TRUE. if the last group has been added and
C                      the final tests can be made before adding this
C                      set to the list
                       LOGICAL COMPLETE_SET

C FOUND_SOL_GR........ =.TRUE. if a solution was found for this group
                       LOGICAL FOUND_SOL_GR(DIM_GROUP)

C USED_SOL_GR......... =.TRUE. if any final set passed using this group
                       LOGICAL USED_SOL_GR(DIM_GROUP)

C TRIED_NULL.......... =.TRUE. if the null solution has already been tried
                       LOGICAL TRIED_NULL(DIM_GROUP)

C TRY_NULL_NOW........ =.TRUE. if meet either of the conditions that allow
C                       trying the null solution
                       LOGICAL TRY_NULL_NOW

                       INTEGER nkill,KILL_LIST(DIM_GROUP),TKILL



C-----------------------------------------------------------------------
C histograms
C-----------------------------------------------------------------------

      REAL CFIX
      COMMON/FIXER/CFIX(250)

C-----------------------------------------------------------------------
C  Executable code starts HERE
C-----------------------------------------------------------------------
      nkill=0
      call vzero(kill_list,dim_group)

#if defined(CLEO_FTDIAG_ALT_MORE)
      go to 52
 51   continue
      print 7701,nkill
 7701 format(' fast_trak_alternate_amb_2: nkill=',I2,
     2       '  enter group to kill')
      call dsf_rfv_input(tkill)
      if(tkill.ne.'null')then
         accept 7702,tkill
 7702    format(i)
         if(tkill.ne.0)then
            if((tkill.ge.1).and.(tkill.le.dim_group))then
               nkill=nkill+1
               kill_list(tkill)=1
               print 7703,tkill
 7703          format(' kill group number',I3)
            else
               print 7704,tkill
 7704          format(' invalid entry=',I8)
            endif
            go to 51
         endif
      endif
 52   continue
#endif
      
C-----------------------------------------------------------------------
C get the number of groups to use and fill group information
C-----------------------------------------------------------------------

      MAX_GROUP_NUM=0
      INDX_GROUP=0
 211  CONTINUE
      IF(INDX_GROUP.LT.NUMB_OF_GROUPS)THEN
         INDX_GROUP=INDX_GROUP+1
         REF_LYR_TEST(INDX_GROUP)=
     2        LYR_FIRST_IN_GR(INDX_GROUP)
         IF(NUM_HITS_SAV_SOL(1,INDX_GROUP).GT.0)THEN
            MAX_GROUP_NUM=INDX_GROUP
         ENDIF
         GO TO 211
      ENDIF

C-----------------------------------------------------------------------
C zero the local stack
C-----------------------------------------------------------------------

      NUM_INDEX_SOL_HOLD=0
      N_TOP=0

C-----------------------------------------------------------------------
C loop over skipped (null) groups
C-----------------------------------------------------------------------

      MAX_NULL_SOL=1

      WANT_NULL_SOL=-1
 311  WANT_NULL_SOL=WANT_NULL_SOL+1
      IF(WANT_NULL_SOL.LE.MAX_NULL_SOL)THEN
         USE_NULL=0
         IF(WANT_NULL_SOL.GT.0)USE_NULL=1


C-----------------------------------------------------------------------
C create base for the first ambiguity group; INDEX_GROUP=1
C the number zero group allows eliminating some if statements
C group set variables filled when adding group
C-----------------------------------------------------------------------

         INDX_GROUP=                 1
         INDX_BASE(INDX_GROUP)=      0
         N_HITS_INCL_TST_GRP(0)=     0
         N_ZMEAS_INCL_TST_GRP(0)=    0
         NUMB_NULL_SOL(0)=           0

         IS_NEW_GROUP=               .TRUE.
         PREV_GROUP=                 .FALSE.
         NEXT_GROUP=                 .FALSE.
         TEST_MORE=                  .TRUE.

#if defined(CLEO_FTDIAG_ALT)
         CALL VZERO(INDX_SOL_USE,NUMB_OF_GROUPS)
#endif

C-----------------------------------------------------------------------
C main test loop
C-----------------------------------------------------------------------

 321     CONTINUE
         N_TOP=N_TOP+1

#if defined(CLEO_FTDIAG_ALT_MORE)
         PRINT 9870
         PRINT 9871,INDX_GROUP,PREV_GROUP,NEXT_GROUP,IS_NEW_GROUP,
     2        (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

         COMPLETE_SET=.FALSE.

C-----------------------------------------------------------------------
C go back to the previous group after exhausting the loop over solutions 
C done if INDX_GROUP=1 solutions are completed
C-----------------------------------------------------------------------

         IF(PREV_GROUP)THEN

#if defined(CLEO_FTDIAG_ALT_MORE)
            PRINT 9872,INDX_GROUP,
     2                (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

            IF(INDX_BASE(INDX_GROUP).GT.0)THEN
               INDX_GROUP=INDX_BASE(INDX_GROUP)
            ELSE
               TEST_MORE=.FALSE.
            ENDIF
            PREV_GROUP=.FALSE.
         ENDIF
C-----------------------------------------------------------------------
C go up to the next group fo each solution of the current group and 
C set the base for the new group
C or, if already added the last group, go to the final evaluation
C-----------------------------------------------------------------------

         IF(NEXT_GROUP)THEN

#if defined(CLEO_FTDIAG_ALT_MORE)
            PRINT 9873,INDX_GROUP,MAX_GROUP_NUM,
     2           (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

            IF(INDX_GROUP.LT.MAX_GROUP_NUM)THEN
               INDX_GROUP_OLD=INDX_GROUP
               INDX_GROUP=INDX_GROUP+1

               IF(INDX_SOL_USE(INDX_GROUP_OLD).NE.0)THEN
                  INDX_BASE(INDX_GROUP)=INDX_GROUP_OLD
               ELSE
                  INDX_BASE(INDX_GROUP)=INDX_BASE(INDX_GROUP_OLD)
               ENDIF

               IS_NEW_GROUP=.TRUE.

            ELSE

#if defined(CLEO_FTDIAG_ALT_MORE)
               PRINT 9874,INDX_GROUP,
     2              (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

               COMPLETE_SET=.TRUE.
               TEST_MORE=.FALSE.
            ENDIF
            NEXT_GROUP=.FALSE.
         ENDIF

C-----------------------------------------------------------------------
C initiate the search in this ambiguity group; INDEX_GROUP
C       ( want to use INDX_SOL_USE()=0 for null, but put that in later )
C-----------------------------------------------------------------------

         IF(IS_NEW_GROUP)THEN

#if defined(CLEO_FTDIAG_ALT_MORE)
            PRINT 9875,INDX_GROUP,
     2           (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

            INDX_SOL_USE(INDX_GROUP)=   0
            FOUND_SOL_GR(INDX_GROUP)=   .FALSE.
            TRIED_NULL(INDX_GROUP)=     .FALSE.
            TEST_MORE=                  .TRUE.
            IS_NEW_GROUP=               .FALSE.
         ENDIF

C-----------------------------------------------------------------------
C loop over INDX_SOL_USE for the current group
C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG_ALT_MORE)
         PRINT 9876,INDX_GROUP,
     2        TEST_MORE,COMPLETE_SET,
     2        (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

         IF(TEST_MORE)THEN
            
            if(kill_list(INDX_GROUP).eq.1) 
     2           INDX_SOL_USE(INDX_GROUP)=MAX_INDX_SOL


#if defined(CLEO_FTDIAG_ALT_MORE)
            PRINT 9877,INDX_GROUP,
     2           MAX_INDX_SOL,TRIED_NULL(INDX_GROUP),
     3           FOUND_SOL_GR(INDX_GROUP),
     4           (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

            IF((INDX_SOL_USE(INDX_GROUP).LT.MAX_INDX_SOL)
     2          .AND.(.NOT.TRIED_NULL(INDX_GROUP)))THEN
               INDX_SOL_USE(INDX_GROUP)=INDX_SOL_USE(INDX_GROUP)+1
               INDX_SOL=INDX_SOL_USE(INDX_GROUP)

C-----------------------------------------------------------------------
C test if this specified solution to the group exists
C-----------------------------------------------------------------------

#if defined(CLEO_FTDIAG_ALT_MORE)
               PRINT 9878,INDX_GROUP,
     2              NUM_HITS_SAV_SOL(INDX_SOL,INDX_GROUP),
     2              (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

               IF(NUM_HITS_SAV_SOL(INDX_SOL,INDX_GROUP).GT.0)THEN
                  FOUND_SOL_GR(INDX_GROUP)=.TRUE.

C-----------------------------------------------------------------------
C build new test set by adding in this solution follwed by the base
C NUMB_NULL_SOL builds on INDX_GROUP-1 rather than INDX_GROUP_OLD
C because NUMB_NULL_SOL is incremented in the skipped group
C-----------------------------------------------------------------------

                  AVE_RES_TEST(INDX_GROUP)=
     2                 AVE_RES_SAV_SOL(INDX_SOL,INDX_GROUP)

                  N_HITS_INCL_TST_GRP(INDX_GROUP)=
     2                 NUM_HITS_SAV_SOL(INDX_SOL,INDX_GROUP)

                  N_ZMEAS_INCL_TST_GRP(INDX_GROUP)=0

                  NUMB_NULL_SOL(INDX_GROUP)=
     2                 NUMB_NULL_SOL(INDX_GROUP-1)

                  VALID_ZMEAS(INDX_GROUP)=.FALSE.


                  INDX_GROUP_OLD=INDX_BASE(INDX_GROUP)
                  IF(INDX_GROUP_OLD.GT.0)THEN

                     N_HITS_INCL_TST_GRP(INDX_GROUP)=
     2                    N_HITS_INCL_TST_GRP(INDX_GROUP)+
     3                    N_HITS_INCL_TST_GRP(INDX_GROUP_OLD)
                     
                     N_ZMEAS_INCL_TST_GRP(INDX_GROUP)=
     2                    N_ZMEAS_INCL_TST_GRP(INDX_GROUP_OLD)

                     SHIFT_ADDING_TST_GRP(INDX_GROUP)=
     2                    AVE_RES_TEST(INDX_GROUP)
     3                    -AVE_RES_TEST(INDX_GROUP_OLD)

                     TAN_DIF =TANSTF(REF_LYR_TEST(INDX_GROUP))
     2                    -TANSTF(REF_LYR_TEST(INDX_GROUP_OLD))

                     IF(ABS(TAN_DIF).GT. 0.015)THEN

                        VALID_ZMEAS(INDX_GROUP)=.TRUE.

                        N_ZMEAS_INCL_TST_GRP(INDX_GROUP)=
     2                       N_ZMEAS_INCL_TST_GRP(INDX_GROUP)+1

                        ZMEAS_ADDING_TST_GRP(INDX_GROUP)=
     2                       SHIFT_ADDING_TST_GRP(INDX_GROUP)
     3                       /TAN_DIF

                     ENDIF
                  ELSE
                     SHIFT_ADDING_TST_GRP(INDX_GROUP)=0.
                  ENDIF

C-----------------------------------------------------------------------
C could do preliminary evaluation of the test set here
C but not implemented yet, set PRE_GOOD=.TRUE. and proceed
C-----------------------------------------------------------------------

                  PRE_GOOD=.TRUE.

C-----------------------------------------------------------------------
C either go to the next INDX_GROUP and create base for that group
C or, if the preliminary test failed, go to the next solution of the group
C without filling in complete sets including this solution
C-----------------------------------------------------------------------

                  IF(PRE_GOOD)THEN
                     NEXT_GROUP=.TRUE.
                  ENDIF

C-----------------------------------------------------------------------
C this specified solution to the group did not exist;
C skip testing other solutions so that next tested solution will be the null
C-----------------------------------------------------------------------

               ELSE
                  INDX_SOL_USE(INDX_GROUP)=MAX_INDX_SOL
               ENDIF

C-----------------------------------------------------------------------
C have exhausted the loop over group solutions 
C if there have been no found solutions
C try the null solution by transfering the current base to the next group
C also try the null solution even if there have been found solutions
C but there are skipped groups allowed
C if can't do the null solution, directly drop down to the previous group 
C NUMB_NULL_SOL builds on INDX_GROUP-1 rather than INDX_GROUP_OLD
C because NUMB_NULL_SOL is incremented in the skipped group
C-----------------------------------------------------------------------

            ELSEIF(.NOT.TRIED_NULL(INDX_GROUP))THEN
               INDX_GROUP_OLD=INDX_BASE(INDX_GROUP)

#if defined(CLEO_FTDIAG_ALT_MORE)
               PRINT 9879,INDX_GROUP,
     2              FOUND_SOL_GR(INDX_GROUP),
     3              NUMB_NULL_SOL(INDX_GROUP-1),
     4              (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

               IF(.NOT.FOUND_SOL_GR(INDX_GROUP))THEN
                  TRY_NULL_NOW=.TRUE.
                  NUMB_NULL_SOL(INDX_GROUP)=
     3                    NUMB_NULL_SOL(INDX_GROUP-1)


               ELSEIF(NUMB_NULL_SOL(INDX_GROUP-1).LT.
     2                 WANT_NULL_SOL)THEN
                  TRY_NULL_NOW=.TRUE.
                  NUMB_NULL_SOL(INDX_GROUP)=
     3                    NUMB_NULL_SOL(INDX_GROUP-1)+1
               ELSE
                  TRY_NULL_NOW=.FALSE.
                  PREV_GROUP=.TRUE.
               ENDIF
               
               IF(TRY_NULL_NOW)THEN
                  INDX_SOL_USE(INDX_GROUP)=   0
                  
c                  IF(INDX_GROUP_OLD.GT.0)THEN
                     N_HITS_INCL_TST_GRP(INDX_GROUP)=
     2                    N_HITS_INCL_TST_GRP(INDX_GROUP_OLD)
                     N_ZMEAS_INCL_TST_GRP(INDX_GROUP)=
     2                    N_ZMEAS_INCL_TST_GRP(INDX_GROUP_OLD)
c                  ELSE
c                     N_HITS_INCL_TST_GRP(INDX_GROUP)=0
c                     N_ZMEAS_INCL_TST_GRP(INDX_GROUP)=0
c                  ENDIF

                  NEXT_GROUP=.TRUE.
               ENDIF
               TRIED_NULL(INDX_GROUP)=     .TRUE.

C-----------------------------------------------------------------------
C have exhausted the loop over group solutions 
C and even tried the null solution
C drop down to the previous group
C-----------------------------------------------------------------------

            ELSE
               PREV_GROUP=.TRUE.
            ENDIF

C-----------------------------------------------------------------------
C end of the TEST_MORE condition
C check for complete set
C-----------------------------------------------------------------------

         ELSEIF(COMPLETE_SET)THEN
               
#if defined(CLEO_FTDIAG_ALT_MORE)
            PRINT 9880,INDX_GROUP,NUMB_NULL_SOL(INDX_GROUP),
     2           (INDX_SOL_USE(ITEMP),ITEMP=1,INDX_GROUP)
#endif

            IF(NUMB_NULL_SOL(INDX_GROUP).EQ.WANT_NULL_SOL)THEN
               MIN_SHIFT= 999.
               MAX_SHIFT=-999.
               MIN_ZMEAS= 999.
               MAX_ZMEAS=-999.
               SUM_DZMEAS=0.
               NUM_DZMEAS=0

               DO 249 ITEMP=1,MAX_GROUP_NUM
                  INDX_SOL=INDX_SOL_USE(ITEMP)
                  IF(INDX_SOL.GT.0)THEN
                     IF(VALID_ZMEAS(ITEMP))THEN
                        IF(ZMEAS_ADDING_TST_GRP(ITEMP).LT.MIN_ZMEAS)
     2                       MIN_ZMEAS=ZMEAS_ADDING_TST_GRP(ITEMP)
                        IF(ZMEAS_ADDING_TST_GRP(ITEMP).GT.MAX_ZMEAS)
     2                       MAX_ZMEAS=ZMEAS_ADDING_TST_GRP(ITEMP)
                        NUM_DZMEAS=NUM_DZMEAS+1
                        SUM_DZMEAS=SUM_DZMEAS+
     2                       ZMEAS_ADDING_TST_GRP(ITEMP)
     2                       /RTF(LYR_FIRST_IN_GR(ITEMP))
#if defined(CLEO_FTDIAG_ALT)
C#if defined(CLEO_FTDIAG_ALT_MORE)
                        PRINT 9884,ITEMP,
     2                       REF_LYR_TEST(INDX_BASE(ITEMP)),
     3                       REF_LYR_TEST(ITEMP),
     4                       AVE_RES_TEST(INDX_BASE(ITEMP)),
     5                       AVE_RES_TEST(ITEMP),
     6                       SHIFT_ADDING_TST_GRP(ITEMP),
     7                       ZMEAS_ADDING_TST_GRP(ITEMP)
#endif

                     ELSEIF(INDX_BASE(ITEMP).GT.0)THEN
                        
                        IF(SHIFT_ADDING_TST_GRP(ITEMP).LT.MIN_SHIFT)
     2                       MIN_SHIFT=SHIFT_ADDING_TST_GRP(ITEMP)
                        IF(SHIFT_ADDING_TST_GRP(ITEMP).GT.MAX_SHIFT)
     2                       MAX_SHIFT=SHIFT_ADDING_TST_GRP(ITEMP)
                        
#if defined(CLEO_FTDIAG_ALT)
C#if defined(CLEO_FTDIAG_ALT_MORE)
                        PRINT 9885,ITEMP,
     2                       REF_LYR_TEST(INDX_BASE(ITEMP)),
     3                       REF_LYR_TEST(ITEMP),
     4                       AVE_RES_TEST(INDX_BASE(ITEMP)),
     5                       AVE_RES_TEST(ITEMP),
     2                       SHIFT_ADDING_TST_GRP(ITEMP)
#endif

                     ENDIF
                  ENDIF
 249           CONTINUE
               
               DIF_SHIFT=MAX_SHIFT-MIN_SHIFT
               DIF_ZMEAS=MAX_ZMEAS-MIN_ZMEAS
               
#if defined(CLEO_FTDIAG_ALT)
               PRINT 9886,MAX_SHIFT,MIN_SHIFT,DIF_SHIFT,
     2              MAX_ZMEAS,MIN_ZMEAS,DIF_ZMEAS
#endif

C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
C save the INDX_SOL_USE in a local list and only transfer the best 5 or 16
C!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

               IF((DIF_SHIFT.LT. DIF_SHIFT_CUT).AND.
     2              (DIF_ZMEAS.LT. DIF_ZMEAS_CUT))THEN
                  NUM_INDEX_SOL_HOLD=NUM_INDEX_SOL_HOLD+1
                  CALL UCOPY(INDX_SOL_USE,
     2                 INDX_SOL_HOLD(1,NUM_INDEX_SOL_HOLD),DIM_GROUP)
                  QUAL_INDEX_SOL_HOLD(NUM_INDEX_SOL_HOLD)=
     2                 (DIF_SHIFT/ DIF_SHIFT_SIGMA)**2+
     3                 (DIF_ZMEAS/ DIF_ZMEAS_SIGMA)**2
                  
                  IF(NUM_DZMEAS.GE.3)THEN
                     AVE_DZMEAS_HOLD(NUM_INDEX_SOL_HOLD)=
     2                    SUM_DZMEAS/NUM_DZMEAS
                  ELSE
                     AVE_DZMEAS_HOLD(NUM_INDEX_SOL_HOLD)=0.
                  ENDIF
                  PICKED_HOLD(NUM_INDEX_SOL_HOLD)=.FALSE.
                  
#if defined(CLEO_FTDIAG_ALT)
                  PRINT 9887,NUM_INDEX_SOL_HOLD,
     2                 QUAL_INDEX_SOL_HOLD(NUM_INDEX_SOL_HOLD),
     3                 AVE_DZMEAS_HOLD(NUM_INDEX_SOL_HOLD)
#endif
                  

#if defined(CLEO_XTSUBS)
C NOTE: this call to FAST_TRAK_AMB_GRP_TO_TRAK is for the display
C       only and is really meant to be inside the diagnostic switch
               CALL FAST_TRAK_AMB_GRP_TO_TRAK
               CALL XTS_TRAK('CF',0,
     2              'FAST_TRAK end of set')
#endif
               ENDIF
            ENDIF


            COMPLETE_SET=.FALSE.

            IF(NUM_INDEX_SOL_HOLD.LT.DIM_SAV_SOL_SET)THEN
               TEST_MORE=.TRUE.
            ENDIF

C-----------------------------------------------------------------------
C end of TEST_MORE and COMPLETE_SET conditions
C-----------------------------------------------------------------------

         ENDIF
         IF(TEST_MORE)GO TO 321

C-----------------------------------------------------------------------
C end of loop over WANT_NULL_SOL
C-----------------------------------------------------------------------

         IF(NUM_INDEX_SOL_HOLD.EQ.0)GO TO 311
      ENDIF

#if defined(CLEO_FTDIAG_ALT)
      PRINT 9888,N_TOP
#endif


C-----------------------------------------------------------------------
C pick out the few best solutions
C-----------------------------------------------------------------------

      IPICK_BEST=0
 259  IPICK_BEST=IPICK_BEST+1
      IF(((NUM_INDEX_SOL_HOLD-IPICK_BEST).GE.0).AND.
     2                        (IPICK_BEST.LE.3))THEN

         BEST_FOUND_QUAL=99999.
         BEST_ELEMENT=0
         DO 269 INDX_ELEMENT=1,NUM_INDEX_SOL_HOLD
            IF(.NOT.PICKED_HOLD(INDX_ELEMENT))THEN
               IF(QUAL_INDEX_SOL_HOLD(INDX_ELEMENT).LT.
     2            BEST_FOUND_QUAL)THEN
                  BEST_FOUND_QUAL=QUAL_INDEX_SOL_HOLD(INDX_ELEMENT)
                  BEST_ELEMENT=INDX_ELEMENT
               ENDIF
            ENDIF
 269     CONTINUE

C-----------------------------------------------------------------------
C load the best element
C-----------------------------------------------------------------------

         IF(BEST_ELEMENT.NE.0)THEN

#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 9889,BEST_ELEMENT
#endif

            CALL UCOPY(INDX_SOL_HOLD(1,BEST_ELEMENT),
     2                 INDX_SOL_USE,DIM_GROUP)
            PICKED_HOLD(BEST_ELEMENT)=.TRUE.

            CALL FAST_TRAK_AMB_GRP_TO_TRAK

            TANDLF=TANDLF+AVE_DZMEAS_HOLD(BEST_ELEMENT)
               
#if defined(CLEO_FTDIAG)
            PRINT 1003
            PRINT 1121,(INDX_SOL_USE(ITEMP),ITEMP=1,12),NFIT
#endif

#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK('CF',0,
     2           'FAST_TRAK alternate ambiguity groups')
#endif


            IF(ITERATION.EQ.1)THEN
               CALL STATUSBITS(kSTfillcf,kstFT1altamb,1)
               CALL STATUSBITS(kSTfillcf,kstFT1altambNUL,USE_NULL)
               CALL STATUSBITS(kSTfillcf,kstFT1nhitlamb,NFIT)
            ELSE
               CALL STATUSBITS(kSTfillcf,kstFT2altamb,1)
               CALL STATUSBITS(kSTfillcf,kstFT2altambNUL,USE_NULL)
               CALL STATUSBITS(kSTfillcf,kstFT2nhitlamb,NFIT)
            ENDIF

            CALL FAST_TRAK_CLOSE_FIT_STORE

            GO TO 259
         ENDIF
      ENDIF
      RETURN

#if defined(CLEO_FTDIAG)
 9870 FORMAT(
     2 ' =============================================================')
 9871 FORMAT(' FTA2:@TOP of LOOP,INDX_g=',I2,
     2       ' Pg=',L1,' Ng=',L1,' INg=',L1,
     8                                       ' use',20I3)
 9872 FORMAT(' FTA2:@PREV_GROUP, INDX_g=',I2,16X,
     8                                       ' use',20I3)
 9873 FORMAT(' FTA2:@NEXT_GROUP, INDX_g=',I2,
     2          ' NUMB_OF_GRPS=',I2,
     8                                       ' use',20I3)
 9874 FORMAT(' FTA2:will complete set,',2X,
     2          ' INDX_GROUP=',I2,4X,
     8                                       ' use',20I3)
 9875 FORMAT(' FTA2@IS_NEW_GROUP,INDX_g=',I2,16X,
     8                                       ' use',20I3)
 9876 FORMAT(' FTA2:@TESTMORE,   INDX_g=',I2,
     2                ' tM,compCHN=',2L2,
     8                                       ' use',20I3)
 9877 FORMAT(' FTA2@INDX_SOL_USE,INDX_g=',I2,
     2                ' mx=',I1,' TN,FSG=',L1,L2,
     8                                       ' use',20I3)
 9878 FORMAT(' FTA2:@NUM_HITS,   INDX_g=',I2,
     2                '   NUM_HITS=',I2,2X,
     8                                       ' use',20I3)
 9879 FORMAT(' FTA2:@try null,   INDX_g=',I2,
     2                ' fsg=',L1,' N_N_SgM=',I1,
     8                                       ' use',20I3)
 9880 FORMAT(' FT:@COMPLETE: INDX_GROUP=',I2,
     2                ' NUM_NULL_SOL=',I2,
     8                                       ' use',20I3)
 9884 FORMAT(' FAST_TRAK:GROUP',I2,
     2                 ' LYR=',2I3,'   AVERES=',2F10.6,
     3                 ' RES_DIF=',F10.6,' Z_MEAS=',F10.4)
 9885 FORMAT(' FAST_TRAK:GROUP',I2,
     2                 ' LYR=',2I3,'   AVERES=',2F10.6,
     3                 ' RES_DIF=',F10.6)
 9886 FORMAT(' FAST_TRAK: SHIFT MAX/MIN/dif=',3F10.6,
     2                 '  ZMEAS MAX/MIN/dif=',3F10.4)
 9887 FORMAT(' FAST_TRAK: HOLDING IN STACK ELEMENT NUMBER',I5,
     2                 ' quality=',F9.4,' dZshift=',F9.4)
 9888 FORMAT(' FAST_TRAK:***** number of loops ****',I10)
 9889 FORMAT(' FAST_TRAK: SELECTED altSTACK ELEMENT NUMBER',I5)
 1003 FORMAT(
     1' --------------------------------------------------------------')
 1121 FORMAT(' FAST_T:   DID AMB GROUPS ',
     2     ' , INDX_SOL_USE=',12I3,
     3     ' , NFIT=',I3)

#endif

      END




