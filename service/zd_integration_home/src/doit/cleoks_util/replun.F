*
* $Id: replun.F,v 1.2 1999/11/29 21:43:11 lyon Exp $
*
* $Log: replun.F,v $
* Revision 1.2  1999/11/29 21:43:11  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.1  1999/05/28 03:10:28  lyon
* Needed cleoks routines
*
* Revision 1.2  1995/08/02 13:29:48  zfiles
* Declare CRNAME
*
* Revision 1.1.1.1  1995/08/01  17:59:55  nk
* Consolidation
*
* Revision 1.1.1.1  1994/10/04 21:54:51  zfiles
* first version of clutil in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  1.00/41 06/12/91  14.19.49  by  B. Heltsley
* remove extraneous statement label
*CMZ :          07/12/88  16.52.13  by  R. Namjoshi
*-- Author : R. Namjoshi
      SUBROUTINE REPLUN
C.......................................................................
C.
C. REPLUN - Report current status of LUN table
C.
C. Inputs    : None
C. Outputs   : None
C.
C. COMMON    : LUNMCI LUNMCC MESREP
C. Banks D   : None
C. Banks L   : None
C. Banks M   : None
C. Banks U   : None
C.
C. Calls     : INILUN LENOCC MESLOG MESOUT NAMLUN
C. Called    : ERRLUN <USER>
C.
C. Detailed description
C.  This routine prints (via MESLOG) a table indicating the status of
C.  all valid unit numbers. The table shows the allocation status, the
C.  owner (if applicable) and the file connected to the unit if any.
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- Argument declarations ---------------
* None
* -------------------- EXTERNAL declarations ---------------
      INTEGER  LENOCC
      EXTERNAL LENOCC
* -------------------- SEQUENCE declarations ---------------
#include "doit/utilseq/lunmcde.inc"
#include "seq/clutil/mesrep.inc"
* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: replun.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.2 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 1999/11/29 21:43:11 $')
      PARAMETER(CHRCSA='$Author: lyon $')
      CHARACTER*(*) CRNAME
      PARAMETER(CRNAME='REPLUN')
      CHARACTER*80 FNAME
      LOGICAL LFIRST
      INTEGER I, NLEN
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
        CHMESS = '-------- CVSinfo --------'
        CALL MESLOG (CRNAME, 1, MSINFO)
        CHMESS = CHRCSS
        CALL MESLOG (CRNAME, 1, MSINFO)
        CHMESS = CHRCSR//' '//CHRCST
        CALL MESLOG (CRNAME, 1, MSINFO)
        CHMESS = CHRCSD//' '//CHRCSA
        CALL MESLOG (CRNAME, 1, MSINFO)
      ENDIF
#endif
*
      IF( LFIRST ) THEN
         LFIRST = .FALSE.
         CALL INILUN
      ENDIF
C
C== Loop over LUN table and report status
C
      WRITE(CHMESS,10000)
      CALL MESLOG( 'REPLUN', 1, MSINFO )
*
      WRITE(CHMESS,10010)
      CALL MESLOG(CHMESS,1,MSINFO)
      DO 10 I = LUNMIN, LUNMAX

         CALL NAMLUN( I, FNAME )
         NLEN = LENOCC(FNAME)

         IF(     LUNTBL(I).EQ.LISFRE ) THEN
            WRITE(CHMESS,10020) I
         ELSEIF( LUNTBL(I).EQ.LISUSE ) THEN
            WRITE(CHMESS,10030) I, LUNOWN(I), FNAME(1:NLEN)
         ELSEIF( LUNTBL(I).EQ.LISLOK ) THEN
            WRITE(CHMESS,10040) I, LUNOWN(I), FNAME(1:NLEN)
         ELSE
            WRITE(CHMESS,10050) I
         ENDIF

         CALL MESLOG(CHMESS,1,MSINFO)

   10 CONTINUE
C
      RETURN
*
10000 FORMAT(' Status of the LUN manager unit allocation table' )
10010 FORMAT(' LUN number',15X,' Status', 18X, 'File connection' )
10020 FORMAT(1X,I6,15X,'Free' )
10030 FORMAT(1X,I6,15X,'Allocated by ', A, 2X, A )
10040 FORMAT(1X,I6,15X,'Locked    by ', A, 2X, A )
10050 FORMAT(1X,I6,15X,'Table entry corrupted' )
*
      END
