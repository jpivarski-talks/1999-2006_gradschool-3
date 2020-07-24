*
* $Id: tfhwrt.F,v 1.1.1.1 1998/02/06 19:11:41 dpp Exp $
*
* $Log: tfhwrt.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:22  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:01  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  13.51.45  by  Steve Schaffner
*CMZ :  5.54/03 22/04/91  22.14.27  by  Steve Schaffner
*CMZ :  5.53/07 25/09/90  16.05.32  by  D. Riley
*CMZ :  5.51/03 27/09/89  11.55.39  by  Steve Schaffner
*CMZ :  5.51/00 18/08/89  15.38.10  by  Steve Schaffner
*CMZ :          28/06/89  12.58.07  by  Steve Schaffner
* Really truely switch to HBOOK4
*CMZ :          28/10/88  00.37.05  by  R. Namjoshi
* Switch to HBOOK4, use message logging
*CMZ :          24/10/88  13.13.48  by  Dan Riley
*CMZ :          20/09/88  14.46.59  by  Dan Riley
* Fix GETLUN calling sequence.
*-- Author :
      SUBROUTINE TFHWRT
C.......................................................................
C.
C. TFHWRT - Interface Duet histogramming to HBOOK
C.
C. COMMON    : ?
C. CALLS     : ?
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 20-Sep-88
C.
C. Modification Log.
C.
C. 20-Dec-87 : Book histograms only if LWHIST is .TRUE.      R. Namjoshi
C. 20-Sep-88 : Fix GETLUN calling sequence.                          dsr
C. 10-Apr-89 : Remove blank common def. (now handled by CLEVER)      sfs
C. 22-Apr-91 : Switch to HROPEN
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfunitcd.inc"
#include "seq/clutil/mesrep.inc"

      SAVE ISTAT

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFHWRT' )

      CHARACTER*(*) HSTOUT
      PARAMETER(    HSTOUT = 'DUETHST.RZN' )

C     ICYCLE -- dummy
      INTEGER ICYCLE,ILUN,ISTAT,IERR

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"
*---
      IF (.NOT.LWHIST) RETURN
      ICYCLE = 0
      ILUN = 20
      CALL GETLUN( ILUN, CRNAME )
**+SELF,IF=-DECS.
**      OPEN(UNIT=ILUN,ORGANIZATION='RELATIVE',ACCESS='DIRECT',RECL=1024,
**     +FILE=HSTOUT,FORM='UNFORMATTED',STATUS='NEW')
**+SELF,IF=DECS.
**      OPEN(UNIT=ILUN,ORGANIZATION='RELATIVE',ACCESS='DIRECT',RECL=1024,
**     +FILE=HSTOUT,FORM='UNFORMATTED',STATUS='UNKNOWN')
**+SELF.
**      CALL HRFILE(ILUN,'DUET','N')

      CALL HROPEN(ILUN,'DUET',HSTOUT,'N',1024,ISTAT)
      IF (ISTAT.NE.0) GOTO 99
C
C== Write out histograms, ASCII first
C
      CALL HOUTPU( IUNLPT )
      CALL HPAGSZ( 60, 1  )
      CALL HISTDO
      CALL HOUTPU( IUNTTW )

      CALL HROUT ( 0, ICYCLE, ' ' )
      CALL HREND('DUET')
      CLOSE (ILUN)
      CALL FRELUN( ILUN, CRNAME )
*
      RETURN

 99   WRITE (CHMESS,199)
199   FORMAT(' Error in HROPEN call')
      IERR = 1
      CALL MESLOG('CRNAME',IERR,MSERRO)
      RETURN

      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
