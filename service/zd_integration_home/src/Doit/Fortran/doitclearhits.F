*
* $Id: doitclearhits.F,v 1.5 2003/03/05 18:58:14 dpp Exp $
*
* $Log: doitclearhits.F,v $
* Revision 1.5  2003/03/05 18:58:14  dpp
*      -> remove sfseq/sfextra.inc
*      -> changed all variable names in cdscrtcd to have common root
*      -> add new variables
*
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
      PARAMETER(CHRCSR='$Revision: 1.5 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2003/03/05 18:58:14 $')
      PARAMETER(CHRCSA='$Author: dpp $')
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

      EvntHit_Num = 0
        
      call vzero(EvntHit_CDlayr,  EvntHitMax)
      call vzero(EvntHit_laWire,  EvntHitMax)
      call vzero(EvntHit_rawTDC,  EvntHitMax)
      call vzero(EvntHit_corTIM,  EvntHitMax)
      call vzero(EvntHit_rawTIM,  EvntHitMax)
      call vzero(EvntHit_Charge,  EvntHitMax)
      call vzero(EvntHit_rawADC,  EvntHitMax)
      call vzero(EvntHit_Drift,   EvntHitMax)
      call vzero(EvntHit_PhiWir,  EvntHitMax)
      call vzero(EvntHit_corPhi,  EvntHitMax)
      call vzero(EvntHit_X,       EvntHitMax)
      call vzero(EvntHit_Y,       EvntHitMax)
      call vzero(EvntHit_Z,       EvntHitMax)
      call vzero(EvntHit_Qual,    EvntHitMax)
      call vzero(EvntHit_TimEarly,EvntHitMax)
      call vzero(EvntHit_TimLate, EvntHitMax)
      call vzero(EvntHit_nMult,   EvntHitMax)

      call vzero(EvntHit_NHitDev, EvntHitMax)
      call vzero(EvntHit_MapDet,  KWS1CD)

      call vzero(EvntHit_OpnHit,  EvntHitMax)
      call vzero(EvntHit_PrimCls, EvntHitMax)
      call vzero(EvntHit_NxtCls,  EvntHitMax)
      call vzero(EvntHit_1stMlt,  EvntHitMax)
      call vzero(EvntHit_NxtMlt,  EvntHitMax)
      call vzero(EvntHit_PrimMlt, EvntHitMax)
C     EvntHit_UsePrim(i)  is filled later and is a logical

      call vzero(EvntHit_NxtLyr,  EvntHitMax)
      call vzero(EvntHit_1stLyr,  KLYRCD)
      call vzero(EvntHit_1stDev,  MXDVCD)


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



