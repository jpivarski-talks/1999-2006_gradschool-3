// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIDedxOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Sep 28 11:26:14 EDT 1998
// $Id: HIDedxOperation.cc,v 1.18 1999/12/15 21:05:30 wsun Exp $
//
// Revision history
//
// $Log: HIDedxOperation.cc,v $
// Revision 1.18  1999/12/15 21:05:30  wsun
// Sped up matrix element access.
//
// Revision 1.17  1999/11/21 22:31:36  wsun
// Sped up calculations.
//
// Revision 1.16  1999/11/03 03:38:04  wsun
// * Downgraded WARNING output to DEBUG.
// * Check if new energy *with straggling* is less than mass.
//
// Revision 1.15  1999/09/16 19:38:19  wsun
// Added error flag as argument to deltaEnergy().
//
// Revision 1.14  1999/09/10 21:24:37  wsun
// Added Runge-Kutta integration of dE/dx over arc length.
//
// Revision 1.13  1999/08/30 21:37:25  wsun
// Added energy loss straggling.
//
// Revision 1.12  1999/08/27 20:14:23  wsun
// Added arc length derivatives to error matrix inflation.
//
// Revision 1.11  1999/08/03 21:59:17  wsun
// Added transport of track parameter corrections.
//
// Revision 1.10  1999/07/17 18:47:26  wsun
// Removed 'error' from warning messsages -- could prevent rotations.
//
// Revision 1.9  1999/07/12 06:45:50  wsun
// Removed extranous printout.
//
// Revision 1.8  1999/07/12 06:40:04  wsun
// Cull surfaces for possible intersection using track's projected phi position
//
// Revision 1.7  1999/07/07 16:41:08  wsun
// Added mechanism for applying constraints in track fitters.
//
// Revision 1.6  1999/06/24 22:24:56  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.5  1999/06/18 19:25:56  wsun
// More improvements to low momentum fitting.
//
// Revision 1.4  1999/06/02 23:05:21  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/05/03 19:29:14  wsun
// Fixed error matrix update.
//
// Revision 1.2  1999/04/27 22:06:24  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:25  wsun
// First submission.
//

#include "Experiment/Experiment.h"
#include "Experiment/cleo_math.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIDedxOperation.h"
#include "HelixIntersection/HIMagField.h"

// TEMPORARY
#include "HelixIntersection/HISiSensorPlane.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Random/RandGauss.h"

//#include "KalmanFilter/KalmanHelix.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HIDedxOperation" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIDedxOperation::HIDedxOperation( DABoolean aApplyStragglingToTrack,
				  DABoolean aApplyStragglingToError,
				  double aStragglingCoeff )
   : m_stragglingCoeff( aStragglingCoeff ),
     m_applyStragglingToTrack( aApplyStragglingToTrack ),
     m_applyStragglingToError( aApplyStragglingToError )
{
}

// HIDedxOperation::HIDedxOperation( const HIDedxOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIDedxOperation::~HIDedxOperation()
{
}

//
// assignment operators
//
// const HIDedxOperation& HIDedxOperation::operator=( const HIDedxOperation& rhs )
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
HIDedxOperation::perform( HIHelix& aHelixAfterOperation,
			  const HIHelix& aHelixBeforeOperation,
			  const HIHelix& aHelixAtLastIntersection,
			  HIIntersectionSurface& aSurface,
			  const HIIntersectionSurface& aLastSurface ) const
{
   // Determine whether to use the inner or outer material description.
   DGConstMaterialPtr material =
      aSurface.materialInLastTransport( aHelixAfterOperation ) ;
   DABoolean dedxOK ;
   GeV dE = deltaEnergy( material, aHelixAfterOperation, dedxOK ) ;

   if( !dedxOK )
   {
      report( DEBUG, kFacilityString ) << "Energy < mass after dE/dx."
					 << endl ;
      aHelixAfterOperation.setCurvature( 0. ) ;
      return false ;
   }

   // Width^2 of the energy loss straggling = sqr( d(deltaE) )
   GeV ddE2 = sqr( m_stragglingCoeff ) * fabs( dE ) ;

   // Gaussian straggling to apply to the track.
   GeV ddE = 0. ;
   if( m_applyStragglingToTrack )
   {
      ddE = RandGauss::shoot() * sqrt( ddE2 ) ;
   }

   // Check for rangeout.
   GeV mass = aHelixAfterOperation.mass() ;
   GeV absmom = aHelixAfterOperation.momentumMag2() ;
   GeV energy = sqrt( absmom + sqr( mass ) ) ;
   absmom = sqrt( absmom ) ;

   GeV newEnergyNoStraggling = energy + dE ;
   GeV newEnergy             = energy + dE + ddE ;

   if( newEnergy < mass )
   {
      report( DEBUG, kFacilityString )
	 << "Energy with straggling < mass after dE/dx." << endl ;
      aHelixAfterOperation.setCurvature( 0. ) ;
      return false ;
   }

   // Calculate new curvature with and without straggling.
   double origCurvature = aHelixBeforeOperation.curvature() ;
   double diffCurvature = aHelixAfterOperation.curvature() - origCurvature ;

   GeV newAbsmomNoStraggling  = sqrt( sqr( absmom ) +
				      2. * energy * dE +
				      sqr( dE ) ) ;
   GeV newAbsmom              = sqrt( sqr( absmom ) +
				      2. * energy * ( dE + ddE ) +
				      sqr( dE + ddE ) ) ;

   double newCurvNoStraggling = origCurvature * absmom / newAbsmomNoStraggling;
   double newCurvature        = origCurvature * absmom / newAbsmom ;

   // Update the track's curvature.
   aHelixAfterOperation.setCurvature( newCurvature + diffCurvature ) ;

   if( printDiagnostics() )
   {
      // TEMPORARY
      if( aSurface.surfaceType() == HISiSensorPlane::kSurfaceType ||
          aSurface.surfaceType() == HISiLadderPlane::kSurfaceType ||
          aSurface.surfaceType() == HIIntersectionPlane::kSurfaceType )
         cout << "Surface point "
              << ( ( HIIntersectionPlane* ) &aSurface )->point()
              << " normal "
              << ( ( HIIntersectionPlane* ) &aSurface )->normal() << endl
	      << " track x/p " << aHelixAfterOperation.position()
	      << " " << aHelixAfterOperation.momentum() << endl ;
      cout << "mat " << material->name()
	   << " den " << material->density() / 1000.
	   << " arc "  << aHelixAfterOperation.lastArcLength()
	   << " dE " << dE << " ddE " << sqrt( ddE2 ) << endl ;
   }


   // Update error matrix (see CBX 96-20).
   if( !aHelixAfterOperation.hasNullErrorMatrix() && dE != 0. )
   {
      if( aHelixAfterOperation.updateErrorMatrix() )
      {
	 double curvScale ;
	 double energyScale ;

	 // Don't include stragging in new energy and momentum
	 // for error matrix update.
	 if( aHelixAfterOperation.savedCurvature() == 0. )
	 {
	    curvScale   = newCurvNoStraggling / origCurvature ;
	    energyScale = newEnergyNoStraggling / energy ;
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

	 const HepSymMatrix& cErrorMatrix = aHelixAfterOperation.errorMatrix();
	 HepSymMatrix errorMatrix = cErrorMatrix ;
	 HepSymMatrix origMatrix = aHelixBeforeOperation.errorMatrix() ;
	 HepSymMatrix diffMatrix = errorMatrix - origMatrix ;

	 if( printDiagnostics() )
	 {
	    cout << "error before " << origMatrix ;
	 }

	 // Calculate partial derivatives of Knew w.r.t. track parameters.
	 double curvScale2 = curvScale * curvScale ;
 	 double a = curvScale * curvScale2 * energyScale ;
	 double b = 0. ;

	 double newSigma2CotTheta =
	    errorMatrix( HIHelix::kCotTheta, HIHelix::kCotTheta ) ;
	 double newSigma2Z0 = errorMatrix( HIHelix::kZ0, HIHelix::kZ0 ) ;
 	 double t = aHelixBeforeOperation.cotTheta() ;

// 	 if( newSigma2CotTheta < 0.01 * sqr( sqr( t ) ) &&
// 	     newSigma2Z0 < 1.e-4 )
	 {
	    b = t / ( 1. + sqr( t ) ) * newCurvNoStraggling *
	       ( 1. - curvScale2 * energyScale ) ;
	 }

	 // Calculate derivatives of deltaE w.r.t. track parameters.
	 double dEdx = dE / aHelixAfterOperation.lastArcLength() ;
	 double dKnewdDe =
	    -newCurvNoStraggling * newEnergyNoStraggling / sqr( newAbsmom ) ;

// 	 HepVector dKnew_dParams = aSurface.arcLengthDerivatives(
// 	    aHelixBeforeOperation,
// 	    aHelixAtLastIntersection,
// 	    aLastSurface ) ;
// 	 dKnew_dParams *= dKnewdDe * dEdx ;

	 // Construct the transport matrix.
// 	 HepMatrix transportMatrix( 5, 5, 1 ) ;
// 	 transportMatrix( HIHelix::kCurvature, HIHelix::kCurvature ) =
// 	    a + dKnew_dParams( HIHelix::kCurvature ) ;
// 	 transportMatrix( HIHelix::kCurvature, HIHelix::kPhi0 ) =
// 	    dKnew_dParams( HIHelix::kPhi0 ) ;
// 	 transportMatrix( HIHelix::kCurvature, HIHelix::kD0 ) =
// 	    dKnew_dParams( HIHelix::kD0 ) ;
// 	 transportMatrix( HIHelix::kCurvature, HIHelix::kCotTheta ) =
// 	    b + dKnew_dParams( HIHelix::kCotTheta ) ;
// 	 transportMatrix( HIHelix::kCurvature, HIHelix::kZ0 ) =
// 	    dKnew_dParams( HIHelix::kZ0 ) ;

// 	 // Transport the error matrix.
// 	 errorMatrix = origMatrix.similarity( transportMatrix ) + diffMatrix ;

	 // Construct the transport matrix.
// 	 HepVector transportVector = aSurface.arcLengthDerivatives(
// 	    aHelixBeforeOperation,
// 	    aHelixAtLastIntersection,
// 	    aLastSurface ) ;
	 HepVector transportVector( HIHelix::kNTrackParameters ) ;
	 aSurface.arcLengthDerivatives(
	    aHelixBeforeOperation,
	    aHelixAtLastIntersection,
	    aLastSurface,
	    transportVector ) ;

	 transportVector *= dKnewdDe * dEdx ;
	 transportVector( HIHelix::kCurvature ) += a ;
	 transportVector( HIHelix::kCotTheta )  += b ;

 	 // Transport the error matrix.
	 HepVector transportDotError( origMatrix * transportVector ) ;
	 double newKK = 0. ;
	 for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
	 {
	    newKK += transportDotError( i ) * transportVector( i ) ;
	 }

	 errorMatrix.fast( HIHelix::kCurvature, HIHelix::kCurvature ) = newKK ;
	 for( int j = HIHelix::kPhi0 ; j <= HIHelix::kZ0 ; ++j )
	 {
	    errorMatrix.fast( j, HIHelix::kCurvature ) =
	       transportDotError( j ) ;

	    for( int k = j ; k <= HIHelix::kZ0 ; ++k )
	    {
	       errorMatrix.fast( k, j ) = origMatrix.fast( k, j ) ;
	    }
	 }
	 errorMatrix += diffMatrix ;


	 // Account for straggling.
	 double dKdDE2 = sqr( origCurvature * energy / sqr( absmom ) ) ;

	 if( m_applyStragglingToError )
	 {
	    errorMatrix( HIHelix::kCurvature, HIHelix::kCurvature ) +=
	       dKdDE2 * ddE2 ;
	 }

	 // Assign the transported error to the helix.
	 aHelixAfterOperation.setErrorMatrix( errorMatrix ) ;


	 // Transport the other error matrix associated with this helix.
	 HepSymMatrix* otherErr = aHelixAfterOperation.otherErrorMatrix() ;

	 HIHelix* beforeNonConst =
	    const_cast< HIHelix* >( &aHelixBeforeOperation ) ;
	 HepSymMatrix* origOtherErr = beforeNonConst->otherErrorMatrix() ;

	 if( otherErr != 0 && origOtherErr != 0 )
	 {
	    HepSymMatrix otherErrDiff = *otherErr - *origOtherErr ;
// 	    *otherErr =
// 	       origOtherErr->similarity( transportMatrix ) + otherErrDiff ;

	    // Transport the error matrix.
	    transportDotError = ( *origOtherErr ) * transportVector ;
	    newKK = 0. ;
	    for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
	    {
	       newKK += transportDotError( i ) * transportVector( i ) ;
	    }

	    otherErr->fast( HIHelix::kCurvature, HIHelix::kCurvature ) = newKK;
	    for( int j = HIHelix::kPhi0 ; j <= HIHelix::kZ0 ; ++j )
	    {
	       otherErr->fast( j, HIHelix::kCurvature ) =
		  transportDotError( j ) ;

	       for( int k = j ; k <= HIHelix::kZ0 ; ++k )
	       {
		  otherErr->fast( k, j ) = origOtherErr->fast( k, j ) ;
	       }
	    }
	    ( *otherErr ) += otherErrDiff ;

	    if( m_applyStragglingToError )
	    {
	       otherErr->fast( HIHelix::kCurvature, HIHelix::kCurvature ) +=
		  dKdDE2 * ddE2 ;
	    }
	 }

	 // Transport the track parameter corrections.
	 HepMatrix transportMatrix( 5, 5, 1 ) ;
//	 transportMatrix.sub( 1, 1, transportVector.T() ) ;
	 for( int n = HIHelix::kCurvature ; n <= HIHelix::kZ0 ; ++n )
	 {
	    transportMatrix( HIHelix::kCurvature, n ) = transportVector( n ) ;
	 }
	 aHelixAfterOperation.transportCorrections( transportMatrix ) ;

	 if( printDiagnostics() )
	 {
	    cout << "KK "
		 << transportMatrix( HIHelix::kCurvature, HIHelix::kCurvature )
		 << " Kphi "
		 << transportMatrix( HIHelix::kCurvature, HIHelix::kPhi0 )
		 << " Kd "
		 << transportMatrix( HIHelix::kCurvature, HIHelix::kD0 )
		 << " Kt "
		 << transportMatrix( HIHelix::kCurvature, HIHelix::kCotTheta )
		 << " Kz "
		 << transportMatrix( HIHelix::kCurvature, HIHelix::kZ0 )
		 << endl ;
	    cout << "error after " << errorMatrix << endl ;
	 }
      }
   }

   return true ;
}

//
// static member functions
//

// GeV
// HIDedxOperation::deltaEnergy( const DGConstMaterialPtr& aMaterial,
// 			      const HIHelix& aHelix )
// {
//    double density = aMaterial->density() / 1000. ;  // in g/cm^3
//    Meters arcLength = aHelix.lastArcLength() ;

//    // Calculate energy loss.  If helix is being propagated backwards,
//    // the arc length will be negative --> positive dE.

//    GeV mass = aHelix.mass() ;
//    GeV absmom = aHelix.momentumMag2() ;
//    GeV energy = absmom + sqr( mass ) ;
//    double beta2 = absmom / energy ;

//    absmom = sqrt( absmom ) ;
//    energy = sqrt( energy ) ;

//    double betaGamma = absmom / mass ;


//    // Loop over the elements of the DGMaterial to calculate the average Z/A.
//    STL_VECTOR( DGMaterial::WeightedElement ) surfaceElements =
//       aMaterial->elements() ;
//    double zOverA  = 0. ;  // atomic weight in g/mol
//    double weights = 0. ; // sum of weights

//    for( STL_VECTOR( DGMaterial::WeightedElement )::const_iterator iter =
// 	   surfaceElements.begin() ;
// 	iter != surfaceElements.end() ;
// 	++iter )
//    {
//       DGMaterial::Element element = ( *iter ).m_element ;
//       double weight    = ( *iter ).m_weight ;

//       weights += weight ;
//       zOverA  += weight * element.m_atomicNumber / element.m_atomicWeight ;
//    }
//    zOverA /= weights ;

//    double rhoZOverA = density * zOverA ;

//    // Change in energy in GeV, arc length in cm, density in g/cm^3.
//    GeV dE = -0.001 * ( arcLength * 100. ) * rhoZOverA *
//       0.307075 / beta2 *
//       ( log( 35466.4 * betaGamma / sqrt( rhoZOverA ) ) - beta2 + 0.5 ) ;

//    return dE ;
// }


GeV
HIDedxOperation::deltaEnergy( const DGConstMaterialPtr& aMaterial,
			      const HIHelix& aHelix,
			      DABoolean& aOK )
{
   double density = aMaterial->density() / 1000. ;  // in g/cm^3
   Meters arcLength = aHelix.lastArcLength() ;

   // Calculate energy loss.  If helix is being propagated backwards,
   // the arc length will be negative --> positive dE.

   GeV mass2 = sqr( aHelix.mass() ) ;
   GeV absmom2 = aHelix.momentumMag2() ;
   GeV energy2 = absmom2 + mass2 ;
   double beta2 = absmom2 / energy2 ;

   GeV energyOrig = sqrt( energy2 ) ;


   // Loop over the elements of the DGMaterial to calculate the average Z/A.
//    STL_VECTOR( DGMaterial::WeightedElement ) surfaceElements =
//       aMaterial->elements() ;
   double zOverA  = 0. ;  // atomic weight in g/mol
   double weights = 0. ; // sum of weights

//    for( STL_VECTOR( DGMaterial::WeightedElement )::const_iterator iter =
// 	   surfaceElements.begin() ;
// 	iter != surfaceElements.end() ;
// 	++iter )
   for( STL_VECTOR( DGMaterial::WeightedElement )::const_iterator iter =
	   aMaterial->elements().begin() ;
	iter != aMaterial->elements().end() ;
	++iter )
   {
      DGMaterial::Element element = ( *iter ).m_element ;
      double weight    = ( *iter ).m_weight ;

      weights += weight ;
      zOverA  += weight * element.m_atomicNumber / element.m_atomicWeight ;
   }
   zOverA /= weights ;

   double rhoZOverA = density * zOverA ;

   // Perform a Runge-Kutta integration of dE/dx over the arc length.

   // Change in energy in GeV, arc length in m, density in g/cm^3.
   GeV dEdx = -rhoZOverA * 0.0307075 / beta2 *
      ( 0.5 * log( 1257865529. * beta2 / ( 1. - beta2 ) / rhoZOverA )
	- beta2 + 0.5 ) ;

   double dE1 = arcLength * dEdx ;

   double energy = energyOrig + 0.5 * dE1 ;
   absmom2 = sqr( energy ) - mass2 ;
//   if( sqr( energy ) < mass2 )
   if( absmom2 < 0. )
   {
      aOK = false ;
      return 0. ;
   }
   beta2 = absmom2 / sqr( energy ) ;
   dEdx = -rhoZOverA * 0.0307075 / beta2 *
      ( 0.5 * log( 1257865529. * beta2 / ( 1. - beta2 ) / rhoZOverA )
	- beta2 + 0.5 ) ;
   double dE2 = arcLength * dEdx ;

   energy = energyOrig + 0.5 * dE2 ;
   absmom2 = sqr( energy ) - mass2 ;
//   if( sqr( energy ) < mass2 )
   if( absmom2 < 0. )
   {
      aOK = false ;
      return 0. ;
   }
   beta2 = absmom2 / sqr( energy ) ;
   dEdx = -rhoZOverA * 0.0307075 / beta2 *
      ( 0.5 * log( 1257865529. * beta2 / ( 1. - beta2 ) / rhoZOverA )
	- beta2 + 0.5 ) ;
   double dE3 = arcLength * dEdx ;

   energy = energyOrig + dE3 ;
   absmom2 = sqr( energy ) - mass2 ;
//   if( sqr( energy ) < mass2 )
   if( absmom2 < 0. )
   {
      aOK = false ;
      return 0. ;
   }
   beta2 = absmom2 / sqr( energy ) ;
   dEdx = -rhoZOverA * 0.0307075 / beta2 *
      ( 0.5 * log( 1257865529. * beta2 / ( 1. - beta2 ) / rhoZOverA )
	- beta2 + 0.5 ) ;
   double dE4 = arcLength * dEdx ;

   double dEtot = ( dE1 + 2. * dE2 + 2. * dE3 + dE4 ) / 6. ;
   if( sqr( energyOrig + dEtot ) < mass2 )
   {
      aOK = false ;
      return 0. ;
   }

   aOK = true ;
   return dEtot ;
}
