*
* $Id: cdcurl.F,v 1.1 2003/03/05 17:02:00 dpp Exp $
*
* $Log: cdcurl.F,v $
* Revision 1.1  2003/03/05 17:02:00  dpp
*      -> NEW ROUTINE, moved from "cdtrack/."
*
* Revision 1.2  2001/11/19 22:45:12  dpp
*     -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:39  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:20  clib
* Developmental version of DUET.
*
* Revision 1.7  1996/05/10 13:04:24  zfiles
* Make error message legal.
*
* Revision 1.6  1996/04/23 22:56:26  zfiles
* Comments in previous version are wrong!
* True comment:
*      Disable CDJOIN if kalman enabled - temporary solution to avoid
*           certain bugs.
*
* Revision 1.5  1996/04/23 07:24:12  zfiles
* Lots of updates from Rob.
*
* Revision 1.4  1996/03/12 14:29:46  zfiles
* MTRAK1 -> MTRAK2. To allow more internal tracks than external
*
* Revision 1.3  1995/11/28 18:45:47  zfiles
* Disable curler joining if ISVDrPR=0. Temporary
*
* Revision 1.2  1995/06/19  20:43:40  zfiles
* Added Kalman filter stuff from Rob
*
* Revision 1.1.1.1  1994/10/08  01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/14 22/09/92  13.51.57  by  B. Geiser
*CMZ :  5.51/00 09/08/89  15.33.10  by  Steve Schaffner
*CMZ :  5.50/00 28/10/88  15.45.22  by  R. Namjoshi
*-- Author :    Dan Riley   12/10/88
      SUBROUTINE CDCURL
C.......................................................................
C.
C. CDCURL - Identify the two halves of curling tracks and attempt
C.          to combine them into one track.
C.
C. COMMON    : ?
C. CALLS     : ?
C. CALLED    : DUET
C. AUTHOR    : M. Ogg, rewritten by D. Riley
C.
C. VERSION   : 1.10
C. CREATED   : ?
C. LAST MOD  : 19-Oct-88
C. 
C. Modification Log.
C.
C. 12-Oct-88 : Rewritten in F77 and extensively revised.  -D. Riley
C. 22-Sep-92 : Improve rho test by adding DACD            -B. Geiser
C. 22-Sep-92 : Save back halves, KINCD -> KINCD - 2       -B. Geiser
C. 15-Mar-95 : R. Kutschke 
C.             Skip tracks already marked for deletion.
C.             ( KINCD .LT. -1)
C. 29-May-95 : R. Kutschke
C.             Do not try CDJOIN if track banks are fill; instead salvage
C.             the front half.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "seq/clutil/mesrep.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duklseq/dutmp1.inc"
#include "doit/duklseq/duklmncd.inc"
C
      INTEGER IPNTR(MTRKCD),INEXTI(MTRKCD),INEXTD(MTRKCD)
      REAL    RHO(MTRKCD),RMAX,CURMAX,RHOI,PHII,Z2PI,DELRHO,DELPHI,PMPI
      REAL    PHICUT,RHOCUT,RTEST1,RTEST2
      INTEGER NCURL,ITRK,IFIRSI,IFIRSD,ICURL,INDI,IND,INDD,ITRKI,NHTDRI
      INTEGER ITRKD,NHTDRD,ITRK1,ITRK2,KTEST,INDKP,INDEL
      LOGICAL LPURGE(MTRKCD),LTEST
      DATA    RHOCUT/0.10/, PHICUT/0.25/

* ----------Executable code starts here---------------------

      IF (NTRKCD.LE.1) RETURN

      CALL VFILL( LPURGE, NTRKCD, .FALSE. )
      RMAX   = RTF(MLYRCF) + (RTF(MLYRCF)-RTF(MLYRCF-1))
      CURMAX = 1./RMAX
      NCURL  = 0
C
C-> FIND ALL CURLING TRACKS AND SORT ON RADIUS OF CURVATURE
C
      DO 10 ITRK=1,NTRKCD
          IF ( KINCD(ITRK) .LT. -1 ) GOTO 10
          IF (ABS(CURCD(ITRK)).GT.CURMAX) THEN
              NCURL  = NCURL + 1
              IPNTR(NCURL) = ITRK
              RHO  (NCURL) = 1./CURCD(ITRK)
          ENDIF
10    CONTINUE

      IF (NCURL.LE.1) RETURN

      IFIRSI = 1
      IFIRSD = 1

      DO 20 ICURL=1,NCURL-1
          INEXTI(ICURL) = ICURL + 1
          INEXTD(ICURL) = ICURL + 1
20    CONTINUE

      INEXTI(NCURL) = 0
      INEXTD(NCURL) = 0

      CALL INCSRR(IFIRSI,INEXTI,RHO)
C                              !INCREASING SORT
      CALL DECSRR(IFIRSD,INEXTD,RHO)
C                             !DECREASING SORT

      IF (RHO(IFIRSI).GE.0.0 .OR. RHO(IFIRSD).LE.0.0) RETURN

      INDI   = IFIRSI

30    IF (RHO(INDI).LT.0.) THEN
          IND    = INDI
          INDI   = INEXTI(INDI)
          GOTO 30
      ENDIF
      INEXTI(IND) = 0

      INDD   = IFIRSD
40    IF (RHO(INDD).GT.0.) THEN
          IND    = INDD
          INDD   = INEXTD(INDD)
          GOTO 40
      ENDIF
      INEXTD(IND) = 0
C
C-> LOOK AT PAIRS OF OPPOSITE CURVATURE
C
      INDI   = IFIRSI
50    IF (INDI.GT.0) THEN
          ITRKI  = IPNTR(INDI)
          NHTDRI = NHTADR(ITRKI)
C                             !NUM OF DR HITS
          RHOI   = RHO(INDI)
          PHII   = Z2PI(PHI0CD(ITRKI) + PI)
C
          INDD   = IFIRSD
60        IF (INDD.GT.0) THEN
              ITRKD  = IPNTR(INDD)
              NHTDRD = NHTADR(ITRKD)
C                             !NUM OF DR HITS
C*********************************  TEST3 ******************
C              DELRHO = RHOI + RHO(INDD)
              DELRHO = (RHOI+D0CD(ITRKI)) + (RHO(INDD)+D0CD(ITRKD))
C*********************************  TEST3 ******************
              DELPHI = PMPI(PHII - PHI0CD(ITRKD))

              LTEST  = ABS(DELPHI).LT.PHICUT
     +          .AND. (ABS(DELRHO).LT.RHOCUT
     +          .OR.   NHTDRI.EQ.0 .OR. NHTDRD.EQ.0)
C
              IF (LTEST) THEN
C-> Decide which end of track is beginning
                  ITRK1    = IPNTR(INDI)
                  ITRK2    = IPNTR(INDD)
                  KTEST    = KINCD(ITRK1) + KINCD(ITRK2)
                  IF (KTEST.EQ.0) THEN
C
C-->  Both tracks KINCD = 0.  Try Z0 (doing it right), VD hits and stereo
C     hits.  Z0 test is opposite sense of the rest, so index is reversed
C     there.  Look for dramatic differences.  (Yes, it's a hack.)
C
                      RTEST1 = FLOAT(NHTLVD(ITRK1))
                      RTEST2 = FLOAT(NHTLVD(ITRK2))
                      IF (ABS(RTEST1-RTEST2).GT.5)    GOTO 70

                      RTEST2 = ABS(Z0CD(ITRK1) - XXBMTF(3))
                      RTEST1 = ABS(Z0CD(ITRK2) - XXBMTF(3))
                      IF (ABS(RTEST1-RTEST2).GT.0.04) GOTO 70

                      RTEST1 = FLOAT(NHTSCT(ITRK1)+NHTSDR(ITRK1))
                      RTEST2 = FLOAT(NHTSCT(ITRK2)+NHTSDR(ITRK2))
                      IF (ABS(RTEST1-RTEST2).GT.2)    GOTO 70

                      RTEST1 = FLOAT( NHTLVD(ITRK1) + NHTAIZ(ITRK1)
     +                              + NHTADR(ITRK1))
                      RTEST2 = FLOAT( NHTLVD(ITRK2) + NHTAIZ(ITRK2)
     +                              + NHTADR(ITRK2))
                      IF (ABS(RTEST1-RTEST2).GT.5)    GOTO 70

                      RTEST2 = ABS(Z0CD(ITRK1) - XXBMTF(3))
                      RTEST1 = ABS(Z0CD(ITRK2) - XXBMTF(3))
                  ELSE IF (KTEST.EQ.-1) THEN
C-->  One track 0, one track -1
                      RTEST1 = FLOAT(KINCD(ITRK1))
                      RTEST2 = FLOAT(KINCD(ITRK2))
                  ELSE IF (KTEST.EQ.-2) THEN
C-->  Both tracks KINCD=-1
                      RTEST1 = FLOAT( NHTLVD(ITRK1) + NHTAIZ(ITRK1)
     +                              + NHTADR(ITRK1))
                      RTEST2 = FLOAT( NHTLVD(ITRK2) + NHTAIZ(ITRK2)
     +                              + NHTADR(ITRK2))
                  ENDIF

70                IF (RTEST1.GE.RTEST2) THEN
                      INDKP = INDI
                      INDEL = INDD
                  ELSE
                      INDKP = INDD
                      INDEL = INDI
                  ENDIF

C                 If there is no room left in the track banks,
C                 then salvage what we can.
                  IF ( KTEST.EQ.-2 .AND.NTRKCD.EQ.MTRAK2) KTEST=-1

C                 Illinois SVD hits are not properly stored here yet.
C                 Only for development purposes.
                  IF ( ISVDPR .GT. 0 .AND. KTEST .EQ. -2 ) THEN
                     WRITE(CHMESS, '(A33,2I8)' )
     +               'CDJOIN disabled for ISVDPR .GT.0 ',
     +               NRUN, NEVENT
                     CALL MESLOG ( 'CDCURL', 1, MSINFO )
                     KTEST = -1
                  ENDIF

                  IF (KTEST.EQ.-2) THEN

                      CALL CDJOIN(IPNTR, LPURGE, INDKP, INDEL)
                      CALL CDPNTR(IFIRSD,INEXTD,INDD)
                      CALL CDPNTR(IFIRSI,INEXTI,INDI)
                      MAPCRL(1,NTRKCD) = IPNTR(INDKP)
                      MAPCRL(2,NTRKCD) = IPNTR(INDEL)
                      INDD = 0

                  ELSE

                      KINCD(IPNTR(INDEL)) = KINCD(IPNTR(INDEL)) - 2
                      IF (INDD.EQ.INDEL) THEN
                          CALL CDPNTR(IFIRSD,INEXTD,INDD)
                      ELSE
                          CALL CDPNTR(IFIRSI,INEXTI,INDI)
                          INDD = 0
                      ENDIF

                  ENDIF
              ENDIF
              IF (INDD.GT.0) INDD = INEXTD(INDD)
              GOTO 60
          ENDIF
          INDI   = INEXTI(INDI)
          GOTO 50
      ENDIF

      END

