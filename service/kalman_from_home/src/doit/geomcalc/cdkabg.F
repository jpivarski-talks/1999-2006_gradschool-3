*
* $Id: cdkabg.F,v 1.2 2001/11/19 23:47:05 dpp Exp $
*
* $Log: cdkabg.F,v $
* Revision 1.2  2001/11/19 23:47:05  dpp
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
*CMZ :  5.54/07 28/05/91  13.39.10  by  Steve Schaffner
*CMZ :  5.54/00 05/02/91  15.32.30  by  Steve Schaffner
*CMZ :  5.52/00 22/01/90  13.54.20  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.15  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.50.50  by  R. Namjoshi
*-- Author :
      SUBROUTINE CDKABG(CUR,PHI,D0,KAPPA,ALPHA,BETA,GAMMA,XI)
C.......................................................................
C.
C. CDKABG - Calculate KAPPA,ALPHA,BETA,GAMMA,XI from CUR,PHI,D0
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 14-Apr-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
      REAL KAPPA,XI,CUR,D0,ALPHA,PHI,BETA,GAMMA
C
* ----------Executable code starts here---------------------

      XI     =  (1.0 + 2.0*CUR*D0)
      ALPHA  =   XI * SIN(PHI)
      BETA   = - XI * COS(PHI)
      GAMMA  =   D0 * (1.0 + CUR*D0)
      KAPPA  =   CUR

      RETURN
      END
