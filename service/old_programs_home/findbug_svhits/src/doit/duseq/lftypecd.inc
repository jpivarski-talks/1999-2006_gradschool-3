*
* $Id: lftypecd.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: lftypecd.inc,v $
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
*CMZ :  5.54/07 17/05/91  19.19.39  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.48.23  by  Steve Schaffner
*-- Author :
*
* lftypecd.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/lftypedo.inc"
#endif
C
      INTEGER         NLRULF, IPLRLF,
     +                LETRLF, METRLF,
     +                IILRLF, IOLRLF,
     +                IRTPLF, IBTPLF,
     +                IFETLF, NXETLF
      COMMON /LFTYPI/ NLRULF, IPLRLF(KLYRLF),
     +                LETRLF(0:KLYRLF), METRLF(0:KLYRLF),
     +                IILRLF(NTYPLF), IOLRLF(NTYPLF),
     +                IRTPLF(NETRLF), IBTPLF(NETRLF),
     +                IFETLF(KFETLF), NXETLF(NETRLF)
*
      REAL            D0LHLF, DIFHLF
      COMMON /LFTYPR/ D0LHLF(NTYPLF), DIFHLF(NETRLF)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
