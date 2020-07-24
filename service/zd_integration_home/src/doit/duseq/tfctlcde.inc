*
* $Id: tfctlcde.inc,v 1.6 2002/09/05 19:16:17 dpp Exp $
*
* $Log: tfctlcde.inc,v $
* Revision 1.6  2002/09/05 19:16:17  dpp
*      -> remove NTRTCD, some kind of count of the number of each KIND
*
* Revision 1.5  2001/11/19 23:15:40  dpp
*      -> comprehensive removal of unused variables
*
* Revision 1.4  2000/10/02 21:15:20  dpp
*      -> add PROGRAM_INDICATOR
*      -> move Z0BIAS to tfctlcde
*
* Revision 1.3  2000/04/26 14:14:33  dpp
*      -> add D0_CONSTRAIN  and Z0_CONSTRAIN
*
* Revision 1.2  2000/02/12 20:36:41  dpp
*      -> comment on NEXPT
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
*
*CMZ :  6.00/28 29/05/94  21.19.10  by  Anders Ryd
*CMZ :  6.00/24 09/05/94  00.56.27  by  Anders Ryd
*CMZ :  6.00/14 21/02/94  19.22.58  by  Dan Peterson
*CMZ :  5.54/15 29/09/92  14.11.54  by  Rob Kutschke
*CMZ :  5.54/14 20/07/92  10.09.02  by  Dan Peterson
*CMZ :  5.54/00 18/10/90  00.35.43  by  Steve Schaffner
*CMZ :  5.53/07 08/10/90  12.03.40  by  Steve Schaffner
*CMZ :  5.53/06 30/07/90  18.27.36  by  Steve Schaffner
*CMZ :  5.53/00 02/04/90  16.02.52  by  Steve Schaffner
*CMZ :  5.52/01 15/03/90  15.56.30  by  Steve Schaffner
*CMZ :  5.52/00 08/02/90  12.44.13  by  Steve Schaffner
*-- Author :
*
* tfctlcde.inc
*
#if !defined(CLEO_NOCOMDOC)
* ----------------------------------------------------------
*  /TFCTL / track finding control
* ----------------------------------------------------------
* NEXPT  - Experiment number of current run
*          now used for the DOIT running condition word
* NRUN   - Run    number of current run
* NEVENT - Event  number of current event
* NTRACK - Track  number of current track

* PROGRAM_INDICATOR - integer specifying which subroutine is adding hits

* BMAGTF - Magnetic field in kgauss
* CHICUT - Chi2 cut for ?FTCUT
* CH2SCL - Chi2 scale factor for given # of degrees of freedom
* DCH2TF - Delta(chi2) cut in TF3FIT
* NDELTF - Max # of hits to delete in TF3FIT
* MP3DTF - # of parameters in helix fit (depends on material)
* IMNVTF - = 0  => use     diagonal cov matrix in TF3FIT
*          = 1  => use non-diagonal cov matrix in TF3FIT
*          = 2  => use diagonal cov matrix but post-fit correction
* LKN6TF - .TRUE. if a helix is fitted with kink at VD-DR,adds 1 to  MP3DTF
* LKN7TF - .TRUE. if a helix is fitted with kink in mid DR,adds 1 to  MP3DTF
* XSCMIN - Minimum material before 6 parameters are used in fit
* LDEBUG - debug flag
* LOPEN  - open lineprinter output file
* LCOMPR - enable Compress histogram output (incompatible with LWHIST)
* LWHIST - enable Hbook histogram output
*                       backward compatability)
* LNAME  - enable reading of namelist file
*
* LUSEQD - .TRUE.  => inside good qdiv run range
* LMCRLO - .TRUE.  => This is CLEO   monte carlo
* LDME   - .TRUE.  => DME in PT
* LKALMN - .TRUE.  => Use Kalman filter instead of TF3FIT.
*                   & check if TFBN process was called for multi-bunch runing
* LSLEAC - .TRUE.  => Enable Stereo Layer Entrance Angle Correction.
* LUSVDE - .TRUE.  => Logical: USe VD East instead of west
* LKNPRC - .TRUE.  => allow a KiNk in Pattern ReCognition for SFIND seeds
* 

* ----------------------------------------------------------
#endif

      INTEGER          
     +   NEXPT,  NRUN  , NEVENT, NTRACK,
     +   PROGRAM_INDICATOR,
     +   NHTCUT, NLKCUT,
     +   NDELTF, MP3DTF, IMNVTF,
     +   NTRSTK

      COMMON /TFCTLI/  
     +   NEXPT,  NRUN  , NEVENT, NTRACK,
     +   PROGRAM_INDICATOR,
     +   NHTCUT, NLKCUT,
     +   NDELTF, MP3DTF, IMNVTF,
     +   NTRSTK

      LOGICAL   
     +   LUSEQD,
     +   LDEBUG,
     +   LWHIST,LCOMPR,LNAME,
     +   LMCRLO,LDME  ,LSV1  ,LOPEN,
     +   LKN6TF,LKN7TF,
     +   LCFHST,LLFHST,
     +   LKALMN,LSLEAC,LUSVDE,
     +   LKNPRC


      COMMON /TFCTLL/  
     +   LUSEQD,
     +   LDEBUG,
     +   LWHIST,LCOMPR,LNAME,
     +   LMCRLO,LDME  ,LSV1  ,LOPEN,
     +   LKN6TF,LKN7TF,
     +   LCFHST,LLFHST,
     +   LKALMN,LSLEAC,LUSVDE,
     +   LKNPRC


      REAL
     +   BMAGTF, CHICUT,
     +   CH2SCL        , DCH2TF, XSCMIN,
     +   CPHIT,  SPHIT

      COMMON /TFCTLR/
     +   BMAGTF, CHICUT,
     +   CH2SCL(KLYRTF), DCH2TF, XSCMIN,
     +   CPHIT(MTRKDR),  SPHIT(MTRKDR)






