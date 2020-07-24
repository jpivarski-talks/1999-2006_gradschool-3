*
* $Id: cdpri2.F,v 1.1.1.1 1998/02/06 19:11:40 dpp Exp $
*
* $Log: cdpri2.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:40  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:44  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:57  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/03 08/08/93  18.00.27  by  Rob Kutschke
*CMZ :  5.54/07 28/05/91  13.39.12  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.46.39  by  Steve Schaffner
*CMZ :  5.50/00 25/02/88  19.50.52  by  R. Namjoshi
*-- Author :
      SUBROUTINE CDPRI2( NRUN1, NRUN2 )
C.......................................................................
C.
C. CDPRI2 - Print new geometry for run NRUN
C.
C. COMMON    : ?
C. CALLS     :
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ?
C. LAST MOD  : 12-May-87
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfunitcd.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfctlcde.inc"

      INTEGER  IUN(2)
      INTEGER I,NRUN1,ISC,ILYR,J,NRUN2
C
* ----------Executable code starts here---------------------
#include "doit/duseq/duexed.inc"

      IUN(1) = IUNTTW
      IUN(2) = IUNLPT
      DO 10 I = 1, 2
      WRITE(IUN(I),100) NRUN1,NRUN2
      WRITE(IUN(I),105) MP3DTF,LKN6TF
      WRITE(IUN(I),110) (ISC,RSCATF(ISC),XSCATF(ISC),ISC=1     ,MSCATF)
      WRITE(IUN(I),115) (ISC,RSCCTF(ISC),XSCCTF(ISC),ISC=LSCCTF,MSCCTF)
      WRITE(IUN(I),120) (ILYR,(XX0TF(J,ILYR),J=1,3),PHIFTF(ILYR),
     +                        (UU0TF(J,ILYR),J=1,3),ILYR=1,NLYRTF)
      WRITE(IUN(I),130)
10    CONTINUE
C
      RETURN
C
100   FORMAT(/' New Geometry initialisation for runs',I7,' -',I7)
105   FORMAT(/' Number of helix parameters =',I4,'  => LKN6TF =',L2)
110   FORMAT(/' Discrete scattering surfaces:',
     +        '   ISCATF     RSCATF     XSCATF',/(33X,I6,2(5X,F6.3)))
115   FORMAT(/' Continuous scattering media: ',
     +        '   ISCCTF     RSCCTF     XSCCTF',/(33X,I6,2(5X,F6.3)))
120   FORMAT(/' Centre of device, rotation and direction cosines:'/,
     +        ' ILYR      X0TF      Y0TF      Z0TF    PHIFTF',
     +              '     UX0TF     UY0TF     UZ0TF',
     +       /(1X,I4,7(2X,F8.6)))
130   FORMAT(/)
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
