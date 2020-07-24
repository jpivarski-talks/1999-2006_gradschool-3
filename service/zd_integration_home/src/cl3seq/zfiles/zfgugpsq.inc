*
* $Id: zfgugpsq.inc,v 1.1 1998/05/07 20:39:42 pg Exp $
*
* $Log: zfgugpsq.inc,v $
* Revision 1.1  1998/05/07 20:39:42  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.1.1.1  1994/10/04 20:58:43  zfiles
* first version of seq in CVS
*
*
*
* zfgugpsq.inc
*
*************************** begin sequence ZFGUGPSQ ************************
*
* This sequence must be included when using the sequence ZFGUGP in
* user code. It contains the variable declarations necessary for that
* sequence. The user is responsible for initializing CALDZF to .FALSE.,
* initializing AND declaring INTEGER PARAMETER MLSTZF, and initializing
* and all other variables to appropriate values.
*************************************************************************
*
* User initialized inputs:
*
*      MLSTZF : number of ZFILES entities used for array lengths (>= NLSTZF)
*               (parameter initialized and declared by USER)
*
*      CALDZF : logical array telling whether first time called, init by USER
*
*      INDFZF : first index in all arrays to start usage filled by USER
*               ( 1 for normal usage )
*
*      TOPDZF : CHAR*16 array stating the top directory, filled by USER
*               ( if INDFZF = 1, can initialize to +SEQ,CLFILE variable
*                 with EQUIVALENCE; else must assign in code because
*                 top directories are in COMMON )
*
*      NLSTZF : array of actual # ZFILES entities requested by USER
*
*      MWRDZF : array of number of expected words filled by USER
*               ( used for checking actual # words; if user wants to
*                 disable checking set this number to 0, usually for
*                 entities which have unknown # words )
*
*      PATHZF : array (CHAR*80) path names filled by USER
*
*      RWMOZF : array (CHAR*2) of read/write modes for ZFGUID filled by USER
*
*      ACFRZF : array (CHAR*2) access frequencies for ZFGUID filled by USER
*
*      UKEYZF : user superkeys (INT) for each entity for ZFGUID filled by USER
*
*      CMNTZF : comment (CHAR*80) for retrieval of each entity filled by USER
*               (this is an array, though commonly only one element is used )
*
*************************************************************************
*
* Output for user:
*
*      IPLSZF : array of pointers to pointers returned from ZFGPTR
*
*-------------------------
*      IDLSZF : id list sent to ZFGPTR by user routine, filled by ZFGUID
*               ( usually not needed by user )
*
*      NWRDZF : array of number of actual words filled by ZFGPTR
*               ( usually not needed by user )
*
*************************************************************************
*
*      LOOPZF : loop index
*
*      NEWCZF : logical flag used in sequence
*
*      IOERZF : ioerr flag used in sequence
*
*************************************************************************
      INTEGER INDFZF, LOOPZF, IOERZF

      INTEGER NLSTZF( MLSTZF ), UKEYZF( MLSTZF ), IPLSZF( MLSTZF ),
     *        MWRDZF( MLSTZF ), NWRDZF( MLSTZF ), IDLSZF( MLSTZF )

      CHARACTER*80 PATHZF( MLSTZF ), CMNTZF( MLSTZF )
      CHARACTER*16 TOPDZF( MLSTZF )
      CHARACTER*2  RWMOZF( MLSTZF ), ACFRZF( MLSTZF )

      LOGICAL CALDZF( MLSTZF ), NEWCZF

      SAVE NLSTZF, MWRDZF, NWRDZF, IDLSZF, PATHZF, IPLSZF, CMNTZF,
     *     UKEYZF, CALDZF, TOPDZF, RWMOZF, ACFRZF
*************************** end sequence ZFGUGPSQ ************************

