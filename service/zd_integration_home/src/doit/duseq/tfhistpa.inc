*
* $Id: tfhistpa.inc,v 1.3 2003/03/05 17:36:54 dpp Exp $
*
* $Log: tfhistpa.inc,v $
* Revision 1.3  2003/03/05 17:36:54  dpp
*      -> changed all variable names in cdscrtcd to have common root,comment
*
* Revision 1.2  2001/11/19 23:25:34  dpp
*      -> merge in documentation
*
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
*CMZ :  5.54/07 20/08/91  21.19.52  by  Steve Schaffner
*CMZ :  5.54/03 16/05/91  22.11.18  by  Steve Schaffner
*CMZ :  5.53/06 16/08/90  14.24.27  by  Steve Schaffner
*CMZ :  5.53/04 05/06/90  20.07.53  by  Steve Schaffner
*CMZ :  5.53/00 10/04/90  10.58.42  by  Steve Schaffner
*CMZ :  5.52/00 23/01/90  21.48.21  by  Steve Schaffner
*-- Author :
* PARAMETERS Defining Duet histogram numbers
*
* tfhistpa.inc
*
#if !defined(CLEO_NOCOMDOC)
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
* NHDWBF - EvntHit_Drift before full corrections
* NHDWAF - EvntHit_Drift after  full corrections
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
#endif
*
      INTEGER    NHHWCD       , NHHIT        , NHLNK
      PARAMETER( NHHWCD = 1001, NHHIT  = 1002, NHLNK  = 1003 )
      INTEGER    NHLMB        , NHCHN        , NHRBL
      PARAMETER( NHLMB  = 1004, NHCHN  = 1005, NHRBL  = 1006 )
      INTEGER    NHTRK        , NHLKT
      PARAMETER( NHTRK  = 1007, NHLKT  = 1008                )
*
      INTEGER    NHDFCF       , NHDLCF       , NHRSCF
      PARAMETER( NHDFCF = 1011, NHDLCF = 1012, NHRSCF = 1013 )
      INTEGER    NHSRCF       , NHPRCF       , NHDTCF
      PARAMETER( NHSRCF = 1014, NHPRCF = 1015, NHDTCF = 1016 )
      INTEGER    NHDACF       , NHDRCF       , NHDRSF
      PARAMETER( NHDACF = 1017, NHDRCF = 1018, NHDRSF = 1019 )
*
      INTEGER    NHDFLF       , NHDLLF       , NHRSLF
      PARAMETER( NHDFLF = 1021, NHDLLF = 1022, NHRSLF = 1023 )
      INTEGER    NHSRLF       , NHPRLF       , NHDTLF
      PARAMETER( NHSRLF = 1024, NHPRLF = 1025, NHDTLF = 1026 )
      INTEGER    NHDALF       , NHDRLF
      PARAMETER( NHDALF = 1027, NHDRLF = 1028                )
*
      INTEGER    NHT0CF       , NHT1CF       , NHT2CF
      PARAMETER( NHT0CF = 1030, NHT1CF = 1031, NHT2CF = 1032 )
      INTEGER    NHT3CF
      PARAMETER( NHT3CF = 1033                               )
*
      INTEGER    NHT0LF       , NHT1LF       , NHT2LF
      PARAMETER( NHT0LF = 1035, NHT1LF = 1036, NHT2LF = 1037 )
      INTEGER    NHT3LF
      PARAMETER( NHT3LF = 1038                               )
*
      INTEGER    NHPRTF       , NHRSTF       , NHRATF
      PARAMETER( NHPRTF = 1040, NHRSTF = 1041, NHRATF = 1042 )
      INTEGER    NHRZTF       , NHQFTF
      PARAMETER( NHRZTF = 1043, NHQFTF = 1047                )
*
      INTEGER    NHPRPF       , NHRSPF       , NHRAPF
      PARAMETER( NHPRPF = 1050, NHRSPF = 1051, NHRAPF = 1052 )
      INTEGER    NHRZPF
      PARAMETER( NHRZPF = 1053                               )
*
      INTEGER    NHCDFT       , NHANFT       , NHCAFT
      PARAMETER( NHCDFT = 1055, NHANFT = 1056, NHCAFT = 1057 )
*
      INTEGER    NHK0TF       , NHK5TF       , NHL0TF
      PARAMETER( NHK0TF = 1060, NHK5TF = 1061, NHL0TF = 1062 )
      INTEGER    NHGRTF
      PARAMETER( NHGRTF = 1065                               )
*
      INTEGER    NHD0TF       , NHDMTF
      PARAMETER( NHD0TF = 1070, NHDMTF = 1071                )
*
      INTEGER    NHRITF       , NHINFT       , NHQIVD
      PARAMETER( NHRITF = 1200, NHINFT = 1220, NHQIVD = 1225 )
      INTEGER    NHIVBF       , NHIVAF
      PARAMETER( NHIVBF = 1230, NHIVAF = 1231                )
*
      INTEGER    NHRVTF       , NHVNFT       , NHQ1VD
      PARAMETER( NHRVTF = 1300, NHVNFT = 1330, NHQ1VD = 1335 )
      INTEGER    NHQ2VD       , NHVWBF       , NHVWAF
      PARAMETER( NHQ2VD = 1336, NHVWBF = 1340, NHVWAF = 1341 )
      INTEGER    NHRQTF
      PARAMETER( NHRQTF = 1350                               )
*
      INTEGER    NHRDTF       , NHDRFT       , NHDAFT
      PARAMETER( NHRDTF = 1400, NHDRFT = 1490, NHDAFT = 1491 )
      INTEGER    NHDSFT       , NHQFDR       , NHDWBF
      PARAMETER( NHDSFT = 1492, NHQFDR = 1500, NHDWBF = 1501 )
      INTEGER    NHDWAF       , NHERCU       , NHERFI
      PARAMETER( NHDWAF = 1502, NHERCU = 1520, NHERFI = 1521 )
      INTEGER    NHERD0       , NHERTD       , NHERZ0
      PARAMETER( NHERD0 = 1522, NHERTD = 1523, NHERZ0 = 1524 )
      INTEGER    NHERP6
      PARAMETER( NHERP6 = 1525                               )
      INTEGER NHDRBD, NTIMPT, NTIMVD
      PARAMETER (NHDRBD = 1493, NTIMPT = 1495, NTIMVD = 1496)
      INTEGER NTIMDR
      PARAMETER (NTIMDR = 1497)
      INTEGER NHDRT0, NHPTT0, NHVDT0
      PARAMETER (NHDRT0 = 1595, NHPTT0 = 1597, NHVDT0 = 1598)
      INTEGER NHCOPT, NHCOVD, NHCOR1
      PARAMETER (NHCOPT = 1591, NHCOVD = 1592, NHCOR1 = 1593)
      INTEGER NHCOR3
      PARAMETER (NHCOR3 = 1594)
*
      INTEGER    NHCTDS       , NHCTDP       , NHCTQS
      PARAMETER( NHCTDS = 1601, NHCTDP = 1602, NHCTQS = 1605 )
      INTEGER    NHCTQP       , NHCTQC       , NHCTNS
      PARAMETER( NHCTQP = 1606, NHCTQC = 1610, NHCTNS = 1611 )
      INTEGER    NHRCTF       , NHCNFT
      PARAMETER( NHRCTF = 1620, NHCNFT = 1630                )
*
      INTEGER    NHDCDS       , NHDCDP       , NHDCQS
      PARAMETER( NHDCDS = 1651, NHDCDP = 1652, NHDCQS = 1655 )
      INTEGER    NHDCQP       , NHDCQC       , NHDCNS
      PARAMETER( NHDCQP = 1656, NHDCQC = 1660, NHDCNS = 1661 )
      INTEGER    NHRDCF       , NHDCFT
      PARAMETER( NHRDCF = 1670, NHDCFT = 1680                )
*
      INTEGER    NHMCNI       , NHMCX2       , NHMCPR
      PARAMETER( NHMCNI = 1701, NHMCX2 = 1702, NHMCPR = 1703 )
      INTEGER    NHMCRS       , NHMCCU       , NHMCFI
      PARAMETER( NHMCRS = 1704, NHMCCU = 1705, NHMCFI = 1706 )
      INTEGER    NHMCD0       , NHMCTD       , NHMCZ0
      PARAMETER( NHMCD0 = 1707, NHMCTD = 1708, NHMCZ0 = 1709 )
      INTEGER    NHMCIP       , NHMCAP
      PARAMETER( NHMCIP = 1710, NHMCAP = 1711                )
*
      INTEGER    NHMC12       , NHMCP1       , NHMCP2
      PARAMETER( NHMC12 = 1721, NHMCP1 = 1722, NHMCP2 = 1723 )
*
      INTEGER    NHMCD1       , NHMCD2       , NHMCD3
      PARAMETER( NHMCD1 = 1731, NHMCD2 = 1732, NHMCD3 = 1733 )
*
      INTEGER    NHMCAK       , NHMCZE       , NHMCXM
      PARAMETER( NHMCAK = 1751, NHMCZE = 1752, NHMCXM = 1753 )
      INTEGER    NHMCCT       , NHMCPM       , NHMCAM
      PARAMETER( NHMCCT = 1754, NHMCPM = 1764, NHMCAM = 1765 )
      INTEGER    NHMCKM       , NHMCDM       , NHMCSM
      PARAMETER( NHMCKM = 1769, NHMCDM = 1770, NHMCSM = 1771 )
*
      INTEGER    NHMD0        , NHMDIF       , NHDST1
      PARAMETER( NHMD0  = 1801, NHMDIF = 1802, NHDST1 = 1803 )
      INTEGER    NHDST2       , NHDST3
      PARAMETER( NHDST2 = 1804, NHDST3 = 1805                )
