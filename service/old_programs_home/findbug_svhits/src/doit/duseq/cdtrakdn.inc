*
* $Id: cdtrakdn.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: cdtrakdn.inc,v $
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
* CDTRAKCD Documentation New version, untested!
*
* cdtrakdn.inc
*
C
* NTRKIN -  Total # of seed tracks read in
* NTRKCD -  Total # of distinct tracks in the list
* NTR2CD -  Total # of secondary vertex tracks
* LTRKCD -  Address of first duplicate track
* IPNTCD -  Pointer to next distinct track                    (ITRKCD)
* INXTCD -  Address of next track with same NTRACK            (ITRKCD)
* KUHCD  -  Bit pattern for R-PHI track - outward branch      (ITRKCD)
* LUHCD  -  Bit pattern for R-PHI track - inward  branch
* KVHCD  -  Bit pattern for S-Z   track - outward branch
* LVHCD  -  Bit pattern for S-Z   track - inward  branch
* KINCD  -  Track type                                        (ITRKCD)
* NHTDCD -  Number of hit wires/device                   (ITRKCD,IDEV)
* IDRCD  -  Number of corresponding seed track if any         (ITRKCD)
* IPLRCD -  Physical layer of hit on track               (ILYR,ITRKCD)
* IHWRCD -  Packed (+-) wire/layer# of hit on track      (ILYR,ITRKCD)
* CURCD  - Curvature (signed)                                 (ITRKCD)
* PHI0CD - PHI0                                               (ITRKCD)
* D0CD   - Distance of closest approach                       (ITRKCD)
* PHI6CD - PHI kink in R-PHI plane from mult. scattering      (ITRKCD)
*
* TANDCD - TAN(DIP) = COT(THETA)                              (ITRKCD)
* Z0CD   - Distance of closest approach in z                  (ITRKCD)
* XYZ0CD - Co-ords of PCA, or secondary vertex if KINCD=2   (3,ITRKCD)
*
* CHI2CD - CHI**2 per D.O.F. for R-PHI fit                    (ITRKCD)
* CHIZCD - CHI**2 per D.O.F. for Z fit                        (ITRKCD)
* CHI3CD - CHI**2 (total) for 3D fit (-1 if no fit)           (ITRKCD)
*
* RESICD - Fit residuals - scaled by D.O.F                    (ITRKCD)
* PULLCD - Pull (residual) of fit - outward branch       (ILYR,ITRKCD)
*
* ERRMCD - Error matrix (symmetric mode) from 3D fit   (NERMTF,ITRKCD)
*
*       1=(CU,CU)  2=(CU,FI)  4=(CU,DA)  7=(CU,CT) 11=(CU,Z0)
*                  3=(FI,FI)  5=(FI,DA)  8=(FI,CT) 12=(FI,Z0)
*                             6=(DA,DA)  9=(DA,CT) 13=(DA,Z0)
*                                       10=(CT,CT) 14=(CT,Z0)
*                                                  15=(Z0,Z0)
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
