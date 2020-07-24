#if !defined(HELIXINTERSECTION_HICATHODESURFACEFACTORY_H)
#define HELIXINTERSECTION_HICATHODESURFACEFACTORY_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICathodeSurfaceFactory
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sun Sep  5 16:35:37 EDT 1999
// $Id: HICathodeSurfaceFactory.h,v 1.6 2000/11/18 00:12:40 wsun Exp $
//
// Revision history
//
// $Log: HICathodeSurfaceFactory.h,v $
// Revision 1.6  2000/11/18 00:12:40  wsun
// Add member data for fitting weight scaling. Updated HICathodeCylinder ctor.
//
// Revision 1.5  2000/09/05 20:34:57  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.4  2000/07/25 21:34:35  wsun
// User can specify usage and and production tags for hits.
//
// Revision 1.3  2000/07/11 03:00:39  wsun
// Moved cathode hit radius to last DR layer.
//
// Revision 1.2  1999/10/13 08:57:46  wsun
// Allow user to give a ADRCathodeStore to the ctor.
//
// Revision 1.1  1999/09/10 21:20:33  wsun
// First submission.
//

// system include files

// user include files
#include "DetectorGeometry/DGTransform.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "DetectorGeometry/DGConstPointer.h"
#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DetectorGeometry/DGConstVolumeType.h"

#include "DataHandler/DataKeyTags.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class Frame ;
class HIIntersectionSurface ;
class HIHelixOperation ;
class ADRCathodeStore ;
class CalibratedCathodeHit ;
class TRSeedTrack ;

class HICathodeSurfaceFactory
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICathodeSurfaceFactory( Frame& aFrame,
			       const ADRCathodeStore* aCathodeStore = 0 ) ;
      virtual ~HICathodeSurfaceFactory();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // If instantiation falied, this is false.
      DABoolean geometryAvailable() const
      { return m_geometryAvailable ; }

      // Generate HIIntersectionCylinder at the cathode inner radius.
      void generateCylinderWithoutHits(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 HIHelixOperation* aOperation ) const ;

      void generateCylinderWithHits(
	 STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
	 const STL_VECTOR( const CalibratedCathodeHit* )& aHits,
	 HIHelixOperation* aOperation,
	 DABoolean aForceFittingWeight = false,
	 double aFittingWeight = 0.,
	 DABoolean aScaleFittingWeight = false,
	 double aFittingWeightScale = 1. ) const ;

      void getCathodeHits(
	 const TRSeedTrack* theSeed,
	 STL_VECTOR( const CalibratedCathodeHit* )& aHits,
         const UsageTag& aUsageTag = UsageTag(),
         const ProductionTag& aProdTag = ProductionTag() ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HICathodeSurfaceFactory( const HICathodeSurfaceFactory& );
      // stop default

      // ---------- assignment operator(s) ---------------------
      const HICathodeSurfaceFactory& operator=(
	 const HICathodeSurfaceFactory& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_geometryAvailable ;

      const ADRCathodeStore* m_cathodeStore ;
      DGConstMaterialPtr m_drGas ;
      DGConstMaterialPtr m_aluminum ;
      Frame& m_frame ;

      Meters m_radius ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICathodeSurfaceFactory.cc"
//#endif

#endif /* HELIXINTERSECTION_HICATHODESURFACEFACTORY_H */
