*
* $Id: cdptrk.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: cdptrk.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:44  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:57  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/37 02/07/93  12.56.57  by  Rob Kutschke
*CMZ :  5.54/07 28/05/91  13.39.13  by  Steve Schaffner
*CMZ :  5.53/07 10/10/90  13.28.14  by  Steve Schaffner
*CMZ :  5.53/06 02/08/90  13.42.44  by  Steve Schaffner
*CMZ :  5.53/04 24/06/90  17.35.45  by  Brian Geiser
*CMZ :  5.53/03 19/04/90  13.58.54  by  D. Riley
*CMZ :          17/04/90  12.29.20  by  D. Riley
*CMZ :  5.51/00 17/08/89  11.30.23  by  Steve Schaffner
*CMZ :  5.50/00 23/03/89  15.12.47  by  Steve Schaffner
*-- Author :
      SUBROUTINE CDPTRK(TRKNAM)
C.......................................................................
C.
C. CDPTRK - CDPTRK prints the tracks in /CDTRAK/ found in DUET
C.
C. Inputs    :
C. Outputs   :
C.
C. COMMON    :
C.
C. Calls     : LBITCD
C. Called    :
C. Author    : D. Riley  18/04/90  21.46.54
C.
C. Detailed description
C.    AUTHOR   M. OGG      MARCH 1982
C.    Ugly brute force F77 translation, April 1990, D. Riley
C.......................................................................
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfunitcd.inc"
#include "doit/duseq/tfctlcde.inc"

      INTEGER JTRKP(MTRKCD),II
      INTEGER NCHAR,NDASH1,NDASH2,I,NBUILD,NTRK,NTRKP,ITRK,ITRKP,NHTOT
      INTEGER ILYR,MM,IP3DTF,LL,J

      CHARACTER*1   HITLYR(KLYRTF),HITYES,HITNO
      CHARACTER*(*) TRKNAM
      CHARACTER*8   SOLNAM,DUENAM
      CHARACTER*8   BUILD(10)
      CHARACTER*400 FOR101,FOR105,DUMMY
      CHARACTER*24  FOR136

      SAVE LINIT , SOLNAM, DUENAM, HITYES, HITNO
      SAVE FOR101, FOR105, FOR136

      LOGICAL LINIT,LBITCD
      DATA    LINIT/.TRUE./
      DATA SOLNAM , DUENAM /
     +    'TRIO  ','DUET  '/

      DATA HITYES,HITNO /
     +      '*' , '-' /

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      IF (LINIT) THEN
          LINIT  = .FALSE.
C
C-> CONSTRUCT FORMAT STATEMENT
          NBUILD = 0
          IF (NLYRVD.GT.0) THEN
              NBUILD = NBUILD + 1
              WRITE(BUILD(NBUILD),150) NLYRVD
          ENDIF
C          IF (NCANIZ.GT.0) THEN
C              NBUILD = NBUILD + 1
C              WRITE(BUILD(NBUILD),150) NCANIZ
C          ENDIF
          IF (NCAXDR.GT.0) THEN
              NBUILD = NBUILD + 1
              WRITE(BUILD(NBUILD),150) NCAXDR
          ENDIF
          IF (NCATTF.GT.0) THEN
              NBUILD = NBUILD + 1
              WRITE(BUILD(NBUILD),150) NCATTF
          ENDIF
          IF (NCSTDR.GT.0) THEN
              NBUILD = NBUILD + 1
              WRITE(BUILD(NBUILD),150) NCSTDR
          ENDIF
C
          NCHAR  = NLYRTF + NBUILD - 15
          NDASH1 = MAX0(1,NCHAR/2)
          NDASH2 = MAX0(1,NCHAR-NDASH1)
          WRITE(FOR101,101) NDASH1,NDASH2
C
          WRITE(DUMMY,105) NBUILD
          WRITE(FOR105,DUMMY) (BUILD(I),I=1,NBUILD)
          WRITE(FOR136,136) NP3DTF
      ENDIF
C
      IF (TRKNAM .EQ. DUENAM) THEN
          NTRK   = NTRKCD
      ELSE
          NTRK   = NTRKIN
      ENDIF
C
      WRITE (IUNDGN,100) TRKNAM,NRUN,NEVENT,NTRK
      IF (NTRK .LE. 0) RETURN
      NTRK   = MIN0(NTRK,MTRKCD)
C
C     FILL-JTRKP-WITH-INDEX-OF-TRACKS-TO-PRINT
C
      NTRKP  = 0
      DO 10 ITRK=1,NTRK
          NTRKP  = NTRKP + 1
          JTRKP(NTRKP)=ITRK
   10 CONTINUE
      IF (LTRKCD.LE.MTRKCD) THEN
          DO 15 ITRK=LTRKCD,MTRKCD
              NTRKP  = NTRKP + 1
              JTRKP(NTRKP)=ITRK
   15     CONTINUE
      ENDIF
C
C     PRINT-THE-TRACK-PARAMETERS
C
      WRITE(IUNDGN,FOR101)
      DO 30 ITRKP=1,NTRKP
          ITRK = JTRKP(ITRKP)
          NHTOT = 0
          IF (NLYRCF.GT.0) THEN
              DO 20 ILYR = LLYRCF,MLYRCF
                  IF(LBITCD(ITRKP,IDVCTF(ILYR),IXSFTF(ILYR),1) .OR.
     +               LBITCD(ITRKP,IDVCTF(ILYR),IXSFTF(ILYR),2) ) THEN
                      NHTOT  = NHTOT + 1
                      HITLYR(ILYR) = HITYES
                  ELSE
                      HITLYR(ILYR) = HITNO
                  ENDIF
   20         CONTINUE
          ENDIF
C
          IF (NLYRLF.GT.0) THEN
              DO 25 ILYR = LLYRLF,MLYRLF
                  IF(LBITCD(ITRKP,IDVCTF(ILYR),IXSFTF(ILYR),1) .OR.
     +               LBITCD(ITRKP,IDVCTF(ILYR),IXSFTF(ILYR),2) ) THEN
                      NHTOT  = NHTOT + 1
                      HITLYR(ILYR) = HITYES
                  ELSE
                      HITLYR(ILYR) = HITNO
                  ENDIF
   25         CONTINUE
          ENDIF
C
          WRITE (IUNDGN,FOR105)
     +     ITRK,INXTCD(ITRK),
     +          CURCD (ITRK),PHI0CD(ITRK),D0CD  (ITRK),CHI2CD(ITRK),
     +          NHTLVD(ITRK),NHTAIZ(ITRK),NHTADR(ITRK),
     +          TANDCD(ITRK),Z0CD  (ITRK),CHIZCD(ITRK),
     +          NHTSCT(ITRK),NHTSDR(ITRK),NHTOT,
     +          KINCD (ITRK),
     +          CHI3CD(ITRK),RESICD(ITRK),(HITLYR(II),II=1,NLYRTF)
   30 CONTINUE
C
C     PRINT-THE-TRACK-ERROR-MATRICES
C
      IF(TRKNAM.EQ.DUENAM) THEN
          WRITE (IUNDGN,130)
          DO 45 ITRKP=1,NTRKP
              ITRK = JTRKP(ITRKP)
              IF (CHI3CD(ITRK) .GE. 0.0) THEN
                  WRITE (IUNDGN,135) ITRK
C
                  MM     = 0
                  DO 40 IP3DTF=1,MP3DTF
                      LL     = MM + 1
                      MM     = MM + IP3DTF
                      WRITE(IUNDGN,FOR136) (ERRMCD(J,ITRK),J=LL,MM)
   40             CONTINUE
              ENDIF
C                             !END IF(CHI3CD
   45     CONTINUE
      ENDIF
      RETURN
C
  100 FORMAT(/' CDPTRK PRINTS THE CONTENTS OF /CDTRAK/ FOR ',A6,' :',
     +        ' RUN ',I6,' : EVENT ',I6,
     +                                  ' : NTRK ',I3)
  101 FORMAT('(/39X,''CFHITS'',26X,''LFHITS     HITS''/
     +  '' TRK NX   CURCD  PHI0CD    D0CD CHI2CD VD IZ DR'',
     +  ''  TANDCD    Z0CD CHIZCD CT DR ALL KN CHI3CD RESICD'',
     +  ''  <''',I3,'(''-''),'' HIT LAYERS ''',I3,'(''-''),''>'')')
  105 FORMAT('(''(I4,I3,3(2X,F6.3),1X,F6.2,3I3,',
     +   '2(2X,F6.3),1X,F6.2,2I3,I4,I3,1X,F6.2,1X,6P,F6.1,1X''',
     +   ',',I3,'A8,',''')'')')
  130 FORMAT(/' CDPTRK PRINTS THE ERROR MATRICES')
  135 FORMAT(/' TRK ',I3,10X,'CU',10X,'FI',10X,'DA',
     +                   10X,'CT',10X,'Z0',10X,'PHI6')
  136 FORMAT('(14X,',I4,'G12.3)')
  150 FORMAT(',1X,',I2,'A1')
C
      END
