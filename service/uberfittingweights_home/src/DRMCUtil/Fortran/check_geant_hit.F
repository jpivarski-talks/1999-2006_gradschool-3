C 
C  $Id: check_geant_hit.F,v 1.9 2003/08/14 18:16:46 dubrovin Exp $
C 
C  $Log: check_geant_hit.F,v $
C  Revision 1.9  2003/08/14 18:16:46  dubrovin
C  Add sinXangle check
C
C  Revision 1.8  2003/01/30 19:23:41  cleo3
C  removed comments after include in order to compile using g77
C
C  Revision 1.7  2001/04/11 15:15:51  dubrovin
C  Add check print for negative Eloss
C
C  Revision 1.6  2000/09/07 22:47:37  dubrovin
C  Add check of NWireInLayer and Zmax_ad
C
C  Revision 1.5  2000/08/02 14:20:04  dubrovin
C  ZMaX has been changed from 1.12 to 1.175
C
C  Revision 1.4  2000/07/07 02:17:09  lkg
C  added x,y position information to the various response utilities
C
C  Revision 1.3  2000/04/18 17:47:47  jjo
C  Change error messages to warnings
C
C  Revision 1.2  2000/02/15 00:27:40  jjo
C  Add purpose to documentation
C
C  Revision 1.1  1999/12/08 00:15:37  jjo
C  Import into cvs
C
C 
      SUBROUTINE CHECK_GEANT_HIT( WireNum, Layer, X, Y, Z,
     &                            cosZ, pathLength,
     &                            Eloss, TOF,
     &                            DriftDist, sinXangle,
     &                            IonDist, MCTrack )
C 
C  begin_doc
C 
C   Documentation for subroutine check_geant_hit
C 
C   Purpose: Check that the DR Geant hit is valid
C   --------
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C     X          - X position of geant hit 
C     Y          - Y position of geant hit 
C     Z          - Z position of geant hit 
C     cosZ       - cos in z direction of geant hit, aka dip angle
C     pathLength - pathLength of geant hit 
C     Eloss      - Energy loss for geant hit 
C     TOF        - TOF for geant hit 
C     DriftDist  - drift distance of geant hit 
C     sinXangle  - sin of crossing angle wrt layer of geant hit 
C     IonDist    - Ion distribution of geant hit 
C     WireNum    - wire number that has geant hit 
C     Layer      - Layer number that has geant hit 
C     Track      - track number that generated geant hit 
C 
C   Output Parameters: None
C   -----------------
C 
C   Other routines:
C   ---------------
C 
C   Called By:  mc_dr_responses.F
C   ----------
C
C   Notes: Simple check of hit parameters validity
C   ------
C 
C   Author:   John J O'Neill      Created:  Tue Dec  7 11:03:19 EST 1999
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

#include "DRMCUtil/Fortran/drmc_param.inc"

#include "cl3seq/cdgm3/cdgeompa.inc"
C for NWIRCD,...,ZENDCD
#include "cl3seq/cdgm3/cdgeomcd.inc"     
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      integer WireNum, Layer, MCTrack
      real X, Y
      real Z, cosZ, pathLength, Eloss, TOF, DriftDist, sinXangle,IonDist

 
C 
C   Local User defined variables:
C   -----------------------------
C     ZMaX - max. value of Z (max. Z of DRWL = 1.1749126)
C     DriftMaX - max drift distance - rough guess at 2cm
C     IonMaX - max Ion distance - rougher guess of 2cm
C     NWireInLayer - max # of wires in Layer
      real ZMaX, DriftMaX, IonMaX
      real Zmax_ad
      integer NWireInLayer

      parameter ( ZMaX = 1.175, DriftMax=0.02, IonMax=0.02 )

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: check_geant_hit.F,v $')
      parameter(chrcsr='$Revision: 1.9 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/08/14 18:16:46 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'check_geant_hit' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine check_geant_hit:
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
        chmess='Checking for valid DR Geant Hits'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

C     WireNum - max. WireNum varies with layer. 
C     call cdhits_nwircd( Layer, NWireInLayer ) 
c       D.M. 09/07/00 add check on NWireInLayer
      NWireInLayer = NWIRCD(Layer)
      if ( WireNum.lt.0 .or. WireNum.ge.NWireInLayer ) then
        write(chmess,*) 'Illegal WireNum=', WireNum
        call report(MS_WARNING, crname, chmess)
        write(chmess,*) 'Layer=',Layer,' NWireInLayer=', NWireInLayer
        call report(MS_WARNING, crname, chmess)
      endif

C     Z 
c       D.M. 09/07/00 ZMaX replaced for Zmax_ad
      Zmax_ad = ZENDCD(Layer)+0.03
      if ( abs(Z).gt.Zmax_ad ) then
        write(chmess,*) 'Illegal Z postion = ',Z
        call report(MS_WARNING, crname, chmess)
        write(chmess,*) 'Layer=',Layer,' Zmax(+0.01m) = ', Zmax_ad
        call report(MS_WARNING, crname, chmess)
      endif

C     cosZ
      if ( abs(cosZ).gt.1 ) then
        write(chmess,*) 'Illegal cosZ = ', cosZ
        call report(MS_WARNING, crname, chmess)
      endif

C     pathLength - not sure about this


C  D.M. 04/11/01 Add this check because Dan C-H observe negative charge.  
C     Eloss
      if ( Eloss .lt. 0. ) then
        write(chmess,*) 'Illegal Eloss = ', Eloss
        call report(MS_WARNING, crname, chmess)
      endif

C     TOF - not sure about this

C     DriftDist
      if ( abs(DriftDist) .gt. DriftMax ) then
        write(chmess,*) 'Illegal Drift Distance = ', DriftDist
        call report(MS_WARNING, crname, chmess)
      endif
        

C     sinXangle
      if ( abs(sinXangle).gt.1 ) then
        write(chmess,*) 'Illegal sinXangle = ', sinXangle, ' restricted'
        call report(MS_WARNING, crname, chmess)
         if( sinXangle.gt.1. )  sinXangle=1.
         if( sinXangle.lt.-1. ) sinXangle=-1.
      endif


C     IonDist
      if ( abs(IonDist) .gt. IonMax ) then
        write(chmess,*) 'Illegal Ion Distance = ', IonDist
        call report(MS_WARNING, crname, chmess)
      endif
        

C     MCTrack
      if ( MCTrack .le. 0 ) then
        write(chmess,*) 'Illegal MC Track # =', MCTrack
        call report(MS_WARNING, crname, chmess)
      endif
        
      return
      end



