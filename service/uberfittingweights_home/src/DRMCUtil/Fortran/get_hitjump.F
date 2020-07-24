C 
C  $Id: get_hitjump.F,v 1.2 1999/12/13 01:48:39 jjo Exp $
C 
C  $Log: get_hitjump.F,v $
C  Revision 1.2  1999/12/13 01:48:39  jjo
C  Use report for debug messages
C
C  Revision 1.1  1999/09/20 22:07:50  jjo
C  Add get routines for DR MC constants
C
C 
      SUBROUTINE GET_HITJUMP( Update,
     &                       Index )
C 
C  begin_doc
C 
C   Documentation for subroutine get_hitjump
C 
C   Purpose: Returns DRMCHitjump, prob. of hit moving to adjacent cell constants, for given Index
C   -------- Update=.true. updates constants from Cleo3 Frame
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = true -> update constants at begin run time
C   Index - integer - index into constants array
C 
C   Output Parameters: (Name - Meaning)
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
C   Author:   John J O'Neill      Created:  Mon Sep 20 11:48:18 EDT 1999
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

C   parameters including array sizes
#include "DRMCUtil/Fortran/drmc_param.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      logical Update
      integer index
 
C 
C   Local User defined variables:
C   -----------------------------
C   arraySize - Array size passed to C++ function that fills constants
C   kXXXSize  - Size of constants from drmc_param.inc
C   XXXArray  - Array(s) that holds the constants filled from Cleo3 Frame
      integer arraySize
c      real (kPhwireSize), 
 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_hitjump.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/12/13 01:48:39 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'get_hitjump' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini
      data rcsini/.true./
 
C   executable code for routine get_hitjump:
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

      chmess=' todo: get hit jump constants going'
      call report(MS_INFO, crname, chmess)       

      return
      end



