*
* $Id: sfchan_extend_chain.F,v 1.1 2002/09/05 18:55:28 dpp Exp $
*
* $Log: sfchan_extend_chain.F,v $
* Revision 1.1  2002/09/05 18:55:28  dpp
*      -> NEW ROUTINE
*      -> extend existing chain to larger/smaller radius
*      -> calls new stripped down sfchan to process chain
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : DAN PETERSON

      SUBROUTINE SFCHAN_EXTEND_CHAIN(INPUT1,INPUT2,SFCERR)

C......................................................................
C.
C. SFCHAN_EXTEND_CHAIN - extend pre-existing chains 
C.                       using a different cut set in SFCHAN
C.
C. COMMON    : /STEPCn/
C. CALLS     : SFCHAN 
C. CALLED    : SFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 03-JUNE-02
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

C INPUT1...LAYER TO START FOR SFCTYP=1; CHAIN# FOR SFCTYP=2
C INPUT2...LAYER TO STOP  FOR SFCTYP=1; BACKTR (SIGNED) FOR SFCTYP=2
C SFCERR...RETURNED ERROR, =0 FOR NO ERROR

      INTEGER SFCTYP_INP,INPUT1,INPUT2,SFCERR

C-----------------------------------------------------------------------
C  LOCAL VARIABLE DOCUMENTION
C-----------------------------------------------------------------------

C BACKTR............NUMBER OF ELEMENTS TO BACKTRACK
      INTEGER BACKTR
C LVL_COMPAR........SAVED VALUE OF LVLCHN TO COMPAR NEW VALUES
      INTEGER LVL_COMPAR

C================================================================
C================================================================
C
C   -----------  Executable code starts here  ---------------
C
C================================================================
C================================================================

      SFCERR=0
      SFCTYP=2

C-----------------------------------------------------------------------
C  load up the address of the hit map in ZFILES
C  careful, it changes between calls, it should be
C  reset after any other ZFILES calls, even DSFHIT
C  (which may call ZFILES in a new version)
C-----------------------------------------------------------------------

      sf_data_input_flag=1
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

C=======================================================================
C  extend pre-existing chain
C=======================================================================

      ICHAIN=INPUT1
      IF((ICHAIN.LT.1).OR.(ICHAIN.GT.NCHAIN))SFCERR=21
      IF(SFCERR.EQ.0)THEN

        BACKTR=INPUT2
        CALL SFCHAN_DIRECTION(BACKTR.LE.0)
        
        IF(BACKTR.LE.0)THEN
          BACKTR=-BACKTR
          MLYRS=NLASSF
        ELSE
          MLYRS=1
        ENDIF

C-----------------------------------------------------------------------
C  backtrack to some element in the chain
C-----------------------------------------------------------------------

        LVL_COMPAR=LVLCHN(ENDHI,ICHAIN)
        LVL_COMPAR_A=LVL_COMPAR
        IF(LVL_COMPAR_A.LT.0)LVL_COMPAR_A=-LVL_COMPAR_A
        LVL_RESTOR=LVL_COMPAR
        LEN_RESTOR=LENCHN(ICHAIN)
        
        IF(BACKTR.GE.LENCHN(ICHAIN))BACKTR=LENCHN(ICHAIN)-1
        STOPBK=BACKTR
c alternative code
c       IF(BACKTR.GT.LENCHN(ICHAIN))BACKTR=LENCHN(ICHAIN)
c       STOPBK=BACKTR-1
c end alternative

        ADTSF=LMICHN(ICHAIN+CMIN)
 36     CONTINUE
        IF(
     1       (SFCERR.EQ.0).AND.
     2       (STOPBK.GT.0))THEN
          STOPBK=STOPBK-1
          ADTSF=ISTFBD(ADTSF+CBAK)
          IF(ADTSF.EQ.0)SFCERR=23
          GO TO 36
        ENDIF

      ENDIF

      IF(SFCERR.EQ.0)THEN
        STOPBK=BACKTR
        ILYRT=0
        CALL SFCHAN_PREDETERMINED(.TRUE.,SFCERR)
      ENDIF

      IF(SFCERR.EQ.0)THEN
        LLYRS=ILYRT
        CALL SFCHAN(.FALSE.,SFCERR)
      ENDIF

C-----------------------------------------------------------------------
C  end of the search loop; flag ADMAP0 as "stale" and return
C-----------------------------------------------------------------------

        sf_data_input_flag=0
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

C-----------------------------------------------------------------------
C print errors
C-----------------------------------------------------------------------

        IF(SFCERR.NE.0)THEN
#if defined(CLEO_SFDIAG)
          PRINT 1001,SFCERR,SFCTYP,ICHAIN
 1001     FORMAT(' SFCHAN_EXTEND_CHAIN: EXITING;',
     2         ' ERROR=',I2,'  ENTRY TYPE',I2,' CHAIN#',I3)
#endif
        ENDIF

C-----------------------------------------------------------------------
C exit
C-----------------------------------------------------------------------

        RETURN
        END











