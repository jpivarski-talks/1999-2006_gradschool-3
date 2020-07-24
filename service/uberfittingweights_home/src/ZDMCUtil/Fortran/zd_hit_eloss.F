      SUBROUTINE ZD_HIT_ELOSS( ElossGeantHit,
     &                         CellID,
     &                         LayerGeantHit,
     &                         DriftDistGeantHit,
     &                         cosZGeantHit,
     &                         ChargeDep )
C 
C  begin_doc
C 
C   Documentation for subroutine zd_hit_eloss
C 
C   Purpose: Convert energy loss to pulse height via ZDMCUtil/Fortran/eloss_to_charge.F
C   -------- and make DOCA and wire corrections  via ZDMCUtil/Fortran/ph_correction.F
C
C   Input Parameters:  
C   ----------------
C     ElossGeantHit      - Energy loss for geant hit 
C     CellId        - ZD CellId number
C     LayerGeantHit - ZD Layer Number
C     DriftDistGeantHit  - drift distance of geant hit 
C     cosZGeantHit  - cos of track-hit angle with Z

C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C     ChargeDep - charge deposited in cell in "Happy Units"
C 
C   Other routines: ZDMCUtil/Fortran/eloss_to_charge.F and
C   --------------- ZDMCUtil/Fortran/ph_correction.F
C 
C   Called By: MCResponseProd/Fortran/mc_zd_responses
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
      integer CellId, LayerGeantHit
      real ElossGeantHit, DriftDistGeantHit, cosZGeantHit
      real ChargeDep 
C 
C   Local User defined variables:
C   -----------------------------
C   kELossToCharge - Convert ElossGeantHit into approximate pulseheight in Happy Units
C                  278.8 counts/keV * 1E6 keV/GeV / 1.407 Cell size @ Up(4S)


      real kELossToCharge
*      parameter ( kELossToCharge =  1.982E8)
* D.M. 01/28/01 The kELossToCharge value is changed to get better agreement for 
*               spectral peak for data and M.C. with unit other constants.
      parameter ( kELossToCharge =  1.487E8)

      real sinZH, tanZH, Qdens

      real P(4), Qpeak, ChargeDensityCorr

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_hit_eloss.F,v $')
      parameter(chrcsr='$Revision: 1.4 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/02/10 20:03:18 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_hit_eloss' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./

C Parameters for charge density correction
c      data P/0., 0.63, 0.22, 1.7/, Qpeak/402./ ! v13
c      data P/0., 0.46, 0.304, 2.0/, Qpeak/395./ ! v14
         data Qpeak/433./ ! v4 for 116044
         data P/1.2049, 0.45456, 0.14045, 2./
C   executable code for routine zd_hit_eloss:
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
        chmess=
     &       'Convert Eloss to pulse height + DOCA and wire corrections'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

C----------------------------------------------------------------------
C     Convert energy loss into charge pulse height
C     call eloss_to_charge( ElossGeantHit, ChargeDep )

C     D.M. 01/24/01 That's all that eloss_to_charge is doing. 
C     Stuppied calls take the time! So, remove eloss_to_charge ...
      ChargeDep = kELossToCharge * ElossGeantHit

C----------------------------------------------------------------------
*C     D.M. 02/10/01 Add the dependence of charge on wire charge density 

      sinZH=sqrt(1.-cosZGeantHit**2)
      tanZH=sinZH/(abs(cosZGeantHit)+0.0001)

      Qdens=ChargeDep*tanZH/Qpeak
**     Qnorm=ChargeDep*sinZH/Qpeak

       if( Qdens .ge. P(4)) then
         ChargeDensityCorr=P(1)*(1.-P(2)*(1.-exp(-(Qdens-P(4))*P(3))))
       else
         ChargeDensityCorr=P(1)
       endif

*c          write(6,*),' L,cos,Q, Q density = ',
*c     >    LayerGeantHit, cosZGeantHit, ChargeDep, Qdens 

       ChargeDep =  ChargeDensityCorr * ChargeDep

C      write(6,*),'cos, sin, tan, Qnorm::', 
C     &            cosZGeantHit,sinZH,tanZH,Qnorm
C----------------------------------------------------------------------

C     Make DCA and wire-to-wire gain corrections for pulseheight.
      call zd_ph_correction( CellID,
     &                    LayerGeantHit, 
     &                    DriftDistGeantHit,
     &                    ChargeDep )

C----------------------------------------------------------------------
      return
      end
