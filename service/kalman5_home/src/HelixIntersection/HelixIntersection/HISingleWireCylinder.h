#if !defined(HELIXINTERSECTION_HISINGLEWIRECYLINDER_H)
#define HELIXINTERSECTION_HISINGLEWIRECYLINDER_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HISingleWireCylinder
// 
// Description: HIIntersectionSurface subclass for defining cylinders with
//              a CalibratedDRHit.  Propagation to this surface entails
//              swimming to the PCA to the hit wire.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Sep 25 19:01:30 EDT 1998
// $Id: HISingleWireCylinder.h,v 1.12 2000/12/02 01:30:35 wsun Exp $
//
// Revision history
//
// $Log: HISingleWireCylinder.h,v $
// Revision 1.12  2000/12/02 01:30:35  wsun
// Added avg chg on track to HICalibratedDRHit and DRHitAndDriftDistance.
//
// Revision 1.11  2000/10/11 05:54:28  wsun
// Use fitting weight from HICalibratedHit, if available.
//
// Revision 1.10  2000/09/05 20:34:59  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.9  2000/06/09 23:46:32  wsun
// Moved correctHitsForHelix() to HICalibratedHit.
//
// Revision 1.8  2000/06/09 00:31:28  wsun
// Added correctHitsForHelix().
//
// Revision 1.7  2000/03/02 08:25:24  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.6  2000/02/22 17:19:52  wsun
// Allow users to set m_checkDirectionSwitch.
//
// Revision 1.5  2000/02/03 07:15:01  wsun
// Added use of HICalibratedHits.
//
// Revision 1.4  1999/12/15 21:01:15  wsun
// * Updated to new HIIntersectionSurface interface.
// * propagateHelix() caches the wire position and direction at the helix' z,
//   which is later used in dca() and derivatives().
// * Assume uniform B field in z direction to speed up derivatives().
//
// Revision 1.3  1999/11/21 22:26:53  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.2  1999/09/10 21:22:11  wsun
// Allow for null pointer to calibrated hit.
//
// Revision 1.1  1999/08/03 21:40:37  wsun
// Renamed from HIWireLayerCylinder.
//
// Revision 1.4  1999/06/30 22:26:55  wsun
// Updated to new HIIntersectionCylinder interface.
//
// Revision 1.3  1999/04/28 22:57:05  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:04:11  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:20  wsun
// First submission.
//

// system include files

// user include files
#include "ActiveElements/AEWireInfo.h"
#include "HelixIntersection/HIDRLayerCylinder.h"
#include "HelixIntersection/HICalibratedDRHit.h"

// forward declarations
class HitCorrector ;

class HISingleWireCylinder : public HIDRLayerCylinder
{
      // ---------- friend classes and functions ---------------
      static const string kSurfaceType ;

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HISingleWireCylinder();
      HISingleWireCylinder(
	 const DGConstMaterialPtr&      aInnerMaterial,
	 const DGConstMaterialPtr&      aOuterMaterial,
	 const AEWireLayerCylindrical&  aWireLayer,
	 int                            aLayerNumber,
	 const DGTransform&             = DGTransform::Identity,
	 HICondition*                   = 0,
	 HIHelixOperation*              = 0,
	 const CalibratedDRHit*         = 0,
	 Meters                         aSignedDriftDistance = 0.,
	 const HitCorrector*            aHitCorrector = 0,
	 DABoolean                      aCheckDirectionSwitch = true,
	 double                         aAverageChargeOnTrack = 0. ) ;
      virtual ~HISingleWireCylinder();

      // ---------- member functions ---------------------------
      void setHICalibratedDRHit( HICalibratedDRHit* aHit ) ;
      void setCalibratedDRHit( const CalibratedDRHit* aHit ) ;
      void setSignedDriftDistance( Meters aSignedDriftDistance ) ;

      virtual void reset() { m_pDRHit = &m_drHit ; }
      virtual void resetHICalibratedHits() { m_drHit.clear() ; }

      // Swim to PCA to wire, NOT to radius of wire layer.  Use the
      // moveToLine member function of HIHelix, which handles both
      // axial and stereo wires.
      //
      // Returns original helix if transport fails.
      virtual DABoolean propagateHelix(
	 HIHelix& helixToPropagate,
	 KTMoveControl::MoveDirection aDirection =
	 KTMoveControl::kDirectionEither ) ;

      virtual void
      addHICalibratedHitsToVector( STL_VECTOR( HICalibratedHit* )& aVector ) ;

      // ---------- const member functions ---------------------
      virtual Meters dca( const HIHelix& ) const ;
      virtual void derivatives(
	 const HIHelix&, HepVector& outputVector ) const ;

      virtual Meters fittingWeight() const
      {
	 if( m_forceFittingWeight )
	 {
	    return m_fittingWeight ;
	 }
	 else if( m_pDRHit != 0 && m_pDRHit->fittingWeightCorrected() )
	 {
	    return m_pDRHit->correctedFittingWeight() ;
	 }
	 else
	 {
	    return ( m_pDRHit != 0 && m_pDRHit->calibratedDRHit() != 0 ) ?
	       m_pDRHit->calibratedDRHit()->weight() : 0. ;
	 }
      }

      virtual Meters measuredDca() const
      { return ( m_pDRHit != 0 && m_pDRHit->calibratedDRHit() != 0 ) ?
	   m_pDRHit->signedDriftDistance() : 0. ; }

      virtual const CalibratedHit* calibratedHit() const
      { return ( m_pDRHit != 0 ) ? m_pDRHit->calibratedDRHit() : 0 ; }

      virtual HICalibratedHit* hiCalibratedHit() const
      { return m_pDRHit ; }

      virtual int numberOfAssociatedHits() const
      { return m_pDRHit->calibratedDRHit() == 0 ? 0 : 1 ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HISingleWireCylinder::kSurfaceType ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      void extractWireInfo() ;

      // ---------- protected const member functions -----------
      DABoolean m_checkSingleDirectionSwitch ;

   private:
      // ---------- Constructors and destructor ----------------
      HISingleWireCylinder( const HISingleWireCylinder& );
      // stop default

      // ---------- assignment operator(s) ---------------------
      const HISingleWireCylinder& operator=
      ( const HISingleWireCylinder& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      AEWireInfo m_wireInfo ;
      AEWireInfo::WireDirZ m_wirePosDir ;

      // Implementation of all member functions should use m_pDRHit,
      // NOT m_drHit.  This way, HIMultiWireCylinder can reassign m_pDRHit
      // instead of m_drHit, thus avoiding calling HICalibratedDRHit's
      // assignment operator repeatedly.
      HICalibratedDRHit* m_pDRHit ;
      HICalibratedDRHit  m_drHit ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HISingleWireCylinder.cc"
//#endif

#endif /* HELIXINTERSECTION_HISINGLEWIRECYLINDER_H */
