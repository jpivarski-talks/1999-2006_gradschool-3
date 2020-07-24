C 
C  $Id: eloss_to_charge.F,v 1.4 1999/12/13 01:44:49 jjo Exp $
C 
C  $Log: eloss_to_charge.F,v $
C  Revision 1.4  1999/12/13 01:44:49  jjo
C  Use report for debug messages
C
C  Revision 1.3  1999/11/22 23:14:31  jjo
C  Fix typo
C
C  Revision 1.2  1999/11/22 22:40:38  jjo
C  Add printout for debugging if CPP flag DEBUG_RESPONSE is defined
C
C  Revision 1.1  1999/10/20 21:50:36  jjo
C  Import routines for DR MC response simulation
C
C 
      SUBROUTINE ELOSS_TO_CHARGE( ELoss, Charge ) 
C 
C  begin_doc
C 
C   Documentation for subroutine eloss_to_charge
C 
C   Purpose: Convert energy loss into a guesstimate of the 
C   -------- pulseheight in Happy Units.
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   ELoss - real - energy loss
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   charge - real - guesstimate of the pulseheight charge in "Happy Units"
C
C   Other routines:
C   ---------------
C 
C   Called By: mc_dr_response.F 
C   ----------
C   
C   Notes:
C   ------
C 
C   Author:   John J O'Neill      Created:  Wed Sep 29 12:50:22 EDT 1999
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
      real ELoss, Charge

C 
C   Local User defined variables:
C   -----------------------------
C     kELossToCharge - Convert ElossGeantHit into approximate pulseheight in Happy Units
C                  278.8 counts/keV * 1E6 keV/GeV / 1.407 Cell size @ Up(4S)
      real kELossToCharge
      parameter ( kELossToCharge =  1.982E8)


C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: eloss_to_charge.F,v $')
      parameter(chrcsr='$Revision: 1.4 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/12/13 01:44:49 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'eloss_to_charge' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine eloss_to_charge:
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
        chmess='Convert ELoss to charge using fixed conversion factor'
        call report(MS_DEBUG, crname, chmess)
        chmess='Need to check conversion factor, kELossToCharge=1.982E8'
        call report(MS_WARNING, crname, chmess)
        lfirst=.false.
      endif

C     Convert energy loss into a guesstimate of the pulseheight in "Happy Units"
      Charge = kELossToCharge * ELoss

#if defined(DEBUG_RESPONSE)
      chmess= ' ------- begin ELOSS_TO_CHARGE debug info -------'
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'ELoss=',ELoss 
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Charge=',Charge
      call report(MS_INFO, crname, chmess)       
      chmess = ' ------- end ELOSS_TO_CHARGE debug info -------'
      call report(MS_INFO, crname, chmess)       
#endif
      
      return
      end



