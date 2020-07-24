*
* $Id: dsfdch.F,v 1.1.1.1 1998/02/06 19:11:41 dpp Exp $
*
* $Log: dsfdch.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.5  1996/02/14 20:28:39  zfiles
* removed call to SFLXF; uses SFLSFA etc, removed calculation of phi
*
* Revision 1.4  1995/04/20 22:38:46  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.3  1995/02/27  21:37:52  zfiles
* More typing from DPP
*
* Revision 1.1.1.1  1994/10/08  01:01:03  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/14 01/03/94  16.59.28  by  Rob Kutschke
*CMZ :  6.00/07 31/08/93  14.36.17  by  Rob Kutschke
*CMZ :  5.54/15 18/10/91  13.01.54  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.32  by  Dan Peterson
*CMZ :  2.00/00 09/05/89  11.25.03  by  Dan Peterson
*-- Author : Dan Peterson

      SUBROUTINE DSFDCH(I,ICALL)

C DUMP CHAIN NUMBER I
C MORE DETAILED DUMP IF ERROR IS FOUND IN DATA STRUCTURE

#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif

      INTEGER I
      INTEGER L1,L2,IW1,IW2
      INTEGER J,JLAST,JTHIS,JBEFOR,JNEXT,IGO,ICALL
      INTEGER KILLA,KILLE,KILLO
      DATA KILLA,KILLE,KILLO/0,0,0/

5     L1=SFLSFA(LINCHN(I))
      IW1=SFWSFA(LINCHN(I))
      L2=SFLSFA(MINCHN(I))
      IW2=SFWSFA(MINCHN(I))

      PRINT 1002,I,LENCHN(I),L1,IW1,LVLCHN(1,I),
     1                        L2,IW2,LVLCHN(2,I)
1002  FORMAT(' DSFDCH:',
     1       I3,' L=',I2,'  S(',I2,':',I3,') V=',I5,
     1                   '  F(',I2,':',I3,') V=',I5)

      J=0
      JLAST=0
      JTHIS=LINCHN(I)
11    CONTINUE
      J=J+1
      JBEFOR=ISTBAK(JTHIS)
      JNEXT=ISTFOR(JTHIS)
      L1=SFLSFA(JTHIS)
      IW1=SFWSFA(JTHIS)


CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

C LOOK FOR ERRORS IN DATA STRUCTURE

      IGO=1
      IF(JBEFOR.NE.JLAST)GO TO 101
      IGO=2

#if !defined(CLEO_SFDIAG)
      IF(INUCHN(JTHIS).NE.I)GO TO 101
#endif

      IGO=3
      IF(J.GE.100)GO TO 101
      IF(JTHIS.EQ.MINCHN(I))GO TO 13
      IGO=4
      IF(JNEXT.LE.0)GO TO 101
13    CONTINUE

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

      IF(KILLO.EQ.1)GO TO 14

#if !defined(CLEO_SFDIAG)
      PRINT 3001,J,JBEFOR,JTHIS,JNEXT,L1,IW1,
     1  INUCHN(JTHIS),DOUBPT(JTHIS)
#endif

#if defined(CLEO_SFDIAG)
      PRINT 3001,J,JBEFOR,JTHIS,JNEXT,L1,IW1,
     1  INUCHN(JTHIS),DOUBPT(JTHIS),INUEFF(JTHIS)
#endif

3001  FORMAT(I4,I7,' -> **NOW=',I6,' ** ->',I6,'  LAY/WIR=',I4,'/',I5,
     1  ' INUCHN=',I4,I6,I6)

14    IF(JTHIS.EQ.MINCHN(I))GO TO 15
      IF(JNEXT.LE.0)GO TO 15
      JLAST=JTHIS
      JTHIS=JNEXT
      GO TO 11

15    IF(KILLA.EQ.1)GO TO 95
      PRINT 2002

2002  FORMAT(' DSFDCH: waiting to acknowledge chain,'
     1   ' type "K"ill to ignore')

#if !defined(CLEO_XTSUBS)
      READ 2001,IGO
2001  FORMAT(A4)
#endif

#if defined(CLEO_XTSUBS)
      IF(ICALL.EQ.-1)GO TO 95
      CALL XTS_RFV_INPUT
      IGO=XTS_ANSWER_IY
#endif

      IF(IGO.NE.'K')GO TO 95
      KILLA=1
      KILLO=1
95    GO TO 199

101   CONTINUE
      IF(KILLE.EQ.1)GO TO 199

102   PRINT 3002,IGO
3002  FORMAT(' DSFDCH: FATAL ERROR IN DATA STRUCTURE, TYPE=',I3/
     1       '         TYPE "K"ILL TO IGNORE ALL'/
     2       '         TYPE "M"ORE TO SHOW STRUCTURE UP TO ERROR'/
     3       '         TYPE <CR> TO IGNORE AND CONTINUE')

#if !defined(CLEO_XTSUBS)
      READ 2001,IGO
#endif

#if defined(CLEO_XTSUBS)
      IF(ICALL.EQ.-1)GO TO 199
      CALL XTS_RFV_INPUT
      IGO=XTS_ANSWER_IY
#endif

      IF(IGO.NE.'K')GO TO 113
      KILLE=1
      GO TO 199
113   IF(IGO.NE.'M')GO TO 115
      KILLA=0
      KILLO=0
      GO TO 5
115   IF(IGO.EQ.' ')GO TO 199
      IF(IGO.EQ.'null')GO TO 199
      GO TO 102
199   RETURN

#if defined(CLEO_SFDIAG)
      ENTRY DSFDCH0
      KILLA=0
      KILLE=0
      KILLO=0
      RETURN
#endif
      END

