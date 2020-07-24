*
* $Id: findhw.F,v 1.7 2000/11/15 16:57:51 dpp Exp $
*
* $Log: findhw.F,v $
* Revision 1.7  2000/11/15 16:57:51  dpp
*      -> "CDHITS" changed to "ROADHITS" to make way for MC infringement
*
* Revision 1.6  2000/10/02 20:44:27  dpp
*      -> diagnostics
*      -> broaden search road with large entrance angle
*
* Revision 1.5  2000/08/18 18:49:26  dpp
*      -> change compilation switch to ROADHITSDIAG to limit outout
*
* Revision 1.4  2000/04/26 14:58:02  dpp
*      -> print statement
*
* Revision 1.3  1999/11/03 22:17:06  dpp
* remove ad-hoc block of layer 56
*
* Revision 1.2  1999/06/08 21:20:03  lyon
* Turn off projecting into cathodes until have geometry
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
      SUBROUTINE FINDHW(ILYR,PHI_IN,Z_IN,KAPRAD,IADR,NHWMAX,NHW )
C.......................................................................
C.
C. FINDHW - Search for hit wires near PHI_IN, Z_IN
C.
C. COMMON    : ?
C. CALLS     : None
C. CALLED    : ROADHITS
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 01-Apr-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
*--
* ILYR      = Layer number
* PHI_IN    = Expected PHI value
* Z_IN      = Expected ZEE value
* RAPRAD    = Kappa * Radius used to broaden search road
*

* IADR   = Addresses of hit wires found
* NHWMAX = Maximum # of hit wires calling program will accept
* NHW    = # of hit wires found
*--
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"

      INTEGER IADR(*),ILYR,NHWMAX,NHW,NSERF,IWIRF0,NWIRF,NWIRZ
      INTEGER ISERF,IWIRF,IWIRZ,IWADR
      REAL PHI_IN,Z_IN,KAPRAD,AKAPRAD,Z2PI

* ----------Executable code starts here---------------------


      NSERF  = SEARCH_LAYER(ILYR,SAV_HIT_TYP,SAV_FIT_TYP)

      AKAPRAD=ABS(KAPRAD)
      IF(AKAPRAD.GT. 0.90)THEN
         NSERF=NSERF+2
      ELSEIF(AKAPRAD.GT. 0.71)THEN
         NSERF=NSERF+1
      ENDIF

c NINT is nearest integer

      IWIRF0 = NINT(Z2PI(PHI_IN-PHIFTF(ILYR))/CELLTF(ILYR))
      NWIRF  = NWIRTF(ILYR)/NWRZTF(ILYR)
      NWIRZ  = NWRZTF(ILYR)

      NHW = 0

#if defined(CLEO_ROADHITSDIAG)
      print 8883,ilyr,kaprad,iwirf0,nserf,indxtf(ilyr)
 8883 format(' findhw: ilyr=',I3,' sine ent ang=',F6.2,
     2     ' search about wire',I5,' +/-',I3,
     3     ' INDXTF=',I6)
#endif

C R-Phi loop

      DO 10 ISERF = IWIRF0-NSERF, IWIRF0+NSERF
          IWIRF  = ISERF
          IF(IWIRF.LT.0)     IWIRF = IWIRF + NWIRF
          IF(IWIRF.GE.NWIRF) IWIRF = IWIRF - NWIRF

C Z loop

          DO 20 IWIRZ = 0, NWIRZ-1
              IWADR = INDXTF(ILYR) + IWIRF*NWIRZ + IWIRZ

              IF( IADRHW(IWADR).GT.0 ) THEN
                  NHW       = MIN0(NHW+1,NHWMAX)
                  IADR(NHW) = IADRHW(IWADR)

#if defined(CLEO_ROADHITSDIAG)
                  IF(IDVCTF(ILYR).EQ.IDRCT)THEN
                     print 8885,IWIRF,IWIRZ,IWADR,IADRHW(IWADR)
 8885                format(' findhw:            ',
     2                    'iwirf=',I3,' iwirz=',I4,
     3                    'IADRHW(',I6,')=',I4)
                  ENDIF
#endif

              ENDIF
20        CONTINUE

10    CONTINUE

      RETURN
      END



