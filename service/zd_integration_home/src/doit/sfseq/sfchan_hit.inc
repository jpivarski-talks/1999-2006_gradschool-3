*
* $Id: sfchan_hit.inc,v 1.2 2002/09/24 00:34:06 dpp Exp $
*
* $Log: sfchan_hit.inc,v $
* Revision 1.2  2002/09/24 00:34:06  dpp
*      -> move remaining sfchan_hit_quality arguments to common
*
* Revision 1.1  2002/09/05 19:19:38  dpp
*      -> NEW COMMON
*      -> information about the current sfchan hit
*
* ------------------------------------------
* COMMON BLOCKS FOR SFCHAN CURRENT HIT
*
* sfchan_hit.inc
*
#if !defined(CLEO_NOCOMDOC)
* SFCHANCTL DOCUMENTATION

C /SFCHAN_HIT/......... SFCHAN CURRENT HIT

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
C                                   INUCHN=-1  HIGH TIME, >1/2 CELL
C                                   INUCHN=-2  HIGH PH (PEDISTAL)
C                                   INUCHN=-3  ALREADY USED IN DUET
C                                   INUCHN=-4  ADJACENT TO USED IN DUET
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
#endif

      INTEGER ILYRT
      INTEGER LADR
      INTEGER MADR
      INTEGER WNTU
      INTEGER ADTSF
      INTEGER ADASF
      INTEGER WNFX
      INTEGER WSHFX
      INTEGER WNFXL
      INTEGER WNFXH
      INTEGER IHQLER
      INTEGER FIRST_SEARCH_DIR
      INTEGER CROSS_WIDTH
      INTEGER OKWSTR
      INTEGER USE_AJUSOK 
      INTEGER PROC_BACKTRACK


      COMMON/SFCHAN_HIT/
     1 ILYRT,
     1 LADR,
     1 MADR,
     1 WNTU,
     1 ADTSF,
     1 ADASF,
     1 WNFX,
     1 WSHFX,
     1 WNFXL,
     1 WNFXH,
     1 IHQLER,
     1 FIRST_SEARCH_DIR,
     1 CROSS_WIDTH,
     1 OKWSTR,
     1 USE_AJUSOK, 
     1 PROC_BACKTRACK
* ------------------------------------------








