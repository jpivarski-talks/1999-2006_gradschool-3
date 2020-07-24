// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICompositeSurface
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Sep 24 17:58:33 EDT 1998
// $Id: HICompositeSurface.cc,v 1.17 2001/03/26 06:56:49 wsun Exp $
//
// Revision history
//
// $Log: HICompositeSurface.cc,v $
// Revision 1.17  2001/03/26 06:56:49  wsun
// Save last state of m_helixAtSurface for revertToPreviousState().
//
// Revision 1.16  2001/02/22 02:01:12  wsun
// Set condition and operation delete flags.
//
// Revision 1.15  2000/06/09 23:46:26  wsun
// Moved correctHitsForHelix() to HICalibratedHit.
//
// Revision 1.14  2000/06/09 00:28:48  wsun
// Added correctHitsForHelix().
//
// Revision 1.13  2000/03/02 08:25:18  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.12  1999/12/15 20:37:24  wsun
// * Updated to new HIIntersectionSurface interface.
// * Added STL vector to cache list of surfaces in range of helix, instead of
//   redetermining every time propagateHelix() is called.
//
// Revision 1.11  1999/11/21 22:29:24  wsun
// Sped up propagation in Si by subclassing to HICompositePlane.
//
// Revision 1.10  1999/08/27 20:17:40  wsun
// Changed signature for performOperation().
//
// Revision 1.9  1999/08/03 21:43:59  wsun
// Added implementation of numberOfAssociatedHits.
//
// Revision 1.8  1999/07/12 06:40:03  wsun
// Cull surfaces for possible intersection using track's projected phi position
//
// Revision 1.7  1999/06/30 22:23:25  wsun
// Added addChildren() function.
//
// Revision 1.6  1999/06/24 22:24:54  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.5  1999/06/18 19:25:54  wsun
// More improvements to low momentum fitting.
//
// Revision 1.4  1999/06/02 23:05:20  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/04/28 22:57:17  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:06:21  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:24  wsun
// First submission.
//

#include "Experiment/Experiment.h"
#include "Experiment/cleo_math.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HICompositeSurface.h"
#include "HelixIntersection/HIIntersectionPlane.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HICompositeSurface" ;

const string HICompositeSurface::kSurfaceType = "HICompositeSurface" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HICompositeSurface::HICompositeSurface()
   : HIIntersectionSurface(),
     m_surfaces( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_surfacesToDelete( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_intersectedSurfaces( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_surfacesInRange( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_intersectedSurface( 0 ),
     m_helixAtSurface( false ),
     m_lastHelixAtSurface( false )
{
   setDtorDeletesCondition( false ) ;
   setDtorDeletesOperation( false ) ;
}

// HICompositeSurface::HICompositeSurface( const HICompositeSurface& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICompositeSurface::~HICompositeSurface()
{
   deleteChildren() ;
   delete m_surfaces ;
   delete m_intersectedSurfaces ;
   delete m_surfacesInRange ;
   delete m_surfacesToDelete ;
}

//
// assignment operators
//
// const HICompositeSurface& HICompositeSurface::operator=( const HICompositeSurface& rhs )
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
HICompositeSurface::addChild( HIIntersectionSurface* aSurface )
{
   m_surfaces->push_back( aSurface ) ;
   m_surfacesToDelete->push_back( aSurface ) ;
}


void
HICompositeSurface::deleteChildren()
{
   for( STL_VECTOR( HIIntersectionSurface* )::iterator iter =
	   m_surfacesToDelete->begin() ;
	iter != m_surfacesToDelete->end() ;
	++iter )
   {
      delete *iter ;
   }

   m_surfaces->clear() ;
   m_intersectedSurfaces->clear() ;
   m_surfacesToDelete->clear() ;
}


void
HICompositeSurface::deleteCondition()
{
   for( STL_VECTOR( HIIntersectionSurface* )::iterator iter =
	   m_surfacesToDelete->begin() ;
	iter != m_surfacesToDelete->end() ;
	++iter )
   {
      ( *iter )->deleteCondition() ;
   }
}


void
HICompositeSurface::deleteOperation()
{
   for( STL_VECTOR( HIIntersectionSurface* )::iterator iter =
	   m_surfacesToDelete->begin() ;
	iter != m_surfacesToDelete->end() ;
	++iter )
   {
      ( *iter )->deleteOperation() ;
   }
}


void
HICompositeSurface::setPrintDiagnostics( DABoolean flag )
{
   for( STL_VECTOR( HIIntersectionSurface* )::iterator iter =
	   m_surfaces->begin() ;
	iter != m_surfaces->end() ;
	++iter )
   {
      ( *iter )->setPrintDiagnostics( flag ) ;
   }

   HIIntersectionSurface::setPrintDiagnostics( flag ) ;
}

void
HICompositeSurface::setUseReverseHitOrder( DABoolean flag )
{
   for( STL_VECTOR( HIIntersectionSurface* )::iterator iter =
	   m_surfaces->begin() ;
	iter != m_surfaces->end() ;
	++iter )
   {
      ( *iter )->setUseReverseHitOrder( flag ) ;
   }

   HIIntersectionSurface::setUseReverseHitOrder( flag ) ;
}


DABoolean
HICompositeSurface::propagateHelix( HIHelix& helixToPropagate,
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
      if( helixAtCompositeRadius.moveToRadius( radius(), aDirection ) !=
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

      DABoolean success =
	( *iter )->propagateHelix( tmpHelix, aDirection ) ;
      Meters tmpArcLength = tmpHelix.lastArcLength() ;

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


DABoolean
HICompositeSurface::performOperation(
   HIHelix& aHelix,
   const HIHelix& aLastHelix,
   const HIIntersectionSurface& aLastSurface )
{
   if( m_intersectedSurface == 0 )
   {
      return false ;
   }
   else if( &aLastSurface == this && lastIntersectedSurface() != 0 )
   {
      return m_intersectedSurface->performOperation(
	 aHelix,
	 aLastHelix,
	 *lastIntersectedSurface() ) ;
   }
   else
   {
      return m_intersectedSurface->performOperation(
	 aHelix,
	 aLastHelix,
	 aLastSurface ) ;
   }
}


void
HICompositeSurface::setIntersectedSurface()
{
   if( m_intersectedSurfaces->size() > 0 )
   {
      m_intersectedSurface = *( m_intersectedSurfaces->end() - 1 ) ;
      setInnerMaterial( m_intersectedSurface->innerMaterial() ) ;
      setOuterMaterial( m_intersectedSurface->outerMaterial() ) ;
      setCondition( m_intersectedSurface->condition() ) ;
      setOperation( m_intersectedSurface->operation() ) ;
   }
   else
   {
      reset() ;
   }
}

void
HICompositeSurface::revertToPreviousState()
{
   m_helixAtSurface = m_lastHelixAtSurface ;

   if( m_intersectedSurfaces->size() > 0 )
   {
      m_intersectedSurfaces->pop_back() ;
   }

   setIntersectedSurface() ;
}

void
HICompositeSurface::reset()
{
   m_intersectedSurface = 0 ;
   setInnerMaterial( 0 ) ;
   setOuterMaterial( 0 ) ;
   setCondition( 0 ) ;
   setOperation( 0 ) ;
   m_intersectedSurfaces->clear() ;
   m_surfacesInRange->clear() ;

   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_surfaces->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_surfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->reset() ;
   }
}

void
HICompositeSurface::revertIntersectedSurface()
{
   if( m_intersectedSurfaces->size() < 2 )
   {
      reset() ;
   }
   else
   {
      m_intersectedSurfaces->pop_back() ;
      setIntersectedSurface() ;
   }
}
   
void
HICompositeSurface::addChildren( const HICompositeSurface& aCompositeSurface )
{
   // Don't add these surfaces to m_surfacesToDelete!!!

   STL_VECTOR( HIIntersectionSurface* )::const_iterator itr =
      aCompositeSurface.m_surfaces->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::const_iterator end =
      aCompositeSurface.m_surfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      m_surfaces->push_back( *itr ) ;
   }
}

void
HICompositeSurface::addHICalibratedHitsToVector(
   STL_VECTOR( HICalibratedHit* )& aVector )
{
   STL_VECTOR( HIIntersectionSurface* )::iterator surfItr =
      m_surfaces->begin();
   STL_VECTOR( HIIntersectionSurface* )::iterator endSurf =
      m_surfaces->end() ;

   for( ; surfItr != endSurf ; ++surfItr )
   {
      ( *surfItr )->addHICalibratedHitsToVector( aVector ) ;
   }
}

//
// const member functions
//

const HIIntersectionSurface*
HICompositeSurface::getChild( int iChild ) const
{
   return ( *m_surfaces )[ iChild ] ;
}

int
HICompositeSurface::numberOfChildren() const
{
   STL_VECTOR( HIIntersectionSurface* )::const_iterator surfItr =
      m_surfaces->begin();
   STL_VECTOR( HIIntersectionSurface* )::const_iterator endSurf =
      m_surfaces->end() ;

   int nChildren = 0 ;
   for( ; surfItr != endSurf ; ++surfItr )
   {
      if( ( *surfItr )->isComposite() )
      {
	 HICompositeSurface* castSurf = ( HICompositeSurface* ) ( *surfItr ) ;
	 nChildren += castSurf->numberOfChildren() ;
      }
      else
      {
	 ++nChildren ;
      }
   }
   return nChildren ;
}

int
HICompositeSurface::numberOfAssociatedHits() const
{
   STL_VECTOR( HIIntersectionSurface* )::const_iterator surfItr =
      m_surfaces->begin();
   STL_VECTOR( HIIntersectionSurface* )::const_iterator endSurf =
      m_surfaces->end() ;

   int nHits = 0 ;
   for( ; surfItr != endSurf ; ++surfItr )
   {
      nHits += ( *surfItr )->numberOfAssociatedHits() ;
   }
   return nHits ;
}

const HIIntersectionSurface*
HICompositeSurface::lastIntersectedSurface() const
{
   if( m_intersectedSurfaces->size() > 1 )
   {
      return *( m_intersectedSurfaces->end() - 2 ) ;
   }
   else
   {
      return 0 ;
   }
}

//
// static member functions
//
