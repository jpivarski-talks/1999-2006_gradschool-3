// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitCathodeLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Mon May 22 11:43:28 EDT 2000
// $Id: DoitCathodeLatticeProxy.cc,v 1.1 2000/05/27 05:31:27 lyon Exp $
//
// Revision history
//
// $Log: DoitCathodeLatticeProxy.cc,v $
// Revision 1.1  2000/05/27 05:31:27  lyon
// Handle cathode clusters on tracks
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

#include "DoitProd/DoitCathodeLatticeProxy.h"
#include "Doit/DoitTrackFinder.h"
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
static const char* const kFacilityString = "DoitProd.DoitCathodeLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitCathodeLatticeProxy.cc,v 1.1 2000/05/27 05:31:27 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitCathodeLatticeProxy::DoitCathodeLatticeProxy()
   : m_DoitCathodeLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DoitCathodeLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DoitCathodeLatticeProxy::DoitCathodeLatticeProxy( const DoitCathodeLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DoitCathodeLatticeProxy::~DoitCathodeLatticeProxy()
{
  // Do not delete the lattice, DoitProxy does this
}

//
// assignment operators
//
// const DoitCathodeLatticeProxy& DoitCathodeLatticeProxy::operator=( const DoitCathodeLatticeProxy& iproxy )
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
DoitCathodeLatticeProxy::invalidateCache()
{
  // Do not delete the lattice - DoitProxy does this
   m_DoitCathodeLattice = 0 ;
}

const DoitCathodeLatticeProxy::value_type*
DoitCathodeLatticeProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
// ---------------- Sample implementation -------------------

  // be assertive: there should be no cached data at this point, else
  // it's a programming error
  assert( 0 == m_DoitCathodeLattice );
  
  // ---------- create the new object --------------------
  
  // Extract the Doit Track Finder
  FAItem< DoitTrackFinder > trackFinder;
  extract(iRecord, trackFinder, iKey.usage().value(),
	  iKey.production().value());
  
  // Is it valid?
  if ( !trackFinder.valid() )
  {
    report(WARNING, kFacilityString) << "Could not extract DoitTrackFinder"
				     << endl;
    return 0;
  }
  
  // Get the lattice
  m_DoitCathodeLattice = trackFinder->getCathodeLattice();
  
  return ( m_DoitCathodeLattice ) ;
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
//DoitCathodeLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< SeedTrackCathodeHitLattice >;

PROXY_BIND_METHOD( DoitCathodeLatticeProxy )
//-----------------------------------------------

