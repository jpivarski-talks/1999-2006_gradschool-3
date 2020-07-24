*
* $Id: lfctht.F,v 1.4 1999/11/09 21:39:29 dpp Exp $
*
* $Log: lfctht.F,v $
* Revision 1.4  1999/11/09 21:39:29  dpp
* remove print statemet which would have been annoying
* it it had actually gotten there without crashing
*
* Revision 1.3  1999/11/04 17:21:58  dpp
* fix cathode include file directory
*
* Revision 1.2  1999/11/03 22:48:34  dpp
* obtain clustered cathode information from new cathode storage
* test for cathode layer useing flag rather than layer number
*
* Revision 1.1.1.1  1998/02/06 19:11:43  dpp
* DOIT first release.
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :
      SUBROUTINE LFCTHT( IADR, ILYR, YSAG, SH, ZH, IER )
C.......................................................................
C.
C. LFCTHT - Calculate S,Z values for cathode strips
C.
C. COMMON    : ?
C. CALLS     : ARCFUN TFXYTR
C. CALLED    : ?
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 23-Nov-88
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
C Calculate the (S,Z) values for the cathode strips
C  Arguments:
C    IADR      Signed address of wire in /CDSCRT/
C    ILYR      Layer number
C    YSAG      Local wire sag
C    SH        S of hit on track
C    ZH        Z of hit on track
C    IER       IER = 0 => Found possible S-Z
C              IER > 0 => Impossible S-Z
C=======================================================================
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"

#include "CathodeFortranAccess/cathclus.inc"
#include "CathodeFortranAccess/clustopad.inc"
#include "CathodeFortranAccess/cathpads.inc"
#include "CathodeFortranAccess/padtoclusll.inc"


C== Argument declarations

      INTEGER IADR, ILYR, IER
      REAL    YSAG, SH, ZH

      INTEGER IBRNCH,INEXT
      REAL XH,YH,PHIH,ARCFUN,ZSUM,QSUM

* ----------Executable code starts here---------------------

C== Get X, Y, PHI from track parameters and radius

      IBRNCH = 1
      CALL TFXYTR( ALPCF, BETCF, KAPCF, GAMCF, RSQTF(ILYR),
     +             IBRNCH, XH, YH, PHIH, IER )
      IF(IER.NE.0) RETURN

      ZH=CLUSZ(IADR)
      QSUM=CLUSCH(IADR)

C== Compute S

C Use this Z to add tilt AND translation to SH

      IF(.NOT.LMCRLO)THEN
         XH     = XH +XX0TF(1,ILYR) + ZH*UU0TF(1,ILYR)
         YH     = YH +XX0TF(2,ILYR) + ZH*UU0TF(2,ILYR)
      ENDIF
      SH     = ARCFUN( KAPCF, ALPCF, BETCF, XH, YH, XICF )

      RETURN
      END




