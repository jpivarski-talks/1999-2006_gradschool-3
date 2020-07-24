C 
C  $Id: fill_layer_eff.F,v 1.1 1999/09/20 22:48:10 jjo Exp $
C 
C  $Log: fill_layer_eff.F,v $
C  Revision 1.1  1999/09/20 22:48:10  jjo
C  Import files into cvs
C
C 
      SUBROUTINE FILL_LAYER_EFF
C 
C  begin_doc
C 
C   Documentation for subroutine fill_layer_eff
C 
C   Purpose: Fill DR layer efficiency constants from constants 
C   -------- database and store in drmc_layer_eff.inc common block
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
C   Notes:
C   ------
C 
C   Author:   John J O'Neill      Created:  Tue Aug 17 16:03:58 EDT 1999
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

C#include "
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
 
C 
C   Local User defined variables:
C   -----------------------------
 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: fill_layer_eff.F,v $')
      parameter(chrcsr='$Revision: 1.1 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/09/20 22:48:10 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'fill_layer_eff' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine fill_layer_eff:
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
        call report(MS_INFO, crname, chmess)
        lfirst=.false.
      endif

      
      return
      end



