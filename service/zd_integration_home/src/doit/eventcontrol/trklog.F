* Urheim - declare ANSWER as a character variable.
*
* $Id: trklog.F,v 1.4 2002/06/06 16:02:31 atems Exp $
*
* $Log: trklog.F,v $
* Revision 1.4  2002/06/06 16:02:31  atems
*         conditionalise definition of variable used as both INTEGER and CHARACTER*1 (for Linux)
*
* Revision 1.3  2002/06/05 03:04:05  atems
* minor fixes to compile under linux
*
* Revision 1.2  2001/11/19 23:37:42  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.5  1996/04/23 07:30:52  zfiles
* Rename some variables.  Length of character string argument now passed in.
*
* Revision 1.4  1995/12/14 03:46:30  zfiles
* Implement "O"ther format
*
* Revision 1.3  1995/09/25 19:38:40  zfiles
* change variable IY to integer for compatibility with XTS_ANSWER_IY
*
* Revision 1.2  1995/06/25  01:18:48  zfiles
* Mods from Rob (and Jon)
* doit/duseq/cdtrakcd.inc
* doit/duseq/cdtrakdo.inc
* Add new variable PATLCD.
* doit/duseq/tfxfitcd.inc
* Change PTLIVx to LOGICAL from INTEGER
*
* Revision 1.1  1995/04/20  22:39:12  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE TRKLOG(NTRK,ITRK,CALLER)

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/anltmp.inc"
#include "seq/clutil/mesrep.inc"

#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif


C NTRK and ITRK are both local variables in CDSTOR

      CHARACTER*(*) CALLER
      INTEGER NTRK,ITRK

      INTEGER KILLA,I,IFORMA,ILUN
#if defined(Linux)
      CHARACTER*1 HEADER(80),ANSWER
#else
      CHARACTER*1 HEADER(80)
      INTEGER ANSWER
#endif
      LOGICAL LFIRST

      INTEGER LNBLNK

      DATA LFIRST /.TRUE./
      DATA KILLA, IFORMA, ILUN /0,0,20/
      DATA HEADER / 80*' '/
      SAVE KILLA, IFORMA, ILUN

C ----------Executable code starts here---------------------

11    CONTINUE

      IF ( KILLA .EQ. 0 ) PRINT 3301,NRUN,NEVENT,NTRK
3301  FORMAT(' TRKLOG: LOG EVENT/TRACK?',I6,'/',I6,'/',I6,
     1   ' "Y", "N", "A"lways, "K"ill or "O"ther')


      IF(KILLA.EQ.-1)GO TO 41
      IF(KILLA.EQ. 1)GO TO 95

#if !defined(CLEO_XTSUBS)
      READ 3303,ANSWER
3303  FORMAT(A1)
#endif

#if defined(CLEO_XTSUBS)
      CALL XTS_RFV_INPUT
      ANSWER=XTS_ANSWER_IY
#endif

      IF(ANSWER.EQ.'O') IFORMA = 1

      IF(ANSWER.EQ.'N')GO TO 95
      IF(ANSWER.EQ.'Y')GO TO 41
      IF(ANSWER.EQ.'A')GO TO 35
      IF(ANSWER.EQ.'O')GO TO 35
      IF(ANSWER.EQ.'K')GO TO 37
      GO TO 11
35    KILLA=-1
      GO TO 41
37    KILLA=1
      GO TO 95
      

41    IF(.NOT.LFIRST)GO TO 49
      CALL GETLUN( ILUN, 'TRKLOG')
      LFIRST = .FALSE.
      PRINT 1001,CALLER(:LNBLNK(CALLER))
1001  FORMAT(' TRKLOG: WILL OPEN "TRACKLOG.LIST", CALLED BY ',A)

      OPEN(UNIT=ILUN,FILE='TRACKLOG.LIST',STATUS='UNKNOWN')

      PRINT 1003
1003  FORMAT(' TRKLOG: enter header for TRACKLOG.LIST file'/
     1       '123456789 123456789 123456789 123456789 ',
     1       '123456789 123456789 123456789 123456789 ')

      READ 1011,HEADER
1011  FORMAT(80A1)

      PRINT 1012,HEADER
1012  FORMAT(' TRKLOG: will write header '/80A1)
      WRITE(ILUN,1011) HEADER

49    CONTINUE

      IF ( IFORMA .EQ. 0 ) THEN
         WRITE(ILUN,1002) NRUN,NEVENT,NTRK,ITRK,CURCD(ITRK),
     2                  (KBITDR(NTRK,I),I=1,3),(STATCD(I,ITRK),I=1,2)
      ELSE
         WRITE(ILUN,1020) NRUN, NEVENT, NTRK, ITRK, KINCD(ITRK)
         WRITE(ILUN,1021) (KBITDR(NTRK,I),I=1,3),(STATCD(I,ITRK),I=1,2)
         WRITE(ILUN,1022) CURCD(ITRK), PHI0CD(ITRK), D0CD(ITRK),
     2                  TANDCD(ITRK), Z0CD(ITRK)
         WRITE(ILUN,1022) (ERRMCD(I,ITRK),I=1,NERMTF)
         WRITE(ILUN,1023) RESICD(ITRK), CHI3CD(ITRK), CHI3PT(ITRK),
     2                  CHI3VD(ITRK), CHI3DR(ITRK), CHI3CT(ITRK)
         WRITE(ILUN,1024) NDOFCD(ITRK), RESMCD(ITRK), NN2CCD(ITRK)

      ENDIF

1002  FORMAT(/,I7,I7,I7,I3,F11.6,5O12)
1020  FORMAT(I7,I7,I7,I7,I7,I7)
1021  FORMAT(5Z10.8)
1022  FORMAT(5E15.6 )
1023  FORMAT(6E13.4 )
1024  FORMAT(I8,2E15.6)

95    CONTINUE

      RETURN
      END
