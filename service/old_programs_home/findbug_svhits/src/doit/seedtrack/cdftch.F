*
* $Id: cdftch.F,v 1.2 2000/12/19 23:37:25 dpp Exp $
*
* $Log: cdftch.F,v $
* Revision 1.2  2000/12/19 23:37:25  dpp
*     -> remove all references to LCSTDR,MCSTDR,MCTICD,LCTOCD (comment only)
*
* Revision 1.1.1.1  1998/02/06 19:11:45  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:40  clib
* Developmental version of DUET.
*
* Revision 1.2  1995/09/05 13:37:43  zfiles
*  Drop banks if filled either by DUET or KLMN.
*
* Revision 1.1.1.1  1994/10/08  01:00:54  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/14 28/09/92  20.28.59  by  Rob Kutschke
*CMZ :  5.54/03 22/04/91  22.14.27  by  Steve Schaffner
*CMZ :  5.54/00 30/11/90  16.05.07  by  Steve Schaffner
*CMZ :  5.53/06 31/07/90  15.28.49  by  Steve Schaffner
*CMZ :  5.53/04 26/06/90  14.40.27  by  Brian Geiser
*CMZ :  5.53/03 14/05/90  19.41.19  by  Steve Schaffner
*CMZ :  5.51/00 25/08/89  14.44.18  by  Steve Schaffner
*CMZ :  5.50/01 23/05/89  14.55.29  by  Steve Schaffner
*CMZ :  5.50/00 11/05/89  17.40.56  by  Steve Schaffner
*CMZ :          09/05/89  16.10.52  by  Steve Schaffner
* Converted to ZFILES calls
*-- Author :
      SUBROUTINE CDFTCH
C.......................................................................
C.
C. CDFTCH - Copy the seed tracks from /ANLCDR/ to /CDTRAK/
C.
C. COMMON    : ?
C. CALLS     : BEGCLK DECSRI ENDCLK INCSRR KBCDDR TFBERR VFILL  VZERO
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 25-Feb-88
C.
C. Modification Log.
C.
C. 25-Feb-88 : Use +SEQ,TFTIME.                              R. Namjoshi
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tftime.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/zfcom.inc"
#include "doit/duseq/anltmp.inc"
#include "seq/clutil/mesrep.inc"

      INTEGER NEXTRK,IKEY
      COMMON /TFSCRT/ NEXTRK(MTRKCD),IKEY (MTRKCD),RKEY (MTRKCD)

*     IPOINT -- local array of pointers to TRAK banks
*     IERR -- MESLOG error number
      INTEGER IERR,NUMBIT,ITRK,NTRK,IPNEXT,IFTRDR,IPOINT(MTRKCD)
      INTEGER KBITIN,ICYL,NTRKDR,NBADDR,IKIND
      REAL CURTMP,RKEY

* ----------Executable Code starts here---------------------
#include "doit/duseq/duexed.inc"

      CALL BEGCLK(ITCDFT)

C
C== Chisquare = CHIDEF => Duet has not found this track
C
      CALL VZERO( INXTCD, MTRKCD )
C      CALL VZERO( KUHCD , MTRKCD )
C      CALL VZERO( KVHCD , MTRKCD )
      CALL VFILL( CHI2CD, MTRKCD, CHIDEF )
      CALL VFILL( CHIZCD, MTRKCD, CHIDEF )
      CALL VFILL( CHI3CD, MTRKCD, CHIDEF )
      CALL VZERO( ERRMCD, NERMTF*MTRKCD  )

      NTRKIN = 0
C     Don't look for seed tracks if there are no analyzed banks
      IF (PZU(IPTRKF).EQ.0) RETURN

C     Get number of good and bad tracks from track header
      NTRKDR = IZU(PZU(IPTRKF)+JGDTRK)
      NBADDR = IZU(PZU(IPTRKF)+JBADTR)
      NTRKIN = NTRKDR + NBADDR
C
C== Make sure that the # of tracks does not overflow
C
      IF( NTRKIN.GT.MTRKCD ) THEN
          LERCTL(IERCDF) = .TRUE.
          NERCTL(IERCDF) = NERCTL(IERCDF) + 1
          CALL TFBERR(ICDFTC,IERCDF,NTRKIN)
      ENDIF
C
      NDMUSE(IERCDF) = MAX0(NTRKIN,NDMUSE(IERCDF))
      NTRKIN = MIN0(NTRKIN,MTRKCD)

      IF( NTRKIN.GT.0 ) THEN
          DO 5 ITRK = 1, NTRKIN
C             Get the pointers to later track banks (n.b. good locally only!)
              IF (ITRK.EQ.1) THEN
                  IPOINT(ITRK) = PZU(IPTRAK(1))
              ELSE
                  IPOINT(ITRK) = LZU(IPOINT(ITRK-1))
                  IF (IPOINT(ITRK).EQ.0) GOTO 8
              END IF
 5        CONTINUE
      ELSE
          CALL ENDCLK(ITCDFT)
          RETURN
      ENDIF

C
C== Order the tracks according to # of hits & curvature
C
8     DO 10 ITRK = 1, NTRKIN
          IKIND = MAX0(-2,MIN0(IZU(IPOINT(ITRK)+JKINCD),2))
          NTRTIN(IKIND) = NTRTIN(IKIND) + 1
CC          NHLVD = 0
CC          NHAIZ = 0
CC          NHADR = 0
CC          NHSIZ = 0
CC          NHSDR = 0
CC          KUVH  = KUHDR(ITRK)
CC*
CC          IF( NCAXDR.GT.0 ) THEN
CC              DO 20 ILYR = LCAXDR, MCAXDR
CC                  IBIT  = MOD(KUVH,2)
CC                  NHADR = NHADR + IBIT
CC                  KUVH  = KUVH/2
CC20            CONTINUE
CC          ENDIF
CC*
CC          IF( NCANIZ.GT.0 ) THEN
CC              LL = LCANIZ
CC              MM = MCANIZ
CC              DO 30 ILYR = LL, MM
CC                  IBIT  = MOD(KUVH,2)
CC                  NHAIZ = NHAIZ + IBIT
CC                  KUVH  = KUVH/2
CC30            CONTINUE
CC          ENDIF
CC*
CC          KUVH  = KVHDR(ITRK)
CC          IF( NCSTDR.GT.0 ) THEN
CC              DO 40 ILYR = loop over stereo layers
CC                  IBIT  = MOD(KUVH,2)
CC                  NHSDR = NHSDR + IBIT
CC                  KUVH  = KUVH/2
CC40            CONTINUE
CC          ENDIF
CC*
CC          NHSLVD(ITRK) = NHLVD
CC          NHSAIZ(ITRK) = NHAIZ
CC          NHSADR(ITRK) = NHADR
CC          NHSSCT(ITRK) = NHSIZ
CC          NHSSDR(ITRK) = NHSDR
CC*
C         Store number of hits on track, based on KBITCD
          IKEY(ITRK) = 0
          DO 20 ICYL=1,MBITDR
              KBITIN = IZU(IPOINT(ITRK)+JKBIT+ICYL-1)
              IKEY(ITRK) = IKEY(ITRK) + NUMBIT(KBITIN)
20        CONTINUE

C         Store curvature
          RKEY  (ITRK) = ABS(ZU(IPOINT(ITRK)+JCURCD))

          NEXTRK(ITRK) = ITRK+1
10    CONTINUE
C
C== Get ready for sorting
C
      IFTRDR = 1
      NEXTRK(NTRKIN) = 0
      CALL INCSRR( IFTRDR, NEXTRK, RKEY )
      CALL DECSRI( IFTRDR, NEXTRK, IKEY )
C
      ITRK   = IFTRDR
      NTRK   = 0
      CALL VZERO( IDRCD , MTRKCD )
C
50    IF( ITRK.GT.0 .AND. NTRK.LT.MTRKCD ) THEN
          NTRK   = NTRK + 1
          CURCD (NTRK) = ZU(IPOINT(ITRK)+JCURCD)
          PHI0CD(NTRK) = ZU(IPOINT(ITRK)+JPHI0)
          D0CD  (NTRK) = ZU(IPOINT(ITRK)+JD0CD)
          PHI6CD(NTRK) = ZU(IPOINT(ITRK)+JFKN6)
          TANDCD(NTRK) = ZU(IPOINT(ITRK)+JCOTAN)
          Z0CD  (NTRK) = ZU(IPOINT(ITRK)+JZ0CD)
          RESICD(NTRK) = ZU(IPOINT(ITRK)+JRESID)
          IF ( IZU(IPOINT(ITRK)+JPMASK) .GE. 64 ) THEN
            PHI7CD(NTRK) = ZU(IPOINT(ITRK)+JFKN7)
            RKN7CD(NTRK) = ZU(IPOINT(ITRK)+JRKN7)
          ELSE
            PHI7CD(NTRK) = 0.0
            RKN7CD(NTRK) = 9.99
          ENDIF
C
          KINCD (NTRK) = IZU(IPOINT(ITRK)+JKINCD)

          DO 60 ICYL=1,MBITDR
             KBITDR(NTRK,ICYL) = IZU(IPOINT(ITRK)+JKBIT+ICYL-1)
60        CONTINUE
          STATCD(1,NTRK) = KBITDR(NTRK,4)
          STATCD(2,NTRK) = KBITDR(NTRK,5)

CC          NHTLVD(NTRK) = NHSLVD(ITRK)
CC          NHTAIZ(NTRK) = NHSAIZ(ITRK)
CC          NHTADR(NTRK) = NHSADR(ITRK)
CC          NHTSCT(NTRK) = NHSSCT(ITRK)
CC          NHTSDR(NTRK) = NHSSDR(ITRK)
C
C
          IDRCD (NTRK) = ITRK
          ITRK   = NEXTRK(ITRK)
          GOTO 50
      ENDIF
*
      IF( NTRK.NE.NTRKIN ) CALL TFBERR( ICDFTC, IERNXT, NTRK )
      CALL ENDCLK(ITCDFT)
C

C     If input data was DUET, drop input bank
      IF (IZU(PZU(IPTRKF)+JHNAME).EQ.IHDUET) CALL ZUDROP(IPTRKF,' ')
      IF (IZU(PZU(IPTRKF)+JHNAME).EQ.IHKLMN) CALL ZUDROP(IPTRKF,' ')
      RETURN

C     In case of ZFILES error
C 99   WRITE (CHMESS,199)
C199   FORMAT(' Error in ZFILES call')
C      IERR = 1
C      CALL MESLOG('CDFTCH',IERR,MSERRO)
C      NERCTL(IERG20) = NERCTL(IERG20) + 1
C      RETURN

      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"

