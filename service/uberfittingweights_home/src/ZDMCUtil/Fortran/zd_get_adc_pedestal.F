      SUBROUTINE ZD_GET_ADC_PEDESTAL( Update,
     &                             Index,
     &                             Ped,
     &                             Ped_Width )
C 
C  begin_doc
C 
C   Documentation for subroutine get_adc_pedestal
C 
C   Purpose: Get ZD adc pedestals, was cleog/crawm/get_pedestals.F in CLEO-II.
C   -------- 
C
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = NOT USED - see NOTES below.  
C                      true -> update constants at begin run time
C   Index - integer - index into constants array
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   Ped       - adc pedestal
C   Ped_Width - adc pedestal width
C 
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:
C   ------
C    Currently, the routines zdmc_adc_pedestal_() & zdmc_adc_width_() 
C    in ZDMCUtil/Class/ZDMCDriftFB.cc, are hardwired routines 
C    and they do not return arrays of the pedestals or widths, 
C    just value for a given cellID.  Until this fetchs real
C    constants from C++ system we will ignore the Update flag of 
C    this routine and call the zdmc_adc_XXX() functions every time.
C 
C    These are the values that KME thought the electronics should give us:
C          PedArray(IWire)      = 210
C          PedWidthArray(IWire) =  10
C 
C 
C  Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
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
#include "ZDMCUtil/Fortran/zdmc_param.inc"
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

C     PedArray      - Array of ADC pedestals
C     PedWidthArray - Array of ADC pedestal widths
      real PedArray(kNZDWires), PedWidthArray(kNZDWires)

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_get_adc_pedestal.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:42 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_get_adc_pedestal' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine get_adc_pedestal:
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
        chmess='Get ZD adc pedestals'
        call report(MS_DEBUG, crname, chmess)
        chmess='Not caching pedestals as not avail. in ZDMCDrift.cc'
        call report(MS_WARNING, crname, chmess)
        lfirst=.false.
      endif


C  Call functions in ZDMCDrift.cc every time so don't cache constants in
C  this routine.  When zdmc_adc_pedestal/width return arrays of constants
C  gotten from the real C++ constants, cache the constants in this
C  routine.
C
CC     Update constants ?
C      if ( Update ) then
CC       Call C++ function in ZDMCUtil/Class/ZDMCxxxFB.cc to get constants 
C        chmess ='Updating ZD ADC Pedestal constants'
C        call report(MS_DEBUG, crname, chmess)
C
CC       Currently, don't have C++ pedestal constants from frame so hardwire 
C        chmess =  'Need ADC Pedestals.  Hardwire Ped=210, Width=10'
C        call report(MS_ERROR, crname, chmess)
C        do IWire = 1, kNZDWires
C          PedArray(IWire)      = 210
C          PedWidthArray(IWire) =  10
C        enddo
C      endif
C 
C     
CC     Return pedestal & width for given Index (=wire number)
C      Ped       = PedArray(Index) 
C      Ped_Width = PedWidthArray(Index) 

C     Call functions in ZDMCUtil/Class/ZDMCDrift.cc every time
      call zdmc_adc_pedestal( Index, Ped )

      call zdmc_adc_width( Index, Ped_Width )

      return
      end
