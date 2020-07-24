*
* $Id: cfctldoc.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: cfctldoc.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:07  zfiles
* first version of doit in CVS
*
*
*  CFCTLCDE Documentation
*
* cfctldoc.inc
*
C
* IFLGCF - = IFGEOM If CFIND geometry arrays have been filled
*          = IFINIT if CFIND links & trees have been defined
* MLRLCF - Max layer for tracks with the minimum # of drift hits
* GMINCF - Minimum gap between two hits in a single layer
* SIGLCF - # of sigma for picking up .NOT.LUSE hits
* SGYSCF - Scale factor for sigma(d0 link)
* DIFSCF - Scale factor for sigma(tree) (Calc. in CFINIT)
* D0MAX  - Maximum d0 for acceptable tracks
* ERRHCF - Scale factor for CFIND  resolution
* ERRCAN - Scale factor for throwing away hits in CDCANH
* NDRPCF - # of layers to drop in 1st CFIND
* CH2CF1 - Chi2 cut of 1st  CFIND
* CH2CF2 - Chi2 cut of 2nd  CFIND
* CH2CFD - Chi2 cut of last CFIND in dredge
* RCCHOP - Radial distance beyond escape point allowed before CHOPping hits
* Control flags:
* LCFIND - CFIND has been successful
* LSFCMP - require CFIND track to be similar to SFIND track
* LCFXLA - in CFXFIT, use local abiguity resolution
* LCFXPR - in CFXFIT, require corroborating pairs of hits
* LCFXLI - in TFGMI2, force PT live wire list to all live.
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
