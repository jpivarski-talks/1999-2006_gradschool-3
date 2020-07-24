C 
C  $Id: get_eff.F,v 1.2 1999/11/23 02:50:16 jjo Exp $
C 
C  $Log: get_eff.F,v $
C  Revision 1.2  1999/11/23 02:50:16  jjo
C  Fix Index check to be < 1, not 0
C
C  Revision 1.1  1999/09/20 22:07:48  jjo
C  Add get routines for DR MC constants
C
C 
      SUBROUTINE GET_EFF( Update,
     &                    Index,
     &                    Param1, 
     &                    Param2, 
     &                    Param3 )
C 
C  begin_doc
C 
C   Documentation for subroutine get_eff
C 
C   Purpose: Returns DRMCEff, cathode efficiency, for a given index.
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
C   Author:   John J O'Neill      Created:  Mon Sep 20 11:48:24 EDT 1999
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
      real Param1, Param2, Param3
 
C 
C   Local User defined variables:
C   -----------------------------
C   arraySize - Array size passed to C++ function that fills constants
C   kXXXSize  - Size of constants from drmc_param.inc
C   XXXArray  - Array(s) that holds the constants filled from Cleo3 Frame
      integer arraySize
      real Param1Array(kEffSize),
     &     Param2Array(kEffSize),
     &     Param3Array(kEffSize)
 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_eff.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/11/23 02:50:16 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'get_eff' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini
      data rcsini/.true./
 
C   executable code for routine get_eff:
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
C     Valid index ?
      if ( Index.lt.1 .or.
     &     Index.gt.kEffSize  ) then
        write(chmess,*) 'Illegal Index = ', Index
        call report(MS_EMERGENCY, crname, chmess)
      endif

C     Update constants ?
      if ( Update ) then
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
        chmess ='Updating DRMCEff constants - Cathode efficiency'
        call report(MS_DEBUG, crname, chmess)

C       Request number of elements to be filled
        ArraySize = kEffSize
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants.
C       C++ function does error checking on the array length.
        call drmc_eff( Param1Array,
     &                 Param2Array, 
     &                 Param3Array, 
     &                 ArraySize ) 
      endif

C     Return constants for this index
      Param1 = Param1Array(Index)
      Param2 = Param2Array(Index)
      Param3 = Param3Array(Index)
 
 
      return
      end



