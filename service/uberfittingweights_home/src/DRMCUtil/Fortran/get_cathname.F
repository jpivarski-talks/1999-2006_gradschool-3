C 
C  $Id: get_cathname.F,v 1.1 1999/09/20 22:07:47 jjo Exp $
C 
C  $Log: get_cathname.F,v $
C  Revision 1.1  1999/09/20 22:07:47  jjo
C  Add get routines for DR MC constants
C
C 
      SUBROUTINE GET_CATHNAME( Update, cathodeName )
C 
C  begin_doc
C 
C   Documentation for subroutine get_cathname
C 
C   Purpose: Returns DRMCCathname, filename of cathode histograms.
C   -------- Update=.true. updates constants from Cleo3 Frame
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = true -> update constants at begin run time
C
C   Output Parameters: (Name - Meaning)
C   -----------------
C   cathodeName - file name with cathode histograms
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
C   Author:   John J O'Neill      Created:  Mon Sep 20 11:48:34 EDT 1999
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
      character*4 cathodeName 
C 
C   Local User defined variables:
C   -----------------------------
 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_cathname.F,v $')
      parameter(chrcsr='$Revision: 1.1 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/09/20 22:07:47 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'get_cathname' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini
      data rcsini/.true./
 
C   executable code for routine get_cathname:
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

C     Update constants ?
      if ( Update ) then
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
        chmess ='Updating DRMCCathname constants - cathode filename'
        call report(MS_DEBUG, crname, chmess)

        call drmc_cathname( cathodeName )
      endif

      return
      end



