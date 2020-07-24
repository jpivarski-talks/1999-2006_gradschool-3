C 
C  $Id: get_phwire.F,v 1.4 2001/01/26 17:40:03 dubrovin Exp $
C 
C  $Log: get_phwire.F,v $
C  Revision 1.4  2001/01/26 17:40:03  dubrovin
C  Add partial print out of tuning constants
C
C  Revision 1.3  1999/11/23 02:50:18  jjo
C  Fix Index check to be < 1, not 0
C
C  Revision 1.2  1999/10/20 21:41:24  jjo
C  Get DR MC Response resolution constants installed
C
C  Revision 1.1  1999/09/20 22:07:54  jjo
C  Add get routines for DR MC constants
C
C 
      SUBROUTINE GET_PHWIRE( Update,
     &                       Index,
     &                       WireCor )
C 
C  begin_doc
C 
C   Documentation for subroutine get_phwire
C 
C   Purpose: Returns DRMCPhwire, Pulse height wire to wire correction constants, for given index
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
C   Author:   John J O'Neill      Created:  Mon Sep 20 11:47:48 EDT 1999
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
      real WireCor
 
C 
C   Local User defined variables:
C   -----------------------------
C   kXXXSize  - Size of constants from drmc_param.inc
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
      parameter(chrcss='$RCSfile: get_phwire.F,v $')
      parameter(chrcsr='$Revision: 1.4 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/01/26 17:40:03 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'get_phwire' )
 
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
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
        chmess ='Updating DRMCPhwire constants - wire to wire correct.'
        call report(MS_DEBUG, crname, chmess)

C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants.
C       C++ function does error checking on the array length.
        call drmc_phwire( WireCorArray, kPhwireSize ) 


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



