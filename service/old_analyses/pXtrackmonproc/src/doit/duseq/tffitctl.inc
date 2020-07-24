*
* $Id: tffitctl.inc,v 1.2 2001/11/19 23:32:47 dpp Exp $
*
* $Log: tffitctl.inc,v $
* Revision 1.2  2001/11/19 23:32:47  dpp
*      -> merge in documentation
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:10  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/14 28/02/94  13.25.14  by  Dan Peterson
*-- Author :
* TF final fitter control
#if !defined(CLEO_NOCOMDOC)
* /TFFITC/ Communicates between the final fitter and
*          the calling program.
* LCALIB       - the final fitter will not delete hits or use the
*                 second kink. Iterations will be limited.
*                 No hits will be deleted from the fit, or from
*                 /TFTRAK/, but USEFIT is used to flag used/unused
*                 hits.
* DUALTR       - the final fitter has been modified to allow
*                 simultanious two track fitting for calibration use
*                 DUALTR = .TRUE. if this is two track fitting
*                 DUALTR = .FALSE. if this is standard one track fitting
* LRMVHT       - uses USEFIT to remove hits from the fit,
*                 uses FITFLT to set variables, will not do other
*                 properties of related "LCALIB"
* LKN6FI       - uses the KINK6 for a phi kink at the origin,
*                 number (2) track, as specified in USEFIT, will then have
*                 PHI(2)=(PHI(1)+ pi+PHI6), will not allow normal use KINK6
* LKN7CT       - uses the KINK7 for a Z kink at the origin,
*                 number (2) track, as specified in USEFIT, will then have
*                 CT(2) = -(CT(1) + PHI7) ,will not allow normal use KINK7
* LADVDE       - ADD the VD East hits to the hit list for residuals
* USEFIT(IFIT) - only checked if DUALTR=.TRUE.
*                use in TF3FIT for calibration fits
*                +/- depending on whether the hit is to be used
*                1 or 2 depending on which subtrack the hit belongs to
* FITFLT(IVAR) - only checked if DUALTR=.TRUE.
*                .TRUE. if this variable is allowed to float in the fit
*                order: CU,FI,DA,CT,Z0,K1,K2
#endif

      LOGICAL         LCALIB,DUALTR,LKN6FI,LRMVHT,LKN7CT,LADVDE
      LOGICAL         FITFLT
      INTEGER         USEFIT

      COMMON /TFFITC/ LCALIB,DUALTR,LRMVHT,LKN6FI,LKN7CT,LADVDE,
     +                FITFLT(NP3DTF),
     +                USEFIT(KLR2TF)
