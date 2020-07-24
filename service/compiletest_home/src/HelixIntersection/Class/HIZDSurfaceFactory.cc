// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIZDSurfaceFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Jul 14 12:38:45 EDT 2003
// $Id: HIZDSurfaceFactory.cc,v 1.5 2003/10/15 19:10:23 mccann Exp $
//
// Revision history
//
// $Log: HIZDSurfaceFactory.cc,v $
// Revision 1.5  2003/10/15 19:10:23  mccann
// Wrapped excessive DEBUG printouts with m_printDiagnostics==true requirement
//
// Revision 1.4  2003/10/10 23:14:42  cdj
// now get DGMaterialStore from material stream
//
// Revision 1.3  2003/09/01 19:46:14  wsun
// Replaced HIZDLayerCylinder with HIWireLayerCylinder.
//
// Revision 1.2  2003/08/30 00:24:18  wsun
// * Use WHCBaseHitCorrector instead of HitCorrector (DR) and ZDHitCorrector.
// * Allow hit corrections to be applied in reverse in HICalibratedHit (for MC).
//
// Revision 1.1  2003/07/26 03:27:37  mccann
// adding file for ZD surfaces
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

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

#include "ZDGeomProd/ZDGeomDescription.h"
#include "AZDGeom/AZDSenseWireStore.h"
// #include "ADRGeom/ADRWireAddress.h"
#include "ActiveElements/AEWireLayerCylindrical.h"
#include "ActiveElements/AEWireInfo.h"

#include "ZDHitCorrectorProd/ZDHitCorrector.h"

#include "HelixIntersection/HIZDSurfaceFactory.h"
#include "HelixIntersection/HIWireLayerCylinder.h"
#include "HelixIntersection/HISingleWireCylinder.h"
#include "HelixIntersection/HIMultiWireCylinder.h"
#include "HelixIntersection/HIZBoundaryCondition.h"

#include "TrackRoot/TRSeedTrack.h"
#include "TrackFinder/SeedZDHitLink.h"

#include "DAException/DANoDataException.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HelixIntersection.HIZDSurfaceFactory" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIZDSurfaceFactory.cc,v 1.5 2003/10/15 19:10:23 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIZDSurfaceFactory::HIZDSurfaceFactory( Frame& theFrame,
					DABoolean useStereoULayers,
					DABoolean useStereoVLayers,
					DABoolean correctZDHits,
					const AZDSenseWireStore* aWireStore )
   : m_geometryAvailable( true )
   , m_frame( theFrame )
   , m_wireStore( aWireStore )
   , m_useStereoULayers( useStereoULayers )
   , m_useStereoVLayers( useStereoVLayers )
   , m_correctZDHits( correctZDHits )
   , m_countHitsInLayer( false )
   , m_hitCorrector( 0 )
{
   report(DEBUG, kFacilityString) << "start" << endl;

   // The records in the Frame from which we need information.
   const Record& zdAlignmentRecord = theFrame.record( Stream::kZDAlignment ) ;

   report(DEBUG, kFacilityString) << "got ZDalign record" << endl;

// We don't really need this for the DR, either
//    // Extract the base (eventually will be aligned) passive ZD geometry.
//    // This is only used to get the ZD gas material.
//    FAItem< DGDetectorStore > zdStore;
//    extract( zdAlignmentRecord, zdStore ) ;
//    if( !zdStore.valid() )
//    {
//       report( ERROR, kFacilityString )
// 	 << "No ZD DetectorStore in the Frame!" << endl ;
//       m_geometryAvailable = false ;
//       return ;
//    }

   DGMaterialStore materials;
   extract( m_frame.record( Stream::kMaterial ), materials );
   m_zdGas = materials[ kZDGasWireVaporMatID ];

   // Extract sense wire store from the Frame.
   if( m_wireStore == 0 )
   {
      FAItem< AZDSenseWireStore > wireStore ;
      extract( zdAlignmentRecord, wireStore ) ;

      if( !wireStore.valid() )
      {
	 report( ERROR, kFacilityString ) << "No ZD wire store in the Frame!"
					  << endl ;
	 m_geometryAvailable = false ;
	 return ;
      }
      m_wireStore = &( *wireStore ) ;
   }

   report(DEBUG, kFacilityString) << "got wire gas material" << endl;

   // Initialize array m_hitsInLayer.
   clearHitsInLayer() ;

   report(DEBUG, kFacilityString) << "cleared hits in layer" << endl;

   // Fill pointer to HitCorrector if necessary.
   FAItem< ZDHitCorrector > hitCorrector ;
   if( m_correctZDHits )
   {
      extract( m_frame.record( Stream::kStartRun ), hitCorrector ) ;
      m_hitCorrector = &*hitCorrector ;
      report(DEBUG, kFacilityString) << "got hit corrector" << endl;
   }

   report(DEBUG, kFacilityString) << "finished!" << endl;
}

// HIZDSurfaceFactory::HIZDSurfaceFactory( const HIZDSurfaceFactory& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIZDSurfaceFactory::~HIZDSurfaceFactory()
{
}

//
// assignment operators
//
// const HIZDSurfaceFactory& HIZDSurfaceFactory::operator=( const HIZDSurfaceFactory& rhs )
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
HIZDSurfaceFactory::clearHitsInLayer()
{
   for( int i = 0 ;
	i < AZDSenseWireStore::kFirstLayer+AZDSenseWireStore::kNumberOfSenseLayers ;
	++i )
   {
      m_hitsInLayer[ i ] = 0 ;
   }
}

//
// const member functions
//

void
HIZDSurfaceFactory::generateSingleWireSurfaces(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
   HIHelixOperation* aOperation,
   DABoolean aCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight )
{
   report(DEBUG, kFacilityString) << "start generateSingleWireSurfaces" << endl;

   if( !m_geometryAvailable )
   {
      report(DEBUG, kFacilityString) << "exit generateSingleWireSurfaces 1" << endl;
      return ;
   }

   STL_VECTOR( ZDHitAndDriftDistance )::const_iterator hitInfoItr =
      aZDHitInfos.begin() ;
   STL_VECTOR( ZDHitAndDriftDistance )::const_iterator lastHit =
      aZDHitInfos.end() ;

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
	    ++m_hitsInLayer[ hitInfoItr->calibratedZDHit()->layer() ] ;
	 }
      }
   }

   report(DEBUG, kFacilityString) << "end generateSingleWireSurfaces" << endl;
}

void
HIZDSurfaceFactory::generateMultiWireSurfaces(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
   HIHelixOperation* aOperation,
   DABoolean aSingleCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight )
{
   report(DEBUG, kFacilityString) << "start generateMultiWireSurfaces" << endl;

   if( !m_geometryAvailable )
   {
      report(DEBUG, kFacilityString) << "exit generateMultiWireSurfaces 1" << endl;
      return ;
   }

   // Group the ZDHitAndDriftDistances by layer.
   STL_VECTOR( ZDHitAndDriftDistance )::const_iterator hitInfoItr =
      aZDHitInfos.begin() ;
   STL_VECTOR( ZDHitAndDriftDistance )::const_iterator lastHit =
      aZDHitInfos.end() ;

   STL_VECTOR( const ZDHitAndDriftDistance* ) hitInfosInLayer[
      AZDSenseWireStore::kFirstLayer + AZDSenseWireStore::kNumberOfSenseLayers ] ;

   for( ; hitInfoItr != lastHit ; ++hitInfoItr )
   {
      int iLayer = hitInfoItr->calibratedZDHit()->layer() ;
      hitInfosInLayer[ iLayer ].push_back( &( *hitInfoItr ) ) ;

      if( m_countHitsInLayer )
      {
	 ++m_hitsInLayer[ iLayer ] ;
      }
   }

   // Loop through layers and make the appropriate surface for each.
   for( int i = AZDSenseWireStore::kFirstLayer ;
	i < AZDSenseWireStore::kFirstLayer+AZDSenseWireStore::kNumberOfSenseLayers ;
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
	 report(DEBUG, kFacilityString) << "about to access wirestore 1" << endl;

	 const AEWireLayerCylindrical& aLayer = m_wireStore->layer( i ) ;

	 if( ( aLayer.isStereoU() && !m_useStereoULayers ) ||
	     ( aLayer.isStereoV() && !m_useStereoVLayers ) )
	 {
	    continue ;
	 }

	 // No z-boundary condition => infinite in z to force intersections
	 // with layers with hits, even if track is outside the detector.
	 HICondition* zBoundary = 0 ;

	 // Make a new wire-layer surface for this hit...
	 HIMultiWireCylinder* pWireSurface = new HIMultiWireCylinder(
	    m_zdGas,           // inner surface
	    m_zdGas,           // outer surface
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

	 STL_VECTOR( const ZDHitAndDriftDistance* )::iterator infoInLayerItr =
	    hitInfosInLayer[ i ].begin() ;
	 STL_VECTOR( const ZDHitAndDriftDistance* )::iterator infoInLayerEnd =
	    hitInfosInLayer[ i ].end() ;

	 for( ; infoInLayerItr != infoInLayerEnd ; ++infoInLayerItr )
	 {
	    pWireSurface->addHit( ( *infoInLayerItr )->calibratedZDHit(),
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

   report(DEBUG, kFacilityString) << "end generateMultiWireSurfaces" << endl;
}

HIIntersectionSurface*
HIZDSurfaceFactory::newSingleWireSurface(
   const ZDHitAndDriftDistance& aHitInfo,
   HIHelixOperation* aOperation,
   DABoolean aCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight ) const
{
   report(DEBUG, kFacilityString) << "start newSingleWireSurface" << endl;

   int iLayer = aHitInfo.calibratedZDHit()->layer() ;

   report(DEBUG, kFacilityString) << "about to access wirestore 2" << endl;

   const AEWireLayerCylindrical& aLayer = m_wireStore->layer( iLayer ) ;

   if( ( aLayer.isStereoU() && !m_useStereoULayers ) ||
       ( aLayer.isStereoV() && !m_useStereoVLayers ) )
   {
      report(DEBUG, kFacilityString) << "exit newSingleWireSurface 1" << endl;
      return 0 ;
   }

   // No z-boundary condition => infinite in z to force intersections with
   // layers with hits, even if track is outside the detector.
   HIZBoundaryCondition* zBoundary = 0 ;

   report(DEBUG, kFacilityString)
      << "about to create surface: " << iLayer << " " << aOperation
      << " > " << aHitInfo.calibratedZDHit() << " < " << aHitInfo.driftDistance()
      << " " << m_hitCorrector << " " << aCheckDirectionSwitch << " " << aHitInfo.averageChargeOnTrack() << endl;

   report(DEBUG, kFacilityString)
      << "brander new hit type is " << aHitInfo.calibratedZDHit()->deviceType() << endl;

   // Make a new wire-layer surface for this hit...
   HISingleWireCylinder* pSurface = new HISingleWireCylinder(
      m_zdGas,           // inner surface
      m_zdGas,           // outer surface
      aLayer,
      iLayer,
      DGTransform::Identity,
      zBoundary,
      aOperation,
      aHitInfo.calibratedZDHit(),
      aHitInfo.driftDistance(),
      m_hitCorrector,
      aCheckDirectionSwitch,
      aHitInfo.averageChargeOnTrack() ) ;

   report(DEBUG, kFacilityString)
      << "brand new surface type is " << pSurface->hiCalibratedHit()->calibratedHit()->deviceType()
      << " (DR = " << CalibratedHit::DR << " and ZD = " << CalibratedHit::ZD << ")" << endl;

   if( aForceFittingWeight )
   {
      pSurface->setFittingWeight( aFittingWeight ) ;
   }

   report(DEBUG, kFacilityString) << "end newSingleWireSurface" << endl;

   return pSurface ;
}

void
HIZDSurfaceFactory::generateAllSurfacesSingle(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
   HIHelixOperation* aOperationForLayersWithHits,
   HIHelixOperation* aOperationForLayersWithoutHits,
   int aFirstLayer,
   int aLastLayer,
   DABoolean aCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight )
{
   report(DEBUG, kFacilityString) << "start generateAllSurfacesSingle" << endl;

   report(WARNING, kFacilityString)
      << "This function, generateAllSurfacesSingle, was just copied from the DR version without testing.  Contact Jim Pivarski." << endl;

   // A vector of DR layer numbers with hits.
   m_countHitsInLayer = true ;

   generateSingleWireSurfaces( aSurfaces,
			       aZDHitInfos,
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

   report(DEBUG, kFacilityString) << "end generateAllSurfacesSingle" << endl;
}

void
HIZDSurfaceFactory::generateAllSurfacesMulti(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   const STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
   HIHelixOperation* aOperationForLayersWithHits,
   HIHelixOperation* aOperationForLayersWithoutHits,
   int aFirstLayer,
   int aLastLayer,
   DABoolean aSingleCheckDirectionSwitch,
   DABoolean aForceFittingWeight,
   double aFittingWeight )
{
   report(DEBUG, kFacilityString) << "start generateAllSurfacesMulti" << endl;

   m_countHitsInLayer = true ;

   generateMultiWireSurfaces( aSurfaces,
			      aZDHitInfos,
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

   report(DEBUG, kFacilityString) << "end generateAllSurfacesMulti" << endl;
}


void
HIZDSurfaceFactory::generateSurfacesWithoutHits(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   HIHelixOperation* aOperation,
   int aFirstLayer,
   int aLastLayer ) const
{
   report(DEBUG, kFacilityString) << "start generateSurfaceWithoutHits" << endl;

   if( !m_geometryAvailable )
   {
      report(DEBUG, kFacilityString) << "exit generateSurfaceWithoutHits 1" << endl;
      return ;
   }

   // Loop over DR layers up to the last hit layer and create surfaces
   // for those intermediate layers without hits.

   if( aFirstLayer == 0 && aLastLayer == 0 )
   {
      aFirstLayer = AZDSenseWireStore::kFirstLayer ;
      aLastLayer  = aFirstLayer - 1 + AZDSenseWireStore::kNumberOfSenseLayers ;
   }

   for( int iLayer = aFirstLayer ; iLayer <= aLastLayer ; ++iLayer )
   {
      if( !m_countHitsInLayer || m_hitsInLayer[ iLayer ] == 0 )
      {
	 // Get layer information.
	 report(DEBUG, kFacilityString) << "about to access wirestore 3" << endl;

	 const AEWireLayerCylindrical& aLayer = m_wireStore->layer( iLayer ) ;

	 if( ( aLayer.isStereoU() && !m_useStereoULayers ) ||
	     ( aLayer.isStereoV() && !m_useStereoVLayers ) )
	 {
	    continue ;
	 }

	 // Instantiate the z-boundary condition.
	 // (Gets deleted at end of event().)
	 HIZBoundaryCondition* zBoundary =
	    new HIZBoundaryCondition( aLayer.zMin(), aLayer.zMax() ) ;

	 aSurfaces.push_back( newSurfaceWithoutHits( m_zdGas,
						     m_zdGas,
						     aLayer,
						     iLayer,
						     DGTransform::Identity,
						     zBoundary,
						     aOperation ) ) ;

      }
   }

   report(DEBUG, kFacilityString) << "end generateSurfaceWithoutHits" << endl;
}

HIIntersectionSurface*
HIZDSurfaceFactory::newSurfaceWithoutHits(
   const DGConstMaterialPtr& aInnerMaterial,
   const DGConstMaterialPtr& aOuterMaterial,
   const AEWireLayerCylindrical& aWireLayer,
   int aLayerNumber,
   const DGTransform& aTransform,
   HICondition* aCondition,
   HIHelixOperation* aOperation ) const
{
   report(DEBUG, kFacilityString) << "newSurfaceWithoutHits (quick)" << endl;

   return new HIWireLayerCylinder(
      aInnerMaterial,
      aOuterMaterial,
      aWireLayer,
      aLayerNumber,
      aTransform,
      aCondition,
      aOperation ) ;
}


void
HIZDSurfaceFactory::generateTubeCylinders(
   STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
   HIHelixOperation* aOperation ) const
{
   report(DEBUG, kFacilityString) << "start generateInnerTubeCylinder" << endl;

   DGMaterialStore materials;
   extract( m_frame.record( Stream::kMaterial ), materials );

   // According to DPP, the aluminum of the ZD is an unknown and unknowable
   // alloy.  Therefore, we will use pure aluminum.

   HIIntersectionCylinder* zdInnerTubeInner = new HIIntersectionCylinder(
      materials[ kNitrogenGasMatID ],
      materials[ kAluminumMatID ],
      ZDGeomDescription::kZDInnerRadius,
      DGTransform::Identity,
      new HIZBoundaryCondition(
	 -ZDGeomDescription::kZDInnerHalfLength,
	 ZDGeomDescription::kZDInnerHalfLength ),
      aOperation ) ;

   HIIntersectionCylinder* zdInnerTubeOuter = new HIIntersectionCylinder(
      materials[ kAluminumMatID ],
      materials[ kZDGasWireVaporMatID ],
      ZDGeomDescription::kZDInnerRadius + ZDGeomDescription::kZDInnerThickness,
      DGTransform::Identity,
      new HIZBoundaryCondition(
	 -ZDGeomDescription::kZDInnerHalfLength,
	 ZDGeomDescription::kZDInnerHalfLength ),
      aOperation ) ;

   HIIntersectionCylinder* zdOuterTube1 = new HIIntersectionCylinder(
      materials[ kZDGasWireVaporMatID ],
      materials[ kAluminumMatID ],
      ZDGeomDescription::kOuterSkinRmin,
      DGTransform::Identity,
      new HIZBoundaryCondition(
	 -ZDGeomDescription::kOuterSkinHalfLength,
	 ZDGeomDescription::kOuterSkinHalfLength ),
      aOperation ) ;

   HIIntersectionCylinder* zdOuterTube2 = new HIIntersectionCylinder(
      materials[ kAluminumMatID ],
      materials[ kMylarMatID ],
      ZDGeomDescription::kOuterSkinRmin
      + ZDGeomDescription::kOuterSkinAlThickness,
      DGTransform::Identity,
      new HIZBoundaryCondition(
	 -ZDGeomDescription::kOuterSkinHalfLength,
	 ZDGeomDescription::kOuterSkinHalfLength ),
      aOperation ) ;

   HIIntersectionCylinder* zdOuterTube3 = new HIIntersectionCylinder(
      materials[ kMylarMatID ],
      materials[ kAluminumMatID ],
      ZDGeomDescription::kOuterSkinRmin
      + ZDGeomDescription::kOuterSkinAlThickness
      + ZDGeomDescription::kOuterSkinMylarThickness,
      DGTransform::Identity,
      new HIZBoundaryCondition(
	 -ZDGeomDescription::kOuterSkinHalfLength,
	 ZDGeomDescription::kOuterSkinHalfLength ),
      aOperation ) ;

   HIIntersectionCylinder* zdOuterTube4 = new HIIntersectionCylinder(
      materials[ kAluminumMatID ],
      materials[ kNitrogenGasMatID ],
      ZDGeomDescription::kOuterSkinRmin
      + ZDGeomDescription::kOuterSkinAlThickness
      + ZDGeomDescription::kOuterSkinMylarThickness
      + ZDGeomDescription::kOuterSkinAlThickness,
      DGTransform::Identity,
      new HIZBoundaryCondition(
	 -ZDGeomDescription::kOuterSkinHalfLength,
	 ZDGeomDescription::kOuterSkinHalfLength ),
      aOperation ) ;

   aSurfaces.push_back( zdInnerTubeInner ) ;
   aSurfaces.push_back( zdInnerTubeOuter ) ;
   aSurfaces.push_back( zdOuterTube1 ) ;
   aSurfaces.push_back( zdOuterTube2 ) ;
   aSurfaces.push_back( zdOuterTube3 ) ;
   aSurfaces.push_back( zdOuterTube4 ) ;

   report(DEBUG, kFacilityString) << "end generateInnerTubeCylinder" << endl;
}

int
HIZDSurfaceFactory::getZDHits(
   const TRSeedTrack* aSeed,
   STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
   const UsageTag& aUsageTag,
   const ProductionTag& aProdTag,
   DABoolean aFittableHitsOnly
   ) const
{
   report(DEBUG, kFacilityString) << "start getZDHits" << endl;

   const Record& theEventRecord = m_frame.record( Stream::kEvent ) ;

   int maxLayer = 0 ;

   if( m_useStereoULayers || m_useStereoVLayers )
   {
      // Get the SeedTrack/CalibratedZDHit lattice.
      FAItem< SeedTrackZDHitLattice > pSeedZDHitLattice ;

      try
      {
	 report(DEBUG, kFacilityString) << "about to extract 1: aUsageTag.value() == " << aUsageTag.value() << endl;
	 extract( theEventRecord, pSeedZDHitLattice,
		  aUsageTag.value(), aProdTag.value()) ;
      }
      catch( NO_ITEM_EXCEPTION( SeedTrackZDHitLattice )& aException )
      {
	 report( WARNING, kFacilityString )
	    << "Could not extract SeedTrackZDHitLattice with usage tag "
	    << aUsageTag.value() << " and production tag " << aProdTag.value()
	    << ".  Will extract default SeedTrackZDHitLattice." << endl ;

	 extract( theEventRecord, pSeedZDHitLattice) ;
      }

      if( !pSeedZDHitLattice.valid() )
      {
	 report(DEBUG, kFacilityString) << "exit getZDHits 1" << endl;
	 return maxLayer ;
      }

      // Extract table of all ZD hits.
      FATable< CalibratedZDHit > zdhits ;

      try
      {
	 report(DEBUG, kFacilityString) << "about to extract 2: aUsageTag.value() == " << aUsageTag.value() << endl;
	 extract( theEventRecord, zdhits, aUsageTag.value(), aProdTag.value());
      }
      catch( NO_TABLE_EXCEPTION( CalibratedZDHit )& aException )
      {
	 report( WARNING, kFacilityString )
	    << "Could not extract CalibratedZDHits with usage tag "
	    << aUsageTag.value() << " and production tag " << aProdTag.value()
	    << ".  Will extract default CalibratedZDHits." << endl ;

	 extract( theEventRecord, zdhits) ;
      }

      if( !zdhits.valid() )
      {
	 report(DEBUG, kFacilityString) << "exit getZDHits 2" << endl;

	 return maxLayer ;
      }


      // The list of CalibratedZDHits for this seed track.
      const SeedTrackZDHitLattice::VectorRightID* zdhitsOnTrack =
	 pSeedZDHitLattice->vRightGivenLeft( aSeed->identifier() ) ;

      if( zdhitsOnTrack == 0 || zdhitsOnTrack->size() == 0 )
      {
	 report( DEBUG, kFacilityString )
	    << "No ZD hits on this track.  " << zdhits.size()
	    << " ZD hits in event." << endl ;
	 return maxLayer ;
      }


      // Counter for average charge.
      double chargeSum = 0. ;
      int nhitsInChargeSum = 0 ;

      // Loop over links to this seed track.
      SeedTrackZDHitLattice::RightItr zdhitsItr = zdhitsOnTrack->begin();
      SeedTrackZDHitLattice::RightItr endZDHit  = zdhitsOnTrack->end() ;

      for( ; zdhitsItr != endZDHit ; ++zdhitsItr )
      {
	 // Retrieve the hit for this link.
	 const CalibratedZDHit& hitOnTrack = zdhits[ *zdhitsItr ] ;

	 // Check if we should be using this hit.
	 int iZDLayer = hitOnTrack.layer() ;

	 report(DEBUG, kFacilityString) << "about to add zd hit from layer " << iZDLayer << endl;
	 report(DEBUG, kFacilityString) << "about to access wirestore 4" << endl;

	 const AEWireLayerCylindrical& aLayer =
	    m_wireStore->layer( iZDLayer ) ;

	 if( ( aLayer.isStereoU() && !m_useStereoULayers ) ||
	     ( aLayer.isStereoV() && !m_useStereoVLayers ) )
	 {
	    report(DEBUG, kFacilityString) << "nope, we're not doing that type " << iZDLayer << endl;
	    continue ;
	 }

	 // Insert this hit and drift distance into the vector
	 // to be given to the HIZDSurfaceFactory.
	 const SeedTrackZDHitLattice::Links& hitLinks =
	    pSeedZDHitLattice->linksGivenRight( zdhitsItr ) ;

//	 const SeedZDHitLink& linkData = ( *hitLinks->begin() )->linkData() ;
	 const SeedZDHitLink& linkData = ( *hitLinks.begin() )->linkData() ;

	 report(DEBUG, kFacilityString)
	    << "we have linkdata, aFittableHitsOnly == " << aFittableHitsOnly
	    << ", linkData.fitFlag() == " << linkData.fitFlag() << " (compare to kFITTABLE == "
	    << SeedZDHitLink::kFITTABLE << ")" << endl;
	 
	 if( !aFittableHitsOnly ||
	     linkData.fitFlag() == SeedZDHitLink::kFITTABLE )
	 {
	    // Increment charge counters for hits with good charge info.
	    double charge = hitOnTrack.charge() ;
	    if( charge > 0. && hitOnTrack.nTDC() == 1 )
	    {
	       chargeSum += charge ;
	       ++nhitsInChargeSum ;
	    }

	    Meters origSignedDrift = linkData.signedDriftDistance() ;

	    aZDHitInfos.push_back(
	       HIZDSurfaceFactory::ZDHitAndDriftDistance(
		  &hitOnTrack,
		  origSignedDrift ) ) ;

	    report(DEBUG, kFacilityString)
	       << "so we're going to add it!  &hitOnTrack == " << &hitOnTrack
	       << ", charge == " << charge << ", origSignedDrift == " << origSignedDrift << endl;
	    maxLayer = ( iZDLayer > maxLayer ) ? iZDLayer : maxLayer ;
	 }
      }

      // Set average charge in all ZDHitAndDriftDistances.
      double averageChargeOnTrack = 0. ;
      if( nhitsInChargeSum > 0 )
      {
         averageChargeOnTrack = chargeSum / nhitsInChargeSum ;
      }

//       cout << "Avg chg " << averageChargeOnTrack
// 	   << " ngood hits " << nhitsInChargeSum
// 	   << " nhits on track " << aZDHitInfos.size() << endl ;

      STL_VECTOR( ZDHitAndDriftDistance )::iterator hitItr =
	 aZDHitInfos.begin() ;
      STL_VECTOR( ZDHitAndDriftDistance )::iterator hitEnd =
	 aZDHitInfos.end() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 hitItr->setAverageChargeOnTrack( averageChargeOnTrack ) ;
      }
   }


   report(DEBUG, kFacilityString) << "end getZDHits" << endl;
   return maxLayer ;
}

//
// static member functions
//
