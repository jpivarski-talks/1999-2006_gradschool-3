*
* $Id: tfctlpar.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tfctlpar.inc,v $
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
*CMZ :  5.54/07 17/05/91  19.19.39  by  Steve Schaffner
*CMZ :  5.54/00 04/12/90  13.45.03  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.48.21  by  Steve Schaffner
*-- Author :
*
* tfctlpar.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/tfctlpdo.inc"
#endif
C
      INTEGER    IFKND0,      IFKND1,      IFKND2,      IFDRDG
      PARAMETER( IFKND0 =  1, IFKND1 =  3, IFKND2 =  5, IFDRDG =  7 )

      INTEGER    NERRTF
      PARAMETER( NERRTF = 27 )
      INTEGER    IERG20,      IERIZH,      IERDRH,      IERCDH
      PARAMETER( IERG20 =  1, IERIZH =  2, IERDRH =  3, IERCDH =  4 )
      INTEGER    IERG22,      IERCDF,      IERHIT,      IERLNK
      PARAMETER( IERG22 =  5, IERCDF =  6, IERHIT =  7, IERLNK =  8 )
      INTEGER    IERLMB,      IERCHN,      IERRBL,      IERTRK
      PARAMETER( IERLMB =  9, IERCHN = 10, IERRBL = 11, IERTRK = 12 )
      INTEGER    IERLKT,      IERFIT,      IERSTK,      IER3FT
      PARAMETER( IERLKT = 13, IERFIT = 14, IERSTK = 15, IER3FT = 16 )
      INTEGER    IER3LP,      IERCDT,      IERCDS,      IERGEN
      PARAMETER( IER3LP = 17, IERCDT = 18, IERCDS = 19, IERGEN = 20 )
      INTEGER    IEROVF,      IERNXT,      IERFAT,      IERNTR
      PARAMETER( IEROVF = 21, IERNXT = 22, IERFAT = 23, IERNTR = 24 )
      INTEGER    IERLOS,      IERTRP,      IERFND
      PARAMETER( IERLOS = 25, IERTRP = 26, IERFND = 27              )

      INTEGER    NERSUB
      PARAMETER( NERSUB = 20 )
      INTEGER    ICDFTC,      IDRHIT,      IIZHIT,      ICDNEW
      PARAMETER( ICDFTC =  1, IDRHIT =  2, IIZHIT =  3, ICDNEW =  4 )
      INTEGER    ICDSTO,      ICFHIT,      ICFDRD,      ICFIND
      PARAMETER( ICDSTO =  5, ICFHIT =  6, ICFDRD =  7, ICFIND =  8 )
      INTEGER    ICFLIN,      ICFLSC,      ICFHIS,      ILFHIT
      PARAMETER( ICFLIN =  9, ICFLSC = 10, ICFHIS = 11, ILFHIT = 12 )
      INTEGER    ILFIND,      ILFLIN,      ILFHIS,      ITFLIM
      PARAMETER( ILFIND = 13, ILFLIN = 14, ILFHIS = 15, ITFLIM = 16 )
      INTEGER    ITFCLM,      ITFNEW,      ITFTCU,      ITF3FI
      PARAMETER( ITFCLM = 17, ITFNEW = 18, ITFTCU = 19, ITF3FI = 20 )
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
