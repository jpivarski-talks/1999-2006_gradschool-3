*
* $Id: sfcuts.F,v 1.20 2003/03/05 18:34:07 dpp Exp $
*
* $Log: sfcuts.F,v $
* Revision 1.20  2003/03/05 18:34:07  dpp
*     -> remove the initial print statement
*
* Revision 1.19  2003/03/05 18:32:54  dpp
*      -> define new cut set for "TPC1"
*      -> move control of z windo center, size and loop to sfcuts.F
*      -> put back in the initial print statement
*
* Revision 1.18  2001/11/20 00:05:40  dpp
*      -> use meslog for all print statements
*
* Revision 1.17  2000/12/19 23:55:50  dpp
*      -> allow tracks starting further out in "short"
*
* Revision 1.16  2000/10/02 20:50:39  dpp
*      -> revised structure to remove redundant assignments
*      -> create new cut definition "2NDPASS" which is
*         the same as "SHORT" except ZTST_RMV_OUTER(2) to cut straglers
*
* Revision 1.15  2000/08/18 19:09:42  dpp
*      -> define cuts to remove Z test near top of curler
*
* Revision 1.14  2000/02/12 20:32:35  dpp
*      -> new argument for SFTCUT
*
* Revision 1.13  2000/01/24 20:00:42  dpp
*      -> entry printout now not in MESLOG
*         so it appears even with "report level error"
*      -> test for endplate exit in "startup"
*
* Revision 1.12  2000/01/06 22:21:26  dpp
* set switch to stop searching layer after finding
*    a wide hit in a stereo transition layer
*
* Revision 1.11  1999/12/09 21:26:40  dpp
* add constants to control
* test of Z escape to remove test of Z stragler
*
* Revision 1.10  1999/11/19 22:57:55  dpp
* add time cuts and multiplier (to make the startup data work)
* add "startup" option
*
* Revision 1.9  1999/11/03 22:59:09  dpp
* new comments
*
* Revision 1.8  1999/10/12 15:57:47  lyon
* SFCUTS and SFTCUT messages only come out once
*
* Revision 1.7  1999/10/12 15:15:04  dpp
* fill array THRS_USE_PNT
*
* Revision 1.6  1999/10/06 12:34:34  dpp
* max SFNEXT start layr, width control ster trans
*
* Revision 1.5  1999/08/03 21:58:00  dpp
* cuts on z consistancy
*
* Revision 1.4  1999/04/27 19:36:47  dpp
* changes to sftyp=2,"clean"
*
* Revision 1.3  1999/01/29 14:53:55  dpp
* changes to "clean"; extend to stereo
*
* Revision 1.2  1998/07/09 00:55:31  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.7  1997/02/25 21:46:59  dpp
* new cuts when including SVX layers in SFBWAL
*
* Revision 1.6  1996/04/16 18:54:23  dpp
* SFBWAL tuning, first time use of VSTORH (velocity calculation step)=5
*
* Revision 1.5  1996/04/15 17:13:30  dpp
* velocity calculation constants
* looser stragler test for SFBWAL
*
* Revision 1.4  1996/04/03 16:13:36  dpp
* bug fix: MINLCH(2) for CNTLSF=5,
* change value of smallest starting layer in CNTLSF=5
*
* Revision 1.3  1996/02/29 17:26:51  zfiles
* changes for level3: calls sftcut to install time cuts after CNTLSF is set
*
* Revision 1.2  1995/04/20 22:39:44  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:47  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/18 17/03/94  19.39.12  by  Dan Peterson
*CMZ :  6.00/01 30/03/93  20.14.57  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.29  by  Dan Peterson
*-- Author :  DAN PETERSON
      SUBROUTINE SFCUTS(SHOWCL,WHICH)
C.......................................................................
C.
C. SFCUTS - SFIND chain finding cuts
C.
C. COMMON    : /STEPCn/ and others
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 27-Jul-88
C. LAST MOD  : 27-Jul-88
C.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "doit/sfseq/sfpar.inc"
#include "seq/clutil/mesrep.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sffix.inc"
C SHOWCL....SHOW WHERE CALLED FROM
C LENSC.....LENTH OF CHARATER STRING =SHOWCL
C WHICH.....INPUT WHICH SET OF CUTS TO USE
C LENSW.....LENTH OF CHARATER STRING =WHICH
C LGOOD.....INDICATES INPUT SPECIFICATION WAS GOOD
C LFIRST....INDICATES THIS IS THE FIRST GOOD CALL

      CHARACTER*(*) SHOWCL,WHICH
      INTEGER LENSC,LENSW,IOS,COUNT,COUNTM
      INTEGER NSETUP,INDX_ST_LA,INDX_SETUP,FND_ST_THRS
      LOGICAL LGOOD,LFIRST
      DATA COUNT,COUNTM/0,10/
      DATA LFIRST/.TRUE./

      IF(LFIRST)THEN
         LENSC=LEN(SHOWCL)
         IF(LENSC.GT.10)LENSC=10
         LENSW=LEN(WHICH)
         IF(LENSW.GT.8)LENSW=8

c         PRINT 1103,SHOWCL(1:LENSC),WHICH(1:LENSW)
c 1103    FORMAT(' SFCUTS: entry: called from',A10,' specify  "',A8,'"')

         WRITE(CHMESS,1101,IOSTAT=IOS)SHOWCL(1:LENSC),WHICH(1:LENSW)
 1101    FORMAT(' entry: called from',A10, ' specify  "',A8,'"')
         CALL MESLOG('SFCUTS',1,MSNOTI)
      ENDIF

C CNTLSF......CONTROL_SFIND  1 : REGULAR CHAIN FINDING
C                            2 : NO 2ND CALL TO SFCHAN, NO SFMERG
C                            5 : SFBWAL CHANGES TO DATA ACCESS
C CNTLMG......CONTROL_SFMERG 1 : FILL NEW TRACKS WITH MERG USING PRIORITY
C                            2 : FILL MERG LIST WITH ALL POSSIBLE MERGES
C FRSTST......FIRST STARTING LAYER FOR CHAIN SEARCH;SFCHAIN
C LASTST......LAST STARTING LAYER FOR CHAIN SEARCH;SFCHAIN
C LASTFN......LAST  HIT FINDING LAYER FOR CHAIN SEARCH;SFCHAIN
C MXLSFU......MAXIMUM STARTING LAYER NUMBER IN SFNEXT FOR USABLE CHAIN
C MNLBSF......MINIMUM LAYERS BACK FROM MLASSF FOR SFIND TO INITIATE CHAIN
C CHNEXT......DIRECTION OF CHAIN EXTEND USING SFCHAN IN SFIND, 2 PASSES
C             ( 2 LOOPS) 1, OUTWARD, -1 INWARD, 0 SKIP

C SFZSLNWINDO.SF Z SLICE, number of dz windows, on each side
C SFZSLWINDOL.SF Z SLICE, windo number least, start of loop
C SFZSLWINDOM.SF Z SLICE, windo number maximum, end of loop
C SFZSLMAXDZ..SF Z SLICE maximum , to define the granularity
C SFZSLOVLAP..SF Z SLICE overlap, to define slice width
C SFZSLOVRAD..SF Z SLICE overlap radius, radius that has specified overlap
C SFZSLCNTR...SF Z SLICE CENTER, FOR LIMITING Z WHEN RUNNING SFCHAN
C SFZSLWID....SF Z SLICE WIDTH, FOR LIMITING Z WHEN RUNNING SFCHAN
C SFZSLVEL....SF Z SLICE VELOCITY; used to convert time into distance

C LCLNSF......CLONE THE SF CHAINS BEFORE THE EXTENSION LOOP (2 LOOPS)
C LSFSVF......CALL SFSVFM FROM SFIND

C MINLCH...MININUM NUMBER OF HITS IN A CHAIN
C NUTSTD......NUMBER OF THE MUTSTC POSSIBLE TESTS DEFINED
C NUTSTP......NUMBER OF THE TESTS CHECKED TO PASS TO NEXT LAYER
C JUTSTC......TEST NUMBER THAT IS REQUIRED FOR LAST HIT IN CHAIN
C LTOPAS(I,J).NUMBER THAT IS NEEDED TO PASS THE TEST,2ND INDEX IS TYPE
C LOUTOF(I,J).NUMBER OF LAYERS -OUT-OF- WHICH THE TEST IS MADE

C NEDCLR...NUMBER OF WIRES "NEEDED CLEAR" FOR ISOLATED HIT
C AJUSOK...ADJACENT WIRE IS ALREADY USED, OK TO CALLED IT CLEAR
C AFT_USED_OK....CONTINUE SEARCHING LAYER AFTER FINDING USED HIT; OK
C AFT_N_ISO_S_OK.CONTINUE SEARCHING LAYER AFTER FINDING NON-ISOLATED HIT
C                IN STEREO TRANSITION LAYER; OK
C WIDAD0...ADDED SEARCH WIDTH WHEN THERE ARE MANY RECENT FOUND LAYERS
C WIDAD1...ADDED SEARCH WIDTH WHEN THERE ARE MINIMAL RECENT FOUND LAYERS
C WIDAD2...ADDED SEARCH WIDTH WHEN THERE IS A GAP IN FOUND LAYERS
C LENCA1...LENGTH OF NEW CHAIN TO SWITCH FROM A2 TO A1
C LENCA0...LENGTH OF NEW CHAIN TO SWITCH FROM A1 TO A0
C GAPWD1...SIZE OF GAP TO CONVERT TO WIDA11
C GAPWD2...SIZE OF GAP TO CONVERT TO WIDA21
C MDOUBH...MAXIMUM NUMBER OF DOUBLE HITS IN A ROW
C MNOTIS...MAXIMUM NUMBER OF UNISOLATED HITS IN A ROW ALLOWED
C......TEST IF RPWSSF(ILYRT) < XXRPWL/XXRPWN *RPWSSF(ILYRL)
C XXRPWL...MULTIPLYER FOR RPWSSF(ILYRL)
C XXRPWN...MULT FOR RPWSSF(ILYRT)
C......
C cuts for extra width for stereo change and scaling of the stereo shift
C N_ST_THR_DEF...NUMBER OF STEREO THRESHOLDS DEFINED
C THRS_USE_PNT...THRESHOLD TO USE FOR NUMBER OF FOUND STEREO MEASUREMENTS
C THRS_ST_MEAS...THRESHOLD IN NUMBER OF STEREO MEASUREMENTS
C WID_AD_ST......WIDTH ADDED AT STEREO TRANSION AS NUMBER OF 16TH
C                OF THE MAXIMUM WIRE CROSSING (*2 FOR EITHER SIDE)
C WID_AD_ST_LM...WIDTH ADDED AT STEREO TRANSIONS; LIMIT 
C SCAL_ST_SHFT...SCALING OF THE "FULL" STEREO SHIFT TO THE "USED"
C                AS NUMBER OF 16TH OF THE FULL SHIFT
C......
C CCTRLO.........CELL CENTER LO, TEST IF LO ENOUGH IN PREDICTED CELL
C CCTRHI.........CELL CENTER LO, TEST IF HI ENOUGH IN PREDICTED CELL
C WSVADD.........WIRE SHIFT FROM VELOCITY ADD, ADD TO DIRECTION OF VELOCITY
C GAPFIM.........MAXIMUM GAP IN PHI BEFORE DENOTING AS STRAGLER
C V_CHANGE_LIM...LIMIT CHANGE IN VELOCITY (LOW END)  FOR SECOND PASS
C ZTST_RMV_OUTER.REMOVE Z TEST WHEN CHAIN EXTENDS CLOSE TO CHAMB OUTER RADIUS
C ZTST_RMV_INNER.REMOVE Z TEST WHEN CHAIN EXTENDS CLOSE TO CHAMB INNER RADIUS
C ZTST_RMV_IN_AX.REMOVE Z TEST WHEN CHAIN EXTENDS INTO INNER AXIAL LAYERS
C ZTST_SEL_IN_AX.SELECT TRIAL LAYERS WHEN CHAIN EXTENDS INTO INNER AXIAL LAYERS
C ZTST_SEL_VALID.SELECT TRIAL LAYERS WHEN VALIDATED BY ADDITIONAL LAYERS

C ZTST_ESC_ZWIND.REMOVE Z TEST WHEN CHAIN IS WITHIN WINDOW OF ENDPLATE

C ZTST_ESC_CURLV.REMOVE Z TEST AT TOP OF CURLER; VELOCITY IS ABOVE MIN
C ZTST_ESC_LEN_C.REQUIRED CHAIN LENGTH TO TEST FOR VELOCITY AT TOP OF CURLER

C ZTST_ESC_MM0...MINIMUM Z MEASUREMENTS (WITHOUT TEST OF LN_SINCE_Z_CHANGE)
C                TO TEST Z ESCAPE
C ZTST_ESC_MMWL..MINIMUM Z MEASUREMENTS (WITH TEST OF LN_SINCE_Z_CHANGE)
C                TO TEST Z ESCAPE
C ZTST_ESC_MLS...MINIMUM LN_SINCE_Z_CHANGE WHEN USING ZTST_ESC_MMWL 

C......
C VSTORH....HALF OF THE TOTAL NUMBER OF HITS SAVED FOR VELOCITY
C VSTORS....THIS IS SUM OF 1+2+...+VSTORH
C VSTORF....FULL NUMBER OF HITS SAVED FOR VELOCITY CALCULATION
C EXTPV1....EXTRAPOLATED IVTEMP(1) TO PREDICTED VLPRLM
C EXTPV2....EXTRAPOLATED IVTEMP(2) TO PREDICTED VLPRLM
C     EXTPDM....DENOMINATOR FOR EXTPV1,EXTPV2
C EXTPRL....EXTRAPLOATED VLPRLM TO VLPRED
C EXTOLD....EXTRAPLOATED VLPOLD TO VLPRED
C EXTODM....DENOMINATOR FOR EXTPRL,EXTOLD
      LGOOD=.TRUE.


C-----------------------------------------------------------------------
C     "CLEAN"  developed in MC for first SF pass for CLEO-III
C              now used as baseline; default 
C-----------------------------------------------------------------------
         NSETUP=2
         CNTLSF=1
         CNTLMG=1
C some day I will try CNTLMG=2

         SFZSLNWINDO=1
         SFZSLWINDOL=0
         SFZSLWINDOM=0
         SFZSLMAXDZ=1.
         SFZSLOVLAP=100.
         SFZSLOVRAD=1.

         SFZSLVEL=50. *1.E-9


         FRSTST=1+MLSSFZD
         MNLBSF=10
         LASTST=NLASSF-MNLBSF
         LASTFN=NLASSF
         MXLSFU=6
         CHNEXT(1)=-1
         CHNEXT(2)=1

         LCLNSF(1)=.FALSE.
         LCLNSF(2)=.FALSE.
         LSFSVF=.FALSE.
         
         VSTORH=4
         NUTSTD=3
         NUTSTP=2
         JUTSTC=3
         
         MINLCH(1)=6
         LTOPAS(1,1)=2
         LOUTOF(1,1)=4
         LTOPAS(2,1)=9
         LOUTOF(2,1)=12
         LTOPAS(3,1)=2
         LOUTOF(3,1)=3

         NEDCLR(1)=2
         AJUSOK(1)=0
         AFT_USED_OK(1)=1
         AFT_N_ISO_S_OK(1)=0
         WIDAD0(1)=0
         WIDAD1(1)=1
         WIDAD2(1)=2
         LENCA1(1)=2
         LENCA0(1)=3
         GAPWD1(1)=3
         GAPWD2(1)=99

         MDOUBH(1)=1
         MNOTIS(1)=0

         XXRPWL(1)=3
         XXRPWN(1)=5

         N_ST_THR_DEF(  1)=4
         THRS_ST_MEAS(1,1)=0
         WID_AD_ST   (1,1)=8
         WID_AD_ST_LM(1,1)=99
         SCAL_ST_SHFT(1,1)=0
         THRS_ST_MEAS(2,1)=1
         WID_AD_ST   (2,1)=6
         WID_AD_ST_LM(2,1)=99
         SCAL_ST_SHFT(2,1)=0
         THRS_ST_MEAS(3,1)=2
         WID_AD_ST   (3,1)=4
         WID_AD_ST_LM(3,1)=99
         SCAL_ST_SHFT(3,1)=8
         THRS_ST_MEAS(4,1)=4
         WID_AD_ST   (4,1)=4
         WID_AD_ST_LM(4,1)=1
         SCAL_ST_SHFT(4,1)=16
         
         CCTRLO(1)=    .3 *MULST
         CCTRHI(1)=(1.-.3)*MULST
         WSVADD(1)=    2.0*MULST
         GAPFIM(1)=    2.0*MULST

         V_CHANGE_LIM(1)=MULST*99
         ZTST_RMV_OUTER(1)=4
         ZTST_RMV_INNER(1)=4
         ZTST_RMV_IN_AX(1)=4
         ZTST_SEL_IN_AX(1)=1
         ZTST_SEL_VALID(1)=2
         ZTST_ESC_ZWIND(1)=9999
         ZTST_ESC_CURLV(1)=9999
         ZTST_ESC_LEN_C(1)=9999
         ZTST_ESC_MM0(1)=99
         ZTST_ESC_MMWL(1)=99
         ZTST_ESC_MLS(1)=99

C SETUP FOR SECOND PASS
         MINLCH(2)=6
         LTOPAS(1,2)=2
         LOUTOF(1,2)=4
         LTOPAS(2,2)=7
         LOUTOF(2,2)=10
         LTOPAS(3,2)=2
         LOUTOF(3,2)=3

         NEDCLR(2)=1
         AJUSOK(2)=1
         AFT_USED_OK(2)=0
         AFT_N_ISO_S_OK(1)=0
         WIDAD0(2)=0
         WIDAD1(2)=1
         WIDAD2(2)=2
         LENCA1(2)=2
         LENCA0(2)=3
         GAPWD1(2)=2
         GAPWD2(2)=4

         MDOUBH(2)=2
         MNOTIS(2)=1

         XXRPWL(2)=3
         XXRPWN(2)=5

         N_ST_THR_DEF(  2)=4
         THRS_ST_MEAS(1,2)=0
         WID_AD_ST   (1,2)=8
         WID_AD_ST_LM(1,2)=99
         SCAL_ST_SHFT(1,2)=0
         THRS_ST_MEAS(2,2)=1
         WID_AD_ST   (2,2)=6
         WID_AD_ST_LM(2,2)=99
         SCAL_ST_SHFT(2,2)=0
         THRS_ST_MEAS(3,2)=2
         WID_AD_ST   (3,2)=4
         WID_AD_ST_LM(3,2)=99
         SCAL_ST_SHFT(3,2)=8
         THRS_ST_MEAS(4,2)=4
         WID_AD_ST   (4,2)=4
         WID_AD_ST_LM(4,2)=1
         SCAL_ST_SHFT(4,2)=16

         CCTRLO(2)=    .3 *MULST
         CCTRHI(2)=(1.-.3)*MULST
         WSVADD(2)=    2.0*MULST
         GAPFIM(2)=    2.0*MULST

         V_CHANGE_LIM(2)=MULST*1/8
         ZTST_RMV_OUTER(2)=4
         ZTST_RMV_INNER(2)=4
         ZTST_RMV_IN_AX(2)=4
         ZTST_SEL_IN_AX(2)=1
         ZTST_SEL_VALID(2)=2
         ZTST_ESC_ZWIND(2)=200
         ZTST_ESC_CURLV(2)=900
         ZTST_ESC_LEN_C(2)=12
         ZTST_ESC_MM0(2)=3
         ZTST_ESC_MMWL(2)=2
         ZTST_ESC_MLS(2)=2
      IF(WHICH.EQ.'CLEAN')THEN

C-----------------------------------------------------------------------
C startup of CLEO3 with clean tracks but allowing longer gaps
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'STARTUP')THEN

         MXLSFU=12

C FIRST PASS
         MINLCH(1)=8
         LTOPAS(2,1)=7
         LOUTOF(2,1)=12

C SECOND PASS
         LTOPAS(2,2)=5
         LOUTOF(2,2)=10

         ZTST_ESC_MLS(2)=3

C-----------------------------------------------------------------------
C  evolved from
C  the traditional set of cuts used in DUET cleanup track finding
C-----------------------------------------------------------------------
      ELSEIF(
     1        (WHICH.EQ.'SHORT').OR.
     2        (WHICH.EQ.'2NDPASS'))THEN
         MXLSFU=NLAYSF-16

C         LASTST=NLASSF-MNLBSF, with MNLBSF=10,   normally
C this is a difference between 'SHORT' and  '2NDPASS' 
         IF(WHICH.EQ.'SHORT')LASTST=NLASSF-6

C FIRST PASS
         MINLCH(1)=4
         LTOPAS(1,1)=1
         LOUTOF(1,1)=3
         LTOPAS(2,1)=8
         LOUTOF(2,1)=11

         NEDCLR(1)=1
         AFT_N_ISO_S_OK(1)=1
         GAPWD1(1)=2
         GAPWD2(1)=4
         
         MDOUBH(1)=3
         MNOTIS(1)=1
         
         WSVADD(1)=    1.5*MULST
         GAPFIM(1)=    2.5*MULST

         ZTST_RMV_OUTER(1)=99
         ZTST_RMV_INNER(1)=99
         ZTST_RMV_IN_AX(1)=1
         ZTST_SEL_VALID(1)=1

C SECOND PASS
         MINLCH(2)=4
         LTOPAS(1,2)=1
         LOUTOF(1,2)=3
         LTOPAS(2,2)=9
         LOUTOF(2,2)=14
         LTOPAS(3,2)=2
         LOUTOF(3,2)=4
         
         AFT_USED_OK(2)=1
         AFT_N_ISO_S_OK(2)=1

C it is odd that this decreases in the second pass
         MDOUBH(2)=2

         WSVADD(2)=    1.5*MULST
         GAPFIM(2)=    2.5*MULST

         V_CHANGE_LIM(2)=MULST*99

C this is a difference between 'SHORT' and  '2NDPASS' 
         IF(WHICH.EQ.'SHORT')ZTST_RMV_OUTER(2)=99

         ZTST_RMV_INNER(2)=99
         ZTST_RMV_IN_AX(2)=1
         ZTST_SEL_VALID(2)=1

C-----------------------------------------------------------------------
C for use with a TPC detector, first iteration
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'TPC1')THEN
C favorite track was with nwindo=80, iwindo=-8, overlap=2          
C favorite track was with nwindo=80, iwindo=-6, overlap=.5          
         SFZSLNWINDO=20
         SFZSLNWINDO=80
         SFZSLWINDOL=-SFZSLNWINDO
         SFZSLWINDOM=+SFZSLNWINDO
c special, only one window
c tried -8,-9,-7
         SFZSLWINDOL=-6
         SFZSLWINDOM=-6
c end of special, only one window
         SFZSLMAXDZ=2.
         SFZSLOVRAD=1.9
         SFZSLOVLAP=0.5
c         SFZSLOVLAP=2.0
c         SFZSLOVLAP=100000.





         MXLSFU=NLAYSF-40

C         LASTST=NLASSF-MNLBSF, with MNLBSF=10,   normally

C FIRST PASS
         MINLCH(1)=20
         LTOPAS(2,1)=7
         LOUTOF(2,1)=12
         NEDCLR(1)=3

C SECOND PASS
         LTOPAS(2,2)=5
         LOUTOF(2,2)=10
         NEDCLR(2)=2

C-----------------------------------------------------------------------
C   for bunch finding
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'TIMING')THEN
         NSETUP=1
         CNTLSF=2

         FRSTST=MLSFVD+1
         MXLSFU=12
         CHNEXT(1)=0
         CHNEXT(2)=0

C FIRST PASS
         MINLCH(1)=8
         LTOPAS(1,1)=1
         LOUTOF(1,1)=3
         LTOPAS(2,1)=7
         LOUTOF(2,1)=10

         AFT_N_ISO_S_OK(1)=1
         GAPWD1(1)=2
         GAPWD2(1)=4

         MDOUBH(1)=2
         MNOTIS(1)=1

         WSVADD(1)=    99.*MULST

         ZTST_RMV_OUTER(1)=99
         ZTST_RMV_INNER(1)=99
         ZTST_RMV_IN_AX(1)=1
         ZTST_SEL_VALID(1)=1

C-----------------------------------------------------------------------
C   for level3 beam wall tests
C-----------------------------------------------------------------------
      ELSEIF(WHICH.EQ.'SFBWAL')THEN
         CNTLSF=5

         FRSTST=MLSFSV+30
         LASTST=MLSFSV+5
         LASTFN=MLSFSV+1
         MXLSFU=12

C for sfsv running: 
C  set CHNEXT(1)=1,CHNEXT(2)=-1,LCLNSF(2)=.TRUE.,LSFSVF=.TRUE.

         VSTORH=5

C FIRST PASS
         MINLCH(1)=5
         LTOPAS(1,1)=1
         LOUTOF(1,1)=3
         LTOPAS(2,1)=8
         LOUTOF(2,1)=11

         NEDCLR(1)=1
         AFT_N_ISO_S_OK(1)=1
         GAPWD1(1)=2
         GAPWD2(1)=4

         MDOUBH(1)=4
         MNOTIS(1)=1

         WSVADD(1)=    99.*MULST
         GAPFIM(1)=    2.5*MULST

         V_CHANGE_LIM(1)=MULST*99
         ZTST_RMV_OUTER(1)=99
         ZTST_RMV_INNER(1)=99
         ZTST_RMV_IN_AX(1)=1
         ZTST_SEL_VALID(1)=1

C SECOND PASS
         MINLCH(2)=5
         LTOPAS(1,2)=1
         LOUTOF(1,2)=3
         LTOPAS(2,2)=9
         LOUTOF(2,2)=14
         LTOPAS(3,2)=2
         LOUTOF(3,2)=5

         AFT_USED_OK(2)=1
         AFT_N_ISO_S_OK(2)=1

         MDOUBH(2)=6

         WSVADD(2)=    99.*MULST
         GAPFIM(2)=    2.5*MULST

         V_CHANGE_LIM(2)=MULST*99
         ZTST_RMV_OUTER(2)=99
         ZTST_RMV_INNER(2)=99
         ZTST_RMV_IN_AX(2)=1
         ZTST_SEL_VALID(2)=1

C-----------------------------------------------------------------------
C FAIL
C-----------------------------------------------------------------------
      ELSE     
         LENSC=LEN(WHICH)
         WRITE(CHMESS,1102,IOSTAT=IOS)WHICH(1:LENSC)
 1102    FORMAT(' bad input...',A20)
         CALL MESLOG('SFCUTS',1,MSERRO)
         LGOOD=.FALSE.
      ENDIF

C-----------------------------------------------------------------------
C-----------------------------------------------------------------------
C-----------------------------------------------------------------------
C this part is to derive other variables
C this is common to all inputs 
C-----------------------------------------------------------------------

      IF(LGOOD)THEN
         IF(COUNT.LT.COUNTM)THEN
            COUNT=COUNT+1
         ELSE
            LFIRST=.FALSE.
         ENDIF

         SFZSLGRANU=SFZSLMAXDZ/SFZSLNWINDO
         SFZSLWID=SFZSLOVLAP*SFZSLOVRAD*SFZSLGRANU


         
         VSTORS=VSTORH*(VSTORH+1)/2
         VSTORF=VSTORH*2
         IF(VSTORF.GT.MVSTOR)THEN
            WRITE(CHMESS,1108,IOSTAT=IOS)VSTORH
 1108       FORMAT(' fatal error:  VSTORH=',I6)
            CALL MESLOG('SFCUTS',1,MSERRO)
         ENDIF
         
         EXTPV1=6*VSTORH+5
         EXTPV2=-2*VSTORH-5
         EXTPDM=EXTPV1+EXTPV2
         EXTPRL=1
         EXTOLD=1
         EXTODM=EXTPRL+EXTOLD

         
         DO 105 INDX_SETUP=1,NSETUP
            DO 103 INDX_ST_LA=0,MSFZM

               FND_ST_THRS=N_ST_THR_DEF(INDX_SETUP)
 101           CONTINUE
               IF(FND_ST_THRS.GE.1)THEN
                  IF(INDX_ST_LA.LT.
     2               THRS_ST_MEAS(FND_ST_THRS,INDX_SETUP))THEN
                     FND_ST_THRS=FND_ST_THRS-1
                     GO TO 101
                  ENDIF
               ELSE
                  FND_ST_THRS=1
               ENDIF
               THRS_USE_PNT(INDX_ST_LA,INDX_SETUP)=FND_ST_THRS
 103        CONTINUE
 105     CONTINUE

         CALL SFTCUT('SFCUTS',WHICH)
      ENDIF
      
      IF ( LFIRST ) THEN
        WRITE(CHMESS,1109,IOSTAT=IOS)CNTLSF
 1109   FORMAT(' exit: CNTLSF=',I4)
        CALL MESLOG('SFCUTS',1,MSINFO)
      ENDIF

      RETURN
      END




























