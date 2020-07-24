*
* $Id: cfctlcde.inc,v 1.2 2001/11/19 23:17:28 dpp Exp $
*
* $Log: cfctlcde.inc,v $
* Revision 1.2  2001/11/19 23:17:28  dpp
*      -> remove unneeded variables, including LCFIND
*      -> merge in documentation
*
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:07  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/21 28/03/94  14.33.04  by  Rob Kutschke
*CMZ :  5.54/36 03/06/93  10.00.11  by  Dan Peterson
*CMZ :  5.54/14 14/01/92  12.09.12  by  Steve Schaffner
*CMZ :  5.53/07 08/10/90  12.03.40  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.48.22  by  Steve Schaffner
*-- Author :
*  CF cuts sequence
*
* cfctlcde.inc
*
#if !defined(CLEO_NOCOMDOC)
* IFLGCF - = IFGEOM If CFIND geometry arrays have been filled
*          = IFINIT if CFIND links & trees have been defined
* MLRLCF - Max layer for tracks with the minimum # of drift hits
* SIGLCF - # of sigma for picking up .NOT.LUSE hits
* ERRCAN - Scale factor for throwing away hits in CDCANH
* Control flags:
* LCFXLA - in CFXFIT, use local abiguity resolution
* LCFXLI - in TFGMI2, force PT live wire list to all live.
#endif

      INTEGER         IFLGCF, MLRLCF
      COMMON /CFCTLI/ IFLGCF, MLRLCF

      REAL    SIGLCF, ERRCAN

      COMMON /CFCTLR/ SIGLCF, ERRCAN

      LOGICAL         LCFXLA, LCFXLI
      COMMON /CFCTLL/ LCFXLA, LCFXLI









