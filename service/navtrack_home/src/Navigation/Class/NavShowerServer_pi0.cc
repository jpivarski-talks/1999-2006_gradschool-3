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
// $Id: NavShowerServer_pi0.cc,v 1.4 2002/08/08 16:55:09 cleo3 Exp $
//
// Revision history
//
// $Log: NavShowerServer_pi0.cc,v $
// Revision 1.4  2002/08/08 16:55:09  cleo3
// NavShower no longer owns any additional memory
//
// Revision 1.3  2002/03/25 11:52:56  bkh
// various optimizations for speed
//
// Revision 1.2  2001/12/13 20:54:03  bkh
// Implement eta-->gamgam access from NavShower just as with pi0
// For track match object, get energy and momentum directly because
//    shower energy can have run-time adjustments
//
// Revision 1.1  2001/11/13 16:05:36  bkh
// Separated off pi0 pieces to avoid making everyone link to PhotonDecays
//
// Revision 1.11  2001/11/09 20:26:39  bkh
// Added in shower-->pi0  functionality
//
// Revision 1.10  2001/10/30 16:55:40  bkh
// Fix bug(s) with con regs
//
// Revision 1.9  2001/10/26 21:53:53  bkh
// Add features to showers and connected region objects
//
// Revision 1.8  2001/06/26 18:01:41  cdj
// modified to work with new Lattice interface
//
// Revision 1.7  2001/04/03 16:57:57  bkh
// Implement lists of nearby showers/tracks based on distance
//
// Revision 1.6  2000/10/04 20:00:20  bkh
// Remove pi0-vetoing from NavShower; bad idea
//
// Revision 1.5  2000/10/03 18:40:48  bkh
// Add pi0/eta access to NavShower for vetoing, etc.
//
// Revision 1.4  1999/12/31 23:14:25  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.3  1999/12/06 18:12:43  bkh
// Change interface to use FATables where possible not vectors
//
// Revision 1.2  1999/08/09 16:25:11  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:39  bkh
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
#include "Navigation/NavShowerServer.h"
#include "Navigation/NavPi0ToGG.h"
#include "Navigation/NavEtaToGG.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAConstPtrTable.h"

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
static const char* const kIdString  = "$Id: NavShowerServer_pi0.cc,v 1.4 2002/08/08 16:55:09 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

const FAPtrTable<NavPi0ToGG>*
NavShowerServer::navPi0s() const
{
   if( 0 == m_navPi0 )
   {
      FATable< NavPi0ToGG > table ;
      extract( m_record, table ) ;
      const_cast< NavShowerServer* >( this )->m_navPi0 = 
	 table.getContents() ;
   }
   return m_navPi0 ;
}

const FAPtrTable<NavEtaToGG>*
NavShowerServer::navEtas() const
{
   if( 0 == m_navEta )
   {
      FATable< NavEtaToGG > table ;
      extract( m_record, table ) ;
      const_cast< NavShowerServer* >( this )->m_navEta = 
	 table.getContents();
   }
   return m_navEta ;
}

const FAPtrTable<NavPi0ToGG>* 
NavShowerServer::matchedPi0s(
   const NavShower::Identifier aNavShowerId     ) const 
{
   FAConstPtrTable<NavPi0ToGG>* tPi0s =
      new FAConstPtrTable< NavPi0ToGG > ;
   
   const_cast<NavShowerServer*>(this)->m_matchedPi0s.push_back(tPi0s);
   
   FATable<NavPi0ToGG> allPi0s ( navPi0s() ) ;
   
   for( FATable<NavPi0ToGG>::const_iterator iP 
	   ( allPi0s.begin() ) ; iP != allPi0s.end() ; ++iP )
   {
      if( aNavShowerId == (*iP).hiEnShower().identifier() ||
	  aNavShowerId == (*iP).loEnShower().identifier()      )
      {
	 tPi0s->insert( &(*iP) ) ;
      }
   }
   return tPi0s ;
}

const FAPtrTable<NavEtaToGG>* 
NavShowerServer::matchedEtas(
   const NavShower::Identifier aNavShowerId     ) const
{
   FAConstPtrTable<NavEtaToGG>* tEtas =
      new FAConstPtrTable< NavEtaToGG > ;
   
   const_cast<NavShowerServer*>(this)->m_matchedEtas.push_back(tEtas);
   
   FATable<NavEtaToGG> allEtas ( navEtas() ) ;
   
   for( FATable<NavEtaToGG>::const_iterator iE 
	   ( allEtas.begin() ) ; iE != allEtas.end() ; ++iE )
   {
      if( aNavShowerId == (*iE).hiEnShower().identifier() ||
	  aNavShowerId == (*iE).loEnShower().identifier()      )
      {
	 tEtas->insert( &(*iE) ) ;
      }
   }
   return tEtas ;
}

//
// static member functions
//


