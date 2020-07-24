*
* $Id: doitclearhits.F,v 1.4 2000/05/15 18:31:05 pg Exp $
*
* $Log: doitclearhits.F,v $
* Revision 1.4  2000/05/15 18:31:05  pg
*    Changes to accomodate modifications to svscratch.inc .
*
* Revision 1.3  2000/03/28 15:59:30  pg
* Now filling Si common blocks
*
* Revision 1.2  2000/03/03 15:19:30  lyon
* Get the TDC right and add quality and window information
*
* Revision 1.1  1999/05/20 13:03:53  lyon
* Needed to pass hit information through fortran functions
*
* Revision 1.3  1999/05/11 19:11:47  lyon
* Uses all VZERO calls
*
* Revision 1.2  1999/05/05 21:11:08  lyon
* Replace loop with VZERO to make it faster
*
* Revision 1.1  1999/05/03 21:17:02  lyon
* Fortran/C++ communication
*
* Revision 1.1  1999/04/16 18:57:45  lyon
* for Fortran communication
*
*
      SUBROUTINE DOITCLEARHITS()
*
* begin_doc
*
*  Documentation for subroutine doitclearhits
*
*  Purpose: Clear the hit information common block
*  --------
*
*  Input Parameters:  None
*  ----------------
*
*  Output Parameters:  None
*  -----------------
*
*  Other routines:
*  ---------------
*
*  Notes:
*  ------
*
*  Author:   Adam Lyon      Created:  Thu Apr 15 13:36:02 EDT 1999
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
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"
#include "doit/sfseq/sfextra.inc"
#include "doit/duseq/svscratch.inc"
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
      PARAMETER(CHRCSS='$RCSfile: doitclearhits.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.4 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/05/15 18:31:05 $')
      PARAMETER(CHRCSA='$Author: pg $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITCLEARHITS' )
*
*  Local User defined variables:
*  -----------------------------
*
      INTEGER i
*
* end_var
*
*  Data statements:
*  ----------------
*
      SAVE RCSINI
*
      DATA RCSINI/.TRUE./
*
*  executable code for routine doitclearhits:
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

C*****Clear out arrays in SCRTCH

      NHTWCD = 0
        
      call vzero(ICYLCD, MHWCD)
      call vzero(IWIRCD, MHWCD)
      call vzero(TDWCD, MHWCD)
      call vzero(TCWCD, MHWCD)
      call vzero(QWCD, MHWCD)
      call vzero(PHIWCD, MHWCD)
      call vzero(DVHTCD, MHWCD)
      call vzero(DWCD, MHWCD)
      call vzero(XWCD, MHWCD)
      call vzero(YWCD, MHWCD)
      call vzero(EXNHCD, MHWCD)
      call vzero(EXRADC, MHWCD)
      call vzero(QUALCD, MHWCD)
      call vzero(EARLYCD, MHWCD)
      call vzero(LATECD, MHWCD)
      call vzero(NTDCCD, MHWCD)

      call vzero(MAPCD, KWS1CD)
      call vzero(EXFHCD, KLYRCD)

      SV_NHITS = 0

      call vzero(SV_TYPE,MHITSV)
      call vzero(SV_INDEX,MHITSV)
      call vzero(SV_PARENT,MHITSV)
      call vzero(SV_LAYER,MHITSV)
      call vzero(SV_HYBRID,MHITSV)
      call vzero(SV_QUALITY,MHITSV)
      call vzero(SV_X,MHITSV)
      call vzero(SV_Y,MHITSV)
      call vzero(SV_Z,MHITSV)
      call vzero(SV_PHI,MHITSV)
      call vzero(SV_PULSE,MHITSV)

      RETURN
      END



