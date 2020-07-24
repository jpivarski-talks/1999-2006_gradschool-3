*
* $Id: svgeompa.inc,v 1.6 2000/01/13 15:49:53 pg Exp $
*
* $Log: svgeompa.inc,v $
* Revision 1.6  2000/01/13 15:49:53  pg
* Added new file svgang.inc to store Si daisy-chain configuration
*
* Revision 1.5  1998/05/07 20:37:40  pg
*   Modified include files to better represent Si3 geometry
*
* Revision 1.4  1998/01/07 20:49:28  pg
*   Added files needed for SVX hit storage. Rearranged and modified
* svgeompa.inc and svlaseq.inc to make hit-related things work.
*
* Revision 1.3  1996/09/30 05:48:09  jjo
* Modify svgeompa.inc to use longer variable names per CLEO3 standards
*        SVNSTR -> NSTRIP_SV3, SVLYRS -> NLAYER_SV3, SVMWFR -> NWAFER_SV3
*        NSVWP? -> NWAF_PHI?_SV3, NSVWZ? -> NWAF_Z?_SV3
*        SV_CHAN_ACTIVE  -> NCHAN_ACTIVE_SV3, SV_CHAN_TOTAL  -> NCHAN_TOTAL_SV3
*
* Revision 1.2  1996/04/23 16:31:29  jjo
* Change SVNCAN to SV_CHAN_TOTAL, SVMCAN to SV_CHAN_ACTIVE
*
* Revision 1.1.1.1  1995/11/01 21:18:36  lkg
* Import renamed CLEO3 include repository: cl3seq
*
* Revision 1.1.1.1  1994/10/04  20:59:04  zfiles
* first version of seq in CVS
*
*Parameters for svd3
C SVNOFF is the number of offset constants. 3 positions, 3 directions
C        2 spares
C
      INTEGER    SVNOFF
      PARAMETER ( SVNOFF = 8 )

C
C Number of wafers in phi for each layer
      INTEGER NWAF_PHI1_SV3, NWAF_PHI2_SV3, NWAF_PHI3_SV3, NWAF_PHI4_SV3
      PARAMETER ( NWAF_PHI1_SV3 = 7  )
      PARAMETER ( NWAF_PHI2_SV3 = 10 )
      PARAMETER ( NWAF_PHI3_SV3 = 18 )
      PARAMETER ( NWAF_PHI4_SV3 = 26 )

C Total number of wafers in phi( ==LADDERS ) = 61.
      INTEGER NWAF_PHI_SV3
      PARAMETER (NWAF_PHI_SV3 = NWAF_PHI1_SV3 + NWAF_PHI2_SV3 +
     +                          NWAF_PHI3_SV3 + NWAF_PHI4_SV3)
      
c Total number of ladders
      INTEGER NLADDER_SV3
      PARAMETER (NLADDER_SV3 = NWAF_PHI_SV3)

c Total number of hybrids
      INTEGER NHYBRID_SV3
      PARAMETER (NHYBRID_SV3 = 2*NLADDER_SV3)

C Number of wafers in z for each layer
      INTEGER NWAF_Z1_SV3, NWAF_Z2_SV3, NWAF_Z3_SV3, NWAF_Z4_SV3
      PARAMETER ( NWAF_Z1_SV3 =  3 )
      PARAMETER ( NWAF_Z2_SV3 =  4 )
      PARAMETER ( NWAF_Z3_SV3 =  7 )
      PARAMETER ( NWAF_Z4_SV3 = 10 )

C Total number of wafers = 447
      INTEGER NWAFER_SV3,SVMWFR
      PARAMETER ( NWAFER_SV3 = NWAF_PHI1_SV3*NWAF_Z1_SV3 +
     &                         NWAF_PHI2_SV3*NWAF_Z2_SV3 +
     &                         NWAF_PHI3_SV3*NWAF_Z3_SV3 +
     &                         NWAF_PHI4_SV3*NWAF_Z4_SV3 )
      PARAMETER (SVMWFR = NWAFER_SV3)
C Number of strips on each detector
      INTEGER NSTRIP_SV3
      PARAMETER ( NSTRIP_SV3 = 511 )

C Number of channels on each detector
      INTEGER NCHAN_SV3
      PARAMETER ( NCHAN_SV3 = NSTRIP_SV3+1 )

C Number of active channel per CAMEX
      INTEGER SVCCMX
      PARAMETER ( SVCCMX = 128 )

C Total number of channels per CAMEX
      INTEGER NCHAN_CHIP,SVTCMX
      PARAMETER ( NCHAN_CHIP = 128 )
      PARAMETER ( SVTCMX = NCHAN_CHIP )


c Number of SVX Databoards = 32
      INTEGER NBOARD_SV3,SVMXDB
      PARAMETER (NBOARD_SV3 = 32)
      PARAMETER (SVMXDB = NBOARD_SV3)


c Number of SVX chips in the system = 976
      INTEGER NCHIP_SV3,SVMCMX
      PARAMETER ( NCHIP_SV3 = NWAF_PHI_SV3*2*2*4 )
      PARAMETER ( SVMCMX = NCHIP_SV3 )

c Number of SVX chips per databoard (maximal 32 = 8 hybrids = 4 ends)
C Some will have less....
      INTEGER NCHIP_BOARD,SVMCXB 
      PARAMETER (NCHIP_BOARD = 32)
      PARAMETER (SVMCXB = NCHIP_BOARD)

c Number of channels per databoard (maximal)
      INTEGER NCHAN_BOARD,SVTCHB
      PARAMETER (NCHAN_BOARD = NCHIP_BOARD * NCHAN_CHIP)
      PARAMETER (SVTCHB = NCHAN_BOARD)
C
C Number of Layers
      INTEGER NLAYER_SV3
      PARAMETER ( NLAYER_SV3 = 4 )

C
C Maximum number of wafers in phi
      INTEGER SVMXWP
      PARAMETER ( SVMXWP = NWAF_PHI4_SV3 )

C
C Maximum number of wafers in Z
      INTEGER SVMXWZ
      PARAMETER ( SVMXWZ = NWAF_Z4_SV3 )

C Number of active channels (STRIPS) = 124684
      INTEGER NCHAN_ACTIVE_SV3 
      PARAMETER ( NCHAN_ACTIVE_SV3 =
     &     (NWAF_PHI1_SV3+NWAF_PHI2_SV3+NWAF_PHI3_SV3+NWAF_PHI4_SV3)
     &     * NSTRIP_SV3 * 2 * 2)

C Total number of channels + common mode noise channels
C NSTRIP_SV3+1 (=512) readout channels per wafer (128*4)  = 
      INTEGER NCHAN_TOTAL_SV3,SVTCAN
      PARAMETER ( NCHAN_TOTAL_SV3 =
     &     (NWAF_PHI1_SV3+NWAF_PHI2_SV3+NWAF_PHI3_SV3+NWAF_PHI4_SV3) 
     &     * NCHAN_SV3 * 2 * 2 )
      PARAMETER (SVTCAN = NCHAN_TOTAL_SV3)

C Offset to the first logical address of Z strip.
      INTEGER SVLAZO
      PARAMETER ( SVLAZO =
     &     (NWAF_PHI1_SV3+NWAF_PHI2_SV3+NWAF_PHI3_SV3+NWAF_PHI4_SV3)
     &     * NCHAN_SV3 *2 )

C Maximum number of strips ganged together
      INTEGER SVMXGN
      PARAMETER ( SVMXGN = NWAF_Z4_SV3 / 2 )

C
C
C conversion factor from meter to cm.
C
      DOUBLE PRECISION MTOCM
      PARAMETER (MTOCM=100.0)
C
C conversion factor from degrees to radians
      DOUBLE PRECISION DTOR
      PARAMETER ( DTOR = 1.74532925199D-2 )




