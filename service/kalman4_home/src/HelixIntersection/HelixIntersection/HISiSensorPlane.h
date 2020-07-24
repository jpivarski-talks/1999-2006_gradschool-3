#if !defined(HELIXINTERSECTION_HISISENSORPLANE_H)
#define HELIXINTERSECTION_HISISENSORPLANE_H
// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HISiSensorPlane
// 
// Description: HIIntersectionSurface subclass for defining planes with a
//              CalibratedSVHit.
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Oct 27 13:10:31 EST 1998
// $Id: HISiSensorPlane.h,v 1.15 2001/03/29 01:40:46 llh14 Exp $
//
// Revision history
//
// $Log: HISiSensorPlane.h,v $
// Revision 1.15  2001/03/29 01:40:46  llh14
// Separated Si Rphi and Z Hit Surfaces
//
// Revision 1.14  2001/03/27 05:30:05  wsun
// Added reverse hit order.
//
// Revision 1.13  2000/09/05 20:34:58  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.12  2000/04/05 00:41:49  wsun
// Added static function derivatives().
//
// Revision 1.11  2000/03/02 08:25:24  wsun
// Added addHICalibratedHitsToVector(...).
//
// Revision 1.10  2000/02/03 07:15:01  wsun
// Added use of HICalibratedHits.
//
// Revision 1.9  1999/12/15 20:47:19  wsun
// Updated to new HIIntersectionSurface interface and sped up derivatives().
//
// Revision 1.8  1999/11/21 22:26:52  wsun
// Got rid of string copy in surfaceType() to speed up code.
//
// Revision 1.7  1999/09/30 00:50:19  wsun
// Added reset() and revertToPreviousState().
//
// Revision 1.6  1999/08/03 21:54:13  wsun
// Mods to match base class changes.
//
// Revision 1.5  1999/06/24 22:18:24  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.4  1999/06/22 22:54:05  pg
// Fixes for New Si hits
//
// Revision 1.3  1999/04/28 22:57:03  wsun
// Fixes for Solaris.
//
// Revision 1.2  1999/04/27 22:04:08  wsun
// Major reorganization and structural changes.
//
// Revision 1.1.1.1  1998/12/07 04:58:19  wsun
// First submission.
//

// system include files

// user include files
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
#include "HelixIntersection/HICalibratedSVHit.h"

#include "HelixIntersection/HISiLadderPlane.h"

#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Geometry/Transform3D.h"

// forward declarations
class HICalibratedSVHit ;

class HISiSensorPlane : public HISiLadderPlane
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kSurfaceType ;

      // ---------- Constructors and destructor ----------------
      HISiSensorPlane();

      // IMPORTANT: the measurement directions are in the +z direction
      // for z sensors and the +phi (= -x) direction for r-phi sensors.
      HISiSensorPlane(
	 const DGConstMaterialPtr& aInnerMaterial,
	 const DGConstMaterialPtr& aOuterMaterial,
	 const ASiSensorInfo& aSensorInfo,
	 HICondition* = 0,
	 HIHelixOperation* = 0,
	 const CalibratedSVRphiHit* = 0,
	 const CalibratedSVZHit* = 0,
	 const HepPoint3D& aCorrectionInSi = (0.0, 0.0, 0.0)) ;
      virtual ~HISiSensorPlane();

      // ---------- member functions ---------------------------
      void addCalibratedSVHit( const CalibratedSVHit* aHit,
			       const Hep3Vector& aMeasurementDirection ) ;

      // Returns false if at the end of the list (or if only one hit).
      virtual DABoolean advanceToNextCalibratedHit( HIHelix& ) ;

      virtual void reset() ;
      virtual void resetHICalibratedHits() ;
      virtual void revertToPreviousState() { reset() ; }

      virtual void
      addHICalibratedHitsToVector( STL_VECTOR( HICalibratedHit* )& aVector ) ;

      virtual void setSVRFittingWeight( double aSVRFittingWeight )
      { setFittingWeight( aSVRFittingWeight ) ; }

      virtual void setSVZFittingWeight( double aSVZFittingWeight )
      { m_forceSVZFittingWeight=true ; m_svzFittingWeight=aSVZFittingWeight ; }

      // Set m_useReverseHitOrder and reset hit iterator to end of list.
      virtual void setUseReverseHitOrder( DABoolean aFlag ) ;

      // ---------- const member functions ---------------------
      virtual Meters dca( const HIHelix& ) const ;

      virtual void derivatives(
	 const HIHelix&, HepVector& outputVector ) const ;

      virtual Meters fittingWeight() const ;

      virtual Meters measuredDca() const
      { return 0. ; }

      virtual const CalibratedHit* calibratedHit() const
      { return m_currentHit->calibratedSVHit() ; }

      virtual HICalibratedHit* hiCalibratedHit() const
      { return &*m_currentHit ; }

      const Hep3Vector& measurementDirection() const
      { return m_currentHit->measurementDirection() ; }

      virtual int numberOfAssociatedHits() const
      { return m_hits.size() ; }

      // Use this function for casting base class pointers into the
      // appropriate subclass.
      virtual const string& surfaceType() const
      { return HISiSensorPlane::kSurfaceType ; }

      // ---------- static member functions --------------------

      // This function gives the derivatives of a measurement in the plane
      // of the sensor w.r.t. the track parameters.
      //
      // Inputs: aHelix = helix transported to the intersection with the plane.
      //         aMeasDir = measurement direction in the plane (
      //                    (must be unit vector).
      //         aNormalToPlane = unit vector normal to plane.
      //
      // Output: outputVector = 5x1 HepVector

      static void derivatives(
	 const HIHelix& aHelix,
	 const HepVector3D& aMeasDir,
	 const HepNormal3D& aNormalToPlane,
	 HepVector& outputVector ) ;

   protected:
      // ---------- protected member functions -----------------
      DABoolean m_forceSVZFittingWeight ;
      double m_svzFittingWeight ;

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HISiSensorPlane( const HISiSensorPlane& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HISiSensorPlane& operator=
      ( const HISiSensorPlane& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      STL_VECTOR( HICalibratedSVHit ) m_hits ;
      STL_VECTOR( HICalibratedSVHit )::iterator m_currentHit ;
      HepTransform3D m_transToSi ;
      HepPoint3D m_CorrectionInSi ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HISiSensorPlane.cc"
//#endif

#endif /* HELIXINTERSECTION_HISISENSORPLANE_H */
