*
* $Id: tf3inv.F,v 1.1 2003/03/05 17:44:47 dpp Exp $
*
* $Log: tf3inv.F,v $
* Revision 1.1  2003/03/05 17:44:47  dpp
*      -> NEW ROUTINE, moved from "fitter/."
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:28  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:49  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/36 12/05/93  10.20.52  by  Dan Peterson
*-- Author : D. Peterson
      SUBROUTINE TF3INV(MP3DLO,GG2D,NP3DLO,TEMP,IFAIL,DET,JFAIL,
     +                  LFLT)
      IMPLICIT NONE
#include "doit/duseq/tfindpar.inc"
C  MP3DLO...passed number of variables in the matrix, including unused
C  NP3DLO...passed value that had better be NP3DTF
C  GG2D(,)..GG expanded to square matrix, then inverse (mult of error matrix)
C  TEMP()...space for the CERNiod routines RFACT and RFINV
C  IFAIL....return flag from CERNiod routines, 0:determinant is filled
C  DET......return determinant from CERNiod routines
C  JFAIL....return flag from CERNiod routines, 0:determinant is not junk
C  LFLT.....input flag that variable is used in the fit
C  NFLT.....number of found floating variables
C  GG2F.....version of GG2D with zeroed rows and columns deleted

C RFACT...(CERN)..prepare for RFINV, NFLT upper corner of NP3DTF square
C               ..IFAIL=0 is good, =-1 is indicates singular matrix
C               ..JFAIL=0 is good, =-1 is determinant to small, +1 to large
C RFINV...(CERN)..invert matrix GGF, stick it in GGF  (overwrite GGF)


      INTEGER MP3DLO,NP3DLO,IFAIL,JFAIL
      REAL  GG2D(NP3DTF,NP3DTF),TEMP(NP3DTF)
      REAL DET
      LOGICAL LFLT(NP3DTF)
      INTEGER NFLT
      INTEGER I,J,N2
      REAL  GGF(NP3DTF,NP3DTF)

C test that MP3DLO is not crazy

      IF(MP3DLO.GT.NP3DTF)GO TO 95

C pull out the unused rows and columns

          NFLT=0
          DO 39 I=1,MP3DLO
          IF(.NOT.LFLT(I))GO TO 39
              NFLT=NFLT+1
              N2=0
              DO 37 J=1,MP3DLO
              IF(.NOT.LFLT(J))GO TO 37
                  N2=N2+1
                  GGF(NFLT,N2)=GG2D(I,J)
37        CONTINUE
39        CONTINUE

C test that NFLT is not crazy

      IF(NFLT.EQ.0)GO TO 96

C call the CERNoid routines

          CALL RFACT(NFLT,GGF,NP3DTF,TEMP,IFAIL,DET,JFAIL)
               IF(IFAIL.NE. 0)GO TO 99
               IF(JFAIL.LE.-1)GO TO 99
               IF(JFAIL.GE. 1)GO TO 99
          CALL RFINV(NFLT,GGF,NP3DTF,TEMP)

C expand the inverted matrix; replace the unused rows and columns

          DO 49 I=1,MP3DLO
          DO 49 J=1,MP3DLO
49        GG2D(I,J)=0.

          NFLT=0
          DO 59 I=1,MP3DLO
          IF(.NOT.LFLT(I))GO TO 59
              NFLT=NFLT+1
              N2=0
              DO 57 J=1,MP3DLO
              IF(.NOT.LFLT(J))GO TO 57
                  N2=N2+1
                  GG2D(I,J)=GGF(NFLT,N2)
57        CONTINUE
59        CONTINUE
      GO TO 99


95    IFAIL=5
      GO TO 99
96    IFAIL=6

99    RETURN
      END
















