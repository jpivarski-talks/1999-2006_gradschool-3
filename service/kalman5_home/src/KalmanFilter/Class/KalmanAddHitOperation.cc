// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanAddHitOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Oct 27 00:31:14 EST 1998
// $Id: KalmanAddHitOperation.cc,v 1.22 2001/03/27 06:37:37 wsun Exp $
//
// Revision history
//
// $Log: KalmanAddHitOperation.cc,v $
// Revision 1.22  2001/03/27 06:37:37  wsun
// Mods for hit removal.
//
// Revision 1.21  2000/10/08 23:41:03  wsun
// Updated to new HICalibratedHit::addResidualAndErrorSquared().
//
// Revision 1.20  2000/06/10 00:13:10  wsun
// Modified HIFitHelix::HitAndLinkData interface.
//
// Revision 1.19  2000/06/09 00:41:59  wsun
// Link data are now created by HICalibratedHit.
//
// Revision 1.18  2000/02/03 18:49:33  wsun
// Added use of HICalibratedHit.
//
// Revision 1.17  1999/12/16 02:46:10  wsun
// Inadvertently introduced an error in the last submission.
//
// Revision 1.16  1999/12/15 21:11:44  wsun
// * More explicit matrix manipulation.
// * Removed HIFitQualityCondition.
// * Added static function invert5x5SymMatrix(), which is much faster than
//   HepSymMatrix::invert().
//
// Revision 1.15  1999/11/21 22:37:07  wsun
// Sped up calculations with explicit matrix manipulation.
//
// Revision 1.14  1999/11/03 03:29:56  wsun
// Updated to new TrackFitDRHitLink interface.
//
// Revision 1.13  1999/09/30 20:02:07  wsun
// Fixed drift-sign-flipping condition.
//
// Revision 1.12  1999/09/10 21:53:14  wsun
// Return from perform() if the surface's calibrated hit is null.
//
// Revision 1.11  1999/08/30 21:55:00  wsun
// Attach discarded hits to the track.
//
// Revision 1.10  1999/08/27 20:34:56  wsun
// * New HIHelixOperation::perform().
// * Moved track parameter update to KalmanUpdateParamsOperation.
//
// Revision 1.9  1999/08/03 22:33:35  wsun
// Mods for fitting curler back-halves.
//
// Revision 1.8  1999/07/07 16:46:38  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.7  1999/06/24 22:44:23  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.6  1999/06/18 19:27:58  wsun
// More improvements to low momentum fitting.
//
// Revision 1.5  1999/06/02 23:07:40  wsun
// Improved low-momentum fitting.
//
// Revision 1.4  1999/05/09 21:38:33  wsun
// Updated code to new Lattice interface.
//
// Revision 1.3  1999/04/30 23:06:06  wsun
// Make new link data only when hit is added.
//
// Revision 1.2  1999/04/29 20:16:40  wsun
// Additional fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:20:49  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:28  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "CalibratedData/CalibratedHit.h"

#include "KalmanFilter/KalmanHelix.h"
#include "KalmanFilter/KalmanAddHitOperation.h"

#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HICondition.h"
#include "HelixIntersection/HIDedxOperation.h"
#include "HelixIntersection/HICompositeOperation.h"
#include "HelixIntersection/HIMultScatOperation.h"

// TEMPORARY!!
#include "HelixIntersection/HISiSensorPlane.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Geometry/Normal3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"KalmanFilter.KalmanAddHitOperation" ;

//
// static data member definitions
//

//
// constructors and destructor
//
KalmanAddHitOperation::KalmanAddHitOperation( double aMaxHitChisq )
   : m_maxHitChisq( aMaxHitChisq ),
     m_removeHitsFromFit( false )
{
}

// KalmanAddHitOperation::KalmanAddHitOperation( const KalmanAddHitOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

KalmanAddHitOperation::~KalmanAddHitOperation()
{
}

//
// assignment operators
//
// const KalmanAddHitOperation& KalmanAddHitOperation::operator=( const KalmanAddHitOperation& rhs )
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
KalmanAddHitOperation::perform(
   HIHelix& aHelixAfterOperation,
   const HIHelix& aHelixBeforeOperation,
   const HIHelix& aHelixAtLastIntersection,
   HIIntersectionSurface& aSurface,
   const HIIntersectionSurface& aLastSurface ) const
{
   // Do nothing if the helix does not have an error matrix or
   // if the surface's CalibratedHit is null.
   if( aHelixAfterOperation.hasNullErrorMatrix() )
   {
      return true ;
   }

   // Cast the aHelixAfterOperation into a local KalmanHelix.
   KalmanHelix* kalmanHelix ;
   if( aHelixAfterOperation.helixType() == KalmanHelix::kHelixType )
   {
      kalmanHelix = ( KalmanHelix* ) &aHelixAfterOperation ;
   }
   else
   {
      report( ERROR, kFacilityString )
	 << "Helix in use is not a KalmanHelix." << endl ;
      return false ;
   }


   // We want to manipulate the helix after the other operations have
   // been performed (like dEdx), so the initial helix is aHelixAfterOperation.
   // aHelixBeforeOperation will be ignored.

   // Don't update the track until all the hits have been added -- otherwise
   // the track gets moved off the PCA.

   // The track parameter corrections have been transported by HIHelix.
   HepVector dParams = kalmanHelix->trackParameterCorrections() ;
   HepSymMatrix transportedError = kalmanHelix->errorMatrix() ;
   double deltaChisq = 0. ;

   STL_VECTOR( KalmanHelix::HitAndLinkData ) hitsToAddToTrack ;
   DABoolean hitsAdded = false ;

   

   // Loop through all the CalibratedHits associated with this surface.
   do
   {
      if( aSurface.hiCalibratedHit() == 0 ||
	  aSurface.hiCalibratedHit()->infoOnTrack() )
      {
	 continue ;
      }

      // Calculate residual and error even if the hit is not to be added.

      // Get the DCA and the vector of derivatives d(DCA)/d(params).
      HepVector derivs( HIHelix::kNTrackParameters ) ;
      aSurface.derivatives( *kalmanHelix, derivs ) ;

      // Calculate error * derivs * derivs.T * error,
      // predicted DCA, and its error.
      HepVector VD( HIHelix::kNTrackParameters, 0 ) ;
      for( int m = 1 ; m <= HIHelix::kNTrackParameters ; ++m )
      {
	 for( int n = 1 ; n <= HIHelix::kNTrackParameters ; ++n )
	 {
	    VD( m ) += transportedError( m, n ) * derivs( n ) ;
	 }
      }

      double sigma2 = 1. / aSurface.fittingWeight() ;

      // If the hit is to be removed instead of added to the fit, assign
      // a negative weight.
      if( m_removeHitsFromFit )
      {
	 sigma2 *= -1. ;
      }

      HepSymMatrix VDDtV( HIHelix::kNTrackParameters ) ;

      Meters dPredicted = aSurface.dca( *kalmanHelix ) ;
      double dPredError2 = 0. ;

      for( int i = 1 ; i <= HIHelix::kNTrackParameters ; ++i )
      {
	 double derivi = derivs( i ) ;
	 dPredicted += derivi * dParams( i ) ;
	 dPredError2 += sqr( derivi ) * transportedError.fast( i, i ) ;

	 VDDtV.fast( i, i ) = sqr( VD( i ) ) ;

	 for( int j = i + 1 ; j <= HIHelix::kNTrackParameters ; ++j )
	 {
	    VDDtV.fast( j, i ) = VD( j ) * VD( i ) ;
	    dPredError2 +=
	       2. * derivi * derivs( j ) * transportedError.fast( j, i ) ;
	 }
      }

      // Error on drift residual where the current hit is not included in fit.
      double residualError2 = sigma2 + dPredError2 ;

//       if( printDiagnostics() )
//       {
// 	 cout << "VD " << VD << endl;
// 	 cout << "dError " << VDDtV * (1./residualError2) << endl;
//       }

      // Get measured signed drift distance and calculate residual where
      // the current hit is not included in the fit.
      Meters dMeasured = aSurface.measuredDca() ;
      Meters residual = dMeasured - dPredicted ;
      Meters dPredictedWithoutHit = dPredicted ;

      if( printDiagnostics() )
      {
	 cout << "helix x/p " << kalmanHelix->position() 
	      << "/" << kalmanHelix->momentum()
	      << endl ;
	 cout << "dmeas " << dMeasured << " - dpred " << dPredicted
	      << " = " << residual
	      << "   error2 = " << sigma2 << " + " << dPredError2 << endl ;
      }

      // Calculate correction to track parameters -- use parentheses to
      // ensure the double multiplies a HepVector, not a HepSymMatrix.
      // dParamsLocal = residual * optimizedError * derivs / sigma2.
      HepVector dParamsLocal( HIHelix::kNTrackParameters, 0 ) ;
      if( aSurface.hiCalibratedHit()->useInFit() )
      {
	 HepSymMatrix optimizedError( 5 ) ;
	 // optimized error = transportedError - ( 1./residualError2 ) * VDDtV
	 for( int r = 1 ; r <= HIHelix::kNTrackParameters ; ++r )
	 {
	    for( int s = r ; s <= HIHelix::kNTrackParameters ; ++s )
	    {
	       optimizedError.fast( s, r ) = transportedError.fast( s, r ) -
		  VDDtV.fast( s, r ) / residualError2 ;
	    }
	 }

	 for( int p = 1 ; p <= HIHelix::kNTrackParameters ; ++p )
	 {
	    for( int q = 1 ; q <= HIHelix::kNTrackParameters ; ++q )
	    {
	       dParamsLocal( p ) += optimizedError( p, q ) * derivs( q ) ;
	    }
	 }
	 dParamsLocal *= residual / sigma2 ;

	 // New predicted DCA = old dPredicted + derivs*dParamsLocal.T
	 for( int k = 1 ; k <= HIHelix::kNTrackParameters ; ++k )
	 {
	    dPredicted += derivs( k ) * dParamsLocal( k ) ;
	 }

	 // Contribution to chisq from hit residual.
	 double chisqLocal = sqr( dMeasured - dPredicted ) / fabs( sigma2 ) ;

	 // Invert old error (using a function that is much faster than
	 // HepSymMatrix::invert()) and calculate chisq for this hit.
	 HepSymMatrix errorInv( 5 ) ;

	 if( m_removeHitsFromFit )
	 {
	    invert5x5SymMatrix( optimizedError, errorInv ) ;
	 }
	 else
	 {
	    invert5x5SymMatrix( transportedError, errorInv ) ;
	 }

	 for( int w = 1 ; w <= HIHelix::kNTrackParameters ; ++w )
	 {
	    double dParamsi = dParamsLocal( w ) ;
	    chisqLocal += sqr( dParamsi ) * errorInv.fast( w, w ) ;

	    for( int j = w + 1 ; j <= HIHelix::kNTrackParameters ; ++j )
	    {
	       chisqLocal +=
		  2. * dParamsi * dParamsLocal( j ) * errorInv.fast( j, w ) ;
	    }
	 }

	 HepVector newDParams( 5 ) ;// = dParams + dParamsLocal ;
	 for( int t = 1 ; t <= HIHelix::kNTrackParameters ; ++t )
	 {
	    newDParams( t ) = dParams( t ) + dParamsLocal( t ) ;
	 }

	 if( printDiagnostics() )
	 {
	    double Kold = kalmanHelix->trackParameters()[0] + dParams[0] ;
	    double told = kalmanHelix->trackParameters()[3] + dParams[3] ;
	    double Knew =kalmanHelix->trackParameters()[0] + newDParams[0] ;
	    double tnew = kalmanHelix->trackParameters()[3] + newDParams[3] ;

	    HepSymMatrix corr0( 5 ) ;
	    for( int w = 1 ; w <= HIHelix::kNTrackParameters ; ++w )
	    {
	       for( int x = w ; x <= HIHelix::kNTrackParameters ; ++x )
	       {
		  corr0.fast( x, w ) =
		     transportedError.fast( x, w ) *
		     1. / sqrt( transportedError.fast( x, x ) *
				transportedError.fast( w, w ) ) ;
	       }
	    }

	    HepSymMatrix corr1( 5 ) ;
	    for( int y = 1 ; y <= HIHelix::kNTrackParameters ; ++y )
	    {
	       for( int z = y ; z <= HIHelix::kNTrackParameters ; ++z )
	       {
		  corr1.fast( z, y ) =
		     optimizedError.fast( z, y ) *
		     1. / sqrt( optimizedError.fast( z, z ) *
				optimizedError.fast( y, y ) ) ;
	       }
	    }

	    double dPredErrorOpt2 = 0. ;

	    for( int i = 1 ; i <= HIHelix::kNTrackParameters ; ++i )
	    {
	       double derivi = derivs( i ) ;
	       dPredErrorOpt2 += sqr( derivi ) * optimizedError.fast( i, i ) ;

	       for( int j = i + 1 ; j <= HIHelix::kNTrackParameters ; ++j )
	       {
		  dPredErrorOpt2 +=
		     2. * derivi * derivs( j ) * optimizedError.fast( j, i ) ;
	       }
	    }

	    cout
	       << "derivatives "
	       << derivs[0] << " "
	       << derivs[1] << " "
	       << derivs[2] << " "
	       << derivs[3] << " "
	       << derivs[4] << " DtVoptD " << dPredErrorOpt2 << endl
	       << "transport matrix"
	       << kalmanHelix->lastTransportMatrix() << endl
	       << "error matrix" << transportedError << endl
	       << "corr matrix" << corr0 << endl
	       << "new error matrix" << optimizedError << endl
	       << "new corr matrix" << corr1 << endl
	       << "reference point " << kalmanHelix->referencePoint() << endl
	       << "params      "
	       << kalmanHelix->trackParameters()[0] << " "
	       << kalmanHelix->trackParameters()[1] << " "
	       << kalmanHelix->trackParameters()[2] << " "
	       << kalmanHelix->trackParameters()[3] << " "
	       << kalmanHelix->trackParameters()[4]
	       << endl
	       << "dParams     "
	       << dParams[0] << " "
	       << dParams[1] << " "
	       << dParams[2] << " "
	       << dParams[3] << " "
	       << dParams[4] << endl
	       << "new dParams "
	       << newDParams[0] << " "
	       << newDParams[1] << " "
	       << newDParams[2] << " "
	       << newDParams[3] << " "
	       << newDParams[4] << " " << endl
	       << "errors      "
	       << sqrt( optimizedError[0][0] ) << " "
	       << sqrt( optimizedError[1][1] ) << " "
	       << sqrt( optimizedError[2][2] ) << " "
	       << sqrt( optimizedError[3][3] ) << " "
	       << sqrt( optimizedError[4][4] ) << " " << endl
	       << "old mom " << 0.225 / fabs( Kold ) * sqrt( 1. + told*told )
	       << " new mom " << 0.225 / fabs( Knew ) * sqrt( 1. + tnew*tnew )
	       << endl
	       << "chi2 " << kalmanHelix->chisq() << " + "
	       << deltaChisq << " + " << chisqLocal
	       << "  ndof " << kalmanHelix->ndof() << " + 1"
	       << "  mass " << kalmanHelix->mass() << endl ;
	 }

	 dParams = newDParams ;
	 deltaChisq += chisqLocal ;
	 transportedError = optimizedError ;
	 hitsAdded = true ;
      }

      // Calculate sin of the entrance angle (using the track
      // momentum BEFORE the current hit has been added), and fill link data.
      KalmanHelix::HitAndLinkData newHitAndLinkData(
	 aSurface.hiCalibratedHit(),
	 aSurface.hiCalibratedHit()->instantiateLinkData(
	    *kalmanHelix,
	    aSurface,
	    residual, // hit not included in residual
	    sqrt( fabs( residualError2 ) ),
	    dPredictedWithoutHit,
	    sqrt( dPredError2 ) ) ) ;

      hitsToAddToTrack.push_back( newHitAndLinkData ) ;

      // Store residual and error2 in HICalibrated hit.
      aSurface.hiCalibratedHit()->addResidualAndErrorSquared(
	 residual,
	 fabs( residualError2 ),
	 dPredictedWithoutHit,
	 dPredError2 ) ;

      // This prevents the current hit from being added to this fit again.
      aSurface.hiCalibratedHit()->setInfoOnTrack( true ) ;
   }
   while( aSurface.advanceToNextCalibratedHit( *kalmanHelix ) ) ;


   if( hitsToAddToTrack.size() > 0 )
   {
      if( hitsAdded )
      {
	 // The original helix info is not been changed.  The new error matrix
	 // and the changes in chisq, ndof, and track parameters are stored
	 // separately.
	 kalmanHelix->setNewErrorMatrix( transportedError ) ;
	 kalmanHelix->setNewTrackParameterCorrections( dParams ) ;
	 kalmanHelix->setDeltaChisq( deltaChisq ) ;
      }

      kalmanHelix->setNewHitsOnTrack( hitsToAddToTrack ) ;
   }

   return true ;
}

//
// static member functions
//

void
KalmanAddHitOperation::invert5x5SymMatrix( const HepSymMatrix& input,
					   HepSymMatrix& output )
{
   double tmp1[4][5] ;
   double tmp2[5][5] ;

   tmp2[0][0] = input.fast(1,1) ;
   tmp2[1][0] = input.fast(2,1) ;
   tmp2[2][0] = input.fast(3,1) ;
   tmp2[3][0] = input.fast(4,1) ;
   tmp2[4][0] = input.fast(5,1) ;

   double tmp211inv = 1. / tmp2[0][0] ;

   tmp1[0][1] = tmp2[1][0] * tmp211inv ;
   tmp1[0][2] = tmp2[2][0] * tmp211inv ;
   tmp1[0][3] = tmp2[3][0] * tmp211inv ;
   tmp1[0][4] = tmp2[4][0] * tmp211inv ;

   tmp2[1][1] = input.fast(2,2) - tmp2[1][0] * tmp1[0][1] ;
   tmp2[2][1] = input.fast(3,2) - tmp2[2][0] * tmp1[0][1] ;
   tmp2[3][1] = input.fast(4,2) - tmp2[3][0] * tmp1[0][1] ;
   tmp2[4][1] = input.fast(5,2) - tmp2[4][0] * tmp1[0][1] ;

   double tmp222inv = 1. / tmp2[1][1] ;

   tmp1[1][2] = tmp2[2][1] * tmp222inv ;
   tmp1[1][3] = tmp2[3][1] * tmp222inv ;
   tmp1[1][4] = tmp2[4][1] * tmp222inv ;

   tmp2[2][2] = input.fast(3,3) -
      tmp2[2][0] * tmp1[0][2] - tmp2[2][1] * tmp1[1][2] ;
   tmp2[3][2] = input.fast(4,3) -
      tmp2[3][0] * tmp1[0][2] - tmp2[3][1] * tmp1[1][2] ;
   tmp2[4][2] = input.fast(5,3) -
      tmp2[4][0] * tmp1[0][2] - tmp2[4][1] * tmp1[1][2] ;

   double tmp233inv = 1. / tmp2[2][2] ;

   tmp1[2][3] = tmp2[3][2] * tmp233inv ;
   tmp1[2][4] = tmp2[4][2] * tmp233inv ;

   tmp2[3][3] = input.fast(4,4) -
      tmp2[3][0] * tmp1[0][3] -
      tmp2[3][1] * tmp1[1][3] -
      tmp2[3][2] * tmp1[2][3] ;
   tmp2[4][3] = input.fast(5,4) -
      tmp2[4][0] * tmp1[0][3] - 
      tmp2[4][1] * tmp1[1][3] -
      tmp2[4][2] * tmp1[2][3] ;

   double tmp244inv = 1. / tmp2[3][3] ;

   tmp1[3][4] = tmp2[4][3] * tmp244inv ;

   tmp2[4][4] = input.fast(5,5) -
      tmp2[4][0] * tmp1[0][4] -
      tmp2[4][1] * tmp1[1][4] -
      tmp2[4][2] * tmp1[2][4] -  
      tmp2[4][3] * tmp1[3][4] ;

   double tmp255inv = 1. / tmp2[4][4] ;

   double out55 = tmp255inv ;
   double out54 = -( tmp2[4][3] * out55 ) * tmp244inv ;
   double out53 = -( tmp2[4][2] * out55 +
		     tmp2[3][2] * out54 ) * tmp233inv ;
   double out52 = -( tmp2[4][1] * out55 +
		     tmp2[3][1] * out54 +
		     tmp2[2][1] * out53 ) * tmp222inv ;
   double out51 = -( tmp2[4][0] * out55 +
		     tmp2[3][0] * out54 +
		     tmp2[2][0] * out53 +
		     tmp2[1][0] * out52 ) * tmp211inv ;

   double out44 = ( 1. -
		    tmp2[4][3] * out54 ) * tmp244inv ;
   double out43 = -( tmp2[4][2] * out54 +
		     tmp2[3][2] * out44 ) * tmp233inv ;
   double out42 = -( tmp2[4][1] * out54 +
		     tmp2[3][1] * out44 +
		     tmp2[2][1] * out43 ) * tmp222inv ;
   double out41 = -( tmp2[4][0] * out54 +
		     tmp2[3][0] * out44 +
		     tmp2[2][0] * out43 +
		     tmp2[1][0] * out42 ) * tmp211inv ;

   double out33 = ( 1. -
		    tmp2[4][2] * out53 -
		    tmp2[3][2] * out43 ) * tmp233inv ;
   double out32 = -( tmp2[4][1] * out53 +
		     tmp2[3][1] * out43 +
		     tmp2[2][1] * out33 ) * tmp222inv ;
   double out31 = -( tmp2[4][0] * out53 +
		     tmp2[3][0] * out43 +
		     tmp2[2][0] * out33 +
		     tmp2[1][0] * out32 ) * tmp211inv ;

   double out22 = ( 1. -
		    tmp2[4][1] * out52 -
		    tmp2[3][1] * out42 -
		    tmp2[2][1] * out32 ) * tmp222inv ;
   double out21 = -( tmp2[4][0] * out52 +
		     tmp2[3][0] * out42 +
		     tmp2[2][0] * out32 +
		     tmp2[1][0] * out22 ) * tmp211inv ;

   output.fast(1,1) = ( 1. -
			tmp2[4][0] * out51 -
			tmp2[3][0] * out41 -
			tmp2[2][0] * out31 - 
			tmp2[1][0] * out21 ) * tmp211inv ;

   output.fast(5,5) = out55 ;
   output.fast(5,4) = out54 ;
   output.fast(5,3) = out53 ;
   output.fast(5,2) = out52 ;
   output.fast(5,1) = out51 ;
   output.fast(4,4) = out44 ;
   output.fast(4,3) = out43 ;
   output.fast(4,2) = out42 ;
   output.fast(4,1) = out41 ;
   output.fast(3,3) = out33 ;
   output.fast(3,2) = out32 ;
   output.fast(3,1) = out31 ;
   output.fast(2,2) = out22 ;
   output.fast(2,1) = out21 ;
}
