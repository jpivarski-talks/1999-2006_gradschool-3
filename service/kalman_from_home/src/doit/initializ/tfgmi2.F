* Urheim - PTLIV1 is declared as integer, treat it as such.
*
* $Id: tfgmi2.F,v 1.5 2001/11/20 00:00:30 dpp Exp $
*
* $Log: tfgmi2.F,v $
* Revision 1.5  2001/11/20 00:00:30  dpp
*      -> remove unused varaiables
*
* Revision 1.4  2000/05/26 16:57:20  dpp
*      -> remove unnecessary common
*
* Revision 1.3  1999/05/28 03:04:58  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.2  1998/07/09 00:51:54  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
* Revision 1.3  1995/06/25 01:22:42  zfiles
* Restore original code for PTLIV1
*
* Revision 1.2  1994/12/06  17:37:20  zfiles
* PTLIV1 is integer, treat it as such. for AIX from J. Urheim
*
* Revision 1.1.1.1  1994/10/08  01:00:55  zfiles
* first version of duet in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/21 28/03/94  14.34.37  by  Rob Kutschke
*CMZ :  6.00/17 16/03/94  14.12.42  by  Rob Kutschke
*CMZ :  6.00/16 14/03/94  17.22.49  by  Rob Kutschke
*CMZ :  5.54/14 16/07/92  17.01.26  by  Dan Peterson
*CMZ :  5.54/00 30/11/90  16.18.34  by  Steve Schaffner
*CMZ :  5.52/01 21/03/90  16.41.31  by  Steve Schaffner
*CMZ :  5.52/00 17/02/90  17.33.05  by  Steve Schaffner
*CMZ :  5.51/04 14/11/89  18.39.51  by  Steve Schaffner
* Convert to get beam position from BPREAD
*CMZ :  5.51/00 11/08/89  14.22.54  by  Steve Schaffner
*CMZ :  5.50/01 23/05/89  17.26.40  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.38  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFGMI2
C.......................................................................
C.
C. TFGMI2 - Update Duet geometry for current run.
C.
C. COMMON    :
C. CALLS     : BPREAD
C. CALLED    : Various places
C. AUTHOR    : R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : 14-Oct-86
C. LAST MOD  : 14-Oct-86
C.
C.
C. Modification Log.
C.
C.......................................................................
C
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cfctlcde.inc"
C#include "doit/duseq/tfxfitcd.inc"
#include "seq/clutil/mesrep.inc"

      INTEGER ILTF,ILCD,IMODE,IERR,IL,IW
      LOGICAL LDEADR,LDEADW
      REAL BMPOSN(12)
      CHARACTER*4 CMODE

* ----------Executable code starts here---------------------

C Get run dependent geometry information

C commented CDGMRU; Now updated automatically (or so they claim)
C      CALL CDGMRU

C Copy from CD to TF arrays

      DO 10 ILTF = 1, NLYRTF
          ILCD = ILTFCD(ILTF)
          PHIFTF(ILTF)  = PHIFCD(ILCD)
          XX0TF(1,ILTF) = XX0CD(1,ILCD)
          XX0TF(2,ILTF) = XX0CD(2,ILCD)
          XX0TF(3,ILTF) = XX0CD(3,ILCD)
          UU0TF(1,ILTF) = UU0CD(1,ILCD)
          UU0TF(2,ILTF) = UU0CD(2,ILCD)
          UU0TF(3,ILTF) = UU0CD(3,ILCD)
10    CONTINUE

C Helix fit parameters

      MP3DTF = 5
      LKN6TF = NLYRVD.GT.0 .AND. XSCATF(2).GT.XSCMIN
      IF( LKN6TF ) MP3DTF = MP3DTF + 1
      LKN7TF = NCANDR.GT.0 .AND. XSCATF(3).GT.XSCMIN
      IF( LKN7TF ) MP3DTF = MP3DTF + 1

C Beam position

C      CALL UCOPY( XXBMCD, XXBMTF, 3 )

C Get beam position

C-A Lyon-5-24-99      IMODE = 0
C-A Lyon-5-24-99      CMODE = 'BEST'
C-A Lyon-5-24-99      CALL BPREAD(BMPOSN,CMODE,IMODE)
C-A Lyon-5-24-99      IF ((CMODE.EQ.'ERRO').OR.(CMODE.EQ.'OOPS')) THEN
C-A Lyon-5-24-99         IERR = 1
C-A Lyon-5-24-99         CHMESS = ' Error in BPREAD'
C-A Lyon-5-24-99         CALL MESLOG('TFGMI2',IERR,MSERRO)
C-A Lyon-5-24-99      END IF
C-A Lyon-5-24-99      CALL UCOPY(BMPOSN,XXBMTF,3)

C PT live wire list



      RETURN
      END





