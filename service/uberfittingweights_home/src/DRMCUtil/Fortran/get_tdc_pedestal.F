C 
C  $Id: get_tdc_pedestal.F,v 1.2 1999/11/22 22:43:15 jjo Exp $
C 
C  $Log: get_tdc_pedestal.F,v $
C  Revision 1.2  1999/11/22 22:43:15  jjo
C  Update to call drmc_tdc_XXX interface functions, which are dummy
C  functions now and we will need to update this routine when the drmc
C  functions returns arrays and we'll want to cache them here.
C
C  Revision 1.1  1999/10/26 18:26:30  jjo
C  Import new routines
C
C 
      SUBROUTINE GET_TDC_PEDESTAL( Update,
     &                             Index,
     &                             Ped,
     &                             Ped_Width )
C 
C  begin_doc
C 
C   Documentation for subroutine get_tdc_pedestal
C 
C   Purpose:
C   --------
C 
C   Purpose: Get DR tdc pedestals, was cleog/crawm/get_pedestals.F in CLEO-II.
C   -------- Update=.true. updates constants from Cleo3 Frame.
C
C            NOTE: Doesn't get constants from C++ interface objects yet as
C            they haven't been written - we'll print an error message.
C
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = NOT USED - see NOTES below.  
C                      true -> update constants at begin run time
C   Index - integer - index into constants array
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   Ped       - tdc pedestal
C   Ped_Width - tdc pedestal width
C 
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:
C   ------
C    Currently, the routines drmc_tdc_pedestal_() & drmc_tdc_width_() 
C    in DRMCUtil/Class/DRMCDriftFB.cc, are hardwired routines 
C    and they do not return arrays of the pedestals or widths, 
C    just value for a given cellID.  Until this fetchs real
C    constants from C++ system we will ignore the Update flag of 
C    this routine and call the drmc_tdc_XXX() functions every time.
C 
C    These are the values that KME thought the electronics should give us:
C          PedArray(IWire)      = 8000
C          PedWidthArray(IWire) =    1
C 
C 
C   Author:   John J O'Neill      Created:  Mon Oct 25 14:34:11 EDT 1999
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

C   parameters including array sizes
#include "DRMCUtil/Fortran/drmc_param.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      logical Update
      integer index
      real Ped, Ped_Width
C 
C   Local User defined variables:
C   -----------------------------
C     IWire - loop over wires
      integer IWire

C     PedArray      - Array of TDC pedestals
C     PedWidthArray - Array of TDC pedestal widths
      real PedArray(kNDRWires), PedWidthArray(kNDRWires)

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_tdc_pedestal.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/11/22 22:43:15 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'get_tdc_pedestal' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine get_tdc_pedestal:
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
        chmess='Get DR tdc pedestals'
        call report(MS_DEBUG, crname, chmess)
        chmess='Not caching pedestals as not avail. in DRMCDrift.cc'
        call report(MS_WARNING, crname, chmess)
        lfirst=.false.
      endif


C  Call functions in DRMCDrift.cc every time so don't cache constants in
C  this routine.  When drmc_adc_pedestal/width return arrays of constants
C  gotten from the real C++ constants, cache the constants in this
C  routine.
C
CC     Update constants ?
C      if ( Update ) then
CC       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
C        chmess ='Updating DR TDC Pedestal constants'
C        call report(MS_DEBUG, crname, chmess)
C
CC       Currently, don't have C++ pedestal constants from frame so hardwire 
C        chmess =  'Need TDC Pedestals.  Hardwire Ped=8000, Width=1'
C        call report(MS_ERROR, crname, chmess)
C        do IWire = 1, kNDRWires
C          PedArray(IWire)      = 8000
C          PedWidthArray(IWire) =    1
C        enddo
C      endif
C      
CC     Return pedestal & width for given Index (=wire number)
C      Ped       = PedArray(Index) 
C      Ped_Width = PedWidthArray(Index) 


C     Call functions in DRMCUtil/Class/DRMCDrift.cc every time
      call drmc_tdc_pedestal( Index, Ped )
      call drmc_tdc_width( Index, Ped_Width )



      return
      end



