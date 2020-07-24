*
* $Id: tfhistpa.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tfhistpa.inc,v $
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
#include "doit/duseq/tfhistpd.inc"
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
C
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
