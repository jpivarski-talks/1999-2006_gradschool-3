C 
C  $Id: dr_hit_is_eff.F,v 1.2 2001/06/12 15:04:23 dubrovin Exp $
C 
C  $Log: dr_hit_is_eff.F,v $
C  Revision 1.2  2001/06/12 15:04:23  dubrovin
C  New generators of random numbers
C
C  Revision 1.1  2000/02/15 00:26:24  jjo
C  Import new routines into cvs
C
C 
      LOGICAL FUNCTION DR_HIT_IS_EFF( CellEff )
C 
C  begin_doc
C 
C   Documentation for subroutine dr_hit_is_eff
C 
C   Purpose: Determine if DR Hit is efficient from cell efficiency
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
C   Author:   John J O'Neill      Created:  Mon Feb  7 10:53:24 EST 2000
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
      parameter(chrcss='$RCSfile: dr_hit_is_eff.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/06/12 15:04:23 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'dr_hit_is_eff' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine dr_hit_is_eff:
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
        chmess='Determine if DR Hit is efficient from cell efficiency'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif


C     Get a random number to see if the hit is "efficient"
C     Store it in the appropriate scratch array later on.
C     call GRNDM(RandomNumber, 1)
      RandomNumber=RANDOM_MODULE_FLAT()
C     Is hit inefficient or efficient ?
      if ( RandomNumber .LE. CellEff ) then
        DR_HIT_IS_EFF = .true.
      else
        DR_HIT_IS_EFF = .false.
      endif
          
      return
      end



