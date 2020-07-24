*
* $Id: inilun.F,v 1.2 1999/11/29 21:43:09 lyon Exp $
*
* $Log: inilun.F,v $
* Revision 1.2  1999/11/29 21:43:09  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1  1999/05/28 03:10:27  lyon
* Needed cleoks routines
*
* Revision 1.1.1.1  1995/08/01 17:59:54  nk
* Consolidation
*
* Revision 1.1.1.1  1994/10/04 21:54:51  zfiles
* first version of clutil in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  1.00/00 17/11/88  15.21.09  by  R. Namjoshi
*-- Author : R. Namjoshi
      SUBROUTINE INILUN
C.......................................................................
C.
C. INILUN - Initialize the LUN manager
C.
C. Inputs    : None
C. Outputs   : None
C.
C. COMMON    : LUNMCC LUNMCI
C. Banks D   : None
C. Banks L   : None
C. Banks M   : None
C. Banks U   : None
C.
C. Calls     : None
C. Called    : FRELUN GETLUN LOKLUN QRYLUN REPLUN <USER>
C.
C. Detailed description
C.  This routine initializes the LUN manager. It marks all units as
C.  being free and clears the owner table. It is called automatically
C.  by FRELUN GETLUN LOKLUN QRYLUN REPLUN. Subsequent calls to INILUN
C.  are ignored.
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- Argument declarations ---------------
* None
* -------------------- EXTERNAL declarations ---------------
* None
* -------------------- SEQUENCE declarations ---------------
#include "doit/utilseq/lunmcde.inc"
* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: inilun.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.2 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 1999/11/29 21:43:09 $')
      PARAMETER(CHRCSA='$Author: lyon $')
      LOGICAL LFIRST
      INTEGER I
* -------------------- SAVE     declarations ---------------
      SAVE RCSINI
      SAVE LFIRST
* -------------------- DATA  initializations ---------------
      DATA RCSINI/.TRUE./
      DATA LFIRST /.TRUE./
* ----------------- Executable code starts here ------------
#if WRITE_RCS_INFO
      IF(RCSINI)THEN
        RCSINI = .FALSE.
        WRITE(6,*)'-------- CVSinfo --------'
        WRITE(6,*)CHRCSS
        WRITE(6,*)CHRCSR//' '//CHRCST
        WRITE(6,*)CHRCSD//' '//CHRCSA
      ENDIF
#endif
C
      IF( LFIRST ) THEN
         LFIRST = .FALSE.
         DO 10 I = LUNMIN, LUNMAX
            LUNTBL(I) = LISFRE
            LUNOWN(I) = ' '
10       CONTINUE
      ENDIF
C
      RETURN
      END
