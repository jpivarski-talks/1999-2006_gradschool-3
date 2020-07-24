*
* $Id: cfxdmp.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: cfxdmp.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.4  1996/04/23 21:15:54  zfiles
* Comments from previous version wrong!
* True comment:
* 	Add new line at end of file.
*
* Revision 1.3  1996/04/23 07:28:54  zfiles
* Add new stack variables for kalman output.
*
* Revision 1.2  1995/03/02 17:47:43  zfiles
* New version fro DPP. flag changes so that DUET display would work.
*
*
C
C CFXFIT DUMP CONTROL
C
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/cfxdmpd.inc"
#endif
      INTEGER CFXFTD,CFXENT
      COMMON/CFXDCM/CFXFTD,CFXENT(4)
      SAVE  /CFXDCM/
