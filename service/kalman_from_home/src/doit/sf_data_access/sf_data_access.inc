*
* $Id: sf_data_access.inc,v 1.2 1999/11/19 15:26:18 lyon Exp $
*
* $Log: sf_data_access.inc,v $
* Revision 1.2  1999/11/19 15:26:18  lyon
* TDC now read from TDCWCD, not TCWCD (oops)
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
#include "cl3seq/cdgm3/cdraw1in.inc" which includes cdrawspa and cdrawsin
#include "doit/duseq/tfctlcde.inc"
#include "cl3seq/zfiles/usbank.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfsvcom.inc"

#ifdef CLEO_SFNOZFILES
C if running without zfiles (e.g. within Suez)...
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"
#endif

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

	INTEGER ADR_USE,ADR_CD_COMPUT,ADR_SV_COMPUT
	INTEGER WIRE_COMPUT
	INTEGER MAP_START
	INTEGER LAYR_SF_COMPUT,LAYR_CD_COMPUT,LAYR_CD_HOLD
        INTEGER LAYER_TEST,MAX_INDXCD
	LOGICAL USE_NON_CD

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
C                  .............ADR_CD_COMPUT,WIRE_COMPUT,ADR_USE
                   
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
	   ADR_USE=ADR_CD_COMPUT

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
C                  .............ADR_CD_COMPUT,WIRE_COMPUT,ADR_USE
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
	   ADR_USE=ADR_CD_COMPUT

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
		       ADR_USE=ADR_CD_COMPUT
     2                        +NWIRCD(LAYR_CD_COMPUT)
		    ELSE
		       ADR_USE=ADR_CD_COMPUT
     2                        -NWIRCD(LAYR_CD_COMPUT)
		    ENDIF
	         ENDIF
	      ENDIF
	   ENDIF
#endif
	   IF(MAP_START.EQ.0)THEN
#ifdef CLEO_SFNOZFILES
	     SF_DATA_OUTPUT_HIT_ADR = mapcd(ADR_USE)
#else
	     SF_DATA_OUTPUT_HIT_ADR=IZU(PZU(IPMPCD)+ADR_USE)
#endif
	   ELSE
#ifdef CLEO_SFNOZFILES
	     SF_DATA_OUTPUT_HIT_ADR = mapcd(ADR_USE)
#else
	     SF_DATA_OUTPUT_HIT_ADR=IZU(MAP_START+ADR_USE)
#endif
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
#ifdef CLEO_SFNOZFILES
C*******  Without zfiles, map_start is always zero (I hope)
	  MAP_START = 0
#else
	  MAP_START=PZU(IPMPCD)
#endif
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
#ifdef CLEO_SFNOZFILES
	  SF_DATA_OUTPUT_HIT_ADR = exfhcd(LAYR_CD_HOLD)
#else
	   SF_DATA_OUTPUT_HIT_ADR=IZU(PZU(IPFHCD)+LAYR_CD_HOLD)
#endif
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
#ifdef CLEO_SFNOZFILES
	  SF_DATA_OUTPUT_HIT_ADR = exnhcd(SF_DATA_INPUT_HIT_ADR)
#else
	   SF_DATA_OUTPUT_HIT_ADR=IZU(PZU(IPNHCD)+SF_DATA_INPUT_HIT_ADR)
#endif
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

#ifdef CLEO_SFNOZFILES
	SF_DATA_OUTPUT_LAYR_CD = icylcd(SF_DATA_INPUT_HIT_ADR)
#else
	SF_DATA_OUTPUT_LAYR_CD=IZU(PZU(IPLYCD)+SF_DATA_INPUT_HIT_ADR)
#endif

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
#ifdef CLEO_SFNOZFILES
	  SF_DATA_OUTPUT_WIRE = iwircd(SF_DATA_INPUT_HIT_ADR)
#else
	  SF_DATA_OUTPUT_WIRE=IZU(PZU(IPWRCD)+SF_DATA_INPUT_HIT_ADR)
#endif
#ifdef CLEO_SFDUET
C swap the VD data
	   IF(LUSVDE)THEN
#ifdef CLEO_SFNOZFILES
	     LAYR_CD_COMPUT = icylcd(SF_DATA_INPUT_HIT_ADR)
#else
	      LAYR_CD_COMPUT=IZU(PZU(IPLYCD)+SF_DATA_INPUT_HIT_ADR)
#endif
	      IF(LAYR_CD_COMPUT.GE.LCANVD)THEN
	         IF(LAYR_CD_COMPUT.LE.MCANVD)THEN
		    IF(SF_DATA_OUTPUT_WIRE.LT.
     2                                     NWIRCD(LAYR_CD_COMPUT))THEN
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

#ifdef CLEO_SFNOZFILES
	SF_DATA_OUTPUT_TDC_RAW = tdcwcd(SF_DATA_INPUT_HIT_ADR)
#else
	SF_DATA_OUTPUT_TDC_RAW=IZU(PZU(IPTDCD)+SF_DATA_INPUT_HIT_ADR)
#endif

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

#ifdef CLEO_SFNOZFILES
	SF_DATA_OUTPUT_R_TIM_PS = tdwcd(SF_DATA_INPUT_HIT_ADR)
#else
	SF_DATA_OUTPUT_R_TIM_PS=ZU(PZU(IPTWCD)+SF_DATA_INPUT_HIT_ADR)
#endif

#undef SF_DATA_TMPS
#endif
#ifdef SF_DATA_RADC

C*************************************************************************
C SF_DATA_RADC --- raw ADC counts for this hit address
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_ADC_RAW
C "local" variables..............
C*************************************************************************

#ifdef CLEO_SFNOZFILES
	SF_DATA_OUTPUT_ADC_RAW = exradc(SF_DATA_INPUT_HIT_ADR)
#else
	SF_DATA_OUTPUT_ADC_RAW=IZU(PZU(IPADCD)+SF_DATA_INPUT_HIT_ADR)
#endif

#undef SF_DATA_RADC
#endif
#ifdef SF_DATA_ADCH

C*************************************************************************
C SF_DATA_ADCH ---PULSE HEIGHT IN HAPPY UNITS
C
C requires input.................SF_DATA_INPUT_HIT_ADR
C provides output................SF_DATA_OUTPUT_PH_HAPPY
C "local" variables..............
C*************************************************************************

#ifdef CLEO_SFNOZFILES
	SF_DATA_OUTPUT_PH_HAPPY = qwcd(SF_DATA_INPUT_HIT_ADR)
#else
	SF_DATA_OUTPUT_PH_HAPPY=IZU(PZU(IPQWCD)+SF_DATA_INPUT_HIT_ADR)
#endif

#undef SF_DATA_ADCH
#endif






