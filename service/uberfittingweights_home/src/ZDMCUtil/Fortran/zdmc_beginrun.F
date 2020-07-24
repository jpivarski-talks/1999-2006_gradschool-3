      SUBROUTINE ZDMC_BEGINRUN( )
C 
C  begin_doc
C 
C   Documentation for subroutine zdmc_beginrun
C 
C   Purpose: Begin run routine for ZD MC simulation.
C   -------- Get constants for this run number.
C 
C   Input Parameters:  None
C   ----------------
C
C   Output Parameters: None
C   -----------------
C 
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:  called by MCResponseProd::updateCleogBeginRun() function.
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
C 
C   Local User defined variables:
C   -----------------------------

C     Update - tell constants access subroutines to update constants
      logical Update

C     Index of the constant we want.  
      integer Index

C     Note: We do not use the  variables that hold the constants
C           we are just updating them for this run.

C     for ZDMCPhlayr via ZDMCUtil/Fortran/get_phlayr.F
      real LayerCor_phlayr

C     for ZDMCPhwire via ZDMCUtil/Fortran/get_phwire.F
      real WireCor_phwire

C     for ZDMCResolutions constants via ZDMCUtil/Fortran/get_resolutions.F
      real    Sigma1_resolution,
     &        Sigma2_resolution,
     &        Offset1_resolution,
     &        Offset2_resolution,
     &        Fraction_resolution

C     for ZDMCWireff constants via ZDMCUtil/Fortran/get_wireeff.F
      real ChargeDep, WireEff_wireeff

C     for DeadWire constants via ZDMCUtil/Fortran/get_deadwire.F
      logical DeadWire_DeadWire

C     for ADC pedestal & widths
      real ADCPed_Ped, ADCPed_Ped_Width

C     for TDC pedestal & widths
      real TDCPed_Ped, TDCPed_Ped_Width

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zdmc_beginrun.F,v $')
      parameter(chrcsr='$Revision: 1.4 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/02/10 20:03:27 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zdmc_beginrun' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine zdmc_beginrun:
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
        chmess='Getting ZD MC resolution constants'
        call report(MS_INFO, crname, chmess)
        lfirst=.false.
      endif


C     tell constants access subroutines to update constants
      Update = .TRUE.
C     Index tells routines which value to return. But 
C     this routine just fills them so set = 1
      Index = 1

C     Get ZDMCPhlayr - pulse height layer conversion factors
      call zd_get_phlayr( Update, Index, 
     &                 LayerCor_phlayr )
     &              

C     Get ZDMCPhwire - Pulse height wire to wire corrections
      call zd_get_phwire( Update, Index, 
     &                 WireCor_phwire )
     &              

C     Get ZDMCResolutions - drift function resolution constants
      call zd_get_resolutions( Update, Index,
     &                      Sigma1_resolution,
     &                      Sigma2_resolution,
     &                      Offset1_resolution,
     &                      Offset2_resolution,
     &                      Fraction_resolution )

      ChargeDep=1000.      
C     Get ZDMCWIREFF - overall wire eff. constants
      call zd_get_wireeff( Update, Index, 
     &                  ChargeDep, WireEff_wireeff )

C     DeadWire constants - not in C++ database yet      
      call zd_get_deadwire( Update,
     &                   Index, 
     &                   DeadWire_DeadWire  )

C     ADC pedestals constants - not in C++ database yet      
      call zd_get_adc_pedestal( Update,
     &                       Index, 
     &                       ADCPed_Ped,
     &                       ADCPed_Ped_Width )

C     TDC pedestals constants - not in C++ database yet      
      call zd_get_tdc_pedestal( Update,
     &                       Index, 
     &                       TDCPed_Ped,
     &                       TDCPed_Ped_Width )

      return
      end
