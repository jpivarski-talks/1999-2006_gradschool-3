*
* $Id: sfgksp.inc,v 1.1.1.1 1998/02/06 19:11:50 dpp Exp $
*
* $Log: sfgksp.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:11  zfiles
* first version of doit in CVS
*
*
* PARAMETERS FOR SFIND GKS ROUTINES
*
* sfgksp.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "doit/sfseq/sfgkspd.inc"
#endif
      INTEGER MSEND
      PARAMETER (MSEND=112)
      INTEGER MCD
      PARAMETER (MCD=14224)
* ------------------------------------------
#include "doit/duseq/duseqd.inc"
