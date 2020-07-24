*
* $Id: doitnewsveff.F,v 1.1 2001/04/05 02:04:24 lyon Exp $
*
* $Log: doitnewsveff.F,v $
* Revision 1.1  2001/04/05 02:04:24  lyon
* Oops - forgot this
*
*
      SUBROUTINE DOITNEWSVEFF(nHybrids, phiEff, zEff)
*
* begin_doc
*
*  Documentation for subroutine doitnewsveff
*
*  Purpose: Fill some doit parameters
*  --------
*
*  Input Parameters:  (Name - Type - Meaning)
*  ----------------
*        run = current run number
*        imode  = Doit mode number (e.g. Doit, DoitFast, etc)
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
*  Author:   Adam Lyon      Created:  Fri May 21 11:23:09 EDT 1999
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
#include "doit/duseq/tfgeomcd.inc"      
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/svgeometry.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
*
      INTEGER nHybrids
      REAL phiEff(*), zEff(*)

*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitnewsveff.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.1 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2001/04/05 02:04:24 $')
      PARAMETER(CHRCSA='$Author: lyon $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITNEWSVEFF' )
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
*  executable code for routine doitnewsveff:
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

      do i=1, nHybrids
        sv_hybrid_eff(i,1) = phiEff(i)
        sv_hybrid_eff(i,2) = zEff(i)
      enddo


      RETURN
      END



