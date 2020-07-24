*
* $Id: doitgettrackinfo.F,v 1.4 2001/11/20 17:58:36 dpp Exp $
*
* $Log: doitgettrackinfo.F,v $
* Revision 1.4  2001/11/20 17:58:36  dpp
*      -> delete unsed nonexistant include file
*
* Revision 1.3  2001/01/05 05:37:43  lyon
* Added Doit status words
*
* Revision 1.2  1999/08/31 21:18:20  lyon
* Gets quality information
*
* Revision 1.1  1999/05/20 13:03:57  lyon
* Needed to pass hit information through fortran functions
*
* Revision 1.1  1999/05/03 21:17:03  lyon
* Fortran/C++ communication
*
* Revision 1.1  1999/04/16 18:57:47  lyon
* for Fortran communication
*
*
      SUBROUTINE DOITGETTRACKINFO( trknum, g_curcd, g_phi0cd, g_d0cd,
     &     g_tancd, g_z0cd, g_nhits, g_chi2, g_chiz, g_chi3, g_ndof, 
     &     g_resicd, g_kincd, g_stat1, g_stat2, g_stat3, g_stat4, ok )
*
* begin_doc
*
*  Documentation for subroutine doitgettrackinfo
*
*  Purpose: Get the information for this bunch
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
#include "doit/duseq/cdscrtcd.inc"
#include "doit/duseq/cdtrakcd.inc"
#include "doit/duseq/cfctlcde.inc"

* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      INTEGER trknum, g_nhits, g_ndof, g_kincd, ok
      INTEGER g_stat1, g_stat2, g_stat3, g_stat4
      REAL g_curcd, g_phi0cd, g_d0cd, g_tancd, g_z0cd, g_chi2, g_chiz,
     &     g_chi3, g_resicd
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitgettrackinfo.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.4 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2001/11/20 17:58:36 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITGETTRACKINFO' )
*
*  Local User defined variables:
*  -----------------------------
*
      INTEGER NWRITE, i
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
*  executable code for routine doitgettrackinfo:
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
      ok = 0

      if ( trknum .lt. 1 .or. trknum .gt. NTRKCD ) then
        ok = 1
        return
      endif
       
      i = trknum
      
      g_curcd = CURCD(i)
      g_phi0cd = PHI0CD(i)
      g_d0cd = D0CD(i)
      g_tancd = TANDCD(i)
      g_z0cd = Z0CD(i)
      g_nhits = NFITCD(i)
      g_chi2 = CHI2CD(i)
      g_chiz = CHIZCD(i)
      g_chi3 = CHI3CD(i)
      g_ndof = NDOFCD(i)
      g_resicd = RESICD(i)
      g_kincd = KINCD(i)

      g_stat1 = STATCD(1, i)
      g_stat2 = STATCD(2, i)
      g_stat3 = STATCD(3, i)
      g_stat4 = STATCD(4, i)

      return
      END



