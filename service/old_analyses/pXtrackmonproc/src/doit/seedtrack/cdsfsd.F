*
* $Id: cdsfsd.F,v 1.3 2001/11/20 00:08:11 dpp Exp $
*
* $Log: cdsfsd.F,v $
* Revision 1.3  2001/11/20 00:08:11  dpp
*      -> delete unused include file
*      -> delete unused variables
*
* Revision 1.2  2000/08/18 19:12:23  dpp
*      -> remove some obsolete variables
*
* Revision 1.1.1.1  1998/02/06 19:11:45  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:40  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/04/23 07:39:43  zfiles
* Initialize error matrix and new stack variables.
*
* Revision 1.1  1996/01/24 22:04:59  zfiles
* initial version: set up CF parameters from CS, for SF seeds
* different from CDRGSD: does not call VZERO(KAPCS or CDKABG(CURCS...
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D. Peterson
      SUBROUTINE CDSFSD
C.......................................................................
C.
C. CDRGSD - Fills the seed parameters for an SF track. (like CDSEED)
C.
C. COMMON    : ?
C. CALLS     : CDKABG UCOPY  VZERO
C. CALLED    : ?
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.10
C. CREATED   : 22-Dec-95
C. LAST MOD  :
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"

      REAL       CURMIN
      PARAMETER( CURMIN = 1.0E-9 )

* ----------Executable code starts here---------------------

C== Setup correct flags

      LSEED  = .FALSE.
      ITRKTF = 0

C== Find track parameters from info supplied
C== KAPCS etc. will have been VZERO'ed in SFNEXT
C    and then relavant values set in SFFTRK
C== SaVe versions will have been set in SFFTRK
 
      NFIT = NFITSV
      CALL UCOPY( IADFSV, IADFIT, NFIT )
      CALL UCOPY( IPLFSV, IPLFIT, NFIT )

C== STATSV may be changed in DULFTR after a failed CFIND

      CALL UCOPY( STATSV, STATCS, 4)

C== Setup defaults from found track parameters

C uncomment this line to reproduce dlib of 1/24/96
c      CALL CDKABG(CURCS,PHI0CS,D0CS,KAPCS,ALPCS,BETCS,GAMCS,XICS)

      CALL UCOPY( KAPCS, KAPCF, NPARTF )

      IF( ABS(KAPCF).LT.CURMIN ) KAPCF = SIGN(CURMIN,KAPCF)
      CHICF  = CHIDEF
      CHILF  = CHIDEF
      CHI3F  = CHIDEF
      CHITF  = CHIDEF
      PPTF   = -BMAGTF*PCONST*SQRT(1. + TANDLF**2)/KAPCF
      P2TF   = PPTF**2
      AM2TF  = AMPI2
      EETF   = SQRT(P2TF + AM2TF)

C==   Clear the rest of the stack and initialize the error matrix.

      CALL VZERO( ERMFIT, NERMTF )
      ERMFIT( 1) = ERRDEF
      ERMFIT( 3) = ERRDEF
      ERMFIT( 6) = ERRDEF
      ERMFIT(10) = ERRDEF
      ERMFIT(15) = ERRDEF
      IF ( NERMTF .GE. 21 ) ERMFIT(21) = ERRDEF
      IF ( NERMTF .GE. 28 ) ERMFIT(28) = ERRDEF

      RETURN
      END
