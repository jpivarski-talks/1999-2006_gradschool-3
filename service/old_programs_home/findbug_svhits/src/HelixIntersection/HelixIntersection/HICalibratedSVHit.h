#if !defined(HELIXINTERSECTION_HICALIBRATEDSVHIT_H)
#define HELIXINTERSECTION_HICALIBRATEDSVHIT_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICalibratedSVHit
// 
/**\class HICalibratedSVHit HICalibratedSVHit.h HelixIntersection/HICalibratedSVHit.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Sun Jan 30 16:24:24 EST 2000
// $Id: HICalibratedSVHit.h,v 1.3 2000/06/09 23:53:17 wsun Exp $
//
// Revision history
//
// $Log: HICalibratedSVHit.h,v $
// Revision 1.3  2000/06/09 23:53:17  wsun
// * Added applyTrackCorrections().
// * TrackFitHitLinks stored temporarily by HICalibratedHits to avoid new-ing
//   them every time the track is copied during fitting.  The TrackFitHitLinks
//   are copied to the helix after fitting is done.  Saves A LOT of time.
// * Renamed makeNewLinkData() -->
//   instantiateLinkData() and makeNewCopyOfLinkData().
//
// Revision 1.2  2000/06/09 00:29:34  wsun
// Added makeNewLinkData().
//
// Revision 1.1  2000/02/03 07:12:38  wsun
// First submission.
//

// system include files

// user include files
#include "HelixIntersection/HICalibratedHit.h"
#include "SiHits/CalibratedSVHit.h"

#include "TrackFitter/TrackFitSVRHitLink.h"
#include "TrackFitter/TrackFitSVZHitLink.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HICalibratedSVHit : public HICalibratedHit
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICalibratedSVHit(
	 const CalibratedSVHit* aHit = 0,
	 const HepVector3D& aMeasurementDirection = HepVector3D( 0., 0., 0. ),
	 DABoolean aUseInFit = true ) ;

      HICalibratedSVHit( const HICalibratedSVHit& rhs ) ;

      const HICalibratedSVHit& operator=( const HICalibratedSVHit& rhs ) ;

      virtual ~HICalibratedSVHit() ;

      // ---------- member functions ---------------------------
      void setCalibratedSVHit( const CalibratedSVHit* aHit )
      { m_hit = aHit ; }
      void setMeasurementDirection( const HepVector3D& aDir )
      { m_measurementDirection = aDir ; }

      virtual void clear() ;

      // ---------- const member functions ---------------------
      virtual double measurementErrorSquared() const
      { return ( m_hit != 0 ) ? sqr( m_hit->coord_error() ) : 0. ; }

      virtual const CalibratedHit* calibratedHit() const
      { return m_hit ; }

      // This function instantiates a TrackFitHitLink and adds it to an
      // internal vector.  This link data does not need to be deleted.
      virtual TrackFitHitLink* instantiateLinkData(
	 const HIFitHelix& aHelix,
	 const HIIntersectionSurface& aSurface,
	 Meters aResidual,
	 Meters aResidualError,
	 Meters aDistPred,
	 Meters aDistPredError ) ;

      // WARNING: this function does not check the type of the input
      // TrackFitHitLink before casting it to a subclass.  This link data
      // is owned by the caller and needs to be deleted.
      virtual TrackFitHitLink* makeNewCopyOfLinkData(
	 const TrackFitHitLink* aLinkData ) ;

      const CalibratedSVHit* calibratedSVHit() const { return m_hit ; }
      const HepVector3D& measurementDirection() const
      { return m_measurementDirection ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const CalibratedSVHit* m_hit ;
      HepVector3D m_measurementDirection ;
      STL_VECTOR( TrackFitSVRHitLink )* m_vectorOfSVRLinkData ;
      STL_VECTOR( TrackFitSVZHitLink )* m_vectorOfSVZLinkData ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICalibratedSVHit.cc"
//#endif

#endif /* HELIXINTERSECTION_HICALIBRATEDSVHIT_H */
