// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIMultScatOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Sep 28 11:26:35 EDT 1998
// $Id: HIMultScatOperation.cc,v 1.12 1999/12/15 21:05:31 wsun Exp $
//
// Revision history
//
// $Log: HIMultScatOperation.cc,v $
// Revision 1.12  1999/12/15 21:05:31  wsun
// Sped up matrix element access.
//
// Revision 1.11  1999/11/21 22:31:40  wsun
// Sped up calculations.
//
// Revision 1.10  1999/09/10 21:25:25  wsun
// Don't add d(scattering angle) to error matrix if too large.
//
// Revision 1.9  1999/08/30 21:37:26  wsun
// Added energy loss straggling.
//
// Revision 1.8  1999/08/27 20:14:24  wsun
// Added arc length derivatives to error matrix inflation.
//
// Revision 1.7  1999/08/03 21:57:10  wsun
// Cleaned up code.
//
// Revision 1.6  1999/06/24 22:25:02  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.5  1999/06/18 19:26:00  wsun
// More improvements to low momentum fitting.
//
// Revision 1.4  1999/06/02 23:05:26  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/05/03 19:30:12  wsun
// Fixed wiggling of K and phi0.
//
// Revision 1.2  1999/04/27 22:06:30  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:26  wsun
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
#include "HelixIntersection/HIMultScatOperation.h"
#include "HelixIntersection/HIMagField.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Geometry/Normal3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HIMultScatOperation" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIMultScatOperation::HIMultScatOperation( DABoolean aModifyHelixParameters )
   : m_modifyHelixParameters( aModifyHelixParameters )
{
}

// HIMultScatOperation::HIMultScatOperation( const HIMultScatOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIMultScatOperation::~HIMultScatOperation()
{
}

//
// assignment operators
//
// const HIMultScatOperation& HIMultScatOperation::operator=( const HIMultScatOperation& rhs )
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
HIMultScatOperation::perform( HIHelix& aHelixAfterOperation,
			      const HIHelix& aHelixBeforeOperation,
			      const HIHelix& aHelixAtLastIntersection,
			      HIIntersectionSurface& aSurface,
			      const HIIntersectionSurface& aLastSurface ) const
{
   if( aHelixAfterOperation.hasNullErrorMatrix() &&
       !m_modifyHelixParameters )
   {
      return true ;
   }

   // Calculate width^2 of scattering angle.
   DGConstMaterialPtr material =
      aSurface.materialInLastTransport( aHelixAfterOperation ) ;
   double scatAngle2 = scatteringAngle2( material, aHelixAfterOperation ) ;

   if( printDiagnostics() )
   {
      DGConstMaterialPtr material =
	 aSurface.materialInLastTransport( aHelixAfterOperation ) ;

      cout << "mat " << material->name()
	   << " radl "<< material->radiationLength()
	   << " arc " << aHelixAfterOperation.lastArcLength()
	   << " MS " << ( scatAngle2 > 0. ? sqrt( scatAngle2 ) : -99. )
	   << endl ;
   }

   if( scatAngle2 < 0. )
   {
      if( printDiagnostics() )
      {
	 report( ERROR, kFacilityString )
	    << "Square of scattering angle width < 0." << endl ;
      }

      return false ;
   }



   double K = aHelixAfterOperation.curvature() ;
   double t = aHelixAfterOperation.cotTheta() ;
   double csc2Theta = 1. + t*t ;

   if( m_modifyHelixParameters )
   {
      // Apply multiple scattering to the two angles phi0 and theta.
      double scatteringAngle = sqrt( scatAngle2 ) ;

      double dphi0 = RandGauss::shoot() * scatteringAngle ;
      aHelixAfterOperation.setPhi0( aHelixAfterOperation.phi0() +
				    dphi0 * sqrt( csc2Theta ) ) ;

      // Both cotTheta and curvature need to be wiggled to conserve momentum.
      double dtheta = RandGauss::shoot() * scatteringAngle ;
      aHelixAfterOperation.setCotTheta( t - dtheta * csc2Theta ) ;
      aHelixAfterOperation.setCurvature( K - K * t * dtheta ) ;
   }


   if( !aHelixAfterOperation.hasNullErrorMatrix() && scatAngle2 != 0. )
   {
      if( printDiagnostics() )
      {
	 cout << "error before " << aHelixAfterOperation.errorMatrix() ;
      }

      // update the error matrix
      const HepSymMatrix& cErrorMatrix = aHelixAfterOperation.errorMatrix() ;
      HepSymMatrix errorMatrix = cErrorMatrix ;

      // Add uncertainty due to surface shape and track parameter variation.
      Meters radLength = material->radiationLength() ;
      double radLengths =
	 fabs( aHelixAfterOperation.lastArcLength() / radLength ) ;
      GeV absmom = aHelixAfterOperation.momentumMag() ;
      GeV energy = sqrt( sqr( absmom ) + sqr( aHelixAfterOperation.mass() ) );
      double beta = absmom / energy ;

      double dAngle_dRadLengths =
	 0.5 * 1.36e-2 / beta / absmom / sqrt( radLengths ) *
	 ( 1.076 + 0.038 * log( radLengths ) ) ;

//       HepVector dKnew_dParams = aSurface.arcLengthDerivatives(
// 	 aHelixBeforeOperation,
// 	 aHelixAtLastIntersection,
// 	 aLastSurface ) ;
      HepVector dKnew_dParams( HIHelix::kNTrackParameters ) ;
      aSurface.arcLengthDerivatives(
	 aHelixBeforeOperation,
	 aHelixAtLastIntersection,
	 aLastSurface,
	 dKnew_dParams ) ;

//       double dAngle2 = sqr( dAngle_dRadLengths / radLength ) *
// 	 ( dKnew_dParams.T() * errorMatrix * dKnew_dParams )( 1, 1 ) ;

      double dAngle2( 0. ) ;
      for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
      {
	 double derivi = dKnew_dParams( i ) ;
	 dAngle2 += sqr( derivi ) * errorMatrix.fast( i, i ) ;

	 for( int j = i + 1 ; j <= HIHelix::kZ0 ; ++j )
	 {
	    dAngle2 +=
	       2. * derivi * dKnew_dParams( j ) * errorMatrix.fast( j, i ) ;
	 }
      }
      dAngle2 *= sqr( dAngle_dRadLengths / radLength ) ;

      if( printDiagnostics() )
      {
	 cout << " scatAngle2 = " << scatAngle2 << " + " << dAngle2 << endl ;
      }


      // For very small arc lengths, dAngle2 can be very large because of
      // 1. / sqrt( radLengths ).
      double angle2PlusDAngle2 = scatAngle2 ;
      if( dAngle2 < scatAngle2 )
      {
	 angle2PlusDAngle2 += dAngle2 ;
      }

      // See CBX 96-20
      errorMatrix.fast( HIHelix::kCurvature, HIHelix::kCurvature ) +=
	 sqr( K * t ) * angle2PlusDAngle2 ;
      errorMatrix.fast( HIHelix::kCotTheta, HIHelix::kCurvature ) +=
	 K * t * csc2Theta * angle2PlusDAngle2 ;
      errorMatrix.fast( HIHelix::kCotTheta,  HIHelix::kCotTheta ) +=
	 sqr( csc2Theta ) * angle2PlusDAngle2 ;
      errorMatrix.fast( HIHelix::kPhi0, HIHelix::kPhi0 ) +=
	 csc2Theta * angle2PlusDAngle2 ;
      aHelixAfterOperation.setErrorMatrix( errorMatrix ) ;

      if( printDiagnostics() )
      {
	 cout << "error after" << errorMatrix << endl ;
      }

      // Inflate other error matrices associated with the helix.
      HepSymMatrix* otherErr = aHelixAfterOperation.otherErrorMatrix() ;
      if( otherErr != 0 )
      {
	 dAngle2 = 0. ;
	 for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
	 {
	    double derivi = dKnew_dParams( i ) ;
	    dAngle2 += sqr( derivi ) * errorMatrix.fast( i, i ) ;

	    for( int j = i + 1 ; j <= HIHelix::kZ0 ; ++j )
	    {
	       dAngle2 +=
		  2. * derivi * dKnew_dParams( j ) * otherErr->fast( j, i ) ;
	    }
	 }
	 dAngle2 *= sqr( dAngle_dRadLengths / radLength ) ;

	 double angle2PlusDAngle2 = scatAngle2 ;
	 if( dAngle2 < scatAngle2 )
	 {
	    angle2PlusDAngle2 += dAngle2 ;
	 }

// 	 dAngle2 = sqr( dAngle_dRadLengths / radLength ) *
// 	    ( dKnew_dParams.T() * ( *otherErr ) * dKnew_dParams )( 1, 1 ) ;

// 	 angle2PlusDAngle2 = scatAngle2 + dAngle2 ;

	 otherErr->fast( HIHelix::kCurvature, HIHelix::kCurvature ) +=
	    sqr( K * t ) * angle2PlusDAngle2 ;
	 otherErr->fast( HIHelix::kCotTheta, HIHelix::kCurvature ) +=
	    K * t * csc2Theta * angle2PlusDAngle2 ;
	 otherErr->fast( HIHelix::kCotTheta,  HIHelix::kCotTheta ) +=
	    sqr( csc2Theta ) * angle2PlusDAngle2 ;
	 otherErr->fast( HIHelix::kPhi0, HIHelix::kPhi0 ) +=
	    csc2Theta * angle2PlusDAngle2 ;
      }
   }

   return true ;
}

//
// static member functions
//
double
HIMultScatOperation::scatteringAngle2( const DGConstMaterialPtr& aMaterial,
				       const HIHelix& aHelix )
{
   Meters arcLength = aHelix.lastArcLength() ;
   
   double radLengths = fabs( arcLength / aMaterial->radiationLength() ) ;
   if( radLengths <= 0. )
   {
      return 0. ;
   }

   GeV absmom = aHelix.momentumMag() ;
   GeV energy = sqrt( sqr( absmom ) + sqr( aHelix.mass() ) );
   double beta = absmom / energy ;

   // from PDG
   return 1.8496e-4 * radLengths *
      sqr( ( 1. + 0.038 * log( radLengths ) ) / beta / absmom ) ;
}
