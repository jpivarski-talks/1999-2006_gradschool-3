*
* $Id: tfstkdmp.F,v 1.3 2000/01/24 19:47:28 dpp Exp $
*
* $Log: tfstkdmp.F,v $
* Revision 1.3  2000/01/24 19:47:28  dpp
*      -> extended print statements
*      -> add in the "$"s in the ID statements
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE TFSTKDMP(ISTAK,WHICH)
C.......................................................................
C.
C. TFSTKDMP - dump stack information
C.
C. COMMON    :
C.
C. Calls     :
C. Called    :
C. Author    :
C.
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- SEQUENCE declarations ---------------
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfstakcd.inc"
* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: tfstkdmp.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.3 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/01/24 19:47:28 $')
      PARAMETER(CHRCSA='$Author: dpp $')

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFSTKDMP' )
      CHARACTER*30 WHICH
      INTEGER ISTAK
      INTEGER ORDERNOW(NSTKDM),CURRENT,INDEX

* ----------Executable code starts here---------------------

      IF(NSTKTF(ISTAK).GE.1)THEN
         INDEX=1
         CURRENT=IFSTK(ISTAK)
 10      ORDERNOW(INDEX)=CURRENT
         INDEX=INDEX+1
         CURRENT=NXTSTK(CURRENT,ISTAK)
         IF(INDEX.LE.NSTKTF(ISTAK))GO TO 10
      ENDIF

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      PRINT 1001,WHICH,IFSTK(ISTAK),NSTKTF(ISTAK)
 1001 format(' TFSTKDMP from ',A30,' IFSTK,NSTKTF=',2I4)
      PRINT 1002,(       ORDERNOW(INDEX) ,INDEX=1,NSTKTF(ISTAK))
 1002 FORMAT('         order:   ',15I8)
      PRINT 1003,(CHIKEY(ORDERNOW(INDEX),ISTAK),INDEX=1,NSTKTF(ISTAK))
 1003 FORMAT('        CHIkey:   ',15F8.3)
      PRINT 1004,(NHTKEY(ORDERNOW(INDEX),ISTAK),INDEX=1,NSTKTF(ISTAK))
 1004 FORMAT('        NHTkey:   ',15I8)
      PRINT 1005,(TMPKEY(ORDERNOW(INDEX)      ),INDEX=1,NSTKTF(ISTAK))
 1005 FORMAT('        TMPkey:   ',15I8)
      PRINT 1006,(LFHKEY(ORDERNOW(INDEX),ISTAK),INDEX=1,NSTKTF(ISTAK))
 1006 FORMAT('        LFHkey:   ',15I8)
      RETURN
      END


