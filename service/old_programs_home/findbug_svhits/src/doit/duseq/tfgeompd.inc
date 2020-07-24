*
* $Id: tfgeompd.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tfgeompd.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:08  zfiles
* first version of doit in CVS
*
*
* TFGEOMPA Documentation
*
* tfgeompd.inc
*
C
* KCANTF - Max. Number of TFIND anode   layers
* KCATTF - Max. Number of TFIND cathode layers
* KLYRTF - Max. Number of TFIND total   layers
*
* KLR2TF - Twice the max. total number of layers
C
C== Following are here for initial compatibility
C
* NLYRVD - Number of (IV or PT) + VD layers
* NLYRCF - Number of layers used in CFIND
* LLYRCF - First CF layer number
* MLYRCF - Last  CF layer number
* NLYRLF - Number of layers used in LFIND
* LLYRLF - First LF layer number
* MLYRLF - Last  LF layer number
*
* NSCATF - Maximum number of discreet   scattering planes
* NSCCTF - Maximum number of continuous scattering media
*
* LLYRVD - First VD layer
* MLYRVD - Last  VD layer
*
* LCAXDR - First axial DR layer
* MCAXDR - Last  axial DR layer
*
* LCATCD - First CD cathode
* MCATCD - Last  CD cathode
*
* LCSTDR - First DR stereo layer
* MCSTDR - Last  DR stereo layer
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
