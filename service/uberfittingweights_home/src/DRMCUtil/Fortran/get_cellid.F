C 
C  $Id: get_cellid.F,v 1.5 2000/09/07 22:50:48 dubrovin Exp $
C 
C  $Log: get_cellid.F,v $
C  Revision 1.5  2000/09/07 22:50:48  dubrovin
C  Add save total_wires_layer instruction
C
C  Revision 1.4  2000/02/15 00:31:33  jjo
C  Improve documentation
C
C  Revision 1.3  1999/12/13 01:47:58  jjo
C  Use report for debug messages
C
C  Revision 1.2  1999/12/09 22:40:27  jjo
C  Remove excessive printouts
C
C  Revision 1.1  1999/12/08 00:15:38  jjo
C  Import into cvs
C
C 
      SUBROUTINE GET_CELLID( Wire, Layer, CellId )
C 
C  begin_doc
C 
C   Documentation for subroutine get_cellid
C 
C   Purpose: Calculate DR CellId based on wire, layer
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
C   Called By: mc_dr_responses.F
C   ----------
C
C   Notes:   Conversion Wire, Layer -> CellId (total wire #) 
C   ------
C 
C   Author:   John J O'Neill      Created:  Tue Dec  7 14:13:02 EST 1999
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

#include "DRMCUtil/Fortran/drmc_param.inc"

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
      integer total_wires_layer(kNDRLayers)
      save    total_wires_layer

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_cellid.F,v $')
      parameter(chrcsr='$Revision: 1.5 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2000/09/07 22:50:48 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'get_cellid' )
 
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
        chmess='Calculate DR CellId based on wire, layer'
        call report(MS_DEBUG, crname, chmess)
        chmess='Should use ADRSenseWireStore to get CellId'
        call report(MS_WARNING, crname, chmess)
        lfirst=.false.

C       Calc total number of wires for given layer
C       initialize 1st layer
        total_wires_layer(1) = NWIRCD(1)
C       loop over rest of layers 
        do iLayer = 2, kNDRLayers
          total_wires_layer(iLayer) =   total_wires_layer(iLayer-1)
     &                                + NWIRCD(iLayer)
        enddo

      endif

C     check for valid layer number
      if ( Layer.lt.0 .or. Layer.gt.kNDRLayers ) then
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
      if ( CellId.lt.1 .or. CellId.gt.kNDRWires ) then
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



