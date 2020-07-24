// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMultiWireCylinder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Jul 25 15:16:32 EDT 1999
// $Id: HIMultiWireCylinder.cc,v 1.13 2001/03/26 07:15:20 wsun Exp $
//
// Revision history
//
// $Log: HIMultiWireCylinder.cc,v $
// Revision 1.13  2001/03/26 07:15:20  wsun
// Mods for hit order reversal.
//
// Revision 1.12  2001/01/26 21:45:05  cleo3
// fixed initialization of base class
//
// Revision 1.11  2000/12/02 01:30:27  wsun
// Added avg chg on track to HICalibratedDRHit and DRHitAndDriftDistance.
//
// Revision 1.10  2000/06/09 23:46:26  wsun
// Moved correctHitsForHelix() to HICalibratedHit.
//
// Revision 1.9  2000/06/09 00:28:49  wsun
// Added correctHitsForHelix().
//
// Revision 1.8  2000/04/06 20:00:11  wsun
// Check if track switched direction in propagateToNextWire().
//
// Revision 1.7  2000/03/02 08:25:18  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.6  2000/02/22 17:19:55  wsun
// Allow users to set m_checkDirectionSwitch.
//
// Revision 1.5  2000/02/03 07:28:37  wsun
// Added use of HICalibratedHits.
//
// Revision 1.4  1999/12/15 21:04:12  wsun
// Updated to new HIIntersectionSurface::normal(...) signature.
//
// Revision 1.3  1999/09/10 21:23:07  wsun
// Added a vector to keep track of used hits.
//
// Revision 1.2  1999/08/27 20:25:38  wsun
// Keep track of last intersected hit.
//
// Revision 1.1  1999/08/03 21:41:16  wsun
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
#include "HelixIntersection/HIMultiWireCylinder.h"
#include "HelixIntersection/HIHelix.h"

#include "DRHitCorrectorProd/HitCorrector.h"

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

static const char* const kFacilityString =
"HelixIntersection.HIMultiWireCylinder" ;

const string HIMultiWireCylinder::kSurfaceType = "HIMultiWireCylinder" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIMultiWireCylinder.cc,v 1.13 2001/03/26 07:15:20 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIMultiWireCylinder::HIMultiWireCylinder()
{
}

HIMultiWireCylinder::HIMultiWireCylinder(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   const AEWireLayerCylindrical& aWireLayer,
   int aLayerNumber,
   const DGTransform& aTransform,
   HICondition* aCondition,
   HIHelixOperation* aOperation,
   DABoolean checkDirectionSwitch,
   const STL_VECTOR( HitAndUsed )* aVector )
   : HISingleWireCylinder( aInnerMaterial,
			   aOuterMaterial,
			   aWireLayer,
			   aLayerNumber,
			   aTransform,
			   aCondition,
			   aOperation,
			   static_cast<const CalibratedDRHit*>(0),
			   0.,      // signed drift dist
			   static_cast<const HitCorrector*>(0),
			   false ), // delay dir switch check to HIMultiWireCyl
   m_checkMultiDirectionSwitch( checkDirectionSwitch )
{
   if( aVector != 0 )
   {
      m_hits = *aVector ;
   }

   setSingleHitAndDist( 0 ) ;
}


// HIMultiWireCylinder::HIMultiWireCylinder( const HIMultiWireCylinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIMultiWireCylinder::~HIMultiWireCylinder()
{
}

//
// assignment operators
//
// const HIMultiWireCylinder& HIMultiWireCylinder::operator=( const HIMultiWireCylinder& rhs )
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

DABoolean
HIMultiWireCylinder::propagateHelix(
   HIHelix& helixToPropagate,
   KTMoveControl::MoveDirection aDirection )
{
   // Because the helix is moved in multiple steps, use a temporary KTHelix
   // to find the total arc length to move.  This way, the HIHelix's last arc
   // length (and transport helix, if applicable) reflects a single transport.
   HIHelix testHelix( helixToPropagate, false ) ;

   // First propagate to radius of layer.
   if( !HIIntersectionCylinder::propagateHelix( testHelix, aDirection ) )
   {
      return false ;
   }

   // Propagate to the first hit in the list for which momentum.normal
   // does not switch sign.  If the hits that satisfy this criterion,
   // have all been used, then the helix has already been at this
   // intersection, so propagate to the next intersection with this radius.
   // Otherwise, the helix remains at the layer radius -- no error is returned.
   HIHelix helixAtRadius( testHelix, false ) ;

   HitFound status = propagateToNextWire( testHelix, true ) ;

   Meters arcToMove = 0. ;
   if( status == kUnusedHitFound )
   {
      arcToMove = testHelix.lastArcLength() ;
   }
   else if( status == kNoHitFound )
   {
      m_usedHits.push_back( 0 ) ;
      arcToMove = helixAtRadius.lastArcLength() ;
   }
   else
   {
      // Move to the next intersection with this radius.
      testHelix = helixAtRadius ;
      Meters savedArcLength = helixAtRadius.lastArcLength() ;

      if( !HIIntersectionCylinder::propagateHelix( testHelix, aDirection ) )
      {
         return false ;
      }

      savedArcLength += testHelix.lastArcLength() ;
      testHelix.setLastArcLength( savedArcLength );

      if( propagateToNextWire( testHelix, false ) == kUnusedHitFound )
      {
	 arcToMove = testHelix.lastArcLength() ;
      }
      else
      {
	 m_usedHits.push_back( 0 ) ;
	 arcToMove = savedArcLength ;
      }
   }

   return
      helixToPropagate.moveByArcLength( arcToMove ) == KTMoveControl::kMoveOK ;
}

// If return value is not kUnusedHitFound, the helix is NOT reverted to its
// original state.
HIMultiWireCylinder::HitFound
HIMultiWireCylinder::propagateToNextWire( HIHelix& aHelix,
					  DABoolean aTestUsedHits )
{
   HIHelix savedHelix = aHelix ;
   Meters lastArcLength = aHelix.lastArcLength() ;
   HepNormal3D norm ;
   normal( aHelix, norm ) ;
   double momDotNormalOrig = aHelix.momentumUnit().dot( norm ) ;
   DABoolean usedHitFound = false ;

   if( !m_useReverseHitOrder )
   {
      STL_VECTOR( HitAndUsed )::iterator hitItr = m_hits.begin() ;
      STL_VECTOR( HitAndUsed )::iterator hitEnd = m_hits.end() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 if( !aTestUsedHits && hitItr->second )  // if hit is used
	 {
	    continue ;
	 }

	 aHelix = savedHelix ;
	 setSingleHitAndDist( &*hitItr ) ;

	 if( !HISingleWireCylinder::propagateHelix(
	    aHelix,
	    KTMoveControl::kDirectionEither ) ||
	     hiCalibratedHit() == 0 )  // means track switched dir
	 {
	    continue ;
	 }

	 normal( aHelix, norm ) ;
	 double momDotNormal = aHelix.momentumUnit().dot( norm ) ;

	 if( !m_checkMultiDirectionSwitch ||
	     momDotNormal * momDotNormalOrig > 0.)
	 {
	    if( hitItr->second )  // if hit is used
	    {
	       usedHitFound = true ;
	    }
	    else
	    {
	       m_usedHits.push_back( &*hitItr ) ;
	       hitItr->second = true ;  // hit is used
	       aHelix.setLastArcLength( lastArcLength +
					aHelix.lastArcLength() ) ;
	       return kUnusedHitFound ;
	    }
	 }
      }
   }
   else
   {
      STL_VECTOR( HitAndUsed )::reverse_iterator hitItr = m_hits.rbegin() ;
      STL_VECTOR( HitAndUsed )::reverse_iterator hitEnd = m_hits.rend() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 if( !aTestUsedHits && ( *hitItr ).second )  // if hit is used
	 {
	    continue ;
	 }

	 aHelix = savedHelix ;
	 setSingleHitAndDist( &*hitItr ) ;

	 if( !HISingleWireCylinder::propagateHelix(
	    aHelix,
	    KTMoveControl::kDirectionEither ) ||
	     hiCalibratedHit() == 0 )  // means track switched dir
	 {
	    continue ;
	 }

	 normal( aHelix, norm ) ;
	 double momDotNormal = aHelix.momentumUnit().dot( norm ) ;

	 if( !m_checkMultiDirectionSwitch ||
	     momDotNormal * momDotNormalOrig > 0.)
	 {
	    if( ( *hitItr ).second )  // if hit is used
	    {
	       usedHitFound = true ;
	    }
	    else
	    {
	       m_usedHits.push_back( &*hitItr ) ;
	       ( *hitItr ).second = true ;  // hit is used
	       aHelix.setLastArcLength( lastArcLength +
					aHelix.lastArcLength() ) ;
	       return kUnusedHitFound ;
	    }
	 }
      }
   }

   if( usedHitFound )
   {
      return kUsedHitFound ;
   }
   else
   {
      return kNoHitFound ;
   }
}

DABoolean
HIMultiWireCylinder::advanceToNextCalibratedHit( HIHelix& aHelix )
{
   HIHelix testHelix( aHelix, false ) ;
   testHelix.setLastArcLength( 0. ) ;
   HitFound status = propagateToNextWire( testHelix, false ) ;

   // If status is not kUnusedHitFound, the helix is not moved.
   if( status == kUnusedHitFound )
   {
      setSingleHitAndDist() ;
      return aHelix.moveByArcLength( testHelix.lastArcLength() ) ==
	 KTMoveControl::kMoveOK ;
   }

   return false ;
}

void
HIMultiWireCylinder::setSingleHitAndDist( HitAndUsed* aHit )
{
   if( aHit != 0 )
   {
      setHICalibratedDRHit( &( aHit->first ) ) ;
   }
   else
   {
      setHICalibratedDRHit( 0 ) ;
   }
}

void
HIMultiWireCylinder::setSingleHitAndDist()
{
   if( m_usedHits.size() > 0 )
   {
      setSingleHitAndDist( *( m_usedHits.end() - 1 ) ) ;
   }
   else
   {
      setSingleHitAndDist( 0 ) ;
   }
}

void
HIMultiWireCylinder::reset()
{
   STL_VECTOR( HitAndUsed )::iterator hitItr = m_hits.begin() ;
   STL_VECTOR( HitAndUsed )::iterator hitEnd = m_hits.end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      hitItr->second = false ;  // hit is unused
   }

   m_usedHits.clear() ;
   setSingleHitAndDist() ;
}

void
HIMultiWireCylinder::resetHICalibratedHits()
{
   STL_VECTOR( HitAndUsed )::iterator hitItr = m_hits.begin() ;
   STL_VECTOR( HitAndUsed )::iterator hitEnd = m_hits.end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      hitItr->first.clear() ;
   }
}

void
HIMultiWireCylinder::revertToPreviousState()
{
   HitAndUsed* currentHit = *( m_usedHits.end() - 1 ) ;

   if( currentHit != 0 )
   {
      currentHit->second = false ;  // hit is unused
   }

   m_usedHits.pop_back() ;
}

void
HIMultiWireCylinder::addHICalibratedHitsToVector(
   STL_VECTOR( HICalibratedHit* )& aVector )
{
   STL_VECTOR( HitAndUsed )::iterator hitItr = m_hits.begin() ;
   STL_VECTOR( HitAndUsed )::iterator hitEnd = m_hits.end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      aVector.push_back( &( hitItr->first ) ) ;
   }
}

//
// const member functions
//

//
// static member functions
//
