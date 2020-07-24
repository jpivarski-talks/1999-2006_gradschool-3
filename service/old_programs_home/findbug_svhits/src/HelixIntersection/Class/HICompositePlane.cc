// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICompositePlane
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Nov  8 22:40:26 EST 1999
// $Id: HICompositePlane.cc,v 1.7 2001/03/26 06:55:16 wsun Exp $
//
// Revision history
//
// $Log: HICompositePlane.cc,v $
// Revision 1.7  2001/03/26 06:55:16  wsun
// Move to radius in either direction.
//
// Revision 1.6  2001/02/22 02:23:41  wsun
// Avoid KTHelix::moveToPlane() by progapating helix as straight line.
//
// Revision 1.5  2000/06/29 17:28:21  wsun
// Added option for not checking boundary conditions.
//
// Revision 1.4  2000/02/03 07:21:45  wsun
// Cleaned up code.
//
// Revision 1.3  2000/02/03 07:19:39  wsun
// Widened search window in attemptIntersection().
//
// Revision 1.2  1999/12/15 20:40:11  wsun
// Use HIIntersectionPlane::rphiHalfWidth() for surface culling.
//
// Revision 1.1  1999/11/21 22:23:01  wsun
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
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HICompositePlane.h"

#include "HelixIntersection/HIIntersectionPlane.h"
#include "HelixIntersection/HISiLadderPlane.h"
#include "HelixIntersection/HISiSensorPlane.h"

#include "SiGeom/SiGeomDescription.h"

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
"HelixIntersection.HICompositePlane" ;

const string HICompositePlane::kSurfaceType = "HICompositePlane" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HICompositePlane.cc,v 1.7 2001/03/26 06:55:16 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HICompositePlane::HICompositePlane( DABoolean aCheckRphiBoundaries )
   : m_checkRphiBoundaries( aCheckRphiBoundaries )
{
}

// HICompositePlane::HICompositePlane( const HICompositePlane& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICompositePlane::~HICompositePlane()
{
}

//
// assignment operators
//
// const HICompositePlane& HICompositePlane::operator=( const HICompositePlane& rhs )
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

void
HICompositePlane::addChild( HIIntersectionSurface* aSurface )
{
   const string& type( aSurface->surfaceType() ) ;

   if( type == HIIntersectionPlane::kSurfaceType ||
       type == HISiLadderPlane::kSurfaceType ||
       type == HISiSensorPlane::kSurfaceType )
   {
      HICompositeSurface::addChild( aSurface ) ;
   }
   else
   {
      report( WARNING, kFacilityString ) << "Trying to add non-plane child."
					 << endl ;
   }
}

void
HICompositePlane::addChildren( const HICompositeSurface& aCompositeSurface )
{
   if( aCompositeSurface.surfaceType() == kSurfaceType )
   {
      HICompositeSurface::addChildren( aCompositeSurface ) ;
   }
   else
   {
      report( WARNING, kFacilityString )
	 << "Trying to add children from a non-HICompositePlane." << endl ;
   }
}


//
// const member functions
//

DABoolean
HICompositePlane::attemptHelixPropagation(
   const HIHelix& helixAtCompositeRadius,
   const HepPoint3D& positionAtCompositeRadius,
   HIIntersectionSurface* aSurface ) const
{
   if( !m_checkRphiBoundaries )
   {
      // HICompositeSurface::propagateHelix() will loop over all surfaces
      // in the composite.
      return true ;
   }

   // Check if the surface is in the same general phi region as the track.
   HIIntersectionPlane* plane = ( HIIntersectionPlane* ) aSurface ;
   HepPoint3D planePoint = plane->point() ;

   Meters window = plane->rphiHalfWidth() ;
   if( window < SiGeomDescription::kSensorWidth / 4. )
   {
      window = SiGeomDescription::kSensorWidth / 4. ;
   }

   // Empirical scale factor for Si Layer 3.  The layer radius corresponds
   // to the inner radius of the crenelation (sensor 1).  But the diamond
   // supports are on the outside of the layer.  So, the outer edge surfaces
   // of the vees may not be considered unless the window is widened.
   window *= 1.25 ;

   if( sqr( positionAtCompositeRadius.x() - planePoint.x() ) +
       sqr( positionAtCompositeRadius.y() - planePoint.y() ) >
       sqr( window ) )
   {
      return false ;
   }

   return true ;
}

DABoolean
HICompositePlane::propagateHelix( HIHelix& helixToPropagate,
				  KTMoveControl::MoveDirection aDirection )
{
   // No need to save order of surfaces because things might change after
   // performing the operation -- would need to loop through all surfaces
   // again anyway.

   Meters bestArcLength = 999999. ;
   HIIntersectionSurface* bestSurface = 0 ;
   Meters nextBestArcLength = 999999. ;
   HIIntersectionSurface* nextBestSurface = 0 ;

   // Don't copy error matrix for temporary helix -- saves time.
   HIHelix helixAtCompositeRadius( helixToPropagate, false ) ;

   if( !m_helixAtSurface )
   {
      if( helixAtCompositeRadius.moveToRadius(
	 radius(),
	 KTMoveControl::kDirectionEither ) !=
	  KTMoveControl::kMoveOK )
      {
	 return false ;
      }
   }

   HepPoint3D positionAtCompositeRadius = helixAtCompositeRadius.position() ;

   DABoolean surfacesInRangeFilled = m_surfacesInRange->size() != 0 ;

   STL_VECTOR( HIIntersectionSurface* )::iterator iter ;
   STL_VECTOR( HIIntersectionSurface* )::iterator endSurf ;

   if( surfacesInRangeFilled )
   {
      iter = m_surfacesInRange->begin() ;
      endSurf = m_surfacesInRange->end() ;
   }
   else
   {
      iter = m_surfaces->begin() ;
      endSurf = m_surfaces->end() ;
   }

   for( ; iter != endSurf ; ++iter )
   {
      // First check if the surface is close enough to attempt propagtion.
      if( !surfacesInRangeFilled )
      {
	 if( attemptHelixPropagation( helixAtCompositeRadius,
				      positionAtCompositeRadius,
				      *iter ) )
	 {
	    m_surfacesInRange->push_back( *iter ) ;
	 }
	 else
	 {
	    continue ;
	 }

      }

      // Don't allow intersections with surfaces already intersected.
      DABoolean attemptProp = true ;

      STL_VECTOR( HIIntersectionSurface* )::iterator intersectedItr =
	 m_intersectedSurfaces->begin() ;
      STL_VECTOR( HIIntersectionSurface* )::iterator intersectedEnd =
	 m_intersectedSurfaces->end() ;
      for( ; intersectedItr != intersectedEnd ; ++intersectedItr )
      {
	 if( *iter == *intersectedItr )
	 {
	    attemptProp = false ;
	    break ;
	 }
      }

      if( !attemptProp )
      {
	 continue ;
      }

      // Don't copy error matrix for temporary helix -- saves time.
      HIHelix tmpHelix( helixToPropagate, false ) ;

      DABoolean success = true ;
      HepPoint3D pos = tmpHelix.position() ;
      HepNormal3D norm ;
      ( *iter )->normal( pos, norm ) ;
      Meters tmpArcLength =
	 norm.dot( (( HIIntersectionPlane* )( *iter ))->point() - pos ) /
	 norm.dot( tmpHelix.momentumUnit() ) ;
      if( ( aDirection == KTMoveControl::kDirectionForward &&
	   tmpArcLength < 0.) ||
	  ( aDirection == KTMoveControl::kDirectionBackward &&
	    tmpArcLength > 0. ) )
      {
	 success = false ;
      }
      else
      {
	 tmpHelix.moveByArcLength( tmpArcLength ) ;

	 pos = tmpHelix.position() ;
	 Meters tmpArcLength2 =
	    norm.dot( (( HIIntersectionPlane* )( *iter ))->point() - pos ) /
	    norm.dot( tmpHelix.momentumUnit() ) ;

	 if( fabs( tmpArcLength2 ) > 1.e-6 )
	 {
	    tmpHelix.moveByArcLength( tmpArcLength2 ) ;
	    tmpArcLength += tmpArcLength2 ;

	    pos = tmpHelix.position() ;
	    tmpArcLength2 =
	       norm.dot( (( HIIntersectionPlane* )( *iter ))->point() - pos ) /
	       norm.dot( tmpHelix.momentumUnit() ) ;

	    if( fabs( tmpArcLength2 ) > 1.e-6 )
	    {
	       tmpHelix.moveByArcLength( tmpArcLength2 ) ;
	       tmpArcLength += tmpArcLength2 ;
	    }
	 }
      }

      if( success &&
	  fabs( tmpArcLength ) < fabs( bestArcLength ) &&
	  fabs( tmpArcLength ) > 1.e-10 &&
	  ( *iter )->conditionSatisfied( tmpHelix ) )
      {
	 nextBestArcLength = bestArcLength ;
	 nextBestSurface = bestSurface ;

	 bestArcLength = tmpArcLength ;
	 bestSurface = *iter ;
      }
   }

   // Using the straight line approx can be problematic at corners.
   // So, perform full swim to plane for the two best surfaces but only if
   // their tmpArcLengths are within 2*deltaX, 2 being a generous fudge
   // factor.  The error introduced by the straight line approx is
   // deltaX = s * sin(deltaPhi) ~= s * deltaPhi = curvature * s^2

   if( bestSurface != 0 )
   {
      if( nextBestSurface != 0 &&
	  fabs( bestArcLength - nextBestArcLength ) <
	  fabs( bestArcLength*bestArcLength*helixToPropagate.curvature() ) )
      {
	 HIHelix bestHelix( helixToPropagate, false ) ;
	 DABoolean bestOK =
	    bestSurface->propagateHelix( bestHelix, aDirection ) ;
	 if( bestOK )
	    bestOK = bestSurface->conditionSatisfied( bestHelix ) ;
	 Meters bestArcLength2 = bestOK ? bestHelix.lastArcLength() : 999999. ;

	 HIHelix nextBestHelix( helixToPropagate, false ) ;
	 DABoolean nextBestOK =
	    nextBestSurface->propagateHelix( nextBestHelix, aDirection ) ;
	 if( nextBestOK )
	    nextBestOK = nextBestSurface->conditionSatisfied( nextBestHelix ) ;
	 Meters nextBestArcLength2 = nextBestOK ?
	    nextBestHelix.lastArcLength() : 999999. ;

	 if( bestOK && nextBestOK )
	 {
	    if( fabs( nextBestArcLength2 ) < fabs( bestArcLength2 ) )
	    {
	       bestSurface = nextBestSurface ;
	       bestArcLength = nextBestArcLength2 ;
	    }
	    else
	    {
	       bestArcLength = bestArcLength2 ;
	    }
	 }
	 else if( bestOK )
	 {
	    bestArcLength = bestArcLength2 ;
	 }
	 else if( nextBestOK )
	 {
	    bestSurface = nextBestSurface ;
	    bestArcLength = nextBestArcLength2 ;
	 }
	 else
	 {
	    bestSurface = 0 ;
	    bestArcLength = 999999. ;
	 }
      }
      else
      {
	 HIHelix tmpHelix2( helixToPropagate, false ) ;

	 if( !bestSurface->propagateHelix( tmpHelix2, aDirection ) ||
	     !bestSurface->conditionSatisfied( tmpHelix2 ) )
	 {
	    bestSurface = 0 ;
	    bestArcLength = 999999. ;
	 }
      }
   }

   if( printDiagnostics() )
   {
      cout << "best composite arc length " << bestArcLength << endl ;
   }

   m_lastHelixAtSurface = m_helixAtSurface ;

   if( bestSurface != 0 )
   {
      // Call the best surface's propagateHelix() with helixToPropagate
      // so that the correct virtual move routines are called.
      m_intersectedSurfaces->push_back( bestSurface ) ;
      setIntersectedSurface() ;
      m_helixAtSurface = true ;
      return bestSurface->propagateHelix( helixToPropagate, aDirection ) ;
   }
   else
   {
      // Clear the cache of surfaces in range.
      m_surfacesInRange->clear() ;

      // aHelixToPropagate is unchanged.
      m_helixAtSurface = false ;
      return false ;
   }
}

//
// static member functions
//
