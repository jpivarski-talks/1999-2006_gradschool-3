#if !defined(HELIXINTERSECTION_HIDRSURFACEFACTORY_H)
#define HELIXINTERSECTION_HIDRSURFACEFACTORY_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIDRSurfaceFactory
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Apr  2 17:08:34 EST 1999
// $Id: HIDRSurfaceFactory.h,v 1.16 2001/03/27 05:28:42 wsun Exp $
//
// Revision history
//
// $Log: HIDRSurfaceFactory.h,v $
// Revision 1.16  2001/03/27 05:28:42  wsun
// Added generateInnerTubeCylinders().
//
// Revision 1.15  2000/12/02 01:30:34  wsun
// Added avg chg on track to HICalibratedDRHit and DRHitAndDriftDistance.
//
// Revision 1.14  2000/10/29 02:40:40  wsun
// Added flag to getDRHits() to ignore hits not to be used in fit.
//
// Revision 1.13  2000/09/05 20:34:57  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.12  2000/07/25 21:34:19  wsun
// User can specify usage and and production tags for hits.
//
// Revision 1.11  2000/06/09 00:31:04  wsun
// Added pointer to HitCorrector.
//
// Revision 1.10  2000/02/22 17:20:46  wsun
// Allow users to set HISingleWireLayer::m_checkDirectionSwitch.
//
// Revision 1.9  2000/02/03 07:16:18  wsun
// Don't call assignment operator in copy ctor.
//
// Revision 1.8  1999/10/13 08:57:11  wsun
// Allow user to give a ADRSenseWireStore to the ctor.
//
// Revision 1.7  1999/10/05 23:06:12  wsun
// Added use of DRHitCorrector.
//
// Revision 1.6  1999/08/03 21:45:39  wsun
// Added functions for generating single-/multi-wire cylinders.
//
// Revision 1.5  1999/06/18 19:25:33  wsun
// More improvements to low momentum fitting.
//
// Revision 1.4  1999/06/02 23:04:20  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/05/14 01:06:34  wsun
// Protect against unavailability of geometry.
//
// Revision 1.2  1999/04/28 22:56:58  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:03:57  wsun
// Major reorganization and structural changes.
//

// system include files

// user include files

// DGTransform is a typedef, so it needs to be included not forward-declared.
#include "DetectorGeometry/DGTransform.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "ADRGeom/ADRSenseWireStore.h"

#include "CalibratedData/CalibratedDRHit.h"
//#include "DRHitCorrectorProd/CorrectedDRHit.h"

#include "DataHandler/DataKeyTags.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

//class CalibratedDRHit ;
class Frame ;
class HICondition ;
class HIHelixOperation ;
class HitCorrector ;
class HIIntersectionSurface ;
class TRSeedTrack ;
class AEWireLayerCylindrical ;

class HIDRSurfaceFactory
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      class DRHitAndDriftDistance
      {
	 public:
	    DRHitAndDriftDistance() : m_hit( ( const CalibratedDRHit* ) 0 ) {}

	    DRHitAndDriftDistance( const CalibratedDRHit* aHit,
				   const double& aDriftDistance )
	       : m_hit( aHit ),
		 m_driftDistance( aDriftDistance ) {}

	    DRHitAndDriftDistance( const DRHitAndDriftDistance& rhs )
	       : m_hit(           rhs.m_hit ),
		 m_driftDistance( rhs.m_driftDistance ),
		 m_averageChargeOnTrack( rhs.m_averageChargeOnTrack ) {}

	    const DRHitAndDriftDistance& operator=(
	       const DRHitAndDriftDistance& rhs )
	    {
	       if( this != &rhs )
	       {
		  m_hit           = rhs.m_hit ;
		  m_driftDistance = rhs.m_driftDistance ;
		  m_averageChargeOnTrack = rhs.m_averageChargeOnTrack ;
	       }
	       return *this;
	    }

	    virtual ~DRHitAndDriftDistance(){}

	    void clear()
	    {
	       m_hit = ( const CalibratedDRHit* ) 0 ;
	       m_driftDistance = 0. ;
	    }

	    void setAverageChargeOnTrack( const double& aCharge )
	    { m_averageChargeOnTrack = aCharge ; }

	    const CalibratedDRHit* calibratedDRHit() const { return m_hit ; }
	    const double& driftDistance() const { return m_driftDistance ; }
	    const double& averageChargeOnTrack() const
	    { return m_averageChargeOnTrack ; }
	    

	 private:
	    const CalibratedDRHit* m_hit ;
	    double m_driftDistance ;
	    double m_averageChargeOnTrack ;
      } ;

      // ---------- Constructors and destructor ----------------
      HIDRSurfaceFactory( Frame& theFrame,
			  DABoolean useAxialLayers = true,
			  DABoolean useStereoULayers = true,
			  DABoolean useStereoVLayers = true,
			  DABoolean correctDRHits = true,
			  const ADRSenseWireStore* aWireStore = 0 ) ;
      virtual ~HIDRSurfaceFactory();

      // ---------- member functions ---------------------------

      // Generate a HISingleWireCylinder for each hit, even if there are
      // multiple hits in a layer.
      void generateSingleWireSurfaces(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 const STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
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
	 const STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
	 HIHelixOperation* aOperation,
	 DABoolean aSingleCheckDirectionSwitch = true,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0. ) ;

      // Generate surfaces for all DR layers, making HIWireLayerCylinders
      // for layers with hits and HIDRLayerCylinders for layers without hits.
      void generateAllSurfacesSingle(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 const STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
	 HIHelixOperation* aOperationForLayersWithHits,
	 HIHelixOperation* aOperationForLayersWithoutHits,
	 int aFirstLayer = 0,
	 int aLastLayer = 0,
	 DABoolean aCheckDirectionSwitch = true,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0. ) ;

      void generateAllSurfacesMulti(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 const STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
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

      // Generate HIDRLayerCylinder surfaces for all DR layers by default.
      void generateSurfacesWithoutHits(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 HIHelixOperation* aOperation,
	 int aFirstLayer = 0,
	 int aLastLayer = 0 ) const ;

      // Generate two cylinders for DR inner tube.
      void generateInnerTubeCylinders(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 HIHelixOperation* aOperation ) const ;

      // Return value is the layer number of the outermost hit layer.
      int getDRHits(
	 const TRSeedTrack* theSeed,
	 STL_VECTOR( DRHitAndDriftDistance )& aDRHitInfos,
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
      HIDRSurfaceFactory( const HIDRSurfaceFactory& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIDRSurfaceFactory& operator=( const HIDRSurfaceFactory& ); // stop default

      // ---------- private member functions -------------------
      void clearHitsInLayer() ;

      HIIntersectionSurface* newSingleWireSurface(
	 const DRHitAndDriftDistance& aHitInfo,
	 HIHelixOperation* aOperation,
	 DABoolean aCheckCrossOrigin,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0. ) const ;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_geometryAvailable ;
      DABoolean m_useAxialLayers ;
      DABoolean m_useStereoULayers ;
      DABoolean m_useStereoVLayers ;

      DABoolean m_correctDRHits ;

      const ADRSenseWireStore* m_wireStore ;
      DGConstMaterialPtr m_drGas ;
      int m_hitsInLayer[ ADRSenseWireStore::kFirstLayer +
		         ADRSenseWireStore::kNumberOfLayers ] ;
      DABoolean m_countHitsInLayer ;
      Frame& m_frame ;

      const HitCorrector* m_hitCorrector ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIDRSurfaceFactory.cc"
//#endif

#endif /* HELIXINTERSECTION_HIDRSURFACEFACTORY_H */
