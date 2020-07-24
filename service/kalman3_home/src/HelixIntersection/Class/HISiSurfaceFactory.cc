// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HISiSurfaceFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Apr  2 18:45:57 EST 1999
// $Id: HISiSurfaceFactory.cc,v 1.26 2001/03/29 01:40:42 llh14 Exp $
//
// Revision history
//
// $Log: HISiSurfaceFactory.cc,v $
// Revision 1.26  2001/03/29 01:40:42  llh14
// Separated Si Rphi and Z Hit Surfaces
//
// Revision 1.25  2001/03/26 07:15:45  wsun
// Changed gas from air to nitrogen.
//
// Revision 1.24  2000/09/05 20:34:52  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.23  2000/08/16 19:59:28  wsun
// Extract seed/hit lattices with usage and production tags.
//
// Revision 1.22  2000/07/25 22:32:43  wsun
// User can specify usage and and production tags for hits.
//
// Revision 1.21  2000/06/29 17:27:24  wsun
// Added option for no boundary conditions.
//
// Revision 1.20  2000/04/11 13:05:08  pg
// Use SVAlignment stream to extract Si active element
//
// Revision 1.19  2000/02/03 07:27:09  wsun
// Cleaned up code.
//
// Revision 1.18  1999/12/15 21:02:33  wsun
// Updated to new HIIntersectionPlane and HISiSensorPlane ctors.
//
// Revision 1.17  1999/11/21 22:29:49  wsun
// Si sensor and diamond surfaces are now kept in HICompositePlanes.
//
// Revision 1.16  1999/11/19 19:56:38  wsun
// Updated to changes in SiGeomDescription.
//
// Revision 1.15  1999/11/03 03:39:27  wsun
// Use SVZ not SVR hits for sensor number.
//
// Revision 1.14  1999/10/14 18:53:38  wsun
// Added flag to transferSurfacesTo() to ignore composites w/o children.
//
// Revision 1.13  1999/10/05 23:06:31  wsun
// Cosmetic changes.
//
// Revision 1.12  1999/08/05 20:06:06  wsun
// Removed debug printout.
//
// Revision 1.11  1999/08/03 21:55:56  wsun
// Reenable generation of surfaces for diamond supports.
//
// Revision 1.10  1999/07/24 03:50:01  wsun
// Allow user to supply DGDetectorStore and ASiStore.
//
// Revision 1.9  1999/07/12 06:50:50  wsun
// * Added two rphi edge surfaces for each sensor and diamond support.  Now
//   four surfaces for each box volume.
// * Make four surfaces for each ladder, instead of for each sensor -- saves
//   a lot of time.
//
// Revision 1.8  1999/07/06 22:25:05  wsun
// Get ASiStore from basegeometry stream instead of beginrun.
//
// Revision 1.7  1999/06/30 22:25:10  wsun
// Fixed memory fault when there are SVZ but no SVR hits.
//
// Revision 1.6  1999/06/24 22:25:04  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.5  1999/06/22 22:54:02  pg
// Fixes for New Si hits
//
// Revision 1.4  1999/06/18 19:26:02  wsun
// More improvements to low momentum fitting.
//
// Revision 1.3  1999/05/14 01:06:44  wsun
// Protect against unavailability of geometry.
//
// Revision 1.2  1999/04/28 22:57:26  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:06:35  wsun
// Major reorganization and structural changes.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HISiSurfaceFactory.h"

#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"

#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAConstants.h"
#include "FrameAccess/extract.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGConstPlacedVolumePtr.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGConstVolumePtr.h"
#include "DetectorGeometry/DGVolumeBox.h"
#include "DetectorGeometry/DGVolumeTube.h"
#include "DetectorGeometry/DGPath.h"
#include "DetectorGeometry/DGMaterialStore.h"
#include "MaterialDelivery/material_identifiers.h"

#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiLadder.h"
#include "ASiStorePro/ASiSensorInfo.h"

#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

#include "HelixIntersection/HICompositePlane.h"
#include "HelixIntersection/HIHelixOperation.h"
#include "HelixIntersection/HIZBoundaryCondition.h"
#include "HelixIntersection/HIRphiBoundaryCondition.h"
#include "HelixIntersection/HICompositeCondition.h"
#include "HelixIntersection/HISiSensorPlane.h"
#include "HelixIntersection/HIIntersectionCylinder.h"

#include "TrackRoot/TRSeedTrack.h"
#include "TrackFinder/SeedTrackSVRHitLattice.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"

#include "DAException/DANoDataException.h"

#include "DBSVRPhiHitRadialCorrections.hh"
#include "C3sv/SiStream.h"
// STL classes

// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HISiSurfaceFactory" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HISiSurfaceFactory.cc,v 1.26 2001/03/29 01:40:42 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HISiSurfaceFactory::HISiSurfaceFactory( Frame& theFrame,
					const DGDetectorStore* aDetectorStore,
					const ASiStore* aSiStore,
					DABoolean aGenerateBoundaryConditions )
   : m_frame( theFrame ),
     m_geometryAvailable( true ),
     m_siSurfaces( new STL_VECTOR( HICompositePlane* ) ),
     m_otherSurfaces( new STL_VECTOR( HIIntersectionSurface* ) ),
     m_deleteSiSurfaces( false ),
     m_siStore( aDetectorStore ),
     m_sensorStore( aSiStore ),
     m_generateBoundaryConditions( aGenerateBoundaryConditions )

{
   const Record& theSVAlignmentRecord = theFrame.record(Stream::kSVAlignment);

   if( aDetectorStore == 0 )
   {
      // Extract the base (eventually will be aligned) passive Si geometry.
      FAItem< DGDetectorStore > siStore;
      extract(theSVAlignmentRecord, siStore, SiGeomDescription::kSecondaryID);

      if( !siStore.valid() )
      {
	 m_geometryAvailable = false ;
	 return ;
      }

      m_siStore = &( *siStore ) ;
   }
   
   //Adding this 3/19/01
   const Record& theSVRadCorrectionRecord 
      = theFrame.record(SiStream::kSVRadCorrection);
   extract( theSVRadCorrectionRecord, m_SetOfCorrections);

   // Get the DGConstMaterialPtr for the gas.
   DGMaterialStore materials;
   extract( theFrame.record( Stream::kBaseGeometry ), materials );
   m_siGas = materials[ kNitrogenGasMatID ] ;


   // Get the DGConstMaterialPtr for the sensor.  Assume all sensors are
   // made of the same material.
   DGPath pathToSensor =
      m_siStore->findPathTo( SiGeomDescription::kSensorName[0], 1 ) ;
   DGConstPlacedVolumePtr placedSensorVolume =
      m_siStore->placedVolume( pathToSensor ) ;

   m_sensorMaterial = placedSensorVolume->volume()->material() ;

   // Initialize sensor store.
   if( aSiStore == 0 )
   {
      // Extract sensor store from the Frame.
      FAItem< ASiStore > sensorStore ;
      extract( theSVAlignmentRecord, sensorStore ) ;

      if( !sensorStore.valid() )
      {
	 m_geometryAvailable = false ;
	 return ;
      }

      m_sensorStore = &( *sensorStore ) ;
   }

   // Figure out radii of layer boundaries.
   for( int i = 0 ; i < SiGeomDescription::kNLayers - 1 ; ++i )
   {
      m_layerBoundaries[ i ] = 0.5 * (
	 SiGeomDescription::PointLayer[ i ].perp() +
	 SiGeomDescription::PointLayer[ i + 1 ].perp() ) ;
   }
}

void
HISiSurfaceFactory::makeSurfaces()
{
   if( m_siSurfaces->size() == 0 )
   {
      // Make four composite surfaces, one for each Si layer.
      int firstSiLayer = SiGeomDescription::kNFirstLayer ;
      for( int iSiLayer = firstSiLayer ;
	   iSiLayer < SiGeomDescription::kNLayers + firstSiLayer ;
	   ++iSiLayer )
      {
	 // If boundary conditions are not generated, then
	 // HICompositeSurface::propagateHelix() should test ALL surfaces
	 // in the composite for an intersection.
	 HICompositePlane* compositeSurface =
	    new HICompositePlane( m_generateBoundaryConditions ) ;

	 const ASiLadder& firstLadderInLayer =
	    m_sensorStore->ladder( iSiLayer, 1 ) ;
	 compositeSurface->setRadius( firstLadderInLayer.radiusInSi() ) ;

	 m_siSurfaces->push_back( compositeSurface ) ;
      }

      m_deleteSiSurfaces = true ;
   }
}


// HISiSurfaceFactory::HISiSurfaceFactory( const HISiSurfaceFactory& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HISiSurfaceFactory::~HISiSurfaceFactory()
{
   if( m_deleteSiSurfaces )
   {
      STL_VECTOR( HICompositePlane* )::iterator surfaceItr =
	 m_siSurfaces->begin() ;
      STL_VECTOR( HICompositePlane* )::iterator surfaceEnd =
	 m_siSurfaces->end() ;

      for( ; surfaceItr != surfaceEnd ; ++surfaceItr )
      {
	 ( *surfaceItr )->deleteCondition() ;
	 delete *surfaceItr ;
      }

      STL_VECTOR( HIIntersectionSurface* )::iterator otherItr =
	 m_otherSurfaces->begin() ;
      STL_VECTOR( HIIntersectionSurface* )::iterator otherEnd =
	 m_otherSurfaces->end() ;

      for( ; otherItr != otherEnd ; ++otherItr )
      {
	 ( *otherItr )->deleteCondition() ;
	 delete *otherItr ;
      }
   }

   delete m_siSurfaces ;
   delete m_otherSurfaces ;

}

//
// assignment operators
//
// const HISiSurfaceFactory& HISiSurfaceFactory::operator=( const HISiSurfaceFactory& rhs )
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

//
// Based on Klmn3FitterProxy::parseSilicon()
//

void
HISiSurfaceFactory::parseSilicon(
   DGConstVolumeType< DGVolumeParentPlaced > aParent,
   const DGTransform& transformToParent,
   HIHelixOperation* aInnerOperation,
   HIHelixOperation* aOuterOperation )
{
   int firstSiLayer = SiGeomDescription::kNFirstLayer ;

   for ( int childNum = 0 ;
         childNum < aParent->numberOfChildren() ;
         ++childNum )
   {
      DGConstPlacedVolumePtr child = aParent->getChild( childNum );
      DGTransform childTransform =
         transformToParent * child->localAlignedToParent();
      DGConstVolumeType< DGVolumeParentPlaced > childParent( child->volume() );
      if ( childParent.isValid() )
      {
         // If it's a parent, get its transform, and recurse.
         parseSilicon( childParent, childTransform,
		       aInnerOperation, aOuterOperation );
      }
      else
      {
         // If not, parse the puppy, looking for the volume and material in
         // the maps.
         DGConstVolumePtr childVolume = child->volume();
         DGConstMaterialPtr childMaterial = childVolume->material();

	 if( childMaterial == m_sensorMaterial )
	 {
	    continue ; 
	 }

         DGConstVolumeType< DGVolumeBox > childBox( childVolume );
         if ( childBox.isValid() )
         {
            // We've got half of a diamond support structure.

	    const DGVolumeBox* box( ( DGVolumeBox* ) &*childVolume ) ;
	    Meters halfWidth, halfThickness ;
	    HepNormal3D normal ;
	    HepVector3D rphiVector ;

	    if( box->halfLengthX() > box->halfLengthY() )
	    {
	       halfWidth     = box->halfLengthX() ;
	       halfThickness = box->halfLengthY() ;
	       normal        = childTransform * HepNormal3D( 0., 1., 0. ) ;
	       rphiVector    = childTransform * HepVector3D( 1., 0., 0. ) ;
	    }
	    else
	    {
	       halfWidth     = box->halfLengthY() ;
	       halfThickness = box->halfLengthX() ;
	       normal        = childTransform * HepNormal3D( 1., 0., 0. ) ;
	       rphiVector    = childTransform * HepVector3D( 0., 1., 0. ) ;
	    }

	    Meters halfZ = box->halfLengthZ() ;

	    HepPoint3D boxCenter = childTransform * HepPoint3D( 0., 0., 0. ) ;

	    // Figure out what layer these surfaces belong to.
	    int nLayer = SiGeomDescription::kNLayers - firstSiLayer ;

	    for( ; nLayer > 0 ; --nLayer )
	    {
	       if( boxCenter.perp() > m_layerBoundaries[ nLayer - 1 ] )
	       {
		  break ;
	       }
	    }

	    // Flip the normal vector if it points inwards.
	    if( normal.dot( boxCenter ) < 0. )
	    {
	       normal *= -1. ;
	    }

	    // rphiVector is the normal for the narrow edge surfaces, and
	    // always points towards the "outer" material.

	    HepPoint3D innerPoint = boxCenter - halfThickness * normal ;
	    HepPoint3D outerPoint = boxCenter + halfThickness * normal ;
	    HepPoint3D edge1Point = boxCenter - halfWidth * rphiVector ;
	    HepPoint3D edge2Point = boxCenter + halfWidth * rphiVector ;

	    // Z boundary conditions.
	    HIZBoundaryCondition* innerZBoundary =
	       new HIZBoundaryCondition( innerPoint.z() - halfZ,
					 innerPoint.z() + halfZ ) ;
	    HIZBoundaryCondition* outerZBoundary =
	       new HIZBoundaryCondition( outerPoint.z() - halfZ,
					 outerPoint.z() + halfZ ) ;
	    HIZBoundaryCondition* edge1ZBoundary =
	       new HIZBoundaryCondition( edge1Point.z() - halfZ,
					 edge1Point.z() + halfZ ) ;
	    HIZBoundaryCondition* edge2ZBoundary =
	       new HIZBoundaryCondition( edge2Point.z() - halfZ,
					 edge2Point.z() + halfZ ) ;

	    // R-phi boundary conditions.
	    HIRphiBoundaryCondition* innerRphiBoundary =
	       new HIRphiBoundaryCondition( innerPoint, rphiVector, halfWidth);
	    HIRphiBoundaryCondition* outerRphiBoundary =
	       new HIRphiBoundaryCondition( outerPoint, rphiVector, halfWidth);
	    HIRphiBoundaryCondition* edge1RphiBoundary =
	       new HIRphiBoundaryCondition( edge1Point, normal, halfThickness);
	    HIRphiBoundaryCondition* edge2RphiBoundary =
	       new HIRphiBoundaryCondition( edge2Point, normal, halfThickness);

	    // Pack these into composite conditions.
	    HICompositeCondition* innerBoundaryConditions =
	       new HICompositeCondition() ;
	    innerBoundaryConditions->addChild( innerZBoundary ) ;
	    innerBoundaryConditions->addChild( innerRphiBoundary ) ;

	    HICompositeCondition* outerBoundaryConditions =
	       new HICompositeCondition() ;
	    outerBoundaryConditions->addChild( outerZBoundary ) ;
	    outerBoundaryConditions->addChild( outerRphiBoundary ) ;

	    HICompositeCondition* edge1BoundaryConditions =
	       new HICompositeCondition() ;
	    edge1BoundaryConditions->addChild( edge1ZBoundary ) ;
	    edge1BoundaryConditions->addChild( edge1RphiBoundary ) ;

	    HICompositeCondition* edge2BoundaryConditions =
	       new HICompositeCondition() ;
	    edge2BoundaryConditions->addChild( edge2ZBoundary ) ;
	    edge2BoundaryConditions->addChild( edge2RphiBoundary ) ;


	    // Instantiate the two surfaces.
	    HIIntersectionSurface* innerSurface =
	       new HIIntersectionPlane( m_siGas,
					childMaterial,
					normal,
					innerPoint,
					halfWidth,
					innerBoundaryConditions,
					aInnerOperation ) ;

	    HIIntersectionSurface* outerSurface =
	       new HIIntersectionPlane( childMaterial,
					m_siGas,
					normal,
					outerPoint,
					halfWidth,
					outerBoundaryConditions,
					aOuterOperation ) ;

	    HIIntersectionSurface* edge1Surface =
	       new HIIntersectionPlane( m_siGas,
					childMaterial,
					rphiVector,
					edge1Point,
					halfThickness,
					edge1BoundaryConditions,
					aInnerOperation ) ;

	    HIIntersectionSurface* edge2Surface =
	       new HIIntersectionPlane( childMaterial,
					m_siGas,
					rphiVector,
					edge2Point,
					halfThickness,
					edge2BoundaryConditions,
					aOuterOperation ) ;

	    ( *m_siSurfaces )[ nLayer ]->addChild( innerSurface );
	    ( *m_siSurfaces )[ nLayer ]->addChild( outerSurface );
	    ( *m_siSurfaces )[ nLayer ]->addChild( edge1Surface );
	    ( *m_siSurfaces )[ nLayer ]->addChild( edge2Surface );
         }
         else
         {
            // It's a chamber
            DGConstVolumeType< DGVolumeTube > childTube( childVolume );
            if ( ! childTube.isValid() )
            {
               report( EMERGENCY, kFacilityString )
                  << "I don't know how to deal with this non-box,"
                  << " non-tube volume."
                  << endl;
               assert(0);
            }

	    // Make the chamber surfaces.
	    const DGVolumeTube* tube( ( DGVolumeTube* ) &*childVolume ) ;

	    HepPoint3D tubeCenter = childTransform * HepPoint3D( 0., 0., 0. ) ;
	    HIZBoundaryCondition* innerZBoundary =
	       new HIZBoundaryCondition( tubeCenter.z() - tube->halfLengthZ(),
					 tubeCenter.z() + tube->halfLengthZ());
	    HIZBoundaryCondition* outerZBoundary =
	       new HIZBoundaryCondition( tubeCenter.z() - tube->halfLengthZ(),
					 tubeCenter.z() + tube->halfLengthZ());

	    HIIntersectionCylinder* innerTube =
	       new HIIntersectionCylinder( m_siGas,
					   childMaterial,
					   tube->innerRadius(),
					   childTransform,
					   innerZBoundary,
					   aInnerOperation ) ;
	    HIIntersectionCylinder* outerTube =
	       new HIIntersectionCylinder( childMaterial,
					   m_siGas,
					   tube->outerRadius(),
					   childTransform,
					   outerZBoundary,
					   aOuterOperation ) ;

	    m_otherSurfaces->push_back( innerTube ) ;
	    m_otherSurfaces->push_back( outerTube ) ;
         }
      }
   }
   return;
}


void
HISiSurfaceFactory::generateLadderBoundarySurfaces(
   HIHelixOperation* aInnerOperation,
   HIHelixOperation* aOuterOperation )
{
   if( !m_geometryAvailable )
   {
      return ;
   }

   makeSurfaces() ;

   // Get the bounding volume for Si and find its transformation into the
   // lab frame.
   DGConstPlacedVolumePtr placedSi = m_siStore->root()->getChild( 0 ) ;
   DGConstVolumeType< DGVolumeParentPlaced > siParent = placedSi->volume() ;
   const DGTransform& transformToParent = placedSi->localAlignedToParent() ;

   // Make surfaces for the passive elements only: carbon fiber support tube
   // and diamond vees.
   parseSilicon( siParent, transformToParent,
		 aInnerOperation, aOuterOperation ) ;

   // Make surfaces for the active silicon elements: four surfaces
   // (inner and outer, two rphi edges) for each ladder.

   int firstSiLayer = SiGeomDescription::kNFirstLayer ;

   int firstLadder = SiGeomDescription::kNFirstLadder ;
   for( int iSiLadder = firstLadder ;
	iSiLadder < SiGeomDescription::kNLadders + firstLadder ;
	++iSiLadder )
   {
      const ASiLadder& ladder = m_sensorStore->ladder( iSiLadder ) ;

      HepNormal3D normal = ladder.unitVectorYInSi() ;
      Hep3Vector rphiVector = ladder.unitVectorXInSi() ;

      // Construct the z and rphi boundary conditions for this ladder --
      // in case of misalignment, make one z for each surface.

      Meters halfZ = ladder.halfLengthZ() ;
      Meters halfX = ladder.halfLengthX() ;
      Meters halfY = ladder.halfLengthY() ;

      HepPoint3D boxPoint = ladder.coorInSi() ;
      HepPoint3D innerPoint = boxPoint - halfY * normal ;
      HepPoint3D outerPoint = boxPoint + halfY * normal ;
      HepPoint3D edge1Point = boxPoint - halfX * rphiVector ;
      HepPoint3D edge2Point = boxPoint + halfX * rphiVector ;

      HICompositeCondition* innerBoundaryConditions = 0 ;
      HICompositeCondition* outerBoundaryConditions = 0 ;
      HICompositeCondition* edge1BoundaryConditions = 0 ;
      HICompositeCondition* edge2BoundaryConditions = 0 ;

      if( m_generateBoundaryConditions )
      {
	 // Z boundary conditions.
	 HIZBoundaryCondition* innerZBoundary =
	    new HIZBoundaryCondition( innerPoint.z() - halfZ,
				      innerPoint.z() + halfZ ) ;
	 HIZBoundaryCondition* outerZBoundary =
	    new HIZBoundaryCondition( outerPoint.z() - halfZ,
				      outerPoint.z() + halfZ ) ;
	 HIZBoundaryCondition* edge1ZBoundary =
	    new HIZBoundaryCondition( edge1Point.z() - halfZ,
				      edge1Point.z() + halfZ ) ;
	 HIZBoundaryCondition* edge2ZBoundary =
	    new HIZBoundaryCondition( edge2Point.z() - halfZ,
				      edge2Point.z() + halfZ ) ;

	 // R-phi boundary conditions.
	 HIRphiBoundaryCondition* innerRphiBoundary =
	    new HIRphiBoundaryCondition( innerPoint, rphiVector, halfX ) ;
	 HIRphiBoundaryCondition* outerRphiBoundary =
	    new HIRphiBoundaryCondition( outerPoint, rphiVector, halfX ) ;
	 HIRphiBoundaryCondition* edge1RphiBoundary =
	    new HIRphiBoundaryCondition( edge1Point, normal, halfY ) ;
	 HIRphiBoundaryCondition* edge2RphiBoundary =
	    new HIRphiBoundaryCondition( edge2Point, normal, halfY ) ;

	 // Pack these into composite conditions.
	 innerBoundaryConditions = new HICompositeCondition() ;
	 innerBoundaryConditions->addChild( innerZBoundary ) ;
	 innerBoundaryConditions->addChild( innerRphiBoundary ) ;

	 outerBoundaryConditions = new HICompositeCondition() ;
	 outerBoundaryConditions->addChild( outerZBoundary ) ;
	 outerBoundaryConditions->addChild( outerRphiBoundary ) ;

	 edge1BoundaryConditions = new HICompositeCondition() ;
	 edge1BoundaryConditions->addChild( edge1ZBoundary ) ;
	 edge1BoundaryConditions->addChild( edge1RphiBoundary ) ;

	 edge2BoundaryConditions =	new HICompositeCondition() ;
	 edge2BoundaryConditions->addChild( edge2ZBoundary ) ;
	 edge2BoundaryConditions->addChild( edge2RphiBoundary ) ;
      }

      // Instantiate the two surfaces.
      HIIntersectionSurface* pInnerSurface =
	 new HIIntersectionPlane( m_siGas,
				  m_sensorMaterial,
				  normal,
				  innerPoint,
				  halfX,
				  innerBoundaryConditions,
				  aInnerOperation ) ;

      HIIntersectionSurface* pOuterSurface =
	 new HIIntersectionPlane( m_sensorMaterial,
				  m_siGas,
				  normal,
				  outerPoint,
				  halfX,
				  outerBoundaryConditions,
				  aOuterOperation ) ;

      HIIntersectionSurface* pEdge1Surface =
	 new HIIntersectionPlane( m_siGas,
				  m_sensorMaterial,
				  rphiVector,
				  edge1Point,
				  halfY,
				  edge1BoundaryConditions,
				  aInnerOperation ) ;

      HIIntersectionSurface* pEdge2Surface =
	 new HIIntersectionPlane( m_sensorMaterial,
				  m_siGas,
				  rphiVector,
				  edge2Point,
				  halfY,
				  edge2BoundaryConditions,
				  aOuterOperation ) ;

      int nLayer = ladder.layerOwner().number() ;

      ( *m_siSurfaces )[ nLayer - firstSiLayer ]->addChild( pInnerSurface ) ;
      ( *m_siSurfaces )[ nLayer - firstSiLayer ]->addChild( pOuterSurface ) ;
      ( *m_siSurfaces )[ nLayer - firstSiLayer ]->addChild( pEdge1Surface ) ;
      ( *m_siSurfaces )[ nLayer - firstSiLayer ]->addChild( pEdge2Surface ) ;
   }
}


void
HISiSurfaceFactory::generateLadderCenterSurfaces(
   HIHelixOperation* aOperation )
{
   if( !m_geometryAvailable )
   {
      return ;
   }

   makeSurfaces() ;

   // Make one surface for each ladder at the center of the ladder.

   int firstLadder = SiGeomDescription::kNFirstLadder ;
   for( int iSiLadder = firstLadder ;
	iSiLadder < SiGeomDescription::kNLadders + firstLadder ;
	++iSiLadder )
   {
      const ASiLadder& ladder = m_sensorStore->ladder( iSiLadder ) ;

      // Construct the z and rphi boundary conditions for this ladder.
      Meters halfZ = ladder.halfLengthZ() ;
      Meters halfX = ladder.halfLengthX() ;
      HepPoint3D point = ladder.coorInSi() ;

      HICompositeCondition* boundaryConditions = 0 ;

      if( m_generateBoundaryConditions )
      {
	 // Z boundary condition.
	 HIZBoundaryCondition* zBoundary =
	    new HIZBoundaryCondition( point.z() - halfZ, point.z() + halfZ ) ;

	 // R-phi boundary condition.
	 Hep3Vector rphiVector = ladder.unitVectorXInSi() ;
	 HIRphiBoundaryCondition* rphiBoundary =
	    new HIRphiBoundaryCondition( point, rphiVector, halfX ) ;

	 // Pack these into composite conditions.
	 boundaryConditions = new HICompositeCondition() ;
	 boundaryConditions->addChild( zBoundary ) ;
	 boundaryConditions->addChild( rphiBoundary ) ;
      }

      // Instantiate the surface.
      HIIntersectionSurface* pSurface =
	 new HISiLadderPlane( m_sensorMaterial,
			      m_sensorMaterial,
			      ladder,
			      boundaryConditions,
			      aOperation ) ;

      int nLayer = ladder.layerOwner().number() ;
      ( *m_siSurfaces )[ nLayer - SiGeomDescription::kNFirstLayer ]->addChild(
	 pSurface ) ;
   }
}



void
HISiSurfaceFactory::generateCenterSurfacesWithHits(
   const STL_VECTOR( const CalibratedSVRphiHit* )& aSVRphiHits,
   const STL_VECTOR( const CalibratedSVZHit* )& aSVZHits,
   HIHelixOperation* aOperation,
   DABoolean aForceSVRFittingWeight,
   double aSVRFittingWeight,
   DABoolean aForceSVZFittingWeight,
   double aSVZFittingWeight )
{
   if( !m_geometryAvailable )
   {
      return ;
   }

   makeSurfaces() ;

   const CalibratedSVRphiHit* nullRphiHit = 0;
   const CalibratedSVZHit* nullZHit = 0;
      
   // Make two separate lists of ladder objects with hits
   STL_VECTOR( const ASiLadder* ) laddersWithRphiHits ;
   STL_VECTOR( const ASiLadder* ) laddersWithZHits ;
   STL_VECTOR( const CalibratedSVZHit* ) zHits ;

   //For debugging!!!
   Count rphiHitNum = aSVRphiHits.size();
   Count zHitNum = aSVZHits.size();
   Count rphiHitCount = 0;
   Count zHitCount = 0;

   // Group hits by ladder.
   typedef pair< const CalibratedSVRphiHit*, const CalibratedSVZHit* >
      SVHitPair ;
   STL_VECTOR( SVHitPair ) matchedHits ;

   STL_VECTOR( const CalibratedSVRphiHit* )::const_iterator rphiItr =
      aSVRphiHits.begin() ;
   STL_VECTOR( const CalibratedSVRphiHit* )::const_iterator lastRphi =
      aSVRphiHits.end() ;

   for( ; rphiItr != lastRphi ; ++rphiItr )
   {
      laddersWithRphiHits.push_back(
	 &( m_sensorStore->sensor( ( *rphiItr )->sensor() ).ladderOwner() ) ) ;
      matchedHits.push_back( SVHitPair( *rphiItr, nullZHit ) ) ;

   }

   STL_VECTOR( const CalibratedSVZHit* )::const_iterator zItr =
      aSVZHits.begin() ;
   STL_VECTOR( const CalibratedSVZHit* )::const_iterator lastZ =
      aSVZHits.end() ;
   STL_VECTOR( SVHitPair )::iterator matchedHitsItr;

   for( ; zItr != lastZ ; ++zItr )
   {
      const ASiLadder& zLadder =
	 m_sensorStore->sensor( ( *zItr )->sensor() ).ladderOwner() ;

      // Check if there is an rphi hit in this ladder.
      DABoolean rphiInLadder = false ;
      
      STL_VECTOR( const ASiLadder* )::iterator checkItr =
	 laddersWithRphiHits.begin() ;
      STL_VECTOR( const ASiLadder* )::iterator lastRphiLadder =
	 laddersWithRphiHits.end() ;

      matchedHitsItr = matchedHits.begin() ;

      for( ; checkItr < lastRphiLadder ; ++checkItr, ++matchedHitsItr )
      {
	 if( *checkItr == &zLadder )
	 {
	    matchedHitsItr->second = *zItr ;
	    rphiInLadder = true ;
	    laddersWithZHits.push_back( &zLadder ) ;
	    zHits.push_back( *zItr );
	    break ;
	 }
      }

      if(!rphiInLadder)
      {	
	 // Save the ladders and sensors with the z hits
	 laddersWithZHits.push_back( &zLadder );
	 zHits.push_back( *zItr);
      }

   }
//////////////////////////////////////////////////////////////////////////////

   // Loop over ladders with hits and generate surfaces.
   STL_VECTOR( const ASiLadder* )::iterator rphiLadderItr =
      laddersWithRphiHits.begin() ;

// Make the rphi hit surface and hit surface boundaries.     
   for( matchedHitsItr = matchedHits.begin() ; 
	rphiLadderItr != laddersWithRphiHits.end() ; 
	++rphiLadderItr, ++matchedHitsItr )
   {
      // Construct the z and rphi boundary conditions for this ladder.
      Meters halfZ = ( *rphiLadderItr )->halfLengthZ() ;
      Meters halfX = ( *rphiLadderItr )->halfLengthX() ;
      //Get the location of the ladder center
      HepPoint3D ladderCenter = ( *rphiLadderItr )->coorInSi() ;

      //Now add the effective radius correction
      HepPoint3D correctionInSi = effRadialCorrections( *rphiLadderItr ); 

      HepPoint3D point = ladderCenter + correctionInSi;
	 
      HICompositeCondition* rphiHitBoundaryConditions = 0 ;
      
      if( m_generateBoundaryConditions )
      {
	 // Z boundary condition.
	 HIZBoundaryCondition* zBoundary =
	    new HIZBoundaryCondition( point.z() - halfZ, point.z() + halfZ ) ;

	 // R-phi boundary condition.
	 // No correction to the unitVectorX!
	 Hep3Vector rphiVector = ( *rphiLadderItr )->unitVectorXInSi() ;
	 HIRphiBoundaryCondition* rphiBoundary =
	    new HIRphiBoundaryCondition( point, rphiVector, halfX ) ;

	 // Pack these into composite conditions.
	 rphiHitBoundaryConditions = new HICompositeCondition() ;
	 rphiHitBoundaryConditions->addChild( zBoundary ) ;
	 rphiHitBoundaryConditions->addChild( rphiBoundary ) ;

      }

      // If there is a z hit, use its sensor number because the rphi hits
      // are always placed at the outermost sensor on the hybrid, which may
      // differ from the z hit sensor.
      const ASiSensorInfo& sensorInfo = matchedHitsItr->second != 0 ?
	 m_sensorStore->sensor( matchedHitsItr->second->sensor() ) :
         m_sensorStore->sensor( matchedHitsItr->first->sensor() ) ;
      
      // If the correction is identically zero, also put the z hit on this 
      // surface, because the rest of the code does not handle two surfaces
      // at identical locations.
      const CalibratedSVZHit* tempZHit;
      
      if (correctionInSi.mag() == 0.0)
      {
	 tempZHit = matchedHitsItr->second;
	 if (matchedHitsItr->second != nullZHit)
	 { zHitCount++; }
      }
      else
      {
	 tempZHit = nullZHit;
      }

      // Instantiate the surface.
      HISiSensorPlane* pRphiHitSurface =
	 new HISiSensorPlane( m_sensorMaterial,
			      m_sensorMaterial,
			      sensorInfo,
			      rphiHitBoundaryConditions,
			      aOperation,
			      matchedHitsItr->first,
			      tempZHit, 
			      correctionInSi ) ;  
      rphiHitCount++;

      if( aForceSVRFittingWeight )
      {
	 pRphiHitSurface->setSVRFittingWeight( aSVRFittingWeight ) ;
      }

      if( aForceSVZFittingWeight )
      {
	 pRphiHitSurface->setSVZFittingWeight( aSVZFittingWeight ) ;
      }

      int nLayer = ( *rphiLadderItr )->layerOwner().number() ;
      ( *m_siSurfaces )[ nLayer - SiGeomDescription::kNFirstLayer ]->addChild(
	 pRphiHitSurface ) ;

   }

// Now do similar for Z...
   STL_VECTOR( const ASiLadder* )::iterator zLadderItr =
      laddersWithZHits.begin() ;

   for(zItr = aSVZHits.begin() ; zLadderItr != laddersWithZHits.end() ; 
       ++zLadderItr, zItr++ )
   {
      HepPoint3D correctionInSi 
	       = effRadialCorrections( *zLadderItr); 
      DABoolean surfaceMade = false;

      for( matchedHitsItr = matchedHits.begin(); matchedHitsItr !=
	      matchedHits.end(); matchedHitsItr++)
      {

	 //This assumes we put both rphi and z on the same surface already!
	 if( matchedHitsItr->second == *zItr && correctionInSi.mag() == 0 )
	 {
	    surfaceMade = true;
	    break;
	 }
	    
      }

      if ( surfaceMade ) continue;

      // Construct the z and rphi boundary conditions for this ladder.
      Meters halfZ = ( *zLadderItr )->halfLengthZ() ;
      Meters halfX = ( *zLadderItr )->halfLengthX() ;
      //Get the location of the ladder center
      HepPoint3D point = ( *zLadderItr )->coorInSi() ;

      HICompositeCondition* zHitBoundaryConditions = 0 ;

      if( m_generateBoundaryConditions )
      {
	 // Z boundary condition.
	 HIZBoundaryCondition* zBoundary =
	    new HIZBoundaryCondition( point.z() - halfZ, point.z() + halfZ ) ;

	 // R-phi boundary condition.
	 Hep3Vector rphiVector = ( *zLadderItr )->unitVectorXInSi() ;
	 HIRphiBoundaryCondition* rphiBoundary =
	    new HIRphiBoundaryCondition( point, rphiVector, halfX ) ;

	 // Pack these into composite conditions.
	 zHitBoundaryConditions = new HICompositeCondition() ;
	 zHitBoundaryConditions->addChild( zBoundary ) ;
	 zHitBoundaryConditions->addChild( rphiBoundary ) ;
        }
      
      const ASiSensorInfo& sensorInfo = m_sensorStore->
	                                sensor( (*zItr)->sensor() ) ;

      // Instantiate the surface.
      HISiSensorPlane* pZHitSurface =
	 new HISiSensorPlane( m_sensorMaterial,
			      m_sensorMaterial,
			      sensorInfo,
			      zHitBoundaryConditions,
			      aOperation,
			      nullRphiHit,
			      *zItr); //No Rphi hits on this surface!
                                      //No Radial corrections!
      zHitCount++;
      
      if( aForceSVRFittingWeight )
      {
	 pZHitSurface->setSVRFittingWeight( aSVRFittingWeight ) ;
      }

      if( aForceSVZFittingWeight )
      {
	 pZHitSurface->setSVZFittingWeight( aSVZFittingWeight ) ;
        }

      int nLayer = ( *zLadderItr )->layerOwner().number() ;
      ( *m_siSurfaces )[ nLayer - SiGeomDescription::kNFirstLayer ]->addChild(
	 pZHitSurface ) ;
   }
   
}

HepPoint3D
HISiSurfaceFactory::effRadialCorrections(const ASiLadder* aLadder )
{
   double yCorrection = 0.0;
   Count aLadderNumber = aLadder->number();

   //This is the correction in the ladder/sensor frame 
   yCorrection = m_SetOfCorrections[aLadderNumber].get_rphiRadialCorrection();
   HepPoint3D correction(0.0, yCorrection, 0.0);
   
   //This is the transformation which goes from ladder to global Si
   HepTransform3D ladderToSi = aLadder->transToSi();

   //Translate the correction to the global silicon frame 
   HepPoint3D correctionInSi = (ladderToSi * correction) 
                                       - (aLadder->coorInSi());
 
   return correctionInSi;
}

void
HISiSurfaceFactory::transferSurfacesTo(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   DABoolean aTransferEmptySurfaces )
{
   if( !m_geometryAvailable )
   {
      return ;
   }

   // Transfer the composite surfaces in m_siSurfaces to
   // the vector of surfaces to be given to the HIHelixIntersector.
   // Transfer all four, regardless of whether they have any children
   // because the passive element surfaces may be cached separately from
   // the surfaces with hits.

   STL_VECTOR( HICompositePlane* )::const_iterator compositeItr =
      m_siSurfaces->begin() ;
   STL_VECTOR( HICompositePlane* )::const_iterator lastSiSurface =
      m_siSurfaces->end() ;

   for( ; compositeItr != lastSiSurface ; ++compositeItr )
   {
      if( aTransferEmptySurfaces || ( *compositeItr )->numberOfChildren() > 0 )
      {
	 aSurfaces.push_back( *compositeItr ) ;
      }
      else
      {
	 delete *compositeItr ;
      }
   }

   // Clear the vector (WITHOUT deleting the surfaces -- the user is now
   // responsible for deleting these).
   m_siSurfaces->clear() ;

   STL_VECTOR( HIIntersectionSurface* )::const_iterator otherItr =
      m_otherSurfaces->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::const_iterator otherEnd =
      m_otherSurfaces->end() ;

   for( ; otherItr != otherEnd ; ++otherItr )
   {
      aSurfaces.push_back( *otherItr ) ;
   }

   m_otherSurfaces->clear() ;

   // Don't delete the composite surfaces -- they are now owned by the user.
   m_deleteSiSurfaces = false ;
}

void
HISiSurfaceFactory::getSVRHits(
   const TRSeedTrack* aSeed,
   STL_VECTOR( const CalibratedSVRphiHit* )& aSVRHits,
   const UsageTag& aUsageTag,
   const ProductionTag& aProdTag ) const
{
   const Record& theEventRecord = m_frame.record( Stream::kEvent ) ;

   // Get the SeedTrack/CalibratedSVRphiHit Lattice.
   // (typedefs in GetSVXHitsProc/*Lattice.h)
   FAItem< SeedTrackSVRHitLattice > pSeedSVRHitLattice ;

   try
   {
      extract( theEventRecord, pSeedSVRHitLattice,
	       aUsageTag.value(), aProdTag.value() ) ;
   }
   catch( NO_ITEM_EXCEPTION( SeedTrackSVRHitLattice )& aException )
   {
      report( WARNING, kFacilityString )
        << "Could not extract SeedTrackSVRHitLattice with usage tag "
        << aUsageTag.value() << " and production tag " << aProdTag.value()
        << ".  Will extract default SeedTrackSVRHitLattice." << endl ;

      extract( theEventRecord, pSeedSVRHitLattice ) ;
   }

   if( !pSeedSVRHitLattice.valid() )
   {
      return ;
   }

   // Extract FATable of all SVR hits.
   FATable< CalibratedSVRphiHit > svrHits ;

   try
   {
      extract( theEventRecord, svrHits, aUsageTag.value(), aProdTag.value() ) ;
   }
   catch( NO_TABLE_EXCEPTION( CalibratedSVRphiHit )& aException )
   {
      report( WARNING, kFacilityString )
        << "Could not extract CalibratedSVRphiHits with usage tag "
        << aUsageTag.value() << " and production tag " << aProdTag.value()
        << ".  Will extract default CalibratedSVRphiHits." << endl ;

      extract( theEventRecord, svrHits ) ;
   }

   if( !svrHits.valid() )
   {
      return ;
   }

   // Get the iterator to the associated CalibratedSVRphiHits.
   const SeedTrackSVRHitLattice::VectorRightID* svrHitsOnTrack =
      pSeedSVRHitLattice->vRightGivenLeft( aSeed->identifier() ) ;

   if( svrHitsOnTrack == 0 || svrHitsOnTrack->size() <= 0 )
   {
      return ;
   }

   // Loop over SVR hits on track.
   SeedTrackSVRHitLattice::RightItr svrHitItr = svrHitsOnTrack->begin() ;
   SeedTrackSVRHitLattice::RightItr svrHitEnd = svrHitsOnTrack->end() ;

   for( ; svrHitItr != svrHitEnd ; ++svrHitItr )
   {
      aSVRHits.push_back( &( svrHits[ *svrHitItr ] ) ) ;
   }
}


void
HISiSurfaceFactory::getSVZHits(
   const TRSeedTrack* aSeed,
   STL_VECTOR( const CalibratedSVZHit* )& aSVZHits,
   const UsageTag& aUsageTag,
   const ProductionTag& aProdTag ) const
{
   const Record& theEventRecord = m_frame.record( Stream::kEvent ) ;

   // Get the SeedTrack/CalibratedSVZHit Lattice.
   FAItem< SeedTrackSVZHitLattice > pSeedSVZHitLattice ;

   try
   {
      extract( theEventRecord, pSeedSVZHitLattice,
	       aUsageTag.value(), aProdTag.value() ) ;
   }
   catch( NO_ITEM_EXCEPTION( SeedTrackSVZHitLattice )& aException )
   {
      report( WARNING, kFacilityString )
        << "Could not extract SeedTrackSVZHitLattice with usage tag "
        << aUsageTag.value() << " and production tag " << aProdTag.value()
        << ".  Will extract default SeedTrackSVZHitLattice." << endl ;

      extract( theEventRecord, pSeedSVZHitLattice ) ;
   }


   if( !pSeedSVZHitLattice.valid() )
   {
      return ;
   }

   // Extract FATable of all SVZ hits.
   FATable< CalibratedSVZHit > svzHits ;

   try
   {
      extract( theEventRecord, svzHits, aUsageTag.value(), aProdTag.value() ) ;
   }
   catch( NO_TABLE_EXCEPTION( CalibratedSVZHit )& aException )
   {
      report( WARNING, kFacilityString )
        << "Could not extract CalibratedSVZHits with usage tag "
        << aUsageTag.value() << " and production tag " << aProdTag.value()
        << ".  Will extract default CalibratedSVZHits." << endl ;

      extract( theEventRecord, svzHits ) ;
   }

   if( !svzHits.valid() )
   {
      return ;
   }

   // Get the iterator to the associated CalibratedSVRphiHits.
   const SeedTrackSVZHitLattice::VectorRightID* svzHitsOnTrack =
      pSeedSVZHitLattice->vRightGivenLeft( aSeed->identifier() ) ;

   if( svzHitsOnTrack == 0 || svzHitsOnTrack->size() <= 0 )
   {
      return ;
   }

   // Loop over SVZ hits on track.
   SeedTrackSVZHitLattice::RightItr svzHitItr = svzHitsOnTrack->begin() ;
   SeedTrackSVZHitLattice::RightItr svzHitEnd = svzHitsOnTrack->end() ;

   for( ; svzHitItr != svzHitEnd ; ++svzHitItr )
   {
      aSVZHits.push_back( &( svzHits[ *svzHitItr ] ) ) ;
   }
}

//
// static member functions
//
