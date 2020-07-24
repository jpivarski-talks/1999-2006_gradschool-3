// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavTkShMatchProxy
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Tue Jun 22 17:00:56 EDT 1999
// $Id: NavTkShMatchProxy.cc,v 1.4 2002/03/16 21:46:53 cdj Exp $
//
// Revision history
//
// $Log: NavTkShMatchProxy.cc,v $
// Revision 1.4  2002/03/16 21:46:53  cdj
// NavTkShMatchProxy no longer extracts NavTracks and NavShowers
//
// Revision 1.3  2000/04/17 20:13:36  bkh
// Elim checks of ptr after new
//
// Revision 1.2  1999/12/31 23:15:16  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.1  1999/08/09 16:27:20  bkh
// Renaming and addition of pi0, eta proxies
//
// Revision 1.1.1.1  1999/07/19 16:47:46  bkh
// imported NavigationProd sources
//
// Revision 1.1  1999/06/29 21:14:57  bkh
// Add Proxies for high level shower object, connected regions, matches
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
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

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"

#include "NavigationProd/NavTkShMatchProxy.h"
//#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

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
static const char* const kReport = "NavigationProd.NavTkShMatchProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavTkShMatchProxy.cc,v 1.4 2002/03/16 21:46:53 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavTkShMatchProxy::NavTkShMatchProxy() :
   m_ptrTable ( new value_type ) ,
   m_server   ( 0              ) 
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavTkShMatchProxy::boundMethod, Stream::kBeginRun );
}

// NavTkShMatchProxy::NavTkShMatchProxy( const NavTkShMatchProxy& iproxy )
// {
//   *this = iproxy;
// }

NavTkShMatchProxy::~NavTkShMatchProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
   delete m_server ;
}

//
// assignment operators
//
// const NavTkShMatchProxy& NavTkShMatchProxy::operator=( const NavTkShMatchProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//

void
NavTkShMatchProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) 
   {
      (*m_ptrTable).eraseAll() ;
   }
   delete m_server ;
   m_server = 0 ;
}

const NavTkShMatchProxy::value_type*
NavTkShMatchProxy::faultHandler( const Record&  iRecord ,
				     const DataKey& iKey      )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   assert( 0 == m_server ) ;

   FAItem< TrackShowerLattice > lattice ;
   extract( iRecord, lattice ) ; 

   const TrackShowerLattice::Links& links ( (*lattice).links() ) ;

   m_server = new NavTkShMatchServer( iRecord ) ;
   assert( 0 != m_server ) ;

   NavTkShMatch::Identifier id ( 0 ) ;

   for( TrackShowerLattice::LinkItr iLink ( links.begin() ) ;
	iLink != links.end() ; ++iLink )
   {
      NavTkShMatch* tmp( new NavTkShMatch( ++id      ,
					   *(*iLink) ,
					   *m_server   ) ) ;
      
      (*m_ptrTable).insert( tmp ) ;
   }

   return m_ptrTable ;
}

//
// const member functions
//

//
// static member functions
//

//
// 'bound' functions
//

//void
//NavTkShMatchProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kReport) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
//#include "ProxyBind/Template/ProxyBindableTemplate.cc"
//template class ProxyBindableTemplate< FAPtrTable< NavTkShMatch > >;

//PROXY_BIND_METHOD( NavTkShMatchProxy )
//-----------------------------------------------


