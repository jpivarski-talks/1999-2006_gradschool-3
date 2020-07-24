#if !defined(HELIXINTERSECTION_HIZDSURFACEFACTORY_H)
#define HELIXINTERSECTION_HIZDSURFACEFACTORY_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIZDSurfaceFactory
// 
/**\class HIZDSurfaceFactory HIZDSurfaceFactory.h HelixIntersection/HIZDSurfaceFactory.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Jul 14 12:38:39 EDT 2003
// $Id: HIZDSurfaceFactory.h,v 1.1 2003/07/26 03:28:01 mccann Exp $
//
// Revision history
//
// $Log: HIZDSurfaceFactory.h,v $
// Revision 1.1  2003/07/26 03:28:01  mccann
// adding file for ZD surfaces
//

// system include files

// user include files

// DGTransform is a typedef, so it needs to be included not forward-declared.
#include "DetectorGeometry/DGTransform.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "AZDGeom/AZDSenseWireStore.h"

#include "CalibratedData/CalibratedZDHit.h"

#include "DataHandler/DataKeyTags.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class Frame ;
class HICondition ;
class HIHelixOperation ;
class HitCorrector ;
class HIIntersectionSurface ;
class TRSeedTrack ;
class AEWireLayerCylindrical ;

class HIZDSurfaceFactory
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      class ZDHitAndDriftDistance
      {
	 public:
	    ZDHitAndDriftDistance() : m_hit( ( const CalibratedZDHit* ) 0 ) {}

	    ZDHitAndDriftDistance( const CalibratedZDHit* aHit,
				   const double& aDriftDistance )
	       : m_hit( aHit ),
		 m_driftDistance( aDriftDistance ) {}

	    ZDHitAndDriftDistance( const ZDHitAndDriftDistance& rhs )
	       : m_hit(           rhs.m_hit ),
		 m_driftDistance( rhs.m_driftDistance ),
		 m_averageChargeOnTrack( rhs.m_averageChargeOnTrack ) {}

	    const ZDHitAndDriftDistance& operator=(
	       const ZDHitAndDriftDistance& rhs )
	    {
	       if( this != &rhs )
	       {
		  m_hit           = rhs.m_hit ;
		  m_driftDistance = rhs.m_driftDistance ;
		  m_averageChargeOnTrack = rhs.m_averageChargeOnTrack ;
	       }
	       return *this;
	    }

	    virtual ~ZDHitAndDriftDistance(){}

	    void clear()
	    {
	       m_hit = ( const CalibratedZDHit* ) 0 ;
	       m_driftDistance = 0. ;
	    }

	    void setAverageChargeOnTrack( const double& aCharge )
	    { m_averageChargeOnTrack = aCharge ; }

	    const CalibratedZDHit* calibratedZDHit() const { return m_hit ; }
	    const double& driftDistance() const { return m_driftDistance ; }
	    const double& averageChargeOnTrack() const
	    { return m_averageChargeOnTrack ; }
	    

	 private:
	    const CalibratedZDHit* m_hit ;
	    double m_driftDistance ;
	    double m_averageChargeOnTrack ;
      } ;

      // ---------- Constructors and destructor ----------------
      HIZDSurfaceFactory(Frame& theFrame,
			 DABoolean useStereoULayers = true,
			 DABoolean useStereoVLayers = true,
			 DABoolean correctZDHits = false,
			 const AZDSenseWireStore* aWireStore = 0 );
      virtual ~HIZDSurfaceFactory();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // Generate a HISingleWireCylinder for each hit, even if there are
      // multiple hits in a layer.
      void generateSingleWireSurfaces(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 const STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
	 HIHelixOperation* aOperation,
	 DABoolean aCheckDirectionSwitch = true,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0. ) ;

      // Generate HISingleWireCylinders for layers with one hit
      // and HIMultiWireSurfaces for layers with more than one hit.
      //
      // If there is only one hit in a layer, an HISingleWireCylinder will
      // be generated.  If that surface's propagateHelix() function should
      // not check if the helix switches direction (towards or away from
      // the origin), then aSingleCheckDirectionSwitch should be false.
      void generateMultiWireSurfaces(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 const STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
	 HIHelixOperation* aOperation,
	 DABoolean aSingleCheckDirectionSwitch = true,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0. ) ;

      // Generate surfaces for all ZD layers, making HIWireLayerCylinders
      // for layers with hits and HIZDLayerCylinders for layers without hits.
      void generateAllSurfacesSingle(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 const STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
	 HIHelixOperation* aOperationForLayersWithHits,
	 HIHelixOperation* aOperationForLayersWithoutHits,
	 int aFirstLayer = 0,
	 int aLastLayer = 0,
	 DABoolean aCheckDirectionSwitch = true,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0. ) ;

      void generateAllSurfacesMulti(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 const STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
	 HIHelixOperation* aOperationForLayersWithHits,
	 HIHelixOperation* aOperationForLayersWithoutHits,
	 int aFirstLayer = 0,
	 int aLastLayer = 0,
	 DABoolean aSingleCheckDirectionSwitch = true,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0. ) ;

      // ---------- const member functions ---------------------

      // If instantiation falied, this is false.
      DABoolean geometryAvailable() const
      { return m_geometryAvailable ; }

      // Generate HIZDLayerCylinder surfaces for all ZD layers by default.
      void generateSurfacesWithoutHits(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 HIHelixOperation* aOperation,
	 int aFirstLayer = 0,
	 int aLastLayer = 0 ) const ;

      // Generate two cylinders for ZD inner tube.
      void generateTubeCylinders(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 HIHelixOperation* aOperation ) const ;

      // Return value is the layer number of the outermost hit layer.
      int getZDHits(
	 const TRSeedTrack* theSeed,
	 STL_VECTOR( ZDHitAndDriftDistance )& aZDHitInfos,
	 const UsageTag& aUsageTag = UsageTag(),
	 const ProductionTag& aProdTag = ProductionTag(),
	 DABoolean aFittableHitsOnly = false
	 ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------
      virtual HIIntersectionSurface* newSurfaceWithoutHits(
	 const DGConstMaterialPtr& aInnerMaterial,
	 const DGConstMaterialPtr& aOuterMaterial,
	 const AEWireLayerCylindrical& aWireLayer,
	 int aLayerNumber,
	 const DGTransform& aTransform,
	 HICondition* aCondition,
	 HIHelixOperation* aOperation ) const ;

   private:
      // ---------- Constructors and destructor ----------------
      HIZDSurfaceFactory( const HIZDSurfaceFactory& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIZDSurfaceFactory& operator=( const HIZDSurfaceFactory& ); // stop default

      // ---------- private member functions -------------------
      void clearHitsInLayer() ;

      HIIntersectionSurface* newSingleWireSurface(
	 const ZDHitAndDriftDistance& aHitInfo,
	 HIHelixOperation* aOperation,
	 DABoolean aCheckCrossOrigin,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0. ) const ;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_geometryAvailable ;
      DABoolean m_useStereoULayers ;
      DABoolean m_useStereoVLayers ;

      DABoolean m_correctZDHits ;

      const AZDSenseWireStore* m_wireStore;
      DGConstMaterialPtr m_zdGas;
      int m_hitsInLayer[ AZDSenseWireStore::kFirstLayer +
			 AZDSenseWireStore::kNumberOfSenseLayers ] ;
      DABoolean m_countHitsInLayer ;
      Frame& m_frame ;

      const HitCorrector* m_hitCorrector ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIZDSurfaceFactory.cc"
//#endif

#endif /* HELIXINTERSECTION_HIZDSURFACEFACTORY_H */
