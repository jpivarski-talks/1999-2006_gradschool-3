*
* $Id: cdhitc.F,v 1.5 2003/03/05 17:52:33 dpp Exp $
*
* $Log: cdhitc.F,v $
* Revision 1.5  2003/03/05 17:52:33  dpp
*      -> remove use of PWCD; PWCD can be used for  other derived values
*      -> but cdhitc is not used in cleo3
*      -> changed all variable names in cdscrtcd to have common root
*      -> remove obsolete comments
*
* Revision 1.4  2001/11/19 23:49:31  dpp
*      -> delete unused include file
*
* Revision 1.3  1998/07/09 00:41:02  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.2  1998/05/08 14:14:00  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.2  1997/02/27 22:24:59  clib
* For dpp: Use effective cathode radius for calculating correction to
*          Z coordinate of hit (tilt).
*
* Revision 1.1.1.1  1994/10/08 01:00:51  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/27 18/05/94  12.04.18  by  Anders Ryd
*CMZ :  6.00/24 08/05/94  17.20.24  by  Anders Ryd
*CMZ :  6.00/21 29/03/94  09.26.57  by  Dan Peterson
*CMZ :          10/05/93  20.15.00  by  Jik Lee
* Use ZTEMP rather than EvntHit_Z to test for in/out of chamber in z.
*
*CMZ :          21/01/93  17.32.01  by  Rob Kutschke
*CMZ :  5.54/15 29/09/92  12.02.49  by  Rob Kutschke
*CMZ :  5.54/14 28/09/92  11.47.12  by  Dan Peterson
*CMZ :  5.54/03 25/04/91  21.27.25  by  Steve Schaffner
*CMZ :  5.54/00 31/01/91  14.59.28  by  Steve Schaffner
*CMZ :  5.53/07 27/09/90  17.39.24  by  Steve Schaffner
*CMZ :  5.53/04 18/06/90  16.35.52  by  Steve Schaffner
*CMZ :  5.52/01 22/03/90  20.26.33  by  Steve Schaffner
*CMZ :  5.52/00 07/03/90  11.23.52  by  Steve Schaffner
*CMZ :  5.51/05 14/12/89  13.52.29  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.48.49  by  Steve Schaffner
*CMZ :  5.50/02 24/05/89  15.21.29  by  Steve Schaffner
*CMZ :  5.50/00 22/04/89  13.35.30  by  Steve Schaffner
*-- Author :
      SUBROUTINE CDHITC
C.......................................................................
C.
C. CDHITC - Compute other hit related quantities
C.
C. COMMON    : SCRTCH CDGEOC CDGEOI CDGEOL CDGEOR
C. CALLS     : CTGMCD DFOFPS Z2PI
C. CALLED    : DUET
C. AUTHOR    : R. Namjoshi
C.
C. VERSION   : 1.03
C. CREATED   : 10-Nov-86
C. LAST MOD  : 25-Oct-88
C.
C. Modification Log.
C.
C. 29-Mar-94 : changed to run with East VD hit as Primary hit
C. 28-Sept-92: converted to glorious FORTRAN-66 (and corrected code)
C.             was not filling EvntHit_Drift for charge division layers
C. 28-Sept-92: use time in picoseconds instead of incorrectly indexed
C.             raw TDC for charge division cut
C. 23-Jun-87 : R. Namjoshi - Use CTGMCD to compute cathode positions
C. 25-Oct-88 : Added a rough version of charge division code, and then
C.           : commented it out, since we don't have the constants. -dsr
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "cl3seq/cdgm3/cdraw1in.inc"
#include "cl3seq/zfiles/usbank.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/qdcom.inc"
#include "doit/duseq/tfctlcde.inc"

      INTEGER IHW,ILCD,IWIR,IDEV
      REAL DSHFCL,Z2PI,PHICAT,STILT,DFOFPS,RLYR

C Q-DIV variables
C..IDLAY.....VD local layer number
C..IHIS......1-800 number (internal VD global wire number)
C..INDXP.....cell ID, Primary hit
C..INDXS.....cell ID, Secondary hit
C..NWIR......number of wires in the layer
C..IADRP.....hit address, Primary hit
C..IADRS.....hit address, Secondary hit
C..IADRW.....hit address, West hit
C..IADRE.....hit address, East hit
C..CSI.......charge assymetry
C..ZTEMP.....a Z value

      INTEGER IDLAY,IHIS
      INTEGER INDXP,INDXS,NWIR
      INTEGER IADRP,IADRS,IADRW,IADRE
      REAL CSI,ZTEMP

* ----------Executable code starts here---------------------

C== Compute positions and approximate drift distances.

      DO 69 IHW = 1, EvntHit_Num
          ILCD        = EvntHit_CDlayr(IHW)
          IWIR        = EvntHit_laWire(IHW)
          IDEV        = IDVCCD(ILCD)
CRYD  DON'T WANT TO DO ANYTHING IF SVX..
          IF (IDEV.EQ.ISVX1) GOTO 69
          IF (IDEV.EQ.ISVZ1) GOTO 69
          EvntHit_PhiWir(IHW)=EvntHit_PhiWir(IHW)
     2         +DSHFCL(ILCD,IWIR)/RCD(ILCD)
          EvntHit_PhiWir(IHW) = Z2PI(EvntHit_PhiWir(IHW))
          EvntHit_X(IHW)=RCD(ILCD)*COS(EvntHit_PhiWir(IHW))
     1         +XX0CD(1,ILCD)
          EvntHit_Y(IHW)=RCD(ILCD)*SIN(EvntHit_PhiWir(IHW))
     1         +XX0CD(2,ILCD)

          IF( IDEV.EQ.IVDCT )GO TO 21
          IF( IDEV.EQ.IDRCT )GO TO 21
          GO TO 31

C CATHODES
C correct cathodes for tilt
C no longer correct for XX0CD here; done in CTGMCD
C no longer fill EvntHit_Drift for cathodes
C no longer fills pwcd, so this variable can be used for corrected phi

21        CALL CTGMCD( ILCD, IWIR, EvntHit_Z(IHW), PHICAT, STILT )
          IF ( CTOACD(ILCD) .EQ. 0 ) THEN
              RLYR = RCD(ILCD)
          ELSE
              RLYR = RCD(CTOACD(ILCD)) + REFFCD(ILCD)
          ENDIF
          IF (.NOT.LMCRLO) EvntHit_Z(IHW)=EvntHit_Z(IHW) -
     +                    COS(PHICAT)*RLYR*UU0CD(1,ILCD)
          IF (.NOT.LMCRLO) EvntHit_Z(IHW)=EvntHit_Z(IHW) -
     +                    SIN(PHICAT)*RLYR*UU0CD(2,ILCD)
C.........EvntHit_Z(IHW) = EvntHit_Z(IHW) + XX0CD(3,ILCD)
C          PWCD(IHW) = EvntHit_Charge(IHW)
          GO TO 69

C WIRES
C including default values if charge division data is deemed garbage
C no longer fills pwcd, so this variable can be used for corrected phi

31        EvntHit_Z(IHW) = 0.0
C          PWCD(IHW) = 0.0
          EvntHit_NxtCls(IHW) = 0
          EvntHit_Drift(IHW)=DFOFPS(ILCD,0,0.0,EvntHit_Charge(IHW),
     1         EvntHit_rawTIM(IHW))
          IF (.NOT.LQDVCD(ILCD))GO TO 69

C CHARGE DIVISION
C require wire number to be less than maximum; indicating WEST

              NWIR = NWIRCD(ILCD)
              IF (IWIR.GE.NWIR)GO TO 59

C cell-id and hit address in west and east; IADRP should equal IHW
C use INWHCD (normally the pointer to next hit in cathode clusters)
C    to point from Primary hit to Secondary hit

                 INDXP = INDXCD(ILCD) + IWIR
                 INDXS = INDXCD(ILCD) + IWIR + NWIR
                 IADRP = EvntHit_MapDet(INDXP)
                 IADRS = EvntHit_MapDet(INDXS)
                 IF(LUSVDE)THEN
                       IADRE=IADRP
                       IADRW=IADRS
                 ELSE
                       IADRW=IADRP
                       IADRE=IADRS
                 ENDIF

C get the 1-800 number (internal VD global wire number)
C caution: this is device dependent hard wired code

                 IDLAY = ILCD-LCANVD+1
                 IF(IDLAY .GT. 5)GO TO 33
                    IHIS=(IDLAY-1)*64+IWIR+1
                    GO TO 35
33                  IHIS=5*64+(IDLAY-6)*96+IWIR+1
35               CONTINUE

C requirements on pulse height and time for acceptable charge division
C caution: this is device dependent hard wired code
C NOTE: the TDC cut is still placed on the WEST, even if EAST is primary

                 IF (IADRS.LE.0)                 GO TO 57
                 IF (EvntHit_Charge(IADRW) .LE.   1500.)  GO TO 57
                 IF (EvntHit_Charge(IADRE) .LE.   1500.)  GO TO 57
                 IF (EvntHit_rawTIM(IADRW) .LE. -20000.)  GO TO 57
                 IF (EvntHit_rawTIM(IADRW) .GE. 220000.)  GO TO 57

C charge division looks good, but still must check that it is in chamber
C NOTE: all the gain calibrations were done with WEST as Primary, and,
C   one must use W/E rather than Pri/Sec to preserve sign of Z
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

                    CSI = ( GAIN(IHIS)*
     1                EvntHit_Charge(IADRW)-EvntHit_Charge(IADRE) ) /
     2                ( GAIN(IHIS)*EvntHit_Charge(IADRW)+
     3                EvntHit_Charge(IADRE) )
                    ZTEMP= S0(IHIS)+S1(IHIS)*CSI +
     +                            S2(IHIS)*CSI**2+ S3(IHIS)*CSI**3

C NOTE: the corrected pulse height must be attached to the Primary hit
C no longer fills pwcd, so this variable can be used for corrected phi

C                    PWCD(IADRP) = EvntHit_Charge(IADRP)

C check that the calculated charge division Z position is inside the device
C caution: this is device dependent hard wired code

                 IF (ABS(ZTEMP).GE. 0.37) GO TO 57

C fill the charge division variables for good data

                       EvntHit_NxtCls(IADRP) = IADRS
                       EvntHit_Z(IADRP)= ZTEMP
                 GO TO 69
57            CONTINUE
C ^---break point for bad charge division data

              GO TO 69

59        CONTINUE
C ^---break point for out-of-range wire number

69    CONTINUE
C ^---end of loop over hit number

      END





