#if !defined(HELIXINTERSECTION_HIINTERSECTIONCYLINDER_H)
#define HELIXINTERSECTION_HIINTERSECTIONCYLINDER_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIIntersectionCylinder
// 
// Description: HIIntersectionSurface subclass for defining cylinders.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Sep 25 16:47:21 EDT 1998
// $Id: HIIntersectionCylinder.h,v 1.6 1999/12/15 20:58:36 wsun Exp $
//
// Revision history
//
// $Log: HIIntersectionCylinder.h,v $
// Revision 1.6  1999/12/15 20:58:36  wsun
// Updated to new HIIntersectionSurface interface.
//
// Revision 1.5  1999/11/21 22:26:47  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.4  1999/06/30 22:22:59  wsun
// Cylinders now keep a copy of the transform instead of a pointer.
//
// Revision 1.3  1999/04/28 22:57:00  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:04:02  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:19  wsun
// First submission.
//

// system include files

// user include files

// DGTransform is a typedef, so it needs to be included not forward-declared.
#include "DetectorGeometry/DGTransform.h"
#include "HelixIntersection/HIIntersectionSurface.h"

// forward declarations

class HIIntersectionCylinder : public HIIntersectionSurface
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kSurfaceType ;

      // ---------- Constructors and destructor ----------------
      HIIntersectionCylinder();
      HIIntersectionCylinder(
	 const DGConstMaterialPtr& aInnerMaterial,
	 const DGConstMaterialPtr& aOuterMaterial,
	 Meters aRadius,
	 const DGTransform& = DGTransform::Identity,
	 HICondition* = 0,
	 HIHelixOperation* = 0 ) ;
      virtual ~HIIntersectionCylinder();

      // ---------- member functions ---------------------------
      void setTransform( const DGTransform& aTransform )
      {
	 m_transform = aTransform ;
	 m_transformIsIdentity = aTransform == DGTransform::Identity ;
      }

      // Returns original helix if transport fails.  The reference point is
      // placed at the helix' position on the cylinder.
      virtual DABoolean propagateHelix(
	 HIHelix& helixToPropagate,
	 KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      // ---------- const member functions ---------------------
      virtual void normal( const HIHelix&,
			   HepNormal3D& outputNormal ) const ;
      virtual void normal( const HepPoint3D&,
			   HepNormal3D& outputNormal ) const ;

      const DGTransform& transform() const { return m_transform ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HIIntersectionCylinder::kSurfaceType ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIIntersectionCylinder( const HIIntersectionCylinder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIIntersectionCylinder& operator=( const HIIntersectionCylinder& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      // HepTransform3D::operator==() compares addresses, not matrix elements!!
      DABoolean m_transformIsIdentity ;
      DGTransform m_transform ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIIntersectionCylinder.cc"
//#endif

#endif /* HELIXINTERSECTION_HIINTERSECTIONCYLINDER_H */
