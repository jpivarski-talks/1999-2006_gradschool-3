// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HISiSensorPlane
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Oct 27 13:10:24 EST 1998
// $Id: HISiSensorPlane.cc,v 1.21 2001/03/29 01:40:42 llh14 Exp $
//
// Revision history
//
// $Log: HISiSensorPlane.cc,v $
// Revision 1.21  2001/03/29 01:40:42  llh14
// Separated Si Rphi and Z Hit Surfaces
//
// Revision 1.20  2001/03/26 07:15:21  wsun
// Mods for hit order reversal.
//
// Revision 1.19  2000/10/11 05:52:24  wsun
// Use fitting weight from HICalibratedHit, if available.
//
// Revision 1.18  2000/09/05 20:34:51  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.17  2000/06/09 23:46:27  wsun
// Moved correctHitsForHelix() to HICalibratedHit.
//
// Revision 1.16  2000/04/05 00:41:46  wsun
// Added static function derivatives().
//
// Revision 1.15  2000/03/02 08:25:19  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.14  2000/02/03 07:28:37  wsun
// Added use of HICalibratedHits.
//
// Revision 1.13  1999/12/15 20:47:16  wsun
// Updated to new HIIntersectionSurface interface and sped up derivatives().
//
// Revision 1.12  1999/11/21 22:31:41  wsun
// Sped up calculations.
//
// Revision 1.11  1999/09/30 00:50:21  wsun
// Added reset() and revertToPreviousState().
//
// Revision 1.10  1999/08/27 20:26:35  wsun
// Cosmetic changes.
//
// Revision 1.9  1999/08/03 21:54:50  wsun
// Mods to match base class changes.
//
// Revision 1.8  1999/06/24 22:25:03  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.7  1999/06/18 19:26:01  wsun
// More improvements to low momentum fitting.
//
// Revision 1.6  1999/06/02 23:05:27  wsun
// Added features for low-momentum fitting.
//
// Revision 1.5  1999/05/10 18:41:16  wsun
// Fixed derivatives for Si sensors.
//
// Revision 1.4  1999/05/03 19:31:47  wsun
// Fixed derivative calculation and measurement directions.
//
// Revision 1.3  1999/04/28 22:57:26  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:06:34  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:26  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ASiStorePro/ASiLadder.h"
#include "ASiStorePro/ASiSensorInfo.h"

#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HISiSensorPlane.h"
#include "HelixIntersection/HIMagField.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/Vector.h"

// STL classes
#include <vector>
#include <map>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HISiSensorPlane" ;

const string HISiSensorPlane::kSurfaceType = "HISiSensorPlane" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HISiSensorPlane::HISiSensorPlane()
{
}

HISiSensorPlane::HISiSensorPlane(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   const ASiSensorInfo& aSensor,
   HICondition* aCondition,
   HIHelixOperation* aOperation,
   const CalibratedSVRphiHit* aSVRphiHit,
   const CalibratedSVZHit* aSVZHit,
   const HepPoint3D& aCorrectionInSi )
   : HISiLadderPlane( aInnerMaterial,
		      aOuterMaterial,
		      aSensor.ladderOwner(),
		      aCondition,
		      aOperation,
		      aCorrectionInSi),
   m_transToSi( aSensor.transToSi() ),
   m_forceSVZFittingWeight( false ),
   m_svzFittingWeight( 0. ),
   m_CorrectionInSi( aCorrectionInSi )
{
   if( aSVRphiHit != 0 )
   {
      m_hits.push_back( HICalibratedSVHit(
	 aSVRphiHit,
	 -aSensor.ladderOwner().unitVectorXInSi() ) ) ;
   }

   if( aSVZHit != 0 )
   {
      m_hits.push_back( HICalibratedSVHit(
	 aSVZHit,
	 aSensor.ladderOwner().unitVectorZInSi() ) ) ;
   }

   m_currentHit = m_hits.begin() ;

}

// HISiSensorPlane::HISiSensorPlane( const HISiSensorPlane& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HISiSensorPlane::~HISiSensorPlane()
{
}

//
// assignment operators
//
// const HISiSensorPlane& HISiSensorPlane::operator=( const HISiSensorPlane& rhs )
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
HISiSensorPlane::setUseReverseHitOrder( DABoolean aFlag )
{
   if( aFlag == m_useReverseHitOrder )
   {
      return ;
   }

   m_useReverseHitOrder = aFlag ;

   if( m_useReverseHitOrder )
   {
      m_currentHit = m_hits.end() - 1 ;
   }
   else
   {
      m_currentHit = m_hits.begin() ;
   }
}

void
HISiSensorPlane::addCalibratedSVHit( const CalibratedSVHit* aHit,
				     const Hep3Vector& aMeasurementDirection )
{
   m_hits.push_back( HICalibratedSVHit( aHit, aMeasurementDirection ) ) ;
}

// Returns false if at the end of the list (or if only one hit).
DABoolean
HISiSensorPlane::advanceToNextCalibratedHit( HIHelix& )
{
   if( !m_useReverseHitOrder )
   {
      if( ++m_currentHit == m_hits.end() )
      {
	 m_currentHit = m_hits.begin() ;
	 return false ;
      }
   }
   else
   {
      if( m_currentHit == m_hits.begin() )
      {
	 m_currentHit = m_hits.end() - 1 ;
	 return false ;
      }
      else
      {
	 --m_currentHit ;
      }
   }

   return true ;
}

void
HISiSensorPlane::reset()
{
   if( m_useReverseHitOrder )
   {
      m_currentHit = m_hits.end() - 1 ;
   }
   else
   {
      m_currentHit = m_hits.begin() ;
   }
}

void
HISiSensorPlane::resetHICalibratedHits()
{
   STL_VECTOR( HICalibratedSVHit )::iterator hitItr = m_hits.begin() ;
   STL_VECTOR( HICalibratedSVHit )::iterator hitEnd = m_hits.end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      hitItr->clear() ;
   }
}

void
HISiSensorPlane::addHICalibratedHitsToVector(
   STL_VECTOR( HICalibratedHit* )& aVector )
{
   STL_VECTOR( HICalibratedSVHit )::iterator itr = m_hits.begin() ;
   STL_VECTOR( HICalibratedSVHit )::iterator end = m_hits.end() ;

   for( ; itr != end ; ++itr )
   {
      aVector.push_back( &( *itr ) ) ;
   }
}

//
// const member functions
//

HIIntersectionSurface::Meters
HISiSensorPlane::dca( const HIHelix& aHelix ) const
{
// Old code w/o radial corrections:
//   Hep3Vector residualVector = aHelix.position() -
//      ( m_transToSi * m_currentHit->calibratedSVHit()->localcoord() ) ;

   // to incorporate effective radial hit corrections, add the
   // correction to the hit position
   Hep3Vector residualVector = aHelix.position() -
      ( m_transToSi * m_currentHit->calibratedSVHit()->localcoord() +
	m_CorrectionInSi ) ;

   return m_currentHit->measurementDirection().dot( residualVector ) ;
}


void HISiSensorPlane::derivatives(
   const HIHelix& aHelix, HepVector& outputVector ) const
{
   derivatives( aHelix,
		m_currentHit->measurementDirection(),
		normal(),
		outputVector ) ;
}

void
HISiSensorPlane::derivatives(
   const HIHelix& aHelix,
   const HepVector3D& aMeasDir, // must be unit vector!
   const HepNormal3D& aNormalToPlane,
   HepVector& outputVector )
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
//   Hep3Vector N = normal() ;
   const HepNormal3D& N = aNormalToPlane ;

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

//   const HepVector3D& measDir = m_currentHit->measurementDirection() ;

   // d(DCA)/d(curvature) = 0 since we are right at the PCA.
   outputVector( HIHelix::kCurvature ) =
      aMeasDir.dot(
	 P * N.dot( positionFromInit ) * nDotPInv - positionFromInit ) /
      aHelix.curvature() ;

   // d(DCA)/d(phi0) = 0 since we are right at the PCA.
   // = X.cross( *m_currentMeasurementDirection ).z() -
   //   X.cross( N ).z() * nDotPInv * m_currentMeasurementDirection->dot( P )
   outputVector( HIHelix::kPhi0 ) =
      ( X.x() * aMeasDir.y() - X.y() * aMeasDir.x() ) -
      ( X.x() * N.y() - X.y() * N.x() ) * nDotPInv * aMeasDir.dot( P ) ;

   // d(DCA)/d(d0) = zCrossPUnit - N.dot( zCrossPUnit ) * nDotPInv * P )
   // zCrossPUnit = momentumAtInit.cross( bField ).unit() ;
   double pt = momentumAtInit.perp() ;
   Hep3Vector zCrossPUnit( -momentumAtInit.y() / pt,
			   momentumAtInit.x() / pt,
			   0. ) ;
   outputVector( HIHelix::kD0 ) =
      aMeasDir.dot( zCrossPUnit -
				 ( N.x() * zCrossPUnit.x() +
				   N.y() * zCrossPUnit.y() ) *
				 nDotPInv * P ) ;

   // d(DCA)/d(z0) = 0 for r-phi layers.
   outputVector( HIHelix::kZ0 ) =
      aMeasDir.z() - N.z() * nDotPInv * aMeasDir.dot( P ) ;

   // d(DCA)/d(cotTheta) = 0
   outputVector( HIHelix::kCotTheta ) =
      totalRadialArcLength * outputVector( HIHelix::kZ0 ) ;
}

Meters
HISiSensorPlane::fittingWeight() const
{
   if( m_forceFittingWeight &&    // = SVR
       m_currentHit->calibratedSVHit()->deviceType() == CalibratedHit::SVR )
   {
      return m_fittingWeight ;
   }
   else if( m_forceSVZFittingWeight &&
	    m_currentHit->calibratedSVHit()->deviceType()==CalibratedHit::SVZ )
   {
      return m_svzFittingWeight ;
   }
   else if( &*m_currentHit != 0 && m_currentHit->fittingWeightCorrected() )
   {
      return m_currentHit->correctedFittingWeight() ;
   }
   else
   {
      return ( &*m_currentHit != 0 &&
	       m_currentHit->calibratedSVHit() != 0 ) ?
	 m_currentHit->calibratedSVHit()->weight() : 0. ;
   }
}

//
// static member functions
//
