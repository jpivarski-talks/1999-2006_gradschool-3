#if !defined(HELIXINTERSECTION_HIRPHIBOUNDARYCONDITION_H)
#define HELIXINTERSECTION_HIRPHIBOUNDARYCONDITION_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIRphiBoundaryCondition
// 
// Description: HICondition subclass for specifying r-phi boundary conditions.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Thu Oct 29 18:48:39 EST 1998
// $Id: HIRphiBoundaryCondition.h,v 1.2 1999/04/27 22:04:06 wsun Exp $
//
// Revision history
//
// $Log: HIRphiBoundaryCondition.h,v $
// Revision 1.2  1999/04/27 22:04:06  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:18  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HICondition.h"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/Point3D.h"

// forward declarations
class HIHelix ;

class HIRphiBoundaryCondition : public HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIRphiBoundaryCondition();

      // The center point lies in the plane halfway between the two
      // r-phi edges of the region.

      // The tangent vector lies in the measurement direction (in the plane),
      // perpendicular to the r-phi edges of the region.
      HIRphiBoundaryCondition( const HepPoint3D& aCenterPoint,
			       const Hep3Vector& aTangentVector,
			       Meters aHalfWidth ) ;
      virtual ~HIRphiBoundaryCondition();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      const HepPoint3D& centerPoint() const { return m_centerPoint ; }
      const Hep3Vector& tangentVector() const { return m_tangentVector ; }
      Meters halfWidth() const { return m_halfWidth ; }

      // Returns true if the helix lies within the r-phi boundaries.
      virtual DABoolean satisfied( const HIHelix& aHelix,
				   HIIntersectionSurface& ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIRphiBoundaryCondition( const HIRphiBoundaryCondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIRphiBoundaryCondition& operator=( const HIRphiBoundaryCondition& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      HepPoint3D m_centerPoint ;
      Hep3Vector m_tangentVector ;
      Meters m_halfWidth ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIRphiBoundaryCondition.cc"
//#endif

#endif /* HELIXINTERSECTION_HIRPHIBOUNDARYCONDITION_H */
