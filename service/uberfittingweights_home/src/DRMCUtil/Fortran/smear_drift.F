C 
C  $Id: smear_drift.F,v 1.11 2003/08/14 18:18:04 dubrovin Exp $
C 
C  $Log: smear_drift.F,v $
C  Revision 1.11  2003/08/14 18:18:04  dubrovin
C  Add cosTheta dependence of resolution
C
C  Revision 1.10  2001/12/18 20:30:46  dubrovin
C  Add dependence of smearing on part.ID. Flat distribution is replaced for 2-nd Gaussian
C
C  Revision 1.9  2001/06/12 15:00:28  dubrovin
C  New generators of random numbers
C
C  Revision 1.8  2001/05/12 23:54:18  dubrovin
C  Add input parameter DRIFTIONS
C
C  Revision 1.7  2001/01/10 16:48:14  dubrovin
C  Significant modification in order to get constants with correct address
C
C  Revision 1.6  1999/12/13 01:49:56  jjo
C  Use report for debug messages
C
C  Revision 1.5  1999/11/22 23:04:21  jjo
C  Improve formatting
C
C  Revision 1.4  1999/11/22 23:03:48  jjo
C  Add debugging printout if CPP flag DEBUG_RESPONSE is defined
C
C  Revision 1.3  1999/10/26 18:43:41  jjo
C  Use Cleo3 geometry common blocks
C
C  Revision 1.2  1999/10/20 21:41:29  jjo
C  Get DR MC Response resolution constants installed
C
C  Revision 1.1  1999/09/20 22:48:12  jjo
C  Import files into cvs
C
C 
      SUBROUTINE SMEAR_DRIFT(LAYER, DRIFT, DRIFTIONS, COSTH, 
     &                       ChargeDep, IDHit, DRFTSM)
C 
C  begin_doc
C 
C   Documentation for subroutine smear_drift
C 
C   Purpose: Smears the distance DRIFT by a function described by two gaussians.
C   -------- Based on CleoII.5 routine cleog/crawm/cdsmer.F
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
C   Reads the DRMCResolutions constants
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
C   Author:   John J O'Neill      Created:  Wed Sep  1 14:51:13 EDT 1999
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
      INTEGER ibin, ibin_unlim

      real HalfDriftCell 
      real spread_cor_factor

C     Index - index into constants for this drift distance, layer
      INTEGER MAXBIN, NBITEM, Index
      PARAMETER ( NBITEM =  5 )
      PARAMETER ( MAXBIN = 20 )

C     DRMCResolutions constants - drift distance resolutions
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

C     kNOUPDATE - Do not update constants in call to DRMCUtil/Fortran/get_resolutions
      logical kNOUPDATE
      parameter ( kNOUPDATE = .false. )

c   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: smear_drift.F,v $')
      parameter(chrcsr='$Revision: 1.11 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/08/14 18:18:04 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'smear_drift' )
 
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
        chmess='Smear drift dist. by 2Gauss. Fuge factors = 1.8, 0.8'
        call report(MS_INFO, crname, chmess)
      endif

C     Initialize DRFTSM - smeared drift distance with the actual drift distance
      DRFTSM = DRIFTIONS


c     Find the half length of the drift cell for this layer
c     where  CELLCD(LAYER) is a Width of cell in PHI (radians)
      HalfDriftCell = 0.5*RCD(LAYER)*CELLCD(LAYER)

C     Find the bin corresponding to the drift distance.
      DRFRAC = DRIFT / HalfDriftCell
      ibin_unlim = INT((DRFRAC + 1.)*MAXBIN*0.5)+1
      ibin   = MIN(ibin_unlim, MAXBIN)
      ibin   = MAX(ibin, 1)


C     Determine the smearing variables 
C     Each item is a two dim array of MAXBIN columns and KLYRCD rows.
      Index  = IBIN + (LAYER-1)*MAXBIN

C     Get drift function resoltion (DRMCResolutions) constants
      call get_resolutions( kNOUPDATE,
     &                      Index,
     &                      Sigma1, Sigma2,
     &                      Offset1, Offset2,
     &                      Fraction )
      
C      if( IDHit.eq.2 .or. IDHit.eq.3 ) then 
C         spread_cor_factor=1.   ! for  e+ or e-
C      else
C         spread_cor_factor=1.20 ! for all other tracks (obtained for pions)
C      endif

C This stuff might be interesting later:
C Linear extrapolation of resolution outside the drift cell transv. dim.
C 9.5-bins from 0 to the 1-st last bin; 1.5 is a fenomenological factor 
C taking into account faster slope than linear extrapolation from 0.
C       if(ibin_unlim .gt. MAXBIN .or. ibin_unlim .lt. 1) then 
C       spread_cor_factor=1.0 + 1.5*float( abs(ibin_unlim-ibin) )/9.5
C       endif

C Account for ion statistics. 
C Presumably: N(ions)~L~d/sinT -> fluct ~ 1/sqrt(N)
C 1.5 is a fudge factor, because tuning was done for e+e- events (small angles)
C       SINTH=sqrt(1.-COSTH**2)
C       spread_cor_factor=1.5*SQRT(SINTH)
C SQRT(SINTH) does not work weel. Simple parabola looks better:

       spread_cor_factor=1.80*(1.-0.80*COSTH**2)

C       CALL GRANOR(RAN1, RAN2)
C       CALL GRNDM(RNDM, 1)
        RNDM=RANDOM_MODULE_FLAT()
      if ( RNDM .LE. Fraction ) THEN
* Truly flat smearing
*        RNDM=RANDOM_MODULE_FLAT()
*        DRFTSM = (2.*RNDM-1.)*HalfDriftCell
* Gaussian#2 smearing (tails)
        Offset = Offset2*XMICRN
        Sigma  =  Sigma2*XMICRN
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
