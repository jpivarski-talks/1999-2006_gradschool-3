// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRichServer
// 
// Description:
//
// Implementation:
//
// Author:      Tomasz Skwarnicki
// Created:     Fri Jul 22 17:29:00 EDT 2000
// $Id: NavRichServer.cc,v 1.2 2000/11/20 00:05:52 ts Exp $
//
// Revision history
//
// $Log: NavRichServer.cc,v $
// Revision 1.2  2000/11/20 00:05:52  ts
// build NavRich on top of RichTrackPrototype
//
// Revision 1.1  2000/07/27 01:31:34  ts
// NavRich added
//
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
#include "Navigation/NavRichServer.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           


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

static const char* const kReport = "Navigation.NavRichServer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavRichServer.cc,v 1.2 2000/11/20 00:05:52 ts Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavRichServer::NavRichServer( const Record& aRecord ) :
   m_record     ( aRecord ) ,
   m_RichTrack () ,
   m_RichImprovedTrack () ,
   m_NavTrack () ,
   m_RichTrackAveAngles () ,
   m_RichTrackChamberCoord () ,
   m_RichTrackCRegionMatch () ,
   m_RichConnectedRegion () ,
   m_RichTrackRadiators () ,
   m_RichTrackPhotons () ,
   m_RichPhoton () ,
   m_RichBump () ,
   m_RichCalibratedHit () ,
   m_RichGeometryUpdated ( false )
{
}


// NavRichServer::NavRichServer( const NavRichServer& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

NavRichServer::~NavRichServer()
{
}

//
// assignment operators
//
// const NavRichServer& NavRichServer::operator=( const NavRichServer& rhs )
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


/*
const NavRich& 
NavRichServer::NavRich(
   const NavRich::Identifier aNavRichId           ) const 
{
   if( !m_NavRich.valid() )
   {
      extract( m_record, 
	       const_cast< NavRichServer* >( this )->m_NavRich ) ;
   }
   return m_NavRich[ aNavRichId ] ;
}
*/
 

//
// static member functions
//
