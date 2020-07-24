*
* $Id: tfhitscd.inc,v 1.12 2003/03/05 17:34:22 dpp Exp $
*
* $Log: tfhitscd.inc,v $
* Revision 1.12  2003/03/05 17:34:22  dpp
*      -> change the name of the hit level arrays again, to RoadWirLst_
*
* Revision 1.11  2002/09/05 19:17:16  dpp
*      -> change names of wire-level arrays to avoid confusion
*      -> add STWRHT (at the  hit level), 
*      -> a copy of the RoadWirLst_stat (wire level)
*
* Revision 1.10  2002/05/23 14:28:19  dpp
*      -> compatibility with new TFTRAKCD variables
*
* Revision 1.9  2001/11/19 23:16:40  dpp
*      -> compatibility with new TFTRAKCD variables
*
* Revision 1.8  2001/09/14 21:18:39  dpp
*     -> add pointer to hit coming from same wire
*
* Revision 1.7  2001/03/30 00:03:02  dpp
*      -> efficiency of layer, especially for silicon
*
* Revision 1.6  2000/12/19 23:27:20  dpp
*      -> add place holder for new TFTRAK variables (stak variables)
*      -> counters of number of DR layers of types in ROAD
*
* Revision 1.5  2000/11/15 17:01:39  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.4  2000/10/24 22:42:21  dpp
*      -> UPTODATE_LYR flag
*      -> LAYER_BLOCK flag for "desparate measure" routines
*
* Revision 1.3  2000/08/18 18:33:04  dpp
*      -> add place holder for new TFTRAK variables (stak variables)
*      -> remove some obsolete variables
*
* Revision 1.2  1999/11/03 22:58:25  dpp
* up-to-date flag used by ROADHITS_COORD etc.
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
*
*
* 27-10-97 added RoadWirLst_Nlyr,RoadWirLst_Nhit,
*                RoadWirLst_adrs,RoadWirLst_stat,ILLRLR
*
*
* tfhitscd.inc

#if !defined(CLEO_NOCOMDOC)
* /TFHITS/ Holds the hit list for the track search and seed track

* variables to describe the _wire_ list

* RoadWirLst_Nhit     - # of hit wires in RoadWirLst_ arrays
* RoadWirLst_Nlyr     - # of layers with hits on wires in RoadWirLst_ arrays
* RoadWirLst_adrs(IHIT)- address of hit wire, positive only
*                  (points to the master hit list in /CDSCRT/)
* RoadWirLst_stat(IHIT) - status of hit wires in RoadWirLst_adrs

* variables to describe the _layer_ scructure of the hit list 

* NLYRLR        - # of layers used in ROAD for which LUSETF = T
* NLYRHT        - # of layers IN ROAD (LUSETF = T or F)
* NLRDRA        - numb layers represented in ROAD, drift chambers, axial
* NLRDRU        - numb layers represented in ROAD, drift chambers, +stereo
* NLRDRV        - numb layers represented in ROAD, drift chambers, -stereo
* IPLRLR(ILLYR) - Physical layer # for logical layer # ILLYR
* ILLRLR(ILYR)  - logical layer # ILLYR for TF  layer # ILYR
* LHITLR(ILLYR) - Address in /TFHITS/ of first hit in layer # ILLYR
* MHITLR(ILLYR) - Address in /TFHITS/ of last  hit in layer # ILLYR
* SLYRLR(ILLYR) - S = R*PSI of seed track in physical layer ILLYR
* EFFILR(ILYR)  - efficiency of TF layer, used for silicon hits when
*                 hit is not found

* variables to describe the hit list

* UPTODATE_HIT  - .TRUE. if the transfer from the wires is up-to-date
*                 ie, if certain parts of WIRES_2_HITS do not need to be
*                 repeated.
* UPTODATE_LYR - .TRUE. if the layer parts of WIRES_2_HITS is up-to-date
* NHIT          - # of hits to use
* IPLHIT(IHIT)  - Physical layer #  for hit # IHIT
* IADRHT(IHIT)  - + (-) address of hit # IHIT
*                   (points to the master hit list in /CDSCRT/)
*                 + (-) if hit has PHIhit .GT. (.LT.) PHIwire
* XHIT  (IHIT)  - X value (independent variable) of hit # IHIT
* YHIT  (IHIT)  - Y value (dependent   variable) of hit # IHIT
* DRFHIT(IHIT)  - signed drift distance          of hit # IHIT
* SHIT  (IHIT)  - S value (independent variable) of hit # IHIT
* ZHIT  (IHIT)  - Z value (dependent   variable) of hit # IHIT
* STRFHT(IHIT)  - r-phi (CF) status of hit
* STSZHT(IHIT)  - s-z   (LF) status of hit
* STWRHT(IHIT)  - wire level status of hit
* HITTRK(IHIT)  - pointer to /TFTRAKCD/
* OTHRHT(IHIT)  - pointer to the hit number that comes from the same wire
*
* LSEED         - .TRUE. if the track comes from seeded tracks

* these values are saved in the stack and transferred in the seed routines
* the number of parameters is NPARTF

* 01 KAPCS         - KAPPA     of the circular track
* 02 ALPCS         - ALPHA     of the circular track
* 03 BETCS         - BETA      of the circular track
* 04 GAMCS         - GAMMA     of the circular track
* 05 XICS          - XI        of the circular track
* 06 CURCS         - KAPPA     of the circular track
* 07 PHI0CS        - PHI0      of the circular track
* 08 D0CS          - D0        of the circular track
* 09 TANDLS        - Slope     of the linear track
* 10 Y0LS          - Intercept of the linear track
* 11 PHI6CS        - PHI kink  of the circular track ; variable 6
* 12 PHI7CS        - PHI kink  of the circular track ; variable 7
* 13 RKN6CS        - radius of PHI kink; PHI6CF
* 14 RKN7CS        - radius of PHI kink; PHI7CF
* 15 SKN6CS        - stiffness factor of PHI kink; PHI6CS
* 16 SKN7CS        - stiffness factor of PHI kink; PHI7CS
* 17 KNK6CS
* 18 KNK7CS
* 19 FTK6CS
* 20 FTK7CS

* 21 NHITCS
* 22 CHICS         - Chisquare of the circular fit
* 23 NHTQLS
* 24 NHITLS
* 25 CHILS         - Chisquare of the linear fit
* 26 NHIT3S
* 27 CHI3S
* 28 NHITTS
* 29 CHITS
* 30 CH2RCCS
* 31 TLRESCS
* 32 NLFDRACS  - numb layers represented in fit, drift chambers, axial
* 33 NLFDRUCS  - numb layers represented in fit, drift chambers, +stereo
* 34 NLFDRVCS  - numb layers represented in fit, drift chambers, -stereo
* 35 PPTS
* 36 P2TS
* 37 EETS
* 38 AM2TS
* 39 LCRLTS        - .TRUE. if seed track curls
* 40 Z0BIASCS      - Z0BIAS (in hit list)
* 41 D0_CONSCS     - D0_CONS (in hit list)
* 42 KBQDV1        - Bitmask of QDIV layers used in the fit.
* 43 STATCS(4)
* 47 NDOFCS        - Number of degrees of freedom.
* 48 LCFEMC_CS     - Circle Fit Error Matrix elements Current, CS version 
* 49 RPRLIMCS      - radius of pattern recognition limit
* 50 RSTRAGLCS     - radius beyond which further hits are straglers 


* layer qualities used especially by REPLACE_Z_HITS and BOOT_SI_Z_HITS
*
* LAYER_BLOCK      - do not make seeds using these layers
#endif

      LOGICAL LSEED   ,LCRLTS  ,D0_CONSCS,LCFEMC_CS
      LOGICAL UPTODATE_HIT,UPTODATE_LYR
      INTEGER KINS     ,
     +        NHITCS   ,NHTQLS   ,NHITLS  ,NHIT3S  ,NHITTS  ,
     +        NLFDRACS ,NLFDRUCS ,NLFDRVCS,
     +        KBQDV1   ,STATCS   ,
     +        KNK6CS   ,KNK7CS   ,FTK6CS  ,FTK7CS  ,
     +        NDOFCS   ,
     +        RoadWirLst_Nlyr,RoadWirLst_Nhit,
     +        NLYRLR  ,NLYRHT  ,NHIT    ,
     +        NLRDRA   ,NLRDRU   ,NLRDRV  ,
     +        IPLRLR   ,LHITLR   ,MHITLR  ,
     +        RoadWirLst_adrs ,RoadWirLst_stat,
     +        IPLHIT  ,IADRHT  ,ILLRLR  ,
     +        STRFHT   ,STSZHT   ,STWRHT  ,HITTRK  ,OTHRHT  ,
     +        LAYER_BLOCK
      REAL    KAPCS   ,ALPCS   ,BETCS   ,GAMCS   ,XICS    ,
     +        CURCS   ,PHI0CS  ,D0CS    ,TANDLS  ,Y0LS    ,
     +        PHI6CS  ,PHI7CS  ,RKN6CS  ,RKN7CS  ,
     +        SKN6CS  ,SKN7CS  ,
     +        CHICS   ,CHILS   ,CHI3S   ,CHITS   ,
     +        CH2RCCS ,TLRESCS ,
     +        PPTS    ,P2TS    ,EETS    ,AM2TS   ,
     +        Z0BIASCS,RPRLIMCS,RSTRAGCS,
     +        SLYRLR  ,EFFILR  ,
     +        XHIT    ,YHIT    ,DRFHIT  ,SHIT    ,ZHIT

C THIS IS TEMP TO COMPILE LIBRARY
      INTEGER KNKECS

      COMMON /TFHITS/LSEED ,KINS  ,
     +        KAPCS     ,ALPCS     ,BETCS     ,GAMCS     ,XICS      ,
     +        CURCS     ,PHI0CS    ,D0CS      ,TANDLS    ,Y0LS      ,
     +        PHI6CS    ,PHI7CS    ,RKN6CS    ,RKN7CS    ,

     +        SKN6CS    ,SKN7CS    ,
     +        KNK6CS    ,KNK7CS    ,FTK6CS    ,FTK7CS    ,

     +        NHITCS    ,CHICS     ,NHTQLS    ,NHITLS    ,CHILS     ,
     +        NHIT3S    ,CHI3S     ,NHITTS    ,CHITS     ,
     +        CH2RCCS   ,TLRESCS   ,
     +        NLFDRACS  ,NLFDRUCS  ,NLFDRVCS  ,
     +        PPTS      ,P2TS      ,EETS      ,AM2TS     ,
     +        LCRLTS    ,Z0BIASCS  ,D0_CONSCS ,KBQDV1    ,STATCS(4) ,
     +        NDOFCS    ,LCFEMC_CS ,RPRLIMCS  ,RSTRAGCS  ,   
     +        RoadWirLst_Nlyr ,RoadWirLst_Nhit ,
     +        NLYRLR     ,NLYRHT   ,NHIT     ,
     +        NLRDRA    ,NLRDRU    ,NLRDRV     ,
     +        IPLRLR  (KLYRDM),SLYRLR   (KLYRDM),
     +        LHITLR  (KLYRDM),MHITLR   (KLYRDM),
     +        RoadWirLst_adrs(NHITDM),RoadWirLst_stat(NHITDM),
     +        IPLHIT  (NHITDM),IADRHT   (NHITDM),
     +        XHIT    (NHITDM),YHIT     (NHITDM),
     +        DRFHIT  (NHITDM),SHIT     (NHITDM),ZHIT    (NHITDM)   ,
     +        ILLRLR  (KLYRTF),EFFILR   (KLYRTF),
     +        STRFHT  (NHITDM),STSZHT   (NHITDM),
     +        STWRHT  (NHITDM),
     +        HITTRK  (NHITDM),OTHRHT   (NHITDM),
     +        UPTODATE_HIT,    UPTODATE_LYR(KLYRDM),
     +        LAYER_BLOCK(KLYRTF)
* ----------------------------------------------------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12


