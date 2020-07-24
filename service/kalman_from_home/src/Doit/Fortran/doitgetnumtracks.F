*
* $Id: doitgetnumtracks.F,v 1.2 2001/11/20 17:58:12 dpp Exp $
*
* $Log: doitgetnumtracks.F,v $
* Revision 1.2  2001/11/20 17:58:12  dpp
* -> delete unsed nonexistant include file
*
* Revision 1.1  1999/05/20 13:03:55  lyon
* Needed to pass hit information through fortran functions
*
* Revision 1.1  1999/05/03 21:17:03  lyon
* Fortran/C++ communication
*
* Revision 1.1  1999/04/16 18:57:47  lyon
* for Fortran communication
*
*
      SUBROUTINE DOITGETNUMTRACKS( ntracks )
*
* begin_doc
*
*  Documentation for subroutine doitgetnumtracks
*
*  Purpose: Get the number of tracks that were produced by DOIT
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
*  Author:   Adam Lyon      Created:  Thu Apr 15 15:29:57 EDT 1999
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
#include "doit/duseq/cdtrakcd.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      INTEGER ntracks
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitgetnumtracks.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.2 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2001/11/20 17:58:12 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITGETNUMTRACKS' )
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
*  executable code for routine doitgetnumtracks:
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
*

C*****Return the number of seed tracks found
      ntracks = ntrkcd

      return
      END



