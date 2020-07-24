C 
C  $Id: get_layer_eff.F,v 1.2 1999/10/26 18:36:35 jjo Exp $
C 
C  $Log: get_layer_eff.F,v $
C  Revision 1.2  1999/10/26 18:36:35  jjo
C  Call get_deadwire for dead wires istead of hard coding them
C
C  Revision 1.1  1999/10/20 21:50:28  jjo
C  Import routines for DR MC response simulation
C
C 
      SUBROUTINE GET_LAYER_EFF( Update,
     &                          Index,
     &                          Layer_Eff)
C 
C  begin_doc
C 
C   Documentation for subroutine get_layer_eff
C 
C   Purpose: Return average anode wire efficiency for a given DR3 layer
C   -------- Update=.true. updates constants derived from constants
C            stored in Cleo3 Frame
C
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = true -> update constants at begin run time
C   Index - integer - index into constants array
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   Layer_Eff - Efficiency for a given DR3 layer - averaged over
C               all drift distances & cos(theta)
C
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:
C   ------
C 
C   Author:   John J O'Neill      Created:  Mon Sep 27 22:35:29 EDT 1999
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

C cleo3 Geometry parameters
#include "cl3seq/cdgm3/cdgeompa.inc"
C cleo3 Geometry 
#include "cl3seq/cdgm3/cdgeomcd.inc"

C   parameters including array sizes
#include "DRMCUtil/Fortran/drmc_param.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      logical Update
      integer index
      real Layer_Eff
C 
C   Local User defined variables:
C   -----------------------------
C     WireEffArray - holds eff for each wire in DR3
      real WireEffArray(kWireEffSize)

C   Ilayer - loop over DR layers
      INTEGER Ilayer

C   nWireLiveLayer - number of live wires in this layer
      INTEGER nWireLiveLayer

C   SumEffLayer - sum of wire eff for all live wires in this layer
      REAL SumEffLayer

C   IwireInLayer - loop over wires in this layer
      INTEGER IwireInLayer

C   IWireCellID - CellID of wire - should be same as wire number
      integer IWireCellID

C   LdeadWire - TRUE if given wire is dead
      LOGICAL LdeadWire

C   eff_layer_array - average eff. for all wires in layer
      real layer_eff_array(kNDRLayers)

C     NoConstantsUpdate - tell constants access subroutines to update constants
      logical NoConstantsUpdate
      parameter ( NoConstantsUpdate = .false. )

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_layer_eff.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/10/26 18:36:35 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'get_layer_eff' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine get_layer_eff:
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
        chmess='Calculate average wire efficiency for a given DR3 layer'
        call report(MS_DEBUG, crname, chmess)

C       It makes no sense to me why we should save this info and not do
C       repeat this calculation every single time we run Cleog.
        chmess='This should be improved:'
        call report(MS_WARNING, crname, chmess)
        chmess='Constants finding job should calculate layer average'
        call report(MS_WARNING, crname, chmess)
        chmess='  do this & save in database'
        call report(MS_WARNING, crname, chmess)
        chmess='This is also wasteful - we fill large array of wire eff'
        call report(MS_WARNING, crname, chmess)
        chmess='  in this routine and in Get_WireEff'
        call report(MS_WARNING, crname, chmess)

        lfirst=.false.
      endif


C     Update constants ?
      if ( Update ) then
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
        chmess ='Updating DRMCWireff constants'
        call report(MS_DEBUG, crname, chmess)

C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants.
C       C++ function does error checking on the array length.
        call drmc_wireff( WireEffArray, kWireEffSize ) 


C       Find the average efficiency of each layer from 
C       the live wires.  This will be used in CDREFF.
C       Again: the average layer eff. should be stored in the database 
C       instead of doing this calculation.  Need to implement this.

C       Loop over anode wires
        do Ilayer = 1, kNDRLayers
C         Reset number of live wires in this layer 
          nWireLiveLayer = 0
C         Reset total eff. for this layer which is a sum of all live wires
          SumEffLayer = 0.0
          
C         Loop over wires in this layer, calc ave. eff. for layer
          do IwireInLayer = 0, NWIRCD(Ilayer)-1

C           Get wire CellID - wire # counting all wires from 1st layer
C           INDXCD - IADRHW address of wire # 0 of CD layer #
            IWireCellID = INDXCD(Ilayer) + IwireInLayer 

C           Is given wire dead or alive ?
            call get_deadwire(NoConstantsUpdate, IWireCellID, LdeadWire)

            if ( .not. LdeadWire ) then
              nWireLiveLayer = nWireLiveLayer + 1
              SumEffLayer = SumEffLayer + WireEffArray(IWireCellID)
            endif
          enddo
          
C         Calculate average LIVE wire efficiency for this layer 
          if ( nWireLiveLayer .gt. 0 ) then
            layer_eff_array(Ilayer) = SumEffLayer/FLOAT(nWireLiveLayer)
          else
            layer_eff_array(Ilayer) = 0.0
          endif
        enddo

C     End of updating constants
      endif

      
C     Return layer eff for given Index (=layer number)
      layer_eff = layer_eff_array(Index)

      return
      end

