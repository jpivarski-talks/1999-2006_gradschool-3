      SUBROUTINE ZD_CHECK_PH( CellId, pulseHeight )
C 
C  begin_doc
C 
C   Documentation for subroutine check_ph
C 
C   Purpose: Correct pulseheights which give anomolous ADC values,
C   -------- was cleog/crawm/chkpls.F for Cleo-II simulation.
C
C   Input Parameters:  
C   ----------------
C.  CellId   - integer - ZD cell Id
C.  pulseHeight  - real -  MC generated pulse height
C 
C   Output Parameters: 
C   -----------------
C   pulseHeight  = Pulse height corrected for anomolies.
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
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      integer CellId
      real pulseHeight  

C 
C   Local User defined variables:
C   -----------------------------
C     NoConstantsUpdate - tell constants access subroutines to update constants
      logical NoConstantsUpdate
      parameter ( NoConstantsUpdate = .false. )

C     ADC_Ped - ADC pedestal 
C     ADC_PedWidth - ADC pedestal width
      real ADC_Ped, ADC_Ped_Width

C     adc - adc value of 
      integer adc

C     MaxAdcValue - max. value of adc 
      integer MaxAdcValue
      parameter (MaxAdcValue = 65536)

C     Random1, Random2 - random numbers from granor
      real Random1, Random2

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_check_ph.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:40 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_check_ph' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine check_ph:
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
        chmess='Correct pulseheights which give anomolous ADC values'
        call report(MS_INFO, crname, chmess)
        chmess='Move Pedestal Smearing digitization ?'
        call report(MS_WARNING, crname, chmess)
        lfirst=.false.
      endif

C     Get pedestal & width for this CellId
      call zd_get_adc_pedestal( NoConstantsUpdate, CellId,
     &                       ADC_Ped, ADC_Ped_Width )
      

C     Provide protection for writing out pulseheights which are too large
C     so their ADC counts go negative in zdmc_charge2adc (in ZDMCUtil/Class/ZDMCDriftFB.cc)
C     Such hits get pegged at 0 ADC counts.
      call zdmc_charge2adc( CellId, pulseHeight, adc )
      if ( adc .lt. 0 ) then       
        adc = 0
        call zdmc_adc2charge( CellId, pulseHeight, adc )
c        write(chmess,*) 'Set ADC=0.  pulseHeight=', pulseHeight
c        call report(MS_INFO, crname, chmess)
      endif


C     If the ADC is near or above pedestal, smear it about the 
C     pedestal with a width equal to the pedestal width.
C     Note, the ZD ADC electronics count up
      if ( (FLOAT(adc) .lt. ADC_Ped) .OR. 
     &     ( abs(FLOAT(adc)-ADC_Ped) .lt. (3.0*ADC_Ped_Width) ) ) then

        call granor(Random1, Random2)
        adc  = ADC_Ped_Width*Random1 + ADC_Ped
        if ( adc .gt. MaxAdcValue ) adc = MaxAdcValue
        if ( adc .lt. 0 ) adc = 0
        
        call zdmc_adc2charge( CellId, pulseHeight, adc )
C        write(chmess,*) 'ADC=', adc, '  pulseHeight=', pulseHeight
C        call report(MS_DEBUG, crname, chmess)
      endif


#if defined(DEBUG_RESPONSE)
      write(chmess,*) ' CellId, pulseHeight, adc',
     &     CellId, pulseHeight, adc
      call report(MS_INFO, crname, chmess)             
#endif

      return
      end
