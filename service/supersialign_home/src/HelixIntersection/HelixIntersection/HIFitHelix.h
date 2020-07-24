#if !defined(HELIXINTERSECTION_HIFITHELIX_H)
#define HELIXINTERSECTION_HIFITHELIX_H
// -*- C++ -*-
//
// Package:     <HelixIntersection>
// Module:      HIFitHelix
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Fri Jul  2 00:31:38 EDT 1999
// $Id: HIFitHelix.h,v 1.15 2001/10/09 17:56:47 llh14 Exp $
//
// Revision history
//
// $Log: HIFitHelix.h,v $
// Revision 1.15  2001/10/09 17:56:47  llh14
// HIFitHelix.h: set m_ownsLinkData to false in clear()
//
// Revision 1.14  2000/10/08 23:39:33  wsun
// Added smoothed dca and error to HitAndLinkData.
//
// Revision 1.13  2000/08/16 20:01:01  wsun
// Updated to new TrackFitHitLink and TRTrackFitQuality.
//
// Revision 1.12  2000/06/09 23:55:51  wsun
// HIFitHelix::HitAndLinkData does not own TrackFitHitLink by default.
// Ownership must be transferred explicitly.  This avoids new-ing the
// TrackFitHitLinks every time the helix is copied during fitting.  Saves
// a lot of time.
//
// Revision 1.11  2000/06/09 00:30:31  wsun
// Changed m_linkData in HitAndLinkData to a pointer.
//
// Revision 1.10  2000/03/02 08:23:26  wsun
// HitAndLinkData::setHICalibratedHit() also sets m_calibratedHit.
//
// Revision 1.9  2000/02/22 00:20:24  wsun
// Set link data resids and error to smoothed quantities.
//
// Revision 1.8  2000/02/03 07:17:50  wsun
// Added use of HICalibratedHits in HitAndLinkData.
//
// Revision 1.7  1999/11/21 22:25:01  wsun
// Got rid of string copy in helixType() to speed up code.
//
// Revision 1.6  1999/08/30 21:36:41  wsun
// Added numberOfHitsInFit(), which ndof() now uses.
//
// Revision 1.5  1999/08/27 20:20:10  wsun
// Cosmetic changes.
//
// Revision 1.4  1999/07/07 16:40:12  wsun
// Added mechanism for applying constraints in track fitters.
//

// system include files

// user include files
#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIFitConstraint.h"
#include "HelixIntersection/HICalibratedHit.h"
#include "TrackFitter/TrackFitDRHitLink.h"
//#include "TrackFitter/TrackFitHitLink.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class HICalibratedHit ;

class HIFitHelix : public HIHelix
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      static const string kHelixType ;

      // The data members of TrackFitDRHitLink are a superset of those
      // of TrackFitSVRHitLink and TrackFitSVZHitLink.  So, for now, at
      // least, we can use a single class for all types of hits.
      class HitAndLinkData
      {
	 public:
	    HitAndLinkData()
	       : m_hit( ( HICalibratedHit* ) 0 ),
		 m_calibratedHit(         ( const CalibratedHit* ) 0 ),
		 m_smoothedResidual(      0. ),
		 m_smoothedResidualError( 0. ),
		 m_smoothedDistPred(      0. ),
		 m_smoothedDistPredError( 0. ),
		 m_linkData(              0 ),
		 m_ownsLinkData(          false ) {}

	    HitAndLinkData( HICalibratedHit* aHit,
			    TrackFitHitLink* aLinkData = 0 )
	       : m_hit(                   aHit ),
		 m_linkData(              aLinkData ),
		 m_smoothedResidual(      0. ),
		 m_smoothedResidualError( 0. ),
		 m_smoothedDistPred(      0. ),
		 m_smoothedDistPredError( 0. ),
		 m_ownsLinkData(          false )
	    {
	       if( aHit != 0 )
	       {
		  m_calibratedHit = aHit->calibratedHit() ;
	       }
	    }

	    HitAndLinkData( const HitAndLinkData& rhs )
	       : m_hit(                   rhs.m_hit ),
		 m_calibratedHit(         rhs.m_calibratedHit ),
		 m_smoothedResidual(      rhs.m_smoothedResidual ),
		 m_smoothedResidualError( rhs.m_smoothedResidualError ),
		 m_smoothedDistPred(      rhs.m_smoothedDistPred ),
		 m_smoothedDistPredError( rhs.m_smoothedDistPredError ),
		 m_linkData(              rhs.m_linkData ),
		 m_ownsLinkData(          rhs.m_ownsLinkData )
	    {
	       // makeNewTrackFitLinkData() does not check the type of
	       // the input link data, but since this is copied from a
	       // preexisting HitAndLinkData, everything is consistent.

	       if( m_ownsLinkData )
	       {
		  takeOwnershipOfLinkData() ;
	       }
	    }

	    const HitAndLinkData& operator=( const HitAndLinkData& rhs )
	    {
	       if( this != &rhs )
	       {
		  m_hit                   = rhs.m_hit ;
		  m_calibratedHit         = rhs.m_calibratedHit ;
		  m_smoothedResidual      = rhs.m_smoothedResidual ;
		  m_smoothedResidualError = rhs.m_smoothedResidualError ;
		  m_smoothedDistPred      = rhs.m_smoothedDistPred ;
		  m_smoothedDistPredError = rhs.m_smoothedDistPredError ;
		  m_linkData              = rhs.m_linkData ;
		  m_ownsLinkData          = rhs.m_ownsLinkData ;

		  if( m_ownsLinkData )
		  {
		     takeOwnershipOfLinkData() ;
		  }
	       }
	       return *this;
	    }

	    virtual ~HitAndLinkData()
	    {
	       if( m_ownsLinkData )
	       {
		  delete m_linkData ;
	       }
	    }

	    void clear()
	    {
	       m_hit                   = ( HICalibratedHit* ) 0 ;
	       m_calibratedHit         = ( const CalibratedHit* ) 0 ;
	       m_smoothedResidual      = 0. ;
	       m_smoothedResidualError = 0. ;
	       m_smoothedDistPred      = 0. ;
	       m_smoothedDistPredError = 0. ;

	       if( m_ownsLinkData )
	       {
		  delete m_linkData ;
		  m_ownsLinkData = false;
	       }

	       m_linkData = 0 ;
	    }

	    void removeFromTrack()
	    {
	       m_hit->setUseInFit( false ) ;

	       if( m_linkData != 0 )
	       {
		  m_linkData->setUsed( false ) ;
	       }
	    }

	    void setHICalibratedHit( HICalibratedHit* aHit )
	    {
	       m_hit = aHit ;

	       if( aHit != 0 )
	       {
		  m_calibratedHit = aHit->calibratedHit() ;
	       }
	    }

	    // This function does not take ownership of the link data.
	    void setLinkData( TrackFitHitLink* aLinkData )
	    { m_linkData = aLinkData ; }

	    // This function takes ownership of the link data.
	    void takeOwnershipOfLinkData()
	    {
	       m_linkData = m_hit->makeNewCopyOfLinkData( m_linkData ) ;
	       m_ownsLinkData = true ;
	    }

	    // Take weighted average of residuals stored in the
	    // HICalibratedHit and transfer the smoothed residual and error
	    // to the link data.
	    DABoolean calculateSmoothedResidualAndError()
	    {
	       if( !m_hit->residualsSmoothed() )
	       {
		  m_hit->calculateSmoothedResidualAndError() ;
	       }

	       if( m_hit->residualsSmoothed() )
	       {
		  m_smoothedResidual = m_hit->smoothedResidual() ;
		  m_smoothedResidualError = m_hit->smoothedResidualError() ;
		  m_linkData->setResidual( m_smoothedResidual ) ;
		  m_linkData->setResidualError( m_smoothedResidualError ) ;

		  if( m_hit->calibratedHit()->deviceType() ==
		      CalibratedHit::DR )
		  {
		     m_smoothedDistPred = m_hit->smoothedDistPred() ;
		     m_smoothedDistPredError = m_hit->smoothedDistPredError();

		     TrackFitDRHitLink* drLinkData =
			( TrackFitDRHitLink* ) m_linkData ;
		     drLinkData->setSignedDcaToWire( m_smoothedDistPred ) ;
		     drLinkData->setSignedDcaError( m_smoothedDistPredError ) ;
		  }
	       }

	       return m_hit->residualsSmoothed() ;
	    }

	    HICalibratedHit* hiCalibratedHit() const
	    { return m_hit ; }

	    const TrackFitHitLink* linkData() const
	    { return m_linkData ; }

	    const CalibratedHit* calibratedHit() const
	    { return ( m_calibratedHit == 0 ) ?
		 m_hit->calibratedHit() : m_calibratedHit ; }

	    Meters smoothedResidual() const
	    { return m_smoothedResidual ; }

	    Meters smoothedResidualError() const
	    { return m_smoothedResidualError ; }

	    Meters smoothedDistPred() const
	    { return m_smoothedDistPred ; }

	    Meters smoothedDistPredError() const
	    { return m_smoothedDistPredError ; }

	 private:
	    HICalibratedHit* m_hit ;
	    TrackFitHitLink* m_linkData ;

	    // This is used to save the CalibratedHit after the HICalibratedHit
	    // is deleted by the surface to which it is attached.
	    const CalibratedHit* m_calibratedHit ;

	    // Same for these quantities.
	    Meters m_smoothedResidual ;
	    Meters m_smoothedResidualError ;
	    Meters m_smoothedDistPred ;
	    Meters m_smoothedDistPredError ;

	    // By default, the HitAndLinkData does not own the TrackFitHitLink.
	    DABoolean m_ownsLinkData ;
      } ;

      // ---------- Constructors and destructor ----------------
      HIFitHelix();
      HIFitHelix( const KTHelix& aHelix,
		  GeV aMass = 0.13957,  // pion mass
		  DABoolean reportFitResultsAtInitRefPt = false,
		  Meters anArcLength = 0.,
		  Identifier id = 0 ) ;
      virtual ~HIFitHelix();

      HIFitHelix( const HIFitHelix& );

      const HIFitHelix& operator=( const HIFitHelix& );

      // ---------- member functions ---------------------------

      // *********** IMPORTANT ************
      // Subclasses need to reimplement these two functions so that the
      // snapshot is of the right type.
      virtual HIHelix* newSnapshot() { return new HIFitHelix( *this ) ; }
      virtual void revertToSnapshot( const HIHelix* aSnapshot )
      {
         if( aSnapshot->helixType() == helixType() )
            *this = *( ( HIFitHelix* )aSnapshot ) ;
      }

      void setNumberOfFreeParameters( int aNFreeParams )
      { m_numberFreeParams = aNFreeParams ; }
      void incrementNumberOfFreeParameters( int aIncr = 1 )
      { m_numberFreeParams += aIncr ; }
      void setNumberOfConstraints( int aNConstraints )
      { m_numberConstraints = aNConstraints ; }

      void addHitOnTrack( const HitAndLinkData& aHit ) ;
      void setHitsOnTrack( const STL_VECTOR( HitAndLinkData )& aHits ) ;

      virtual void resetHitsOnTrack() ;
      virtual void reset() ;

      void incrementChisq( double deltaChisq ) { m_chisq += deltaChisq ; }
      void setChisq( double aChisq ) { m_chisq = aChisq ; }

      void applyConstraintResults( const HIFitConstraint::Results& aResults ) ;

      void setFitFailed( DABoolean aFlag ) { m_fitFailed = aFlag ; }

      // aNumberOfHitsDropped is the number of hits discarded by the fit.
      //
      // aNumerOfHitsFlipped is the number of hits whose measurement sign
      // was flipped by the fit.  This only really applies to DR anode hits.

      void setNumberHitsDropped( int aNumberOfHitsDropped )
      { m_nHitsDropped = aNumberOfHitsDropped ; }

      void setNumberHitsFlipped( int aNumberOfHitsFlipped )
      { m_nHitsFlipped = aNumberOfHitsFlipped ; }

      // ---------- const member functions ---------------------

      // *********** IMPORTANT ************
      // Subclasses should reimplement helixType(), which is used to
      // test for the helix' actual type when casting base class pointers
      // into the appropriate subclass.
      virtual const string& helixType() const
      { return HIFitHelix::kHelixType ; }

      int numberOfFreeParameters() const { return m_numberFreeParams ; }
      int numberOfConstraints() const { return m_numberConstraints ; }

      STL_VECTOR( HitAndLinkData )& hitsOnTrack() const
      { return *m_hitsOnTrack ; }

      double chisq() const { return m_chisq ; }

      virtual int numberOfHitsInFit() const ;
      virtual int ndof() const ;

      DABoolean fitFailed() const { return m_fitFailed ; }

      // Unknown parameters in constraint, which is applied after the
      // unconstrained fit.
      const HepVector& unknownParameters() const { return m_unknownParams ; }

      const HepSymMatrix& unknownParameterErrorMatrix() const
      { return m_unknownError ; }

      // covariance( unknown constraint parameters, constrained track params )
      const HepMatrix& unknownTrackCovariance() const
      { return m_covarUnknownHelix ; }

      int numberHitsDropped() { return m_nHitsDropped ; }
      int numberHitsFlipped() { return m_nHitsFlipped ; }

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
      DABoolean m_fitFailed ;

      double m_chisq ;
      STL_VECTOR( HitAndLinkData )* m_hitsOnTrack ;

      int m_numberFreeParams ;
      int m_numberConstraints ;

      // From applying constraints.
      HepVector    m_unknownParams ;
      HepSymMatrix m_unknownError ;
      HepMatrix    m_covarUnknownHelix ;

      int m_nHitsDropped ;
      int m_nHitsFlipped ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HelixIntersection/Template/HIFitHelix.cc"
//#endif

#endif /* HELIXINTERSECTION_HIFITHELIX_H */
