// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavConRegServer
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Thu Jun 24 15:03:58 EDT 1999
// $Id: NavConRegServer.cc,v 1.4 2002/03/25 11:52:54 bkh Exp $
//
// Revision history
//
// $Log: NavConRegServer.cc,v $
// Revision 1.4  2002/03/25 11:52:54  bkh
// various optimizations for speed
//
// Revision 1.3  1999/12/31 23:19:43  bkh
// Elim valid() checking after extract
//
// Revision 1.2  1999/12/31 23:14:25  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.1  1999/08/09 16:25:07  bkh
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
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "Navigation/NavConRegServer.h"

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

static const char* const kReport = "Navigation.NavConRegServer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavConRegServer.cc,v 1.4 2002/03/25 11:52:54 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

NavConRegServer::NavConRegServer( const Record& aRecord ) :
   m_record    ( aRecord ) ,
   m_navShower (         ) ,
   m_atts      (         ) ,
   m_lattice   ( 0 ) 
{
}

// NavConRegServer::NavConRegServer( const NavConRegServer& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavConRegServer::~NavConRegServer()
{
}

//
// assignment operators
//
// const NavConRegServer& NavConRegServer::operator=( const NavConRegServer& rhs )
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

const NavShower& 
NavConRegServer::navShower( const NavShower::Identifier aNavShowerId  ) const 
{
   if( !m_navShower.valid() )
   {
      extract( m_record, 
	       const_cast< NavConRegServer* >( this )->m_navShower ) ;
   }
   return m_navShower[ aNavShowerId ] ;
}

const CcConRegAttributes& 
NavConRegServer::attributes( const CcConRegAttributes::Identifier aId ) const 
{
   if( !m_atts.valid() )
   {
      extract( m_record, 
	       const_cast< NavConRegServer* >( this )->m_atts ) ;
   }
   return m_atts[ aId ] ;
}

const TrackShowerLattice& 
NavConRegServer::trackShowerLattice() const 
{
   if( 0 == m_lattice )
   {
      FAItem< TrackShowerLattice > lattice ;
      extract( m_record, lattice ) ;
      const_cast< NavConRegServer* >( this )->m_lattice = &(*lattice) ;
   }
   return *m_lattice ;
}

//
// static member functions
//
