*
* $Id: sfchan_scan_seed_hits.F,v 1.2 2002/09/24 00:33:16 dpp Exp $
*
* $Log: sfchan_scan_seed_hits.F,v $
* Revision 1.2  2002/09/24 00:33:16  dpp
*      -> move remaining sfchan_hit_quality arguments to common
*
* Revision 1.1  2002/09/05 18:54:47  dpp
*      -> NEW ROUTINE
*      -> loop over hits as posible new seed
*      -> calls new stripped down sfchan to process chain
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : DAN PETERSON

      SUBROUTINE SFCHAN_SCAN_SEED_HITS(INPUT1,INPUT2,SFCERR)

C......................................................................
C.
C. SFCHAN_SCAN_SEED_HITS - loop over hits oin a group to
C.                         look for new chains using SFCHAN
C.
C. COMMON    : /STEPCn/
C. CALLS     : SFCHAN_DIRECTION,SFCHAN_HIT_QUALITY,SFCHAN 
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

C LADRS....ADDRESS OF FIRST WIRE IN THE INPUT LAYER, LLYRS,   SSF INDEXING
      INTEGER LADRS
C MADRS....ADDRESS OF LAST WIRE IN THE INPUT LAYER, LLYRS,    SSF INDEXING
      INTEGER MADRS
C MWIRS....MAXIMUM WIRE NUMBER (NOT NUMBER OF WIRES) IN INPUT LAYER
      INTEGER MWIRS
C ISFSSF...NUMBER OF THE SF SUBLAYER IN THE INPUT SUPER LAYER
      INTEGER ISFSSF
C LYRSF....LAYER NUMBER IN SF     OF A WIRE IN AN SSF LAYER LLYRS
      INTEGER LYRSF
C ADRHN....ADDRESS OF KNOWN HIT WIRE, IN THE SHORT HIT LIST
      INTEGER ADRHN
C WIRNUM...WIRE NUMBER IN CDGEOM, OR IN SF, BUT NOT SSF
      INTEGER WIRNUM
C HAVE_ROOM.=.TRUE. IF THERE IS ROOM FOR MORE CHAINS
      LOGICAL HAVE_ROOM 
C ABORT....ABORT FLAG RETURNED FROM DSF_ABORT, =1 for abort
      INTEGER ABORT

C================================================================
C================================================================
C
C   -----------  Executable code starts here  ---------------
C
C================================================================
C================================================================

      ABORT=0
      SFCERR=0
      SFCTYP=1

C-----------------------------------------------------------------------
C  load up the address of the hit map in ZFILES
C  careful, it changes between calls, it should be
C  reset after any other ZFILES calls, even DSFHIT
C  (which may call ZFILES in a new version)
C-----------------------------------------------------------------------

      sf_data_input_flag=1
#define SF_DATA_MAP_START
#include "doit/sf_data_access/sf_data_access.inc"

C================================================================
C  start of normal search for new chains
C================================================================

      ICHAIN=NCHAIN
      LLYRS=INPUT1
      MLYRS=INPUT2
      CALL SFCHAN_DIRECTION(MLYRS.GT.LLYRS)

      STOPBK=-1
      LADRS=LADSSF(LLYRS)
      MWIRS=NWRSSF(LLYRS)-1
      MADRS=MADSSF(LLYRS)

      HAVE_ROOM=(ICHAIN.LT.MCHAIN)

C-----------------------------------------------------------------------
C  this is to loop over all wires in a layer, LLYRS, as possible seeds
C  either method of choosing wires sends wire numbers to the common code
C  this is protected from "alternate readout" by the upper limit at MWIRS
C-----------------------------------------------------------------------

#if !defined(CLEO_SFLLIS)
      WNTU=0
11    CONTINUE
      IF(
     1     (ABORT.EQ.0).AND.
     2     (SFCERR.EQ.0).AND.
     3     (HAVE_ROOM).AND.
     4     (WNTU.LE.MWIRS))THEN
#endif

C-----------------------------------------------------------------------
C  link-list driven
C  this is loop over all real hits in layer, LLYRS
C  as opposed to the case of driving the program from a hit map only
C  either method of choosing wires sends wire numbers to the common code
C-----------------------------------------------------------------------

#if defined(CLEO_SFLLIS)
      ISFSSF=0
 21   ISFSSF=ISFSSF+1
      IF(
     1     (ABORT.EQ.0).AND.
     2     (SFCERR.EQ.0).AND.
     3     (HAVE_ROOM).AND.
     4     (ISFSSF.LE.MSFSSF))THEN
        LYRSF=LAYSF(ISFSSF,LLYRS)
        IF(LYRSF.NE.0)THEN

          sf_data_input_layr_sf=LYRSF
#define SF_DATA_1STH
#include "doit/sf_data_access/sf_data_access.inc"
          ADRHN=sf_data_output_hit_adr

 31       CONTINUE
          IF(
     1         (ABORT.EQ.0).AND.
     2         (SFCERR.EQ.0).AND.
     3         (HAVE_ROOM).AND.
     4         (ADRHN.NE.0))THEN

            sf_data_input_hit_adr=ADRHN
#define SF_DATA_WIRN
#include "doit/sf_data_access/sf_data_access.inc"
            WIRNUM=sf_data_output_wire

C select VD PRIMARY readout, and select requested SVX readout
            IF(
     1           (WIRNUM.GE.0           ).AND.
     2           (WIRNUM.LT.NWRSF(LYRSF)))THEN
              WNTU=WIRSSF(INSFSF(LYRSF)+WIRNUM)
#endif

C-----------------------------------------------------------------------
C  common code
C-----------------------------------------------------------------------

              ILYRT=LLYRS
              LADR=LADRS
              MADR=MADRS

              CROSS_WIDTH=0
              FIRST_SEARCH_DIR=-1
              
              CALL SFCHAN_HIT_QUALITY
              IF(
     1             (IHQLER.EQ.0).OR.
     2             (IHQLER.EQ.1))THEN
                CALL SFCHAN(.TRUE.,SFCERR)
                HAVE_ROOM=(ICHAIN.LT.MCHAIN)

C-----------------------------------------------------------------------
C  allow abort of event
C-----------------------------------------------------------------------

#if defined(CLEO_SFDIAG)
                CALL DSF_ABORT(ABORT)
#endif
                      
C-----------------------------------------------------------------------
C  record failed seeds
C-----------------------------------------------------------------------

              ELSE

#if defined(CLEO_SFDIAG)
                CALL DSFHFS(ADTSF,LLYRS,WNTU,IHQLER)
#endif
              ENDIF

C-----------------------------------------------------------------------
C  link-list driven; get next real hit in layer
C-----------------------------------------------------------------------

c valid WIRNUM
            ENDIF

#if defined(CLEO_SFLLIS)

            sf_data_input_hit_adr=ADRHN
#define SF_DATA_NXTH
#include "doit/sf_data_access/sf_data_access.inc"
            ADRHN=sf_data_output_hit_adr

            GO TO 31
C valid ADRHN
          ENDIF
          GO TO 21
C valid lyrsf
        ENDIF
#endif


C-----------------------------------------------------------------------
C  not link-list driven; increment wire number
C-----------------------------------------------------------------------

#if !defined(CLEO_SFLLIS)
        WNTU=WNTU+1
        GO TO 11
#endif

C-----------------------------------------------------------------------
C  common code; return of loop and normal exit
C-----------------------------------------------------------------------

C valid isfssf for link list driven
C valid WNTU for not link list driven
      ENDIF

      NCHAIN=ICHAIN

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
 1001    FORMAT(' SFCHAN_SCAN_SEED_HITS: EXITING;',
     2        ' ERROR=',I2,'  ENTRY TYPE',I2,' CHAIN#',I3)
#endif
      ENDIF

C-----------------------------------------------------------------------
C exit
C-----------------------------------------------------------------------

      RETURN
      END








