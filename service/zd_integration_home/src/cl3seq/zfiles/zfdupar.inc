*
* $Id: zfdupar.inc,v 1.2 1998/05/08 14:16:09 pg Exp $
*
* $Log: zfdupar.inc,v $
* Revision 1.2  1998/05/08 14:16:09  pg
*  Forgot to rename one include file.
*
* Revision 1.1  1998/05/07 20:39:40  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.3  1996/04/23 07:08:25  zfiles
* Mods from Rob. Add offsets to access variables in TRHT bank.
*
* Revision 1.2  1996/03/12 15:05:30  zfiles
* Add BDOFCD RESNMX NN2COR and KBITXT offsets
*
* Revision 1.1.1.1  1994/10/04 20:58:43  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/38 24/08/93  23.38.38  by  Rob Kutschke
*CMZ :  1.02/37 09/08/93  09.39.02  by  Rob Kutschke
*CMZ :  5.54/15 03/10/92  21.02.25  by  Rob Kutschke
*CMZ :  5.54/14 28/09/92  20.36.21  by  Rob Kutschke
*CMZ :  5.52/00 23/01/90  19.17.54  by  Steve Schaffner
*-- Author :
C
C Parameter definitions for the structure of the duet track banks.
C
#if !defined(CLEO_NOCOMDOC)
#include "cl3seq/zfiles/zfdupard.inc"
#endif
*
* Offsets to variables in ZEBRA banks (only analyzed data now):
*
      INTEGER JHNAME, JNHTWR, JHTTRK, JGDTRK, JBADTR, JNVTRK, JXVDR,
     +  JYVDR,
     +  JZVDR, JDXVDR, JDYVDR, JDZVDR, JKINCD, JMISCD, JKBIT,
     +  JRESID, JRKN6, JPMASK, JCURCD, JPHI0, JD0CD, JCOTAN, JZ0CD,
     +  JFKN6,  JFKN7,  JRKN7, JERRM,
     +  JCH3CD, JCH3PT, JCH3VD, JCH3DR, JCH3CT
      INTEGER JNDFCD, JRESMX, JN2COR, JKBITX

C     For TRHT:
      INTEGER JTRHTV, JNHITS, JITSVX, JSTAT4, JSTAT3, JARCL1, JARCL2

C
C     Adjustments for variables which come after the error matrix.
C     ( to allow the error matrix to be of variable length )
      INTEGER IOFFK7, IOFFKL

      PARAMETER ( JHNAME = 1,  JHTTRK = 5,  JGDTRK =  7, JBADTR =  8)
      PARAMETER ( JNVTRK = 1,  JXVDR  = 2,  JYVDR  =  3, JZVDR  =  4)
      PARAMETER ( JDXVDR = 5,  JDYVDR = 6,  JDZVDR =  7, JKINCD =  1)
      PARAMETER ( JMISCD = 2,  JKBIT  = 3,  JRESID =  8, JRKN6  =  9)
      PARAMETER ( JPMASK = 10, JCURCD = 11, JPHI0  = 12, JD0CD  = 13)
      PARAMETER ( JCOTAN = 14, JZ0CD  = 15, JFKN6  = 16, JERRM  = 17)
      PARAMETER ( JNHTWR =  3)
      PARAMETER ( JFKN7  = 45, JRKN7  = 46)
      PARAMETER ( JCH3CD = 47, JCH3PT = 48, JCH3VD = 49, JCH3DR = 50)
      PARAMETER ( JCH3CT = 51 )
      PARAMETER ( JNDFCD = 52 )
      PARAMETER ( JRESMX = 53 )
      PARAMETER ( JN2COR = 54 )
      PARAMETER ( JKBITX = 55 )
*
      PARAMETER ( IOFFK7 =  9 )
      PARAMETER ( IOFFKL = 15 )

C     For TRHT.
      PARAMETER ( JTRHTV = 1,  JNHITS = 2,  JITSVX = 3 )
      PARAMETER ( JSTAT3 = 4,  JSTAT4 = 5)
      PARAMETER ( JARCL1 = 6,  JARCL2 = 7 )
