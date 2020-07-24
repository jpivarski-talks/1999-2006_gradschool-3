#if !defined(TRACKFITTER_TRACKFITTER_H)
#define TRACKFITTER_TRACKFITTER_H
// -*- C++ -*-
//
// Package:     <TrackFitter>
// Module:      TrackFitter
// 
// Description: Base class for all Track Fitters
//
// Usage:
//    This is a base class for all track fitters, defining the interface.
//    Your track fitter must inherit from this and must implement 
//    these pure virtual member functions...
//
//       fitTracks = Does the actual fitting of hits on seed tracks
//       hypothesis = Returns the specified hypothesis
//       hitLattice = Returns the specified hit lattice
//    
//    A few notes:
//
//    * Added function event(...) to fit hypotheses one by one.  TrackFitter
//      keeps track of which hypotheses have already been filled for this event
//      and will do nothing if the requested fits have been performed.
//    * Cache the vector of seed tracks for each event.  If one instantiation
//      of TrackFitter is used for multiple events, the user should clear the
//      cache by calling clearCache() at the end of each event.
//
// Author:      Adam Lyon
// Created:     Fri Sep 25 13:47:55 EDT 1998
// $Id: TrackFitter.h,v 1.10 2000/08/16 19:32:37 wsun Exp $
//
// Revision history
//
// $Log: TrackFitter.h,v $
// Revision 1.10  2000/08/16 19:32:37  wsun
// Added usage and production tags for TRSeedTrack extraction.
//
// Revision 1.9  1999/10/08 23:08:44  wsun
// Added functions hypoFilled(), setHypoFilled(), and outwardFitIsInwardSeed().
//
// Revision 1.8  1999/06/16 02:40:41  wsun
// Additional member functions for user to provide lists of hits to fit.
//
// Revision 1.7  1999/05/28 22:21:19  wsun
// Added access function for seed tracks.
//
// Revision 1.6  1999/05/28 22:03:32  wsun
// Added event(...) function that fits hypos one by one.
//
// Revision 1.5  1999/05/13 23:23:18  wsun
// Removed caching of seed tracks.
//
// Revision 1.4  1999/05/13 22:08:17  wsun
// Added event() that fits single hypotheses.
//
// Revision 1.3  1999/01/26 22:44:11  marsh
// Overloaded fitTracks() to take a mass or a hypothesis.  event() uses
// the hypothesis version.
//
// Revision 1.2  1998/12/07 04:51:52  wsun
// New interface for fitting single tracks and hypotheses at a time.
//
// Revision 1.1.1.1  1998/09/28 15:56:11  lyon
// TrackFitter sources
//

// system include files
#include "Experiment/Experiment.h"

// user include files
#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "CleoDB/DBCandidate.h"

#include "DataHandler/DataKeyTags.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
class TDKinematicFit;
class TRSeedTrack;
class KTHelix ;

class TrackFitter
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef double GeV ;

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
      typedef int HypoType ;
#else
      typedef DBCandidate::Hypo HypoType ;
#endif

      enum FitDirection { kOutwardFit,
			  kInwardFit } ;

      // ---------- Constructors and destructor ----------------
      TrackFitter();
      virtual ~TrackFitter();

      // ---------- member functions ---------------------------
      

      //////////////////////////////////////////////////////////////////////
      // Do the main track fitting loop for this event
      //   *** DO NOT OVERRIDE THIS FUNCTION UNLESS YOU 
      //                       KNOW WHAT YOU ARE DOING! ***
      //
      //  This may not be const if the hypotheses are stored as member data
      //

      // Use a const_cast in this function to allow modification of member
      // data.  This allows the user to call this function on a TrackFinder
      // extracted from the Frame.  Normally, this would be handled by
      // declaring the TrackFitter "mutable".
      DABoolean event( Frame& theFrame ) const ;

      // Fit the tracks under the given hypothesis.  If the fits for the
      // given hypothesis have already been performed for this event (i.e.
      // for this instantiation of the TrackFinder), then nothing happens.
      // This function allows the proxy to fit only the requested hypotheses,
      // not all possible hypotheses, which could be as many as 10.
      DABoolean event( Frame& theFrame,
		       HypoType aHypo ) const ;


      //////////////////////////////////////////////////////////////////////
      // Filter the seed tracks, perhaps throwing away garbage
      //
      //  You can override this if you don't want the default actions
      //    (which is to accept all seed tracks)
      //
      virtual void filterSeeds( const Record& theRecord ) ;

      virtual void clearCache() ;


      //////////////////////////////////////////////////////////////////////
      // Fit the tracks (PURE VIRTUAL)
      //

      // This function is called by event, and should probably call
      // fittedTrack().
      virtual DABoolean fitTrack(
         const TRSeedTrack* theSeed,
         Frame& theFrame,
         DBCandidate::Hypo aHypo = DBCandidate::kChargedPion ) = 0 ;

      // These two functions return a new KTHelix which the user (or the
      // Frame) is responsible for deleting.
      virtual KTHelix* fittedTrack(
	 const TRSeedTrack* theSeed,
	 Frame& theFrame,
         DBCandidate::Hypo aHypo = DBCandidate::kChargedPion ) = 0 ;

      virtual KTHelix* fittedTrack(
	 const TRSeedTrack* theSeed,
	 Frame& theFrame,
	 GeV aMass = 0.13957,   // pion mass
	 FitDirection aFitDirection = kInwardFit ) = 0 ;

      void setSeedTrackUsageTag( const string& aTag )
      { m_seedTrackUsageTag = UsageTag( aTag.c_str() ) ; }
      void setSeedTrackProductionTag( const string& aTag )
      { m_seedTrackProdTag = ProductionTag( aTag.c_str() ) ; }


      //////////////////////////////////////////////////////////////////////
      // Return a hypothesis
      //
      virtual TDKinematicFit* hypothesis( DBCandidate::Hypo hypo ) const = 0;


      //////////////////////////////////////////////////////////////////////
      // Return a lattice
      // Uh - let's do this later
				  

      // ---------- const member functions ---------------------
      DABoolean hypoFilled( DBCandidate::Hypo hypo ) const
      { return m_hypoFilled[ hypo ] ; }

      virtual STL_VECTOR(const TRSeedTrack*)* seedTracks() const
      { return m_seedTracks ; }

      virtual DABoolean outwardFitIsInwardSeed() const { return false ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------
      void setHypoFilled( DBCandidate::Hypo hypo )
      { m_hypoFilled[ hypo ] = true ; }

   private:
      // ---------- Constructors and destructor ----------------
      TrackFitter( const TrackFitter& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TrackFitter& operator=( const TrackFitter& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      STL_VECTOR( const TRSeedTrack* )* m_seedTracks ;
      DABoolean m_hypoFilled[ DBCandidate::kMaxHypo ] ;

      UsageTag m_seedTrackUsageTag ;
      ProductionTag m_seedTrackProdTag ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "TrackFitter/Template/TrackFitter.cc"
//#endif

#endif /* TRACKFITTER_TRACKFITTER_H */
