*
* $Id: xts_trak.F,v 1.23 2003/03/05 17:26:57 dpp Exp $
*
* $Log: xts_trak.F,v $
* Revision 1.23  2003/03/05 17:26:57  dpp
*      -> diagnostic format
*      -> use device, rather than LCLSTR_LAYER, to specify cathodes
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.22  2002/09/24 00:28:43  dpp
*     -> formatting
*
* Revision 1.21  2002/09/05 19:10:40  dpp
*      -> localize how the color is set
*
* Revision 1.20  2002/05/23 14:35:31  dpp
*      -> include more info in text
*      -> start to use "fetch" from statusbits routine
*         instead of direct access of status words
*      -> modify  setting INTERRUPT_THRESH so FAST_TRAK_RECALL... will not
*         reset the stops after a skip to next pass
*
* Revision 1.19  2001/11/19 23:12:08  dpp
*      -> remove unused variable
*      -> add new control parameter include file
*      -> change some variable names
*
* Revision 1.18  2001/09/12 22:08:34  dpp
*      -> diagnostic printout of new status bits
*      -> change "z at layer" to use s rather than r
*      -> expand use of program indicator
*
* Revision 1.17  2000/12/19 23:50:36  dpp
*      -> new interrupt in fast_trak
*      -> change result of XTS_TRAK_INTERRUPT_VALUE ('-') and ('+')
*      -> new answer: other2
*
* Revision 1.16  2000/10/24 22:33:33  dpp
*      -> fix printout label
*
* Revision 1.15  2000/10/02 20:54:06  dpp
*      -> new use of STAT words
*      -> add NFIT info of first line
*      -> adjust interupt values; make room/define value for BOOT_SI_Z_HITS
*      -> change the printed hit info for silicon
*      -> other printed hit/fit info
*
* Revision 1.14  2000/08/28 17:41:04  dpp
*      -> format
*
* Revision 1.13  2000/08/18 18:41:15  dpp
*      -> print track parameters after the kink
*
* Revision 1.12  2000/05/26 16:51:59  dpp
*      -> add hit label in "fit" dump
*      -> remove use of IHWRCD
*
* Revision 1.11  2000/04/26 14:49:13  dpp
*      -> control interupt for display with separate threshold and
*         interupt level variables rather than old XTS_NEW_TRACK
*      -> reduce possible "which" input to 'CS','CF','CD'
*      -> use call to RESIDUALS instead of RESICF
*      -> do not display silicon in r-phi view
*      -> MUST fix printed information
*
* Revision 1.10  2000/02/12 20:25:17  dpp
*      -> print out residual fropm fit for all hits in hit list
*
* Revision 1.9  2000/01/24 19:54:24  dpp
*      -> print the time_like residual
*
* Revision 1.8  2000/01/06 22:18:48  dpp
* provide CHICF on terminal
*
* Revision 1.7  1999/11/03 22:37:46  dpp
* use cathode flag rather than layer number to id cathodes
*
* Revision 1.6  1999/09/12 22:38:13  dpp
* handle fit hits not in hit list
*
* Revision 1.5  1999/08/03 21:53:06  dpp
* "??" input flag
*
* Revision 1.4  1999/04/27 20:13:35  dpp
* reorder ETYPE; change XTS_NEW_TRACK; try to inhibit until TFXFIT
*
* Revision 1.3  1999/01/29 15:34:44  dpp
* force update
*
* Revision 1.2  1998/07/09 00:37:00  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.8  1997/02/25 21:41:05  dpp
* use XTS numbering of cells rather than CD
*
* Revision 1.7  1996/08/08 16:07:04  dpp
* process qdiv hits already in the fit list
*
* Revision 1.6  1996/01/26 19:49:44  zfiles
* expanded track data structure information
*
* Revision 1.5  1996/01/25 15:12:06  zfiles
* 4 status bit words
*
* Revision 1.4  1995/12/18 19:45:19  nk
* changes to TFXFIT hit selection, updates of TFXFIT graphics
*
* Revision 1.3  1995/11/27 16:56:46  nk
* more inhibit options with XTS_NEW_TRACK
*
* Revision 1.2  1995/09/25 19:25:53  zfiles
* enable TFXFIT command based on value of XTS_TFXFIT_ON
* proper labeling of SVX hits
* print sigma used in the fit
*
* Revision 1.1.1.1  1994/10/08  01:01:00  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.22.24  by  Rob Kutschke
*CMZ :  6.00/01 28/05/93  11.32.55  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.40  by  Dan Peterson
*-- Author :dpp
      SUBROUTINE XTS_TRAK(WHICH_INPUT,ITR,CALLER)

      IMPLICIT NONE
      SAVE

C +SEQ,CDGEOMPA. INCLUDED IN TFINDPAR
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/statusbits.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfxtsp.inc"
#include "doit/sfseq/sfxtsc.inc"
#include "doit/sfseq/xts_cntl.inc"
#include "doit/duseq/tfstakcd.inc"
#include "doit/duseq/svgeometry.inc"
#include "doit/duseq/svscratch.inc"

      INTEGER INTERRUPT_VALUE,INTERRUPT_THRESH
      INTEGER INT_THRESH_TEMP

      INTEGER IENTER,KILLA,IY,STOPIT
      INTEGER IHIT,ILYRCD,ILYRTF,IWIR,ILYRHT
      INTEGER NHITU,IHABS,IHSIGN,HYBR,HITPNT,STRF,STSZ,SOURCE
      LOGICAL DISPLAY_THIS_HIT

      INTEGER HOLD(4)
      INTEGER kSTfetch
      INTEGER HERE
      CHARACTER*2 IDSEED
      INTEGER SF_PASS_NUMBER
      CHARACTER*6 SF_MERGE_TYPE
      CHARACTER*1 MERGE_INDICATOR
      INTEGER SF_CHAIN_NUMBER
      INTEGER HITS_WIRE_FIT
      CHARACTER*6 WIRE_D0_CONS
      CHARACTER*1 WIRE_ADD_KINK,WIRE_CONT_KINK
      INTEGER WIRE_FIT_CHISQ
      CHARACTER*1 FIT_CONT_KINK,FIT_ADD_KINK
      CHARACTER*1 FIT_CONT_KINK_1,FIT_CONT_KINK_2

      CHARACTER*1 F_T_1_D0_CONS,F_T_1_Z0_CONS
      CHARACTER*1 F_T_1_ALT_AMB,F_T_1_ALT_AMB_NULL
      INTEGER HITS_LOC_AMB_1
      CHARACTER*1 F_T_2_D0_CONS,F_T_2_Z0_CONS
      CHARACTER*1 F_T_2_ALT_AMB,F_T_2_ALT_AMB_NULL
      INTEGER HITS_LOC_AMB_2
      CHARACTER*1 RECALL_LIMR_TEST,RECALL_LIMR_SOL,RECALL_LIMR_SEL
      CHARACTER*1 TFXFIT_CALL_CF_1,REPLACE_Z_HITS_1
      CHARACTER*1 RECALL_DZ_WIND_0,RECALL_DZ_WIND_1,RECALL_DZ_WIND_2
      CHARACTER*1 RECALL_DZ_WIND_NEW_REP
      CHARACTER*1 TFXFIT_CALL_CF_2,REPLACE_Z_HITS_2
      CHARACTER*1 REMOVE_Z0_CONS
      CHARACTER*1 TFXFIT_CALL_LF_1
      CHARACTER*1 TEST_Z0_CONS,USE_Z0_CONS,BOOT_SI_Z

      REAL PQ,WGTTMP,SIGMAP
      INTEGER IBIT11,IBIT12,IBIT22,IBIT1,IBIT2,ION,TEMP
      INTEGER NAUTHR,IAUTHR,JAUTHR,AUTHRZ(100,2)
      LOGICAL GOOD_INIT
      INTEGER SEN_HOLD
      REAL ZLAYER,VEL,SIGCAL,RESCAL,DISPLATIME
      REAL CURAFT6,PHI0AFT6,D0AFT6
      INTEGER IDHITT
      INTEGER WAS_SIL_LAYER,NOW_SIL_LAYER

      CHARACTER*2 WHICH_INPUT
      INTEGER ITR
      CHARACTER*(*)CALLER

      CHARACTER*2 DISPLAY_FIT_TYP
      CHARACTER*2 WHICH_OLD,WHICH
      CHARACTER*1 INTERRUPT_INPUT
      CHARACTER*2 TEST_WHICH(8),USE_OLD
      CHARACTER*8 TYPE_FLAG
      CHARACTER*16 P_I_FLAG
      CHARACTER*8 TIMEUNITS


#if defined(CLEO_ULTRIX)
      INTEGER     CLHIT,CLFHIT
#else
C*****Alpha and SunOS
      INTEGER*8   CLHIT,CLFHIT
#endif


      DATA TEST_WHICH/'CS','CF','CD',5*'  '/
      DATA USE_OLD,WHICH_OLD/'??','CF'/
      DATA KILLA/0/
      DATA NAUTHR/0/



      IENTER=1

C branch point to restart, for extra hit information display
 5    CONTINUE
      IF(INTERRUPT_VALUE.LT.INTERRUPT_THRESH)GO TO 595

      TRKHLD=ITR

      IF(WHICH_INPUT.EQ.USE_OLD)THEN
         WHICH=WHICH_OLD
      ELSE
         WHICH=WHICH_INPUT
      ENDIF
      WHICH_OLD=WHICH

      IF    (WHICH.EQ.TEST_WHICH(1))THEN
         ETYPE=1
         kSTfetch=kSTfetchcs
         CALL UCOPY(STATCS,HOLD,4)
      ELSEIF(WHICH.EQ.TEST_WHICH(2))THEN
         ETYPE=2
         kSTfetch=kSTfetchcf
         CALL UCOPY(STATCF,HOLD,4)
      ELSEIF(WHICH.EQ.TEST_WHICH(3))THEN
         ETYPE=3
         kSTfetch=kSTfetchcd
         CALL UCOPY(STATCD(1,ITR),HOLD,4)
      ELSE
         PRINT 1002,WHICH
1002     FORMAT(' XTS_TRAK:GARBAGE INPUT:',A20)
         GO TO 595
      ENDIF

      HERE=0
      CALL CBYT(HOLD(1),1,HERE,1,1)
      IF(HERE.NE.0)IDSEED='SF'
      HERE=0
      CALL CBYT(HOLD(1),2,HERE,1,1)
      IF(HERE.NE.0)IDSEED='?2'
      HERE=0
      CALL CBYT(HOLD(1),3,HERE,1,1)
      IF(HERE.NE.0)IDSEED='?3'
      SF_PASS_NUMBER=0
      CALL CBYT(HOLD(1),4,SF_PASS_NUMBER,1,3)
      HERE=0
      CALL CBYT(HOLD(1),7,HERE,1,2)
      IF(HERE.EQ.0)SF_MERGE_TYPE='__0?__'
      IF(HERE.EQ.1)SF_MERGE_TYPE='NORMAL'
      IF(HERE.EQ.2)SF_MERGE_TYPE='SUPER_'
      IF(HERE.EQ.3)SF_MERGE_TYPE='__3?__'
      HERE=0
      CALL CBYT(HOLD(1),9,HERE,1,1)
      MERGE_INDICATOR='_'
      IF(HERE.NE.0)MERGE_INDICATOR='Y'
      SF_CHAIN_NUMBER=0
      CALL CBYT(HOLD(1),10,SF_CHAIN_NUMBER,1,8)
      HITS_WIRE_FIT=0
      CALL CBYT(HOLD(1),18,HITS_WIRE_FIT,1,6)
      HERE=0
      CALL CBYT(HOLD(1),24,HERE,1,1)
      WIRE_D0_CONS='KEEP  '
      IF(HERE.NE.0)WIRE_D0_CONS='REMOVE'
      HERE=0
      CALL CBYT(HOLD(1),25,HERE,1,1)
      WIRE_ADD_KINK='_'
      IF(HERE.NE.0)WIRE_ADD_KINK='Y'
      HERE=0
      CALL CBYT(HOLD(1),26,HERE,1,1)
      WIRE_CONT_KINK='_'
      IF(HERE.NE.0)WIRE_CONT_KINK='Y'


      HERE=0
      CALL CBYT(HOLD(1),27,HERE,1,1)
      FIT_CONT_KINK='_'
      IF(HERE.NE.0)FIT_CONT_KINK='Y'


      HERE=0
      CALL CBYT(HOLD(1),28,HERE,1,1)
      FIT_ADD_KINK='_'
      IF(HERE.NE.0)FIT_ADD_KINK='Y'


      HERE=0
      CALL CBYT(HOLD(1),29,HERE,1,1)
      FIT_CONT_KINK_1='_'
      IF(HERE.NE.0)FIT_CONT_KINK_1='Y'


      HERE=0
      CALL CBYT(HOLD(1),30,HERE,1,1)
      FIT_CONT_KINK_2='_'
      IF(HERE.NE.0)FIT_CONT_KINK_2='Y'

C      WIRE_FIT_CHISQ=0
C      CALL CBYT(HOLD(1),27,WIRE_FIT_CHISQ,1,5)

      HERE=0
      CALL CBYT(HOLD(2),1,HERE,1,1)
      F_T_1_D0_CONS='_'
      IF(HERE.NE.0)F_T_1_D0_CONS='Y'
      HERE=0
      CALL CBYT(HOLD(2),2,HERE,1,1)
      F_T_1_Z0_CONS='_'
      IF(HERE.NE.0)F_T_1_Z0_CONS='Y'
      HERE=0
      CALL CBYT(HOLD(2),3,HERE,1,1)
      F_T_1_ALT_AMB='_'
      IF(HERE.NE.0)F_T_1_ALT_AMB='Y'
      HERE=0
      CALL CBYT(HOLD(2),4,HERE,1,1)
      F_T_1_ALT_AMB_NULL='_'
      IF(HERE.NE.0)F_T_1_ALT_AMB_NULL='Y'
      HITS_LOC_AMB_1=0
      CALL CBYT(HOLD(2),5,HITS_LOC_AMB_1,1,6)

      HERE=0
      CALL CBYT(HOLD(2),11,HERE,1,1)
      F_T_2_D0_CONS='_'
      IF(HERE.NE.0)F_T_2_D0_CONS='Y'
      HERE=0
      CALL CBYT(HOLD(2),12,HERE,1,1)
      F_T_2_Z0_CONS='_'
      IF(HERE.NE.0)F_T_2_Z0_CONS='Y'
      HERE=0
      CALL CBYT(HOLD(2),13,HERE,1,1)
      F_T_2_ALT_AMB='_'
      IF(HERE.NE.0)F_T_2_ALT_AMB='Y'
      HERE=0
      CALL CBYT(HOLD(2),14,HERE,1,1)
      F_T_2_ALT_AMB_NULL='_'
      IF(HERE.NE.0)F_T_2_ALT_AMB_NULL='Y'
      HITS_LOC_AMB_2=0
      CALL CBYT(HOLD(2),15,HITS_LOC_AMB_2,1,6)

      HERE=0
      CALL STATUSBITS(kSTfetch,kstFTlimitrTEST,HERE)
      RECALL_LIMR_TEST='_'
      IF(HERE.NE.0)RECALL_LIMR_TEST='Y'
      HERE=0
      CALL STATUSBITS(kSTfetch,kstFTlimitrSOL,HERE)
      RECALL_LIMR_SOL='_'
      IF(HERE.NE.0)RECALL_LIMR_SOL='Y'
      HERE=0
      CALL STATUSBITS(kSTfetch,kstFTlimitrSEL,HERE)
      RECALL_LIMR_SEL='_'
      IF(HERE.NE.0)RECALL_LIMR_SEL='Y'

      HERE=0
      CALL CBYT(HOLD(2),21,HERE,1,1)
      TFXFIT_CALL_CF_1='_'
      IF(HERE.NE.0)TFXFIT_CALL_CF_1='Y'
      HERE=0
      CALL CBYT(HOLD(2),22,HERE,1,1)
      REPLACE_Z_HITS_1='_'
      IF(HERE.NE.0)REPLACE_Z_HITS_1='Y'
      HERE=0
      CALL CBYT(HOLD(2),23,HERE,1,1)
      RECALL_DZ_WIND_0='_'
      IF(HERE.NE.0)RECALL_DZ_WIND_0='Y'
      HERE=0
      CALL CBYT(HOLD(2),24,HERE,1,1)
      RECALL_DZ_WIND_1='_'
      IF(HERE.NE.0)RECALL_DZ_WIND_1='Y'
      HERE=0
      CALL CBYT(HOLD(2),25,HERE,1,1)
      RECALL_DZ_WIND_2='_'
      IF(HERE.NE.0)RECALL_DZ_WIND_2='Y'

      HERE=0
      CALL CBYT(HOLD(2),26,HERE,1,1)
      RECALL_DZ_WIND_NEW_REP='_'
      IF(HERE.NE.0)RECALL_DZ_WIND_NEW_REP='Y'

      HERE=0
      CALL CBYT(HOLD(2),27,HERE,1,1)
      TFXFIT_CALL_CF_2='_'
      IF(HERE.NE.0)TFXFIT_CALL_CF_2='Y'
      HERE=0
      CALL CBYT(HOLD(2),28,HERE,1,1)
      REPLACE_Z_HITS_2='_'
      IF(HERE.NE.0)REPLACE_Z_HITS_2='Y'

      HERE=0
      CALL CBYT(HOLD(2),29,HERE,1,1)
      REMOVE_Z0_CONS='_'
      IF(HERE.NE.0)REMOVE_Z0_CONS='Y'

      HERE=0
      CALL CBYT(HOLD(3),1,HERE,1,1)
      TFXFIT_CALL_LF_1='_'
      IF(HERE.NE.0)TFXFIT_CALL_LF_1='Y'
      HERE=0
      CALL CBYT(HOLD(3),2,HERE,1,1)
      TEST_Z0_CONS='_'
      IF(HERE.NE.0)TEST_Z0_CONS='Y'
      HERE=0
      CALL CBYT(HOLD(3),3,HERE,1,1)
      USE_Z0_CONS='_'
      IF(HERE.NE.0)USE_Z0_CONS='Y'
      HERE=0
      CALL CBYT(HOLD(3),4,HERE,1,1)
      BOOT_SI_Z='_'
      IF(HERE.NE.0)BOOT_SI_Z='Y'




      IF    (SAV_FIT_TYP.EQ.FITTYP_CF)THEN
         DISPLAY_FIT_TYP='CF'
      ELSEIF(SAV_FIT_TYP.EQ.FITTYP_LF)THEN
         DISPLAY_FIT_TYP='LF'
      ELSE
         DISPLAY_FIT_TYP='??'
      ENDIF

C CHECK TO SEE IF IT IS AUTHORIZED
      IF(NAUTHR.LE.0)GO TO 595
      DO 53 IAUTHR=1,NAUTHR
      IF(     AUTHRZ(IAUTHR,1)         .NE.ETYPE           )GO TO 53
      IF(IAND(AUTHRZ(IAUTHR,2),HOLD(1)).NE.AUTHRZ(IAUTHR,2))GO TO 53
      GO TO 59
53    CONTINUE
      GO TO 595
59    CONTINUE

c23456789 123456789 123456789 123456789 123456789 123456789 123456789012
      PRINT 1003
 1003 FORMAT(
     1' ==============================================================')
      PRINT 1001,CALLER,DISPLAY_FIT_TYP,WHICH,ITR,
     2     NHITCF,NHITLF,NHIT3F,NHITTF,NFIT
1001  FORMAT(
     2     ' XTS_TRAK:[ ',A30,' ]',
     3     ' fit type "',A2,'"',
     4     '  data type "',A2,'"',
     5     ' ITR=',I4,
     6     '  NHITCF=',I3,'  NHITLF=',I3,'  NHIT3F=',I3,
     7     '  NHITTF=',I3,'  NFIT=',I3)
      PRINT 1004,D0_CONS,Z0BIAS,RPRLIM,RSTRAG
 1004 FORMAT(
     1     '         : D0_CONS=',L1,'  Z0BIAS=',F7.4,
     2     '   rad FT PR=',F6.3,'  R stragler=',F6.3)
      PRINT 1005,
     1     IDSEED,SF_PASS_NUMBER,
     2     SF_MERGE_TYPE,MERGE_INDICATOR,SF_CHAIN_NUMBER,
     1     HITS_WIRE_FIT,WIRE_D0_CONS,
     2     WIRE_ADD_KINK,WIRE_CONT_KINK,
     2     FIT_CONT_KINK,FIT_ADD_KINK,FIT_CONT_KINK_1,FIT_CONT_KINK_2,
     1     F_T_1_D0_CONS,F_T_1_Z0_CONS,
     2     F_T_1_ALT_AMB,F_T_1_ALT_AMB_NULL,HITS_LOC_AMB_1,
     1     F_T_2_D0_CONS,F_T_2_Z0_CONS,
     2     F_T_2_ALT_AMB,F_T_2_ALT_AMB_NULL,HITS_LOC_AMB_2,
     1     RECALL_LIMR_TEST,RECALL_LIMR_SOL,RECALL_LIMR_SEL,
     1     TFXFIT_CALL_CF_1,REPLACE_Z_HITS_1,
     1     RECALL_DZ_WIND_0,RECALL_DZ_WIND_1,RECALL_DZ_WIND_2,
     1     RECALL_DZ_WIND_NEW_REP,
     1     TFXFIT_CALL_CF_2,REPLACE_Z_HITS_2,
     1     REMOVE_Z0_CONS,
     1     TFXFIT_CALL_LF_1,TEST_Z0_CONS,USE_Z0_CONS,BOOT_SI_Z

 1005 FORMAT(
     1     '         : seed_',A2,'   pass',I2,
     2     '   merge type_',A6,'  merge_',A1,'   chain',I4/
     1     '         : wirefit: hits',I3,'   D0 constrain_',A6,
     2     '   add kink_',A1,'  keep kink_',A1/
     1     '         :  hitfit:   keep kink_',A1,'   add kink_',A1,
     2     '  keep kink aft fit_',A1,'  keep kink aft del__',A1/
     1     '         : FT iteration 1: D0 constrain_',A1,
     2     '   Z0 constrain_',A1,'   alt amb_',A1,'   use NULL_',A1,
     3     '   hits from loc amb=',I3/
     1     '         : FT iteration 2: D0 constrain_',A1,
     2     '   Z0 constrain_',A1,'   alt amb_',A1,'   use NULL_',A1,
     3     '   hits from loc amb=',I3/
     1     '         : FTrecall limRtest_',A1,'  FTrecall limRsol_',A1,
     2     '   FTrecall limRselect_',A1/

     1     '         : TFXFIT CF: 1st call_',A1,'   replace z hits_',A1,
     2     '   req FTrecall DZ_',A1,
     3     '   1ST FTrecall(&TFXFIT) DZ_',A1,'   2ND FTrecall DZ_',A1,
     4     '   NEWrep FTrecall DZ_',A1/
     1     '         : TFXFIT CF: 2nd call_',A1,'   replace z hits_',A1,
     2     '   remove Z0 constrain_',A1/
     1     '         : TFXFIT LF: normal_',A1,'   test z0 constr_',A1,
     2     '   use z0 constr_',A1,'   boot Silicon z hits_',A1)

c23456789 123456789 123456789 123456789 123456789 123456789 123456789012


      CLHIT =CLDUCH
      CLFHIT=CLDUMB

      NIDHLD=0

C DISPLAY THE TFHITSCD HIT LIST

      IF( (ETYPE.EQ.2).AND.(NLYRHT.GT.0))THEN

         DO 189 ILYRHT=1,NLYRHT
            IF(LHITLR(ILYRHT).LE.MHITLR(ILYRHT))THEN
               DO 179 IHIT=LHITLR(ILYRHT),MHITLR(ILYRHT)
                  IHSIGN=IADRHT(IHIT)
                  DISPLAY_THIS_HIT=.TRUE.
                  
                  IF(DISPLAY_THIS_HIT)THEN
                     ILYRTF=IPLHIT(IHIT)
                     IHABS=IABS(IHSIGN)
C process the cathodes
                     IF(IDVCTF(ILYRTF).EQ.IVDCT)THEN
                        DISPLAY_THIS_HIT=.FALSE.
                     ELSEIF(IDVCTF(ILYRTF).EQ.IDRCT)THEN
                        DISPLAY_THIS_HIT=.FALSE.
C process charge division
                     ELSEIF((ILYRTF.GT.QDIVAD).OR.
     2                       (ILYRTF.GT.KLYRTF))THEN
                        DISPLAY_THIS_HIT=.FALSE.
C process the silicon                  
                     ELSEIF(IDVCTF(ILYRTF).EQ.ISVX1)THEN
                        DISPLAY_THIS_HIT=.FALSE.
                     ELSEIF(IDVCTF(ILYRTF).EQ.ISVZ1)THEN
                        DISPLAY_THIS_HIT=.FALSE.
C process regular anodes
                     ELSE
                        ILYRCD=EvntHit_CDlayr(IHABS)
                        IWIR=EvntHit_laWire(IHABS)
                     ENDIF
                  ENDIF

                  IF(DISPLAY_THIS_HIT)THEN
                     IF(ILYRCD.LE.MCANCD)THEN
                        
                        IF(NIDHLD.LT.MIDHLD)NIDHLD=NIDHLD+1
                        IDHITT=INXSCD(ILYRCD)+IWIR
                        
                        IF(NIDHLD.LT.MIDHLD)THEN
                           IDHLD(NIDHLD)=IDHITT
                           CLHLD(NIDHLD)=CLHIT

#if defined(CLEO_XTSDEBUG)
                     PRINT 1011,NIDHLD,IWIR,ILYRCD,
     2                    INXSCD(ILYRCD),IDHITT
 1011                FORMAT(' XTS_TRAK: Hit list',
     2                    ' filling hold list entry ',I5, 
     3                    '  with   wir=',I5,'  lyr=',I4,
     4                    ' (inxsxd=',I7,')',
     5                    ' xs_id=',I7)
#endif

                        ENDIF
                        
                     ENDIF
                  ENDIF
 179           CONTINUE
            ENDIF
 189     CONTINUE
      ENDIF
      
C DISPLAY THE TFTRAKCD FITTED HIT LIST 
C OR THE CD FITTED HIT LIST

      IF(
     2     (ETYPE.EQ.1).OR.
     3     (ETYPE.EQ.2))THEN
         NHITU=NFIT
      ELSEIF(ETYPE.EQ.3)THEN
         NHITU=NFITCD(ITR)
      ENDIF

      
      IF(NHITU.GT.0)THEN
         DO 279 IHIT=1,NHITU
            IF(
     2           (ETYPE.EQ.1).OR.
     3           (ETYPE.EQ.2))THEN
               IHSIGN=IADFIT(IHIT)
               ILYRTF=IPLFIT(IHIT)
            ELSEIF(ETYPE.EQ.3)THEN
               IHSIGN=IADFCD(IHIT,ITR)
               ILYRTF=IPLRCD(IHIT,ITR)
            ENDIF

            DISPLAY_THIS_HIT=(IHSIGN.NE.0)
            
            IF(DISPLAY_THIS_HIT)THEN
               IHABS=IABS(IHSIGN)
C process the cathodes
             IF(IDVCTF(ILYRTF).EQ.IVDCT)THEN
               DISPLAY_THIS_HIT=.FALSE.
             ELSEIF(IDVCTF(ILYRTF).EQ.IDRCT)THEN
               DISPLAY_THIS_HIT=.FALSE.
C process charge division
               ELSEIF((ILYRTF.GT.QDIVAD).OR.
     2                 (ILYRTF.GT.KLYRTF))THEN
                  DISPLAY_THIS_HIT=.FALSE.
C process the silicon                  
               ELSEIF(IDVCTF(ILYRTF).EQ.ISVX1)THEN
                  DISPLAY_THIS_HIT=.FALSE.
               ELSEIF(IDVCTF(ILYRTF).EQ.ISVZ1)THEN
                  DISPLAY_THIS_HIT=.FALSE.
C process regular anodes
               ELSE
                  ILYRCD=EvntHit_CDlayr(IHABS)
                  IWIR=EvntHit_laWire(IHABS)
               ENDIF   
            ENDIF   

            IF(DISPLAY_THIS_HIT)THEN
               IF(ILYRCD.LE.MCANCD)THEN
                  IF(NIDHLD.LT.MIDHLD)NIDHLD=NIDHLD+1
               
                  IDHITT=INXSCD(ILYRCD)+IWIR
                  IF(NIDHLD.LT.MIDHLD)THEN
                     IDHLD(NIDHLD)=IDHITT
                     CLHLD(NIDHLD)=CLFHIT

#if defined(CLEO_XTSDEBUG)
                     PRINT 1012,NIDHLD,IWIR,ILYRCD,
     2                    INXSCD(ILYRCD),IDHITT
 1012                FORMAT(' XTS_TRAK: Fit list',
     2                    ' filling hold list entry ',I5, 
     3                    '  with   wir=',I5,'  lyr=',I4,
     4                    ' (inxsxd=',I7,')',
     5                    ' xs_id=',I7)
#endif

                  ENDIF
                ENDIF
            ENDIF
 279     CONTINUE
      ELSE
         IF(NIDHLD.LE.0)GO TO 595
      ENDIF

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      GO TO 501
      ENTRY XTS_TRK2(CALLER)
      IENTER=2
      PRINT 2001,CALLER
2001  FORMAT(' XTS_TRK2: ENTER:  CALLED FROM ',A30)
501   CONTINUE
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


      IF(WKID.NE.0)THEN
        IF(NIDHLD.GT.0)THEN
          
          NSEND=0
          IHIT=0
 511      IHIT=IHIT+1
          IF(IHIT.GT.NIDHLD)GO TO 515
          NSEND=NSEND+1
          IDHIT(NSEND)=IDHLD(IHIT)
          COLRHT(NSEND)=CLHLD(IHIT)
          IF(NSEND.LT.MSEND)GO TO 511
 515      CONTINUE
          
#if defined(CLEO_XTSDEBUG)
          PRINT 1006,NSEND
 1006     FORMAT(' XTS_TRAK: WILL CALL XTS_FILL WITH NSEND=',I6)
#endif
          
          CALL XTS_FILL('XTS_TRAK')
          NSEND=0
          IF(IHIT.LT.NIDHLD)GO TO 511
          
          IF(IENTER.NE.1)GO TO 593
          
c23456789 123456789 123456789 123456789 123456789 123456789 123456789012
          
 521      IF(ETYPE.EQ.2)THEN
            CALL CDCFID(CURCF,PHI0CF,D0CF,
     2           KAPCF,ALPCF,BETCF,GAMCF,XICF)
            CALL KABG2(KABG2_CALC,
     1           0,0,0,KABG2_XI_RECALC,
     2           KAPCF,  ALPCF,  BETCF,  GAMCF,  XICF, PHI6CF,
     3           KAPAFT6,ALPAFT6,BETAFT6,GAMAFT6,XIAFT6)
            CALL CDCFID(CURAFT6,PHI0AFT6,D0AFT6,
     2           KAPAFT6,ALPAFT6,BETAFT6,GAMAFT6,XIAFT6)
            PQ=.225/CURCF      * SQRT(1.+TANDLF     **2)
            PRINT 2005,CURCF     ,D0CF       ,PHI0CF     ,
     2           TANDLF    ,Y0LF       ,
     3           PHI6CF    ,PHI7CF     ,PQ    ,CHICF  ,TLRESCF
            PRINT 2006,CURAFT6,D0AFT6,PHI0AFT6,CHILF,CH2RCCF
          ELSEIF(ETYPE.EQ.3)THEN
            PQ=.225/CURCD(ITR) * SQRT(1.+TANDCD(ITR)**2)
            PRINT 2007,CURCD(ITR) ,D0CD(ITR)  ,PHI0CD(ITR),
     2           TANDCD(ITR),Z0CD(ITR)  ,
     3           PHI6CD(ITR),PHI7CD(ITR),PQ,CHI2CD(ITR)
          ENDIF
          
 2007     FORMAT(' XTS_TRAK:',
     2         '  CU=',F6.3,' D0=',F9.6,' F=',F6.3,
     3         ' DZ=',F6.3,' Z0=',F7.4,
     4         ' F6=',F6.3,' F7=',F6.3,' P=',F7.3,' chisq',F7.2)
 2005     FORMAT(' XTS_TRAK:',
     2         '  CU=',F6.3,' D0=',F9.6,' F=',F6.3,
     3         ' DZ=',F6.3,' Z0=',F7.4,
     4         ' F6=',F6.3,' F7=',F6.3,' P=',F7.3,' CHICF',F7.2,
     5         ' extr=',F9.6)
 2006     FORMAT('         :',
     2         'after',F6.3,'    ',F9.6,'   ',F6.3,
     3         '  CHILF=',F7.2,
     4         ' ',12X,' Chi2 recalc in fit_whatever=',F7.2)
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
          
          IF(KILLA.EQ.1)GO TO 581
          
 522      CONTINUE
          
          PRINT 2012
 2012     FORMAT(
     1         ' XTS_TRAK:ACKNOWLEDGE...',
     1         '("YES" to show the hit info )    ',
     1         '("MORE" to show the track info )'/
     2         ' XTS_TRAK:              ',
     2         '("KILL" to stop asking)          ',
     2         '("NO"; inhibit until next track)')
          
C THIS PART MUST BE COORDINATED WITH DIAG_DUFIND
          
          IF    (INTERRUPT_VALUE.EQ.2)THEN
            PRINT 2013
          ELSE
            PRINT 2014
          ENDIF
          
 2013     FORMAT(
     4         ' XTS_TRAK:              ',
     4         '("<CR>"; INHIBIT UNTIL HERE AGAIN)')
 2014     FORMAT(
     5         ' XTS_TRAK:              ',
     5         '("<CR>"; stop at next call)')
          
          
          
          IF    (INTERRUPT_VALUE.LE.1)THEN
            PRINT 2021
          ELSEIF(INTERRUPT_VALUE.LE.2)THEN
            PRINT 2022
          ELSEIF(INTERRUPT_VALUE.LE.3)THEN
            PRINT 2023
          ELSEIF(INTERRUPT_VALUE.LE.4)THEN
            PRINT 2024
          ELSEIF(INTERRUPT_VALUE.LE.5)THEN
            PRINT 2025
          ELSEIF(INTERRUPT_VALUE.LE.6)THEN
            PRINT 2026
          ELSEIF(INTERRUPT_VALUE.LE.7)THEN
            PRINT 2027
          ENDIF
          
 2021     FORMAT(
     4         ' XTS_TRAK:              ',
     4         '("OTHER"; inhibit until end of rep)'/
     5         ' XTS_TRAK:              ',
     5         '("OTHER2";inhibit until TFXFIT-CF)')
 2022     FORMAT(
     4         ' XTS_TRAK:              ',
     4         '("OTHER"; inhibit until TFXFIT-CF)')
 2023     FORMAT(
     4         ' XTS_TRAK:              ',
     4         '("OTHER"; inhibit until TFXFIT-LF)')
 2024     FORMAT(
     4         ' XTS_TRAK:              ',
     4         '("OTHER"; inhibit until BOOT_SI_Z)')
 2025     FORMAT(
     4         ' XTS_TRAK:              ',
     4         '("OTHER"; inhibit until end of BOOT)')
 2026     FORMAT(
     4         ' XTS_TRAK:              ',
     4         '("OTHER"; inhibit until final fit)')
 2027     FORMAT(
     4         ' XTS_TRAK:              ',
     4         '("OTHER"; inhibit until storing)')
          
          PRINT 1003
          
          CALL XTS_SENSITIZE(3,0,0,0)
          IF(ETYPE.EQ.2)THEN
            CALL XTS_SENSITIZE(4,LAZHIT,1,0)
            CALL XTS_SENSITIZE(4,LARESI,1,0)
          ENDIF
          CALL XTS_SENSITIZE(1,0,0,SEN_HOLD)
          
          CALL XTS_RFV_INPUT
          IY=XTS_ANSWER_IY
          
          CALL XTS_SENSITIZE(0,0,0,SEN_HOLD)
          
          IF(XTS_ABORT.EQ.1)THEN
            INTERRUPT_THRESH=9
            GO TO 595
          ENDIF         
          
          IF    (IY.EQ.'null')THEN
            GO TO 581
          ELSEIF(IY.EQ.'Y')THEN
            GO TO 550
          ELSEIF(IY.EQ.'M')THEN
            GO TO 560
          ELSEIF(IY.EQ.'K')THEN
            KILLA=1
          ELSEIF(IY.EQ.'N')THEN
            INTERRUPT_THRESH=9
          ELSEIF(IY.EQ.'O')THEN
            INTERRUPT_THRESH=INTERRUPT_VALUE+1
          ELSEIF(IY.EQ.'2')THEN
            INTERRUPT_THRESH=INTERRUPT_VALUE+2
          ELSE
            PRINT 2004,IY
 2004       FORMAT(' XTS_TRAK: garbage input="',A4,'"')
            GO TO 522
          ENDIF
          GO TO 581
          
C print the hit info
 550      CONTINUE
          
          PRINT 5508,Y0LF,TANDLF
 5508     FORMAT(' XTS_TRAK: TFHITSCD,  Y0LF,TAND=',2F7.3)
          
 5521     FORMAT(
     1         ' ---- ---- ---- ---- ------ ---- ------',
     2         ' ------ ------ ------- -------- -----',
     3         ' ------ ------ ------ ------',
     4         ' --------- -- -- --- --------'/
     1         ' logc   TF  1st  Lst   Z at adrs   adrs',
     2         '    X      Y    drift    drift       ',
     3         '    X      Y      S      Z  ',
     4         ' residual status fit         '/
     1         ' layr layr  hit  hit  layer ROAD   HITS',
     2         '   wire   wire  TIME   distance  VEL ',
     3         '   hit    hit    hit    hit ',
     4         ' from fit  RF SZ numbr sigma '/
     1         ' ---- ---- ---- ---- ------ ---- ------',
     2         ' ------ ------',A8,  ' -------- -----',
     3         ' ------ ------ ------ ------',
     4         ' --------- -- -- --- --------')
          
 5522     FORMAT(
     1         ' ---- ---- ---- ---- ------ ---- ------',
     2         ' -----   ----- ------   ----- ------ ',
     3         ' ------ ------ ------ ------',
     4         ' --------- -- -- --- --------'/
     1         ' logc   TF  1st  Lst   Z at adrs   adrs',
     2         'HYBRID     hybrid         hybrid     ',
     3         '    X      Y      S      Z  ',
     4         ' residual status fit        '/
     1         ' layr layr  hit  hit  layer ROAD   HITS',
     2         ' NUMBR  ___z_range___  __phi_range__ ',
     3         '   hit    hit    hit    hit ',
     4         ' from fit  RF SZ numbr sigma'/
      
     1         ' ---- ---- ---- ---- ------ ---- ------',
     2         ' -----   ----- ------   ----- ------ ',
     3         ' ------ ------ ------ ------',
     4         ' --------- -- -- --- --------')
      

 5511     FORMAT(I5,I5,I5,I5,F7.3,
     2         I5,I7,
     3         F7.3,F7.3,F8.3,F9.6,F6.1,
     4         4F7.3,F10.6,2I3,
     5         I4,F9.6)
 5512     FORMAT(I5,I5,I5,I5,F7.3,
     2         I5,I7,
     3         F7.3,F7.3,F8.3,F9.6,F6.1,
     4         4F7.3,F10.6,2I3)
 5513     FORMAT(I5,I5,I5,I5,F7.3,
     2         I5,I7,
     3         I6,' ',2('(',F6.3,F7.3,')'),
     4         4F7.3,F10.6,2I3,
     5         I4,F9.6)
 5514     FORMAT(I5,I5,I5,I5,F7.3,
     2         I5,I7,
     3         I6,' ',2('(',F6.3,F7.3,')'),
     4         4F7.3,F10.6,2I3)
          
          
 5515     FORMAT(27X,
     2         I5,I7,
     3         F7.3,F7.3,F8.3,F9.6,F6.1,
     4         4F7.3,F10.6,2I3,
     5         I4,F9.6)
 5516     FORMAT(27X,
     2         I5,I7,
     3         F7.3,F7.3,F8.3,F9.6,F6.1,
     4         4F7.3,F10.6,2I3)
 5517     FORMAT(27X,
     2         I5,I7,
     3         I6,' ',2('(',F6.3,F7.3,')'),
     4         4F7.3,F10.6,2I3,
     5         I4,F9.6)
 5518     FORMAT(27X,
     2         I5,I7,
     3         I6,' ',2('(',F6.3,F7.3,')'),
     4         4F7.3,F10.6,2I3)
          
          
          WAS_SIL_LAYER=0
          DO 559 ILYRHT=1,NLYRHT
            ILYRTF=IPLRLR(ILYRHT)
C         ZLAYER=Y0LS+TANDLS*RTF(ILYRTF)
            ZLAYER=Y0LS+TANDLS*SLYRLR(ILYRHT)
            IF(
     1           (IDVCTF(ILYRTF).EQ.ISVX1).OR.
     2           (IDVCTF(ILYRTF).EQ.ISVZ1))THEN
              NOW_SIL_LAYER=1
            ELSE
              NOW_SIL_LAYER=-1
            ENDIF
            
            IF(
     1           (NOW_SIL_LAYER.EQ.-1).AND.
     2           (WAS_SIL_LAYER.NE.-1))THEN


            IF(IDVCTF(ILYRTF).EQ.ITPC1)THEN
              TIMEUNITS=' micro-s'
            ELSE
              TIMEUNITS='  nano-s'
            ENDIF

              PRINT 5521,TIMEUNITS
            ENDIF
            
            IF(
     1           (NOW_SIL_LAYER.EQ. 1).AND.
     2           (WAS_SIL_LAYER.NE. 1))THEN
              PRINT 5522
            ENDIF
            
            WAS_SIL_LAYER=NOW_SIL_LAYER
            
            DO 557 IHIT=LHITLR(ILYRHT),MHITLR(ILYRHT)
              IHSIGN=IADRHT(IHIT)
              IHABS=IABS(IHSIGN)
              STRF=STRFHT(IHIT)
              STSZ=STSZHT(IHIT)

            IF(IDVCTF(ILYRTF).EQ.ITPC1)THEN
              DISPLATIME=EvntHit_rawTIM(IHABS)/1.E6
            ELSE
              DISPLATIME=EvntHit_rawTIM(IHABS)/1.E3
            ENDIF

              VEL=0.
              IF(EvntHit_rawTIM(IHABS).GT.500.)
     1             VEL= 1.E9 
     2             *EvntHit_Drift(IHABS)
     3             /EvntHit_rawTIM(IHABS)
              CALL RESIDUALS('HIT',0,IHIT,RESCAL)
              
              IF(HITTRK(IHIT).NE.0)THEN
                IF(WGTFIT(HITTRK(IHIT)).GT.0.)THEN
                  SIGCAL=1./SQRT(WGTFIT(HITTRK(IHIT)))
                ELSE
                  SIGCAL=9.9
                ENDIF
              ENDIF
              
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
              
              IF(IHIT.EQ.LHITLR(ILYRHT))THEN
                IF(HITTRK(IHIT).NE.0)THEN
                  IF(NOW_SIL_LAYER.EQ.-1)THEN
C     first hit in layer/     hit in fit/ NOT silicon
                    PRINT 5511,
     1                   ILYRHT,ILYRTF,LHITLR(ILYRHT),MHITLR(ILYRHT),
     2                   ZLAYER,
     3                   IHIT,IHSIGN,
     4                   EvntHit_X(IHABS),EvntHit_Y(IHABS),
     5                   DISPLATIME,EvntHit_Drift(IHABS),VEL,
     7                   XHIT(IHIT),YHIT(IHIT),SHIT(IHIT),ZHIT(IHIT),
     8                   RESCAL,STRF,STSZ,
     9                   HITTRK(IHIT),SIGCAL
                  ELSE
C     first hit in layer/     hit in fit/     silicon
                    HYBR=SV_HYBRID(IHABS)
                    PRINT 5513,
     1                   ILYRHT,ILYRTF,LHITLR(ILYRHT),MHITLR(ILYRHT),
     2                   ZLAYER,
     3                   IHIT,IHSIGN,
     4                   HYBR,
     5                   SVBOUNDARY_Z(HYBR,1),SVBOUNDARY_Z(HYBR,2),
     6                   SVBOUNDARY_PHI(HYBR,1),SVBOUNDARY_PHI(HYBR,2),
     7                   XHIT(IHIT),YHIT(IHIT),SHIT(IHIT),ZHIT(IHIT),
     8                   RESCAL,STRF,STSZ,
     9                   HITTRK(IHIT),SIGCAL
                  ENDIF
                ELSE
                  IF(NOW_SIL_LAYER.EQ.-1)THEN
C     first hit in layer/ hit NOT in fit/ NOT silicon
                    PRINT 5512,
     1                   ILYRHT,ILYRTF,LHITLR(ILYRHT),MHITLR(ILYRHT),
     2                   ZLAYER,
     3                   IHIT,IHSIGN,
     4                   EvntHit_X(IHABS),EvntHit_Y(IHABS),
     5                   DISPLATIME,EvntHit_Drift(IHABS),VEL,
     7                   XHIT(IHIT),YHIT(IHIT),SHIT(IHIT),ZHIT(IHIT),
     8                   RESCAL,STRF,STSZ
                  ELSE
C     first hit in layer/ hit NOT in fit/     silicon
                    HYBR=SV_HYBRID(IHABS)
                    PRINT 5514,
     1                   ILYRHT,ILYRTF,LHITLR(ILYRHT),MHITLR(ILYRHT),
     2                   ZLAYER,
     3                   IHIT,IHSIGN,
     4                   HYBR,
     5                   SVBOUNDARY_Z(HYBR,1),SVBOUNDARY_Z(HYBR,2),
     6                   SVBOUNDARY_PHI(HYBR,1),SVBOUNDARY_PHI(HYBR,2),
     7                   XHIT(IHIT),YHIT(IHIT),SHIT(IHIT),ZHIT(IHIT),
     8                   RESCAL,STRF,STSZ
                  ENDIF
                ENDIF
              ELSE
                IF(HITTRK(IHIT).NE.0)THEN
                  IF(NOW_SIL_LAYER.EQ.-1)THEN
C NOT first hit in layer/     hit in fit/ NOT silicon
                    PRINT 5515,
     3                   IHIT,IHSIGN,
     4                   EvntHit_X(IHABS),EvntHit_Y(IHABS),
     5                   DISPLATIME,EvntHit_Drift(IHABS),VEL,
     7                   XHIT(IHIT),YHIT(IHIT),SHIT(IHIT),ZHIT(IHIT),
     8                   RESCAL,STRF,STSZ,
     9                   HITTRK(IHIT),SIGCAL
                  ELSE
C NOT first hit in layer/     hit in fit/     silicon
                    HYBR=SV_HYBRID(IHABS)
                    PRINT 5517,
     3                   IHIT,IHSIGN,
     4                   HYBR,
     5                   SVBOUNDARY_Z(HYBR,1),SVBOUNDARY_Z(HYBR,2),
     6                   SVBOUNDARY_PHI(HYBR,1),SVBOUNDARY_PHI(HYBR,2),
     7                   XHIT(IHIT),YHIT(IHIT),SHIT(IHIT),ZHIT(IHIT),
     8                   RESCAL,STRF,STSZ,
     9                   HITTRK(IHIT),SIGCAL
                  ENDIF
                ELSE
C NOT first hit in layer/ NOT hit in fit/ NOT silicon
                  IF(NOW_SIL_LAYER.EQ.-1)THEN
                    PRINT 5516,
     3                   IHIT,IHSIGN,
     4                   EvntHit_X(IHABS),EvntHit_Y(IHABS),
     5                   DISPLATIME,EvntHit_Drift(IHABS),VEL,
     7                   XHIT(IHIT),YHIT(IHIT),SHIT(IHIT),ZHIT(IHIT),
     8                   RESCAL,STRF,STSZ
                  ELSE
C NOT first hit in layer/ NOT hit in fit/     silicon
                    HYBR=SV_HYBRID(IHABS)
                    PRINT 5518,
     3                   IHIT,IHSIGN,
     4                   HYBR,
     5                   SVBOUNDARY_Z(HYBR,1),SVBOUNDARY_Z(HYBR,2),
     6                   SVBOUNDARY_PHI(HYBR,1),SVBOUNDARY_PHI(HYBR,2),
     7                   XHIT(IHIT),YHIT(IHIT),SHIT(IHIT),ZHIT(IHIT),
     8                   RESCAL,STRF,STSZ
                  ENDIF
                ENDIF
              ENDIF
              
 557        CONTINUE
 559      CONTINUE
          GO TO 521

C print the track info

 560      CONTINUE
          
          IF(
     2         (ETYPE.EQ.1).OR.
     3         (ETYPE.EQ.2))THEN
            NHITU=NFIT
            PRINT 5615,NHITU
 5615       FORMAT(' TFTRAKCD HIT LIST, NHITU=',I5)
          ELSEIF(ETYPE.EQ.3)THEN
            NHITU=NFITCD(ITR)
            PRINT 5616,NHITU
 5616       FORMAT(' CD FITTED HIT LIST, NHITU=',I5)
          ENDIF
          PRINT 5618
 5618     FORMAT(9X,' seq layr layr  WIRE  hit  CELLID hit  status',
     1         '   XFIT    YFIT    SFIT    ZFIT   SIGMA'/
     2         9X,'     (CD) (TF)      address      number RF SZ',
     3         '                               (micron)')
 5603     FORMAT(9X,I4,I5,I5,I6,I6,I7,I5,2I3,4F8.4,F8.1,3X,A8,X,A16)
          
          DO 569 IHIT=1,NHITU
            
            IF(
     2           (ETYPE.EQ.1).OR.
     3           (ETYPE.EQ.2))THEN
              IHSIGN=IADFIT(IHIT)
              ILYRTF=IPLFIT(IHIT)
              HITPNT=TRKHIT(IHIT)
              STRF=STRFFT(IHIT)
              STSZ=STSZFT(IHIT)
              WGTTMP=WGTFIT(IHIT)     
              SOURCE=SRCFIT(IHIT)
            ELSEIF(ETYPE.EQ.3)THEN
              IHSIGN=IADFCD(IHIT,ITR)
              ILYRTF=IPLRCD(IHIT,ITR)
              HITPNT=0
              STRF=0
              STSZ=0
              WGTTMP=WEIGHT_LAYER(ILYRTF,SAV_HIT_TYP,SAV_FIT_TYP)     
              SOURCE=0
            ENDIF
            
C process charge division
            IF(ILYRTF.GT.QDIVAD)THEN

              PRINT 5605,IHIT,ILYRTF
 5605         FORMAT(9X,I4,'  ILYRTF=',I9)
              
C process hits that are outside of layer limit
            ELSEIF(ILYRTF.GT.KLYRTF)THEN
              PRINT 5606,IHIT,ILYRTF
 5606         FORMAT(9X,I4,'  ILYRTF=',I9)
              
C process hits that have no address
            ELSEIF(IHSIGN.EQ.0)THEN
              PRINT 5607,IHIT,IHSIGN
 5607         FORMAT(9X,I4,'  IHSIGN=',I9)
              
              
C process regular hits
            ELSE
              IHABS=IABS(IHSIGN)
              ILYRCD=ILTFCD(ILYRTF)
              
              
              TYPE_FLAG=   '   -?-  '
              IWIR=0
              IDHITT=0
              IF    (IDVCTF(ILYRTF).EQ.IZDA)THEN
                TYPE_FLAG='CLEOc ZD'
                IWIR=EvntHit_laWire(IHABS)
                IDHITT=INXSCD(ILYRCD)+IWIR
              ELSEIF(IDVCTF(ILYRTF).EQ.IDRFT)THEN
                TYPE_FLAG='DR anode'
                IWIR=EvntHit_laWire(IHABS)
                IDHITT=INXSCD(ILYRCD)+IWIR
              ELSEIF(IDVCTF(ILYRTF).EQ.ITPC1)THEN
                TYPE_FLAG='TPC     '
                IWIR=EvntHit_laWire(IHABS)
                IDHITT=INXSCD(ILYRCD)+IWIR
              ELSEIF(IDVCTF(ILYRTF).EQ.IDRCT)THEN
                TYPE_FLAG='CATH-dr '
                IWIR=EvntHit_laWire(IHABS)
                IDHITT=INXSCD(ILYRCD)+IWIR
              ELSEIF(IDVCTF(ILYRTF).EQ.IVDCT)THEN
                TYPE_FLAG='CATH-vd '
                IDHITT=INXSCD(ILYRCD)+IWIR
              ELSEIF(IDVCTF(ILYRTF).EQ.ISVX1)THEN
                TYPE_FLAG='SV r-phi'
              ELSEIF(IDVCTF(ILYRTF).EQ.ISVZ1)THEN
                TYPE_FLAG='SV s-z  '
              ENDIF
              
              
              P_I_FLAG=   ' -unknown-      '
              IF    (SOURCE.EQ.P_I_FAST_TRAK          )THEN
                P_I_FLAG='FAST_TRACK ?    '
              ELSEIF(SOURCE.EQ.P_I_F_T_AMB_GRP_TO_TRAK)THEN
                P_I_FLAG='_AMB_GRP_TO_TRAK'
              ELSEIF(SOURCE.EQ.P_I_F_T_CLOSE_FIT_STORE)THEN
                P_I_FLAG='_CLOSE_FIT_STORE'
              ELSEIF(SOURCE.EQ.P_I_F_T_DEL_BAD_AND_ADD)THEN
                P_I_FLAG='_DEL_BAD_AND_ADD'
              ELSEIF(SOURCE.EQ.P_I_F_T_FIT_WIRE_POSITN)THEN
                P_I_FLAG='_FIT_WIRE_POSITN'
              ELSEIF(SOURCE.EQ.P_I_TFXFIT)THEN
                P_I_FLAG='TFXFIT'
              ELSEIF(SOURCE.EQ.P_I_TFXFIT_REDUCE)THEN
                P_I_FLAG='TFXFIT_REDUCE   '
              ELSEIF(SOURCE.EQ.P_I_REPLACE_Z_HITS)THEN
                P_I_FLAG='REPLACE_Z_HITS  '
              ELSEIF(SOURCE.EQ.P_I_BOOT_SI_Z_HITS)THEN
                P_I_FLAG='BOOT_SI_Z_HITS  '
              ENDIF
              
              
              IF(WGTTMP.GT.0.)THEN
                SIGMAP=1./SQRT(WGTTMP)*1000000.
              ELSE
                SIGMAP=9999.
              ENDIF
              
              PRINT 5603,
     1             IHIT,ILYRCD,ILYRTF,IWIR,IHSIGN,IDHITT,
     2             HITPNT,STRF,STSZ,
     3             XFIT(IHIT),YFIT(IHIT),
     4             SFIT(IHIT),ZFIT(IHIT),SIGMAP,
     5             TYPE_FLAG,P_I_FLAG
              
            ENDIF
 569      CONTINUE
          GO TO 521

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
          
C branch point if ACKNOWLEDGE not requested
 581      CONTINUE
C refreshes event without drawing the track; ready for next call to XTS_TRAK
          CALL XTS_RFVIEW2('XTS_TRAK')
          
 593      CONTINUE
          CALL UPDATE_C
        ENDIF
        
      ELSE

        PRINT 1007,CALLER
 1007   FORMAT(' XTS_TRAK (OR TRK2): XTS NOT OPEN:'
     1       ' CALLED FROM ',A30)
        

      ENDIF


 595  CONTINUE
      RETURN

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      ENTRY XTS_TRK0
      IENTER=3
      NIDHLD=0
      RETURN

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      ENTRY XTS_TRAK_INIT(WHICH_INPUT,IBIT11,ION,CALLER)
      IBIT1=IBIT11
      IBIT2=0
      GO TO 604

      ENTRY XTS_TRAK_INIT2(WHICH_INPUT,IBIT12,IBIT22,ION,CALLER)
      IBIT1=IBIT12
      IBIT2=IBIT22
      GO TO 604

604   TEMP=0
      IF((IBIT1.GE.1).AND.(IBIT1.LE.32))CALL SBYT(1,TEMP,IBIT1,1)
      IF((IBIT2.GE.1).AND.(IBIT2.LE.32))CALL SBYT(1,TEMP,IBIT2,1)

      IF(TEMP.EQ.0)THEN
         PRINT 6006,IBIT1,IBIT2
 6006    FORMAT(' XTS_TRAK_INIT:GARBAGE INPUT: IBIT1=',I4,' IBIT2=',I4)
         GO TO 685
      ENDIF

      IF    (WHICH_INPUT.EQ.TEST_WHICH(1))THEN
         ETYPE=1
      ELSEIF(WHICH_INPUT.EQ.TEST_WHICH(2))THEN
         ETYPE=2
      ELSEIF(WHICH_INPUT.EQ.TEST_WHICH(3))THEN
         ETYPE=3
      ELSE
         PRINT 6002,WHICH_INPUT
6002     FORMAT(' XTS_TRAK_INIT:GARBAGE INPUT: WHICH=',A20)
         GO TO 685
      ENDIF

C CHECK TO SEE IF IT IS ALREADY THERE

      JAUTHR=0
      IF(NAUTHR.GT.0)THEN
         IAUTHR=0
 621     IAUTHR=IAUTHR+1
         IF(IAUTHR.LE.NAUTHR)THEN
            IF(
     1           (AUTHRZ(IAUTHR,1).EQ.ETYPE).AND.
     2           (AUTHRZ(IAUTHR,2).EQ.TEMP ))THEN
               JAUTHR=IAUTHR
            ELSE
               GO TO 621
            ENDIF
         ENDIF
      ENDIF



C TURN IT ON
      IF(ION.EQ.1)THEN
         GOOD_INIT=.FALSE.
         IF(JAUTHR.EQ.0)THEN
C LOOK FOR OPENING; IS IT THE FIRST ENTRY?
            JAUTHR=1
            IF(NAUTHR.GT.0)THEN

C LOOK FOR OPENING; IS THERE A GAP?
               JAUTHR=0
               IAUTHR=0
 631           IAUTHR=IAUTHR+1
               IF(IAUTHR.LE.NAUTHR)THEN
                  IF(AUTHRZ(IAUTHR,1).EQ.  0 )THEN
                     JAUTHR=IAUTHR
                  ELSE
                     GO TO 631
                  ENDIF
               ENDIF

C LOOK FOR OPENING; IS THERE ROOM AT THE END?
               IF(JAUTHR.EQ.0)THEN
                  IF(NAUTHR.LT.100)THEN
                     JAUTHR=NAUTHR+1
C LOOK FOR OPENING; FOUND NO OPENING
                  ELSE
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
                     PRINT 6007,WHICH_INPUT,IBIT1,IBIT2,ION,
     2                    NAUTHR,CALLER
 6007                FORMAT(' XTS_TRAK_INIT: NO PLACE TO PUT IT'/
     1                    '"',A2,'" BIT1=',I2,' BIT2=',I2,
     2                    ' OFF/ON=',I1,' N=',I3,' CALLED BY',A30)
                  ENDIF
               ENDIF
            ENDIF
         
C TURN IT ON
            IF(JAUTHR.GT.0)THEN
               AUTHRZ(JAUTHR,1)=ETYPE
               AUTHRZ(JAUTHR,2)=TEMP
               IF(JAUTHR.GT.NAUTHR)NAUTHR=JAUTHR
               GOOD_INIT=.TRUE.
            ENDIF
         ENDIF

C TURN IT OFF
      ELSEIF(ION.EQ.0)THEN
         IF(JAUTHR.GT.0)THEN
            AUTHRZ(JAUTHR,1)=0
            GOOD_INIT=.TRUE.
         ELSE
            GOOD_INIT=.FALSE.
         ENDIF
         
      ELSE
         PRINT 6003,ION
 6003    FORMAT(' XTS_TRAK_INIT:GARBAGE INPUT: ION=',I10)
         GOOD_INIT=.FALSE.
      ENDIF

      IF(GOOD_INIT)THEN
         PRINT 6008,WHICH_INPUT,IBIT1,IBIT2,ION,CALLER
 6008    FORMAT(' XTS_TRAK_INIT:"',A2,'" BIT1=',I2,' BIT2=',I2,
     2        ' OFF/ON=',I1,' CALLED BY ',A30)
      ENDIF


685   CONTINUE
      RETURN


      ENTRY XTS_TRAK_INTERRUPT_THRESH(INTERRUPT_INPUT)

C will not stop until the value is increased again
C note: this set the value relative to the current value
      IF(INTERRUPT_INPUT.EQ.'+')THEN
         INTERRUPT_THRESH=INTERRUPT_VALUE+1

C will stop, unless the threshold is 9
C note: this set the value relative to the current value
      ELSEIF(INTERRUPT_INPUT.EQ.'-')THEN
         IF(INTERRUPT_THRESH.LT.9)THEN
            IF(INTERRUPT_THRESH.LE.(INTERRUPT_VALUE+1))
     1           INTERRUPT_THRESH=INTERRUPT_VALUE
         ENDIF

C note: these set an absolute value
      ELSE
            INT_THRESH_TEMP=-1
         IF(    INTERRUPT_INPUT.EQ.'0')THEN
            INT_THRESH_TEMP=0
         ELSEIF(INTERRUPT_INPUT.EQ.'1')THEN
            INT_THRESH_TEMP=1
         ELSEIF(INTERRUPT_INPUT.EQ.'2')THEN
            INT_THRESH_TEMP=2
         ELSEIF(INTERRUPT_INPUT.EQ.'3')THEN
            INT_THRESH_TEMP=3
         ELSEIF(INTERRUPT_INPUT.EQ.'4')THEN
            INT_THRESH_TEMP=4
         ELSEIF(INTERRUPT_INPUT.EQ.'5')THEN
            INT_THRESH_TEMP=5
         ELSEIF(INTERRUPT_INPUT.EQ.'6')THEN
            INT_THRESH_TEMP=6
         ELSEIF(INTERRUPT_INPUT.EQ.'7')THEN
            INT_THRESH_TEMP=7
         ELSEIF(INTERRUPT_INPUT.EQ.'8')THEN
            INT_THRESH_TEMP=8
         ELSEIF(INTERRUPT_INPUT.EQ.'9')THEN
            INT_THRESH_TEMP=9
         ENDIF
         IF(
     1        (INT_THRESH_TEMP.EQ.0).OR.
     2        (INT_THRESH_TEMP.EQ.9))THEN
            INTERRUPT_THRESH=INT_THRESH_TEMP
         ELSEIF(
     1           (INT_THRESH_TEMP.GT.0).AND.
     1           (INT_THRESH_TEMP.LT.9))THEN
            IF(INTERRUPT_THRESH.NE.9)THEN
               INTERRUPT_THRESH=INT_THRESH_TEMP
            ENDIF
         ENDIF
      ENDIF
      RETURN

      ENTRY XTS_TRAK_INTERRUPT_VALUE(INTERRUPT_INPUT)
      IF(    INTERRUPT_INPUT.EQ.'0')THEN
         INTERRUPT_VALUE=0
      ELSEIF(INTERRUPT_INPUT.EQ.'1')THEN
         INTERRUPT_VALUE=1
      ELSEIF(INTERRUPT_INPUT.EQ.'2')THEN
         INTERRUPT_VALUE=2
      ELSEIF(INTERRUPT_INPUT.EQ.'3')THEN
         INTERRUPT_VALUE=3
      ELSEIF(INTERRUPT_INPUT.EQ.'4')THEN
         INTERRUPT_VALUE=4
      ELSEIF(INTERRUPT_INPUT.EQ.'5')THEN
         INTERRUPT_VALUE=5
      ELSEIF(INTERRUPT_INPUT.EQ.'6')THEN
         INTERRUPT_VALUE=6
      ELSEIF(INTERRUPT_INPUT.EQ.'7')THEN
         INTERRUPT_VALUE=7
      ELSEIF(INTERRUPT_INPUT.EQ.'8')THEN
         INTERRUPT_VALUE=8
      ELSEIF(INTERRUPT_INPUT.EQ.'9')THEN
         INTERRUPT_VALUE=9

C note: these set the value relative to the threshold

C will stop, unless the threshold is 9
      ELSEIF(INTERRUPT_INPUT.EQ.'+')THEN
         IF(INTERRUPT_THRESH.LT.9)THEN
            INTERRUPT_VALUE=INTERRUPT_THRESH
         ENDIF

C will not stop until the value is increased again
      ELSEIF(INTERRUPT_INPUT.EQ.'-')THEN
         INTERRUPT_VALUE=INTERRUPT_THRESH-1
      ENDIF
      RETURN
      END



