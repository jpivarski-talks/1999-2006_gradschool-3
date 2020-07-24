// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavRichProxy
// 
// Description:
//
// Implementation:
//
// Author:      Tomasz Skwarnicki
// Created:     Sun Jul 23 05:40:01 EDT 2000
// $Id: NavRichProxy.cc,v 1.2 2000/11/20 00:09:03 ts Exp $
//
// Revision history
//
// $Log: NavRichProxy.cc,v $
// Revision 1.2  2000/11/20 00:09:03  ts
// build NavRich on top of RichTrackPrototype
//
// Revision 1.1  2000/07/27 01:35:19  ts
// NavRich added
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

#include "NavigationProd/NavRichProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "RichTrackPrototypesProd/RichTrackPrototype.h"
#include "Navigation/NavRichServer.h" 

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
static const char* const kReport = "NavigationProd.NavRichProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavRichProxy.cc,v 1.2 2000/11/20 00:09:03 ts Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavRichProxy::NavRichProxy() :
   m_ptrTable ( new value_type ) ,
   m_server   ( 0              )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavRichProxy::boundMethod, Stream::kBeginRun );
}

// NavRichProxy::NavRichProxy( const NavRichProxy& iproxy )
// {
//   *this = iproxy;
// }

NavRichProxy::~NavRichProxy()
{
   delete m_server ;
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const NavRichProxy& NavRichProxy::operator=( const NavRichProxy& iproxy )
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
NavRichProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) 
   {
      (*m_ptrTable).eraseAll() ;
   }
   delete m_server ;
   m_server = 0 ;
}

const NavRichProxy::value_type*
NavRichProxy::faultHandler( const Record&  iRecord ,
			      const DataKey& iKey      )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   assert( 0 == m_server ) ;

   m_server = new NavRichServer( iRecord ) ;

   assert( 0 != m_server ) ;

   FATable< RichTrackPrototype > richTrack  ;
   extract( iRecord, richTrack ); 

   for ( FATable< RichTrackPrototype >::const_iterator iTrk ( richTrack.begin() ) ;
         iTrk != richTrack.end() ; ++iTrk ) 
   {
      NavRich* tmp ( new NavRich( *iTrk    ,
				      *m_server ) ) ;
      
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
//NavRichProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kReport) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
//#include "ProxyBind/Template/ProxyBindableTemplate.cc"
//template class ProxyBindableTemplate< FAPtrTable< NavRich > >;

//PROXY_BIND_METHOD( NavRichProxy )
//-----------------------------------------------


