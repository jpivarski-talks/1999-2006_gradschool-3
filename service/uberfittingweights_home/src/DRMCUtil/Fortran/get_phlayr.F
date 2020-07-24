C 
C  $Id: get_phlayr.F,v 1.3 2001/01/26 17:40:35 dubrovin Exp $
C 
C  $Log: get_phlayr.F,v $
C  Revision 1.3  2001/01/26 17:40:35  dubrovin
C  Add partial print out of tuning constants
C
C  Revision 1.2  1999/11/23 02:50:17  jjo
C  Fix Index check to be < 1, not 0
C
C  Revision 1.1  1999/09/20 22:07:53  jjo
C  Add get routines for DR MC constants
C
C 
      SUBROUTINE GET_PHLAYR( Update,
     &                       Index,
     &                       LayerCor )
C 
C  begin_doc
C 
C   Documentation for subroutine get_phlayr
C 
C   Purpose: Returns DRMCPhlayr, pulse height layer conversion factors constants, for given index.
C   -------- Update=.true. updates constants from Cleo3 Frame
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = true -> update constants at begin run time
C   Index - integer - index into constants array
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   LayerCor - Pulse height layer conversion correction constants
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
C   Author:   John J O'Neill      Created:  Mon Sep 20 11:47:56 EDT 1999
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
      real LayerCor
 
C 
C   Local User defined variables:
C   -----------------------------
C   arraySize - Array size passed to C++ function that fills constants
C   kXXXSize  - Size of constants from drmc_param.inc
C   XXXArray  - Array(s) that holds the constants filled from Cleo3 Frame
      integer arraySize
      real LayerCorArray(kPhwireSize)
      save LayerCorArray
      integer ind 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_phlayr.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/01/26 17:40:35 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'get_phlayr' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini
      data rcsini/.true./
 
C   executable code for routine get_phlayr:
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
     &     Index.gt.kPhlayrSize  ) then
        write(chmess,*) 'Illegal Index = ', Index
        call report(MS_EMERGENCY, crname, chmess)
      endif

C     Update constants ?
      if ( Update ) then
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
        chmess ='Updating DRMCLayerCor constants - PH layer conv factor'
        call report(MS_DEBUG, crname, chmess)

C       Request number of elements to be filled
        ArraySize = kPhlayrSize
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants.
C       C++ function does error checking on the array length.
        call drmc_phlayr( LayerCorArray, ArraySize ) 

        do ind=1,kPhlayrSize 
          if( ind.le.20
     >        .or.mod(ind,100).eq.0
     >        .or.ind.eq.kPhlayrSize  ) then
            write(chmess,'(''Index='',I5,'' Ampl.corr.factor='',f5.3)') 
     >                      ind, LayerCorArray(ind)
            call report(MS_INFO, crname, chmess)
          endif
        enddo


      endif

C     Return constants for this index
      LayerCor = LayerCorArray(Index)


      return
      end



