*
* $Id: cdcorh.F,v 1.6 2003/03/05 17:47:13 dpp Exp $
*
* $Log: cdcorh.F,v $
* Revision 1.6  2003/03/05 17:47:13  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.5  2001/11/19 23:47:47  dpp
*      -> remove diagdoit (old duet diagnostics) routines
*      -> delete unused include file
*
* Revision 1.4  1999/11/29 21:43:45  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.3  1999/05/28 03:04:52  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.2  1998/07/09 00:39:40  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.6  1997/03/19 18:05:15  clib
* Now calls DFCORR for drift corrections.
*
* Revision 1.5  1996/06/27 18:11:52  clib
* Consistent implementation of meaning of ZDTGL parameter.
*
* Revision 1.4  1996/05/03 21:10:49  zfiles
* Calls STCORR if control parameters (ZDTGL) enables it.
* STCORR treated as a timelike variable.
*
* Revision 1.3  1996/02/15 13:57:04  zfiles
* Bug fixes from Ken
*
* Revision 1.2  1996/02/08 00:29:31  zfiles
* Mods from Ken. Works with new and old calibration
*
* Revision 1.1.1.1  1994/10/08  01:00:52  zfiles
* first version of duet in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/32 08/08/94  17.56.47  by  Alain Bellerive
*CMZ :  6.00/30 15/06/94  20.58.11  by  Unknown
*CMZ :  6.00/18 22/03/94  15.26.27  by  Dan Peterson
*CMZ :  5.54/03 05/05/91  18.23.27  by  Steve Schaffner
*CMZ :  5.54/00 29/01/91  16.02.11  by  Steve Schaffner
*CMZ :  5.53/07 27/09/90  01.20.15  by  D. Riley
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
      SUBROUTINE CDCORH( UFXYHT )
C.......................................................................
C.
C. CDCORH -  Correct drift distance and hit position for all hits
C.
C. COMMON    : ?
C. CALLS     : CDCFID DFOFPS RCORCD TCORCL UFXYHT
C. CALLED    : DUFIND
C. AUTHOR    : R. Namjoshi
C.
C. VERSION   : 1.08
C. CREATED   : 07-Dec-86
C. LAST MOD  : 13-Sep-88
C.
C. Modification Log.
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
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/utilseq/cdcalpa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/utilseq/cdcalin.inc"
#include "cdoffcal/cdseq/cdtggl.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdcorrcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"

C== Externals

      REAL     DFOFPS,TCR2CL,PHCORR,CORSZ,DCORSZ,DFCORR !,RCORCD,TCORPH
      EXTERNAL DFOFPS,TCR2CL,PHCORR,CORSZ,DCORSZ,DFCORR !,RCORCD,TCORPH
      EXTERNAL UFXYHT

C== Local variables

      REAL    TRACK(5),ARCL,DWMAX,ZW,YSAG,SINT,TDRFT,DDRFT
      REAL    ZZ,PHIW,RLYR,RDUM,DTDZ,CFRAC,AMB
      INTEGER ILLYR,ILYR,ILCD,IDEV,IADLST,I,IAMB,JAMB,IADR,IERR,IWIRE

* ----------Executable code starts here---------------------

C!!!TEMP
      IF( ABS(TANDLF).GT.5.0 ) TANDLF=0.0

      CALL CDCFID( TRACK(1), TRACK(2), TRACK(3), KAPCF, ALPCF, BETCF,
     +             GAMCF, XICF            )
      TRACK(4) = TANDLF
      TRACK(5) = Y0LF
*
      DO 20 ILLYR = 1, NLYRHT
         ILYR   = IPLRLR(ILLYR)
         IDEV   = IDVCTF(ILYR)
         IF(IDEV.EQ.IDRFT .OR. IDEV.EQ.IVDET .OR. IDEV.EQ.IIVDT .OR.
     +                                   IDEV.EQ.IPTLA) THEN
            ILCD   = ILTFCD(ILYR)
            ARCL   = SLYRLR(ILLYR)
            DWMAX  = 0.5*SQRT(2.)*CELLTF(ILYR) * RTF(ILYR)
            ZW     = TANDLF*ARCL + Y0LF
            YSAG   = -BSAGCD(ILCD)*(1. - (ZW/ZENDTF(ILYR))**2)
            SINT   = (KAPCF*RSQTF(ILYR)-GAMCF)/RTF(ILYR)
            IF( ABS(SINT).GT.1.0 ) SINT = SIGN( 1.0, SINT )
            IADLST = 0
            DO 30 I = LHITLR(ILLYR), MHITLR(ILLYR)
              IAMB   =      IADRHT(I)
              JAMB   = ISIGN(1,IAMB)
              AMB    = FLOAT(JAMB)
              IADR   = IABS(IADRHT(I))

C---          Swap the VD east wire numbers into the east locations if
C---          needed - Alainb

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
c     1                ILCD,IWIRE,TRACK,AMPI2,ARCL,ZW)
C-A Lyon-5-24-99              TDRFT = TCR2CL(EvntHit_rawTIM(IADR),
c     1                               ILCD,TRACK,AMPI2,ARCL,ZW)

C==============================================================================
C No Time Corrections 9/1/96 KMCL
C                  2/2/96 KMCL Introduce toggles
C==============================================================================
C 9/1/96 KMCL Simplified somewhat... 
C 2/2/96 KMCL Introduced toggles
C 2/20/97 PIH move drift corrections to dfcorr
C DFOFPS/DDRFT is signed

c              DDRFT = DFCORR(TDRFT,ILCD,ZW,SINT,JAMB,SINT) ! signed 
C 30 microns/nanosecond; 3E-8 meters/ps 
              DDRFT=3.E-8 * TDRFT * AMB                     ! signed


              DDRFT = AMIN1(AMAX1(DDRFT*AMB,0.),DWMAX)
                                  ! REMOVE AMBIGUITY (OLD DF CAN GIVE
                                  ! GIVE NEGATIVE |DDRFT| FOR +VE AMBIGUITY
                                  ! IF EvntHit_Charge = 0, ENTERTAINING EH.). 
              EvntHit_Drift(IADR) = DDRFT 	      ! UNSIGNED
C==============================================================================
              IADLST = IADR
              CALL UFXYHT(IAMB,ILYR,YSAG,XHIT(I),YHIT(I),IERR)
30          CONTINUE
         ENDIF
20    CONTINUE
      RETURN
      END








