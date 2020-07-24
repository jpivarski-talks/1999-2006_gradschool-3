// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      NavKsProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      David Urner
// Created:     Tue Mar 20 14:41:54 EST 2001
// $Id: NavKsProxy.cc,v 1.2 2002/05/06 17:45:07 hajime Exp $
//
// Revision history
//
// $Log: NavKsProxy.cc,v $
// Revision 1.2  2002/05/06 17:45:07  hajime
// changes for new VXFitVeeCandidate library
//
// Revision 1.1  2001/03/26 20:15:14  urner
// added Nav Ks
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

#include "NavigationProd/NavKsProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD
#include "VXFitVeeCandidate/VXFitVeeKShort.h"
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
static const char* const kFacilityString = "Navigation.NavKsProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: NavKsProxy.cc,v 1.2 2002/05/06 17:45:07 hajime Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
NavKsProxy::NavKsProxy()
   : m_ptrTable( new value_type )
{

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &NavKsProxy::boundMethod, Stream::kBeginRun );
}

// NavKsProxy::NavKsProxy( const NavKsProxy& iproxy )
// {
//   *this = iproxy;
// }

NavKsProxy::~NavKsProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const NavKsProxy& NavKsProxy::operator=( const NavKsProxy& iproxy )
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
NavKsProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

const NavKsProxy::value_type*
NavKsProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{

   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   //If an exception is thrown in this routine, m_ptrTable will be
   // automatically emptied (which is the proper behavior).
   // Use ptrTable just like you would use m_ptrTable
   FAAutoEraseAllPtr<NavKs> ptrTable( m_ptrTable );

   // ---------- sample implementation ---------------------

   // ---------- fill FAPtrTable with new items ----------------


   typedef FATable< VXFitVeeKShort > VXFitVeeKShortTable;

   VXFitVeeKShortTable vXFitKShorts;
   extract ( iRecord, vXFitKShorts );


   // this typedef here for convenience
   typedef FATable< NavTrack > NavTrackTable;

   // get the table out of the Record
   NavTrackTable navTracks;
   extract( iRecord, navTracks ); 


   //create NavKs from NavTracks
   VXFitVeeKShortTable::const_iterator vXFitKShortsEnd( vXFitKShorts.end() );
   for ( VXFitVeeKShortTable::const_iterator itKs( vXFitKShorts.begin() ) ;
         itKs != vXFitKShortsEnd ;
         ++itKs ) 
     {
       const VXFitVeeKShort& kshort ( *itKs) ;
       NavKs* tmp (new NavKs( &kshort                         ,
			      &navTracks[ kshort.piPlusId() ] ,
			      &navTracks[ kshort.piMinusId() ] ) ) ;
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
//NavKsProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< NavKs > >;

PROXY_BIND_METHOD( NavKsProxy )
//-----------------------------------------------


