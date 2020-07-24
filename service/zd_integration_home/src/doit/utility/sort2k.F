*
* $Id: sort2k.F,v 1.2 2001/11/20 00:15:49 dpp Exp $
*
* $Log: sort2k.F,v $
* Revision 1.2  2001/11/20 00:15:49  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:44  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:42  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/36 11/05/93  13.59.14  by  Dan Peterson
*-- Author :D. Peterson
      SUBROUTINE SORT2K(IKEY1,RKEY2,IPNTR,NUM,IERR)
C.......................................................................
C.
C. SORT2K    : perform an ascending sort, using 2 keys
C.           : first (integer) key is first priority and used in absolute value
C.           : second (real) key is lower priority
C.
C. COMMON    : None
C. CALLS     : None
C. CALLED    : TF3FIT during calibration use
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 31-April-93
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C== Argument declarations

      INTEGER IKEY1(*), IPNTR(*), NUM, IERR
      REAL    RKEY2(*)

C== Local variables

      INTEGER SIZE
      PARAMETER (SIZE=500)
      INTEGER NEXT(SIZE)
      INTEGER I,NOW

C-->  BEST  = ADDRESS OF HIT WITH LOWEST IKEY1 AND RKEY2
C-->  WORST = ADDRESS OF HIT WITH LOWEST IKEY1 AND RKEY2
C-->  TESTED= ADDRESS OF HIT THAT NEEDS TO FIND
C                       ITS PROPER PLACE IN THE ORDER
C-->  BETTER= ADDRESS OF HIT THAT MUST BE BETTER THAN TESTED
C-->  MAYBE = ADDRESS OF HIT THAT MAY_BE WORSE THAN TESTED

      INTEGER BEST,WORST,TESTED,BETTER,MAYBE

* ----------Executable code starts here---------------------

      IF(NUM.LE.0)GO TO 98
      IF(NUM.GT.SIZE)GO TO 91

C set up default values of IPNTR, and the link list

      BEST=1
      DO 1 I=1,NUM
      NEXT(I)=I+1
1     IPNTR(I)=I
      IF(NUM.EQ.1)GO TO 98
      NEXT(NUM)=0

C sort: first set up a link list for faster sorting
C        this part is modeled after TFSORT

      WORST =BEST

10    TESTED=NEXT(WORST)
      IF(TESTED.LE.0)GO TO 40
      BETTER=0
      MAYBE =BEST

20    IF(IABS(IKEY1(TESTED)).LT.IABS(IKEY1(MAYBE)))GO TO 35
      IF(IABS(IKEY1(TESTED)).GT.IABS(IKEY1(MAYBE)))GO TO 27
      IF(     RKEY2(TESTED) .LT.     RKEY2(MAYBE) )GO TO 35

C "MAYBE" is better than "TESTED"

27    BETTER=MAYBE
      MAYBE =NEXT(MAYBE)
      IF(MAYBE.NE.TESTED)GO TO 20

C everything is better than "TESTED"

      WORST =TESTED
      GO TO 10

C "MAYBE" is worse than "TESTED"
C insert "TESTED" after "BETTER"

35    IF(BETTER.EQ.0)GO TO 36
      NEXT(BETTER)=TESTED
      GO TO 37

C nothing has been found that is better than "TESTED"

36    BEST  =TESTED

C insert "TESTED" before "MAYBE"

37    NEXT(WORST)=NEXT(TESTED)
      NEXT(TESTED)=MAYBE
      GO TO 10

C sorting is done
C load into pointer array IPNTR

40    NOW=BEST
      DO 47 I=1,NUM
      IPNTR(I)=NOW
47    NOW=NEXT(NOW)
      GO TO 98

91    IERR=1
      GO TO 99

98    IERR=0
99    RETURN
      END











