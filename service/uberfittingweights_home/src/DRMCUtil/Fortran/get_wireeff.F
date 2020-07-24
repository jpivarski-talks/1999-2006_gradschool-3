C 
C  $Id: get_wireeff.F,v 1.5 2001/12/18 20:36:05 dubrovin Exp $
C 
C  $Log: get_wireeff.F,v $
C  Revision 1.5  2001/12/18 20:36:05  dubrovin
C  Add parameter ChargeDep. Add smooth pulse hight threshold for efficiency
C
C  Revision 1.4  2001/01/05 23:52:49  dubrovin
C  Add access to real WireEff constants
C
C  Revision 1.3  1999/12/14 00:00:05  jjo
C  Change debug printout if wireEff > 1, not = 1
C
C  Revision 1.2  1999/11/23 02:50:20  jjo
C  Fix Index check to be < 1, not 0
C
C  Revision 1.1  1999/09/20 22:07:59  jjo
C  Add get routines for DR MC constants
C
C 
      SUBROUTINE Get_WireEff( Update,
     &                        Index,
     &                        ChargeDep,
     &                        WireEff )
C 
C  begin_doc
C 
C   Documentation for subroutine Get_WireEff
C 
C   Purpose: Returns DRMCWireff, overall efficiency constants, for a given index.
C   -------- Update=.true. updates constants from Cleo3 Frame
C 
C   Input Parameters:  
C   ----------------
C   Update - logical = true -> update constants at begin run time
C   Index - integer - index into constants array 
C                     Logical Address in DR3
C   ChargeDep - pulse hight
C
C   Output Parameters: (Name - Meaning)
C   -----------------
C   WireEff - real  - overall efficiency for this Index (wire)
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
C   Author:   John J O'Neill      Created:  Wed Sep  1 11:47:12 EDT 1999
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
      INTEGER Index
      REAL    ChargeDep, WireEff

 
C 
C   Local User defined variables:
C   -----------------------------
C   arraySize - Array size passed to C++ function that fills constants
C   kXXXSize  - Size of constants from drmc_param.inc
C   XXXArray  - Array(s) that holds the constants filled from Cleo3 Frame
      integer arraySize
      real WireEffArray(kWireEffSize)
      save WireEffArray 

      real pi, A, B, D, x, thresh
      data pi/3.1415927/


C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: get_wireeff.F,v $')
      parameter(chrcsr='$Revision: 1.5 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/12/18 20:36:05 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'Get_WireEff' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine Get_WireEff:
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
     &     Index.gt.kWireEffSize ) then
        write(chmess,*) 'Illegal Index = ', Index
        call report(MS_EMERGENCY, crname, chmess)
      endif


C     Update constants ?
      if ( Update ) then
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
        chmess ='Updating DRMC WireEff constants - Eff for a given Wire'
        call report(MS_DEBUG, crname, chmess)

C       Request number of elements to be filled
        ArraySize = kWireEffSize
C       Call C++ function in DRMCUtil/Class/DRMCxxxFB.cc to get constants 
C       C++ function does error checking on the array length.
        call drmc_wireff( WireEffArray, ArraySize ) 
      endif

C     set WireEff for this index
      WireEff = WireEffArray(Index)

c ---------------------------------
c 2001/12/17 D.M. 
c Apply threshold dependence of efficiency on pulse hight
c The threshold has a shape of a pice of 1-cos function on the range [A,B]
c The threshold function has a different parameters for axial and stereo layers

      x = ChargeDep

      if ( Index .le. 1696 ) then  ! Threshold for 16 axial layers

        A=23.
        B=48.
        D=B-A
        if(x.gt.B) then
           thresh = 1.
        else if(x.lt.A) then
           thresh = 0.        
        else
           thresh = 0.5*(1.-cos(pi*(x-A)/D))
        endif

      else                         ! Threshold for stereo layers

        A=18.
        B=45.
        D=B-A
        if(x.gt.B) then
           thresh = 1.
        else if(x.lt.A) then
           thresh = 0.        
        else
           thresh = 0.5*(1.-cos(pi*(x-A)/D))
        endif

      endif


      WireEff = WireEff * thresh

*      write(chmess,*) ' ChargeDep, thresh, WireEff = ', 
*     &                  ChargeDep, thresh, WireEff
*        call report(MS_INFO, crname, chmess)

c ---------------------------------      
C     Precision in zfiles can make WireEff < 0 (i.e., for dead wires)
C     so check for Cleo3 as well.  
      IF ( WireEff .LT. 0.0 ) then
        chmess='WireEff < 0.0 Set = 0.0'
        call report(MS_WARNING, crname, chmess)
        WireEff = 0.
      endif
      
      IF ( WireEff .GT. 1.0 ) then 
        write(chmess,*) 'WireEff=', WireEff, '  Set = 1.0'
        call report(MS_DEBUG, crname, chmess)
        WireEff = 1.
      endif

      return
      end




