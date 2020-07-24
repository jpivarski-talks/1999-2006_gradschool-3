#if !defined(HELIXINTERSECTION_HICOMPOSITEPLANE_H)
#define HELIXINTERSECTION_HICOMPOSITEPLANE_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICompositePlane
// 
/**\class HICompositePlane HICompositePlane.h HelixIntersection/HICompositePlane.h

 Description: The children of this composite can only be of type
              HIIntersectionPlane, HISiLadderPlane, or HISiSensorPlane.

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Mon Nov  8 22:30:43 EST 1999
// $Id: HICompositePlane.h,v 1.3 2001/02/22 02:23:38 wsun Exp $
//
// Revision history
//
// $Log: HICompositePlane.h,v $
// Revision 1.3  2001/02/22 02:23:38  wsun
// Avoid KTHelix::moveToPlane() by progapating helix as straight line.
//
// Revision 1.2  2000/06/29 17:28:19  wsun
// Added option for not checking boundary conditions.
//
// Revision 1.1  1999/11/21 22:22:58  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HICompositeSurface.h"

// forward declarations

class HICompositePlane : public HICompositeSurface
{
      // ---------- friend classes and functions ---------------
      static const string kSurfaceType ;

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      // If aCheckRphiBoundaries is false, then attemptHelixPropagation()
      // always returns true, and HICompositeSurface::propagateHelix() will
      // check ALL surfaces in the composite for an intersection.
      HICompositePlane( DABoolean aCheckRphiBoundaries = true );
      virtual ~HICompositePlane();

      // ---------- member functions ---------------------------
      // Verify that the surface is of type HIIntersectionPlane,
      // HISiLadderPlane, or HISiSensorPlane and call
      // HICompositeSurface::addChild().
      virtual void addChild( HIIntersectionSurface* aSurface ) ;

      // Add another HICompositePlane's children to this one.
      // Children added with this function will NOT be delete when the
      // destructor is called.
      virtual void addChildren( const HICompositeSurface& aCompositeSurface ) ;

      virtual DABoolean propagateHelix(
	 HIHelix& helixToPropagate,
	 KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      // ---------- const member functions ---------------------

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HICompositePlane::kSurfaceType ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------
      virtual DABoolean attemptHelixPropagation(
	 const HIHelix& helixAtCompositeRadius,
	 const HepPoint3D& positionAtCompositeRadius,
	 HIIntersectionSurface* aSurface ) const ;

   private:
      // ---------- Constructors and destructor ----------------
      HICompositePlane( const HICompositePlane& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HICompositePlane& operator=( const HICompositePlane& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_checkRphiBoundaries ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICompositePlane.cc"
//#endif

#endif /* HELIXINTERSECTION_HICOMPOSITEPLANE_H */
