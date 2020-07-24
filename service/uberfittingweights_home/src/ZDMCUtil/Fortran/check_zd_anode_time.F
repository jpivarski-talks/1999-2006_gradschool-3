      SUBROUTINE CHECK_ZD_ANODE_TIME( CellId, DriftTime )
C 
C  begin_doc
C 
C   Documentation for subroutine check_zd_anode_time
C 
C   Purpose: Check DriftTime for an anode hit for anomolies.
C   -------- Needs to be implemented for Cleo3, was cleog/crawm/chktim.F in Cleo2
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   CellID    - ZD cell Id
C   DriftTime - time of the hit in picoseconds
C
C   Output Parameters: (Name - Meaning)
C   -----------------
C   DriftTime - smeared if tdc value is near pedestal value
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
      real DriftTime
C 
C   Local User defined variables:
C   -----------------------------
 
C     NoConstantsUpdate - tell constants access subroutines to update constants
      logical NoConstantsUpdate
      parameter ( NoConstantsUpdate = .false. )

C     TDC_Ped - ADC pedestal 
C     TDC_PedWidth - ADC pedestal width
      real TDC_Ped, TDC_Ped_Width

C     tdc - TDC value 
      integer tdc

C     MaxTdcValue - max. value of TDC 
      integer MaxTdcValue
      parameter (MaxTdcValue = 65536)


C     Random1, Random2 - random numbers from granor
      real Random1, Random2

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: check_zd_anode_time.F,v $')
      parameter(chrcsr='$Revision: 1.4 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/02/10 20:03:15 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'check_zd_anode_time' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine check_zd_anode_time:
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
        chmess='Check DriftTime for an anode hit for anomolies.'
        call report(MS_DEBUG, crname, chmess)
        chmess='No Pedestal Smearing - move to digitization ?'
        call report(MS_WARNING, crname, chmess)
        lfirst=.false.
      endif

C     Get pedestal & width for this CellId
      call zd_get_tdc_pedestal( NoConstantsUpdate, CellId,
     &                       TDC_Ped, TDC_Ped_Width )

C     Get TDC of hit
      call zdmc_time2tdc(CellId, DriftTime, tdc)

C     If TDC is below zero, peg the time at TDC=0
      IF ( tdc .LT. 0 ) THEN
        tdc = 0
        call zdmc_tdc2time( CellId, DriftTime, tdc )
C        write(chmess,*) 'Set TDC=0.  DriftTime=', DriftTime
C        call report(MS_INFO, crname, chmess)
      ENDIF


C     12/13/99 - Need to implement this.  Problem is that our current
C     Pedestal is 0 but it will be > than hit tdc value.  Currently we
C     don't have t0's in which will add a position offset to the
C     pedestal.  When this gets staightened out, implement smearing.
C     The smearing should be done in digitization phase, no ?
C
CC     If the TDC is above pedestal or just slightly below it, smear the
CC     TDC normally about the pedestal and get the time from the smeared TDC.
CC     Note, the ZD electrons count down.
C      if ( (FLOAT(tdc) .GT. TDC_Ped) .OR. 
C     +     ( (TDC_Ped-FLOAT(tdc)) .LT. (3.*TDC_Ped_Width)) ) then
C
CC       Smear tdc value 
C        call granor(random1, random2)
C        tdc  = TDC_Ped_Width*random1 + TDC_Ped
C
CC       Check for over/underflows
C        if ( tdc .gt. MaxTdcValue ) tdc = MaxTdcValue
C        if ( tdc .LT. 0 ) tdc = 0
C
C        call zdmc_tdc2time( CellId, DriftTime, tdc )
C
C        write(chmess,*) 'TDC=',tdc, '  DriftTime=', DriftTime
C        call report(MS_INFO, crname, chmess)
C      endif
      
      return
      end
