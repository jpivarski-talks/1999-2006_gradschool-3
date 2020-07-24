*
* $Id: tfustk.F,v 1.8 2002/09/05 19:27:17 dpp Exp $
*
* $Log: tfustk.F,v $
* Revision 1.8  2002/09/05 19:27:17  dpp
*      -> add names for substaks
*
* Revision 1.7  2002/05/23 14:31:01  dpp
*      -> stak is now structured as sub-staks in a global stak
*          rather than having the stak be multidimensioned
*
* Revision 1.6  2001/11/20 00:10:07  dpp
*      -> delete unused include file
*      -> diagnostic
*      -> keys are now set with NHIT3F,CHI3F instead of ..TF,
*      -> no longer correct hit number with number of charge division hits
*
* Revision 1.5  2000/10/02 21:11:24  dpp
*      -> change statcf(3) -> (4)
*      -> program indicator
*
* Revision 1.4  1999/12/09 21:24:56  dpp
* save the new de-weight in stak
*
* Revision 1.3  1999/08/03 22:00:04  dpp
* add index
*
* Revision 1.2  1999/04/27 20:18:16  dpp
* new diagnostics, switched by CLEO_DPPDIAG
*
* Revision 1.1.1.1  1998/02/06 19:11:47  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:43  clib
* Developmental version of DUET.
*
* Revision 1.3  1996/05/10 13:03:31  zfiles
* Copy MESSTK.
* Keep QDIV hits out of NHTKEY.
*
* Revision 1.2  1996/04/23 07:42:05  zfiles
* Add code for new stack variables.
*
* Revision 1.1.1.1  1994/10/08 01:00:56  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  13.51.45  by  Steve Schaffner
*CMZ :  5.54/03 16/05/91  22.13.55  by  Steve Schaffner
*CMZ :  5.54/00 07/01/91  18.36.44  by  Steve Schaffner
*CMZ :  5.53/07 08/08/90  16.45.47  by  D. Riley
*-- Author :
      SUBROUTINE TFUSTK(SUBSTAK,KFITIN)
C.......................................................................
C.
C. TFUSTK - Updates the current stack track from /TFHITS/
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
      PARAMETER(CHRCSS='$RCSfile: tfustk.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.8 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2002/09/05 19:27:17 $')
      PARAMETER(CHRCSA='$Author: dpp $')

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFUSTK' )
      INTEGER KFIT,KFITIN,SUBSTAK,GLOBAL_LOC

      INTEGER JBIT

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------

      IF (ISTKTF(SUBSTAK).EQ.0) RETURN
      KFIT= KFITIN
      GLOBAL_LOC=ISTKTF(SUBSTAK)

C--> Update track ISTKTF from TFHITS and TFTRAK
C    Copy the track parameters and errors directly

      IF(ABS(KAPCF).LT.1.E-9) KAPCF = 1.E-9
      PPTF   = -BMAGTF*PCONST*SQRT(1. + TANDLF**2)/KAPCF
      P2TF   = PPTF**2

C UNLESS WE KNOW BETTER........
      AM2TF  = AMPI2

      EETF   = SQRT(P2TF + AM2TF)
      CALL UCOPY( KAPCF , PRMSTK(1,GLOBAL_LOC), NPARTF )
      CALL UCOPY( ERMFIT, ERMSTK(1,GLOBAL_LOC), NERMTF )

C Put CFIND and TFIND 3D fit data in the stack (KFIT = 0)
C OR Put LFIND data after CFIND data in the stack (KFIT = NHITCF)

      CALL UCOPY( IPLFIT, IPLSTK(KFIT+1,GLOBAL_LOC), NFIT )
      CALL UCOPY( IADFIT, IADSTK(KFIT+1,GLOBAL_LOC), NFIT )
      CALL UCOPY( SRCFIT, SRCSTK(KFIT+1,GLOBAL_LOC), NFIT )
      CALL UCOPY( DWTFIT, DWTSTK(KFIT+1,GLOBAL_LOC), NFIT )
      CALL UCOPY( SFIT  , SLRSTK(KFIT+1,GLOBAL_LOC), NFIT )
      CALL UCOPY( PULFIT, PULSTK(KFIT+1,GLOBAL_LOC), NFIT )
      CALL UCOPY( MESFIT, MESSTK(KFIT+1,GLOBAL_LOC), NFIT )
      CALL UCOPY( SG1FIT, SG1STK(KFIT+1,GLOBAL_LOC), NFIT )
      CALL UCOPY( SG2FIT, SG2STK(KFIT+1,GLOBAL_LOC), NFIT )

      IF (NHITLF+NHTQLF.GT.0) THEN
          LFHKEY(GLOBAL_LOC) = 1
      ELSE
          LFHKEY(GLOBAL_LOC) = 0
      ENDIF

      NHTKEY(GLOBAL_LOC) = NHIT3F 
      CHIKEY(GLOBAL_LOC) = CHI3F

#if defined(CLEO_DPPDIAG)
      PRINT 1001,
     1     SUBSTAK,NAMSSTK(SUBSTAK),
     2     IFSTK(SUBSTAK),ISTKTF(SUBSTAK),
     3     NHITCF,NHITLF,
     2     CHIKEY(GLOBAL_LOC),NHTKEY(GLOBAL_LOC)
 1001 FORMAT(' TFUSTK:',
     1     ' SUBSTAK=',I2,3X,A15,
     2     12X,
     3     '   IFSTK=',I3,
     4     '  ISTKTF=',I3,
     5     '  NHITCF=',I3,
     6     '  NHITLF=',I3,
     7     '  CHIKEY=',F20.10,'  NHTKEY=',I4)
#endif

      RETURN
      END




