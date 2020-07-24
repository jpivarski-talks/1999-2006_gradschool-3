// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICalibratedSVHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Jun  8 17:08:35 EDT 2000
// $Id: HICalibratedSVHit.cc,v 1.3 2000/08/16 20:00:56 wsun Exp $
//
// Revision history
//
// $Log: HICalibratedSVHit.cc,v $
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
// Revision 1.1  2000/06/09 00:24:18  wsun
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
#include "HelixIntersection/HICalibratedSVHit.h"
#include "HelixIntersection/HIFitHelix.h"

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

static const char* const kFacilityString = "HelixIntersection.HICalibratedSVHit" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HICalibratedSVHit.cc,v 1.3 2000/08/16 20:00:56 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HICalibratedSVHit::HICalibratedSVHit( const CalibratedSVHit* aHit,
				      const HepVector3D& aMeasurementDirection,
				      DABoolean aUseInFit )
   : HICalibratedHit( aUseInFit ),
     m_hit(                  aHit ),
     m_measurementDirection( aMeasurementDirection ),
     m_vectorOfSVRLinkData(  new STL_VECTOR( TrackFitSVRHitLink ) ),
     m_vectorOfSVZLinkData(  new STL_VECTOR( TrackFitSVZHitLink ) )
{
}

HICalibratedSVHit::HICalibratedSVHit( const HICalibratedSVHit& rhs )
   : HICalibratedHit( rhs ),
     m_hit(                  rhs.m_hit ),
     m_measurementDirection( rhs.m_measurementDirection ),
     m_vectorOfSVRLinkData(  new STL_VECTOR( TrackFitSVRHitLink ) ),
     m_vectorOfSVZLinkData(  new STL_VECTOR( TrackFitSVZHitLink ) )
{
   *m_vectorOfSVRLinkData = *( rhs.m_vectorOfSVRLinkData ) ;
   *m_vectorOfSVZLinkData = *( rhs.m_vectorOfSVZLinkData ) ;
}

HICalibratedSVHit::~HICalibratedSVHit()
{
   delete m_vectorOfSVRLinkData ;
   delete m_vectorOfSVZLinkData ;
}

//
// assignment operators
//
const HICalibratedSVHit& HICalibratedSVHit::operator=( const HICalibratedSVHit& rhs )
{
   if( this != &rhs )
   {
      HICalibratedHit::operator=( rhs ) ;
      m_hit = rhs.m_hit ;
      m_measurementDirection = rhs.m_measurementDirection ;

      if( m_vectorOfSVRLinkData == 0 )
      {
	 m_vectorOfSVRLinkData = new STL_VECTOR( TrackFitSVRHitLink ) ;
      }
      *m_vectorOfSVRLinkData   = *( rhs.m_vectorOfSVRLinkData ) ;

      if( m_vectorOfSVZLinkData == 0 )
      {
	 m_vectorOfSVZLinkData = new STL_VECTOR( TrackFitSVZHitLink ) ;
      }
      *m_vectorOfSVZLinkData   = *( rhs.m_vectorOfSVZLinkData ) ;
   }
   return *this;
}

//
// member functions
//
void
HICalibratedSVHit::clear()
{
   HICalibratedHit::clear() ;
   m_vectorOfSVRLinkData->clear() ;
   m_vectorOfSVZLinkData->clear() ;
}

//
// const member functions
//

TrackFitHitLink*
HICalibratedSVHit::instantiateLinkData(
   const HIFitHelix& aHelix,
   const HIIntersectionSurface& aSurface,
   Meters aResidual,
   Meters aResidualError,
   Meters aDistPred,
   Meters aDistPredError )
{
   if( m_hit->deviceType() == CalibratedHit::SVR )
   {
      m_vectorOfSVRLinkData->push_back( TrackFitSVRHitLink(
	 useInFit(),
	 aResidual,
	 aResidualError,
//	 aHelix.curvature(),
	 aHelix.momentum().mag() ) ) ;

      return &*( m_vectorOfSVRLinkData->end() - 1 ) ;
   }
   else if( m_hit->deviceType() == CalibratedHit::SVZ )
   {
      m_vectorOfSVZLinkData->push_back( TrackFitSVZHitLink(
	 useInFit(),
	 aResidual,
	 aResidualError,
//	 aHelix.curvature(),
	 aHelix.momentum().mag() ) ) ;

      return &*( m_vectorOfSVZLinkData->end() - 1 ) ;
   }
}

TrackFitHitLink*
HICalibratedSVHit::makeNewCopyOfLinkData( const TrackFitHitLink* aLinkData )
{
   if( aLinkData == 0 )
   {
      return 0 ;
   }
   else if( m_hit->deviceType() == CalibratedHit::SVR )
   {
      return new TrackFitSVRHitLink(
	 *( ( const TrackFitSVRHitLink* ) aLinkData ) ) ;
   }
   else if( m_hit->deviceType() == CalibratedHit::SVZ )
   {
      return new TrackFitSVZHitLink(
	 *( ( const TrackFitSVZHitLink* ) aLinkData ) ) ;
   }
}

//
// static member functions
//
