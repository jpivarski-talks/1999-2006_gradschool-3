#if !defined(HELIXINTERSECTION_HIMULTIWIRECYLINDER_H)
#define HELIXINTERSECTION_HIMULTIWIRECYLINDER_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIMultiWireCylinder
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sun Jul 25 14:41:09 EDT 1999
// $Id: HIMultiWireCylinder.h,v 1.9 2000/12/02 01:30:34 wsun Exp $
//
// Revision history
//
// $Log: HIMultiWireCylinder.h,v $
// Revision 1.9  2000/12/02 01:30:34  wsun
// Added avg chg on track to HICalibratedDRHit and DRHitAndDriftDistance.
//
// Revision 1.8  2000/06/09 23:46:32  wsun
// Moved correctHitsForHelix() to HICalibratedHit.
//
// Revision 1.7  2000/06/09 00:31:27  wsun
// Added correctHitsForHelix().
//
// Revision 1.6  2000/03/02 08:25:24  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.5  2000/02/03 07:15:00  wsun
// Added use of HICalibratedHits.
//
// Revision 1.4  1999/11/21 22:26:50  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.3  1999/09/10 21:23:04  wsun
// Added a vector to keep track of used hits.
//
// Revision 1.2  1999/08/27 20:21:11  wsun
// Keep track of last intersected hit.
//
// Revision 1.1  1999/08/03 21:40:12  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HISingleWireCylinder.h"

#include <vector>

// forward declarations
#include "STLUtility/fwd_vector.h"
class HitCorrector ;

class HIMultiWireCylinder : public HISingleWireCylinder
{
      // ---------- friend classes and functions ---------------
      static const string kSurfaceType ;
      enum HitFound { kNoHitFound, kUsedHitFound, kUnusedHitFound } ;

   public:
      // ---------- constants, enums and typedefs --------------
      typedef pair< HICalibratedDRHit, DABoolean > HitAndUsed ;

      // ---------- Constructors and destructor ----------------
      HIMultiWireCylinder();
      HIMultiWireCylinder(
	 const DGConstMaterialPtr& aInnerMaterial,
	 const DGConstMaterialPtr& aOuterMaterial,
	 const AEWireLayerCylindrical& aWireLayer,
	 int aLayerNumber,
	 const DGTransform& = DGTransform::Identity,
	 HICondition* = 0,
	 HIHelixOperation* = 0,
	 DABoolean checkMultiDirectionSwitch = true,  // see below.
	 const STL_VECTOR( HitAndUsed )* = 0 ) ; // vector gets copied.
      virtual ~HIMultiWireCylinder();

      // ---------- member functions ---------------------------
      void addHit( const CalibratedDRHit* aHit,
		   const HitCorrector* aHitCorrector,
		   Meters aSignedDriftDistance,
		   const double& aAverageChargeOnTrack )
      {
	 m_hits.push_back(
	    HitAndUsed(
	       HICalibratedDRHit( aHit,
				  aHitCorrector,
				  aSignedDriftDistance,
				  aAverageChargeOnTrack,
				  aHit == 0 ? 0. : aHit->time() ),
	       false ) ) ;
      }

      // If this flag is true, then the given helix will only be propagated
      // to those this where momentum.normal has the same sign as when
      // transported to the radius of the layer.
      void setCheckDirectionSwitch( DABoolean flag )
      { m_checkMultiDirectionSwitch = flag ; }

      // First, transport the helix to the radius of the layer.  Then,
      // transport to the first hit in the list for which momentum.normal
      // does not change sign (if m_checkMultiDirectionSwitch is true).
      virtual DABoolean propagateHelix(
	 HIHelix& helixToPropagate,
	 KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      // The given helix should have already been transported to the first
      // hit by propagateHelix(...).  Advancing to subsequent hits will
      // repropagate the helix to the PCA to the wire.  Energy loss and
      // multiple scattering will NOT be applied for these transports.
      virtual DABoolean advanceToNextCalibratedHit( HIHelix& ) ;

      virtual void reset() ;
      virtual void resetHICalibratedHits() ;
      virtual void revertToPreviousState() ;

      virtual void
      addHICalibratedHitsToVector( STL_VECTOR( HICalibratedHit* )& aVector ) ;

      // ---------- const member functions ---------------------

      virtual int numberOfAssociatedHits() const { return m_hits.size() ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HIMultiWireCylinder::kSurfaceType ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIMultiWireCylinder( const HIMultiWireCylinder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIMultiWireCylinder& operator=( const HIMultiWireCylinder& );
      // stop default

      // ---------- private member functions -------------------
      HitFound propagateToNextWire( HIHelix& aHelix, DABoolean aTestUsedHits );
      void setSingleHitAndDist() ;
      void setSingleHitAndDist( HitAndUsed* ) ;

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      STL_VECTOR( HitAndUsed )  m_hits ;
      STL_VECTOR( HitAndUsed* ) m_usedHits ;

      DABoolean m_checkMultiDirectionSwitch ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIMultiWireCylinder.cc"
//#endif

#endif /* HELIXINTERSECTION_HIMULTIWIRECYLINDER_H */
