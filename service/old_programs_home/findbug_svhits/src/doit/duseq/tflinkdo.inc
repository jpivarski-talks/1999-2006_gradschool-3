*
* $Id: tflinkdo.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tflinkdo.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:09  zfiles
* first version of doit in CVS
*
*
* TFLINKCD Documentation
*
* tflinkdo.inc
*
C
* /TFLINK/ Holds the list of links
*  A link appears in this list if it passes the link cuts
*
* NLNK         - # of links accepted in the list
* LLNKTP(ITYP) - Address in /TFLINK/ of first link of type # ITYP
* MLNKTP(ITYP) - Address in /TFLINK/ of last  link of type # ITYP
*
* IIHLNK(ILNK) - Hit # of the inner hit in link # ILINK
* IOHLNK(ILNK) - Hit # of the outer hit in link # ILINK
*
* ITPLNK(ILNK) - Type # of the link
*
* ALNK  (ILNK) - Primary   link variable for elementary trees
* BLNK  (ILNK) - Secondary link variable for elementary trees
*
*                    ALNK = TANGENT(DIP-ANGLE)      for LFIND
*                    ALNK = KAPPA = CURCF = 1/(2*R) for CFIND
*
*                    BLNK = Z0                      for LFIND
*                    BLNK = D0                      for CFIND
*
* IFRUT (ILNK) - Index of first link that is a root of link # ILNK
* ILRUT (ILNK) - Index of first link that is a root of link # ILNK
*
* IFBRN (ILNK) - Index of first link that is a branch of link # ILNK
* ILBRN (ILNK) - Index of first link that is a branch of link # ILNK
*
* NLMB         - # of roots or branches in the limb list
* NXTLMB(ILMB) - Index of next limb in the limb list
* ILKLMB(ILMB) - Index ILNK of the limb ILMB
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
