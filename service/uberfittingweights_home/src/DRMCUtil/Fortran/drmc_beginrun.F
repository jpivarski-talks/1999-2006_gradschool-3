C 
C  $Id: drmc_beginrun.F,v 1.6 2001/12/18 20:33:07 dubrovin Exp $
C 
C  $Log: drmc_beginrun.F,v $
C  Revision 1.6  2001/12/18 20:33:07  dubrovin
C  Add parameter ChargeDep at call get_wireeff
C
C  Revision 1.5  2000/02/15 00:28:36  jjo
C  Comment out cathode routines which aren't used now
C
C  Revision 1.4  1999/10/27 22:40:27  jjo
C  fix typo in get_xdc_pedestal name.
C
C  Revision 1.3  1999/10/26 18:32:12  jjo
C  Initialize get_deadwire, get_adc_pedestals and get_tdc_pedestals
C
C  Revision 1.2  1999/10/20 21:41:20  jjo
C  Get DR MC Response resolution constants installed
C
C  Revision 1.1  1999/09/20 22:48:06  jjo
C  Import files into cvs
C
C 
      SUBROUTINE DRMC_BEGINRUN( )
C 
C  begin_doc
C 
C   Documentation for subroutine drmc_beginrun
C 
C   Purpose: Begin run routine for DR MC simulation.
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
C   Author:   John J O'Neill      Created:  Mon Sep 13 13:51:38 EDT 1999
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


C     for DRMCCathname via DRMCUtil/Fortran/get_cathname.F
      character*4 cathodeName 

C     for DRMCCoseff via DRMCUtil/Fortran/get_coseff.F
      real Min_coseff, Max_coseff, Cheb1_coseff, Cheb2_coseff

C     for DRMCEff via DRMCUtil/Fortran/get_eff.F
      real Param1_eff, Param2_eff, Param3_eff

C     for DRMCHitJump via DRMCUtil/Fortran/get_hitjump.F
C     need to get hit jump constants working
c      real 

C     for DRMCIons via DRMCUtil/Fortran/get_ions.F
      integer N_ions

C     for DRMCLyreff via DRMCUtil/Fortran/get_lyreff.F
      real Slpn_lyreff, Slpp_lyreff,
     &     Dstn_lyreff, Dstp_lyreff,
     &     Norm_lyreff,
     &     Chb1_lyreff, Chb2_lyreff, 
     &     Slpm_lyreff,
     &     Xr_lyreff

C     for DRMCPhlayr via DRMCUtil/Fortran/get_phlayr.F
      real LayerCor_phlayr

C     for DRMCPhwire via DRMCUtil/Fortran/get_phwire.F
      real WireCor_phwire

C     for DRMCResolutions constants via DRMCUtil/Fortran/get_resolutions.F
      real    Sigma1_resolution,
     &        Sigma2_resolution,
     &        Offset1_resolution,
     &        Offset2_resolution,
     &        Fraction_resolution

C     for DRMCSmear via DRMCUtil/Fortran/get_smear.F
      real DeltaZ_smear

C     for DRMCSpefud via DRMCUtil/Fortran/get_spefud.F
      real ElecFudge_spefud, MuonFudge_spefud,
     &     PionFudge_spefud, KaonFudge_spefud,
     &     ProtonFudge_spefud

C     for DRMCTmlayr via DRMCUtil/Fortran/get_tmlayr.F
      real Shift_tmlayr

C     for DRMCWireff constants via DRMCUtil/Fortran/get_wireeff.F
      real ChargeDep, WireEff_wireeff

C     Calc average layer efficiency
      real LayerEff_layereff

C     for DeadWire constants via DRMCUtil/Fortran/get_deadwire.F
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
      parameter(chrcss='$RCSfile: drmc_beginrun.F,v $')
      parameter(chrcsr='$Revision: 1.6 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/12/18 20:33:07 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'drmc_beginrun' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine drmc_beginrun:
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
        chmess='Getting DR MC resolution constants'
        call report(MS_INFO, crname, chmess)
        lfirst=.false.
      endif


C     tell constants access subroutines to update constants
      Update = .TRUE.
C     Index tells routines which value to return. But 
C     this routine just fills them so set = 1
      Index = 1

C     Get DRMCCoseff - 2D fit of Cos(theta) vs drift (???)
      call get_coseff( Update, Index,
     &                 Min_coseff, Max_coseff,
     &                 Cheb1_coseff, Cheb2_coseff )

C     Get DRMCEff - cathode efficiency
      call get_eff( Update, Index,
     &              Param1_eff, Param2_eff, Param3_eff )

C     Get DRMCHitJump - prob. of hit moving to adjacent cell
C     need to get hit jump constants working
      call get_hitjump( Update, Index )


C     Get DRMCIons - ion statistics constants (???)
      call get_ions( Update, Index, 
     &               N_ions )

C     Get DRMCLyreff - drift view of drift vs. cos(theta) (???)
      call get_lyreff( Update, Index, 
     &                 Slpn_lyreff, Slpp_lyreff, 
     &                 Dstn_lyreff, Dstp_lyreff,
     &                 Norm_lyreff,
     &                 Chb1_lyreff, Chb2_lyreff, 
     &                 Slpm_lyreff, 
     &                 Xr_lyreff )

C     Get DRMCPhlayr - pulse height layer conversion factors
      call get_phlayr( Update, Index, 
     &                 LayerCor_phlayr )
     &              

C     Get DRMCPhwire - Pulse height wire to wire corrections
      call get_phwire( Update, Index, 
     &                 WireCor_phwire )
     &              

C     Get DRMCResolutions - drift function resolution constants
      call get_resolutions( Update, Index,
     &                      Sigma1_resolution,
     &                      Sigma2_resolution,
     &                      Offset1_resolution,
     &                      Offset2_resolution,
     &                      Fraction_resolution )

C     Get DRMCSmear - cathode resolution constants
      call get_smear( Update, Index, 
     &                DeltaZ_smear )
     &              
      
C     Get DRMCSpefud - species dependent fudge constants
      call get_spefud( Update, Index, 
     &                 ElecFudge_spefud, MuonFudge_spefud, 
     &                 PionFudge_spefud, KaonFudge_spefud,
     &                 ProtonFudge_spefud )

C     Get DRMCTmlayr - cathode times constants (times layer) (???)
      call get_tmlayr( Update, Index, 
     &                 Shift_tmlayr )


      ChargeDep=1000.      
C     Get DRMCWIREFF - overall wire eff. constants
      call get_wireeff( Update, Index, 
     &                  ChargeDep, WireEff_wireeff )


C     get_layer_eff - calculate Average Eff. per layer
      call get_layer_eff( Update,
     &                    Index, 
     &                    LayerEff_layereff )

C     DeadWire constants - not in C++ database yet      
      call get_deadwire( Update,
     &                   Index, 
     &                   DeadWire_DeadWire  )

C     ADC pedestals constants - not in C++ database yet      
      call get_adc_pedestal( Update,
     &                       Index, 
     &                       ADCPed_Ped,
     &                       ADCPed_Ped_Width )

C     TDC pedestals constants - not in C++ database yet      
      call get_tdc_pedestal( Update,
     &                       Index, 
     &                       TDCPed_Ped,
     &                       TDCPed_Ped_Width )


C      Cathodes currently don't use these constants so comment out
CC     Get DRMCCathname - cathode histogram filename
C      call get_cathname( Update, cathodeName )


      return
      end



