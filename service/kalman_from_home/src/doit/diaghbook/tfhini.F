*
* $Id: tfhini.F,v 1.2 2001/11/19 23:05:26 dpp Exp $
*
* $Log: tfhini.F,v $
* Revision 1.2  2001/11/19 23:05:26  dpp
*     -> delete unused include file
*
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:22  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:01  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 21/05/91  13.51.45  by  Steve Schaffner
*CMZ :  5.53/07 25/09/90  16.05.31  by  D. Riley
*CMZ :  5.51/03 27/09/89  11.55.39  by  Steve Schaffner
*CMZ :  5.51/00 18/08/89  15.38.10  by  Steve Schaffner
*CMZ :          28/06/89  12.58.07  by  Steve Schaffner
* Really truely switch to HBOOK4
*CMZ :          28/10/88  00.37.05  by  R. Namjoshi
* Switch to HBOOK4, use message logging
*CMZ :          24/10/88  13.13.48  by  Dan Riley
*CMZ :          20/09/88  14.46.59  by  Dan Riley
* Fix GETLUN calling sequence.
*-- Author :
      SUBROUTINE TFHINI
C.......................................................................
C.
C. TFHINI - Interface Duet histogramming to HBOOK
C.
C. COMMON    : ?
C. CALLS     : ?
C. CALLED    : Various places
C. AUTHOR    : M. Ogg. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.01
C. CREATED   : ?
C. LAST MOD  : 20-Sep-88
C.
C. Modification Log.
C.
C. 20-Dec-87 : Book histograms only if LWHIST is .TRUE.      R. Namjoshi
C. 20-Sep-88 : Fix GETLUN calling sequence.                          dsr
C. 10-Apr-89 : Remove blank common def. (now handled by CLEVER)      sfs
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfunitcd.inc"
#include "seq/clutil/mesrep.inc"

      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'TFHINI' )
*
* ----------Executable code starts here---------------------

*---
C--> INITIALISE HISTOGRAMMING
      IF(LWHIST) THEN
          CALL DUHBOK
C         open HBOOK output file, specified to be 'new'
      END IF

      RETURN
      END
