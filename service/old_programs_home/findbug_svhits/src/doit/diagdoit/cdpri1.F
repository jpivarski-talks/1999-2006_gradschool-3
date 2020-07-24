*
* $Id: cdpri1.F,v 1.3 2000/12/19 23:37:52 dpp Exp $
*
* $Log: cdpri1.F,v $
* Revision 1.3  2000/12/19 23:37:52  dpp
*      -> remove all references to LCSTDR,MCSTDR,MCTICD,LCTOCD (unused FORMAT)
*
* Revision 1.2  1998/07/09 00:25:45  lyon
* seq/cdgeom -> cl3seq/cdgm3
*
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
*CMZ :  5.54/14 07/01/92  12.46.31  by  Steve Schaffner
*CMZ :  5.51/00 09/08/89  15.33.20  by  Steve Schaffner
*CMZ :  5.50/00 13/05/89  17.04.44  by  Steve Schaffner
*-- Author :
      SUBROUTINE CDPRI1
C     The Formats are messed up.  Fix them if you want to.
C.......................................................................
C.
C. CDPRI1 - Print geometry at job start
C.
C. COMMON    : ?
C. CALLS     :
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 25-Feb-88
C.
C. Modification Log.
C.
C. 25-Feb-88 : Remove +SEQ,TFTIME.                           R. Namjoshi
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
C
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlpar.inc"
C
#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfunitcd.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "seq/clutil/mesrep.inc"
      INTEGER NWIR,NWIRET,NQDVLR,ILYR,ILIN,NTODO,LTHIS,NTHIS,MTHIS,I

* ----------Executable code starts here---------------------
C
C== Make consistency checks
C
      NWIR   = 0
      NWIRET = 0
      NQDVLR = 0
      DO 10 ILYR = 1, NLYRTF
          NWIR   = NWIRTF(ILYR)
          IF(LQDVTF(ILYR)) THEN
              NWIR   = 2*NWIR
              NQDVLR = NQDVLR + 1
          ENDIF
          NWIRET = NWIRET + NWIR
10    CONTINUE
C
C== Print the TFIND geometry
C
      WRITE (IUNLPT,110)
      DO 20 ILYR = LLYRCF, MLYRCF
          WRITE (IUNLPT,115) ILYR,DEVNAM(IDVCTF(ILYR)),
     +                       IXSFTF(ILYR),ILCRTF(ILYR),
     +                       LUSETF(ILYR),LU3DTF(ILYR),LQDVTF(ILYR),
     +                       RENDTF(ILYR),RTF   (ILYR),RSQTF (ILYR),
     +                       ZENDTF(ILYR),
     +                       NWIRTF(ILYR),INDXTF(ILYR),NSRHTF(ILYR),
     +                       CELLTF(ILYR),SIGQTF(ILYR)
20    CONTINUE
C
      WRITE (IUNLPT,120)
      DO 30 ILYR = LLYRLF, MLYRLF
          WRITE (IUNLPT,125) ILYR,DEVNAM(IDVCTF(ILYR)),
     +                       IXSFTF(ILYR),ILCRTF(ILYR),
     +                       LUSETF(ILYR),LU3DTF(ILYR),LQDVTF(ILYR),
     +                       RENDTF(ILYR),RTF   (ILYR),
     +                       ZENDTF(ILYR),
     +                       NWIRTF(ILYR),NWRZTF(ILYR),
     +                       INDXTF(ILYR),NSRHTF(ILYR),
     +                       CELLTF(ILYR),
     +                       CELZTF(ILYR),SIGHTF(ILYR),SIGQTF(ILYR),
     +                       TANSTF(ILYR)
30    CONTINUE
C
      WRITE (IUNLPT,130) NWIRET
      IF( NWIRET.NE.MWIRCD ) THEN
          WRITE (IUNTTW,180) NWIRET,MWIRCD
          WRITE (IUNLPT,180) NWIRET,MWIRCD
          CHMESS = 'Something horrible happened to the DUET geometry'
          CALL MESLOG('CDPRI1',1,MSABOR)
      ENDIF
C
      IF( NQDVLR.NE.NQDVCD ) THEN
          WRITE(IUNTTW,185) NQDVLR,NQDVCD
          WRITE(IUNLPT,185) NQDVLR,NQDVCD
          CHMESS = 'Something horrible happened to the DUET geometry'
          CALL MESLOG('CDPRI1',1,MSABOR)
      ENDIF
C
C== Print the Chi2 array
C
      ILIN   = 20
      NTODO  = NLYRTF
      LTHIS  = 1
40    IF( NTODO.GT.0 ) THEN
          NTHIS  = MIN0(NTODO,ILIN)
          MTHIS  = LTHIS + NTHIS - 1
          WRITE(IUNLPT,145) (I        ,I=LTHIS,MTHIS)
          WRITE(IUNLPT,146) (CH2SCL(I),I=LTHIS,MTHIS)
          LTHIS  = MTHIS + 1
          NTODO  = NTODO - ILIN
          GOTO 40
      ENDIF

      RETURN
C
  110 FORMAT(/' TFINIT : PRINTS THE GEOMETRY WITH LAYERS APPEARING',
     +        ' IN TFIND ORDER :'/
     +       /'   ','ILYR DEVIC XSF LCR USE U3D QDV',
     +                '  RENDTF     RTF   RSQTF  ZENDTF',
     +            ' NWIR   INDX NSRC  CELLTF  SGQDIV'/)
  115 FORMAT( ' CF',I4,2X,A4,2I4,3L4,4F8.4,I5,I7,I5,
     +         F8.4,6P,F7.0,0P,F7.3)
  120 FORMAT(/'0  ','ILYR DEVIC XSF LCR USE U3D QDV',
     +                '  RENDTF     RTF  ZENDTF',
     +            ' NWIR NWZ  INDX NS CELLTF',
     +            '   CELZTF SGQDIV   TANSTF'/)
  125 FORMAT( ' LF',I4,2X,A4,2I4,3L4,3F8.4,I5,I4,I6,I3,F7.4,
     +         6P,F6.0,0P,F9.5,6P,F7.0,0P,F7.3,F9.5)
  130 FORMAT(76X,'-----'/55X,'TOTAL # OF WIRES = ',I7)
  145 FORMAT(/' NDF:   ',20I5)
  146 FORMAT( ' SCALE: ',20F5.2)
  149 FORMAT(32X,2F12.3)
  170 FORMAT(/'0? : TFINIT : FATAL ERR :',
     +        ' (LCANIZ .NE. LLYRCF) .OR. (MCAXDR .NE. MLYRCF)'/
     +        '                         ',
     +        '  ',I6,'      ',I6,'        ',I6,'      ',I6)
  180 FORMAT(/'0? : TFINIT : FATAL ERR : TOTAL # OF WIRES = ',I6/
     +      /12X,'.NE. MWIRCD = ',I6/)
  185 FORMAT(/'0? : TFINIT : FATAL ERR : TOTAL # OF Q DIV LAYERS = ',I6/
     +      /12X,'.NE. NQDVCD = ',I6/)
200   FORMAT(/' Error in geometry in TFINIT. Inconsistency with',
     +        ' RSCATF & RTF.',
     +       /' ILYR:',I4,'  RTF: ',F6.3,'  RINNER: ',F6.3,
     +        '  ROUTER: ',F6.3,'  RSCAT: ',F6.3/' ABORT.')
C
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
