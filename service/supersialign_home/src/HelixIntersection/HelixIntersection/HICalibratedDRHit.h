#if !defined(HELIXINTERSECTION_HICALIBRATEDDRHIT_H)
#define HELIXINTERSECTION_HICALIBRATEDDRHIT_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICalibratedDRHit
// 
/**\class HICalibratedDRHit HICalibratedDRHit.h HelixIntersection/HICalibratedDRHit.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Sun Jan 30 16:24:19 EST 2000
// $Id: HICalibratedDRHit.h,v 1.6 2002/02/17 17:23:07 wsun Exp $
//
// Revision history
//
// $Log: HICalibratedDRHit.h,v $
// Revision 1.6  2002/02/17 17:23:07  wsun
// Corrected previous revision log.
//
// Revision 1.5  2002/02/17 17:20:35  wsun
// Protect against calling applyTrackCorrections() twice.
//
// Revision 1.4  2000/12/02 01:30:33  wsun
// Added avg chg on track to HICalibratedDRHit and DRHitAndDriftDistance.
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

// user include files
#include "HelixIntersection/HICalibratedHit.h"
#include "CalibratedData/CalibratedDRHit.h"

#include "TrackFitter/TrackFitDRHitLink.h"

// forward declarations
class HitCorrector ;
#include "STLUtility/fwd_vector.h"

class HICalibratedDRHit : public HICalibratedHit
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICalibratedDRHit( const CalibratedDRHit* aHit = 0,
			 const HitCorrector* aHitCorrector = 0,
			 Meters aSignedDriftDistance = 0.,
			 const double& aAverageChargeOnTrack = 0.,
			 CalibratedTimeHit::PicoSec aDriftTime = 0.,
			 DABoolean aUseInFit = true ) ;

      HICalibratedDRHit( const HICalibratedDRHit& rhs ) ;

      const HICalibratedDRHit& operator=( const HICalibratedDRHit& rhs ) ;

      virtual ~HICalibratedDRHit() ;

      // ---------- member functions ---------------------------
      void setCalibratedDRHit( const CalibratedDRHit* aHit )
      { m_hit = aHit ; }
      void setSignedDriftDistance( Meters aDrift )
      { m_signedDriftDistance = aDrift ; }
      void setDriftTime( CalibratedTimeHit::PicoSec aTime )
      { m_driftTime = aTime ; }
      void setHitCorrector( const HitCorrector* aHitCorrector )
      { m_hitCorrector = aHitCorrector ; }
      void setAverageChargeOnTrack( const double& aCharge )
      { m_averageChargeOnTrack = aCharge ; }

      // clear() does not undo the track corrections.  It just clears
      // the residuals and smoothed quantities.
      virtual void clear() ;
      virtual DABoolean reverseMeasurementSign() ;

      // Apply signal propagation and time of flight corrections for DR anodes.
      virtual void applyTrackCorrections( const HIHelix& aHelix ) ;

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
      Meters signedDriftDistance() const { return m_signedDriftDistance ; }
      CalibratedTimeHit::PicoSec driftTime() const { return m_driftTime ; }
      DABoolean driftSignFlipped() const { return m_driftSignFlipped ; }

      const double& averageChargeOnTrack() const
      { return m_averageChargeOnTrack ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------
      const HitCorrector* m_hitCorrector ;

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
      STL_VECTOR( TrackFitDRHitLink )* m_vectorOfLinkData ;
      double                           m_averageChargeOnTrack ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICalibratedDRHit.cc"
//#endif

#endif /* HELIXINTERSECTION_HICALIBRATEDDRHIT_H */
