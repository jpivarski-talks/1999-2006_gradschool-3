*
* $Id: tfptms.F,v 1.1.1.1 1998/02/06 19:11:41 dpp Exp $
*
* $Log: tfptms.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:45  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:58  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  13.51.40  by  Steve Schaffner
*CMZ :  5.53/04 14/06/90  21.11.18  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.23.03  by  Steve Schaffner
*CMZ :  5.50/00 26/02/88  00.28.34  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFPTMS(JTIMDG,JTIMCF,JTIMLF,TIMEDG,TIMECF,TIMELF)

C     ==================================================================
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"

#include "doit/duseq/tftime.inc"
#include "doit/duseq/tfunitcd.inc"

      INTEGER NCALDG,JTIMDG,NCALCF,JTIMCF,NCALLF,JTIMLF
      REAL TCALDG,TCALCF,TCALLF,PERCDG,PERCCF,PERCLF,TIMEDG,TIMECF
      REAL TIMELF

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      NCALDG = NCALLS(JTIMDG)
      NCALCF = NCALLS(JTIMCF)
      NCALLF = NCALLS(JTIMLF)

      TCALDG = ITIMES(JTIMDG)
      TCALCF = ITIMES(JTIMCF)
      TCALLF = ITIMES(JTIMLF)

C-->  CALCULATE WHAT % OF THE TIME IS SPENT IN THIS SUBROUTINE

      PERCDG = 0.0
      PERCCF = 0.0
      PERCLF = 0.0

      IF (TIMEDG .GT. 0.0) PERCDG = 100.0*TCALDG / TIMEDG
      IF (TIMECF .GT. 0.0) PERCCF = 100.0*TCALCF / TIMECF
      IF (TIMELF .GT. 0.0) PERCLF = 100.0*TCALLF / TIMELF

C-->  CALCULATE THE TIME/CALL IN MSEC

      IF (NCALDG .GT. 0  ) TCALDG =       TCALDG / FLOAT(NCALDG)
      IF (NCALCF .GT. 0  ) TCALCF =       TCALCF / FLOAT(NCALCF)
      IF (NCALLF .GT. 0  ) TCALLF =       TCALLF / FLOAT(NCALLF)

      IF (TIMELF .GE. 0.0) GO TO 20

      IF (TIMECF .GE. 0.0) GO TO 10

      WRITE (IUNLPT,105) SUBTTL(JTIMDG),NCALDG,TCALDG,PERCDG

      RETURN

   10 CONTINUE

      WRITE (IUNLPT,105) SUBTTL(JTIMDG),NCALDG,TCALDG,PERCDG,
     +                   SUBTTL(JTIMCF),NCALCF,TCALCF,PERCCF

      RETURN

   20 CONTINUE

      WRITE (IUNLPT,105) SUBTTL(JTIMDG),NCALDG,TCALDG,PERCDG,
     +                   SUBTTL(JTIMCF),NCALCF,TCALCF,PERCCF,
     +                   SUBTTL(JTIMLF),NCALLF,TCALLF,PERCLF

      RETURN

  105 FORMAT(' ',3(7X,A6,I11,F11.1,F8.1))

      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
