      SUBROUTINE ZD_CHECK_GEANT_HIT( WireNum, Layer, X, Y, Z,
     &                            cosZ, pathLength,
     &                            Eloss, TOF,
     &                            DriftDist, sinXangle,
     &                            IonDist, MCTrack )
C 
C  begin_doc
C 
C   Documentation for subroutine check_geant_hit
C 
C   Purpose: Check that the ZD Geant hit is valid
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
C   Called By:  mc_zd_responses.F
C   ----------
C
C   Notes: Simple check of hit parameters validity
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

#include "ZDMCUtil/Fortran/zdmc_param.inc"

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
C     ZMaX - max. value of Z (max. Z of ZDWL = 1.1749126)
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
      parameter(chrcss='$RCSfile: zd_check_geant_hit.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:40 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_check_geant_hit' )
 
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
        chmess='Checking for valid ZD Geant Hits'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

C     WireNum - max. WireNum varies with layer. 
C     call cdhits_nwircd( Layer, NWireInLayer ) 
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
        write(chmess,*) 'Illegal sinXangle = ', sinXangle
        call report(MS_WARNING, crname, chmess)
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
