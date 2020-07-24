*
*     $Id: cdcorf.F,v 1.6 2003/03/05 17:46:55 dpp Exp $
*
* $Log: cdcorf.F,v $
* Revision 1.6  2003/03/05 17:46:55  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.5  2001/11/19 23:47:33  dpp
*      -> remove diagdoit (old duet diagnostics) routines
*      -> delete unused include file
*
* Revision 1.4  1999/11/29 21:43:45  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.3  1999/05/28 03:04:52  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.2  1998/07/09 00:39:33  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.8  1997/03/19 18:04:10  clib
* Generalized layer references for radial wire displacement for
* CLEO2/CLEO2.5 compatibility.
*
* Revision 1.7  1997/02/06 12:40:31  clib
* Monte Carlo geometry corrections.
*
* Revision 1.6  1996/12/04 10:11:39  clib
* Truncation at large distance changed, moved to dfcorr.F
*
* Revision 1.4.2.1  1996/06/21 10:13:42  clib
* Updates from Ken for recompress.
*
* Revision 1.5  1996/06/21 08:31:11  clib
* Radial displacements for stereo layers and CD6,7,16 included.
*
* Revision 1.4  1996/05/03 21:09:41  zfiles
* Calls STCORR if control parameters (ZDTGL) enables it.
* STCORR treated as a timelike variable.
*
* Revision 1.3  1996/02/15 13:57:04  zfiles
* Bug fixes from Ken
*
* Revision 1.2  1996/02/08 00:29:30  zfiles
* Mods from Ken. Works with new and old calibration
*
* Revision 1.1.1.1  1994/10/08  01:00:53  zfiles
* first version of duet in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/32 08/08/94  17.56.18  by  Alain Bellerive
*CMZ :  6.00/30 15/06/94  20.55.46  by  Unknown
*CMZ :  6.00/27 18/05/94  19.03.13  by  Anders Ryd
*CMZ :  6.00/24 08/05/94  17.38.41  by  Anders Ryd
*CMZ :  6.00/18 22/03/94  15.26.49  by  Dan Peterson
*CMZ :  5.54/03 06/05/91  10.29.26  by  Steve Schaffner
*CMZ :  5.54/00 31/01/91  14.59.28  by  Steve Schaffner
*CMZ :  5.53/07 25/09/90  10.17.00  by  D. Riley
*CMZ :  5.53/04 25/06/90  12.17.24  by  Steve Schaffner
*CMZ :  5.53/03 16/05/90  19.09.37  by  Steve Schaffner
*CMZ :          10/05/90  13.52.20  by  Steve Schaffner
*CMZ :  5.53/01 23/04/90  13.29.32  by  Steve Schaffner
*CMZ :  5.52/01 22/03/90  14.40.00  by  Steve Schaffner
*CMZ :  5.52/00 07/03/90  11.23.51  by  Steve Schaffner
*CMZ :  5.51/04 08/12/89  21.32.04  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.09  by  Steve Schaffner
*CMZ :  5.50/00 10/04/89  11.22.35  by  Steve Schaffner
*Remove call to ASIN0 (now called by ASIN1)
*CMZ :          13/09/88  13.35.51  by  Dan Riley
* if computed drift distance is outside of the cell, set it to the edge,
* not 0.
*-- Author :
      SUBROUTINE CDCORF( UFXYHT )
C.......................................................................
C.
C. CDCORF -  Correct drift distance and hit position for hits in fit
C.
C. COMMON    : ?
C. CALLS     : CDCFID DFOFPS TCR2CL UFXYHT
C.             DCORSZ CORSZ PHCORR ZTCOR
C. CALLED    : DUFIND
C. AUTHOR    : R. Namjoshi
C.
C. VERSION   : 1.08
C. CREATED   : 07-Dec-86
C. LAST MOD  : 13-Sep-88
C.
C. Modification Log.
C.
C.  2-Feb-96 : RCORCD,TCORPH have been obsolete for years, calls removed
C.             even as comments. Mechanisms to toggle between new and
C.             old drift functions and corrections introduced. KMCL
C.
C. 10-Apr-87 : Do not pass YSAG to UFXYHT for CDCORF. This avoids a
C.             double correction for it.            R. Namjoshi
C.
C. 14-Apr-87 : Protect against ABS(SINT) > 1.0      R. Namjoshi
C.
C. 19-Apr-87 : Change sign of SINT, new convention. R. Namjoshi
C.
C. 11-May-87 : Implement RCORCD correction.         R. Namjoshi
C.
C. 27-Jul-87 : Implement TCORPH correction.         R. Namjoshi
C.
C. 17-Dec-87 : Pass YSAG to UFXYHT, do YSAG correction to EvntHit_Y after
C.             the call. -dsr
C. 25-Feb-88 : use +SEQ,TFTIME.                     R. Namjoshi
C.
C. 13-Sep-88 : Don't set drift times outside the cell to 0. dsr
C.
C.  7-May-93 : use ABS(IPLFIT) instead of IPLFIT and
C.             save local KAPSAV etc version of initial KAPCF which
C.             allows changing sign of KAPCF with  USEFIT to
C.             allow compatability with latest version of TF3FIT
C.                                                  D. Peterson
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/utilseq/cdcalpa.inc"
#include "doit/utilseq/cdcalin.inc"
#include "cdoffcal/cdseq/cdtggl.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdcorrcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tffitctl.inc"

C== Externals

      REAL     DFOFPS,TCR2CL,PHCORR,CORSZ,DCORSZ,STCORR,DFCORR
      EXTERNAL DFOFPS,TCR2CL,PHCORR,CORSZ,DCORSZ,STCORR,DFCORR
      EXTERNAL UFXYHT

C== Local variables

      REAL KAPSAV,ALPSAV,BETSAV,GAMSAV,TANSAV,D1,D2,D3,D4
      REAL CURSAV,PH1SAV,PH2SAV,D0SAV
      REAL TRACK(5),ARCL,DWMAX,ZW,YSAG,SINT,TDRFT,ZZ,
     &PHIW,RLYR,CFRAC,DDRFT,DTDZ,AMB
      INTEGER ILLYR,ILYR,ILCD,IDEV,IADLST,I,IAMB,JAMB,IADR,IERR,IWIRE

      integer where
* ----------Executable code starts here---------------------

C delete very large TANDLF, long ago labeled temporary, but still in use
      IF( ABS(TANDLF).GT.5.0 ) TANDLF=0.0

      CALL CDCFID( TRACK(1), TRACK(2), TRACK(3),
     +                   KAPCF, ALPCF, BETCF, GAMCF, XICF )
      TRACK(4) = TANDLF
      TRACK(5) = Y0LF

      IF(LCALIB)THEN
          KAPSAV = KAPCF
          ALPSAV = ALPCF
          BETSAV = BETCF
          GAMSAV = GAMCF
          TANSAV = TANDLF
          CURSAV = TRACK(1)
          PH1SAV = TRACK(2)
          PH2SAV=PH1SAV+PI
          IF(PH2SAV.GT.TWOPI)PH2SAV=PH2SAV-TWOPI
          D0SAV  = TRACK(3)
      ENDIF

C Correct hits in fit only

      DO 40 I = 1, NFIT
          IAMB   =      IADFIT(I)
          IADR   = IABS(IADFIT(I))
          JAMB   = ISIGN(1,IAMB)
          AMB    = FLOAT(JAMB)
          ILYR   = IABS(IPLFIT(I))
          ILCD   = ILTFCD(ILYR)
          IDEV   = IDVCTF(ILYR)

C flip the track parameters for subtrack 2 in calibration
          IF(LCALIB)THEN
              IF     (IABS(USEFIT(I)).EQ.1)THEN
                  KAPCF = KAPSAV
                  ALPCF = ALPSAV
                  BETCF = BETSAV
                  GAMCF = GAMSAV
                  TANDLF= TANSAV
                  TRACK(1) = CURSAV
                  TRACK(2) = PH1SAV
                  TRACK(3) = D0SAV
                  TRACK(4) = TANSAV
              ELSE IF(IABS(USEFIT(I)).EQ.2)THEN
                  KAPCF =-KAPSAV
                  ALPCF =-ALPSAV
                  BETCF =-BETSAV
                  GAMCF =-GAMSAV
                  TANDLF=-TANSAV
                  TRACK(1) =-CURSAV
                  TRACK(2) = PH2SAV
                  TRACK(3) =-D0SAV
                  TRACK(4) =-TANSAV
              ELSE
                  GO TO 40
              ENDIF
          ENDIF

          IF( IDEV.EQ.IIVDT .OR. IDEV.EQ.IVDET .OR. IDEV.EQ.IDRFT
     +                           .OR. IDEV.EQ.IPTLA )THEN

              ARCL   = SFIT(I)
C              DWMAX  = 0.5*SQRT(2.)*CELLTF(ILYR) * RTF(ILYR)
              DWMAX  = DMAXCD(ILCD)
              ZW    = TANDLF*SFIT(I) + Y0LF
              YSAG  = -BSAGCD(ILCD)*(1. - (ZW/ZENDTF(ILYR))**2)
              SINT  = (KAPCF*RSQTF(ILYR)-GAMCF)/RTF(ILYR)
              IF( ABS(SINT).GT.1.0 ) SINT = SIGN( 1.0, SINT )


C--- Swap the VD east wire numbers into the east locations if 
C--- needed - Alainb

              IWIRE = EvntHit_laWire(IADR)
              IF (LUSVDE .AND. IDVCCD(ILCD).EQ.IVDET) THEN
                IF (IWIRE.LT.NWIRCD(ILCD)) THEN
                  IWIRE = IWIRE + NWIRCD(ILCD)
                ELSE
                  IWIRE = IWIRE - NWIRCD(ILCD)
                ENDIF
              ENDIF

C--- Arclength flight and wire propagation corrections


c              TDRFT = TCR2CL(EvntHit_rawTIM(IADR),
c     1                 ILCD,IWIRE,TRACK,AMPI2,ARCL,ZW)
C-A Lyon-5-24-99              TDRFT = TCR2CL(EvntHit_rawTIM(IADR),
c     1                 ILCD,TRACK,AMPI2,ARCL,ZW)


C==============================================================================
C Time Corrections 9/1/96 KMCL
C                  2/2/96 KMCL Introduce toggles
              EvntHit_corTIM(IADR) = TDRFT

C==============================================================================
c              DFIT(I)    = DFCORR(TDRFT,ILCD,ZW,SINT,JAMB)   ! signed 
C 30 microns/nanosecond; 3E-8 meters/ps 
              DFIT(I)=3.E-8 * TDRFT * AMB                     ! signed
              EvntHit_Drift(IADR) = ABS(DFIT(I))              ! unsigned
C==============================================================================

              ZZ      = ZW - XX0TF(3,ILYR)
              PHIW    = EvntHit_PhiWir(IADR)
              RLYR    = RTF(ILYR)   ! + RCORCD( ZZ, ILCD )
                                    ! THIS HAS BEEN =0 SINCE 1989! KMCL 2/2/96 
              EvntHit_X(IADR) = RLYR*COS(PHIW)+XX0TF(1,ILYR)+
     +                     ZZ*UU0TF(1,ILYR)
              EvntHit_Y(IADR) = RLYR*SIN(PHIW)+XX0TF(2,ILYR)+
     +                     ZZ*UU0TF(2,ILYR)

              CALL UFXYHT( IAMB, ILYR, YSAG, XFIT(I), YFIT(I), IERR )
              EvntHit_Y(IADR) = EvntHit_Y(IADR) + YSAG
              IF(.NOT.LMCRLO)THEN


C PIH         radial displacement correction
C             This is seen in CLEO2.5 as well, so seems
C             to be a real effect.  Make default for CLEO2.5
C             by checking if the SVX exists.
              IF( (ZDTGL(IDEV).EQ.5).OR.(IDTOCD(ISVX1).NE.0) )THEN
                IF( LSTLCD(ILCD) ) THEN ! A Stereo layer
                  D4 = -321.E-06 
                ELSEIF(IDEV.EQ.IPTLA.AND.ILCD.EQ.6)THEN
                  D4 = -157.E-06
                ELSEIF(IDEV.EQ.IVDET.AND.ILCD.EQ.ILDVCD(1,IDEV))THEN
                  D4 = +159.E-06
                ELSEIF(IDEV.EQ.IVDET.AND.ILCD.EQ.ILDVCD(2,IDEV))THEN
                  D4 = -287.E-06
                ELSE 
                  D4 = 0.0
                ENDIF

              ELSEIF(ZDTGL(IDEV).EQ.4.AND.LSTLCD(ILCD))THEN
                D1 = ZW / ZENDTF(ILYR)
                D2 = -375.E-06 
                D3 = 1.0 - 1.5*D1**2 + 0.5*D1**4
                D4 = D2*D3
              ELSE
                D4 = 0
              ENDIF

              EvntHit_X(IADR) = EvntHit_X(IADR) - D4*COS(PHIW)
              EvntHit_Y(IADR) = EvntHit_Y(IADR) - D4*SIN(PHIW)
              ENDIF
          ELSEIF( IDEV.EQ.IPROP ) THEN
              XFIT(I) = EvntHit_X(IADR)
              YFIT(I) = EvntHit_Y(IADR)
          ELSEIF( IDEV.EQ.IVDCT .OR. IDEV.EQ.IDRCT ) THEN

              CALL UFXYHT( IAMB, ILYR, 0.0, XFIT(I), YFIT(I), IERR )
              SFIT(I) = XFIT(I)
          ELSEIF (IDEV.EQ.ISVX1) THEN
              CALL UFXYHT(IAMB,ILYR,1.0,XFIT(I),YFIT(I),IERR)
CRYD          SFIT(I)=RCD(ILYR)
              SFIT(I)=SQRT(XFIT(I)**2+YFIT(I)**2)
          ELSEIF (IDEV.EQ.ISVZ1) THEN
              CALL UFXYHT(IAMB,ILYR,0.0,SFIT(I),DFIT(I),IERR)
          ENDIF
40    CONTINUE
C restore the initial values
          IF(LCALIB)THEN
                  KAPCF = KAPSAV
                  ALPCF = ALPSAV
                  BETCF = BETSAV
                  GAMCF = GAMSAV
                  TANDLF= TANSAV
                  TRACK(1) = CURSAV
                  TRACK(2) = PH1SAV
                  TRACK(3) = D0SAV
                  TRACK(4) = TANSAV
          ENDIF
      RETURN
      END
