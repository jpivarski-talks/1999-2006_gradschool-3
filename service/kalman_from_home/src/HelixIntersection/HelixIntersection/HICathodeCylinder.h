#if !defined(HELIXINTERSECTION_HICATHODECYLINDER_H)
#define HELIXINTERSECTION_HICATHODECYLINDER_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICathodeCylinder
// 
// Description: HIIntersectionSurface subclass for defining cylinders with
//              a CalibratedCathodeHit.  Propagation to this surface entails
//              swimming to the cathode radius.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:28:18 EDT 1999
// $Id: HICathodeCylinder.h,v 1.6 2000/11/18 00:09:19 wsun Exp $
//
// Revision history
//
// $Log: HICathodeCylinder.h,v $
// Revision 1.6  2000/11/18 00:09:19  wsun
// Added checkPropagation() flag and ability to scale fitting weight.
//
// Revision 1.5  2000/03/02 08:25:22  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.4  2000/02/03 07:14:59  wsun
// Added use of HICalibratedHits.
//
// Revision 1.3  1999/12/15 20:38:08  wsun
// Updated to new HIIntersectionSurface interface and sped up derivatives().
//
// Revision 1.2  1999/11/21 22:26:45  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.1  1999/09/10 21:20:34  wsun
// First submission.
//
//

// system include files

// user include files
#include "HelixIntersection/HIIntersectionCylinder.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CalibratedData/CalibratedCathodeHit.h"
#include "HelixIntersection/HICalibratedCathodeHit.h"

#include <vector>

// forward declarations
class ADRSenseWireStore ;

#include "STLUtility/fwd_vector.h"

class HICathodeCylinder : public HIIntersectionCylinder
{
      // ---------- friend classes and functions ---------------
      static const string kSurfaceType ;

   public:
      // ---------- constants, enums and typedefs --------------
      typedef pair< HICalibratedCathodeHit, DABoolean > HitAndUsed ;

      // ---------- Constructors and destructor ----------------
      HICathodeCylinder();
      HICathodeCylinder(
         const DGConstMaterialPtr& aInnerMaterial,
         const DGConstMaterialPtr& aOuterMaterial,
         Meters aRadius,
         const DGTransform& = DGTransform::Identity,
         HICondition* = 0,
         HIHelixOperation* = 0,
	 const Hep3Vector& aMeasurementDirection = Hep3Vector( 0., 0., 1. ),
	 const STL_VECTOR( const CalibratedCathodeHit* )* = 0,
	 const ADRSenseWireStore* aSenseWireStore = 0,
	 DABoolean aScaleFittingWeight = false,
	 DABoolean aFittingWeightScale = 1.,
	 DABoolean checkDirectionSwitch = true ) ;
      virtual ~HICathodeCylinder();

      // ---------- member functions ---------------------------
      void addHit( const CalibratedCathodeHit* aHit )
      { m_hits.push_back( HitAndUsed( HICalibratedCathodeHit( aHit ),
				      false ) ) ; }

      // If this flag is true, then the given helix will only be propagated
      // to those this where momentum.normal has the same sign as when
      // transported to the radius of the layer.
      void setCheckDirectionSwitch( DABoolean flag )
      { m_checkDirectionSwitch = flag ; }

      // Swim to cathode radius.  Returns original helix if transport fails.
      virtual DABoolean propagateHelix(
	 HIHelix& helixToPropagate,
	 KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      virtual DABoolean advanceToNextCalibratedHit( HIHelix& ) ;

      virtual void reset() ;
      virtual void resetHICalibratedHits() ;
      virtual void revertToPreviousState() ;

      virtual void
      addHICalibratedHitsToVector( STL_VECTOR( HICalibratedHit* )& aVector ) ;

      // ---------- const member functions ---------------------
      virtual Meters dca( const HIHelix& ) const ;
      virtual void derivatives(
	 const HIHelix&, HepVector& outputVector ) const ;

      virtual Meters fittingWeight() const ;
      virtual Meters measuredDca() const { return 0. ; }
      virtual const CalibratedHit* calibratedHit() const ;
      virtual HICalibratedHit* hiCalibratedHit() const ;
      virtual int numberOfAssociatedHits() const { return m_hits.size() ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HICathodeCylinder::kSurfaceType ; }

      // A flag to tell HIHelixIntersection whether or not to check if
      // the origin was crossed or the path length was too large or small.
      // Set to false for cathodes because the track may already by at
      // layer 47, in which case, the helix propagation will not move it.
      virtual DABoolean checkPropagation() const { return false ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HICathodeCylinder( const HICathodeCylinder& );
      // stop default

      // ---------- assignment operator(s) ---------------------
      const HICathodeCylinder& operator=
      ( const HICathodeCylinder& ); // stop default

      // ---------- private member functions -------------------
      DABoolean findNextHit( const HIHelix& aHelix ) ;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Hep3Vector m_measurementDirection ;

      STL_VECTOR( HitAndUsed ) m_hits ;
      STL_VECTOR( HitAndUsed* ) m_usedHits ;

      DABoolean m_checkDirectionSwitch ;

      const ADRSenseWireStore* m_wireStore ;

      DABoolean m_scaleFittingWeight ;
      double m_fittingWeightScale ;

      // ---------- static data members ------------------------
};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICathodeCylinder.cc"
//#endif

#endif /* HELIXINTERSECTION_HISINGLECATHODECYLINDER_H */
