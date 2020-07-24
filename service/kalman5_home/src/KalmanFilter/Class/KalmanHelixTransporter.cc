// -*- C++ -*-
//
// Package:     <KalmanFilter>
// Module:      KalmanHelixTransporter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Feb 10 15:25:09 EST 2001
// $Id: KalmanHelixTransporter.cc,v 1.2 2002/07/06 00:42:42 wsun Exp $
//
// Revision history
//
// $Log: KalmanHelixTransporter.cc,v $
// Revision 1.2  2002/07/06 00:42:42  wsun
// Check momDotPos after first step, and check error matrix after last step.
//
// Revision 1.1  2001/03/27 06:33:48  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "KalmanFilter/KalmanHelixTransporter.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"
#include "DataHandler/Frame.h"

#include "HelixIntersection/HIIRSurfaceFactory.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HIDRSurfaceFactory.h"
#include "HelixIntersection/HICathodeSurfaceFactory.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "KalmanFilter.KalmanHelixTransporter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanHelixTransporter.cc,v 1.2 2002/07/06 00:42:42 wsun Exp $";
static const char* const kTagString = "$Name: v12_04_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
KalmanHelixTransporter::KalmanHelixTransporter( Frame& aFrame )
   : m_surfacesOK( true ),
     m_printDiagnostics( false ),
     m_surfaces( new STL_VECTOR( HIIntersectionSurface* ) )
{
   generateSurfaces( aFrame ) ;
}

// KalmanHelixTransporter::KalmanHelixTransporter( const KalmanHelixTransporter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

KalmanHelixTransporter::~KalmanHelixTransporter()
{
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_surfaces->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_surfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      delete *itr ;
   }

   m_surfaces->clear() ;
   delete m_surfaces ;
}

//
// assignment operators
//
// const KalmanHelixTransporter& KalmanHelixTransporter::operator=( const KalmanHelixTransporter& rhs )
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

//
// const member functions
//

DABoolean
KalmanHelixTransporter::transportThroughMaterial(
   KTHelix& aHelixToTransport,
   GeV aMass,
   HepPoint3D& aDestinationRefPt ) const
{
   if( !m_surfacesOK )
   {
      return false ;
   }

   // Make a non-const copy of the seed track.
   HIHelix* helix = new HIHelix( aHelixToTransport, aMass ) ;
   HIHelix copyOfOriginal = *helix ;
  
   if( helix->momentum().mag2() == 0. )
   {
      delete helix ;
      return false ;
   }

   HIHelixIntersector intersector( *m_surfaces,
				   HIHelixIntersector::kIncreasingRadius,
				   helix ) ;

   intersector.setPrintDiagnostics( m_printDiagnostics ) ;
   intersector.resetSurfaces() ;
   intersector.setCheckCrossOrigin( false ) ;

   STL_VECTOR( HIIntersectionSurface* )::const_iterator itr =
      m_surfaces->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::const_iterator end =
      m_surfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->setPrintDiagnostics( m_printDiagnostics ) ;
   }

   Meters oneRevolution = M_PI / fabs( helix->curvature() ) *
      sqrt( 1. + ( helix->cotTheta() * helix->cotTheta() ) ) ;

   // Figure out which direction to swim the track.
   HepPoint3D posInit = helix->position() ;
   HepVector3D momUnitInit = helix->momentumUnit() ;
   double momDotPosInit = 
      ( aDestinationRefPt.x() - posInit.x() ) * momUnitInit.x() +
      ( aDestinationRefPt.y() - posInit.y() ) * momUnitInit.y() ;

   KTMoveControl::MoveDirection moveDir = momDotPosInit > 0. ?
      KTMoveControl::kDirectionForward : KTMoveControl::kDirectionBackward ;

   intersector.setClosestSurfaceToCurrent( moveDir ) ;

   HIHelixIntersector::IntersectionStatus status =
      intersector.swimToCurrentSurface( moveDir ) ;

   if( status == HIHelixIntersector::kGlobalOperationError ||
       status == HIHelixIntersector::kSurfaceOperationError ||
       status == HIHelixIntersector::kNoMoreSurfaces ||
       status == HIHelixIntersector::kHelixTransportError ||
       fabs( helix->lastArcLength() ) > oneRevolution )
   {
      delete helix ;
      return 0 ;
   }

   // Save helix and surface at last transport.
   HIHelix lastHelix( copyOfOriginal ) ;
   HIIntersectionSurface* lastSurface = intersector.currentSurface() ;
   DABoolean transportFailed = false ;
   DGConstMaterialPtr lastMaterial =
      lastSurface->materialInLastTransport( *helix ) ;

   HepPoint3D pos = helix->position() ;
   HepVector3D momUnit = helix->momentumUnit() ;
   double momDotPos =
      ( aDestinationRefPt.x() - pos.x() ) * momUnit.x() +
      ( aDestinationRefPt.y() - pos.y() ) * momUnit.y() ;

   // Enter loop only if helix has not passed destination point.
   if( momDotPos * momDotPosInit > 0. )
   {
      do
      {
	 lastHelix = *helix ;

	 status = intersector.swimToNextIntersection( moveDir ) ;

	 transportFailed =
	    status == HIHelixIntersector::kGlobalOperationError ||
	    status == HIHelixIntersector::kSurfaceOperationError ||
	    status == HIHelixIntersector::kHelixTransportError ||
	    !helix->errorMatrixOK() ;

	 if( !transportFailed &&
	     status != HIHelixIntersector::kNoMoreSurfaces )
	 {
	    lastSurface = intersector.currentSurface() ;

	    pos = helix->position() ;
	    momUnit = helix->momentumUnit() ;
	    momDotPos = 
	       ( aDestinationRefPt.x() - pos.x() ) * momUnit.x() +
	       ( aDestinationRefPt.y() - pos.y() ) * momUnit.y() ;

	    lastMaterial = lastSurface->materialInLastTransport( *helix ) ;
	 }
      }
      while( status != HIHelixIntersector::kNoMoreSurfaces &&
	     fabs( helix->totalArcLength() ) < oneRevolution &&
	     momDotPos * momDotPosInit > 0. &&
	     !transportFailed ) ;
   }

   if( transportFailed || lastMaterial.isNull() )
   {
      delete helix ;
      return false ;
   }

   *helix = lastHelix ;

   // The destination reference point is between the helix' current position
   // and the next surface.  So, transport the helix to the destination
   // point, and use lastSurface to apply energy loss and multiple scattering.

   if( helix->moveToReferencePoint( aDestinationRefPt, moveDir ) !=
       KTMoveControl::kMoveOK )
   {
      delete helix ;
      return false ;
   }

   HIHelix helixCopy = *helix ;
   if( m_printDiagnostics ) cout << "helixcopy momumit "
				 << helixCopy.momentumUnit() << endl;
   if( !m_msdedxOperation.perform( *helix, helixCopy, lastHelix,
				   *lastSurface, *lastSurface, lastMaterial )
       || !helix->errorMatrixOK() )
   {
      delete helix ;
      return false ;
   }

   aHelixToTransport = *helix ;
   delete helix ;
   return true ;
}


void
KalmanHelixTransporter::generateSurfaces( Frame& theFrame )
{
   // First, clear the existing cache.
   STL_VECTOR( HIIntersectionSurface* )::iterator itr = m_surfaces->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::iterator end = m_surfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      delete *itr ;
   }

   m_surfaces->clear() ;

   // IR cylinders.
   HIIRSurfaceFactory irFactory( theFrame ) ;
   if( irFactory.geometryAvailable() )
   {
      irFactory.generateSurfaces( *m_surfaces, &m_msdedxOperation ) ;
   }
   else
   {
      m_surfacesOK = false ;
      return ;
   }

   // Generate the Si sensor, diamond Vs, and carbon fiber tube boundaries.
   HISiSurfaceFactory siFactory( theFrame ) ;
   if( siFactory.geometryAvailable() )
   {
      siFactory.generateLadderBoundarySurfaces( &m_msdedxOperation,
						&m_msdedxOperation );
      siFactory.transferSurfacesTo( *m_surfaces ) ;
   }
   else
   {
      m_surfacesOK = false ;
      return ;
   }

   // DR wire layers
   HIDRSurfaceFactory drFactory( theFrame ) ;

   if( drFactory.geometryAvailable() )
   {
      drFactory.generateInnerTubeCylinders( *m_surfaces, &m_msdedxOperation ) ;
      drFactory.generateSurfacesWithoutHits( *m_surfaces, &m_msdedxOperation );
   }
   else
   {
      m_surfacesOK = false ;
      return ;
   }

   // DR cathode
   HICathodeSurfaceFactory cathodeFactory( theFrame ) ;

   if( cathodeFactory.geometryAvailable() )
   {
      cathodeFactory.generateCylinderWithoutHits( *m_surfaces,
						  &m_msdedxOperation ) ;
   }
   else
   {
      m_surfacesOK = false ;
      return ;
   }

   m_surfacesOK = true ;
}

//
// static member functions
//
