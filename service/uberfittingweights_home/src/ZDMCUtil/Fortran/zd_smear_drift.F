      SUBROUTINE ZD_SMEAR_DRIFT(LAYER, DRIFT, DRIFTIONS, COSTH, 
     &                       ChargeDep, IDHit, DRFTSM)
C 
C  begin_doc
C 
C   Documentation for subroutine smear_drift
C 
C   Purpose: Smears the distance DRIFT by a function described by two gaussians.
C   -------- Based on CleoII.5 routine cleog/crawm/zdsmer.F
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   LAYER  - CD layer number
C   DRIFT  - drift distance to wire D.C.A. in meters (signed)
C   DRIFTIONS  - drift distance to wire (after ion stat.corr)in meters (signed)
C   COSTH  - cos(theta) of the track
C   ChargeDep - Signal pulse hight
C   IDHit  - GEANT particle ID for this hit 2=pos., 3=elec.,...
C   
C   Output Parameters: (Name - Meaning)
C   -----------------
C   DRFTSM - smeared drift distance
C
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes: 
C   ------
C   Reads the ZDMCResolutions constants
C
C   This routine smears the drift distance by a function described
C   by two gaussians. The gaussians are described by a sigma
C   (SIGMA1 and SIGMA2) and an offset from zero (OFFST1 and OFFST2).
C   Each of these values is in microns (10**-6). The relative
C   weights of the two gaussians is determined from the variable
C   FRAC which is the ratio (gaus1)/(gaus1+gaus2) where gaus1 is the
C   area of the first gaussian. The variables are binned by
C   the layer number and by the distance of the track to the center of
C   the cell.
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

C   cdgeom includes
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      INTEGER LAYER, IDHit
      REAL DRIFT, DRIFTIONS, COSTH, ChargeDep, DRFTSM
C
C   Local User defined variables:
C   -----------------------------
      real drfrac
      INTEGER ibin

      real HalfDriftCell 
      real spread_cor_factor

C     Index - index into constants for this drift distance, layer
      INTEGER MAXBIN, NBITEM, Index
      PARAMETER ( NBITEM =  5 )
      PARAMETER ( MAXBIN = 20 )

C     ZDMCResolutions constants - drift distance resolutions
      real Sigma1, Sigma2, Offset1, Offset2, Fraction 

      REAL XMICRN
      PARAMETER ( XMICRN = 1.0E-6 )

C     Offset - ???
C     Sigma
C     
      real Offset, Sigma

C     Random numbers
      REAL   RNDM
      REAL   RAN1,RAN2
      REAL*8 RANDOM_MODULE_FLAT
      REAL*8 RANDOM_MODULE_GAUSS

C     kNOUPDATE - Do not update constants in call to ZDMCUtil/Fortran/get_resolutions
      logical kNOUPDATE
      parameter ( kNOUPDATE = .false. )

c   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_smear_drift.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:46 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_smear_drift' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine smear_drift:
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
        chmess='Smear drift distance by two gaussians'
        call report(MS_INFO, crname, chmess)
      endif

C     Initialize DRFTSM - smeared drift distance with the actual drift distance
      DRFTSM = DRIFTIONS


c     Find the half length of the drift cell for this layer
c     where  CELLCD(LAYER) is a Width of cell in PHI (radians)
      HalfDriftCell = 0.5*RCD(LAYER)*CELLCD(LAYER)

C     Find the bin corresponding to the drift distance.
      DRFRAC = DRIFT / HalfDriftCell
      ibin   = MIN(INT((DRFRAC + 1.)*MAXBIN*0.5)+1, MAXBIN)
      ibin   = MAX(ibin, 1)


C     Determine the smearing variables 
C     Each item is a two dim array of MAXBIN columns and KLYRCD rows.
      Index  = IBIN + (LAYER-1)*MAXBIN

C     Get drift function resoltion (ZDMCResolutions) constants
      call zd_get_resolutions( kNOUPDATE,
     &                      Index,
     &                      Sigma1, Sigma2,
     &                      Offset1, Offset2,
     &                      Fraction )
      
C     Commented out in CleoII.5 code
C     Adjust the core gaussian according to its sin(theta) dependence
C     for ZD axial wires only
C          IF ( (IDVC .EQ. IDRFT) .AND. 
C     +         (.NOT. LSTLCD(LAYER)) ) THEN
C              SIGMA1 = SIGMA1*SQRT(1.10*SINTH)
C          ENDIF

      if( IDHit.eq.2 .or. IDHit.eq.3 ) then 
         spread_cor_factor=1.   ! for  e+ or e-
      else
         spread_cor_factor=1.20 ! for all other tracks (obtained for pions)
      endif

C       CALL GRANOR(RAN1, RAN2)
C       CALL GRNDM(RNDM, 1)
        RNDM=RANDOM_MODULE_FLAT()
      if ( RNDM .LE. Fraction ) THEN
* Truly flat smearing
*        RNDM=RANDOM_MODULE_FLAT()
*        DRFTSM = (2.*RNDM-1.)*HalfDriftCell
* Gaussian#2 smearing (tails)
        Offset = Offset2*XMICRN
        Sigma  =  Sigma2*XMICRN*spread_cor_factor
        RAN2=RANDOM_MODULE_GAUSS()
        DRFTSM = Sigma*RAN2 + Offset + DRIFTIONS
      else
* Gaussian#1 smearing (core)
        Offset = Offset1*XMICRN
        Sigma  =  Sigma1*XMICRN*spread_cor_factor
        RAN1=RANDOM_MODULE_GAUSS()
        DRFTSM = Sigma*RAN1 + Offset + DRIFTIONS
      endif


#if defined(DEBUG_RESPONSE)
      chmess = ' ------- begin SMEAR_DRIFT debug info -------'
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'LAYER   =', LAYER
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'DRIFT   =', DRIFT
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'COSTH   =', COSTH
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'DRFRAC  =', DRFRAC
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'ibin    =', ibin
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Index   =', Index
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Sigma1  =', Sigma1
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Sigma2  =', Sigma2
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Offset1 =', Offset1
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Offset2 =', Offset2
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Fraction=', Fraction
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'DRFTSM =',DRFTSM
      call report(MS_INFO, crname, chmess)       
      chmess = ' ------- end   SMEAR_DRIFT debug info -------'
      call report(MS_INFO, crname, chmess)       
#endif

      return
      end
