// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICalibratedCathodeHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Jun  8 17:08:35 EDT 2000
// $Id: HICalibratedCathodeHit.cc,v 1.3 2000/08/16 20:00:55 wsun Exp $
//
// Revision history
//
// $Log: HICalibratedCathodeHit.cc,v $
// Revision 1.3  2000/08/16 20:00:55  wsun
// Updated to new TrackFitHitLink and TRTrackFitQuality.
//
// Revision 1.2  2000/06/09 23:53:26  wsun
// * Added applyTrackCorrections().
// * TrackFitHitLinks stored temporarily by HICalibratedHits to avoid new-ing
//   them every time the track is copied during fitting.  The TrackFitHitLinks
//   are copied to the helix after fitting is done.  Saves A LOT of time.
// * Renamed makeNewLinkData() -->
//   instantiateLinkData() and makeNewCopyOfLinkData().
//
// Revision 1.1  2000/06/09 00:24:20  wsun
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
#include "HelixIntersection/HICalibratedCathodeHit.h"
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

static const char* const kFacilityString = "HelixIntersection.HICalibratedCathodeHit" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HICalibratedCathodeHit.cc,v 1.3 2000/08/16 20:00:55 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HICalibratedCathodeHit::HICalibratedCathodeHit(
   const CalibratedCathodeHit* aHit,
   DABoolean aUseInFit )
   : HICalibratedHit( aUseInFit ),
     m_hit(              aHit ),
     m_vectorOfLinkData( new STL_VECTOR( TrackFitCathodeHitLink ) )
{
}

HICalibratedCathodeHit::HICalibratedCathodeHit(
   const HICalibratedCathodeHit& rhs )
   : HICalibratedHit( rhs ),
     m_hit(              rhs.m_hit ),
     m_vectorOfLinkData( new STL_VECTOR( TrackFitCathodeHitLink ) )
{
   *m_vectorOfLinkData = *( rhs.m_vectorOfLinkData ) ;
}

HICalibratedCathodeHit::~HICalibratedCathodeHit()
{
   delete m_vectorOfLinkData ;
}

//
// assignment operators
//
const HICalibratedCathodeHit& HICalibratedCathodeHit::operator=(
   const HICalibratedCathodeHit& rhs )
{
   if( this != &rhs )
   {
      HICalibratedHit::operator=( rhs ) ;
      m_hit = rhs.m_hit ;

      if( m_vectorOfLinkData == 0 )
      {
	 m_vectorOfLinkData = new STL_VECTOR( TrackFitCathodeHitLink ) ;
      }
      *m_vectorOfLinkData   = *( rhs.m_vectorOfLinkData ) ;
   }
   return *this;
}

//
// member functions
//
void
HICalibratedCathodeHit::clear()
{
   HICalibratedHit::clear() ;
   m_vectorOfLinkData->clear() ;
}

//
// const member functions
//

TrackFitHitLink*
HICalibratedCathodeHit::instantiateLinkData(
   const HIFitHelix& aHelix,
   const HIIntersectionSurface& aSurface,
   Meters aResidual,
   Meters aResidualError,
   Meters aDistPred,
   Meters aDistPredError )
{
   m_vectorOfLinkData->push_back( TrackFitCathodeHitLink(
      useInFit(),
      aResidual,
      aResidualError,
//      aHelix.curvature(),
      aHelix.momentum().mag() ) ) ;

   return &*( m_vectorOfLinkData->end() - 1 ) ;
}

TrackFitHitLink*
HICalibratedCathodeHit::makeNewCopyOfLinkData(const TrackFitHitLink* aLinkData)
{
   if( aLinkData == 0 )
   {
      return 0 ;
   }
   else
   {
      return new TrackFitCathodeHitLink(
	 *( ( const TrackFitCathodeHitLink* ) aLinkData ) ) ;
   }
}

//
// static member functions
//
