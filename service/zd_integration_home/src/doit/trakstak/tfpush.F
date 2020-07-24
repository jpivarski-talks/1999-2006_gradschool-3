*
* $Id: tfpush.F,v 1.7 2002/09/05 19:26:51 dpp Exp $
*
* $Log: tfpush.F,v $
* Revision 1.7  2002/09/05 19:26:51  dpp
*      -> add names for substa
*
* Revision 1.6  2002/05/23 14:30:41  dpp
*      -> stak is now structured as sub-staks in a global stak
*          rather than having the stak be multidimensioned
*
* Revision 1.5  2001/11/20 00:09:45  dpp
*      -> diagnostic
*      -> return stack location number
*         instead of requiring calling program to find it in common
*      -> remove error statistics
*
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
      SUBROUTINE TFPUSH(SUBSTAK,OUTP_REL_LOC)
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
      PARAMETER(CHRCSR='$Revision: 1.7 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2002/09/05 19:26:51 $')
      PARAMETER(CHRCSA='$Author: dpp $')

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFPUSH' )

      INTEGER KFIT,SUBSTAK,OUTP_REL_LOC

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      IF(NSTKTF(SUBSTAK).LT.LENSSTK(SUBSTAK))THEN

C put representation in to next location

         NSTKTF(SUBSTAK)= NSTKTF(SUBSTAK) + 1
         ISTKTF(SUBSTAK)= GSTKLOCL(SUBSTAK)-1+NSTKTF(SUBSTAK)
         OUTP_REL_LOC=    NSTKTF(SUBSTAK)
         KFIT=                0

C Prepare for sorting

         IF (NSTKTF(SUBSTAK) .EQ. 1) THEN
            IFSTK(SUBSTAK)= ISTKTF(SUBSTAK)
         ELSE
            NXTSTK(ISTKTF(SUBSTAK)-1)= ISTKTF(SUBSTAK)
         ENDIF
         NXTSTK(ISTKTF(SUBSTAK)) = 0

#if defined(CLEO_DPPDIAG)
         PRINT 1001,
     1        SUBSTAK,NAMSSTK(SUBSTAK),
     2        NSTKTF(SUBSTAK),ISTKTF(SUBSTAK),
     3        NFIT,NHIT3F,CHI3F
 1001    FORMAT(' TFPUSH:',
     1        ' SUBSTAK=',I2,3X,A15,
     2        '  NSTKTF=',I3,
     3        12X,
     4        '  ISTKTF=',I3,
     5        '    NFIT=',I3,
     6        '  NHIT3F=',I3,
     7        '   CHI3F=',F20.10)
#endif
         
         CALL TFUSTK(SUBSTAK,KFIT)

      ELSE
         OUTP_REL_LOC=0
      ENDIF
      RETURN
      END







