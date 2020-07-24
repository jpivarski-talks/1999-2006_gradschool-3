*
* $Id: sf_data_access.inc,v 1.4 2003/04/07 17:30:33 cleo3 Exp $
*
* $Log: sf_data_access.inc,v $
* Revision 1.4  2003/04/07 17:30:33  cleo3
* now compiles under Linux
*
* Revision 1.3  2003/03/05 18:39:12  dpp
*      -> remove zfiles stuff and use of SFNOZFILES switch
*      -> add access to next hit in a cluster array, etc.
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.2  1999/11/19 15:26:18  lyon
* TDC now read from EvntHit_rawTDC, not EvntHit_corTIM (oops)
*
* Revision 1.1  1998/07/09 01:14:13  lyon
* Adding sf_data_access.inc (code snipits for getting data that are #included)
*
*
*
C......................................................................
C.
C. sf_data_access  : a group of code segments to be spliced into
C.                   SF fortran programs to access the tracking
C.                   detector data
C.                   These are, in effect, in-line subroutines 
C.
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 22-April-98
C. LAST MOD  : 22-April-98
C.
C. Modification Log.
C.    A. Lyon  6-Jul-98:  Added code for retrieving data without zfiles.
C.                        use CLEO_SFNOZFILES compiler directive
C......................................................................
C common blocks required in the "calling" subroutine
C to variables: 
C              NLYRCD,NWIRCD      cdgeomcd
C              INDXCD             cdgeomcd
C              LCANVD,MCANVD      cdgeomcd
C              IPMPCD etc         cdrawsin which is included in cdraw1in
C              IZU                usbank
C              PZU                uslink which is included in usbank
C              LUSVDE             tfctlcde

#ifdef SF_DATA_INCLUDES

#ifdef CLEO_SFDUET
C if running under DOIT....include tfindpar which includes cdgeompa.
#include "doit/duseq/tfindpar.inc"
#endif

#ifndef CLEO_SFDUET
C if not running under DOIT...
#include "cl3seq/cdgm3/cdgeompa.inc"
#endif

C in either case
#include "cl3seq/cdgm3/cdgeomcd.inc"
C the following includes cdrawspa and cdrawsin
#include "cl3seq/cdgm3/cdraw1in.inc" 
#include "doit/duseq/tfctlcde.inc"
#include "cl3seq/zfiles/usbank.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"

C if running without zfiles (e.g. within Suez)...
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"

#undef SF_DATA_INCLUDES
#endif
#ifdef SF_DATA_DECLARE

C*************************************************************************
C SF_DATA_DEFINE --- local variables required for the SF_DATA code segments 
C*************************************************************************


	INTEGER SF_DATA_INPUT_HIT_ADR
        INTEGER SF_DATA_OUTPUT_HIT_ADR
	INTEGER SF_DATA_INPUT_SF_ADR
	INTEGER SF_DATA_INPUT_CD_ADR
	INTEGER SF_DATA_INPUT_SFSVWE
	INTEGER SF_DATA_INPUT_FLAG
        INTEGER SF_DATA_INPUT_LAYR_SF
        INTEGER SF_DATA_INPUT_LAYR_CD
	INTEGER SF_DATA_OUTPUT_LAYR_CD
        INTEGER SF_DATA_OUTPUT_WIRE
	INTEGER SF_DATA_OUTPUT_TDC_RAW
	INTEGER SF_DATA_OUTPUT_ADC_RAW
	INTEGER SF_DATA_OUTPUT_PH_HAPPY
	REAL    SF_DATA_OUTPUT_R_TIM_PS
	REAL    SF_DATA_OUTPUT_DRIFT
        INTEGER SF_DATA_OUTPUT_OPEN_HIT
	INTEGER SF_DATA_OUTPUT_NXT_CLUS
	INTEGER SF_DATA_INPUT_NXT_CLUS
	INTEGER SF_DATA_OUTPUT_PRIMARY
	INTEGER SF_DATA_INPUT_PRIMARY
        INTEGER SF_DATA_OUTPUT_HITS_CELL
        INTEGER SF_DATA_OUTPUT_NEXT_MULT
	
	INTEGER MapCellID_use
	INTEGER ADR_CD_COMPUT,ADR_SV_COMPUT
	INTEGER WIRE_COMPUT
	INTEGER MAP_START
	INTEGER LAYR_SF_COMPUT,LAYR_CD_COMPUT,LAYR_CD_HOLD
        INTEGER LAYER_TEST,MAX_INDXCD
	LOGICAL USE_NON_CD
	LOGICAL USE_PRIME_HOLD

	INTEGER SFSVWE_HOLD
        COMMON/SF_DATA_COM/SFSVWE_HOLD

	DATA MAP_START/0/

#undef SF_DATA_DECLARE
#endif


#ifdef SF_DATA_SET_SFSVWE
C*************************************************************************
C SF_DATA_SET_SFSVWE --- set the w/e index for SFSV (ie non CD) layers
C
C requires input................SF_DATA_INPUT_SFSVWE
C provides output...
C "local" variables.............SFSVWE_HOLD
C*************************************************************************
	SFSVWE_HOLD=SF_DATA_INPUT_SFSVWE

#undef SF_DATA_SET_SFSVWE
#endif


#ifdef SF_DATA_MAPV
C*************************************************************************
C SF_DATA_MAPV --- hit number stored in map, starting with SF CELL-ID
C
C requires input................SF_DATA_INPUT_SF_ADR
C provides output...............SF_DATA_OUTPUT_HIT_ADR
C "local" variables.............LAYR_SF_COMPUT,LAYR_CD_COMPUT,ADR_SV_COMPUT,
C                  .............ADR_CD_COMPUT,WIRE_COMPUT,MapCellID_use
                   
C*************************************************************************
	LAYR_SF_COMPUT=SFLSFA(SF_DATA_INPUT_SF_ADR)
	LAYR_CD_COMPUT=CDLASF(LAYR_SF_COMPUT)

	IF(LAYR_CD_COMPUT.LT.0)THEN
	   ADR_SV_COMPUT=SF_DATA_INPUT_SF_ADR-INSFSF(LAYR_SF_COMPUT)
     2          +SFSVIN(-LAYR_CD_COMPUT,SFSVWE_HOLD)
           IF(SFSVWE_HOLD.EQ.2)
     1         ADR_SV_COMPUT=ADR_SV_COMPUT+SFSVCL(-LAYR_CD_COMPUT)
           SF_DATA_OUTPUT_HIT_ADR=SFSVMP(ADR_SV_COMPUT)

	ELSE
	   WIRE_COMPUT=SF_DATA_INPUT_SF_ADR-INSFSF(LAYR_SF_COMPUT)
	   ADR_CD_COMPUT=WIRE_COMPUT       +INCDSF(LAYR_SF_COMPUT)
	   MapCellID_use=ADR_CD_COMPUT

#define SF_DATA_MAPV_MORE
#undef SF_DATA_MAPV
#endif


#ifdef SF_DATA_MAPV_CD
C*************************************************************************
C SF_DATA_MAPV_CD --- hit number stored in map, starting with CD CELL-ID
C
C requires input................SF_DATA_INPUT_CD_ADR
C provides output...............SF_DATA_OUTPUT_HIT_ADR
C "local" variables.............LAYR_SF_COMPUT,LAYR_CD_COMPUT,
C                  .............ADR_CD_COMPUT,WIRE_COMPUT,MapCellID_use
C                  .............LAYER_TEST,MAX_INDXCD                   
C*************************************************************************
	IF(.TRUE.)THEN
	   ADR_CD_COMPUT=SF_DATA_INPUT_CD_ADR
           DO LAYER_TEST=1,NLYRCD
	      IF(INDXCD(LAYER_TEST).LE.ADR_CD_COMPUT)THEN
	         IF(INDXCD(LAYER_TEST).GE.MAX_INDXCD)THEN
	            MAX_INDXCD=INDXCD(LAYER_TEST)
	            LAYR_CD_COMPUT=LAYER_TEST
	            WIRE_COMPUT=ADR_CD_COMPUT-MAX_INDXCD
   	         ENDIF
	      ENDIF
	   ENDDO
	   LAYR_SF_COMPUT=SFLACD(LAYR_CD_COMPUT)
	   MapCellID_use=ADR_CD_COMPUT

#define SF_DATA_MAPV_MORE
#undef SF_DATA_MAPV_CD
#endif


#ifdef SF_DATA_MAPV_MORE
C*************************************************************************
C SF_DATA_MAPV_MORE --- extension to SF_DATA_MAPV and SF_DATA_MAPV_CD 
C*************************************************************************
#ifdef CLEO_SFDUET
C swap the VD data
	   IF(LUSVDE)THEN
	      IF(LAYR_CD_COMPUT.GE.LCANVD)THEN
	         IF(LAYR_CD_COMPUT.LE.MCANVD)THEN
		    IF(WIRE_COMPUT.LT.NWIRCD(LAYR_CD_COMPUT))THEN
		       MapCellID_use=ADR_CD_COMPUT
     2                        +NWIRCD(LAYR_CD_COMPUT)
		    ELSE
		       MapCellID_use=ADR_CD_COMPUT
     2                        -NWIRCD(LAYR_CD_COMPUT)
		    ENDIF
	         ENDIF
	      ENDIF
	   ENDIF
#endif
	   IF(MAP_START.EQ.0)THEN
	     SF_DATA_OUTPUT_HIT_ADR = EvntHit_MapDet(MapCellID_use)
	   ELSE
	     SF_DATA_OUTPUT_HIT_ADR = EvntHit_MapDet(MapCellID_use)
	   ENDIF
	   
	   IF(EvntHit_UsePrim)THEN
	     SF_DATA_OUTPUT_HIT_ADR=EvntHit_PrimMlt(
	1	  EvntHit_1stMlt(SF_DATA_OUTPUT_HIT_ADR))
	   ENDIF

	ENDIF

#undef SF_DATA_MAPV_MORE
#endif



#ifdef SF_DATA_MAP_START
C*************************************************************************
C SF_DATA_MAP_START --- set the map address in MAP_START
C
C requires input...............SF_DATA_INPUT_FLAG
C provides output..............
C "local" variables............MAP_START
C*************************************************************************

	IF(SF_DATA_INPUT_FLAG.EQ.0)THEN
	   MAP_START=0
	ELSE
C*******  Without zfiles, map_start is always zero (I hope)
	  MAP_START = 0
	ENDIF

#undef SF_DATA_MAP_START
#endif


#ifdef SF_DATA_1STH
C*************************************************************************
C SF_DATA_1STH --- hit address of the first hit of this layer, SF address
C
C requires input.................SF_DATA_INPUT_LAYR_SF
C provides for subsequent calls..LAYR_CD_HOLD
C provides output................SF_DATA_OUTPUT_HIT_ADR
C "local" variables..............
C*************************************************************************
	LAYR_CD_HOLD=CDLASF(SF_DATA_INPUT_LAYR_SF)
	USE_NON_CD=.TRUE.

#define SF_DATA_1STH_MORE
#undef SF_DATA_1STH
#endif


#ifdef SF_DATA_1STH_CD
C*************************************************************************
C SF_DATA_1STH_CD --- hit address of the first hit of this layer, CD address
C
C requires input.................SF_DATA_INPUT_LAYR_CD
C provides for subsequent calls..LAYR_CD_HOLD
C provides output................SF_DATA_OUTPUT_HIT_ADR
C "local" variables..............
C*************************************************************************
	LAYR_CD_HOLD=SF_DATA_INPUT_LAYR_CD
	USE_NON_CD=.FALSE.

#define SF_DATA_1STH_MORE
#undef SF_DATA_1STH_CD
#endif


#ifdef SF_DATA_1STH_MORE
C*************************************************************************
C SF_DATA_1STH_MORE extension to  SF_DATA_1STH and SF_DATA_1STH_CD
C*************************************************************************
        IF(LAYR_CD_HOLD.GE.0)THEN

	  IF(EvntHit_UsePrim)THEN
	    SF_DATA_OUTPUT_HIT_ADR=
     1           EvntHit_PrimMlt(
     2           EvntHit_1stLyr(LAYR_CD_HOLD))

	  ELSE
	    SF_DATA_OUTPUT_HIT_ADR = EvntHit_1stLyr(LAYR_CD_HOLD)
	  ENDIF

        ELSEIF(USE_NON_CD)THEN
           SF_DATA_OUTPUT_HIT_ADR=SFSV1S(-LAYR_CD_HOLD)
	ENDIF



#undef SF_DATA_1STH_MORE
#endif


#ifdef SF_DATA_NXTH
C*************************************************************************
C SF_DATA_NXTH --- next address, after the current address, in this layer
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C requires set in earlier call...LAYR_CD_HOLD
C provides output................SF_DATA_OUTPUT_HIT_ADR
C "local" variables..............
C*************************************************************************

	IF(LAYR_CD_HOLD.LT.0)THEN
	   SF_DATA_OUTPUT_HIT_ADR=SFSVNX(SF_DATA_INPUT_HIT_ADR)
	ELSE

	  IF(EvntHit_UsePrim)THEN
	    SF_DATA_OUTPUT_HIT_ADR=
     1	       EvntHit_PrimMlt(
     2	       EvntHit_NxtLyr(
     3	       EvntHit_1stMlt(SF_DATA_INPUT_HIT_ADR)))
	  ELSE

	    SF_DATA_OUTPUT_HIT_ADR = 
     1         EvntHit_NxtLyr(SF_DATA_INPUT_HIT_ADR)

	  ENDIF
	ENDIF
	
#undef SF_DATA_NXTH
#endif


#ifdef SF_DATA_LAYR_CD
C*************************************************************************
C SF_DATA_LAYR_CD --- layer number for this hit address
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_LAYR_CD
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_LAYR_CD = EvntHit_CDlayr(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_LAYR_CD
#endif


#ifdef SF_DATA_WIRN
C*************************************************************************
C SF_DATA_WIRN --- wire number for this hit address
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C requires set in earlier call...LAYR_CD_HOLD
C provides output................SF_DATA_OUTPUT_WIRE
C "local" variables..............LAYR_CD_COMPUT
C*************************************************************************
        IF(LAYR_CD_HOLD.LT.0)THEN
           SF_DATA_OUTPUT_WIRE=SF_DATA_INPUT_HIT_ADR
     2                        -SFSVIN(-LAYR_CD_HOLD,SFSVWE_HOLD)
        ELSE
	  SF_DATA_OUTPUT_WIRE = EvntHit_laWire(SF_DATA_INPUT_HIT_ADR)
#ifdef CLEO_SFDUET
C swap the VD data
	   IF(LUSVDE)THEN
	     LAYR_CD_COMPUT = EvntHit_CDlayr(SF_DATA_INPUT_HIT_ADR)
	      IF(LAYR_CD_COMPUT.GE.LCANVD)THEN
	         IF(LAYR_CD_COMPUT.LE.MCANVD)THEN
		    IF(SF_DATA_OUTPUT_WIRE.LT.
     2    		NWIRCD(LAYR_CD_COMPUT))THEN
		       SF_DATA_OUTPUT_WIRE=SF_DATA_OUTPUT_WIRE
     2                                    +NWIRCD(LAYR_CD_COMPUT)
		    ELSE
		       SF_DATA_OUTPUT_WIRE=SF_DATA_OUTPUT_WIRE
     2                                    -NWIRCD(LAYR_CD_COMPUT)
		    ENDIF
	         ENDIF
	      ENDIF
	   ENDIF
#endif
	ENDIF

#undef SF_DATA_WIRN
#endif


#ifdef SF_DATA_RTDC
C*************************************************************************
C SF_DATA_RTDC --- raw TDC counts for this hit address
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_TDC_RAW
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_TDC_RAW = EvntHit_rawTDC(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_RTDC
#endif


#ifdef SF_DATA_TMPS
C*************************************************************************
C SF_DATA_TMPS --- time in picoseconds for this hit address
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_R_TIM_PS
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_R_TIM_PS = EvntHit_rawTIM(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_TMPS
#endif


#ifdef SF_DATA_DRIFT_DISTANCE
C*************************************************************************
C SF_DATA_DRIFT_DISTANCE --- drift distance in meters
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_DRIFT
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_DRIFT = EvntHit_Drift(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_DRIFT_DISTANCE
#endif


#ifdef SF_DATA_RADC
C*************************************************************************
C SF_DATA_RADC --- raw ADC counts for this hit address
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_ADC_RAW
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_ADC_RAW = EvntHit_rawADC(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_RADC
#endif


#ifdef SF_DATA_ADCH
C*************************************************************************
C SF_DATA_ADCH ---pulse height in happy units
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_PH_HAPPY
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_PH_HAPPY = EvntHit_Charge(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_ADCH
#endif




#ifdef SF_DATA_OPEN_HIT
C*************************************************************************
C SF_DATA_OPEN_HIT ---indication of hit previously used
C                     <0 previously used, >/= 0 open for use
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_OPEN_HIT
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_OPEN_HIT = EvntHit_OpnHit(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_OPEN_HIT
#endif



#ifdef SF_DATA_GET_NEXT_HIT_CLUSTER
C*************************************************************************
C SF_DATA_GET_NEXT_HIT_CLUSTER ---address of next hit in a cluster
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_NXT_CLUS
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_NXT_CLUS = EvntHit_NxtCls(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_GET_NEXT_HIT_CLUSTER
#endif


#ifdef SF_DATA_PUT_NEXT_HIT_CLUSTER
C*************************************************************************
C SF_DATA_PUT_NEXT_HIT_CLUSTER ---address of next hit in a cluster
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C requires input.................SF_DATA_INPUT_NXT_CLUS
C provides no output
C "local" variables..............
C*************************************************************************

	EvntHit_NxtCls(SF_DATA_INPUT_HIT_ADR) = SF_DATA_INPUT_NXT_CLUS

#undef SF_DATA_PUT_NEXT_HIT_CLUSTER
#endif


#ifdef SF_DATA_GET_CLUSTER_PRIMARY
C*************************************************************************
C SF_DATA_GET_CLUSTER_PRIMARY ---address primary hit in cluster
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_PRIMARY
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_PRIMARY = EvntHit_PrimCls(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_GET_CLUSTER_PRIMARY
#endif


#ifdef SF_DATA_PUT_CLUSTER_PRIMARY
C*************************************************************************
C SF_DATA_PUT_CLUSTER_PRIMARY ---address primary hit in cluster
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C requires input.................SF_DATA_INPUT_PRIMARY
C provides no output
C "local" variables..............
C*************************************************************************

	EvntHit_PrimCls(SF_DATA_INPUT_HIT_ADR) = SF_DATA_INPUT_PRIMARY

#undef SF_DATA_PUT_CLUSTER_PRIMARY
#endif


#ifdef SF_DATA_NHITS_MULTIHIT
C*************************************************************************
C SF_DATA_NHITS_MULTIHIT ---number of hits in the same cell ID
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_HITS_CELL
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_HITS_CELL = EvntHit_nMult(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_NHITS_MULTIHIT
#endif



#ifdef SF_DATA_NEXTHIT_MULTIHIT
C*************************************************************************
C SF_DATA_NEXTHIT_MULTIHIT ---next hit number in the same cell ID
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_NEXT_MULT
C "local" variables..............
C*************************************************************************

	SF_DATA_OUTPUT_NEXT_MULT = EvntHit_NxtMlt(SF_DATA_INPUT_HIT_ADR)

#undef SF_DATA_NEXTHIT_MULTIHIT
#endif


#ifdef SF_DATA_SET_PRIME_MULTIHIT
C*************************************************************************
C SF_DATA_SET_PRIME_MULTIHIT --- sets the prime hit, the one that should
C                             be used for later calculations
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides no output
C "local" variables..............
C*************************************************************************
	
	
	EvntHit_PrimMlt(EvntHit_1stMlt(SF_DATA_INPUT_HIT_ADR))=
	1    SF_DATA_INPUT_HIT_ADR

#undef SF_DATA_SET_PRIME_MULTIHIT
#endif


#ifdef SF_DATA_USE_PRIME_MULTIHIT
C*************************************************************************
C SF_DATA_USE_PRIME_MULTIHIT --- use the primary hit of multi-hit electronics
C                             instead of the hit addressed by 1st, nxt or map
C
C requires no input
C provides no output
C "local" variables..............USE_PRIME_HOLD
C*************************************************************************

	USE_PRIME_HOLD=EvntHit_UsePrim
	EvntHit_UsePrim=.TRUE.

#undef SF_DATA_USE_PRIME_MULTIHIT
#endif



#ifdef SF_DATA_USE_ADDRESSED_MULTIHIT
C*************************************************************************
C SF_DATA_USE_ADDRESSED_MULTIHIT --- opposite of SF_DATA_USE_PRIME_MULTIHIT
C
C requires no input
C provides no output
C "local" variables..............USE_PRIME_HOLD
C*************************************************************************

	USE_PRIME_HOLD=EvntHit_UsePrim
	EvntHit_UsePrim=.FALSE.

#undef SF_DATA_USE_ADDRESSED_MULTIHIT
#endif



#ifdef SF_DATA_USE_MULTIHIT_RESTORE
C*************************************************************************
C SF_DATA_USE_MULTIHIT_RESTORE --- reset the promary hit use to prev value
C
C requires no input
C provides no output
C "local" variables..............USE_PRIME_HOLD
C*************************************************************************

	EvntHit_UsePrim=USE_PRIME_HOLD

#undef SF_DATA_USE_MULTIHIT_RESTORE
#endif
