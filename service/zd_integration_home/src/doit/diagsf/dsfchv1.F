*
* $Id: dsfchv1.F,v 1.2 2002/09/05 19:06:24 dpp Exp $
*
* $Log: dsfchv1.F,v $
* Revision 1.2  2002/09/05 19:06:24  dpp
*     -> put diagnostic stops in common
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:23  clib
* Developmental version of DUET.
*
* Revision 1.4  1996/02/14 20:28:38  zfiles
* removed call to SFLXF; uses SFLSFA etc, removed calculation of phi
*
* Revision 1.3  1995/04/20 22:38:45  zfiles
* See the comments for 6.00/39 in TITLE.doit
*
* Revision 1.1.1.1  1994/10/08  01:01:03  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/25 10/05/94  17.50.02  by  Anders Ryd
*CMZ :  6.00/07 31/08/93  14.33.53  by  Rob Kutschke
*CMZ :  5.54/15 18/12/91  11.43.36  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.32  by  Dan Peterson
*CMZ :  2.00/00 09/05/89  11.25.03  by  Dan Peterson
*-- Author : D. Peterson

      SUBROUTINE DSF_CHVIEW1(I)

C displays one chain from SF data structure
C makes simple consistancy tests of chain data structute
C calls DSFDCH to dump structure if error is found
C includes acknowledge point

#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif


      INTEGER IPRNAK,NPRNAK,I
      INTEGER L1,L2,IW1,IW2,IGO
      INTEGER IN(2)

C NUMBER PRINTED AFTER KILL
      DATA IPRNAK,NPRNAK/0,100/



      IF(DSFCHV1_KILLA.NE.1)GO TO 3
      IPRNAK=IPRNAK+1
      IF(IPRNAK.LT.NPRNAK)GO TO 5
      IF(IPRNAK.GT.NPRNAK)GO TO 21

      PRINT 1005,NPRNAK
1005  FORMAT(' DSF_CHVIEW1:WILL NO LONGER TYPE: N=',I5)
      GO TO 21

3     IPRNAK=0
5     IF(I.EQ.0)THEN
          PRINT 1011
1011      FORMAT(' DSF_CHVIEW1: called with 0 chain')

      ELSEIF((I.LT.0).OR.(I.GT.NCHAIN))THEN
          PRINT 1012,I,NCHAIN
1012      FORMAT(' DSF_CHVIEW1:',
     2         ' called with garbage chain#',I4,' NCHAIN=',I3/
     3         ' waiting to acknowledge chain')
#if !defined(CLEO_XTSUBS)
          READ 2001,IGO
#endif
#if defined(CLEO_XTSUBS)
          CALL XTS_RFV_INPUT
          IGO=XTS_ANSWER_IY
#endif
          GO TO 29

      ELSE
          L1=SFLSFA(LINCHN(I))
          IW1=SFWSFA(LINCHN(I))
          L2=SFLSFA(MINCHN(I))
          IW2=SFWSFA(MINCHN(I))


          PRINT 1015,I,LENCHN(I),L1,IW1,LVLCHN(1,I),
     1                           L2,IW2,LVLCHN(2,I)
1015      FORMAT(' DSF_CHVIEW1:',
     1           I3,' L=',I2,'  S(',I2,':',I3,') V=',I5,
     1                       '  F(',I2,':',I3,') V=',I5)


          IF(L1.LT.1)GO TO 17
          IF(L1.GT.NLAYSF)GO TO 17
          IF(L2.LT.1)GO TO 17
          IF(L2.GT.NLAYSF)GO TO 17
          GO TO 19
17        CALL DSFDCH(I,0)
19        CONTINUE
      ENDIF


21    IN(1)=I
      IN(2)=0
      CALL DSF_CHVIEW(IN)

      IF(DSFCHV1_KILLA.NE.1)THEN
        IF(I.NE.0)THEN
          
          PRINT 2002
 2002     FORMAT(' DSF_CHVIEW1: waiting to acknowledge chain,'
     1         '  "K"ill to ignore')
          
#if !defined(CLEO_XTSUBS)
          READ 2001,IGO
 2001     FORMAT(A1)
#endif
#if defined(CLEO_XTSUBS)
          CALL XTS_RFV_INPUT
          IGO=XTS_ANSWER_IY
#endif

          IF(IGO.EQ.'K')DSFCHV1_KILLA=1

        ENDIF
      ENDIF

29    CONTINUE
      RETURN

      ENTRY DSF_CHVIEW10
      IPRNAK=0
      DSFCHV1_KILLA=0
      RETURN

      END




