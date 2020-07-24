*
* $Id: tfstkdmp.F,v 1.4 2002/05/23 14:31:45 dpp Exp $
*
* $Log: tfstkdmp.F,v $
* Revision 1.4  2002/05/23 14:31:45  dpp
*      -> stak is now structured as sub-staks in a global stak
*          rather than having the stak be multidimensioned
*
* Revision 1.3  2000/01/24 19:47:28  dpp
*      -> extended print statements
*      -> add in the "$"s in the ID statements
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE TFSTKDMP(SUBSTAK,WHICH)
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
      PARAMETER(CHRCSR='$Revision: 1.4 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2002/05/23 14:31:45 $')
      PARAMETER(CHRCSA='$Author: dpp $')

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFSTKDMP' )
      CHARACTER*30 WHICH
      INTEGER SUBSTAK
      INTEGER ORDERNOW(NSTKDM),CURRENT,INDEX

* ----------Executable code starts here---------------------

      IF(NSTKTF(SUBSTAK).GE.1)THEN
         INDEX=1
         CURRENT=IFSTK(SUBSTAK)
 10      ORDERNOW(INDEX)=CURRENT
         INDEX=INDEX+1
         CURRENT=NXTSTK(CURRENT)
         IF(INDEX.LE.NSTKTF(SUBSTAK))GO TO 10
      ENDIF

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
      PRINT 1001,WHICH,IFSTK(SUBSTAK),NSTKTF(SUBSTAK)
 1001 format(' TFSTKDMP from ',A30,' IFSTK,NSTKTF=',2I4)
      PRINT 1002,(       ORDERNOW(INDEX) ,INDEX=1,NSTKTF(SUBSTAK))
 1002 FORMAT('         order:   ',15I8)
      PRINT 1003,(CHIKEY(ORDERNOW(INDEX)),INDEX=1,NSTKTF(SUBSTAK))
 1003 FORMAT('        CHIkey:   ',15F8.3)
      PRINT 1004,(NHTKEY(ORDERNOW(INDEX)),INDEX=1,NSTKTF(SUBSTAK))
 1004 FORMAT('        NHTkey:   ',15I8)
      PRINT 1005,(TMPKEY(ORDERNOW(INDEX)),INDEX=1,NSTKTF(SUBSTAK))
 1005 FORMAT('        TMPkey:   ',15I8)
      PRINT 1006,(LFHKEY(ORDERNOW(INDEX)),INDEX=1,NSTKTF(SUBSTAK))
 1006 FORMAT('        LFHkey:   ',15I8)
      RETURN
      END


