// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackFitHelixProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:18:33 EST 2003
// $Id: DualTrackFitHelixProxy.cc,v 1.1 2003/12/15 17:53:10 mccann Exp $
//
// Revision history
//
// $Log: DualTrackFitHelixProxy.cc,v $
// Revision 1.1  2003/12/15 17:53:10  mccann
// adding hit lattices for dual-constrained residuals
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

#include "DualTrackProd/DualTrackFitHelixProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h" 

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
static const char* const kFacilityString = "DualTrackProd.DualTrackFitHelixProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackFitHelixProxy.cc,v 1.1 2003/12/15 17:53:10 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackFitHelixProxy::DualTrackFitHelixProxy()
   : m_ptrTable( new value_type )
{

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DualTrackFitHelixProxy::boundMethod, Stream::kBeginRun );
}

// DualTrackFitHelixProxy::DualTrackFitHelixProxy( const DualTrackFitHelixProxy& iproxy )
// {
//   *this = iproxy;
// }

DualTrackFitHelixProxy::~DualTrackFitHelixProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const DualTrackFitHelixProxy& DualTrackFitHelixProxy::operator=( const DualTrackFitHelixProxy& iproxy )
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
DualTrackFitHelixProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

const DualTrackFitHelixProxy::value_type*
DualTrackFitHelixProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
   assert((*m_ptrTable).size() == 0);
   FAAutoEraseAllPtr<DualTrackFitHelix> ptrTable(m_ptrTable);

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   if (dual->results().fitSuccessful()) {
      ptrTable->insert(new DualTrackFitHelix(dual->positive()));
      ptrTable->insert(new DualTrackFitHelix(dual->negative()));
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
//DualTrackFitHelixProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< DualTrackFitHelix > >;

PROXY_BIND_METHOD( DualTrackFitHelixProxy )
//-----------------------------------------------


