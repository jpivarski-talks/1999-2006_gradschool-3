      SUBROUTINE PHI_OF_ZD_WIRE( WireNum, LayerNum, PhiWire )
C 
C  begin_doc
C 
C   Documentation for subroutine phi_of_zd_wire
C 
C   Purpose: Calc phi of ZD wire in a given layer
C   --------
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   WireNum  - wire number in layer
C   LayerNum - layer number
C
C   Output Parameters: (Name - Meaning)
C   -----------------
C   PhiWire - phi of wire in layer
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

C   cdgeom includes
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      integer WireNum, LayerNum
      real PhiWire
C 
C   Local User defined variables:
C   -----------------------------
C Parameters      
      REAL        kTWOPI
      PARAMETER ( kTWOPI = 6.283185308 )


 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: phi_of_zd_wire.F,v $')
      parameter(chrcsr='$Revision: 1.4 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/02/10 20:03:17 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'phi_of_zd_wire' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine phi_of_zd_wire:
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
        chmess='Begining of routine'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif


      if ( WireNum .LT. NWIRCD(LayerNum) ) then
        PhiWire = PHIFCD(LayerNum) +
     &       WireNum*CELLCD(LayerNum)
      else
C       this should never happen.  Cleo2 code would try to fix it with this
C       PhiWire =PHIFCD(LayerNum) + (WireNum-NWIRCD(LayerNum))*CELLCD(LayerNum)
        chmess = 'WireNumGeantHit.ge.NWIRCD(LayerNum)!'
        call report(MS_EMERGENCY, crname, chmess)
        chmess='Will Stop the program now'
        call report(MS_EMERGENCY, crname, chmess)
        stop
      endif
      
C     Check phi calculation
      if ( PhiWire .LT. 0 ) then
C       Not sure why this happens
        PhiWire = PhiWire + kTWOPI
      endif
      if ( PhiWire .GT. kTWOPI ) then
C       Stop program for unphysical phi.
        write(chmess,*) 'PhiWire > 2pi  PhiWire=', PhiWire
        call report(MS_EMERGENCY, crname, chmess)
        chmess='Will Stop the program now'
        call report(MS_EMERGENCY, crname, chmess)
        stop
      endif
      
      
      return
      end
