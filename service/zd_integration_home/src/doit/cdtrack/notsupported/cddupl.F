*
* $Id: cddupl.F,v 1.1 2003/03/05 17:02:01 dpp Exp $
*
* $Log: cddupl.F,v $
* Revision 1.1  2003/03/05 17:02:01  dpp
*      -> NEW ROUTINE, moved from "cdtrack/."
*
* Revision 1.2  2001/11/19 22:45:31  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:20  clib
* Developmental version of DUET.
*
* Revision 1.3  1996/03/12 14:46:58  zfiles
* Deleted tracks now have KINCD=-20
*
* Revision 1.2  1995/06/19 20:43:43  zfiles
* Added Kalman filter stuff from Rob
*
* Revision 1.1.1.1  1994/10/08  01:00:41  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 28/05/91  13.39.07  by  Steve Schaffner
*CMZ :  5.54/00 29/10/90  15.58.37  by  Steve Schaffner
*CMZ :  5.53/06 02/08/90  15.52.25  by  Steve Schaffner
*CMZ :  5.52/01 13/03/90  12.12.15  by  Steve Schaffner
*CMZ :  5.51/04 03/12/89  00.17.00  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.11  by  Steve Schaffner
*CMZ :  5.50/00 24/03/89  12.47.42  by  Steve Schaffner
*-- Author :
      SUBROUTINE CDDUPL
C.......................................................................
C.
C. CDDUPL - Attempt to remove any duplicate tracks from /CDTRAK/
C.
C. COMMON    : ?
C. CALLS     : CDCMPR VFILL  VZERO
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 16-Apr-87
C.
C. Modification Log.
C.
C. Mar 15/95 - R. Kutschke 
C.           - Do not compress list.  Just flag with KINCD.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
C Delete a track if:
C     1) R-PHI track parameters are the 'same'
C     2) It has the smaller number of hits
C     3) All the hits are next to hits on the other track
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/duscrt.inc"
#include "doit/duklseq/dutmp1.inc"
C
      INTEGER  IHITS(2*KLYRCF,MTRKCD)
      INTEGER  NHITS(MTRKCD)
      LOGICAL  LDUPL
      INTEGER  ITRK,ILAST,ILYR,IPLYR,JLYR,IDUPL,JTRK,NDUPL,IDIFF
      REAL     CURCUT,PHICUT

C     Equivalence large arrays to scratch area
      EQUIVALENCE (GOG(1),NHITS)
      EQUIVALENCE (GOG(2*MTRKCD+1),IHITS)

      DATA     CURCUT /0.10/
      DATA     PHICUT /0.20/

* ----------Executable code starts here---------------------
C
C== First pack R-PHI wire numbers into IHITS
C
      IF( NTRKCD.LE.1 ) RETURN
      CALL VZERO( IHITS, 2*KLYRCF*MTRKCD )

      DO 10 ITRK = 1, NTRKCD
          IF ( KINCD(ITRK) .LT. -3 ) GOTO 10
          NHITS(ITRK) = NHTLVD(ITRK) + NHTAIZ(ITRK) + NHTADR(ITRK)
          ILAST       = 0

          DO 20 ILYR = 1, NLR2TF
              IPLYR  = IPLRCD(ILYR,ITRK)
              IF( IPLYR.GE.LLYRCF .AND. IPLYR.LE.MLYRCF ) THEN
                  IF( IPLYR.GT.ILAST ) THEN
                      JLYR = IPLYR
                  ELSE
                      JLYR = (2*NLYRCF+1) - IPLYR
                  ENDIF
                  ILAST  = IPLYR
                  IHITS(JLYR,ITRK) = IABS(IHWRCD(ILYR,ITRK))
              ENDIF
20        CONTINUE

10    CONTINUE
C
C== Check the difference of wire numbers on the same layer
C
      DO 30 ITRK = 1, NTRKCD-1
        IF ( KINCD(ITRK) .LT. -3 ) GOTO 30
        IDUPL  = 0
        DO 40 JTRK = ITRK+1, NTRKCD
          IF ( KINCD(JTRK) .LT. -3 ) GOTO 40
          IF( IDUPL.EQ.0 ) THEN
          IF( ABS(CURCD (ITRK)-CURCD (JTRK)).LT.CURCUT ) THEN
          IF( ABS(PHI0CD(ITRK)-PHI0CD(JTRK)).LT.PHICUT ) THEN
            LDUPL  = .TRUE.
            NDUPL  = 0
            DO 50 ILYR = 1, 2*NLYRCF
              IF( LDUPL ) THEN
              IF( IHITS(ILYR,ITRK).NE.0 .AND.
     +            IHITS(ILYR,JTRK).NE.0       ) THEN
                IF(ILYR.GE.LLYRCF .AND. ILYR.LE.MLYRCF) THEN
                    IPLYR = ILYR
                ELSE
                    IPLYR = 2*NLYRCF+1-ILYR
                ENDIF
                IDIFF  = IABS(IHITS(ILYR,ITRK) - IHITS(ILYR,JTRK))
                LDUPL  = IDIFF.EQ.1 .OR. IDIFF.EQ.(NWIRTF(IPLYR)-1)
                IF( LDUPL ) NDUPL = NDUPL + 1
              ENDIF
              ENDIF
50          CONTINUE
C
C== See which track was duplicate
C
            LDUPL  = LDUPL .AND. NDUPL.GE.MDPLCF
            IF( LDUPL ) THEN
                IDUPL  = JTRK
                IF(NHITS(ITRK).LT.NHITS(JTRK)) IDUPL = ITRK
                KINCD (IDUPL) = KINCD(IDUPL) - 20
                MAPO2N(IDUPL) = -999
            ENDIF
          ENDIF
          ENDIF
          ENDIF

40      CONTINUE
30    CONTINUE
C
      RETURN
      END
