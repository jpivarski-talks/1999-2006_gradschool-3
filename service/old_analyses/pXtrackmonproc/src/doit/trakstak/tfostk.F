*
* $Id: tfostk.F,v 1.6 2001/11/20 00:08:51 dpp Exp $
*
* $Log: tfostk.F,v $
* Revision 1.6  2001/11/20 00:08:51  dpp
*      -> delete unused include file
*
* Revision 1.5  2000/10/02 21:11:57  dpp
*      -> format, comments
*
* Revision 1.4  2000/01/24 19:46:57  dpp
*      -> add ordering by NHTKEY before CHIKEY
*
* Revision 1.3  1999/08/03 21:58:51  dpp
* add index and hit number saturation
*
* Revision 1.2  1999/04/28 14:01:01  dpp
* add switch to the diagnostic
*
* Revision 1.1.1.1  1998/02/06 19:11:47  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:43  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:56  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 24/09/91  16.46.55  by  Steve Schaffner
*CMZ :  5.54/00 07/01/91  18.33.18  by  Steve Schaffner
*CMZ :  5.53/07 08/08/90  16.45.47  by  D. Riley
*-- Author :
      SUBROUTINE TFOSTK(ISTAK)
C.......................................................................
C.
C. TFOSTK - Orders the stack with CHI3F & NHIT3F
C.
C. COMMON    :
C.
C. Calls     :
C. Called    :
C. Author    : D. Cassel, M. Ogg.
C.
C. Converted to F77, D. Riley  08/08/90  13.34.03
C. ISTKTF = ordinal number of track in /TFSTAK/ just popped
C.        = 0 => all tracks have been popped
C.......................................................................
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
* -------------------- SEQUENCE declarations ---------------
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfstakcd.inc"

* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: tfostk.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.6 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2001/11/20 00:08:51 $')
      PARAMETER(CHRCSA='$Author: dpp $')

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFOSTK' )
      INTEGER ISTAK
      INTEGER MAXTMP,IND,SATURATE
      
* ----------Executable code starts here---------------------


      CALL UCOPY(NHTKEY(1,ISTAK),TMPKEY(1),NSTKTF(ISTAK))
      IF(NSTKTF(ISTAK).GE.2)THEN
         MAXTMP=0
C maximum number of hits
         DO 10 IND=1,NSTKTF(ISTAK)
            IF(TMPKEY(IND).GT.MAXTMP)MAXTMP=TMPKEY(IND)
 10      CONTINUE
C saturate if over the threshold (20)
         IF(MAXTMP.GT.MIN_HITS_SAT_STK_ORD)THEN
C saturate at a level of (5) below the maximum
            SATURATE=MAXTMP-WID_SAT_HITS_STK_ORD
C do not place the saturate level below the threshold
            IF(SATURATE.LT.MIN_HITS_SAT_STK_ORD)
     2                        SATURATE=MIN_HITS_SAT_STK_ORD
            DO 20 IND=1,NSTKTF(ISTAK)
               IF(TMPKEY(IND).GT.SATURATE)TMPKEY(IND)=SATURATE
 20         CONTINUE
         ENDIF
      ENDIF


#if defined(CLEO_DPPDIAG)
      CALL TFSTKDMP(ISTAK,' TFOSTK: enter:              ')
#endif

C old order was CHIKEY, then NHTKEY, then LFTKEY

      CALL DECSRI(IFSTK(ISTAK) ,NXTSTK(1,ISTAK),NHTKEY(1,ISTAK))
#if defined(CLEO_DPPDIAG)
      CALL TFSTKDMP(ISTAK,' TFOSTK: after NHTKEY        ')
#endif


      CALL INCSRR(IFSTK(ISTAK) ,NXTSTK(1,ISTAK),CHIKEY(1,ISTAK))
#if defined(CLEO_DPPDIAG)
      CALL TFSTKDMP(ISTAK,' TFOSTK: after CHIKEY        ')
#endif


      CALL DECSRI(IFSTK(ISTAK) ,NXTSTK(1,ISTAK),TMPKEY(1))
#if defined(CLEO_DPPDIAG)
      CALL TFSTKDMP(ISTAK,' TFOSTK: after TMPKEY        ')
#endif


      CALL DECSRI(IFSTK(ISTAK) ,NXTSTK(1,ISTAK),LFHKEY(1,ISTAK))
#if defined(CLEO_DPPDIAG)
      CALL TFSTKDMP(ISTAK,' TFOSTK: after LFHKEY        ')
#endif

      RETURN
      END


