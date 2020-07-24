*
* $Id: tfpopf.F,v 1.6 2000/10/02 21:11:42 dpp Exp $
*
* $Log: tfpopf.F,v $
* Revision 1.6  2000/10/02 21:11:42  dpp
*      -> program indicator
*
* Revision 1.5  1999/12/09 21:24:29  dpp
* save the new de-weight in stak
*
* Revision 1.4  1999/11/03 23:01:15  dpp
* set the up-to-date flag in /TFHITS/ =FALSE
*
* Revision 1.3  1999/08/03 21:59:01  dpp
* add index
*
* Revision 1.2  1999/04/27 20:18:05  dpp
* new diagnostics, switched by CLEO_DPPDIAG
*
* Revision 1.1.1.1  1998/02/06 19:11:47  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:43  clib
* Developmental version of DUET.
*
* Revision 1.3  1996/05/10 13:02:09  zfiles
* Copy MESSTK.
*
* Revision 1.2  1996/04/23 07:42:03  zfiles
* Add code for new stack variables.
*
* Revision 1.1.1.1  1994/10/08 01:00:56  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  13.51.45  by  Steve Schaffner
*CMZ :  5.54/03 11/03/91  13.35.28  by  Steve Schaffner
*CMZ :  5.54/00 04/12/90  17.02.52  by  Steve Schaffner
*CMZ :  5.53/07 08/08/90  16.45.47  by  D. Riley
*-- Author :
      SUBROUTINE TFPOPF(ISTAK,FIRST_NEXT,KFITIN)
C.......................................................................
C.
C. TFPOPF - Pops the first track from /TFSTAK/
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
#include "doit/duseq/tfctlpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfstakcd.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif
* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: tfpopf.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.6 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/10/02 21:11:42 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFPOPF' )

      INTEGER KFIT,KFITIN,ISTAK,FIRST_NEXT,ISTKTFLOC

* ----------Executable code starts here---------------------


      KFIT   = KFITIN

      IF(FIRST_NEXT.EQ.1)THEN
         ISTKTF(ISTAK) = IFSTK(ISTAK)
      ELSE
         ISTKTF(ISTAK) = NXTSTK(ISTKTF(ISTAK),ISTAK)
      ENDIF

      ISTKTFLOC=ISTKTF(ISTAK)

      IF (ISTKTFLOC .GT. 0) THEN

C Fill TFHITS and TFTRAK from track ISTKTF
C The parameters in the stack are stored in the /TFHITS/ order

          CALL UCOPY( PRMSTK(1,ISTKTFLOC,ISTAK), KAPCF , NPARTF )
          CALL UCOPY( ERMSTK(1,ISTKTFLOC,ISTAK), ERMFIT, NERMTF )

          NFIT = NHITTF - KFIT

          CALL UCOPY( IPLSTK(KFIT+1,ISTKTFLOC,ISTAK), IPLFIT, NFIT )
          CALL UCOPY( IADSTK(KFIT+1,ISTKTFLOC,ISTAK), IADFIT, NFIT )
          CALL UCOPY( SRCSTK(KFIT+1,ISTKTFLOC,ISTAK), SRCFIT, NFIT )
          CALL UCOPY( DWTSTK(KFIT+1,ISTKTFLOC,ISTAK), DWTFIT, NFIT )
          CALL UCOPY( SLRSTK(KFIT+1,ISTKTFLOC,ISTAK), SFIT  , NFIT )
          CALL UCOPY( PULSTK(KFIT+1,ISTKTFLOC,ISTAK), PULFIT, NFIT )
          CALL UCOPY( MESSTK(KFIT+1,ISTKTFLOC,ISTAK), MESFIT, NFIT )
          CALL UCOPY( SG1STK(KFIT+1,ISTKTFLOC,ISTAK), SG1FIT, NFIT )
          CALL UCOPY( SG2STK(KFIT+1,ISTKTFLOC,ISTAK), SG2FIT, NFIT )

C set the up-to-date flag in /TFHITS/ to indicate that the
C pointers from /TFHITS/ to /TRTRAK/ are corrupted
          
          UPTODATE_HIT=.FALSE.


#if defined(CLEO_DPPDIAG)
          PRINT 1001,ISTAK,ISTKTF(ISTAK),KFIT,NHITTF
 1001     FORMAT(' TFPOPF: ISTAK=',I2,
     2         20X,'  ISTKTF=',I4,' KFIT=',I4,' NHITTF=',I4)
      ELSE
          PRINT 1002,ISTAK,ISTKTF(ISTAK),KFIT
 1002     FORMAT(' TFPOPF: ISTAK=',I2,
     2         20X,'  ISTKTF=',I4,' KFIT=',I4)
#endif


      ENDIF
      END



