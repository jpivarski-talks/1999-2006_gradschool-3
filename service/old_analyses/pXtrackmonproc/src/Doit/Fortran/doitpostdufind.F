*
* $Id: doitpostdufind.F,v 1.3 2001/11/20 17:59:00 dpp Exp $
*
* $Log: doitpostdufind.F,v $
* Revision 1.3  2001/11/20 17:59:00  dpp
* -> delete unsed nonexistant include file
*
* Revision 1.2  1999/05/28 03:17:52  lyon
* For post-dufind activities
*
* Revision 1.1  1999/05/20 13:03:59  lyon
* Needed to pass hit information through fortran functions
*
*
      SUBROUTINE DOITPOSTDUFIND
*
* begin_doc
*
*  Documentation for subroutine doitpostdufind
*
*  Purpose: Do the post-dufind duties. Like cddupl, cdcurl and cdkill
*  --------
*
*  Input Parameters:  (Name - Type - Meaning)
*  ----------------
*
*  Output Parameters:  (Name - Type - Meaning)
*  -----------------
*
*  Other routines:
*  ---------------
*
*  Notes:
*  ------
*
*  Author:   Adam Lyon      Created:  Wed May 19 14:00:49 EDT 1999
*  -------
*
*  Major revisions:
*  ----------------
*     
*
* end_doc
*
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfhistpa.inc"
#include "doit/duseq/tfconspa.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cfctlcde.inc"
#include "doit/duklseq/dutmp1.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitpostdufind.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.3 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2001/11/20 17:59:00 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITPOSTDUFIND' )
*
*  Local User defined variables:
*  -----------------------------
*
      INTEGER NWRITE
*
* end_var
*
*  Data statements:
*  ----------------
*
      SAVE RCSINI
      SAVE NWRITE
*
      DATA RCSINI/.TRUE./
      DATA NWRITE/0/
*
*  executable code for routine doitpostdufind:
#if WRITE_RCS_INFO
      IF(RCSINI)THEN
        RCSINI = .FALSE.
        WRITE(6,*)'-------- CVSinfo --------'
        WRITE(6,*)CHRCSS
        WRITE(6,*)CHRCSR//' '//CHRCST
        WRITE(6,*)CHRCSD//' '//CHRCSA
      ENDIF
#endif
*----6----------------------------------------------------------------72

      ntkmap = ntrkcd

C*****Clear the map array
      call VZERO(MAPO2N, NTKMAP)
      call VZERO(MAPCRL, 2*MTRAK2)

C*****Find duplicates
C     call cddupl()

C*****Find curlers
C      call cdcurl()

C*****Kill the bad parts
C      call cdkill()
      
      RETURN
      END



