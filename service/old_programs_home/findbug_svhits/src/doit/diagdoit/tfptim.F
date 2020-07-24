*
* $Id: tfptim.F,v 1.1.1.1 1998/02/06 19:11:41 dpp Exp $
*
* $Log: tfptim.F,v $
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
*CMZ :  5.53/04 14/06/90  21.11.17  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.37.14  by  Steve Schaffner
*CMZ :  5.50/00 26/02/88  00.28.33  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFPTIM

C-->  PRINTS A SUMMARY OF ERRORS AT THE END OF THE ANALYSIS

C     ==================================================================
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"

#include "doit/duseq/tftime.inc"
#include "doit/duseq/tfunitcd.inc"

      REAL TIMETL,TIMEDU,TIMETF,TIMEDG,TIMECF,TIMELF,TSECTF,TSECDG
      REAL TSECCF,TSECLF,TTRKTF,TTRKDG,TTRKCF,TTRKLF,TCALDG,TCALCF
      REAL TCALLF
      INTEGER NCALTF,NCALDG,NCALCF,NCALLF
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

C     INCREASE CFDRDG, CFIND & LFIND TIMES TO INCLUDE REST OF TIME

      ITIMES(KTIMDG) = ITIMES(KTIMDG)
     +                +ITIMES(KTIMDG+ITHITS)+ITIMES(KTIMDG+ITCORR)
     +                +ITIMES(KTIMDG+ITRFIT)+ITIMES(KTIMDG+ITHIST)
      ITIMES(KTIMCF) = ITIMES(KTIMCF)
     +                +ITIMES(KTIMCF+ITHITS)+ITIMES(KTIMCF+ITCORR)
     +                +ITIMES(KTIMCF+ITRFIT)+ITIMES(KTIMCF+ITHIST)
      ITIMES(KTIMLF) = ITIMES(KTIMLF)
     +                +ITIMES(KTIMLF+ITHITS)+ITIMES(KTIMLF+ITCORR)
     +                +ITIMES(KTIMLF+ITRFIT)+ITIMES(KTIMLF+ITHIST)

      ITIMES(KTIMDG+ITFIND) = ITIMES(KTIMDG)
      ITIMES(KTIMCF+ITFIND) = ITIMES(KTIMCF)
      ITIMES(KTIMLF+ITFIND) = ITIMES(KTIMLF)

C     PRINT OVERALL DUET STATISTICS

      NCALLS(ITTOTL) = NCALLS(ITDUET)
C                                         ! TOTAL CALLS = # TRACKS PUT

C      ITIMES(ITDUET) = ITIMES(ITDUE3) - ITIMES(ITGT20) - ITIMES(ITCDGP)
      ITIMES(ITDUET) = ITIMES(ITDUE3)

      TIMETL = ITIMES(ITTOTL)

      WRITE (IUNLPT,100)
      WRITE (IUNLPT,101)

      WRITE (IUNLPT,105)
      CALL TFPTMS(ITTOTL, 1  , 1,
     +            TIMETL,-1.0,-1.0)

      WRITE (IUNLPT,105)
C      CALL TFPTMS(ITIZA2,ITIZA3, 1,
C     +            TIMETL,TIMETL,-1.0)
      CALL TFPTMS(ITDUE2,ITDUE3,ITDUET,
     +            TIMETL,TIMETL,TIMETL)
C      CALL TFPTMS(ITGT20,ITCDGP, 1,
C     +            TIMETL,TIMETL,-1.0)

      TIMEDU = ITIMES(ITDUET)

      WRITE (IUNLPT,105)
      CALL TFPTMS(ITDUET, 1  , 1,
     +            TIMEDU,-1.0,-1.0)

      WRITE (IUNLPT,105)
      CALL TFPTMS(ITDRHW, 1 , 1,
     +            TIMEDU,-1.0,-1.0)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG,KTIMCF,KTIMLF,
     +            TIMEDU,TIMEDU,TIMEDU)

      WRITE (IUNLPT,105)
      CALL TFPTMS(ITTFCR,IT3FIT,ITCDHS,
     +            TIMEDU,TIMEDU,TIMEDU)

      WRITE (IUNLPT,105)
      CALL TFPTMS(ITCDFT, 1  , 1,
     +            TIMEDU,-1.0,-1.0)
      CALL TFPTMS(ITCDNT, 1  , 1,
     +            TIMEDU,-1.0,-1.0)
      CALL TFPTMS(ITDKFN, 1  , 1,
     +            TIMEDU,-1.0,-1.0)
      CALL TFPTMS(ITPHFN, 1  , 1,
     +            TIMEDU,-1.0,-1.0)
      CALL TFPTMS(ITCDST, 1  , 1,
     +            TIMEDU,-1.0,-1.0)

C     CFIND & LFIND TIMING FOR EACH TRACK TRIED :

      NCALTF = NCALLS(ITTFND)
      NCALDG = NCALLS(KTIMDG)
      NCALCF = NCALLS(KTIMCF)
      NCALLF = NCALLS(KTIMLF)

      TIMETF = ITIMES(ITTFND)
      TIMEDG = ITIMES(KTIMDG)
      TIMECF = ITIMES(KTIMCF)
      TIMELF = ITIMES(KTIMLF)

      TSECTF = 0.001*TIMETF
      TSECDG = 0.001*TIMEDG
      TSECCF = 0.001*TIMECF
      TSECLF = 0.001*TIMELF

      IF (NCALTF .GT. 0) TTRKTF = TIMETF / FLOAT(NCALTF)
      IF (NCALDG .GT. 0) TTRKDG = TIMEDG / FLOAT(NCALDG)
      IF (NCALCF .GT. 0) TTRKCF = TIMECF / FLOAT(NCALCF)
      IF (NCALLF .GT. 0) TTRKLF = TIMELF / FLOAT(NCALLF)

      WRITE (IUNLPT,110) TSECDG,NCALDG,TTRKDG,
     +                   TSECCF,NCALCF,TTRKCF,TSECLF,NCALLF,TTRKLF

C     NOW FOR THE TIMMING FOR EACH CALL OF CFIND & LFIND :

      NCALDG = NCALLS(KTIMDG+ITFIND)
      NCALCF = NCALLS(KTIMCF+ITFIND)
      NCALLF = NCALLS(KTIMLF+ITFIND)

      TSECDG = 0.001*TIMEDG
      TSECCF = 0.001*TIMECF
      TSECLF = 0.001*TIMELF

      IF (NCALDG .GT. 0) TCALDG = TIMEDG / FLOAT(NCALDG)
      IF (NCALCF .GT. 0) TCALCF = TIMECF / FLOAT(NCALCF)
      IF (NCALLF .GT. 0) TCALLF = TIMELF / FLOAT(NCALLF)

      WRITE (IUNLPT,111) TSECDG,NCALDG,TCALDG,
     +                   TSECCF,NCALCF,TCALCF,TSECLF,NCALLF,TCALLF
      WRITE (IUNLPT,101)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITHITS,KTIMCF+ITHITS,KTIMLF+ITHITS,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITCORR,KTIMCF+ITCORR,KTIMLF+ITCORR,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITGETR,KTIMCF+ITGETR,KTIMLF+ITGETR,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITLINK,KTIMCF+ITLINK,KTIMLF+ITLINK,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITTREE,KTIMCF+ITTREE,KTIMLF+ITTREE,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITFRST,KTIMCF+ITFRST,KTIMLF+ITFRST,
     +            TIMEDG       ,TIMECF       ,TIMELF)
      CALL TFPTMS(KTIMDG+ITCLMB,KTIMCF+ITCLMB,KTIMLF+ITCLMB,
     +            TIMEDG       ,TIMECF       ,TIMELF)
      CALL TFPTMS(KTIMDG+ITNEWT,KTIMCF+ITNEWT,KTIMLF+ITNEWT,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITTFIT,KTIMCF+ITTFIT,KTIMLF+ITTFIT,
     +            TIMEDG       ,TIMECF       ,TIMELF)
      CALL TFPTMS(KTIMDG+ITFIT ,KTIMCF+ITFIT ,KTIMLF+ITFIT ,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITSORT,KTIMCF+ITSORT,KTIMLF+ITSORT,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITTCUT,KTIMCF+ITTCUT,KTIMLF+ITTCUT,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITRFIT,KTIMCF+ITRFIT,KTIMLF+ITRFIT,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      WRITE (IUNLPT,105)
      CALL TFPTMS(KTIMDG+ITHIST,KTIMCF+ITHIST,KTIMLF+ITHIST,
     +            TIMEDG       ,TIMECF       ,TIMELF)

      RETURN

  100 FORMAT( '1TFPSUM : PRINTS A SUMMARY OF DUET TIMING STATISTICS :'/)
  101 FORMAT( ' ',3('       CALLED    # CALLS  MSEC/CALL  % TIME'))
  110 FORMAT(/' ',  '   <--------------- CFDRDG --------------->',
     +              '   <--------------- CFIND  --------------->',
     +              '   <--------------- LFIND  --------------->'/
     +       /' ',3('      TOT-SEC    # CALLS  MSEC/CALL        ')/
     +       /' ',3(2X,F11.3,I11,F11.1,'  TRACKS'))
  111 FORMAT(/' ',3(2X,F11.3,I11,F11.1,'  CALLS ')/)
  105 FORMAT( ' ')

      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
