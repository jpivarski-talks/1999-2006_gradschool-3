*
* $Id: cdjoin.F,v 1.1 2003/03/05 17:01:59 dpp Exp $
*
* $Log: cdjoin.F,v $
* Revision 1.1  2003/03/05 17:01:59  dpp
*      -> NEW ROUTINE, moved from "cdtrack/."
*
* Revision 1.6  2001/11/19 22:44:11  dpp
*      -> use status bit encapsulation
*      -> comment regarding NHITCF, outdated use
*      -> test TF3FIT with NHITTF rather than NHIT3F
*
* Revision 1.5  2001/09/12 22:06:05  dpp
*      -> change statcf assignments
*
* Revision 1.4  2000/10/02 21:00:27  dpp
*      -> change statcf(3) -> (4)
*      -> change statcf(1) -> (3)
*
* Revision 1.3  1999/09/12 22:31:23  dpp
* remove argument in call to CFCFIT
*
* Revision 1.2  1999/05/28 03:04:40  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:20  clib
* Developmental version of DUET.
*
* Revision 1.8  1996/05/10 13:05:24  zfiles
* Flag track before entry to TF3FIT.
* Fill NHITTF, CHITF after fit.
*
* Revision 1.7  1996/04/23 22:54:09  zfiles
* Comments in previous version are wrong!
* True comment:
*      Remove call to KLUSTK.
*
* Revision 1.6  1996/04/23 07:24:13  zfiles
* Lots of updates from Rob.
*
* Revision 1.5  1996/03/12 14:46:59  zfiles
* Deleted tracks now have KINCD=-20
*
* Revision 1.4  1996/01/25 16:26:08  zfiles
* 4 status bit words
*
* Revision 1.3  1995/09/05 13:33:49  zfiles
*  Update kalman stack extention after TF3FIT called.
*
* Revision 1.2  1995/06/19  20:43:44  zfiles
* Added Kalman filter stuff from Rob
*
* Revision 1.1.1.1  1994/10/08  01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/30 19/11/92  19.53.02  by  Rob Kutschke
*CMZ :  5.54/18 19/10/92  15.49.36  by  Rob Kutschke
*CMZ :  5.54/17 09/10/92  00.54.40  by  Rob Kutschke
*CMZ :  5.54/14 28/09/92  20.29.22  by  Rob Kutschke
*CMZ :  5.51/00 09/08/89  15.33.14  by  Steve Schaffner
*CMZ :  5.50/00 16/10/88  20.42.53  by  Dan Riley
*-- Author :    Dan Riley   12/10/88
      SUBROUTINE CDJOIN(IPNTR, LPURGE, INDKP, INDEL)
C.......................................................................
C.
C. CDJOIN - Join the two halves of curling tracks into one track.
C.
C. COMMON    : ?
C. CALLS     : ?
C. CALLED    : DUET
C. AUTHOR    : M. Ogg, rewritten by D. Riley
C. 
C. VERSION   : 1.10
C. CREATED   : ?
C. LAST MOD  : 12-Oct-88
C.
C. Modification Log.
C.
C. 12-Oct-88 : Rewritten in F77 and extensively revised.  -D. Riley
C. 29-May-95 : Use KINCD rather than LPURGE to indicate that a track is
C.             to be deleted.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/statusbits.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfstakcd.inc"

      EXTERNAL CFXYHT

      INTEGER  IPNTR(MTRKCD)
      LOGICAL  LPURGE(MTRKCD)
      INTEGER INDKP,ITRK,INDEL,IBRNCH
      REAL DCH2SV

C A place to save the fit parameters in case TF3FIT fails.
      REAL KAPSAV(NPARTF), SFITSV(KLR2TF), WGTFSV(KLR2TF)
      INTEGER STATTM(4)

* ----------Executable code starts here---------------------

C mark the input tracks for deletion.
      KINCD(IPNTR(INDKP)) = KINCD(IPNTR(INDKP))-20
      KINCD(IPNTR(INDEL)) = KINCD(IPNTR(INDEL))-20

C fit one track to all the points
      ITRK   = IPNTR(INDKP)
      CALL CDKABG(CURCD(ITRK),PHI0CD(ITRK),D0CD(ITRK),
     2            KAPCF,ALPCF,BETCF,GAMCF,XICF)
      TANDLF = TANDCD(ITRK)
      Y0LF   = Z0CD(ITRK)

C 10/23/2001, dpp,  This routine includes outdated use of NHITCF.
C   It fills NHITCF directly rather than relying on fitter. 
C   NHITCF is zeroed here and incremented in CDFILL.
C   CDJOIN is the only known caller of CDFILL.


C fill /TFTRACK/ with first half of track

      NHITLF = 0
      NHITCF = 0
      NHTQLF = 0
      NHITTF = 0
      NFIT   = 0
      IBRNCH = 1
      CALL CDFILL(ITRK, IBRNCH)

C fill /TFTRACK/ with second half of track

      IBRNCH = -1
      KAPCF  = -KAPCF
      ALPCF  = -ALPCF
      BETCF  = -BETCF
      GAMCF  = -GAMCF
      ITRK   = IPNTR(INDEL)
      CALL CDFILL(ITRK, IBRNCH)

C fit the track

      KAPCF  = -KAPCF
      ALPCF  = -ALPCF
      BETCF  = -BETCF
      GAMCF  = -GAMCF
      CHILF  =  1.
      CHICF  =  1.
      CHITF  =  1.
      NHITTF = NFIT

C Save the status bits from the old front half
      CALL UCOPY(STATCD(1,ITRK),STATCF,4)

C Enough z information to do the fit?
      IF ((NHITLF+NHTQLF).LE.MNHTLF) GOTO 800
      DCH2SV =  DCH2TF
      DCH2TF =  20.

C Save r-phi hits, in case TF3FIT fails.
      NFITSV = NFIT
      CALL UCOPY( IPLFIT, IPLFSV, NFIT )
      CALL UCOPY( IADFIT, IADFSV, NFIT )
      CALL UCOPY(   SFIT, SFITSV, NFIT )
      CALL UCOPY( WGTFIT, WGTFSV, NFIT )
      CALL UCOPY( KAPCF,  KAPSAV, NPARTF )

C Clear TF3FIT part of status words ( will be refilled by TF3FIT ).
      CALL STATUSBITS(kSTfillcf,kstCDJjoin,0)
      CALL STATUSBITS(kSTfillcf,kstCDJdied,0)
      CALL STATUSBITS(kSTfillcf,kstTF3ent,0)
      CALL STATUSBITS(kSTfillcf,kstTF3knk1allow,0)
      CALL STATUSBITS(kSTfillcf,kstTF3knk1kill,0)
      CALL STATUSBITS(kSTfillcf,kstTF3stLOOPit,0)
      CALL STATUSBITS(kSTfillcf,kstTF3knk2activ,0)
      CALL STATUSBITS(kSTfillcf,kstTF3knk2kill,0)
      CALL STATUSBITS(kSTfillcf,kstTF3notCONV,0)
      CALL STATUSBITS(kSTfillcf,kstTF3yesCONV,0)
      CALL STATUSBITS(kSTfillcf,kstTF3hitsdel,0)
      CALL STATUSBITS(kSTfillcf,kstTF3iter,0)
      CALL STATUSBITS(kSTfillcf,kstTF3err,0)

      CALL TF3FIT
      DCH2TF =  DCH2SV
C Clear fourth status word ( will be refilled by KLMNFT ).
C Then tell KLMNFT that this is coming from CDJOIN.
      STATCF(4) = 0
      CALL SBIT1(STATCF(4),19)


C TF3FIT succeeded.
      IF (NHITTF .GT. 0) THEN
         GOTO 900
      ENDIF

C TF3FIT failed.  Prepare to redo the circle fit.
C NHITLF and NHTQLF must also be zeroed for failed TF3FIT.
C In this case it is done below.
      CALL STATUSBITS(kSTfillcf,kstCDJdied,1)
      CALL UCOPY(STATCF,STATTM,4)
      CALL UCOPY ( KAPSAV, KAPCF, NPARTF )
      CALL UCOPY(STATTM,STATCF,4)
      NFIT   = NHITCF
      NHITTF = NFIT
      CHITF  = CHICF
      CALL UCOPY( IADFSV, IADFIT, NFIT )
      CALL UCOPY( IPLFSV, IPLFIT, NFIT )
      CALL UCOPY( SFITSV,   SFIT, NFIT )
      CALL UCOPY( WGTFSV, WGTFIT, NFIT )

C Not enough z information, or failed TF3FIT.  Just do a circle fit.
 800  CONTINUE
      PHI6CF = 0
      PHI7CF = 0
      RKN7CF = 9.92
      NHITLF = 0
      NHTQLF = 0
C-A Lyon-5-24-99      CALL CDCORF(CFXYHT)
      CALL CFCFIT

C Set the status bit saying that this track is from CDJOIN
 900  CALL STATUSBITS(kSTfillcf,kstCDJjoin,1)

C Add the track to the list.
      NTRACK = NTRACK + 1
      CALL CDNEWT

      END



