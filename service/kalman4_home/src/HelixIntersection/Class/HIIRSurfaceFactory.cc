// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIIRSurfaceFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Aug 29 22:36:35 EDT 1999
// $Id: HIIRSurfaceFactory.cc,v 1.3 2002/11/21 14:04:20 mccann Exp $
//
// Revision history
//
// $Log: HIIRSurfaceFactory.cc,v $
// Revision 1.3  2002/11/21 14:04:20  mccann
// Beampipe was made infinitely long (HIIRSurfaceFactory.cc) to protect it from changes in IRGeomProd.
//
// Revision 1.2  2001/03/26 07:03:08  wsun
// Changed air to nitrogen.
//
// Revision 1.1  1999/08/30 21:32:46  wsun
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
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "HelixIntersection/HIIRSurfaceFactory.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/FAItem.h"
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

#include "IRGeom/IRGeomParameters.h"

#include "HelixIntersection/HIIntersectionCylinder.h"
#include "HelixIntersection/HIZBoundaryCondition.h"

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

static const char* const kFacilityString = "HelixIntersection.HIIRSurfaceFactory" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIIRSurfaceFactory.cc,v 1.3 2002/11/21 14:04:20 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIIRSurfaceFactory::HIIRSurfaceFactory( Frame& theFrame,
					const DGDetectorStore* aDetectorStore )
   : m_frame( theFrame ),
     m_geometryAvailable( true ),
     m_irStore( aDetectorStore )
{
   // Initialize the tube pointers.
   for( int i = 0 ; i < kNumberOfTubes ; ++i )
   {
      m_tubes[ i ] = 0 ;
   }

   // Extract the IR DGDetectorStore.
   const Record& baseGeomRecord = theFrame.record( Stream::kBaseGeometry ) ;

   if( aDetectorStore == 0 )
   {
      FAItem< DGDetectorStore > irStore;
      extract( baseGeomRecord, irStore, IRGeomParameters::kUsageTag );

      if( !irStore.valid() )
      {
	 m_geometryAvailable = false ;
	 return ;
      }

      m_irStore = &( *irStore ) ;
   }
}

// HIIRSurfaceFactory::HIIRSurfaceFactory( const HIIRSurfaceFactory& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIIRSurfaceFactory::~HIIRSurfaceFactory()
{
}

//
// assignment operators
//
// const HIIRSurfaceFactory& HIIRSurfaceFactory::operator=( const HIIRSurfaceFactory& rhs )
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
HIIRSurfaceFactory::parseIR(
   DGConstVolumeType< DGVolumeParentPlaced > aParent,
   const DGTransform& transformToParent )
{
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
         parseIR( childParent, childTransform );
      }
      else
      {
         // If not, parse the puppy, looking for the volume and material in
         // the maps.
         DGConstVolumePtr childVolume = child->volume();

	 // Test if this volume is a tube.
	 DGConstVolumeType< DGVolumeTube > childTube( childVolume );
	 if ( !childTube.isValid() )
	 {
	    continue ;
	 }

	 const DGVolumeTube* tube( ( DGVolumeTube* ) &*childVolume ) ;

	 // See if this is one of the four volumes we're interested in.
	 const string& volumeName = childVolume->name() ;
	 if( volumeName == IRGeomParameters::kOuterBePipeVolName )
	 {
	    m_tubes[ 3 ]      = tube ;
	    m_transforms[ 3 ] = childTransform ;
	 }
	 else if( volumeName == IRGeomParameters::kCoolantPipeVolName )
	 {
	    m_tubes[ 2 ]      = tube ;
	    m_transforms[ 2 ] = childTransform ;
	 }
	 else if( volumeName == IRGeomParameters::kInnerBePipeVolName )
	 {
	    m_tubes[ 1 ]      = tube ;
	    m_transforms[ 1 ] = childTransform ;
	 }
	 else if( volumeName == IRGeomParameters::kAuLiningVolName )
	 {
	    m_tubes[ 0 ]      = tube ;
	    m_transforms[ 0 ] = childTransform ;
	 }
      }
   }
   return ;
}

//
// const member functions
//


// Only generate surfaces for the inner and outer beryllium cylinders,
// the water between them, and the gold inner coating.
void
HIIRSurfaceFactory::generateSurfaces(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   HIHelixOperation* aOperation )
{
   // Get the bounding volume for IR3 and find its transformation into the
   // lab frame.
   DGConstPlacedVolumePtr placedIR = m_irStore->root()->getChild( 0 ) ;
   DGConstVolumeType< DGVolumeParentPlaced > irParent = placedIR->volume() ;
   const DGTransform& transformToParent = placedIR->localAlignedToParent() ;

   // Get the beampipe tubes and make sure they are adjacent to each other.
   parseIR( irParent, transformToParent ) ;

   for( int i = 0 ; i < kNumberOfTubes ; ++i )
   {
      if( m_tubes[ i ] == 0 )
      {
	 return ;
      }
   }

   for( int j = 0 ; j < kNumberOfTubes - 1 ; ++j )
   {
      if( m_tubes[ j ]->outerRadius() != m_tubes[ j + 1 ]->innerRadius() )
      {
	 return ;
      }
   }

   // Get the material store for the vaccuum and air materials.
   DGMaterialStore materials;
   extract( m_frame.record( Stream::kBaseGeometry ), materials );

   // Make the HIIntersectionSurface for the gold/vaccuum boundary.
   HepPoint3D center = m_transforms[ 0 ] * HepPoint3D( 0., 0., 0. ) ;
   const DGVolumeTube* tube  = m_tubes[ 0 ] ;

   // Jim Pivarski Nov 18, 2002: The beampipe was changed in
   // IRGeomProd to consist of two independent objects, and parseIR
   // (above) missed them in the list, since it is only looking for
   // one object of each beampipe layer.  Therefore, Kalman only saw a
   // beampipe on the west side of the detector, and didn't get to
   // apply scattering to it.  The beampipe, effectively infinite in
   // extent, shouldn't have a z bounary condition anyway, so I will
   // set all halfZ's to a very large value.  IRGeomProd continues to
   // express the radii, but can no longer shrink the beampipe in z,
   // intentionally or accidentally.
   // 
   // Meters halfZ = tube->halfLengthZ() ;
   // 
   Meters halfZ = 100.;
   // end of comment

   DGConstMaterialPtr innerMaterial = materials[ kVacuumMatID ] ;
   DGConstMaterialPtr outerMaterial = tube->material() ;

   HIIntersectionCylinder* cylinder = new HIIntersectionCylinder(
      innerMaterial,
      outerMaterial,
      tube->innerRadius(),
      m_transforms[ 0 ],
      new HIZBoundaryCondition( center.z() - halfZ, center.z() + halfZ ),
      aOperation ) ;

//    cout << "IR cylinder r " << tube->innerRadius()
// 	<< " halfZ " << halfZ << " mat "
// 	<< innerMaterial->name() << "/" << outerMaterial->name() << endl ;

   aSurfaces.push_back( cylinder ) ;

   // Make the other cylinders.
   for( int k = 0 ; k < kNumberOfTubes ; ++k )
   {
      center = m_transforms[ k ] * HepPoint3D( 0., 0., 0. ) ;
      tube  = m_tubes[ k ] ;

      // See the comment above (Jim Pivarski Nov 18, 2002).
      //
      // halfZ = tube->halfLengthZ() ;
      // 
      halfZ = 100.;
      // end of comment

      innerMaterial = tube->material() ;

      if( k == kNumberOfTubes - 1 )
      {
	 outerMaterial = materials[ kNitrogenGasMatID ] ;
      }
      else
      {
	 outerMaterial = m_tubes[ k + 1 ]->material() ;
      }

      HIIntersectionCylinder* cylinder = new HIIntersectionCylinder(
	 innerMaterial,
	 outerMaterial,
	 tube->outerRadius(),
	 m_transforms[ k ],
	 new HIZBoundaryCondition( center.z() - halfZ, center.z() + halfZ ),
	 aOperation ) ;

//       cout << "IR cylinder r " << tube->outerRadius()
// 	   << " halfZ " << halfZ << " mat "
// 	   << innerMaterial->name() << "/" << outerMaterial->name() << endl ;

      aSurfaces.push_back( cylinder ) ;
   }
}

//
// static member functions
//
