*
* $Id: tfhistpd.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tfhistpd.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:09  zfiles
* first version of doit in CVS
*
*
* TFHISTPA Documentation
*
* tfhistpd.inc
*
C
* NHWCD  - Number of hits  used in /CDSCRT/
* NHHIT  - Number of hits  used in /TFHITS/
* NHLNK  - Number of links used in /TFLINK/
* NHLMB  - Number of next or prev links used in /TFLINK/
* NHCHN  - Number of RUT or BRN chains  used in  TFCLMB
* NHRBL  - Number of RUT or BRN links   used in  TFCLMB
* NHTRK  - Number of tracks used in /TFTRKS/
* NHLKT  - Number of links  used in /TFTRKS/
*
* NHDFCF - CF Difference of DIFCCF
* NHDLCF - CF D0 (Link in track)
* NHRSCF - CF Residuals
* NHSRCF - CF Scaled residuals
* NHPRCF - CF Probability of fit
* NHDTCF - CF D0 (Track)
* NHDACF - CF D0 (All links)
* NHDRCF - CF Residuals in D
* NHDRSF - SF Residuals in D
*
* NHDFLF - LF Difference of DIFCLF
* NHDLLF - LF Z0 (Link in track)
* NHRSLF - LF Residuals
* NHSRLF - LF Scaled residuals
* NHPRLF - LF Probability of fit
* NHDTLF - LF Z0 (Track)
* NHDALF - LF Z0 (All links)
* NHDRLF - LF Scaled residuals in D
*
* NHT0CF - CFIND Time for NHITS = NLYRHT
* NHT1CF - CFIND Time for NHITS = NLYRHT - 1
* NHT2CF - CFIND Time for NHITS = NLYRHT - 2
* NHT3CF - CFIND Time for NHITS < NLYRHT - 2
*
* NHT0LF - LFIND Time for NHITS = NLYRHT
* NHT1LF - LFIND Time for NHITS = NLYRHT - 1
* NHT2LF - LFIND Time for NHITS = NLYRHT - 2
* NHT3LF - LFIND Time For NHITS < NLYRHT - 2
*
* NHPRTF - TF Probability of 3D fit
* NHRSTF - TF Residuals - All    layers
* NHRATF - TF Residuals - Axial  layers
* NHRZTF - TF Residuals - Stereo layers
* NHQFTF - Pulse height on anode wire used in fit
* NTIMPT - Time on PT wire used in fit
* NTIMVD - Time on VD wire used in fit
* NTIMDR - Time on DR wire used in fit
*
* NHPRPF - TF Probability of 3D fit     with P cut
* NHRSPF - TF Residuals - all layers    with P cut
* NHRAPF - TF Residuals - axial layers  with P cut
* NHRZPF - TF Residuals - stereo layers with P cut
C
* NHANFT - Number of anode   layers in fit
* NHCAFT - Number of cathode layers in fit
C
* NHK0TF - PI+ PI- (K0) mass - 2 MeV bins
* NHK5TF - PI+ PI- (K0) mass - 5 MeV bins
* NHL0TF - PI  P   (L0) mass
* NHGRTF - Radius of conversion for photons
C
* NHD0TF - D0(1)+D0(2) for 2-prongs
* NHDMTF - D0(1)+D0(2) 2-prongs, > 90% of VD hits
*
* NHRITF - IV/PT residuals
* NHINFT - Number of IV/PT hits used in fit
* NHQIVD - Pulse in IV/PT wire used in fit
* NHIVBF - IV/PT hit before corrections
* NHIVAF - IV/PT hit after  corrections
*
* NHRVTF - VD rsiduals
* NHVNFT - Number of VD hits used in fit
* NHQ1VD - Pulse on VD wire used in fit (1 end)
* NHQ2VD - Pulse on VD wire used in fit (2 ends)
* NHVWBF - VD hit before corrections
* NHVWAF - VD hit after  corrections
* NHRQTF - VD z-resolution from Q-div
*
* NHRDTF - DR residuals
* NHDRFT - Number of DR hits used in fit
* NHDAFT - Number of DR axial  hits in fit
* NHDSFT - Number of DR stereo hits in fit
* NHQFDR - Pulse height on anode wire used in fit
* NHDWBF - DWCD before full corrections
* NHDWAF - DWCD after  full corrections
* NHERCU - SQRT(ERMFIT(1 ))  - CU
* NHERFI - SQRT(ERMFIT(3 ))  - FI
* NHERD0 - SQRT(ERMFIT(6 ))  - D0
* NHERTD - SQRT(ERMFIT(10))  - TD
* NHERZ0 - SQRT(ERMFIT(15))  - Z0
* NHERP6 - SQRT(ERMFIT(21))  - PHI6
*
* NHDRT0 - DR axial resid (PULL*SIGN(DRIFT)) vs drift distance
* NHPTT0 - PT         "    etc
* NHVDT0 - VD         "
* NHCOR3 - DR axial resid vs resid of previous layer (0.3 - 0.5 GeV)
* NHCOR1 - DR axial resid vs resid of previous layer (p > 1.0 GeV)
* NHCOPT - PT axial resid vs resid of previous layer
* NHCOVD - VD axial resid vs resid of previous layer
*
* NHCTDS - Time on each VD cathode  - anode time
* NHCTDP - Time on peak VD cathode  - anode time
* NHCTQS - Charge on VD cathode strip
* NHCTQP - Charge on peak VD cathode strip
* NHCTQC - Total charge on VD cathode cluster
* NHCTNS - Number of cathode strips in cluster
* NHRCTF - VD cathode residual
* NHCNFT - Number of VD cathodes in fit
*
* NHDCDS - Time on each DR cathode  - anode time
* NHDCDP - Time on peak DR cathode  - anode time
* NHDCQS - Charge on DR cathode strip
* NHDCQP - Charge on peak DR cathode strip
* NHDCQC - Total charge on DR cathode cluster
* NHDCNS - Number of cathode strips in cluster
* NHRDCF - DR cathode residual
* NHDCFT - Number of DR cathodes in fit
*
* NHMCNI - Number of iterations for 3D fit
* NHMCX2 - Chi2/dof for 3D fit
* NHMCPR - Probability of 3D fit
* NHMCRS - Residuals from 3D fit (microns)
* NHMCCU - KAPPA(meas) - KAPPA(start)
* NHMCFI - PHI(meas) - PHI(start)
* NHMCD0 - D0(meas) - D0(start)
* NHMCTD - TANDIP(meas) - TANDIP(start)
* NHMCZ0 - Z0(meas) - Z0(start)
* NHMCIP - DELTA (1/P)  = DP/P**2
* NHMCAP - ABS(P)/EBEAM
*
* NHMC12 - PCLE #1 PCLE #2 MASS
* NHMCP1 - Momentum of decay PCLE #1
* NHMCP2 - Momentum of decay PCLE #2
* NHMCD1 - DELU(1)
* NHMCD2 - DELU(2)
* NHMCD3 - ACOS(DELU(3))
* NHMCAK - Photon energy K ( EGAMMA/EBEAM)
* NHMCZE - DELTA:   MU+ MU- Acollinearity
* NHMCXM - P / EBEAM
* NHMCCT - COS(THETA)
* NHMCPM - ABS(PQDR(N))/EBEAM  **Mupair**
* NHMCAM - ALPHA               **Mupair**
* NHMCKM - KAPPA(meas) - KAPPA(pred)   **Mupair**
* NHMCDM - d0
* NHMCSM - d0(1) + d0(2)
*
* NHMD0  - D0 mass (cutting on delta M)
* NHMDIF - Delta M (cutting on D0 mass)
* NHDST1 - Momentum of PI+ (from D*)
* NHDST2 - Momentum of K-  (from D0)
* NHDST3 - Momentum of PI+ (from D0)
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
