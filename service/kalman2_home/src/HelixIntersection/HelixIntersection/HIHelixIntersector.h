#if !defined(HELIXINTERSECTION_HIHELIXINTERSECTOR_H)
#define HELIXINTERSECTION_HIHELIXINTERSECTOR_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIHelixIntersector
// 
// Description: The navigation object that provides the interface between
//              the detector geometry (specified by the user as a vector of
//              HIIntersectionSurface objects), helices (HIHelix), and
//              track finders, fitters, MC generators, etc.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Sep 21 17:20:49 EDT 1998
// $Id: HIHelixIntersector.h,v 1.11 2001/03/27 05:29:19 wsun Exp $
//
// Revision history
//
// $Log: HIHelixIntersector.h,v $
// Revision 1.11  2001/03/27 05:29:19  wsun
// Added setClosestSurfaceToCurrentByRadius().
//
// Revision 1.10  2000/02/03 07:18:51  wsun
// Added setClosestSurfaceToCurrent() and helixCurling().
//
// Revision 1.9  1999/09/30 00:50:48  wsun
// Turned currentSurface() into a non-const function.
//
// Revision 1.8  1999/08/27 20:21:38  wsun
// Added resetSurfaces().
//
// Revision 1.7  1999/08/03 21:51:06  wsun
// Modifications for handling curlers.
//
// Revision 1.6  1999/06/24 22:18:23  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.5  1999/06/18 19:25:35  wsun
// More improvements to low momentum fitting.
//
// Revision 1.4  1999/06/02 23:03:20  wsun
// Added status enum.
//
// Revision 1.3  1999/04/28 22:56:59  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:04:00  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:16  wsun
// First submission.
//

// system include files

// user include files
#include "KinematicTrajectory/KTMoveControl.h"
#include "HelixIntersection/HIHelix.h"

#include <vector>

// forward declarations
#include "STLUtility/fwd_vector.h"

class HIIntersectionSurface ;
class HICondition ;
class HIHelixOperation ;


class HIHelixIntersector
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef double Meters ;
      typedef double GeV ;

      enum SurfaceOrder { kDecreasingRadius = -1,
			  kIncreasingRadius =  1,
			  kOrderAsGiven = 0 } ;

      enum IntersectionStatus { kIntersectionOK,
				kHelixTransportError,
				kGlobalOperationError,
				kSurfaceOperationError,
				kNoIntersection,
				kNoMoreSurfaces } ;

      // ---------- Constructors and destructor ----------------
      HIHelixIntersector();

      HIHelixIntersector( const STL_VECTOR( HIIntersectionSurface* )&,
			  SurfaceOrder anOrder,
			  HIHelix* aHelix = 0,
			  const HICondition* globalCondition = 0,
			  const HIHelixOperation* globalOperation = 0,
			  DABoolean checkCrossOrigin = true,
			  DABoolean attemptCurling = true ) ;

      HIHelixIntersector( const HIHelixIntersector& );

      const HIHelixIntersector& operator=( const HIHelixIntersector& );

      virtual ~HIHelixIntersector();


      // ---------- member functions ---------------------------
      void setSurfaces( const STL_VECTOR( HIIntersectionSurface* )& surfaces,
			SurfaceOrder aOrder = kIncreasingRadius )
      { m_surfaces = surfaces ; sortSurfaces( aOrder ) ; }

      // Set m_currentSurface to the surface in the list that matches
      // the given one.  Return value is false if no match is found.
      DABoolean setCurrentSurface( const HIIntersectionSurface* aSurface ) ;

      void setHelix( HIHelix* aHelix )
      { m_helix = aHelix ; }

      void setGlobalCondition( const HICondition* aGlobalCondition )
      { m_globalCondition = aGlobalCondition ; }

      void setGlobalOperation( const HIHelixOperation* aGlobalOperation )
      { m_globalOperation = aGlobalOperation ; }

      void setPrintDiagnostics( DABoolean flag )
      { m_printDiagnostics = flag ; }

      // Reject a propagation if the track crosses the origin -- set this
      // to false for dual track fits or for cosmic rays.
      void setCheckCrossOrigin( DABoolean flag )
      { m_checkCrossOrigin = flag ; }

      // If a track fails to propagate to the next layer, should
      // HIHelixIntersector attempt to repropagate to the current layer?
      void setAttemptCurling( DABoolean flag ) { m_attemptCurling = flag ; }

      void sortSurfaces( SurfaceOrder ) ;

      // Go back to the first surface.
      void resetSurfacePointer() { m_currentSurface = m_surfaces.begin() ; }

      // Call each surface's reset() function.
      void resetSurfaces() ;

      // Perform ray tracing to determine next surface intersected
      // and propagate there.
      IntersectionStatus swimToNextIntersection(
	 const KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      IntersectionStatus swimToSurface(
	 const HIIntersectionSurface*,
	 const KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      IntersectionStatus swimToCurrentSurface(
	 const KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      IntersectionStatus swimWhile(
	 const HICondition*,
	 const KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      // Find the surface that yields the smallest propagation arc length for
      // the intersection helix in the given direction.  If no direction is
      // given, then the closest surface in either direction is set to current.
      void setClosestSurfaceToCurrent(
	 const KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      // Find the surface with the radius closest to that of the
      // intersection helix in the given direction.  If no direction is given,
      // then the closest surface in either direction is set to current.
      // Much faster than setClosestSurfaceToCurrent(...).
      void setClosestSurfaceToCurrentByRadius(
	 const KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      HIIntersectionSurface* currentSurface() ;

      // ---------- const member functions ---------------------
      const HIIntersectionSurface* nextSurface(
	 const KTMoveControl::MoveDirection aDirection ) const ;

      const STL_VECTOR( HIIntersectionSurface* )&
      vectorOfSurfaces() const
      { return m_surfaces ; }

      HIHelix* helix() const
      { return m_helix ; }

      const HICondition* globalCondition() const
      { return m_globalCondition ; }

      const HIHelixOperation* globalOperation() const
      { return m_globalOperation ; }

      DABoolean printDiagnostics() const { return m_printDiagnostics ; }
      DABoolean checkCrossOrigin() const { return m_checkCrossOrigin ; }
      DABoolean attemptCurling() const   { return m_attemptCurling ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------
      IntersectionStatus performOperation(
	 const HIHelix& oldHelix,
	 STL_VECTOR( HIIntersectionSurface* )::iterator oldSurface,
	 const KTMoveControl::MoveDirection aDirection ) ;

      int nextSurfaceIncrement(
	 const KTMoveControl::MoveDirection aDirection ) const ;

      // Returns -1 if helix is moving with decreasing radius, +1 if positive.
      int helixCurling() const ;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      STL_VECTOR( HIIntersectionSurface* ) m_surfaces ;
      STL_VECTOR( HIIntersectionSurface* )::iterator m_currentSurface ;

      SurfaceOrder m_surfaceOrder ;
      HIHelix* m_helix ;

      const HICondition* m_globalCondition ;
      const HIHelixOperation* m_globalOperation ;

      DABoolean m_printDiagnostics ;
      DABoolean m_checkCrossOrigin ;
      DABoolean m_attemptCurling ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIHelixIntersector.cc"
//#endif

#endif /* HELIXINTERSECTION_HIHELIXINTERSECTOR_H */
