C 
C  $Id: get_coseff.F,v 1.2 1999/11/23 02:45:40 jjo Exp $
C 
C  $Log: get_coseff.F,v $
C  Revision 1.2  1999/11/23 02:45:40  jjo
C  Change that Index < 1, not 0
C
C  Revision 1.1  1999/09/20 22:07:48  jjo
C  Add get routines for DR MC constants
C
C 
      SUBROUTINE GET_COSEFF( Update,
     &                       Index,
     &                       Min,
     &                       Max,
     &                       Cheb1,
     &                       Cheb2 )
C 
C  begin_doc
C 
C   Documentation for subroutine get_coseff
C 
C   Purpose: Returns DRMCCoseff, 2D fit of Cos(theta) vs drift, for a given index.
C   -------- Update=.true. updates constants from Cleo3 Frame
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = true -> update constants at begin run time
C   Index - integer - index into constants array
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   Min   -
C   Max   -
C   Cheb1 - 
C   Cheb2 - 
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
C   Author:   John J O'Neill      Created:  Mon Sep 20 11:48:29 EDT 1999
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
      real min, max, cheb1, cheb2
C 
C   Local User defined variables:
C   -----------------------------
C   arraySize - Array size passed to C++ function that fills constants
C   kXXXSize  - Size of constants from drmc_param.inc
C   XXXArray  - Array(s) that holds the constants filled from Cleo3 Frame
      integer arraySize

      real MinArray(kCoseffSize),
     &     MaxArray(kCoseffSize),
     &     Cheb1Array(kCoseffSize),
     &     Cheb2Array(kCoseffSize)

 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_coseff.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/11/23 02:45:40 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'get_coseff' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini
      data rcsini/.true./
 
C   executable code for routine get_coseff:
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
     &     Index.gt.kCoseffSize  ) then
        write(chmess,*) 'Illegal Index = ', Index
        call report(MS_EMERGENCY, crname, chmess)
      endif

C     Update constants ?
      if ( Update ) then
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
        chmess ='Updating DRMCCoseff constants - cos(theta) vs drift'
        call report(MS_DEBUG, crname, chmess)

C       Request number of elements to be filled
        ArraySize = kCoseffSize
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants.
C       C++ function does error checking on the array length.
        call drmc_coseff( MinArray,
     &                    MaxArray, 
     &                    Cheb1Array, 
     &                    Cheb2Array, 
     &                    arraySize ) 
      endif

C     Return constants for this index
      Min   = MinArray(Index)
      Max   = MaxArray(Index)
      Cheb1 = Cheb1Array(Index)
      Cheb2 = Cheb2Array(Index)

      return
      end



