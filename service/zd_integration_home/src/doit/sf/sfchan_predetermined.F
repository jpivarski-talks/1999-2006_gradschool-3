*
* $Id: sfchan_predetermined.F,v 1.3 2003/03/05 18:11:46 dpp Exp $
*
* $Log: sfchan_predetermined.F,v $
* Revision 1.3  2003/03/05 18:11:46  dpp
*      -> new assignments for negative INUCHN
*
* Revision 1.2  2002/09/24 00:33:37  dpp
*      -> move remaining sfchan_hit_quality arguments to commo
*
* Revision 1.1  2002/09/05 18:53:18  dpp
*      -> NEW ROUTINE
*      -> PROCEDURE TO PROCESS-PREDETERMINED-HITS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : DAN PETERSON

      SUBROUTINE SFCHAN_PREDETERMINED(CALC_ILYRT,SFCERR)


C......................................................................
C.
C. SFCHAN_PREDETERMINED - procedure to process predetermined hits 
C.                            for SFCHAN
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
C  ARGUMENT VARIABLE DOCUMENTION
C-----------------------------------------------------------------------
C CALC_ILYRT........=.TRUE. if ILYRT MUST BE CALCULATED FROM INPUT ADTSF
      LOGICAL CALC_ILYRT
C SFCERR...RETURNED ERROR, =0 FOR NO ERROR
      INTEGER SFCERR

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

C LSFTMP...LAYER SF, TEMPORARY; FOR CALCULATION OF CD ADDRESS
      INTEGER LSFTMP
C ILYRX....LAYER NUMBER OF NEXT FOUND HIT IN THE PREDETERMINED CHAIN
      INTEGER ILYRX
C IADRX....ADDRESS OF NEXT FOUND HIT IN THE PREDETERMINED CHAIN
      INTEGER IADRX
C WNXFU....WIRE NUMBER  -NEXT-FOUND    -USER
      INTEGER WNXFU
C ILYRA....LAYER NUMBER OF ADJACENT HIT
      INTEGER ILYRA
C WNAU.....WIRE NUMBER  -NOW -ADJACENT -USER
      INTEGER WNAU
C IFCOMP...WIRE NUMBER COMPARATOR, IN VELOCITY
      INTEGER IFCOMP
C WSTRNG...TOTAL WIDTH OF STRING, COMPARE TO OKWSTR
      INTEGER WSTRNG
C WSTRP....WIDTH OF STRING TO PLUS  SIDE
      INTEGER WSTRP
C WSTRM....WIDTH OF STRING TO MINUS SIDE
      INTEGER WSTRM
C ADTSSF...INPUT ADDRESS OF THE "TESTED" WIRE,            SSF INDEXING
      INTEGER ADTSSF
C ISIGNP...SIGN OF THE SIDE THAT IS BEING CHECKED
      INTEGER ISIGNP
C ADPSSF...LOCAL PROCEDURE WIRE ADDRESS,            SSF INDEXING
      INTEGER ADPSSF
C ADPSF....LOCAL PROCEDURE WIRE ADDRESS,            SF INDEXING
      INTEGER ADPSF
C ADRHT....ADDRESS OF TESTED HIT WIRE, IN THE SHORT HIT LIST
      INTEGER ADRHT

C================================================================
C================================================================
C
C   -----------  Executable code starts here  ---------------
C
C================================================================
C================================================================

C================================================================
C PROCEDURE TO PROCESS-PREDETERMINED-HITS
C================================================================

C input SF address; calculate layer number
C this is the start of an established chain

      IF(CALC_ILYRT)THEN
        ILYRT=LAYSSF(SFLSFA(ADTSF))
        LSFTMP=SFLSFA(ADTSF)
        IF(CDLASF(LSFTMP).LT.0)THEN
          IF(SFSVWE.EQ.2)THEN
            WNTU=WIRSSF(ADTSF-SFSVCL(-CDLASF(LSFTMP)))   
          ELSE
            WNTU=WIRSSF(ADTSF)   
          ENDIF
        ELSE
          WNTU=WIRSSF(ADTSF)   
        ENDIF

C input layer number, compare with layer number of next hit in the chain
C and fill the SF adress
C set the SF adress to =0 if the layer number is not the next in the list
        
      ELSE
        IF(ILYRT.EQ.ILYRX)THEN
          ADTSF=IADRX
          WNTU=WNXFU
        ELSE
          ADTSF=0
        ENDIF
      ENDIF
      
C continue for valid hits

      IF(ADTSF.NE.0)THEN
        IHQLER=0
        USE_AJUSOK=0
        WSHFX=0
        LADR=LADSSF(ILYRT)
        MADR=MADSSF(ILYRT)

        IF(INUCHN(ADTSF).NE.HTUCHN(ICHAIN))THEN
          SFCERR=24
          GO TO 991
        ENDIF
        
        ADASF=DOUBPT(ADTSF)
        IF(ADASF.GT.0)THEN
          IHQLER=1
          
          IF(INUCHN(ADASF).NE.HTUCHN(ICHAIN))THEN
            SFCERR=25
            GO TO 991
          ENDIF
          
          ILYRA=LAYSSF(SFLSFA(ADASF))
          LSFTMP=SFLSFA(ADASF)
          IF(CDLASF(LSFTMP).LT.0)THEN
            IF(SFSVWE.EQ.2)THEN
              WNAU=WIRSSF(ADASF-SFSVCL(-CDLASF(LSFTMP)))   
            ELSE
              WNAU=WIRSSF(ADASF)   
            ENDIF
          ELSE
            WNAU=WIRSSF(ADASF)   
          ENDIF
          
          IF(ILYRA.NE.ILYRT)THEN
            SFCERR=27
            GO TO 991
          ENDIF
         
          WSHFX=WNAU-WNTU
          IFCOMP=NWRSSF(ILYRT)/2
          IF(WSHFX.GT. IFCOMP)WSHFX=WSHFX-NWRSSF(ILYRT)
          IF(WSHFX.LE.-IFCOMP)WSHFX=WSHFX+NWRSSF(ILYRT)
          
          IF((WSHFX.NE.-1).AND.(WSHFX.NE. 1))THEN
            SFCERR=28
            GO TO 991
          ENDIF
          
          WSHFX=WSHFX*MULSTH
        ENDIF
        
C CALCULATE WSTRM AND WSTRP AS IN SFCHAN_QUAL
        ADTSSF=LADR+WNTU
        FIRST_SEARCH_DIR=-1
        OKWSTR=10
        
        ISIGNP=FIRST_SEARCH_DIR
        WSTRNG=1
        WSTRM=0
        WSTRP=0
        
 835    ADPSSF=ADTSSF
 837    ADPSSF=ADPSSF+ISIGNP
        
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
          IF(
     1         (INUCHN(ADPSF).EQ.HTUCHN(ICHAIN)).OR.
     2         (INUCHN(ADPSF)             .EQ.0)
     3         )THEN
            IF(WSTRNG.GE.OKWSTR)GO TO 861
            
            WSTRNG=WSTRNG+1
            
            IF(ISIGNP.EQ.1)THEN
              WSTRP=WSTRP+1
            ELSE
              WSTRM=WSTRM+1
            ENDIF
            
            GO TO 837
          ENDIF
        ENDIF
        
        IF(ISIGNP.EQ.FIRST_SEARCH_DIR)THEN
          ISIGNP=-FIRST_SEARCH_DIR
          GO TO 835
        ENDIF
 861    CONTINUE
        
        IF(IHQLER.NE.1)THEN
C THIS WAS FOUND TO CAUSE ERROR: 10/2/91
C ALLOW STRINGS THAT ARE NOT PREVIOUS IDENTIFIED DOUBLET TO PASS AS IHQLER=0
CCC      IF(WSTRNG.NE.1)IHQLER=2
        ENDIF

        WNFX=WNTU*MULST+WSHFX
        WNFXL=(WNTU-WSTRM)*MULST-MULSTH
        WNFXH=(WNTU+WSTRP)*MULST+MULSTH
        
        STOPBK=STOPBK-1
        IF(STOPBK.GE.0)THEN
          IADRX=ISTFBD(ADTSF+CFOR)
          
          IF(IADRX.EQ.0)THEN
            SFCERR=29
            GO TO 991
          ENDIF
          
          ILYRX=LAYSSF(SFLSFA(IADRX))
          LSFTMP=SFLSFA(IADRX)
          IF(CDLASF(LSFTMP).LT.0)THEN
            IF(SFSVWE.EQ.2)THEN
              WNXFU=WIRSSF(IADRX-SFSVCL(-CDLASF(LSFTMP)))   
            ELSE
              WNXFU=WIRSSF(IADRX)
            ENDIF
          ELSE
            WNXFU=WIRSSF(IADRX)
          ENDIF
        ENDIF
      ENDIF
      
      PROC_BACKTRACK=1

 991  CONTINUE
      RETURN
      END




