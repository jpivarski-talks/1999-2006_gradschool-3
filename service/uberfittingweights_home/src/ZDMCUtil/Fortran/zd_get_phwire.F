      SUBROUTINE ZD_GET_PHWIRE( Update,
     &                       Index,
     &                       WireCor )
C 
C  begin_doc
C 
C   Documentation for subroutine get_phwire
C 
C   Purpose: Returns ZDMCPhwire, Pulse height wire to wire correction constants, for given index
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

C   parameters including array sizes
#include "ZDMCUtil/Fortran/zdmc_param.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      logical Update
      integer index
      real WireCor
 
C 
C   Local User defined variables:
C   -----------------------------
C   kXXXSize  - Size of constants from zdmc_param.inc
C   XXXArray  - Array(s) that holds the constants filled from Cleo3 Frame
      real WireCorArray(kPhwireSize) 
      save WireCorArray
      integer IWire
C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_get_phwire.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:44 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_get_phwire' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini
      data rcsini/.true./
 
C   executable code for routine get_phwire:
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
     &     Index.gt.kPhwireSize  ) then
        write(chmess,*) 'Illegal Index = ', Index
        call report(MS_EMERGENCY, crname, chmess)
      endif

C     Update constants ?
      if ( Update ) then
C       Call C++ function in ZDMCUtil/Class/ZDMCxxxFB.cc to get constants 
        chmess ='Updating ZDMCPhwire constants - wire to wire correct.'
        call report(MS_DEBUG, crname, chmess)

C       Call C++ function in ZDMCUtil/Class/ZDMCxxxFB.cc to get constants.
C       C++ function does error checking on the array length.
        call zdmc_phwire( WireCorArray, kPhwireSize ) 


        do IWire=1,kPhwireSize
          if( IWire.le.10
     >        .or.mod(IWire,1000).eq.0
     >        .or.IWire.eq.kPhwireSize ) then
            write(chmess,'(''Wire='',I5,'' Ampl.corr.factor='',f5.3)') 
     >                      IWire, WireCorArray(IWire)
            call report(MS_INFO, crname, chmess)
          endif
        enddo


      endif

C     Return constants for this index
      WireCor = WireCorArray(Index)
      
      return
      end
