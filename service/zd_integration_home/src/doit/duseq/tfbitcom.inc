*
* $Id: tfbitcom.inc,v 1.2 2001/11/19 23:36:39 dpp Exp $
*
* $Log: tfbitcom.inc,v $
* Revision 1.2  2001/11/19 23:36:39  dpp
*      ->clean up
*
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
*CMZ :  5.54/07 21/05/91  14.42.17  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  19.17.54  by  Steve Schaffner
*-- Author :
*  Variables used in xBITCD routines
*NDEVDR -- Number of devices stored in bit map
*NLYRDR(IDEV) -- Number of layers in device IDEV
      INTEGER IADR,NDEVDR,NLYRDR
      COMMON /CDEVIC/IADR(MXDVCD,2),NDEVDR,NLYRDR(MXDVCD)
