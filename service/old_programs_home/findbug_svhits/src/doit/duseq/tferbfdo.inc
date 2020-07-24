*
* $Id: tferbfdo.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tferbfdo.inc,v $
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
* TFERBFCD Documentation
*
* tferbfdo.inc
*
C
* /TFERBF/ is the error reporting buffer
* NTFEBF - Number of errors in buffer
* ITFEBF(1,IERR) -  Run    number
* ITFEBF(2,IERR) -  Event  number
* ITFEBF(3,IERR) -  Record number
* ITFEBF(4,IERR) -  Track  number
* ITFEBF(5,IERR) -  Error code  (ISUB*IERPAC + IERTYP)
* ITFEBF(6,IERR) -  Error parameter
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
