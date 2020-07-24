// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      NavLambdaProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Eric Eckhart
// Created:     Tue Mar 20 14:41:54 EST 2001
// $Id: NavLambdaProxy.cc,v 1.2 2002/05/06 17:45:08 hajime Exp $
//
// Revision history
//
// $Log: NavLambdaProxy.cc,v $
// Revision 1.2  2002/05/06 17:45:08  hajime
// changes for new VXFitVeeCandidate library
//
// Revision 1.1  2001/09/07 23:11:42  eckhart
// Created File
//
// Revision 1.1  2001/03/26 20:15:14  eckhart
// added Nav Lambda
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"
#include "FrameAccess/FAAutoEraseAllPtr.h"

#include "NavigationProd/NavLambdaProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD
#include "VXFitVeeCandidate/VXFitVeeLambda.h"
#include "Navigation/NavTrack.h"


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
static const char* const kFacilityString = "Navigation.NavLambdaProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavLambdaProxy.cc,v 1.2 2002/05/06 17:45:08 hajime Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavLambdaProxy::NavLambdaProxy()
   : m_ptrTable( new value_type )
{

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavLambdaProxy::boundMethod, Stream::kBeginRun );
}

// NavLambdaProxy::NavLambdaProxy( const NavLambdaProxy& iproxy )
// {
//   *this = iproxy;
// }

NavLambdaProxy::~NavLambdaProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const NavLambdaProxy& NavLambdaProxy::operator=( const NavLambdaProxy& iproxy )
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
NavLambdaProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

const NavLambdaProxy::value_type*
NavLambdaProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{

   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   //If an exception is thrown in this routine, m_ptrTable will be
   // automatically emptied (which is the proper behavior).
   // Use ptrTable just like you would use m_ptrTable
   FAAutoEraseAllPtr<NavLambda> ptrTable( m_ptrTable );

   // ---------- sample implementation ---------------------

   // ---------- fill FAPtrTable with new items ----------------


   typedef FATable< VXFitVeeLambda > VXFitVeeLambdaTable;

   VXFitVeeLambdaTable vXFitLambdas;
   extract ( iRecord, vXFitLambdas );


   // this typedef here for convenience
   typedef FATable< NavTrack > NavTrackTable;

   // get the table out of the Record
   NavTrackTable navTracks;
   extract( iRecord, navTracks ); 


   //create NavLambda from NavTracks
   VXFitVeeLambdaTable::const_iterator vXFitLambdasEnd( vXFitLambdas.end() );
   for ( VXFitVeeLambdaTable::const_iterator itLambda( vXFitLambdas.begin() ) ;
         itLambda != vXFitLambdasEnd ;
         ++itLambda ) 
     {
       const VXFitVeeLambda& Lambda ( *itLambda) ;
       NavLambda* tmp (new NavLambda( &Lambda                         ,
			      &navTracks[ Lambda.protonId() ] ,
			      &navTracks[ Lambda.pionId() ] ) ) ;
       ptrTable->insert( tmp ) ;
     }


   //The release returns m_ptrTable and stops ptrTable from erasing the 
   // contents of m_ptrTable.
   return ( ptrTable.release() ) ;
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
//NavLambdaProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< NavLambda > >;

PROXY_BIND_METHOD( NavLambdaProxy )
//-----------------------------------------------


