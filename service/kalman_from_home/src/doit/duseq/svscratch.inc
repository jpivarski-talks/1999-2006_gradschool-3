*
* $Id: svscratch.inc,v 1.3 2000/05/26 16:54:10 dpp Exp $
*
* $Log: svscratch.inc,v $
* Revision 1.3  2000/05/26 16:54:10  dpp
*      -> new array to flag available clusters
*
* Revision 1.2  2000/05/15 18:29:01  pg
*   Modified  common block holding Si hits for doit: one array for R and
* Z hits.
*
* Revision 1.1  2000/03/28 15:55:27  pg
* Added svscratch.inc and svgeometry.inc to store Si hits and geometry information for doit
*
* SV Scratch common sequence for Doit
#if !defined(CLEO_NOCOMDOC)
* SVSCRATCH Documentation
*
* SVSCRATCH.inc
*
C     20-Mar-00: First version of Si common block for Doit
C
C MHITSV          -  Max. no. of Si hits ( = 2000)
* SV_NHITS    - Number of Si R-Phi hits
* SV_INDEX    - index of the hit in FATable on C++ side
* SV_PARENT   - parent # for the hit - same for Z-hits coming from 
*               the same cluster
* SV_TYPE     - type of the hit: 0 for R-Phi, 1 for Z . DPP prefers to have
*               one master list of hits.
* SV_LAYER    - Layer # (1-4)
* SV_HYBRID   - Hybrid # (1-122)
* SV_QUALITY  - Hit quality
* SV_X        - X-coordinate in the world system
* SV_Y        - Y-coordinate in the world system
* SV_Z        - Z-coordinate in the world system - center of the 
*                   halfladder is assumed.
* SV_PHI      - Phi of the hit
* SV_PULSE    - Pulseheight (thousands of electrons)
* SV_AVAIL    - =1 if hit is available to put on a track, otherwise=-1 
#endif

      INTEGER MHITSV
      PARAMETER( MHITSV = 2000 )

      INTEGER SV_NHITS, SV_INDEX, SV_PARENT, SV_TYPE 
      INTEGER SV_LAYER, SV_HYBRID, SV_QUALITY, SV_AVAIL

      REAL    SV_X, SV_Y, SV_Z, SV_PHI, SV_PULSE

      COMMON /SV_SCRATCH/
     +       SV_NHITS, SV_INDEX(MHITSV),SV_PARENT(MHITSV),
     +       SV_TYPE(MHITSV), SV_LAYER(MHITSV),
     +       SV_HYBRID(MHITSV),  SV_QUALITY(MHITSV),
     +       SV_X(MHITSV ), SV_Y (MHITSV), SV_Z(MHITSV), SV_PHI(MHITSV),
     +       SV_PULSE (MHITSV)
      COMMON/SV_SCRATCH_MORE/
     +       SV_AVAIL(MHITSV)



