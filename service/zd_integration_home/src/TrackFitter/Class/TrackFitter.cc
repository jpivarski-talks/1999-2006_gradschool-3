// -*- C++ -*-
//
// Package:     <TrackFitter>
// Module:      TrackFitter
// 
// Description: See ../TrackFitter/TrackFitter.h for information
//
// Implementation:
//     See ../TrackFitter/TrackFitter.h for information
//
// Author:      Adam Lyon
// Created:     Fri Sep 25 14:24:00 EDT 1998
// $Id: TrackFitter.cc,v 1.16 2000/08/16 19:32:39 wsun Exp $
//
// Revision history
//
// $Log: TrackFitter.cc,v $
// Revision 1.16  2000/08/16 19:32:39  wsun
// Added usage and production tags for TRSeedTrack extraction.
//
// Revision 1.15  1999/10/08 23:08:42  wsun
// Added functions hypoFilled(), setHypoFilled(), and outwardFitIsInwardSeed().
//
// Revision 1.14  1999/07/23 13:43:57  marsh
// Uncommented report.h include.
//
// Revision 1.13  1999/07/22 13:07:53  marsh
// Protected against NULL seed track pointer.  Warn if no seeds.
//
// Revision 1.12  1999/06/16 02:42:25  wsun
// Additional member functions for user to provide lists of hits to fit.
//
// Revision 1.11  1999/06/02 20:19:21  marsh
// Test m_seedTracks before dereferencing.
//
// Revision 1.10  1999/05/28 22:03:38  wsun
// Added event(...) function that fits hypos one by one.
//
// Revision 1.9  1999/05/13 23:23:24  wsun
// Removed caching of seed tracks.
//
// Revision 1.8  1999/05/13 22:08:26  wsun
// Added event() that fits single hypotheses.
//
// Revision 1.7  1999/05/08 01:00:54  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.6  1999/02/11 22:44:05  marsh
// Integer "hypo" was being passed to fitTracks(), which was interpreted
// as a double rather than an enum, causing the wrong version of the
// function to be used.
//
// Revision 1.5  1999/01/26 22:43:52  marsh
// Overloaded fitTracks() to take a mass or a hypothesis.  event() uses
// the hypothesis version.
//
// Revision 1.4  1998/12/07 04:52:13  wsun
// New interface for fitting single tracks and hypotheses at a time.
//
// Revision 1.3  1998/09/29 19:26:26  lyon
// added #include <map> for Record.h
//
// Revision 1.2  1998/09/28 18:51:28  marsh
// Re-ordered includes and commented out part of a report statement.
//
// Revision 1.1.1.1  1998/09/28 15:56:12  lyon
// TrackFitter sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>    // this is needed for Record.h
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"

#include "TrackRoot/TRSeedTrack.h"
#include "TrackDelivery/TDKinematicFit.h"

#include "TrackFitter/TrackFitter.h"

#include "DAException/DANoDataException.h"

// STL classes
#include <vector>
#include <map>    // this is needed for Record.h

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackFitter" ;

//
// static data member definitions
//

//
// constructors and destructor
//
TrackFitter::TrackFitter()
   : m_seedTracks( 0 ),
     m_seedTrackUsageTag( UsageTag() ),
     m_seedTrackProdTag( ProductionTag() )
{
   for( int i = 0 ; i < DBCandidate::kMaxHypo ; ++i )
   {
      m_hypoFilled[ i ] = false ;
   }
}

// TrackFitter::TrackFitter( const TrackFitter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

TrackFitter::~TrackFitter()
{
   delete m_seedTracks ;
}

//
// assignment operators
//
// const TrackFitter& TrackFitter::operator=( const TrackFitter& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//////////////////////////////////////////////////////////////////////
// Do the main track fitting loop for this event
//   *** DO NOT OVERRIDE THIS FUNCTION UNLESS YOU 
//                       KNOW WHAT YOU ARE DOING! ***
//
//  This may not be const if the hypotheses are stored as member data
//
DABoolean TrackFitter::event( Frame& theFrame ) const
{
   // Cast away the const on *this.
   TrackFitter* thisNonConst = const_cast< TrackFitter* >( this ) ;

   // Get the record from the frame
   const Record& theRecord = theFrame.record( Stream::kEvent ) ;

   // Filter the Seed Tracks
   thisNonConst->filterSeeds( theRecord ) ;

   DABoolean success;

   // Do fitting if there are seed tracks
   if ( ( 0 != m_seedTracks ) && ( m_seedTracks->size() > 0 ) )
   {
      // Loop over seed tracks.
      for( STL_VECTOR( const TRSeedTrack* )::iterator seedIter =
	      m_seedTracks->begin() ;
	   seedIter != m_seedTracks->end() ;
	   ++seedIter )
      {
	 // If the outward fits are used as inward seeds, only loop over
	 // the inward hypotheses.
	 int maxHypo = outwardFitIsInwardSeed() ?
	    DBCandidate::kExitElectron : DBCandidate::kMaxHypo ;

	 // Loop over hypotheses.
	 for( int hypo = DBCandidate::kElectron ; hypo < maxHypo ; ++hypo )
	 {
	    if( !m_hypoFilled[ hypo ] )
	    {
	       FitDirection direction = kInwardFit ;

	       if( !DBCandidate::hypoIsInward( ( DBCandidate::Hypo ) hypo ) )
	       {
		  direction = kOutwardFit ;
	       }

	       success = thisNonConst->fitTrack( *seedIter,
						 theFrame,
						 DBCandidate::Hypo(hypo) ) ;

	       if ( !success )
	       {
		  // report( WARNING, kFacilityString )
		  // << "fitTracks failed :-( " << endl;
	       }
	    }
	 }
      }
   }
   else
   {
      //report( WARNING, kFacilityString ) << "No seed tracks to fit :-( " 
      //                                   << endl;

      success = true;   // Fitter didn't fail, just no tracks to fit
   }


   for( int i = 0 ; i < DBCandidate::kMaxHypo ; ++i )
   {
      thisNonConst->m_hypoFilled[ i ] = true ;
   }

   return success;
}


// This function is the same as the above one but without the loop over
// hypotheses.
DABoolean TrackFitter::event( Frame& theFrame,
			      HypoType aHypo ) const
{
   if( m_hypoFilled[ ( int ) aHypo ] )
   {
      return true ;
   }

   // Cast away the const on *this.
   TrackFitter* thisNonConst = const_cast< TrackFitter* >( this ) ;

   // Get the record from the frame
   const Record& theRecord = theFrame.record(Stream::kEvent);

   // Filter the Seed Tracks
   thisNonConst->filterSeeds( theRecord ) ;

   DABoolean success;

   // Do fitting if there are seed tracks
   if ( ( 0 != m_seedTracks ) && ( m_seedTracks->size() > 0 ) )
   {
      // Loop over seed tracks.
      for( STL_VECTOR( const TRSeedTrack* )::iterator seedIter =
	      m_seedTracks->begin() ;
	   seedIter != m_seedTracks->end() ;
	   ++seedIter )
      {
	 FitDirection direction = kInwardFit ;
	 // Commented out 1/26/99 by marsh -- use hypo instead
	 //GeV mass = kParticleMasses[ hypo ] ;

	 if( aHypo >= DBCandidate::kExitElectron )
	 {
	    direction = kOutwardFit ;
	 }

	 success = thisNonConst->fitTrack( *seedIter,
					   theFrame,
					   DBCandidate::Hypo( aHypo ) ) ;

	 if ( !success )
	 {
	    // report( WARNING, kFacilityString ) << "fitTracks failed :-( "
	    // << endl;
	 }
      }
   }
   else
   {
      //report( WARNING, kFacilityString ) << "No seed tracks to fit :-( " 
      //                                   << endl;

      success = true;   // Fitter didn't fail, just no tracks to fit
   }

   thisNonConst->m_hypoFilled[ ( int ) aHypo ] = true ;

   if( DBCandidate::hypoIsInward( ( DBCandidate::Hypo ) aHypo ) &&
       outwardFitIsInwardSeed() )
   {
      thisNonConst->m_hypoFilled[ ( int ) DBCandidate::outwardHypo(
	 ( DBCandidate::Hypo ) aHypo ) ] = true ;
   }

   return success;
}


//////////////////////////////////////////////////////////////////////
// Filter the seed tracks
//
//  Extract the seeds and put them in the seed track vector (no filtering)
//
//    Returns a NEW'D vector of seed tracks
//
void
TrackFitter::filterSeeds( const Record& theRecord )
{
   if( m_seedTracks != 0 && m_seedTracks->size() > 0 )
   {
      return ;
   }

   // Extract the seeds
   FATable<TRSeedTrack> allOfTheSeeds;

   try
   {
      extract( theRecord,
	       allOfTheSeeds,
	       m_seedTrackUsageTag.value(),
	       m_seedTrackProdTag.value() );
   }
   catch( NO_TABLE_EXCEPTION( TRSeedTrack )& aException )
   {
      report( WARNING, kFacilityString )
	 << "Could not extract TRSeedTracks with usage tag "
	 << m_seedTrackUsageTag.value() << " and production tag "
	 << m_seedTrackProdTag.value()
	 << ".  Will extract default TRSeedTracks." << endl ;

      extract( theRecord, allOfTheSeeds ) ;
   }
   

   if ( !allOfTheSeeds.valid() ) 
   {
      report(WARNING, kFacilityString) << "No Seed Tracks" << endl;
      return ;
   }

   // Make the vector of seed track on which to run fitting
   if( m_seedTracks == 0 )
   {
      m_seedTracks = new STL_VECTOR( const TRSeedTrack* ) ;
   }

   // Loop over seeds and put them in the vector
   for ( FATable<TRSeedTrack>::const_iterator iter = allOfTheSeeds.begin();
	 iter != allOfTheSeeds.end(); ++iter )
   {
      m_seedTracks->push_back(&(*iter));
   }
}
       

void
TrackFitter::clearCache()
{
//    if( m_seedTracks != 0 )
//    {
//       delete m_seedTracks ;
//       m_seedTracks = 0 ;
//    }

   if( m_seedTracks != 0 )
   {
      m_seedTracks->clear() ;
   }

   for( int i = 0 ; i < DBCandidate::kMaxHypo ; ++i )
   {
      m_hypoFilled[ i ] = false ;
   }
}


//
// const member functions
//

//
// static member functions
//
