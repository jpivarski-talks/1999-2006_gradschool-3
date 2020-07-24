// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICathodeCylinder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:30:48 EDT 1999
// $Id: HICathodeCylinder.cc,v 1.12 2003/04/17 20:17:50 mccann Exp $
//
// Revision history
//
// $Log: HICathodeCylinder.cc,v $
// Revision 1.12  2003/04/17 20:17:50  mccann
// HICathodeCylinder has been set to always propagate in kDirectionEither, because sometimes the refpoint starts on the wrong side (because cathodes have radius of layer47)
//
// Revision 1.11  2001/03/26 06:44:42  wsun
// Mods for reversing hit order.
//
// Revision 1.10  2001/01/31 23:18:13  wsun
// Use layer 47 radius at endplate if track is outside the chamber.
//
// Revision 1.9  2000/11/18 00:11:05  wsun
// * Propagate helix to z-dependent layer 47 radius.
// * Allow fitting weight to be scaled by parameter.
//
// Revision 1.8  2000/10/11 05:53:15  wsun
// Corrected previous log comment.
//
// Revision 1.7  2000/10/11 05:25:55  wsun
// Use corrected fitting weight from HICalibratedHit, if available.
//
// Revision 1.6  2000/09/05 20:34:49  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.5  2000/03/02 08:25:17  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.4  2000/02/03 07:28:35  wsun
// Added use of HICalibratedHits.
//
// Revision 1.3  1999/12/15 20:38:11  wsun
// Updated to new HIIntersectionSurface interface and sped up derivatives().
//
// Revision 1.2  1999/11/21 22:31:34  wsun
// Sped up calculations.
//
// Revision 1.1  1999/09/10 21:19:50  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"
#include "Experiment/cleo_math.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HICathodeCylinder.h"
#include "HelixIntersection/HIMagField.h"
#include "ADRGeom/ADRSenseWireStore.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HICathodeCylinder" ;

const string HICathodeCylinder::kSurfaceType = "HICathodeCylinder" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HICathodeCylinder::HICathodeCylinder()
{
}

HICathodeCylinder::HICathodeCylinder(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   Meters aRadius,
   const DGTransform& aTransform,
   HICondition* aCondition,
   HIHelixOperation* aOperation,
   const Hep3Vector& aMeasurementDirection,
   const STL_VECTOR( const CalibratedCathodeHit* )* aVector,
   const ADRSenseWireStore* aSenseWireStore,
   DABoolean aScaleFittingWeight,
   DABoolean aFittingWeightScale,
   DABoolean checkDirectionSwitch )
   : HIIntersectionCylinder( aInnerMaterial,
			     aOuterMaterial,
			     aRadius,
			     aTransform,
			     aCondition,
			     aOperation ),
   m_measurementDirection( aMeasurementDirection ),
   m_wireStore( aSenseWireStore ),
   m_scaleFittingWeight( aScaleFittingWeight ),
   m_fittingWeightScale( aFittingWeightScale ),
   m_checkDirectionSwitch( checkDirectionSwitch )
{
   if( aVector != 0 )
   {
      STL_VECTOR( const CalibratedCathodeHit* )::const_iterator itr =
	 aVector->begin() ;
      STL_VECTOR( const CalibratedCathodeHit* )::const_iterator end =
	 aVector->end() ;

      for( ; itr != end ; ++itr )
      {
	 m_hits.push_back( HitAndUsed( HICalibratedCathodeHit( *itr ),
				       false ) ) ;
      }
   }
}

// HICathodeCylinder::HICathodeCylinder( const HICathodeCylinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICathodeCylinder::~HICathodeCylinder()
{
}

//
// assignment operators
//
// const HICathodeCylinder& HICathodeCylinder::operator=( const HICathodeCylinder& rhs )
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
HICathodeCylinder::propagateHelix(
   HIHelix& helixToPropagate,
   KTMoveControl::MoveDirection aDirection )
{
   // Jim Pivarski and Chris Jones 4/17/03: Because the cylinder
   // radius is set to the layer 47 radius, sometimes a track is
   // already outside the cylinder radius when asked to propogate to
   // it.  Outward fits have been set to kDirectionForward, which can
   // cause the intersection to take place at the wrong intersection
   // of the cylinder.  Here we set the direction explicitly to
   // "either" for all cathode hits.  (See the code for
   // moveToXYCylinder--- this won't worsen the efficiency.)
   aDirection = KTMoveControl::kDirectionEither;

   // First propagate to radius of layer.
//    if( !HIIntersectionCylinder::propagateHelix( helixToPropagate,
// 						aDirection ) )
//    {
//       return false ;
//    }

   if( m_wireStore == 0 )
   {
      return false ;
   }


   // If track is outside the chamber, use layer radius at endplate.
   Meters helixZ = helixToPropagate.position().z() ;
   int layerNumber = ADRSenseWireStore::kFirstLayer +
      ADRSenseWireStore::kNumberOfLayers - 1 ;

   if( helixZ > m_wireStore->zMax( layerNumber ) )
   {
      helixZ = m_wireStore->zMax( layerNumber ) ;
   }
   else if( helixZ <  m_wireStore->zMin( layerNumber ) )
   {
      helixZ = m_wireStore->zMin( layerNumber ) ;
   }

   DABoolean success ;
   Meters layer47Radius = m_wireStore->radiusAtz( success,
						  layerNumber,
						  helixZ ) ;

   if( !success )
   {
      return false ;
   }

   if( printDiagnostics() )
   {
      cout << "init pos/mom: " << helixToPropagate.position() << " / "
           << helixToPropagate.momentum() 
           << " r " << helixToPropagate.position().perp() << endl ;
   }

   KTMoveControl::MoveStatus status =
      helixToPropagate.moveToRadius( layer47Radius, aDirection ) ;

   if( printDiagnostics() )
   {
      cout << "move pos/mom: " << helixToPropagate.position() << " / "
           << helixToPropagate.momentum()
           << " r " << helixToPropagate.position().perp() << endl ;
   }

   if( status != KTMoveControl::kMoveOK )
   {
      return false ;
   }

   // Now find the first non-used hit that is close to the track.
   // helixToPropagate will not be moved.
   if( m_hits.size() == 0 )
   {
      return true ;
   }

   if( !findNextHit( helixToPropagate ) )
   {
      m_usedHits.push_back( 0 ) ;
   }

   return true ;
}

DABoolean
HICathodeCylinder::findNextHit( const HIHelix& aHelix )
{
   HIHelix testHelix( aHelix, false ) ; // don't copy error matrix

   if( !m_useReverseHitOrder )
   {
      STL_VECTOR( HitAndUsed )::iterator hitItr = m_hits.begin() ;
      STL_VECTOR( HitAndUsed )::iterator hitEnd = m_hits.end() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 // If the current hit has already been used, continue.
	 if( hitItr->second )
	 {
	    continue ;
	 }

	 hitItr->second = true ;
	 m_usedHits.push_back( &*hitItr ) ;
	 return true ;
      }
   }
   else
   {
      STL_VECTOR( HitAndUsed )::reverse_iterator hitItr = m_hits.rbegin() ;
      STL_VECTOR( HitAndUsed )::reverse_iterator hitEnd = m_hits.rend() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 // If the current hit has already been used, continue.
	 if( (* hitItr ).second )
	 {
	    continue ;
	 }

	 ( *hitItr ).second = true ;
	 m_usedHits.push_back( &*hitItr ) ;
	 return true ;
      }
   }

   return false ;
}

DABoolean
HICathodeCylinder::advanceToNextCalibratedHit( HIHelix& aHelix )
{
   return findNextHit( aHelix ) ;
}

void
HICathodeCylinder::reset()
{
   STL_VECTOR( HitAndUsed )::iterator hitItr = m_hits.begin() ;
   STL_VECTOR( HitAndUsed )::iterator hitEnd = m_hits.end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      hitItr->second = false ;
   }

   m_usedHits.clear() ;
}

void
HICathodeCylinder::resetHICalibratedHits()
{
   STL_VECTOR( HitAndUsed )::iterator hitItr = m_hits.begin() ;
   STL_VECTOR( HitAndUsed )::iterator hitEnd = m_hits.end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      hitItr->first.clear() ;
   }
}
   
void
HICathodeCylinder::revertToPreviousState()
{
   HitAndUsed* currentHit = *( m_usedHits.end() - 1 ) ;

   if( m_usedHits.size() > 0 && currentHit != 0 )
   {
      currentHit->second = false ;
   }

   m_usedHits.pop_back() ;
}


void
HICathodeCylinder::addHICalibratedHitsToVector(
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

HIIntersectionSurface::Meters
HICathodeCylinder::dca( const HIHelix& aHelix ) const
{
   HitAndUsed* currentHit = *( m_usedHits.end() - 1 ) ;

   if( m_usedHits.size() > 0 && currentHit != 0 )
   {
      return aHelix.position().z() -
	 currentHit->first.calibratedCathodeHit()->z() ;
   }
   else
   {
      return 0. ;
   }
}


void
HICathodeCylinder::derivatives(
   const HIHelix& aHelix, HepVector& outputVector ) const
{
   if( outputVector.num_row() != HIHelix::kNTrackParameters )
   {
      report( WARNING, kFacilityString )
	 << "Vector to be filled by arcLengthDerivatives()"
	 << " has the wrong size." << endl ;
      return ;
   }

   // The reference point is coincident with the helix position.

   // Position of helix w.r.t. reference point.
   Hep3Vector X = aHelix.position() - aHelix.referencePoint() ;

   // Unit vector tangent to helix.
   Hep3Vector P = aHelix.momentumUnit() ;

   // normal() is a unit vector.
   HepNormal3D N ;
   normal( aHelix, N ) ;

   double nDotPInv = 1. / N.dot( P ) ;

   Hep3Vector positionFromInit( 0., 0., 0. ) ;
   Hep3Vector momentumAtInit( P ) ;
   Meters totalRadialArcLength = 0. ;

   if( aHelix.reportFitResultsAtInitRefPt() )
   {
      Hep3Vector bField = HIMagField::instance()->bfield( aHelix.position() ) ;
      positionFromInit = aHelix.position() - aHelix.initialHelix().position() ;
      X = aHelix.position() - aHelix.initialHelix().referencePoint() ;
      momentumAtInit   = aHelix.initialHelix().momentum( bField ).unit() ;
      totalRadialArcLength =
         aHelix.totalArcLength() / sqrt( 1. + sqr( aHelix.cotTheta() ) ) ;
   }

   // d(DCA)/d(curvature) = 0 since we are right at the PCA.
   outputVector( HIHelix::kCurvature ) = m_measurementDirection.dot(
      P * N.dot( positionFromInit ) * nDotPInv - positionFromInit ) /
      aHelix.curvature() ;

   // d(DCA)/d(phi0) = 0 since we are right at the PCA.
   // = X.cross( *m_measurementDirection ).z() -
   //   X.cross( N ).z() * nDotPInv * m_measurementDirection.dot( P )
   outputVector( HIHelix::kPhi0 ) =
      ( X.x() * m_measurementDirection.y() -
	X.y() * m_measurementDirection.x() ) -
      ( X.x() * N.y() - X.y() * N.x() ) * nDotPInv *
      m_measurementDirection.dot( P ) ;

   // d(DCA)/d(d0) = zCrossPUnit - N.dot( zCrossPUnit ) * nDotPInv * P )
   // zCrossPUnit = momentumAtInit.cross( bField ).unit() ;
   double pt = momentumAtInit.perp() ;
   Hep3Vector zCrossPUnit( -momentumAtInit.y() / pt,
			   momentumAtInit.x() / pt,
			   0. ) ;
   outputVector( HIHelix::kD0 ) =  m_measurementDirection.dot(
      zCrossPUnit -
      ( N.x() * zCrossPUnit.x() + N.y() * zCrossPUnit.y() ) * nDotPInv * P ) ;

   // d(DCA)/d(z0) = 0 for r-phi layers.
   outputVector( HIHelix::kZ0 ) = m_measurementDirection.z() -
      N.z() * nDotPInv * m_measurementDirection.dot( P ) ;

   // d(DCA)/d(cotTheta) = 0
   outputVector( HIHelix::kCotTheta ) =
      totalRadialArcLength * outputVector( HIHelix::kZ0 ) ;
}

Meters
HICathodeCylinder::fittingWeight() const
{
   if( m_forceFittingWeight )
   {
      return m_fittingWeight ; 
   }
   else
   {
      HitAndUsed* currentHit = *( m_usedHits.end() - 1 ) ;

      if( m_usedHits.size() > 0 && currentHit != 0 )
      {
	 double weight ;

	 if( currentHit->first.fittingWeightCorrected() )
	 {
	    weight = currentHit->first.correctedFittingWeight() ;
	 }
	 else
	 {
	    weight = currentHit->first.calibratedCathodeHit()->weight() ;
	 }

	 if( m_scaleFittingWeight )
	 {
	    weight *= m_fittingWeightScale ;
	 }

	 return weight ;
      }
      else
      {
	 return 0. ;
      }
   }
}

const CalibratedHit*
HICathodeCylinder::calibratedHit() const
{
   HitAndUsed* currentHit = *( m_usedHits.end() - 1 ) ;

   if( m_usedHits.size() > 0 && currentHit != 0 )
   {
      return currentHit->first.calibratedCathodeHit() ;
   }
   else
   {
      return 0;
   }
}

HICalibratedHit*
HICathodeCylinder::hiCalibratedHit() const
{
   HitAndUsed* currentHit = *( m_usedHits.end() - 1 ) ;

   if( m_usedHits.size() > 0 && currentHit != 0 )
   {
      return &( currentHit->first ) ;
   }
   else
   {
      return 0;
   }
}
  
//
// static member functions
//
