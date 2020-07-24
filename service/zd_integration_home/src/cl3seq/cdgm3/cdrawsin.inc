*
*CMZ :  1.01/05 23/05/89  17.51.49  by  Nobu Katayama
*CMZ :  2.00/00 17/05/89  12.41.46  by  Nobu Katayama
*-- Author :
C
C     Raw data commons (VD, DR, etc.) used in track fitting
C
* NHWCD  - Number of hit wires and strips in the central detector
* NHWDCD - Number of hit wires or  strips in each device
*          This is device definition ordering.
* NFRWCD - Number of anode or cathode hits on tape for each device.
*          This is device definition ordering.
* NLRWCD - Number of anode or cathodehits put in raw data list for each device
*          This is device definition ordering.
* NERWCD - Number of errors for raw data for each device
C
* IPTWCD - PZU pointer for Uncorrected drift time in psec ( ZU/NHWCD)
* IPQWCD - PZU pointer for Uncorrected charge             ( ZU/NHWCD)
* IPLYCD - PZU pointer for CD LaYer number                (IZU/NHWCD)
* IPWRCD - PZU pointer for WiRe number in layer (0 for cahtodes)
*                                                         (IZU/NHWCD)
* IPCPCD - PZU pointer for Cahtode Phi seg. number in layer (0 for anodes)
*                                                         (IZU/NHWCD)
* IPSTCD - PZU pointer for cathode STrip number in z in layer (0 for anodes)
*                                                         (IZU/NHWCD)
* IPPWCD - PZU pointer for Phi position of each hit       ( ZU/NHWCD)
* IPXWCD - PZU pointer for X coordinate of CD hit wires   ( ZU/NHWCD)
* IPYWCD - PZU pointer for Y coordinate of CD hit wires   ( ZU/NHWCD)
* IPZWCD - PZU pointer for Z coordinate of CD hit wires   ( ZU/NHWCD)
* IPDWCD - PZU pointer for Corrected drift distance in m  ( ZU/NHWCD)
* IPPPCD - PZU pointer for Corrected charge               ( ZU/NHWCD)
* IPNHCD - PZU pointer to the first hit of the layer.     (IZU/NHWCD)
*          In order to get the next hit, use IPNHCD. IHNEXT will be
*          IHNEXT = IZU(PZU(IPNHCD)+IH). IHNEXT will be zero in case there
*          is no more hits.
* IPADCD - PZU pointer for the raw ADC counts             (IZU/NHWCD)
* IPTDCD - PZU pointer for the raw ADC counts             (IZU/NHWCD)

* IPFHCD - PZU pointer to the first hit of the layer.     (IZU/NLYRCD)
*          IH = IZU(PZU(IPFHCD)+ILYRCD) is the hit number of the first
*          hit in a given layer. IH iz null if there is no hits in the layer.
* IPMPCD - PZU Pointer to raw data arrays                 (IZU/MWIRCD)
C

      INTEGER NHWCD,  NHWDCD, NFRWCD, NLRWCD, NERWCD
C IP
      INTEGER IPLSCD
      INTEGER IPLYCD, IPWRCD, IPCPCD, IPSTCD, IPTWCD
      INTEGER IPPWCD, IPXWCD, IPYWCD, IPZWCD, IPDWCD
      INTEGER IPPPCD, IPNHCD, IPADCD, IPTDCD, IPQWCD
      INTEGER IPMPCD, IPFHCD
C JO
C      INTEGER JOLSCD, JOTOTL
C      INTEGER JOLYCD, JOWRCD, JOCPCD, JOSTCD, JOTWCD
C      INTEGER JOPWCD, JOXWCD, JOYWCD, JOZWCD, JODWCD
C      INTEGER JOPPCD, JONHCD, JOADCD, JOTDCD, JOQWCD
      INTEGER CDRZLA
C
      COMMON /CDRAWZ/ NHWCD,  NHWDCD(MXDVCD), NFRWCD(MXDVCD),
     *NLRWCD(MXDVCD), NERWCD(MXDVCD),
     *        IPLSCD(MCDRAW),
C     *        JOLSCD(MCDRWC), JOTOTL,
     *        CDRZLA
      SAVE   /CDRAWZ/
      EQUIVALENCE(IPLSCD(JPLYCD),IPLYCD)
      EQUIVALENCE(IPLSCD(JPWRCD),IPWRCD)
      EQUIVALENCE(IPLSCD(JPCPCD),IPCPCD)
      EQUIVALENCE(IPLSCD(JPSTCD),IPSTCD)
      EQUIVALENCE(IPLSCD(JPTWCD),IPTWCD)
      EQUIVALENCE(IPLSCD(JPQWCD),IPQWCD)
      EQUIVALENCE(IPLSCD(JPPWCD),IPPWCD)
      EQUIVALENCE(IPLSCD(JPXWCD),IPXWCD)
      EQUIVALENCE(IPLSCD(JPYWCD),IPYWCD)
      EQUIVALENCE(IPLSCD(JPZWCD),IPZWCD)
      EQUIVALENCE(IPLSCD(JPDWCD),IPDWCD)
      EQUIVALENCE(IPLSCD(JPPPCD),IPPPCD)
      EQUIVALENCE(IPLSCD(JPNHCD),IPNHCD)
      EQUIVALENCE(IPLSCD(JPADCD),IPADCD)
      EQUIVALENCE(IPLSCD(JPTDCD),IPTDCD)

C      EQUIVALENCE(JOLSCD(JPLYCD),JOLYCD)
C      EQUIVALENCE(JOLSCD(JPWRCD),JOWRCD)
C      EQUIVALENCE(JOLSCD(JPCPCD),JOCPCD)
C      EQUIVALENCE(JOLSCD(JPSTCD),JOSTCD)
C      EQUIVALENCE(JOLSCD(JPTWCD),JOTWCD)
C      EQUIVALENCE(JOLSCD(JPQWCD),JOQWCD)
C      EQUIVALENCE(JOLSCD(JPPWCD),JOPWCD)
C      EQUIVALENCE(JOLSCD(JPXWCD),JOXWCD)
C      EQUIVALENCE(JOLSCD(JPYWCD),JOYWCD)
C      EQUIVALENCE(JOLSCD(JPZWCD),JOZWCD)
C      EQUIVALENCE(JOLSCD(JPDWCD),JODWCD)
C      EQUIVALENCE(JOLSCD(JPPPCD),JOPPCD)
C      EQUIVALENCE(JOLSCD(JPNHCD),JONHCD)
C      EQUIVALENCE(JOLSCD(JPADCD),JOADCD)
C      EQUIVALENCE(JOLSCD(JPTDCD),JOTDCD)

      EQUIVALENCE(IPLSCD(JPFHCD),IPFHCD)
      EQUIVALENCE(IPLSCD(JPMPCD),IPMPCD)
C
C The following variables are local. They are used for the call GTCDRW
C
      LOGICAL LPLSCD (MCDRWL)
      SAVE    LPLSCD
      LOGICAL LPLYCD, LPWRCD, LPCPCD, LPSTCD, LPTWCD, LPQWCD
      LOGICAL LPPWCD, LPMPCD, LPXWCD, LPYWCD, LPZWCD, LPDWCD
      LOGICAL LPPPCD, LPFHCD, LPNHCD, LPADCD, LPTDCD
      LOGICAL LPIVDT, LPVDET, LPDRFT, LPVDCT, LPDRCT
      LOGICAL LPPROP, LPPTLA, LPPTLC, LPSORT, LPVEST

      EQUIVALENCE(LPLSCD(JPLYCD),LPLYCD)
      EQUIVALENCE(LPLSCD(JPWRCD),LPWRCD)
      EQUIVALENCE(LPLSCD(JPCPCD),LPCPCD)
      EQUIVALENCE(LPLSCD(JPSTCD),LPSTCD)
      EQUIVALENCE(LPLSCD(JPTWCD),LPTWCD)
      EQUIVALENCE(LPLSCD(JPQWCD),LPQWCD)
      EQUIVALENCE(LPLSCD(JPPWCD),LPPWCD)
      EQUIVALENCE(LPLSCD(JPXWCD),LPXWCD)
      EQUIVALENCE(LPLSCD(JPYWCD),LPYWCD)
      EQUIVALENCE(LPLSCD(JPZWCD),LPZWCD)
      EQUIVALENCE(LPLSCD(JPDWCD),LPDWCD)
      EQUIVALENCE(LPLSCD(JPPPCD),LPPPCD)
      EQUIVALENCE(LPLSCD(JPNHCD),LPNHCD)
      EQUIVALENCE(LPLSCD(JPADCD),LPADCD)
      EQUIVALENCE(LPLSCD(JPTDCD),LPTDCD)

      EQUIVALENCE(LPLSCD(JPFHCD),LPFHCD)
      EQUIVALENCE(LPLSCD(JPMPCD),LPMPCD)

      EQUIVALENCE(LPLSCD(JPTDCD),LPTDCD)
      EQUIVALENCE(LPLSCD(JLIVDT),LPIVDT)
      EQUIVALENCE(LPLSCD(JLVDET),LPVDET)
      EQUIVALENCE(LPLSCD(JLDRFT),LPDRFT)
      EQUIVALENCE(LPLSCD(JLVDCT),LPVDCT)
      EQUIVALENCE(LPLSCD(JLDRCT),LPDRCT)
      EQUIVALENCE(LPLSCD(JLPROP),LPPROP)
      EQUIVALENCE(LPLSCD(JLPTLA),LPPTLA)
      EQUIVALENCE(LPLSCD(JLPTLC),LPPTLC)
      EQUIVALENCE(LPLSCD(JLSORT),LPSORT)
      EQUIVALENCE(LPLSCD(JLVEST),LPVEST)
C
C



