*
* $Id: tfunitcd.inc,v 1.2 2001/11/19 23:28:03 dpp Exp $
*
* $Log: tfunitcd.inc,v $
* Revision 1.2  2001/11/19 23:28:03  dpp
*     -> merge in documentation
*
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
*CMZ :  5.54/07 17/05/91  19.19.41  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  19.01.16  by  Steve Schaffner
*-- Author :
*
#if !defined(CLEO_NOCOMDOC)
* /TFUNIT/ Passes the logical unit numbers for I/O routines
* IUNLPT - Line printer
* IUNTMP - Temp unit (must be opened & closed within SUBROUTINE)
* IUNTTR - Unit for reading from terminal
* IUNTTW - Unit for writing to   terminal
* IUNDGN - Unit for diagnostic printout (multiple width)
#endif

      INTEGER         IUNLPT,IUNTMP,IUNTTR,IUNTTW,IUNDGN
      COMMON /TFUNIT/ IUNLPT,IUNTMP,IUNTTR,IUNTTW,IUNDGN
