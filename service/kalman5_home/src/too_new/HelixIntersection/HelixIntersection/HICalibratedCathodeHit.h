#if !defined(HELIXINTERSECTION_HICALIBRATEDCATHODEHIT_H)
#define HELIXINTERSECTION_HICALIBRATEDCATHODEHIT_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HICalibratedCathodeHit
// 
/**\class HICalibratedCathodeHit HICalibratedCathodeHit.h HelixIntersection/HICalibratedCathodeHit.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Werner Sun
// Created:     Sun Jan 30 16:24:29 EST 2000
// $Id: HICalibratedCathodeHit.h,v 1.4 2002/04/27 17:22:29 cleo3 Exp $
//
// Revision history
//
// $Log: HICalibratedCathodeHit.h,v $
// Revision 1.4  2002/04/27 17:22:29  cleo3
// Compiler now writes the copy constructor, destructor and assigment operator.  Replaced pointer to vector with a deque.  Need a deque since, unlike a vector, pointers to data in a deque are guaranteed to be valid even if deque is resized.  Resizing vectors caused memory problems on Solaris 8.
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
// Revision 1.1  2000/02/03 07:12:39  wsun
// First submission.
//

// system include files
#include <deque>

// user include files
#include "HelixIntersection/HICalibratedHit.h"
#include "CalibratedData/CalibratedCathodeHit.h"

#include "TrackFitter/TrackFitCathodeHitLink.h"

// forward declarations
#include "STLUtility/fwd_deque.h"

class HICalibratedCathodeHit : public HICalibratedHit
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HICalibratedCathodeHit( const CalibratedCathodeHit* aHit = 0,
			      DABoolean aUseInFit = true ) ;

      //HICalibratedCathodeHit( const HICalibratedCathodeHit& rhs ) ;

      //const HICalibratedCathodeHit& operator=(
//	 const HICalibratedCathodeHit& rhs ) ;

      //virtual ~HICalibratedCathodeHit() ;

      // ---------- member functions ---------------------------
      void setCalibratedCathodeHit( const CalibratedCathodeHit* aHit )
      { m_hit = aHit ; }

      virtual void clear() ;

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

      const CalibratedCathodeHit* calibratedCathodeHit() const
      { return m_hit ; }

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
      const CalibratedCathodeHit* m_hit ;
      STL_DEQUE( TrackFitCathodeHitLink ) m_vectorOfLinkData ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HICalibratedCathodeHit.cc"
//#endif

#endif /* HELIXINTERSECTION_HICALIBRATEDCATHODEHIT_H */
