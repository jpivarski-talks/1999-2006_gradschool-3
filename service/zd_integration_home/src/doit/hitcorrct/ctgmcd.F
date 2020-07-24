*
* $Id: ctgmcd.F,v 1.3 2003/03/05 17:48:36 dpp Exp $
*
* $Log: ctgmcd.F,v $
* Revision 1.3  2003/03/05 17:48:36  dpp
*      -> changed all variable names in cdscrtcd to have common root,comment
*
* Revision 1.2  1998/02/24 18:23:28  dpp
* restore the pilot.h
*
* Dummycdcal library for dr3
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE CTGMCD( ILCD, IWCD, ZCAT, PHICAT, STILT )
C.......................................................................
C.
C. CTGMCD - Compute Z, PHI, SIN(tilt) at centre of cathode at ILCD, IWCD
C.
C. COMMON    : CDCTLI CDGEOC CDGEOI CDGEOL CDGEOR
C. CALLS     : VSCALE VSUM
C. CALLED    : User
C. AUTHOR    : R. Namjoshi
C.
C. VERSION   : 1.03
C. CREATED   : 23-Jun-87
C. LAST MOD  : 23-Nov-88
C.
C. Modification Log.
C.
C.  4-May-91 : If IWCD<0 on input: Computes IWCD using ZCAT and
C.             PHICAT.  Returns ZCAT, PHICAT of center of pad.    G. Ludwig
C. 16-Dec-90 : Corrected C2MOVE                                   B. Geiser
C.  7-Oct-90 : Would you believe it... (Added DC2MOV)             B. Geiser
C. 23-Nov-88 : Even better numbers from Dan Peterson.             R. Namjoshi
C. 12-Oct-87 : Better numbers from Dan Peterson.                  R. Namjoshi
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE
C
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
C
C== Argument declarations
C
      INTEGER ILCD, IWCD
      REAL    ZCAT, PHICAT, STILT
C
C== External function declarations
C
      REAL     VSUM
      EXTERNAL VSUM
C
C== Local variables
C
* IDEV   - Device number
* IPHI   - Phi segment number
* IPAD   - Cathode pad number
* JPHI   - Loop index variable
* INXT   - Next cathode index
* IW     - East/West indicator
*
* CALLED - Is .TRUE. if this routines has been called at least once
*
* WRPAVG - Average  of DCTWRP
* OFFAVG - Average  offset of outer cathode panels
* OFFDIF - Diff. in offset of outer cathode panels
* DCTOWD - Width of DR cathode outer panels
* DCTOTN - Temp. storage for tan(tilt)
*
      INTEGER IDEV,   IPHI,   IPAD,   JPHI,   INXT,   IW
      LOGICAL CALLED
      REAL    WRPAVG, OFFAVG, OFFDIF, DCTOWD, DCTOTN
C
C== More local variables for inverse calculation
C
* FPHI   - Float-valued PHI number
* FPAD   - Float-valued PAD number
* ZMID   - Midpoint between pads 47 and 48 for DR inner cathode
* DZPAD  - Z step between pads (equals CELZCD(ILCD) except for DR outer)
*
      REAL    FPHI, FPAD, DZPAD, ZMID
C
C== Geometry variables, candidates for COMMON & BLOCKDATA
C
* INTOME - Multiplicative factor for inches to meters
*
* LCTPVD - Number of inner VD cathode phi segmentations
* MCTPVD - Number of outer VD cathode phi segmentations
* LCTPDR - Number of inner DR cathode phi segmentations
* MCTPDR - Number of outer DR cathode phi segmentations
*
      REAL       INTOME
      PARAMETER( INTOME = 0.0254 )

      INTEGER    LCTPVD,      MCTPVD,      LCTPDR,      MCTPDR
      PARAMETER( LCTPVD =  8, MCTPVD =  8, LCTPDR = 16, MCTPDR = 8 )
*
* NCTPCD - Number of cathode phi segmentations in layer
*
* DRWARP - DR endplate warp at DR inner cathode radius (in)
* DRCOLR - DR collar thickness (in)
* DRZEND - DR endplate inner z (in)
* DCTERR - Cumulative errors in DRWARP etc. (m)
* DCTZ47 - DR inner cathode z of pad 47 from inside collar (m)
* DCTZ48 - DR inner cathode z of pad 48 from inside collar (m)
* DCTWRP - DR outer cathode offsets due to endplate warp (in)
* DCTZ0L - DR outer cathode z of pad 0 offset from endplate,low  phi(in)
* DCTZ0H - DR outer cathode z of pad 0 offset from endplate,high phi(in)
*
* DCTIZ0 - DR inner cathode z of pad 0/48 in sector (m) computed
* DCTOZ0 - DR outer cathode z of pad 0/48 in sector (m) computed
* DCTOSN - DR outer cathode sin of tilt angle
* DCTOCS - DR outer cathode cos of tilt angle
* DC2MOV - Unexpained movement of DR outer cathode in CLEO2

*
      INTEGER NCTPCD(0:3)
*
      REAL    DRWARP, DRCOLR, DRZEND, DCTERR
      REAL    DCTZ47(LCTPDR), DCTZ48(LCTPDR)
      REAL    DCTWRP(MCTPDR), DCTZ0L(MCTPDR), DCTZ0H(MCTPDR)
      REAL    DCTOSN(MCTPDR), DCTOCS(MCTPDR), DC2MOV(MCTPDR)
      REAL    DCTIZ0(2,1:LCTPDR), DCTOZ0(2,1:MCTPDR)
C
      DATA NCTPCD / LCTPVD, MCTPVD,
     +              LCTPDR, MCTPDR  /
C
      DATA DRWARP / 0.3115 /

      DATA DRCOLR / 1.0160 /

      DATA DRZEND / 38.000 /

      DATA DCTERR /  0.0   /

      DATA DCTZ47 / 0.92780, 0.92850, 0.92910, 0.92970,
     +              0.92950, 0.92930, 0.92920, 0.92820,
     +              0.92890, 0.92890, 0.92830, 0.92890,
     +              0.92890, 0.92820, 0.92900, 0.92850  /

      DATA DCTZ48 / 0.92870, 0.92950, 0.93010, 0.93070,
     +              0.93040, 0.93050, 0.93010, 0.92920,
     +              0.92980, 0.92980, 0.92970, 0.93010,
     +              0.93020, 0.93000, 0.93010, 0.92950  /

      DATA DCTWRP / 0.080, 0.072, 0.073, 0.096,
     +              0.113, 0.093, 0.084, 0.079  /

      DATA DCTZ0L / 2.100, 2.170, 2.130 ,2.120,
     +              2.090, 2.090, 2.120, 2.080  /

      DATA DCTZ0H / 2.130, 2.160, 2.130, 2.130,
     +              2.110, 2.130, 2.130, 2.110  /

      DATA DC2MOV / -0.00032,  0.00057,  0.00001, -0.00086,
     +               0.00005, -0.00040, -0.00158,  0.00041 /
*
      DATA CALLED / .FALSE. /
C----------------------------------------------------------------------
C
C== Do one time initializations
C
      IF( .NOT.CALLED ) THEN
          CALLED = .TRUE.
*
* Convert some inches to meters
*
          DRWARP = DRWARP*INTOME
          DRCOLR = DRCOLR*INTOME
          DRZEND = DRZEND*INTOME
*
* Outer cathode panel warps
*
          WRPAVG = VSUM( DCTWRP, MCTPDR ) / MCTPDR

          DO 10 JPHI = 1, MCTPDR
              INXT = JPHI + 1
              IF( INXT.GT.MCTPDR ) INXT = 1
              DCTZ0L(JPHI) =   DCTZ0L(JPHI) +
     +                       ( DCTWRP(JPHI) - WRPAVG )/2.0
              DCTZ0H(JPHI) =   DCTZ0H(JPHI) +
     +                       ( DCTWRP(INXT) - WRPAVG )/2.0
10        CONTINUE

          CALL VSCALE( DCTZ0L, INTOME, DCTZ0L, MCTPDR )
          CALL VSCALE( DCTZ0H, INTOME, DCTZ0H, MCTPDR )
*
* Compute z of pad 0/48 for inner and outer DR cathodes
*
          DO 20 JPHI = 1, LCTPDR
              DCTIZ0(1,JPHI) = -DRZEND + DRWARP + DRCOLR + DCTERR +
     +                          DCTZ47(JPHI) - 47.45*CELZCD(LCATDR)
              DCTIZ0(2,JPHI) = -DRZEND + DRWARP + DRCOLR + DCTERR +
     +                          DCTZ48(JPHI) - 47.55*CELZCD(LCATDR)
20        CONTINUE

          DO 30 JPHI = 1, MCTPDR
              OFFAVG = (DCTZ0L(JPHI) + DCTZ0H(JPHI)) / 2.0
              OFFDIF = (DCTZ0H(JPHI) - DCTZ0L(JPHI))
              DCTOZ0(1,JPHI) = -DRZEND + DCTERR + OFFAVG +
     +                          DC2MOV(JPHI) + 0.45*CELZCD(MCATDR)
              DCTOZ0(2,JPHI) =  OFFDIF
30        CONTINUE
C
C== Compute Sin/Cos of tilt of each outer DR cathode panel
C
          DCTOWD = RENDCD(MCATDR)*CELLCD(MCATDR)
          DO 40 JPHI = 1, MCTPDR
              DCTOTN = DCTOZ0(2,JPHI) / DCTOWD
              DCTOSN(JPHI) = DCTOTN / SQRT( 1 + DCTOTN**2 )
              DCTOCS(JPHI) =    1.0 / SQRT( 1 + DCTOTN**2 )
40        CONTINUE
C
      ENDIF

      PRINT 9900,IWCD,NWIRCD(ILCD),ILCD,IDEV,IDRCT
 9900 FORMAT(' CTGMCD: IWCD=',I6,' WANT 0<wire<NWIRCD=',I5,
     2' ILCD=',I2,' IDEV=',I3,' WANT ',I3)

      IF (IWCD .GE. 0) THEN
C
C==== DUET case: compute ZCAT, PHICAT, STILT of center of cathode pad.
C
         ZCAT   = 0.0
         PHICAT = 0.0
         STILT  = 0.0

C     ---Check for valid cathode layer and wire number
         IDEV = IDVCCD(ILCD)
         IF( IDEV.NE.IDRCT ) RETURN
         IF( IWCD.GT.NWIRCD(ILCD) )              RETURN

C     ---Compute PHI and PAD number from wire number
         IPHI = IWCD/NWRZCD(ILCD) + 1
         PRINT 9903,IWCD,NWRZCD(ILCD),IPHI,NCTPCD(ILCD-LCATCD),LCATCD
 9903    FORMAT(' CTGMCD: IWCD=',I5,' NWRZCD=',I5,
     2          ' IPHI=',I5,' WANT <',I5,' LCATCD=',I5)
C         IF( IPHI.GT.NCTPCD(ILCD-LCATCD) )              RETURN

         IPAD = MOD( IWCD, NWRZCD(ILCD) )
         PRINT 9904,IWCD,IPAD
 9904    FORMAT(' CTGMCD: IWCD=',I5,' IPAD=',I5)

C     ---Handle VD and DR cathodes separately
         PHICAT = PHIFCD(ILCD) + (IPHI-1)*CELLCD(ILCD)
         PRINT 9905,PHICAT,ILCD,LCATDR
 9905    FORMAT(' CTGMCD: PHICAT=',F6.3,' ILCD=',I5,' ?=? LCATDR=',I5)
         IF( ILCD.EQ.LCATDR ) THEN
            IW   = 1
            IF( IPAD.GE.NWRZCD(ILCD)/2 ) IW = 2
            ZCAT = DCTIZ0(IW,IPHI) + IPAD*CELZCD(ILCD)
         PRINT 9906,ZCAT
 9906    FORMAT(' CTGMCD: ILCD.EQ.LCATDR: ZCAT=',F9.6)
         ELSE
            IW    = 1
            ZCAT  = DCTOZ0(IW,IPHI) + IPAD*CELZCD(ILCD)
            ZCAT  = ZCAT * DCTOCS(IPHI)
            STILT = DCTOSN(IPHI)
         PRINT 9907,ZCAT
 9907    FORMAT(' CTGMCD: ILCD.NE.LCATDR: ZCAT=',F9.6)
         ENDIF

      ELSE
C
C==== CLEOG case: compute IWCD using ZCAT and PHICAT as input.
C                 Return ZCAT, PHICAT of center of pad.
C
C     ---Check for valid cathode layer
         IDEV = IDVCCD(ILCD)
         IF (IDEV.NE.IDRCT) RETURN

C     ---Compute PHI number
         FPHI = (PHICAT-PHIFCD(ILCD))/CELLCD(ILCD) + 1.0
         IPHI = NINT(FPHI)
         IF (IPHI.LT.1 .OR. IPHI.GT.NWIRCD(ILCD)/NWRZCD(ILCD)) THEN
            FPHI = FPHI + SIGN(NWIRCD(ILCD)/NWRZCD(ILCD),1-IPHI)
            IPHI = NINT(FPHI)
         END IF

C     ---Compute PAD number separately for VD and DR
         DZPAD = CELZCD(ILCD)
         IF (ILCD .EQ. LCATDR) THEN
            ZMID = 0.5*(DCTIZ0(1,IPHI)+DCTIZ0(2,IPHI))
     +                                              + 47.5*DZPAD
            IW   = 1
            IF (ZCAT .GT. ZMID) IW = 2
            FPAD = (ZCAT-DCTIZ0(IW,IPHI)) / DZPAD
         ELSE
            IW   = 1
            FPAD = (ZCAT/DCTOCS(IPHI)-DCTOZ0(IW,IPHI)) / DZPAD
            DZPAD = DCTOCS(IPHI)*DZPAD
         END IF
         IPAD = NINT(FPAD)
         IF (IPAD .LT. 0) IPAD = 0
         IF (IPAD .GE. NWRZCD(ILCD)) IPAD = NWRZCD(ILCD)-1

C     ---Return the wire number  0..NWIRCD(ILCD)-1
         IWCD = (IPHI-1)*NWRZCD(ILCD) + IPAD

C     ---Also return Z and PHI of the center of the pad
         ZCAT   = ZCAT   - (FPAD-IPAD)*DZPAD
         PHICAT = PHICAT - (FPHI-IPHI)*CELLCD(ILCD)

      END IF

      RETURN
      END



