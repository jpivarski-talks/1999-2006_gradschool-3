#if !defined(HELIXINTERSECTION_HIIRSURFACEFACTORY_H)
#define HELIXINTERSECTION_HIIRSURFACEFACTORY_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIIRSurfaceFactory
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sun Aug 29 22:29:51 EDT 1999
// $Id: HIIRSurfaceFactory.h,v 1.1 1999/08/30 21:32:43 wsun Exp $
//
// Revision history
//
// $Log: HIIRSurfaceFactory.h,v $
// Revision 1.1  1999/08/30 21:32:43  wsun
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

// forward declarations
#include "STLUtility/fwd_vector.h"

class HIIntersectionSurface ;
class HIHelixOperation ;
class Frame ;
class DGDetectorStore ;
class DGVolumeTube ;

class HIIRSurfaceFactory
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // If this were a static const int, it could not be used to initialize
      // the data member array lengths.
      enum{ kNumberOfTubes = 4 } ;

      // ---------- Constructors and destructor ----------------
      HIIRSurfaceFactory( Frame& theFrame,
			  const DGDetectorStore* aDetectorStore = 0 ) ;
      virtual ~HIIRSurfaceFactory();

      // ---------- member functions --------------------------

      // Only generate surfaces for the inner and outer beryllium cylinders,
      // the water between them, and the gold inner coating.
      void generateSurfaces(
         STL_VECTOR( HIIntersectionSurface* )& aSurfaces,
         HIHelixOperation* aOperation ) ;

      // ---------- const member functions ---------------------
      DABoolean geometryAvailable() const { return m_geometryAvailable ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIIRSurfaceFactory( const HIIRSurfaceFactory& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIIRSurfaceFactory& operator=( const HIIRSurfaceFactory& ); // stop default

      // ---------- private member functions -------------------
      void parseIR(
	 DGConstVolumeType< DGVolumeParentPlaced > aParent,
	 const DGTransform& transformToParent ) ;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Frame& m_frame ;
      DABoolean m_geometryAvailable ;
      const DGDetectorStore* m_irStore ;

      DGTransform m_transforms[ kNumberOfTubes ] ;
      const DGVolumeTube* m_tubes[ kNumberOfTubes ] ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIIRSurfaceFactory.cc"
//#endif

#endif /* HELIXINTERSECTION_HIIRSURFACEFACTORY_H */
