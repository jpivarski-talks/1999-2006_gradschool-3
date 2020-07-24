// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackMaster
// 
// Description: Master block for NavTrack object, inherits from TDTrackMaster.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Brian Heltsley
// Created:     Thu Nov 07 17:42:04 EDT 2002
// $Id: NavTrackMaster_mcTag.cc,v 1.5 2003/02/18 16:46:37 tom Exp $
//
// Revision history
//
// $Log: NavTrackMaster_mcTag.cc,v $
// Revision 1.5  2003/02/18 16:46:37  tom
// Fix typo in array limits check at end of mcTag() fn
//
// Revision 1.4  2003/02/03 20:16:19  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.2  2002/11/21 16:52:41  bkh
// Add new NavShower fcn giving back track matches in same con reg
// Require NavTrack mcTag to have same charge as track
//
// Revision 1.1  2002/11/07 17:42:04  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <set>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"

#include "Navigation/NavTrack.h"
#include "Navigation/NavTrackMaster.h"
#include "MCInfo/MCDecayTree/MCParticle.h"
#include "TrackRoot/TRSeedTrack.h" // include TRSeedTrack
#include "MCInfo/MCDecayTree/MCDecayTree.h" // include MCDecayTree
#include "MCInfo/MCDecayTree/MCParticle.h" // include MCParticle

#include "MCTrackTagger/MCTrackTagsByTrack.h" // matches for a track
#include "MCTrackTagger/MCATrackTag.h" // matches for a track
#include "MCTrackTagger/MCTagBestByMostTotalHits.h" // best tag decision maker


// STL classes
#include <map>
#include <vector>
#include <set>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Navigation.NavTrackMaster" ;

const MCTrackTagsByTrack* 
NavTrackMaster::mcTags( const NavTrack::Identifier aId ) const 
{
   if( 0 == m_mcTags )
   {
      FATable< NavTrack > navTracks;
      extract( dict(),    navTracks );

      FATable< MCTrackTagsByTrack > tags   ;
      extract( dict(),              tags ) ;

      const_cast<NavTrackMaster*>(this)->m_mcTags = tags.getContents() ;
   }
   FAPtrTable<MCTrackTagsByTrack>::const_iterator found
      ( m_mcTags->find( aId ) ) ;
   return ( found == m_mcTags->end() ? 0 : &( *found ) ) ;
}

const MCParticle* 
NavTrackMaster::mcTag( const NavTrack::Identifier aId ) const 
{
   if( 0 == m_mcTag )
   {
      FATable< NavTrack > navTracks;
      extract( dict(),    navTracks );

      const_cast<NavTrackMaster*>(this)->m_mcTag = 
	 new STL_VECTOR( const MCParticle* ) ;
      const_cast<NavTrackMaster*>(this)->m_mcTag->reserve( navTracks.size() ) ;

      // Extract the MCDecayTree (Note: MUST get the CLEOG tree)
      FAItem< MCDecayTree > decayTree;
      extract( dict(),      decayTree ); // No usage tag

      for( FATable<NavTrack>::const_iterator iT ( navTracks.begin() ) ;
	   iT != navTracks.end() ; ++ iT )
      {
	 // Get the track identifier
	 const NavTrack::Identifier trackID ( iT->identifier() ) ;

	 const MCTrackTagsByTrack* myTags ( mcTags( trackID ) ) ;

	 const MCParticle* tag ( 0 ) ;

	 if( 0 != myTags           &&
	     myTags->isMatched()   &&
	     iT->pionFit().valid()     )
	 {
	    const DABoolean chgPos ( iT->pionFit()->charge() > 0 ) ;

	    MCTrackTagsByTrack::MatchVector::const_iterator mEnd 
	       ( myTags->matches().end() ) ;

	    for( MCTrackTagsByTrack::MatchVector::const_iterator iM 
		    ( myTags->matches().begin() ) ;  iM != mEnd ; ++iM )
	    {
	       const MCParticle& part ( decayTree->getParticle(
		  iM->mcParticleID() ) ) ;
	       const DABoolean mcPos ( part.properties().charge() > 0 ) ;
	       if( chgPos == mcPos )
	       {
		  tag = &part ;
		  break ;
	       }
	    }
	 }
	 const_cast<NavTrackMaster*>(this)->m_mcTag->push_back( tag ) ;	 
      }
   }
   return ( aId > 0 && aId <= m_mcTag->size() ? (*m_mcTag)[ aId-1 ] : 0 ) ;
}


