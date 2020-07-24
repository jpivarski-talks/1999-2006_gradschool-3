// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavEtaPrimeToGGProxy
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Aug  6 13:20:44 EDT 1999
// $Id: NavEtaPrimeToGGProxy.cc,v 1.1 2001/05/10 19:56:40 bkh Exp $
//
// Revision history
//
// $Log: NavEtaPrimeToGGProxy.cc,v $
// Revision 1.1  2001/05/10 19:56:40  bkh
// Addition of etaPrime classes
//
// Revision 1.3  2000/04/17 20:13:36  bkh
// Elim checks of ptr after new
//
// Revision 1.2  1999/12/31 23:15:14  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.1  1999/08/09 16:27:19  bkh
// Renaming and addition of pi0, etaPrime proxies
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

#include "NavigationProd/NavEtaPrimeToGGProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "PhotonDecays/PhdEtaPrimeToGG.h" 
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
static const char* const kReport = "Navigation.NavEtaPrimeToGGProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavEtaPrimeToGGProxy.cc,v 1.1 2001/05/10 19:56:40 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavEtaPrimeToGGProxy::NavEtaPrimeToGGProxy()
   : m_ptrTable( new value_type )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavEtaPrimeToGGProxy::boundMethod, Stream::kBeginRun );
}

// NavEtaPrimeToGGProxy::NavEtaPrimeToGGProxy( const NavEtaPrimeToGGProxy& iproxy )
// {
//   *this = iproxy;
// }

NavEtaPrimeToGGProxy::~NavEtaPrimeToGGProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const NavEtaPrimeToGGProxy& NavEtaPrimeToGGProxy::operator=( const NavEtaPrimeToGGProxy& iproxy )
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
NavEtaPrimeToGGProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) 
   {
      (*m_ptrTable).eraseAll() ;
   }
}

const NavEtaPrimeToGGProxy::value_type*
NavEtaPrimeToGGProxy::faultHandler( const Record&  iRecord ,
				    const DataKey& iKey      )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   FATable< PhdEtaPrimeToGG > etaPrimes ;
   extract( iRecord,          etaPrimes  ); 

   FATable< NavShower > sh ;
   extract( iRecord,    sh ); 

   for ( FATable< PhdEtaPrimeToGG >::const_iterator iEtaPrime 
	    ( etaPrimes.begin() ) ; iEtaPrime != etaPrimes.end() ;
	 ++iEtaPrime ) 
   {
      const PhdEtaPrimeToGG& etaPrime ( *iEtaPrime ) ;

      NavEtaPrimeToGG* tmp ( new NavEtaPrimeToGG( &etaPrime                ,
						  &sh[ etaPrime.hiEnId() ] ,
						  &sh[ etaPrime.loEnId() ]  ));

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
//NavEtaPrimeToGGProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kReport) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< NavEtaPrimeToGG > >;

PROXY_BIND_METHOD( NavEtaPrimeToGGProxy )
//-----------------------------------------------


