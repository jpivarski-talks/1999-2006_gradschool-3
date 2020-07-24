*
* $Id: tfunitdo.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tfunitdo.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:10  zfiles
* first version of doit in CVS
*
*
* TFUNITCD Documentation
*
* tfunitdo.inc
*
C
* /TFUNIT/ Passes the logical unit numbers for I/O routines
*
* IUNLPT - Line printer
* IUNTMP - Temp unit (must be opened & closed within SUBROUTINE)
* IUNTTR - Unit for reading from terminal
* IUNTTW - Unit for writing to   terminal
* IUNDGN - Unit for diagnostic printout (multiple width)
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
