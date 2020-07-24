*
* $Id: cfctlcde.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: cfctlcde.inc,v $
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
#include "doit/duseq/cfctldoc.inc"
#endif

      INTEGER         IFLGCF, MLRLCF, NDRPCF
      COMMON /CFCTLI/ IFLGCF, MLRLCF, NDRPCF

      REAL    GMINCF, SIGLCF, SGYSCF, DIFSCF,
     +                ERRHCF, ERRCAN,
     +                D0MAX , D0MAX1, D0MAX2,
     +                        CH2CF1, CH2CF2, CH2CFD,
     +                        CH2CD1, CH2CD2, RCCHOP

      COMMON /CFCTLR/ GMINCF, SIGLCF, SGYSCF, DIFSCF,
     +                ERRHCF, ERRCAN,
     +                D0MAX , D0MAX1, D0MAX2,
     +                        CH2CF1, CH2CF2, CH2CFD,
     +                        CH2CD1, CH2CD2, RCCHOP

      LOGICAL         LSFCMP, LCFIND, LCFXLA, LCFXPR, LCFXLI
      COMMON /CFCTLL/ LSFCMP, LCFIND, LCFXLA, LCFXPR, LCFXLI
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"









