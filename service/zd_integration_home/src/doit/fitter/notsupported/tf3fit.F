*
* $Id: tf3fit.F,v 1.1 2003/03/05 17:44:49 dpp Exp $
*
* $Log: tf3fit.F,v $
* Revision 1.1  2003/03/05 17:44:49  dpp
*      -> NEW ROUTINE, moved from "fitter/."
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.10  2002/09/05 19:22:21  dpp
*    -> remove histograming
*
* Revision 1.9  2001/11/19 23:40:42  dpp
*      -> remove diagdoit (old duet diagnostics) routines
*      -> use status bit encapsulation
*      -> delete unused include file
*      -> fill NHITTF,CHITF directly instead of intermediate NHIT3F,CHI3F
*      -> use NHIT3F,CHI3F to hold the last fitted values from any fitter
*      -> remove error statistics
*
* Revision 1.8  2001/09/12 22:05:36  dpp
*      -> change statcf assignments
*
* Revision 1.7  2000/10/02 21:07:56  dpp
*      -> change statcf(3) -> (4)
*      -> change statcf(1) -> (3)
*      -> clear out use of STATCF(2)
*
* Revision 1.6  2000/08/18 18:57:34  dpp
*      -> new arguments for KABG2
*
* Revision 1.5  2000/04/26 14:53:29  dpp
*      -> use new XTS_TRAK variables: INTERUPT_VALUE and INTERUPT_THRESH
*      -> compatibility with reduced set of XTS_TRAK "which" inputs
*
* Revision 1.4  1999/11/29 21:43:43  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.3  1999/05/28 03:04:49  lyon
* Commented out calls to clever routines or zfiles
*
* Revision 1.2  1998/07/09 00:39:05  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:28  clib
* Developmental version of DUET.
*
* Revision 1.8  1997/04/03 00:06:58  clib
* Call prep_klmn and klmnft at top of routine if running kalman.
*
* Revision 1.7  1996/08/08 16:16:50  dpp
* remove extra "return" statement at KLMNFT
*
* Revision 1.6  1996/05/10 18:43:00  zfiles
* Update of fitting weights from Rene.
*
* Revision 1.5  1996/05/10 13:00:34  zfiles
* Keep QDIV hits in tftrakcd at exit.
* Fill MESFIT, SG1FIT, SG2FIT.
*
* Revision 1.4  1995/09/06  00:05:00  zfiles
* Removed print statement
*
* Revision 1.3  1995/06/25  01:20:44  zfiles
* Fix error in XFIT,YFIT calculation for QDIV in DO 197 loop.
* ( The bug had no impact on any compressed data. )
*
* Revision 1.2  1995/06/19  20:46:49  zfiles
* Added Kalman filter stuff from Rob
*
* Revision 1.1.1.1  1994/10/08  01:00:49  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE TF3FIT
C
C=======================================================================
C     AUTHORS  M. OGG         JUL 1982
C              K. READ        OCT 1984
C              R. NAMJOSHI    JAN 1986
C              S. SCHAFFNER       1992
C              D. PETERSON        1992
C
C     TF3FIT does the 3-dimensional track fit and produces
C     a 7*7 error matrix in the CLEO track variables:
C     (1) CURV, (2) PHI0, (3) D0, (4) TANDIP, (5) Z0, (6) PHI6
C     and (7) PHI7 (a kink in mid DR)
C
C     The error matrix is stored in symmetric mode as
C     a column vector: ERMFIT
C
C     CURV    (1)
C     PHI0    (2)  (3)
C     D0      (4)  (5)  (6)
C     TANDIP  (7)  (8)  (9) (10)
C     Z0     (11) (12) (13) (14) (15)
C     PHI6   (16) (17) (18) (19) (20) (21)
C     PHI7   (22) (23) (24) (25) (26) (27) (28)
C
C changes of Feb 28, 94,
C     1..allows a kink in PHI at the origin using LKN6FI, if DUALTR set
C     2..allows a kink in "z" at the origin using LKN7CT, if DUALTR set
C        either LKN6FI or LKN7CT disables LKN6LO and LKN7LO
C     3..VD-EAST hits in the fit list, unfitted, using LADVDE and LCALIB
C     4..remove hits from fit with logical LRMVHT, does not require DUALTR
C changes were made in May 93 to allow constants finding
C     1..calibration and dual track modes specified with LCALIB and DUALTR
C     2..second track, indicated by USEFIT=2, uses reflection of track 1
C     3..some of the derivatives were changed
C     4..use call to TF3INV to allow for unfitted (fixed) variables
C several changes were made in July 92
C     1..new structure
C     2..new call sequence to get charge division constants (not yet)
C     3..new transcendental iteration
C     4..correcter entrance angle correection after transcendental iter
C     5..refresh of S for cathodes on each iteration; call to LFCTHT
C     6..cleaner update of CHI2
C     7..calls to CFHITA and TFKINK
C     8..call to CDCORF(TFXYHT)
C changes of October 92
C     1..activate 2nd kink
C     2..remove refresh of S for cathodes on each iteration; call to LFCTHT
C     3..call CTWTQD directly for charge division constants
C=======================================================================
      IMPLICIT NONE
#include "cl3seq/sv3seq/svgeompa.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/statusbits.inc"
#include "doit/duseq/tfhistpa.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tffitctl.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "seq/clutil/mesrep.inc"
#include "cl3seq/sv3seq/svlaseq.inc"
#include "doit/duklseq/duklmncd.inc"
C
C documentation of local variables, sort of in order of appearance
C
C  IER......returned error type number
C  NITMIN...minimum number if iterations before testing for convergence
C  ITDRPH...minimum iteration number to allow dropping hits
C  IFIT.....index for hits in fit
C  LKN6LO...local flag that LKN6TF if set and kink in at VD-DR in use
C  RKINK6...radius of kink, at VD DR interface
C  SIG6.....sigma for kink; variable number 6
C  LKN7LO...local flag that LKN7TF if set and kink in DR in use
C  LKN7TM...local temporay flag that LKN7LO will be set at end of iteration
C  RKINK7...radius of kink, in mid DR
C  SIG7.....sigma for kink; variable number 7
C  MP3DLO...local value of MP3DTF; number of variables in fit
C  IPNTR()..array giving radial order of entering TFTRAK arrays
C  IADTMP().copy of entering IADFIT, for reordering
C  IPLTMP().copy of entering IPLFIT, for reordering
C  STMP()...copy of entering SFIT, for reordering
C  USETMP().copy of entering USEFIT, for reordering
C  NFSAVE...saved NFIT, from before refilling
C  NDELET...number of hits deleted because of worst residual
C  NADDED...number of hits added to the fit list, but not in fit
C  NHTSTR...number stereo hits, counts as both r-phi and z degrees of freedom
C  NREGN()..number of r-phi hits
C            normal fitting  1:before Kink6,   2:K6 to K7,   3:after K7
C             DUALTR          1:track1 before Kink6,  2:track1 after Kink6,
C                             3:track1 before Kink7,  4:track2 after Kink7,
C         DUALTR.and.LKN7CT   5:track1 z-hits      ,  6:track2 z-hits
C  NFITT()..for dual track use, number of hits in each sub track
C  REGNHT().array giving the region number (1:3), 0 for z hits
C           for dual track use, (1:2) for first subtrack (2, after kink)
C                               (3:4) for first subtrack (4, after kink)
C  RFIT.....index of hits fit using IPNTR to put in radial order
C  ILYRS....physical layer number, ILYR, signed to show unfitted layers
C  ILYR.....physical layer number, (+QDIVAD for charge division)
C  IADR.....hit address
C  IWADR....wire address
C  RESI.....residual of hit i
C  RWSQ()...radius of wire for hit, squared
C  RW().....radius of wire for hit
C  TANT()...TANSTF of layer for hit
C  SINW()...sin(angle of wire) for hit
C  COSW()...cos(angle of wire) for hit
C  CHI2SV...chisquare saved to test for convergence
C  CHIBIG...rediculously large chisquare to start CHI2SV
C  ITRN.....iteration number; after exiting loop, equals last value used
C  NITMAX...maximum number of fitting iterations allowed,real time
C  NITMXD...maximum number of fitting iterations; default

C these are in the tftrakcd common
C  KAPNOW.... CU                     ,change after         2nd track
C  ALPNOW.... (1+2*CU*DA)*sin(FI)    ,change after kink or 2nd track
C  BETNOW....-(1+2*CU*DA)*cos(FI)    ,change after kink or 2nd track
C  GAMNOW.... DA*(1+CU*DA)           ,change after kink or 2nd track
C  XINOW..... (1+2*CU*DA)            ,change after kink
C  TANDNOW... CT; TAN(DIP ANGLE)     ,change after         2nd track
C  Y0NOW..... Z0; Z INTERCEPT
C  PHI6NOW... kink angle  at VD-DR interface
C  PHI7NOW... kink angle  in mid DR

C  DVARIB().computed excesses of "varib", to be subtracted from "varib"

C  TWOKAP...2*KAPPA                ,change after         2nd track
C  SPHI0....sin(phi 0 of the track),change after kink or 2nd track
C  CPHI0....cos(phi 0 of the track),change after kink or 2nd track
C  D0NOW...DA                     ,change after kink or 2nd track

C  DDDPH6...change in DA by change in kink angle, PHI6
C  KAPRK2...( kappa * rkink )**2; does track curl inside kink radius
C  DDDPH7...change in DA by change in kink angle, PHI7
C  L2NDTR...flag that the 2nd track has been encountered in the hit list
C  LAFTR6...flag that radius has progressed beyond the kink at VD-DR
C  LAFTR7...flag that radius has progressed beyond the kink in DR
C  PULMAX...maximum found residual**2 in the hit list
C  PL2MAX...preset for PULMAX for threshold for deleting hits
C  INDMAX...index, IFIT, value coresponding to the worst hit
C  TANTAU...TANSTF for layer, through TANT()
C  LSLANT...flag that this hit is on a stereo wire
C  LCATH....flag that this hit is on a cathode layer
C  LQDIV....flag that this hit is from charge division
C  LSVX.....flag that this hit is from silicon X side
C  LSVZ.....flag that this hit is from silicon Z side
C  SPHIW....sin(angle of wire), through SINW()
C  CPHIW....cos(angle of wire), through CINW()
C  XXW......EvntHit_X of wire, through XFIT()
C  YYW......EvntHit_Y of wire, through YFIT()
C  RRW......radius of wire, through RW()
C  RRWSQ....radius of wire, through RWSQ()
C  LSTR.....logical strip address
C  WFN......wafer number
C  IERR.....Silicon geometry error code
C  SPHI0S...SPHI0 saved to perform transformation
C  SPHI0W...sin(phi0 of track  -  angle of wire)
C  CPHI0W...cos(phi0 of track  -  angle of wire)
C  THETAW...XI * CPHI0W, used as correction to calculated drift distance
C  DELI.....distance, (track - wire)
C  XNOW.....current X location,                  transcendental iteration
C  YNOW.....current Y location,                  transcendental iteration
C  ZNOW.....current Z location,                  transcendental iteration
C  PSI......angle of arc of track
C  DXDZ.....change in x with z, along wire,      transcendental iteration
C  DYDZ.....change in y with z, along wire,      transcendental iteration
C  NTRY.....iteration number,                    transcendental iteration
C  MTRY.....maximum iteration number,            transcendental iteration
C  SPHI0P...parameter, close to sin(phi0),       transcendental iteration
C  CPHI0P...parameter, close to cos(phi0),       transcendental iteration
C  ZZ.......wanted  Z location from z projection,transcendental iteration
C  ZDIF.....change in Z, ZZ-ZNOW,                transcendental iteration
C  XDIF.....change in X, along wire,             transcendental iteration
C  YDIF.....change in Y, along wire,             transcendental iteration
C  XNEW.....new X location at z=ZZ,              transcendental iteration
C  YNEW.....new Y location at z=ZZ,              transcendental iteration
C  R2NEW....radius squared, XNEW**2+YNEW**2,     transcendental iteration
C  CIRC.....distance of point {xnew,ynew}-track, transcendental iteration
C  DYDX.....derivative dy/dx of point on track,  transcendental iteration
C  DXDY.....derivative dx/dy of point on track,  transcendental iteration
C  DCDX.....derivative d(circ)/dx,               transcendental iteration
C  DCDY.....derivative d(circ)/dy,               transcendental iteration
C  XDIF2....change in X, to put on track,        transcendental iteration
C  YDIF2....change in Y, to put on track,        transcendental iteration
C  DSPI.....incremental change in PSI,           transcendental iteration
C  ZZT......product ZZ*TANTAU, along stereo wire
C  PULL.....strength of residual, WGTFIT*PULFIT
C  DF(,)....derivative of function at hit, d(fitted distance)/d(variable)
C  ETA......sign change for back of curler
C  DSNUM....factor in numerator   of d(arc length)/d(miss distance)
C  DSDEN....factor in denominator of d(arc length)/d(miss distance)
C  DSDD0....derivative d(arc length)/d(miss distance)
C  SPTHLN...arc length
C  DSDK.....derivative d(arc length)/d(kappa)
C  DDELZ3...derivative d(DELI)/d(miss distance)
C  ITAKNK...iteration number to activate 2nd kink
C  RADKNK...radius of kink returned from TFKINK
C  RESKNK...weighted sum of residuals returned from TFKINK
C  ANGKNK...angle of kink returned from TFKINK
C  SUMWT....sum of weights for hits used
C  SUMWTA...sum of abs of weights for hits used, cut in TFKINK
C  RADKN2...radius of second best kink returned from TFKINK
C  RESKN2...weighted sum of residuals for second best returned from TFKINK
C  RESRAN...random weighted sums of residuals returned from TFKINK
C  THRSHK...threshold of RESKNK for activating 2nd kink
C  CHI2.....chi squared of residuals, including bias against kink
C  G()......column vector, cf BEVINGTON p246  -2*[beta]
C  GG().....square matrix (triagle form) cf BEVINGTON p246 2*[alpha] (linear)
C  GG2D(,)..GG expanded to square matrix, then inverse (mult of error matrix)
C  WGT2.....2 * WGTFIT for hit
C  IGG......index counting seqential position in the triangle matrix, GG
C  L........index for outer loop over rows of the triangle matrix
C  K........index for inner loop over columns of the triangle matrix
C  GYINV()..matrix of sigmas**2, including correlations from mult scat
C  GY().....inverse of GYINV
C  JFIT.....index for inner loop over hits, for hit correlations
C  IGY......index counting seqential position in the triangle matrix, GY
C  RESJ.....residual of hit j
C  DELCHI...contribution to CHI2 with multiple scattering
C  DELG.....contribution to G with multiple scattering
C  DELGG....contribution to GG with multiple scattering
C  TF3FLT...logical to specify that TF3FIT allows variable to float
C  SCALEM...scale factor for matrix elements to avoid MATIN1 overflow
C  TEMP()...space for the CERNiod routines RFACT and RFINV
C  IFAIL....return flag from CERNiod routines, 0:determinant is filled
C  DET......return determinant from CERNiod routines
C  CHI2S1...CHI2 saved from last iteration before 2nd kink is activated
C  CHI2S2...CHI2 saved from 2nd iteration after 2nd kink is activated
C  CURVS1...KAPPA saved from last iteration before 2nd kink is activated
C  CURVS2...KAPPA saved from 2nd iteration after 2nd kink is activated
C  PTOTS1...momentum saved from last iteration before 2nd kink is activated
C  PTOTS2...momentum saved from 2nd iteration after 2nd kink is activated
C  JFAIL....return flag from CERNiod routines, 0:determinant is not junk
C  TEMPDF(,)temporary matrix for approximate mult scattering treatment
C  VSMALL...very small number, to ignore trivial results
C  SUM......temporary sum in approximate mult scattering treatment
C
C real variables
C
      REAL RKINK6,SIG6,RKINK7,SIG7,RESI,CHI2SV,CHIBIG,R_S_BIG
      REAL TWOKAP
      REAL SPHI0,CPHI0,D0CFAB,PHI6,DDDPH6,KAPRK2,PHI7,DDDPH7
      REAL PULMAX,PL2MAX
      REAL TANTAU,SPHIW,CPHIW,XXW,YYW,RRW,RRWSQ
      REAL SPHI0S,SPHI0W,CPHI0W,THETAW,DELI
      REAL XNOW,YNOW,ZNOW,PSI,DXDZ,DYDZ,SPHI0P,CPHI0P
      REAL ZZ,ZDIF,XDIF,YDIF,XNEW,YNEW,R2NEW
      REAL CIRC,DYDX,DXDY,DCDX,DCDY,XDIF2,YDIF2
      REAL DPSI,ZZT,PULL
      REAL ETA,DSNUM,DSDEN,DSDD0,SPTHLN,DSDK,DDELZ3
      REAL RADKNK,RESKNK,THRSHK,CHI2S1,CHI2S2
      REAL CHI2,WGT2,RESJ,DELCHI,DELG,DELGG
      REAL SCALEM,DET,VSMALL,SUM,XXX
      REAL PHIW,MYPI

      REAL KADUM,ALDUM,BEDUM,GADUM,XIDUM
cryd
      REAL CHISVZ,NCHSVZ
      REAL FITWGT(100)
      INTEGER KL,USEFTRYD(100)
      REAL SPERP,RNOW,BETA_W,DELTA_W,XB,YB,U0B,UB,V0B,VB,X0B,Y0B
      REAL SPERHO,SPEPHI,SPERDA,DTCUR,DTPHI,DTDA
      REAL ARCFUN

C real arrays

      REAL  STMP(KLR2TF),    RW(KLR2TF), RWSQ(KLR2TF)
      REAL  SINW(KLR2TF),  COSW(KLR2TF), TANT(KLR2TF)
      REAL  DVARIB(NP3DTF)
      REAL  DF(NP3DTF,KLR2TF)
      REAL  G(NP3DTF),GG(NERMTF),GG2D(NP3DTF,NP3DTF)
      REAL GYINV(KGYMTF),GY(KGYMTF)
      REAL  TEMP(NP3DTF),TEMPDF(NP3DTF,KLR2TF)

C integer variables

      INTEGER IER,NITMIN,ITDRPH
      INTEGER IFIT,MP3DLO
      INTEGER NFSAVE,NDELET,NADDED,NHTSTR
      INTEGER RFIT,ILYRS,ILYR,IADR,IWADR,ITRN,NITMAX,NITMXD
      INTEGER INDMAX,NTRY,MTRY,ITAKNK,IGG,L,K,JFIT,IGY
      INTEGER IFAIL,JFAIL
      INTEGER LSTR,WFN,IERR

C integer arrays

      INTEGER NREGN(6),NFITT(2),REGNHT(KLR2TF)
      INTEGER IPNTR(KLR2TF)
      INTEGER IADTMP(KLR2TF),IPLTMP(KLR2TF),USETMP(KLR2TF)

C logical variables

      LOGICAL LKN6LO,LKN7LO,LKN7TM
      LOGICAL L2NDTR,LAFTR6,LAFTR7,LSLANT,LCATH,LQDIV,LSVX,LSVZ

C logical arrays

      LOGICAL TF3FLT(NP3DTF)

C function declarations

      REAL     CDWGT3,CDWGTD,CDWTQD,ATN2PI
      EXTERNAL CDWGT3,CDWGTD,CDWTQD,ATN2PI
      EXTERNAL TFXYHT

      INTEGER JBIT,IABS
      EXTERNAL JBIT

C temporary variables mainly for diagnostics of 2nd KINK

#if defined(CLEO_KINKDIAG)
      REAL ANGKNK,SUMWT ,SUMWTA,RADKN2,RESKN2,RESRAN(20)
      REAL CURVS1,CURVS2,PTOTS1,PTOTS2
      REAL CFIX
      COMMON/FIXER/CFIX(250)
#endif

C equivalence to large scratch arrays

#if defined(CLEO_VAX)
#include "doit/duseq/duscrt.inc"
      EQUIVALENCE (GOG(1),RW), (GOG(KLR2TF+1),RWSQ)
      EQUIVALENCE (GOG(2*KLR2TF+1),SINW),(GOG(3*KLR2TF+1),COSW)
      EQUIVALENCE (GOG(4*KLR2TF+1),TANT),(GOG(5*KLR2TF+1),STMP)
      EQUIVALENCE (GOG(6*KLR2TF+1),IPNTR),(GOG(7*KLR2TF+1),IADTMP)
      EQUIVALENCE (GOG(8*KLR2TF+1),IPLTMP),(GOG(9*KLR2TF+1),TEMPDF)
      EQUIVALENCE (GOG((9+NP3DTF)*KLR2TF + 1),DF)
      EQUIVALENCE (GOG((9+2*NP3DTF)*KLR2TF + 1),GYINV)
      EQUIVALENCE (GOG((9+2*NP3DTF)*KLR2TF + KGYMTF + 1),GY)
#endif

C local parameters

      PARAMETER( CHIBIG  =   1. E10   )
      PARAMETER( R_S_BIG =   99.      )
      PARAMETER( NITMXD  =   10       )
      PARAMETER( MTRY    =   2        )
      PARAMETER( PL2MAX  =   16.      )
      PARAMETER( THRSHK  =   0.000350 )
      PARAMETER( SCALEM  =   1. E-6   )
      PARAMETER( VSMALL  =   1. E-18  )

* ----------Executable code starts here---------------------

C initialization


      CALL STATUSBITS(kSTfillcf,kstTF3ent,1)
      IER = 0

C before getting started, check if there are enough hits

#if defined(CLEO_TF3FDIAG)
      PRINT 9701,NFIT,NHTQLF,MNHTCF,MNHTLF
 9701 FORMAT(' TF3FIT: entry, NFIT,NHITQLF,MNHTCF,MNHTLF=',4I5)
#endif

      IF (NFIT+NHTQLF.LT.(MNHTCF + MNHTLF)) THEN
          IER = 1
          GOTO 410
      ENDIF

C set up the minimum iterations and the iteration to allow dropping hits
C can change if there is a 2nd kink

C alternative setup for dual track calibration fitting
C do not allow dropping hits,
C do not add hits to fit
C will not use second kink (or call TFKINK) on a single track
C second kink variable will be used as kink on second track

C fill in extra hits in the DR not in the original fit
C reset IPLFIT negative for added hits
C note: CFHITA filld only IADFIT, IPLFIT, SFIT

#if defined(CLEO_TF3FDIAG)
      PRINT 9703,NFIT,DUALTR,LCALIB
 9703 FORMAT(' TF3FIT:before CFHITA, NFIT',I3,' DUALTR,LCALIB 'L1,X,L1)
#endif

      IF(DUALTR)LCALIB=.TRUE.
      IF(LCALIB)THEN
         NITMIN = 2
         ITDRPH = 99
         ITAKNK = 99
      ELSE
         NITMIN = 2
         ITDRPH = 2
         ITAKNK = 3
         CALL CFHITA
      ENDIF

C get the final fitting wire corrections

#if defined(CLEO_TF3FDIAG)
      PRINT 9704,NFIT,NITMIN,ITDRPH,ITAKNK
 9704 FORMAT(' TF3FIT:before CDCORF, NFIT',I3,
     2       ' NITMIN,ITDRPH,ITAKNK=',3I3)
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,' TF3FIT before CDCORF')
#endif
#endif

      CALL CDCORF(TFXYHT)


C set up the radius and sigma of kinks in the track

#if defined(CLEO_TF3FDIAG)
      PRINT 9705,NFIT,LKN6TF,LKN7TF,LKN6FI,LKN7CT
 9705 FORMAT(' TF3FIT: after CDCORF, NFIT',I3,
     2       ' LKN6TF,LKN7TF,LKN6FI,LKN7CT',4(X,L1))
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,' TF3FIT after CDCORF')
#endif
#endif

      IF(LKN6FI.OR.LKN7CT)GO TO 113

      LKN6LO=LKN6TF
      IF (LKN6LO) THEN
          RKINK6  = RSCATF(IKN6TF)
          SIG6   = SCTGEV*SQRT(XSCATF(IKN6TF))*EETF/P2TF
          MP3DLO = 6
          CALL STATUSBITS(kSTfillcf,kstTF3knk1allow,1)
      ELSE
          RKINK6  = R_S_BIG
          SIG6   = R_S_BIG
          MP3DLO = 5
      ENDIF

C for normal single track fitting, set up the second kink

      IF(DUALTR)GO TO 111
      RKINK7  = R_S_BIG
      SIG7   = R_S_BIG
      LKN7LO = .FALSE.
      LKN7TM = .FALSE.
      GO TO 115

C for calibration, dual track, fitting, set up the second kink
C to be a kink at the same radius as RKINK6 for the second subtrack

 111  LKN7LO=LKN6LO
      IF (LKN7LO) THEN
          RKINK7=RKINK6
          SIG7  =SIG6
          MP3DLO = MP3DLO + 1
          CALL STATUSBITS(kSTfillcf,kstTF3knk2activ,1)
      ELSE
          RKINK7  = R_S_BIG
          SIG7   = R_S_BIG
      ENDIF
      GO TO 115

C for special use of either of the kink variables
C can use kink 6 for a kink phi
C can use kink 7 for a kink in Z

113   LKN6LO = .FALSE.
      LKN7LO = .FALSE.
      MP3DLO = 5
      IF(LKN6FI)THEN
           RKINK6 = 0.00001
           SIG6 = .010
           MP3DLO = 6
           CALL STATUSBITS(kSTfillcf,kstTF3knk1allow,1)
      ELSE
           RKINK6 = R_S_BIG
           SIG6 = R_S_BIG
      ENDIF
      IF(LKN7CT)THEN
           RKINK7 = 0.00001
           SIG7 = .100
           MP3DLO = 7
           CALL STATUSBITS(kSTfillcf,kstTF3knk2activ,1)
      ELSE
           RKINK7 = R_S_BIG
           SIG7 = R_S_BIG
      ENDIF
 115  CONTINUE

#if defined(CLEO_TF3FDIAG)
      PRINT 9706,NFIT,LKN6TF,LKN7TF,LKN6FI,LKN7CT,
     2           RKINK6,SIG6,RKINK7,SIG7
 9706 FORMAT(' TF3FIT: setup kinks, NFIT',I3,
     2       ' LKN6TF,LKN7TF,LKN6FI,LKN7CT',4(X,L1),
     3       ' R6,sig6,R7,sig7=',4F10.6)
#endif


C refill /TFTRAK/, SEQ:TFTRAKCD
C reordering by increasing S, to get past the kink
C possibly delete some hits

      IF(DUALTR)GO TO 125
      CALL SORTZV(SFIT  , IPNTR , NFIT,1,0,0)
      GO TO 127
 125  CALL SORT2K(USEFIT,SFIT,IPNTR,NFIT,IER)
      IF(IER.EQ.0)GO TO 127
          IER=14
          GO TO 410
 127  CALL UCOPY( IADFIT, IADTMP, NFIT )
      CALL UCOPY( IPLFIT, IPLTMP, NFIT )
      CALL UCOPY( SFIT  , STMP  , NFIT )
      IF(LCALIB)
     +CALL UCOPY( USEFIT, USETMP, NFIT )

C this loop to count the number of hits in each catagory
C and create the charge division hits
C NFIT will now count the total number of points in the list
C NFSAVE holds the old value of NFIT, for purpose of the loop
C require layer to be specified useable in LU3DTF
C require that NFIT does not overfill the arrays with limit NLR2TF
C keep hits-added-but-not-in-fit (neg IPLTMP), but do not add charge div
C throw out VDEAST hits that were added in earlier passes

      NFSAVE = NFIT
      NFIT   = 0
      NDELET = 0
      NADDED = 0
      NHITCF = 0
      NHITLF = 0
      NHTSTR = 0
      NHTQLF = 0
      CALL VZERO(NREGN,6)
      CALL VZERO( REGNHT, KLR2TF )
      IF(DUALTR)THEN
         NFITT(1) = 0
         NFITT(2) = 0
      ENDIF

      DO 168 IFIT = 1,NFSAVE
         RFIT   = IPNTR(IFIT)
         ILYRS  = IPLTMP(RFIT)
         ILYR   = IABS(ILYRS)
         IF (LU3DTF(ILYR).AND.(NFIT.LT.KLR2TF))THEN
            IF(EvntHit_laWire(IABS(IADTMP(RFIT)))
     2          .LT.NWIRTF(ILYR))THEN
               NFIT   = NFIT+1

C check if this is an added hit
               IF(ILYRS.GT.0)THEN

C this is for standard use of a hit, including anodes and cathode
C charge division hits are added below
                  IF(.NOT.LCALIB)THEN

C CF layers
                     IF (ILYR.LE.MLYRCF)THEN

C delete the hit from the fit, leave in the hit list for residuals
                        IF(LRMVHT.AND.(USETMP(RFIT).NE.1))THEN
                           ILYRS = -ILYR
                        ELSE
                
C for single track fitting, there are three regions on the track
                           NHITCF = NHITCF + 1
                           IF(RTF(ILYR).LE.RKINK6)THEN
                              NREGN(1) = NREGN(1) + 1
                              REGNHT(NFIT)=1
                           ELSEIF(RTF(ILYR).LE.RKINK7)THEN
                              NREGN(2) = NREGN(2) + 1
                              REGNHT(NFIT)=2
                           ELSE
                              NREGN(3) = NREGN(3) + 1
                              REGNHT(NFIT)=3
                           ENDIF
                        ENDIF

C LF layers
                     ELSE
                        NHITLF = NHITLF + 1
                     ENDIF

C for calibration, dual track, fitting, there are two regions on each track
C ILYRS had better be >0 since CFHITA was not called,
C set ILYRS=-ILYRS for hits flagged "not in fit" by USEFIT
                  ELSE

C CF layers
                     IF (ILYR.LE.MLYRCF)THEN
                        IF(USETMP(RFIT).EQ.1)THEN
                           NFITT(1)=NFITT(1) + 1
                           NHITCF = NHITCF + 1
                           IF(RTF(ILYR).LE.RKINK6)THEN
                              NREGN(1) = NREGN(1) + 1
                              REGNHT(NFIT)=1
                           ELSE
                              NREGN(2) = NREGN(2) + 1
                              REGNHT(NFIT)=2
                           ENDIF
                        ELSEIF(DUALTR.AND.(USETMP(RFIT).EQ.2))THEN
                           NFITT(2)=NFITT(2) + 1
                           NHITCF = NHITCF + 1
                           IF(RTF(ILYR).LE.RKINK7)THEN
                              NREGN(3) = NREGN(3) + 1
                              REGNHT(NFIT)=3
                           ELSE
                              NREGN(4) = NREGN(4) + 1
                              REGNHT(NFIT)=4
                           ENDIF

C delete the hit from the fit, leave in the hit list for residuals
                        ELSE
                           ILYRS = -ILYR
                        ENDIF

C LF layers
                     ELSE
                        IF(USETMP(RFIT).EQ.1)THEN
C special outer cathode requirement for stereo layer calb check
                           IF(
     1                         EvntHit_Drift(IABS(IADTMP(RFIT)))
     2                         .LT.90.)THEN
                              NHITLF = NHITLF + 1
                              IF(IDVCTF(ILYR).EQ.IDRFT)NHTSTR=NHTSTR+1
                              NREGN(5) = NREGN(5) + 1
                              REGNHT(NFIT)=5
                           ENDIF
                        ELSEIF(DUALTR.AND.(USETMP(RFIT).EQ.2))THEN
                           NHITLF = NHITLF + 1
                           IF(IDVCTF(ILYR).EQ.IDRFT)NHTSTR=NHTSTR+1
                           NREGN(6) = NREGN(6) + 1
                           REGNHT(NFIT)=6
                        ELSE
C delete the hit from the fit, leave in the hit list for residuals
                           ILYRS = -ILYR
                        ENDIF
                     ENDIF
                  ENDIF
C resume common code for normal track fitting and calibration
               ENDIF

               IF(ILYRS.LT.0)NADDED=NADDED+1
               IPLFIT(NFIT) = ILYRS
               IADFIT(NFIT) = IADTMP(RFIT)
               SFIT  (NFIT) = STMP  (RFIT)
               IF(LCALIB.OR.LRMVHT) USEFIT(NFIT) = USETMP(RFIT)
          
C charge division hits added with layer number offset by QDIVAD
C only used for tracks that are found, in z, with charge division

               IF(
     1              (.NOT.LCALIB)            .AND.
     2              LQDVTF(ILYR)             .AND.
     4              (ILYRS.GT.0)             .AND.
     5              (NFIT.LT.KLR2TF)         )THEN
C     3              (JBIT(STATCF(2),27).NE.0).AND.
                  IADR = IABS(IADFIT(NFIT))
                  IF(EvntHit_NxtCls(IADR).GT.0)THEN
                     RESI = EvntHit_Z(IADR)-(Y0LF+SFIT(NFIT)*TANDLF)
                     IF ((WGTQTF(ILYR)*RESI**2).LT.PL2MAX)THEN
                        NHTQLF = NHTQLF + 1
                        NFIT   = NFIT + 1
                        IPLFIT(NFIT) = ILYR + QDIVAD
                        IADFIT(NFIT) = IADTMP(RFIT)
                        SFIT  (NFIT) = STMP  (RFIT)
                     ENDIF
                  ENDIF
               ENDIF

C add in the VD EAST hits to the hit list for the residual calculation
C calibration only, and require flag ; LADVDE
C west VD only

 165           IF(
     2         LCALIB  .AND. LADVDE   .AND. 
     3         (IDVCTF(ILYR).NE.IVDET)  )THEN
                  IWADR=INDXTF(ILYR)+EvntHit_laWire(IABS(IADFIT(NFIT)))
     2                 + NWIRTF(ILYR)
                  IADR=IABS(EvntHit_MapDet(IWADR))
                  IF(IADR.NE.0)THEN
                     IF (NFIT.LT.KLR2TF)THEN
                        NFIT   = NFIT + 1
                        NADDED = NADDED + 1
                        IPLFIT(NFIT) = -ILYR
                        IADFIT(NFIT) = IADR
                        IF(IADTMP(RFIT).LT.0)IADFIT(NFIT)=-IADFIT(NFIT)
                        SFIT  (NFIT) = STMP  (RFIT)
                        USEFIT(NFIT) = -IABS(USETMP(RFIT))
                     ENDIF
                  ENDIF
               ENDIF
C --- drop hit from list
            ENDIF
         ENDIF
 168   CONTINUE
C  ^--- end of loop over hits

C again test if there are enough hits in both r-phi and z

#if defined(CLEO_TF3FDIAG)
      PRINT 9707,NFIT,NHITCF,NHTSTR,NHITLF,NHTQLF,MNHTCF,MNHTLF
 9707 FORMAT(' TF3FIT: hit test, ',
     2 'NFIT,NHITCF,NHTSTR,NHITLF,NHTQLF,MNHTCF,MNHTLF=',7I5)
#endif

C LF hits
C do not test for LF hits if this is a calibration fit with z contrained
      IF(((.NOT.LCALIB).OR.FITFLT(4).OR.FITFLT(5)).AND.
     +   (NHITLF+NHTQLF).LT.MNHTLF)GO TO 177

C CF hits
      IF( NHITCF        .GE.MNHTCF)GO TO 175
      IF(LCALIB.AND.(NHTSTR.GE.MNHTCF))GO TO 175
      GO TO 177

C total hits
 175  IF( NFIT-NADDED   .LE.MP3DLO)GO TO 177
      GO TO 179

 177      IER = 2
          GOTO 410
 179  CONTINUE
C  ^--- number of hits is satisfied

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C test is there are sufficient hits on both sides of a kink to provide
C a measurement of the kink angle, and thus,  to allow the kink
#if defined(CLEO_TF3FDIAG)
      PRINT 9708,DUALTR,LCALIB,LKN6LO,LKN7LO,NREGN
 9708 FORMAT(' TF3FIT: region test, DUALTR,LCALIB,LKN6LO,LKN7LO=',
     2 4(X,L1),' NREGN 123456=',6I3)
#endif
      IF(DUALTR)GO TO 181

C this is for normal single track fitting

      IF(.NOT.LKN6LO)GO TO 189
cccc      IF(NREGN(1)         .LT.3)GO TO 183
cccc      IF(NREGN(2)+NREGN(3).LT.3)GO TO 183
      GO TO 189

C this is for calibration, dual track, fitting
C LKN6LO and LKN7LO should both be .FALSE. if LKN6FI or LKN7CT are .TRUE.

 181  IF(.NOT.LKN6LO)GO TO 182
      IF(NREGN(1)         .LT.3)GO TO 183
      IF(NREGN(2)         .LT.3)GO TO 183
 182  IF(.NOT.LKN7LO)GO TO 185
      IF(NREGN(3)         .LT.3)GO TO 184
      IF(NREGN(4)         .LT.3)GO TO 184
      GO TO 189

C kill the kink at RKINK6
C do not lower MP3DLO for calibration, dual track, fitting

 183  LKN6LO=.FALSE.
      RKINK6  = R_S_BIG
      SIG6   = R_S_BIG
      CALL STATUSBITS(kSTfillcf,kstTF3knk1kill,1)
      IF(DUALTR)GO TO 182
      MP3DLO = MP3DLO - 1
      CALL KABG2(KABG2_CLEAR,
     1     0,0,0,0, 
     2     0.,0.,0.,0.,0., 0., 
     3     KADUM, ALDUM, BEDUM, GADUM, XIDUM)
      GO TO 189

C for calibration, dual track, fitting,
C kill the kink at RKINK6 for the second track
C do not lower MP3DLO for calibration

 184  LKN7LO=.FALSE.
      RKINK7  = R_S_BIG
      SIG7   = R_S_BIG
      CALL STATUSBITS(kSTfillcf,kstTF3knk2kill,1)
      GO TO 189

C test if there are enough hits for kinks specified at the origin

 185  IF(.NOT.LKN6FI)GO TO 186
      IF(NFITT(1)         .LT.5)GO TO 187
      IF(NFITT(2)         .LT.5)GO TO 187
 186  IF(.NOT.LKN7CT)GO TO 189
      IF(NREGN(5)         .LT.3)GO TO 188
      IF(NREGN(6)         .LT.3)GO TO 188
      GO TO 189

C kill the phi kink at the origin
C do not lower MP3DLO for calibration, dual track, fitting

 187  LKN6FI = .FALSE.
      RKINK6  = R_S_BIG
      SIG6   = R_S_BIG
      CALL STATUSBITS(kSTfillcf,kstTF3knk1kill,1)
      GO TO 186

C kill the CT kink at the origin
C do not lower MP3DLO for calibration, dual track, fitting

 188  LKN7CT = .FALSE.
      RKINK7  = R_S_BIG
      SIG7   = R_S_BIG
      CALL STATUSBITS(kSTfillcf,kstTF3knk2kill,1)

C done with testing for enough hits
 189  CONTINUE
C
#if defined(CLEO_KINKDIAG)
      CFIX(11)=ABS(KAPCF)
      CFIX(12)=NFIT
      CFIX(13)=NHITCF
      CFIX(14)=NADDED
C-A Lyon-5-24-99      CALL MAKE
#endif

C loop through the hits currently in the fit list
C this loop is before the loop over fitting iterations
C set other values in /TFTRAK/, SEQ:TFTRAKCD
C {X,Y} is set to the wire position, even for stereo wires
C DFIT is set to the measured drift distance, signed by IADFIT
C also fill variables for hits-added-but-not-in-fit (neg IPLFIT)
C note: for dual track fitting:
C    this should all be track parameter independent

      DO 197 IFIT = 1, NFIT
          IADR       = IABS(IADFIT(IFIT))
          ILYR          = IABS(IPLFIT(IFIT))
          IF (ILYR.LT.100) THEN
            IF (IDVCTF(ILYR).NE.ISVX1.AND.
     +          IDVCTF(ILYR).NE.ISVZ1) THEN
              XFIT   (IFIT) = EvntHit_X(IADR)
              YFIT   (IFIT) = EvntHit_Y(IADR)
            ELSE
              CALL TFXYHT(IADR,ILYR,0.0,XFIT(IFIT),YFIT(IFIT),IERR)
            ENDIF
          ELSE
            XFIT(IFIT) = EvntHit_X(IADR)
            YFIT(IFIT) = EvntHit_Y(IADR)            
          ENDIF

          RWSQ   (IFIT) = XFIT(IFIT)**2 + YFIT(IFIT)**2
          RW     (IFIT) = SQRT(RWSQ(IFIT))
          SINW   (IFIT) = YFIT(IFIT)/RW(IFIT)
          COSW   (IFIT) = XFIT(IFIT)/RW(IFIT)
          ILYR          = IABS(IPLFIT(IFIT))

C Get the phi of the wire in the range 0-2PI for the weights

          MYPI=3.14159265
          PHIW=ABS(ASIN(SINW(IFIT)))
          IF((XFIT(IFIT).LT.0.0).AND.(YFIT(IFIT).GT.0.0))THEN
             PHIW=MYPI-PHIW
          ELSEIF((XFIT(IFIT).LT.0.0).AND.(YFIT(IFIT).LT.0.0))THEN
             PHIW=MYPI+PHIW
          ELSEIF((XFIT(IFIT).GT.0.0).AND.(YFIT(IFIT).LT.0.0))THEN
             PHIW=(2*MYPI)-PHIW
          ENDIF

          IF(ILYR        .GT.QDIVAD)GO TO 193
          IF(IDVCTF(ILYR).EQ.IVDCT )GO TO 195
          IF(IDVCTF(ILYR).EQ.IDRCT )GO TO 195
          IF(IDVCTF(ILYR).EQ.ISVX1) GO TO 196
          IF(IDVCTF(ILYR).EQ.ISVZ1) GO TO 1196

C distance measurement and fitting weight for anodes

          DFIT  (IFIT) = EvntHit_Drift(IADR)
          IF(IADFIT(IFIT).LT.0) DFIT(IFIT) = -DFIT(IFIT)
          TANT  (IFIT) = TANSTF(ILYR)
          WGTFIT(IFIT) = CDWGT3(ILYR,DFIT(IFIT),TANDLF,PHIW)
          GO TO 197

C distance measurement and fitting weight charge division
C this is compatable with the old CDWGTD which uses -ILYR  for qdiv

 193      DFIT  (IFIT) = EvntHit_Z(IADR)
          TANT  (IFIT) = TANSTF(ILYR-QDIVAD)
          WGTFIT(IFIT) = CDWTQD((ILYR-QDIVAD),IADR)
          GO TO 197

C distance measurement and fitting weight for cathodes

 195      DFIT  (IFIT) = EvntHit_Drift(IADR)
          TANT  (IFIT) = TANSTF(ILYR)
          WGTFIT(IFIT) = CDWGT3(ILYR,DFIT(IFIT),TANDLF,PHIW)
          GOTO 197
C fitting weight for silicon X
196       DFIT(IFIT)=0.0
          TANT(IFIT)=0.0
CRYD          WGTFIT(IFIT) = 1.0/(0.000012)**2
          WGTFIT(IFIT) = 1.0/(0.000025)**2
          GOTO 197

C fitting weight for silicon Z
1196      DFIT(IFIT)=EvntHit_Drift(IADR)
          TANT(IFIT)=0.0
CRYD          WGTFIT(IFIT) = 1.0/(0.000019)**2
          WGTFIT(IFIT) = 1.0/(0.000040)**2

 197  CONTINUE
C  ^--- end of loop over hits

C initialization for iterative fit
C fill CU,FI,DA from KABG
C the ...NOW variables change through the kinks
C while the ...CF/LF variables describe the track at the origin  
C a maximum of NITMAX (starting at NITMXD=10) iterations are allowed

#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,' TF3FIT iter init')
#endif


      CALL CDCFID(CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)
      CALL UCOPY(KAPCF,KAPNOW,NTKPAR)
      CHI2SV   = CHIBIG

C loop over iterations, now manual loop with NITMAX variable

      CALL STATUSBITS(kSTfillcf,kstTF3stLOOPit,1)
      NITMAX=NITMXD
      ITRN=0

C start the iteration

 200     ITRN=ITRN+1
         CALL STATUSBITS(kSTfillcf,kstTF3iter,ITRN)

C KAPCF,ALPCF,   are now the fitting variables
C KAPNOW,ALPNOW, are initialized as KAPCF,ALPCF
C *** LFCTHT used KAPCF and must be changed to recognize kinks;
C             ie, use KABG2 ****

         CALL CDKABG(CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)
         CALL UCOPY(KAPCF,KAPNOW,NTKPAR)

#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,' TF3FIT after CDKABG')
#endif

C fill other track variables

         TWOKAP = 2.0*KAPNOW
         SPHI0  = ALPNOW / XINOW
         CPHI0  =-BETNOW  / XINOW

         DDDPH6 = 0.0
         IF (LKN6LO) THEN
            KAPRK2 = ( KAPNOW * RKINK6 ) ** 2
            IF (KAPRK2.LT.1.0) DDDPH6 = -RKINK6*SQRT(1.0 - KAPRK2)
         ENDIF

         DDDPH7 = 0.0
         IF (LKN7LO) THEN
            KAPRK2 = ( KAPNOW * RKINK7 ) ** 2
            IF (KAPRK2.LT.1.0) DDDPH7 = -RKINK7*SQRT(1.0 - KAPRK2)
         ENDIF

         LAFTR6 = .FALSE.
         LAFTR7 = .FALSE.
         L2NDTR = .FALSE.

      PRINT 1001,ITRN,NFIT,NDELET,NADDED,NHITCF,NHITLF,NHTQLF,
     1          KAPNOW,XINOW,ALPNOW,BETNOW,GAMNOW,CHI2,CHI2SV
1001  FORMAT(' TF3FIT: START ITERATION:',I3/
     2       '                    NHIT:',I3/
     3       '           NDELET,NADDED:',I3,I4/
     4       '                  NHITCF:',I3/
     5       '                  NHITLF:',I3/
     6       '                  NHTQLF:',I3/
     7       '                  KAPNOW:',F9.3/
     8       '     XINOW,ALPNOW,BETNOW:',3F9.3/
     9       '                  GAMNOW:',F9.6/
     X       '             CHI2,CHI2SV:',2F15.2)


C set maximum pull to threshold to throw out hits

         PULMAX = PL2MAX
         INDMAX = 0

C recalculate the number in each region once RKINK7 is defined
C rely on TFKINK to demand enough hits on both sides on the 2nd kink

         IF(LKN7LO .AND. (ITRN.EQ.ITAKNK))THEN
            NREGN(1) = 0
            NREGN(2) = 0
            NREGN(3) = 0
            CALL VZERO( REGNHT, KLR2TF )
         ENDIF

C big loop over hits inside the iteration loop- fill hit variables
C hits with negative IPLFIT are either hits deleted from the fit
C for large residuals or are  hits-added-but-not-in-fit
C these must not be skiped at this point so that PULFIT
C can be calculated to search for kinks

         DO 269 IFIT=1,NFIT
            CALL VZERO( DF(1,IFIT), NP3DTF )
            ILYRS = IPLFIT(IFIT)
            ILYR  = IABS(ILYRS)

C identify stereo wire layers

            TANTAU = TANT(IFIT)
            LSLANT = TANTAU.NE.0.0

C identify charge division and cathode hits

            LQDIV  = .FALSE.
            LCATH  = .FALSE.
            LSVX   = .FALSE.
            LSVZ   = .FALSE.
            IF(ILYR        .GT.QDIVAD) LQDIV  = .TRUE.
            IF(IDVCTF(ILYR).EQ.IVDCT ) LCATH  = .TRUE.
            IF(IDVCTF(ILYR).EQ.IDRCT ) LCATH  = .TRUE.
            IF(IDVCTF(ILYR).EQ.ISVX1 ) LSVX   = .TRUE.
            IF(IDVCTF(ILYR).EQ.ISVZ1 ) LSVZ   = .TRUE.


C continue filling hit variables

            SPHIW  = SINW(IFIT)
            CPHIW  = COSW(IFIT)
            XXW    = XFIT(IFIT)
            YYW    = YFIT(IFIT)
            RRW    = RW(IFIT)
            RRWSQ  = RWSQ(IFIT)

C recalculate the number in each region once RKINK7 is defined

            IF(LKN7LO .AND. (ITRN.EQ.ITAKNK)
     2      .AND. (ILYRS.GE. 0) .AND. (ILYR .LE. MLYRCF))THEN
                  IF(RTF(ILYR).LE.RKINK6)THEN
                     NREGN(1) = NREGN(1) + 1
                     REGNHT(IFIT)=1
                  ELSEIF(RTF(ILYR).LE.RKINK7)THEN
                     NREGN(2) = NREGN(2) + 1
                     REGNHT(IFIT)=2
                  ELSE
                     NREGN(3) = NREGN(3) + 1
                  REGNHT(IFIT)=3
               ENDIF
            ENDIF

C change the track parameter values for the second subtrack

            IF(DUALTR .AND. (.NOT.L2NDTR) 
     2                .AND. (IABS(USEFIT(IFIT)).EQ.2))THEN
               L2NDTR = .TRUE.
               KAPNOW =-KAPCF
               ALPNOW =-ALPCF
               BETNOW =-BETCF
               GAMNOW =-GAMCF
C              XINOW  =+XICF
               TANDNOW=-TANDLF
C              Y0NOW is unchanged

C..... WHATS THIS FOR ??????

                    KAPCF  = KAPNOW
                    ALPCF  = ALPNOW
                    BETCF  = BETNOW
                    GAMCF  = GAMNOW
C.....

               TWOKAP = 2.0*KAPNOW
               SPHI0  = ALPNOW / XINOW
               CPHI0  =-BETNOW  / XINOW
               D0NOW =-D0CF
               LAFTR6 = .FALSE.
               LAFTR7 = .FALSE.

C change the values for a kink in PHI at the origin, test on LAFTR6 not needed

               IF(LKN6FI)THEN
                  LAFTR6 = .TRUE.
                  SPHI0S = SPHI0
                  SPHI0  = SPHI0 + PHI6NOW*CPHI0
                  CPHI0  = CPHI0 - PHI6NOW*SPHI0S
C                 D0NOW is unchanged because DDDPH6 is zero
C                 XINOW     is unchanged because D0NOW IS unchanged
                  ALPNOW  =  XINOW*SPHI0
                  BETNOW   = -XINOW*CPHI0
C                 GAMNOW  is unchanged because D0NOW IS unchanged
               ENDIF

C change the values for a kink in CT at the origin, test on LAFTR7 not needed

 233           IF(LKN7CT)THEN
                  LAFTR7 = .TRUE.
                  TANDNOW = TANDNOW-PHI7NOW
               ENDIF
            ENDIF

C correct cathode hits for new S on each iteration, added by SFS
C calculates SFIT from a call to ARCFUN
C calculates DFIT as ph weighted average Z of cathode cluster
C uses KAPCF etc.
C DELETED 9/17/92 BECAUSE WE STILL FIT ALL THE WAY AROUND FROM CDJOIN
C
C           IF (LCATH .AND. ITRN.GT.1) THEN
C              IADR   = IABS(IADFIT(IFIT))
C              CALL LFCTHT(IADR,ILYR,0.,SFIT(IFIT),DFIT(IFIT),IER)
C           ENDIF


            IF (LSVZ.AND.ITRN.GT.1) THEN
               IADR = IABS(IADFIT(IFIT))
               CALL XYSVX1(IADR,ILYR,SFIT(IFIT),XXX,IER)
            ENDIF

C change values once as pass through kink

            IF(LKN6LO .AND. (.NOT.LAFTR6) .AND. (RRW.GT.RKINK6)
     2      .AND. (.NOT.(DUALTR.AND.IABS(USEFIT(IFIT)).EQ.2)))THEN
               LAFTR6 = .TRUE.
               SPHI0S = SPHI0
               SPHI0  = SPHI0 + PHI6NOW*CPHI0
               CPHI0  = CPHI0 - PHI6NOW*SPHI0S
               D0NOW = D0NOW + PHI6NOW*DDDPH6
               XINOW     = 1. + TWOKAP*D0NOW
               ALPNOW  =  XINOW*SPHI0
               BETNOW   = -XINOW*CPHI0
               GAMNOW  = D0NOW*(1. + KAPNOW*D0NOW)
            ENDIF

            IF(LKN7LO .AND. (.NOT.LAFTR7) .AND. (RRW.GT.RKINK7)
     2      .AND. (.NOT.(DUALTR.AND.IABS(USEFIT(IFIT)).EQ.1)))THEN
               LAFTR7 = .TRUE.
               SPHI0S = SPHI0
               SPHI0  = SPHI0 + PHI7NOW*CPHI0
               CPHI0  = CPHI0 - PHI7NOW*SPHI0S
               D0NOW = D0NOW + PHI7NOW*DDDPH7
               XINOW     = 1. + TWOKAP*D0NOW
               ALPNOW  =  XINOW*SPHI0
               BETNOW   = -XINOW*CPHI0
               GAMNOW  = D0NOW*(1. + KAPNOW*D0NOW)
            ENDIF

C sin and cos(phi0 of track  -  angle of wire)

            SPHI0W = SPHI0*CPHIW - CPHI0*SPHIW
            CPHI0W = CPHI0*CPHIW + SPHI0*SPHIW
            THETAW = XINOW * CPHI0W

C calculate the residual for hit IFIT and store it in PULFIT
C different treatment for cathodes, charge division and anodes

C z-silicon

            IF(LSVZ)THEN
               PULFIT(IFIT)=DFIT(IFIT)-(Y0NOW+SFIT(IFIT)*TANDNOW)

C cathodes

            ELSEIF(LCATH)THEN
               PULFIT(IFIT)=DFIT(IFIT)-(Y0NOW+SFIT(IFIT)*TANDNOW)

C charge division

            ELSEIF(LQDIV)THEN
               PULFIT(IFIT)=DFIT(IFIT)-(Y0NOW+SFIT(IFIT)*TANDNOW)

C X-silicon

            ELSEIF(LSVX)THEN

C Get the logical strip

C-A Lyon-5-24-99               CALL SVWRLA(LSTR,0,
C                    +         EvntHit_CDlayr(IABS(IADFIT(IFIT)))-
C-A Lyon-5-24-99     +         IXLRCD(1,ISVX1)+1,
C                    +         EvntHit_laWire(IABS(IADFIT(IFIT))),IERR)

              LSTR = 1


C-A Lyon-10-19-99  DANGER - DANGER - the 2nd subscript is supposed to handle
C-                                   Z-ganging. This is FAKED for now
               WFN=SVLAWN(LSTR,1)
               CALL XYSVWF(ALPNOW,BETNOW,KAPNOW,GAMNOW,WFN,
     +              1,XNOW,YNOW,IERR)
               
               IF (IERR.LT.0) PRINT *,'RYD:TF3FIT NO CROSSING'
               
               PULFIT(IFIT)=SQRT((XNOW-XXW)**2+(YNOW-YYW)**2)
               DELI = KAPNOW*RRWSQ + ALPNOW*XXW + BETNOW*YYW + GAMNOW
               IF (DELI.GT.0.0) PULFIT(IFIT)=-PULFIT(IFIT)



C anodes

            ELSE
               DELI = KAPNOW*RRWSQ + ALPNOW*XXW + BETNOW*YYW + GAMNOW

C get Z for this stereo anode hit ...transcendental iteration
C must find {x,y,z} which simutaniously satisfy
C  psi=atan(|2k|(ay-bx)/(2k*(ax+by)+xi**2),  the arc length
C  z=z0 + CT/(2k) * psi,                     z position
C  x=x0 + TANSCD + sin(angle of wire) * z,   x position
C  y=y0 + TANSCD - cos(angle of wire) * z,   y position
C this treatment (5/8/92) differs from the previous method
C by moving the {x,y} point off the wire: onto the track

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
               IF (LSLANT)THEN
                  XNOW=XXW
                  YNOW=YYW
                  ZNOW=0.
                  PSI = SIGN(
     +                 ATN2PI(ABS(TWOKAP)*(ALPNOW*YNOW-BETNOW*XNOW),
     +                 TWOKAP *(ALPNOW*XNOW+BETNOW*YNOW)+XINOW**2)
     +                 ,KAPNOW)
                  DXDZ= TANTAU*SPHIW
                  DYDZ=-TANTAU*CPHIW
                  DO 247 NTRY=1,MTRY
                     SPHI0P =   TWOKAP*XNOW + ALPNOW
                     CPHI0P = -(TWOKAP*YNOW + BETNOW)
                     ZZ = Y0NOW + PSI*TANDNOW / TWOKAP
                     ZDIF=ZZ-ZNOW
                     XDIF=DXDZ*ZDIF
                     YDIF=DYDZ*ZDIF
                     XNEW=XNOW+XDIF
                     YNEW=YNOW+YDIF
                     R2NEW=XNEW**2+YNEW**2
                     CIRC=R2NEW*KAPNOW+XNEW*ALPNOW+YNEW*BETNOW+GAMNOW

C X>Y ; DERIVATIVE WITH RESPECT TO Y IS DOMINANT
                     IF(ABS(XNEW).GE.ABS(YNEW))THEN
                        DXDY=-YNEW/XNEW
                        DCDY= BETNOW +  ALPNOW * DXDY
                        YDIF2=-CIRC/DCDY
                        XDIF2=DXDY*YDIF2

C X<Y ; DERIVATIVE WITH RESPECT TO X IS DOMINANT
                     ELSE
                        DYDX=-XNEW/YNEW
                        DCDX=ALPNOW +   BETNOW *DYDX
                        XDIF2=-CIRC/DCDX
                        YDIF2=DYDX*XDIF2
                     ENDIF
                     
                     XNEW=XNEW+XDIF2
                     YNEW=YNEW+YDIF2
                     DPSI=   (TWOKAP)*( SPHI0P*(YDIF+YDIF2)
     +                                 +CPHI0P*(XDIF+XDIF2))
     +                                /(SPHI0P**2+CPHI0P**2)
                     PSI=PSI+DPSI
                     XNOW=XNEW
                     YNOW=YNEW
                     ZNOW=ZZ
 247              CONTINUE

C this is now the final z position of the track at this radius
C DELI is corrected for the new {x,y} point on the wire
C first correction is from angle between track and xy projection
C of the stereo wire
C second correction is from curvature of track

                  ZZ = Y0NOW + PSI*TANDNOW / TWOKAP
                  ZZT = ZZ * TANTAU
                 
C             RKK. Nov 5/92. BPG sees that the stereo wire and
C             cathode residuals improve when the SPHI0W factor
C             is omitted.
                  IF ( LSLEAC ) THEN
                     DELI = DELI + THETAW * ZZT
     +                    + SIGN(1.,-ZZT) * SPHI0W*KAPNOW*ZZT**2
                  ELSE
                     DELI = DELI + THETAW * ZZT
     +                    + KAPNOW*ZZT**2
                  ENDIF
               ENDIF
C now continue with common code for axial and stereo wires
C higher order correction to distance from point to track
C PULFIT is (hit-wire) - (track-wire) = (hit-track)

               DELI = DELI*(1.0 - KAPNOW*DELI)
               PULFIT(IFIT) = DFIT(IFIT) - DELI
            ENDIF

C now continue with common code for all hits, anode, cathode, Qdiv
C choose a hit with large residuals to delete

C!!!TEMP
C this traps certain fits that are blowing up that otherwise can
C lead to disaster.  It should be removed only when we have
C figured out how to fix the fitter.  SFS 11/7/90

              IF (ABS(PULFIT(IFIT)).GT.1.0) THEN
                  IER = 3 + IFIT*16
                  GOTO 410
              ENDIF
C!!!TEMP-end

              IF(ILYRS.LE.0)GO TO 259
                  PULL = WGTFIT(IFIT)*PULFIT(IFIT)**2
              IF (PULL.LE.PULMAX)GO TO 259
              IF (ABS(IPLFIT(IFIT)).LT.100) THEN
              IF (IDVCTF(IABS(IPLFIT(IFIT))).EQ.ISVX1.OR.
     +            IDVCTF(IABS(IPLFIT(IFIT))).EQ.ISVZ1) GO TO 259
              ENDIF

                  PULMAX = PULL
                  INDMAX = IFIT
 259          CONTINUE

C calculate derivatives
C these are ......d(fitted distance)/d(variable)
C which are the derivatives, with the proper sign, as in BEVINGTON p246

C  varib()..CU,FI,DA,CT,Z0,KINK_VDDR,KINK_DR for current iteration

C note: about fitting dual tracks: with the second track the derivatives
C       are calculated w.r.t the second track variable (CURNOW)
C       but is needed w.r.t CURCF, the first track variable.
C varib1=-curv  : d(fitted distance)/d(-curv)   = -d(fitted distance)/d(curv)
C varib2=phi0+pi: d(fitted distance)/d(phi0+pi) =  d(fitted distance)/d(phi0)
C varib3=-da    : d(fitted distance)/d(-da)     = -d(fitted distance)/d(da)
C varib4=-ct    : d(fitted distance)/d(-ct)     = -d(fitted distance)/d(ct)
C varib5; z0 is the same for both tracks
C note: about the kink angles and their signs in dual track use
C       the kink angles, PHI6NOW and PHI7NOW are local (not flipped) variables
C       in dual track use. They can be factorized as
C       d(fitdis)/d(kink angle)=d(fitdis)/d(phi0)*d(phi0)/d(kink angle)
C                              +d(fitdis)/d(da)  *d(da)/d(kink angle)
C       since         d(phi0)/d(kink angle)=1
C       and we define d(da)/d(kink angle)=DDDPHn
C       d(fitdis)/d(kink)=   DF(2)*1 + DF(3)*DDDPHn
C       Since the kink angle is the local track variable, DF(3) must be used
C       before the sign is changed for flipped tracks.
C
C lines labeled "C old" were inherited from before 1992
C lines labeled "C sfs" are from Steve Schaffner, 4/8/92, but never implemented
C lines labeled "C dpp" and
C lines labeled "C dpp(old)" alternative formulae from Dan Peterson, 11/3/92
C lines labeled "C dpp(dual)"are formulae changed for dual track use, 5/4/93
C note: regarding dpp vs sfs: D0NOW changes with kink, D0CF does not

              IF (LCATH)GO TO 261
              IF (LQDIV)GO TO 261
              IF (LSVZ) GO TO 261
              IF (LSVX) GO TO 262
              GO TO 263

C cathodes: measurement is z position of cluster
C           fitted z position is a function of arc length
C           so it is dependent on curvature and miss distance
C
C charge division: previously (5/11/92) the dependence
C           on arc length was ignored, but since
C           it is used predominantly on such large curvature,
C           it really should be treated like the cathodes

 261              CONTINUE
C--------------------------------------------------------------------------
C old.............DF(1,IFIT)  = TANDLF*RRW*(CURCF*RRWSQ/3.0 -
C    +                          D0CF)
C sfs.............DF(1,IFIT)  = TANDLF*RRW*(CURCF*RRWSQ/2.0 -
C    +                          D0CF)
C dpp..May04-93...
                  DF(1,IFIT)  = TANDNOW*RRW*(KAPNOW*RRWSQ/2.0 -D0NOW)
C--------------------------------------------------------------------------
C old.............DF(3,IFIT)  =-TANDLF*(CURCF*RRWSQ +
C    +                          D0CF)/ RRW
C sfs.............DF(3,IFIT)  =-TANDLF*(CURCF*RRWSQ +
C    +                          D0CF)/(RRW*(1. +
C    +                            AMAX1(-0.9,TWOKAP*D0CF)  ))
C dpp..May04-93...
                  DF(3,IFIT)  =-TANDNOW*(KAPNOW*RRWSQ + D0NOW)/ RRW
C--------------------------------------------------------------------------
                  DF(4,IFIT)  = SFIT(IFIT)
C--------------------------------------------------------------------------
                  DF(5,IFIT)  = 1.0
C--------------------------------------------------------------------------
C dpp(dual).......
                  IF (LKN7CT.AND.LAFTR7)
     +            DF(7,IFIT) = DF(4,IFIT)

C--------------------------------------------------------------------------
              GO TO 267


C SVX

  262         CONTINUE

C             Get the logical strip

C-A Lyon-5-24-99               CALL SVWRLA(LSTR,0,
C                    +         EvntHit_CDlayr(IABS(IADFIT(IFIT)))-
C-A Lyon-5-24-99     +         IXLRCD(1,ISVX1)+1,
C                    +         EvntHit_laWire(IABS(IADFIT(IFIT))),IERR)

              LSTR = 1

C-A Lyon-10-19-99  DANGER - DANGER - the 2nd subscript is supposed to handle
C-                                   Z-ganging. This is FAKED for now
              WFN=SVLAWN(LSTR,1)
              CALL XYSVWF(
     +             ALPNOW,BETNOW,KAPNOW,GAMNOW,WFN,1,XNOW,YNOW,IERR)

              IF (IERR.LT.0) PRINT *,'RYD:TF3FIT NO CROSSING der'

              SPERP=ARCFUN(KAPNOW,ALPNOW,BETNOW,XNOW,YNOW,XINOW)
              RNOW=SQRT(XNOW**2+YNOW**2)
              CALL WAFCOR(WFN,BETA_W,DELTA_W)
              XB=XNOW*COS(BETA_W)+YNOW*SIN(BETA_W)+DELTA_W
              YB=YNOW*COS(BETA_W)-XNOW*COS(BETA_W)
              U0B=COS(PHI0CF-BETA_W)
              V0B=SIN(PHI0CF-BETA_W)
              UB=U0B*COS(2*CURCF*SPERP)-V0B*SIN(2*CURCF*SPERP)
              VB=V0B*COS(2*CURCF*SPERP)+U0B*SIN(2*CURCF*SPERP)
              X0B=-D0CF*V0B-DELTA_W
              Y0B=-D0CF*U0B

              SPERHO=-(X0B+SPERP*UB)/(2*CURCF*UB)
              SPEPHI=(Y0B+ (U0B-UB)/(2*CURCF))  /UB
              SPERDA=V0B/UB

              DTCUR=2*(UB-U0B)/(2*CURCF)**2+
     +              2*(SPERP/(2*CURCF)+SPERHO)*VB
              DTPHI=DELTA_W+VB*SPEPHI
              DTDA=U0B+VB*SPERDA
C--------------------------------------------------------------------------
              DF(1,IFIT)=DTCUR
              DF(2,IFIT)=DTPHI
              DF(3,IFIT)=DTDA
              DF(4,IFIT)=0.0
              DF(5,IFIT)=0.0
C--------------------------------------------------------------------------
              GO TO 267

C anodes:

 263              CONTINUE
C--------------------------------------------------------------------------
C old.............DF(1,IFIT)  = RRWSQ
C sfs.............DF(1,IFIT)  = RRWSQ+2.*D0CF*RRW*SPHI0W
C dpp..Nov03-92...
                  DF(1,IFIT)  = RRWSQ+2.*D0NOW*RRW*SPHI0W+D0NOW**2
C--------------------------------------------------------------------------
C old.............DF(2,IFIT)  = RRW*CPHI0W
C sfs.............DF(2,IFIT)  = RRW*CPHI0W*(1.+TWOKAP*D0CF)
C dpp..Nov03-92...
                  DF(2,IFIT)  = RRW*THETAW
C--------------------------------------------------------------------------
C old.............DF(3,IFIT)  = 1.0+TWOKAP*RRW*SPHI0W
C sfs.............DF(3,IFIT)  = 1.0+TWOKAP*RRW*SPHI0W+TWOKAP*D0CF
C dpp..Nov03-92...
                  DF(3,IFIT)  = XINOW+TWOKAP*RRW*SPHI0W
C--------------------------------------------------------------------------
                  IF (.NOT.LSLANT)GO TO 265
                      ETA    = 1.
                      IF (ABS(PSI).GT.PI) ETA = -1.
                      DSNUM  = KAPNOW*RRWSQ + GAMNOW
                      DSDEN  = SQRT(ABS(RRWSQ*XINOW**2 - DSNUM**2))
C  TEMP! RKK Nov 2/92
C  We need to understand how this happens.
      IF ( DSDEN .EQ. 0.0 ) THEN
         WRITE(CHMESS,2001) NRUN, NEVENT
         CALL MESLOG ( 'TF3FIT', 1, MSERRO )
         IER = 9
         GOTO 410
 2001 FORMAT ( ' DSDEN = 0. RUN: ', I10, '  EVENT: ', I10 )
      ENDIF
                      DSDD0  = -ETA*DSNUM/(XINOW*DSDEN)
                      SPTHLN = PSI/TWOKAP
                      DSDK   = -SPTHLN/KAPNOW +
     +                         ETA*(1. + 1./XINOW)*RRWSQ/(TWOKAP*DSDEN)
                      DDELZ3 = (DSDD0*TANDNOW*TANTAU + TWOKAP*ZZT)*
     +                         CPHI0W
C--------------------------------------------------------------------------
C old.................
                      DF(1,IFIT) = DF(1,IFIT) +
     +                             DSDK*TANDNOW*CPHI0W*TANTAU
C dpp..May04-93.......DF(1,IFIT) = DF(1,IFIT) +
C    +                             DSDK*TANDNOW*THETAW*TANTAU
C    +                             + 2.*D0NOW*CPHI0W*ZZT
C--------------------------------------------------------------------------
C old.................
                      DF(2,IFIT) = DF(2,IFIT) - ZZT*SPHI0W
C dpp..May04-93.......DF(2,IFIT) = DF(2,IFIT) - ZZT*XINOW*SPHI0W
C--------------------------------------------------------------------------
                      DF(3,IFIT) = DF(3,IFIT) + DDELZ3
C--------------------------------------------------------------------------
C old.................DF(4,IFIT) = DF(4,IFIT) + SPTHLN*CPHI0W*TANTAU
C dpp..Nov03-92.......
                      DF(4,IFIT) = DF(4,IFIT) + SPTHLN*THETAW*TANTAU
C--------------------------------------------------------------------------
C old.................DF(5,IFIT) = DF(5,IFIT) + CPHI0W*TANTAU
C dpp..Nov03-92.......
                      DF(5,IFIT) = DF(5,IFIT) + THETAW*TANTAU
C--------------------------------------------------------------------------
 265      CONTINUE
C  ^--- end of section for stereo wires
C--------------------------------------------------------------------------
C note: about logical switches:
C       the derivative for the kink angles are only filled if the
C       LAFTRn switches are set; the latter are properly set for
C       dual track use in that kinks are only "on" for the correct track
C old....................DF(6,IFIT) = RRW*CPHI0W + DDDPH6*DF(3,IFIT)
C then added for stereo..DF(6,IFIT) = DF(6,IFIT) -
C    +                             ZZT*SPHI0W + DDDPH6*DDELZ3
C   it is equivalent to calculate it, at once, from the values
C   for DF(2) and DF(3), complete with stereo corrections, later
C dpp(dual).......
                  IF (LKN6LO.AND.LAFTR6)
     +            DF(6,IFIT)=DF(2,IFIT)+DDDPH6*DF(3,IFIT)
                  IF (LKN7LO.AND.LAFTR7)
     +            DF(7,IFIT)=DF(2,IFIT)+DDDPH7*DF(3,IFIT)
C dpp(dual).......
                  IF (LKN6FI.AND.LAFTR6)
     +            DF(6,IFIT) = DF(2,IFIT)
                  IF (LKN7CT.AND.LAFTR7)
     +            DF(7,IFIT) = DF(4,IFIT)
C--------------------------------------------------------------------------
 267      IF(.NOT.DUALTR)GO TO 269
          IF(.NOT.L2NDTR)GO TO 269
                  DF(1,IFIT)=-DF(1,IFIT)
                  DF(3,IFIT)=-DF(3,IFIT)
                  DF(4,IFIT)=-DF(4,IFIT)
          IF(LKN7CT.AND.LAFTR7)
     +            DF(7,IFIT)=-DF(7,IFIT)
 269      CONTINUE
C  ^--- end of loop over hits


C check for kinks
C Test for kink is done in iteration ITAKNK-1 with PULFIT freshly
C recalculated from the result of iteration ITAKNK-2.
C This iteration (ITAKNK-1) does not have the kink activated.
C Can not actually activate the kink until the end of this iteration.
C Test is done here to allow for blocking hit deletion until after
C the kink is in the fit.
      IF(LCALIB)GO TO 279
      IF(.NOT.LKN7TF)GO TO 279
      IF(ITRN.NE.(ITAKNK-1))GO TO 279
#if !defined(CLEO_KINKDIAG)
      CALL TFKINK(RADKNK,RESKNK)
#endif
#if defined(CLEO_KINKDIAG)
      CALL TFKINK
     +(RADKNK,RESKNK,ANGKNK,SUMWT ,SUMWTA,RADKN2,RESKN2,RESRAN)
      CFIX(31)=ABS(KAPCF)
      CFIX(32)=NFIT
      CFIX(33)=NHITCF
      CFIX(34)=NADDED
      CFIX(35)=PHI6NOW
      CFIX(41)=RADKNK
      CFIX(42)=ABS(RESKNK)
      CFIX(43)=ANGKNK
      CFIX(44)=ABS(SUMWT )
      CFIX(45)=SUMWTA
      CFIX(46)=RADKN2
      CFIX(47)=ABS(RESKN2)
      DO 275 L=1,20
 275  CFIX(50-1+L)=ABS(RESRAN(L))
#endif

C cuts on what tracks will try a second kink
C ABS(RESKNK)>.9 used to flag errors
C do not try to use 2nd kink if first is not allowed, matraces are clumsy
C allow more iterations if 2nd kink is added

      IF(ABS(RESKNK).LT. THRSHK)GO TO 279
      IF(ABS(RESKNK).GT. 0.99  )GO TO 279
      IF(ABS(CURCF).GT.1.)GO TO 279
      IF(.NOT.LKN6LO)GO TO 279
      LKN7TM=.TRUE.
      NITMIN=ITAKNK+1
      ITDRPH=ITAKNK+1
      NITMAX=ITRN+NITMXD
 279  CONTINUE

C delete a hit with big residuals
C do not delete hits for calibration
C try only if there is a hit to delete: INDMAX
C this hit was previously test for positive IPLFIT
C try only on and after the second iteration if there is no second kink
C try only after finishing an iteration after adding the second kink
C try only for a limited number of deletions: NDELTF=3 in DUETNL
C try only deleting the hit won't make the fit impossible:
C  MP3DLO is local,
C  MP3DTF=5,6,7 in TFGMI2; MNLFLF=3 and MNHTCF=4 are parameters,
C if there are kinks, try only if deleting hits will leave enough hits
C on each side of the kink

          IF(LCALIB                         )GO TO 299
          IF(INDMAX            .LE. 0       )GO TO 299
          IF(ITRN              .LT. ITDRPH  )GO TO 299
          IF(NDELET            .GE. NDELTF  )GO TO 299
          IF(NFIT-NDELET-NADDED.LT. MP3DLO+2)GO TO 299
          IF(NHITLF+NHTQLF     .LE. MNHTLF  )GO TO 299
          IF(NHITCF            .LE. MNHTCF  )GO TO 299
          IF(.NOT.(LKN6TF.OR.LKN7TF) )GO TO 283
          IF(REGNHT(INDMAX).LT.1     )GO TO 283
          IF(REGNHT(INDMAX).GT.3     )GO TO 283
          IF(NREGN(REGNHT(INDMAX)).LE.3     )GO TO 299

 283      ILYR   = IPLFIT(INDMAX)


C reset CHI2SV, to force another iteration after this one
C decrease the hit counter for this type of hit

          CHI2SV = CHIBIG
          NDELET = NDELET + 1
          IF(ILYR.GT.QDIVAD)GO TO 293
          IF(ILYR.GT.MLYRCF)GO TO 295
              NHITCF = NHITCF - 1
              GO TO 297
 293          NHTQLF = NHTQLF - 1
              GO TO 297
 295          NHITLF = NHITLF - 1

C delete the hit............
C IPLFIT is made negative to flag unused hit
C WGTFIT is zeroed for the matrices below
C        filled before loop over iterations
C PULFIT kept for testing for kinks
C             it is used without WGTFIT in full mult scat matrix
C             so , must test IPLFIT when using PULFIT
C IADFIT kept for calculations in the next iteration
C DF     kept for no good reason
C             it is used without WGTFIT in full mult scat matrix
C             so , will test IPLFIT anyway when using PULFIT
 297      IPLFIT(INDMAX) = -IPLFIT(INDMAX)
          WGTFIT(INDMAX) = 0.0
C          PULFIT(INDMAX) = 0.0
C          IADFIT(INDMAX) = 0
C          CALL VZERO(DF(1, INDMAX), NP3DTF)
      PRINT 2005,IPLFIT(INDMAX)
2005  FORMAT(' TF3FIT: DELETE HIT IN LAYER',I6)
 299     CONTINUE
C  ^--- branch point is no hit is deleted
      PRINT 2006
2006  FORMAT(' TF3FIT: MAYBE DELETED A HIT, THIS IS WHATS LEFT')
      DO 301 IFIT=1,NFIT
 301  PRINT 1002,IFIT,IPLFIT(IFIT),IADFIT(IFIT),
     +          SFIT(IFIT),PULFIT(IFIT)
1002  FORMAT(I5,I5,I7,F10.3,F12.6)

C now do CHI2 and matrices
C loop over all hits, but skip hits that are not in fit
C column matrix,      G(k), is  -2*[beta](k) from BEVINGTON p246
C GG is the triangular representation of
C square matrix, GG2D(j,k), which is 2*[alpha](j,k) BEVINGTON p246
C IMNVTF=0 is no multiple scattering,
C IMNVTF=1 is full covarient matrix  multiple scattering,
C IMNVTF=2 is no multiple scattering, followed by approximation to mult scat
C we are currently using IMNVTF=2 (5/12/92)

          CHI2 = 0.
          CALL VZERO( G     , NP3DTF )
          CALL VZERO( GG    , NERMTF )
          IF(LCALIB)GO TO 321
          IF (IMNVTF.EQ.1)GO TO 331

C no multiple scattering, or approximation to multiple scattering
C PULFIT is used only with WGTFIT, but will test on IPLFIT anyway

 321      CONTINUE
          DO 327 IFIT = 1, NFIT
              IF(IPLFIT(IFIT).LE.0)GO TO 327
              RESI = PULFIT(IFIT)
              WGT2 = 2.0*WGTFIT(IFIT)
              CHI2 = CHI2 + RESI**2 * WGTFIT(IFIT)
              IGG = 0
              DO 325 L = 1, MP3DLO
                  G(L) = G(L) - WGT2*RESI*DF(L,IFIT)
                  DO 323 K = 1 , L
                      IGG = IGG + 1
                      GG(IGG) = GG(IGG) + WGT2*DF(L,IFIT)*DF(K,IFIT)
 323              CONTINUE
C  ^--- end of inner loop over parameters
 325          CONTINUE
C  ^--- end of outer loop over parameters
 327      CONTINUE
C  ^--- end of loop over hits
          GO TO 341

C alternativley , do full covariant matrix of hits
C careful, TFCOV uses IPLFIT, will have ZEROed rows for unused hits
C TRSINV is happy to work around ZEROed rows

 331      CALL TFCOV(GYINV,ALPNOW,BETNOW,KAPNOW,GAMNOW,XINOW,TANDNOW)
          CALL TRSINV(GYINV, GY, NFIT)
          IGY = 0

C full multiple scattering error matrix
C guard against unused hits that now have non-zero PULFIT & DF even
C though it is always used with GY which will be zero for unused hits
C test on IPLFIT must come after incrementing IGY for compatability
C with matrix GY built in TFCOV

              DO 339 IFIT = 1, NFIT
                  RESI = PULFIT(IFIT)
                  DO 337 JFIT = 1, IFIT
                      IF(IPLFIT(IFIT).LE.0)GO TO 337
                      IF(IPLFIT(JFIT).LE.0)GO TO 337
                      IGY = IGY + 1
                      RESJ = PULFIT(JFIT)

                      DELCHI = RESI * RESJ * GY(IGY)
C-> Use symmetry of gy
                      IF (IFIT.NE.JFIT) DELCHI = 2.0 * DELCHI
                      CHI2 = CHI2 + DELCHI

                      IGG = 0
                      DO 335 L = 1, MP3DLO
                          DELG = GY(IGY) *
     +                          (RESI*DF(L,JFIT) + RESJ*DF(L,IFIT))
C-> Use symmetry
                          IF (IFIT.NE.JFIT) DELG = 2.0 * DELG
                          G(L) = G(L) - DELG
                          DO 333 K = 1, L
                              IGG   = IGG + 1
                              DELGG = GY(IGY)*
     +                               (DF(L,JFIT)*DF(K,IFIT) +
     +                                DF(L,IFIT)*DF(K,JFIT))
C-> Use symmetry
                              IF (IFIT.NE.JFIT) DELGG = 2.0 * DELGG
                              GG(IGG) = GG(IGG) + DELGG
 333                      CONTINUE
C  ^--- end of inner loop over parameters
 335                  CONTINUE
C  ^--- end of outer loop over parameters
 337              CONTINUE
C  ^--- end of inner loop over hits
 339         CONTINUE
C  ^--- end of outer loop over hits

C come here for both no multiple scattering
C and full covariant multiple scattering matrix

C this is a bias toward ---- PHI6NOW =0 , PHI7NOW =0------
C the kink angle is treated like a hit with experimentally measured value =0

 341      IF (.NOT.(LKN6LO.OR.LKN6FI))GO TO 343
              CHI2 = CHI2 + (PHI6NOW/SIG6)**2
              G(6) = G(6) + 2.*PHI6NOW/SIG6**2
              GG(21) = GG(21) + 2./SIG6**2
 343      IF (.NOT.(LKN7LO.OR.LKN7CT))GO TO 345
              CHI2 = CHI2 + (PHI7NOW/SIG7)**2
              G(7) = G(7) + 2.*PHI7NOW/SIG7**2
              GG(28) = GG(28) + 2./SIG7**2
 345      CONTINUE

C set up internal array to specify which variables are free in the fit
C the CERN inverter currently being used, RFACT/RFINV,  cannot handle
C zeroed rows/columns

          IF(.NOT.LCALIB)GO TO 357
             DO 355 L=1,NP3DTF
                IF(L.GT.MP3DLO)GO TO 353
                   IF((L.EQ.6).AND.(.NOT.(LKN6LO.OR.LKN6FI)))GO TO 353
                   IF((L.EQ.7).AND.(.NOT.(LKN7LO.OR.LKN7CT)))GO TO 353
                      IF(.NOT.DUALTR)GO TO 351
                         IF(.NOT.FITFLT(L))GO TO 353
 351              TF3FLT(L)=.TRUE.
                  GO TO 355
 353              TF3FLT(L)=.FALSE.
 355          CONTINUE
 357      CONTINUE

C scale, invert GG, recalculate parameters & get chi2
C .....column matrix, G(k), is  -2*[beta](k) from BEVINGTON p246
C .....GG is the triangular representation of
C .....square matrix, GG2D(j,k), which is 2*[alpha](j,k) BEVINGTON p246
C .....fitting involves solving the matrix equation GG*DVARIB=G
C......for the unknown vector matrix, DVARIB, which is (-) the needed change
C TRUPCK..(CERN)..unpack GG into square matrix GG2D
C VSCALE..(CERN)..multiply GG2D by SCALEM to avoid overflows in inverse
C TF3INV..(DUET)..pulls out unused columns and rows using TF3FLT,
C                 calls RFACT/RFINV,
C                 then restores zeroes in unused colums and rows.
C               ..IFAIL=5 if MP3DLO.GT.NP3DTF
C ..RFACT.(CERN)..prepare for RFINV, MP3DLO upper corner of NP3DTF square
C               ..IFAIL=0 is good, =-1 is indicates singular matrix
C               ..JFAIL=0 is good, =-1 is determinant to small, +1 to large
C ..RFINV.(CERN)..invert matrix GG2D, stick it in GG2D  (overwrite GG2D)
C               ..does not accept zeroed columns&rows
C TRPCK...(CERN)..pack square matrix GG2D**-1 into triangle ERMFIT
C VSCALE..(CERN)..remove the previous scale factor from error matrix ERMFIT
C SMXVEC..(DUET)..calculate changes to parameters, ERMFIT*G, standard fitting
C VSUB....(CERN)..subtract DVARIB to get new varib (G was defined negative)
C VSCALE..(CERN)..remove factor of 2 from ERMFIT (G and GG both defined *2)
C in the past, have used these alternate routines, don't know why the change
          CALL TRUPCK( GG    , GG2D  , NP3DTF )
          CALL VSCALE( GG2D  , SCALEM, GG2D  , NP3DTF*NP3DTF )
      IF(LCALIB)GO TO 361
          CALL RFACT(MP3DLO,GG2D,NP3DTF,TEMP,IFAIL,DET,JFAIL)
      GO TO 362
 361      CALL TF3INV(MP3DLO,GG2D,NP3DTF,TEMP,IFAIL,DET,JFAIL,TF3FLT)
 362           IF(IFAIL.NE. 0)GO TO 375
               IF(JFAIL.LE.-1)GO TO 377
               IF(JFAIL.GE. 1)GO TO 379
      IF(LCALIB)GO TO 363
          CALL RFINV(MP3DLO,GG2D,NP3DTF,TEMP)
 363      CALL TRPCK ( GG2D  , ERMFIT, NP3DTF )
          CALL VSCALE( ERMFIT, SCALEM, ERMFIT, NERMTF )
          CALL SMXVEC( ERMFIT,     G , DVARIB, MP3DLO )
          CALL VSUB  ( CURCF , DVARIB, CURCF , MP3DLO )
          CALL VSCALE( ERMFIT,    2.0, ERMFIT, NERMTF )

C calculate the changes to CHI2, using a 2nd order Taylor expansion
C DEL(CHI2)=sum(i)(
C                  d(CHI2)/d(varib(i))
C                 +1/2 sum(j)(d2(CHI2)/(d(varib(i) d(varib(j)) * DEL(varib(j))
C                     * DEL(varib(i)) )
C given the matrices used above, this becomes
C DEL(CHI2)= -G * DVARIB + 1/2*(GG*DVARIB)*DVARIB
C but, by definition, GG*DVARIB=G, so, DEL(CHI2)= -1/2 * G * DVARIB
C this was tested 7/6/92: gives identical results
C          CALL SMXVEC(GG    , DVARIB, TEMP  , MP3DLO)
          DO 369 L=1,MP3DLO
 369          CHI2= CHI2 - 0.5 *G(L)*DVARIB(L)

C test if fit went off the rails?

          IF (ABS(CHI2).LE.CHIBIG)GO TO 371
              IER = 7
              GO TO 410
 371      CONTINUE

#if defined(CLEO_DECS)||defined(CLEO_UNIX)

C Did the fit return a NaN?

          IF ((CHI2.LE.0.0.OR.CHI2.GE.0.0))GO TO 373
              IER = 8
              GO TO 410
 373      CONTINUE
#endif
          GO TO 381

C handle errors from RFACT: singular matrix, etc.
 375      IER = 4
          GO TO 410
 377      IER = 5
          GO TO 410
 379      IER = 6
          GO TO 410

C activate the kink for the next iteration

 381  IF(LCALIB)GO TO 385
      IF(.NOT.LKN7TM)GO TO 385
      IF(ITRN.NE.(ITAKNK-1))GO TO 385
      CHI2S1=CHI2
      CALL UCOPY(KAPCF, KAPTMP, NTKPAR)
      RKINK7  = RADKNK
      SIG7   = SCTGEV*SQRT(XSCATF(IKN7TF))*EETF/P2TF
      MP3DLO = MP3DLO + 1
      LKN7LO = .TRUE.
      CALL STATUSBITS(kSTfillcf,kstTF3knk2activ,1)
#if defined(CLEO_KINKDIAG)
      CURVS1=CURCF
      PTOTS1=.225/ABS(CURCF)*SQRT(1.+TANDNOW**2)
      PRINT 3001,RESKNK,ANGKNK,PHI6NOW,CHI2
#endif
 385  CONTINUE

C test the results of the kink

      IF((.NOT.LCALIB) .AND. LKN7LO .AND. (ITRN.EQ.ITAKNK+1))THEN
         CHI2S2=CHI2
#if defined(CLEO_KINKDIAG)
         CURVS2=CURCF
         PTOTS2=.225/ABS(CURCF)*SQRT(1.+TANDNOW**2)
         CFIX(81)=CHI2S1
         CFIX(82)=CHI2S2
         CFIX(83)=CHI2S2-CHI2S1
         CFIX(84)=PHI7NOW
         CFIX(85)=PHI7NOW-ANGKNK
         CFIX(86)=PHI6NOW
         IF(ABS(CURVS1).GT.0.00001)CFIX(87)=(CURVS2-CURVS1)/CURVS1
         CFIX(88)=PTOTS1
         CFIX(89)=PTOTS2
C-A Lyon-5-24-99         CALL MAKE
      PRINT 3002,PHI6NOW,PHI7NOW,CHI2,PTOTS1,PTOTS2
 3001 FORMAT(' '/' '/
     1       ' TF3FIT: PREPARE FOR EXTRA KINK'/
     2       '      RESKNK=',F9.6,' ANGKNK=',F9.6,
     3       '        PHI6NOW=',F9.6,'   CHI2=',F6.0)
 3002 FORMAT(' TF3FIT: FINISHED  EXTRA KINK'/
     1       '        PHI6NOW=',F9.6,' PHI7NOW=',F9.6,
     2       '                      CHI2=',F6.0,
     3       '  P=',2F6.3)
#endif

C remove kink if CHI2 increases
C  require at least on more iteration before testing for convergence
C  allow for more iterations, like it is starting over

         IF(CHI2S2.GT.CHI2S1)THEN
            CALL UCOPY(KAPTMP, KAPCF, NTKPAR)
            CHI2=CHI2S1
            NITMIN=ITRN+1
            ITDRPH=ITRN+1
            NITMAX=ITRN+NITMXD
            RKINK7  = R_S_BIG
            SIG7   = R_S_BIG
            MP3DLO = MP3DLO - 1
            LKN7LO=.FALSE.
            CALL STATUSBITS(kSTfillcf,kstTF3knk2kill,1)
#if defined(CLEO_KINKDIAG)
      PRINT 3003,CHI2S1,CHI2S2
3003  FORMAT(' TF3FIT: REMOVE EXTRA KINK: CHI2S1=',F6.0,
     1                                 '  CHI2S2=',F6.0)
#endif
         ENDIF
      ENDIF

C test for convergence DCH2TF=.05 IN DUETNL (5/13/92)

      IF(ITRN.LT.NITMIN)GO TO 398
      IF (ABS(CHI2-CHI2SV).LT.DCH2TF)GO TO 531
 398  CHI2SV = CHI2

C end of loop over iterations
      IF(ITRN.LT.NITMAX)GO TO 200

C no convergence, complete NITMAX iterations without convergence

      IER = 15
      CALL STATUSBITS(kSTfillcf,kstTF3notCONV,1)

C there was an error, either no convergence or somewhere else
 410  CONTINUE
      GO TO 920


C test for convergence passed within NITMAX iterations; store results

 531      CONTINUE

#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,' TF3FIT after converge')
#endif

          IF(LCALIB)GO TO 569
          IF (IMNVTF.NE.2)GO TO 569

C compute error matrix of parameters according to P. Avery
C cf CBX 87-38,39, and especially CBX 87-40
C ERMFIT = ERMFIT * DF(T) * GY0 * GYINV * GY0 * DF * ERMFIT
C where DF  is the derivatives
C GY0 is approx covariance matrix of params;  1/SIGMA**2

C compute covariance matrix of hits
C careful, TFCOV uses IPLFIT, will have ZEROed rows for unused hits

          CALL TFCOV(GYINV,ALPNOW,BETNOW,KAPNOW,GAMNOW,XINOW,TANDNOW)

C fill vector TEMPDF = GY0 * DF

          CALL VZERO(TEMPDF, NP3DTF*KLR2TF )
          DO 535 IFIT  = 1, NFIT
              DO 533 L = 1, MP3DLO
                  TEMPDF(L,IFIT) = WGTFIT(IFIT) * DF(L,IFIT)
 533          CONTINUE
 535      CONTINUE

C now overwrite DF = ERMFIT * TEMPDF and TEMPDF = GYINV  * DF

          CALL TRAS( TEMPDF , ERMFIT, DF    , NFIT  , NP3DTF )
          CALL TRSA( GYINV  , DF    , TEMPDF, NFIT  , NP3DTF )

C finally ERMFIT = DF * TEMPDF

          IGG = 0
          DO 557 L = 1, MP3DLO
              DO 551 IFIT = 1, NFIT
                  IF (ABS(    DF(L,IFIT)).LT.VSMALL)DF(L,IFIT)=0.
                  IF (ABS(TEMPDF(L,IFIT)).LT.VSMALL)TEMPDF(L,IFIT)=0.
 551          CONTINUE
C use symmetry
              DO 555 K = 1, L
                  IGG = IGG + 1
                  SUM = 0.0
                  DO 553 IFIT = 1, NFIT
                      SUM = SUM + DF(L,IFIT) * TEMPDF(K,IFIT)
 553              CONTINUE
                  ERMFIT(IGG) = SUM
 555          CONTINUE
C  ^--- end of inner loop over parameters
 557      CONTINUE
C  ^--- end of outer loop over parameters
 569      CONTINUE
C  ^--- branch point if approximate multiple scattering is skipped

C fill /TFTRAK/
          IF(LKN7LO)THEN
             RKN7CF = RKINK7
          ELSE
             RKN7CF = 9.99
          ENDIF
          
          CALL CDKABG(CURCF,PHI0CF,D0CF,KAPCF,ALPCF,BETCF,GAMCF,XICF)
          
#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK('CF',0,' TF3FIT after "fill"')
#endif

C chisq/degree of freedom

              CHITF  = CHI2/FLOAT(NFIT-NDELET-NADDED-MP3DLO)
C-A Lyon-5-24-99              CALL TF3DIAG ( NFIT-NDELET-NADDED,
C     +                       NFIT-NDELET-NADDED-MP3DLO,
C     +                       NDELET-NADDED )

C!!!TEMP
C              DO 583 I=1,MP3DLO
C                  ERMFIT(I*(I+1)/2) = AMAX1(ERMFIT(I*(I+1)/2),0.)
C 583          CONTINUE
C!!!TEMP

C             CALL HF1( 1, FLOAT(ITRN), 1.0 )
      CALL STATUSBITS(kSTfillcf,kstTF3yesCONV,1)
      CALL STATUSBITS(kSTfillcf,kstTF3hitsdel,NDELET)
C return with no error

 920  CONTINUE

C repack the output commons to remove QDIV and deleted hits

      IF(LCALIB)GO TO 960
      NFSAVE = NFIT
      NFIT   = 0
      NHITCF = 0
      NHITLF = 0
      NHTQLF = 0
      KBQDIV = 0

      DO 959 IFIT=1,NFSAVE
          ILYR   = IPLFIT(IFIT)
          IF(ILYR.LE. 0    )GO TO 959
          IF(ILYR.GT.QDIVAD)GO TO 953
          IF(ILYR.GT.MLYRCF)GO TO 955
              NHITCF = NHITCF + 1
              GO TO 957
 953          NHTQLF = NHTQLF + 1
              CALL SBIT1 ( KBQDIV, IXSFTF(ILYR-100))
              IF ( IER .EQ. 0 ) GO TO 957
              GOTO 959
 955          NHITLF = NHITLF + 1
 957      NFIT         = NFIT + 1
          IPLFIT(NFIT) = IPLFIT(IFIT)
          IADFIT(NFIT) = IADFIT(IFIT)
          WGTFIT(NFIT) = WGTFIT(IFIT)
          PULFIT(NFIT) = PULFIT(IFIT)
          XFIT  (NFIT) = XFIT  (IFIT)
          YFIT  (NFIT) = YFIT  (IFIT)
          SFIT  (NFIT) = SFIT  (IFIT)
          DFIT  (NFIT) = DFIT  (IFIT)

C         Clone the extra information that is created by Kalman.
C         The 1.E-3 is just a safe value.
          MESFIT(NFIT) = DFIT  (IFIT)
          IF ( WGTFIT(NFIT) .GT. 0. ) THEN
             SG1FIT(NFIT) = 1./SQRT(WGTFIT(NFIT))
          ELSE
             SG1FIT(NFIT) = 1.E-3
          ENDIF
          SG2FIT(NFIT) = 1.E-3

 959  CONTINUE
 960  CONTINUE

C     Tell the world that the QDIV hits, if any, are in the list.
      IF ( IER .EQ. 0 ) CALL SBIT1 ( STATCF(4), 20 )

#if defined(CLEO_KINKDIAG)
      PRINT 3004,PHI6CF,PHI7CF,CHI2
3004  FORMAT(' TF3FIT: DONE: PHI6=',F9.6,' PHI7=',F9.6,
     1                  '  CHI2=',F6.0)
#endif

C     Clear no. of hits if there was an error
      CALL STATUSBITS(kSTfillcf,kstTF3err,IER)
      IF (IER.EQ.0) THEN
          NHITTF = NFIT
      ELSE
          NHITTF = 0
          RKN7CF = 9.91
      ENDIF

C save the last fitted values in NHIT3F, CHI3F for use by the stak ordering
      NHIT3F=NHITTF
      CHI3F=CHITF


 990  CONTINUE
      RETURN
      END
