*
* $Id: tfxyht.F,v 1.3 2003/03/05 17:49:10 dpp Exp $
*
* $Log: tfxyht.F,v $
* Revision 1.3  2003/03/05 17:49:10  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.2  2001/11/19 23:48:31  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.2  1996/07/08 21:50:30  dpp
* use TFCTHT for z-side silicon hits
*
* Revision 1.1.1.1  1994/10/08 01:00:53  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author : R. NAMJOSHI
      SUBROUTINE TFXYHT(IADR,ILYR,YSAG,XF,YF,IERR)
C......................................................................
C.
C. TFXYHT - FILL XF, YF WITH X,Y LOCATION OF THE WIRE
C.
C. COMMON    : 
C. CALLS     : LFCTHT
C. CALLED    : 
C. AUTHOR    : R. Namjoshi
C. VERSION   : 1.10
C. CREATED   : 25-Feb-88
C. LAST MOD  :
C.
C. Modification Log.
C.
C. 5/03/96 ADF4: ADDED ISVZ1 TO CATHODE CALL LIST TO FILL SILICON Z
C.*CMZ :  5.54/07 21/05/91  13.51.42  by  Steve Schaffner
C.*CMZ :  5.53/04 18/06/90  16.44.39  by  Steve Schaffner
C.*CMZ :  5.51/00 11/08/89  14.23.06  by  Steve Schaffner
C.*CMZ :  5.50/00 25/02/88  19.51.52  by  R. Namjoshi
C......................................................................



#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
      INTEGER JADR,IADR,IERR,ILYR
      REAL XF,YF,YSAG,SF,ZF

* ----------Executable code starts here---------------------

      JADR   = IABS(IADR)
      XF     =  EvntHit_X(JADR)
      YF     =  EvntHit_Y(JADR)
      IERR   = 0


      IF (    IDVCTF(ILYR).EQ.IVDCT 
     2    .OR.IDVCTF(ILYR).EQ.IDRCT
     3    .OR.IDVCTF(ILYR).EQ.ISVZ1) THEN

          CALL LFCTHT(JADR,ILYR,YSAG,SF,ZF,IERR)

	  IF (IERR.EQ.0) THEN
                XF     = SF
                YF     = ZF
                 EvntHit_Drift(JADR) = ZF
	   ELSE
	        PRINT *,'Error in LFCTHT: ',IERR
	   ENDIF
      ENDIF


      RETURN
      END






