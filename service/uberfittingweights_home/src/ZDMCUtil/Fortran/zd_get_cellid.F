      SUBROUTINE ZD_GET_CELLID( Wire, Layer, CellId )
C 
C  begin_doc
C 
C   Documentation for subroutine get_cellid
C 
C   Purpose: Calculate ZD CellId based on wire, layer
C   --------
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C 
C   Other routines:
C   ---------------
C 
C   Called By: mc_zd_responses.F
C   ----------
C
C   Notes:   Conversion Wire, Layer -> CellId (total wire #) 
C   ------
C 
C   Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
C   -------
C 
C   Detailed description:
C   ---------------------
C 
C   Major revisions:
C   ----------------
C      
C 
C  end_doc
C 
      IMPLICIT NONE
 
C  begin_inc
C   Include files :
C   ---------------------
C   Cleo3 Message Reporter
#include "ToolBox/report.inc"

#include "ZDMCUtil/Fortran/zdmc_param.inc"

C   cdgeom includes
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      integer Wire, Layer, CellId 

C 
C   Local User defined variables:
C   -----------------------------
C     iLayer - loop over layers
      integer iLayer

C     total_wires_layer - add up total # of wires up to this layer
      integer total_wires_layer(kNZDLayers)
      save    total_wires_layer

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_get_cellid.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:42 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_get_cellid' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine get_cellid:
#if WRITE_RCS_INFO
      if(rcsini)then
        rcsini = .false.
        write(6,*)'-------- CVSinfo --------'
        write(6,*)chrcss
        write(6,*)chrcsr//' '//chrcst
        write(6,*)chrcsd//' '//chrcsa
      endif
#endif
C ----6----------------------------------------------------------------72
 
      if (lfirst) then
        chmess='Calculate ZD CellId based on wire, layer'
        call report(MS_DEBUG, crname, chmess)
        chmess='Should use AZDSenseWireStore to get CellId'
        call report(MS_WARNING, crname, chmess)
        lfirst=.false.

C       Calc total number of wires for given layer
C       initialize 1st layer
        total_wires_layer(1) = NWIRCD(IXLRCD(1, IZDA))
C       loop over rest of layers 
        do iLayer = 2, kNZDLayers
          total_wires_layer(iLayer) =   total_wires_layer(iLayer-1)
     &                                + NWIRCD(IXLRCD(iLayer, IZDA))
        enddo

      endif

C     check for valid layer number
      if ( Layer.lt.0 .or. Layer.gt.kNZDLayers ) then
        write(chmess,*) 'Illegal layer =', Layer
        call report(MS_EMERGENCY, crname, chmess)
        chmess='Will Stop the program now'
        call report(MS_EMERGENCY, crname, chmess)
        stop
      endif

C     check for valid wire number in layer
      if ( Wire.lt.0 .or. Wire.gt.NWIRCD(Layer) ) then
        write(chmess,*) 'Illegal Wire =', Wire
        call report(MS_EMERGENCY, crname, chmess)
        chmess='Will Stop the program now'
        call report(MS_EMERGENCY, crname, chmess)
        stop
      endif

C     Initialize
      CellId = 0

C     Loop over layers to count up cellid's.
C     CellId from total # of wires in layer (1 -- Layer-1) + (Wire + 1) 
C     Note: wires start counting at 0, CellID at 1
      if ( Layer .gt. 1 ) then
        CellId = total_wires_layer(Layer-1) + Wire + 1
      else
        CellId = Wire + 1
      endif

C     check for valid CellId
      if ( CellId.lt.1 .or. CellId.gt.kNZDWires ) then
        write(chmess,*) 'Illegal CellId Calculated,  CellId =', CellId
        call report(MS_EMERGENCY, crname, chmess)
        write(chmess,*) 'layer =', Layer
        call report(MS_EMERGENCY, crname, chmess)
        write(chmess,*) 'Wire =', Wire
        call report(MS_EMERGENCY, crname, chmess)
        write(chmess,*) 'total_wires_layer(Layer)=',
     &                   total_wires_layer(Layer)
        call report(MS_EMERGENCY, crname, chmess)
        chmess='Will Stop the program now'
        call report(MS_EMERGENCY, crname, chmess)
        stop

#if defined(DEBUG_RESPONSE)
      else
c       for debugging
        write(chmess,*) 'Wire=', Wire,' Layer=',Layer, ' CellId=',CellId
        call report(MS_INFO, crname, chmess)       
#endif

      endif

      return
      end
