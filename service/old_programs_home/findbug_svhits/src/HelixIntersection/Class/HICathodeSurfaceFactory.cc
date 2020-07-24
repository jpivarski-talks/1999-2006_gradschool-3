// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICathodeSurfaceFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Sep  5 17:10:53 EDT 1999
// $Id: HICathodeSurfaceFactory.cc,v 1.9 2001/03/26 06:45:12 wsun Exp $
//
// Revision history
//
// $Log: HICathodeSurfaceFactory.cc,v $
// Revision 1.9  2001/03/26 06:45:12  wsun
// HePr --> vaporized wire gas
//
// Revision 1.8  2000/11/18 00:12:44  wsun
// Add member data for fitting weight scaling. Updated HICathodeCylinder ctor.
//
// Revision 1.7  2000/09/05 20:34:50  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.6  2000/08/16 19:59:27  wsun
// Extract seed/hit lattices with usage and production tags.
//
// Revision 1.5  2000/07/25 21:34:32  wsun
// User can specify usage and and production tags for hits.
//
// Revision 1.4  2000/07/11 03:00:36  wsun
// Moved cathode hit radius to last DR layer.
//
// Revision 1.3  2000/03/02 08:22:15  wsun
// Extract DR geometry from dralignment instead of basegeometry stream.
//
// Revision 1.2  1999/10/13 08:57:44  wsun
// Allow user to give a ADRCathodeStore to the ctor.
//
// Revision 1.1  1999/09/10 21:19:49  wsun
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
#include "Experiment/report.h"

#include "HelixIntersection/HICathodeSurfaceFactory.h"
#include "HelixIntersection/HIZBoundaryCondition.h"
#include "HelixIntersection/HICathodeCylinder.h"

#include "TrackFinder/SeedTrackCathodeHitLattice.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGPath.h"
#include "DetectorGeometry/DGConstVolumePtr.h"
#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGConstPlacedVolumePtr.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGMaterialStore.h"
#include "MaterialDelivery/material_identifiers.h"

#include "ADRGeom/ADRSenseWireStore.h"
#include "ADRGeom/ADRCathodeStore.h"
#include "DRGeom/DRGeometryParams.h"
#include "CalibratedData/CalibratedCathodeHit.h"

#include "DAException/DANoDataException.h"

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
"HelixIntersection.HICathodeSurfaceFactory" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HICathodeSurfaceFactory.cc,v 1.9 2001/03/26 06:45:12 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HICathodeSurfaceFactory::HICathodeSurfaceFactory(
   Frame& aFrame,
   const ADRCathodeStore* aCathodeStore )
   : m_frame( aFrame ),
     m_geometryAvailable( true ),
     m_cathodeStore( aCathodeStore ),
     m_radius( 0. )
{
   // The records in the Frame from which we need information.
   const Record& drAlignmentRecord = aFrame.record( Stream::kDRAlignment ) ;

   // Extract the base (eventually will be aligned) passive DR geometry.
   if( m_cathodeStore == 0 )
   {
      FAItem< ADRCathodeStore > cathodeStore;
      extract( drAlignmentRecord, cathodeStore ) ;

      if( !cathodeStore.valid() )
      {
	 report( ERROR, kFacilityString )
	    << "No ADRCathodeStore in the Frame!" << endl ;
	 m_geometryAvailable = false ;
	 return ;
      }

      m_cathodeStore = &( *cathodeStore ) ;
   }


   // Extract the base (eventually will be aligned) passive DR geometry.
   // Needed for the gas.
   FAItem< DGDetectorStore > drStore;
   extract( drAlignmentRecord, drStore, DRGeometryParams::kSecondaryID );

   if( !drStore.valid() )
   {
      report( ERROR, kFacilityString )
	 << "No DR DetectorStore in the Frame!" << endl ;
      m_geometryAvailable = false ;
      return ;
   }

//    // Get the DGConstMaterialPtr for the DR gas -- at the moment this is
//    // only He-Pr.  Wires to be vaporized.
//    DGPath pathToDR = drStore->findPathTo( DRGeometryParams::kDRName, 1 );
//    DGConstPlacedVolumePtr placedDRvolume = drStore->placedVolume( pathToDR ) ;
//    m_drGas = placedDRvolume->volume()->material() ;

   // Get the material store for the vaporized wire gas.
   DGMaterialStore materials;
   extract( m_frame.record( Stream::kBaseGeometry ), materials );
   m_drGas = materials[ kGasWireVaporMatID ] ;

   // Get the DGConstMaterialPtr for the aluminum.
   DGPath pathToAl =
      drStore->findPathTo( DRGeometryParams::kCathodeAluminumPieceName, 1 );
   DGConstPlacedVolumePtr placedAlVolume = drStore->placedVolume( pathToAl ) ;
   m_aluminum = placedAlVolume->volume()->material() ;


   // Extract the DRSenseWireStore for the radius of the last anode layer.
   FAItem< ADRSenseWireStore > wireStore ;
   extract( drAlignmentRecord, wireStore ) ;

   DABoolean success ;
   int lastLayer =
      ADRSenseWireStore::kFirstLayer - 1 + ADRSenseWireStore::kNumberOfLayers ;
   Meters tmpRadius = wireStore->radiusAtz( success,
					    lastLayer,
					    wireStore->zMax( lastLayer ) ) ;

   if( success )
   {
      // Increase cathode radius by 5 microns to avoid collision with
      // last DR layer.
      m_radius = tmpRadius + 0.000005 ;
   }
   else
   {
      report( ERROR, kFacilityString )
	 << "Could not find last DR layer radius." << endl ;
   }
}


// HICathodeSurfaceFactory::HICathodeSurfaceFactory( const HICathodeSurfaceFactory& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICathodeSurfaceFactory::~HICathodeSurfaceFactory()
{
}

//
// assignment operators
//
// const HICathodeSurfaceFactory& HICathodeSurfaceFactory::operator=( const HICathodeSurfaceFactory& rhs )
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

void
HICathodeSurfaceFactory::generateCylinderWithoutHits(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   HIHelixOperation* aOperation ) const
{ 
   const ADRCathodePanel& panel =
      m_cathodeStore->panel( ADRCathodeStore::kFirstPanel ) ;

//    // Get the inner cathode radius.
//    Meters radius = panel.panelRadius() ;

   if( m_radius == 0. )
   {
      return ;
   }

   HIIntersectionCylinder* cathode =
      new HIIntersectionCylinder( m_drGas,
				  m_aluminum,
				  m_radius,
				  DGTransform::Identity,
				  new HIZBoundaryCondition( panel.zMin(),
							    panel.zMax() ),
				  aOperation ) ;
   if( cathode == 0 )
   {
      return ;
   }

   aSurfaces.push_back( cathode ) ;
}

void
HICathodeSurfaceFactory::generateCylinderWithHits(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( const CalibratedCathodeHit* )& aHits,
   HIHelixOperation* aOperation,
   DABoolean aForceFittingWeight,
   double aFittingWeight,
   DABoolean aScaleFittingWeight,
   double aFittingWeightScale ) const
{
   if( m_radius == 0. )
   {
      return ;
   }

   const ADRCathodePanel& panel =
      m_cathodeStore->panel( ADRCathodeStore::kFirstPanel ) ;

//    // Get the inner cathode radius.
//    Meters radius = panel.panelRadius() ;

   // Use the average of the two phi edges as the measurement direction.
   Hep3Vector measurementDirection =
      ( ( panel.cornerWestPhiMin() - panel.cornerEastPhiMin() +
	  panel.cornerWestPhiMax() - panel.cornerEastPhiMax() ) ).unit() ;

   // Get the sense wire store.
   FAItem< ADRSenseWireStore > wireStore ;
   extract( m_frame.record( Stream::kDRAlignment ), wireStore ) ;

   HICathodeCylinder* newCyl = new HICathodeCylinder(
      m_drGas,
      m_aluminum,
      m_radius,
      DGTransform::Identity,
      new HIZBoundaryCondition( panel.zMin(),
				panel.zMax() ),
      aOperation,
      measurementDirection,
      &aHits,
      &*wireStore,
      aScaleFittingWeight,
      aFittingWeightScale ) ;

   if( aForceFittingWeight )
   {
      newCyl->setFittingWeight( aFittingWeight ) ;
   }

   aSurfaces.push_back( newCyl ) ;
}

void
HICathodeSurfaceFactory::getCathodeHits(
   const TRSeedTrack* aSeed,
   STL_VECTOR( const CalibratedCathodeHit* )& aHits,
   const UsageTag& aUsageTag,
   const ProductionTag& aProdTag ) const
{
   const Record& eventRecord = m_frame.record( Stream::kEvent ) ;

   // Get the SeedTrack/CalibratedCathodeHit Lattice.
   FAItem< SeedTrackCathodeHitLattice > pSeedCathodeHitLattice ;

   try
   {
      extract( eventRecord, pSeedCathodeHitLattice,
	       aUsageTag.value(), aProdTag.value() ) ;
   }
   catch( NO_ITEM_EXCEPTION( SeedTrackCathodeHitLattice )& aException )
   {
      report( WARNING, kFacilityString )
	<< "Could not extract SeedTrackCathodeHitLattice with usage tag "
	<< aUsageTag.value() << " and production tag " << aProdTag.value()
	<< ".  Will extract default SeedTrackCathodeHitLattice." << endl ;

      extract( eventRecord, pSeedCathodeHitLattice ) ;
   }

   if( !pSeedCathodeHitLattice.valid() )
   {
      return ;
   }

   // Extract table of all cathode hits.
   FATable< CalibratedCathodeHit > cathodeHits ;

   try
   {
      extract( eventRecord, cathodeHits, aUsageTag.value(), aProdTag.value() );
   }
   catch( NO_TABLE_EXCEPTION( CalibratedCathodeHit )& aException )
   {
      report( WARNING, kFacilityString )
	<< "Could not extract CalibratedCathodeHits with usage tag "
	<< aUsageTag.value() << " and production tag " << aProdTag.value()
	<< ".  Will extract default CalibratedCathodeHits." << endl ;

      extract( eventRecord, cathodeHits ) ;
   }

   if( !cathodeHits.valid() )
   {
      return ;
   }


   // The list of CalibratedCathodeHits for this seed track.
   const SeedTrackCathodeHitLattice::VectorRightID* cathodeHitsOnTrack =
      pSeedCathodeHitLattice->vRightGivenLeft( aSeed->identifier() ) ;

   if( cathodeHitsOnTrack == 0 || cathodeHitsOnTrack->size() == 0 )
   {
      report( DEBUG, kFacilityString )
	 << "No cathode hits on this track." << endl ;
      return ;
   }

   // Loop over links to this seed track.
   SeedTrackCathodeHitLattice::RightItr cathodeHitsItr =
      cathodeHitsOnTrack->begin();
   SeedTrackCathodeHitLattice::RightItr endCathodeHit =
      cathodeHitsOnTrack->end() ;

   for( ; cathodeHitsItr != endCathodeHit ; ++cathodeHitsItr )
   {
      // Retrieve the hit for this link.
      const CalibratedCathodeHit& hitOnTrack = cathodeHits[ *cathodeHitsItr ] ;

      // Insert this hit into the vector to be given to
      // the HICathodeSurfaceFactory.
      aHits.push_back( &hitOnTrack ) ;
   }
}

//
// static member functions
//
