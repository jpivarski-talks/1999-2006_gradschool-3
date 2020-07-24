*
* $Id: cdscrtcd.inc,v 1.5 2003/03/05 17:33:42 dpp Exp $
*
* $Log: cdscrtcd.inc,v $
* Revision 1.5  2003/03/05 17:33:42  dpp
*      -> MHTWCD specifically for TPC, with compile switch
*      -> more arrays for clustering
*      -> more arrays for multihit electronics
*      -> added flag for indicating a usable hit which is to be used
*          instead of setting the map value to negative
* 	 it would be insufficient, with multihit readout,
*          to flag used hits by readout channel
*           in each above case, these are in a different common block so
*           it does not interfere with other routines using CDSCRTCD
*      -> merge in arrays formerly in SFEXTRA but
*           in a different common block so
*           it does not interfere with other routines using CDSCRTCD
*      -> add scaler flag to indicate that the primary hit of
*         multi-hit electronics is to be used instaed of the hit located from
*         1stlyr, nxtlyr or mapdet
*
* Revision 1.4  2001/11/19 23:34:52  dpp
*      ->clean up
*
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

* CD Scratch common sequence.

#if !defined(CLEO_NOCOMDOC)
* CDSCRTCD Documentation
*
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
* PWCD   - Corrected charge, or calculated phi of cluster
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


* EXFHCD(KLYRCD)   - First hit number in the layer (indexed by layer)
*                    KLYRCD = max number of layers in the cd (see cdgeompa.inc)
*                    Equivalent to IZU(PZU(IPFHCD)+layerNum) [see sfdata.F]
*
* EXNHCD(MHWCD)    - Gives next hit number. If last hit in layer, then
*                    next hit is zero.
*                    MHWCD = Maximum number of hits allowed [see cdscrtcd.inc]
*                    Equivalent to IZU(PZU(IPNHCD)+hitNum)   [see sfdata.F]
*
* EXRADC(MHWCD)    - Raw ADC value
*                    Equivalent to IZU(PZU(IPADCD)+hitNum)   [see sfdata.F]

* NEW VARIABLE NAMES with more understandable and/or objectionable names
* EvntHitMax       - maximum number of hits in the event hit list
* EvntHit_Num      - current number of hits in the event hit list
* EvntHit_OpnHit   - >/= 0, hit is usable;  <0 not usable 
* EvntHit_rawTDC   - raw time in TDC counts 
* EvntHit_rawTIM   - ucorrected time, typically in ps
* EvntHit_corTIM   - corrected time, typically in ps
* EvntHit_Charge   - uncorrected charge in happy units
* EvntHit_rawADC   - uncorrected charge in raw ADC counts
* EvntHit_CDlayr   - CD layer number
* EvntHit_laWire   - wire number in layer 
* EvntHit_PhiWir   - phi of the wire location
* EvntHit_corPhi   - corrected phi of the hit
* EvntHit_MapDet   - map from detector element to event hit element
* EvntHit_X        - x coordinate
* EvntHit_Y        - y coordinate
* EvntHit_Z        - z coordinate
* EvntHit_Drift    - corrected drift distance in meters
* EvntHit_NxtCls   - next hit in EvntHit list in cluster 
* EvntHit_PrimCls  - primary hit of cluster,in EvntHit list,for clustered hit
* EvntHit_1stMlt   - 1st hit on a multi-hit cell, pointed to by EvntHit_MapDet 
* EvntHit_NxtMlt   - next hit in EvntHit list on the same Multi-hit cell 
* EvntHit_PrimMlt  - the hit on a multi-hit cell pointed to by EvntHit_MapDet 
* EvntHit_UsePrim  - logical, overide the located hit with the prime hit
* EvntHit_NHitDev  - local hit number in device
* EvntHit_Qual     - quality word from cdoffcal
* EvntHit_nMult    - number of (TDC) hits on the same channel,  
* EvntHit_TimEarly - Gap (in ps) between this hit and the next earlier TDC
* EvntHit_TimLate  - Gap (in ps) between this hit and the next later TDC
* EvntHit_1stLyr   - first hit in EvntHit list for a layer
* EvntHit_NxtLyr   - next hit in EvntHit list in the same layer for a hit
* EvntHit_1stDev   - location in event hit list of first instance of 
*                    a indexed device (device definition ordering) 
*
* note on multi-hit electronics,the way this will work is  

*   EvntHit_MapDet  points from the cell-id to the first hit on the cell
*     EvntHit_1stMlt   points from any hit to the first hit on the cell 
*     EvntHit_NxtMlt   points to other hits on the same cell
*     EvntHit_PrimMlt  points from the 1st hit to the hit that should be used 
* only 1stMlt is guaranteed to point to PrimMlt; it would require
*  an extra loop to have all elements on a multi-hit cell point to PrimMlt

* another way to sift through the data is using
*   EvntHit_1stLyr, which give the first seq hit in the list for a layer
*       (and this first hit must also be a first recognized hit on the cell)
*   EvntHit_NxtLyr gives the next hit on the layer on a different cell,
*     EvntHit_NxtMlt is still used to point to other hits on the same cell
*  and EvntHit_PrimMlt is still used to point from the fisrt hit to the one 
*    that should be used
* multihit electronics should only be tested on bonafide multi-hit layers



#endif
* ----------------------------------------------------------

C Must include this to get MXSVAH.
#include "cl3seq/sv3seq/svahc100.inc"

      INTEGER MHWCD1
      PARAMETER( MHWCD1 = 2000 )
      INTEGER MHWCD

#if defined(CLEO_DOITTPC)
      PARAMETER( MHWCD=200000 )
#else
      PARAMETER( MHWCD  = MHWCD1 + 3*MXSVAH )
#endif

      INTEGER INXHCD,
     +        NHTWCD,  ICYLCD       , IWIRCD,     TDCWCD,
     +        MAPCD,   DVHTCD       , IAH2SC,     QUALCD,
     +        NTDCCD

      INTEGER EXFHCD, EXNHCD, EXRADC

      INTEGER EvntHit_OpnHit,EvntHit_PrimCls
      INTEGER EvntHit_1stMlt,EvntHit_NxtMlt,EvntHit_PrimMlt
      INTEGER EvntHit_1stDev

      LOGICAL EvntHit_UsePrim

      REAL    XWCD, YWCD, ZWCD, DWCD, PWCD,
     +       TDWCD, TCWCD ,QWCD, PHIWCD, EARLYCD, LATECD

      COMMON /SCRTCH/
     +       NHTWCD,
     +       TDCWCD(MHWCD),TDWCD(MHWCD),  TCWCD(MHWCD),
     +       QWCD(MHWCD),  ICYLCD(MHWCD), IWIRCD(MHWCD),
     +       PHIWCD(MHWCD ), MAPCD (KWS1CD),
     +       XWCD  (MHWCD),  YWCD(MHWCD),    ZWCD(MHWCD),
     +       DWCD  (MHWCD), PWCD(MHWCD),
     +       INXHCD(MHWCD), DVHTCD(MHWCD),
     +       QUALCD(MHWCD),
     +       NTDCCD(MHWCD), EARLYCD(MHWCD),LATECD(MHWCD),
     +       IAH2SC(0:2,MXSVAH)

      COMMON/SFXTRAALL/
     +     EXFHCD(KLYRCD), EXNHCD(MHWCD), EXRADC(MHWCD)

      COMMON/SCRTCH3/
     +     EvntHit_OpnHit(MHWCD),EvntHit_PrimCls(MHWCD),
     +     EvntHit_1stMlt(MHWCD),EvntHit_NxtMlt(MHWCD),
     +     EvntHit_PrimMlt(MHWCD)

      COMMON/SCRTCHL/
     +     EvntHit_UsePrim

      COMMON/SCRTCH4/
     +     EvntHit_1stDev(MXDVCD)

      INTEGER EvntHitMax
      PARAMETER (EvntHitMax=MHWCD)

      INTEGER EvntHit_Num
      EQUIVALENCE (EvntHit_Num  ,NHTWCD)

      INTEGER EvntHit_rawTDC  (EvntHitMax)
      EQUIVALENCE (EvntHit_rawTDC  ,TDCWCD )

      REAL    EvntHit_rawTIM  (EvntHitMax)
      EQUIVALENCE (EvntHit_rawTIM  ,TDWCD )

      REAL    EvntHit_corTIM  (EvntHitMax)
      EQUIVALENCE (EvntHit_corTIM  ,TCWCD )

      REAL    EvntHit_Charge  (EvntHitMax)
      EQUIVALENCE (EvntHit_Charge  ,QWCD )

      INTEGER EvntHit_rawADC  (EvntHitMax)
      EQUIVALENCE (EvntHit_rawADC  ,EXRADC )

      INTEGER EvntHit_CDlayr  (EvntHitMax)
      EQUIVALENCE (EvntHit_CDlayr  ,ICYLCD )

      INTEGER EvntHit_laWire  (EvntHitMax)
      EQUIVALENCE (EvntHit_laWire  ,IWIRCD )

      REAL    EvntHit_PhiWir  (EvntHitMax)
      EQUIVALENCE (EvntHit_PhiWir  ,PHIWCD )

      REAL    EvntHit_corPhi  (EvntHitMax)
      EQUIVALENCE (EvntHit_corPhi  ,PWCD )

      REAL    EvntHit_X       (EvntHitMax)
      EQUIVALENCE (EvntHit_X  ,     XWCD)

      REAL    EvntHit_Y       (EvntHitMax)
      EQUIVALENCE (EvntHit_Y  ,     YWCD)

      REAL    EvntHit_Z       (EvntHitMax)
      EQUIVALENCE (EvntHit_Z  ,     ZWCD)

      REAL    EvntHit_Drift   (EvntHitMax)
      EQUIVALENCE (EvntHit_Drift  , DWCD)

      INTEGER EvntHit_NxtCls  (EvntHitMax)
      EQUIVALENCE (EvntHit_NxtCls  ,INXHCD )

      INTEGER EvntHit_NHitDev (EvntHitMax)
      EQUIVALENCE (EvntHit_NHitDev ,DVHTCD )

      INTEGER EvntHit_Qual    (EvntHitMax)
      EQUIVALENCE (EvntHit_Qual  ,  QUALCD )

      INTEGER EvntHit_nMult   (EvntHitMax)
      EQUIVALENCE (EvntHit_nMult  , NTDCCD )

      REAL    EvntHit_TimEarly(EvntHitMax)
      EQUIVALENCE (EvntHit_TimEarly,EARLYCD )

      REAL    EvntHit_TimLate (EvntHitMax)
      EQUIVALENCE (EvntHit_TimLate ,LATECD )

      INTEGER EvntHit_1stLyr  (KLYRCD)
      EQUIVALENCE (EvntHit_1stLyr  ,EXFHCD )

      INTEGER EvntHit_NxtLyr  (EvntHitMax)
      EQUIVALENCE (EvntHit_NxtLyr  ,EXNHCD )


C note: Solo does not use iadrhw
C note: Doit does not use iadrhw
C note: however, doit  DID   use iadrhw
c      INTEGER      IADRHW(KWS1CD)
c      EQUIVALENCE( IADRHW, MAPCD )

      INTEGER EvntHit_MapDet  (KWS1CD)
      EQUIVALENCE (EvntHit_MapDet  ,MAPCD )




















