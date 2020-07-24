*
* $Id: lfctlcde.inc,v 1.4 2003/03/05 17:35:09 dpp Exp $
*
* $Log: lfctlcde.inc,v $
* Revision 1.4  2003/03/05 17:35:09  dpp
*      -> remove unused variables, qcrlct,qpkmct,qminct,etc. ,sigllf
*
* Revision 1.3  2001/11/19 23:27:01  dpp
*      -> remove unneeded variables
*      -> merge in documentation
*
* Revision 1.2  2000/10/02 21:16:22  dpp
*     -> move Z0BIAS to tfctlcde
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:07  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/14 14/01/92  12.09.12  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.48.23  by  Steve Schaffner
*-- Author :
* LF cuts sequence
*
* lfctlcde.inc
*
#if !defined(CLEO_NOCOMDOC)
* /LFCTL / Defines the cuts for finding linear tracks
*
* IFLGLF - = IFGEOM if LFIND geometry arrays have been filled
*          = IFINIT if LFIND links & trees have been defined
* YHTCUT - Maximum Z for hits in the stereo layers
#endif
C
      INTEGER         IFLGLF
      COMMON /LFCTLI/ IFLGLF
*
      REAL            YHTCUT
      COMMON /LFCTLR/ YHTCUT






