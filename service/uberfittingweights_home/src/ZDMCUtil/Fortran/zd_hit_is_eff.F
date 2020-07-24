      LOGICAL FUNCTION ZD_HIT_IS_EFF( CellEff )
C 
C  begin_doc
C 
C   Documentation for subroutine zd_hit_is_eff
C 
C   Purpose: Determine if ZD Hit is efficient from cell efficiency
C   --------
C 
C   Input Parameters:  
C   ----------------
C   CellEff - efficiency of Cell for hit to fire
C
C   Output Parameters: 
C   -----------------
C   Return logical for the hit to be (in)efficient
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
      real CellEff
C 
C   Local User defined variables:
C   -----------------------------
C     RandomNumber - Random number to determine if hit was efficient
      real RandomNumber

      real*8 RANDOM_MODULE_FLAT

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_hit_is_eff.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/02/10 20:03:19 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_hit_is_eff' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine zd_hit_is_eff:
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
        chmess='Determine if ZD Hit is efficient from cell efficiency'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif


C     Get a random number to see if the hit is "efficient"
C     Store it in the appropriate scratch array later on.
C     call GRNDM(RandomNumber, 1)
      RandomNumber=RANDOM_MODULE_FLAT()
C     Is hit inefficient or efficient ?
      if ( RandomNumber .LE. CellEff ) then
        ZD_HIT_IS_EFF = .true.
      else
        ZD_HIT_IS_EFF = .false.
      endif
          
      return
      end
