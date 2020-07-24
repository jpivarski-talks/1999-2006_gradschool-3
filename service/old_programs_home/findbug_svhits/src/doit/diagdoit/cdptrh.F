*
* $Id: cdptrh.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: cdptrh.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:44  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/05/10 12:53:58  zfiles
* Changes to allow for QDIV hits to be in the list.
*
* Revision 1.1.1.1  1994/10/08  01:00:57  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/10 06/12/91  20.37.34  by  Steve Schaffner
*CMZ :  5.53/03 19/04/90  13.58.54  by  D. Riley
*CMZ :          17/04/90  12.29.20  by  D. Riley
*CMZ :  5.51/00 17/08/89  11.30.23  by  Steve Schaffner
*CMZ :  5.50/00 23/03/89  15.12.47  by  Steve Schaffner
*-- Author :
      SUBROUTINE CDPTRH(TRKNAM)
C.......................................................................
C.
C. CDPTRH - prints the hit wires and residuals in /CDTRAK/ found in DUET
C.
C. Inputs    :
C. Outputs   :
C.
C. COMMON    :
C.
C. Calls     :
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

      INTEGER NLFDIG,NCFDIG,NCHAR,NDASH1,NDASH2,NTRKP,ITRK,NTRK,MLYR
      PARAMETER (NLFDIG = 6)
      PARAMETER (NCFDIG = 4)
      INTEGER IBITS(KLYRTF,2)
      INTEGER JTRKP(MTRKCD)
      INTEGER ILYR,ITRKP,NFIT,IFIT,IWIR,IBIT,IBR,NMAX,ITEST,LLYR

      CHARACTER*(*) TRKNAM
      CHARACTER*8   SOLNAM,DUENAM
      CHARACTER*24  FOR111,FOR115,FOR117
      CHARACTER*132 FOR110,FOR116,FOR120,FOR126

      LOGICAL LINIT

      SAVE LINIT , SOLNAM, DUENAM
      SAVE FOR110, FOR116, FOR120, FOR126

      DATA    LINIT/.TRUE./
      DATA SOLNAM , DUENAM /'SOLO  ','DUET  '/

* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      IF (LINIT) THEN
          LINIT  = .FALSE.
C
          NCHAR  = NCFDIG*NLYRCF - 16
          NDASH1 = MAX0(1,NCHAR/2)
          NDASH2 = MAX0(1,NCHAR-NDASH1)
          WRITE(FOR110,110) NDASH1,NDASH2
          WRITE(FOR120,120) NDASH1,NDASH2
C
          NCHAR  = NLFDIG*NLYRLF - 16
          NDASH1 = MAX0(1,NCHAR/2)
          NDASH2 = MAX0(1,NCHAR-NDASH1)
          WRITE(FOR116,116) NDASH1,NDASH2
          WRITE(FOR126,126) NDASH1,NDASH2
      ENDIF

      IF(TRKNAM.EQ.SOLNAM) RETURN
      IF (TRKNAM .EQ. DUENAM) THEN
          NTRK   = NTRKCD
      ELSE
          NTRK   = NTRKIN
      ENDIF
C
C     FILL-JTRKP-WITH-INDEX-OF-TRACKS-TO-PRINT
C
      NTRKP  = 0
      DO 50 ITRK=1,NTRK
          NTRKP  = NTRKP + 1
          JTRKP(NTRKP)=ITRK
  50  CONTINUE
      IF (LTRKCD.LE.MTRKCD) THEN
          DO 55 ITRK=LTRKCD,MTRKCD
              NTRKP  = NTRKP + 1
              JTRKP(NTRKP)=ITRK
  55     CONTINUE
      ENDIF

C     PRINT THE CF WIRE HITS
      IF (NLYRCF.GT.0) THEN
          WRITE(FOR111,111) NLYRCF,NCFDIG
          WRITE(FOR115,115) NLYRCF,NCFDIG
          WRITE(FOR117,117) NLYRCF,NCFDIG
          WRITE (IUNDGN,FOR110)
          LLYR = LLYRCF
          MLYR = MLYRCF
C
C PRINT-THE-HIT-WIRES
C
          WRITE (IUNDGN,FOR111) (ILYR,ILYR=LLYR,MLYR)
          DO 65 ITRKP=1,NTRKP
              ITRK = JTRKP(ITRKP)
              IF (CHI2CD(ITRK) .GE. 0.0) THEN
                  NFIT  = NHTLVD(ITRK) + NHTAIZ(ITRK) + NHTADR(ITRK)
     +                  + NHTSCT(ITRK) + NHTSDR(ITRK)
                  CALL VZERO( IBITS , KLYRTF*2 )
C
                  DO 60 IFIT = 1,NFIT
                      ILYR = IPLRCD(IFIT,ITRK)
                      IF (ILYR.GT.0.AND.ILYR.LT.QDIVAD) THEN
                          IWIR = IHWRCD(IFIT,ITRK)
                          IBIT = MOD(IWIR,IWRPTF)
                          IF (IBIT.EQ.0) IBIT = ISIGN(NWIRTF(ILYR),IWIR)
                          IF (IBITS(ILYR,1).EQ.0) THEN
                              IBR = 1
                          ELSE
                              IBR = 2
                          ENDIF
                          IBITS(ILYR,IBR) = IBIT
                      ENDIF
C                             !END IF(ILYR
   60             CONTINUE
              ENDIF
              WRITE(IUNDGN,FOR115) ITRK,(IBITS(ILYR,1),ILYR=LLYR,MLYR)
              IF(IBR.EQ.2)
     +             WRITE(IUNDGN,FOR117) (IBITS(ILYR,2),ILYR=LLYR,MLYR)
   65     CONTINUE
      ENDIF

C     PRINT THE LF WIRE HITS
      IF (NLYRLF.GT.0) THEN
          WRITE(FOR111,111) NLYRLF,NLFDIG
          WRITE(FOR115,115) NLYRLF,NLFDIG
          WRITE(FOR117,117) NLYRLF,NLFDIG
          WRITE (IUNDGN,FOR116)
          LLYR = LLYRLF
          MLYR = MLYRLF
C
C PRINT-THE-HIT-WIRES
C
          WRITE (IUNDGN,FOR111) (ILYR,ILYR=LLYR,MLYR)
          DO 75 ITRKP=1,NTRKP
              ITRK = JTRKP(ITRKP)
              IF (CHI2CD(ITRK) .GE. 0.0) THEN
                  NFIT  = NHTLVD(ITRK) + NHTAIZ(ITRK) + NHTADR(ITRK)
     +                  + NHTSCT(ITRK) + NHTSDR(ITRK)
                  CALL VZERO( IBITS , KLYRTF*2 )
C
                  DO 70 IFIT = 1,NFIT
                      ILYR = IPLRCD(IFIT,ITRK)
                      IF (ILYR.GT.0.AND.ILYR.LT.QDIVAD) THEN
                          IWIR = IHWRCD(IFIT,ITRK)
                          IBIT = MOD(IWIR,IWRPTF)
                          IF (IBIT.EQ.0) IBIT = ISIGN(NWIRTF(ILYR),IWIR)
                          IF (IBITS(ILYR,1).EQ.0) THEN
                              IBR = 1
                          ELSE
                              IBR = 2
                          ENDIF
                          IBITS(ILYR,IBR) = IBIT
                      ENDIF
C                             !END IF(ILYR
   70             CONTINUE
              ENDIF
              WRITE(IUNDGN,FOR115) ITRK,(IBITS(ILYR,1),ILYR=LLYR,MLYR)
              IF(IBR.EQ.2)
     +             WRITE(IUNDGN,FOR117) (IBITS(ILYR,2),ILYR=LLYR,MLYR)
   75     CONTINUE
      ENDIF
C
      IF (NLYRCF.GT.0) THEN
C                             ! PRINT THE CF WIRE RESIDUALS
          WRITE(FOR111,111) NLYRCF,NCFDIG
          WRITE(FOR115,115) NLYRCF,NCFDIG
          WRITE(FOR117,117) NLYRCF,NCFDIG
          WRITE (IUNDGN,FOR120)
          LLYR = LLYRCF
          MLYR = MLYRCF
          NMAX = 10**(NCFDIG-1) - 1
C
C PRINT-THE-TRACK-RESIDUALS
C
          WRITE (IUNDGN,FOR111) (ILYR,ILYR=LLYR,MLYR)
          DO 85 ITRKP=1,NTRKP
              ITRK = JTRKP(ITRKP)
              IF (CHI2CD(ITRK) .GE. 0.0) THEN
                  NFIT  = NHTLVD(ITRK) + NHTAIZ(ITRK) + NHTADR(ITRK)
     +                  + NHTSCT(ITRK) + NHTSDR(ITRK)
                  CALL VZERO( IBITS , KLYRTF*2 )
C
                  DO 80 IFIT = 1,NFIT
                      ILYR = IPLRCD(IFIT,ITRK)
                      IF (ILYR.GT.0.AND.ILYR.LT.QDIVAD) THEN
                          IF (IBITS(ILYR,1).EQ.0) THEN
                              IBR = 1
                          ELSE
                              IBR = 2
                          ENDIF
                          ITEST = NINT(AMTOMU*PULLCD(IFIT,ITRK))
                          ITEST = ISIGN(MIN0(NMAX,IABS(ITEST)),ITEST)
                          IBITS(ILYR,IBR) = ITEST
                      ENDIF
C                             !END IF(ILYR
   80             CONTINUE
              ENDIF
C                             !END IF(CHI2CD
              WRITE(IUNDGN,FOR115) ITRK,(IBITS(ILYR,1),ILYR=LLYR,MLYR)
              IF(IBR.EQ.2)
     +             WRITE(IUNDGN,FOR117) (IBITS(ILYR,2),ILYR=LLYR,MLYR)
   85     CONTINUE
      ENDIF

C     PRINT THE LF WIRE RESIDUALS
      IF (NLYRLF.GT.0) THEN
          WRITE(FOR111,111) NLYRLF,NLFDIG
          WRITE(FOR115,115) NLYRLF,NLFDIG
          WRITE(FOR117,117) NLYRLF,NLFDIG
          WRITE (IUNDGN,FOR126)
          LLYR = LLYRLF
          MLYR = MLYRLF
          NMAX = 10**(NLFDIG-1) - 1
C
C PRINT-THE-TRACK-RESIDUALS
C
          WRITE (IUNDGN,FOR111) (ILYR,ILYR=LLYR,MLYR)
          DO 95 ITRKP=1,NTRKP
              ITRK = JTRKP(ITRKP)
              IF (CHI2CD(ITRK) .GE. 0.0) THEN
                  NFIT  = NHTLVD(ITRK) + NHTAIZ(ITRK) + NHTADR(ITRK)
     +                  + NHTSCT(ITRK) + NHTSDR(ITRK)
                  CALL VZERO( IBITS , KLYRTF*2 )
C
                  DO 90 IFIT = 1,NFIT
                      ILYR = IPLRCD(IFIT,ITRK)
                      IF (ILYR.GT.0.AND.ILYR.LT.QDIVAD) THEN
                          IF (IBITS(ILYR,1).EQ.0) THEN
                              IBR = 1
                          ELSE
                              IBR = 2
                          ENDIF
                          ITEST = NINT(AMTOMU*PULLCD(IFIT,ITRK))
                          ITEST = ISIGN(MIN0(NMAX,IABS(ITEST)),ITEST)
                          IBITS(ILYR,IBR) = ITEST
                      ENDIF
C                             !END IF(ILYR
   90             CONTINUE
              ENDIF
C                             !END IF(CHI2CD
              WRITE(IUNDGN,FOR115) ITRK,(IBITS(ILYR,1),ILYR=LLYR,MLYR)
              IF(IBR.EQ.2)
     +             WRITE(IUNDGN,FOR117) (IBITS(ILYR,2),ILYR=LLYR,MLYR)
   95     CONTINUE
      ENDIF
C
      RETURN
C
  110 FORMAT('(/5X,''<'',',
     +    I3,'(''-''),'' CF ADDRESSES ''',I3,'(''-''),''>'')')
  116 FORMAT('(/5X,''<'',',
     +    I3,'(''-''),'' LF ADDRESSES ''',I3,'(''-''),''>'')')
  120 FORMAT('(/5X,''<'',',
     +    I3,'(''-''),'' CF RESIDUALS ''',I3,'(''-''),''>'')')
  126 FORMAT('(/5X,''<'',',
     +    I3,'(''-''),'' LF RESIDUALS ''',I3,'(''-''),''>'')')
  111 FORMAT('('' TRK '',',I3,'I',I2,')')
  115 FORMAT('(I4,1X,',I3,'I',I2,')')
  117 FORMAT('(   5X,',I3,'I',I2,')')
C
      END
