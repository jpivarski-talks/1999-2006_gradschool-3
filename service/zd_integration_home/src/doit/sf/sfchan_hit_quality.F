*
* $Id: sfchan_hit_quality.F,v 1.3 2003/03/05 18:18:47 dpp Exp $
*
* $Log: sfchan_hit_quality.F,v $
* Revision 1.3  2003/03/05 18:18:47  dpp
*      -> new assignments for negative INUCHN
*
* Revision 1.2  2002/09/24 00:32:50  dpp
*      -> move remaining sfchan_hit_quality arguments to common
*
* Revision 1.1  2002/09/05 18:52:49  dpp
*      -> NEW ROUTINE
*      -> PROCEDURE TO DETERMINE HIT-QUALITY-OR-ERROR
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : DAN PETERSON

      SUBROUTINE SFCHAN_HIT_QUALITY

C......................................................................
C.
C. SFCHAN_HIT_QUALITY - hit quality or error procedure for SFCHAN
C.
C. COMMON    : /STEPCn/
C. CALLS     :  
C. CALLED    : SFCHAN
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 28-MAY-02
C.
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"

#include "doit/sfseq/sfchan_ctl.inc"
#include "doit/sfseq/sfchan_hit.inc"

#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

C-----------------------------------------------------------------------
C  VARIABLES WITHIN SF_DATA_ACCESS
C-----------------------------------------------------------------------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

C-----------------------------------------------------------------------
C copied from sfchan_hit.inc  
C-----------------------------------------------------------------------

C ILYRT....LAYER CURRENTLY BEING TESTED             SSF INDEXING
C LADR.....ADDRESS OF FIRST WIRE IN PRESENT LAYER,  SSF INDEXING
C MADR.....ADDRESS OF LAST WIRE IN PRESENT LAYER,   SSF INDEXING
C WNTU.....WIRE NUMBER  -NOW -TESTING  -USER        SSF INDEXING
C ADTSF....ADDRESS OF THE "TESTED" WIRE,            SF INDEXING
C ADASF....ADDRESS OF ADJACENT HIT WIRE, IF ANY,    SF INDEXING
C WNFX.....WIRE NUMBER  -NOW -FOUND    -MULTIPLIED
C WSHFX....SHIFT OF WIRE POSITION, DUE TO DOUBLETS, MULTIPLIED
C WNFXL....WIRE NUMBER  -NOW -FOUND    -MULTIPLIED   -LOLIMIT
C WNFXH....WIRE NUMBER  -NOW -FOUND    -MULTIPLIED   -HILIMIT
C IHQLER............HIT QUALITY OR ERROR CODE WORD
C                   ERROR: -5  NO HIT FOUND IN SFDATA MAP
C                   ERROR: -4  HIT FLAGGED BAD IN SFIND,
C                                   INUCHN=-1  clustered hit
C                                   INUCHN=-2  HIGH TIME
C                                   INUCHN=-3  HIGH TIME
C                                   INUCHN=-4  HIGH TIME
C                                   INUCHN=-5  HIGH TIME
C                                   INUCHN=-6  HIGH PH (PEDISTAL)
C                                   INUCHN=-7  ALREADY USED IN DUET
C                                   INUCHN=-8  ADJACENT TO USED IN DUET
C                                   INUCHN=-9  not clustered, low charge
C                   ERROR: -3  ALREADY USED IN ANOTHER CHAIN IN SFIND
C                   QUAL:  -2  TRIPLET OR WORSE HIT, HIT NOT USED
C                   QUAL:   0  GOOD
C                   QUAL:   1  DOUBLET HIT, ADDRESS OF PARTNER IN ADASF
C                   QUAL:   2  TRIPLET OR WORSE HIT, HIT USED
C FIRST_SEARCH_DIR..INITIAL DIR TO MOVE FOR NEXT WIRE, USED FOR ADJACENT
C CROSS_WIDTH.......WIDTH OF CELL CROSSED DUE TO VELOCITY, ABS
C OKWSTR...OK-WIDTH-OF-STRING, WIDENING ALLOWED FOR LARGE VEL_PRED
C USE_AJUSOK........=1 IF IHQLER IS GOOD ONLY BY USING THE AJUSOK FLAG
C PROC_BACKTRACK....=1 I IT IS A BACKTRACK LAYER; =0 IF A NORMAL LAYER

C-----------------------------------------------------------------------
C  LOCAL VARIABLE DOCUMENTION
C-----------------------------------------------------------------------

C ADRHT....ADDRESS OF TESTED HIT WIRE, IN THE SHORT HIT LIST
      INTEGER ADRHT
C ADTSSF...INPUT ADDRESS OF THE "TESTED" WIRE,            SSF INDEXING
      INTEGER ADTSSF
C ISIGNP...SIGN OF THE SIDE THAT IS BEING CHECKED
      INTEGER ISIGNP
C ADPSF....LOCAL PROCEDURE WIRE ADDRESS,            SF INDEXING
      INTEGER ADPSF
C ADPCD....LOCAL PROCEDURE WIRE ADDRESS,            CD INDEXING
      INTEGER ADPCD
C ADPSSF...LOCAL PROCEDURE WIRE ADDRESS,            SSF INDEXING
      INTEGER ADPSSF
C ADPSV....LOCAL PROCEDURE WIRE ADDRESS,            SV INDEXING
      INTEGER ADPSV
C CELL_CLEAR.......=1 IF THE CELL IS TREATED AS CLEAR
      INTEGER CELL_CLEAR
C FNDCLR...NUMBER OF WIRES FOUND CLEAR; TO COMPARE TO NEDCLR
      INTEGER FNDCLR
C CONTIG...........=1 IF HITS ARE CONTIGUOUS ON THIS SIDE OF PRIMARY WIRE
      INTEGER CONTIG
C WSTRNG...TOTAL WIDTH OF STRING, COMPARE TO OKWSTR
      INTEGER WSTRNG
C WSTRP....WIDTH OF STRING TO PLUS  SIDE
      INTEGER WSTRP
C WSTRM....WIDTH OF STRING TO MINUS SIDE
      INTEGER WSTRM

C================================================================
C================================================================
C
C   -----------  Executable code starts here  ---------------
C
C================================================================
C================================================================

C================================================================
C PROCEDURE TO DETERMINE HIT-QUALITY-OR-ERROR
C================================================================

      ADTSSF=LADR+WNTU
      ADTSF=MAPSSF(ADTSSF)
      
      WSHFX=0
      ADASF=0
      
      sf_data_input_sf_adr=ADTSF
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      ADRHT=sf_data_output_hit_adr

C this tests only that a hit exists, used hits are identified with INUCHN
      
      IF(ADRHT.LE.0)THEN
        IHQLER=-5
      ELSEIF(INUCHN(ADTSF).LT.0)THEN
        IHQLER=-4
      ELSEIF(INUCHN(ADTSF).GT.0)THEN
        IHQLER=-3
      ELSEIF(INUCHN(ADTSF).EQ.0)THEN
        IHQLER=0
        USE_AJUSOK=0
        OKWSTR=2+(CROSS_WIDTH+MULSTH)/MULST
        ISIGNP=FIRST_SEARCH_DIR
        WSTRNG=1
        WSTRM=0
        WSTRP=0
 735    ADPSSF=ADTSSF
        FNDCLR=0
        CONTIG=1
 737    ADPSSF=ADPSSF+ISIGNP
        IF(ISIGNP.EQ.1)THEN
          IF(ADPSSF.GT.MADR)ADPSSF=LADR
        ELSE
          IF(ADPSSF.LT.LADR)ADPSSF=MADR
        ENDIF
        ADPSF=MAPSSF(ADPSSF)
        
        sf_data_input_sf_adr=ADPSF
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
        ADRHT=sf_data_output_hit_adr
        
        IF(ADRHT.GT.0)THEN
          IF(INUCHN(ADPSF).EQ.0)THEN
            CELL_CLEAR=0
            IF(CONTIG.NE.1)THEN
              IHQLER=2
            ELSEIF(WSTRNG.GE.OKWSTR)THEN
              IHQLER=2
            ELSE
              WSTRNG=WSTRNG+1
              IF(ISIGNP.EQ.1)THEN
                WSTRP=WSTRP+1
              ELSE
                WSTRM=WSTRM+1
              ENDIF
              IF(ADASF.EQ.0)THEN
                IF(IHQLER.NE.2)THEN
                  WSHFX=ISIGNP*MULSTH
                  ADASF=ADPSF
                  IHQLER=1
                ENDIF
              ENDIF
            ENDIF
          ELSEIF(INUCHN(ADPSF).GT.0)THEN
            IF(AJUSOK(SFCTYP).EQ.0)THEN
              CELL_CLEAR=0
              IHQLER=2
            ELSE
              CELL_CLEAR=1
              USE_AJUSOK=1
            ENDIF
          ELSEIF(INUCHN(ADPSF).LT.0)THEN
            CELL_CLEAR=1
          ENDIF
        ELSE            
          CELL_CLEAR=1
        ENDIF
        
C CLEAR CELL FOUND
        IF(IHQLER.NE.2)THEN
          IF(CELL_CLEAR.EQ.1)THEN
            FNDCLR=FNDCLR+1
            CONTIG=0
          ENDIF
          IF(FNDCLR.LT.NEDCLR(SFCTYP))GO TO 737
        ENDIF
        
C ENOUGH CLEAR; LOOP BACK (EVEN FOR IHQLER=2 TO DETERMINE WNFXL,WNFXH) 
        IF(ISIGNP.EQ.FIRST_SEARCH_DIR)THEN
          ISIGNP=-FIRST_SEARCH_DIR
          GO TO 735
        ENDIF
        
        IF(IHQLER.EQ.2)THEN
          WSHFX=0
          ADASF=0
        ENDIF
        
        WNFX=WNTU*MULST+WSHFX
        WNFXL=(WNTU-WSTRM)*MULST-MULSTH
        WNFXH=(WNTU+WSTRP)*MULST+MULSTH
      ENDIF
      
      PROC_BACKTRACK=0
      
      RETURN
      END








