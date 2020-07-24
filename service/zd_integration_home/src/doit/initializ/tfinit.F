*
* $Id: tfinit.F,v 1.5 2003/03/05 18:04:06 dpp Exp $
*
* $Log: tfinit.F,v $
* Revision 1.5  2003/03/05 18:04:06  dpp
*      -> add call to dpp histogram initialization with compile switch
*      -> set the sf_data_access switch EvntHit_UsePrim to FALSE
*
* Revision 1.4  2002/09/05 19:11:28  dpp
*      -> remove unused NSRVTF, search width using VD seed
*      -> remove NTRTCD, some kind of count of the number of each KIND
*
* Revision 1.3  2001/11/19 23:59:54  dpp
*      -> remove diagdoit (old duet diagnostics) routines
*      -> delete unused include file
*      -> remove unused varaiables
*      -> remove error statistics
*
* Revision 1.2  2000/12/19 23:32:26  dpp
*      -> comment only
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :          24/05/88  11.18.39  by  D. Riley
* Rewrite the loops over device layers to remove bad assumptions about
* device contiguity and layer ordering.
*-- Author :
      SUBROUTINE TFINIT
C.......................................................................
C.
C. TFINIT - Initialize quantities for trackfinding
C.
C. COMMON    : ?
C. CALLS     : TFCLBD TFGMI1 VZERO
C. CALLED    :
C. AUTHOR    : D. Cassel. Converted to Fortran-77 by R. Namjoshi
C.
C. VERSION   : 1.00
C. CREATED   : ??-Apr-82
C. 1996/04/23 Initialize LUHTTF.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfconspa.inc"

#include "doit/duseq/cfctlcde.inc"
#include "doit/duseq/lfctlcde.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"

#include "doit/duseq/tfunitcd.inc"

* ----------Executable code starts here---------------------


C== Initialize geometry

      CALL TFGMI1
      CALL UCOPY(LSAVTF(1), LUSETF(1), KLYRTF)

C== Initialize all sigmas and weight
C==   DOIT_INIT_SIGMA requires TFGMI1 first

      CALL DOIT_INIT_SIGMA

C== initialize sf_data_access to delived the addressed hit, not prime
C== this feature is part of the multi-hit capabilities

      EvntHit_UsePrim=.FALSE.

#if defined(CLEO_DUFIND_ANALYSIS)
      CALL HISTI
#endif

      RETURN
      END



