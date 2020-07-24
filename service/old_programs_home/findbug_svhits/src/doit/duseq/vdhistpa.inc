*
* $Id: vdhistpa.inc,v 1.1.1.1 1998/02/06 19:11:50 dpp Exp $
*
* $Log: vdhistpa.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
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
*CMZ :  5.52/00 23/01/90  19.17.53  by  Steve Schaffner
*-- Author :
* Parameters for VDWHST, VDFPIE
*
* vdhistpa.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "seq/rpuser/vfhistpd.inc"
#endif
      INTEGER     NSECT1,  NSECT2,  NSBAND,  MSECN1
      PARAMETER(  NSECT1=64,NSECT2=96,NSBAND=4,MSECN1=2)
      INTEGER     LLYRS1,MLYRS1,LLYRS2,MLYRS2
      INTEGER     LLYRS3,MLYRS3,LLYRS4,MLYRS4
      INTEGER     MLBAND
      PARAMETER(  LLYRS1=1,MLYRS1=3,LLYRS2=4,MLYRS2=8)
      PARAMETER(  LLYRS3=9,MLYRS3=13,LLYRS4=14,MLYRS4=16)
      PARAMETER(  MLBAND=5)
      INTEGER     NLMNS1,NLMNS2,NLMNS3,NLMNS4
      PARAMETER(  NLMNS1=1,NLMNS2=3,NLMNS3=3,NLMNS4=2)
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
