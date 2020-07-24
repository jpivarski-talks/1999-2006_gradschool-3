*
* $Id: cdnewt.F,v 1.12 2003/03/05 16:26:24 dpp Exp $
*
* $Log: cdnewt.F,v $
* Revision 1.12  2003/03/05 16:26:24  dpp
*      -> changed all variable names in cdscrtcd to have common root
*      -> use EvntHit_OpnHit to indicate usable hits
*      -> remove references to cdcanh
*
* Revision 1.11  2002/09/05 19:14:22  dpp
*      -> remove NTRTCD, some kind of count of the number of each KINDR
*
* Revision 1.10  2001/11/19 22:43:46  dpp
*      -> remove diagdoit (old duet diagnostics) routines
*      -> delete unused include file
*      -> fill CHI3CD with CHITF rather than CHI3F; always call CDCFID
*         (since NHIT3F was non zero, it was not call CDCFID)
*      -> transfer hits from TF to CD using number=NFIT rather than NHITTF
*      -> delete unused variables
*      -> remove error statistics
*
* Revision 1.9  2000/11/15 16:59:57  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.8  2000/10/24 22:53:39  dpp
*      -> label the hit if it is not to be used in the fit
*
* Revision 1.7  2000/10/02 17:37:32  dpp
* fix hit deletion for silicon
*
* Revision 1.6  2000/08/18 19:13:02  dpp
*      -> remove some obsolete variables
*
* Revision 1.5  2000/07/07 21:57:28  dpp
* require same SV_TYPE to remove sv hit
*
* Revision 1.4  2000/05/26 16:56:04  dpp
*      -> revised for cleo3 silicon
*
* Revision 1.3  2000/04/26 15:03:17  dpp
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*
* Revision 1.2  1999/05/28 03:04:41  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:21  clib
* Developmental version of DUET.
*
* Revision 1.14  1997/02/12 19:16:07  clib
* Mark TKNASV in svahc100.inc.
*
* Revision 1.13  1996/05/21 15:34:35  clib
* Cancel other all z-gangs when one is cancelled.
*
* Revision 1.12  1996/05/10 12:53:03  zfiles
* Changes to allow for QDIV hits to be in the list.
* Do not call SVDPR MODE 2 if it was never called with MODE 1.
* Redefine RESICD to be the RMS of the DR residuals.
*
* Revision 1.11  1996/04/29 16:10:23  zfiles
* Copy MESFIT to MEASCD.
*
* Revision 1.10  1996/04/23 22:24:54  zfiles
* Comments from previous version are wrong!
* True comment:
*      Fill NFITCD, IADFCD, SIG1CD, SIG2CD
*      Remove code that forced a bogus error matrix on
*           KINCD=-1,-3 tracks; they are now properly initialized.
*
* Revision 1.9  1996/04/23 07:24:13  zfiles
* Lots of updates from Rob.
*
* Revision 1.8  1996/03/12 14:29:47  zfiles
* MTRAK1 -> MTRAK2. To allow more internal tracks than external
*
* Revision 1.7  1996/01/25 16:20:17  zfiles
* 4 status bit words
*
* Revision 1.6  1995/11/28 18:46:43  zfiles
* Change arguments to SVDPR, mode 2.
*
* Revision 1.5  1995/08/24  16:42:25  zfiles
* Fixed a typo.
*
* Revision 1.4  1995/08/21  23:49:26  zfiles
* crear previous tracks in the previous events
*
* Revision 1.3  1995/06/19  20:43:49  zfiles
* Added Kalman filter stuff from Rob
*
* Revision 1.2  1995/05/16  18:20:43  zfiles
* New extended common blocks. Limits to MTRAK1 rather than MTRKCD
*
* Revision 1.1.1.1  1994/10/08  01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/11 20/09/93  17.52.28  by  Rob Kutschke
*CMZ :  6.00/04 20/08/93  17.33.37  by  Rob Kutschke
*CMZ :  5.54/14 28/09/92  20.30.37  by  Rob Kutschke
*CMZ :  5.54/03 02/04/91  00.34.18  by  Steve Schaffner
*CMZ :  5.53/07 09/08/90  17.40.31  by  D. Riley
*CMZ :  5.52/00 22/01/90  13.42.43  by  Steve Schaffner
*CMZ :  5.51/00 10/08/89  14.43.26  by  Steve Schaffner
*CMZ :  5.50/00 12/04/89  14.08.41  by  Steve Schaffner
*CMZ :          15/09/88  11.11.23  by  Dan Riley
*-- Author :
      SUBROUTINE CDNEWT
C.......................................................................
C.
C. CDNEWT - Store the current track in /TFHITS/ in /CDTRAK/
C.
C. COMMON    : 
C. Calls     : 
C. Called    : 
C. Author    : D. Cassel, M. Ogg
C.
C. Converted to f77 by Dan Riley  08/08/90  13.34.03
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- SEQUENCE declarations ---------------
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfstakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/svscratch.inc"
#include "doit/duklseq/duklmncd.inc"
#include "doit/duklseq/dutmp1.inc"
#include "seq/clutil/mesrep.inc"

#include "CathodeFortranAccess/cathclus.inc"
#include "CathodeFortranAccess/clustopad.inc"
#include "CathodeFortranAccess/cathpads.inc"
#include "CathodeFortranAccess/padtoclusll.inc"

* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: cdnewt.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.12 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2003/03/05 16:26:24 $')
      PARAMETER(CHRCSA='$Author: dpp $')

      INTEGER INXT,IKIND,IFIT,ILYR,IADR,ABS_ADR,IWADR,IPAD
      INTEGER NDRDGF,LTRMIN,ITRK
      INTEGER NTRLST,IERR,JLYR,SV_HIT

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

C Number of hits used in calculating CHI2/Layer for each device.
C NHTLVD etc have a different meaning and are used in other routines.
      INTEGER NLPT,NLVD,NLDR,NLCT

C Flags to SVDPR
C MODE2 =  2 end of track.
C IHYP  =  0 place holder
      INTEGER MODE2, IHYP
      PARAMETER ( MODE2 = 2 )
      PARAMETER ( IHYP  = 0 )

C Offsets to the other z gang solutions for this hit.
      INTEGER IZGOFF(0:2,2), IZGANG

C Pointer to this hit in SVAH.
      INTEGER INDXAH, SVAHHIT

C Functions called.
      INTEGER JBIT

      LOGICAL LCDNEW, LTRADD
      DATA IZGOFF / 1, -1, -1, 2, 1, -2 /

* ----------Executable code starts here---------------------

C-----------------------------------------------------------------------
C Check for track list overflow
C-----------------------------------------------------------------------
      IF(NTRKCD.LT.MTRAK2)THEN

C-----------------------------------------------------------------------
C LTRADD is set to .true. if there is a track to add.
C-----------------------------------------------------------------------
         LTRADD=.FALSE.
         IF(NTRKCD.LE.0)NTRLST=0
         
C-----------------------------------------------------------------------
C NTRACK is seed number of the current track; incremented after FAST_TRACK 
C NTRKIN is the number if seed tracks read in
C NTRLST is the seed number of the last track passed to CDNEWT
C NTRKCD is the number of distict tracks in the CDNEWT list
C-----------------------------------------------------------------------
C new track
C-----------------------------------------------------------------------
         IF(NTRACK.NE.NTRLST)THEN
            NTRLST=  NTRACK
            NTRKCD=  NTRKCD + 1
            ITRK=    NTRKCD
            LCDNEW=  .TRUE.
            
C-----------------------------------------------------------------------
C save room for all SOLO tracks
C-----------------------------------------------------------------------
            IF(NTRACK.LE.NTRKIN)THEN
               IDRCD(ITRK)=NTRACK
               LTRMIN=     NTRKIN+1

C-----------------------------------------------------------------------
C put duplicates above primaries
C-----------------------------------------------------------------------
            ELSE
               IDRCD(ITRK)= 0
               LTRMIN=      NTRKCD+1
            ENDIF

            INXTCD(ITRK)=  0
            LTRADD=        .TRUE.


C-----------------------------------------------------------------------
C duplicate track
C-----------------------------------------------------------------------
         ELSE

C-----------------------------------------------------------------------
C reject duplicate if no Z-FIT
C-----------------------------------------------------------------------
            IF((NHITLF+NHTQLF).GT.0)THEN

C-----------------------------------------------------------------------
C only if there is room
C-----------------------------------------------------------------------
               IF(LTRKCD.GT.LTRMIN)THEN
                  LTRKCD = LTRKCD - 1
                  ITRK   = LTRKCD
                  INXT   = NTRKCD
                  LCDNEW = .FALSE.
 15               IF (INXTCD(INXT).GT.0) THEN
                     INXT=INXTCD(INXT)
                     GO TO 15
                  ENDIF
                  INXTCD(INXT) = ITRK
                  INXTCD(ITRK) = 0
                  LTRADD = .TRUE.
               ENDIF
            ENDIF
C-----------------------------------------------------------------------
C end of "new track" "duplicate track" condition
C-----------------------------------------------------------------------
         ENDIF

C-----------------------------------------------------------------------
C continue only if there is a track to add
C-----------------------------------------------------------------------
         IF(LTRADD)THEN

C-----------------------------------------------------------------------
C Add the track to the list
C-----------------------------------------------------------------------
            CHI2CD(ITRK) = CHICF
            IF((NHITLF+NHTQLF).GT.0)THEN
               CHIZCD(ITRK)= CHILF
               KINCD(ITRK)=  0
            ELSE
               CHIZCD(ITRK)= -1.0
               KINCD(ITRK)=  -1
            ENDIF
            
            CHI3CD(ITRK)= CHITF
            CALL CDCFID(CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)

            NFITCD(ITRK)= NFIT
            CURCD (ITRK)= CURCF
            PHI0CD(ITRK)= PHI0CF
            D0CD  (ITRK)= D0CF
            PHI6CD(ITRK)= PHI6CF
            TANDCD(ITRK)= TANDLF
            Z0CD  (ITRK)= Y0LF
            PHI7CD(ITRK)= PHI7CF
            RKN7CD(ITRK)= RKN7CF
            KBQDCD(ITRK)= KBQDIV
            NDOFCD(ITRK)= NDOFTF

            CALL UCOPY(STATCF,STATCD(1,ITRK),4)

            XYZ0CD(1,ITRK)= -2.*ALPCF*GAMCF/(XICF*(XICF + 1.))
            XYZ0CD(2,ITRK)= -2.*BETCF*GAMCF/(XICF*(XICF + 1.))
            XYZ0CD(3,ITRK)= Y0LF

            IKIND= KINCD(ITRK)

            CALL VZERO(         IPLRCD(1,ITRK), KLR2TF )
            CALL VZERO(         BUCKCD(1,ITRK), KLR2TF )
            CALL UCOPY( IPLFIT, IPLRCD(1,ITRK), NFIT   )
            CALL UCOPY( IADFIT, IADFCD(1,ITRK), NFIT   )
            CALL UCOPY( PULFIT, PULLCD(1,ITRK), NFIT   )
            CALL UCOPY( SG1FIT, SIG1CD(1,ITRK), NFIT   )
            CALL UCOPY( SG2FIT, SIG2CD(1,ITRK), NFIT   )
            CALL UCOPY( MESFIT, MEASCD(1,ITRK), NFIT   )
            CALL UCOPY( ERMFIT, ERRMCD(1,ITRK), NERMTF )
            CALL VZERO(         IHWRCD(1,ITRK), KLR2TF )

            RESICD(ITRK)= 0.0
            CHI3PT(ITRK)= 0.0
            CHI3VD(ITRK)= 0.0
            CHI3DR(ITRK)= 0.0
            CHI3CT(ITRK)= 0.0
            NHTLVD(ITRK)= 0
            NHTAIZ(ITRK)= 0
            NHTADR(ITRK)= 0
            NHTSCT(ITRK)= 0
            NHTSDR(ITRK)= 0
            NLPT=         0
            NLVD=         0
            NLDR=         0
            NLCT=         0

C-----------------------------------------------------------------------
C calculate some chi square variables
C-----------------------------------------------------------------------
C WGTFIT is not a stack variable - restore it.
C For some sorts of "bad" tracks even SG1FIT will not be
C defined.
C-----------------------------------------------------------------------

            DO 50 IFIT = 1,NFIT
               ILYR = IPLFIT(IFIT)

               IF ( SG1FIT(IFIT) .NE. 0. ) THEN
                  WGTFIT(IFIT) = 1./SG1FIT(IFIT)**2

                  IF ((ILYR .GE. LLYRCF) .AND. (ILYR .LE. MLYRCF)) THEN

                     IF (IDVCTF(ILYR).EQ.IPROP) THEN
                        NHTAIZ(ITRK) = NHTAIZ(ITRK) + 1

                     ELSE IF (IDVCTF(ILYR).EQ.IIVDT) THEN
                        NHTLVD(ITRK) = NHTLVD(ITRK) + 1

                     ELSE IF (IDVCTF(ILYR).EQ.IPTLA) THEN
                        NHTLVD(ITRK) = NHTLVD(ITRK) + 1
                        CHI3PT(ITRK) = CHI3PT(ITRK) +
     +                       PULFIT(IFIT)**2 * WGTFIT(IFIT)
                        NLPT         = NLPT + 1

                     ELSE IF (IDVCTF(ILYR).EQ.IVDET) THEN
                        NHTLVD(ITRK) = NHTLVD(ITRK) + 1
                        CHI3VD(ITRK) = CHI3VD(ITRK) +
     +                       PULFIT(IFIT)**2 * WGTFIT(IFIT)
                        NLVD         = NLVD + 1

                     ELSE IF (IDVCTF(ILYR).EQ.IDRFT) THEN
                        NHTADR(ITRK) = NHTADR(ITRK) + 1
                        RESICD(ITRK) = RESICD(ITRK) + PULFIT(IFIT)**2
                        CHI3DR(ITRK) = CHI3DR(ITRK) +
     +                       PULFIT(IFIT)**2 * WGTFIT(IFIT)
                        NLDR         = NLDR + 1
                     ENDIF

                  ELSE IF ( ILYR .LT. QDIVAD ) THEN

                     IF (IDVCTF(ILYR).EQ.IDRCT) THEN
                        NHTSCT(ITRK) = NHTSCT(ITRK) + 1
                        CHI3CT(ITRK) = CHI3CT(ITRK) +
     +                       PULFIT(IFIT)**2 * WGTFIT(IFIT)
                        NLCT         = NLCT         + 1

                     ELSE IF (IDVCTF(ILYR).EQ.IVDCT) THEN
                        NHTSCT(ITRK) = NHTSCT(ITRK) + 1
                        CHI3CT(ITRK) = CHI3CT(ITRK) +
     +                       PULFIT(IFIT)**2 * WGTFIT(IFIT)
                        NLCT         = NLCT         + 1
                     ELSE IF (IDVCTF(ILYR).EQ.IDRFT) THEN
                        NHTSDR(ITRK) = NHTSDR(ITRK) + 1
                        RESICD(ITRK) = RESICD(ITRK) + PULFIT(IFIT)**2
                        CHI3DR(ITRK) = CHI3DR(ITRK) +
     +                       PULFIT(IFIT)**2 * WGTFIT(IFIT)
                        NLDR         = NLDR + 1
                     ENDIF
                  ENDIF
               ENDIF

C-----------------------------------------------------------------------
C fill the "packed layer and wire" array
C this is used in some old duet routines: 
C    cdtrack/cddupl, 
C    hitsontrak/cdfill (which is only called by /cdtrack/cdjoin)
C    multibunch/duhold
C    diagxts/xts_trak   (which should be changed)              
C    (if there are no more, kill it)
C-----------------------------------------------------------------------
               IADR   = IADFIT(IFIT)
               ABS_ADR=   IABS(IADR)
               IHWRCD(IFIT,ITRK)=ISIGN(
     1              ILYR*IWRPTF+EvntHit_laWire(ABS_ADR),IADR)

C-----------------------------------------------------------------------
C Mark the MAP to show that the hit is used.
C-----------------------------------------------------------------------
               IF (LCDNEW) THEN
                  JLYR = ILYR
                  IF(JLYR.GT.QDIVAD)JLYR=JLYR-QDIVAD
 30               CONTINUE
                  
C-----------------------------------------------------------------------
C SV hits -  (also mark the other z-gang solutions)
C-----------------------------------------------------------------------
                  IF(
     1                 (IDVCTF(JLYR).EQ.ISVX1).OR.
     2                 (IDVCTF(JLYR).EQ.ISVZ1))THEN
                     SV_AVAIL(ABS_ADR)=-IABS(SV_AVAIL(ABS_ADR))
                     DO 35 SV_HIT=1,SV_NHITS
                        IF(SV_HIT.NE.ABS_ADR)THEN
                           IF(
     1                          (
     2                          SV_PARENT(SV_HIT).EQ.SV_PARENT(ABS_ADR))
     2                          .AND.
     3                          (SV_TYPE(SV_HIT).EQ.SV_TYPE(ABS_ADR))
     4                          )THEN
                              SV_AVAIL(SV_HIT)=-IABS(SV_AVAIL(SV_HIT))
                           ENDIF
                        ENDIF
 35                  CONTINUE

C-----------------------------------------------------------------------
C CDSCRTCH hits - cathodes
C-----------------------------------------------------------------------
                  ELSEIF(IDVCTF(JLYR).EQ.IDRCT)THEN

C ABS_ADR is the cathode hit number
C CLUSPADPTR(ABS_ADR) give a location in PADPTR
C PADPTR(CLUSPADPTR(ABS_ADR)) give the number of pad_hits that follow
C PADPTR(CLUSPADPTR(ABS_ADR)+1) give the first of these pad_hits
C PADID give the cellID of a pad_hit 

C while anodes and TPC have been switched to use EvntHit_OpnHit to
C indicate that a hit is still available, cathodes will still 
C use nagative EvntHit_MapHit

                     IPAD=PADID(PADPTR(CLUSPADPTR(ABS_ADR)+1))

C from cfaFillDoitMap.F   EvntHit_MapDet(INDXTF(LYR)-1+IPAD) = id
C does IPAD start at =1, rather than =0  ?  (wires in a layer start at 0)

                     IWADR  = INDXTF(JLYR) -1+IPAD

#if defined(CLEO_ROADHITSDIAG)
                     IF(IDVCTF(ILYR).EQ.IDRCT)THEN
                        PRINT 8886,ITRK,ABS_ADR,CLUSPADPTR(ABS_ADR),
     2                       IPAD,INDXTF(ILYR),IWADR,
     3                      EvntHit_MapDet(IWADR)
 8886                   FORMAT(' CDNEWT:flagging used cathode:',
     2                       ' ITRK=',I3,' ADR=',I3,' CLUSPADPTR=',I5,
     3                       ' PADID=',I5,' INDXTF=',I6,
     4                       ' IWADR=',I6,' _MapDet=',I5)
                     ENDIF
#endif
                     EvntHit_MapDet(IWADR)=-IABS(EvntHit_MapDet(IWADR))

C-----------------------------------------------------------------------
C CDSCRTCH hits - everything else is assumed anodes
C-----------------------------------------------------------------------
                  ELSE

                    IWADR  = INDXTF(JLYR) + EvntHit_laWire(ABS_ADR)

                    EvntHit_OpnHit(ABS_ADR)=-1
c                    EvntHit_MapDet(IWADR)=-IABS(EvntHit_MapDet(IWADR))
                  ENDIF
                  
C-----------------------------------------------------------------------
C loop back for clustered hits
C-----------------------------------------------------------------------
                  ABS_ADR    = EvntHit_NxtCls(ABS_ADR)
                  IF (ABS_ADR.GT.0) GOTO 30
               ENDIF

C-----------------------------------------------------------------------
C label the hit if it is not to be used in the fit
C-----------------------------------------------------------------------

               IF(SRCFIT(IFIT).EQ.P_I_TFXFIT_REDUCE)
     2              BUCKCD(IFIT,ITRK)=1

C-----------------------------------------------------------------------
C end of loop over NFIT
C-----------------------------------------------------------------------
 50         CONTINUE

C-----------------------------------------------------------------------
C finish off the chisqr calculations
C All of the functions of residuals which are computed below
C are done so for historical reasons.  None are really properly
C normalized chi2 distributions - that information is in 
C CHI3CD(ITRK) and NDOFCD(ITRK).

C RESICD was historically defined in a very silly way.
C It is now just the RMS residual of the DR anodes.
C-----------------------------------------------------------------------
            IF ( NHTADR(ITRK)+NHTSDR(ITRK) .GT. 0 ) THEN
               RESICD(ITRK) = SQRT(RESICD(ITRK)/
     +              FLOAT(NHTADR(ITRK)+NHTSDR(ITRK)) )
            ELSE
               RESICD(ITRK) = 0.
            ENDIF

            IF ( NLPT .GT. 0 ) THEN
               CHI3PT(ITRK) = CHI3PT(ITRK)/FLOAT(NLPT)
            ELSE
               CHI3PT(ITRK) = -1.
            ENDIF

            IF ( NLVD .GT. 0 ) THEN
               CHI3VD(ITRK) = CHI3VD(ITRK)/FLOAT(NLVD)
            ELSE
               CHI3VD(ITRK) = -1.
            ENDIF

            IF ( NLDR .GT. 0 ) THEN
               CHI3DR(ITRK) = CHI3DR(ITRK)/FLOAT(NLDR)
            ELSE
               CHI3DR(ITRK) = -1.
            ENDIF

            IF ( NLCT .GT. 0 ) THEN
               CHI3CT(ITRK) = CHI3CT(ITRK)/FLOAT(NLCT)
            ELSE
               CHI3CT(ITRK) = -1.
            ENDIF

C-----------------------------------------------------------------------
C end of condition of having a track to add
C-----------------------------------------------------------------------

#if defined(CLEO_XTSUBS)
            CALL XTS_TRAK('CD',ITRK,'CDNEWT')
#endif

         ENDIF

C-----------------------------------------------------------------------
C Track list overflow
C-----------------------------------------------------------------------
      ELSE

      ENDIF

      RETURN
      END
