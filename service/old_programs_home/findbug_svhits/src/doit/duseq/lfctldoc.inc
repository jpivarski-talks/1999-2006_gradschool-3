*
* $Id: lfctldoc.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: lfctldoc.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:07  zfiles
* first version of doit in CVS
*
*
* LFCTLCDE Documentation
*
* lfctldoc.inc
*
*
* /LFCTL / Defines the cuts for finding linear tracks
*
* IFLGLF - = IFGEOM if LFIND geometry arrays have been filled
*          = IFINIT if LFIND links & trees have been defined
* MLRLLF - Maximum layer for tracks with the minimum # of drift hits
* YHTCUT - Maximum Z for hits in the stereo layers
* GMINLF - Minimum gap between two hits in a single layer
* SIGLLF - Number of sigma for picking up .NOT.LUSE hits
* SGYSLF - Scale factor for sigma(z0 link)
* DIFSLF - Scale factor for sigma(tree) (calc. in LFINIT)
* Y0MIN  - Minimum Y0 for acceptable tracks
* Y0MAX  - Maximum Y0 for acceptable tracks
* YMXCUT - Maximum Z at maximum radius in axial layers
* ERRHLF - Scale factor for LFIND  resolution
* ERRDLF - Scale factor for TF3FIT resolution
* NDRPLF - Number of layers to drop in 1st LFIND
* CH2LF1 - Chi2 cut of 1st  LFIND
* CH2LF2 - Chi2 cut of 2nd  LFIND
* CH2LFD - Chi2 cut of last LFIND in dredge
* Z0BIAS - Z0 error used in biased chi2 in LFLFIT
* RLCHOP - Z distance beyond escape point allowed before CHOPping hits
* TCRLCT - Max delta T between cathode and correlated anode (psec) VD
* QPKMCT - Minimum charge of peak strip in cathode cluster VD
* QMINCT - Minimum charge of each strip in cathode cluster VD
* TCRLDC - Max delta T between cathode and correlated anode (psec) DR
* QPKMDC - Minimum charge of peak strip in cathode cluster DR
* QMINDC - Minimum charge of each strip in cathode cluster DR
* Control flag:
* LLFIND - LFIND has been successful
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
