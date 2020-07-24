*
* $Id: cdtrakcd.inc,v 1.3 2000/10/24 22:41:49 dpp Exp $
*
* $Log: cdtrakcd.inc,v $
* Revision 1.3  2000/10/24 22:41:49  dpp
*      -> add word to indicate hits that are not to be fit
*
* Revision 1.2  2000/04/26 15:30:20  dpp
*      -> added "missed hit" and "hit not in fit" variables
*
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.10  1996/04/24 01:09:04  zfiles
* Add X0INCD, Y0INCD, MEASCD.
*
* Revision 1.9  1996/04/23 21:38:42  zfiles
* Comments for previous version are wrong!
* True comment:
*      Add: ARC1CD, ARC2CD, X0LHCD, Y0LHCD, ISTVTCD
*      Rename: RESMCD, NN2CCD
*
* Revision 1.8  1996/04/23 07:28:53  zfiles
* Add new stack variables for kalman output.
*
* Revision 1.7  1996/03/12 15:06:55  zfiles
* Add Sig1cd. sig2cd.
*
* Revision 1.6  1996/02/29 17:55:38  zfiles
* added NFITCD and IADFCD to allow silicon hit info into output
*
* Revision 1.5  1996/01/25 16:03:06  zfiles
* incorporate the documentation
* 4 STATCD words
*
* Revision 1.4  1995/06/25 01:16:48  zfiles
* Mods from Rob (and Jon)
* doit/duseq/cdtrakcd.inc
* doit/duseq/cdtrakdo.inc
* Add new variable PATLCD.
* doit/duseq/tfxfitcd.inc
* Change PTLIVx to LOGICAL from INTEGER
*
* Revision 1.3  1995/05/30  18:09:51  zfiles
* Change the name from resmax to resnmx
*
* Revision 1.1.1.1  1994/10/08  01:01:06  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/04 04/08/93  13.33.44  by  Rob Kutschke
*CMZ :  5.54/14 28/09/92  20.34.50  by  Rob Kutschke
*CMZ :  5.54/03 02/04/91  00.03.51  by  Steve Schaffner
*CMZ :  5.53/06 02/08/90  13.35.32  by  Steve Schaffner
*CMZ :  5.53/00 05/04/90  15.59.32  by  Steve Schaffner
*CMZ :  5.52/00 24/01/90  14.44.12  by  Steve Schaffner
*-- Author :
* CD output track COMMON
*
* cdtrakcd.inc
*
#if !defined(CLEO_NOCOMDOC)
* /CDTRAK/  Contains the DUET track output
*
* NTRKIN          - Total # of tracks read in
* NTRKCD          - Total # of distinct tracks in the list
* LTRKCD          - Address of first duplicate track
* IPNTCD(ITRKCD)  - Pointer  to next distinct track
* INXTCD(ITRKCD)  - Addresss of next track with same NTRACK
* CURCD (ITRKCD)  - Curvature (signed)
* PHI0CD(ITRKCD)  - PHI0
* D0CD  (ITRKCD)  - Distance of closest approach
* PHI6CD(ITRKCD)  - PHI kink in R-PHI plane at VD/DR interface
* PHI7CD(ITRKCD)  - PHI kink in R-PHI plane in mid DR
* RKN7CD(ITRKCD)  - radius for kink angle PHI7CD
* TANDCD(ITRKCD)  - TAN(DIP) = COT(THETA)
* Z0CD  (ITRKCD)  - Distance of closest approach
* XYZ0CD(3,ITRKCD) - Co-ords of PCA, or secondary vertex if KINCD=2
* STATCD(4,ITRKCD)  - Track status word (integer)
* 
*
* CHI2CD(ITRKCD)  - Chi**2 per D.O.F. for R-PHI fit
* CHIZCD(ITRKCD)  - Chi**2 per D.O.F. for Z     fit
* CHI3CD(ITRKCD)  - Chi**2 (total) for 3D       fit (-1 if no fit)
*
* RESICD(ITRKCD)  - Fit residuals - scaled by D.O.F
*
C* KUHCD (ITRKCD)  - Bit pattern for R-PHI track - outward branch (commented
C* LUHCD (ITRKCD)  - Bit pattern for R-PHI track - inward  branch    out
C* KVHCD (ITRKCD)  - Bit pattern for S-Z   track - outward branch    in
C* LVHCD (ITRKCD)  - Bit pattern for S-Z   track - inward  branch   code)
*
* KINCD (ITRKCD)  - KINDR for track # ITRKCD

* NHTSVX(ITRKCD)  - Number of hits in silicon vertex r-phi
* NHTSVZ(ITRKCD)  - Number of hits in silicon vertex Z
* NHTLVD(ITRKCD)  - Number of hit VD wires
* NHTAIZ(ITRKCD)  - Number of hit IZ anode wires
* NHTADR(ITRKCD)  - Number of hit DR anode wires
* NHTSCT(ITRKCD)  - Number of hit CT cathode wires
* NHTSDR(ITRKCD)  - Number of hit DR stereo wires

*                   missed hits are to take into account track parameters
* NMSSVX(ITRKCD)  - Number of MISSED hits in silicon vertex r-phi
* NMSSVZ(ITRKCD)  - Number of MISSED hits in silicon vertex Z
* NMSLVD(ITRKCD)  - Number of MISSED hits in VD wires
* NMSAIZ(ITRKCD)  - Number of MISSED hits in IZ anode wires
* NMSADR(ITRKCD)  - Number of MISSED hits in DR anode wires
* NMSSCT(ITRKCD)  - Number of MISSED hits in CT cathode wires
* NMSSDR(ITRKCD)  - Number of MISSED hits in DR stereo wires
*
* NFITCD(ITRKCD)  - Number of hits in the track
* IDRCD (ITRKCD)  - Number of seed track for this ITRKCD
*
* KBQDCD(ITRKCD)  - Bitmask of VD layers used in QDIV
* CHI3PT(ITRKCD)  - (CHI2 from PT anodes)/( Number PT anodes)
* CHI3VD(ITRKCD)  - (CHI2 from VD anodes)/( Number VD anodes)
* CHI3DR(ITRKCD)  - (CHI2 from DR anodes)/( Number DR anodes)
* CHI3CT(ITRKCD)  - (CHI2 from cathodes) /( Number CT hits)
*
* NDOFCD(ITRKCD)  - Number of degrees of freedom in chi2.
* RESMCD(ITRKCD)  - Magnitude of largest normalized residual.
* NN2CCD(ITRKCD)  - Nearest neighbour 2pt correlation of residuals.
*
* ARC1CD(ITRKCD)  - 3D path length from PCAO to PCA outermost hit.
* ARC2CD(ITRKCD)  - 3D path length from PCAO to impact on TOF system.
* X0LHCD(ITRKCD)  - X origin of coord system at outermost hit.
* Y0LHCD(ITRKCD)  - Y origin of coord system at outermost hit.
* X0INCD(ITRKCD)  - X origin of coord system for track stopped outside svd
* Y0INCD(ITRKCD)  - X origin of coord system for track stopped outside svd
* ISVTCD(ITRKCD)  - Number of SVHT track added to this track.
*
* IPLRCD(ILYR,ITRKCD) - Physical layer of hit on track
* IHWRCD(ILYR,ITRKCD) - (+-) wire # of hit on track
* BUCKCD(ILYR,ITRKCD) - passing the buck on whether hit should be fit
*                       do not fit hits with non-zero BUCKCD
* PULLCD(ILYR,ITRKCD) - Pull (residual) of fit - outward branch
* MEASCD(ILYR,ITRKCD) - Measurement at this hit. ( ie drift distance for
*                       anode; position on wafer for SVD ).
* IADFCD(ILYR,ITRKCD) - DUET address of hit on track
* IN_FIT_CD(ILYR,ITRKCD) - flag, =0 if hit is not in fit, =1 if in fit 
* SIG1CD(ILYR,ITRKCD) - Resolution used in fit; measurement component
* SIG2CD(ILYR,ITRKCD) - signed contribution to resolution from other hits.
* ERRMCD(IERM,ITRKCD) - Error matrix (symmetric mode) from 3D fit
*
*       1=(CU,CU)  2=(CU,FI)  4=(CU,DA)  7=(CU,CT) 11=(CU,Z0)
*                  3=(FI,FI)  5=(FI,DA)  8=(FI,CT) 12=(FI,Z0)
*                             6=(DA,DA)  9=(DA,CT) 13=(DA,Z0)
*                                       10=(CT,CT) 14=(CT,Z0)
*                                                  15=(Z0,Z0)
*
*
#endif

      INTEGER         NTRKIN,NTRKCD,LTRKCD,
     + IPNTCD        ,INXTCD        ,
     + KINCD         ,
     + NHTSVX        ,NHTSVZ        ,
     + NHTLVD        ,NHTAIZ        ,NHTADR        ,
     + NHTSCT        ,NHTSDR        ,
     + NMSSVX        ,NMSSVZ        ,
     + NMSLVD        ,NMSAIZ        ,NMSADR        ,
     + NMSSCT        ,NMSSDR        ,
     + IDRCD         ,STATCD,KBQDCD,
     + NDOFCD        ,ISVTCD,
     + IPLRCD               ,
     + IHWRCD        ,BUCKCD,
     + NFITCD        ,IADFCD

      COMMON /CDTRKI/ NTRKIN,NTRKCD,LTRKCD,
     + IPNTCD(MTRKCD),INXTCD(MTRKCD),
     + KINCD (MTRKCD),
     + NHTSVX(MTRKCD),NHTSVZ(MTRKCD),
     + NHTLVD(MTRKCD),NHTAIZ(MTRKCD),NHTADR(MTRKCD),
     + NHTSCT(MTRKCD),NHTSDR(MTRKCD),
     + NMSSVX(MTRKCD),NMSSVZ(MTRKCD),
     + NMSLVD(MTRKCD),NMSAIZ(MTRKCD),NMSADR(MTRKCD),
     + NMSSCT(MTRKCD),NMSSDR(MTRKCD),
     + IDRCD (MTRKCD),STATCD(4,MTRKCD),KBQDCD(MTRKCD),
     + NDOFCD(MTRKCD),
     + IPLRCD(KLR2TF,MTRKCD),
     + IHWRCD(KLR2TF,MTRKCD),
     + NFITCD(MTRKCD),
     + IADFCD(KLR2TF,MTRKCD),BUCKCD(KLR2TF,MTRKCD)

      REAL
     + CURCD         ,PHI0CD        ,D0CD          ,
     + PHI6CD        ,PHI7CD        ,RKN7CD        ,
     + TANDCD        ,Z0CD          ,XYZ0CD        ,
     + CHI2CD        ,CHIZCD        ,CHI3CD        ,
     + RESICD        ,CHI3PT        ,
     + CHI3VD        ,CHI3DR        ,CHI3CT        ,
     + RESMCD        ,NN2CCD        ,ARC1CD        ,
     + ARC2CD        ,X0LHCD        ,Y0LHCD        ,
     + PULLCD        ,ERRMCD        ,SIG1CD        ,
     + SIG2CD        ,MEASCD        ,X0INCD        ,
     + Y0INCD

      COMMON /CDTRKR/
     + CURCD (MTRKCD),PHI0CD(MTRKCD),D0CD  (MTRKCD),
     + PHI6CD(MTRKCD),PHI7CD(MTRKCD),RKN7CD(MTRKCD),
     + TANDCD(MTRKCD),Z0CD  (MTRKCD),XYZ0CD(3,MTRKCD),
     + CHI2CD(MTRKCD),CHIZCD(MTRKCD),CHI3CD(MTRKCD),
     + RESICD(MTRKCD),CHI3PT(MTRKCD),
     + CHI3VD(MTRKCD),CHI3DR(MTRKCD),CHI3CT(MTRKCD),
     + RESMCD(MTRKCD),NN2CCD(MTRKCD),ARC1CD(MTRKCD),
     + ARC2CD(MTRKCD),X0LHCD(MTRKCD),Y0LHCD(MTRKCD),
     + ISVTCD(MTRKCD),X0INCD(MTRKCD),Y0INCD(MTRKCD),
     + PULLCD(KLR2TF,MTRKCD),ERRMCD(NERMTF,MTRKCD),
     + SIG1CD(KLR2TF,MTRKCD),SIG2CD(KLR2TF,MTRKCD),
     + MEASCD(KLR2TF,MTRKCD)
