C 
C  $Id: get_spefud.F,v 1.3 1999/11/23 02:50:19 jjo Exp $
C 
C  $Log: get_spefud.F,v $
C  Revision 1.3  1999/11/23 02:50:19  jjo
C  Fix Index check to be < 1, not 0
C
C  Revision 1.2  1999/10/20 21:41:26  jjo
C  Get DR MC Response resolution constants installed
C
C  Revision 1.1  1999/09/20 22:07:57  jjo
C  Add get routines for DR MC constants
C
C 
      SUBROUTINE GET_SPEFUD( Update,
     &                       Index,
     &                       ElecFudge,
     &                       MuonFudge,
     &                       PionFudge,
     &                       KaonFudge,
     &                       ProtonFudge )
C 
C  begin_doc
C 
C   Documentation for subroutine get_spefud
C 
C   Purpose: Returns DRMCSpefud, species dependent fudge constants, for a given index
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
C   Author:   John J O'Neill      Created:  Mon Sep 20 11:47:34 EDT 1999
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
      REAL ElecFudge,
     &     MuonFudge,
     &     PionFudge,
     &     KaonFudge,
     &     ProtonFudge
 
C 
C   Local User defined variables:
C   -----------------------------
C   arraySize - Array size passed to C++ function that fills constants
C   kXXXSize  - Size of constants from drmc_param.inc
C   XXXArray  - Array(s) that holds the constants filled from Cleo3 Frame
      integer arraySize
      real ElecFudgeArray(kSpeFudSize),
     &     MuonFudgeArray(kSpeFudSize),
     &     PionFudgeArray(kSpeFudSize),
     &     KaonFudgeArray(kSpeFudSize),
     &     ProtonFudgeArray(kSpeFudSize)

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_spefud.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/11/23 02:50:19 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'get_spefud' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine get_spefud:
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

C     Valid Index ?
      if ( Index.lt.1 .or.
     &     Index.gt.kSpeFudSize  ) then
        write(chmess,*) 'Illegal Index = ', Index
        call report(MS_EMERGENCY, crname, chmess)
      endif

C     Update constants ?
      if ( Update ) then
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
        chmess ='Updating DRMCSpefud constants - species dependent corr'
        call report(MS_DEBUG, crname, chmess)

C       Request number of elements to be filled
        ArraySize = kSpeFudSize
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants
C       C++ function does error checking on the array length.
        call drmc_spefud( ElecFudgeArray,
     &                    MuonFudgeArray,
     &                    PionFudgeArray,
     &                    KaonFudgeArray,
     &                    ProtonFudgeArray,
     &                    arraySize ) 
      endif

C     Return constants for this index
      ElecFudge   = ElecFudgeArray(Index)
      MuonFudge   = MuonFudgeArray(Index)
      PionFudge   = PionFudgeArray(Index)
      KaonFudge   = KaonFudgeArray(Index)
      ProtonFudge = ProtonFudgeArray(Index)
      
      return
      end



