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
      SUBROUTINE TFXFIN_DR3Si
C.......................................................................
C.
C. TFXFIN_DR3Si - Initialize cuts for TFXFIT
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
      INTEGER TEMP
      LOGICAL MEAS_EFF/.FALSE./
      LOGICAL DFLT_EFF/.FALSE./

      DATA TMP01/ 1, 5, 9,13,17,25,29,33,37,41,45,49,53,2*0/
      DATA TMP02/ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3,2*0/
      DATA TMP03/ 1, 14*0/
      DATA TMP04/ 4, 14*0/
      
* ----------Executable code starts here---------------------

      CALL EVENT_CONTROL_BITS(EXB_CALIB,L_CALIB)
      LCFXLA   = .TRUE.

C ***************************************************************
C SI-DR3 ********************************************************
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
         LLAYGR(I,1) =                                TMP01(I)
         NLAYGR(I,1) =                                TMP02(I)
         PASSAG(I,1) =                                9
 211  CONTINUE

C EXTRA ZEROED SKIPPS(5) IS TO DISPLAY LAST LOC-AM SEPARATELY
      DO 213 I=1,4
213   SKIPPS(I,1) =                                   0
      DO 215 I=5,10
215   SKIPPS(I,1) =                                   1

      MPASSA(1) =                                     3
      MPASSS(1) =                                     3
      MPASSL(1) =                                     3

      IF((MPASSA(1)+MPASSS(1)).GT.MPASS)THEN
         IF(MPASSA(1).GT.MPASS)THEN
            MPASSA(1)=MPASS
            MPASSS(1)=0
         ELSE
            MPASSS(1)=MPASS-MPASSA(1)
         ENDIF
      ENDIF

C ***************************************************************

      L0TFXF(1)=                                      LLYRCF-1

C..BSRSRD..A. BEST STRING RESIDUAL ROAD (for each pass)

      DO 309 I=1,MPASS
         BSRSRD(I,1)=                                 0.000800
 309  CONTINUE

      DO 315 I=1,NGRUP
         IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
         IF    (IDEV.EQ.ISVX1)THEN
            PASS1G(I,1) =                             3
            BSRSRD(PASS1G(I,1),1)=                    0.004500
         ELSEIF(IDEV.EQ.IPTLA)THEN
            PASS1G(I,1) =                             9
         ELSEIF(IDEV.EQ.IVDET)THEN
            PASS1G(I,1) =                             9
         ELSEIF(IDEV.EQ.IDRFT)THEN
            IF(LLAYGR(I,1).LT.25)THEN
               PASS1G(I,1) =                          1
               BSRSRD(PASS1G(I,1),1)=                 0.004500
            ELSE
               PASS1G(I,1) =                          2
               BSRSRD(PASS1G(I,1),1)=                 0.004500
            ENDIF
         ELSE
            PASS1G(I,1) =                             9
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
         IF    (IDEV.EQ.ISVX1)THEN
            ULEFSC(I,1)=                              .TRUE.
            HEFSCL(I,1)=                              2.
            LEFSCL(I,1)=                              4.
            LCMPEF(I,1)=                              0.9
            HCMPEF(I,1)=                              1.8
         ELSE
            ULEFSC(I,1)=                              .FALSE.
            HEFSCL(I,1)=                              2.
         ENDIF
 318  CONTINUE
            
      STANRD(1) =                                     0.50

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
         IF    (IDEV.EQ.ISVX1)THEN
            ISHFRD(I,1) =                             0.0008
            ISHTRD(I,1) =                             0.0005
            ILAMRD(I,1) =                             0.010

C (DON'T) KILL LOC AMB ISVX1
C            ILAMRD(I,1) =                             0.0
            CHRDHE(I,1) =                             0.000900
            CHRDLE(I,1) =                             0.000300
            HPRDEF(I,1) =                             0.40
            LPRDEF(I,1) =                             0.05
            WSRDRD(I,1) =                             0.0005
            LDRFRD(I,1) =                             9.9
            DEWRDR(I,1) =                             1.0

         ELSE
C testing modification ISHFRD(I,1) =                             0.0084
            ISHFRD(I,1) =                             0.0084
C            ISHFRD(I,1) =                             0.0040
C testing modification ISHTRD(I,1) =                             0.0042
            ISHTRD(I,1) =                             0.0025

            ILAMRD(I,1) =                             0.035
            CHRDHE(I,1) =                             0.0021
C testing modification WSRDRD(I,1) =                             0.00035
            WSRDRD(I,1) =                             0.00055
            LDRFRD(I,1) =                             0.60
            DEWRDR(I,1) =                             9.0
         ENDIF
 327  CONTINUE

C..SAMHRD..A. SAME HIT ROAD,  within which two hits are essentially identical

      DO 333 I=1,NGRUP
      IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
      IF    (IDEV.EQ.ISVX1)THEN
C LSMHRA = false in DOITNL
       IF(LSMHRA)THEN
        SAMHRD(I,1) =                                 0.000200
       ELSE
C testing modification SAMHRD(I,1) =                                 0.000040
        SAMHRD(I,1) =                                 0.000100
       ENDIF
      ELSE

         IF(L_CALIB)THEN
            SAMHRD(I,1) =                                   0.002000
            print 9902
 9902       format('           and I am setting SAMHRD 2mm')
         ELSE
C testing modification SAMHRD(I,1) =                             0.000120
C testing modification SAMHRD(I,1) =                             0.000200
            SAMHRD(I,1) =                                   0.000550
         ENDIF
      ENDIF
333   CONTINUE

C..SCALEN..S. SCALE TO USE WHEN LAYER IS NOT IN A GROUP
C..SCALNW..S. SCALE TO USE WHEN LAYER IS NOT IN A GROUP, WIDE ROAD
C..STZERD..S. STANDARD TFXFIT Z at Endplate absolute ROAD
C..MAXEXT..S. MAXimum EXTrapolation error into layer
C..SMLDIF..S. SMALL ABS RESIDUAL DIF;below which is confusing which hit to take
C..LRGDIF..S. LARGE RESIDUAL ABS DIF;above which, do not assign confusing hits
C..DEWCRL.. . DEWEIGHT when hit is at top of CURLER

      DO 335 I=1,MLTFX
        IDEV = IDVCTF(I+L0TFXF(1))
        IF    (IDEV.EQ.ISVX1)THEN
          STZERD(I,1) =                                   0.30
          MAXEXT(I,1) =                                   0.99  
          SMLDIF(I,1) =                                   0.000150
          LRGDIF(I,1) =                                   0.000350
          DEWCRL(I,1) =                                   1.
        ELSE
          STZERD(I,1) =                                   1.20
          MAXEXT(I,1) =                                   0.99  
          SMLDIF(I,1) =                                   0.000250
          LRGDIF(I,1) =                                   0.000350
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
      IF    (IDEV.EQ.ISVX1)THEN
      MAXGAP(I,1) =                                   1
      TOTGAP(I,1) =                                   1
      BSNHMN(I,1) =                                   2
      ELSEIF(IDEV.EQ.IPTLA)THEN
      MAXGAP(I,1) =                                   0
      TOTGAP(I,1) =                                   0
      BSNHMN(I,1) =                                   2
      ELSEIF(IDEV.EQ.IVDET)THEN
      MAXGAP(I,1) =                                   0
      TOTGAP(I,1) =                                   0
      BSNHMN(I,1) =                                   2
      ELSEIF(IDEV.EQ.IDRFT)THEN
      MAXGAP(I,1) =                                   1
      TOTGAP(I,1) =                                   1
      BSNHMN(I,1) =                                   3
      ELSE
      MAXGAP(I,1) =                                   0
      TOTGAP(I,1) =                                   0
      BSNHMN(I,1) =                                   9
      ENDIF
345   CONTINUE

C..SRDFL0..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 0
C..SRDFL1..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 1

      SRDFL0(1) =                                     0.000300
      SRDFL1(1) =                                     0.000800

C..OSRDRD..A. OTHER STRING RESIDUAL DIFFERENCE ROAD

      DO 347 I=1,NGRUP
      IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
      IF    (IDEV.EQ.ISVX1)THEN
      OSRDRD(I,1)=                                    0.000250
      ELSE
      OSRDRD(I,1)=                                    0.000600
      ENDIF
347   CONTINUE

C..IGSSLN..A. IGNORED STRING SIGNIFICANT LENGTH; to set IGNORE flag

CCC      should IGSSLN=3 ????  10/25/92
      IGSSLN(1) =                                     2

C..MULTIH..A. MULTIPLE HITS; add more than one hit per layer only in LOC AMB

      DO 349 I=1,NGRUP
      IDEV = IDVCTF(LLAYGR(I,1)+L0TFXF(1))
      IF    (IDEV.EQ.ISVX1)THEN
      MULTIH(I,1)=                                    .TRUE.
      ELSE
      MULTIH(I,1)=                                    .TRUE.
      ENDIF
349   CONTINUE




C ***************************************************************
C SIlicon - z ********************************************************
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


      IF    ((IDTOCD(ISVZ1).GT.0)      )THEN

      DO 411 I=1,NGRUP
      LLAYGR(I,2) =                                   TMP03(I)
      NLAYGR(I,2) =                                   TMP04(I)
411   PASSAG(I,2) =                                   9

C EXTRA ZEROED SKIPPS(5) IS TO DISPLAY LAST LOC-AM SEPARATELY
      DO 423 I=1,2
423   SKIPPS(I,2) =                                   0
      DO 425 I=3,10
425   SKIPPS(I,2) =                                   1

      MPASSA(2) =                                     1
      MPASSS(2) =                                     1
      MPASSL(2) =                                     1

      IF((MPASSA(2)+MPASSS(2)).GT.MPASS)THEN
         IF(MPASSA(2).GT.MPASS)THEN
            MPASSA(2)=MPASS
            MPASSS(2)=0
         ELSE
            MPASSS(2)=MPASS-MPASSA(2)
         ENDIF
      ENDIF

      ENDIF

C ***************************************************************

      L0TFXF(2)=                                      LLYRLF-1

C..BSRSRD..A. BEST STRING RESIDUAL ROAD (for each pass)

      DO 509 I=1,MPASS
         BSRSRD(I,2)=                                 0.000800
 509  CONTINUE

      DO 515 I=1,NGRUP
         IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
         IF    (IDEV.EQ.ISVZ1)THEN
            PASS1G(I,2) =                             1
            BSRSRD(PASS1G(I,2),2)=                    0.015000
         ELSE
            PASS1G(I,2) =                             9
         ENDIF
 515  CONTINUE


C..HEFSCL..S. HIGH EFFICIENCY SCALE OF RESOLUTION TO DETERMINE ROAD, 
C..ULEFSC..S. LOGICAL: USE LOW EFFICIENCY SCALING 
C..LEFSCL..S. LOW EFFICIENCY SCALE OF RESOLUTION TO DETERMINE ROAD, 
C..LCMPEF..S. LOW EFF SCALE USED BELOW THIS LOW COMPLIMENTARY EFFICIENCY
C..HCMPEF..S. HIGH EFF SCALE USED ABOVE THIS HIGH COMPLIMENTARY EFFICIENCY
C..STANRD..S. STANDARD TFXFIT narrow absolute ROAD, fraction of CHISQ cut

      DO 518 I=1,NGRUP
         HEFSCL(I,2)=                                 2.
         IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
         IF    (IDEV.EQ.ISVZ1)THEN
            ULEFSC(I,2)=                              .TRUE.
            LEFSCL(I,2)=                              4.
            LCMPEF(I,2)=                              0.9
            HCMPEF(I,2)=                              1.8
         ELSE
            ULEFSC(I,2)=                              .FALSE.
         ENDIF
 518  CONTINUE

      STANRD(2) =                                     0.50

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
         IF    (IDEV.EQ.ISVZ1)THEN
            ISHFRD(I,2) =                             0.003
            ISHTRD(I,2) =                             0.002
            ILAMRD(I,2) =                             0.02
            CHRDHE(I,2) =                             0.002
            CHRDLE(I,2) =                             0.001
            HPRDEF(I,2) =                             0.4
            LPRDEF(I,2) =                             0.1
            WSRDRD(I,2) =                             0.0013
C testing modification SAMHRD(I,2) =                             0.000060
            SAMHRD(I,2) =                             0.000300
            LDRFRD(I,2) =                             9.9
            DEWRDR(I,2) =                             1.0
         ELSE
            ISHFRD(I,2) =                             0.014
            ISHTRD(I,2) =                             0.0042
            ILAMRD(I,2) =                             0.035
            CHRDHE(I,2) =                             0.0021
            WSRDRD(I,2) =                             0.00035
            SAMHRD(I,2) =                             0.000400
            LDRFRD(I,2) =                             0.50
            DEWRDR(I,2) =                             9.0
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
        IF    (IDEV.EQ.ISVZ1)THEN
          STZERD(I,2) =                             0.30
          MAXEXT(I,2) =                             0.003  
          SMLDIF(I,2) =                             0.000150
          LRGDIF(I,2) =                             0.000350
          DEWCRL(I,2) =                             1.
        ELSE
          STZERD(I,2) =                             1.20
          MAXEXT(I,2) =                             0.99  
          SMLDIF(I,2) =                             0.000250
          LRGDIF(I,2) =                             0.000350
          DEWCRL(I,2) =                           100.
        ENDIF
        IF    (IDEV.EQ.IDRFT)THEN
          SCALEN(I,2) =                                   2.
          SCALNW(I,2) =                                   6.
        ELSE
          SCALEN(I,2) =                                   2.
          SCALNW(I,2) =                                   2.
        ENDIF
 535  CONTINUE

C..MAXGAP..A. MAXIMUM SINGLE GAP allowed in a string
C..TOTGAP..A. MAXIMUM TOTAL of all GAPS allowed in a string 
C..BSNHMN..A. BEST STRING, NUMBER of HITS MINIMUM, now a variable

      DO 545 I=1,NGRUP
      IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
      IF    (IDEV.EQ.ISVZ1)THEN
      MAXGAP(I,2) =                                   1
      TOTGAP(I,2) =                                   1
      BSNHMN(I,2) =                                   2
      ELSE
      MAXGAP(I,2) =                                   0
      TOTGAP(I,2) =                                   0
      BSNHMN(I,2) =                                   9
      ENDIF
545   CONTINUE

C..SRDFL0..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 0
C..SRDFL1..A. SIGNIFICANT RESOLUTION DIFFERENCE, LENGTH DIFFERENT BY 1

      SRDFL0(2) =                                     0.003000
      SRDFL1(2) =                                     0.008000

C..OSRDRD..A. OTHER STRING RESIDUAL DIFFERENCE ROAD

      DO 547 I=1,NGRUP
      IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
      IF    (IDEV.EQ.ISVZ1)THEN
      OSRDRD(I,2)=                                    0.000250
      ELSE
      OSRDRD(I,2)=                                    0.000600
      ENDIF
547   CONTINUE

C..IGSSLN..A. IGNORED STRING SIGNIFICANT LENGTH; to set IGNORE flag

CCC      should IGSSLN=3 ????  10/25/92
      IGSSLN(2) =                                     2

C..MULTIH..A. MULTIPLE HITS; add more than one hit per layer only in LOC AMB

      DO 549 I=1,NGRUP
      IDEV = IDVCTF(LLAYGR(I,2)+L0TFXF(2))
      IF    (IDEV.EQ.ISVZ1)THEN
      MULTIH(I,2)=                                    .TRUE.
      ELSE
      MULTIH(I,2)=                                    .FALSE.
      ENDIF
549   CONTINUE

C **************************************
C extra varaibles that used to be filled in linktree parts

      CH2SCL(1)=2.0
      CH2SCL(2)=1.6
      CH2SCL(3)=1.3
      CH2SCL(4)=1.1
      DO 611 I=5,KLYRTF
         CH2SCL(I)=1.0
 611  CONTINUE


C initialize the hybrid efficiencies
      DO 629 I=1,NUM_SV_HYBRIDS
         IF(DFLT_EFF)THEN
            SV_HYBRID_EFF(I,1)=1.
            SV_HYBRID_EFF(I,2)=1.
         ENDIF
C find the layer for this hybrid
         IF(NUM_SV_LAYERS.GT.1)THEN
            DO 627 LAY=1,NUM_SV_LAYERS
               IF(NUM_HYBRIDS_LAYER(LAY).GT.1)THEN
                  DO 625 J=1,NUM_HYBRIDS_LAYER(LAY)
                     IF(SV_HYBRIDS(LAY,J).EQ.I)THEN
                        SV_HYBRID_LAYER(I)=LAY

                        IF(DFLT_EFF)THEN
                           IF(LAY.EQ.1)THEN
                              SV_HYBRID_EFF(I,1)=0.1
                           ELSEIF(LAY.EQ.2)THEN
                              SV_HYBRID_EFF(I,1)=0.2
                           ELSEIF(LAY.EQ.3)THEN
                              SV_HYBRID_EFF(I,1)=0.6
                           ELSEIF(LAY.EQ.4)THEN
                              SV_HYBRID_EFF(I,1)=0.9
                           ENDIF
                        ENDIF

                     ENDIF
 625              CONTINUE
               ENDIF
 627        CONTINUE
         ENDIF
C  end loop over hybrids         
 629  CONTINUE

C  these are calculated from runs 116131 to 116185 and
C are suitable for dataset 10. Let me know if you prefer
C a different format.
C                    jean
      IF(MEAS_EFF)THEN

C Phi Side
         SV_HYBRID_EFF(1,1)=0.0663515
         SV_HYBRID_EFF(2,1)=0.0473431
         SV_HYBRID_EFF(3,1)=0.283276
         SV_HYBRID_EFF(4,1)=0.0134133
         SV_HYBRID_EFF(5,1)=0.0314201
         SV_HYBRID_EFF(6,1)=0.
         SV_HYBRID_EFF(7,1)=0.0788762
         SV_HYBRID_EFF(8,1)=0.242347
         SV_HYBRID_EFF(9,1)=0.562374
         SV_HYBRID_EFF(10,1)=0.10112
         SV_HYBRID_EFF(11,1)=0.159645
         SV_HYBRID_EFF(12,1)=0.0683994
         SV_HYBRID_EFF(13,1)=0.0914961
         SV_HYBRID_EFF(14,1)=0.0291721
         SV_HYBRID_EFF(15,1)=0.306947
         SV_HYBRID_EFF(16,1)=0.0898389
         SV_HYBRID_EFF(17,1)=0.321846
         SV_HYBRID_EFF(18,1)=0.780131
         SV_HYBRID_EFF(19,1)=0.00858275
         SV_HYBRID_EFF(20,1)=0.676116
         SV_HYBRID_EFF(21,1)=0.00390625
         SV_HYBRID_EFF(22,1)=0.659885
         SV_HYBRID_EFF(23,1)=0.499842
         SV_HYBRID_EFF(24,1)=0.751365
         SV_HYBRID_EFF(25,1)=0.608158
         SV_HYBRID_EFF(26,1)=0.591767
         SV_HYBRID_EFF(27,1)=0.768072
         SV_HYBRID_EFF(28,1)=0.793788
         SV_HYBRID_EFF(29,1)=0.376446
         SV_HYBRID_EFF(30,1)=0.
         SV_HYBRID_EFF(31,1)=0.602173
         SV_HYBRID_EFF(32,1)=0.828248
         SV_HYBRID_EFF(33,1)=0.
         SV_HYBRID_EFF(34,1)=0.681011
         SV_HYBRID_EFF(35,1)=0.00518018
         SV_HYBRID_EFF(36,1)=0.846781
         SV_HYBRID_EFF(37,1)=0.950193
         SV_HYBRID_EFF(38,1)=0.928512
         SV_HYBRID_EFF(39,1)=0.866793
         SV_HYBRID_EFF(40,1)=0.282671
         SV_HYBRID_EFF(41,1)=0.00479524
         SV_HYBRID_EFF(42,1)=0.661741
         SV_HYBRID_EFF(43,1)=0.942944
         SV_HYBRID_EFF(44,1)=0.926277
         SV_HYBRID_EFF(45,1)=0.851799
         SV_HYBRID_EFF(46,1)=0.767026
         SV_HYBRID_EFF(47,1)=0.92102
         SV_HYBRID_EFF(48,1)=0.921258
         SV_HYBRID_EFF(49,1)=0.929838
         SV_HYBRID_EFF(50,1)=0.919845
         SV_HYBRID_EFF(51,1)=0.923569
         SV_HYBRID_EFF(52,1)=0.947368
         SV_HYBRID_EFF(53,1)=0.915953
         SV_HYBRID_EFF(54,1)=0.431334
         SV_HYBRID_EFF(55,1)=0.93413
         SV_HYBRID_EFF(56,1)=0.935997
         SV_HYBRID_EFF(57,1)=0.904573
         SV_HYBRID_EFF(58,1)=0.943681
         SV_HYBRID_EFF(59,1)=0.0193183
         SV_HYBRID_EFF(60,1)=0.925738
         SV_HYBRID_EFF(61,1)=0.780146
         SV_HYBRID_EFF(62,1)=0.0205252
         SV_HYBRID_EFF(63,1)=0.0824874
         SV_HYBRID_EFF(64,1)=0.0457792
         SV_HYBRID_EFF(65,1)=0.
         SV_HYBRID_EFF(66,1)=0.282209
         SV_HYBRID_EFF(67,1)=0.0464643
         SV_HYBRID_EFF(68,1)=0.0299661
         SV_HYBRID_EFF(69,1)=0.284956
         SV_HYBRID_EFF(70,1)=0.286349
         SV_HYBRID_EFF(71,1)=0.169939
         SV_HYBRID_EFF(72,1)=0.177829
         SV_HYBRID_EFF(73,1)=0.0803865
         SV_HYBRID_EFF(74,1)=0.214579
         SV_HYBRID_EFF(75,1)=0.215743
         SV_HYBRID_EFF(76,1)=0.421403
         SV_HYBRID_EFF(77,1)=0.783037
         SV_HYBRID_EFF(78,1)=0.116111
         SV_HYBRID_EFF(79,1)=0.813632
         SV_HYBRID_EFF(80,1)=0.848807
         SV_HYBRID_EFF(81,1)=0.669791
         SV_HYBRID_EFF(82,1)=0.719486
         SV_HYBRID_EFF(83,1)=0.317905
         SV_HYBRID_EFF(84,1)=0.828389
         SV_HYBRID_EFF(85,1)=0.783754
         SV_HYBRID_EFF(86,1)=0.791546
         SV_HYBRID_EFF(87,1)=0.610419
         SV_HYBRID_EFF(88,1)=0.648211
         SV_HYBRID_EFF(89,1)=0.576329
         SV_HYBRID_EFF(90,1)=0.927147
         SV_HYBRID_EFF(91,1)=0.846652
         SV_HYBRID_EFF(92,1)=0.924833
         SV_HYBRID_EFF(93,1)=0.457421
         SV_HYBRID_EFF(94,1)=0.931418
         SV_HYBRID_EFF(95,1)=0.830341
         SV_HYBRID_EFF(96,1)=0.919431
         SV_HYBRID_EFF(97,1)=0.
         SV_HYBRID_EFF(98,1)=0.956678
         SV_HYBRID_EFF(99,1)=0.934762
         SV_HYBRID_EFF(100,1)=0.952644
         SV_HYBRID_EFF(101,1)=0.933812
         SV_HYBRID_EFF(102,1)=0.912032
         SV_HYBRID_EFF(103,1)=0.
         SV_HYBRID_EFF(104,1)=0.86957
         SV_HYBRID_EFF(105,1)=0.91708
         SV_HYBRID_EFF(106,1)=0.733937
         SV_HYBRID_EFF(107,1)=0.941615
         SV_HYBRID_EFF(108,1)=0.712089
         SV_HYBRID_EFF(109,1)=0.941143
         SV_HYBRID_EFF(110,1)=0.939167
         SV_HYBRID_EFF(111,1)=0.699369
         SV_HYBRID_EFF(112,1)=0.92284
         SV_HYBRID_EFF(113,1)=0.918018
         SV_HYBRID_EFF(114,1)=0.709193
         SV_HYBRID_EFF(115,1)=0.920673
         SV_HYBRID_EFF(116,1)=0.0120507
         SV_HYBRID_EFF(117,1)=0.933684
         SV_HYBRID_EFF(118,1)=0.00968972
         SV_HYBRID_EFF(119,1)=0.806888
         SV_HYBRID_EFF(120,1)=0.947459
         SV_HYBRID_EFF(121,1)=0.0465139
         SV_HYBRID_EFF(122,1)=0.942085
C Z side **************************************8
         SV_HYBRID_EFF(1,2)=0.82454
         SV_HYBRID_EFF(2,2)=0.533843
         SV_HYBRID_EFF(3,2)=0.862591
         SV_HYBRID_EFF(4,2)=0.620652
         SV_HYBRID_EFF(5,2)=0.907118
         SV_HYBRID_EFF(6,2)=0.
         SV_HYBRID_EFF(7,2)=0.896088
         SV_HYBRID_EFF(8,2)=0.941245
         SV_HYBRID_EFF(9,2)=0.935701
         SV_HYBRID_EFF(10,2)=0.948968
         SV_HYBRID_EFF(11,2)=0.943153
         SV_HYBRID_EFF(12,2)=0.943176
         SV_HYBRID_EFF(13,2)=0.921304
         SV_HYBRID_EFF(14,2)=0.933428
         SV_HYBRID_EFF(15,2)=0.920268
         SV_HYBRID_EFF(16,2)=0.940869
         SV_HYBRID_EFF(17,2)=0.950195
         SV_HYBRID_EFF(18,2)=0.767965
         SV_HYBRID_EFF(19,2)=0.830271
         SV_HYBRID_EFF(20,2)=0.938678
         SV_HYBRID_EFF(21,2)=0.0604131
         SV_HYBRID_EFF(22,2)=0.935362
         SV_HYBRID_EFF(23,2)=0.792341
         SV_HYBRID_EFF(24,2)=0.875171
         SV_HYBRID_EFF(25,2)=0.891245
         SV_HYBRID_EFF(26,2)=0.803006
         SV_HYBRID_EFF(27,2)=0.938975
         SV_HYBRID_EFF(28,2)=0.930884
         SV_HYBRID_EFF(29,2)=0.939244
         SV_HYBRID_EFF(30,2)=0.78574
         SV_HYBRID_EFF(31,2)=0.93278
         SV_HYBRID_EFF(32,2)=0.589986
         SV_HYBRID_EFF(33,2)=0.
         SV_HYBRID_EFF(34,2)=0.
         SV_HYBRID_EFF(35,2)=0.332601
         SV_HYBRID_EFF(36,2)=0.94396
         SV_HYBRID_EFF(37,2)=0.945638
         SV_HYBRID_EFF(38,2)=0.923826
         SV_HYBRID_EFF(39,2)=0.893851
         SV_HYBRID_EFF(40,2)=0.930278
         SV_HYBRID_EFF(41,2)=0.851923
         SV_HYBRID_EFF(42,2)=0.918864
         SV_HYBRID_EFF(43,2)=0.87311
         SV_HYBRID_EFF(44,2)=0.929619
         SV_HYBRID_EFF(45,2)=0.942403
         SV_HYBRID_EFF(46,2)=0.941171
         SV_HYBRID_EFF(47,2)=0.932426
         SV_HYBRID_EFF(48,2)=0.938355
         SV_HYBRID_EFF(49,2)=0.945199
         SV_HYBRID_EFF(50,2)=0.941
         SV_HYBRID_EFF(51,2)=0.929183
         SV_HYBRID_EFF(52,2)=0.934737
         SV_HYBRID_EFF(53,2)=0.949914
         SV_HYBRID_EFF(54,2)=0.889723
         SV_HYBRID_EFF(55,2)=0.912419
         SV_HYBRID_EFF(56,2)=0.924582
         SV_HYBRID_EFF(57,2)=0.883028
         SV_HYBRID_EFF(58,2)=0.924562
         SV_HYBRID_EFF(59,2)=0.147972
         SV_HYBRID_EFF(60,2)=0.92349
         SV_HYBRID_EFF(61,2)=0.909562
         SV_HYBRID_EFF(62,2)=0.52955
         SV_HYBRID_EFF(63,2)=0.935798
         SV_HYBRID_EFF(64,2)=0.900443
         SV_HYBRID_EFF(65,2)=0.
         SV_HYBRID_EFF(66,2)=0.935641
         SV_HYBRID_EFF(67,2)=0.797186
         SV_HYBRID_EFF(68,2)=0.929457
         SV_HYBRID_EFF(69,2)=0.93051
         SV_HYBRID_EFF(70,2)=0.908814
         SV_HYBRID_EFF(71,2)=0.903729
         SV_HYBRID_EFF(72,2)=0.922546
         SV_HYBRID_EFF(73,2)=0.938724
         SV_HYBRID_EFF(74,2)=0.933776
         SV_HYBRID_EFF(75,2)=0.915307
         SV_HYBRID_EFF(76,2)=0.933002
         SV_HYBRID_EFF(77,2)=0.93207
         SV_HYBRID_EFF(78,2)=0.865049
         SV_HYBRID_EFF(79,2)=0.896611
         SV_HYBRID_EFF(80,2)=0.762082
         SV_HYBRID_EFF(81,2)=0.920371
         SV_HYBRID_EFF(82,2)=0.945129
         SV_HYBRID_EFF(83,2)=0.984077
         SV_HYBRID_EFF(84,2)=0.920305
         SV_HYBRID_EFF(85,2)=0.734813
         SV_HYBRID_EFF(86,2)=0.935543
         SV_HYBRID_EFF(87,2)=0.936417
         SV_HYBRID_EFF(88,2)=0.945093
         SV_HYBRID_EFF(89,2)=0.735206
         SV_HYBRID_EFF(90,2)=0.936103
         SV_HYBRID_EFF(91,2)=0.914028
         SV_HYBRID_EFF(92,2)=0.933968
         SV_HYBRID_EFF(93,2)=0.923874
         SV_HYBRID_EFF(94,2)=0.94473
         SV_HYBRID_EFF(95,2)=0.918598
         SV_HYBRID_EFF(96,2)=0.921833
         SV_HYBRID_EFF(97,2)=0.
         SV_HYBRID_EFF(98,2)=0.94266
         SV_HYBRID_EFF(99,2)=0.947344
         SV_HYBRID_EFF(100,2)=0.947022
         SV_HYBRID_EFF(101,2)=0.932773
         SV_HYBRID_EFF(102,2)=0.894379
         SV_HYBRID_EFF(103,2)=0.
         SV_HYBRID_EFF(104,2)=0.926758
         SV_HYBRID_EFF(105,2)=0.903659
         SV_HYBRID_EFF(106,2)=0.933975
         SV_HYBRID_EFF(107,2)=0.925574
         SV_HYBRID_EFF(108,2)=0.598693
         SV_HYBRID_EFF(109,2)=0.92941
         SV_HYBRID_EFF(110,2)=0.932966
         SV_HYBRID_EFF(111,2)=0.926081
         SV_HYBRID_EFF(112,2)=0.93885
         SV_HYBRID_EFF(113,2)=0.764537
         SV_HYBRID_EFF(114,2)=0.507955
         SV_HYBRID_EFF(115,2)=0.908846
         SV_HYBRID_EFF(116,2)=0.0304384
         SV_HYBRID_EFF(117,2)=0.917711
         SV_HYBRID_EFF(118,2)=0.398928
         SV_HYBRID_EFF(119,2)=0.932484
         SV_HYBRID_EFF(120,2)=0.68607
         SV_HYBRID_EFF(121,2)=0.803791
         SV_HYBRID_EFF(122,2)=0.89297
      ENDIF

      RETURN
      END


