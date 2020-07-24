*
* $Id: fdivid.F,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: fdivid.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:44  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  14.40.53  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  13.17.04  by  Steve Schaffner
*CMZ :  5.50/00 24/10/88  13.14.43  by  Dan Riley
*CMZ :          20/09/88  14.47.49  by  Dan Riley
* Fix GETLUN calling sequence.
*-- Author :
      SUBROUTINE FDIVID(FILIN)
C
C     Read a double width lineprinter file and
C     divide into two single width files.
C
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      INTEGER LRECUT,LRECIN,IUNIN,IUNUT1,IUNUT2,LENOCC,INDREC,IRECUT
      INTEGER IRECIN
      PARAMETER (LRECUT=132)
      PARAMETER (LRECIN=2*LRECUT-1)
      CHARACTER*263 LINE
      CHARACTER*(*) FILIN
      CHARACTER*80  FILUT1,FILUT2

C
      DATA IUNUT1/1/,IUNUT2/2/,IUNIN/3/

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      CALL GETLUN(IUNIN, 'FDIVID')
      CALL GETLUN(IUNUT1, 'FDIVID')
      CALL GETLUN(IUNUT2, 'FDIVID')
      FILUT1 = FILIN(1:LENOCC(FILIN))//'1'
      FILUT2 = FILIN(1:LENOCC(FILIN))//'2'
C
      OPEN(UNIT=IUNIN ,FILE=FILIN ,STATUS='OLD')
      OPEN(UNIT=IUNUT1,FILE=FILUT1,STATUS='NEW',FORM='FORMATTED')
      OPEN(UNIT=IUNUT2,FILE=FILUT2,STATUS='NEW',FORM='FORMATTED')
C
10    CONTINUE
      READ(IUNIN,110,END=20) LINE
      INDREC = LENOCC(LINE)
      IRECUT = MAX0(1,       MIN0(LRECUT,INDREC))
      IRECIN = MAX0(LRECUT+1,MIN0(LRECIN,INDREC))
      WRITE(IUNUT1,120)           LINE(1       :IRECUT)
      WRITE(IUNUT2,121)LINE(1:1)//LINE(LRECUT+1:IRECIN)
      GOTO 10
C
20    CONTINUE
      CLOSE(UNIT=IUNIN,STATUS='DELETE')
      CLOSE(UNIT=IUNUT1)
      CLOSE(UNIT=IUNUT2)
      CALL FRELUN(IUNIN, 'FDIVID')
      CALL FRELUN(IUNUT1, 'FDIVID')
      CALL FRELUN(IUNUT2, 'FDIVID')
      RETURN
C
110   FORMAT(A)
120   FORMAT(A)
121   FORMAT(1X,A)
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
