// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaToGGProxy
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: NavEtaToGGProxy.cc,v 1.3 2000/04/17 20:13:36 bkh Exp $
//
// Revision history
//
// $Log: NavEtaToGGProxy.cc,v $
// Revision 1.3  2000/04/17 20:13:36  bkh
// Elim checks of ptr after new
//
// Revision 1.2  1999/12/31 23:15:14  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.1  1999/08/09 16:27:19  bkh
// Renaming and addition of pi0, eta proxies
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

#include "NavigationProd/NavEtaToGGProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "PhotonDecays/PhdEtaToGG.h" 
#include "Navigation/NavShower.h"

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
// constants, enums and typedef //
static const char* const kReport = "Navigation.NavEtaToGGProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavEtaToGGProxy.cc,v 1.3 2000/04/17 20:13:36 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavEtaToGGProxy::NavEtaToGGProxy()
   : m_ptrTable( new value_type )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavEtaToGGProxy::boundMethod, Stream::kBeginRun );
}

// NavEtaToGGProxy::NavEtaToGGProxy( const NavEtaToGGProxy& iproxy )
// {
//   *this = iproxy;
// }

NavEtaToGGProxy::~NavEtaToGGProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const NavEtaToGGProxy& NavEtaToGGProxy::operator=( const NavEtaToGGProxy& iproxy )
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
NavEtaToGGProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) 
   {
      (*m_ptrTable).eraseAll() ;
   }
}

const NavEtaToGGProxy::value_type*
NavEtaToGGProxy::faultHandler( const Record&  iRecord ,
			       const DataKey& iKey      )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   FATable< PhdEtaToGG > etas ;
   extract( iRecord, etas ); 

   FATable< NavShower > sh ;
   extract( iRecord, sh ); 

   for ( FATable< PhdEtaToGG >::const_iterator iEta ( etas.begin() ) ;
         iEta != etas.end() ; ++iEta ) 
   {
      const PhdEtaToGG& eta ( *iEta ) ;

      NavEtaToGG* tmp ( new NavEtaToGG( &eta                ,
					&sh[ eta.hiEnId() ] ,
					&sh[ eta.loEnId() ]   ) ) ;

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
//NavEtaToGGProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kReport) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< NavEtaToGG > >;

PROXY_BIND_METHOD( NavEtaToGGProxy )
//-----------------------------------------------


