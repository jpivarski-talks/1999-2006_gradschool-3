*
* $Id: svgeometry.inc,v 1.4 2001/03/30 00:03:27 dpp Exp $
*
* $Log: svgeometry.inc,v $
* Revision 1.4  2001/03/30 00:03:27  dpp
*      -> efficiency of hybrids
*      -> layer number for the hybrids
*
* Revision 1.3  2000/04/26 14:26:46  dpp
*      -> add variables to flag usable wafers, pointers to other geometries
*
* Revision 1.2  2000/04/03 15:01:19  pg
* Updated svgeometry.inc with new information
*
* Revision 1.1  2000/03/28 15:55:26  pg
* Added svscratch.inc and svgeometry.inc to store Si hits and geometry information for doit
*
*
C SVBOUNDARY_X(hybrid,2)..... X coordinate of boundary of wafer
C                             Edge = 1 gives the channel = 0 side,
C                             Edge = 2 gives the channel = NSTRIPS - 1
C SVBOUNDARY_Y(hybrid,2)..... Y coordinate of boundary of wafer
C SVBOUNDARY_PHI(hybrid,2)... phi coordinate of boundary of wafer
C SVBOUNDARY_R(hybrid,2)..... R coordinate of boundary of wafer
C SVBOUNDARY_Z(hybrid,2)..... Z coordinate of boundary of wafer
C SVDXDZ(hybrid)............. DX/DZ derivative if the location of a wafer
C SVDYDZ(hybrid)............. DY/DZ derivative if the location of a wafer
 	REAL SVBOUNDARY_X,SVBOUNDARY_Y,SVBOUNDARY_PHI,SVBOUNDARY_R
	REAL SVBOUNDARY_Z,SVDXDZ,SVDYDZ

C NUM_SV_HYBRIDS............. number of hybrids in the entire detector
	INTEGER NUM_SV_HYBRIDS
	PARAMETER ( NUM_SV_HYBRIDS = 122 )

	COMMON /SVGEOMETRY/
     +          SVBOUNDARY_X  (NUM_SV_HYBRIDS, 2),
     +  	SVBOUNDARY_Y  (NUM_SV_HYBRIDS, 2),
     + 	        SVBOUNDARY_R  (NUM_SV_HYBRIDS, 2),
     +          SVBOUNDARY_PHI(NUM_SV_HYBRIDS, 2),
     +          SVBOUNDARY_Z  (NUM_SV_HYBRIDS, 2),
     +          SVDXDZ        (NUM_SV_HYBRIDS   ),
     +          SVDYDZ        (NUM_SV_HYBRIDS   )
	

C NUM_SV_LAYERS.............. number of layers in detector
	INTEGER NUM_SV_LAYERS
	PARAMETER ( NUM_SV_LAYERS = 4 )

C MAXHYBRID.................. Max number of hybrids in any layer
	INTEGER MAXHYBRID
	PARAMETER ( MAXHYBRID = 52 )

C NUM_HYBRIDS_LAYER.........  Number of hybrids in indexed layer
C SV_HYBRIDS(4,MAXHYBRID)...  Hybrid numbers for indexed layer
	INTEGER NUM_HYBRIDS_LAYER
	INTEGER SV_HYBRIDS

C SV_RADIUS.................. Radii of Si layers (average)
	REAL SV_RADIUS

C NUM_SV_STRIPS.............. Number of strips in Si wafer
	INTEGER NUM_SV_STRIPS

C SV_PITCH_PHI............... strip-to-strip spacing in r-phi
C SV_PITCH_Z................. strip-to-strip spacing in z
	REAL SV_PITCH_PHI, SV_PITCH_Z

C SV_HYBRID_LAYER............ SVX layer for hybrid (filled by DOIT) 
	INTEGER SV_HYBRID_LAYER

	COMMON /SVPARAMETERS/
     +          SV_RADIUS        (NUM_SV_LAYERS),
     +          NUM_HYBRIDS_LAYER(NUM_SV_LAYERS),
     +          SV_HYBRIDS       (NUM_SV_LAYERS,MAXHYBRID),
     +          NUM_SV_STRIPS,
     +          SV_PITCH_PHI,
     +          SV_PITCH_Z,
     +          SV_HYBRID_LAYER  (NUM_SV_HYBRIDS)
C IL_TF_SV_F................. convert TF layer to SVX (phi) layer
C IL_TF_SV_Z................. convert TF layer to SVX (Z) layer
C IL_SV_F_TF................. convert SVX (phi) layer to TF layer
C IL_SV_Z_TF................. convert SVX (Z) layer to TF layer
	INTEGER IL_TF_SV_F
	INTEGER IL_TF_SV_Z
	INTEGER IL_SV_F_TF
	INTEGER IL_SV_Z_TF

	COMMON /SV_TF_INTERFACE/
     + 	         IL_TF_SV_F(KLYRTF),
     +           IL_TF_SV_Z(KLYRTF),
     +           IL_SV_F_TF(NUM_SV_LAYERS),
     +           IL_SV_Z_TF(NUM_SV_LAYERS)


C SV_HYBRID_USE_VALID........ =.TRUE. if there is valid info in SV_HYBRID_USE
C SV_HYBRID_USE.............. =.TRUE. if this hybid can be used, based on
C                             other information
C SV_HYBRID_IDENTIFY......... =.TRUE. if the hybrid has been identified for
C                             this layer
	LOGICAL SV_HYBRID_USE_VALID
	LOGICAL SV_HYBRID_USE
	LOGICAL SV_HYBRID_IDENTIFY

	COMMON /SV_FLAGS/
     +          SV_HYBRID_USE_VALID,
     +          SV_HYBRID_USE(NUM_SV_HYBRIDS),
     +          SV_HYBRID_IDENTIFY(NUM_SV_LAYERS)

C SV_HYBRID_EFF(hybrid,2).... efficiency of hybrid on PHI and Z sides
	REAL SV_HYBRID_EFF

	COMMON /SV_EFFS/
     +	SV_HYBRID_EFF         (NUM_SV_HYBRIDS, 2)
	









