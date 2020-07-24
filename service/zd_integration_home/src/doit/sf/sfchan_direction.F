*
* $Id: sfchan_direction.F,v 1.1 2002/09/05 18:54:12 dpp Exp $
*
* $Log: sfchan_direction.F,v $
* Revision 1.1  2002/09/05 18:54:12  dpp
*      -> NEW ROUTINE
*      -> PROCEDURE TO SET-DIRECTION-ADDRESSES
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : DAN PETERSON

      SUBROUTINE SFCHAN_DIRECTION(FORWARDS)

C......................................................................
C.
C. SFCHAN_DIRECTION - set up control of direction for SFCHAN
C.
C. COMMON    : /STEPCn/
C. CALLS     :  
C. CALLED    : SFCHAN
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 30-MAY-02
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

C FORWARDS.....INPUT LOGICAL, if TRUE, set up for bacwards runnning
      LOGICAL FORWARDS

C-----------------------------------------------------------------------
C  LOCAL VARIABLE DOCUMENTION
C-----------------------------------------------------------------------

C================================================================
C================================================================
C
C   -----------  Executable code starts here  ---------------
C
C================================================================
C================================================================

C=======================================================================
C PROCEDURE TO SET-DIRECTION-ADDRESSES
C=======================================================================

      IF(FORWARDS)THEN
        IDIRSF=+1
        CFOR=0
        CLIN=0
        ENDLO=1

C BACKWARDS
      ELSE
        IDIRSF=-1
        CFOR=MHITSF
        CLIN=MCHAIN
        ENDLO=2
      ENDIF

      CBAK=MHITSF-CFOR
      CMIN=MCHAIN-CLIN
      ENDHI=3-ENDLO

      RETURN
      END








