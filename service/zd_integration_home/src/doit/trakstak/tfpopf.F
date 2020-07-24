*
* $Id: tfpopf.F,v 1.9 2002/09/05 19:28:00 dpp Exp $
*
* $Log: tfpopf.F,v $
* Revision 1.9  2002/09/05 19:28:00  dpp
*      -> add names for substaks
*
* Revision 1.8  2002/05/23 14:31:24  dpp
*      -> stak is now structured as sub-staks in a global stak
*          rather than having the stak be multidimensioned
*
* Revision 1.7  2001/11/20 00:09:06  dpp
*      -> delete unused include file
*      -> return stack location number
*         instead of requiring calling program to find it in common
*
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
      SUBROUTINE TFPOPF(SUBSTAK,FIRST_NEXT,KFITIN,OUTP_REL_LOC)
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
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tfstakcd.inc"
#include "doit/duseq/controlpar.inc"
#if defined(CLEO_XTSUBS)
#include "doit/sfseq/xts_cntl.inc"
#endif
* -------------------- Local    declarations ---------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: tfpopf.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.9 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2002/09/05 19:28:00 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFPOPF' )

      INTEGER KFIT,KFITIN,SUBSTAK,FIRST_NEXT
      INTEGER GLOBAL_LOC,OUTP_REL_LOC

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


      KFIT   = KFITIN

      IF(FIRST_NEXT.EQ.STAK_PICK_FIRST)THEN
         ISTKTF(SUBSTAK) = IFSTK(SUBSTAK)
      ELSE
         ISTKTF(SUBSTAK) = NXTSTK(ISTKTF(SUBSTAK))
      ENDIF

      GLOBAL_LOC=ISTKTF(SUBSTAK)

      IF (GLOBAL_LOC .GT. 0) THEN
      OUTP_REL_LOC=GLOBAL_LOC-(GSTKLOCL(SUBSTAK)-1)

C Fill TFHITS and TFTRAK from track ISTKTF
C The parameters in the stack are stored in the /TFHITS/ order

         CALL UCOPY( PRMSTK(1,GLOBAL_LOC), KAPCF , NPARTF )
         CALL UCOPY( ERMSTK(1,GLOBAL_LOC), ERMFIT, NERMTF )

         NFIT = NHITTF - KFIT
         
         CALL UCOPY( IPLSTK(KFIT+1,GLOBAL_LOC), IPLFIT, NFIT )
         CALL UCOPY( IADSTK(KFIT+1,GLOBAL_LOC), IADFIT, NFIT )
         CALL UCOPY( SRCSTK(KFIT+1,GLOBAL_LOC), SRCFIT, NFIT )
         CALL UCOPY( DWTSTK(KFIT+1,GLOBAL_LOC), DWTFIT, NFIT )
         CALL UCOPY( SLRSTK(KFIT+1,GLOBAL_LOC), SFIT  , NFIT )
         CALL UCOPY( PULSTK(KFIT+1,GLOBAL_LOC), PULFIT, NFIT )
         CALL UCOPY( MESSTK(KFIT+1,GLOBAL_LOC), MESFIT, NFIT )
         CALL UCOPY( SG1STK(KFIT+1,GLOBAL_LOC), SG1FIT, NFIT )
         CALL UCOPY( SG2STK(KFIT+1,GLOBAL_LOC), SG2FIT, NFIT )

C set the up-to-date flag in /TFHITS/ to indicate that the
C pointers from /TFHITS/ to /TRTRAK/ are corrupted
          
         UPTODATE_HIT=.FALSE.


#if defined(CLEO_DPPDIAG)
         PRINT 1001,
     1        SUBSTAK,NAMSSTK(SUBSTAK),
     2        OUTP_REL_LOC,
     3        IFSTK(SUBSTAK),ISTKTF(SUBSTAK),KFIT,NHITTF
 1001    FORMAT(' TFPOPF:',
     1        ' SUBSTAK=',I2,3X,A15,
     2        ' REL_LOC=',I3,
     3        '   IFSTK=',I3,
     4        '  ISTKTF=',I3,
     5        '    KFIT=',I3,
     6        '  NHITTF=',I3)
#endif

      ELSE
         OUTP_REL_LOC=0

#if defined(CLEO_DPPDIAG)
         PRINT 1002,
     1        SUBSTAK,NAMSSTK(SUBSTAK),
     2        OUTP_REL_LOC,
     3        IFSTK(SUBSTAK),ISTKTF(SUBSTAK),KFIT
 1002    FORMAT(' TFPOPF:',
     1     ' SUBSTAK=',I2,3X,A15,
     2        ' REL_LOC=',I3,
     3        '   IFSTK=',I3,
     4        '  ISTKTF=',I3,
     5        '    KFIT=',I3)
#endif
         

      ENDIF
      RETURN
      END



