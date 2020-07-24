// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavShowerServer
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Jun 25 17:29:00 EDT 1999
// $Id: NavShowerServer_mcTag.cc,v 1.2 2003/02/03 20:16:17 bkh Exp $
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Navigation/NavShowerServer.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAConstPtrTable.h"
#include "MCCCTagger/MCCCTagger.h"
#include "MCInfo/MCParticleProperty/MCParticleProperty.h"
#include "MCInfo/MCDecayTree/MCDecayTree.h"
#include "MCInfo/MCDecayTree/MCParticle.h"
#include "MCInfo/MCDecayTree/MCVertex.h"

#include "MCCCTagger/MCCCTagsByShower.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kReport = "Navigation.NavShowerServer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavShowerServer_mcTag.cc,v 1.2 2003/02/03 20:16:17 bkh Exp $";
static const char* const kTagString = "$Name:  $";


const MCParticle* 
NavShowerServer::mcTag( const NavShower::Identifier aId ) const 
{
   if( 0 == m_mcTag )
   {
      FATable< NavShower > navShowers;
      extract( m_record,   navShowers );

      const_cast<NavShowerServer*>(this)->m_mcTag = 
	 new STL_VECTOR( const MCParticle* ) ;
      const_cast<NavShowerServer*>(this)->m_mcTag->reserve(
	 navShowers.size() ) ;

      // Extract the MCDecayTree (Note: MUST get the CLEOG tree)
      FAItem< MCDecayTree > decayTree;
      extract( m_record,    decayTree ); // No usage tag

      for( FATable<NavShower>::const_iterator iS ( navShowers.begin() ) ;
	   iS != navShowers.end() ; ++ iS )
      {
	 // Get the track identifier
	 const NavTrack::Identifier showerID ( iS->identifier() ) ;

	 const MCCCTags* myTags ( mcTags( showerID ) ) ;

	 const MCParticle* tag ( 0 ) ;

	 if( 0 != myTags           &&
	     myTags->isMatched()       )
	 {
	    tag = &(*decayTree).getParticle(
	       myTags->bestMCParticleId() ) ;
	 }
	 const_cast<NavShowerServer*>(this)->m_mcTag->push_back( tag ) ;
      }
   }
   return ( aId >0 && aId <= m_mcTag->size() ? (*m_mcTag)[ aId-1 ] : 0 ) ;
}

const MCCCTagsByShower* 
NavShowerServer::mcTags( const NavShower::Identifier aId ) const 
{
   if( 0 == m_mcTags )
   {
      FATable< NavShower > navShowers;
      extract( m_record,   navShowers );

      FATable< MCCCTagsByShower > tags   ;
      extract( m_record,          tags ) ;

      const_cast<NavShowerServer*>(this)->m_mcTags = tags.getContents() ;
   }
   return &(*m_mcTags->find( aId ) ) ;
}
