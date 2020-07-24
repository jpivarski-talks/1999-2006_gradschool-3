*
* $Id: svrhc100.inc,v 1.3 2000/05/09 15:30:45 pg Exp $
*
* $Log: svrhc100.inc,v $
* Revision 1.3  2000/05/09 15:30:45  pg
* Bumped up number of raw hits to 125000
*
* Revision 1.2  1998/07/29 20:57:17  pg
*  All elements numbered from 1 to N. Changed array's declaration to
* reflect this.
*
* Revision 1.1  1998/01/07 20:49:31  pg
*   Added files needed for SVX hit storage. Rearranged and modified
* svgeompa.inc and svlaseq.inc to make hit-related things work.
*
* Revision 1.5  1997/02/14 03:37:27  clib
* Increase MXSVST from 17 to 18.  Add RBitSV to SVSTAT common.
*
* Revision 1.4  1996/12/23 23:50:10  clib
* Add sequencer status and L3 trigger bit commons.
*
* Revision 1.3  1995/08/08 20:00:19  sidev
* Add IFLRSV to svrhc100.inc for status bit for SVRH.
*
* Revision 1.2  1995/03/14  15:19:44  zfiles
* Added common mode noise information.
*
* Revision 1.1.1.1  1994/10/04  20:59:00  zfiles
* first version of seq in CVS
*
*
*
* svrhc100.inc
*
C
C
C
      INTEGER MXSVRH
      PARAMETER ( MXSVRH = 125000 )
C NSVRH  - Number of hits in the SVD detector
C ISTRSV - Strip logical address
C IFLRSV - Strip status (bad channel, picked up by sparsifier or Level3).
C ESTRSV - Energy deposit
      INTEGER NSVRH
      INTEGER ISTRSV, ISTATSV, IFLRSV
      REAL ESTRSV, PEDSV, NOISESV, GAINSV

      COMMON /SVRH00/ NSVRH, ISTRSV ( MXSVRH ), ESTRSV ( MXSVRH ), 
     +                PEDSV(MXSVRH), NOISESV(MXSVRH), GAINSV(MXSVRH)
      SAVE /SVRH00/


C Layer number, Phi(Z) number, Phi/Z flag are derived from ISTRSV
      INTEGER ILYRSV, IPHZSV, LPHZSV
      COMMON /SVRH01/ IFLRSV(MXSVRH), ISTATSV(MXSVRH), 
     +                LPHZSV(MXSVRH)
      SAVE   /SVRH01/

C
C CMNRSV - Common mode noise (CMN) of logical CAMEX number
C SLPRSV - CMN SLOPE of logical CAMEX number
C Filled from SVRA bank. Not kept in ROAR field.

C      REAL CMNRSV, SLPRSV
C      COMMON /SVRHAU/ CMNRSV(0:SVMCMX-1),SLPRSV(0:SVMCMX-1)
C      SAVE   /SVRHAU/

C SVX Sequencer Status Word and Level3 Trigger Bit information:
C This information IS stored in ROAR (Added 12/19/96 - M. Palmer)
C SVSQAD - Address of SVX Sequencer Status Word
C SVL3TA - First address of SVX L3 Trigger Bit Words
C nSVSQT - Number of entries for Level3 Trigger Bit words (16) plus 
C          SVX Sequencer Status Word (1). 
C SVRHFA - Address of raw hit pulse height correction status word
C IADRSV - Address for each word
C SWDRSV - Value of each word (Real)
      Integer    SVSQAD
      Parameter (SVSQAD = 30000)
      Integer    SVL3TA
      Parameter (SVL3TA = 31000)
      Integer    SVRHFA
      Parameter (SVRHFA = 30001)
      Integer    MXSVST
      Parameter (MXSVST = 18)

      Integer nSVST
      Integer IADRSV
      Real    SWDRSV

      COMMON /SVRH02/ nSVST, IADRSV(MXSVST), SWDRSV(MXSVST)
      SAVE   /SVRH02/

C Decoded SVX Sequencer Status Word Information - derived at 
C unpack time (not stored in ROAR).    (Added 12/19/96 - M. Palmer)
C XPhRSV  phase of X-gen counter
C MXPRSV  phase of sid-mux counter
C S4RSV   true if S-pulse 4 has occurred since last reset
C R1PRSV  R1 pending
C R1ARSV  R1 active
C NSKRSV  True if S-pulses NOT paused for event
C DTARSV  True if R1 deadtime being asserted

      Integer XPhRSV, MXPRSV, S4RSV, R1PRSV, R1ARSV, NSKRSV, DTARSV 

C SVRH pulseheight correction word (Added 02/13/97 - M. Palmer)
C   Bit               Meaning
C    0                Pulse height corrections from sv.zfx applied
C    1                Hardwired pulse height corrections applied
C    2-31             Currently undefined
      Integer RBitSV


      COMMON /SVSTAT/ XPhRSV, MXPRSV, S4RSV, R1PRSV, R1ARSV, NSKRSV, 
     +                DTARSV,
     +                RBitSV
      SAVE   /SVSTAT/

