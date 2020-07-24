// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIFitHelix
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Jul  2 01:16:48 EDT 1999
// $Id: HIFitHelix.cc,v 1.9 2000/08/16 20:00:57 wsun Exp $
//
// Revision history
//
// $Log: HIFitHelix.cc,v $
// Revision 1.9  2000/08/16 20:00:57  wsun
// Updated to new TrackFitHitLink and TRTrackFitQuality.
//
// Revision 1.8  2000/06/09 00:28:29  wsun
// Updated to new HitAndLinkData.
//
// Revision 1.7  2000/02/03 07:23:19  wsun
// reset() also clears te HICalibratedHits and chisq.
//
// Revision 1.6  1999/12/15 20:49:34  wsun
// Sped up copy ctors.
//
// Revision 1.5  1999/08/30 21:36:43  wsun
// Added numberOfHitsInFit(), which ndof() now uses.
//
// Revision 1.4  1999/08/27 20:24:03  wsun
// Cosmetic changes.
//
// Revision 1.3  1999/07/07 16:41:11  wsun
// Added mechanism for applying constraints in track fitters.
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

static const char* const kFacilityString = "HelixIntersection.HIFitHelix" ;

const string HIFitHelix::kHelixType = "HIFitHelix" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIFitHelix.cc,v 1.9 2000/08/16 20:00:57 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIFitHelix::HIFitHelix()
   : m_fitFailed(         false ),
     m_chisq(             0. ),
     m_hitsOnTrack(       new STL_VECTOR( HitAndLinkData ) ),
     m_numberFreeParams(  kZ0 ),
     m_numberConstraints( 0 ),
     m_nHitsDropped(      0 ),
     m_nHitsFlipped(      0 )
{
}

HIFitHelix::HIFitHelix( const KTHelix& aHelix,
			GeV aMass,
			DABoolean reportFitResultsAtInitRefPt,
			Meters anArcLength,
			Identifier id )
   : HIHelix( aHelix, aMass, reportFitResultsAtInitRefPt, anArcLength, id ),
     m_fitFailed(         false ),
     m_chisq(             0. ),
     m_hitsOnTrack(       new STL_VECTOR( HitAndLinkData ) ),
     m_numberFreeParams(  kZ0 ),
     m_numberConstraints( 0 ),
     m_nHitsDropped(      0 ),
     m_nHitsFlipped(      0 )
{
}

HIFitHelix::HIFitHelix( const HIFitHelix& rhs )
   : HIHelix( rhs ),
     m_fitFailed(         rhs.m_fitFailed ),
     m_chisq(             rhs.m_chisq ),
     m_numberFreeParams(  rhs.m_numberFreeParams ),
     m_numberConstraints( rhs.m_numberConstraints ),
     m_nHitsDropped(      rhs.m_nHitsDropped ),
     m_nHitsFlipped(      rhs.m_nHitsFlipped )
{
   m_hitsOnTrack = new STL_VECTOR( HitAndLinkData ) ;
   *m_hitsOnTrack = *( rhs.m_hitsOnTrack ) ;
}

HIFitHelix::~HIFitHelix()
{
   delete m_hitsOnTrack ;
}

//
// assignment operators
//
const HIFitHelix& HIFitHelix::operator=( const HIFitHelix& rhs )
{
  if( this != &rhs ) {
     HIHelix::operator=( rhs );
     m_fitFailed         = rhs.m_fitFailed ;
     m_chisq             = rhs.m_chisq ;
     m_numberFreeParams  = rhs.m_numberFreeParams ;
     m_numberConstraints = rhs.m_numberConstraints ;
     m_nHitsDropped      = rhs.m_nHitsDropped ;
     m_nHitsFlipped      = rhs.m_nHitsFlipped ;

     if( m_hitsOnTrack == 0 )
     {
        m_hitsOnTrack = new STL_VECTOR( HitAndLinkData ) ;
     }
     *m_hitsOnTrack = *( rhs.m_hitsOnTrack ) ;
  }
  return *this;
}

//
// member functions
//

void
HIFitHelix::addHitOnTrack( const HitAndLinkData& aHit )
{
   m_hitsOnTrack->push_back( aHit ) ;
}

void
HIFitHelix::setHitsOnTrack( const STL_VECTOR( HitAndLinkData )& aHits )
{
   *m_hitsOnTrack = aHits ;
}

void
HIFitHelix::applyConstraintResults( const HIFitConstraint::Results& aResults )
{
   if( !aResults.fitSuccessful() ) return ;

   setTrackParameters( trackParameters() + aResults.deltaTrackParameters() ) ;
   setErrorMatrix( aResults.trackParameterErrorMatrix() ) ;
   m_chisq            += aResults.chisq() ;
   m_numberConstraints = aResults.ndof() ;
   m_unknownParams     = aResults.unknownParameters() ;
   m_unknownError      = aResults.unknownParameterErrorMatrix() ;
   m_covarUnknownHelix = aResults.unknownTrackCovariance() ;
}

void
HIFitHelix::resetHitsOnTrack()
{
   m_hitsOnTrack->clear() ;
   m_nHitsDropped = 0 ;
   m_nHitsFlipped = 0 ;
}

void
HIFitHelix::reset()
{
   HIHelix::reset() ;

   // Allow all the hits on track to be reused in another fit.
   STL_VECTOR( HitAndLinkData )::iterator hitItr = m_hitsOnTrack->begin() ;
   STL_VECTOR( HitAndLinkData )::iterator hitEnd = m_hitsOnTrack->end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      hitItr->hiCalibratedHit()->setInfoOnTrack( false ) ;
   }

   resetHitsOnTrack() ;
   m_chisq = 0. ;
   m_fitFailed = false ;
}

//
// const member functions
//

int
HIFitHelix::numberOfHitsInFit() const
{
   STL_VECTOR( HitAndLinkData )::iterator hitItr = m_hitsOnTrack->begin() ;
   STL_VECTOR( HitAndLinkData )::iterator hitEnd = m_hitsOnTrack->end() ;

   int nUsedHits = 0 ;
   for( ; hitItr != hitEnd ; ++hitItr )
   {
      if( ( *hitItr ).linkData()->used() )
      {
	 ++nUsedHits ;
      }
   }

   return nUsedHits ;
}

int
HIFitHelix::ndof() const
{
   return numberOfHitsInFit() - m_numberFreeParams + m_numberConstraints ;
}

//
// static member functions
//
