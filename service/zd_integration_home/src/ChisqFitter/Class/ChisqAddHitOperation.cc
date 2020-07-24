// -*- C++ -*-
//
// Package:     <ChisqFitter>
// Module:      ChisqAddHitOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed Jun  9 03:13:52 EDT 1999
// $Id: ChisqAddHitOperation.cc,v 1.16 2000/08/16 20:30:51 wsun Exp $
//
// Revision history
//
// $Log: ChisqAddHitOperation.cc,v $
// Revision 1.16  2000/08/16 20:30:51  wsun
// * Bug fix: hits were not really being dropped before.
// * Update to new TRTrackFitQuality.
// * Added usage and production tags for hits.
//
// Revision 1.15  2000/06/10 00:31:32  wsun
// New interface for HIFitHelix::HitAndLinkData.
//
// Revision 1.14  2000/06/09 01:40:39  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.13  2000/02/22 17:23:11  wsun
// Added flag for disabling hit rejection.
//
// Revision 1.12  2000/02/15 23:50:13  wsun
// Check for null CalibratedHit.
//
// Revision 1.11  2000/02/03 19:10:16  wsun
// Added use of HICalibratedHit.
//
// Revision 1.10  2000/01/11 20:15:08  wsun
// Track error is added to chisq only if error matrix is nonnull.
//
// Revision 1.9  1999/12/15 21:58:18  wsun
// Updated to new HIIntersectionSurface interface.
//
// Revision 1.8  1999/11/03 20:18:11  wsun
// Updated to new TrackFitDRHitLink interface.
//
// Revision 1.7  1999/08/30 22:06:05  wsun
// Attach discarded hits to track.
//
// Revision 1.6  1999/08/27 21:02:01  wsun
// Update to new HIHelixOperation::perform() and TrackFitDRHitLink.
//
// Revision 1.5  1999/08/03 22:40:44  wsun
// Mods for fitting curler back-halves.
//
// Revision 1.4  1999/07/07 16:44:12  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.3  1999/06/24 22:53:05  wsun
// * Added use of HIMagField and new SiHits.
// * Added ability to fix any combination of parameters in track fitting.
//
// Revision 1.2  1999/06/20 21:17:08  wsun
// Allow user to fix parameters.
//
// Revision 1.1.1.1  1999/06/16 02:46:48  wsun
// First submission.
//

#include "Experiment/Experiment.h"

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
#include "ChisqFitter/ChisqAddHitOperation.h"
#include "ChisqFitter/ChisqHelix.h"

#include "HelixIntersection/HIIntersectionSurface.h"
#include "CalibratedData/CalibratedHit.h"

#include "CLHEP/Geometry/Normal3D.h"

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

static const char* const kFacilityString = "ChisqFitter.ChisqAddHitOperation" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqAddHitOperation.cc,v 1.16 2000/08/16 20:30:51 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChisqAddHitOperation::ChisqAddHitOperation( double aMaxHitChisq,
					    DABoolean aRejectHits )
   : m_maxHitChisq( aMaxHitChisq ),
     m_rejectHits(  aRejectHits )
{
}

// ChisqAddHitOperation::ChisqAddHitOperation( const ChisqAddHitOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ChisqAddHitOperation::~ChisqAddHitOperation()
{
}

//
// assignment operators
//
// const ChisqAddHitOperation& ChisqAddHitOperation::operator=( const ChisqAddHitOperation& rhs )
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
ChisqAddHitOperation::perform(
   HIHelix& aHelixAfterOperation,
   const HIHelix& aHelixBeforeOperation,
   const HIHelix& aHelixAtLastIntersection,
   HIIntersectionSurface& aSurface,
   const HIIntersectionSurface& aLastSurface ) const
{
   // Cast the aHelixAfterOperation into a local ChisqHelix.
   ChisqHelix* chisqHelix ;
   if( aHelixAfterOperation.helixType() == ChisqHelix::kHelixType )
   {
      chisqHelix = ( ChisqHelix* ) &aHelixAfterOperation ;
      chisqHelix->setReportFitResultsAtInitRefPt( true ) ;
   }
   else
   {
      report( ERROR, kFacilityString )
	 << "Helix in use is a " << aHelixAfterOperation.helixType()
	 << ", not a ChisqHelix." << endl ;
      return false ;
   }



   STL_VECTOR( ChisqHelix::HitAndLinkData ) hitsToAddToTrack ;

   // Loop through all the CalibratedHits associated with this surface.
   do
   {
      if( aSurface.hiCalibratedHit() == 0 ||
	  aSurface.hiCalibratedHit()->infoOnTrack() )
      {
	 continue ;
      }

      // Get the measured and predicted signed drift distances and
      // calculate the residual.
      Meters dPredicted = aSurface.dca( *chisqHelix ) ;
      Meters dMeasured = aSurface.measuredDca() ;
      Meters residual = dMeasured - dPredicted ;

      double weight = aSurface.fittingWeight() ;

      HepVector derivs( 5, 0 ) ;
      aSurface.derivatives( *chisqHelix, derivs ) ;

      double trackErr2 = 0. ;
      double deltaChisq ;

      // If the initial error matrix is not to be included in the fit, it
      // will be null.
      if( chisqHelix->hasNullErrorMatrix() )
      {
	 deltaChisq = sqr( residual ) * weight ;
      }
      else
      {
	 trackErr2 = chisqHelix->errorMatrix().similarity( derivs ) ;
	 weight /= 1. + trackErr2 * weight ;
	 deltaChisq = sqr( residual ) * weight ;
      }

      if( printDiagnostics() )
      {
         cout << "helix pos " << chisqHelix->position()
	      << " dm-dp " << residual << " chi2 " << deltaChisq << endl
	      << " weight " << weight << " trackErr2 " << trackErr2 << endl
	      << "derivs " << derivs[0] << " " << derivs[1] << " " << derivs[2]
	      << " " << derivs[3] << " " << derivs[4]
//	      << chisqHelix->errorMatrix()
	      << endl ;
      }

      // Check the hit chisq is not too large.
//      DABoolean hitUsed = true ;
      if( !aSurface.hiCalibratedHit()->useInFit() )
      {
	 // numberHitsDropped is zeroed at the beginning of each iteration.
	 chisqHelix->setNumberHitsDropped(
	    chisqHelix->numberHitsDropped() + 1 ) ;
      }	 
      else if( m_rejectHits && deltaChisq > m_maxHitChisq )
      {
	 if( printDiagnostics() )
	 {
	    cout << "Dropping " << aSurface.calibratedHit()->deviceType()
		 << " hit ID " << aSurface.calibratedHit()->identifier()
		 << " chisq " << deltaChisq
		 << endl ;
	 }

//	 hitUsed = false ;
	 aSurface.hiCalibratedHit()->setUseInFit( false ) ;
	 chisqHelix->setNumberHitsDropped(
	    chisqHelix->numberHitsDropped() + 1 ) ;
      }
      else
      {
	 int numberFreeParameters = chisqHelix->numberOfFreeParameters() ;

	 // Get the vector of derivatives d(DCA)/d(params).  The derivatives
	 // are given at the initial reference point.
	 HepVector tmpDerivs( numberFreeParameters, 0 ) ;

	 int fillIndex = 1 ;
	 for( int readIndex = HIHelix::kCurvature ;
	      readIndex <= HIHelix::kZ0 ;
	      ++readIndex )
	 {
	    if( chisqHelix->parameterFree( readIndex ) )
	    {
	       tmpDerivs( fillIndex ) = derivs( readIndex ) ;
	       ++fillIndex ;
	    }
	 }
	 derivs = tmpDerivs ;


	 // Accumulate information matrix and residual vector.
	 HepSymMatrix infoMatrix( numberFreeParameters, 0 ) ;
	 HepVector    residualVector( numberFreeParameters, 0 ) ;

	 for( int i = 1 ; i <= numberFreeParameters ; ++i )
	 {
	    residualVector( i ) = residual * weight * derivs( i ) ;

	    for( int j = i ; j <= numberFreeParameters ; ++j )
	    {
	       infoMatrix.fast( j, i ) = derivs( i ) * derivs( j ) * weight;
	    }
	 }

	 chisqHelix->incrementInfoMatrix( infoMatrix ) ;
	 chisqHelix->incrementResidualVector( residualVector ) ;
	 chisqHelix->incrementChisq( deltaChisq ) ;
      }

      // Calculate sin of the entrance angle (using the track
      // momentum BEFORE the current hit has been added), and fill link data.
      ChisqHelix::HitAndLinkData newHitAndLinkData(
	 aSurface.hiCalibratedHit(),
	 aSurface.hiCalibratedHit()->instantiateLinkData(
	    *chisqHelix,
	    aSurface,
	    residual,
	    sqrt( 1. / weight ),
	    dPredicted,
	    trackErr2 >= 0. ? sqrt( trackErr2 ) : 0. ) ) ;

      chisqHelix->addHitOnTrack( newHitAndLinkData ) ;

      // This prevents the current hit from being added to this fit again.
      aSurface.hiCalibratedHit()->setInfoOnTrack( true ) ;
   }
   while( aSurface.advanceToNextCalibratedHit( *chisqHelix ) ) ;

   // Return helix to position at start of fit, where the fitted track
   // parameters are to be given.
   chisqHelix->returnToStart() ;

   return true ;
}

//
// static member functions
//
