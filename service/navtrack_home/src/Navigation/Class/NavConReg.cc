// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavConReg
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Wed Jun 23 14:40:12 EDT 1999
// $Id: NavConReg.cc,v 1.8 2003/02/03 20:16:14 bkh Exp $
//
// Revision history
//
// $Log: NavConReg.cc,v $
// Revision 1.8  2003/02/03 20:16:14  bkh
// Adjust to new mc tagging interface
// Get rid of friends with public access to record
// New track matching alternatives
//
// Revision 1.7  2002/03/25 11:52:54  bkh
// various optimizations for speed
//
// Revision 1.6  2001/10/30 16:55:40  bkh
// Fix bug(s) with con regs
//
// Revision 1.5  2001/10/26 21:53:53  bkh
// Add features to showers and connected region objects
//
// Revision 1.4  2001/02/02 19:34:14  bkh
// New constructor and new way of traversing lattice
//
// Revision 1.3  2000/10/26 16:01:00  bkh
// Bring up to date with CcConRegAttributes changes
//
// Revision 1.2  1999/12/06 18:12:41  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.1  1999/08/09 16:25:06  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:37  bkh
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
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "Navigation/NavConReg.h"
#include "Navigation/NavShower.h"
#include "Navigation/NavConRegServer.h"

#include "CLHEP/Vector/ThreeVector.h"

#include "FrameAccess/FAConstPtrTable.h"
#include "FrameAccess/FATable.h"


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

static const char* const kReport = "Navigation.NavConReg" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavConReg.cc,v 1.8 2003/02/03 20:16:14 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

NavConReg::NavConReg( const NavConReg::Identifier    aId     ,
		      const CcConRegShowerLattice&   aLattice  ,
		      const NavConRegServer&         aServer   ) :
   m_id            ( aId      ) ,
   m_lattice       ( &aLattice ) ,
   m_server        ( aServer  ) ,
   m_attsPtr       ( 0        ) ,
   m_showers       ( 0        ) ,
   m_doneTrackMatch ( false   ) ,
   m_noTrackMatch   ( true    ) ,
   m_matchedShower  ( 0       )
{
}

// NavConReg::NavConReg( const NavConReg& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavConReg::~NavConReg()
{
   delete m_showers   ;
}

//
// assignment operators
//
// const NavConReg& NavConReg::operator=( const NavConReg& rhs )
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

const CcConRegAttributes& 
NavConReg::atts() const 
{
   if( 0 == m_attsPtr )
   {
      if( 0 >= m_id )
      {
	 report( ERROR, kReport ) 
	    << "Zero/neg leftID for ConReg link" << endl ;
	 assert( false ) ;
	 ::exit( 1 ) ;
      }
      const_cast< NavConReg* >( this )->m_attsPtr =
	 &( m_server.attributes( m_id ) ) ;
   }
   return *m_attsPtr ;
}

NavConReg::Identifier                         
NavConReg::identifier() const 
{
   return atts().identifier() ;
}

const Hep3Vector& 
NavConReg::momentum()   const 
{
   return atts().momentum() ;
}

CcGeV             
NavConReg::energy()     const 
{
   return atts().energy() ;
}

CcGeV             
NavConReg::mass()       const 
{
   return atts().mass() ;
}

const NavConReg::ShowerIdList&
NavConReg::showerIds()    const 
{
   return *m_lattice->vRightGivenLeft( m_id ) ;
}

DABoolean
NavConReg::noTrackMatchConReg()    const 
{
   if( !m_doneTrackMatch )
   {
      const_cast< NavConReg* >( this )->m_doneTrackMatch = true ;

      const TrackShowerLattice& tkShLat ( m_server.trackShowerLattice() ) ;

      const NavConReg::ShowerIdList& list ( showerIds() ) ;
      ShowerIdList::const_iterator shEnd ( list.end() ) ;
      for( ShowerIdList::const_iterator iS ( list.begin() ) ;
	   iS != shEnd ; ++iS )
      {
	 const TrackShowerLattice::Links& trackLinks (
	    tkShLat.linksGivenRight( *iS ) ) ;
	 if( 0 != trackLinks.size() )
	 {
	    const_cast< NavConReg* >( this )->m_noTrackMatch = false ;
	    const_cast< NavConReg* >( this )->m_matchedShower = 
	       &m_server.navShower(*iS) ;
	    break ;	    
	 }
      }
   }
   return m_noTrackMatch ;
}

FATable< NavShower >
NavConReg::showers()    const 
{
   if( 0 == m_showers )
   {
      const_cast< NavConReg* >( this )->m_showers = 
	 new FAConstPtrTable< NavShower > ;

      const ShowerIdList& sIds ( showerIds() ) ;
      ShowerIdList::const_iterator sEnd ( sIds.end() ) ;
      for( ShowerIdList::const_iterator iSh ( sIds.begin() ) ;
	   iSh != sEnd ; ++iSh )
      {
	 m_showers->insert( &m_server.navShower( *iSh ) ) ;
      }
   }
   return FATable< NavShower >( m_showers ) ;
}


FAItem< NavShower >
NavConReg::matchedShower()    const 
{
   if( !m_doneTrackMatch )
   {
      noTrackMatchConReg() ;
   }
   return FAItem<NavShower>( m_matchedShower ) ;
}

//
// static member functions
//

