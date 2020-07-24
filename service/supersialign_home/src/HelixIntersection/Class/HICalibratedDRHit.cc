// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICalibratedDRHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Jun  8 16:35:08 EDT 2000
// $Id: HICalibratedDRHit.cc,v 1.10 2002/02/17 17:23:04 wsun Exp $
//
// Revision history
//
// $Log: HICalibratedDRHit.cc,v $
// Revision 1.10  2002/02/17 17:23:04  wsun
// Corrected previous revision log.
//
// Revision 1.9  2002/02/17 17:20:32  wsun
// Protect against calling applyTrackCorrections() twice.
//
// Revision 1.8  2001/09/26 08:03:49  wsun
// Updated to new TrackFitDRHitLink.
//
// Revision 1.7  2000/12/02 01:30:26  wsun
// Added avg chg on track to HICalibratedDRHit and DRHitAndDriftDistance.
//
// Revision 1.6  2000/10/29 02:37:05  wsun
// Correct fitting weight along with drift time and distance.
//
// Revision 1.5  2000/10/11 05:53:14  wsun
// Corrected previous log comment.
//
// Revision 1.4  2000/10/11 05:25:51  wsun
// Updated to new TrackFitDRHitLink.
//
// Revision 1.3  2000/08/16 20:00:56  wsun
// Updated to new TrackFitHitLink and TRTrackFitQuality.
//
// Revision 1.2  2000/06/09 23:53:27  wsun
// * Added applyTrackCorrections().
// * TrackFitHitLinks stored temporarily by HICalibratedHits to avoid new-ing
//   them every time the track is copied during fitting.  The TrackFitHitLinks
//   are copied to the helix after fitting is done.  Saves A LOT of time.
// * Renamed makeNewLinkData() -->
//   instantiateLinkData() and makeNewCopyOfLinkData().
//
// Revision 1.1  2000/06/09 00:24:17  wsun
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
//#include "Experiment/report.h"
#include "HelixIntersection/HICalibratedDRHit.h"
#include "HelixIntersection/HIFitHelix.h"
#include "HelixIntersection/HIIntersectionSurface.h"

#include "DRHitCorrectorProd/HitCorrector.h"

#include "CLHEP/Geometry/Vector3D.h"
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

static const char* const kFacilityString = "HelixIntersection.HICalibratedDRHit" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HICalibratedDRHit.cc,v 1.10 2002/02/17 17:23:04 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HICalibratedDRHit::HICalibratedDRHit( const CalibratedDRHit* aHit,
				      const HitCorrector* aHitCorrector,
				      Meters aSignedDriftDistance,
				      const double& aAverageChargeOnTrack,
				      CalibratedTimeHit::PicoSec aDriftTime,
				      DABoolean aUseInFit )
   : HICalibratedHit(                   aUseInFit ),
     m_hit(                             aHit ),
     m_signedDriftDistance(             aSignedDriftDistance ),
     m_uncorrectedSignedDriftDistance(  aSignedDriftDistance ),
     m_driftTime(                       aDriftTime ),
     m_driftSignFlipped(                false ),
     m_vectorOfLinkData(                new STL_VECTOR( TrackFitDRHitLink ) ),
     m_hitCorrector(                    aHitCorrector ),
     m_averageChargeOnTrack(            aAverageChargeOnTrack )
{
}

HICalibratedDRHit::HICalibratedDRHit( const HICalibratedDRHit& rhs )
   : HICalibratedHit( rhs ),
     m_hit(                             rhs.m_hit ),
     m_signedDriftDistance(             rhs.m_signedDriftDistance ),
     m_uncorrectedSignedDriftDistance(  rhs.m_signedDriftDistance ),
     m_driftTime(                       rhs.m_driftTime ),
     m_driftSignFlipped(                rhs.m_driftSignFlipped ),
     m_vectorOfLinkData(                new STL_VECTOR( TrackFitDRHitLink ) ),
     m_hitCorrector(                    rhs.m_hitCorrector ),
     m_averageChargeOnTrack(            rhs.m_averageChargeOnTrack )
{
   *m_vectorOfLinkData = *( rhs.m_vectorOfLinkData ) ;
}

HICalibratedDRHit::~HICalibratedDRHit()
{
   delete m_vectorOfLinkData ;
}

//
// assignment operators
//
const HICalibratedDRHit& HICalibratedDRHit::operator=(
   const HICalibratedDRHit& rhs )
{
   if( this != &rhs )
   {
      HICalibratedHit::operator=( rhs ) ;
      m_hit                            = rhs.m_hit ;
      m_signedDriftDistance            = rhs.m_signedDriftDistance ;
      m_uncorrectedSignedDriftDistance = rhs.m_uncorrectedSignedDriftDistance ;
      m_driftTime                      = rhs.m_driftTime ;
      m_driftSignFlipped               = rhs.m_driftSignFlipped ;
      m_hitCorrector                   = rhs.m_hitCorrector ;
      m_averageChargeOnTrack           = rhs.m_averageChargeOnTrack ;

      if( m_vectorOfLinkData == 0 )
      {
	 m_vectorOfLinkData = new STL_VECTOR( TrackFitDRHitLink ) ;
      }
      *m_vectorOfLinkData   = *( rhs.m_vectorOfLinkData ) ;
   }
   return *this;
}

//
// member functions
//
void
HICalibratedDRHit::clear()
{
   HICalibratedHit::clear() ;
   m_vectorOfLinkData->clear() ;
}

DABoolean
HICalibratedDRHit::reverseMeasurementSign()
{
   // Don't flip the drift sign unless that improves the residual.
   if( m_residualsSmoothed &&
       fabs( m_smoothedResidual ) > fabs( m_signedDriftDistance ) &&
       m_smoothedResidual * m_signedDriftDistance > 0. )
   {
      m_driftSignFlipped = true ;
      m_signedDriftDistance *= -1. ;
      m_uncorrectedSignedDriftDistance *= -1. ;
      return true ;
   }
   else
   {
      return false ;
   }
}

// Apply signal propagation and time of flight corrections for DR anodes.
void
HICalibratedDRHit::applyTrackCorrections( const HIHelix& aHelix )
{
   if( m_hitCorrector == 0 )
   {
      return ;
   }

   Meters correctedSignedDist = 0. ;
   Meters correctedResolution = 0. ;
   m_hitCorrector->correctDriftTimeAndDist(
      aHelix,                            // input
      *m_hit,                            // input
      aHelix.mass(),                     // input
      m_uncorrectedSignedDriftDistance,  // input
      m_averageChargeOnTrack,            // input
      m_driftTime,                       // output
      correctedSignedDist,               // output
      correctedResolution ) ;            // output

   m_signedDriftDistance = correctedSignedDist ;

   m_fittingWeightCorrected = true ;
   m_correctedFittingWeight = 1./ sqr( correctedResolution ) ;
}

//
// const member functions
//

TrackFitHitLink*
HICalibratedDRHit::instantiateLinkData(
   const HIFitHelix& aHelix,
   const HIIntersectionSurface& aSurface,
   Meters aResidual,
   Meters aResidualError,
   Meters aDistPred,
   Meters aDistPredError )
{
   // Calculate sin of the entrance angle.
   HepVector3D mom = aHelix.momentum() ;
   GeV absmom = mom.mag() ;

   HepNormal3D norm ;
   aSurface.normal( aHelix, norm ) ;

   double sinTrackToNormal =
      ( norm.x() * mom.y() - norm.y() * mom.x() ) /
      ( norm.perp() * mom.perp() ) ;

   double weight = m_correctedFittingWeight ;
   if( !m_fittingWeightCorrected && m_hit != 0 )
   {
      weight = m_hit->weight() ;
   }

   // Make a new link data for this hit.
   m_vectorOfLinkData->push_back( TrackFitDRHitLink(
      useInFit(),
      aResidual,
      aResidualError,
      absmom,
      aHelix.referencePoint(),
      aDistPred,
      aDistPredError,
      sinTrackToNormal,
      m_driftTime,
      weight,
      m_signedDriftDistance ) ) ;

   return &*( m_vectorOfLinkData->end() - 1 ) ;
}

TrackFitHitLink*
HICalibratedDRHit::makeNewCopyOfLinkData( const TrackFitHitLink* aLinkData )
{
   if( aLinkData == 0 )
   {
      return 0 ;
   }
   else
   {
      return new TrackFitDRHitLink(
	 *( ( const TrackFitDRHitLink* ) aLinkData ) ) ;
   }
}

//
// static member functions
//
