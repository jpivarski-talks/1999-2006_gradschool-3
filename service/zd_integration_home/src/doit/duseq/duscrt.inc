*
* $Id: duscrt.inc,v 1.2 2001/11/19 23:35:23 dpp Exp $
*
* $Log: duscrt.inc,v $
* Revision 1.2  2001/11/19 23:35:23  dpp
*      ->clean up
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:10  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/07 17/05/91  19.19.42  by  Steve Schaffner
*CMZ :  5.53/06 01/08/90  11.38.16  by  Steve Schaffner
*-- Author :
C     This is a scratch area for writing large local arrays.  It is currently
C     used in TF3FIT, CFCUTS, TFBETR, and CDDUPL; it's size is governed by
C     the requirements of TF3FIT.  Change it with care!

      INTEGER GOG
      COMMON/DUSCRT/GOG((2*NP3DTF+9)*KLR2TF + 2*KGYMTF)

