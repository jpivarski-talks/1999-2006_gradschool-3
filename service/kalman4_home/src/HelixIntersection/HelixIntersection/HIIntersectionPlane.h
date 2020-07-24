#if !defined(HELIXINTERSECTION_HIINTERSECTIONPLANE_H)
#define HELIXINTERSECTION_HIINTERSECTIONPLANE_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIIntersectionPlane
// 
// Description: HIIntersectionSurface subclass for defining planes.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Sep 25 16:46:38 EDT 1998
// $Id: HIIntersectionPlane.h,v 1.5 1999/12/15 20:43:00 wsun Exp $
//
// Revision history
//
// $Log: HIIntersectionPlane.h,v $
// Revision 1.5  1999/12/15 20:43:00  wsun
// Updated to new HIIntersectionSurface interface and added rphiHalfWidth().
//
// Revision 1.4  1999/11/21 22:26:48  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.3  1999/04/28 22:57:01  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:04:03  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:19  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HIIntersectionSurface.h"

#include "CLHEP/Geometry/Normal3D.h"
#include "CLHEP/Geometry/Point3D.h"

// forward declarations

class HIIntersectionPlane : public HIIntersectionSurface
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kSurfaceType ;

      // ---------- Constructors and destructor ----------------
      HIIntersectionPlane();
      HIIntersectionPlane(
	 const DGConstMaterialPtr& aInnerMaterial,
	 const DGConstMaterialPtr& aOuterMaterial,
	 const HepNormal3D& aNormalToPlane,
	 const HepPoint3D& aPointOnPlane,
	 Meters aRphiHalfWidth,
	 HICondition* = 0,
	 HIHelixOperation* = 0 ) ;
      virtual ~HIIntersectionPlane();

      // ---------- member functions ---------------------------
      void setNormal( const HepNormal3D& aNormal )
      { m_normal = aNormal ; }

      void setPoint( const HepPoint3D& aPoint )
      { m_point = aPoint ; }

      // Returns original helix if transport fails.  The reference point
      // is placed at the helix' position on the plane.
      virtual DABoolean propagateHelix(
	 HIHelix& helixToPropagate,
	 KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      // ---------- const member functions ---------------------
      virtual void normal( const HIHelix&, HepNormal3D& outputNormal ) const
      { outputNormal = m_normal ; }

      virtual void normal( const HepPoint3D&, HepNormal3D& outputNormal ) const
      { outputNormal = m_normal ; }

      const HepNormal3D& normal() const
      { return m_normal ; }

      const HepPoint3D& point() const
      { return m_point ; }

      // Used by HICompositePlane::attemptHelixPropagation().
      // Assumes planes are roughly oriented parallel to coordinate system.
      Meters rphiHalfWidth() const
      { return m_rphiHalfWidth ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HIIntersectionPlane::kSurfaceType ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIIntersectionPlane( const HIIntersectionPlane& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIIntersectionPlane& operator=( const HIIntersectionPlane& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      HepNormal3D m_normal ;
      HepPoint3D  m_point ;
      Meters      m_rphiHalfWidth ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIIntersectionPlane.cc"
//#endif

#endif /* HELIXINTERSECTION_HIINTERSECTIONPLANE_H */
