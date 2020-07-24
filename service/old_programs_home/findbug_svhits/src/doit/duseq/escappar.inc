*
* $Id: escappar.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: escappar.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:11  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/01 30/03/93  13.09.47  by  Dan Peterson
*CMZ :  3.01/01 28/03/90  17.22.36  by  Dan Peterson
*CMZ :          15/05/89  13.39.18  by  Dan Peterson
*           VELOCITY STORAGE NOW OF SIZE=VSTORF
*CMZ :          15/05/89  13.36.09  by  Dan Peterson
*CMZ :  2.00/00 05/05/89  10.35.18  by  Dan Peterson
*-- Author :
* Parameters for CFLAST
*
* escappar.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/escappdo.inc"
#endif
      REAL RESTST
      INTEGER NLNED1,NLNED2,NLNED3,NOUTA1,NOUTA2,NOUTA3
      INTEGER RESCC1,RESCC2,RESCC3,RESCC4,RESCC5
      INTEGER RESCC6,RESCC7,RESCC8,RESCC9
C
      PARAMETER (RESTST=.008)
      PARAMETER (NLNED1=1,NOUTA1=3)
      PARAMETER (NLNED2=7,NOUTA2=14)
      PARAMETER (NLNED3=2,NOUTA3=5)
C
C RESCC1 thru RESCC9  are device dependent; calculated
C with TF layer numbers and the radii and lengths of the devices
      PARAMETER (RESCC1=12)
      PARAMETER (RESCC2=16)
      PARAMETER (RESCC3=20)
      PARAMETER (RESCC4= 7)
      PARAMETER (RESCC5=10)
      PARAMETER (RESCC6=24)
      PARAMETER (RESCC7= 7)
      PARAMETER (RESCC8=14)
      PARAMETER (RESCC9=31)











