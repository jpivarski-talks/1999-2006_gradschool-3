// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannTRSeedTrackProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 17:28:09 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
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

#include "TrackRoot/TRHelixPionFit.h"
#include "McCannProd/McCannTRSeedTrackProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

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
static const char* const kFacilityString = "McCannProd.McCannTRSeedTrackProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: proxymany.cc,v 1.14 2000/12/04 19:11:16 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
McCannTRSeedTrackProxy::McCannTRSeedTrackProxy()
   : m_ptrTable( new value_type )
{

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &McCannTRSeedTrackProxy::boundMethod, Stream::kBeginRun );
}

// McCannTRSeedTrackProxy::McCannTRSeedTrackProxy( const McCannTRSeedTrackProxy& iproxy )
// {
//   *this = iproxy;
// }

McCannTRSeedTrackProxy::~McCannTRSeedTrackProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const McCannTRSeedTrackProxy& McCannTRSeedTrackProxy::operator=( const McCannTRSeedTrackProxy& iproxy )
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
McCannTRSeedTrackProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

const McCannTRSeedTrackProxy::value_type*
McCannTRSeedTrackProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{

   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   //If an exception is thrown in this routine, m_ptrTable will be
   // automatically emptied (which is the proper behavior).
   // Use ptrTable just like you would use m_ptrTable
   FAAutoEraseAllPtr<TRSeedTrack> ptrTable( m_ptrTable );

   FATable<TRHelixPionFit> mtracks;
   extract(iRecord, mtracks, "McCann");
   FATable<TRHelixPionFit>::const_iterator mtrack_iter = mtracks.begin();
   FATable<TRHelixPionFit>::const_iterator mtrack_end = mtracks.end();
   for (; mtrack_iter != mtrack_end;  ++mtrack_iter) {
      m_ptrTable->insert( new TRSeedTrack(mtrack_iter->identifier(),
					  mtrack_iter->curvature(),
					  mtrack_iter->phi0(),
					  mtrack_iter->d0(),
					  mtrack_iter->cotTheta(),
					  mtrack_iter->z0()) );
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
//McCannTRSeedTrackProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< TRSeedTrack > >;

PROXY_BIND_METHOD( McCannTRSeedTrackProxy )
//-----------------------------------------------


