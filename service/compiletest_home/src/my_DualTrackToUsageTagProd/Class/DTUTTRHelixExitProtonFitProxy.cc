// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTTRHelixExitProtonFitProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:39:50 EST 2003
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

#include "DualTrackToUsageTagProd/DTUTTRHelixExitProtonFitProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "DualTrackProd/DualTrackFitHelix.h"

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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTTRHelixExitProtonFitProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: proxymany.cc,v 1.14 2000/12/04 19:11:16 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTTRHelixExitProtonFitProxy::DTUTTRHelixExitProtonFitProxy()
   : m_ptrTable( new value_type )
{

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTTRHelixExitProtonFitProxy::boundMethod, Stream::kBeginRun );
}

// DTUTTRHelixExitProtonFitProxy::DTUTTRHelixExitProtonFitProxy( const DTUTTRHelixExitProtonFitProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTTRHelixExitProtonFitProxy::~DTUTTRHelixExitProtonFitProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const DTUTTRHelixExitProtonFitProxy& DTUTTRHelixExitProtonFitProxy::operator=( const DTUTTRHelixExitProtonFitProxy& iproxy )
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
DTUTTRHelixExitProtonFitProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

const DTUTTRHelixExitProtonFitProxy::value_type*
DTUTTRHelixExitProtonFitProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ptrTable->size() == 0);
   FAAutoEraseAllPtr<TRHelixExitProtonFit> ptrTable(m_ptrTable);

   FAItem<DualTrack> dual;
   extract(iRecord, dual);
   if (!dual->results().fitSuccessful()) return ptrTable.release();

   FATable<DualTrackFitHelix> tracks;
   extract(iRecord, tracks);
   
   for (FATable<DualTrackFitHelix>::const_iterator track_iter = tracks.begin();
	track_iter != tracks.end();
	++track_iter) {
      ptrTable->insert(new TRHelixExitProtonFit(track_iter->identifier(), *track_iter));
   }
   return ptrTable.release();
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
//DTUTTRHelixExitProtonFitProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< TRHelixExitProtonFit > >;

PROXY_BIND_METHOD( DTUTTRHelixExitProtonFitProxy )
//-----------------------------------------------


