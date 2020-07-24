#if !defined(HELIXINTERSECTION_HICALIBRATEDWIREHIT_H)
#define HELIXINTERSECTION_HICALIBRATEDWIREHIT_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICalibratedWireHit
// 
/**\class HICalibratedWireHit HICalibratedWireHit.h HelixIntersection/HICalibratedWireHit.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Sun Jan 30 16:24:19 EST 2000
// $Id: HICalibratedWireHit.h,v 1.1 2003/09/01 19:44:54 wsun Exp $
//
// Revision history
//
// $Log: HICalibratedWireHit.h,v $
// Revision 1.1  2003/09/01 19:44:54  wsun
// Renamed HICalibratedDRHit --> HICalibratedWireHit.
//
// Revision 1.8  2003/08/30 00:24:45  wsun
// * Use WHCBaseHitCorrector instead of HitCorrector (DR) and ZDHitCorrector.
// * Allow hit corrections to be applied in reverse in HICalibratedHit (for MC).
//
// Revision 1.7  2002/04/27 17:22:29  cleo3
// Compiler now writes the copy constructor, destructor and assigment operator.  Replaced pointer to vector with a deque.  Need a deque since, unlike a vector, pointers to data in a deque are guaranteed to be valid even if deque is resized.  Resizing vectors caused memory problems on Solaris 8.
//
// Revision 1.6  2002/02/17 17:23:07  wsun
// Corrected previous revision log.
//
// Revision 1.5  2002/02/17 17:20:35  wsun
// Protect against calling applyTrackCorrections() twice.
//
// Revision 1.4  2000/12/02 01:30:33  wsun
// Added avg chg on track to HICalibratedWireHit and DRHitAndDriftDistance.
//
// Revision 1.3  2000/06/09 23:53:16  wsun
// * Added applyTrackCorrections().
// * TrackFitHitLinks stored temporarily by HICalibratedHits to avoid new-ing
//   them every time the track is copied during fitting.  The TrackFitHitLinks
//   are copied to the helix after fitting is done.  Saves A LOT of time.
// * Renamed makeNewLinkData() -->
//   instantiateLinkData() and makeNewCopyOfLinkData().
//
// Revision 1.2  2000/06/09 00:29:33  wsun
// Added makeNewLinkData().
//
// Revision 1.1  2000/02/03 07:12:38  wsun
// First submission.
//

// system include files
#include <deque>

// user include files
#include "HelixIntersection/HICalibratedHit.h"
#include "CalibratedData/CalibratedDRHit.h"

#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations
class WHCBaseHitCorrector ;
#include "STLUtility/fwd_deque.h"

class HICalibratedWireHit : public HICalibratedHit
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICalibratedWireHit( const CalibratedDRHit* aHit = 0,
			   const WHCBaseHitCorrector* aHitCorrector = 0,
			   Meters aSignedDriftDistance = 0.,
			   const double& aAverageChargeOnTrack = 0.,
			   CalibratedTimeHit::PicoSec aDriftTime = 0.,
			   DABoolean aUseInFit = true ) ;

      //HICalibratedWireHit( const HICalibratedWireHit& rhs ) ;

      //const HICalibratedWireHit& operator=( const HICalibratedWireHit& rhs ) ;

      //virtual ~HICalibratedWireHit() ;

      // ---------- member functions ---------------------------
      void setCalibratedDRHit( const CalibratedDRHit* aHit )
      { m_hit = aHit ; }
      void setSignedDriftDistance( Meters aDrift )
      { m_signedDriftDistance = aDrift ; }
      void setDriftTime( CalibratedTimeHit::PicoSec aTime )
      { m_driftTime = aTime ; }
      void setHitCorrector( const WHCBaseHitCorrector* aHitCorrector )
      { m_hitCorrector = aHitCorrector ; }
      void setAverageChargeOnTrack( const double& aCharge )
      { m_averageChargeOnTrack = aCharge ; }

      // clear() does not undo the track corrections.  It just clears
      // the residuals and smoothed quantities.
      virtual void clear() ;
      virtual DABoolean reverseMeasurementSign() ;

      // Apply signal propagation and time of flight corrections for DR anodes.
      virtual void applyTrackCorrections(
	 const HIHelix& aHelix,
	 DABoolean aReverseCorrection = false ) ;

      // ---------- const member functions ---------------------
      virtual double measurementErrorSquared() const
      { return ( m_hit != 0 ) ? 1. / m_hit->weight() : 0. ; }

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

      const CalibratedDRHit* calibratedDRHit() const { return m_hit ; }

      DABoolean driftSignFlipped() const { return m_driftSignFlipped ; }

      const Meters& signedDriftDistance() const
      { return m_signedDriftDistance ; }

      const CalibratedTimeHit::PicoSec& driftTime() const
      { return m_driftTime ; }

      const Meters& uncorrectedSignedDriftDistance() const
      { return m_uncorrectedSignedDriftDistance ; }

      const double& averageChargeOnTrack() const
      { return m_averageChargeOnTrack ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------
//       const HitCorrector* m_hitCorrector ;
      const WHCBaseHitCorrector* m_hitCorrector ;
      STL_DEQUE( TrackFitDRHitLink )  m_vectorOfLinkData ;

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const CalibratedDRHit*           m_hit ;
      CalibratedTimeHit::PicoSec       m_driftTime ;
      Meters                           m_signedDriftDistance ;
      Meters                           m_uncorrectedSignedDriftDistance ;
      DABoolean                        m_driftSignFlipped ;
      //use a deque since it is guaranteed to keep its pointers 
      // accurate even when doing insertions on the end
      double                           m_averageChargeOnTrack ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICalibratedWireHit.cc"
//#endif

#endif /* HELIXINTERSECTION_HICALIBRATEDWIREHIT_H */
