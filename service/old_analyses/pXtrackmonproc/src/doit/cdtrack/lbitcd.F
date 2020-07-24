*
* $Id: lbitcd.F,v 1.1 1998/02/24 18:59:31 dpp Exp $
*
* $Log: lbitcd.F,v $
* Revision 1.1  1998/02/24 18:59:31  dpp
* not entered 2/9/98
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      LOGICAL FUNCTION LBITCD( ITRK, JDEV, ILYR, INUT )
C.......................................................................
C.
C. LBITCD - Return status of given bit
C.
C. COMMON    : ?
C. CALLS     : IBITCD JBIT
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 01-Apr-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C     Returns .TRUE. if the bit corresponding to layer ILYR is set.
C     Arguments:
C        ITRK  track number in /ANLCDR/
C        JDEV  device number (in CD device definition ordering)
C        ILYR  layer number in device numbering system (1-N)
C        INUT  =1 for outward branch of track, 2 for inward

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/anltmp.inc"
#include "doit/duseq/tfbitcom.inc"

      INTEGER ITRK,ILYR,INUT,IBIT,IWORD,JDEV
      INTEGER  JBIT
      EXTERNAL JBIT

      LOGICAL LINIT
      DATA    LINIT/.TRUE./

* ----------Executable code starts here---------------------

      IF( LINIT ) THEN
          CALL IBITCD
          LINIT  = .FALSE.
      ENDIF

      LBITCD = .FALSE.
      IF( ITRK.LE.0 .OR. ITRK.GT.MTRKDR )       RETURN
      IF( JDEV.LE.0 .OR. JDEV.GT.MXDVCD )       RETURN
      IF( INUT.LE.0 .OR. INUT.GT.2 )            RETURN
      IF( ILYR.LE.0 .OR. ILYR.GT.NLYRDR(JDEV) ) RETURN
      IF (IADR(JDEV,INUT).EQ.-1)                 RETURN

      IBIT   = IADR(JDEV,INUT) + ILYR
      IWORD  = (IBIT-1)/32 + 1
      IBIT   = MOD(IBIT-1,32) + 1
      LBITCD = JBIT(KBITDR(ITRK,IWORD),IBIT).EQ.1

      RETURN
      END

