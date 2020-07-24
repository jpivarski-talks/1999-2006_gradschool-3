*
* $Id: cftypecd.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: cftypecd.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:07  zfiles
* first version of doit in CVS
*
*
* CF Definition of link types sequence
*
* cftypecd.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/cftypedo.inc"
#endif
C
      INTEGER         NLRUCF, IPLRCF,
     +                LETRCF, METRCF,
     +                IILRCF, IMLRCF, IOLRCF,
     +                IRTPCF, IBTPCF,
     +                IFETCF, NXETCF
      COMMON /CFTYPI/ NLRUCF, IPLRCF(KLYRCF),
     +                LETRCF(0:KLYRCF), METRCF(0:KLYRCF),
     +                IILRCF(NTYPCF), IMLRCF(NTYPCF), IOLRCF(NTYPCF),
     +                IRTPCF(NETRCF), IBTPCF(NETRCF),
     +                IFETCF(KFETCF), NXETCF(NETRCF)
*
      REAL            D0LWCF, DIFWCF,
     +                D0LHCF, DIFHCF
      COMMON /CFTYPR/ D0LWCF(NTYPCF), DIFWCF(NETRCF),
     +                D0LHCF(NTYPCF), DIFHCF(NETRCF)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
