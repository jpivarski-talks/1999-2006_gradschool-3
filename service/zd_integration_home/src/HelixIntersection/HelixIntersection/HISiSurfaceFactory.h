#if !defined(HELIXINTERSECTION_HISISURFACEFACTORY_H)
#define HELIXINTERSECTION_HISISURFACEFACTORY_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HISiSurfaceFactory
// 
// Description: <one line class summary>
//
// Usage:
//
// The constructor takes a reference to the Frame, which is used to extract
// the Si hits on a given TRSeedTrack.  If the DGDetectorStore and ASiStore
// pointers are null (default), then these are extracted from the basegeometry
// record.  Otherwise, the user-supplied DGDetectorStore is used for all the
// volumes EXCEPT the silicon ladders, which are obtained from the ASiStore
// instead.  The reason for this difference is that the silicon volumes at
// the bottom of the parent tree are the sensors, not the ladders.  Treating
// each sensor separately is unneccessary and very time-consuming.
//
// N.B. If the user supplies the DGDetectorStore and ASiStore pointers, they
// should reflect the same geometry.  That is, the ASiStore should be
// initialized with the same DGDetectorStore given to the constructor.
//
//
// Author:      Werner Sun
// Created:     Sat Apr  3 02:26:28 EST 1999
// $Id: HISiSurfaceFactory.h,v 1.13 2001/03/29 01:40:46 llh14 Exp $
//
// Revision history
//
// $Log: HISiSurfaceFactory.h,v $
// Revision 1.13  2001/03/29 01:40:46  llh14
// Separated Si Rphi and Z Hit Surfaces
//
// Revision 1.12  2000/09/05 20:34:59  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.11  2000/07/25 22:32:47  wsun
// User can specify usage and and production tags for hits.
//
// Revision 1.10  2000/06/29 17:24:01  wsun
// Added option for no boundary conditions.
//
// Revision 1.9  1999/11/21 22:27:55  wsun
// Si sensor and diamond surfaces are now kept in HICompositePlanes.
//
// Revision 1.8  1999/10/14 18:53:36  wsun
// Added flag to transferSurfacesTo() to ignore composites w/o children.
//
// Revision 1.7  1999/07/24 03:46:50  wsun
// Allow user to supply DGDetectorStore and ASiStore.
//
// Revision 1.6  1999/06/30 22:26:17  wsun
// Added parseSilicon(), which creates surfaces for all the volumes in the Si DGDetectorStore.
//
// Revision 1.5  1999/06/24 22:18:25  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.4  1999/06/18 19:25:38  wsun
// More improvements to low momentum fitting.
//
// Revision 1.3  1999/05/14 01:06:35  wsun
// Protect against unavailability of geometry.
//
// Revision 1.2  1999/04/28 22:57:04  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:04:09  wsun
// Major reorganization and structural changes.
//

// system include files

// user include files
#include "DetectorGeometry/DGTransform.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "DetectorGeometry/DGConstPointer.h"
#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DetectorGeometry/DGConstVolumeType.h"

#include "SiGeom/SiGeomDescription.h"
#include "DataHandler/DataKeyTags.h"

#include "DBSVRPhiHitRadialCorrections.hh"
#include "FrameAccess/FAConstants.h"

#include "ASiStorePro/ASiLadder.h"
// forward declarations
#include "STLUtility/fwd_vector.h"

class CalibratedSVRphiHit;
class CalibratedSVZHit ;
class HIHelixOperation ;
class HIIntersectionSurface ;
class HICompositePlane ;
class ASiStore ;
class Frame ;
class TRSeedTrack ;
class DGDetectorStore ;

class HISiSurfaceFactory
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      // If the user supplies aDetectorStore and aSiStore, they should
      // reflect the same geometry.
      HISiSurfaceFactory( Frame& theFrame,
			  const DGDetectorStore* aDetectorStore = 0,
			  const ASiStore* aSiStore = 0,
			  DABoolean aGenerateBoundaryConditions = true ) ;
      virtual ~HISiSurfaceFactory();

      // ---------- member functions ---------------------------
      void generateLadderBoundarySurfaces(
	 HIHelixOperation* aInnerOperation,
	 HIHelixOperation* aOuterOperation ) ;

      void generateLadderCenterSurfaces(
	 HIHelixOperation* aOperation ) ;

      void generateCenterSurfacesWithHits(
	 const STL_VECTOR( const CalibratedSVRphiHit* )& aSVRphiHitInfos,
	 const STL_VECTOR( const CalibratedSVZHit* )& aSVZHitInfos,
	 HIHelixOperation* aOperation,
	 DABoolean aForceSVRFittingWeight = false,
	 double aSVRFittingWeight = 0.,
	 DABoolean aForceSVZFittingWeight = false,
	 double aSVZFittingWeight = 0. ) ;

      void transferSurfacesTo(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 DABoolean aTransferEmptySurfaces = true ) ;

      // ---------- const member functions ---------------------

      // If instantiation falied, this is false.
      DABoolean geometryAvailable() const
      { return m_geometryAvailable ; }

      // The vector given by the user is filled with the Si hits.
      void getSVRHits(
	 const TRSeedTrack* theSeed,
	 STL_VECTOR( const CalibratedSVRphiHit* )& aSVRHots,
	 const UsageTag& aUsageTag = UsageTag(),
         const ProductionTag& aProdTag = ProductionTag() ) const ;

      void getSVZHits(
	 const TRSeedTrack* theSeed,
	 STL_VECTOR( const CalibratedSVZHit* )& aSVZHots,
         const UsageTag& aUsageTag = UsageTag(),
         const ProductionTag& aProdTag = ProductionTag() ) const ;


      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HISiSurfaceFactory( const HISiSurfaceFactory& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HISiSurfaceFactory& operator=( const HISiSurfaceFactory& ); // stop default

      // ---------- private member functions -------------------
      void parseSilicon(
	 DGConstVolumeType< DGVolumeParentPlaced > aParent,
	 const DGTransform& transformToParent,
	 HIHelixOperation* aInnerOperation,
	 HIHelixOperation* aOuterOperation ) ;

      void makeSurfaces() ;

      HepPoint3D effRadialCorrections(const ASiLadder* aLadder );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_geometryAvailable ;
      DABoolean m_generateBoundaryConditions ;

      const DGDetectorStore* m_siStore ;
      DGConstMaterialPtr m_siGas ;
      DGConstMaterialPtr m_sensorMaterial ;
      const ASiStore* m_sensorStore ;
      STL_VECTOR( HICompositePlane* )* m_siSurfaces ;
      STL_VECTOR( HIIntersectionSurface* )* m_otherSurfaces ;
      DABoolean m_deleteSiSurfaces ;

      FAConstants< DBSVRPhiHitRadialCorrections > m_SetOfCorrections ;

      Frame& m_frame ;

      Meters m_layerBoundaries[ SiGeomDescription::kNLayers - 1 ] ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HISiSurfaceFactory.cc"
//#endif
#endif /* HELIXINTERSECTION_HISISURFACEFACTORY_H */
