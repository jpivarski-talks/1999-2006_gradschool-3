*
* $Id: cdscrtcd.inc,v 1.3 2000/03/03 15:08:56 lyon Exp $
*
* $Log: cdscrtcd.inc,v $
* Revision 1.3  2000/03/03 15:08:56  lyon
* Added new variables - QUALCD, NTDCCD, EARLYCD, LATECD
*
* Revision 1.2  1999/11/29 21:43:39  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:24  clib
* Developmental version of DUET.
*
* Revision 1.5  1996/05/21 15:49:29  clib
* Expand arrays so that they can hold 2000 vd/dr hits plus all SVAH hits.
* Include svahc100.inc
* Add IAH2SC.
*
* Revision 1.4  1996/04/23 21:43:46  zfiles
* Comments for previous version are wrong!
* True comment:
*      Updated documentation.
*
* Revision 1.3  1996/04/23 07:28:53  zfiles
* Add new stack variables for kalman output.
*
* Revision 1.2  1996/02/29 17:57:01  zfiles
* added DVHTCD to allow silicon hit info into output
* incorporated documentation
*
* Revision 1.1.1.1  1994/10/08 01:01:06  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/28 06/06/94  13.44.59  by  Anders Ryd
*CMZ :  6.00/26 17/05/94  14.30.41  by  Anders Ryd
*CMZ :  6.00/24 09/05/94  01.04.03  by  Anders Ryd
*CMZ :  5.54/36 14/05/93  15.25.37  by  Dan Peterson
*CMZ :  5.53/06 01/08/90  15.51.38  by  Steve Schaffner
*CMZ :  5.52/00 24/01/90  14.43.22  by  Steve Schaffner
*-- Author :
* CD Scratch common sequence.
#if !defined(CLEO_NOCOMDOC)
* CDSCRTCD Documentation
*
* cdscrtdo.inc
*
C     22-Apr-89: Variables formerly in CDRAWxxx moved here to accomodate
C       reading out of ZEBRA bank; NHWCD => NHTWCD changed for same reason.
C       xxxTMP arrays removed.   sfs
C
C     MWHCD  --  Max. no. raw hits to store

* XWCD   - X coordinate of CD hit wires
* YWCD   - Y coordinate of CD hit wires
* ZWCD   - Z coordinate of CD hit wires
* DWCD   - Corrected drift distance in m
* PWCD   - Corrected charge
* INXHCD - Pointer to next hit in cathode cluster
*
* NHTWCD - Number of hit wires in the central detector
* TDCWCD - Raw TDC
* TDWCD  - Wire chambers: Uncorrected drift time in psec
*          SVD hits:      IQLASV
* TCWCD  - Wire chambers: Corrected drift time in psec
*          SVD hits:      NHCASV
* QWCD   - Uncorrected charge
* ICYLCD - CD layer number
* IWIRCD - Wire number in layer
* PHIWCD - Phi position of each hit
* MAPCD  - Pointer to raw data arrays
* DVHTCD - Device local hit number (e.g. the SVAH number for silicon)
*          For SVX Z-side hits, the interpretation is a little different:
*          DVHTCD mod 10000 is the index in SVAH, DVHTCD/10000 is the 
*          ganging
* QUALCD - Quality word for this hit (from CDOffCal)
* NTDCCD - Number of TDCs for this hit
* EARLYCD - Gap (in ps) between this hit and the next earlier TDC
* LATECD  - Gap (in ps) between this hit and the next later TDC
* 
* IAH2SC - pointer from svahc100.inc to doit/duseq/cdscrtcd.inc
*          ( the inverse of DVHTCD )
* ----------------------------------------------------------
#endif

C Must include this to get MXSVAH.
#include "cl3seq/sv3seq/svahc100.inc"

      INTEGER MHWCD1
      PARAMETER( MHWCD1 = 2000 )
      INTEGER MHWCD
      PARAMETER( MHWCD  = MHWCD1 + 3*MXSVAH )

      INTEGER INXHCD,
     +        NHTWCD,  ICYLCD       , IWIRCD,     TDCWCD,
     +        MAPCD,   DVHTCD       , IAH2SC,     QUALCD,
     +        NTDCCD

      REAL    XWCD, YWCD, ZWCD, DWCD, PWCD,
     +       TDWCD, TCWCD ,QWCD, PHIWCD, EARLYCD, LATECD

      COMMON /SCRTCH/
     +       NHTWCD,
     +       TDCWCD(MHWCD),TDWCD(MHWCD),  TCWCD(MHWCD),
     +       QWCD(MHWCD),  ICYLCD(MHWCD), IWIRCD(MHWCD),
     +       PHIWCD(MHWCD ), MAPCD (KWS1CD),
     +       XWCD  (MHWCD),  YWCD(MHWCD),    ZWCD(MHWCD),
     +                DWCD  (MHWCD), PWCD(MHWCD),
     +                INXHCD(MHWCD), DVHTCD(MHWCD),
     +       QUALCD(MHWCD), NTDCCD(MHWCD), EARLYCD(MHWCD),
     &       LATECD(MHWCD),
     +       IAH2SC(0:2,MXSVAH)

      INTEGER      IADRHW(KWS1CD)
      EQUIVALENCE( IADRHW, MAPCD )

* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"



