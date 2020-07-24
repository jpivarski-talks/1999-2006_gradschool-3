*
* $Id: tferbfcd.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tferbfcd.inc,v $
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
*CMZ :  5.52/00 23/01/90  21.48.23  by  Steve Schaffner
*-- Author :
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/tferbfdo.inc"
#endif
C
      INTEGER         NTFEBF, ITFEBF
      COMMON /TFERBF/ NTFEBF, ITFEBF(NERWRD,NERBDM)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
