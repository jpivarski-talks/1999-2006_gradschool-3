*
* $Id: tfstakcd.inc,v 1.10 2002/09/05 19:17:37 dpp Exp $
*
* $Log: tfstakcd.inc,v $
* Revision 1.10  2002/09/05 19:17:37  dpp
*      -> add names for substaks
*
* Revision 1.9  2002/05/23 14:27:03  dpp
*      -> define new sub-stak limits
*      -> remove the sub-stak dimension from the storage area
*      -> stak is now structured as sub-staks in a global stak
*          rather than having the stak be multidimensioned
*
* Revision 1.8  2001/11/19 23:16:20  dpp
*      -> comments for new TFTRAKCD variables
*
* Revision 1.7  2000/12/19 23:28:01  dpp
*      -> update comments about stak position for TFTRAK variables
*
* Revision 1.6  2000/10/02 21:15:58  dpp
*     -> add stak for program indicator
*
* Revision 1.5  2000/08/18 18:33:49  dpp
*      -> update comments about stak position for TFTRAK variables
*
* Revision 1.4  2000/01/24 19:46:05  dpp
*      -> add to common: TMPKEY,  the saturated NHIT key
*
* Revision 1.3  1999/12/09 21:01:23  dpp
* deweight variable in stak
*
* Revision 1.2  1999/08/03 22:01:58  dpp
* add 2nd index
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
*
*
* Track stack sequence
#if !defined(CLEO_NOCOMDOC)
* /TFSTAK/ Contains the stack of TF tracks passing the chi2 cut
*
* NSTKTF             Number of tracks in each of the sub-stacks
* ISTKTF             Current global stak track in each of the sub-stacks
* IFSTK              First global stak track in each of the sub-stacks
* GSTKLOCL           global stak location of the LOW end of the substack
* GSTKLOCM           global stak location of the MAX end of the substack
* LENSSTK            length of the substack, taken from parameter 
* NAMSSTK            name of substak
*
* NXTSTK(ISTK)       Pointer to the next track in the stack
* LFHKEY(ISTK)       Last   sorting key = 1 if NHITLF > 0
* TMPKEY(ISTK)       alternate to Second sorting key = NHITTF
* NHTKEY(ISTK)       Second sorting key = NHITTF
* CHIKEY(ISTK)       First  sorting key = CHITF
*
* PRMSTK( 1,ISTK)    KAPPA
* PRMSTK( 2,ISTK)    ALPHA
* PRMSTK( 3,ISTK)    BETA
* PRMSTK( 4,ISTK)    GAMMA
* PRMSTK( 5,ISTK)    XI
* PRMSTK( 6,ISTK)    CUR
* PRMSTK( 7,ISTK)    PHI0
* PRMSTK( 8,ISTK)    D0
* PRMSTK( 9,ISTK)    TANDLF
* PRMSTK(10,ISTK)    Y0LF
* PRMSTK(11,ISTK)    PHI6
* PRMSTK(12,ISTK)    PHI7
* PRMSTK(13,ISTK)    RKN6
* PRMSTK(14,ISTK)    RKN7
* PRMSTK(15,ISTK)    SKN6
* PRMSTK(16,ISTK)    SKN7
* IPMSTK(17,ISTK)    KNK6CF kink enable flag 
* IPMSTK(18,ISTK)    KNK7CF kink enable flag
* IPMSTK(19,ISTK)    FTK6CF  - fit kink enable flag
* IPMSTK(20,ISTK)    FTK7CF  - fit kink enable flag

* IPMSTK(21,ISTK)    NHITCF (INTEGER not real value !)
* PRMSTK(22,ISTK)    CHICF
* IPMSTK(23,ISTK)    NHTQLF (INTEGER not real value !)
* IPMSTK(24,ISTK)    NHITLF (INTEGER not real value !)
* PRMSTK(25,ISTK)    CHILF
* IPMSTK(26,ISTK)    NHIT3F (INTEGER not real value !)
* PRMSTK(27,ISTK)    CHI3F
* IPMSTK(28,ISTK)    NHITTF (INTEGER not real value !)
* PRMSTK(29,ISTK)    CHITF
* PRMSTK(30,ISTK)    CH2RCCF - Chi2, recalculated in FIT_WHATEVER
* PRMSTK(31,ISTK)    TLRESCF - average time like residual
* PRMSTK(32,ISTK)    NLFDRA  - numb lyrs rep'ed in fit, drift chambs, axial
* PRMSTK(33,ISTK)    NLFDRU  - numb lyrs rep'ed in fit, drift chambs, +stereo
* PRMSTK(34,ISTK)    NLFDRV  - numb lyrs rep'ed in fit, drift chambs, -stereo
* PRMSTK(35,ISTK)    PPTF
* PRMSTK(36,ISTK)    P2TF
* PRMSTK(37,ISTK)    EETF
* PRMSTK(38,ISTK)    AM2TF
* IPMSTK(39,ISTK)    LCRLTF
* IPMSTK(40,ISTK)    Z0BIAS - sigma of Z0 origin used in the fit
* IPMSTK(41,ISTK)    D0_CONS - .TRUE. if D0 is to constrained to =0 
* IPMSTK(42,ISTK)    KBQDIV, bitmask of qdiv layers used in fit.
* IPMSTK(43,ISTK)    STATCF(1), status bits
* IPMSTK(44,ISTK)    STATCF(2), status bits
* IPMSTK(45,ISTK)    STATCF(3), status bits
* IPMSTK(46,ISTK)    STATCF(4), status bits
* IPMSTK(47,ISTK)    Number of degrees of freedom
* IPMSTK(48,ISTK)    Circle Fit Error Matrix elements are Current
*
* IPLSTK(IHIT,ISTK)  Physical layer #   for hit # IHIT
* IADSTK(IHIT,ISTK)  Address in /CDSCRT/ of hit # IHIT
* SRCSTK(IHIT,ISTK)  source program of hit # IHIT
* DWTSTK(IHIT,ISTK)  deweight amount
* SLRSTK(IHIT,ISTK)  S = arc length of track to layer of hit # IHIT
* PULSTK(IHIT,ISTK)  Pulls for tracks in the stack
* MESSTK(IHIT,ISTK)  Measurement for this hit
* SG1STK(IHIT,ISTK)  Expected resolution for this hit
* SG2STK(IHIT,ISTK)  Error on residual from extrapolation of track
* ERMSTK(IERM,ISTK)  Error matrix for the fit track
#endif

* These parameters define the position of variables in the stack.
c      INTEGER JKAPPA, JALPHA, JBETA,  JGAMMA, JXI
c      INTEGER JHITCF, JCHICF, JTANLF, JY0LF,  JHTQLF
c      INTEGER JHITLF, JCHILF, JCUR,   JPHI0,  JD0
c      INTEGER JPHI6,  JRKN6,  JPHI7,  JRKN7,  JHIT3F, JCHI3F
c      INTEGER JHITTF, JCHITF, JPPTF,  JP2TF,  JEETF
c      INTEGER JAM2TF, JCODTF, JCRLTF, JLRES1, JLRES2
c      INTEGER JKBQDV, JSTAC1, JSTAC2, JSTAC3, JSTAC4
c      INTEGER JKNK6C, JKNK7C,JNDOF

c      PARAMETER ( JKAPPA =  1 )
c      PARAMETER ( JALPHA =  2 )
c      PARAMETER ( JBETA  =  3 )
c      PARAMETER ( JGAMMA =  4 )
c      PARAMETER ( JXI    =  5 )
c      PARAMETER ( JCUR   =  6 )
c      PARAMETER ( JPHI0  =  7 )
c      PARAMETER ( JD0    =  8 )
c      PARAMETER ( JTANLF =  9 )
c      PARAMETER ( JY0LF  = 10 )
c      PARAMETER ( JPHI6  = 11 )
c      PARAMETER ( JPHI7  = 12 )
c      PARAMETER ( JRKN6  = 13 )
c      PARAMETER ( JRKN7  = 14 )
c      PARAMETER ( JHITCF = 15 )
c      PARAMETER ( JCHICF = 16 )
c      PARAMETER ( JHTQLF = 17 )
c      PARAMETER ( JHITLF = 18 )
c      PARAMETER ( JCHILF = 19 )
c      PARAMETER ( JHIT3F = 20 )
c      PARAMETER ( JCHI3F = 21 )
c      PARAMETER ( JHITTF = 22 )
c      PARAMETER ( JCHITF = 23 )
c      PARAMETER ( JPPTF  = 24 )
c      PARAMETER ( JP2TF  = 25 )
c      PARAMETER ( JEETF  = 26 )
c      PARAMETER ( JAM2TF = 27 )
c      PARAMETER ( JCODTF = 28 )
c      PARAMETER ( JCRLTF = 29 )
c      PARAMETER ( JLRES1 = 30 )
c      PARAMETER ( JLRES2 = 31 )
c      PARAMETER ( JKBQDV = 32 )
c      PARAMETER ( JSTAC1 = 33 )
c      PARAMETER ( JSTAC2 = 34 )
c      PARAMETER ( JSTAC3 = 35 )
c      PARAMETER ( JSTAC4 = 36 )
c      PARAMETER ( JKNK6C = 37 )
c      PARAMETER ( JKNK7C = 38 )
c      PARAMETER ( JNDOF  = 39 )

      INTEGER         NSTKTF,
     +                ISTKTF,
     +                IFSTK ,
     +                GSTKLOCL,
     +                GSTKLOCM,
     +                LENSSTK,
     +                NXTSTK,
     +                LFHKEY,
     +                TMPKEY,
     +                NHTKEY,
     +                IPLSTK,
     +                IADSTK,
     +                SRCSTK

      REAL            CHIKEY,
     +                PRMSTK,
     +                DWTSTK,
     +                SLRSTK,
     +                PULSTK,
     +                SG1STK,
     +                SG2STK,
     +                MESSTK,
     +                ERMSTK

      CHARACTER*15    NAMSSTK

      COMMON /TFSTAK/ 
     + 	              NSTKTF   (NSTKTYP),
     +                ISTKTF   (NSTKTYP),
     +                IFSTK    (NSTKTYP),
     +                GSTKLOCL (NSTKTYP),
     +                GSTKLOCM (NSTKTYP),
     +                LENSSTK  (NSTKTYP),
     +                NXTSTK   (NSTKDM),
     +                LFHKEY   (NSTKDM),
     +                TMPKEY   (NSTKDM),
     +                NHTKEY   (NSTKDM),
     +                CHIKEY   (NSTKDM),
     +                PRMSTK(NPARTF,NSTKDM),
     +                IPLSTK(KLR2TF,NSTKDM),
     +                IADSTK(KLR2TF,NSTKDM),
     +                SRCSTK(KLR2TF,NSTKDM),
     +                DWTSTK(KLR2TF,NSTKDM),
     +                SLRSTK(KLR2TF,NSTKDM),
     +                PULSTK(KLR2TF,NSTKDM),
     +                SG1STK(KLR2TF,NSTKDM),
     +                SG2STK(KLR2TF,NSTKDM),
     +                MESSTK(KLR2TF,NSTKDM),
     +                ERMSTK(NERMTF,NSTKDM)

      COMMON /TFSTAKC/ 
     +                NAMSSTK  (NSTKTYP)


      INTEGER         IPMSTK(NPARTF,NSTKDM)
      EQUIVALENCE    (IPMSTK(1,1),  PRMSTK(1,1))
* ----------------------------------------------------------
