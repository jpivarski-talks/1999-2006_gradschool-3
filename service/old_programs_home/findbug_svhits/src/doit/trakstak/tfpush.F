*
* $Id: tfpush.F,v 1.4 2000/10/02 21:13:31 dpp Exp $
*
* $Log: tfpush.F,v $
* Revision 1.4  2000/10/02 21:13:31  dpp
*      -> fix protection from overflowing stack
*
* Revision 1.3  1999/08/03 21:59:36  dpp
* add index
*
* Revision 1.2  1999/04/27 20:17:57  dpp
* new diagnostics, switched by CLEO_DPPDIAG
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
*CMZ :  5.54/07 21/05/91  14.04.18  by  Steve Schaffner
*CMZ :  5.54/00 07/01/91  18.11.39  by  Steve Schaffner
*CMZ :  5.53/07 08/08/90  16.45.47  by  D. Riley
*CMZ :  5.52/01 11/03/90  22.48.56  by  Steve Schaffner
*CMZ :  5.51/00 10/08/89  14.43.36  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.25  by  R. Namjoshi
*-- Author :
      SUBROUTINE TFPUSH(ISTAK)
C.......................................................................
C.
C. TFPUSH - Pushes the track in /TFHITS/ onto the stack
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
      PARAMETER(CHRCSS='$RCSfile: tfpush.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.4 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/10/02 21:13:31 $')
      PARAMETER(CHRCSA='$Author: dpp $')

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFPUSH' )

      INTEGER KFIT,ISTAK

* ----------Executable code starts here---------------------

      IF(NSTKTF(ISTAK).LT.NSTKDM)THEN

         NSTKTF(ISTAK) = NSTKTF(ISTAK) + 1
         ISTKTF(ISTAK) = NSTKTF(ISTAK)
         KFIT   = 0

#if defined(CLEO_DPPDIAG)
         PRINT 1001,ISTAK,NSTKTF(ISTAK),NFIT,NHITTF,CHITF
 1001    FORMAT(' TFPUSH: ISTAK=',I2,
     1          '  NSTKTF=',I4,' NFIT=',I4,
     2          ' NHITTF=',I4,'  CHITF=',F20.10)
#endif
         CALL TFUSTK(ISTAK,KFIT)
         
C--> Prepare for sorting

         IF (NSTKTF(ISTAK) .EQ. 1) THEN
            IFSTK(ISTAK)  = 1
         ELSE
            NXTSTK(NSTKTF(ISTAK)-1,ISTAK) = NSTKTF(ISTAK)
         ENDIF
         NXTSTK(NSTKTF(ISTAK),ISTAK) = 0


C--> Update the statistics

         NDMUSE(IERSTK) = MAX0(NSTKTF(ISTAK),NDMUSE(IERSTK))
         IF (NSTKTF(ISTAK) .GE. NSTKDM) THEN
            LERCTL(IERSTK) = .TRUE.
            NERCTL(IERSTK) = NERCTL(IERSTK) + 1
         ENDIF

      ENDIF
      RETURN
      END


