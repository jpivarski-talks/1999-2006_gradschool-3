*
* $Id: sfchan.F,v 1.19 2003/03/05 18:35:58 dpp Exp $
*
* $Log: sfchan.F,v $
* Revision 1.19  2003/03/05 18:35:58  dpp
*      -> re-organize calculation to avoid MULST*NWIR**2, which can overflow
*         ... but restored to original for "no affect on compress" version
*
* Revision 1.18  2002/09/24 00:32:17  dpp
*      -> move remaining sfchan_hit_quality arguments to common
*      -> diagnostics using dsfhit
*
* Revision 1.17  2002/09/05 18:52:05  dpp
*      -> split off sfchan_hit_quality, hit quality and error
*      -> split off sfchan_predetermined, process predetermined hits
*      -> split off sfchan_scan_seed_hits, outer loop over seed hits
*      -> split off sfchan_extend_chain, extend existing chains
*
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

      SUBROUTINE SFCHAN(NEW_CHAIN,SFCERR)

C......................................................................
C.
C. SFCHAN - Create the basic chains from close hits
C.
C. COMMON    : /STEPCn/
C. CALLS     : SFCHAN_QUAL,  DSFHIT,DSFDMP,DSFHTF
C. CALLED    : SFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 20-JULY-88
C.
C......................................................................
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

#include "doit/sfseq/sfchan_ctl.inc"
#include "doit/sfseq/sfchan_hit.inc"

#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

C-----------------------------------------------------------------------
C  VARIABLES WITHIN SF_DATA_ACCESS
C-----------------------------------------------------------------------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

C-----------------------------------------------------------------------
C  ARGUMENT VARIABLE DOCUMENTION
C-----------------------------------------------------------------------

C NEW_CHAIN....INPUT, =.TRUE.  TO CREATE A NEW CHAIN  
      LOGICAL NEW_CHAIN
C SFCERR...RETURNED ERROR, =0 FOR NO ERROR
      INTEGER SFCERR

C-----------------------------------------------------------------------
C  LOCAL VARIABLE DOCUMENTION
C-----------------------------------------------------------------------

C ADTMP....ADDRESS TEMPORARY
C ADLSF....ADDRESS OF THE LAST FOUND WIRE,          SF INDEXING
C ADTCD....ADDRESS OF THE "TESTED" WIRE,            CD INDEXING
C ADACD....ADDRESS OF ADJACENT HIT WIRE, IF ANY,    CD INDEXING
C ADLCD....ADDRESS OF THE LAST FOUND WIRE,          CD INDEXING
C ADASSF...ADDRESS OF ADJACENT HIT WIRE, IF ANY,    SSF INDEXING
C ADLSSF...ADDRESS OF THE LAST FOUND WIRE,          SSF INDEXING
C ADTSV....ADDRESS OF THE "TESTED" WIRE,            SV INDEXING
C DIFTNV...DIFFERENCE IN TANSSF(ILYRT) AND TANSSF(ILYRV)
C          THIS IS RELATIVE TO THE LAST VELOCITY CALCULATING LAYER
C DIFTNVA..(ABS) DIFFERENCE IN TANSSF(ILYRT) AND TANSSF(ILYRV)
C DIFTNL...DIFFERENCE IN TANSSF(ILYRT) AND TANSSF(ILYRL)
C          THIS IS RELATIVE TO THE LAST LAYER, VELOCITY CALCULATING OR NOT
C DIFTNLA..(ABS) DIFFERENCE IN TANSSF(ILYRT) AND TANSSF(ILYRL)
C ZCALCW...WEIGHT FOR Z CALCULATION
C WDIFF....DIFFERENCE IN WIRE NUMBER FOR Z CALCULATION
C DFAHLD...HOLD THE OFFSET OF THE LAST 4 WIRES RELATIVE TO THE 5TH BACK
C GAPFIL...GAP IN PHI (CELLS) TO LOW SIDE OF CURRENT HIT FROM LAST
C GAPFIH...GAP IN PHI (CELLS) TO HIGH SIDE OF CURRENT HIT FROM LAST
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
C ILYRL....LAYER NUMBER, LAST GOOD FOUND HIT
C ILYRV....LAYER NUMBER, LAST GOOD FOUND HIT USED IN VELOCITY
C IUTEST...COUNTER FOR THE SUCCESS TESTS
C IULOC....LOCATION IN THE "USED" ARRAYS FOR THE LATEST DATA
C IULOCR...OUTER RANGE IN THE "USED" ARRAYS
C CONT_CHAIN........FLAG; =1 IF SEACH FOR HITS IS TO BE CONTINUED
C KEEP_CHAIN........FLAG; =1 IF CURRENT CHAIN IS TO BE KEPT
C GOOD_HIT..........FLAG; =1 IF GOOD HIT IS FOUND
C NONSTRAGLER.......FLAG; =1 IF CURRENT HIT, IF IT IS LAST, IS NOT A STRAGLER
C VEL_INBOUNDS......FLAG; =1 IF VELOCITY AFTER CURRENT HIT IS IN BOUNDS
C CONT_SEARCH_LAYER.FLAG; =1 IF CONTINUE TO SEARCH FOR HIT IN THIS LAYER
C SEARCH_DIR........DIRECTION TO MOVE TO LOOK FOR THE NEXT POSSIBLE WIRE
C IREL.....INDEX, RELATIVE POSITION OF THE TESTED WIRE TO THE PREDICTED
C IRELN....NEW (PROBATIONAL) VALUE OF IREL
C I........GENERAL COUNTER
C LSFTMP...LAYER SF, TEMPORARY; FOR CALCULATION OF CD ADDRESS
C LYRCD....LAYER NUMBER IN CDGEOM OF A WIRE IN AN SSF LAYER LLYRS
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
C MADR.....ADDRESS OF LAST WIRE IN PRESENT LAYER,             SSF INDEXING
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
C WID_AD_PREV.......SEARCH WIDTH ADDED, BASED ON CONDITIONS OF PREVIOUS HITS
C WID_AD_LAYR.......SEARCH WIDTH ADDED, BASED ON CONDITIONS OF THIS LAYER
C FND_ST_THRS.......FOUND STEREO THRESHOLD, USED TO SELECT SCALING
C WID_AD_STER.......WIDTH ADDED FOR CHANGE IN STEREO ANGLE
C WID_AD_SCAL.......WIDTH ADDED (TO NHI OF NLO) FOR SCALED STEREO SHIFT
C WIDHIS...WIDTH HISTORY ARRAY
C NEW_VEL_OR_Z......FLAG FOR DIAGNOSTIC, 1:NEW VELOCITY CALCULATED, 2:NEW Z
C WNRX.....WIRE NUMBER  -NOW -RADIALOUT-MULTIPLIED
C WNRXZ....WIRE NUMBER  -NOW -RADIALOUT-MULTIPLIED if in layer w/same TANSSF
C WNRXL....WIRE NUMBER  -NOW -RADIALOUT-MULTIPLIED -LOLIMIT
C WNRXH....WIRE NUMBER  -NOW -RADIALOUT-MULTIPLIED -HILIMIT

      integer wnrxz_old,wnrxl_old,wnrxh_old 

C WNPX.....WIRE NUMBER  -NOW -PREDICTED-MULTIPLIED
C WNPXA....WIRE NUMBER  -NOW -PREDICTED-MULTIPLIED, MULSTH ADDED for rounding
C WNPXZ....WIRE NUMBER  -NOW -PREDICTED-MULTIPLIED if in layer w/same TANSSF
C WNPU.....WIRE NUMBER  -NOW -PREDICTED-USER
C WLF0.....WIRE NUMBER  -LAST-FOUND    -MULTIPLIED AND RELATIVE TO 0
C WLF0L....WIRE NUMBER  -LAST-FOUND    -MULTIPLIED AND RELATIVE TO 0 -LOLIMIT
C WLF0H....WIRE NUMBER  -LAST-FOUND    -MULTIPLIED AND RELATIVE TO 0 -HILIMIT
C WSHFV....SHIFT IN WIRE NUMBER DUE TO VELOCITY, MULTIPLIED
C WSHFZF...RELATIVE SHIFT IN WIRE POSITION DUE TO Z, MULTIPLIED, FULL 
C WSHFZS...RELATIVE SHIFT IN WIRE POSITION DUE TO Z, MULTIPLIED, SCALED
C WSHFZC...RELATIVE SHIFT IN WIRE POSITION DUE TO Z, MULTIPLIED, COMPLEMENT 
C WSHCH....SHIFT IN WIRE POSITION CHANGED FOR IMPLAUSABLE DOUBLET
C WSHSM....SHIFT IN PREDICTED WIRE POSITION DUE TO SMOOTHING
C ZNOWX....Z VALUE IN CURRENT LAYER; ILYRT, IN meters MULTIPLIED
C ZNEWX....NEW Z VALUE IN CURRENT LAYER; ILYRT, IN meters MULTIPLIED
C ZNEWXA...NEW Z VALUE IN CURRENT LAYER; ILYRT, IN meters MULTIPLIED, ABS
C ZTEMP....TEMP VALUE OF Z IN CALULATION
C SCAL_STEREO_SHIFT.SCALING OF STEREO SHIFT ESPECIALLY FOR EARLY SHIFTS

      INTEGER ADTMP
      INTEGER ADLSF
      INTEGER ADTCD, ADACD, ADLCD, ADASSF,ADLSSF
      INTEGER ADTSV
      INTEGER DIFTNV, DIFTNVA,DIFTNL, DIFTNLA,ZCALCW,WDIFF
      INTEGER DFAHLD
      INTEGER GAPFIL,GAPFIH
      INTEGER RAD_LAST_VEL, RAD_1_PREV,RAD_2_PREV,CELL_HEIGHT
      INTEGER IFCOMP,IFADDR,IV
      INTEGER RFX_ST,IRTEMP,IFTEMP
      INTEGER ILYRL, ILYRV
      INTEGER IUTEST,IULOC, IULOCR
      INTEGER CONT_CHAIN,KEEP_CHAIN, GOOD_HIT, NONSTRAGLER, VEL_INBOUNDS
      INTEGER CONT_SEARCH_LAYER
      INTEGER SEARCH_DIR, IREL,  IRELN, I
      INTEGER IRELX
      INTEGER LSFTMP,LYRCD  
      INTEGER LN_GOOD_STR,LN_GOOD_VEL,LN_GOOD,VEL_GOOD_VEL
      INTEGER LN_GOOD_Z,LN_GOOD_Z_TRIAL,LN_GOOD_Z_NEXT
      INTEGER VEL_GOOD_Z,VEL_GOOD_Z_TRIAL,VEL_GOOD_Z_NEXT
      INTEGER Z_TEST_ACTIVE,SELECT_TRIAL
      INTEGER PROC_NEW_Z_GROUP,PROC_USE_ZCALC
      INTEGER COUNTER_Z_CHANGE,LN_SINCE_Z_CHANGE
      INTEGER VELU_SINCE_Z_CH,VELU_PREV_Z_CH
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
      INTEGER WID_AD_PREV,WID_AD_LAYR,WID_AD_STER,WID_AD_SCAL
      INTEGER FND_ST_THRS
      INTEGER WIDHIS(MWIDH)
      INTEGER WNRX,  WNRXZ, WNRXL, WNRXH
      INTEGER WNPX,  WNPXA, WNPXZ, WNPU
      INTEGER WLF0,  WLF0L, WLF0H
      INTEGER WSHFV, WSHFZF,WSHFZS,WSHFZC
      INTEGER WSHSM, WSHCH
      INTEGER ZNOWX, ZNEWX, ZNEWXA, ZTEMP
      INTEGER SCAL_STEREO_SHIFT

C ABORT....ABORT FLAG RETURNED FROM DSF_ABORT, =1 for abort
      INTEGER ABORT
C LVLENDA...........ABS(IVTEMP(1))
      INTEGER LVLENDA

C================================================================
C================================================================
C
C   -----------  Executable code starts here  ---------------
C
C================================================================
C================================================================

      SFCERR=0

C=======================================================================
C  start of new chain
C=======================================================================

      IF(NEW_CHAIN)THEN
        ICHAIN=ICHAIN+1

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
C-----------------------------------------------------------------------
      ENDIF

C=======================================================================
C  initialization common to new chain and extending chain
C=======================================================================

      WSHCH=0
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
      CONT_CHAIN=1
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
        WIDHIS(I)=0
 49   CONTINUE
      ISTORV=0
      VLLOC1=VSTORF
      DFAHLD=0
      DO 51 IUTEST=1,NUTSTD
        IUCNTR(IUTEST)=LOUTOF(IUTEST,SFCTYP)
 51   CONTINUE
      DO 52 I=1,16
        IUSAV(I)=1
 52   CONTINUE
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
        DO 58 IV=1,3
          RFX_SS(IV,I)=VSTORH*RFX_S(IV,1)
 58     CONTINUE
 59   CONTINUE
      

C================================================================
C  recent history of success; come here if hits found or not
C================================================================

 61   CONTINUE

      IF(GOOD_HIT.EQ.1)THEN
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

        CONT_CHAIN=0
        IUTEST=0
 87     IUTEST=IUTEST+1
        IF((IUTEST.LE.NUTSTP).AND.(CONT_CHAIN.EQ.0))THEN
          IF(IUCNTR(IUTEST).GE.LTOPAS(IUTEST,SFCTYP))CONT_CHAIN=1
          GO TO 87
        ENDIF

C-----------------------------------------------------------------------
C  test that the chain does not start with stragler, can kill chain,
C  then test if current found hit is a stragler
C-----------------------------------------------------------------------

        IF(CONT_CHAIN.EQ.1)THEN
          IF((JUTSTC.GT.0).AND.(JUTSTC.LE.NUTSTD))THEN
            IF(ILTRY.EQ.LOUTOF(JUTSTC,SFCTYP))THEN
              IF(IUCNTR(JUTSTC).LT.LTOPAS(JUTSTC,SFCTYP))THEN
                CONT_CHAIN=0
                KEEP_CHAIN=0
              ENDIF
            ENDIF
            
            IF(GOOD_HIT.EQ.1)THEN
              IF(IUCNTR(JUTSTC).LT.LTOPAS(JUTSTC,SFCTYP))
     2             NONSTRAGLER=0
            ENDIF
          ENDIF
C endof.......IF(CONT_CHAIN.EQ.1)THEN
        ENDIF
        
C-----------------------------------------------------------------------
C  test for a stragler in phi
C-----------------------------------------------------------------------

        IF(CONT_CHAIN.EQ.1)THEN
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
C endof.......IF(CONT_CHAIN.EQ.1)THEN
        ENDIF
C endof.......IF(PROC_BACKTRACK.EQ.0)THEN
      ENDIF
      
C-----------------------------------------------------------------------
C  increment the good length ( excluding straglers)
C-----------------------------------------------------------------------

      IF(CONT_CHAIN.EQ.1)THEN
        IF((GOOD_HIT.EQ.1).AND.(NONSTRAGLER.EQ.1))THEN
          LN_GOOD_STR=LENCHN(ICHAIN)
        ENDIF
        
C-----------------------------------------------------------------------
C  record the good hit
C-----------------------------------------------------------------------
        IF(GOOD_HIT.EQ.1)THEN
          
#if defined(CLEO_SFDIAG)
          CALL DSFHIT(ADTSF,3,1,
     1         SFCTYP,ILYRV, ILYRT,
     2         WNRX,  WSHSM, WNPX,  WNPU,
     3         VEL_PRLM,VEL_PRED,WIDHIS,
     4         DIFTNV, ZNOWX, WSHFZS,WSHFZC,
     5         WID_AD_PREV,WID_AD_LAYR,WID_AD_STER,WID_AD_SCAL,
     6         NHI,NLO,OKWSTR,   
     7         WNTU,  WNFX,  IHQLER,
     8         VELOCITY_USE, PROC_USE_ZCALC, STOPBK, NONSTRAGLER)
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
     2               RFX_SS(IV,1)+RFX_ST          -RFX_S(IV,VLLOC3)
                RFX_SS(IV,2)=
     2               RFX_SS(IV,2)+RFX_S(IV,VLLOC3)-RFX_S(IV,VLLOC1)
                RFX_L(IV)=RFX_N(IV)
                RFX_S(IV,VLLOC1)=RFX_ST
                VELADD(VLLOC1)=ADTSF
                RFX_S(IV+3,VLLOC1)=RFX_N(IV)

c                if(iv.eq.2)then
c                  print 9911,ilyrt,RFX_S(IV,VLLOC1),RFX_S(IV+3,VLLOC1)
c 9911             format(' sfchan debug: ilyrt=',I3,' rfx_s=',2I6)
c                endif


 163          CONTINUE
              DFAHLD=DFAHLD-VSTORH*RFX_S(2,VLLOC3)+RFX_SS(2,1)
              PHICOR=VSTORH*DFAHLD/VSTORS-RFX_SS(2,1)
              DO 165 IV=1,2
                IRTEMP=RFX_SS(1,IV)/2
                IFTEMP=RFX_SS(2,IV)*MULSTH
                IVTEMP(IV)=IFTEMP/IRTEMP
 165          CONTINUE
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
     2               SFZMZ(ZNCHN(ICHAIN),ICHAIN)*100)
     3               /SFZMR(ZNCHN(ICHAIN),ICHAIN)
                
                ZR0CHN(ICHAIN)=0
                ZDRCHN(ICHAIN)=(SFZMS2(ICHAIN)*MULST)/SFZMS1(ICHAIN)
                ZNEWX=ZR0CHN(ICHAIN)+ZDRCHN(ICHAIN)*RADSSF(ILYRT)
     2               /100/MULST
                ZNEWXA=ZNEWX
                IF(ZNEWXA.LT.0)ZNEWXA=-ZNEWXA
                
#if defined(CLEO_SFDIAG)
                  PRINT 9901,ZNCHN(ICHAIN),ILYRT,
     2                 WNPX,WNFX,WNPXZ,WDIFF,DIFTNV,     
     3                 SFZMR(ZNCHN(ICHAIN),ICHAIN),
     4                 SFZMZ(ZNCHN(ICHAIN),ICHAIN),
     5                 ZDRCHN(ICHAIN),
     6                 ZNEWX,ZNDSSF(ILYRT)
 9901           FORMAT(' SFCHAN:Z calc: n=',I3,' ilyrt=',I4,
     2               ' WNPX=',I6,' WNFX=',I6,' WNPXZ=',I6,
     3               ' WDIFF=',I5,' DIFTNV=',I5,
     4               ' r=',I6,' z=',I5,' dz=',I5,' Z=',I5,' END=',I5)
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
     2               )THEN
                  WIDHIS(WH_V_Z_RZT_Z)=1
                ELSE
                  IF(
     1                 (ZNCHN(ICHAIN).GE.ZTST_ESC_MM0(SFCTYP))
     2                 )THEN
                    Z_TEST_ACTIVE=0
                    WIDHIS(WH_V_Z_RZT_Z)=2
                  ELSEIF(  
     3                   (ZNCHN(ICHAIN).GE.ZTST_ESC_MMWL(SFCTYP))
     4                   .AND.
     5                   (LN_SINCE_Z_CHANGE.GE.ZTST_ESC_MLS(SFCTYP))
     8                   )THEN
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
     1             (ILYRT-1).GE.ZTST_RMV_INNER(SFCTYP)
     2             )THEN
                WIDHIS(WH_V_Z_RZT_R)=11
              ELSE
                Z_TEST_ACTIVE=0
                WIDHIS(WH_V_Z_RZT_R)=12
              ENDIF
              IF(IABS(TANSSF(ILYRT)).LT.MULSTH/2)THEN
                IF(
     1               (SELECT_TRIAL.EQ.0)
     2               .AND.
     3               (LN_SINCE_Z_CHANGE.EQ.ZTST_SEL_IN_AX(SFCTYP))
     4               )THEN
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
C endof.......IF(Z_TEST_ACTIVE.EQ.1)THEN
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
     1         NEW_VEL_OR_Z,IVTEMP(1),
     2         LN_GOOD_STR,LN_GOOD_VEL,LN_GOOD_Z,
     3         LVL_RESTOR,
     4         ZTEMP,Z_TEST_ACTIVE,COUNTER_Z_CHANGE,LN_SINCE_Z_CHANGE,
     5         VELU_SINCE_Z_CH,VELU_PREV_Z_CH,
     6         ZNEWX,
     7         LENCHN(ICHAIN),WIDHIS)
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
C endof......IF(GOOD_HIT.EQ.1)THEN
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
C endof......IF(GOOD_HIT.EQ.1)THEN
        ENDIF

C-----------------------------------------------------------------------
C  START OF NEW LAYER
C-----------------------------------------------------------------------

        IF(ILYRT.EQ.MLYRS)CONT_CHAIN=0

C endof.......IF(CONT_CHAIN.EQ.1)THEN
      ENDIF

      IF(CONT_CHAIN.EQ.1)THEN
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
     1         XXRPWL(SFCTYP)*RPWSSF(ILYRT) )THEN
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
     2         WID_AD_ST(FND_ST_THRS,SFCTYP)/16/MULST
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
        
        IF(DIFTNVA.GE.MULST)THEN
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
C     calculation uses wire difference to avoid integer overflow
C-----------------------------------------------------------------------

        WSHFZF=ZNOWX*DIFTNV/MULST
        WSHFZS=ZNOWX*DIFTNV*SCAL_STEREO_SHIFT/16/MULST
        WSHFZC=WSHFZF-WSHFZS

        WNRXZ=WLF0+WLF0*(NWIRTU-NWIRVU)/NWIRVU-OFFSSF(ILYRT)

        wnrxz_old=WLF0  *NWIRTU/NWIRVU-OFFSSF(ILYRT)

c        print 9912,ilyrt,
c     2       wlf0,nwirtu,nwirvu,offssf(ilyrt),
c     3       wnrxz,wnrxz_old
c 9912   format(' sfchan_debug: ilyrt=',I3,
c     2       ' wlf0,nwirtu,nwirvu,offssf=',4i8,
c     3       ' wnrxz,wnrxz_old=',2i8)
        wnrxz=wnrxz_old

        IF(WNRXZ .LT.     0)WNRXZ =WNRXZ +NWIRTX
        IF(WNRXZ .GE.NWIRTX)WNRXZ =WNRXZ -NWIRTX

        WNRX=WNRXZ+WSHFZS
        IF(WNRX  .LT.     0)WNRX  =WNRX  +NWIRTX
        IF(WNRX  .GE.NWIRTX)WNRX  =WNRX  -NWIRTX
        
        WNRXL=WLF0L+WLF0L*(NWIRTU-NWIRLU)/NWIRLU-OFFSSF(ILYRT)+WSHFZS
        wnrxl_old=WLF0L *NWIRTU/NWIRLU-OFFSSF(ILYRT) + WSHFZS
        wnrxl=wnrxl_old

        IF(WNRXL .LT.     0)WNRXL =WNRXL +NWIRTX
        IF(WNRXL .GE.NWIRTX)WNRXL =WNRXL -NWIRTX
        WNRXH=WLF0H+WLF0H*(NWIRTU-NWIRLU)/NWIRLU-OFFSSF(ILYRT)+WSHFZS
        wnrxh_old=WLF0H *NWIRTU/NWIRLU-OFFSSF(ILYRT) + WSHFZS
        wnrxh=wnrxh_old

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
     1         (LN_SINCE_Z_CHANGE.GE.1).AND.
     1         (LN_SINCE_Z_CHANGE.LE.2))THEN
          WIDHIS(WH_S_SMO)=2
        ELSE
          WIDHIS(WH_S_SMO)=3
          WSHSM=(-RFX_L(2)*MULST + RFX_L(1)*VEL_PRED)/2/RPWSSF(ILYRT)
     2         *(RAD_1_PREV-RAD_2_PREV)/RFX_L(1)
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
C   insert truncated version of processing IHQLER 
C-----------------------------------------------------------------------

        IF(STOPBK.GE.0)THEN
          
          CALL SFCHAN_PREDETERMINED(.FALSE.,SFCERR)
          IF(SFCERR.NE.0)GO TO 599
          IF(ADTSF.EQ.0)THEN
            CONT_SEARCH_LAYER=0
          ELSE
            CONT_SEARCH_LAYER=1

            IF(IHQLER.EQ.0)THEN
              NNOTIS=0
              VELOCITY_USE=2
            ELSEIF(IHQLER.EQ.1)THEN
              NNOTIS=NNOTIS+1
              VELOCITY_USE=3
            ENDIF

          ENDIF

C=======================================================================
C  normal procedure to search for hits
C=======================================================================

        ELSE

C-----------------------------------------------------------------------
C  adjust SEARCH WIDTH when predicted is too close to cell center
C-----------------------------------------------------------------------

          IF(WID_AD_LAYR.GE.1)THEN
            WIDHIS(WH_W_CNTR)=0
          ELSEIF(
     1           (IRELX.LT.CCTRLO(SFCTYP)).OR.
     2           (IRELX.GT.CCTRHI(SFCTYP)))THEN
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
            FIRST_SEARCH_DIR=1
          ELSE
            NLO=-(1+WID_AD_LAYR/2)
            NHI=(WID_AD_LAYR+1)/2
            FIRST_SEARCH_DIR=-1
          ENDIF
          SEARCH_DIR=FIRST_SEARCH_DIR
          
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
              IF(WSHFZC.EQ.0)THEN
                WIDHIS(WH_W_ST_SH)=1
              ELSE
                WIDHIS(WH_W_ST_SH)=4
              ENDIF
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
          
C-----------------------------------------------------------------------
C  loop, test wires around the predicted wire
C-----------------------------------------------------------------------

 511      CONTINUE
          IF(NTEST.NE.0)THEN
            IF(CONT_SEARCH_LAYER.EQ.1)THEN
              IRELN=IREL+NUMR*SEARCH_DIR
              IF(IRELN.GT.NHI)THEN
                IF(MISLO.NE.0)THEN
                  CONT_SEARCH_LAYER=0
                ELSE
                  MISHI=1
                  NUMR=1
                  SEARCH_DIR=-1
                  GO TO 511
                ENDIF
              ELSEIF(IRELN.LT.NLO)THEN
                IF(MISHI.NE.0)THEN
                  CONT_SEARCH_LAYER=0
                ELSE
                  MISLO=1
                  NUMR=1
                  SEARCH_DIR=1
                  GO TO 511
                ENDIF
              ENDIF
            ENDIF
            
            IF(CONT_SEARCH_LAYER.EQ.1)THEN
              IF((MISLO.EQ.0).AND.(MISHI.EQ.0))THEN
                NUMR=NUMR+1
                SEARCH_DIR=-SEARCH_DIR
              ENDIF
              
              IREL=IRELN
              WNTU=WNPU+IREL
              IF(WNTU.LT.0)WNTU=WNTU+NWIRTU
              IF(WNTU.GE.NWIRTU)WNTU=WNTU-NWIRTU
            ENDIF
          ENDIF
          
          IF(CONT_SEARCH_LAYER.EQ.1)THEN
            NTEST=NTEST+1
            CALL SFCHAN_HIT_QUALITY
          ENDIF
                  
C-----------------------------------------------------------------------
C  process the HIT-QUALITY-OR-ERROR information 
C-----------------------------------------------------------------------

          IF(CONT_SEARCH_LAYER.EQ.1)THEN
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
              IF    (OKWSTR.GT.2)THEN
                VELOCITY_USE=4
              ELSEIF(NNOTIS.LE.MDOUBH(SFCTYP))THEN
                VELOCITY_USE=5
              ENDIF
              
            ELSEIF(IHQLER.EQ.2)THEN
              IF(  (AFT_N_ISO_S_OK(SFCTYP).EQ.0).AND.
     2             (PROC_NEW_Z_GROUP.EQ.1))THEN
                CONT_SEARCH_LAYER=0
              ENDIF
              IF(NNOTIS.GE.MNOTIS(SFCTYP))THEN
                IHQLER=-2
              ELSE
                NNOTIS=NNOTIS+1
              ENDIF
            ENDIF
            
            IF(IHQLER.LT.0)THEN
              
#if defined(CLEO_SFDIAG)
              CALL DSFHTF(ADTSF,
     1             SFCTYP,ILYRV, ILYRT,
     2             WNRX,  WSHSM, WNPX,  WNPU,
     3             VEL_PRLM,VEL_PRED,WIDHIS,
     4             DIFTNV, ZNOWX, WSHFZS,WSHFZC,
     5             WID_AD_PREV,WID_AD_LAYR,WID_AD_STER,WID_AD_SCAL,
     6             NHI,NLO,OKWSTR,   
     7             WNTU,         IHQLER)
#endif
              
              GO TO 511
            ENDIF

C endof.......IF(CONT_SEARCH_LAYER.EQ.1)THEN
          ENDIF
C endof.......IF(STOPBK.GE.0)THEN ... ELSE
        ENDIF
          
          
C-----------------------------------------------------------------------
C  BASIC INFORMATION TO DEFINE CHAIN
C-----------------------------------------------------------------------
          
        IF(CONT_SEARCH_LAYER.EQ.1)THEN
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
          
C-----------------------------------------------------------------------
C  loop back for a new layer
C-----------------------------------------------------------------------

C endof.......IF(CONT_SEARCH_LAYER.EQ.1)THEN
        ENDIF

        GO TO 61
        
C-----------------------------------------------------------------------
C  end of search for new layers in a chain
C-----------------------------------------------------------------------

C endof.......IF(CONT_CHAIN.EQ.1)THEN
      ENDIF

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
     2     (LN_GOOD_VEL.LT.LN_GOOD))THEN
        LN_GOOD=LN_GOOD_VEL
        IVTEMP(1)=VEL_GOOD_VEL
        IF(LN_GOOD.LT.MINLCH(SFCTYP))KEEP_CHAIN=0
      ENDIF
      
      IF((KEEP_CHAIN.EQ.1).AND.
     2     (Z_TEST_ACTIVE.EQ.1).AND.(LN_GOOD_Z.LT.LN_GOOD))THEN
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
 593    CONTINUE
        
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
     2               SFZMZ(IZHIT,ICHAIN)*100)
     3               /SFZMR(IZHIT,ICHAIN)
              ENDIF
 595        CONTINUE
            
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
        ENDIF

C endof.......IF(STRBAD.LE.LENCHN(ICHAIN))THEN
      ENDIF
      
C-----------------------------------------------------------------------
C  end of successful chain
C    note: this should be fixed for straglers
C-----------------------------------------------------------------------
      
      IF(KEEP_CHAIN.EQ.1)THEN
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
 9903   FORMAT(' SFCHAN: finished chain ',I3,' length=',I4)
        CALL DSFDMP
#endif
      
C-----------------------------------------------------------------------
C  allow abort of event
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
        CALL DSF_ABORT(ABORT)
#endif
      
C-----------------------------------------------------------------------
C  this cancels the hits of the current chain, now being displayed
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
        CALL DSF_CHVIEW_SIMPLE(ICHAIN,0)
#endif

      ENDIF
      
C-----------------------------------------------------------------------
C  return for next chain start
C-----------------------------------------------------------------------
      
 599  CONTINUE
      
C-----------------------------------------------------------------------
C print errors
C-----------------------------------------------------------------------
      
      IF(SFCERR.NE.0)THEN
#if defined(CLEO_SFDIAG)
        PRINT 1001,SFCERR,SFCTYP,ICHAIN
 1001   FORMAT(' SFCHAN: EXITING;',
     2       ' ERROR=',I2,'  ENTRY TYPE',I2,' CHAIN#',I3)
#endif
      ENDIF
      
C=======================================================================
      
      RETURN
      END








