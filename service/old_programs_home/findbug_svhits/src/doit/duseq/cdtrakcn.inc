*
* $Id: cdtrakcn.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: cdtrakcn.inc,v $
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
*CMZ :  5.54/07 17/05/91  19.19.38  by  Steve Schaffner
*CMZ :  5.53/06 02/08/90  13.35.32  by  Steve Schaffner
*CMZ :  5.52/00 24/01/90  14.44.35  by  Steve Schaffner
*-- Author :
* CD output track COMMON. New version untested!
*
* cdtrakcn.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/cdtrakdn.inc"
#endif
C
      INTEGER NTRKIN, NTRKCD, NTR2CD, LTRKCD, IPNTCD,
     +        INXTCD, KUHCD,  KVHCD,  LUHCD,  LVHCD,  KINCD,  IDRCD,
     +        NHTDCD, IPLRCD, IHWRCD
C
      COMMON /CDTRKI/
     +       NTRKIN, NTRKCD, NTR2CD, LTRKCD,
     +       IPNTCD(MTRKCD), INXTCD(MTRKCD),
     +       KUHCD (MTRKCD), KVHCD (MTRKCD),
     +       LUHCD (MTRKCD), LVHCD (MTRKCD),
     +       KINCD (MTRKCD), IDRCD (MTRKCD),
     +       NHTDCD(MTRKCD,MXDVCD),
     +       IPLRCD(KLR2TF,MTRKCD),
     +       IHWRCD(KLR2TF,MTRKCD)
C
      REAL CURCD,  PHI0CD, D0CD,   PHI6CD, TANDCD, Z0CD,  XYZ0CD,
     +     CHI2CD, CHIZCD, CHI3CD, RESICD, PULLCD, ERRMCD
C
      COMMON /CDTRKR/
     + CURCD (MTRKCD), PHI0CD(MTRKCD), D0CD  (MTRKCD), PHI6CD(MTRKCD),
     + TANDCD(MTRKCD), Z0CD  (MTRKCD), XYZ0CD(3,MTRKCD),
     + CHI2CD(MTRKCD), CHIZCD(MTRKCD), CHI3CD(MTRKCD),
     + RESICD(MTRKCD),
     + PULLCD(KLR2TF,MTRKCD), ERRMCD(NERMTF,MTRKCD)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
