*
* $Id: cdcfid.F,v 1.2 2001/11/19 23:46:43 dpp Exp $
*
* $Log: cdcfid.F,v $
* Revision 1.2  2001/11/19 23:46:43  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:28  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:51  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 28/05/91  13.39.06  by  Steve Schaffner
*CMZ :  5.52/00 22/01/90  13.44.31  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.08  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.50.46  by  R. Namjoshi
*-- Author :
      SUBROUTINE CDCFID(CUR,PHI,D0,KAPPA,ALPHA,BETA,GAMMA,XI)
C.......................................................................
C.
C. CDCFID - Compute CUR, PHI, D0 given KAPPA, ALPHA etc.
C.
C. COMMON    : ?
C. CALLS     : ATN2PI
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 06-Apr-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
      REAL CUR,ALPHA,BETA,PHI,ATN2PI,D0,GAMMA,XI,KAPPA

* ----------Executable Code starts here---------------------

      CUR = KAPPA
      IF( ALPHA.NE.0.0 .AND. BETA.NE.0.0 ) THEN
          PHI = ATN2PI(ALPHA,-BETA )
      ELSE
          PHI = 0.0
      ENDIF

      D0  = 2.0*GAMMA/(1.0 + XI)

      RETURN
      END
