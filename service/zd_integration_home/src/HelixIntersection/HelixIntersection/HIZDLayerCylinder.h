#if !defined(HELIXINTERSECTION_HIZDLAYERCYLINDER_H)
#define HELIXINTERSECTION_HIZDLAYERCYLINDER_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIZDLayerCylinder
// 
/**\class HIZDLayerCylinder HIZDLayerCylinder.h HelixIntersection/HIZDLayerCylinder.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Jul 15 14:56:53 EDT 2003
// $Id: HIZDLayerCylinder.h,v 1.1 2003/07/26 03:28:14 mccann Exp $
//
// Revision history
//
// $Log: HIZDLayerCylinder.h,v $
// Revision 1.1  2003/07/26 03:28:14  mccann
// adding file for ZD surfaces
//

// system include files

// user include files
#include "HelixIntersection/HIDRLayerCylinder.h"

// forward declarations
class AEWireLayerCylindrical ;

class HIZDLayerCylinder : public HIDRLayerCylinder
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kSurfaceType ;

      // ---------- Constructors and destructor ----------------
      HIZDLayerCylinder();
      HIZDLayerCylinder(
	 const DGConstMaterialPtr& aInnerMaterial,
	 const DGConstMaterialPtr& aOuterMaterial,
	 const AEWireLayerCylindrical& aWireLayer,
	 int aLayerNumber,
	 const DGTransform& = DGTransform::Identity,
	 HICondition* = 0,
	 HIHelixOperation* = 0 ) ;
      virtual ~HIZDLayerCylinder();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HIZDLayerCylinder::kSurfaceType ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIZDLayerCylinder( const HIZDLayerCylinder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIZDLayerCylinder& operator=( const HIZDLayerCylinder& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIZDLayerCylinder.cc"
//#endif

#endif /* HELIXINTERSECTION_HIZDLAYERCYLINDER_H */
