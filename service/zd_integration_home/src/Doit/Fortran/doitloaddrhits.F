*
* $Id: doitloaddrhits.F,v 1.6 2003/03/05 18:57:57 dpp Exp $
*
* $Log: doitloaddrhits.F,v $
* Revision 1.6  2003/03/05 18:57:57  dpp
*      -> fill new cdscrtcd variable
*      -> remove sfseq/sfextra.inc
*      -> changed all variable names in cdscrtcd to have common root
*      -> fill Evnt_OpnHit = 1
*      -> add new variables
*
* Revision 1.5  2000/03/03 15:19:30  lyon
* Get the TDC right and add quality and window information
*
* Revision 1.4  1999/12/01 22:15:00  lyon
* allow for gaps in identifier numbers
*
* Revision 1.3  1999/11/19 15:15:22  lyon
* Put tdc into EvntHit_rawTDC instead of EvntHit_corTIM -- oops
*
* Revision 1.2  1999/11/18 22:33:16  lyon
* tdc is a Integer, not a Real (duh)
*
* Revision 1.1  1999/05/20 13:03:58  lyon
* Needed to pass hit information through fortran functions
*
* Revision 1.1  1999/05/03 21:17:04  lyon
* Fortran/C++ communication
*
* Revision 1.1  1999/04/16 18:57:48  lyon
* for Fortran communication
*
*
      SUBROUTINE DOITLOADDRHITS(device,
     &     layer, wire, tdc, time,
     &     charge, phi, distance, xpos, ypos, wir_adr, qual,
     &     early, late, ntdc, o_idx)
*
* begin_doc
*
*  Documentation for subroutine doitloaddrhits
*
*  Purpose:
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
*  Author:   Adam Lyon      Created:  Thu Apr 15 14:08:16 EDT 1999
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
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      INTEGER device
      INTEGER layer, wire, hitnum, wir_adr
      INTEGER fh, tdc, o_idx, qual, ntdc
      REAL time, charge, phi, distance, xpos, ypos, early, late
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitloaddrhits.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.6 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2003/03/05 18:57:57 $')
      PARAMETER(CHRCSA='$Author: dpp $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITLOADDRHITS' )
*
*  Local User defined variables:
*  -----------------------------
      INTEGER i
      INTEGER cdlayer
      INTEGER cd_cell_id
*
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
*  executable code for routine doitloaddrhits:
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
      EvntHit_Num=EvntHit_Num + 1

      if (EvntHit_Num .gt. EvntHitMax ) then
        CHMESS='Doit -- Number of hits exceeded EvntHitMax!!'
        CALL MESLOG(CRNAME, 1, MSEMER)
        STOP
      endif
        
      i=EvntHit_Num
      cdlayer=layer + (ILDVCD(1,device)-1)
      cd_cell_id=wir_adr + (INDXCD(ILDVCD(1,device))-1)

C*****Copy hit information into the common block

      EvntHit_CDlayr(EvntHit_Num)   = cdlayer
      EvntHit_laWire(EvntHit_Num)   = wire
      EvntHit_rawTDC(EvntHit_Num)   = tdc
      EvntHit_corTIM(EvntHit_Num)   = time
      EvntHit_rawTIM(EvntHit_Num)   = time
      EvntHit_Charge(EvntHit_Num)   = charge
C     EvntHit_rawADC(EvntHit_Num)   is not filled
      EvntHit_Drift(EvntHit_Num)    = distance
      EvntHit_PhiWir(EvntHit_Num)   = phi
C     EvntHit_corPhi(EvntHit_Num)   is filled later and can be track dependent
      EvntHit_X(EvntHit_Num)        = xpos
      EvntHit_Y(EvntHit_Num)        = ypos
C     EvntHit_Z(EvntHit_Num)        is filled later
      EvntHit_Qual(EvntHit_Num)     = qual
      EvntHit_TimEarly(EvntHit_Num) = early
      EvntHit_TimLate(EvntHit_Num)  = late
      EvntHit_nMult(EvntHit_Num)    = ntdc 


      EvntHit_NHitDev(EvntHit_Num)  = EvntHit_Num 
      EvntHit_MapDet(cd_cell_id)= EvntHit_Num

C*****new variables for multihit electronics
C     set for single hit

      EvntHit_OpnHit(EvntHit_Num)   = 7
      EvntHit_PrimCls(EvntHit_Num)  = 0
      EvntHit_NxtCls(EvntHit_Num)   = 0
      EvntHit_1stMlt(EvntHit_Num)   = EvntHit_Num
      EvntHit_NxtMlt(EvntHit_Num)   = 0
      EvntHit_PrimMlt(EvntHit_Num)  = 0
C     EvntHit_UsePrim(EvntHit_Num)  is filled later


C*****return the current index      
      o_idx = EvntHit_Num

C fill the SF link list        
      fh = EvntHit_1stLyr(cdlayer)
C Is there already a first hit in the layer, move it out of the way
      if ( fh .ne. 0 ) then
        EvntHit_NxtLyr(EvntHit_Num) = fh 
      endif
      EvntHit_1stLyr(cdlayer) = EvntHit_Num

C is this the first instance of this device ?
      IF(EvntHit_1stDev(device).eq.0)THEN
        EvntHit_1stDev(device)=EvntHit_Num
      ENDIF

            
      RETURN
      END



