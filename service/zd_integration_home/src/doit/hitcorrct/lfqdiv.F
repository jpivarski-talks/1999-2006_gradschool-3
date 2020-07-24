*
* $Id: lfqdiv.F,v 1.3 2003/03/05 17:49:33 dpp Exp $
*
* $Log: lfqdiv.F,v $
* Revision 1.3  2003/03/05 17:49:33  dpp
*      -> changed all variable names in cdscrtcd to have common root
*
* Revision 1.2  2001/11/19 23:48:50  dpp
*      -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:29  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:53  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 20/05/91  09.59.42  by  Steve Schaffner
*CMZ :  5.54/03 24/04/91  15.30.02  by  Steve Schaffner
*CMZ :  5.53/07 18/09/90  11.34.06  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  12.57.46  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.51.32  by  R. Namjoshi
*-- Author :
      SUBROUTINE LFQDIV( IADR, ILYR, YSAG, SH, ZH, IER )
C.......................................................................
C.
C. LFQDIV - Calculate S,Z values for charge division hits
C.
C. COMMON    : ?
C. CALLS     : ARCFUN TFXYTR
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ??-Mar-82
C. LAST MOD  : 23-Jul-87
C.
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C=======================================================================
C
C Calculate the (S,Z) values for wires with charge division
C
C Arguments:
C    IADR      Signed address of wire in /CDSCRT/
C    ILYR      Layer number
C    YSAG      Local wire sag
C    SH        S of hit on track
C    ZH        Z of hit on track
C    IER       IER = 0 => Found possible S-Z
C              IER > 0 => Impossible S-Z
C=======================================================================
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/tfgeomcd.inc"
      INTEGER IER,IADRW,IADR,IADRE,IBRNCH,ILYR
      REAL XH,YH,PHIH,SH,ARCFUN,ZH,YSAG

* ----------Executable code starts here---------------------

      IER    = 1
      IADRW  = IABS(IADR)
      IADRE  = EvntHit_NxtCls(IADRW)
      IF(IADRE.LE.0) RETURN

C== Find X, Y, PHI on outward branch

      IBRNCH = 1
      CALL TFXYTR( ALPCF, BETCF, KAPCF, GAMCF, RSQTF(ILYR),
     +             IBRNCH, XH, YH, PHIH, IER )
      IF(IER.NE.0) RETURN

C== Compute S and use Z computed earlier

      SH     = ARCFUN(KAPCF,ALPCF,BETCF,XH,YH,XICF)
      ZH     = EvntHit_Z(IADRW)

      RETURN
      END



