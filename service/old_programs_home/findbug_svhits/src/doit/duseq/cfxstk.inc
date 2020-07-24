*
* $Id: cfxstk.inc,v 1.1.1.1 1998/02/06 19:11:48 dpp Exp $
*
* $Log: cfxstk.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:48  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.2  1995/03/02 17:47:44  zfiles
* New version fro DPP. flag changes so that DUET display would work.
*
*
C
C CFXFIT STACK FOR HOLDING RESIDUALS SEEN IN CFXFIT
C
#if !defined(CLEO_NOCOMDOC)
#include "doit/duseq/cfxstkd.inc"
#endif
      INTEGER MCFXFS,MCFXFL,MCFXFH
      PARAMETER (MCFXFS=200)
      PARAMETER (MCFXFL=16)
      PARAMETER (MCFXFH=2)
      INTEGER CFXSNU
      INTEGER CFXSHT,CFXSFL
      REAL    CFXSMS,CFXSCL
      COMMON/CFXSCM/CFXSNU(MCFXFS,MCFXFL),
     1              CFXSHT(MCFXFS,MCFXFL,MCFXFH),
     2              CFXSFL(MCFXFS,MCFXFL,MCFXFH),
     3              CFXSMS(MCFXFS,MCFXFL,MCFXFH),
     4              CFXSCL(MCFXFS,MCFXFL,MCFXFH)
      SAVE /CFXSCM/
