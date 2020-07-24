*
* $Id: sfchan.F,v 1.16 2001/08/21 14:09:29 dpp Exp $
*
* $Log: sfchan.F,v $
* Revision 1.16  2001/08/21 14:09:29  dpp
*      -> comment out  print statements
*
* Revision 1.15  2000/10/02 20:50:20  dpp
*      -> diagnostics
*      -> number of VELOCITY_USE layers since z change (and previous value)
*      -> count  COUNTER_Z_CHANGE, LN_SINCE_Z_CHANGE even when Z test is
*         not active,  this has the the side effect that LN_SINCE_Z_CHANGE
*         can be different (and more correct) when tested to continue
*         with smoothing. Previously, there was no smoothing if the Z test
*         was not active.
*      -> remove smoothing if LN_SINCE_Z_CHANGE is 1 or 2 (not just 1)
*         to partially restore condition of no smoothing if there is
*         no Z test.
*
* Revision 1.14  2000/08/18 19:09:20  dpp
*      -> fix bug: zero LN_SINCE_Z_CHANGE at initialization, was stale
*      -> remove Z test near top of curler
*
* Revision 1.13  2000/02/21 23:45:19  dpp
*      -> fix wrap-around error in adjusting search width to include
*         radially out dating from 6-Jan-2000
*
* Revision 1.12  2000/02/12 20:32:08  dpp
*      -> calculate DIFTN from last "velocity use" layer, not last layer
*      -> rationalize difference between PROC_NEW_Z_GROUP and PROC_USE_ZCALC
*      -> eliminate "smoothing" for the layer after a stereo transition
*
* Revision 1.11  2000/01/06 22:22:58  dpp
* ---> reset USE_AJUSOK=0 in process_predetermined_hits
*        lack caused velocity to changed to =0 in extend
* ---> add the close to cell center width increase
*        even at a stereo shift
* ---> change the hits weight for Z calculation to
*        DIFTNV**2 instead of DIFTNV
* ---> increase search width to include the radially-out
*        hit if the velocity calculation is based on
*        an incomplete set of measurements to keep
*        outward, decreasing velocity tracks, (ie tracks
*        that miss the origin) from diverging
* ---> implement  switch to stop searching layer after finding
*        a wide hit in a stereo transition layer
*
* Revision 1.10  1999/12/09 21:27:38  dpp
* remove archaic use of WSHFZ
* add test of Z escape to remove test of Z stragler
*
* Revision 1.9  1999/10/12 15:14:42  dpp
* use array THRS_USE_PNT for stereo transition threshold
*
* Revision 1.8  1999/10/06 12:31:07  dpp
* improved width control for stereo transitions
*
* Revision 1.7  1999/08/10 21:28:41  dpp
* check chain length after hit deletions
*
* Revision 1.6  1999/08/03 21:57:40  dpp
* verify superlayer with hits in layer stereo superlayer
*
* Revision 1.5  1999/04/27 19:30:28  dpp
* delete stragler z; delete divergent inbound extension; count single hits
*
* Revision 1.4  1999/01/29 14:54:53  dpp
* new cuts for stereo transitions; F77
*
* Revision 1.3  1998/07/09 01:39:57  lyon
* Retrofitted for sf_data_access.inc
*
* Revision 1.2  1998/05/08 14:14:06  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
* DSF_CHVIEW1: waiting to acknowledge chain,"K"ill to ignore
*
* Revision 1.9  1997/02/25 21:45:40  dpp
* use SVX layers in addition to CD layers
*
* Revision 1.8  1996/06/07 20:19:00  dpp
* set ILYRT=LLYRS earlier to avoid array bounds problem in "hit qual or error"
*
* Revision 1.7  1996/03/28 20:31:51  dpp
* smooth the LVLCHN on the ENDHI end, for beamwall (CNTLSF=5) only
*
* Revision 1.6  1996/03/21 19:28:07  zfiles
* provide direct access to data, without SFDATA buffer, for beam wall use
*
* Revision 1.5  1996/03/20 17:15:55  zfiles
* various changes for speed: veroing in SFIND, removed SFCDAD
*
* Revision 1.4  1996/02/29 17:39:28  zfiles
* removed error in which velocity was calculated for the (logical)small
* radius end when in extend mode, this is a change in output; it can
* affect merges
*
* Revision 1.3  1996/02/14 21:04:15  zfiles
* remove call to SFLWF; use SFLSFA etc
*
* Revision 1.2  1995/04/20 22:39:43  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:43  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/18 22/03/94  14.34.34  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.25  by  Dan Peterson
*CMZ :  2.00/00 08/05/89  16.01.07  by  Dan Peterson
*-- Author : DAN PETERSON

      SUBROUTINE SFCHAN(SFCTYP,INPUT1,INPUT2,SFCERR)

C......................................................................
C.
C. SFCHAN - Create the basic chains from close hits
C.
C. COMMON    : /STEPCn/
C. CALLS     :  DSFHIT,DSFDMP,DSFHTF
C. CALLED    : SFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 20-JULY-88
C. LAST MOD  : 12-Oct-88
C.
C. Modification Log.
C. 22-Mar-95  backward compatible with CLEO2 
C. 21-Aug-94  stereo wire z corrections
C. 20-Nov-91  change test to do smoothing to check for having
C.            crossed VD-DR and stereo layers, not just layer count
C.            allow multiple double hits in velocity if (OKWSTR.GT.2)
C. 19-Nov-91  modify gap in phi test with WSHFV,(taken out 20-Nov-91)
C.            change the initial velocity for extending chains backward
C.            by using LVLCHN
C. 26-Mar-91  add variables WNRXL,WNRXH,WNFXL,WNFXH,WLF0L,WLFOH,
C.            L/H variables to track gaps in phi, test for LN_GOOD_STR,
C.            no longer ignore LN_GOOD_STR test for SFCTYP=2,
C. 21-Mar-91  allow wider than 2 wide strings if velocity is large
C. 27-Dec-90  change smoothing correction to WNPX to not only
C.            shift back half of the last phi change but
C.            also, recoorect with half of the last radius change
C.            multiplied with VEL_PRED
C. 01-Aug-89  recreates and extends existing track if SFCTYP=2
C. 28-July-89 now run backwards if MLYRS < LLYRS
C. 25-July-89 calculate slopes by arc width instead of cell
C.            using new variable RPWSSF(ILYRT)
C. 22-June-89 use SUPER layers instead of regular SF layers;
C.            note: LINCHN,MINCHN,ISTFOR,ISTBAK,INUCHN are still
C.            addressed by the standard SF index;
C. 12-June-89 check INUCHN in the HIT_QUALITY_OR_ERR procedure for
C.            double hits because some chain may have skipped a
C.            partner in a double hit; use DOUBPT to point to
C.            other part of a double hit so that it can be
C.            deleted in the case of an aborted chain
C. 22-May-89  limit amount of modifiing WNPX by scaling by
C.            (RAD_1_PREV-RAD_2_PREV)/RFX_L(1);
C.            added RAD_1_PREV and RAD_2_PREV;
C. 19-May-89  limit amount of modifying WNPX by scaling by
C.            RFX_N(1)/RFX_L(1);
C.            ignore partner in double hit if it is too far away
C.            by modifying WNFX,MISNOW,INUCHN;
C. 17-May-89  project out from point that is some average of last
C.            two hit layers by modifying WNPX; added NGAPCL;
C.            cannot modify WLF0 or WNRX: will affect velocity;
C. 16-May-89  do velocity between midpoints of adjacent found layers,
C.            instead of the found layers themselves, this introduces
C.            velocity smoothing fundimentally; added RFX_L;
C.            POSSIBLY SMOOTHING (VEL_PRED=) IS NO LONGER NEEDED;
C. 15-May-89  do velocity with variable (=VSTORF) saved steps;
C. 12 MAY-89  added more initiallization (for DSFHIT);
C.            now delete hits from view after finding chain
C. 08-May-89  retrieve hit map from ZFILES, with address ADMAP0;
C.            get seed wire number WNTU from ZFILES;
C. 05-May-89  retrieve raw data from ZFILES;
C.            Change from NWIRTF to NWRSSFF (in SFCOM);
C.            added variabes NWIRTU and NWIRLU for less indexing;
C. 12-Oct-88  fill LVLCHN(1,n) when ISTORV=4 not LENCHN=4;
C. 30-Sept-88 use variable DFAHLD to modifiy search path;
C. 26-Sept-88 use pointers to hits in layers instead of all wires,
C.            restructure code for main loop over seeds
C. 21-Sept-88 change velocity usage for doublets;
C. 05-Aug-88  now using IWWIDT for total added width; eliminated IWWID;
C.
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"

#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

C-----------------------------------------------------------------------
C  VARIABLES WITHIN SF_DATA_ACCESS
C-----------------------------------------------------------------------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"


C NOTE: FUNCTION:ISHFT IS LOGICAL SHIFT, NOT ARITH, IE  -1/2  -->  +2**31-1

C-----------------------------------------------------------------------
C  ARGUMENT VARIABLE DOCUMENTION
C-----------------------------------------------------------------------

C SFCTYP...FLAG; 1 FOR INITIAL SCAN, 2 TO EXTEND EXISTING CHAIN,
C                3 FOR GETTING GOOD ISOLATED CHAINS FOR TIMING
C INPUT1...LAYER TO START FOR SFCTYP=1; CHAIN# FOR SFCTYP=2
C INPUT2...LAYER TO STOP  FOR SFCTYP=1; BACKTRACK (SIGNED) FOR SFCTYP=2
C SFCERR...RETURNED ERROR, =0 FOR NO ERROR

      INTEGER SFCTYP,INPUT1,INPUT2,SFCERR

C-----------------------------------------------------------------------
C  LOCAL VARIABLE DOCUMENTION
C-----------------------------------------------------------------------

C ADTMP....ADDRESS TEMPORARY
C ADRHN....ADDRESS OF KNOWN HIT WIRE, IN THE SHORT HIT LIST
C ADRHT....ADDRESS OF TESTED HIT WIRE, IN THE SHORT HIT LIST
C ADTSF....ADDRESS OF THE "TESTED" WIRE,            SF INDEXING
C ADASF....ADDRESS OF ADJACENT HIT WIRE, IF ANY,    SF INDEXING
C ADLSF....ADDRESS OF THE LAST FOUND WIRE,          SF INDEXING
C ADTCD....ADDRESS OF THE "TESTED" WIRE,            CD INDEXING
C ADACD....ADDRESS OF ADJACENT HIT WIRE, IF ANY,    CD INDEXING
C ADLCD....ADDRESS OF THE LAST FOUND WIRE,          CD INDEXING
C ADTSSF...ADDRESS OF THE "TESTED" WIRE,            SSF INDEXING
C ADASSF...ADDRESS OF ADJACENT HIT WIRE, IF ANY,    SSF INDEXING
C ADLSSF...ADDRESS OF THE LAST FOUND WIRE,          SSF INDEXING
C ADTSV....ADDRESS OF THE "TESTED" WIRE,            SV INDEXING
C CLIN.....ADDRESS CORRECTION FOR LINCHN
C CMIN.....ADDRESS CORRECTION FOR MINCHN
C CFOR.....ADDRESS CORRECTION FOR ISTFOR
C CBAK.....ADDRESS CORRECTION FOR ISTBAK
C DIFTNV...DIFFERENCE IN TANSSF(ILYRT) AND TANSSF(ILYRV)
C          THIS IS RELATIVE TO THE LAST VELOCITY CALCULATING LAYER
C DIFTNVA..(ABS) DIFFERENCE IN TANSSF(ILYRT) AND TANSSF(ILYRV)
C DIFTNL...DIFFERENCE IN TANSSF(ILYRT) AND TANSSF(ILYRL)
C          THIS IS RELATIVE TO THE LAST LAYER, VELOCITY CALCULATING OR NOT
C DIFTNLA..(ABS) DIFFERENCE IN TANSSF(ILYRT) AND TANSSF(ILYRL)
C ZCALCW...WEIGHT FOR Z CALCULATION
C WDIFF....DIFFERENCE IN WIRE NUMBER FOR Z CALCULATION
C DFAHLD...HOLD THE OFFSET OF THE LAST 4 WIRES RELATIVE TO THE 5TH BACK
C ENDLO....INDEX FOR LOGICAL LOW END OF CHAIN
C ENDHI....INDEX FOR LOGICAL HIGH END OF CHAIN
C GAPFIL...GAP IN PHI (CELLS) TO LOW SIDE OF CURRENT HIT FROM LAST
C GAPFIH...GAP IN PHI (CELLS) TO HIGH SIDE OF CURRENT HIT FROM LAST
C ICHAIN...CURRENT CHAIN NUMBER
C IDIRSF...DIRECTION OF STEP
C ISFSSF...NUMBER OF THE SF SUBLAYER IN THE INPUT SUPER LAYER
C RAD_LAST_VEL......RADIUS OF THE LAST (PREVIOUS) FOUND HIT USED IN VELOCITY
C RAD_1_PREV........RADIUS 1 LAYER BACK, WHETHER OR NOT A HIT WAS FOUND
C RAD_2_PREV........RADIUS 2 LAYERS BACK, WHETHER OR NOT A HIT WAS FOUND
C CELL_HEIGHT.......APPRXIMATE CELL HEIGHT FOR CALCULATING CROSS_CELL
C IFCOMP...WIRE NUMBER COMPARATOR, IN VELOCITY
C IFADDR...WIRE NUMBER ADDER, IN VELOCITY
C IV.......COUNTER USED IN VELOCITY SECTION
C RFX_ST...TEMPORARY STORAGE IN VELOCITY
C IRTEMP...TEMPORARY RADIUS SUM, SHIFTED PRIOR TO DIVISION
C IRELX....FRACTIONAL CORRECTION TO PREDICTED WIRE NUMBER
C ILYRT....LAYER CURRENTLY BEING TESTED
C ILYRL....LAYER NUMBER, LAST GOOD FOUND HIT
C ILYRV....LAYER NUMBER, LAST GOOD FOUND HIT USED IN VELOCITY
C IUTEST...COUNTER FOR THE SUCCESS TESTS
C IULOC....LOCATION IN THE "USED" ARRAYS FOR THE LATEST DATA
C IULOCR...OUTER RANGE IN THE "USED" ARRAYS
C KEEP_CHAIN........FLAG; =1 IF CURRENT CHAIN IS TO BE KEPT
C GOOD_HIT..........FLAG; =1 IF GOOD HIT IS FOUND
C NONSTRAGLER.......FLAG; =1 IF CURRENT HIT, IF IT IS LAST, IS NOT A STRAGLER
C VEL_INBOUNDS......FLAG; =1 IF VELOCITY AFTER CURRENT HIT IS IN BOUNDS
C IRTRN1...RETURN POINTER 1
C IHQLER............HIT QUALITY OR ERROR CODE WORD
C                   ERROR: -5  NO HIT FOUND IN SFDATA MAP
C                   ERROR: -4  HIT FLAGGED BAD IN SFIND,
C                                   INUCHN=-1  HIGH TIME, >1/2 CELL
C                                   INUCHN=-2  HIGH PH (PEDISTAL)
C                                   INUCHN=-3  ALREADY USED IN DUET
C                                   INUCHN=-4  ADJACENT TO USED IN DUET
C                   ERROR: -3  ALREADY USED IN ANOTHER CHAIN IN SFIND
C                   QUAL:  -2  TRIPLET OR WORSE HIT, HIT NOT USED
C                   QUAL:   0  GOOD
C                   QUAL:   1  DOUBLET HIT, ADDRESS OF PARTNER IN ADASF
C                   QUAL:   2  TRIPLET OR WORSE HIT, HIT USED
C USE_AJUSOK........=1 IF IHQLER IS GOOD ONLY BY USING THE AJUSOK FLAG
C CONT_SEARCH_LAYER.FLAG; =1 IF CONTINUE TO SEARCH FOR HIT IN THIS LAYER
C IDIRW....DIRECTION TO MOVE TO LOOK FOR THE NEXT POSSIBLE WIRE
C IREL.....INDEX, RELATIVE POSITION OF THE TESTED WIRE TO THE PREDICTED
C IRELN....NEW (PROBATIONAL) VALUE OF IREL
C I........GENERAL COUNTER
C LLYRS....LAYER NUMBER TO START, THIS IS NOW A SUPER SSF LAYER
C LYRSF....LAYER NUMBER IN SF     OF A WIRE IN AN SSF LAYER LLYRS
C LSFTMP...LAYER SF, TEMPORARY; FOR CALCULATION OF CD ADDRESS
C LYRCD....LAYER NUMBER IN CDGEOM OF A WIRE IN AN SSF LAYER LLYRS
C LADRS....ADDRESS OF FIRST WIRE IN THE INPUT LAYER, LLYRS,   SSF INDEXING
C LADR.....ADDRESS OF FIRST WIRE IN PRESENT LAYER,            SSF INDEXING
C LN_GOOD...........CHAIN LENGTH; GOOD HITS
C LN_GOOD_STR.......CHAIN LENGTH UP TO LAST NONSTRAGLER HIT
C LN_GOOD_VEL.......CHAIN LENGTH UP TO ERATIC CHANGE IN VELOCITY
C VEL_GOOD_VEL......SAVED VALUE OF IVTEMP(1) FOR LAST LN_GOOD_VEL
C LN_GOOD_Z.........CHAIN LENGTH UP TO ACCEPTED HITS IN PREVIOUS Z GROUP
C LN_GOOD_Z_TRIAL...GOOD CHAIN LENTH THAT WILL BE ACCEPTED IF CURRENT GROUP
C                   IS LONG ENOUGH
C LN_GOOD_Z_NEXT....THE NEXT VALUE FOR LN_GOOD_S_TRIAL AFTER LN_GOOD_S_TRIAL
C                   IS ACCEPTED
C VEL_GOOD_Z........SAVED VALUE OF IVTEMP(1) FOR LAST LN_GOOD_Z
C VEL_GOOD_Z_TRIAL..SAVED VALUE OF IVTEMP(1) FOR LAST LN_GOOD_Z_TRIAL
C VEL_GOOD_Z_NEXT...SAVED VALUE OF IVTEMP(1) FOR LAST LN_GOOD_Z_NEXT
C Z_TEST_ACTIVE.....FLAG, =1 IF UNCERTIFIED Z-CHANGES CAN BE ELIMINATED 
C SELECT_TRIAL......FLAG; =1 IF "TRIAL" VALUES ARE TO BE SELECTED
C PROC_NEW_Z_GROUP..FLAG, =1 IF THIS IS THE FIRST LAYER HIT IN Z GROUP
C                   THIS IS NOT NECCESARILY WHERE THE Z CALCULATION IS MADE
C PROC_USE_ZCALC....FLAG, 0:DO NOT USE IN Z CALCULATION, 1: USE IN Z CALC
C                   THIS IS NOT NECCESARILY THE FIRST LAYER IN THE GROUP 
C COUNTER_Z_CHANGE..COUNT THE NUMBER OF Z CHANGES FOUND, diff from Z calcs
C LN_SINCE_Z_CHANGE.CHAIN LENGTH SINCE (AND INCLUDING) THE LAST Z CHANGE
C VELU_SINCE_Z_CH...NUMB VELOCITY_USE>0 LAYERS SINCE (AND INCL) LAST Z CHANGE
C VELU_PREV_Z_CH....NUMB VELOCITY_USE>0 LAYERS AFTER (AND INCL) PREV Z CHANGE
C MLYRS....LAYER NUMBER TO FINISH
C MADRS....ADDRESS OF LAST WIRE IN THE INPUT LAYER, LLYRS,    SSF INDEXING
C MADR.....ADDRESS OF LAST WIRE IN PRESENT LAYER,             SSF INDEXING
C MWIRS....MAXIMUM WIRE NUMBER (NOT NUMBER OF WIRES) IN INPUT LAYER
C MISNOW...MISS DISTANCE, FOUND-PREDICTED, MULTIPLIED
C MISSUM...MISS SUM (LATER, AVERAGE), FOUND-PREDICTED
C MISLO....FLAG THAT THE LO DIRECTION HAS RUN OUT OF RANGE
C MISHI....FLAG THAT THE HI DIRECTION HAS RUN OUT OF RANGE
C NWIRTX...NUMBER OF WIRES IN THE TESTED LAYER, MULTIPLIED
C NWIRTU...NUMBER OF WIRES IN SF LAYER ILYRT
C NWIRLU...NUMBER OF WIRES IN SF LAYER ILYRL
C NWIRVU...NUMBER OF WIRES IN SF LAYER ILYRV
C NGAPCH...CURRENT SIZE OF GAP IN THIS CHAIN
C NGAPCL...SIZE OF THE GAP BEFORE THE LAST FOUND HIT
C NNOTIS...CURRENT NUMBER OF UNISOLATED HITS
C NLO......NUMBER OF WIRES TO TEST IN THE LO DIRECTION
C NHI......NUMBER OF WIRES TO TEST IN THE HI DIRECTION
C N_RADL...NUMBER OF WIRES SHIFT REQUIRED TO ALLIGN WITH RADIALLY OUT
C NUMR.....NUMBER OF WIRES TO MOVE TO LOOK FOR THE NEXT POSSIBLE WIRE
C NTEST....NUMBER OF WIRES THAT HAVE BEEN TESTED IN CURRENT LAYER NOW
C PHICOR...CORRECTION TO PHI BASED ON DFAHLD
C STRBAD...START OF THE BAD PART OF CHAIN TO BE DELETED, ie LN_GOOD_STR+1
C DELETZ...NUMBER OF Z MEASUREMNTS DELETED
C IZHIT....LOOPING VARIABLE FOR COMPRESSING THE Z HIT LIST
C NZHIT....TEMPORARTY NUMBER OF Z HITS  FOR COMPRESSING THE Z HIT LIST
C VELOCITY_USE..FLAG, 0:DO NOT USE IN VELOCITY, >0:USE IN VELOCITY OR Z
C VEL_PRLM..........PREDICTED VELOCITY, PRELIMINARY CALCULATION
C VEL_PRED..........PREDICTED VELOCITY, AFTER SMOOTHING
C VEL_PRLM_OLD......PRELIM PREDICTED VELOCITY, OLD, FROM PREVIOUS FOUND HIT
C VEL_PRED_OLD......PREDICTED VELOCITY, AFTER SMOOTHING,OLD,PREVIOUS FOUND HIT
C VEL_PRED_OLDA.....ABS(VEL_PRED_OLD)
C CROSS_WIDTH.......WIDTH OF CELL CROSSED DUE TO VELOCITY, ABS
C WID_AD_PREV.......SEARCH WIDTH ADDED, BASED ON CONDITIONS OF PREVIOUS HITS
C WID_AD_LAYR.......SEARCH WIDTH ADDED, BASED ON CONDITIONS OF THIS LAYER
C FND_ST_THRS.......FOUND STEREO THRESHOLD, USED TO SELECT SCALING
C WID_AD_STER.......WIDTH ADDED FOR CHANGE IN STEREO ANGLE
C WID_AD_SCAL.......WIDTH ADDED (TO NHI OF NLO) FOR SCALED STEREO SHIFT
C WIDHIS...WIDTH HISTORY ARRAY
C NEW_VEL_OR_Z......FLAG FOR DIAGNOSTIC, 1:NEW VELOCITY CALCULATED, 2:NEW Z
C WIRNUM...WIRE NUMBER IN CDGEOM, OR IN SF, BUT NOT SSF
C WNRX.....WIRE NUMBER  -NOW -RADIALOUT-MULTIPLIED
C WNRXZ....WIRE NUMBER  -NOW -RADIALOUT-MULTIPLIED if in layer with same TANSSF
C WNRXL....WIRE NUMBER  -NOW -RADIALOUT-MULTIPLIED -LOLIMIT
C WNRXH....WIRE NUMBER  -NOW -RADIALOUT-MULTIPLIED -HILIMIT
C WNPX.....WIRE NUMBER  -NOW -PREDICTED-MULTIPLIED
C WNPXA....WIRE NUMBER  -NOW -PREDICTED-MULTIPLIED, MULSTH ADDED for rounding
C WNPXZ....WIRE NUMBER  -NOW -PREDICTED-MULTIPLIED if in layer with same TANSSF
C WNPU.....WIRE NUMBER  -NOW -PREDICTED-USER
C WNTU.....WIRE NUMBER  -NOW -TESTING  -USER
C WNFX.....WIRE NUMBER  -NOW -FOUND    -MULTIPLIED
C WNFXL....WIRE NUMBER  -NOW -FOUND    -MULTIPLIED   -LOLIMIT
C WNFXH....WIRE NUMBER  -NOW -FOUND    -MULTIPLIED   -HILIMIT
C WLF0.....WIRE NUMBER  -LAST-FOUND    -MULTIPLIED AND RELATIVE TO 0
C WLF0L....WIRE NUMBER  -LAST-FOUND    -MULTIPLIED AND RELATIVE TO 0 -LOLIMIT
C WLF0H....WIRE NUMBER  -LAST-FOUND    -MULTIPLIED AND RELATIVE TO 0 -HILIMIT
C WSHFV....SHIFT IN WIRE NUMBER DUE TO VELOCITY, MULTIPLIED
C WSHFZF...RELATIVE SHIFT IN WIRE POSITION DUE TO Z, MULTIPLIED, FULL 
C WSHFZS...RELATIVE SHIFT IN WIRE POSITION DUE TO Z, MULTIPLIED, SCALED
C WSHFZC...RELATIVE SHIFT IN WIRE POSITION DUE TO Z, MULTIPLIED, COMPLEMENT 
C WSHFX....SHIFT OF WIRE POSITION, DUE TO DOUBLETS, MULTIPLIED
C WSHCH....SHIFT IN WIRE POSITION CHANGED FOR IMPLAUSABLE DOUBLET
C WSHSM....SHIFT IN PREDICTED WIRE POSITION DUE TO SMOOTHING
C ZNOWX....Z VALUE IN CURRENT LAYER; ILYRT, IN meters MULTIPLIED
C ZNEWX....NEW Z VALUE IN CURRENT LAYER; ILYRT, IN meters MULTIPLIED
C ZNEWXA...NEW Z VALUE IN CURRENT LAYER; ILYRT, IN meters MULTIPLIED, ABS
C ZTEMP....TEMP VALUE OF Z IN CALULATION
C SCAL_STEREO_SHIFT.SCALING OF STEREO SHIFT ESPECIALLY FOR EARLY SHIFTS
C ABORT....

      INTEGER ADTMP, ADRHN, ADRHT
      INTEGER ADTSF, ADASF, ADLSF
      INTEGER ADTCD, ADACD, ADLCD, ADTSSF,ADASSF,ADLSSF
      INTEGER ADTSV
      INTEGER CLIN,  CMIN,  CFOR,  CBAK
      INTEGER DIFTNV, DIFTNVA,DIFTNL, DIFTNLA,ZCALCW,WDIFF
      INTEGER DFAHLD
      INTEGER ENDLO, ENDHI
      INTEGER GAPFIL,GAPFIH
      INTEGER ICHAIN,IDIRSF
      INTEGER ISFSSF
      INTEGER RAD_LAST_VEL, RAD_1_PREV,RAD_2_PREV,CELL_HEIGHT
      INTEGER IFCOMP,IFADDR,IV
      INTEGER RFX_ST,IRTEMP,IFTEMP
      INTEGER ILYRT, ILYRL, ILYRV
      INTEGER IUTEST,IULOC, IULOCR
      INTEGER KEEP_CHAIN, GOOD_HIT, NONSTRAGLER, VEL_INBOUNDS
      INTEGER IRTRN1,IHQLER,USE_AJUSOK,CONT_SEARCH_LAYER
      INTEGER IDIRW, IREL,  IRELN, I
      INTEGER IRELX
      INTEGER LLYRS, LYRSF, LSFTMP,LYRCD, LADRS, LADR  
      INTEGER LN_GOOD_STR,LN_GOOD_VEL,LN_GOOD,VEL_GOOD_VEL
      INTEGER LN_GOOD_Z,LN_GOOD_Z_TRIAL,LN_GOOD_Z_NEXT
      INTEGER VEL_GOOD_Z,VEL_GOOD_Z_TRIAL,VEL_GOOD_Z_NEXT
      INTEGER Z_TEST_ACTIVE,SELECT_TRIAL
      INTEGER PROC_NEW_Z_GROUP,PROC_USE_ZCALC
      INTEGER COUNTER_Z_CHANGE,LN_SINCE_Z_CHANGE
      INTEGER VELU_SINCE_Z_CH,VELU_PREV_Z_CH
      INTEGER MLYRS, MADR,  MADRS, MWIRS
      INTEGER MISNOW,MISSUM,MISLO, MISHI
      INTEGER NWIRTX,NWIRTU,NWIRLU,NWIRVU
      INTEGER NGAPCH,NGAPCL,NNOTIS
      INTEGER NLO,   NHI,   N_RADL,NUMR,  NTEST
      INTEGER PHICOR
      INTEGER STRBAD,DELETZ,IZHIT,NZHIT
      INTEGER VELOCITY_USE
      INTEGER VEL_PRLM,VEL_PRED
      INTEGER VEL_PRLM_OLD,VEL_PRED_OLD,VEL_PRED_OLDA
      INTEGER NEW_VEL_OR_Z
      INTEGER CROSS_WIDTH
      INTEGER WIRNUM
      INTEGER WID_AD_PREV,WID_AD_LAYR,WID_AD_STER,WID_AD_SCAL
      INTEGER FND_ST_THRS
      INTEGER WIDHIS(MWIDH)
      INTEGER WNRX,  WNRXZ, WNRXL, WNRXH
      INTEGER WNPX,  WNPXA, WNPXZ, WNPU,  WNTU
      INTEGER WNFX,  WNFXL, WNFXH
      INTEGER WLF0,  WLF0L, WLF0H
      INTEGER WSHFV, WSHFZF,WSHFZS,WSHFZC
      INTEGER WSHFX, WSHSM, WSHCH
      INTEGER ZNOWX, ZNEWX, ZNEWXA, ZTEMP
      INTEGER SCAL_STEREO_SHIFT
      INTEGER ABORT


C-----------------------------------------------------------------------
C  VARIABLES WITHIN THE PROCEDURE HIT-QUALITY-OR-ERROR
C-----------------------------------------------------------------------

C ISIGNP...SIGN OF THE SIDE THAT IS BEING CHECKED
C ADPSF....LOCAL PROCEDURE WIRE ADDRESS,            SF INDEXING
C ADPCD....LOCAL PROCEDURE WIRE ADDRESS,            CD INDEXING
C ADPSSF...LOCAL PROCEDURE WIRE ADDRESS,            SSF INDEXING
C ADPSV....LOCAL PROCEDURE WIRE ADDRESS,            SV INDEXING
C CELL_CLEAR.......=1 IF THE CELL IS TREATED AS CLEAR
C FNDCLR...NUMBER OF WIRES FOUND CLEAR; TO COMPARE TO NEDCLR
C IDIRWI...INITIL DIR TO MOVE FOR NEXT WIRE, USED FOR ADJACENT
C CONTIG...........=1 IF HITS ARE CONTIGUOUS ON THIS SIDE OF PRIMARY WIRE
C WSTRNG...TOTAL WIDTH OF STRING, COMPARE TO OKWSTR
C WSTRP....WIDTH OF STRING TO PLUS  SIDE
C WSTRM....WIDTH OF STRING TO MINUS SIDE
C OKWSTR...OK-WIDTH-OF-STRING, WIDENING ALLOWED FOR LARGE VEL_PRED

      INTEGER ISIGNP,ADPSF,ADPCD,ADPSSF,ADPSV
      INTEGER CELL_CLEAR,FNDCLR,IDIRWI
      INTEGER CONTIG,WSTRNG,WSTRP,WSTRM,OKWSTR

C-----------------------------------------------------------------------
C  NEW VARIABLES FOR THE EXTENSION OF EXISTING CHAINS
C-----------------------------------------------------------------------
C BACKTR............NUMBER OF ELEMENTS TO BACKTRACK
C STOPBK............COUNTER TO STOP BACKTRACKING
C PROC_BACKTRACK....=1 I IT IS A BACKTRACK LAYER; =0 IF A NORMAL LAYER
C LVL_COMPAR........SAVED VALUE OF LVLCHN TO COMPAR NEW VALUES
C LVL_COMPAR_A......ABS(LVL_COMPAR)
C LVL_RESTOR........SAVED VALUE OF LVLCHN; TO RETSTOR WHEN DELETING EXTENSION
C LVLENDA...........ABS(IVTEMP(1))
C LEN_RESTOR........SAVED LENGTH; TO RETSTOR WHEN DELETING EXTENSION

      INTEGER BACKTR,STOPBK,PROC_BACKTRACK
      INTEGER LVL_COMPAR,LVL_COMPAR_A,LVL_RESTOR,LVLENDA,LEN_RESTOR

C-----------------------------------------------------------------------
C  VARIABLES WITHIN THE PROCEDURE TO PROCESS-PREDETERMINED-HITS
C-----------------------------------------------------------------------

C ILYRA....LAYER NUMBER OF ADJACENT HIT
C ILYRX....LAYER NUMBER OF NEXT HIT
C WNAU.....WIRE NUMBER  -NOW -ADJACENT -USER
C WNXFU....WIRE NUMBER  -NEXT-FOUND    -USER
C IADRX....ADDRESS OF NEXT FOUND WIRE

      INTEGER ILYRA,ILYRX,IADRX
      INTEGER WNAU, WNXFU


C================================================================
C================================================================
C
C   -----------  Executable code starts here  ---------------
C
C================================================================
C================================================================

C================================================================
C  load up the address of the hit map in ZFILES
C  careful, it changes between calls, it should be
C  reset after any other ZFILES calls, even DSFHIT
C  (which may call ZFILES in a new version)
C================================================================
      sf_data_input_flag=1
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

C================================================================
C  determine program flow
C================================================================

      SFCERR=0
      IF(SFCTYP.EQ.1)GO TO 1
      IF(SFCTYP.EQ.2)GO TO 31
      SFCERR=1
      GO TO 23

C================================================================
C  start of normal search for new chains
C================================================================

1     ICHAIN=NCHAIN
      LLYRS=INPUT1
      MLYRS=INPUT2

      IRTRN1=1
      IF(MLYRS.GT.LLYRS)GO TO 611
      GO TO 621
3     CONTINUE

      STOPBK=-1
      LADRS=LADSSF(LLYRS)
      MWIRS=NWRSSF(LLYRS)-1
      MADRS=MADSSF(LLYRS)

C-----------------------------------------------------------------------
C  this is to loop over all wires in a layer, LLYRS, as possible seeds
C  either method of choosing wires sends wire numbers to the common code
C  this is protected from "alternate readout" by the upper limit at MWIRS
C-----------------------------------------------------------------------

#if !defined(CLEO_SFLLIS)
      WNTU=0
11    IF(WNTU.GT.MWIRS)GO TO 21
#endif

C-----------------------------------------------------------------------
C  link-list driven
C  this is loop over all real hits in layer, LLYRS
C  as opposed to the case of driving the program from a hit map only
C  either method of choosing wires sends wire numbers to the common code
C-----------------------------------------------------------------------

#if defined(CLEO_SFLLIS)
      ISFSSF=0
9     ISFSSF=ISFSSF+1
      IF(ISFSSF.GT.MSFSSF)GO TO 21
      LYRSF=LAYSF(ISFSSF,LLYRS)
      IF(LYRSF.EQ.0)GO TO 21

      sf_data_input_layr_sf=LYRSF
#define SF_DATA_1STH
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHN=sf_data_output_hit_adr

11    IF(ADRHN.EQ.0)GO TO 9

      sf_data_input_hit_adr=ADRHN
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
      WIRNUM=sf_data_output_wire

C select VD PRIMARY readout, and select requested SVX readout
      IF(WIRNUM.LT.0           )GO TO 19
      IF(WIRNUM.GE.NWRSF(LYRSF))GO TO 19
      WNTU=WIRSSF(INSFSF(LYRSF)+WIRNUM)
#endif

C-----------------------------------------------------------------------
C  common code
C-----------------------------------------------------------------------

      IF(ICHAIN.GE.MCHAIN)GO TO 21
      ILYRT=LLYRS
      LADR=LADRS
      MADR=MADRS
      ADTSSF=LADR+WNTU
      WSHFV=0
      CROSS_WIDTH=0

      IDIRWI=-1
      IRTRN1=1
      GO TO 711
13    CONTINUE
      
      IF(IHQLER.EQ.0)GO TO 41
      IF(IHQLER.EQ.1)GO TO 41

C-----------------------------------------------------------------------
C  record failed seeds
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
      CALL DSFHFS(ADTSF,LLYRS,WNTU,IHQLER)
#endif

C-----------------------------------------------------------------------
C  not link-list driven; increment wire number
C-----------------------------------------------------------------------

#if !defined(CLEO_SFLLIS)
 19   WNTU=WNTU+1
#endif

C-----------------------------------------------------------------------
C  link-list driven; get next real hit in layer
C-----------------------------------------------------------------------

#if defined(CLEO_SFLLIS)
 19   CONTINUE

      sf_data_input_hit_adr=ADRHN
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHN=sf_data_output_hit_adr

#endif

C-----------------------------------------------------------------------
C  common code; return of loop and normal exit
C-----------------------------------------------------------------------

      GO TO 11
21    NCHAIN=ICHAIN

C-----------------------------------------------------------------------
C  errors
C-----------------------------------------------------------------------

 23   IF(SFCERR.NE.0)THEN
#if defined(CLEO_SFDIAG)
         PRINT 1001,SFCERR,SFCTYP,ICHAIN
 1001    FORMAT(' SFCHAN: EXITING; ERROR=',I2,'  ENTRY TYPE',I2,
     1        ' CHAIN#',I3)
#endif
      ENDIF

C-----------------------------------------------------------------------
C  end or the search loop; flag ADMAP0 as "stale" and return
C-----------------------------------------------------------------------

      sf_data_input_flag=0
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

      RETURN

C=======================================================================
C  extend pre-existing chain
C=======================================================================

 31   ICHAIN=INPUT1

      IF((ICHAIN.LT.1).OR.(ICHAIN.GT.NCHAIN))THEN
         SFCERR=21
         GO TO 23
      ENDIF

      BACKTR=INPUT2
      IRTRN1=2
      IF(BACKTR.LE.0)THEN
         BACKTR=-BACKTR
         MLYRS=NLASSF
         GO TO 611
      ELSE
         MLYRS=1
         GO TO 621
      ENDIF
 34   CONTINUE

C-----------------------------------------------------------------------
C  backtrack to some element in the chain
C-----------------------------------------------------------------------

      LVL_COMPAR=LVLCHN(ENDHI,ICHAIN)
      LVL_COMPAR_A=LVL_COMPAR
      IF(LVL_COMPAR_A.LT.0)LVL_COMPAR_A=-LVL_COMPAR_A
      LVL_RESTOR=LVL_COMPAR
      LEN_RESTOR=LENCHN(ICHAIN)

      IF(BACKTR.GE.LENCHN(ICHAIN))BACKTR=LENCHN(ICHAIN)-1
      STOPBK=BACKTR
c      IF(BACKTR.GT.LENCHN(ICHAIN))BACKTR=LENCHN(ICHAIN)
c      STOPBK=BACKTR-1

      ADTSF=LMICHN(ICHAIN+CMIN)
 36   CONTINUE
      IF(STOPBK.GT.0)THEN
         STOPBK=STOPBK-1
         ADTSF=ISTFBD(ADTSF+CBAK)
         IF(ADTSF.EQ.0)THEN
            SFCERR=23
            GO TO 23
         ENDIF
         GO TO 36
      ENDIF

      STOPBK=BACKTR
      IRTRN1=1
      GO TO 811
39    LLYRS=ILYRT

      GO TO 43

C=======================================================================
C  start of new chain
C=======================================================================

41    ICHAIN=ICHAIN+1

C-----------------------------------------------------------------------
C  BASIC INFORMATION TO DEFINE CHAIN
C-----------------------------------------------------------------------

      LENCHN(ICHAIN)=1
      LINCHN(ICHAIN)=ADTSF
      MINCHN(ICHAIN)=ADTSF
      LVLCHN(ENDLO,ICHAIN)=0
      LVLCHN(ENDHI,ICHAIN)=0
      ZR0CHN(ICHAIN)=0
      ZDRCHN(ICHAIN)=0
      HTUCHN(ICHAIN)=ICHAIN
      CLNCHN(ICHAIN)=1
      ISTFOR(ADTSF)=0
      ISTBAK(ADTSF)=0
      INUCHN(ADTSF)=ICHAIN

      ZMESPT(ADTSF)=0
      ZNCHN(ICHAIN)=0
      SFZMS1(ICHAIN)=0
      SFZMS2(ICHAIN)=0

      IF(IHQLER.EQ.0)THEN
         SNGCHN(ICHAIN)=1
      ELSEIF(IHQLER.EQ.1)THEN
         INUCHN(ADASF)=ICHAIN
         DOUBPT(ADTSF)=ADASF
         ZMESPT(ADASF)=0
         SNGCHN(ICHAIN)=0
      ELSEIF(IHQLER.EQ.2)THEN
         DOUBPT(ADTSF)=-2
         SNGCHN(ICHAIN)=0
      ENDIF


C=======================================================================
C  initialization common to new chain and extending chain
C=======================================================================

43    WSHCH=0
      ZNOWX=ZR0CHN(ICHAIN)+ZDRCHN(ICHAIN)*RADSSF(ILYRT)/100/MULST

C-----------------------------------------------------------------------
C  fake data used only in the chain history in DSFHIT
C-----------------------------------------------------------------------

      WNPX=WNFX
      WNPU=WNTU

C-----------------------------------------------------------------------
C  fake data for first successful hit and/or velocity
C-----------------------------------------------------------------------

      RAD_LAST_VEL=RADSSF(LLYRS)-IDIRSF*MULST
      RFX_N(1)=RADSSF(LLYRS)-RAD_LAST_VEL
      RFX_L(1)=RFX_N(1)

C-----------------------------------------------------------------------
C  set phi change and radially out wire number for
C  second pass, searching radial inward
C-----------------------------------------------------------------------

      IF((SFCTYP.EQ.2).AND.(ENDLO.EQ.2))THEN
         RFX_L(2)=IDIRSF*LVLCHN(ENDLO,ICHAIN)
         WNRX =WNFX -IDIRSF*LVLCHN(ENDLO,ICHAIN)
         WSHFZS=0
         WNRXZ=WNRX-WSHFZS
         WNPXZ=WNRXZ

C-----------------------------------------------------------------------
C  set phi change and radially out wire number for
C  first pass; or  second pass, searching radially outward
C-----------------------------------------------------------------------

      ELSE
         RFX_L(2)=0
         WNRX=WNFX
         WSHFZS=0
         WNRXZ=WNRX-WSHFZS
         WNPXZ=WNRXZ
      ENDIF

C-----------------------------------------------------------------------
C  resume common code of fake data
C-----------------------------------------------------------------------

      WNRXL=WNRX
      WNRXH=WNRX
      RFX_L(3)=0
      VEL_PRED=0
      VEL_PRLM=0
      IRELX=MULSTH

C-----------------------------------------------------------------------
C  more initialization
C-----------------------------------------------------------------------

      RAD_1_PREV=RAD_LAST_VEL
      NWIRTU=NWRSSF(ILYRT)
      ILTRY=0
      MISSUM=0
      NNOTIS=0
      VELOCITY_USE=1
      KEEP_CHAIN=1
      GOOD_HIT=1
      NGAPCH=0

      PROC_USE_ZCALC=0
      PROC_NEW_Z_GROUP=0
      Z_TEST_ACTIVE=0
      COUNTER_Z_CHANGE=0
      LN_SINCE_Z_CHANGE=0
      VELU_SINCE_Z_CH=0
      VELU_PREV_Z_CH=0

      WID_AD_PREV=WIDAD2(SFCTYP)
      WIDHIS(WH_W_LEN)=1
      DO 49 I=2,8
49    WIDHIS(I)=0

      ISTORV=0
      VLLOC1=VSTORF
      DFAHLD=0
      DO 51 IUTEST=1,NUTSTD
51    IUCNTR(IUTEST)=LOUTOF(IUTEST,SFCTYP)
      DO 52 I=1,16
52    IUSAV(I)=1

      DO 57 I=1,VSTORF
         RFX_S(1,I)=IDIRSF*MULST

C-----------------------------------------------------------------------
C  store fake velocity for extend
C  do this for backward direction only, for now
C  maybe later extend for both directions
C-----------------------------------------------------------------------

         IF((SFCTYP.EQ.2).AND.(ENDLO.EQ.2))THEN
            RFX_S(2,I)=IDIRSF*LVLCHN(ENDLO,ICHAIN)

C-----------------------------------------------------------------------
C  store fake velocity for primary chains
C-----------------------------------------------------------------------
         ELSE
            RFX_S(2,I)=0
         ENDIF
         
         RFX_S(3,I)=0
         VELADD(I)=-1
 57   CONTINUE
      
      DO 59 I=1,2
      DO 59 IV=1,3
59    RFX_SS(IV,I)=VSTORH*RFX_S(IV,1)

C================================================================
C  recent history of success; come here if hits found or not
C================================================================

 61   IF(GOOD_HIT.EQ.1)THEN
         IFCOMP=NWIRTU*MULSTH
         IFADDR=NWIRTU*MULST
      ENDIF

      ILTRY=ILTRY+1
      IULOC=IBITS((ILTRY-1),0,4)+1

      DO 83 IUTEST=1,NUTSTD
         IULOCR=IULOC-LOUTOF(IUTEST,SFCTYP)
         IF(IULOCR.LT.1)IULOCR=IULOCR+16
         IUCNTR(IUTEST)=IUCNTR(IUTEST)-IUSAV(IULOCR)
         IF(GOOD_HIT.EQ.1)IUCNTR(IUTEST)=IUCNTR(IUTEST)+1
 83   CONTINUE

      IF(GOOD_HIT.EQ.1)THEN
         IUSAV(IULOC)=1
      ELSE
         IUSAV(IULOC)=0
      ENDIF

C-----------------------------------------------------------------------
C  test that the chain may be continued
C  note: this can end chain, do not end while extending established chain 
C-----------------------------------------------------------------------
      NONSTRAGLER=1
      IF(PROC_BACKTRACK.EQ.0)THEN

         DO 87 IUTEST=1,NUTSTP
            IF(IUCNTR(IUTEST).GE.LTOPAS(IUTEST,SFCTYP))GO TO 89
 87      CONTINUE
         GO TO 575
 89      CONTINUE

C-----------------------------------------------------------------------
C  test that the chain does not start with stragler, can kill chain,
C  then test if current found hit is a stragler
C-----------------------------------------------------------------------

         IF((JUTSTC.GT.0).AND.(JUTSTC.LE.NUTSTD))THEN
            IF(ILTRY.EQ.LOUTOF(JUTSTC,SFCTYP))THEN
               IF(IUCNTR(JUTSTC).LT.LTOPAS(JUTSTC,SFCTYP))THEN
                  KEEP_CHAIN=0
                  GO TO 575
               ENDIF
            ENDIF
            
            IF(GOOD_HIT.EQ.1)THEN
               IF(IUCNTR(JUTSTC).LT.LTOPAS(JUTSTC,SFCTYP))
     2              NONSTRAGLER=0
            ENDIF
         ENDIF

C-----------------------------------------------------------------------
C  test for a stragler in phi
C-----------------------------------------------------------------------

         IF(GOOD_HIT.EQ.1)THEN
            IF(NONSTRAGLER.EQ.1)THEN
               GAPFIL=WNFXL- WNRXH
               IF(GAPFIL.GT. IFCOMP)GAPFIL=GAPFIL-IFADDR
               IF(GAPFIL.LT.-IFCOMP)GAPFIL=GAPFIL+IFADDR
               IF(GAPFIL.GT. GAPFIM(SFCTYP))NONSTRAGLER=0
            ENDIF

            IF(NONSTRAGLER.EQ.1)THEN
               GAPFIH=WNFXH- WNRXL
               IF(GAPFIH.GT. IFCOMP)GAPFIH=GAPFIH-IFADDR
               IF(GAPFIH.LT.-IFCOMP)GAPFIH=GAPFIH+IFADDR
               IF(GAPFIH.LT.-GAPFIM(SFCTYP))NONSTRAGLER=0
            ENDIF
         ENDIF
      ENDIF
        
      IF((GOOD_HIT.EQ.1).AND.(NONSTRAGLER.EQ.1))THEN
         LN_GOOD_STR=LENCHN(ICHAIN)
      ENDIF

C-----------------------------------------------------------------------
C  record the good hit
C-----------------------------------------------------------------------
      IF(GOOD_HIT.EQ.1)THEN

#if defined(CLEO_SFDIAG)
         CALL DSFHIT(ADTSF,3,1,
     1        SFCTYP,ILYRV, ILYRT,
     2        WNRX,  WSHSM, WNPX,  WNPU,
     3        VEL_PRLM,VEL_PRED,WIDHIS,
     4        DIFTNV, ZNOWX, WSHFZS,WSHFZC,
     5        WID_AD_PREV,WID_AD_LAYR,WID_AD_STER,WID_AD_SCAL,
     6        NHI,NLO,   
     7        WNTU,  WNFX,  IHQLER,
     8        VELOCITY_USE, PROC_USE_ZCALC, STOPBK, NONSTRAGLER)
         NEW_VEL_OR_Z=0
#endif

C=======================================================================
C  velocity book-keeping and preliminary calculation
C=======================================================================
C-----------------------------------------------------------------------
C  VELOCITY_USE is set =1 for seed hit and then only in the section to 
C  process the HIT-QUALITY-OR-ERROR information 
C-----------------------------------------------------------------------

         IF(VELOCITY_USE.NE.0)THEN
            IF((PROC_USE_ZCALC.NE.1).OR.(PROC_BACKTRACK.EQ.1))THEN
               ISTORV=ISTORV+1
               VLLOC2=VLLOC1
               VLLOC1=VLLOC1+1
               IF(VLLOC1.GT.VSTORF)VLLOC1=VLLOC1-VSTORF
               VLLOC3=VLLOC1-VSTORH
               IF(VLLOC3.LT.1)VLLOC3=VLLOC3+VSTORF
               RFX_N(2)=WNFX-WNRX
               IF(RFX_N(2).GT. IFCOMP)RFX_N(2)=RFX_N(2)-IFADDR
               IF(RFX_N(2).LE.-IFCOMP)RFX_N(2)=RFX_N(2)+IFADDR
               RFX_N(2)=RFX_N(2)*RPWSSF(ILYRT)/MULST
               RFX_N(3)=IRELX-MULSTH
               DO 163 IV=1,3
C     RFX_ST=RFX_N(IV)   USE THIS STATEMENT FOR VELOCITY TO SINGLE LAST HIT
                  RFX_ST=(RFX_N(IV)+RFX_L(IV))/2
                  RFX_SS(IV,1)=
     2                 RFX_SS(IV,1)+RFX_ST          -RFX_S(IV,VLLOC3)
                  RFX_SS(IV,2)=
     2                 RFX_SS(IV,2)+RFX_S(IV,VLLOC3)-RFX_S(IV,VLLOC1)
                  RFX_L(IV)=RFX_N(IV)
                  RFX_S(IV,VLLOC1)=RFX_ST
                  VELADD(VLLOC1)=ADTSF
                  RFX_S(IV+3,VLLOC1)=RFX_N(IV)
 163           CONTINUE
               DFAHLD=DFAHLD-VSTORH*RFX_S(2,VLLOC3)+RFX_SS(2,1)
               PHICOR=VSTORH*DFAHLD/VSTORS-RFX_SS(2,1)
               DO 165 IV=1,2
                  IRTEMP=RFX_SS(1,IV)/2
                  IFTEMP=RFX_SS(2,IV)*MULSTH
                  IVTEMP(IV)=IFTEMP/IRTEMP
 165           CONTINUE
               VEL_PRLM_OLD=VEL_PRLM
               VEL_PRED_OLD=VEL_PRED
               VEL_PRED_OLDA=VEL_PRED_OLD
               IF(VEL_PRED_OLDA.LT.0)VEL_PRED_OLDA=-VEL_PRED_OLDA

#if defined(CLEO_SFDIAG)
               NEW_VEL_OR_Z=1
#endif

C-----------------------------------------------------------------------
C  preliminary velocity calculation for diagnostic purposes
C-----------------------------------------------------------------------
               VEL_PRLM=(EXTPV1*IVTEMP(1)+EXTPV2*IVTEMP(2)+2)/EXTPDM
               VEL_PRED=(EXTPRL*VEL_PRLM+EXTOLD*VEL_PRLM_OLD)/EXTODM

C-----------------------------------------------------------------------
C  load the velocity for the beginning of the chain
C  but not for extend
C-----------------------------------------------------------------------

               IF(SFCTYP.NE.2)THEN
                  IF(ISTORV.EQ.4)LVLCHN(ENDLO,ICHAIN)=IVTEMP(1)
               ENDIF

C================================================================
C  Z CALCULATION,  here if PROC_USE_ZCALC.EQ.1
C================================================================

            ELSE
               IF(ZNCHN(ICHAIN).LT.MSFZM)THEN

                  WDIFF=WNFX-WNPXZ
                  IF(WDIFF.LT.-NWIRTX/2)WDIFF=WDIFF+NWIRTX
                  IF(WDIFF.GE. NWIRTX/2)WDIFF=WDIFF-NWIRTX
                  ZNCHN(ICHAIN)=ZNCHN(ICHAIN)+1
                  SFZML(ZNCHN(ICHAIN),ICHAIN)=ILYRT
                  SFZMR(ZNCHN(ICHAIN),ICHAIN)=RADSSF(ILYRT)
                  SFZMZ(ZNCHN(ICHAIN),ICHAIN)=((WDIFF)*MULST) /DIFTNV
                  ZTEMP=SFZMZ(ZNCHN(ICHAIN),ICHAIN)
                  SFZMD(ZNCHN(ICHAIN),ICHAIN)=DIFTNV
                  PTZMES(ZNCHN(ICHAIN),ICHAIN)=ADTSF
                  ZMESPT(ADTSF)=ZNCHN(ICHAIN)
                 
                  ZCALCW=DIFTNV*DIFTNV/MULST
                  SFZMS1(ICHAIN)=SFZMS1(ICHAIN) + ZCALCW
                  SFZMS2(ICHAIN)=SFZMS2(ICHAIN) +(ZCALCW*
     2                           SFZMZ(ZNCHN(ICHAIN),ICHAIN)*100)
     3                          /SFZMR(ZNCHN(ICHAIN),ICHAIN)
 
                  ZR0CHN(ICHAIN)=0
                  ZDRCHN(ICHAIN)=(SFZMS2(ICHAIN)*MULST)/SFZMS1(ICHAIN)
                  ZNEWX=ZR0CHN(ICHAIN)+ZDRCHN(ICHAIN)*RADSSF(ILYRT)
     2                 /100/MULST
                  ZNEWXA=ZNEWX
                  IF(ZNEWXA.LT.0)ZNEWXA=-ZNEWXA
                  
#if defined(CLEO_SFDIAG)
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C                  PRINT 9901,ZNCHN(ICHAIN),ILYRT,
C     2                 WNPX,WNFX,WNPXZ,WDIFF,DIFTNV,     
C     3                 SFZMR(ZNCHN(ICHAIN),ICHAIN),
C     4                 SFZMZ(ZNCHN(ICHAIN),ICHAIN),
C     5                 ZDRCHN(ICHAIN),
C     6                 ZNEWX,ZNDSSF(ILYRT)
 9901             FORMAT(' SFCHAN:Z calc: n=',I3,' ilyrt=',I4,
     2            ' WNPX=',I6,
     2            ' WNFX=',I6,' WNPXZ=',I6,' WDIFF=',I5,' DIFTNV=',I5,
     3            ' r=',I6,' z=',I5,' dz=',I5,' Z=',I5,' END=',I5)
#endif
               ENDIF         
               PROC_USE_ZCALC=0      

#if defined(CLEO_SFDIAG)
               NEW_VEL_OR_Z=2
#endif
            ENDIF


C-----------------------------------------------------------------------
C  save information on "last"  { velocity or Z }  calculation
C-----------------------------------------------------------------------

            RAD_LAST_VEL=RADSSF(ILYRT)
            ILYRV=ILYRT
            NWIRVU=NWIRTU
            WLF0 =WNFX +OFFSSF(ILYRV)

C endof.......IF(VELOCITY_USE.NE.0)THEN
         ENDIF

C-----------------------------------------------------------------------
C  end of  { velocity or Z }  calculation
C  test if velocity is changed out of bounds
C  restor to unextended chain if velocity goes out of bounds
C   but allow to restor to last single hit that was in bounds
C-----------------------------------------------------------------------

         IF((SFCTYP.EQ.2).AND.(ENDLO.EQ.2))THEN
            LVLENDA=IVTEMP(1)
            IF(LVLENDA.LT.0)LVLENDA=-LVLENDA
            IF((LVLENDA-LVL_COMPAR_A).LT.V_CHANGE_LIM(SFCTYP))THEN
               VEL_INBOUNDS=1
            ELSE
               VEL_INBOUNDS=0
            ENDIF
         ELSE
            VEL_INBOUNDS=1
         ENDIF

         IF(VEL_INBOUNDS.EQ.1)THEN
            LN_GOOD_VEL=LENCHN(ICHAIN)
            VEL_GOOD_VEL=IVTEMP(1)
            IF(IHQLER.EQ.0)THEN
               LVL_RESTOR=LVLCHN(ENDHI,ICHAIN)
               LEN_RESTOR=LENCHN(ICHAIN)
            ENDIF
         ELSE
            LN_GOOD_VEL=LEN_RESTOR
            VEL_GOOD_VEL=LVL_RESTOR
         ENDIF

C-----------------------------------------------------------------------
C  require that z change keep chain following track
C  this is executed regardless of whether the z calculation was performed
C         ie, regardless of VELOCITY_USE
C
C  If this is the first hit after the Z change, reset the hit count
C  and update the trial length up to this Z change. 
C
C  If this is the first Z change, or the first after elimimating the
C  Z test, select the trail to certify the chain up to the the current hit 
C
C  Increment the hit count since the last Z change
C  Test if there is a significant hit count after the
C  last Z change. If so, tag the hits preceeding this Z change as "good".
C  MUST come after resetting hit count at a new z group.
C-----------------------------------------------------------------------
         SELECT_TRIAL=0
         WIDHIS(WH_V_Z_STR)=0
         WIDHIS(WH_V_Z_RZT_R)=0
         WIDHIS(WH_V_Z_RZT_Z)=0
         WIDHIS(WH_V_Z_RZT_C)=0

         IF(PROC_NEW_Z_GROUP.EQ.1)THEN
            IF(Z_TEST_ACTIVE.EQ.0)THEN
               SELECT_TRIAL=1
               WIDHIS(WH_V_Z_STR)=1
            ENDIF
            Z_TEST_ACTIVE=1
            COUNTER_Z_CHANGE=COUNTER_Z_CHANGE+1
            LN_GOOD_Z_TRIAL=LN_GOOD_Z_NEXT
            VEL_GOOD_Z_TRIAL=VEL_GOOD_Z_NEXT
            LN_SINCE_Z_CHANGE=0
            VELU_PREV_Z_CH=VELU_SINCE_Z_CH
            VELU_SINCE_Z_CH=0
            PROC_NEW_Z_GROUP=0
         ENDIF
                     
         LN_GOOD_Z_NEXT=LENCHN(ICHAIN)
         VEL_GOOD_Z_NEXT=IVTEMP(1)

         ZNEWX=ZR0CHN(ICHAIN)+ZDRCHN(ICHAIN)*RADSSF(ILYRT) /100/MULST
         ZNEWXA=ZNEWX
         IF(ZNEWXA.LT.0)ZNEWXA=-ZNEWXA

         IF(COUNTER_Z_CHANGE.GE.1)THEN
            LN_SINCE_Z_CHANGE=LN_SINCE_Z_CHANGE+1
            IF(VELOCITY_USE.NE.0)VELU_SINCE_Z_CH=VELU_SINCE_Z_CH+1
         ENDIF

         IF(Z_TEST_ACTIVE.EQ.1)THEN
            IF(
     1           (COUNTER_Z_CHANGE.GE.2).AND.
     2           (LN_SINCE_Z_CHANGE.EQ.ZTST_SEL_VALID(SFCTYP)))THEN
               SELECT_TRIAL=1
               WIDHIS(WH_V_Z_STR)=2
            ENDIF

            IF(ENDLO.EQ.1)THEN

C OUTWARD remove Z test when chain approches outer radius
               IF((MLYRS-ILYRT).GE.ZTST_RMV_OUTER(SFCTYP))THEN
                  WIDHIS(WH_V_Z_RZT_R)=1
               ELSEIF(ZTST_RMV_OUTER(SFCTYP).GE.MLYRS)THEN
                  Z_TEST_ACTIVE=0
                  WIDHIS(WH_V_Z_RZT_R)=2
               ELSE
                  Z_TEST_ACTIVE=0
                  WIDHIS(WH_V_Z_RZT_R)=3
               ENDIF

C OUTWARD remove Z test when chain approaches endplate               
               IF(Z_TEST_ACTIVE.EQ.1)THEN
                  IF((ZNDSSF(ILYRT)-ZNEWXA).GE.ZTST_ESC_ZWIND(SFCTYP)
     2              )THEN
                     WIDHIS(WH_V_Z_RZT_Z)=1
                  ELSE
                     IF(
     1                    (ZNCHN(ICHAIN).GE.ZTST_ESC_MM0(SFCTYP))
     2                    )THEN
                        Z_TEST_ACTIVE=0
                        WIDHIS(WH_V_Z_RZT_Z)=2
                     ELSEIF(  
     3                    (ZNCHN(ICHAIN).GE.ZTST_ESC_MMWL(SFCTYP))
     4                    .AND.
     5                    (LN_SINCE_Z_CHANGE.GE.ZTST_ESC_MLS(SFCTYP))
     8                    )THEN
                        Z_TEST_ACTIVE=0
                        WIDHIS(WH_V_Z_RZT_Z)=3
                     ELSE
                        WIDHIS(WH_V_Z_RZT_Z)=4
                     ENDIF
                  ENDIF
               ENDIF

C OUTWARD remove Z test when chain curls
               IF(Z_TEST_ACTIVE.EQ.1)THEN
                  IF(VEL_PRED_OLDA.LT.ZTST_ESC_CURLV(SFCTYP))THEN
                     WIDHIS(WH_V_Z_RZT_C)=1
                  ELSE
                     IF(LENCHN(ICHAIN).GE.ZTST_ESC_LEN_C(SFCTYP))THEN
                        Z_TEST_ACTIVE=0
                        WIDHIS(WH_V_Z_RZT_C)=2
                     ELSE
                        WIDHIS(WH_V_Z_RZT_C)=3
                     ENDIF
                  ENDIF
               ENDIF

            ELSEIF(ENDLO.EQ.2)THEN

C INWARD remove z test when chain approaches inner radius
               IF(
     1              (ILYRT-1).GE.ZTST_RMV_INNER(SFCTYP)
     2              )THEN
                  WIDHIS(WH_V_Z_RZT_R)=11
               ELSE
                  Z_TEST_ACTIVE=0
                  WIDHIS(WH_V_Z_RZT_R)=12
               ENDIF
               IF(IABS(TANSSF(ILYRT)).LT.MULSTH/2)THEN
                  IF(
     1                 (SELECT_TRIAL.EQ.0)
     2                 .AND.
     3                 (LN_SINCE_Z_CHANGE.EQ.ZTST_SEL_IN_AX(SFCTYP))
     4                 )THEN
                     SELECT_TRIAL=1
                     WIDHIS(WH_V_Z_STR)=3
                  ENDIF

C INWARD remove z test when chain extends far enough into axial
                  IF(Z_TEST_ACTIVE.EQ.1)THEN
                     IF(LN_SINCE_Z_CHANGE.LT.ZTST_RMV_IN_AX(SFCTYP))THEN
                        WIDHIS(WH_V_Z_RZT_R)=13
                     ELSE
                        Z_TEST_ACTIVE=0
                        WIDHIS(WH_V_Z_RZT_R)=14
                     ENDIF
                  ENDIF
               ENDIF
            ENDIF
         ENDIF

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

         IF(Z_TEST_ACTIVE.EQ.0)THEN

C use this line to get back to 
C old use of COUNTER_Z_CHANGE and LN_SINCE_Z_CHANGE
C            COUNTER_Z_CHANGE=0

            LN_GOOD_Z=LENCHN(ICHAIN)
         ELSEIF(SELECT_TRIAL.EQ.1)THEN
            LN_GOOD_Z=LN_GOOD_Z_TRIAL
            VEL_GOOD_Z=VEL_GOOD_Z_TRIAL
         ENDIF



C-----------------------------------------------------------------------
C  record more info on hit after velocity or Z is recaluculated
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
         CALL DSFHIT_MORE(
     1        NEW_VEL_OR_Z,IVTEMP(1),
     2        LN_GOOD_STR,LN_GOOD_VEL,LN_GOOD_Z,
     3        LVL_RESTOR,
     4        ZTEMP,Z_TEST_ACTIVE,COUNTER_Z_CHANGE,LN_SINCE_Z_CHANGE,
     5        VELU_SINCE_Z_CH,VELU_PREV_Z_CH,
     6        ZNEWX,
     7        LENCHN(ICHAIN),WIDHIS)
#endif


C-----------------------------------------------------------------------
C  re-label the current layer as the "last" layer in preparation
C  to move to a new layer
C-----------------------------------------------------------------------
         
         ADLSF=ADTSF
         ILYRL=ILYRT
         NWIRLU=NWIRTU
         WLF0L=WNFXL+OFFSSF(ILYRL)
         WLF0H=WNFXH+OFFSSF(ILYRL)
         NGAPCL=NGAPCH
         NGAPCH=0
         VELOCITY_USE=0

C endof......IF(GOOD_HIT.EQ.1)THEN
      ENDIF

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH based on the chain length
C  this is done when a new hit is added to the chain
C-----------------------------------------------------------------------

      IF(GOOD_HIT.EQ.1)THEN
         IF(LENCHN(ICHAIN).GE.LENCA0(SFCTYP))THEN
            WID_AD_PREV=WIDAD0(SFCTYP)
            WIDHIS(WH_W_LEN)=3
         ELSEIF(LENCHN(ICHAIN).GE.LENCA1(SFCTYP))THEN
            WID_AD_PREV=WIDAD1(SFCTYP)
            WIDHIS(WH_W_LEN)=2
         ELSE
            WIDHIS(WH_W_LEN)=1
         ENDIF
      ENDIF

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH based on gaps in the chain
C  this is done only when a hit is not added
C-----------------------------------------------------------------------

      IF(GOOD_HIT.EQ.1)THEN
         WIDHIS(WH_W_GAP)=0
      ELSE
         NGAPCH=NGAPCH+1
         IF(NGAPCH.GE.GAPWD2(SFCTYP))THEN
            IF(WID_AD_PREV.LT.WIDAD2(SFCTYP))THEN
               WID_AD_PREV=WIDAD2(SFCTYP)
               WIDHIS(WH_W_GAP)=3
            ENDIF
         ELSEIF(NGAPCH.GE.GAPWD1(SFCTYP))THEN
            IF(WID_AD_PREV.LT.WIDAD1(SFCTYP))THEN
               WID_AD_PREV=WIDAD1(SFCTYP)
               WIDHIS(WH_W_GAP)=2
            ENDIF
         ELSE
            WIDHIS(WH_W_GAP)=1
         ENDIF
      ENDIF

C-----------------------------------------------------------------------
C  START OF NEW LAYER
C-----------------------------------------------------------------------

      IF(ILYRT.EQ.MLYRS)GO TO 575
      RAD_2_PREV=RAD_1_PREV
      RAD_1_PREV=RADSSF(ILYRT)
      ILYRT=ILYRT+IDIRSF
      NWIRTU=NWRSSF(ILYRT)
      ZNOWX=ZR0CHN(ICHAIN)+ZDRCHN(ICHAIN)*RADSSF(ILYRT)/100/MULST
      LADR=LADSSF(ILYRT)
      MADR=LADR+NWIRTU-1
      NWIRTX=NWIRTU*MULST
      GOOD_HIT=0
      RFX_N(1)=RADSSF(ILYRT)-RAD_LAST_VEL
      CELL_HEIGHT=RADSSF(ILYRT)-RAD_1_PREV
      DIFTNV=TANSSF(ILYRT)-TANSSF(ILYRV)
      DIFTNVA=DIFTNV
      IF(DIFTNV.LT.0)DIFTNVA=-DIFTNV
      DIFTNL=TANSSF(ILYRT)-TANSSF(ILYRL)
      DIFTNLA=DIFTNL
      IF(DIFTNL.LT.0)DIFTNLA=-DIFTNL
      WID_AD_LAYR=WID_AD_PREV

C-----------------------------------------------------------------------
C  adjust the SEARCH WIDTH based on gaps in the chain
C  this time around, include inactive gaps between lyaers
C  this code allows a wider search when crossing VD/DR interface
C  (add the 1 to GAPWD to account for normal advance to the next layer
C  CELSSF becomes active in chain finding; since RPWSSF contains
C  an artificial factor of CELSSF/(2*pi*100); so if
C         GAPWD1=2 and CELSSF=450, this trips if RFX_N>2.43*(cell width)
C         GAPWD1=4 and CELSSF=450, this trips if RFX_N>3.86*(cell width)
C-----------------------------------------------------------------------

C     IF(RFX_N(1).GE.(1+GAPWD2(SFCTYP))*RPWSSF(ILYRT))THEN
C        IF(WID_AD_LAYR.LT.WIDAD2(SFCTYP))THEN
C           WID_AD_LAYR=WIDAD2(SFCTYP)
C           WIDHIS(WH_W_GAP)=5
C        ENDIF
C     ELSEIF(RFX_N(1).GE.(1+GAPWD1(SFCTYP))*RPWSSF(ILYRT))THEN
C        IF(WID_AD_LAYR.LT.WIDAD1(SFCTYP))THEN
C           WID_AD_LAYR=WIDAD1(SFCTYP)
C           WIDHIS(WH_W_GAP)=4
C        ENDIF
C     ELSE
C     ENDIF

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH when narrow cell layers follow wide
C-----------------------------------------------------------------------

      IF(WID_AD_LAYR.GE.WIDAD2(SFCTYP))THEN
         WIDHIS(WH_W_NAR)=0
      ELSEIF(XXRPWN(SFCTYP)*RPWSSF(ILYRL).GT.
     1       XXRPWL(SFCTYP)*RPWSSF(ILYRT) )THEN
         WIDHIS(WH_W_NAR)=1
      ELSE
         WID_AD_LAYR=WIDAD2(SFCTYP)
         WIDHIS(WH_W_NAR)=2
      ENDIF

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH for a change in the stereo angle
C    add WID_AD_STER to WID_AD_LAYER after checking for cell center
C-----------------------------------------------------------------------

      IF(DIFTNVA.LT.MULSTH/2)THEN
         WID_AD_STER=0
         WIDHIS(WH_W_ST_CH)=0
      ELSEIF(ISTORV.LT.3)THEN
         WID_AD_STER=0
         SCAL_STEREO_SHIFT=16
         WIDHIS(WH_W_ST_CH)=1
      ELSE

         FND_ST_THRS=THRS_USE_PNT(ZNCHN(ICHAIN),SFCTYP)
         SCAL_STEREO_SHIFT=SCAL_ST_SHFT(FND_ST_THRS,SFCTYP)
         WID_AD_STER=2*(DIFTNVA+MULSTH)*
     2               WID_AD_ST(FND_ST_THRS,SFCTYP)/16/MULST
         WIDHIS(WH_W_ST_CH)=2+  FND_ST_THRS

         IF(WID_AD_STER.GT.WID_AD_ST_LM(FND_ST_THRS,SFCTYP))THEN
            WID_AD_STER=WID_AD_ST_LM(FND_ST_THRS,SFCTYP)
            WIDHIS(WH_W_ST_CH)=2
         ENDIF

      ENDIF
      
C-----------------------------------------------------------------------
C  set up for "z" calculation at change in stereo angle
C-----------------------------------------------------------------------

      IF(DIFTNLA.GE.MULSTH)THEN
         PROC_NEW_Z_GROUP=1
      ELSE
         PROC_NEW_Z_GROUP=0
      ENDIF

      IF(DIFTNVA.GE.MULSTH)THEN
         PROC_USE_ZCALC=1
      ELSE
         PROC_USE_ZCALC=0
      ENDIF

C-----------------------------------------------------------------------
C  calculate the VELOCITY into the current layer
C   and the width of the layer crosses by the track
C-----------------------------------------------------------------------

      EXTUV1=  2*RFX_SS(1,1)+RFX_SS(1,2)+RFX_N(1)
      EXTUV2= -(RFX_SS(1,1)+RFX_N(1))
      EXTUDM= RFX_SS(1,1)+RFX_SS(1,2)
      VEL_PRLM=(EXTUV1*IVTEMP(1)+EXTUV2*IVTEMP(2)+EXTUDM/2)/EXTUDM
      VEL_PRED=(EXTPRL*VEL_PRLM+EXTOLD*VEL_PRLM_OLD)/EXTODM
      CROSS_WIDTH=(VEL_PRED*CELL_HEIGHT)/RPWSSF(ILYRT)
      IF(CROSS_WIDTH.LT.0)CROSS_WIDTH=-CROSS_WIDTH

C-----------------------------------------------------------------------
C  predict the wire number if the track moved radially; velocity=0
C   WNRXZ used to calculate WNRX and WNPXZ
C   WNRX  used to calculate WNPX and in velocity calculation
C   WNRXL used for identifying straglers 
C   WNRXH used for identifying straglers 
C-----------------------------------------------------------------------

      WSHFZF=ZNOWX*DIFTNV/MULST
      WSHFZS=ZNOWX*DIFTNV*SCAL_STEREO_SHIFT/16/MULST
      WSHFZC=WSHFZF-WSHFZS

      WNRXZ =WLF0  *NWIRTU/NWIRVU-OFFSSF(ILYRT)
      IF(WNRXZ .LT.     0)WNRXZ =WNRXZ +NWIRTX
      IF(WNRXZ .GE.NWIRTX)WNRXZ =WNRXZ -NWIRTX
      WNRX  =WNRXZ                              + WSHFZS
      IF(WNRX  .LT.     0)WNRX  =WNRX  +NWIRTX
      IF(WNRX  .GE.NWIRTX)WNRX  =WNRX  -NWIRTX

      WNRXL =WLF0L *NWIRTU/NWIRLU-OFFSSF(ILYRT) + WSHFZS
      IF(WNRXL .LT.     0)WNRXL =WNRXL +NWIRTX
      IF(WNRXL .GE.NWIRTX)WNRXL =WNRXL -NWIRTX
      WNRXH =WLF0H *NWIRTU/NWIRLU-OFFSSF(ILYRT) + WSHFZS
      IF(WNRXH .LT.     0)WNRXH =WNRXH +NWIRTX
      IF(WNRXH .GE.NWIRTX)WNRXH =WNRXH -NWIRTX

C-----------------------------------------------------------------------
C  predict the wire number; including shift from velocity
C  "new" works with standard-cell-phi-differences; 
C  "old" statement was... WNPX=WNRX+(VEL_PRED*RFX_N(1))/MULST
C   WNPX used to calculate WNPU, the predicted wire number in user units
C   WNRXZ used in "z" calculation
C  predict the wire number; including velocity and FULL stereo shift
C-----------------------------------------------------------------------

      WSHFV=(VEL_PRED*RFX_N(1))/RPWSSF(ILYRT)
      WNPX=WNRX+WSHFV
      WNPXZ=WNRXZ+WSHFV

C-----------------------------------------------------------------------
C  smoothing correction to WNPX: backtrack half of the last phi change
C  but also; phi shift with VEL_PRED for half of the last radius change
C  exception: do not do smoothing after finding a hit after a large gap
C  a large gap can be 2 missing layers, or 1 missing layer and
C  a stereo layer, or the VD/DR interface
C  also apply smoothing to WNPXZ
C-----------------------------------------------------------------------
      WSHSM=0
      IF(IDIRSF*RFX_L(1).GE.(1+GAPWD1(SFCTYP))*RPWSSF(ILYRT))THEN
         WIDHIS(WH_S_SMO)=1
      ELSEIF(
     1        (LN_SINCE_Z_CHANGE.GE.1).AND.
     1        (LN_SINCE_Z_CHANGE.LE.2))THEN
         WIDHIS(WH_S_SMO)=2
      ELSE
         WIDHIS(WH_S_SMO)=3
         WSHSM=(-RFX_L(2)*MULST + RFX_L(1)*VEL_PRED)/2/RPWSSF(ILYRT)
     2        *(RAD_1_PREV-RAD_2_PREV)/RFX_L(1)
         WNPX=WNPX+WSHSM
         WNPXZ=WNPXZ+WSHSM
      ENDIF

C-----------------------------------------------------------------------
C  final prediction of wire number, with modulus
C-----------------------------------------------------------------------
      IF(WNPX.LT.0)WNPX=WNPX+NWIRTX
      IF(WNPX.GE.NWIRTX)WNPX=WNPX-NWIRTX

      IF(WNPXZ .LT.     0)WNPXZ =WNPXZ +NWIRTX
      IF(WNPXZ .GE.NWIRTX)WNPXZ =WNPXZ -NWIRTX

      WNPXA=WNPX+MULSTH
      IF(WNPXA.LT.0)WNPX=WNPXA+NWIRTX
      IF(WNPXA.GE.NWIRTX)WNPXA=WNPXA-NWIRTX
      WNPU=WNPXA/MULST
      IRELX=WNPXA-WNPU*MULST

C-----------------------------------------------------------------------
C  skip to the procedure for predetermined hits if applicable
C-----------------------------------------------------------------------

      IF(STOPBK.GE.0)THEN
         IRTRN1=2
         GO TO 813
      ENDIF

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH when predicted is too close to cell center
C-----------------------------------------------------------------------

      IF(WID_AD_LAYR.GE.1)THEN
         WIDHIS(WH_W_CNTR)=0
      ELSEIF((IRELX.LT.CCTRLO(SFCTYP)).OR.(IRELX.GT.CCTRHI(SFCTYP)))THEN
         WIDHIS(WH_W_CNTR)=1
      ELSE
         WID_AD_LAYR=1
         WIDHIS(WH_W_CNTR)=2
      ENDIF

C-----------------------------------------------------------------------
C finally add the stereo part the width
C-----------------------------------------------------------------------

         WID_AD_LAYR=WID_AD_LAYR+WID_AD_STER

C-----------------------------------------------------------------------
C  separate the search width into "hi" side and "lo" side parts
C-----------------------------------------------------------------------

      IF(IRELX.GE.MULSTH)THEN
         NHI=1+WID_AD_LAYR/2
         NLO=-(WID_AD_LAYR+1)/2
         IDIRWI=1
      ELSE
         NLO=-(1+WID_AD_LAYR/2)
         NHI=(WID_AD_LAYR+1)/2
         IDIRWI=-1
      ENDIF
      IDIRW=IDIRWI

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH to include the full stereo shift
C  note: CCTRLO and CCTRHI already are increased by MULSTH
C-----------------------------------------------------------------------

      IF(SCAL_STEREO_SHIFT.LT.16)THEN
         IF(WSHFZC.GT. 0)THEN
            WID_AD_SCAL=(WSHFZC+MULSTH+(CCTRHI(SFCTYP)-MULSTH))/MULST
            NHI=NHI+WID_AD_SCAL
            WIDHIS(WH_W_ST_SH)=2
         ELSEIF(WSHFZC.LT. 0)THEN
            WID_AD_SCAL=(WSHFZC-MULSTH+(CCTRLO(SFCTYP)-MULSTH))/MULST
            NLO=NLO+WID_AD_SCAL
            WIDHIS(WH_W_ST_SH)=3
         ELSE
            WIDHIS(WH_W_ST_SH)=1
         ENDIF
      ELSE
         WIDHIS(WH_W_ST_SH)=0
      ENDIF

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH when velocity is large
C-----------------------------------------------------------------------

      IF(WSHFV.GE. WSVADD(SFCTYP))THEN
         NHI=NHI+1
         WIDHIS(WH_W_VEL)=1
      ELSEIF(WSHFV.LE.-WSVADD(SFCTYP))THEN
         NLO=NLO-1
         WIDHIS(WH_W_VEL)=2
      ELSE
         WIDHIS(WH_W_VEL)=0
      ENDIF

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH after ignoring doublet in velocity calculation
C-----------------------------------------------------------------------

      IF    (WSHCH.EQ.0)THEN
         WIDHIS(WH_W_IG_DOUB)=0
      ELSEIF(WSHCH.LE.-MULSTH)THEN
         NHI=NHI+1
         WIDHIS(WH_W_IG_DOUB)=2
      ELSEIF(WSHCH.GE. MULSTH)THEN
         NLO=NLO-1
         WIDHIS(WH_W_IG_DOUB)=3
      ELSE
         WIDHIS(WH_W_IG_DOUB)=1
      ENDIF

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH based of PHICOR
C-----------------------------------------------------------------------

      IF    ((PHICOR.GT. MULSTH).AND.(NHI.LT. 1))THEN
         NHI=NHI+1
         WIDHIS(WH_W_PHICOR)=1
      ELSEIF((PHICOR.LT.-MULSTH).AND.(NLO.GT.-1))THEN
         NLO=NLO-1
         WIDHIS(WH_W_PHICOR)=2
      ELSE
         WIDHIS(WH_W_PHICOR)=0
      ENDIF

C-----------------------------------------------------------------------
C adjust search width to include "radially out" if velocity storage
C  is not saturated to remove problems with decreasing velocity chains
C  at the start-up
C-----------------------------------------------------------------------
      IF(ISTORV.LT.VSTORF)THEN
            N_RADL=(WNRX-WNPX)/MULST
            IF(N_RADL.LT.-NWIRTU/2)N_RADL=N_RADL+NWIRTU
            IF(N_RADL.GE. NWIRTU/2)N_RADL=N_RADL-NWIRTU
         IF(N_RADL.GT.NHI)THEN
            NHI=N_RADL
            WIDHIS(WH_W_RAD_OUT)=2
         ELSEIF(N_RADL.LT.NLO)THEN
            NLO=N_RADL
            WIDHIS(WH_W_RAD_OUT)=3
         ELSE
            WIDHIS(WH_W_RAD_OUT)=1
         ENDIF
      ELSE
         WIDHIS(WH_W_RAD_OUT)=0
      ENDIF

C-----------------------------------------------------------------------
C  test wires around the predicted wire
C-----------------------------------------------------------------------

      NUMR=1
      MISLO=0
      MISHI=0
      IREL=0
      NTEST=0
      CONT_SEARCH_LAYER=1
      WNTU=WNPU

 511  CONTINUE

      IF(NTEST.NE.0)THEN
         IF(CONT_SEARCH_LAYER.EQ.1)THEN
            IRELN=IREL+NUMR*IDIRW
            IF(IRELN.GT.NHI)THEN
               IF(MISLO.NE.0)THEN
                  CONT_SEARCH_LAYER=0
               ELSE
                  MISHI=1
                  NUMR=1
                  IDIRW=-1
                  GO TO 511
               ENDIF
            ELSEIF(IRELN.LT.NLO)THEN
               IF(MISHI.NE.0)THEN
                  CONT_SEARCH_LAYER=0
               ELSE
                  MISLO=1
                  NUMR=1
                  IDIRW=1
                  GO TO 511
               ENDIF
            ENDIF
         ENDIF

         IF(CONT_SEARCH_LAYER.EQ.1)THEN
            IF((MISLO.EQ.0).AND.(MISHI.EQ.0))THEN
               NUMR=NUMR+1
               IDIRW=-IDIRW
            ENDIF
            
            IREL=IRELN
            WNTU=WNPU+IREL
            IF(WNTU.LT.0)WNTU=WNTU+NWIRTU
            IF(WNTU.GE.NWIRTU)WNTU=WNTU-NWIRTU
         ENDIF
      ENDIF

      IF(CONT_SEARCH_LAYER.NE.1)THEN
         GO TO 571
      ENDIF

      NTEST=NTEST+1
      ADTSSF=LADR+WNTU

      IRTRN1=2
      GO TO 711
 529  CONTINUE

C-----------------------------------------------------------------------
C  process the HIT-QUALITY-OR-ERROR information 
C-----------------------------------------------------------------------

 533  CONTINUE
      IF    (IHQLER.EQ.-3)THEN
         IF(AFT_USED_OK(SFCTYP).EQ.0)THEN
            CONT_SEARCH_LAYER=0
         ENDIF

      ELSEIF(IHQLER.EQ.0)THEN
         NNOTIS=0
         IF(USE_AJUSOK.EQ.0)THEN
            VELOCITY_USE=2
         ENDIF
            
      ELSEIF(IHQLER.EQ.1)THEN
         NNOTIS=NNOTIS+1
         IF    (PROC_BACKTRACK.EQ.1)THEN
            VELOCITY_USE=3
         ELSEIF(OKWSTR.GT.2)THEN
            VELOCITY_USE=4
         ELSEIF(NNOTIS.LE.MDOUBH(SFCTYP))THEN
            VELOCITY_USE=5
         ENDIF

      ELSEIF(IHQLER.EQ.2)THEN
         IF(  (AFT_N_ISO_S_OK(SFCTYP).EQ.0).AND.
     2        (PROC_NEW_Z_GROUP.EQ.1))THEN
            CONT_SEARCH_LAYER=0
         ENDIF
         IF(NNOTIS.GE.MNOTIS(SFCTYP))THEN
            IHQLER=-2
            GO TO 533
         ENDIF
         NNOTIS=NNOTIS+1
      ENDIF

      IF(IHQLER.LT.0)THEN
         
#if defined(CLEO_SFDIAG)


         CALL DSFHTF(ADTSF,
     1        SFCTYP,ILYRV, ILYRT,
     2        WNRX,  WSHSM, WNPX,  WNPU,
     3        VEL_PRLM,VEL_PRED,WIDHIS,
     4        DIFTNV, ZNOWX, WSHFZS,WSHFZC,
     5        WID_AD_PREV,WID_AD_LAYR,WID_AD_STER,WID_AD_SCAL,
     6        NHI,NLO,   
     7        WNTU,         IHQLER)
#endif

         GO TO 511
      ENDIF


C-----------------------------------------------------------------------
C  BASIC INFORMATION TO DEFINE CHAIN
C-----------------------------------------------------------------------

      IF(PROC_BACKTRACK.EQ.0)THEN

         LENCHN(ICHAIN)=LENCHN(ICHAIN)+1
         LMICHN(ICHAIN+CMIN)=ADTSF
         ISTFBD(ADLSF+CFOR)=ADTSF
         ISTFBD(ADTSF+CBAK)=ADLSF
         INUCHN(ADTSF)=ICHAIN
         ZMESPT(ADTSF)=0

         IF(IHQLER.EQ.0)THEN
            SNGCHN(ICHAIN)=SNGCHN(ICHAIN)+1
         ELSEIF(IHQLER.EQ.1)THEN
            INUCHN(ADASF)=ICHAIN
            DOUBPT(ADTSF)=ADASF
            ZMESPT(ADASF)=0
         ELSEIF(IHQLER.EQ.2)THEN
            DOUBPT(ADTSF)=-2
         ENDIF

      ENDIF

C-----------------------------------------------------------------------
C  record the miss distance from the predicted for diagnostics
C-----------------------------------------------------------------------

      MISNOW=WNFX-WNPX
      IF(MISNOW.GT. NWIRTX/2)MISNOW=MISNOW-NWIRTX
      IF(MISNOW.LT.-NWIRTX/2)MISNOW=MISNOW+NWIRTX

C-----------------------------------------------------------------------
C  adjust the found wire position if an adjacent hit is implausable
C  but not if this is a previously found reprocessed double hit
C-----------------------------------------------------------------------

      WSHCH=0
      IF((WSHFX.NE.0).AND.(PROC_BACKTRACK.EQ.0))THEN
         IF((MISNOW.GT.NHI*MULST).OR.(MISNOW.LT.NLO*MULST))THEN
            WNFX =WNFX -WSHFX
            MISNOW=MISNOW-WSHFX
            WSHCH=-WSHFX
            INUCHN(ADASF)=0
            DOUBPT(ADTSF)=-1
         ENDIF
      ENDIF

C-----------------------------------------------------------------------
C  (continue with) miss distance from the predicted for diagnostics
C-----------------------------------------------------------------------

      IF(MISNOW.LT.0)MISNOW=-MISNOW
      MISSUM=MISSUM+MISNOW
      GOOD_HIT=1
      GO TO 61

C-----------------------------------------------------------------------
C  end of (test wires around the predicted wire) with failure
C-----------------------------------------------------------------------

571   GO TO 61

C-----------------------------------------------------------------------
C  end of search for new layers in a chain
C-----------------------------------------------------------------------

 575  CONTINUE

      IF(LENCHN(ICHAIN).GT.0)MISSUM=MISSUM/LENCHN(ICHAIN)

      IF(KEEP_CHAIN.EQ.1)THEN
         LN_GOOD=LN_GOOD_STR
         IF(LN_GOOD.LT.MINLCH(SFCTYP))KEEP_CHAIN=0
      ENDIF

C!!! KEEP_CHAIN SHOULD BE CHECKED FOR EACH LENGTH TEST
C!!! AND LN_GOOD SHOULD BE TESTED AGAINST MINLCH(SFCTYP) FOR EACH
C!!  VELOCITIES SHOULD BE KEPT FOR EACH HIT AND GET RID OF ALL THIS 
C      VEL_GOOD STUFF  

      IF((KEEP_CHAIN.EQ.1).AND.
     2   (LN_GOOD_VEL.LT.LN_GOOD))THEN
         LN_GOOD=LN_GOOD_VEL
         IVTEMP(1)=VEL_GOOD_VEL
         IF(LN_GOOD.LT.MINLCH(SFCTYP))KEEP_CHAIN=0
      ENDIF
         
      IF((KEEP_CHAIN.EQ.1).AND.
     2   (Z_TEST_ACTIVE.EQ.1).AND.(LN_GOOD_Z.LT.LN_GOOD))THEN
         LN_GOOD=LN_GOOD_Z
         IVTEMP(1)=VEL_GOOD_Z
         IF(LN_GOOD.LT.MINLCH(SFCTYP))KEEP_CHAIN=0
      ENDIF
         
      IF(KEEP_CHAIN.EQ.1)THEN
         STRBAD=LN_GOOD+1
      ELSE 
         STRBAD=1
      ENDIF

C-----------------------------------------------------------------------
C  eliminate aborted chain start
C  or, cut off the stragler end of chain (therefore, this runs backwards)
C  eliminate the Z hits for eliminated cells
C-----------------------------------------------------------------------

      IF(STRBAD.LE.LENCHN(ICHAIN))THEN
         ADTSF=LMICHN(ICHAIN+CMIN)
         DELETZ=0
      
         DO 593 I=LENCHN(ICHAIN),STRBAD,-1
            ISTFBD(ADTSF+CFOR)=0
#if defined(CLEO_SFDIAG)
            CALL DSFHTP(ADTSF,2)
#endif
            INUCHN(ADTSF)=0

            IF(ZMESPT(ADTSF).NE.0)THEN
               DELETZ=DELETZ+1
               ZMESPT(ADTSF)=0
            ENDIF

            IF(DOUBPT(ADTSF).EQ.0)THEN
               SNGCHN(ICHAIN)=SNGCHN(ICHAIN)-1
            ELSEIF(DOUBPT(ADTSF).GT.0)THEN
               INUCHN(DOUBPT(ADTSF))=0
               DOUBPT(ADTSF)=0
            ENDIF
            
            ADLSF=ADTSF
            ADTSF=ISTFBD(ADTSF+CBAK)
            ISTFBD(ADLSF+CBAK)=0
 593     CONTINUE

         IF(DELETZ.GT.0)THEN
            IF(ZNCHN(ICHAIN).GT.DELETZ)THEN
               NZHIT=0
               DO 595 IZHIT=1,ZNCHN(ICHAIN)
                  ADTMP=PTZMES(IZHIT,ICHAIN)
                  IF(ZMESPT(ADTMP).NE.0)THEN
                     NZHIT=NZHIT+1
                     SFZML(NZHIT,ICHAIN)=SFZML(IZHIT,ICHAIN)
                     SFZMR(NZHIT,ICHAIN)=SFZMR(IZHIT,ICHAIN)
                     SFZMZ(NZHIT,ICHAIN)=SFZMZ(IZHIT,ICHAIN)
                     SFZMD(NZHIT,ICHAIN)=SFZMD(IZHIT,ICHAIN)
                     PTZMES(NZHIT,ICHAIN)=PTZMES(IZHIT,ICHAIN)
                     ZMESPT(ADTMP)=NZHIT
                  ELSE
                     ZCALCW=SFZMD(IZHIT,ICHAIN)
                     ZCALCW=ZCALCW*ZCALCW/MULST
                     SFZMS1(ICHAIN)=SFZMS1(ICHAIN) - ZCALCW
                     SFZMS2(ICHAIN)=SFZMS2(ICHAIN) -(ZCALCW*
     2                              SFZMZ(IZHIT,ICHAIN)*100)
     3                             /SFZMR(IZHIT,ICHAIN)
                  ENDIF
 595           CONTINUE

               ZNCHN(ICHAIN)=NZHIT
               ZR0CHN(ICHAIN)=0
               ZDRCHN(ICHAIN)=(SFZMS2(ICHAIN)*MULST)/SFZMS1(ICHAIN)

            ELSE
               ZNCHN(ICHAIN)=0
               ZR0CHN(ICHAIN)=0
               ZDRCHN(ICHAIN)=0
            ENDIF
         ENDIF

C-----------------------------------------------------------------------
C  fix the end of the remaining chain when cutting off stragler
C-----------------------------------------------------------------------

         IF(KEEP_CHAIN.EQ.1)THEN
            ISTFBD(ADTSF+CFOR)=0
            LENCHN(ICHAIN)=LN_GOOD
            LMICHN(ICHAIN+CMIN)=ADTSF

C-----------------------------------------------------------------------
C  or, kill the chain number when eliminating aborted chain start
C-----------------------------------------------------------------------
         ELSE
            ICHAIN=ICHAIN-1
#if defined(CLEO_SFDIAG)
            CALL DSFDM0
#endif
            GO TO 599
         ENDIF
      ENDIF

C-----------------------------------------------------------------------
C  end of successful chain
C    note: this should be fixed for straglers
C-----------------------------------------------------------------------

      IF((CNTLSF.EQ.5).AND.(SFCTYP.EQ.1))THEN
         LVLCHN(ENDHI,ICHAIN)=(2*IVTEMP(1)+IVTEMP(2))/3
      ELSE
         LVLCHN(ENDHI,ICHAIN)=IVTEMP(1)
      ENDIF
C-----------------------------------------------------------------------
C  display the chain
C-----------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
      CALL DSF_CHVIEW_FINALE(ICHAIN,3)
C      PRINT 9903,ICHAIN,LENCHN(ICHAIN)
 9903 FORMAT(' SFCHAN: finished chain ',I3,' length=',I4)
      CALL DSFDMP
#endif

C-----------------------------------------------------------------------
C  allow abort of event
C-----------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
            CALL DSF_ABORT(ABORT)
            IF(ABORT.EQ.1)GO TO 23
#endif

C-----------------------------------------------------------------------
C  this cancels the hits of the current chain, now being displayed
C-----------------------------------------------------------------------
#if defined(CLEO_SFDIAG)
      CALL DSF_CHVIEW_SIMPLE(ICHAIN,0)
#endif

C-----------------------------------------------------------------------
C  loop back for next chain
C-----------------------------------------------------------------------

599   IF(SFCTYP.EQ.2)GO TO 23
      GO TO 19

C=======================================================================
C PROCEDURE TO SET-DIRECTION-ADDRESSES
C=======================================================================

C NORMAL FORWARD DIRECTION
611   IDIRSF=+1
      CFOR=0
      CLIN=0
      ENDLO=1
      GO TO 631
C BACKWARDS
621   IDIRSF=-1
      CFOR=MHITSF
      CLIN=MCHAIN
      ENDLO=2
C COMMON PART FOR OTHER END
631   CBAK=MHITSF-CFOR
      CMIN=MCHAIN-CLIN
      ENDHI=3-ENDLO
      IF(IRTRN1.EQ.1)GO TO 3
      IF(IRTRN1.EQ.2)GO TO 34
      SFCERR=3
      GO TO 21

C================================================================
C PROCEDURE TO DETERMINE HIT-QUALITY-OR-ERROR
C================================================================

711   ADTSF=MAPSSF(ADTSSF)
      WSHFX=0
      ADASF=0

      sf_data_input_sf_adr=ADTSF
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHT=sf_data_output_hit_adr

      IF(ADRHT.LE.0)THEN
         IHQLER=-5
      ELSEIF(INUCHN(ADTSF).LT.0)THEN
         IHQLER=-4
      ELSEIF(INUCHN(ADTSF).GT.0)THEN
         IHQLER=-3
      ELSEIF(INUCHN(ADTSF).EQ.0)THEN
         IHQLER=0
         USE_AJUSOK=0
         OKWSTR=2+(CROSS_WIDTH+MULSTH)/MULST
         ISIGNP=IDIRWI
         WSTRNG=1
         WSTRM=0
         WSTRP=0
 735     ADPSSF=ADTSSF
         FNDCLR=0
         CONTIG=1
 737     ADPSSF=ADPSSF+ISIGNP
         IF(ISIGNP.EQ.1)THEN
            IF(ADPSSF.GT.MADR)ADPSSF=LADR
         ELSE
            IF(ADPSSF.LT.LADR)ADPSSF=MADR
         ENDIF
         ADPSF=MAPSSF(ADPSSF)
         
      sf_data_input_sf_adr=ADPSF
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHT=sf_data_output_hit_adr

         IF(ADRHT.GT.0)THEN
            IF(INUCHN(ADPSF).EQ.0)THEN
               CELL_CLEAR=0
               IF(CONTIG.NE.1)THEN
                  IHQLER=2
               ELSEIF(WSTRNG.GE.OKWSTR)THEN
                  IHQLER=2
               ELSE
                  WSTRNG=WSTRNG+1
                  IF(ISIGNP.EQ.1)THEN
                     WSTRP=WSTRP+1
                  ELSE
                     WSTRM=WSTRM+1
                  ENDIF
                  IF(ADASF.EQ.0)THEN
                     IF(IHQLER.NE.2)THEN
                        WSHFX=ISIGNP*MULSTH
                        ADASF=ADPSF
                        IHQLER=1
                     ENDIF
                  ENDIF
               ENDIF
            ELSEIF(INUCHN(ADPSF).GT.0)THEN
               IF(AJUSOK(SFCTYP).EQ.0)THEN
                  CELL_CLEAR=0
                  IHQLER=2
               ELSE
                  CELL_CLEAR=1
                  USE_AJUSOK=1
               ENDIF
            ELSEIF(INUCHN(ADPSF).LT.0)THEN
               CELL_CLEAR=1
            ENDIF
         ELSE            
            CELL_CLEAR=1
         ENDIF

C CLEAR CELL FOUND
         IF(IHQLER.NE.2)THEN
            IF(CELL_CLEAR.EQ.1)THEN
               FNDCLR=FNDCLR+1
               CONTIG=0
            ENDIF
            IF(FNDCLR.LT.NEDCLR(SFCTYP))GO TO 737
         ENDIF
         
C ENOUGH CLEAR; LOOP BACK (EVEN FOR IHQLER=2 TO DETERMINE WNFXL,WNFXH) 
         IF(ISIGNP.EQ.IDIRWI)THEN
            ISIGNP=-IDIRWI
            GO TO 735
         ENDIF
         
         IF(IHQLER.EQ.2)THEN
            WSHFX=0
            ADASF=0
         ENDIF
         
         WNFX=WNTU*MULST+WSHFX
         WNFXL=(WNTU-WSTRM)*MULST-MULSTH
         WNFXH=(WNTU+WSTRP)*MULST+MULSTH
      ENDIF

      PROC_BACKTRACK=0
      IF(IRTRN1.EQ.1)GO TO 13
      IF(IRTRN1.EQ.2)GO TO 529
      SFCERR=2
      GO TO 23

C================================================================
C PROCEDURE TO PROCESS-PREDETERMINED-HITS
C================================================================

811   CONTINUE
      ILYRT=LAYSSF(SFLSFA(ADTSF))
            LSFTMP=SFLSFA(ADTSF)
            IF(CDLASF(LSFTMP).LT.0)THEN
               IF(SFSVWE.EQ.2)THEN
                  WNTU=WIRSSF(ADTSF-SFSVCL(-CDLASF(LSFTMP)))   
               ELSE
                  WNTU=WIRSSF(ADTSF)   
               ENDIF
            ELSE
               WNTU=WIRSSF(ADTSF)   
            ENDIF
      ZNOWX=ZR0CHN(ICHAIN)+ZDRCHN(ICHAIN)*RADSSF(ILYRT)/100/MULST
      GO TO 815

813   CONTINUE
      IF(ILYRT.NE.ILYRX)GO TO 571
      ADTSF=IADRX
      WNTU=WNXFU

815   CONTINUE
      IHQLER=0
      USE_AJUSOK=0
      WSHFX=0
      LADR=LADSSF(ILYRT)
      MADR=MADSSF(ILYRT)

      IF(INUCHN(ADTSF).NE.HTUCHN(ICHAIN))THEN
         SFCERR=24
         GO TO 23
      ENDIF

      ADASF=DOUBPT(ADTSF)
      IF(ADASF.GT.0)THEN
         IHQLER=1

         IF(INUCHN(ADASF).NE.HTUCHN(ICHAIN))THEN
            SFCERR=25
            GO TO 23
         ENDIF
         
         ILYRA=LAYSSF(SFLSFA(ADASF))
               LSFTMP=SFLSFA(ADASF)
               IF(CDLASF(LSFTMP).LT.0)THEN
                  IF(SFSVWE.EQ.2)THEN
                     WNAU=WIRSSF(ADASF-SFSVCL(-CDLASF(LSFTMP)))   
                  ELSE
                     WNAU=WIRSSF(ADASF)   
                  ENDIF
               ELSE
                  WNAU=WIRSSF(ADASF)   
               ENDIF
         
         IF(ILYRA.NE.ILYRT)THEN
            SFCERR=27
            GO TO 23
         ENDIF
         
         WSHFX=WNAU-WNTU
         IFCOMP=NWRSSF(ILYRT)/2
         IF(WSHFX.GT. IFCOMP)WSHFX=WSHFX-NWRSSF(ILYRT)
         IF(WSHFX.LE.-IFCOMP)WSHFX=WSHFX+NWRSSF(ILYRT)
         
         IF((WSHFX.NE.-1).AND.(WSHFX.NE. 1))THEN
            SFCERR=28
            GO TO 23
         ENDIF
         
         WSHFX=WSHFX*MULSTH
      ENDIF

C CALCULATE WSTRM AND WSTRP AS ABOVE
      ADTSSF=LADR+WNTU
      IDIRWI=-1
      OKWSTR=10

      ISIGNP=IDIRWI
      WSTRNG=1
      WSTRM=0
      WSTRP=0

 835  ADPSSF=ADTSSF
 837  ADPSSF=ADPSSF+ISIGNP

      IF(ISIGNP.EQ.1)THEN
         IF(ADPSSF.GT.MADR)ADPSSF=LADR
      ELSE
         IF(ADPSSF.LT.LADR)ADPSSF=MADR
      ENDIF

      ADPSF=MAPSSF(ADPSSF)

      sf_data_input_sf_adr=ADPSF
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHT=sf_data_output_hit_adr

      IF(ADRHT.GT.0)THEN
         IF((INUCHN(ADPSF).EQ.HTUCHN(ICHAIN)).OR.
     2      (INUCHN(ADPSF)             .EQ.0))THEN
            IF(WSTRNG.GE.OKWSTR)GO TO 861

            WSTRNG=WSTRNG+1
            
            IF(ISIGNP.EQ.1)THEN
               WSTRP=WSTRP+1
            ELSE
               WSTRM=WSTRM+1
            ENDIF

            GO TO 837
         ENDIF
      ENDIF
         
      IF(ISIGNP.EQ.IDIRWI)THEN
         ISIGNP=-IDIRWI
         GO TO 835
      ENDIF
861   CONTINUE

      IF(IHQLER.NE.1)THEN
C THIS WAS FOUND TO CAUSE ERROR: 10/2/91
C ALLOW STRINGS THAT ARE NOT PREVIOUS IDENTIFIED DOUBLET TO PASS AS IHQLER=0
CCC      IF(WSTRNG.NE.1)IHQLER=2
      ENDIF

      WNFX=WNTU*MULST+WSHFX
      WNFXL=(WNTU-WSTRM)*MULST-MULSTH
      WNFXH=(WNTU+WSTRP)*MULST+MULSTH

      STOPBK=STOPBK-1
      IF(STOPBK.GE.0)THEN
         IADRX=ISTFBD(ADTSF+CFOR)
         
         IF(IADRX.EQ.0)THEN
            SFCERR=29
            GO TO 23
         ENDIF
         
         ILYRX=LAYSSF(SFLSFA(IADRX))
               LSFTMP=SFLSFA(IADRX)
               IF(CDLASF(LSFTMP).LT.0)THEN
                  IF(SFSVWE.EQ.2)THEN
                     WNXFU=WIRSSF(IADRX-SFSVCL(-CDLASF(LSFTMP)))   
                  ELSE
                     WNXFU=WIRSSF(IADRX)
                  ENDIF
               ELSE
                  WNXFU=WIRSSF(IADRX)
               ENDIF
      ENDIF
      PROC_BACKTRACK=1
      
      IF(IRTRN1.EQ.1)GO TO 39
      IF(IRTRN1.EQ.2)GO TO 529

      SFCERR=30
      GO TO 23

      END




