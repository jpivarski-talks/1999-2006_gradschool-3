*
* $Id: svlaseq.inc,v 1.6 1998/07/29 20:57:16 pg Exp $
*
* $Log: svlaseq.inc,v $
* Revision 1.6  1998/07/29 20:57:16  pg
*  All elements numbered from 1 to N. Changed array's declaration to
* reflect this.
*
* Revision 1.5  1998/05/07 20:37:41  pg
*   Modified include files to better represent Si3 geometry
*
* Revision 1.4  1998/01/07 20:49:30  pg
*   Added files needed for SVX hit storage. Rearranged and modified
* svgeompa.inc and svlaseq.inc to make hit-related things work.
*
* Revision 1.3  1996/09/30 05:48:11  jjo
* Modify svgeompa.inc to use longer variable names per CLEO3 standards
*        SVNSTR -> NSTRIP_SV3, SVLYRS -> NLAYER_SV3, SVMWFR -> NWAFER_SV3
*        NSVWP? -> NWAF_PHI?_SV3, NSVWZ? -> NWAF_Z?_SV3
*        SV_CHAN_ACTIVE  -> NCHAN_ACTIVE_SV3, SV_CHAN_TOTAL  -> NCHAN_TOTAL_SV3
*
* Revision 1.2  1996/04/23 16:31:33  jjo
* Change SVNCAN to SV_CHAN_TOTAL, SVMCAN to SV_CHAN_ACTIVE (jjo)
*
* Revision 1.1.1.1  1995/11/01 21:18:37  lkg
* Import renamed CLEO3 include repository: cl3seq
*
*
* Arrays for logical address
C SVLAWN - wafer number of a logical address (for a given ganged strip)
C SVLALY - Layer number of a logical address
C SVLAPZ - .TRUE. if PHI strip
C SVWNLA - The offset to the logical address of a wafer
C SVWFRS - number of wafers per layer
C SVWNLN - Returns Layer number (1-4) given input wafer number
C SVGANG - Returns # of ganging given input wafer number
C SVLANG - Returns # of ganging given logical address
C SVWRPZ - wafer number --> layer number(1), IZ(2), IPHI(3)
C SVRPZW - layer number, IZ, IPHI --> wafer number
C SVLANZ - number of wafers ganged to this channel
C SVLACX - chip number corresponding to this channel

C SVLXLA - Local t0 Logical address map

C WAFER2LADDER - Wafer # to Ladder #  
C WAFER2UNIT   - Wafer # to UNIT #  

c WAFER2HYBRID - Wafer # to Hybrid #
c HYBRID2WAFER - Hybrid # to Wafer #
      INTEGER SVLAWN, SVLALY, SVWNLA
      INTEGER SVWFRS
      INTEGER SVWNWT, SVWTNN
      INTEGER SVWNLN, SVGANG, SVLANG
      INTEGER SVLACX

      INTEGER WAFER2LADDER,WAFER2UNIT,WAFER2HYBRID,HYBRID2WAFER

      LOGICAL SVLAPZ

      COMMON /SVLA00/ SVLAWN(SVMXGN,NCHAN_TOTAL_SV3),
     $                SVLALY(NCHAN_TOTAL_SV3),
     $                SVLAPZ(NCHAN_TOTAL_SV3),SVLACX(NCHAN_TOTAL_SV3)

      COMMON /SVLA01/ SVWNLA(0:1,NWAFER_SV3),
     *                SVWNLN(NWAFER_SV3),
     *                WAFER2LADDER(NWAFER_SV3),
     *                WAFER2UNIT(NWAFER_SV3),
     *                WAFER2HYBRID(NWAFER_SV3),
     *                HYBRID2WAFER(NHYBRID_SV3)

      COMMON / SVLA02 / SVGANG(NWAFER_SV3), SVLANG(NCHAN_TOTAL_SV3)


      COMMON /SVLA03/ SVWFRS(NLAYER_SV3)

      INTEGER SVWRPZ(3,NWAFER_SV3), SVRPZW(NLAYER_SV3,SVMXWP,SVMXWZ)
      COMMON / SVLA04 / SVWRPZ, SVRPZW

      INTEGER SVLXLA(0:1,NCHAN_SV3,NWAFER_SV3),
     &        SVLANZ(NCHAN_TOTAL_SV3)
      COMMON / SVLA05 / SVLXLA, SVLANZ

      SAVE /SVLA00/, /SVLA01/, /SVLA02/, /SVLA03/, /SVLA04/,
     *     /SVLA05/

