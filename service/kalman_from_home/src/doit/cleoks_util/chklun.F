*
* $Id: chklun.F,v 1.2 1999/11/29 21:43:07 lyon Exp $
*
* $Log: chklun.F,v $
* Revision 1.2  1999/11/29 21:43:07  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1  1999/05/28 03:10:25  lyon
* Needed cleoks routines
*
* Revision 1.1.1.1  1995/08/01 17:59:53  nk
* Consolidation
*
* Revision 1.1.1.1  1994/10/04 21:54:51  zfiles
* first version of clutil in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  1.00/00 17/11/88  15.21.08  by  R. Namjoshi
*-- Author : R. Namjoshi
      SUBROUTINE CHKLUN( CRNAME )
C.......................................................................
C.
C. CHKLUN - Check integrity of LUN allocation table
C.
C. Inputs    : CRNAME - Character name of owner of lock (GETLUN/FRELUN)
C. Outputs   : None
C.
C. COMMON    : LUNMCI LUNMCI
C. Banks D   : None
C. Banks L   : None
C. Banks M   : None
C. Banks U   : None
C.
C. Calls     : ERRLUN
C. Called    : FRELUN GETLUN
C.
C. Detailed description
C.  This routine checks the integrity of the LUN allocation table. For
C.  each allowed unit number, an INQUIRE is performed to find out if
C.  an OPEN file is connected to the unit. If that is the case but the
C.  allocation table indicates that the unit is free, then the unit
C.  is locked by calling LOKLUN and an error message is logged.
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- Argument declarations ---------------
      CHARACTER*(*) CRNAME
* -------------------- EXTERNAL declarations ---------------
* None
* -------------------- SEQUENCE declarations ---------------
#include "doit/utilseq/lunmcde.inc"
* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: chklun.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.2 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 1999/11/29 21:43:07 $')
      PARAMETER(CHRCSA='$Author: lyon $')
      INTEGER I
      LOGICAL LOPEN
* -------------------- SAVE     declarations ---------------
      SAVE RCSINI
* None
* -------------------- DATA  initializations ---------------
      DATA RCSINI/.TRUE./
* None
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
C== If a unit is OPEN but table indicates it is free then complain
C
      DO 10 I = LUNMIN, LUNMAX
         INQUIRE( UNIT=I, OPENED=LOPEN )
         IF( LOPEN .AND. LUNTBL(I).EQ.LISFRE ) THEN
            CALL LOKLUN( I, 'LUNMGR' )
            CALL ERRLUN( CRNAME, I, LUALER, ' ' )
         ENDIF
   10 CONTINUE
C
      RETURN
      END
