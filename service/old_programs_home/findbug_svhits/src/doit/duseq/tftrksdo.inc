*
* $Id: tftrksdo.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tftrksdo.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:09  zfiles
* first version of doit in CVS
*
*
* TFTRKSCD Documentation
*
* tftrksdo.inc
*
C
* /TFTRKS/ Holds the list of tracks found in CFIND and LFIND
*
* NTRK          - # of tracks in the track list
* ITRK          - Address of a track in the list
* IFTRK (NLNKS) - Address of first track with NLNKS links
* ILTRK (NLNKS) - Address of last  track with NLNKS links
* NXTTRK(ITRK)  - Address of next track with same # links as ITRK
* LLKTRK(ITRK)  - Address of first link of track # ITRK in ILKTRK
* MLKTRK(ITRK)  - Address of last  link of track # ITRK in ILKTRK
*
* NLKT          - # of links in the track link list
* ILKTRK(ILKT)  - Address in /TFLINK/ of a link on a track
* ATFTRK(ITRK)  - SLOPE / ALPHA     for LFIND BZW. CFIND
* BTFTRK(ITRK)  - Z0    / BETA      for LFIND BZW. CFIND
* GTFTRK(ITRK)  - GAMMA             for CFIND
* CTFTRK(ITRK)  - KAPPA             for CFIND
* CHITRK(ITRK)  - CHISQ/(N.D.F.)    for LFIND BZW. CFIND
* CHATRK(ITRK)  - CHISQ/(N.D.F.)    for LFIND with (optional) z=0 bias
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
