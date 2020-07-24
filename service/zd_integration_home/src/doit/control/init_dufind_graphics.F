*
* $Id: init_dufind_graphics.F,v 1.1 2001/11/19 22:58:11 dpp Exp $
*
* $Log: init_dufind_graphics.F,v $
* Revision 1.1  2001/11/19 22:58:11  dpp
*      -> NEW ROUTINE
*      -> initialize graphics for higher level tracks
*
**
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : D Peterson
      SUBROUTINE INIT_DUFIND_GRAPHICS
C.......................................................................
C.
C. INIT_DUFIND_GRAPHICS - initialization of graphics for 
C.                        higher level tracks
C.
C.
C. COMMON    :
C. CALLS     :
C. CALLED    :
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.0
C. CREATED   : 15-Nov-2001
C.......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif


#if defined(CLEO_XTSUBS)
      CALL XTS_TRAK_INIT ('CS', 1,   1,'SFINIT')
      CALL XTS_TRAK_INIT ('CF', 1,   1,'SFINIT')
      CALL XTS_TRAK_INIT ('CD', 1,   1,'SFINIT')
#endif
      RETURN
      END



