      SUBROUTINE ZD_GET_RESOLUTIONS( Update,
     &                            Index,
     &                            Sigma1, Sigma2,
     &                            Offset1, Offset2,
     &                            Fraction )
C 
C  begin_doc
C 
C   Documentation for subroutine get_resolutions
C 
C   Purpose: Returns ZDMCResolutions, wire resolutions constants, for a given index
C   -------- Update=.true. updates constants from Cleo3 Frame
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = true -> update constants at begin run time
C   Index - integer - index into constants array
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   Resolution constants returned for this wire
C   These constants are used to smear the drift distance resolutions.
C   A double gaussian describes the function
C   Fraction -  ratio of areas: (gaus1)/(gaus1+gaus2) 
C   Sigma1  
C   Sigma2
C   Offset1
C   Offset2
C   Fraction
C
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes: The 2nd Gaussian fit values, Sigma2 & Offst2,
C   ------ are not used.  See cbx 98-70.
C 
C  Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
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
#include "ZDMCUtil/Fortran/zdmc_param.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      logical Update
      integer Index
      real Sigma1, Sigma2, Offset1, Offset2, Fraction
C 
C   Local User defined variables:
C   -----------------------------
C   arraySize - Array size passed to C++ function that fills constants
C   kXXXSize  - Size of constants from zdmc_param.inc
C   XXXArray  - Array(s) that holds the constants filled from Cleo3 Frame
      integer arraySize
      real Sigma1Array(kResolutionsSize),
     &     Sigma2Array(kResolutionsSize),
     &     Offset1Array(kResolutionsSize),
     &     Offset2Array(kResolutionsSize),
     &     FractionArray(kResolutionsSize) 

      integer i
 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_get_resolutions.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:44 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_get_resolutions' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine get_resolutions:
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
     &     Index.gt.kResolutionsSize  ) then
        write(chmess,*) 'Illegal Index = ', Index
        call report(MS_EMERGENCY, crname, chmess)
      endif

C     Update constants ?
      if ( Update ) then
C       Call C++ function in ZDMCUtil/Class/ZDMCxxxFB.cc to get constants 
        chmess ='Updating ZDMCResolutions constants - Wire Resolution'
        call report(MS_DEBUG, crname, chmess)

C       Request number of elements to be filled
        arraySize = kResolutionsSize
C       Call C++ function in ZDMCUtil/Class/ZDMCxxxFB.cc to get constants 
C       C++ function does error checking on the array length.
        call zdmc_resolutions( Sigma1Array,
     &                         Sigma2Array,
     &                         Offset1Array,
     &                         Offset2Array,
     &                         FractionArray,
     &                         arraySize )


        write(chmess,*) 'First entry to get resolutions constants'
        call report(MS_INFO, crname, chmess)

        do i=1,kResolutionsSize
          if( i.le.40
     >        .or.mod(i,100).eq.0
     >        .or.i.eq.kResolutionsSize  ) then

            write(chmess,1100) i,Sigma1Array(i),Sigma2Array(i),
     &         Offset1Array(i),Offset2Array(i),FractionArray(i)
            call report(MS_INFO, crname, chmess)
          endif
        enddo

 1100   format(i5,5f8.2)

      endif


C.....Return constants for given Index
      Sigma1   = Sigma1Array(Index)
      Sigma2   = Sigma2Array(Index)
      Offset1  = Offset1Array(Index)
      Offset2  = Offset2Array(Index)
      Fraction = FractionArray(Index)
      
      return
      end
