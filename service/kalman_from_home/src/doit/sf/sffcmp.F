*
* $Id: sffcmp.F,v 1.3 2000/10/02 20:52:00 dpp Exp $
*
* $Log: sffcmp.F,v $
* Revision 1.3  2000/10/02 20:52:00  dpp
*      -> clear out use of STATCF(2)
*
* Revision 1.2  2000/08/18 19:10:02  dpp
*     -> remove some obsolete variables
*      -> F77
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.3  1996/01/25 16:36:47  zfiles
* now uses logical argument
*
* Revision 1.2  1995/04/20 22:39:47  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:00:46  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.06.51  by  Rob Kutschke
*CMZ :  5.54/15 22/09/92  16.26.43  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  15.35.54  by  Dan Peterson
*-- Author :
      SUBROUTINE SFFCMP(LGOOD)
C.......................................................................
C.
C. SFFCMP - Compares the CFIND track with the seed SFFTRK track
C.
C. COMMON    : ?
C. CALLS     : VZERO
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 26-May-88
C. LAST MOD  : 17-Aug-88
C.
C. Modification Log.
C. 23-Mar-90 NLYRCF ->KLYRCF for CLEOII
C. 15-Aug-88 tighten criteria by demanding overlap in all bands
C.
C.......................................................................
C.   Sets LGOOD=.FALSE. if the comparison FAILS
C.   Returns without any action if there is no saved track (NFITSV=0)
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/sfseq/sfpar.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"


C----------------------------------------------------------------------
C Argument
C----------------------------------------------------------------------
      LOGICAL LGOOD

C----------------------------------------------------------------------
C CONTROL PARAMETERS
C ISFCMW-Width of the test regions in SFFCMP
C MSFCMB-Number of overlapping test bands of width ISFCMW
C----------------------------------------------------------------------
      INTEGER ISFCMW,MSFCMB
      PARAMETER (ISFCMW=12)
      PARAMETER (MSFCMB=1+KLYRCF-ISFCMW)


C----------------------------------------------------------------------
C variables
C----------------------------------------------------------------------
      INTEGER NMATCH,ITRYSV,ITRY
      INTEGER IMBAND,LMBAND,MMBAND,NMBAND
      INTEGER NHITMA,IPLRMA,IWANTM
      INTEGER NMTCHB(MSFCMB),NMTRYB(MSFCMB)

#if defined(CLEO_SFFTDIAG)
      INTEGER TWANT,ID1,ID2,ID3,ITTMP
#endif

C----------------------Executable code starts here---------------------

      LGOOD=.TRUE.
#if defined(CLEO_SFFTDIAG)
C      CALL CBYT(STATCF(2),12,ITTMP,1,10)
      PRINT 1003,ITTMP
1003  FORMAT(' SFFCMP: ENTER: ITRK (FROM STATCF) =',I6)
#endif

C do not try if there is no current fit
      IF(NFIT.GT.0)THEN

C select the master hits list
C this is a SFIND seeded track, compare to hits in IADFSV
         NHITMA=NFITSV

C do not try if there is no saved fit
         IF(NHITMA.GT.0)THEN

C initialization
            NMATCH=0
            CALL VZERO(NMTCHB,MSFCMB)
            CALL VZERO(NMTRYB,MSFCMB)
            NMBAND=0

C loop over the hits in the saved fit, must find these hits in new fit
            DO 39 ITRYSV=1,NHITMA

C find the layer number
               IPLRMA=IPLFSV(ITRYSV)
               IWANTM=IADFSV(ITRYSV)

C increment the "try" scalers
               MMBAND=IPLRMA
               LMBAND=MMBAND+1-ISFCMW
               IF(LMBAND.LT.1     )LMBAND=1
               IF(MMBAND.GT.MSFCMB)MMBAND=MSFCMB
               DO 15 IMBAND=LMBAND,MMBAND
                  NMTRYB(IMBAND)=NMTRYB(IMBAND)+1
 15            CONTINUE

C NMBAND is the first band of the last hit, the last band filled
               IF(LMBAND.GT.NMBAND)NMBAND=LMBAND

C loop over the hits in the new (probationary) fit
               DO 29 ITRY=1,NFIT

C track is seeded by SFIND, must be same wire/amb

C..............
C looser criterion of only demanding correspondence of wire number
                  IF( IABS(IADFIT(ITRY)).EQ.IABS(IWANTM))THEN
C= tigher criterion demand that the sign of the ambiguity is the same
C                 IF(     (IADFIT(ITRY)).EQ.    (IWANTM))THEN
C..............

C increment the "found match" scalers
                     NMATCH=NMATCH+1
                     DO 27 IMBAND=LMBAND,MMBAND
                        NMTCHB(IMBAND)=NMTCHB(IMBAND)+1
 27                  CONTINUE

C now finished with this layer; jump out of loop
                     GO TO 31
                  ENDIF

C end loop over hits in new fit
 29            CONTINUE

C end of loop over hits in the saved fit
 31            CONTINUE

#if defined(CLEO_SFFTDIAG)
C              TWANT='LAYR'
               TWANT='ADDR'
               PRINT 1001,ITRYSV,IPLRMA,TWANT
     2              IWANTM,LMBAND,MMBAND,NMATCH
 1001          FORMAT(' SFFCMP: LAYER(',I2,')=',I2,' IN SAVED FIT,  ',
     2              ' WANT ',A4,'=',I5,'    BANDS ',I2,' TO ',I2,
     3              ' MATCH=',I2)
#endif

 39         CONTINUE

C global requirement for all tracks

#if defined(CLEO_SFFTDIAG)
            ID1=NFITSV/2
            IF(ID1.LT.3)ID1=3
            PRINT 1002,NMATCH,ID1
 1002       FORMAT(' SFFCMP: NEED NMATCH=',I3,' .GE. ',I2)
#endif

            IF(NMATCH.LT.3)LGOOD=.FALSE.
            IF(NMATCH.LT.NFITSV/2)LGOOD=.FALSE.

C now test the smaller bands
C must have half of the layers on in the saved fit, to be tested
C then...must have half of those saved layers found in the new fit
            IMBAND=0
 51         IMBAND=IMBAND+1
            IF(LGOOD.AND.(IMBAND.LE.NMBAND))THEN

#if defined(CLEO_SFFTDIAG)
               ID1=ISFCMW/2
               PRINT 1005,IMBAND,NMTRYB(IMBAND),ID1
 1005          FORMAT(' SFFCMP: NEED TRYS IN BAND#',I2,' = ',I2,
     1              ' .GE. ',I2,' TO TRY')
#endif

               IF(NMTRYB(IMBAND).GE.ISFCMW/2)THEN

#if defined(CLEO_SFFTDIAG)
                  ID1=NMTRYB(IMBAND)/2
                  PRINT 1006,NMTCHB(IMBAND),ID1
 1006             FORMAT(' SFFCMP: NEED MATCHES IN BAND = ',I2,
     1                 ' .GE. ',I2,' TO PASS')
#endif

                  IF(NMTCHB(IMBAND).LT.NMTRYB(IMBAND)/2)LGOOD=.FALSE.
               ENDIF
               GO TO 51
            ENDIF

#if defined(CLEO_SFFTDIAG)
            IF(LGOOD)THEN
               PRINT 1012
 1012          FORMAT(' SFFCMP: PASSED')
            ELSE
               PRINT 1011
 1011          FORMAT(' SFFCMP: FAILED')
            ENDIF
#endif
               
         ENDIF
      ENDIF

#if defined(CLEO_SFFTDIAG)
      PRINT 1013
 1013 FORMAT(' SFFCMP: ...................... EXIT')
#endif

      RETURN
      END




