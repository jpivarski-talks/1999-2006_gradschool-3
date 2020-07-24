*
* $Id: $
*
* $Log: $
*
* these comments inherited from TFXFIN
*
* Revision 1.21  2002/09/24 00:29:37  dpp
*      -> initialize MAXEXT, maximum allowed extrapolation width
*         this will effectively NOT allow standard tfxfit for silicon Z
*         if there is nothing found in local ambiguity tfxfit
*
* Revision 1.20  2001/11/20 17:57:17  dpp
* -> use new variable SCALNW for wide road selection of non group layers
*
* Revision 1.19  2001/11/19 23:59:40  dpp
*      -> remove direct access of the event control parameter
*      -> remove print statement
*      -> change SCALEN to be layer dependent
*
* Revision 1.18  2001/09/12 22:04:41  dpp
*      -> deweight hits added in STANDARD that have large entrance angle
*
* Revision 1.17  2001/05/01 17:26:46  dpp
*      -> tune cuts for efficiency dependent scaling  LOC AMB TFXFIT
*
* Revision 1.16  2001/04/19 17:20:40  dpp
*      -> clean up the changes made by Adam; conditions for initialization
*      -> initialize efficiency dependent roads
*      -> and set the efficiency initialization flags .FALSE.
*
* Revision 1.15  2001/04/19 17:19:14  dpp
*      -> clean up the changes made by Adam; conditions for initialization
*      -> initialize efficiency dependent roads
*
* Revision 1.14  2001/04/09 19:11:24  lyon
* Add back hidden initialization
*
* Revision 1.13  2001/04/05 01:58:35  lyon
* Remove hard wiring of SV Hybrid Efficiencies
*
* Revision 1.12  2001/03/30 00:01:00  dpp
*      -> initialize silicon hybrid efficiencies to default value
*         (this must be overwritten later)d3link
*      -> initialize layer number for each hybrid
*
* Revision 1.11  2000/12/19 23:34:44  dpp
*      -> change ISHFRD for drift chamber, .0084 -> .0040
*
* Revision 1.10  2000/10/24 22:50:30  dpp
*      -> increase SAMHRD for silicon r-phi and Z
*
* Revision 1.9  2000/10/02 20:48:11  dpp
*      -> reduce ISHTRD for r-phi wire hits from 4200 micron to 2500 micron
*      -> increase SAMHRD for r-phi wire hits from 120 micron to 550 micron
*      -> increase WSRDRD for r-phi wire hits from 350 micron to 550 micron
*      -> allow muli-hits in DR r-phi
*      -> z test on hits in standard
*      -> increase SAMHRD for r-phi wire hits from 2mm for calibration
*
* Revision 1.8  2000/08/28 17:42:01  dpp
*      -> increase value of BSRSRD on z side silicon
*      -> increase value of WSRDRD on z side silicon
*
* Revision 1.7  2000/08/18 18:29:46  dpp
*      -> comment about how to kill loc amb ISVX1 (currently NOT KILLED)
*
* Revision 1.6  2000/05/26 16:58:01  dpp
*      -> correct number of layers in silicon Z group
*      -> change some cuts from (fraction of cell size) to (absolute)
*      -> widen roads for unaligned silicon
*
* Revision 1.5  2000/04/26 15:07:24  dpp
*      -> allow loc amb for silicon f-phi
*
* Revision 1.4  2000/01/06 22:19:27  dpp
* initialize CH2SCL and CHICUT which used to be
*     initialized in linktree somewhere
*
* Revision 1.3  1999/04/27 20:20:02  dpp
* drift distance saturation .5 to .6;
* isolated hit separation 1. to .6;
* single gap allowed in drift chamber strings
*
* Revision 1.2  1998/07/09 00:52:07  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:30  clib
* Developmental version of DUET.
*
* Revision 1.7  1997/02/26 06:31:37  clib
* Narrow local window, eliminate SV isolation.
*
* Revision 1.6  1996/07/08 21:52:55  dpp
* change value of WSRDRD for z-side silicon
* fix loop index
* allow alternate value of SAMHRD for r-phi silicon
*
* Revision 1.5  1996/04/15 17:09:06  dpp
* change loop limit from MLTRX to NLYRCF (or NLYRLF) for some loops
*
* Revision 1.4  1996/02/06 00:13:20  zfiles
* allow gaps in strings. turned on in the silicon only
*
* Revision 1.3  1995/11/27 16:57:44  nk
* removed reference to run number, uses IDTOCD(ISVX1) etc
*
* Revision 1.2  1995/09/26 14:38:34  zfiles
* made consistant with LF hits and silicon hits
*
* Revision 1.1.1.1  1994/10/08  01:00:55  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/25 12/05/94  11.28.00  by  Anders Ryd
*CMZ :  6.00/15 05/03/94  12.35.10  by  Rob Kutschke
*-- Author :
      SUBROUTINE TFXFIN_DR3ZD
C.......................................................................
C.
C. TFXFIN_DR3ZD - Initialize cuts for TFXFIT
C.
C. COMMON    : TFXFITCD
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 22-Jul-87
C.
C.
C. Modification Log.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfxfitcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/svgeometry.inc"


C PARAMETERS IN TFXFITCD

C..NGRUP..... NUMBER OF AMBIGUITY GROUPS
C..MGLAYR.... MAXIMUM NUMBER OF LAYERS IN A GROUP
C..MGSTOR.... MAXIMUM NUMBER OF HITS IN A LAYER
C..TOTLGR.... PRODUCT GROUP*LAYER
C..TOTHGR.... PRODUCT GROUP*LAYER*HIT
C..NFTMIN.... MINIMUM OF NFIT TO ALLOW CALL TO CFCFIT, OTHERWISE RETURN ERROR

      LOGICAL L_CALIB
      INTEGER I,J,IDEV,LAY
      INTEGER TMP01(NGRUP),TMP02(NGRUP)
      INTEGER TMP03(NGRUP),TMP04(NGRUP)

      DATA TMP01/ 1, 5, 9,13,  17,20,  23,27,31,35,39,43,47,51,  1*0/
      DATA TMP02/ 4, 4, 4, 4,   3, 3,   4, 4, 4, 4, 4, 4, 4, 3,  1*0/
      DATA TMP03/ 1, 4,  13*0/
      DATA TMP04/ 3, 3,  13*0/
      
* ----------Executable code starts here---------------------

      CALL EVENT_CONTROL_BITS(EXB_CALIB,L_CALIB)
      LCFXLA   = .TRUE.

C ***************************************************************
C ZD-DR3 ********************************************************
C ***************************************************************

C..LLAYGR.... FIRST PHYSICAL LAYER, ILYR,  IN A GROUP
C..NLAYGR.... NUMBER OF TF LAYERS IN A GROUP
C..SKIPPS.... SKIP PASS; ok to skip this pass if previous had IGNORD=0
C..PASSAG..A. PASS NUMBER TO BEGIN ALWAYS ALLOWING USE OF THIS GROUP
C..MPASSA..A. MAXIMUM NUMBER OF PASSES THROUGH LOCAL AMBIGUITY
C..MPASSS..S. MAXIMUM NUMBER OF PASSES THROUGH STANDARD TFXFIT
C..MPASSL..A. MAXIMUM NUMBER OF PASSES selecting other groups
C             with residual difference relative to best
C..PASS1G..A. FIRST PASS TO ALLOW USE OF THIS GROUP

      DO 211 I=1,NGRUP
        LLAYGR(I,1) =                                   TMP01(I)
        NLAYGR(I,1) =                                   TMP02(I)
        PASSAG(I,1) =                                   9
 211  CONTINUE
      
C EXTRA ZEROED SKIPPS(5) IS TO DISPLAY LAST LOC-AM SEPARATELY
      DO 213 I=1,4
        SKIPPS(I,1) =                                   0
 213  CONTINUE
      DO 215 I=5,10
        SKIPPS(I,1) =                                   1
 215  CONTINUE
      MPASSA(1) =                                       3
      MPASSS(1) =                                       3
      MPASSL(1) =                                       3
      
      IF((MPASSA(1)+MPASSS(1)).GT.MPASS)THEN
        IF(MPASSA(1).GT.MPASS)THEN
          MPASSA(1)=MPASS
          MPASSS(1)=0
        ELSE
          MPASSS(1)=MPASS-MPASSA(1)
        ENDIF
      ENDIF
          
C ***************************************************************

      L0TFXF(1)=                                    LLYRCF-1

C ***************************************************************

C..BSRSRD..A. BEST STRING RESIDUAL ROAD (for each pass)

      DO 309 I=1,MPASS
        BSRSRD(I,1)=                                    0.000800
 309  CONTINUE
      
      DO 315 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
        IF    (IDEV.EQ.IZDA)THEN
          PASS1G(I,1) =                                 3
          BSRSRD(PASS1G(I,1),1)=                        0.004500
        ELSEIF(IDEV.EQ.IDRFT)THEN
          IF(LLAYGR(I,1).LT.23)THEN
            PASS1G(I,1) =                               1
            BSRSRD(PASS1G(I,1),1)=                      0.004500
          ELSE  
            PASS1G(I,1) =                               2
            BSRSRD(PASS1G(I,1),1)=                      0.004500
          ENDIF
        ELSE
          PASS1G(I,1) =                                 9
        ENDIF
 315  CONTINUE
      

C..HEFSCL..S. HIGH EFFICIENCY SCALE OF RESOLUTION TO DETERMINE ROAD, 
C..ULEFSC.... LOGICAL: USE LOW EFFICIENCY SCALING, otherwise default to HEFSCL 
C..LEFSCL..S. LOW EFFICIENCY SCALE OF RESOLUTION TO DETERMINE ROAD, 
C..LCMPEF..S. LOW EFF SCALE USED BELOW THIS LOW COMPLIMENTARY EFFICIENCY
C..HCMPEF..S. HIGH EFF SCALE USED ABOVE THIS HIGH COMPLIMENTARY EFFICIENCY
C..STANRD..S. STANDARD TFXFIT narrow absolute ROAD, fraction of CHISQ cut

      DO 318 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
        IF    (IDEV.EQ.IZDA)THEN
          ULEFSC(I,1)=                                  .FALSE.
          HEFSCL(I,1)=                                  2.
        ELSE
          ULEFSC(I,1)=                                  .FALSE.
          HEFSCL(I,1)=                                  2.
        ENDIF
 318  CONTINUE
            
      STANRD(1) =                                       0.50

C..ISHFRD..A. ISOLATED HIT ROAD when no previous hit(was LUSETF=.FALSE.) 
C             two hits are isolated if separated by this absolute distance
C..ISHTRD..A. ISOLATED HIT ROAD,as ISHFRD but previous hit,(was LUSETF=.TRUE.) 
C..ILAMRD..A. INITIAL LOCAL AMBIGUITY ROAD, absolute
C..CHRDHE..A. CORRELATED HITS ROAD, HIGH PRODUCT EFFICIENCY, loc amb, absolute
C these also use the logical ULEFSC.... otherwise default to CHRDHE 
C..CHRDLE..A. CORRELATED HITS ROAD, LOW PRODUCT EFFICIENCY, loc amb, absolute
C..HPRDEF..A. HIGH PRODUCT EFFICIENCY, above which use CHRDHE
C..LPRDEF..A. LOW PRODUCT EFFICIENCY, below which use CHRDLE
C..WSRDRD..A. WITHIN STRING RESIDUAL DIFFERENCE ROAD; resid relative to average
C..DEWRDR..A. DEWEIGHT REDUCE DRIFT DISTANCE
C..LDRFRD..A. LARGE DRIFTDISTANCE ROAD,  cut, fraction of a full cell

      DO 327 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
        IF    (IDEV.EQ.IZDA)THEN
          ISHFRD(I,1) =                                 0.0084
          ISHTRD(I,1) =                                 0.0025
          ILAMRD(I,1) =                                 0.035
          CHRDHE(I,1) =                                 0.0021
          WSRDRD(I,1) =                                 0.00055
          LDRFRD(I,1) =                                 0.60
          DEWRDR(I,1) =                                 9.0
        ELSE
          ISHFRD(I,1) =                                 0.0084
          ISHTRD(I,1) =                                 0.0025
          ILAMRD(I,1) =                                 0.035
          CHRDHE(I,1) =                                 0.0021
          WSRDRD(I,1) =                                 0.00055
          LDRFRD(I,1) =                                 0.60
          DEWRDR(I,1) =                                 9.0
        ENDIF
 327  CONTINUE
      
C..SAMHRD..A. SAME HIT ROAD,  within which two hits are essentially identical
C LSMHRA = false in DOITNL

      DO 333 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
        IF    (IDEV.EQ.IZDA)THEN
          SAMHRD(I,1) =                                 0.000550
        ELSE
          
          IF(L_CALIB)THEN
            SAMHRD(I,1) =                               0.002000
            print 9902
 9902       format('           and I am setting SAMHRD 2mm')
          ELSE
            SAMHRD(I,1) =                               0.000550
          ENDIF
        ENDIF
 333  CONTINUE

C..SCALEN..S. SCALE TO USE WHEN LAYER IS NOT IN A GROUP
C..SCALNW..S. SCALE TO USE WHEN LAYER IS NOT IN A GROUP, WIDE ROAD
C..STZERD..S. STANDARD TFXFIT Z at Endplate absolute ROAD
C..MAXEXT..S. MAXimum EXTrapolation error into layer
C..SMLDIF..S. SMALL ABS RESIDUAL DIF;below which is confusing which hit to take
C..LRGDIF..S. LARGE RESIDUAL ABS DIF;above which, do not assign confusing hits
C..DEWCRL.. . DEWEIGHT when hit is at top of CURLER

      DO 335 I=1,MLTFX
        IDEV = IDVCTF(I+L0TFXF(1))
        IF    (IDEV.EQ.IZDA)THEN
          STZERD(I,1) =                                 1.20
          MAXEXT(I,1) =                                 0.99  
          SMLDIF(I,1) =                                 0.000250
          LRGDIF(I,1) =                                 0.000350
          DEWCRL(I,1) =                                 100.
        ELSE
          STZERD(I,1) =                                 1.20
          MAXEXT(I,1) =                                 0.99  
          SMLDIF(I,1) =                                 0.000250
          LRGDIF(I,1) =                                 0.000350
          DEWCRL(I,1) =                                 100.
        ENDIF
        SCALEN(I,1) =                                   2.
        SCALNW(I,1) =                                   2.
 335  CONTINUE
      
C..MAXGAP..A. MAXIMUM SINGLE GAP allowed in a string
C..TOTGAP..A. MAXIMUM TOTAL of all GAPS allowed in a string 
C..BSNHMN..A. BEST STRING, NUMBER of HITS MINIMUM, now a variable

      DO 345 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
        IF    (IDEV.EQ.IZDA)THEN
          MAXGAP(I,1) =                                 1
          TOTGAP(I,1) =                                 1
          BSNHMN(I,1) =                                 3
        ELSEIF(IDEV.EQ.IDRFT)THEN
          MAXGAP(I,1) =                                 1
          TOTGAP(I,1) =                                 1
          BSNHMN(I,1) =                                 3
        ELSE
          MAXGAP(I,1) =                                 0
          TOTGAP(I,1) =                                 0
          BSNHMN(I,1) =                                 9
        ENDIF
 345  CONTINUE
      
C..SRDFL0..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 0
C..SRDFL1..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 1

      SRDFL0(1) =                                       0.000300
      SRDFL1(1) =                                       0.000800
      
C..OSRDRD..A. OTHER STRING RESIDUAL DIFFERENCE ROAD

      DO 347 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
        IF    (IDEV.EQ.IZDA)THEN
          OSRDRD(I,1)=                                  0.000600
        ELSE
          OSRDRD(I,1)=                                  0.000600
        ENDIF
 347  CONTINUE
      
C..IGSSLN..A. IGNORED STRING SIGNIFICANT LENGTH; to set IGNORE flag

CCC      should IGSSLN=3 ????  10/25/92
      IGSSLN(1) =                                       2

C..MULTIH..A. MULTIPLE HITS; add more than one hit per layer only in LOC AMB

      DO 349 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
        IF    (IDEV.EQ.IZDA)THEN
          MULTIH(I,1)=                                  .TRUE.
        ELSE
          MULTIH(I,1)=                                  .TRUE.
        ENDIF
 349  CONTINUE
      



C ***************************************************************
C ZD and cathode ************************************************
C ***************************************************************

C..LLAYGR.... FIRST PHYSICAL LAYER, ILYR,  IN A GROUP
C..NLAYGR.... NUMBER OF TF LAYERS IN A GROUP
C..SKIPPS.... SKIP PASS; ok to skip this pass if previous had IGNORD=0
C..PASSAG..A. PASS NUMBER TO BEGIN ALWAYS ALLOWING USE OF THIS GROUP
C..MPASSA..A. MAXIMUM NUMBER OF PASSES THROUGH LOCAL AMBIGUITY
C..MPASSS..S. MAXIMUM NUMBER OF PASSES THROUGH STANDARD TFXFIT
C..MPASSL..A. MAXIMUM NUMBER OF PASSES selecting other groups
C             with residual difference relative to best
C..PASS1G..A. FIRST PASS TO ALLOW USE OF THIS GROUP

      DO 411 I=1,NGRUP
        LLAYGR(I,2) =                                   TMP03(I)
        NLAYGR(I,2) =                                   TMP04(I)
        PASSAG(I,2) =                                   9
 411  CONTINUE
C EXTRA ZEROED SKIPPS(5) IS TO DISPLAY LAST LOC-AM SEPARATELY
      DO 423 I=1,2
        SKIPPS(I,2) =                                   0
 423  CONTINUE
      DO 425 I=3,10
        SKIPPS(I,2) =                                   1
 425  CONTINUE
      MPASSA(2) =                                       1
      MPASSS(2) =                                       1
      MPASSL(2) =                                       1
      
      IF((MPASSA(2)+MPASSS(2)).GT.MPASS)THEN
        IF(MPASSA(2).GT.MPASS)THEN
          MPASSA(2)=MPASS
          MPASSS(2)=0
        ELSE
          MPASSS(2)=MPASS-MPASSA(2)
        ENDIF
      ENDIF
          
        
C ***************************************************************
            
      L0TFXF(2)=                                        LLYRLF-1

C..BSRSRD..A. BEST STRING RESIDUAL ROAD (for each pass)

      DO 509 I=1,MPASS
        BSRSRD(I,2)=                                    0.000800
 509  CONTINUE
      
      DO 515 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
        IF    (IDEV.EQ.IZDA)THEN
          PASS1G(I,2) =                                 1
          BSRSRD(PASS1G(I,2),2)=                        0.015000
        ELSE
          PASS1G(I,2) =                                 9
        ENDIF
 515  CONTINUE
      
      
C..HEFSCL..S. HIGH EFFICIENCY SCALE OF RESOLUTION TO DETERMINE ROAD, 
C..ULEFSC..S. LOGICAL: USE LOW EFFICIENCY SCALING 
C..LEFSCL..S. LOW EFFICIENCY SCALE OF RESOLUTION TO DETERMINE ROAD, 
C..LCMPEF..S. LOW EFF SCALE USED BELOW THIS LOW COMPLIMENTARY EFFICIENCY
C..HCMPEF..S. HIGH EFF SCALE USED ABOVE THIS HIGH COMPLIMENTARY EFFICIENCY
C..STANRD..S. STANDARD TFXFIT narrow absolute ROAD, fraction of CHISQ cut

      DO 518 I=1,NGRUP
        HEFSCL(I,2)=                                    2.
        IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
        IF    (IDEV.EQ.IZDA)THEN
          ULEFSC(I,2)=                                 .FALSE.
        ELSE
          ULEFSC(I,2)=                                 .FALSE.
        ENDIF
 518  CONTINUE
      
      STANRD(2) =                                      0.50
      
C..ISHFRD..A. ISOLATED HIT ROAD when no previous hit(was LUSETF=.FALSE.) 
C             two hits are isolated if separated by this absolute distance
C..ISHTRD..A. ISOLATED HIT ROAD,as ISHFRD but previous hit,(was LUSETF=.TRUE.) 
C..ILAMRD..A. INITIAL LOCAL AMBIGUITY ROAD, absolute
C..CHRDHE..A. CORRELATED HITS ROAD, HIGH PRODUCT EFFICIENCY, loc amb, absolute
C these also use the logical ULEFSC.... otherwise default to CHRDHE 
C..CHRDLE..A. CORRELATED HITS ROAD, LOW PRODUCT EFFICIENCY, loc amb, absolute
C..HPRDEF..A. HIGH PRODUCT EFFICIENCY, above which use CHRDHE
C..LPRDEF..A. LOW PRODUCT EFFICIENCY, below which use CHRDLE
C..SAMHRD..A. SAME HIT ROAD,  within which two hits are essentially identical
C..WSRDRD..A. WITHIN STRING RESIDUAL DIFFERENCE ROAD; resid relative to average
C..DEWRDR..A. DEWEIGHT REDUCE DRIFT DISTANCE
C..LDRFRD..A. LARGE DRIFTDISTANCE ROAD,  cut, fraction of a full cell

      DO 527 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
        IF    (IDEV.EQ.IZDA)THEN
          ISHFRD(I,2) =                                0.014
          ISHTRD(I,2) =                                0.0042
          ILAMRD(I,2) =                                0.035
          CHRDHE(I,2) =                                0.0021
          WSRDRD(I,2) =                                0.00035
          SAMHRD(I,2) =                                0.000400
          LDRFRD(I,2) =                                0.50
          DEWRDR(I,2) =                                9.0
        ELSE
          ISHFRD(I,2) =                                0.014
          ISHTRD(I,2) =                                0.0042
          ILAMRD(I,2) =                                0.035
          CHRDHE(I,2) =                                0.0021
          WSRDRD(I,2) =                                0.00035
          SAMHRD(I,2) =                                0.000400
          LDRFRD(I,2) =                                0.50
          DEWRDR(I,2) =                                9.0
        ENDIF
 527  CONTINUE
      
C..SCALEN..S. SCALE TO USE WHEN LAYER IS NOT IN A GROUP
C..SCALNW..S. SCALE TO USE WHEN LAYER IS NOT IN A GROUP, WIDE ROAD
C..STZERD..S. STANDARD TFXFIT Z at Endplate absolute ROAD
C..MAXEXT..S. MAXimum EXTrapolation error into layer
C..SMLDIF..S. SMALL ABS RESIDUAL DIF;below which is confusing which hit to take
C..LRGDIF..S. LARGE RESIDUAL ABS DIF;above which, do not assign confusing hits
C..DEWCRL.. . DEWEIGHT when hit is at top of CURLER

      DO 535 I=1,NLYRLF
        IDEV = IDVCTF(I+L0TFXF(2))
        IF    (IDEV.EQ.IZDA)THEN
          STZERD(I,2) =                               1.20
          MAXEXT(I,2) =                               0.99  
          SMLDIF(I,2) =                               0.000250
          LRGDIF(I,2) =                               0.000350
          DEWCRL(I,2) =                               100.
        ELSE
          STZERD(I,2) =                               1.20
          MAXEXT(I,2) =                               0.99  
          SMLDIF(I,2) =                               0.000250
          LRGDIF(I,2) =                               0.000350
          DEWCRL(I,2) =                               100.
        ENDIF
        IF    (IDEV.EQ.IDRFT)THEN
          SCALEN(I,2) =                               2.
          SCALNW(I,2) =                               6.
        ELSE
          SCALEN(I,2) =                               2.
          SCALNW(I,2) =                               2.
        ENDIF
 535  CONTINUE

C..MAXGAP..A. MAXIMUM SINGLE GAP allowed in a string
C..TOTGAP..A. MAXIMUM TOTAL of all GAPS allowed in a string 
C..BSNHMN..A. BEST STRING, NUMBER of HITS MINIMUM, now a variable

      DO 545 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
        IF    (IDEV.EQ.IZDA)THEN
          MAXGAP(I,2) =                               1
          TOTGAP(I,2) =                               1
          BSNHMN(I,2) =                               2
        ELSE
          MAXGAP(I,2) =                               0
          TOTGAP(I,2) =                               0
          BSNHMN(I,2) =                               9
        ENDIF
 545  CONTINUE
      
C..SRDFL0..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 0
C..SRDFL1..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 1

      SRDFL0(2) =                                     0.003000
      SRDFL1(2) =                                     0.008000
      
C..OSRDRD..A. OTHER STRING RESIDUAL DIFFERENCE ROAD

      DO 547 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
        IF    (IDEV.EQ.IZDA)THEN
          OSRDRD(I,2)=                                0.000250
        ELSE
          OSRDRD(I,2)=                                0.000600
        ENDIF
 547  CONTINUE
      
C..IGSSLN..A. IGNORED STRING SIGNIFICANT LENGTH; to set IGNORE flag

CCC      should IGSSLN=3 ????  10/25/92
      IGSSLN(2) =                                     2
      
C..MULTIH..A. MULTIPLE HITS; add more than one hit per layer only in LOC AMB
      
      DO 549 I=1,NGRUP
        IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
        IF    (IDEV.EQ.IZDA)THEN
          MULTIH(I,2)=                                .TRUE.
        ELSE
          MULTIH(I,2)=                                .FALSE.
        ENDIF
 549  CONTINUE
      
C **************************************
C extra varaibles that used to be filled in linktree parts

      CH2SCL(1)=2.0
      CH2SCL(2)=1.6
      CH2SCL(3)=1.3
      CH2SCL(4)=1.1
      DO 611 I=5,KLYRTF
         CH2SCL(I)=1.0
 611  CONTINUE

      RETURN
      END


