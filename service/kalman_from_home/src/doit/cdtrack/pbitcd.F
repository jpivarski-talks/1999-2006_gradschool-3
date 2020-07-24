*
* $Id: pbitcd.F,v 1.2 2001/11/19 22:46:22 dpp Exp $
*
* $Log: pbitcd.F,v $
* Revision 1.2  2001/11/19 22:46:22  dpp
*     -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:21  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  14.40.54  by  Steve Schaffner
*CMZ :  5.53/04 18/06/90  16.18.13  by  Steve Schaffner
*CMZ :  5.51/03 31/10/89  12.45.49  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  13.17.06  by  Steve Schaffner
*CMZ :  5.50/00 16/05/89  14.18.43  by  Steve Schaffner
*-- Author :
      SUBROUTINE PBITCD( ITRK, JDEV, ILYR, INUT )
C.......................................................................
C.
C. PBITCD - Set the specified bit
C.
C. COMMON    : ?
C. CALLS     : IBITCD
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

C     Sets bits corresponding to layers of tracking chamber with
C     a used hit.
C     Arguments:
C        ITRK  track number in /ANLCDR/
C        JDEV  device number (CD device definition ordering)
C        ILYR  layer number in device numbering system (1-N)
C        INUT  =1 for outward branch of track, 2 for inward

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/anltmp.inc"
#include "doit/duseq/tfbitcom.inc"
#include "doit/duseq/tfgeomcd.inc"

      INTEGER NLAYER(MXDVCD),IQUIT,ITRK,ILYR,INUT,IBIT
      INTEGER IWORD,NDEVIC,JDEV

      LOGICAL LINIT
      DATA    LINIT/.TRUE./

* ----------Executable code starts here---------------------

      IF( LINIT ) THEN
          CALL IBITCD

          LINIT     = .FALSE.
      ENDIF

      IF( ITRK.LE.0 .OR. ITRK.GT.MTRKDR )       RETURN
      IF( JDEV.LE.0 .OR. JDEV.GT.MXDVCD )       RETURN
      IF( INUT.LE.0 .OR. INUT.GT.2 )            RETURN
      IF( IADR(JDEV,INUT).EQ.-1)                RETURN
      IF( ILYR.LE.0 .OR. ILYR.GT.NLYRDR(JDEV) ) RETURN

      IBIT   = IADR(JDEV,INUT) + ILYR
      IWORD  = (IBIT-1)/32 + 1
      IBIT   = MOD(IBIT-1,32)
      KBITDR(ITRK,IWORD) = IBSET(KBITDR(ITRK,IWORD),IBIT)

      RETURN
      END





