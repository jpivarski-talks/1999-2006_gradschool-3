
*
* $Id: cdcalin.inc,v 1.1 1999/11/29 21:44:17 lyon Exp $
*
* $Log: cdcalin.inc,v $
* Revision 1.1  1999/11/29 21:44:17  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.9  1998/02/18 00:39:37  cpp
* Add new variables: DDMAX, DTMAX, DVAV, DDMAX2, DTMAX2, DVAV2.
*
* Revision 1.8  1997/10/14 22:51:13  clib
* Added CDCTL (mclean)
*
* Revision 1.7  1996/05/24 16:43:30  clib
* Commons for invalid constants removed.  Version comments fixed.
*
* Revision 1.6  1996/05/03 22:07:19  zfiles
* New common block for CDOFFCAL B field variable updated at each CDCABR call.
*
* Revision 1.5  1996/03/11 14:05:11  zfiles
* Integer*2 --> Integer*4
*
* Revision 1.4  1996/03/08 01:25:51  zfiles
* Pointer arrays reduced to integer*2, Msegcl2 parameter reduced to 3400 from
* 6000, to restrain ridiculous growth in executable size. (integer*2 is illegal
* in CLEO standards (nk))
*
* Revision 1.3  1996/02/09 23:44:43  zfiles
* Z corrections. Parameter sizing new drift function arrays extended
*
* Revision 1.2  1996/02/07 22:14:20  zfiles
* Added variables for recompress. From Ken Mclean
*
* Revision 1.1.1.1  1994/10/04  20:58:49  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/55 01/07/94  10.53.22  by  Karen Lingel, Physicist
*CMZ :  1.02/53 13/06/94  15.18.59  by  A. Bellerive & G. Ludwig
*     Added parameters for new signal propagation correction
*CMZ :  1.01/84 07/02/91  18.19.52  by  Maurice Garcia-Sciveres
*CMZ :          13/09/90  13.38.27  by  G. Ludwig
*     Working version of /CANGCL/.   Added PDEDCL to /CCONCL/
*CMZ :          21/06/90  16.35.23  by  G. Ludwig
*     Added PFLGCL and QOFFCL to /CCONCL/
*     Deleted XSEGCL, added WSEGCL and CSIZCL in /CDRFCL/
*CMZ :          21/06/90  12.35.22  by  Maurice Garcia-Sciveres
*CMZ :  3.00/00 06/02/90  16.18.18  by  G. Ludwig
*CMZ :          05/02/90  14.34.39  by  G. Ludwig
*     Added /CCALCL/ for contERrol variables
*CMZ :          17/12/89  18.28.11  by  G. Ludwig
*     /CELECL/ replaced by /CCONCL/ with ptrs instead of arrays
*     Tzero commons combined into /CTZECL/
*     Arrays of size KWIRCD changed to Zfiles ptrs-to-ptrs per device
*CMZ :          19/10/89  12.26.56  by  G. Ludwig
*     Added variables to /CELECL/
*CMZ :          27/09/89  22.14.49  by  G. Ludwig
*-- Author :
C
C.........................................................................
C.
C. CDCALIN - SEQuence for CLEO-II Central Detector Calibration COMMONS
C.
C. COMMON   : CCONCL  CTZECL  CDRFCL  CORQCL  CORZCL  CANGCL
C           : CPOSCL  CSIGCL  CWGTCL
C.          : CDIACL  CVIACL  CVDGCL  CVDQCL  CLEVCL
C.
C. AUTHOR   : ?? (CLEO-1.5 version CLINC:CALSCD.INC)
C.          : G. Ludwig, CLEO-II version.    25-Jun-89
C.
C. Modification Log.    (Later modifications recorded in CMZ history records)
C.
C.    27-Sep-89  G. Ludwig  Allow multiple parameters for electronic "slopes"
C.
C.    29-Jul-89  G. Ludwig  Deleted /CPRACL/, preamp arrays in /CPRECL/ now
C.                          indexed by crate, slot, half-card.
C.
C............................................................................
C
C /CCONCL/  ---  /CLEO/CD/CALIBRATION/devc/ELEC/PEDS
C                /CLEO/CD/CALIBRATION/devc/ELEC/TIME_SLPS
C                /CLEO/CD/CALIBRATION/devc/ELEC/CHARGE_SLPS
C
C  The Time (in psec) and Charge (in ??) are obtained for each
C  channel (DVN, DLA) as follows:
C
C                - tslp2 - sqrt( tslp2^2 - 4*(tslp1-TDC)*tslp3 )
C       Time   = -----------------------------------------------   if TTSLCL=1
C                                2 * (tslp1-ITDC)
C
C                     (should have tslp2<0 and tslp1,tslp3>0)
C
C
C       Charge = 10. * (qped-IADC)                                 if TQSLCL=1
C
C  where
C       tped   = ZU(PZU(PTPDCL(DVN)) + DLA)
C       tslpn  = ZU(PZU(PTSLCL(n,DVN)) + DLA)
C       qped   = ZU(PZU(PQPDCL(DVN)) + DLA)
C       qslpn  = ZU(PZU(PQSLCL(n,DVN)) + DLA)
C
*
* PTPDCL - Pointer-to-pointer for time pedestals
* PTWDCL - Pointer-to-pointer for time pedestal-widths
* PTSLCL - Pointer-to-pointers for time "slopes"
* NTSLCL - Number of time slope parameters used
* TTSLCL - Type of time slope parametrization
*
* PQPDCL - Pointer-to-pointer for charge pedestals
* PQWDCL - Pointer-to-pointer for charge pedestal-widths
* PQSLCL - Pointer-to-pointers for charge "slopes"
* NQSLCL - Number of charge slope parameters used
* TQSLCL - Type of charge slope parametrization
*
* QOFFCL - Charge offsets per crate
* PFLGCL - Pointer-to-pointer for list of calibration flags
* PDEDCL - Pointer-to-pointer for list of dead channels
*
      INTEGER         PTPDCL, PTWDCL, PTSLCL, NTSLCL, TTSLCL,
     +                PQPDCL, PQWDCL, PQSLCL, NQSLCL, TQSLCL,
     +                PFLGCL, PDEDCL
      REAL            QOFFCL
      COMMON /CCONCL/ PTPDCL(MXDVCD), PTWDCL(MXDVCD),
     +                PTSLCL(MTSLCL,MXDVCD),
     +                NTSLCL(MXDVCD), TTSLCL(MXDVCD),
     +                PQPDCL(MXDVCD), PQWDCL(MXDVCD),
     +                PQSLCL(MQSLCL,MXDVCD),
     +                NQSLCL(MXDVCD), TQSLCL(MXDVCD),
     +                QOFFCL(MCRTCL), PFLGCL(MXDVCD), PDEDCL(MXDVCD)
      SAVE   /CCONCL/
C
C -----------------------------------------------------------------------
C
C /CTZECL/  ---  /CLEO/CD/CALIBRATION/devc/TZERO/CRATE
C                /CLEO/CD/CALIBRATION/devc/TZERO/PREAMP
C                /CLEO/CD/CALIBRATION/devc/TZERO/WIRE
C
C     These variables contain all the T-zero corrections.
C
C     The "preamp" T-zero is really per half-card per crate.  Most of the
C     correction is incorporated in the slope (due to propagation delay
C     in the card) and thus wire to wire corrections can be made small
C     and random.  The correction is computed as
C             DT = SLPPCL*ICHAN + OFFPCL
C     where ICHAN is the channel number within the half-card (0 to 23).
C
C     SLPPCL and OFFPCL are indexed by crate, slot (1 to MSLTCL) and
C     which half (1=channels 1 to 24, 2=channels 25 to 48).
C
*
* DVCRCL - Device definition number for each crate (or zero)
* T0CRCL - Tzero value per crate (may change per run)
*
* OFFPCL - Offset in psec for channel 0 in preamp
* SLPPCL - Slope (psec/channel). This is about the same for all preamps.
*
* PTWRCL - Ptr-to-ptrs (for each device) to bank of time shifts per DLA
* LTWRCL - If TRUE, the PTWRCL banks include the crate, preamp and crude
*          time-of-flight corrections (computed at the beginning of the run).
*
      REAL            T0CRCL,  SLPPCL,  OFFPCL
      INTEGER         DVCRCL,  PTWRCL
      LOGICAL         LTWRCL
      COMMON /CTZECL/ DVCRCL(MCRTCL), T0CRCL(MCRTCL),
     +                SLPPCL(2,MSLTCL,MCRTCL), OFFPCL(2,MSLTCL,MCRTCL),
     +                PTWRCL(MXDVCD), LTWRCL(MXDVCD)
      SAVE   /CTZECL/
C
C -----------------------------------------------------------------------
C
C /CDRFCL/  ---  /CLEO/CD/CALIBRATION/anod/DRIFT/DRIFT_TIME
C                /CLEO/CD/CALIBRATION/anod/DRIFT/DRIFT_BINS
C
C     The drift-vs-time relation is stored here as a piecewise-linear
C     function for each anode layer and side of the wire:
C              DRIFT = OFFCL(bin,side,lyr) + T * VELCL(bin,side,lyr)
C     where NSEGCL(side,lyr) is the number of bins, and DSEGCL(side,lyr)
C     is the width of each.  (Bin 1 always starts at T=0.)
*
* NSEGCL - Number of drift bins per layer/side
* DSEGCL - Width of each drift bin (m)
* OFFCL  - The offset of each segment (m)
* VELCL  - The drift velocity of each segment (m/psec)
*
* WSEGCL - Width of each drift bin (ZERO unless both sides have same bin
* CSIZCL - Cell size per layer (m)
*
      INTEGER         NSEGCL
      REAL            DSEGCL, OFFCL, VELCL, WSEGCL, CSIZCL
      COMMON /CDRFCL/ NSEGCL(2,KCANCD), DSEGCL(2,KCANCD),
     +                OFFCL(MSEGCL,2,KCANCD), VELCL(MSEGCL,2,KCANCD),
     +                WSEGCL(KCANCD), CSIZCL(KLYRCD)
      SAVE   /CDRFCL/

C
C Simple drift parametrization (all layer indices are CD numbering)
C   DNTIME - TIME OF POINT (PS)
C   DNDIST - DISTANCE OF POINT (M)
C   DDMAX  - MAXIMUM DISTANCE OF DRIFT FUNCTION (M)
C   DTMAX  - TIME CORRESPONDING TO MAXIMUM DISTANCE (PS)
C   DDMAX2 - FIRST MAXIMUM DISTANCE OF DRIFT FUNCTION (M)
C   DTMAX2 - TIME CORRESPONDING TO FIRST MAXIMUM DISTANCE (PS)
C   DVAV   - DDMAX/DTMAX
C   DVAV2  - DDMAX2/DTMAX2
C   DNINDX(1,1,L) - 1ST DATA FOR EACH LAYER , averaged 
C   DNINDX(2,1,L) - 1ST DATA FOR EACH LAYER , L (+ve ambiguity)
C   DNINDX(3,1,L) - 1ST DATA FOR EACH LAYER , R (-ve ambiguity)
C          *,2,L  - LAST DATA ....
C   S2MAP  logical mask enabling 2 sided drift functions for each layer
C   ZMAP   logical mask enabling Z dependent drift functions for each layer
C   PARPRP =T enables old VD propagation correction
C The following are accessed by detector (e.g. by pointers IPTLA,IVDET,IDRFT)
C   DFTGL  =T enables old drift functions
C   PHTGL  =1 enables timelike cos(theta) corrs.
C          =3 enables timelike cos(theta) and normalized pulseheight corrs.
C   ZDTGL  =0 disables Z corrections
C          =1 enables old Z corrections for Stereo layers
C          =2 enables new Z corrections for any layers flagged by ZMAP
C          =3 enables new Z corrections for any layers flagged by ZMAP
C             and adds z dependent entrance angle corrections
C   ZDLEV  =1 Uses raw histogrammed data as Z correction
C          =2 Bilinear (2D) interpolation from raw histogram
C          =3 Adds quadratic term to ZDLEV=2 to ensure mean = ZDLEV=1 option
C   CDCTL  =1 FALSE Disables DFINDA re-init of S2MAP card parameters
C          =2 FALSE Disables DFINDA re-init of ZMAP card parameters
C          =3 FALSE Disables DFINDA re-init of ZCOR card parameters
C          =4 FALSE Disables DFINDA re-init of TGLCOR card parameters
C          =5 FALSE Disables DFINDA re-init of PULSE card parameters
C          =6 FALSE Disables DFINDA re-init of CHOOSEDF card parameters
C          =7 FALSE Disables DFINDA re-init of PROP card parameters
C          =8 FALSE Disables DFINDA re-init of CDPDMP card parameters
C          =9 FALSE Disables DFINDA re-init of WTFILES card parameters
C
      REAL DNTIME,DNDIST,DMAXCD,DDMAX,DTMAX,DVAV,DDMAX2,DTMAX2,DVAV2 
      INTEGER*4 DNINDX
      INTEGER*4 PHTGL,ZDTGL,ZDLEV 
      LOGICAL ZMAP,S2MAP,TSVD(NCANVD),TSDR(NCANDR),
     +                   ZDVD(NCANVD),ZDDR(NCANDR),
     +                   PRPTGL,DFTGL,ZINIT,UNPKND,CDCTL
      COMMON /DNDRFT/DNTIME(MSEGCL2,KCANCD),
     +               DNDIST(MSEGCL2,KCANCD),
     +               DNINDX(1000,2,KCANCD),DMAXCD(KCANCD),
     +               ZMAP(KCANCD),S2MAP(KCANCD),
     +               PRPTGL,DFTGL(MXDVCD),PHTGL(MXDVCD),ZDTGL(MXDVCD),
     +               ZINIT,UNPKND,ZDLEV,CDCTL(10),
     +               DDMAX (KCANCD,3),DTMAX (KCANCD,3),DVAV (KCANCD,3),
     +               DDMAX2(KCANCD,3),DTMAX2(KCANCD,3),DVAV2(KCANCD,3)

      SAVE   /DNDRFT/
      EQUIVALENCE (ZMAP (7),ZDVD(1)),(ZMAP (17),ZDDR(1))
      EQUIVALENCE (S2MAP(7),TSVD(1)),(S2MAP(17),TSDR(1))
*      
C
C -----------------------------------------------------------------------
C
C /CORQCL/  ---  /CLEO/CD/CALIBRATION/anod/DRIFT/Q_CORR
C
C      These are the pulse-height dependent corrections to the drift TIME.
C      The correction is in the form of a piecewise linear function of Q,
C               TCORPH = -SLPQCL * Q - OFFQCL
C      There is one correction function for each of NDFQCL(devc) regions of
C      drift distance.  The last index for each variable is the device
C      configuration number.
*
* XSGQCL - Starting point of the first "Q piece"
* DSGQCL - Length of each Q piece
* NSGQCL - Number of Q pieces
* DDFQCL - Size of each scaled drift region.  NOT USED  (Should equal 1/NDFQCL)
* NDFQCL - Number of scaled drift regions
* SLPQCL - Slope of each piece (see above equation)
* OFFQCL - Intercept of each piece (see above equation)
*
C      INTEGER         NSGQCL, NDFQCL
C      REAL            XSGQCL, DSGQCL, DDFQCL
C      REAL            OFFQCL, SLPQCL
C      COMMON /CORQCL/ XSGQCL(MDVQCL), DSGQCL(MDVQCL), NSGQCL(MDVQCL),
C     +                                DDFQCL(MDVQCL), NDFQCL(MDVQCL),
C     +                OFFQCL(MSGQCL,MDFQCL,MDVQCL),
C     +                SLPQCL(MSGQCL,MDFQCL,MDVQCL)
C      SAVE   /CORQCL/
C
C -----------------------------------------------------------------------
C
C /CORZCL/  ---  /CLEO/CD/CALIBRATION/DRAN/DRIFT/Z_CORR
C
C      These are the z dependent corrections to the drift DISTANCE. Only
C      the stereo layers of the DR are affected. The correction is in
C      the form of a piecewise linear function of Z,
C                   DCORZ = -SLPZCL * DSCALE - OFFZCL
C      There is one correction function for each of NDFZCL regions of
C      SCALED drift distance (only absolute value matters).
*
* XSGZCL - Starting point of the first "Z piece"
* DSGZCL - Length of each Z piece
* NSGZCL - Number of Z pieces
* DDFZCL - Size of each scaled drift region
* NDFZCL - Number of scaled drift regions
* SLPZCL - Slope of each piece (see above equation)
* OFFZCL - Intercept of each piece (see above equation)
*
      INTEGER                         NSGZCL,          NDFZCL
      REAL            XSGZCL, DSGZCL,          DDFZCL
      REAL            OFFZCL,                  SLPZCL
      COMMON /CORZCL/ XSGZCL, DSGZCL, NSGZCL,  DDFZCL, NDFZCL,
     +                OFFZCL(MSGZCL,MDFZCL,2), SLPZCL(MSGZCL,MDFZCL,2)
      SAVE   /CORZCL/
C
C -----------------------------------------------------------------------
C
C /CANGCL/  ---  /CLEO/CD/CALIBRATION/DRAN/DRIFT/ANG_CORR
C
C     Entrance-angle dependent correction to drift DISTANCE.
C
C     The correction takes the form
C        Corrected drift = Measured drift * (1+CFANCL),
C     where CFANCL depends on the sine of the entrance-angle and the
C     scaled drift distance.
*
* NSGACL - Sine of entrance-angle: Number if bins
* FSGACL - Sine of entrance-angle: 1/bin-width
*          Note: start of bin 1 is -NSGACL/(2*FSGACL)
* NDFACL - Scaled drift distance: number of bins
* FDFACL - Scaled drift distance: 1/bin-width
* XDFACL - Scaled drift distance: start of bin 1
* CFANCL - Coefficients of correction
*
C      INTEGER         NSGACL,         NDFACL
C      REAL                    FSGACL,         FDFACL, XDFACL
C      REAL            CFANCL
C      COMMON /CANGCL/ NSGACL, FSGACL, NDFACL, FDFACL, XDFACL,
C     +                CFANCL(MSGACL,MDFACL)
C      SAVE   /CANGCL/
C
C -----------------------------------------------------------------------
C
C /CPOSCL/  ---  /CLEO/CD/CALIBRATION/devc/PHYSICAL/POSITION
*
* PXWRCL - Ptr-to-ptr (for each device) to bank of position shifts per DLA
*
      INTEGER         PXWRCL
      COMMON /CWIRCL/ PXWRCL(MXDVCD)
      SAVE   /CWIRCL/
C
C -----------------------------------------------------------------------
C
C /CSIGCL/  ---  /CLEO/CD/CALIBRATION/devc/PHYSICAL/SIGNAL
*
* BSIGCL - Signal propagation velocity along wire
*          Need some sort of saturation correction here?
*
*********** KLUDGE ********************** by MGS 2/5/91
C-- Empirical, nonlinear propagation correction
* OFSGCL \ offsets and slopes for piecewise linear time saturation correction,
* SLSGCL /  function of Z * WENDCD(layer).
* NSIGCL  number of segments per device
* DSIGCL  width of each segment ********** IN MILLIMETERS ***********
* first index in offsets and slopes is IDTOCD(IDEVCD(LAYER))
* second index is |Z|/DSIGCL + 1
* third index in offsets and slopes is sign of Z*WENDCD
      REAL            BSIGCL(KLYRCD)
      REAL NSIGCL(3), DSIGCL(3)
      REAL OFSGCL(3,5,2),SLSGCL(3,5,2)
      EQUIVALENCE (SLSGCL(1,1,1),BSIGCL(1))
      EQUIVALENCE (OFSGCL(1,1,1),BSIGCL(31))
      EQUIVALENCE (DSIGCL(1),BSIGCL(61))
      EQUIVALENCE (NSIGCL(1),BSIGCL(64))
C
      COMMON /CSIGCL/ BSIGCL
      SAVE   /CSIGCL/
C
C *NEW* signal propagation correction scheme from Alain Bellerive
C
C /CSIPCL/  ---  /CLEO/CD/CALIBRATION/devc/PHYSICAL/SIGNAL
C
* NSIPCL - Number of segments for the piecewise fit
* DSIPCL - The width in Z of each segment (in meters)
* RSIPCL - "Index of refraction" for the pulse speed along wire (=c/v)
* OFSPCL - The time offset for each segment (ps)
* SLSPCL - The time slope for each segment (ps/m)
C
C Note: The negative indices are used as a kludge for the VD east
C       hits only.
C
      INTEGER NSIPCL(-KEVDCL:KCANCD)
      REAL    DSIPCL(-KEVDCL:KCANCD), RSIPCL(-KEVDCL:KCANCD)
      REAL    OFSPCL(MSIPCL,-KEVDCL:KCANCD),
     +        SLSPCL(MSIPCL,-KEVDCL:KCANCD)
C
      COMMON /CSIPCL/ NSIPCL, DSIPCL, RSIPCL, OFSPCL, SLSPCL
      SAVE   /CSIPCL/
C
C -----------------------------------------------------------------------
C
C /CWGTCL/  ---  /CLEO/CD/CALIBRATION/devc/WEIGHTS[/NUM]
C
C     Parameters to calculate 1/Sigma**2 of a hit, as a function of
C     the scaled drift distance (0 to 1).
*
* NMWTCL - Number of parameters used in CFWTCL (per CD layer)
* CFWTCL - Coefficients of function (polynomial in DRFSCL) for sigma**2.
*
      INTEGER         NMWTCL
      REAL                            CFWTCL
      COMMON /CWGTCL/ NMWTCL(KLYRCD), CFWTCL(MXWTCL,KLYRCD)
      SAVE   /CWGTCL/
C
C -----------------------------------------------------------------------
#if defined(CLEO_DIANA)
C
C /CDIACL/  ---
*
* DXWRDR - dE/dx calibration constants per wire
*
      REAL            DXWRDR
      COMMON /CDIACL/ DXWRDR(KWIRDR)
      SAVE   /CDIACL/
#endif
C
C -----------------------------------------------------------------------
#if defined(CLEO_VIANA)
C
C /CVIACL/  ---
*
* DXWRDR - dE/dx calibration constants per wire
*
      REAL            DXWRVD
      COMMON /CVIACL/ DXWRVD(KWIRVD)
      SAVE   /CVIACL/
#endif
C
C -----------------------------------------------------------------------
#if defined(CLEO_VIANA)
C
C /CVDGCL/  ---
C
C 14-Dec-87  MH   The following was added by Martin Hempstead and Vivek Sharma
C            VS   to account for VD anode charge gains and various corrections
C                 needed by VIANA.
*
* CHGNVD - VD anode charge gains
*
      REAL            CHGNVD
      COMMON /CVDGCL/ CHGNVD(KWIRVD)
      SAVE   /CVDGCL/
#endif
C
C -----------------------------------------------------------------------
#if defined(CLEO_VIANA)
C
C /CVDQCL/  ---
C
C 15-Feb-88  VS   We decided instead to make a "map" for all PH corrections.
*
* KQNORM - ??
* DRFQVD - Params, drift-time corr.
* DIPQVD - Params, dip-ang sat. cor.
* CRTQVD - Params, preamp sat. cor.
*
      INTEGER         KQNORM
      REAL            DRFQVD, DIPQVD, CRTQVD
      COMMON /CVDQCL/ KQNORM(MBINGN,MBINCT,MBINDF,MBINPH),
     +                DRFQVD(MPCDRF*NCANVD),
     +                DIPQVD(MPCDIP*NCANVD),
     +                CRTQVD(MVDCRT)
      SAVE   /CVDQCL/
#endif
C
C -----------------------------------------------------------------------
#if defined(CLEO_OLD)
C
C /CLEVCL/  ---  Current run quantities, available from CLEVER.
*
* BFLDCL - B Field in Kgauss
* ENRGCL - Beam energy
* KRUNCL - Current run
* KEVTCL - Current event
* NRUNCL - # runs processed
* LN20CL - # items to unpack (3 for new data, 2 for old)
*
      REAL            BFLDCL, ENRGCL
      INTEGER         KRUNCL, KEVTCL, NRUNCL, LN20CL
      COMMON /CLEVCL/ BFLDCL, ENRGCL,
     +                KRUNCL, KEVTCL, NRUNCL, LN20CL
      SAVE   /CLEVCL/
C
#endif
      REAL CDBFLD
      COMMON /BFLDCD/CDBFLD

