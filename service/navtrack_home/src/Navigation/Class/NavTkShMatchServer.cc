// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTkShMatchServer
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Tue Jun 22 12:18:38 EDT 1999
// $Id: NavTkShMatchServer.cc,v 1.2 1999/12/31 23:14:26 bkh Exp $
//
// Revision history
//
// $Log: NavTkShMatchServer.cc,v $
// Revision 1.2  1999/12/31 23:14:26  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.1  1999/08/09 16:25:12  bkh
// Massive renaming
//
// Revision 1.1  1999/06/29 21:10:41  bkh
// New classes associated with analysis-level shower object
//

#include "Experiment/Experiment.h"

// system include files

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
//#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           

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

//static const char* const kReport = "Navigation.NavTkShMatchServer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavTkShMatchServer.cc,v 1.2 1999/12/31 23:14:26 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

NavTkShMatchServer::NavTkShMatchServer( const Record& aRecord ) :
   m_record ( aRecord )
{
}

// NavTkShMatchServer::NavTkShMatchServer( const NavTkShMatchServer& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavTkShMatchServer::~NavTkShMatchServer()
{
}

//
// assignment operators
//
// const NavTkShMatchServer& NavTkShMatchServer::operator=( const NavTkShMatchServer& rhs )
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

const NavTrack&           
NavTkShMatchServer::navTrack(
   const NavTrack::Identifier  aNavTrackId  ) const 
{
   if( !m_navTrack.valid() )
   {
      extract( m_record, 
	       const_cast< NavTkShMatchServer* >( this )->m_navTrack ) ;
   }
   return m_navTrack[ aNavTrackId ] ;
}

const NavShower&          
NavTkShMatchServer::navShower(
   const NavShower::Identifier aNavShowerId ) const 
{
   if( !m_navShower.valid() )
   {
      extract( m_record, 
	       const_cast< NavTkShMatchServer* >( this )->m_navShower ) ;
   }
   return m_navShower[ aNavShowerId ] ;
}

//
// static member functions
//
