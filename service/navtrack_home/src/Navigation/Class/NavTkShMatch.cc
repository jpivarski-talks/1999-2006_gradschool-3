// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTkShMatch
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Mon Jun 21 17:28:50 EDT 1999
// $Id: NavTkShMatch.cc,v 1.5 2002/03/25 11:52:56 bkh Exp $
//
// Revision history
//
// $Log: NavTkShMatch.cc,v $
// Revision 1.5  2002/03/25 11:52:56  bkh
// various optimizations for speed
//
// Revision 1.4  2001/12/13 20:54:05  bkh
// Implement eta-->gamgam access from NavShower just as with pi0
// For track match object, get energy and momentum directly because
//    shower energy can have run-time adjustments
//
// Revision 1.3  2000/08/11 00:20:59  bkh
// Add operator<< functionality to these classes
//
// Revision 1.2  1999/12/06 18:12:44  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.1  1999/08/09 16:25:12  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:41  bkh
// New classes associated with analysis-level shower object
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
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "FrameAccess/FATable.h"
#include "FrameAccess/FAConstPtrTable.h"

#include "Navigation/NavTkShMatch.h"
#include "Navigation/NavTkShMatchServer.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kReport = "Navigation.NavTkShMatch" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavTkShMatch.cc,v 1.5 2002/03/25 11:52:56 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavTkShMatch::NavTkShMatch( 
   const NavTkShMatch::Identifier aId     ,
   const LatticeLink&             aLink   ,
   const NavTkShMatchServer&      aServer   ) :
   m_identifier    ( aId             ) ,
   m_link          ( aLink           ) ,
   m_server        ( aServer         ) ,
   m_navTrack      ( 0               ) ,
   m_showers       ( 0               ) ,
   m_ener          ( 0               ) ,
   m_eOverP        ( 0               )
{
   if( 0 == m_link.leftID() )
   {
      report( ERROR, kReport ) << "No TrackId for Id=" << aId << endl ;
      assert( false ) ;
      ::exit( 1 ) ;
   }
   if( 0 == m_link.vRightID().size() )
   {
      report( ERROR, kReport ) << "No ShowerIds for Id=" << aId << endl ;
      assert( false ) ;
      ::exit( 1 ) ;
   }
}

// NavTkShMatch::NavTkShMatch( const NavTkShMatch& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavTkShMatch::~NavTkShMatch()
{
   delete m_showers ;
}

//
// assignment operators
//
// const NavTkShMatch& NavTkShMatch::operator=( const NavTkShMatch& rhs )
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

//
// const member functions
//

NavTkShMatch::Identifier
NavTkShMatch::identifier()    const
{
   return m_identifier ;
}

const NavTrack&   
NavTkShMatch::navTrack()      const 
{
   if( 0 == m_navTrack )
   {
      ((NavTkShMatch*)this)->m_navTrack = 
	 &( m_server.navTrack( trackId() ) ) ;
      assert( 0 != m_navTrack ) ;
   }
   return *m_navTrack ;
}

FATable< NavShower >
NavTkShMatch::navShowers()    const 
{
   if( 0 == m_showers )
   {
      const_cast< NavTkShMatch* >( this )->m_showers = 
	 new FAConstPtrTable< NavShower > ;
      const ShowerIdList& sIds ( showerIds() ) ;
      ShowerIdList::const_iterator sEnd ( sIds.end() ) ;
      for( ShowerIdList::const_iterator iSh ( sIds.begin() ) ;
	   iSh != sEnd ; ++iSh )
      {
	 m_showers->insert( &( m_server.navShower( *iSh ) ) ) ;
      }
   }
   return FATable< NavShower >( m_showers ) ;
}

CcGeV             
NavTkShMatch::matchedEnergy() const 
{
   if( 0 == m_ener )
   {
      ShowerIdList::const_iterator sEnd ( showerIds().end() ) ;
      for( ShowerIdList::const_iterator iS ( showerIds().begin() ) ;
	   iS != sEnd ; ++iS )
      {
	 const_cast< NavTkShMatch* >( this )->m_ener += 
	    m_server.navShower( *iS ).attributes().energy() ;
      }
   }
//   return m_link.linkData().matchedEnergy() ;
   return m_ener ;
}

CcReal            
NavTkShMatch::eOverP()        const 
{
   if( 0 == m_eOverP )
   {
      const_cast< NavTkShMatch* >( this )->m_eOverP =
	 matchedEnergy()/m_server.navTrack( trackId() ).
	 pionFit()->momentum().mag() ;
   }
   return m_eOverP ;
//   return m_link.linkData().eOverP() ;
}
CcMeters          
NavTkShMatch::distance()      const 
{
   return m_link.linkData().distance() ;
}

NavTkShMatch::TrackId             
NavTkShMatch::trackId()       const 
{
   return *( m_link.leftID() ) ;
}

const NavTkShMatch::ShowerIdList& 
NavTkShMatch::showerIds()     const 
{
   return m_link.vRightID() ;
}



//
// static member functions
//
