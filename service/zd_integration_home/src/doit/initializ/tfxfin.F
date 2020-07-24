*
* $Id: tfxfin.F,v 1.21 2002/09/24 00:29:37 dpp Exp $
*
* $Log: tfxfin.F,v $
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
      SUBROUTINE TFXFIN
C.......................................................................
C.
C. TFXFIN - Initialize cuts for TFXFIT
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
c#include "doit/duseq/cfctlcde.inc"
c#include "doit/duseq/svgeometry.inc"


C PARAMETERS IN TFXFITCD
C..NGRUP..... NUMBER OF AMBIGUITY GROUPS
C..MGLAYR.... MAXIMUM NUMBER OF LAYERS IN A GROUP
C..MGSTOR.... MAXIMUM NUMBER OF HITS IN A LAYER
C..TOTLGR.... PRODUCT GROUP*LAYER
C..TOTHGR.... PRODUCT GROUP*LAYER*HIT
C..NFTMIN.... MINIMUM OF NFIT TO ALLOW CALL TO CFCFIT, OTHERWISE RETURN ERROR
      
* ----------Executable code starts here---------------------

      CALL VZERO(LLAYGR,2*NGRUP)
      CALL VZERO(NLAYGR,2*NGRUP)
      MPASSA(1)=0
      MPASSS(1)=1
      MPASSA(2)=0
      MPASSS(2)=1

      IF (IXLRTF(1,IZDA).GT.0) THEN
        CALL TFXFIN_DR3ZD
      ELSE
        CALL TFXFIN_DR3Si
      ENDIF

C OLD, USED CHICUT=CF2CF1= 3(IN NAMELIST) IN 1ST PASS
C OLD, USED CHICUT=CF2CF2=25(IN NAMELIST) IN 2ND PASS
C CH1CUT NOT USED IN LF

      CHICUT=25.

      RETURN
      END


