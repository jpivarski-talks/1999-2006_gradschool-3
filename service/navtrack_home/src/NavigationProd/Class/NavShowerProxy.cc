// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavShowerProxy
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Sun Jun 27 19:58:56 EDT 1999
// $Id: NavShowerProxy.cc,v 1.3 2000/04/17 20:13:36 bkh Exp $
//
// Revision history
//
// $Log: NavShowerProxy.cc,v $
// Revision 1.3  2000/04/17 20:13:36  bkh
// Elim checks of ptr after new
//
// Revision 1.2  1999/12/31 23:15:16  bkh
// Eliminate obsolete checking of valid() after extracts
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

#include "NavigationProd/NavShowerProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "C3cc/CcShowerAttributes.h" 
#include "Navigation/NavShowerServer.h" 

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
static const char* const kReport = "NavigationProd.NavShowerProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavShowerProxy.cc,v 1.3 2000/04/17 20:13:36 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavShowerProxy::NavShowerProxy() :
   m_ptrTable ( new value_type ) ,
   m_server   ( 0              )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavShowerProxy::boundMethod, Stream::kBeginRun );
}

// NavShowerProxy::NavShowerProxy( const NavShowerProxy& iproxy )
// {
//   *this = iproxy;
// }

NavShowerProxy::~NavShowerProxy()
{
   delete m_server ;
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const NavShowerProxy& NavShowerProxy::operator=( const NavShowerProxy& iproxy )
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
NavShowerProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) 
   {
      (*m_ptrTable).eraseAll() ;
   }
   delete m_server ;
   m_server = 0 ;
}

const NavShowerProxy::value_type*
NavShowerProxy::faultHandler( const Record&  iRecord ,
			      const DataKey& iKey      )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   assert( 0 == m_server ) ;

   m_server = new NavShowerServer( iRecord ) ;

   assert( 0 != m_server ) ;

   FATable< CcShowerAttributes > atts  ;
   extract( iRecord, atts ); 

   for ( FATable< CcShowerAttributes >::const_iterator iSh ( atts.begin() ) ;
         iSh != atts.end() ; ++iSh ) 
   {
      NavShower* tmp ( new NavShower( *iSh    ,
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
//NavShowerProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kReport) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
//#include "ProxyBind/Template/ProxyBindableTemplate.cc"
//template class ProxyBindableTemplate< FAPtrTable< NavShower > >;

//PROXY_BIND_METHOD( NavShowerProxy )
//-----------------------------------------------


