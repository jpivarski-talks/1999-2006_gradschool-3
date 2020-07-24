C 
C  $Id: wire_eff.F,v 1.8 2001/12/18 20:27:05 dubrovin Exp $
C 
C  $Log: wire_eff.F,v $
C  Revision 1.8  2001/12/18 20:27:05  dubrovin
C  Add parameter ChargeDep at call get_wireeff
C
C  Revision 1.7  2000/02/15 00:36:00  jjo
C  Print error warning if drift distance is outside sqrt(2)*cell size.
C  Change debug printout to use MS_DEBUG.
C
C  Revision 1.6  1999/12/13 01:50:29  jjo
C  Use report for debug messages
C
C  Revision 1.5  1999/12/08 00:18:50  jjo
C  Reduce/improve printouts
C
C  Revision 1.4  1999/11/23 02:46:08  jjo
C  Add layer in call to get_coseff which was missing
C
C  Revision 1.3  1999/11/22 23:05:10  jjo
C  Improve debugging printout
C
C  Revision 1.2  1999/11/22 01:18:02  jjo
C  Fix bug in call to get_lyreff (layer not cellid),
C  Improve protection from divid by zero,
C  use DEBUG_RESPONSE for debugging
C
C  Revision 1.1  1999/10/26 18:25:21  jjo
C  Rename dr_eff.F to wire_eff.F
C
C  Revision 1.2  1999/10/20 21:41:19  jjo
C  Get DR MC Response resolution constants installed
C
C  Revision 1.1  1999/09/20 22:48:06  jjo
C  Import files into cvs
C
C 
      SUBROUTINE WIRE_EFF( GeantTrkPtr, CellId, Layer, 
     &                     DIST, COSTH, SINA, EFFOUT )
C 
C  begin_doc
C 
C   Documentation for subroutine wire_eff
C 
C   Purpose: Returns efficiency for readout of DR hit for a given CellId.
C   --------
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   GeantTrkPtr - Zebra ptr to Geant track that caused current hit
C                 Used to lookup e/mu/pi/K/p of track for species corrections
C   CellId - DR cell ID
C   Layer  - DR layer number
C   DIST   - Unsmeared Drift Distance (signed)
C   COSTH  - Cos(theta)
C   SINA   - Sine of entrance angle (unused)
C 
C   Output Parameters: (Name - Meaning)
C   -----------------
C   EFFOUT - efficiency for this hit
C
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes: Based on Cleo-II cleog/crawm/cdreff.F
C   ------
C   Returns efficiency based on CellId in DR3, signed drift distance, 
C   entrance angle, and cos(theta).  Efficiencies are stored in constants 
C   database in 20 bins of drift distance.
C 
C   Author:   John J O'Neill      Created:  Tue Aug 17 11:20:15 EDT 1999
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

C   Geant includes
#include "geant321/gcbank.inc"
#include "geant321/gcflag.inc"

C   cdgeom includes
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      INTEGER  GeantTrkPtr, CellId, Layer
      REAL     DIST, COSTH, SINA, EFFOUT
 
C 
C   Local User defined variables:
C   -----------------------------
      integer arraySize, kLyreffSize
      parameter ( kLyreffSize =  47)

C     Variables to describe fit to layer efficiency.
C     Fit layer eff. to have '2 legs on each side + 2nd order Cheb. with
C                             a "tooth" in the center'
C     or in different words:
C     The efficiency as a function of scaled drift distance is modeled
C     by fitting a data distribution of nonstale hit efficiencies for each
C     layer to a 2nd order Chebyshev Polynomial in the center of the cell,
C     with linear falloffs at the edges of the cell.  In addition, a symmetric
C     notch is added centered on the wire to model the efficiency 
C     drop there.  The notch has a slope (same on both sides of the wire), 
C     and a distance (on the pos. side) as constants.

C DFRACT   : Signed drift distance / (0.5 * full cell width)
C SlopeNeg : slope of efficiency falloff for DFRACT < DISTN
C SlopePos : slope of efficiency falloff for DFRACT > DISTP
C DistNeg  : distance on neg. side of wire at which efficiency drops
C DistPos  : distance on pos. side of wire at which efficiency drops
C Norm     : overall normalization of Chebyshev
C Cheb1    : 1st order term
C Cheb2    : 2nd order term
C SlopeM   : Slope of notch
C XR       : distance on pos. side where notch matches up with Chebyshev

      REAL SlopeNeg,
     &     SlopePos,
     &     DistNeg,
     &     DistPos,
     &     Norm, 
     &     Cheb1, 
     &     Cheb2, 
     &     SlopeM, 
     &     Xr


C Variables taken from cdreff.F
C Stuff for wire efficiency:
      REAL          PI
      PARAMETER (   PI = 3.141592654 )

      INTEGER ISBIN

C DFRACT     - Signed drift distance / (0.5 * full cell width)
      REAL DFRACT

      REAL YN, YP, BL, BH, YL, YH,
     &     YR, Y0, DISC, X1, X2, XL


C     LayerEff - eff. for the whole layer; i.e. of the average wire 
C              in the layer, averaged over all drift and cos(theta)
      REAL         LayerEff, EFFWIR, Y, EFFIC


C  For CosEff constants
      REAL CosEff_Min, CosEff_Max,
     &     CosEff_Cheb1, CosEff_Cheb2

C     local copy constants from above constants.
C     Cross1,Cross2 = Min,Max.
C     I think these constants are stored in database
C     using other names due to cleo2 history.  
C     Need to change for Cleo3.
      real CHEB1_COS, CHEB2_COS, CROSS1, CROSS2

      REAL EFFCOS

C     Species eff. dependent correction.  Currently, only electrons are used.
      REAL ElecFudge,
     &     MuonFudge,
     &     PionFudge,
     &     KaonFudge,
     &     ProtonFudge


C     NoConstantsUpdate - tell constants access subroutines to update constants
      logical NoConstantsUpdate
      parameter ( NoConstantsUpdate = .false. )


C     For particle species dependent corrections:
C     particleSpecies = 1 for electrons, other particle species not used
C     kParticleOffset - Offset in Geant JKINE bank for particle type
      integer particleSpecies, kParticleOffset 
      parameter (kParticleOffset = 5)

C     ValidConstants - is there a problem w/ the constants ?
      logical ValidConstants

C     CellWidth - full width of cell
      real CellWidth
      real ChargeDep      

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: wire_eff.F,v $')
      parameter(chrcsr='$Revision: 1.8 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/12/18 20:27:05 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'wire_eff' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine wire_eff:
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
        chmess='Returns efficiency for readout of DR hit'
        call report(MS_INFO, crname, chmess)
        lfirst=.false.
      endif


C     Initialize
      ValidConstants = .TRUE.
      EFFWIR = 0.0
      LayerEff = 0.0
      EFFCOS = 0.0
      EFFOUT = 1.
      
C     Get DRMCLyreff constants
      call get_lyreff( NoConstantsUpdate,
     &                 Layer, 
     &                 SlopeNeg, SlopePos,
     &                 DistNeg, DistPos,
     &                 Norm,
     &                 Cheb1, Cheb2,
     &                 SlopeM,
     &                 Xr )

c     Protect against division by zero
C     TODO: Require this in constants finding job 
C     Old method would be to set the constants to a small, non-zero
C     number which is very arbitrary.
      if ( SlopePos.eq.0.0  .or.
     &     SlopeNeg.eq.0.0  .or.
     &     cheb2   .eq. 0. )     then
        chmess = 'Illegal constants would result in  Division by zero'
        call report(MS_EMERGENCY, crname, chmess)        
        write(chmess,*) 'Layer =', Layer
        call report(MS_EMERGENCY, crname, chmess)
        write(chmess,*) 'SlopePos, SlopeNeg, cheb2',
     &                   SlopePos, SlopeNeg, cheb2
        call report(MS_EMERGENCY, crname, chmess)
        chmess = 'Constants finding job must require non-zero constants'
        call report(MS_EMERGENCY, crname, chmess)
        chmess = 'Will stop job now'
        call report(MS_EMERGENCY, crname, chmess)
        stop
      endif


C     get the DOCA and convert to fraction of cell
      CellWidth = 2*PI*RCD(Layer)/NWIRCD(Layer)
      DFRACT = DIST / (CellWidth/2.)


C     Note: CleoII.5 code disables sina binning
C     get and bin entrance angle in 10 bins
c         ISBIN  = (10*SINA + 10)/2 + 1
c         IF ( ISBIN .LT.  1 ) ISBIN = 1
c         IF ( ISBIN .GT. 10 ) ISBIN = 10
c
C     Disable SINA binning
      ISBIN = 1


c      Note: cleo3 dont need layer # in device - just the layer #
C      Get the layer number in the device IDEV
c       ICYL  = IXSFCD(Layer)


C     Calculate the efficiency for the layer by integrating the 
C     shape stored in constants database
      YN = Norm + Cheb1 * DistNeg +
     &     Cheb2 * (2*distNeg*DistNeg - 1)

      YP = Norm + Cheb1 * distPos +
     &     Cheb2 * (2*distPos*distPos - 1)

      BL = YN - (SlopeNeg * DistNeg)
      YL = (-1.0 * SlopeNeg) + BL
      BH = YP - (SlopePos * DistPos)
      YH = ( 1.0 * SlopePos) + BH

C     Calculate coords of cheb/line match.
      YR     = Norm + Cheb1 * Xr +
     &         Cheb2 * (2*Xr*Xr - 1)

      Y0     = YR - (SlopeM * Xr)
      DISC   = (Cheb1 + SlopeM)**2 - 
     &         4*(2*Cheb2)*(Norm - Y0 - Cheb2)
      IF ( DISC .GT. 0. ) THEN
          X1 = (-(Cheb1+SlopeM) +
     &         SQRT(DISC))/(2*(2*Cheb2))
          X2 = (-(Cheb1+SlopeM) -
     &         SQRT(DISC))/(2*(2*cheb2))
          XL = X1
          IF ( XL .GT. 0. ) XL = X2
      ELSE
          XL = 0.
      ENDIF

C     What is the efficiency for this hit?
      X1 = -BL/SlopeNeg
      IF ( (X1 .LT. -1.) .AND. (DFRACT .LT. X1) ) THEN
C         There is a problem so set eff=0
          EFFIC = 0.
          ValidConstants = .FALSE.
C          It seems that the problem is always DFRACT to big
C          print*,crname,' Fails X1.LT.-1. & DFRACT .LT. X1'
C          print*,crname,' X1=',X1, 'DFRACT=',DFRACT
      ENDIF
      X1 = -BH/SlopePos
      IF ( (X1 .GT.  1.) .AND. (DFRACT .GT. X1) ) THEN
C         There is a problem so set eff=0
          EFFIC = 0.
          ValidConstants = .FALSE.
C          It seems that the problem is always DFRACT to big
C          print*,crname,' Fails X1.GT.1. & DFRACT .GT. X1'
c          print*,crname,' X1=',X1, 'DFRACT=',DFRACT
      ENDIF

      IF ( DFRACT .LE. DistNeg ) THEN
          Y  = (SlopeNeg * DFRACT) + BL
      ELSEIF ( DFRACT .LT. XL ) THEN
          Y  = Norm + Cheb1 * DFRACT +
     &       CHEB2 * (2*DFRACT*DFRACT - 1.)
      ELSEIF ( DFRACT .LT. 0.0D0 ) THEN
          Y  = Y0 - SlopeM*DFRACT
      ELSEIF ( DFRACT .LT. Xr ) THEN
          Y  = Y0 + SlopeM*DFRACT
      ELSEIF ( DFRACT .LT. DistPos ) THEN
          Y  = Norm + Cheb1 * DFRACT +
     &         Cheb2 * (2*DFRACT*DFRACT - 1.)
      ELSE
          Y  = (slopePos * DFRACT) + BH
      ENDIF

      EFFIC = Y

C     If we are outside the cell and EFFIC is less than zero, set EFFIC to
C     zero and be done with it.
      IF ( (ABS(DFRACT) .GT. SQRT(2.0) ) .AND. (EFFIC .LT. 0.) ) THEN
        write(chmess,*) 'Bad Drift Fraction & EFFIC<0: dfract=', dfract,
     &       ' EFFIC=',EFFIC, '  Will Set Eff=0'
        call report(MS_WARNING, crname, chmess)        

C       There is a problem so set eff=0
        EFFIC = 0.
        ValidConstants = .FALSE.
      ENDIF

C     Print message if DFRACT>SQRT(2) when effic is Ok (>=0.)
      IF ( (ABS(DFRACT) .GT. SQRT(2.0) ) .AND. (EFFIC .GE. 0.) ) THEN
        write(chmess,*) 'Bad Drift Fraction & EFFIC>=0: dfract=',dfract
        call report(MS_WARNING, crname, chmess)        
      ENDIF



#if defined(DEBUG_RESPONSE)
C     It seems that invalid constants are due to DFRACT
      if (.not.ValidConstants ) then
        write(chmess,*) ' DFRACT        =', DFRACT
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' CellWidth     =', CellWidth
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' RCD(Layer)    =', RCD(Layer)
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' 2*pi*R(layer) =', 2*PI*RCD(Layer)
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' NWIRCD(Layer) =', NWIRCD(Layer)
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' -BL/SLOPN     =', -BL/SLOPENEG
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' -BH/SLOPP     =', -BH/SLOPEPOS
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' BH            =', BH
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' BL            =', BL
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' YP            =', YP
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' YN            =', YN
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' SlopeNeg      =', SlopeNeg
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' SlopePos      =', SlopePos
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' DistNeg       =', DistNeg
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' DistPos       =', DistPos
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' Norm          =', Norm
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' Cheb1         =', Cheb1
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' Cheb2         =', Cheb2
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' SlopeM        =', SlopeM
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) ' Xr            =', Xr
        call report(MS_DEBUG, crname, chmess)       
      endif
#endif        

C     If above constants are not valid don't bother with 
C     cos(theta), wire eff, layer eff & species corrections
      if ( ValidConstants ) then

C       Unpack the constants for cos(theta) corrections.
C       Translation from database variable names to names 
C       used in this routine - why different names ?
C       Existing constants entities were reused in Cleo2 and
C       Cleo3 inherited this.  Need to update for cleo3
C       CROSS1    = COSEFF.MIN
C       CROSS2    = COSEFF.MAX
C       CHEB1_COS = COSEFF.CHEB1
C       CHEB2_COS = COSEFF.CHEB2

        call get_coseff( NoConstantsUpdate,
     &                   Layer,
     &                   CosEff_Min,
     &                   CosEff_Max,
     &                   CosEff_Cheb1,
     &                   CosEff_Cheb2, arraySize)

C       Copy into local variables.  This isn't needed.  Done to make
C       it easier to use cleo2 code.  Should be fixed.
        CROSS1    = CosEff_Min
        CROSS2    = CosEff_Max
        CHEB1_COS = CosEff_Cheb1
        CHEB2_COS = CosEff_Cheb2

        EFFCOS=1. + CHEB1_COS * COSTH * (1. + CROSS1 * DFRACT * DFRACT)+ 
     &       CHEB2_COS * COSTH * COSTH * (1. + CROSS2 * DFRACT * DFRACT)

#if defined(DEBUG_RESPONSE)
        WRITE(CHMESS,*) ' DEBUGGING COSTH CONSTANTS'
        call report(MS_DEBUG, crname, chmess)       
        WRITE(CHMESS,*) 'CROSS1=',CROSS1
        call report(MS_DEBUG, crname, chmess)       
        WRITE(CHMESS,*) 'CROSS2=',CROSS2
        call report(MS_DEBUG, crname, chmess)       
        WRITE(CHMESS,*) 'CHEB1_COS=',CHEB1_COS
        call report(MS_DEBUG, crname, chmess)       
        WRITE(CHMESS,*) 'CHEB2_COS=',CHEB2_COS
        call report(MS_DEBUG, crname, chmess)       
        WRITE(CHMESS,*) 'EFFCOS=',EFFCOS
        call report(MS_DEBUG, crname, chmess)       
#endif

C       The efficiency for the average channel at this drift and cos(theta) is:
        EFFIC = EFFIC*EFFCOS

        IF ( EFFIC .GT. 1. ) EFFIC = 1.


         ChargeDep=1000. ! Effective charge value ABOVE THRESHOLD !
C       Now get the overall efficiency for this channel
        CALL Get_WireEff( NoConstantsUpdate,
     &                    CellId,
     &                    ChargeDep,
     &                    EFFWIR )


c       Calculating Average layer efficiency via get_layer_eff
        call get_layer_eff( NoConstantsUpdate,
     &                    Layer,
     &                    LayerEff )

c       Protect against division by zero
        if ( LayerEff .eq. 0.0 ) then
          chmess = 'Illegal constants would result in  Division by zero'
          call report(MS_EMERGENCY, crname, chmess)        
          write(chmess,*) 'LayerEff=',LayerEff
          stop
        endif

C       LayerEff is the efficiency for average channel in the layer.  We have to 
C       scale EFFIC up or down depending what the efficiency of the wire was.
C       If it is greater than average, scale down the ineffiency; if less than, 
C       scale down the efficiency.  Do this carefully so that the efficiency 
C       curve is ALWAYS between 0 and 1.
        IF ( EFFWIR .LT. LayerEff ) THEN
            EFFIC = EFFIC * EFFWIR/LayerEff
        ELSE
            EFFIC = 1. - (1. - EFFIC)*(1. - EFFWIR)/(1. - LayerEff)
        ENDIF

C       The 2D fit is not perfect...
        IF     ( EFFIC .LT. 0. ) THEN
          EFFIC = 0.
        ELSEIF ( EFFIC .GT. 1. ) THEN
          EFFIC = 1.
        ENDIF


C       Apply species dependednt fudges.
C       Is Geant track an electron or positron ?
        if( (Q(LQ(JKINE-GeantTrkPtr)+kParticleOffset) .EQ. 2) .OR.
     +      (Q(LQ(JKINE-GeantTrkPtr)+kParticleOffset) .EQ. 3) ) then
          particleSpecies = 1
        else
          particleSpecies = 0
        endif

C       Only electrons are implemented here.
        IF ( particleSpecies .EQ. 1 ) THEN
          call get_spefud( NoConstantsUpdate,
     &                     Layer,
     &                     ElecFudge, 
     &                     MuonFudge, 
     &                     PionFudge, 
     &                     KaonFudge, 
     &                     ProtonFudge )

C         This needs to be formulated better
          if ( ElecFudge .LT. 0. ) then
            EFFIC = EFFIC * (1.0 + ElecFudge)
          else
            EFFIC = 1.0 - (1.0-EFFIC) * (1.0-ElecFudge)
          endif
        endif

      endif  ! checks if lyreff constants are valid

      EFFOUT = EFFIC

#if defined(DEBUG_RESPONSE)
      write(chmess,*) ' ------- begin WIRE_EFF debug info -------'
      call report(MS_DEBUG, crname, chmess)       
      write(chmess,*) 'CellID         =',CellID
      call report(MS_DEBUG, crname, chmess)       
      write(chmess,*) 'Layer          =',Layer
      call report(MS_DEBUG, crname, chmess)       
      write(chmess,*) 'DRIFT Dist     =',Dist
      call report(MS_DEBUG, crname, chmess)       
      write(chmess,*) 'COSTH          =',COSTH
      call report(MS_DEBUG, crname, chmess)       
      write(chmess,*) 'SINA           =',SINA
      call report(MS_DEBUG, crname, chmess)       
      write(chmess,*) 'ValidConstants =',ValidConstants
      call report(MS_DEBUG, crname, chmess)       
C     only print constants if they're valid
      if ( ValidConstants ) then
        write(chmess,*) 'EFFWIR         =',EFFWIR
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) 'LayerEff       =',LayerEff
        call report(MS_DEBUG, crname, chmess)       
        write(chmess,*) 'EFFCOS         =',EFFCOS
        call report(MS_DEBUG, crname, chmess)       
      endif
      write(chmess,*) 'EFFOUT         =',EFFOUT
      call report(MS_DEBUG, crname, chmess)       
      write(chmess,*) ' ------- end WIRE_EFF debug info -------'
      call report(MS_DEBUG, crname, chmess)       
#endif

      return
      end



