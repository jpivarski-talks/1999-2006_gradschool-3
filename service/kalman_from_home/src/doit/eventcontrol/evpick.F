*
* $Id: evpick.F,v 1.5 2002/06/06 16:02:30 atems Exp $
*
* $Log: evpick.F,v $
* Revision 1.5  2002/06/06 16:02:30  atems
*         conditionalise definition of variable used as both INTEGER and CHARACTER*1 (for Linux)
*
* Revision 1.4  2002/06/05 03:04:03  atems
* minor fixes to compile under linux
*
* Revision 1.3  2001/11/20 17:55:49  dpp
*      -> delete stupid useless nonexistent include file
*
* Revision 1.2  2001/11/19 23:37:29  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:42  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:26  clib
* Developmental version of DUET.
*
* Revision 1.3  1995/09/25 19:38:18  zfiles
* change variable ITRK to integer for compatibility with XTS_ANSWER_IY
*
* Revision 1.2  1995/06/25  01:18:46  zfiles
* Mods from Rob (and Jon)
* doit/duseq/cdtrakcd.inc
* doit/duseq/cdtrakdo.inc
* Add new variable PATLCD.
* doit/duseq/tfxfitcd.inc
* Change PTLIVx to LOGICAL from INTEGER
*
* Revision 1.1  1995/04/20  22:39:09  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :          03/06/93  18.07.46  by  Dan Peterson
*CMZ :          06/01/92  21.02.59  by  Dan Peterson
      SUBROUTINE EVPICK(LUSEEV,CALLER)

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "seq/clutil/mesrep.inc"


      LOGICAL LUSEEV
      CHARACTER*20 CALLER
#if defined(Linux)
      CHARACTER*1 HEADER(80),ITRK
#else
      CHARACTER*1 HEADER(80)
      INTEGER ITRK
#endif
      INTEGER KILLA
      INTEGER ILST,NWANT,WANTRU(1000),WANTEV(1000)
      REAL WANTST(1000),STATUS
      LOGICAL LFIRST

#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

      DATA LFIRST /.TRUE./
      DATA KILLA/0/

C ------------- executable code starts here ----------------------------
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12

34    CONTINUE

      IF(NEVENT.NE.0)THEN
        PRINT 3301,NRUN,NEVENT
3301    FORMAT(' EVPICK: TESTING EVENT=',I6,'/',I6,
     1   ' "Y" OR "N" OR "A" OR "O"ther for LIST ?')

      ELSE
        PRINT 3302,NRUN
3302    FORMAT(' EVPICK: TESTING EVENT=',I6,'/','*ZERO*',
     1   ' "Y" OR "N" OR "A" OR "O"ther for LIST ?'/
     2   '         ** THINK TWICE BEFORE YOU SKIP THE CALIBRATION **')
      ENDIF

      IF(KILLA.EQ.-1)GO TO 91
      IF(KILLA.EQ. 1)GO TO 39

#if !defined(CLEO_XTSUBS)
      READ 3303,ITRK
3303  FORMAT(A1)
#endif

#if defined(CLEO_XTSUBS)
      CALL XTS_RFV_INPUT
      ITRK=XTS_ANSWER_IY
#endif

      IF(ITRK.EQ.'N')GO TO 93
      IF(ITRK.EQ.'Y')GO TO 91
      IF(ITRK.EQ.'A')GO TO 35
      IF(ITRK.EQ.'O')GO TO 37
      GO TO 34
35    KILLA=-1
      GO TO 91
37    KILLA=1
39    CONTINUE

      IF(.NOT.LFIRST)GO TO 69
      LFIRST = .FALSE.
      PRINT 1001,CALLER
1001  FORMAT(' EVPICK: WILL OPEN "EVENT.LIST", CALLED BY ',A20)
      OPEN(UNIT=33,FILE='EVENT.LIST',STATUS='UNKNOWN')
      READ(33,1011,END=55,ERR=53),HEADER
1011  FORMAT(80A1)
      PRINT 1012,HEADER
1012  FORMAT('   EVPICK: '/' ',80A1)
      GO TO 60
53    PRINT 1013
1013  FORMAT(' EVPICK: ERROR READING HEADER')
      GO TO 67
55    PRINT 1015
1015  FORMAT(' EVPICK: END-OF-FILE READING HEADER')
      GO TO 67

60    NWANT=0
61    NWANT=NWANT+1
      READ(33,1002,END=65,ERR=63)
     1    WANTRU(NWANT),WANTEV(NWANT),WANTST(NWANT)
1002  FORMAT(I7,I7,F7.2)
      IF(NWANT.LT.1000)GO TO 61
      PRINT 1003
1003  FORMAT(' EVPICK: NUMBER OF ENTRIES >/=1000, CLOSE')
      GO TO 67
63    PRINT 1004,NWANT
1004  FORMAT(' EVPICK: ERROR IN FILE READING ENTRY#',I7)
      NWANT=NWANT-1
      GO TO 67
65    NWANT=NWANT-1
      PRINT 1005,NWANT
1005  FORMAT(' EVPICK: FOUND ',I5,' ENTRIES')
67    CLOSE(UNIT=33)
69    CONTINUE

      IF(NWANT.LE.0)GO TO 93
      DO 83 ILST=1,NWANT
      IF(  NRUN.NE.WANTRU(ILST))GO TO 83
      IF(CALLER.EQ.'DUETBR')GO TO 81
      IF(NEVENT.NE.WANTEV(ILST))GO TO 83
81    STATUS=WANTST(ILST)
      GO TO 85
83    CONTINUE
      GO TO 93
85    CONTINUE
      PRINT 3305,NRUN,NEVENT,STATUS
3305  FORMAT(' EVPICK: SELECTED EVENT',I6,'/',I6,
     1       '...  STATUS=',F7.2)

91    LUSEEV=.TRUE.
      GO TO 95
93    LUSEEV=.FALSE.
95    CONTINUE
      RETURN
      END
