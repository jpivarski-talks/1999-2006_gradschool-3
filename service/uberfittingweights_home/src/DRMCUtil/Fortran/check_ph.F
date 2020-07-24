C 
C  $Id: check_ph.F,v 1.6 1999/12/13 23:59:16 jjo Exp $
C 
C  $Log: check_ph.F,v $
C  Revision 1.6  1999/12/13 23:59:16  jjo
C  Remove debug printouts
C
C  Revision 1.5  1999/12/13 01:44:00  jjo
C  Use report for debug messages
C
C  Revision 1.4  1999/11/22 01:15:25  jjo
C  Remove device numbe in call to drmc_charge2adc
C
C  Revision 1.3  1999/10/26 18:28:30  jjo
C  Use get_adc_pedestal to check pulse heights.
C  Previously was a dummy routine.
C
C  Revision 1.2  1999/10/20 21:41:18  jjo
C  Get DR MC Response resolution constants installed
C
C  Revision 1.1  1999/09/20 22:48:04  jjo
C  Import files into cvs
C
C 
      subroutine check_ph( CellId, pulseHeight )
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
C.  CellId   - integer - DR cell Id
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
C   Author:   John J O'Neill      Created:  Tue Aug 17 10:39:37 EDT 1999
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
      parameter(chrcss='$RCSfile: check_ph.F,v $')
      parameter(chrcsr='$Revision: 1.6 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/12/13 23:59:16 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'check_ph' )
 
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
      call get_adc_pedestal( NoConstantsUpdate, CellId,
     &                       ADC_Ped, ADC_Ped_Width )
      

C     Provide protection for writing out pulseheights which are too large
C     so their ADC counts go negative in drmc_charge2adc (in DRMCUtil/Class/DRMCDriftFB.cc)
C     Such hits get pegged at 0 ADC counts.
      call drmc_charge2adc( CellId, pulseHeight, adc )
      if ( adc .lt. 0 ) then       
        adc = 0
        call drmc_adc2charge( CellId, pulseHeight, adc )
c        write(chmess,*) 'Set ADC=0.  pulseHeight=', pulseHeight
c        call report(MS_INFO, crname, chmess)
      endif


C     If the ADC is near or above pedestal, smear it about the 
C     pedestal with a width equal to the pedestal width.
C     Note, the DR3 ADC electronics count up
      if ( (FLOAT(adc) .lt. ADC_Ped) .OR. 
     &     ( abs(FLOAT(adc)-ADC_Ped) .lt. (3.0*ADC_Ped_Width) ) ) then

        call granor(Random1, Random2)
        adc  = ADC_Ped_Width*Random1 + ADC_Ped
        if ( adc .gt. MaxAdcValue ) adc = MaxAdcValue
        if ( adc .lt. 0 ) adc = 0
        
        call drmc_adc2charge( CellId, pulseHeight, adc )
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



