*
* $Id: dsfchv.F,v 1.1.1.1 1998/02/06 19:11:41 dpp Exp $
*
* $Log: dsfchv.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:03  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/03 08/08/93  17.48.31  by  Rob Kutschke
*CMZ :  5.54/15 26/11/91  12.24.59  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.32  by  Dan Peterson
*CMZ :  2.00/00 09/05/89  13.38.38  by  Dan Peterson
*-- Author :
C
      SUBROUTINE DSF_CHVIEW(IN)
C THIS IS THE GENERAL ROUTINE TO DISPLAY ONE CHAIN
C AND (MOST IMPORTANTLY) REMOVE THE PREVIOUS CHAIN
      INTEGER IN(*)
#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
      INTEGER I, INLAST
      COMMON/CHV/INLAST(100)
C
C DELETE THE LAST DISPLAYED CHAIN
      I=0
1     I=I+1
      IF(INLAST(I).EQ.0)GO TO 10
      CALL DSF_CHVIEW_SIMPLE(INLAST(I),2)
      GO TO 1
10    I=0
C
C DIPLAY THE NEW CHAIN
11    I=I+1
      INLAST(I)=IN(I)
      IF(IN(I).EQ.0)RETURN
      CALL DSF_CHVIEW_SIMPLE(IN(I),3)
      GO TO 11
C
      END

