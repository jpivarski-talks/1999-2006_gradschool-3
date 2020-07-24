*
* $Id: vdhistcd.inc,v 1.1.1.1 1998/02/06 19:11:50 dpp Exp $
*
* $Log: vdhistcd.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:10  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/07 17/05/91  19.19.41  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  19.17.53  by  Steve Schaffner
*-- Author :
* COMMONS for VDWHST and VDFPIE
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/vdhistdo.inc"
#endif
      INTEGER     JSECTR,
     +         ITMPSC,NFTMPS,
     +         LLYRSC,     MLYRSC,
     +         NLMNSC,  NSECTU,
     +         LBINSC,MBINSC,IBESTS,
     +         NHITSC,IPKSCP,IPKSC0,IPKSC1,
     +         IPKSCV,IUSESC,NXTUSC,
     +         IHSECT
      COMMON /SFCOMI/   JSECTR(NSECT1),
     +         ITMPSC(NSECT2,MLBAND),NFTMPS(NSECT2),
     +         LLYRSC(NSBAND),      MLYRSC(NSBAND),
     +         NLMNSC(NSBAND),      NSECTU(NSBAND),
     +         LBINSC(NSBAND),MBINSC(NSBAND),IBESTS(NSBAND),
     +         NHITSC(NSBAND),
     +         IPKSCP(NSBAND),IPKSC0(NSBAND),IPKSC1(NSBAND),
     +         IPKSCV(NSBAND),IUSESC(NSBAND),NXTUSC(NSECT2),
     +         IHSECT(NSBAND,NSECT2)
      REAL     FITMPS,  PHISEC,  DPHISC
      COMMON /SFCOMR/   FITMPS(NSECT2),PHISEC(NSBAND,NSECT2),
     +         DPHISC(NSBAND)
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
