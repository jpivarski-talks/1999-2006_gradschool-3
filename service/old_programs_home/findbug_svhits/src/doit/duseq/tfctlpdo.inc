*
* $Id: tfctlpdo.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tfctlpdo.inc,v $
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
* TFCTLPAR Documentation
*
* tfctlpdo.inc
*
C
* IFKND0 - Index for CFIND KINDR >=  0 statistics
* IFKND1 - Index for CFIND KINDR  = -1 statistics
* IFKND2 - Index for CFIND KINDR  = -2 statistics
* IFDRDG - Index for CFIND dredged     statistics
* NERRTF - Number of types of error from TFBERR
* IERG20 - An error in GET DIN 20 (DR hits)
* IERIZH - Too many IZ hits for /CDSCRT/
* IERDRH - Too many DR hits for /CDSCRT/
* IERCDH - Too many CD hits for /CDSCRT/
* IERG22 - An error in GET DIN 22 (SEED tracks)
* IERCDF - Too many DR trks /CDTRAK/ in (CDFTCH)
* IERHIT - Too many hits  in /TFHITS/
* IERLNK - Too many links in /TFLINK/
* IERLMB - Too many next or prev links /TFLINK/
* IERCHN - Too many roots or branches     TFCLMB
* IERRBL - Too many root  or branch links TFCLMB
* IERTRK - Too many tracks in /TFTRKS/
* IERLKT - Too many links  in /TFTRKS/
* IERFIT - CHISQ < 0 in the circle fit
* IERSTK - Too many tracks in /TFSTAK/
* IER3FT - Trouble in TF3FIT
* IER3LP - T3FFIT looping in calculating Z
* IERCDT - Too many  DU trks /CDTRAK/ in (CDNEWT)
* IERCDS - Too many /CDTRAK/ for ANLCDR  (CDSTOR)
* IERGEN - General error
* IEROVF - General overflow
* IERNXT - General next error
* IERFAT - General fatal error
* IERNTR - No elementary tree
* IERLOS - General lost track error
* IERTRP - General ERTRAP error
* IERFND - General IFNDNM inconsistency error
* NERSUB - Number of subroutines reporting errors
*
* ICDFTC - CDFTCH
* IDRHIT - DRHITW
* IIZHIT - IZHITW
* ICDNEW - CDNEWT
* ICDSTO - CDSTOR
* ICFHIT - CFHITS
* ICFDRD - CFDRDG
* ICFIND - CFIND
* ICFLIN - CFLINK
* ICFLSC - CFCFIT
* ICFHIS - CFHIST
* ILFHIT - LFHITS
* ILFIND - LFIND
* ILFLIN - LFLINK
* ILFHIS - LFHIST
* ITFLIM - TFLIMB
* ITFCLM - TFCLMB
* ITFNEW - TFNEWT
* ITFTCU - TFTCUT
* ITF3FI - TF3FIT
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
