// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavConRegProxy
// 
// Description:
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Fri Jun 25 12:00:26 EDT 1999
// $Id: NavConRegProxy.cc,v 1.6 2002/03/25 11:54:47 bkh Exp $
//
// Revision history
//
// $Log: NavConRegProxy.cc,v $
// Revision 1.6  2002/03/25 11:54:47  bkh
// Speed up and match new interface of con reg
//
// Revision 1.5  2001/10/30 16:57:05  bkh
// Fix bugs on con regs
//
// Revision 1.4  2001/02/02 19:43:03  bkh
// Make consistent with new NavConReg constructor and new method of
// traversing lattice.
//
// Revision 1.3  2000/04/17 20:13:35  bkh
// Elim checks of ptr after new
//
// Revision 1.2  1999/12/31 23:15:14  bkh
// Eliminate obsolete checking of valid() after extracts
//
// Revision 1.1  1999/08/09 16:27:18  bkh
// Renaming and addition of pi0, eta proxies
//
// Revision 1.1.1.1  1999/07/19 16:47:46  bkh
// imported NavigationProd sources
//
// Revision 1.1  1999/06/29 21:14:56  bkh
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

#include "NavigationProd/NavConRegProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

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
static const char* const kReport = "NavigationProd.NavConRegProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavConRegProxy.cc,v 1.6 2002/03/25 11:54:47 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavConRegProxy::NavConRegProxy() :
   m_ptrTable ( new value_type ) ,
   m_server   ( 0              )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavConRegProxy::boundMethod, Stream::kBeginRun );
}

// NavConRegProxy::NavConRegProxy( const NavConRegProxy& iproxy )
// {
//   *this = iproxy;
// }

NavConRegProxy::~NavConRegProxy()
{
   delete m_server ;
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const NavConRegProxy& NavConRegProxy::operator=( const NavConRegProxy& iproxy )
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
NavConRegProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) 
   {
      (*m_ptrTable).eraseAll() ;
   }
   delete m_server ;
   m_server = 0 ;
}

const NavConRegProxy::value_type*
NavConRegProxy::faultHandler( const Record&  iRecord ,
			      const DataKey& iKey      )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   assert( 0 == m_server ) ;

   FATable< NavShower > navShowers ;
   extract( iRecord, navShowers ) ;

   FATable< CcConRegAttributes > conRegs ;
   extract( iRecord,             conRegs ) ;

   // -----------------------------------------------------------------------

   FAItem< CcConRegShowerLattice > lattice ;
   extract( iRecord, lattice ) ; 

//   const CcConRegShowerLattice::VectorLeftID& lefts ( (*lattice).vLeftID() );

   m_server = new NavConRegServer( iRecord ) ;
   assert( 0 != m_server ) ;

   FATable< CcConRegAttributes >::const_iterator cEnd ( conRegs.end() ) ;
   for( FATable< CcConRegAttributes >::const_iterator iCR ( conRegs.begin() ) ;
	iCR != cEnd ; ++iCR )
   {
      NavConReg* tmp( new NavConReg( iCR->identifier(),
				     *lattice ,
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
//NavConRegProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kReport) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
//#include "ProxyBind/Template/ProxyBindableTemplate.cc"
//template class ProxyBindableTemplate< FAPtrTable< NavConReg > >;

//PROXY_BIND_METHOD( NavConRegProxy )
//-----------------------------------------------


