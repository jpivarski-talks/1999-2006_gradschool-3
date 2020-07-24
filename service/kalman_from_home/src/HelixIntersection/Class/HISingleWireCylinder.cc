// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HISingleWireCylinder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Sep 25 19:01:33 EDT 1998
// $Id: HISingleWireCylinder.cc,v 1.13 2002/01/23 22:17:39 llh14 Exp $
//
// Revision history
//
// $Log: HISingleWireCylinder.cc,v $
// Revision 1.13  2002/01/23 22:17:39  llh14
// Added HICorrectForBFieldOperation, and small modifications to HISingleWireCylinder to incorporate the quad fields
//
// Revision 1.12  2001/09/26 08:04:13  wsun
// Added global wire number and drift time to diagnostics.
//
// Revision 1.11  2000/12/02 01:30:28  wsun
// Added avg chg on track to HICalibratedDRHit and DRHitAndDriftDistance.
//
// Revision 1.10  2000/06/09 23:46:27  wsun
// Moved correctHitsForHelix() to HICalibratedHit.
//
// Revision 1.9  2000/06/09 00:28:50  wsun
// Added correctHitsForHelix().
//
// Revision 1.8  2000/03/02 08:25:19  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.7  2000/02/22 17:19:54  wsun
// Allow users to set m_checkDirectionSwitch.
//
// Revision 1.6  2000/02/05 21:38:11  wsun
// Hits were never diabled if tracks switched direction.
//
// Revision 1.5  2000/02/03 07:27:53  wsun
// propagateHelix() now checks if track has switched direction.
//
// Revision 1.4  1999/12/15 21:01:07  wsun
// * Updated to new HIIntersectionSurface interface.
// * propagateHelix() caches the wire position and direction at the helix' z,
//   which is later used in dca() and derivatives().
// * Assume uniform B field in z direction to speed up derivatives().
//
// Revision 1.3  1999/11/21 22:31:42  wsun
// Sped up calculations.
//
// Revision 1.2  1999/09/10 21:22:13  wsun
// Allow for null pointer to calibrated hit.
//
// Revision 1.1  1999/08/03 21:41:06  wsun
// Renamed from HIWireLayerCylinder.
//
// Revision 1.9  1999/06/30 22:26:58  wsun
// Updated to new HIIntersectionCylinder interface.
//
// Revision 1.8  1999/06/24 22:25:05  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.7  1999/06/18 19:26:03  wsun
// More improvements to low momentum fitting.
//
// Revision 1.6  1999/06/02 23:05:28  wsun
// Added features for low-momentum fitting.
//
// Revision 1.5  1999/05/14 01:06:45  wsun
// Protect against unavailability of geometry.
//
// Revision 1.4  1999/04/28 22:57:27  wsun
// Fixes for Solaris.
//
// Revision 1.3  1999/04/27 22:06:36  wsun
// Major reorganization and structural changes.
//
// Revision 1.2  1999/02/11 21:08:49  wsun
// Added use of AEWireInfo::WireDirZ.
//
// Revision 1.1.1.1  1998/12/07 04:58:27  wsun
// First submission.
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
#include "HelixIntersection/HISingleWireCylinder.h"
#include "HelixIntersection/HIMagField.h"

#include "FrameAccess/FAItem.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"         
#include "DataHandler/Record.h"

#include "ADRGeom/ADRSenseWireStore.h"
#include "ADRGeom/ADRWireAddress.h"
#include "ActiveElements/AEWireInfo.h"
#include "ActiveElements/AEWireLayerCylindrical.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/Vector.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HISingleWireCylinder" ;

const string HISingleWireCylinder::kSurfaceType = "HISingleWireCylinder" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HISingleWireCylinder::HISingleWireCylinder()
{
}


HISingleWireCylinder::HISingleWireCylinder(
   const DGConstMaterialPtr&     aInnerMaterial,
   const DGConstMaterialPtr&     aOuterMaterial,
   const AEWireLayerCylindrical& aWireLayer,
   int                           aLayerNumber,
   const DGTransform&            aTransform,
   HICondition*                  aCondition,
   HIHelixOperation*             anOperation,
   const CalibratedDRHit*        aHit,
   Meters                        aSignedDriftDistance,
   const HitCorrector*           aHitCorrector,
   DABoolean                     aCheckDirectionSwitch,
   double                        aAverageChargeOnTrack )
   : HIDRLayerCylinder( aInnerMaterial,
			aOuterMaterial,
			aWireLayer,
			aLayerNumber,
			aTransform,
			aCondition,
			anOperation ),
   m_drHit( aHit, aHitCorrector, aSignedDriftDistance, aAverageChargeOnTrack ),
   m_pDRHit( &m_drHit ),
   m_checkSingleDirectionSwitch( aCheckDirectionSwitch )
{
   if( aHit != 0 && aSignedDriftDistance == 0. )
   {
      m_drHit.setSignedDriftDistance( aHit->distance() ) ;
   }

   extractWireInfo() ;
}

// HISingleWireCylinder::HISingleWireCylinder( const HISingleWireCylinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HISingleWireCylinder::~HISingleWireCylinder()
{
}

//
// assignment operators
//
// const HISingleWireCylinder& HISingleWireCylinder::operator=( const HISingleWireCylinder& rhs )
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
HISingleWireCylinder::setHICalibratedDRHit( HICalibratedDRHit* aHit )
{
   m_pDRHit = aHit ;
   extractWireInfo() ;
}

void
HISingleWireCylinder::setCalibratedDRHit( const CalibratedDRHit* aHit )
{
   m_pDRHit->setCalibratedDRHit( aHit ) ;
   extractWireInfo() ;
}

void
HISingleWireCylinder::setSignedDriftDistance( Meters aSignedDriftDistance )
{
   m_pDRHit->setSignedDriftDistance( aSignedDriftDistance ) ;
}


void
HISingleWireCylinder::extractWireInfo()
{
   if( m_pDRHit != 0 &&
       m_pDRHit->calibratedDRHit() != 0 &&
       m_pDRHit->calibratedDRHit()->layer() == layerNumber() )
   {
      m_wireInfo =
	 wireLayer().wire( m_pDRHit->calibratedDRHit()->wireInLyr() ) ;
   }
}


DABoolean
HISingleWireCylinder::propagateHelix(
   HIHelix& helixToPropagate,
   KTMoveControl::MoveDirection aDirection )
{
   if( m_pDRHit == 0 )
   {
      if( m_checkSingleDirectionSwitch )
      {
	 m_pDRHit = &m_drHit ;
      }
      else
      {
	 return HIDRLayerCylinder::propagateHelix( helixToPropagate,
						   aDirection ) ;
      }
   }

   // Because the helix is moved in multiple steps, use a temporary KTHelix
   // to find the total arc length to move.  This way, the HIHelix's last arc
   // length (and transport helix, if applicable) reflects a single transport.
   HIHelix testHelix( helixToPropagate.curvature(),
		      helixToPropagate.phi0(),
		      helixToPropagate.d0(),
		      helixToPropagate.cotTheta(),
		      helixToPropagate.z0(),
		      helixToPropagate.referencePoint() ) ;
   Meters arcLengthToMove ;

   // Wire position and direction at the helix's z position.
   AEWireInfo::WireDirZ wirePosDir =
      m_wireInfo.zPoint( testHelix.referencePoint().z() + testHelix.z0() ) ;

   if( printDiagnostics() )
   {
      cout << "Layer " << m_pDRHit->calibratedDRHit()->layer()
	   << " wire in layer " << m_pDRHit->calibratedDRHit()->wireInLyr()
	   << " global wire " << m_pDRHit->calibratedDRHit()->wire()
	   << " id " << m_pDRHit->calibratedDRHit()->identifier()
	   << " radius " << radius()
	   << " time " << m_pDRHit->driftTime() << endl
	   <<"init pos/mom " << helixToPropagate.position() << " / "
	   << helixToPropagate.momentum() << endl
	   << "wire pos/dir " << wirePosDir.first << " / "
	   << wirePosDir.second << endl ;
   }


   // First do a move to reference point (= wire position) and then
   // move iteratively to a line.  This is not as slow as it sounds because
   // moving close to the line reduces the number of iterations taken.
   // In the end, this stepwise transport takes about as much time as moving
   // directly to the line, but is a bit more robust.
   HepNormal3D norm ;
   normal( testHelix, norm ) ;
   double momDotNormalOrig = testHelix.momentumUnit().dot( norm ) ;

   KTMoveControl::MoveStatus status =
      testHelix.moveToReferencePoint( wirePosDir.first, // wire pos
				      aDirection ) ;

   Meters savedArcLength = testHelix.lastArcLength() ;

   // Check whether the track has switched direction.  If so, the wire is
   // supposed be on the other half of the track.  In this case, return to
   // the first intersection with the layer radius.
   normal( testHelix, norm ) ;
   double momDotNormal = testHelix.momentumUnit().dot( norm ) ;

   if( m_checkSingleDirectionSwitch && momDotNormalOrig * momDotNormal < 0. )
   {
      // Don't allow the hit to be fit.
      m_pDRHit = 0 ;

      // If the track has switched direction, go the layer radius instead.
      return HIDRLayerCylinder::propagateHelix( helixToPropagate,
						aDirection ) ;
   }
   else
   {
      wirePosDir =
	 m_wireInfo.zPoint( testHelix.referencePoint().z() + testHelix.z0() ) ;
      status = testHelix.moveToLine( wirePosDir.first,        // wire pos
				     wirePosDir.second.unit(),// wire dir
				     KTMoveControl::kDirectionEither ) ;
      arcLengthToMove = savedArcLength + testHelix.lastArcLength() ;

      DABoolean ok = ( status == KTMoveControl::kMoveOK ) ;

      if( ok )
      {
	 ok = helixToPropagate.moveByArcLength( arcLengthToMove ) ==
	    KTMoveControl::kMoveOK ; 

	 if( ok )
	 {
	    m_wirePosDir = wirePosDir ;
	 }

	 if( printDiagnostics() )
	 {
	    cout << "move pos/mom " << helixToPropagate.position() << " / "
		 << helixToPropagate.momentum() << endl ;
	    cout << "arc length " << helixToPropagate.lastArcLength() << endl ;
	 }
      }

      return ok ;
   }
}

void
HISingleWireCylinder::addHICalibratedHitsToVector(
   STL_VECTOR( HICalibratedHit* )& aVector )
{
   aVector.push_back( &m_drHit ) ;
}

//
// const member functions
//

HIIntersectionSurface::Meters
HISingleWireCylinder::dca( const HIHelix& aHelix ) const
{
   if( m_pDRHit == 0 )
   {
      return 0. ;
   }

   // Vector from helix to any point on the wire (preferably close to
   // the helix position so sag effect is not appreciable.
   Hep3Vector U = m_wirePosDir.first - aHelix.position() ;

   // Unit vector perpendicular to both the helix and the wire.
   Hep3Vector V =
      ( m_wirePosDir.second.cross( aHelix.momentumUnit() ) ).unit() ;

   return -U.dot( V ) ;
}


void
HISingleWireCylinder::derivatives(
   const HIHelix& aHelix, HepVector& outputVector ) const
{
   if( outputVector.num_row() != HIHelix::kNTrackParameters )
   {
      report( WARNING, kFacilityString )
	 << "Vector to be filled by arcLengthDerivatives()"
	 << " has the wrong size." << endl ;
      return ;
   }

   if( m_pDRHit == 0 )
   {
      return ;
   }

   //Not anymore!!! - Jan 2002//////////////////////////////
   // ****** Approx uniform B field in z direction. ******//
   // Hardwire the magnetic field for now.                //
   /////////////////////////////////////////////////////////

   // Unit vector perpendicular to both the helix and the wire.
   Hep3Vector momentumAtInit = aHelix.momentumUnit() ;
   Hep3Vector V =
      ( m_wirePosDir.second.cross( momentumAtInit ) ).unit() ;

   // If the fitted track parameters are not given at the helix
   // reference point (e.g. when doing a chisq fit), then we will need
   // the arc length traveled and the vector between the current position
   // and the "origin".

   Hep3Vector positionFromInit( 0., 0., 0. ) ;
   Hep3Vector X( 0., 0., 0. ) ; // = pos - aHelix.referencePoint()
   Meters totalRadialArcLength = 0. ;

   HepPoint3D pos = aHelix.position() ; 
   //Note, bfield( pos ) returns the uniform b field!!!
   //So, add on the correction to get the nonuniform field.
   Hep3Vector totBField = HIMagField::instance()->bfield( pos ) +
                       HIMagField::instance()->deltab( pos );
   //We also need the uniformB to access the correct momentum
   Hep3Vector uniformBField = HIMagField::instance()->bfield( pos );

   // d(DCA)/d(z0) = 0 for axial wires.
   outputVector( HIHelix::kZ0 ) = -V.dot( totBField.unit() );

   if( aHelix.reportFitResultsAtInitRefPt() )
   {
      positionFromInit = pos - aHelix.initialHelix().position() ;
      X = pos - aHelix.initialHelix().referencePoint() ;

      momentumAtInit = aHelix.initialHelix().momentum( uniformBField ).unit() ;

      totalRadialArcLength   =
	 aHelix.totalArcLength() / sqrt( 1. + sqr( aHelix.cotTheta() ) ) ;

      outputVector( HIHelix::kCurvature ) =
	 -V.dot( positionFromInit ) / aHelix.curvature() ;

      outputVector( HIHelix::kCotTheta ) =
	 totalRadialArcLength * outputVector( HIHelix::kZ0 ) ;

      // d(DCA)/d(phi0) = V.dot( X.cross( totBField.unit() ) )
      //Changing this to the generic definition
      outputVector( HIHelix::kPhi0 ) = V.dot( X.cross( totBField.unit() ) );
      //This was the old approximation when we had only a constant z field
//      outputVector( HIHelix::kPhi0 ) = V.y() * X.x() - V.x() * X.y() ;
   }
   else
   {
      // d(DCA)/d(curvature) = 0 since we are right at the PCA.
      outputVector( HIHelix::kCurvature ) = 0. ;

      // d(DCA)/d(phi0) = 0 if fit results reported at helix ref pt
      // since the ref pt is on the helix.
      outputVector( HIHelix::kPhi0 ) = 0. ;

      // d(DCA)/d(cotTheta) = 0 if fit results reported at helix ref pt.
      outputVector( HIHelix::kCotTheta ) = 0. ;
   }

   // d(DCA)/d(d0) = momentumAtInit.cross( totBField ) ).unit().dot( V )
   //              = 1 for axial wires.
   // Unit vector perpendicular to both helix momentum and totBField.
   // This was the old approximation when we had only a constant z field
   outputVector( HIHelix::kD0 ) = 
      (momentumAtInit.cross( totBField ) ).unit().dot( V );
//   double ptmag = momentumAtInit.perp() ;
//   outputVector( HIHelix::kD0 ) =
//      ( V.y() * momentumAtInit.x() - V.x() * momentumAtInit.y() ) / ptmag ;
}

//
// static member functions
//
