// -*- C++ -*-
//
// Pakcage:     <HelixIntersection>
// Module:      HIDRSurfaceFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Apr  2 17:28:29 EST 1999
// $Id: HIDRSurfaceFactory.cc,v 1.29 2001/07/07 18:48:35 wsun Exp $
//
// Revision history
//
// $Log: HIDRSurfaceFactory.cc,v $
// Revision 1.29  2001/07/07 18:48:35  wsun
// Updated to new Lattice interface.
//
// Revision 1.28  2001/03/26 06:58:16  wsun
// Added generateInnerTubeCylinders().
//
// Revision 1.27  2001/02/26 01:00:55  cdj
// now gets DRHitCorrector from StartRun
//
// Revision 1.26  2000/12/08 02:07:16  pappas
// protect against divide by zero
//
// Revision 1.25  2000/12/02 01:30:27  wsun
// Added avg chg on track to HICalibratedDRHit and DRHitAndDriftDistance.
//
// Revision 1.24  2000/10/29 02:39:53  wsun
// * Switch DR gas from He-Pr to mixture with vaporized sense and field wires.
// * Added flag to getDRHits() to ignore hits not to be used in fit.
//
// Revision 1.23  2000/10/11 05:53:16  wsun
// Corrected previous log comment.
//
// Revision 1.22  2000/10/11 05:25:56  wsun
// Cosmetic changes accidentally checked in.
//
// Revision 1.21  2000/09/05 20:34:50  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.20  2000/08/16 19:58:23  wsun
// Get HitCorrector from beginrun instead of dralignment stream.
//
// Revision 1.19  2000/07/25 21:34:16  wsun
// User can specify usage and and production tags for hits.
//
// Revision 1.18  2000/06/09 23:57:58  wsun
// Updated to new HIMultiWireCylinder interface.
//
// Revision 1.17  2000/06/09 00:27:58  wsun
// Updated to new HitCorrector.
//
// Revision 1.16  2000/04/06 19:59:03  wsun
// Was not actually using aligned DR geometry before.
//
// Revision 1.15  2000/03/02 08:22:16  wsun
// Extract DR geometry from dralignment instead of basegeometry stream.
//
// Revision 1.14  2000/02/22 17:20:48  wsun
// Allow users to set HISingleWireLayer::m_checkDirectionSwitch.
//
// Revision 1.13  2000/02/03 07:28:35  wsun
// Added use of HICalibratedHits.
//
// Revision 1.12  1999/12/15 20:40:59  wsun
// Disable DRHitCorrector for now.
//
// Revision 1.11  1999/11/21 22:31:22  wsun
// Remove sign adjustment for drift dist of CorrectedDRHits.
//
// Revision 1.10  1999/10/13 08:57:09  wsun
// Allow user to give a ADRSenseWireStore to the ctor.
//
// Revision 1.9  1999/10/05 23:06:09  wsun
// Added use of DRHitCorrector.
//
// Revision 1.8  1999/08/03 21:46:54  wsun
// Added functions for generating single-/multi-wire cylinders.
//
// Revision 1.7  1999/07/23 19:50:34  wsun
// Adapted to changes in Lattice interface.
//
// Revision 1.6  1999/06/30 22:24:19  wsun
// Updated to new HIIntersectionCylinder interface.
//
// Revision 1.5  1999/06/18 19:25:55  wsun
// More improvements to low momentum fitting.
//
// Revision 1.4  1999/06/02 23:05:20  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/05/14 01:06:43  wsun
// Protect against unavailability of geometry.
//
// Revision 1.2  1999/04/28 22:57:20  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:06:23  wsun
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

// user include files
#include "Experiment/report.h"

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

#include "DRGeom/DRGeometryParams.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "ADRGeom/ADRWireAddress.h"
#include "ActiveElements/AEWireLayerCylindrical.h"
#include "ActiveElements/AEWireInfo.h"

#include "DRHitCorrectorProd/HitCorrector.h"

#include "HelixIntersection/HIDRSurfaceFactory.h"
#include "HelixIntersection/HISingleWireCylinder.h"
#include "HelixIntersection/HIMultiWireCylinder.h"
#include "HelixIntersection/HIZBoundaryCondition.h"

#include "TrackRoot/TRSeedTrack.h"
#include "TrackFinder/SeedDRHitLink.h"

#include "DAException/DANoDataException.h"

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

static const char* const kFacilityString = "HIDRSurfaceFactory" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIDRSurfaceFactory.cc,v 1.29 2001/07/07 18:48:35 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIDRSurfaceFactory::HIDRSurfaceFactory( Frame& theFrame,
					DABoolean useAxialLayers,
					DABoolean useStereoULayers,
					DABoolean useStereoVLayers,
					DABoolean correctDRHits,
					const ADRSenseWireStore* aWireStore )
   : m_geometryAvailable( true ),
     m_frame( theFrame ),
     m_wireStore( aWireStore ),
     m_useAxialLayers( useAxialLayers ),
     m_useStereoULayers( useStereoULayers ),
     m_useStereoVLayers( useStereoVLayers ),
     m_correctDRHits( correctDRHits ),
     m_countHitsInLayer( false ),
     m_hitCorrector( 0 )
{
   // The records in the Frame from which we need information.
   const Record& drAlignmentRecord = theFrame.record( Stream::kDRAlignment ) ;

   // Extract the base (eventually will be aligned) passive DR geometry.
   // This is only used to get the DR gas material.
   FAItem< DGDetectorStore > drStore;
   extract( drAlignmentRecord, drStore, DRGeometryParams::kSecondaryID ) ;

   if( !drStore.valid() )
   {
      report( ERROR, kFacilityString )
	 << "No DR DetectorStore in the Frame!" << endl ;
      m_geometryAvailable = false ;
      return ;
   }
      
//    // Get the DGConstMaterialPtr for the DR gas -- at the moment this is
//    // only He-Pr.  Wires to be vaporized.
//    DGPath pathToDR =
//       drStore->findPathTo( DRGeometryParams::kDRName, 1 );
//    DGConstPlacedVolumePtr placedDRvolume =
//       drStore->placedVolume( pathToDR ) ;
//    m_drGas = placedDRvolume->volume()->material() ;

   // Get the material store for the vaporized wire gas.
   DGMaterialStore materials;
   extract( m_frame.record( Stream::kBaseGeometry ), materials );
   m_drGas = materials[ kGasWireVaporMatID ] ;
//   m_drGas = materials[ kHeliumPropaneMatID ] ;

   // Extract sense wire store from the Frame.
   if( m_wireStore == 0 )
   {
      FAItem< ADRSenseWireStore > wireStore ;
      extract( drAlignmentRecord, wireStore ) ;

      if( !wireStore.valid() )
      {
	 report( ERROR, kFacilityString ) << "No DR wire store in the Frame!"
					  << endl ;
	 m_geometryAvailable = false ;
	 return ;
      }
      m_wireStore = &( *wireStore ) ;
   }

   // Initialize array m_hitsInLayer.
   clearHitsInLayer() ;

   // Fill pointer to HitCorrector if necessary.
   FAItem< HitCorrector > hitCorrector ;
   if( m_correctDRHits )
   {
//      extract( m_frame.record( Stream::kDRCalibration ), hitCorrector ) ;
      extract( m_frame.record( Stream::kStartRun ), hitCorrector ) ;
      m_hitCorrector = &*hitCorrector ;
   }
}

// HIDRSurfaceFactory::HIDRSurfaceFactory( const HIDRSurfaceFactory& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIDRSurfaceFactory::~HIDRSurfaceFactory()
{
}

//
// assignment operators
//
// const HIDRSurfaceFactory& HIDRSurfaceFactory::operator=( const HIDRSurfaceFactory& rhs )
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
HIDRSurfaceFactory::clearHitsInLayer()
{
   for( int i = 0 ;
	i < ADRSenseWireStore::kFirstLayer+ADRSenseWireStore::kNumberOfLayers ;
	++i )
   {
      m_hitsInLayer[ i ] = 0 ;
   }
}

//
// const member functions
//
void
HIDRSurfaceFactory::generateSingleWireSurfaces(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
   HIHelixOperation* aOperation,
   DABoolean aCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight )
{
   if( !m_geometryAvailable )
   {
      return ;
   }

   STL_VECTOR( DRHitAndDriftDistance )::const_iterator hitInfoItr =
      aDRHitInfos.begin() ;
   STL_VECTOR( DRHitAndDriftDistance )::const_iterator lastHit =
      aDRHitInfos.end() ;

   for( ; hitInfoItr != lastHit ; ++hitInfoItr )
   {
      HIIntersectionSurface* pSurface =
	 newSingleWireSurface( *hitInfoItr,
			       aOperation,
			       aCheckDirectionSwitch,
			       aForceFittingWeight,
			       aFittingWeight ) ;

      if( pSurface != 0 )
      {
	 aSurfaces.push_back( pSurface ) ;

	 if( m_countHitsInLayer )
	 {
	    ++m_hitsInLayer[ hitInfoItr->calibratedDRHit()->layer() ] ;
	 }
      }
   }
}

void
HIDRSurfaceFactory::generateMultiWireSurfaces(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
   HIHelixOperation* aOperation,
   DABoolean aSingleCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight )
{
   if( !m_geometryAvailable )
   {
      return ;
   }

   // Group the DRHitAndDriftDistances by layer.
   STL_VECTOR( DRHitAndDriftDistance )::const_iterator hitInfoItr =
      aDRHitInfos.begin() ;
   STL_VECTOR( DRHitAndDriftDistance )::const_iterator lastHit =
      aDRHitInfos.end() ;

   STL_VECTOR( const DRHitAndDriftDistance* ) hitInfosInLayer[
      ADRSenseWireStore::kFirstLayer + ADRSenseWireStore::kNumberOfLayers ] ;

   for( ; hitInfoItr != lastHit ; ++hitInfoItr )
   {
      int iLayer = hitInfoItr->calibratedDRHit()->layer() ;
      hitInfosInLayer[ iLayer ].push_back( &( *hitInfoItr ) ) ;

      if( m_countHitsInLayer )
      {
	 ++m_hitsInLayer[ iLayer ] ;
      }
   }

   // Loop through layers and make the appropriate surface for each.
   for( int i = ADRSenseWireStore::kFirstLayer ;
	i < ADRSenseWireStore::kFirstLayer+ADRSenseWireStore::kNumberOfLayers ;
	++ i )
   {
      HIIntersectionSurface* pSurface = 0 ;

      if( hitInfosInLayer[ i ].size() == 1 )
      {
	 pSurface = newSingleWireSurface( **hitInfosInLayer[ i ].begin(),
					  aOperation,
					  aSingleCheckDirectionSwitch,
					  aForceFittingWeight,
					  aFittingWeight ) ;
      }
      else if( hitInfosInLayer[ i ].size() > 0 )
      {
	 const AEWireLayerCylindrical& aLayer = m_wireStore->layer( i ) ;

	 if( ( aLayer.isAxial()   && !m_useAxialLayers ) ||
	     ( aLayer.isStereoU() && !m_useStereoULayers ) ||
	     ( aLayer.isStereoV() && !m_useStereoVLayers ) )
	 {
	    continue ;
	 }

	 // No z-boundary condition => infinite in z to force intersections
	 // with layers with hits, even if track is outside the detector.
	 HICondition* zBoundary = 0 ;

	 // Make a new wire-layer surface for this hit...
	 HIMultiWireCylinder* pWireSurface = new HIMultiWireCylinder(
	    m_drGas,           // inner surface
	    m_drGas,           // outer surface
	    aLayer,
	    i,
	    DGTransform::Identity,
	    zBoundary,
	    aOperation ) ;

	 if( pWireSurface == 0 )
	 {
	    continue ;
	 }

	 if( aForceFittingWeight )
	 {
	    pWireSurface->setFittingWeight( aFittingWeight ) ;
	 }

	 STL_VECTOR( const DRHitAndDriftDistance* )::iterator infoInLayerItr =
	    hitInfosInLayer[ i ].begin() ;
	 STL_VECTOR( const DRHitAndDriftDistance* )::iterator infoInLayerEnd =
	    hitInfosInLayer[ i ].end() ;

	 for( ; infoInLayerItr != infoInLayerEnd ; ++infoInLayerItr )
	 {
	    pWireSurface->addHit( ( *infoInLayerItr )->calibratedDRHit(),
				  m_hitCorrector,
				  ( *infoInLayerItr )->driftDistance(),
				  ( *infoInLayerItr )->averageChargeOnTrack()
	       ) ;
	 }

	 pSurface = pWireSurface ;
      }

      if( pSurface != 0 )
      {
	 aSurfaces.push_back( pSurface ) ;
      }
   }
}

HIIntersectionSurface*
HIDRSurfaceFactory::newSingleWireSurface(
   const DRHitAndDriftDistance& aHitInfo,
   HIHelixOperation* aOperation,
   DABoolean aCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight ) const
{
   int iLayer = aHitInfo.calibratedDRHit()->layer() ;
   const AEWireLayerCylindrical& aLayer = m_wireStore->layer( iLayer ) ;

   if( ( aLayer.isAxial()   && !m_useAxialLayers ) ||
       ( aLayer.isStereoU() && !m_useStereoULayers ) ||
       ( aLayer.isStereoV() && !m_useStereoVLayers ) )
   {
      return 0 ;
   }

   // No z-boundary condition => infinite in z to force intersections with
   // layers with hits, even if track is outside the detector.
   HIZBoundaryCondition* zBoundary = 0 ;

   // Make a new wire-layer surface for this hit...
   HISingleWireCylinder* pSurface = new HISingleWireCylinder(
      m_drGas,           // inner surface
      m_drGas,           // outer surface
      aLayer,
      iLayer,
      DGTransform::Identity,
      zBoundary,
      aOperation,
      aHitInfo.calibratedDRHit(),
      aHitInfo.driftDistance(),
      m_hitCorrector,
      aCheckDirectionSwitch,
      aHitInfo.averageChargeOnTrack() ) ;

   if( aForceFittingWeight )
   {
      pSurface->setFittingWeight( aFittingWeight ) ;
   }

   return pSurface ;
}

void
HIDRSurfaceFactory::generateAllSurfacesSingle(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
   HIHelixOperation* aOperationForLayersWithHits,
   HIHelixOperation* aOperationForLayersWithoutHits,
   int aFirstLayer,
   int aLastLayer,
   DABoolean aCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight )
{
   // A vector of DR layer numbers with hits.
   m_countHitsInLayer = true ;

   generateSingleWireSurfaces( aSurfaces,
			       aDRHitInfos,
			       aOperationForLayersWithHits,
			       aCheckDirectionSwitch,
			       aForceFittingWeight,
			       aFittingWeight ) ;

   generateSurfacesWithoutHits( aSurfaces,
				aOperationForLayersWithoutHits,
				aFirstLayer,
				aLastLayer ) ;

   clearHitsInLayer() ;
   m_countHitsInLayer = false ;
}

void
HIDRSurfaceFactory::generateAllSurfacesMulti(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
   HIHelixOperation* aOperationForLayersWithHits,
   HIHelixOperation* aOperationForLayersWithoutHits,
   int aFirstLayer,
   int aLastLayer,
   DABoolean aSingleCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight )
{
   m_countHitsInLayer = true ;

   generateMultiWireSurfaces( aSurfaces,
			      aDRHitInfos,
			      aOperationForLayersWithHits,
			      aSingleCheckDirectionSwitch,
			      aForceFittingWeight,
			      aFittingWeight ) ;

   generateSurfacesWithoutHits( aSurfaces,
				aOperationForLayersWithoutHits,
				aFirstLayer,
				aLastLayer ) ;

   clearHitsInLayer() ;
   m_countHitsInLayer = false ;
}


void
HIDRSurfaceFactory::generateSurfacesWithoutHits(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   HIHelixOperation* aOperation,
   int aFirstLayer,
   int aLastLayer ) const
{
   if( !m_geometryAvailable )
   {
      return ;
   }

   // Loop over DR layers up to the last hit layer and create surfaces
   // for those intermediate layers without hits.

   if( aFirstLayer == 0 && aLastLayer == 0 )
   {
      aFirstLayer = ADRSenseWireStore::kFirstLayer ;
      aLastLayer  = aFirstLayer - 1 + ADRSenseWireStore::kNumberOfLayers ;
   }

   for( int iLayer = aFirstLayer ; iLayer <= aLastLayer ; ++iLayer )
   {
      if( !m_countHitsInLayer || m_hitsInLayer[ iLayer ] == 0 )
      {
	 // Get layer information.
	 const AEWireLayerCylindrical& aLayer = m_wireStore->layer( iLayer ) ;

	 if( ( aLayer.isAxial()   && !m_useAxialLayers ) ||
	     ( aLayer.isStereoU() && !m_useStereoULayers ) ||
	     ( aLayer.isStereoV() && !m_useStereoVLayers ) )
	 {
	    continue ;
	 }

	 // Instantiate the z-boundary condition.
	 // (Gets deleted at end of event().)
	 HIZBoundaryCondition* zBoundary =
	    new HIZBoundaryCondition( aLayer.zMin(), aLayer.zMax() ) ;

	 aSurfaces.push_back( newSurfaceWithoutHits( m_drGas,
						     m_drGas,
						     aLayer,
						     iLayer,
						     DGTransform::Identity,
						     zBoundary,
						     aOperation ) ) ;

      }
   }
}

HIIntersectionSurface*
HIDRSurfaceFactory::newSurfaceWithoutHits(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   const AEWireLayerCylindrical& aWireLayer,
   int aLayerNumber,
   const DGTransform& aTransform,
   HICondition* aCondition,
   HIHelixOperation* aOperation ) const
{
   return new HIDRLayerCylinder(
      aInnerMaterial,
      aOuterMaterial,
      aWireLayer,
      aLayerNumber,
      aTransform,
      aCondition,
      aOperation ) ;
}


void
HIDRSurfaceFactory::generateInnerTubeCylinders(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   HIHelixOperation* aOperation ) const
{
   DGMaterialStore materials;
   extract( m_frame.record( Stream::kBaseGeometry ), materials );

   HIIntersectionCylinder* drInnerTubeInner = new HIIntersectionCylinder(
      materials[ kNitrogenGasMatID ],
      materials[ kDR3InnerTubeMaterialMatID ],
      DRGeometryParams::kInnerTubeInnerRadius,
      DGTransform::Identity,
      new HIZBoundaryCondition(
	 -DRGeometryParams::kInnerTubeZHalfWidth,
	 DRGeometryParams::kInnerTubeZHalfWidth ),
      aOperation ) ;

   HIIntersectionCylinder* drInnerTubeOuter = new HIIntersectionCylinder(
      materials[ kDR3InnerTubeMaterialMatID ],
      materials[ kHeliumPropaneMatID ],
      DRGeometryParams::kInnerTubeOuterRadius,
      DGTransform::Identity,
      new HIZBoundaryCondition(
	 -DRGeometryParams::kInnerTubeZHalfWidth,
	 DRGeometryParams::kInnerTubeZHalfWidth ),
      aOperation ) ;

   aSurfaces.push_back( drInnerTubeInner ) ;
   aSurfaces.push_back( drInnerTubeOuter ) ;
}

int
HIDRSurfaceFactory::getDRHits(
   const TRSeedTrack* aSeed,
   STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
   const UsageTag& aUsageTag,
   const ProductionTag& aProdTag,
   DABoolean aFittableHitsOnly
   ) const
{
   const Record& theEventRecord = m_frame.record( Stream::kEvent ) ;

   int maxLayer = 0 ;

   if( m_useAxialLayers || m_useStereoULayers || m_useStereoVLayers )
   {
      // Get the SeedTrack/CalibratedDRHit lattice.
      FAItem< SeedTrackDRHitLattice > pSeedDRHitLattice ;

      try
      {
	 extract( theEventRecord, pSeedDRHitLattice,
		  aUsageTag.value(), aProdTag.value() ) ;
      }
      catch( NO_ITEM_EXCEPTION( SeedTrackDRHitLattice )& aException )
      {
	 report( WARNING, kFacilityString )
	    << "Could not extract SeedTrackDRHitLattice with usage tag "
	    << aUsageTag.value() << " and production tag " << aProdTag.value()
	    << ".  Will extract default SeedTrackDRHitLattice." << endl ;

	 extract( theEventRecord, pSeedDRHitLattice ) ;
      }

      if( !pSeedDRHitLattice.valid() )
      {
	 return maxLayer ;
      }

      // Extract table of all DR hits.
      FATable< CalibratedDRHit > drHits ;

      try
      {
	 extract( theEventRecord, drHits, aUsageTag.value(), aProdTag.value());
      }
      catch( NO_TABLE_EXCEPTION( CalibratedDRHit )& aException )
      {
	 report( WARNING, kFacilityString )
	    << "Could not extract CalibratedDRHits with usage tag "
	    << aUsageTag.value() << " and production tag " << aProdTag.value()
	    << ".  Will extract default CalibratedDRHits." << endl ;

	 extract( theEventRecord, drHits ) ;
      }

      if( !drHits.valid() )
      {
	 return maxLayer ;
      }


      // The list of CalibratedDRHits for this seed track.
      const SeedTrackDRHitLattice::VectorRightID* drHitsOnTrack =
	 pSeedDRHitLattice->vRightGivenLeft( aSeed->identifier() ) ;

      if( drHitsOnTrack == 0 || drHitsOnTrack->size() == 0 )
      {
	 report( DEBUG, kFacilityString )
	    << "No DR hits on this track.  " << drHits.size()
	    << " DR hits in event." << endl ;
	 return maxLayer ;
      }


      // Counter for average charge.
      double chargeSum = 0. ;
      int nhitsInChargeSum = 0 ;

      // Loop over links to this seed track.
      SeedTrackDRHitLattice::RightItr drHitsItr = drHitsOnTrack->begin();
      SeedTrackDRHitLattice::RightItr endDRHit  = drHitsOnTrack->end() ;

      for( ; drHitsItr != endDRHit ; ++drHitsItr )
      {
	 // Retrieve the hit for this link.
	 const CalibratedDRHit& hitOnTrack = drHits[ *drHitsItr ] ;

	 // Check if we should be using this hit.
	 int iDRLayer = hitOnTrack.layer() ;
	 const AEWireLayerCylindrical& aLayer =
	    m_wireStore->layer( iDRLayer ) ;

	 if( ( aLayer.isAxial()   && !m_useAxialLayers ) ||
	     ( aLayer.isStereoU() && !m_useStereoULayers ) ||
	     ( aLayer.isStereoV() && !m_useStereoVLayers ) )
	 {
	    continue ;
	 }

	 // Insert this hit and drift distance into the vector
	 // to be given to the HIDRSurfaceFactory.
	 const SeedTrackDRHitLattice::Links& hitLinks =
	    pSeedDRHitLattice->linksGivenRight( drHitsItr ) ;

//	 const SeedDRHitLink& linkData = ( *hitLinks->begin() )->linkData() ;
	 const SeedDRHitLink& linkData = ( *hitLinks.begin() )->linkData() ;

	 if( !aFittableHitsOnly ||
	     linkData.fitFlag() == SeedDRHitLink::kFITTABLE )
	 {
	    // Increment charge counters for hits with good charge info.
	    double charge = hitOnTrack.charge() ;
	    if( charge > 0. && hitOnTrack.nTDC() == 1 )
	    {
	       chargeSum += charge ;
	       ++nhitsInChargeSum ;
	    }

	    Meters origSignedDrift = linkData.signedDriftDistance() ;

	    aDRHitInfos.push_back(
	       HIDRSurfaceFactory::DRHitAndDriftDistance(
		  &hitOnTrack,
		  origSignedDrift ) ) ;

	    maxLayer = ( iDRLayer > maxLayer ) ? iDRLayer : maxLayer ;
	 }
      }

      // Set average charge in all DRHitAndDriftDistances.
      double averageChargeOnTrack = 0. ;
      if( nhitsInChargeSum > 0 )
      {
         averageChargeOnTrack = chargeSum / nhitsInChargeSum ;
      }

//       cout << "Avg chg " << averageChargeOnTrack
// 	   << " ngood hits " << nhitsInChargeSum
// 	   << " nhits on track " << aDRHitInfos.size() << endl ;

      STL_VECTOR( DRHitAndDriftDistance )::iterator hitItr =
	 aDRHitInfos.begin() ;
      STL_VECTOR( DRHitAndDriftDistance )::iterator hitEnd =
	 aDRHitInfos.end() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 hitItr->setAverageChargeOnTrack( averageChargeOnTrack ) ;
      }
   }


   return maxLayer ;
}

//
// static member functions
//
