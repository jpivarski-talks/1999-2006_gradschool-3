// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMSDedxOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Dec  2 13:04:50 EST 1999
// $Id: HIMSDedxOperation.cc,v 1.8 2002/02/27 00:07:14 wsun Exp $
//
// Revision history
//
// $Log: HIMSDedxOperation.cc,v $
// Revision 1.8  2002/02/27 00:07:14  wsun
// Fixed Runge-Kutta integration for mixtures.
//
// Revision 1.7  2002/02/21 07:17:48  wsun
// Fixed bug in dE/dx weighting for mixtures and compounds.
//
// Revision 1.6  2001/08/23 16:52:08  wsun
// Fixed for-loop index declaration for OSF.
//
// Revision 1.5  2001/08/23 03:12:55  wsun
// Fixed dE/dx calculation.
//
// Revision 1.4  2001/03/27 06:28:33  wsun
// Mods for reverse transport.
//
// Revision 1.3  2000/10/19 23:49:30  wsun
// Protect against beta >= 1.
//
// Revision 1.2  2000/08/16 20:00:58  wsun
// Updated to new TrackFitHitLink and TRTrackFitQuality.
//
// Revision 1.1  1999/12/15 21:06:20  wsun
// Merged functionality of HIDedxOperation and HIMultScatOperation.
//

#include "Experiment/Experiment.h"
#include "Experiment/cleo_math.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIMSDedxOperation.h"
#include "HelixIntersection/HIMultScatOperation.h"
#include "HelixIntersection/HIDedxOperation.h"

#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIMagField.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Geometry/Normal3D.h"

#include "KalmanFilter/KalmanHelix.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HelixIntersection.HIMSDedxOperation" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIMSDedxOperation.cc,v 1.8 2002/02/27 00:07:14 wsun Exp $";
static const char* const kTagString = "$Name: v12_01_01 $";

//
// static data member definitions
//

const double HIMSDedxOperation::m_ionizationPotentials[ 92 ] = {
   16., 29.8571, 43.006, 55.7152, 68.1072, 80.252, 92.1952, 103.968, 115.595, 127.093, 138.476, 149.756, 160.942, 172.042, 183.064, 194.012, 204.892, 215.707, 226.464, 237.163, 247.809, 258.405, 268.952, 279.454, 289.912, 300.328, 310.704, 321.042, 331.343, 341.609, 351.84, 362.039, 372.205, 382.341, 392.447, 402.524, 412.574, 422.596, 432.592, 442.562, 452.507, 462.428, 472.326, 482.2, 492.052, 501.882, 511.691, 521.479, 531.247, 540.995, 550.723, 560.432, 570.123, 579.795, 589.449, 599.086, 608.706, 618.308, 627.895, 637.465, 647.019, 656.557, 666.08, 675.588, 685.081, 694.56, 704.024, 713.474, 722.91, 732.332, 741.741, 751.137, 760.52, 769.89, 779.247, 788.592, 797.924, 807.245, 816.553, 825.85, 835.135, 844.408, 853.67, 862.922, 872.162, 881.391, 890.609, 899.817, 909.015, 918.202, 927.379, 936.546 } ;

const double HIMSDedxOperation::m_logIonizationPotentials[ 92 ] = {
   2.77259, 3.39642, 3.76134, 4.02025, 4.22108, 4.38517, 4.52391, 4.64409, 4.75009, 4.84492, 4.93069, 5.009, 5.08104, 5.14774, 5.20983, 5.26792, 5.32248, 5.37392, 5.42258, 5.46875, 5.51266, 5.55453, 5.59453, 5.63284, 5.66958, 5.70488, 5.73884, 5.77157, 5.80315, 5.83367, 5.86318, 5.89175, 5.91945, 5.94631, 5.9724, 5.99776, 6.02241, 6.04642, 6.06979, 6.09258, 6.1148, 6.13649, 6.15767, 6.17836, 6.19858, 6.21837, 6.23772, 6.25667, 6.27523, 6.29341, 6.31123, 6.32871, 6.34585, 6.36267, 6.37919, 6.39541, 6.41133, 6.42699, 6.44237, 6.4575, 6.47238, 6.48701, 6.50141, 6.51558, 6.52954, 6.54328, 6.55681, 6.57015, 6.58328, 6.59623, 6.609, 6.62159, 6.634, 6.64625, 6.65833, 6.67025, 6.68201, 6.69363, 6.70509, 6.71641, 6.72759, 6.73864, 6.74955, 6.76032, 6.77097, 6.7815, 6.79191, 6.80219, 6.81236, 6.82242, 6.83236, 6.8422 } ;

//
// constructors and destructor
//
HIMSDedxOperation::HIMSDedxOperation( DABoolean aModifyHelixParametersForMS,
				      DABoolean aApplyStragglingToTrack,
				      DABoolean aApplyStragglingToError,
				      double aStragglingCoeff,
				      DABoolean aApplyMS,
				      DABoolean aApplyDedx,
				      DABoolean aReverseTransport )
   : m_modifyHelixParametersForMS( aModifyHelixParametersForMS ),
     m_stragglingCoeff( aStragglingCoeff ),
     m_applyStragglingToTrack( aApplyStragglingToTrack ),
     m_applyStragglingToError( aApplyStragglingToError ),
     m_applyMS( aApplyMS ),
     m_applyDedx( aApplyDedx ),
     m_reverseTransport( aReverseTransport )
{
}

// HIMSDedxOperation::HIMSDedxOperation( const HIMSDedxOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIMSDedxOperation::~HIMSDedxOperation()
{
}

//
// assignment operators
//
// const HIMSDedxOperation& HIMSDedxOperation::operator=( const HIMSDedxOperation& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

DABoolean
HIMSDedxOperation::perform( HIHelix& aHelixAfterOperation,
			    const HIHelix& aHelixBeforeOperation,
			    const HIHelix& aHelixAtLastIntersection,
			    HIIntersectionSurface& aSurface,
			    const HIIntersectionSurface& aLastSurface ) const
{
   if( !m_applyMS && !m_applyDedx )
   {
      return true ;
   }

   // Determine whether to use the inner or outer material description.
   DGConstMaterialPtr material =
      aSurface.materialInLastTransport( aHelixAfterOperation ) ;

   return perform( aHelixAfterOperation,
		   aHelixBeforeOperation,
		   aHelixAtLastIntersection,
		   aSurface,
		   aLastSurface,
		   material ) ;
}

DABoolean
HIMSDedxOperation::perform( HIHelix& aHelixAfterOperation,
			    const HIHelix& aHelixBeforeOperation,
			    const HIHelix& aHelixAtLastIntersection,
			    HIIntersectionSurface& aSurface,
			    const HIIntersectionSurface& aLastSurface,
			    const DGConstMaterialPtr& aMaterial ) const
{
   if( !m_applyMS && !m_applyDedx )
   {
      return true ;
   }

   if( m_reverseTransport )
   {
      // To reverse energy loss and multiple scattering, the helix at the
      // LAST intersection should be corrected for the material in the
      // transport to the CURRENT surface, and then RETRANSPORTED to the
      // current surface.

      Meters lastArc = aHelixAfterOperation.lastArcLength() ;
      Meters totalArc = aHelixAfterOperation.totalArcLength() ;
      DABoolean lastAtSurface = aHelixAfterOperation.atSurface() ;
      aHelixAfterOperation.revertToSnapshot( &aHelixAtLastIntersection ) ;
      aHelixAfterOperation.setLastArcLength( lastArc ) ;
      aHelixAfterOperation.setTotalArcLength( totalArc ) ;
      aHelixAfterOperation.setAtSurface( lastAtSurface ) ;
   }
      

   // Arc length of last transport.
   Meters arcLength = aHelixAfterOperation.lastArcLength() ;

   // Some kinematic quantities needed for calculations.
   GeV mass = aHelixAfterOperation.mass() ;
   double massInv = 1. / mass ;
   GeV absmom2 = aHelixAfterOperation.momentumMag2() ;

   // Make absmom const so it is guaranteed to be the momentum BEFORE
   // energy loss.
   const GeV absmom = sqrt( absmom2 ) ;
   const GeV absmomInv = 1. / absmom ;

   const GeV energyOrig = sqrt( absmom2 + mass*mass ) ;
   double gamma = energyOrig * massInv ;
   const double betaInvAbsmomInv = energyOrig * absmomInv * absmomInv ;


   // ~~~~~~~~~~~~~~~~~~ Calculate energy loss ~~~~~~~~~~~~~~~~~~~~
   // If helix is being propagated backwards, the arc length is negative
   // --> positive dE.

   GeV dE = 0. ;

   if( m_applyDedx )
   {
      DABoolean dedxOK = true ;

      // First calculate sum of weights (is usually 1, but just to be safe...).

      DGMaterial::WeightType weightType = aMaterial->weightType() ;
      double sumOfWeights = 0. ;

      STL_VECTOR( DGMaterial::WeightedElement )::const_iterator iter =
	 aMaterial->elements().begin() ;

      for(  ; iter != aMaterial->elements().end() ; ++iter )
      {
 	 double weight    = ( *iter ).m_weight ;

	 if( weightType == DGMaterial::kNumberOfAtoms )
	 {
	    weight *= ( *iter ).m_element.m_atomicWeight ;
	 }

	 sumOfWeights += weight ;
      }

      // From /nfs/cern/current/src/geant321/gphys/gdrela.F:
      //
      // For chemical mixtures,compounds & molecules the approximation
      // is made that
      // 
      //         DE/DX = W(1)*DE/DX(1)+W(2)*DE/DX(2)...+W(N)*DE/DX(N)
      // with,
      //         DE/DX(i) appropriate to the i'th constituent.
      // 
      // For mixtures    W(i) = fractional wght of each element.
      // For molecules   W(i) = No. atoms*atomic wght/molecular wght.

      double massFrac = DBCandidate::mass( DBCandidate::kElectron ) * massInv ;

      // Perform a Runge-Kutta integration of dE/dx over the arc length.

      // Change in energy in GeV, arc length in m, density in g/cm^3.
      if( gamma < 1. )
      {
	 report( DEBUG, kFacilityString ) << "gamma < 1"
					  << endl ;
	 aHelixAfterOperation.setCurvature( 0. ) ;
	 return false ;
      }

      float kinE = energyOrig - mass ;
      GeV dE1 = arcLength * energyLossPerPathLength( aMaterial,
						     sumOfWeights,
						     kinE,
						     gamma,
						     massFrac ) ;
      double energy = energyOrig + 0.5 * dE1 ;
      gamma = energy * massInv ;
      if( gamma < 1. )
      {
	 dedxOK = false ;
      }
      else
      {
	 kinE = energy - mass ;
	 GeV dE2 = arcLength * energyLossPerPathLength( aMaterial,
							sumOfWeights,
							kinE,
							gamma,
							massFrac ) ;
	 energy = energyOrig + 0.5 * dE2 ;
	 gamma = energy * massInv ;
	 if( gamma < 1. )
	 {
	    dedxOK = false ;
	 }
	 else
	 {
	    kinE = energy - mass ;
	    GeV dE3 = arcLength * energyLossPerPathLength( aMaterial,
							   sumOfWeights,
							   kinE,
							   gamma,
							   massFrac ) ;
	    energy = energyOrig + dE3 ;
	    gamma = energy * massInv ;
	    if( gamma < 1. )
	    {
	       dedxOK = false ;
	    }
	    else
	    {
	       kinE = energy - mass ;
	       GeV dE4 = arcLength * energyLossPerPathLength( aMaterial,
							      sumOfWeights,
							      kinE,
							      gamma,
							      massFrac ) ;

	       // 2-20-02: No need to weight dE for this element because
	       // energyLossPerPathLength() already multiplies by the
	       // weighted density.
	       // dE += weight * ( dE1 + 2. * dE2 + 2. * dE3 + dE4 ) / 6. ;
	       dE = ( dE1 + 2. * dE2 + 2. * dE3 + dE4 ) / 6. ;
	       dedxOK = fabs( energyOrig + dE ) > mass ;
	    }
	 }
      }

      if( printDiagnostics() )
      {
	 cout << "mat " << aMaterial->name() << " " << aSurface.surfaceType()
	      << " radl "<< aMaterial->radiationLength()
	      << " arc " << aHelixAfterOperation.lastArcLength() << endl
	      << " den " << aMaterial->density() / 1000.
	      << " energy orig " << energyOrig << " dE " << dE << endl;
      }

      if( !dedxOK )
      {
	 report( DEBUG, kFacilityString ) << "Energy < mass after dE/dx."
					  << endl ;
	 aHelixAfterOperation.setCurvature( 0. ) ;
	 return false ;
      }
   }


   // ~~~~~~~~~~~~~~~~~ Apply dE/dx to track ~~~~~~~~~~~~~~~~~~

   double K = aHelixAfterOperation.curvature() ;
   double t = aHelixAfterOperation.cotTheta() ;
   double csc2Theta = 1. + t*t ;
   GeV newAbsmom = absmom ;
   double deltaCurv = 0. ;

   GeV newEnergyNoStraggling = energyOrig ;
   double newCurvNoStraggling = K ;
   double newCurvature = K ;
   GeV newAbsmomNoStraggling = absmom ;

   // Width^2 of the energy loss straggling = sqr( d(deltaE) )
   GeV ddE2 = sqr( m_stragglingCoeff ) * fabs( dE ) ;

   // Straggling.
   GeV ddE = 0. ;

   if( dE != 0. )
   {
      // Gaussian straggling to apply to the track.
      if( m_applyStragglingToTrack )
      {
	 ddE = RandGauss::shoot() * sqrt( ddE2 ) ;
      }

      // Check for rangeout.
      newEnergyNoStraggling += dE ;
      GeV newEnergy          = newEnergyNoStraggling + ddE ;

      if( newEnergy < mass )
      {
	 report( DEBUG, kFacilityString )
	    << "Energy with straggling < mass after dE/dx." << endl ;
	 aHelixAfterOperation.setCurvature( 0. ) ;
	 return false ;
      }

      // Calculate new curvature with and without straggling.
      newAbsmomNoStraggling = sqrt( sqr( absmom ) +
				    2. * energyOrig * dE +
				    sqr( dE ) ) ;
      newAbsmom             = sqrt( sqr( absmom ) +
				    2. * energyOrig * ( dE + ddE ) +
				    sqr( dE + ddE ) ) ;

      newCurvNoStraggling *= absmom / newAbsmomNoStraggling ;
      newCurvature        *= absmom / newAbsmom ;
   }

   double newBeta = newAbsmomNoStraggling / newEnergyNoStraggling ;

   // ~~~~~~~~~~~~~~~~~~ Calculate multiple scattering ~~~~~~~~~~~~~~~~~~~~

   double scatAngle2 = 0. ;

   double radLength  = aMaterial->radiationLength() ;
   double radLengths = fabs( arcLength / radLength ) ;
   double logRadLengths = 0. ;

   // If reverse transport, use beta and momentum AFTER energy loss.
   if( m_applyMS && radLengths > 0. &&
       ( !aHelixAfterOperation.hasNullErrorMatrix() ||
	 m_modifyHelixParametersForMS ) )
   {
      if( printDiagnostics() )
	 cout << "beta new/old " << newBeta
	      << "/" << 1./betaInvAbsmomInv/absmom
	      << " mom new/old " << newAbsmomNoStraggling << "/" << absmom
	      << endl;

      logRadLengths = log( radLengths ) ;

      if( m_reverseTransport )
      {
	 // from PDG
	 scatAngle2 = 1.8496e-4 * radLengths *
	    sqr( ( 1. + 0.038 * logRadLengths ) / newBeta /
		 newAbsmomNoStraggling ) ;
      }
      else
      {
	 // from PDG
	 scatAngle2 = 1.8496e-4 * radLengths *
	    sqr( ( 1. + 0.038 * logRadLengths ) * betaInvAbsmomInv ) ;
      }
   }

   if( scatAngle2 < 0. )
   {
      if( printDiagnostics() )
      {
	 report( DEBUG, kFacilityString )
	    << "Square of scattering angle width < 0." << endl ;
      }

      return false ;
   }

   // ~~~~~~~~~~~~~~~~~ Apply MS to track parameters ~~~~~~~~~~~~~~~~~~

   // Update phi0 and t.  Save K update for later.
   if( m_applyMS && m_modifyHelixParametersForMS && scatAngle2 > 0. )
   {
      // Apply multiple scattering to the two angles phi0 and theta.
      double scatteringAngle = sqrt( scatAngle2 ) ;

      double dphi0 = RandGauss::shoot() * scatteringAngle ;
      aHelixAfterOperation.setPhi0( aHelixAfterOperation.phi0() +
				    dphi0 * sqrt( csc2Theta ) ) ;

      // Wiggle both cotTheta and curvature to conserve momentum.
      double dtheta = RandGauss::shoot() * scatteringAngle ;
      aHelixAfterOperation.setCotTheta( t - dtheta * csc2Theta ) ;
      deltaCurv = -K * t * dtheta ;
   }

   if( printDiagnostics() )
   {
      cout << " ddE " << sqrt( ddE2 )
	   << " MS " << ( scatAngle2 > 0. ? sqrt( scatAngle2 ) : -99. )
	   << endl ;
   }

   // Update the track's curvature.
   aHelixAfterOperation.setCurvature( newCurvature + deltaCurv ) ;

   // Update energy lost by track.
   aHelixAfterOperation.incrementDeltaEnergy( dE + ddE ) ;

   // ~~~~~~~~~~~~~~ Update error matrix (see CBX 96-20) ~~~~~~~~~~~~~~~

   if( !aHelixAfterOperation.hasNullErrorMatrix() &&
       ( ( m_applyDedx && dE != 0. ) || ( m_applyMS && scatAngle2 != 0. ) ) &&
       aHelixAfterOperation.updateErrorMatrix() )
   {
      // First make a non-const copy of the error matrix.
      const HepSymMatrix& cErrorMatrix = aHelixAfterOperation.errorMatrix() ;
      HepSymMatrix errorMatrix = cErrorMatrix ;

      if( printDiagnostics() )
      {
	 cout << "error before " << errorMatrix ;
	 aHelixAfterOperation.errorMatrixOK() ;

	 if( aHelixAfterOperation.otherErrorMatrix() != 0 )
	 {
	    cout << "other error before"
		 << *aHelixAfterOperation.otherErrorMatrix() ;
	    aHelixAfterOperation.otherErrorMatrixOK() ;
	 }
      }

      HepVector dKnew_dParams( HIHelix::kNTrackParameters, 0 ) ;

      aSurface.arcLengthDerivatives(
	 aHelixBeforeOperation,
	 aHelixAtLastIntersection,
	 aLastSurface,
	 dKnew_dParams ) ;

      if( printDiagnostics() )
      {
	 cout << "arc length derivs" ;
	 for( int i = 1 ; i <= HIHelix::kNTrackParameters ; ++i )
	 {
	    cout << " " << dKnew_dParams( i ) ;
	 }
	 cout << endl ;
      }

      HepVector transportVector( HIHelix::kNTrackParameters, 0 ) ;

      // ~~~~~~ dEdx ~~~~~~

      if( m_applyDedx )
      {
	 double curvScale ;
	 double energyScale ;

	 // Don't include straggling in new energy and momentum
	 // for error matrix update.
	 if( aHelixAfterOperation.savedCurvature() == 0. )
	 {
	    curvScale   = newCurvNoStraggling / K ;
	    energyScale = newEnergyNoStraggling / energyOrig ;
	 }
	 else
	 {
	    if( printDiagnostics() )
	    {
	       cout << "err matrix dE "
		    << newEnergyNoStraggling-aHelixAfterOperation.savedEnergy()
		    << endl ;
	    }

	    curvScale =
	       newCurvNoStraggling / aHelixAfterOperation.savedCurvature() ;
	    energyScale =
	       newEnergyNoStraggling / aHelixAfterOperation.savedEnergy() ;
	    aHelixAfterOperation.unsaveCurvatureAndEnergy() ;
	 }

	 // Calculate partial derivatives of Knew w.r.t. track parameters.
	 double curvScale2 = curvScale * curvScale ;
	 double a = curvScale * curvScale2 * energyScale ;

	 double newSigma2CotTheta =
	    errorMatrix( HIHelix::kCotTheta, HIHelix::kCotTheta ) ;
	 double newSigma2Z0 = errorMatrix( HIHelix::kZ0, HIHelix::kZ0 ) ;
	 double t = aHelixBeforeOperation.cotTheta() ;

	 double b = t / ( 1. + sqr( t ) ) * newCurvNoStraggling *
	    ( 1. - curvScale2 * energyScale ) ;

	 // Calculate derivatives of deltaE w.r.t. track parameters.
	 double dEdx = dE / aHelixAfterOperation.lastArcLength() ;
	 double dKnewdDe =
	    -newCurvNoStraggling * newEnergyNoStraggling / sqr( newAbsmom ) ;

	 // Construct the transport matrix.
	 // Don't apply arc length derivatives to energy loss because the
	 // Jacobian should only include partial derivatives.
	 // (Otherwise, non-reversible)
// 	 if( !m_reverseTransport )
// 	 {
// 	    transportVector = dKnew_dParams ;
// 	    transportVector *= dKnewdDe * dEdx ;
// 	 }
	 transportVector( HIHelix::kCurvature ) += a ;
	 transportVector( HIHelix::kCotTheta )  += b ;
      }

      // ~~~~~~ MS ~~~~~~

      double angle2PlusDAngle2 = scatAngle2 ;
      double dAngle_dRadLengths = 0. ;
      double dAngle2 = 0. ;

      if( scatAngle2 != 0. )
      {
	 // Add uncertainty due to surface shape and track parameter
	 // variation to the scattering angle.
	 double dAngle_dRadLengths =
	    0.5 * 1.36e-2 * betaInvAbsmomInv / sqrt( radLengths ) *
	    ( 1.076 + 0.038 * logRadLengths ) ;

	 for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
	 {
	    double derivi = dKnew_dParams( i ) ;
	    dAngle2 += sqr( derivi ) * errorMatrix.fast( i, i ) ;

	    for( int j = i + 1 ; j <= HIHelix::kZ0 ; ++j )
	    {
	       dAngle2 += 2. * derivi * dKnew_dParams( j ) *
		  errorMatrix.fast( j, i ) ;
	    }
	 }
	 dAngle2 *= sqr( dAngle_dRadLengths / radLength ) ;

	 // For very small arc lengths, dAngle2 can be very large because of
	 // 1. / sqrt( radLengths ).
	 if( dAngle2 < scatAngle2 )
	 {
	    angle2PlusDAngle2 += dAngle2 ;
	 }

	 if( printDiagnostics() )
	 {
	    cout << " scatAngle2 = " << scatAngle2 << " + " << dAngle2 << endl;
	 }

      }

      // ~~~~~~~~~~~~ Transport the error matrix ~~~~~~~~~~~~~~~

      double msErrorSign = m_reverseTransport ?  -1. : 1. ;

      double dKdDE2 = 0. ;
      if( m_applyDedx )
      {
	 // errorMatrix = transportMatrix * errorMatrix * transportMatrix.T().
	 // Use the fact that the transport matrix is the identity except for
	 // the kCurvature row to optimize the matrix multiplication.

	 // transportDotError = errorMatrix * transportVector
	 HepVector transportDotError( HIHelix::kNTrackParameters, 0 ) ;
	 for( int m = 1 ; m <= HIHelix::kNTrackParameters ; ++m )
	 {
	    for( int n = 1 ; n <= HIHelix::kNTrackParameters ; ++n )
	    {
	       transportDotError( m ) +=
		  errorMatrix( m, n ) * transportVector( n ) ;
	    }
	 }

	 double newKK = 0. ;
	 for( int j = 1 ; j <= HIHelix::kNTrackParameters ; ++j )
	 {
	    newKK += transportDotError( j ) * transportVector( j ) ;

	    if( j != HIHelix::kCurvature )
	    {
	       errorMatrix.fast( j, HIHelix::kCurvature ) =
		  transportDotError( j );
	    }
	 }
	 errorMatrix.fast( HIHelix::kCurvature, HIHelix::kCurvature ) = newKK ;


	 // Add contribution from straggling.
	 dKdDE2 = sqr( K * energyOrig * absmomInv * absmomInv ) ;

	 if( m_applyStragglingToError )
	 {
	    errorMatrix( HIHelix::kCurvature, HIHelix::kCurvature ) +=
	       msErrorSign * dKdDE2 * ddE2 ;
	 }
      }

      if( m_applyMS )
      {
	 // Add contribution from MS.
	 errorMatrix( HIHelix::kCurvature, HIHelix::kCurvature ) +=
	    msErrorSign * sqr( K * t ) * angle2PlusDAngle2 ;
	 errorMatrix( HIHelix::kCurvature, HIHelix::kCotTheta ) +=
	    msErrorSign * K * t * csc2Theta * angle2PlusDAngle2 ;
	 errorMatrix( HIHelix::kCotTheta,  HIHelix::kCotTheta ) +=
	    msErrorSign * sqr( csc2Theta ) * angle2PlusDAngle2 ;
	 errorMatrix( HIHelix::kPhi0, HIHelix::kPhi0 ) +=
	    msErrorSign * csc2Theta * angle2PlusDAngle2 ;
      }

      // Assign the transported error to the helix.
      aHelixAfterOperation.setErrorMatrix( errorMatrix ) ;


      // ~~~ Transport the other error matrix associated with this helix ~~~

      HepSymMatrix* otherErr = aHelixAfterOperation.otherErrorMatrix() ;
      if( otherErr != 0 )
      {
	 // Add uncertainty due to surface shape and track parameter
	 // variation to the scattering angle.
	 double angle2PlusDAngle2 = scatAngle2 ;

	 if( m_applyMS )
	 {
	    double dAngle2 = 0. ;
	    for( int i = 1 ; i <= HIHelix::kNTrackParameters ; ++i )
	    {
	       double derivi = dKnew_dParams( i ) ;
	       dAngle2 += sqr( derivi ) * errorMatrix.fast( i, i ) ;

	       for( int j = i + 1 ; j <= HIHelix::kNTrackParameters ; ++j )
	       {
		  dAngle2 +=
		     2. * derivi * dKnew_dParams( j ) * otherErr->fast( j, i );
	       }
	    }
	    dAngle2 *= sqr( dAngle_dRadLengths / radLength ) ;

	    if( dAngle2 < scatAngle2 )
	    {
	       angle2PlusDAngle2 += dAngle2 ;
	    }
	 }

	 if( m_applyDedx )
	 {
	    // transportDotError = ( *otherErr ) * transportVector
	    HepVector transportDotError( HIHelix::kNTrackParameters, 0 ) ;
	    for( int p = 1 ; p <= HIHelix::kNTrackParameters ; ++p )
	    {
	       for( int q = 1 ; q <= HIHelix::kNTrackParameters ; ++q )
	       {
		  transportDotError( p ) +=
		     ( *otherErr )( p, q ) * transportVector( q ) ;
	       }
	    }

	    double newKK = 0. ;
	    for( int m = 1 ; m <= HIHelix::kNTrackParameters ; ++m )
	    {
	       newKK += transportDotError( m ) * transportVector( m ) ;

	       if( m != HIHelix::kCurvature )
	       {
		  otherErr->fast( m, HIHelix::kCurvature ) =
		     transportDotError( m ) ;
	       }
	    }
	    otherErr->fast( HIHelix::kCurvature, HIHelix::kCurvature ) = newKK;

	    if( m_applyStragglingToError )
	    {
	       otherErr->fast( HIHelix::kCurvature, HIHelix::kCurvature ) +=
		  msErrorSign * dKdDE2 * ddE2 ;
	    }
	 }

	 if( m_applyMS )
	 {
	    otherErr->fast( HIHelix::kCurvature, HIHelix::kCurvature ) +=
	       msErrorSign * sqr( K * t ) * angle2PlusDAngle2 ;
	    otherErr->fast( HIHelix::kCotTheta, HIHelix::kCurvature ) +=
	       msErrorSign * K * t * csc2Theta * angle2PlusDAngle2 ;
	    otherErr->fast( HIHelix::kCotTheta,  HIHelix::kCotTheta ) +=
	       msErrorSign * sqr( csc2Theta ) * angle2PlusDAngle2 ;
	    otherErr->fast( HIHelix::kPhi0, HIHelix::kPhi0 ) +=
	       msErrorSign * csc2Theta * angle2PlusDAngle2 ;
	 }
      }

      // Transport the track parameter corrections.  Only the
      // curvatures need to be updated.
      if( m_applyDedx )
      {
	 aHelixAfterOperation.transportCorrectionsDedx( transportVector ) ;

	 if( printDiagnostics() )
	 {
	    cout << "KK "
		 << transportVector( HIHelix::kCurvature )
		 << " Kphi "
		 << transportVector( HIHelix::kPhi0 )
		 << " Kd "
		 << transportVector( HIHelix::kD0 )
		 << " Kt "
		 << transportVector( HIHelix::kCotTheta )
		 << " Kz "
		 << transportVector( HIHelix::kZ0 )
		 << endl ;
	 }
      }

      if( m_reverseTransport )
      {
	 if( printDiagnostics() )
	 {
	    cout << "error before retransport"
		 << aHelixAfterOperation.errorMatrix() << endl;
	 }

	 // Helix is now at last surface.  Retransport to current surface.
	 aSurface.revertToPreviousState() ;
	 aSurface.propagateHelix(
	    aHelixAfterOperation,
	    ( aHelixAfterOperation.lastArcLength() > 0. ?
	      KTMoveControl::kDirectionForward :
	      KTMoveControl::kDirectionBackward ) ) ;
      }

      if( printDiagnostics() )
      {
	 cout << "last arc " << aHelixAfterOperation.lastArcLength() << endl ;
	 cout << "error after " << aHelixAfterOperation.errorMatrix() << endl ;
	 aHelixAfterOperation.errorMatrixOK() ;

	 if( aHelixAfterOperation.otherErrorMatrix() != 0 )
	 {
	    cout << "other error after"
		 << *aHelixAfterOperation.otherErrorMatrix() ;
	    aHelixAfterOperation.otherErrorMatrixOK() ;
	 }

	 if( aHelixAfterOperation.helixType() ==
	     "KalmanHelix" )
	    cout << "transport matrix "
		 << ((KalmanHelix*)(&aHelixAfterOperation))->lastTransportMatrix() << endl;
	 cout
	    << aHelixAtLastIntersection.curvature() << " "
	    << aHelixAtLastIntersection.phi0() << " "
	    << aHelixAtLastIntersection.d0() << " "
	    << aHelixAtLastIntersection.cotTheta() << " "
	    << aHelixAtLastIntersection.z0() << " "
	    << aHelixAtLastIntersection.referencePoint()
	    << endl ;
	 cout
	    << aHelixAfterOperation.curvature() << " "
	    << aHelixAfterOperation.phi0() << " "
	    << aHelixAfterOperation.d0() << " "
	    << aHelixAfterOperation.cotTheta() << " "
	    << aHelixAfterOperation.z0() << " "
	    << aHelixAfterOperation.referencePoint()
	    << endl ;
      }
   }

   return true ;
}

//
// static member functions
//

// GeV/meter
GeV
HIMSDedxOperation::energyLossPerPathLength(
   const DGConstMaterialPtr& aMaterial,
   double sumOfWeights,
   GeV kinE,
   double gamma,
   double massFrac ) const
{
   GeV dE = 0. ;

   DGMaterial::WeightType weightType = aMaterial->weightType() ;

   STL_VECTOR( DGMaterial::WeightedElement )::const_iterator iter =
      aMaterial->elements().begin() ;
   for( ; iter != aMaterial->elements().end() ; ++iter )
   {
      DGMaterial::Element element = ( *iter ).m_element ;
      double weight    = ( *iter ).m_weight ;

      if( weightType == DGMaterial::kNumberOfAtoms )
      {
	 weight *= ( *iter ).m_element.m_atomicWeight ;
      }

      weight /= sumOfWeights ;

      // Density in g/cm^3
      double density = aMaterial->density()/1000. * weight ;

      double rhoZOverA =
	 density * element.m_atomicNumber / element.m_atomicWeight ;

      double I = 1e-16 ;
      double logI = 1e-16 ;
      if( element.m_atomicNumber >= 1. && element.m_atomicNumber < 93. )
      {
	 I =
	    m_ionizationPotentials[ int( element.m_atomicNumber ) - 1 ] ;
	 logI =
	    m_logIonizationPotentials[ int( element.m_atomicNumber ) - 1 ] ;
      }

      // plasmaE = 28.816 * sqrt( rhoZOverA ) ; // eV for rho in g/cm^3
      // C = 2. * log( I/plasmaE ) + 1. ;
      double C = 2. * ( logI - 3.3609308 - 0.5 * log( rhoZOverA ) ) + 1. ;

      double x0, x1 ;

      if( density > 0.05 )  // condensed material
      {
	 if( I < 100. )
	 {
	    x0 = C < 3.681 ? 0.2 : 0.326 * C - 1. ;
	    x1 = 2. ;
	 }
	 else
	 {
	    x0 = C < 5.215 ? 0.2 : 0.326 * C - 1.5 ;
	    x1 = 3. ;
	 }
      }
      else  // gas
      {
	 if( C <= 12.25 )
	 {
	    int ip = int( ( C - 10. ) / 0.5 ) + 1 ;
	    if( ip < 0 )
	    {
	       ip = 0 ;
	    }
	    else if( ip > 4 )
	    {
	       ip = 4 ;
	    }

	    x0 = 1.6 + 0.1 * ( double ) ip ;
	    x1 = 4. ;
	 }
	 else
	 {
	    x0 = C <= 13.804 ? 2. : 0.326 * C - 2.5 ;
	    x1 = 5. ;
	 }
      }

      double x2 = x1 - x0 ;
      double a = ( C - 4.606 * x0 ) / ( x2 * x2 * x2 ) ;

      dE += energyLossPerPathLength( element.m_atomicNumber,
				     rhoZOverA,
				     I,
				     logI,
				     C,
				     x0,
				     x1,
				     a,
				     gamma,
				     kinE,
				     massFrac ) ;
   }

   return dE ;
}


// GeV/meter
// particleMassFrac = electron mass / particle mass

GeV
HIMSDedxOperation::energyLossPerPathLength( const double& atomicNumber,
					    const double& rhoZOverA,
					    const double& I,
					    const double& logI,
					    const double& C,
					    const double& x0,
					    const double& x1,
					    const double& a,
					    const double& gamma,
					    const double& kineticEnergy,
					    const GeV& particleMassFrac )
{
   double gamma2 = gamma * gamma ;
   double b2g2 = gamma2 - 1. ;
   double beta2 = b2g2 / gamma2 ;
   double logB2g2 = log( b2g2 ) ;
   double b2g2Inv = 1. / b2g2 ;

   double x = 0.2171081 * logB2g2 ;

   double delta = 0. ;
   if( x > x0 )
   {
      delta = 4.606 * x - C ;

      if( x < x1 )
      {
	 double tmp = x1 - x ;
	 delta += a * tmp * tmp * tmp ;
      }
   }

   double ce ;
   if( b2g2 > 0.0169 )
   {
      double IGeV = I * 1.e-3 ;

      ce = IGeV * IGeV * ( ( 0.42237 + 3.858 * IGeV + (
	 0.304 - 0.1668 * IGeV + (
	    -0.00038 + 0.00158 * IGeV ) * b2g2Inv ) * b2g2Inv ) * b2g2Inv ) ;
   }
   else
   {
      double b2g2Tmp = 0.0169 ;
      double b4g4 = 0.0002856 ;
      double b6g6 = 4.8268e-6 ;

      ce = I*I*( ( 0.42237/b2g2Tmp + 0.304/b4g4 - 0.00038/b6g6 ) * 1.e-6 +
		 ( 3.858/b2g2Tmp - 0.1668/b4g4 + 0.00158/b6g6 ) * I * 1.e-9 ) ;

      double t2lim = 0.002 / 0.9382723 *
	 DBCandidate::mass( DBCandidate::kElectron ) / particleMassFrac ;
	 
      ce *= log( kineticEnergy / t2lim ) / log( 0.0079 / t2lim ) ;
   }

   // tmax in GeV = 2. * DBCandidate::mass( DBCandidate::kElectron ) * b2g2 /
   //   ( 1. + 2*gamma*particleMassFrac + particleMassFrac*particleMassFrac )

   // Use approximations for log( 1 + x ) that give less than 1% error
   //    x                 for x < 0.02
   //    x - x^2/2         for x < 0.18
   //    x - x^2/2 + x^3/3 for x < 0.35

   double small = ( 2.*gamma + particleMassFrac ) * particleMassFrac ;
   double logOnePlusSmall ;
   if( small < 0.02 )
   {
      logOnePlusSmall = small ;
   }
   else if( small < 0.18 )
   {
      logOnePlusSmall = small * ( 1. - 0.5 * small ) ;
   }
   else if( small < 0.35 )
   {
      logOnePlusSmall = small * ( 1. - small * ( 0.5 - small / 3. ) ) ;
   }
   else
   {
      logOnePlusSmall = log( 1. + small ) ;
   }

   // dE/dx = -rhoZOverA * 0.0307075 / beta2 *
   //         ( 0.5 * log( 2. * DBCandidate::mass( DBCandidate::kElectron ) *
   //         b2g2 * tmax / ( I*I * 1.e-18 ) ) -
   // 	      beta2 - delta/2. - ce/atomicNumber ) ;

   // 0.5 * log( 1e-18 *
   //    sqr( 2. * DBCandidate::mass( DBCandidate::kElectron ) ) ) = 13.837272

   return -rhoZOverA * 0.0307075 * b2g2Inv * gamma2 *
      ( 13.837272 + logB2g2 - logI -
	0.5 * ( logOnePlusSmall - delta ) - beta2 - ce/atomicNumber ) ;
}
