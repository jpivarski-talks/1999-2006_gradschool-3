#if !defined(HELIXINTERSECTION_HIWIRELAYERCYLINDER_H)
#define HELIXINTERSECTION_HIWIRELAYERCYLINDER_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIWIRELayerCylinder
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Apr 20 14:21:48 EDT 1999
// $Id: HIWireLayerCylinder.h,v 1.6 2003/09/01 19:43:38 wsun Exp $
//
// Revision history
//
// $Log: HIWireLayerCylinder.h,v $
// Revision 1.6  2003/09/01 19:43:38  wsun
// Renamed HIDRLayerCylinder --> HIWireLayerCylinder.
//
// Revision 1.4  1999/11/21 22:26:47  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.3  1999/06/30 22:22:58  wsun
// Cylinders now keep a copy of the transform instead of a pointer.
//
// Revision 1.2  1999/04/28 22:56:57  wsun
// Fixes for Solaris.
//
// Revision 1.1  1999/04/27 22:03:56  wsun
// Major reorganization and structural changes.
//

// system include files

// user include files
#include "HelixIntersection/HIIntersectionCylinder.h"

// forward declarations
class AEWireLayerCylindrical ;

class HIWireLayerCylinder : public HIIntersectionCylinder
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kSurfaceType ;

      // ---------- Constructors and destructor ----------------
      HIWireLayerCylinder() ;
      HIWireLayerCylinder(
	 const DGConstMaterialPtr& aInnerMaterial,
	 const DGConstMaterialPtr& aOuterMaterial,
	 const AEWireLayerCylindrical& aWireLayer,
	 int aLayerNumber,
	 const DGTransform& = DGTransform::Identity,
	 HICondition* = 0,
	 HIHelixOperation* = 0 ) ;
      virtual ~HIWireLayerCylinder();

      // ---------- member functions ---------------------------
      void setWireLayer( const AEWireLayerCylindrical& aWireLayer,
			 int aLayerNumber )
      {
	 m_wireLayer = &aWireLayer ;
	 m_layerNumber = aLayerNumber ;
      }

      // ---------- const member functions ---------------------
      const AEWireLayerCylindrical& wireLayer() const
      { return *m_wireLayer ; }

      int layerNumber() const
      { return m_layerNumber ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HIWireLayerCylinder::kSurfaceType ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIWireLayerCylinder( const HIWireLayerCylinder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIWireLayerCylinder& operator=( const HIWireLayerCylinder& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const AEWireLayerCylindrical* m_wireLayer ;
      int m_layerNumber ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIWireLayerCylinder.cc"
//#endif

#endif /* HELIXINTERSECTION_HIWIRELAYERCYLINDER_H */
