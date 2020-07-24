// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavPi0ToGGProxy
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: NavPi0ToGGProxy.cc,v 1.3 2000/04/17 20:13:36 bkh Exp $
//
// Revision history
//
// $Log: NavPi0ToGGProxy.cc,v $
// Revision 1.3  2000/04/17 20:13:36  bkh
// Elim checks of ptr after new
//
// Revision 1.2  1999/12/31 23:15:15  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.1  1999/08/09 16:27:20  bkh
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

#include "NavigationProd/NavPi0ToGGProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "PhotonDecays/PhdPi0.h" 
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
static const char* const kReport = "Navigation.NavPi0ToGGProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavPi0ToGGProxy.cc,v 1.3 2000/04/17 20:13:36 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavPi0ToGGProxy::NavPi0ToGGProxy()
   : m_ptrTable( new value_type )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavPi0ToGGProxy::boundMethod, Stream::kBeginRun );
}

// NavPi0ToGGProxy::NavPi0ToGGProxy( const NavPi0ToGGProxy& iproxy )
// {
//   *this = iproxy;
// }

NavPi0ToGGProxy::~NavPi0ToGGProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const NavPi0ToGGProxy& NavPi0ToGGProxy::operator=( const NavPi0ToGGProxy& iproxy )
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
NavPi0ToGGProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) 
   {
      (*m_ptrTable).eraseAll() ;
   }
}

const NavPi0ToGGProxy::value_type*
NavPi0ToGGProxy::faultHandler( const Record&  iRecord ,
			       const DataKey& iKey      )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   FATable< PhdPi0 > pi0s ;
   extract( iRecord, pi0s ); 

   FATable< NavShower > sh ;
   extract( iRecord, sh ); 

   for ( FATable< PhdPi0 >::const_iterator iPi0 ( pi0s.begin() ) ;
         iPi0 != pi0s.end() ; ++iPi0 ) 
   {
      const PhdPi0& pi0 ( *iPi0 ) ;

      NavPi0ToGG* tmp ( new NavPi0ToGG( &pi0                ,
					&sh[ pi0.hiEnId() ] ,
					&sh[ pi0.loEnId() ]   ) ) ;

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
//NavPi0ToGGProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kReport) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< NavPi0ToGG > >;

PROXY_BIND_METHOD( NavPi0ToGGProxy )
//-----------------------------------------------


