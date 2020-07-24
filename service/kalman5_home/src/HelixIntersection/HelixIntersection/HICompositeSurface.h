#if !defined(HELIXINTERSECTION_HICOMPOSITESURFACE_H)
#define HELIXINTERSECTION_HICOMPOSITESURFACE_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICompositeSurface
// 
// Description: Composite of HIIntersectionSurface.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Thu Sep 24 17:58:28 EDT 1998
// $Id: HICompositeSurface.h,v 1.17 2001/03/27 05:28:20 wsun Exp $
//
// Revision history
//
// $Log: HICompositeSurface.h,v $
// Revision 1.17  2001/03/27 05:28:20  wsun
// Added m_lastHelixAtSurface.
//
// Revision 1.16  2001/02/22 02:01:15  wsun
// Set condition and operation delete flags.
//
// Revision 1.15  2000/11/18 00:08:57  wsun
// Added checkPropagation() flag.
//
// Revision 1.14  2000/06/09 23:46:30  wsun
// Moved correctHitsForHelix() to HICalibratedHit.
//
// Revision 1.13  2000/06/09 00:31:25  wsun
// Added correctHitsForHelix().
//
// Revision 1.12  2000/03/02 08:25:23  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.11  1999/12/15 20:36:36  wsun
// * Updated to new HIIntersectionSurface interface.
// * Added STL vector to cache list of surfaces in range of helix, instead of
//   redetermining every time propagateHelix() is called.
//
// Revision 1.10  1999/11/21 22:26:46  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.9  1999/09/30 00:49:46  wsun
// Added advanceToNextCalibratedHit().
//
// Revision 1.8  1999/08/27 20:17:18  wsun
// Changed signature for performOperation().
//
// Revision 1.7  1999/08/03 21:43:54  wsun
// Added implementation of numberOfAssociatedHits.
//
// Revision 1.6  1999/06/30 22:21:19  wsun
// Added addChildren() function.
//
// Revision 1.5  1999/06/24 22:18:21  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.4  1999/06/02 23:04:19  wsun
// Added features for low-momentum fitting.
//
// Revision 1.3  1999/04/28 22:56:56  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:03:54  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:17  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HIIntersectionSurface.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Geometry/Normal3D.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HICompositeSurface : public HIIntersectionSurface
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kSurfaceType ;

      // ---------- Constructors and destructor ----------------
      HICompositeSurface();
      virtual ~HICompositeSurface();

      // ---------- member functions ---------------------------

      virtual DABoolean advanceToNextCalibratedHit( HIHelix& aHelix )
      { return ( m_intersectedSurface == 0 ) ?
	   false : m_intersectedSurface->advanceToNextCalibratedHit( aHelix );}

      // Children added with this function will be deleted when the
      // destructor is called.
      virtual void addChild( HIIntersectionSurface* aSurface ) ;

      // Add another composite surface's children to this one.
      // Children added with this function will NOT be delete when the
      // destructor is called.
      virtual void addChildren( const HICompositeSurface& aCompositeSurface ) ;

      virtual void deleteChildren() ;

      // Delete conditions and operations of all children owned by
      // composite (i.e. not the ones added with addChildren() ).
      virtual void deleteCondition() ;
      virtual void deleteOperation() ;

      void deleteConditions() { deleteCondition() ; }
      void deleteOperations() { deleteOperation() ; }

      // Returns surface to state before last propagation.
      void revertIntersectedSurface() ;

      // Clear the vector of intersected surfaces.
      virtual void reset() ;

      virtual void revertToPreviousState() ;

      virtual void
      addHICalibratedHitsToVector( STL_VECTOR( HICalibratedHit* )& ) ;

      virtual void setPrintDiagnostics( DABoolean flag ) ;

      virtual void setUseReverseHitOrder( DABoolean aFlag ) ;

      // ---------- const member functions ---------------------
      virtual int numberOfAssociatedHits() const ;

      virtual const HIIntersectionSurface* getChild( int iChild ) const ;

      virtual int numberOfChildren() const ;

      virtual const HIIntersectionSurface* intersectedSurface() const
      { return m_intersectedSurface ; }

      virtual const HIIntersectionSurface* lastIntersectedSurface() const ;

      virtual Meters dca( const HIHelix& aHelix ) const
      { return ( m_intersectedSurface == 0 ) ?
	   0. : m_intersectedSurface->dca( aHelix ) ; }

      virtual void derivatives(
	 const HIHelix& aHelix, HepVector& outputVector ) const
      { if( m_intersectedSurface == 0 )
	   m_intersectedSurface->derivatives( aHelix, outputVector ) ; }

      virtual Meters fittingWeight() const
      { return ( m_intersectedSurface == 0 ) ?
	   0. : m_intersectedSurface->fittingWeight() ; }

      virtual Meters measuredDca() const
      { return ( m_intersectedSurface == 0 ) ?
	   0. : m_intersectedSurface->measuredDca() ; }

      virtual const CalibratedHit* calibratedHit() const
      { return ( m_intersectedSurface == 0 ) ?
	   0 : m_intersectedSurface->calibratedHit() ; }

      virtual void normal( const HIHelix& aHelix,
			   HepNormal3D& outputNormal ) const
      { if( m_intersectedSurface != 0 )
	   m_intersectedSurface->normal( aHelix, outputNormal ) ; }

      virtual void normal( const HepPoint3D& aPoint,
			   HepNormal3D& outputNormal ) const
      { if( m_intersectedSurface != 0 )
	   m_intersectedSurface->normal( aPoint, outputNormal ) ; }

      // Find surface with smallest arc length from current helix position.
      // This surface will be pointed to by m_intersectedSurface.
      // Original helix returned if transport fails.
      // No conditions are applied.
      virtual DABoolean propagateHelix(
	 HIHelix& helixToPropagate,
	 KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      virtual DABoolean isComposite() const { return true ; }

      virtual DABoolean conditionSatisfied( const HIHelix& aHelix )
      { return ( m_intersectedSurface == 0 ) ? false :
	 m_intersectedSurface->conditionSatisfied( aHelix ) ; }

      virtual DABoolean performOperation(
	 HIHelix& aHelix,
	 const HIHelix& aLastHelix,
	 const HIIntersectionSurface& aLastSurface ) ;

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HICompositeSurface::kSurfaceType ; }

      // A flag to tell HIHelixIntersection whether or not to check if
      // the origin was crossed or the path length was too large or small.
      virtual DABoolean checkPropagation() const
      { return ( m_intersectedSurface == 0 ) ? true :
	 m_intersectedSurface->checkPropagation() ; }

      // ---------- static member functions --------------------

   protected:
      STL_VECTOR( HIIntersectionSurface* )* m_surfacesInRange ;
      STL_VECTOR( HIIntersectionSurface* )* m_surfaces ;
      DABoolean m_helixAtSurface ;
      DABoolean m_lastHelixAtSurface ;
      STL_VECTOR( HIIntersectionSurface* )* m_intersectedSurfaces ;
      HIIntersectionSurface* m_intersectedSurface ;

      // ---------- protected member functions -----------------
      void setIntersectedSurface() ;

      // ---------- protected const member functions -----------
      virtual DABoolean attemptHelixPropagation(
	 const HIHelix& helixAtCompositeRadius,
	 const HepPoint3D& positionAtCompositeRadius,
	 HIIntersectionSurface* aSurface ) const { return true ; }

   private:
      // ---------- Constructors and destructor ----------------
      HICompositeSurface( const HICompositeSurface& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HICompositeSurface& operator=( const HICompositeSurface& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // The destructor should only delete surfaces that it owns.
      STL_VECTOR( HIIntersectionSurface* )* m_surfacesToDelete ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICompositeSurface.cc"
//#endif

#endif /* HELIXINTERSECTION_HICOMPOSITESURFACE_H */
