*
* $Id: tftypecd.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tftypecd.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:09  zfiles
* first version of doit in CVS
*
*
*CMZ :  5.54/07 17/05/91  19.19.41  by  Steve Schaffner
*CMZ :  5.53/06 01/08/90  12.31.53  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  19.01.16  by  Steve Schaffner
*-- Author :
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/tftypedo.inc"
#endif
C
      LOGICAL         LSTRUT,        LSTBRN
      LOGICAL         LUSRUT,        LUSBRN
*
      INTEGER         IPLRTF        ,
     +                LETRTF          ,METRTF          ,
     +                IILRTF        ,IMLRTF        ,IOLRTF        ,
     +                IRTPTF        ,IBTPTF        ,
     +                IFETTF        ,NXETTF        ,
     +                MLYRTP
*
      REAL            D0LHTF        ,DIFHTF
*
      COMMON /TFSCRT/ IPLRTF(KLYRDM),
     +                LETRTF(0:KLYRDM),METRTF(0:KLYRDM),
     +                IILRTF(NTYPDM),IMLRTF(NTYPDM),IOLRTF(NTYPDM),
     +                IRTPTF(NETRDM),IBTPTF(NETRDM),
     +                IFETTF(KFETDM),NXETTF(NETRDM),
     +                D0LHTF(NTYPDM),DIFHTF(NETRDM),
     +                MLYRTP(NTYPDM),
     +                LSTRUT(NTYPDM),LSTBRN(NTYPDM),
     +                LUSRUT(NTYPDM),LUSBRN(NTYPDM)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
