
*
* $Id: tftrakcd.inc,v 1.7 2002/05/23 14:27:51 dpp Exp $
*
* $Log: tftrakcd.inc,v $
* Revision 1.7  2002/05/23 14:27:51  dpp
*      -> add the radius of pat rec limit and stragler radius
*
* Revision 1.6  2001/11/19 23:16:00  dpp
*      -> remove ILRESC
*      -> add Z0BIAS as a full track variable held in the stak
*      -> add D0_CONS as a full track variable held in the stak
*
* Revision 1.5  2000/12/19 23:28:23  dpp
*      -> counters of number of DR layers of types in fit, new stak variable
*
* Revision 1.4  2000/10/02 21:15:43  dpp
*     -> add program indicator for each hit
*
* Revision 1.3  2000/08/18 18:32:08  dpp
*      -> add LCFEMC, note: new stak variable
*      -> add FTK6CF etc, new stak variable, new order
*      -> remove some obsolete variables
*BOOT_SI_Z_HITS
* Revision 1.2  1999/12/09 21:02:20  dpp
* deweight variable
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
*
*
* TF track sequence
#if !defined(CLEO_NOCOMDOC)
* /TFTRAK/ Communicates between TFTHIT and fitting programs. TFTHIT
*          organizes the hits for a track in /TFTRKS/ for fitting
*
* ITRKTF       - Address of found track in /TFTRKS/

* these values are saved in the stack and transferred in the seed routines
* the number of parameters is NPARTF
* AND DON'T GET ANY IDEAS ABOUT CHANGING THE ORDER

*  1    KAPCF   - KAPPA     of the circular track
*  2    ALPCF   - ALPHA     of the circular track
*  3    BETCF   - BETA      of the circular track
*  4    GAMCF   - GAMMA     of the circular track
*  5    XICF:SQRT(1.0+4.0*KAPPA*GAMMA)=SQRT(ALPHA**2 + BETA**2)=1+2*CURCD*D0CD
*  6    CURCF   - KAPPA     of the circular track
*  7    PHI0CF  - PHI0      of the circular track
*  8    D0CF    - D0        of the circular track
*  9    TANDLF  - Slope     of the linear track
* 10    Y0LF    - Intercept of the linear track
* 11    PHI6CF  - PHI kink  of the circular track ; variable 6
* 12    PHI7CF  - PHI kink  of the circular track ; variable 7
* 13    RKN6CF  - radius of PHI kink; PHI6CF
* 14    RKN7CF  - radius of PHI kink; PHI7CF
* 15    SKN6CF  - stiffness factor for kink, maybe material or sigma
* 16    SKN7CF  - stiffness factor for kink, maybe material or sigma
* 17    KNK6CF  - kink enable flag, number specifies type of transformation
* 18    KNK7CF  - kink enable flag, number specifies type of transformation
* 19    FTK6CF  - fit kink enable flag
* 20    FTK7CF  - fit kink enable flag
* 21    NHITCF  - Number of hits on circle fit
* 22    CHICF   - Chi2 of           circle fit
* 23    NHTQLF  - Number of charge division points on linear fit
* 24    NHITLF  - number of points (excluding q-div points) on linear fit
* 25    CHILF   - Chi2 of        linear fit
* 26    NHIT3F  - Num of hits on last fit done
* 27    CHI3F   - Chi2 of        last fit done
* 28    NHITTF  - Num of hits on helix fit
* 29    CHITF   - Chi2 of        helix fit
* 30    CH2RCCF - Chi2, recalculated in FIT_WHATEVER
* 31    TLRESCF - average time like residual 
* 32    NLFDRA  - numb layers represented in fit, drift chambers, axial
* 33    NLFDRU  - numb layers represented in fit, drift chambers, +stereo
* 34    NLFDRV  - numb layers represented in fit, drift chambers, -stereo
* 35    PPTF    - Momentum (signed by charge)
* 36    P2TF    - Momentum squared
* 37    EETF    - Energy
* 38    AM2TF   - Mass squared of particle
* 39    LCRLTF  - .TRUE. if this track curls
* 40    Z0BIAS  - sigma of Z0 origin used in the fit
* 41    D0_CONS - .TRUE. if D0 is to constrained to =0
* 42    KBQDIV  - Bitmask of layers used in charge division.
* 44:46 STATCF  - Status wordS for track
* 47    NDOFTF  - Number of degrees of freedom.
* 48    LCFEMC  - .TRUE. if Circle Fit Error Matrix elements are Current 
* 49    RPRLIM  - radius of pattern recognition limit
* 50    RSTRAG  - radius beyond which further hits are straglers 
*
* "stak" means that it is saved in the stak
*
*       NFIT         - # of hits to fit in the track (NHITTF - KFIT)
*  stak IPLFIT(IFIT) - Physical layer #  for hit # IFIT
*  stak IADFIT(IFIT) - Address of hit # IFIT in /CDSCRT/
*       TRKHIT(IFIT) - Address of hit # IHIT in /TFHITSCD/
*  stak SRCFIT(IFIT) - source of hit in fit (program that added it)
*       XFIT  (IFIT) - X value of hit # IFIT
*       YFIT  (IFIT) - Y value of hit # IFIT
*       WGTFIT(IFIT) - Weight  of hit # IFIT
*  stak DWTFIT(IFIT) - amount to deweight the fitting weight 
*       DFIT  (IFIT) - Signed drift distance for hit # IFIT
*  stak SFIT  (IFIT) - S = arc length on circle to hit # IFIT
*       ZFIT  (IFIT) - Z value of  hit # IFIT
*  stak PULFIT(IFIT) - Pull    of hit # IFIT
*  stak MESFIT(IFIT) - Measurement at this point ( ie drift distance or
*                position on svd wafer )

*       STRFFT(IHIT)  - r-phi (CF) status of hit
*       STSZFT(IHIT)  - s-z   (LF) status of hit

*  stak SG1FIT(IFIT) - 1./SQRT(WGTFIT)
*  stak SG2FIT(IFIT) - error on residual from extrapolated track
*  stak ERMFIT(IERM) - Error matrix for the fit
*
*       NFITSV       - Saved NFIT
*       IPLFSV(IFIT) - Saved IPLFIT
*       IADFSV(IFIT) - Saved IADFIT
*       STATSV()     - Saved status bits
*       KAPHLD()     - saved track parameters, kapcf etc.
*       PULHLD()     - saved pulfit
*       STATT()      - temporary status bits 
#endif
      LOGICAL LCRLTF  ,D0_CONS ,LCFEMC
      INTEGER ITRKTF  ,
     +        NHITCF  ,NHTQLF  ,NHITLF  ,NHIT3F  ,NHITTF,
     +        NLFDRA  ,NLFDRU  ,NLFDRV  ,
     +        KBQDIV  ,STATCF  ,
     +        KNK6CF  ,KNK7CF  ,FTK6CF  ,FTK7CF  ,
     +        NDOFTF  ,
     +        NFIT    ,IPLFIT  ,IADFIT  ,STRFFT  ,STSZFT  ,
     +        TRKHIT  ,SRCFIT  ,
     +        NFITSV  ,IPLFSV  ,IADFSV	,STATSV  ,STATT   ,

     +        KNK6TMP ,KNK7TMP ,FTK6TMP ,FTK7TMP ,
     +        KNK6NOW ,KNK7NOW ,FTK6NOW ,FTK7NOW 

      REAL    KAPCF   ,ALPCF   ,BETCF   ,GAMCF   ,XICF    ,
     +        CURCF   ,PHI0CF  ,D0CF    ,TANDLF  ,Y0LF    ,
     +        PHI6CF  ,PHI7CF  ,RKN6CF  ,RKN7CF  ,
     +        SKN6CF  ,SKN7CF  ,

     +        CHICF   ,CHILF   ,CHI3F   ,CHITF   ,
     +        CH2RCCF ,TLRESCF ,
     +        PPTF    ,P2TF    ,EETF    ,AM2TF   ,
     +        Z0BIAS  ,RPRLIM  ,RSTRAG  ,
     +        XFIT    ,YFIT    ,
     +        WGTFIT  ,DWTFIT  ,
     +        DFIT    ,SFIT    ,ZFIT    ,
     +        PULFIT  ,
     +        SG1FIT  ,SG2FIT  ,MESFIT  ,
     +        ERMFIT  ,

     +        KAPTMP  ,ALPTMP  ,BETTMP  ,GAMTMP  ,XITMP   ,
     +        CURTMP  ,PHI0TMP ,D0TMP   ,TANDTMP ,Y0TMP   ,
     +        PHI6TMP ,PHI7TMP ,RKN6TMP ,RKN7TMP ,
     +        SKN6TMP ,SKN7TMP ,

     +        KAPNOW  ,ALPNOW  ,BETNOW  ,GAMNOW  ,XINOW   ,
     +        CURNOW  ,PHI0NOW ,D0NOW   ,TANDNOW ,Y0NOW   ,
     +        PHI6NOW ,PHI7NOW ,RKN6NOW ,RKN7NOW ,
     +        SKN6NOW ,SKN7NOW ,

     +        KAPAFT6 ,ALPAFT6 ,BETAFT6 ,GAMAFT6 ,XIAFT6  ,
     +        KAPAFT7 ,ALPAFT7 ,BETAFT7 ,GAMAFT7 ,XIAFT7  ,
     +        KAPHLD  ,PULHLD

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      COMMON /TFTRAK/ ITRKTF,

     +        KAPCF   ,ALPCF   ,BETCF   ,GAMCF   ,XICF    ,
     +        CURCF   ,PHI0CF  ,D0CF    ,TANDLF  ,Y0LF    ,
     +        PHI6CF  ,PHI7CF  ,RKN6CF  ,RKN7CF  ,
     +        SKN6CF  ,SKN7CF  ,
     +        KNK6CF  ,KNK7CF  ,FTK6CF  ,FTK7CF  ,

     +        NHITCF  ,CHICF   ,NHTQLF  ,NHITLF  ,CHILF   ,
     +        NHIT3F  ,CHI3F   ,NHITTF  ,CHITF   ,
     +        CH2RCCF ,TLRESCF ,
     +        NLFDRA  ,NLFDRU  ,NLFDRV  ,
     +        PPTF    ,P2TF    ,EETF    ,AM2TF   ,
     +        LCRLTF  ,Z0BIAS  ,D0_CONS ,KBQDIV  ,STATCF        (4),
     +        NDOFTF  ,LCFEMC  ,RPRLIM  ,RSTRAG  ,

     +        KAPTMP  ,ALPTMP  ,BETTMP  ,GAMTMP  ,XITMP   ,
     +        CURTMP  ,PHI0TMP ,D0TMP   ,TANDTMP ,Y0TMP   ,
     +        PHI6TMP ,PHI7TMP ,RKN6TMP ,RKN7TMP ,
     +        SKN6TMP ,SKN7TMP ,
     +        KNK6TMP ,KNK7TMP ,FTK6TMP ,FTK7TMP ,

     +        KAPNOW  ,ALPNOW  ,BETNOW  ,GAMNOW  ,XINOW   ,
     +        CURNOW  ,PHI0NOW ,D0NOW   ,TANDNOW ,Y0NOW   ,
     +        PHI6NOW ,PHI7NOW ,RKN6NOW ,RKN7NOW ,
     +        SKN6NOW ,SKN7NOW ,
     +        KNK6NOW ,KNK7NOW ,FTK6NOW ,FTK7NOW ,

     +        KAPAFT6 ,ALPAFT6 ,BETAFT6 ,GAMAFT6 ,XIAFT6  ,
     +        KAPAFT7 ,ALPAFT7 ,BETAFT7 ,GAMAFT7 ,XIAFT7  ,

     +        NFIT    ,
     +        IPLFIT   (KLR2TF),IADFIT   (KLR2TF),
     +        TRKHIT   (KLR2TF),SRCFIT   (KLR2TF),
     +        XFIT     (KLR2TF),YFIT     (KLR2TF),
     +        WGTFIT   (KLR2TF),DWTFIT   (KLR2TF),
     +        DFIT     (KLR2TF),SFIT     (KLR2TF),
     +        ZFIT     (KLR2TF),PULFIT   (KLR2TF),
     +        STRFFT   (KLR2TF),STSZFT   (KLR2TF),

     +        SG1FIT   (KLR2TF),SG2FIT   (KLR2TF),
     +        MESFIT   (KLR2TF),
     +        ERMFIT   (NERMTF),

     +        NFITSV  ,
     +        IPLFSV   (KLR2TF),IADFSV   (KLR2TF),
     +        STATSV        (4),STATT         (4),
     +        KAPHLD   (NPARTF),PULHLD   (KLR2TF)









