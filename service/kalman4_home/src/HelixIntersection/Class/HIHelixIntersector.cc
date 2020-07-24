// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIHelixIntersector
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon Sep 21 17:21:51 EDT 1998
// $Id: HIHelixIntersector.cc,v 1.19 2003/01/20 23:46:09 mccann Exp $
//
// Revision history
//
// $Log: HIHelixIntersector.cc,v $
// Revision 1.19  2003/01/20 23:46:09  mccann
// oldMomDotPos had not properly been protected against having a very small, signed value; this caused some 5% of outward fits to silently drop all hits (and be identical to the inward fit, but for a very large error matrix), when in conjunction with the 'missing -z half of beampipe' bug.
//
// Revision 1.18  2002/04/27 17:05:08  cleo3
// Was deleting two arrays incorrectly. Switched to using a vectors instead.
//
// Revision 1.17  2001/03/26 07:01:27  wsun
// * Added setClosestSurfaceToCurrentByRadius()
// * Modified setClosestSurfaceToCurrent() to use arc length to each surface.
//
// Revision 1.16  2000/11/18 00:13:38  wsun
// Check surface flag before checking helix propagation.
//
// Revision 1.15  2000/02/22 00:18:19  wsun
// Don't revert to snapshot after failed transport or finishing composite.
//
// Revision 1.14  2000/02/03 07:23:57  wsun
// Added setClosestSurfaceToCurrent() and helixCurling().
//
// Revision 1.13  2000/01/11 20:32:13  wsun
// Sped up curling calc and call HIHelix::setAtSurface() after intersection.
//
// Revision 1.12  1999/11/21 22:31:37  wsun
// Sped up calculations.
//
// Revision 1.11  1999/11/03 03:43:47  wsun
// New method of checking if helix crossed the origin.
//
// Revision 1.10  1999/09/30 00:50:51  wsun
// Turned currentSurface() into a non-const function.
//
// Revision 1.9  1999/08/27 20:25:23  wsun
// New HIHelixOperation::perform() signature and added resetSurfaces().
//
// Revision 1.8  1999/08/03 21:51:12  wsun
// Modifications for handling curlers.
//
// Revision 1.7  1999/07/17 18:47:27  wsun
// Removed 'error' from warning messsages -- could prevent rotations.
//
// Revision 1.6  1999/06/24 22:24:58  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.5  1999/06/18 19:25:58  wsun
// More improvements to low momentum fitting.
//
// Revision 1.4  1999/06/02 23:05:24  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/04/28 22:57:21  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:06:26  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:22  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <algorithm>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HICompositeSurface.h"
#include "HelixIntersection/HICondition.h"
#include "HelixIntersection/HIHelixOperation.h"

#include "CLHEP/Vector/ThreeVector.h"

// STL classes
#include <vector>
#include <algorithm>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HIHelixIntersector" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIHelixIntersector::HIHelixIntersector() :
   m_printDiagnostics( false )
{
}

HIHelixIntersector::HIHelixIntersector( const HIHelixIntersector& rhs )
{
   // do actual copying here; if you implemented
   // operator= correctly, you may be able to use just say      
   *this = rhs;
}


// m_surfaceOrder gets initialized in sortSurfaces().

HIHelixIntersector::HIHelixIntersector(
   const STL_VECTOR( HIIntersectionSurface* )& vSurfaces,
   SurfaceOrder anOrder,
   HIHelix* aHelix,
   const HICondition* globalCondition,
   const HIHelixOperation* globalOperation,
   DABoolean checkCrossOrigin,
   DABoolean attemptCurling )
   : m_surfaces( vSurfaces ),
     m_helix( aHelix ),
     m_globalCondition( globalCondition ),
     m_globalOperation( globalOperation ),
     m_printDiagnostics( false ),
     m_checkCrossOrigin( checkCrossOrigin ),
     m_attemptCurling( attemptCurling )
{
   sortSurfaces( anOrder ) ;
}


HIHelixIntersector::~HIHelixIntersector()
{
}

//
// assignment operators
//
const HIHelixIntersector& HIHelixIntersector::operator=(
   const HIHelixIntersector& rhs )
{
  if( this != &rhs ) {
     m_surfaces         = rhs.m_surfaces ;
     m_currentSurface   = rhs.m_currentSurface ;
     m_surfaceOrder     = rhs.m_surfaceOrder ;
     m_helix            = rhs.m_helix ;
     m_globalCondition  = rhs.m_globalCondition ;
     m_globalOperation  = rhs.m_globalOperation ;
     m_printDiagnostics = rhs.m_printDiagnostics ;
     m_checkCrossOrigin = rhs.m_checkCrossOrigin ;
     m_attemptCurling   = rhs.m_attemptCurling ;
  }

  return *this;
}

//
// member functions
//

// template void
// __quick_sort_loop_aux< STL_VECTOR( HIIntersectionSurface* )::const_iterator,
//    RadiusLessThan > ;

// template void
// __quick_sort_loop_aux< STL_VECTOR( HIIntersectionSurface* )::const_iterator,
//    RadiusGreaterThan > ;


void
HIHelixIntersector::sortSurfaces( SurfaceOrder anOrder )
{
   if( anOrder == kOrderAsGiven )
   {
      m_surfaceOrder = anOrder ;
      m_currentSurface = m_surfaces.begin() ;
      return ;
   }

   int nsurfaces = m_surfaces.size() ;

   if( nsurfaces == 0 )
   {
      report( ERROR, kFacilityString )
	 << "No surfaces in vector." << endl ;
      assert( nsurfaces > 0 ) ;
   }

   // Map from old vector to new vector.
   //int* map = new int[ nsurfaces ] ;
   STL_VECTOR(int) map(nsurfaces, int(0));

   // The ordered surface radii.
   //Meters* radii = new Meters[ nsurfaces ] ;
   STL_VECTOR(Meters) radii(nsurfaces, Meters(0));

   // Loop over surfaces to order by radius.

   map[ 0 ] = 0 ;
   radii[ 0 ] = ( *m_surfaces.begin() )->radius() ;

   for( int inew = 1 ; inew < nsurfaces ; ++inew )
   {
      Meters newRadius = ( m_surfaces[ inew ] )->radius() ;

      int isorted = 0 ;
      for( ; isorted < inew ; ++isorted )
      {
	 if( anOrder == kIncreasingRadius )
	 {
	    if( newRadius < radii[ isorted ] )
	    {
	       break ;
	    }
	 }
	 else if( anOrder == kDecreasingRadius )
	 {
	    if( newRadius > radii[ isorted ] )
	    {
	       break ;
	    }
	 }
      }

      // isorted is now the position of the new surface.  Shift everything
      // after isorted to the right.

      for( int ishift = inew ; ishift > isorted ; --ishift )
      {
	 map[ ishift ] = map[ ishift - 1 ] ;
	 radii[ ishift ] = radii[ ishift - 1 ] ;
      }
      map[ isorted ] = inew ;
      radii[ isorted ] = newRadius ;
   }

   // Make a temporary vector of ordered surfaces.

   STL_VECTOR( HIIntersectionSurface* ) tmpSurfaces ;
   for( int index = 0 ; index < nsurfaces ; ++index )
   {
      tmpSurfaces.push_back( m_surfaces[ map[ index ] ] ) ;
   }

   m_surfaces = tmpSurfaces ;

   //delete [] map ;
   //delete [] radii ;

//    if( anOrder == kIncreasingRadius )
//    {
//       sort( m_surfaces.begin(), m_surfaces.end(), RadiusLessThan() ) ;
//    }
//    else if( anOrder == kDecreasingRadius )
//    {
//       sort( m_surfaces.begin(), m_surfaces.end(), RadiusGreaterThan() ) ;
//    }

   m_surfaceOrder = anOrder ;
   m_currentSurface = m_surfaces.begin() ;
}


DABoolean
HIHelixIntersector::setCurrentSurface( const HIIntersectionSurface* aSurface )
{
   DABoolean foundMatch( false ) ;

   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_surfaces.begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_surfaces.end() ;

   for( ; itr != end ; ++itr )
   {
      if( *itr == aSurface )
      {
	 foundMatch = true ;
	 m_currentSurface = itr ;
	 break ;
      }
   }

   return foundMatch ;
}


int
HIHelixIntersector::nextSurfaceIncrement(
   const KTMoveControl::MoveDirection aDirection ) const
{
   if( m_surfaceOrder == kOrderAsGiven )
   {
      return ( m_currentSurface == m_surfaces.end() - 1 ) ? 0 : 1 ;
   }

   int surfaceIncrement = helixCurling() * m_surfaceOrder *
      ( ( aDirection == KTMoveControl::kDirectionBackward ) ? -1 : 1 ) ;

   if( ( m_currentSurface == m_surfaces.begin() &&
	 surfaceIncrement == -1 ) ||
       ( m_currentSurface == m_surfaces.end() - 1 &&
	 surfaceIncrement == 1 ) )
   {
      return 0 ;
   }
   else
   {
      return surfaceIncrement ;
   }
}

int
HIHelixIntersector::helixCurling() const
{
   const HepPoint3D& refPt = m_helix->referencePoint() ;
   double phi0 = m_helix->phi0() ;
   return ( refPt.x() * cos( phi0 ) + refPt.y() * sin( phi0 ) ) < 0. ?
      -1 : 1 ;
}

HIHelixIntersector::IntersectionStatus
HIHelixIntersector::swimToNextIntersection(
   const KTMoveControl::MoveDirection aDirection )
{
   // Find the next surface intersected whose condition is satisfied.
   // tmpHelix is a return argument containing the helix propagated to
   // tmpSurface.  m_helix is not modified if nextSurface fails.

   // Must be deleted later.
   HIHelix* savedHelix = m_helix->newSnapshot() ;
//    HIHelix oldHelix = *m_helix ;
//    HIHelix* savedHelix = &oldHelix ;

   STL_VECTOR( HIIntersectionSurface* )::iterator savedSurface =
      m_currentSurface ;
   DABoolean transportSuccessful = false ;
   DABoolean finishedWithComposite = false ;

   Meters totalArcLength = 0. ;
   Meters oneRevolution = M_PI / fabs( m_helix->curvature() ) *
      sqrt( 1. + sqr( m_helix->cotTheta() ) ) ;

   DABoolean tryCurling = false ;
   DABoolean atEndOfSurfaces = false ;
   int lastSurfaceIncrement = 0 ;

//    // Break the loop if too many intersections attempts are made.
//    int attemptCounter = 0 ;
//    int maxAttempts = 2 * m_surfaces.size() ;

   do
   {
//       if( ++attemptCounter > maxAttempts )
//       {
// 	 m_helix->revertToSnapshot( savedHelix ) ;
// 	 delete savedHelix ;
// 	 return kNoMoreSurfaces ;
//       }

      // Don't increment the current surface if it is a composite (there
      // may be other intersections within the same composite).

      if( !( *m_currentSurface )->isComposite() || finishedWithComposite )
      {
	 if( tryCurling )
	 {
	    lastSurfaceIncrement *= -1 ;
	    m_currentSurface += lastSurfaceIncrement ;
	    finishedWithComposite = false ;
	 }
	 else
	 {
	    // Find the next layer and advance.
	    int surfaceIncrement = nextSurfaceIncrement( aDirection ) ;

	    if( surfaceIncrement == 0 )
	    {
	       if( aDirection != KTMoveControl::kDirectionEither &&
		   m_attemptCurling )
	       {
		  atEndOfSurfaces = true ;
		  tryCurling = true ;

		  // Try to curl -- if fail, then return kNoMoreSurfaces.
		  lastSurfaceIncrement *= -1 ;
		  m_currentSurface += lastSurfaceIncrement ;
		  finishedWithComposite = false ;
	       }
	       else
	       {
		  m_helix->revertToSnapshot( savedHelix ) ;
		  delete savedHelix ;
		  return kNoMoreSurfaces ;
	       }
	    }
	    else
	    {
	       if( m_printDiagnostics )
	       {
		  cout << "old radius " << ( *m_currentSurface )->radius() ;
	       }

	       lastSurfaceIncrement = surfaceIncrement ;
	       m_currentSurface += lastSurfaceIncrement ;
	       finishedWithComposite = false ;

	       if( m_printDiagnostics )
	       {
		  cout << " new radius " << ( *m_currentSurface )->radius()
		       << endl ;
	       }
	    }
	 }
      }


      Hep3Vector oldPos = m_helix->position() ;
      Hep3Vector oldMom = m_helix->momentumUnit() ;
      double oldMomDotPos = oldPos.x() * oldMom.x() + oldPos.y() * oldMom.y() ;

      if( ( *m_currentSurface )->propagateHelix( *m_helix, aDirection ) )
      {
	 Hep3Vector newPos = m_helix->position() ;
	 Hep3Vector newMom = m_helix->momentumUnit() ;
	 double newMomDotPos = newPos.x()*newMom.x() + newPos.y()*newMom.y() ;
	 Meters lastArc = fabs( m_helix->lastArcLength() ) ;
	 Meters circleCenterRadius =
	    fabs( m_helix->d0() + 0.5 / m_helix->curvature() ) ;
	 Meters radialPosDiff = sqrt(
	    sqr( newPos.x() - oldPos.x() ) + sqr( newPos.y() - oldPos.y() ) ) ;

	 if( m_printDiagnostics )
	 {
	    cout << "lastArc " << lastArc
		 << " radialPosDiff " << radialPosDiff
		 << " curl " << ( tryCurling ? 1 : 0 )
		 << " oldMomDotPos " << oldMomDotPos
		 << " newMomDotPos " << newMomDotPos
		 << " centerRadius " << circleCenterRadius
		 << " currentRadius " << ( *m_currentSurface )->radius()
		 << " checkCrossOrigin " << ( m_checkCrossOrigin ? 1 : 0 )
		 << " " << ( *m_currentSurface )->surfaceType()
// 		 << " " << ( *m_currentSurface )->innerMaterial()->name()
// 		 << " " << ( *m_currentSurface )->outerMaterial()->name()
		 << endl ;
	 }

	 if( tryCurling && oldMomDotPos * newMomDotPos > 0. )
	 {
	    m_helix->revertToSnapshot( savedHelix ) ;
	    delete savedHelix ;

	    if( atEndOfSurfaces )
	    {
	       return kNoMoreSurfaces ;
	    }
	    else
	    {
	       return kHelixTransportError ;
	    }
	 }
	 // Jim Pivarski Jan 20, 2003: the normal (good hit) condition
	 // in this `else if' is to fail and go on to the `else'.  The
	 // normal (good hit) condition for old or newMomDotPos is to
	 // be positive.  Before scattering on the first surface,
	 // oldMomDotPos is zero, but not necessarily exactly.
	 // Sometimes oldMomDotPos is ~ -1e-20, so 
	 //           ( oldMomDotPos * newMomDotPos < 0. &&
	 // yielded true (error) when it shouldn't have.  I added a
	 // check to make sure that oldMomDotPos is significant
	 // (absolute value larger than 1e-10).
	 else if( ( *m_currentSurface )->checkPropagation() &&
		  ( ( m_checkCrossOrigin &&
		      ( fabs( oldMomDotPos ) > 1.e-10 &&
			oldMomDotPos * newMomDotPos < 0. &&
			circleCenterRadius > ( *m_currentSurface )->radius() ) )
		    || lastArc > 10. * radialPosDiff // prevents full revolution
		    || lastArc > 1000. || lastArc < 1.e-10 ) )
	 {
	    m_helix->revertToSnapshot( savedHelix ) ;
	    transportSuccessful = false ;

	    if( tryCurling )
	    {
	       delete savedHelix ;
	       return kNoMoreSurfaces ;
//	       return kHelixTransportError ;
	    }

	    // If the current surface is a composite, there are no more
	    // intersections in this layer, so allow the surface to be
	    // incremented.
	    if( ( *m_currentSurface )->isComposite() )
	    {
	       m_helix->revertToSnapshot( savedHelix ) ;
	       finishedWithComposite = true ;
	    }
	 }
	 else
	 {
	    transportSuccessful = true ;
	    tryCurling = false ;
	 }
      }
      else if( tryCurling ||
	 ( !( *m_currentSurface )->isComposite() &&
	   aDirection == KTMoveControl::kDirectionEither ) )
      {
	 // Helix propagation failed, so return with an error.
	 m_helix->revertToSnapshot( savedHelix ) ;
	 delete savedHelix ;

	 if( atEndOfSurfaces )
	 {
	    return kNoMoreSurfaces ;
	 }
	 else
	 {
	    return kHelixTransportError ;
	 }
      }
      else if( ( *m_currentSurface )->isComposite() )
      {
	 // If the current surface is a composite, propagation failure
	 // may be due to there being only one surface in the composite.
//	 m_helix->revertToSnapshot( savedHelix ) ;
	 transportSuccessful = false ;
	 finishedWithComposite = true ;
      }
      else if( aDirection != KTMoveControl::kDirectionEither )
      {
//	 m_helix->revertToSnapshot( savedHelix ) ;
	 transportSuccessful = false ;

	 if( m_attemptCurling )
	 {
	    tryCurling = true ;
	 }
      }

      // Check for infinite looping.
      totalArcLength += m_helix->lastArcLength() ;
      if( fabs( totalArcLength ) > oneRevolution )
      {
	 delete savedHelix ;
	 return kNoIntersection ;
      }

   } while( !( transportSuccessful &&
	      ( *m_currentSurface )->conditionSatisfied( *m_helix ) ) ) ;


   // At this point, helix has been transported successfully.  Return
   // false if operation fails.
   m_helix->setAtSurface( true ) ;
   IntersectionStatus status = performOperation( *savedHelix,
						 savedSurface,
						 aDirection ) ;
   delete savedHelix ;
   return status ;
}


HIHelixIntersector::IntersectionStatus
HIHelixIntersector::performOperation(
   const HIHelix& oldHelix,
   STL_VECTOR( HIIntersectionSurface* )::iterator oldSurface,
   const KTMoveControl::MoveDirection aDirection )
{
   // Check to see which operation is to be performed.
   DABoolean performGlobalOperation = false ;

   if( m_globalCondition != 0 )
   {
      performGlobalOperation =
	 m_globalCondition->satisfied( *m_helix, *( *m_currentSurface ) ) ;
   }

   DABoolean success ;

   if( performGlobalOperation && m_globalOperation != 0 )
   {
      // If the global condition is satisfied, untransport the helix,
      // perform the global operation, and then decrement the current
      // surface.  This way, the right material description is used.

      // Since the assignment operator is not virtual, only the data in
      // HIHelix will be restored.  Data that is added by subclasses of
      // HIHelix will not be changed (I think).
      m_helix->revertToSnapshot( &oldHelix ) ;
      success = m_globalOperation->perform( *m_helix,
					    oldHelix,
					    oldHelix,
					    *( *m_currentSurface ),
					    *( *oldSurface ) ) ;
      if( ( *m_currentSurface )->isComposite() )
      {
	 HICompositeSurface* tempComposite =
	    ( HICompositeSurface* ) *m_currentSurface ;
	 tempComposite->revertIntersectedSurface() ;
      }

      if ( !success )
      {
	 report( DEBUG, kFacilityString ) << "Problem with global operation."
					  << endl ;
	 return kGlobalOperationError ;
      }

      ( *m_currentSurface )->revertToPreviousState() ;
      return swimToCurrentSurface( aDirection ) ;
   }
   else
   {
      success =
	 ( *m_currentSurface )->performOperation( *m_helix,
						  oldHelix,
						  *( *oldSurface ) ) ;

      if( !success )
      {
	 // Don't decrement the current surface if the operation fails
	 // because we have successfully intersected the surface.

	 m_helix->revertToSnapshot( &oldHelix ) ;
	 report( DEBUG, kFacilityString ) << "Problem with surface operation."
					  << endl ;
	 return kSurfaceOperationError ;
      }
   }

   return kIntersectionOK ;
}


HIHelixIntersector::IntersectionStatus
HIHelixIntersector::swimToSurface(
   const HIIntersectionSurface* aSurface,
   const KTMoveControl::MoveDirection aDirection )
{
   // Even if the given surface is the current surface, the helix may not
   // have been swum using the surface's move routine.

   IntersectionStatus status = kIntersectionOK ;

   if( *m_currentSurface == aSurface )
   {
      // Must delete later.
      HIHelix* savedHelix = m_helix->newSnapshot() ;

      DABoolean success =
	 ( *m_currentSurface )->propagateHelix( *m_helix, aDirection ) ;

      if( success )
      {
	 if( ( *m_currentSurface )->conditionSatisfied( *m_helix ) )
	 {
	    m_helix->setAtSurface( true ) ;
	    status = performOperation( *savedHelix,
				       m_currentSurface,
				       aDirection ) ;
	 }
	 else
	 {
	    m_helix->revertToSnapshot( savedHelix ) ;
	    status = kNoIntersection ;
	 }
      }
      else
      {
	 m_helix->revertToSnapshot( savedHelix ) ;
	 status = kHelixTransportError ;
      }

      delete savedHelix ;
      return status ;
   }

   while ( status == kIntersectionOK && *m_currentSurface != aSurface )
   {
      status = swimToNextIntersection( aDirection ) ;
   }

   return status ;
}


HIHelixIntersector::IntersectionStatus
HIHelixIntersector::swimToCurrentSurface(
   const KTMoveControl::MoveDirection aDirection )
{
   return swimToSurface( *m_currentSurface, aDirection ) ;
}


HIHelixIntersector::IntersectionStatus
HIHelixIntersector::swimWhile( const HICondition* aCondition,
			       const KTMoveControl::MoveDirection aDirection )
{
   IntersectionStatus status = kIntersectionOK ;

   // There's probably a better way to do this...

   HIHelixIntersector initialIntersector = *this ;
   HIHelixIntersector savedIntersector ;

   while( status == kIntersectionOK &&
	  aCondition->satisfied( *m_helix, *( *m_currentSurface ) ) )
   {
      savedIntersector = *this ;
      status = swimToNextIntersection( aDirection ) ;
   }

   if( status == kIntersectionOK )
   {
      *this = savedIntersector ;
   }
   else
   {
      *this = initialIntersector ;
   }

   return status ;
}

void
HIHelixIntersector::setClosestSurfaceToCurrent(
   const KTMoveControl::MoveDirection aDirection )
{
   STL_VECTOR( HIIntersectionSurface* )::iterator surfaceItr =
      m_surfaces.begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator surfaceEnd =
      m_surfaces.end() ;

   Meters bestArcLength = 999999. ;
   HIIntersectionSurface* bestSurface = 0 ;

   HIHelix* tmpHelix = m_helix->newSnapshot() ;

   for( ; surfaceItr != surfaceEnd ; ++surfaceItr )
   {
      tmpHelix->revertToSnapshot( m_helix ) ;

      if( ( *surfaceItr )->propagateHelix( *tmpHelix, aDirection ) )
      {
	 ( *surfaceItr )->revertToPreviousState() ;

	 if( fabs( tmpHelix->lastArcLength() ) < bestArcLength )
	 {
	    bestArcLength = fabs( tmpHelix->lastArcLength() ) ;
	    bestSurface = *surfaceItr ;
	 }
      }
   }

   delete tmpHelix ;

   if( bestSurface != 0 && bestArcLength > 1.e-10 )
   {
      setCurrentSurface( bestSurface ) ;
   }
}

void
HIHelixIntersector::setClosestSurfaceToCurrentByRadius(
   const KTMoveControl::MoveDirection aDirection )
{
   Meters helixRadius = m_helix->position().perp() ;

   // If helix is inside layer with smallest radius, set current surface
   // to that layer.
   HIIntersectionSurface* closestSurface = 0 ;

   if( m_surfaceOrder == kIncreasingRadius )
   {
      closestSurface = *m_surfaces.begin() ;
   }
   else if( m_surfaceOrder == kDecreasingRadius )
   {
      closestSurface = *( m_surfaces.end() - 1 ) ;
   }
   else
   {
      STL_VECTOR( HIIntersectionSurface* )::iterator surfaceItr =
	 m_surfaces.begin() ;
      STL_VECTOR( HIIntersectionSurface* )::iterator surfaceEnd =
	 m_surfaces.end() ;

      for( ; surfaceItr != surfaceEnd ; ++surfaceItr )
      {
	 if( closestSurface == 0 ||
	     ( *surfaceItr )->radius() < closestSurface->radius() )
	 {
	    closestSurface = *surfaceItr ;
	 }
      }
   }

   if( helixRadius < closestSurface->radius() )
   {
      setCurrentSurface( closestSurface ) ;
      return ;
   }


   // If the helix is outside the innermost surface....
   Meters bestRadiusDiff = 99999. ;

   int diffSign = helixCurling() *
      ( aDirection == KTMoveControl::kDirectionForward ? 1 : -1 ) ;

   STL_VECTOR( HIIntersectionSurface* )::iterator surfaceItr =
      m_surfaces.begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator surfaceEnd =
      m_surfaces.end() ;

   for( ; surfaceItr != surfaceEnd ; ++surfaceItr )
   {
      Meters radiusDiff = ( *surfaceItr )->radius() - helixRadius ;

      if( ( ( aDirection != KTMoveControl::kDirectionEither &&
	      diffSign * radiusDiff > 0.) ||
	    aDirection == KTMoveControl::kDirectionEither ) &&
	  fabs( radiusDiff ) < bestRadiusDiff && fabs( radiusDiff ) > 1.e-6 )
      {
	 closestSurface = *surfaceItr ;
	 bestRadiusDiff = fabs( radiusDiff ) ;
      }
   }

   if( closestSurface != 0 )
   {
      setCurrentSurface( closestSurface ) ;
   }
}


void
HIHelixIntersector::resetSurfaces() 
{
   // Reset the surfaces.
   STL_VECTOR( HIIntersectionSurface* )::iterator surfaceItr =
      m_surfaces.begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator surfaceEnd =
      m_surfaces.end() ;

   for( ; surfaceItr != surfaceEnd ; ++surfaceItr )
   {
      ( *surfaceItr )->reset() ;
   }
}

HIIntersectionSurface*
HIHelixIntersector::currentSurface()
{
   return *m_currentSurface ;
}

//
// const member functions
//
const HIIntersectionSurface*
HIHelixIntersector::nextSurface(
   const KTMoveControl::MoveDirection aDirection ) const
{
   return *( m_currentSurface + nextSurfaceIncrement( aDirection ) ) ;
}


//
// static member functions
//
