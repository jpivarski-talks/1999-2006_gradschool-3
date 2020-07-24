C 
C  $Id: get_deadwire.F,v 1.7 2001/12/18 20:32:47 dubrovin Exp $
C 
C  $Log: get_deadwire.F,v $
C  Revision 1.7  2001/12/18 20:32:47  dubrovin
C  Add parameter ChargeDep at call get_wireeff
C
C  Revision 1.6  2001/04/10 15:27:03  dubrovin
C  Change dead wire threshold
C
C  Revision 1.5  2001/01/16 22:27:13  dubrovin
C  Remove initialization at first call get_wireff
C
C  Revision 1.4  2001/01/05 23:54:21  dubrovin
C  Define dead wires based on WireEff constants
C
C  Revision 1.3  2000/04/18 17:46:19  jjo
C  Change error message to a warning
C
C  Revision 1.2  1999/11/22 01:16:53  jjo
C  Make it clearer that we need to call C++ function to serve real dead
C  wire constants
C
C  Revision 1.1  1999/10/26 18:26:27  jjo
C  Import new routines
C
C 
      SUBROUTINE GET_DEADWIRE( Update,
     &                         Index,
     &                         DeadWire )
C 
C  begin_doc
C 
C   Documentation for subroutine get_deadwire
C 
C   Purpose: Return logical flag true if given DR cellId has 
C   -------- a dead wire.  Update=.true. updates constants 
C            stored in Cleo3 Frame.  
C
C            NOTE: Doesn't get constants from C++ interface objects yet as
C            they haven't been written - we'll print an error message.
C
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   Update - logical = true -> update constants at begin run time
C   Index - integer - index into constants array
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   DeadWire = true if given wire is dead
C
C   Other routines:
C   ---------------
C 
C   Called By: DR users who need dead wires
C   ----------
C
C   Notes:
C   ------
C 
C   Author:   John J O'Neill      Created:  Mon Oct 25 10:22:07 EDT 1999
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

C   DR MC parameters
#include "DRMCUtil/Fortran/drmc_param.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      logical Update
      integer index
      logical DeadWire
 
C 
C   Local User defined variables:
C   -----------------------------
C     DeadWireArray - array that holds DeadWire constants for all wires
      logical DeadWireArray(kNDRWires)
      save    DeadWireArray

      logical UpdateFirst
      real EffWir
      real ChargeDep

C     IWire - loop over wires
      integer IWire

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_deadwire.F,v $')
      parameter(chrcsr='$Revision: 1.7 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/12/18 20:32:47 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'get_deadwire' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine get_deadwire:
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
          lfirst=.false.

      chmess='Determine if given DR wire is dead based on WireEff<0.01'
        call report(MS_INFO, crname, chmess)

*        UpdateFirst = .true. ! Because initialization is doing in other place
         UpdateFirst = .false.

C       Hardwire all wires to be alive until real constants exist

         ChargeDep=1000. ! Effective charge value ABOVE THRESHOLD !

        DO IWire = 1, kNDRWires

          call Get_WireEff( UpdateFirst, IWire, ChargeDep, EffWir )

          if( EffWir .le. 0.01 ) then
            DeadWireArray(IWire) = .TRUE.
          else  
            DeadWireArray(IWire) = .FALSE.
          endif

          if( IWire.le.10
     >        .or.mod(IWire,1000).eq.0
     >        .or.IWire.eq.kNDRWires ) then
            write(chmess,'(''Wire='',I5,'' Efficiency='',f5.3)') 
     >                      IWire, EffWir
            call report(MS_INFO, crname, chmess)
          endif

        ENDDO

      endif

c     Update constants ?
c      if ( Update ) then
c       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
c        chmess ='Updating DRMCSpefud constants - species dependent corr'
c        call report(MS_DEBUG, crname, chmess)
c       Don't have cleo3 dead wire routine for now - routine doesn't exist
c       Need fortran callable C++ function that gets constants from
c       frame to serve us the dead wire constants.
c        call dr_deadwire()
c      endif

C     Return DeadWire for given Index (=wire number)
      DeadWire = DeadWireArray(Index) 

      return
      end



