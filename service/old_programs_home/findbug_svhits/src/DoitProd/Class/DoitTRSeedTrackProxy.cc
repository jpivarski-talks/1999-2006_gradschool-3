// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitTRSeedTrackProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 15:44:40 EDT 1999
// $Id: DoitTRSeedTrackProxy.cc,v 1.4 2000/02/14 19:21:47 lyon Exp $
//
// Revision history
//
// $Log: DoitTRSeedTrackProxy.cc,v $
// Revision 1.4  2000/02/14 19:21:47  lyon
// Propagate Usage and Production tags
//
// Revision 1.3  1999/10/31 16:11:13  lyon
// Fixes to call clearCache
//
// Revision 1.2  1999/10/07 17:44:59  lyon
// Added some handy DEBUB printouts
//
// Revision 1.1.1.1  1999/09/08 18:53:33  lyon
// imported DoitProd sources
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

#include "DoitProd/DoitTRSeedTrackProxy.h"
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
static const char* const kFacilityString = "DoitProd.DoitTRSeedTrackProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitTRSeedTrackProxy.cc,v 1.4 2000/02/14 19:21:47 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitTRSeedTrackProxy::DoitTRSeedTrackProxy()
   : m_ptrTable( 0 )
{}

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DoitTRSeedTrackProxy::boundMethod, Stream::kBeginRun );


// DoitTRSeedTrackProxy::DoitTRSeedTrackProxy( const DoitTRSeedTrackProxy& iproxy )
// {
//   *this = iproxy;
// }

DoitTRSeedTrackProxy::~DoitTRSeedTrackProxy()
{
  // Do not delete m_ptrTable - soloProxy takes care of this
}

//
// assignment operators
//
// const DoitTRSeedTrackProxy& DoitTRSeedTrackProxy::operator=( const DoitTRSeedTrackProxy& iproxy )
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
DoitTRSeedTrackProxy::invalidateCache()
{
  // Do not delete m_ptrTable - soloProxy takes care of this
  m_ptrTable = 0;
}

const DoitTRSeedTrackProxy::value_type*
DoitTRSeedTrackProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
// ---------- sample implementation ---------------------

   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == m_ptrTable );

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

   // Get the pointer table of the found tracks
   m_ptrTable = trackFinder->getSeedTracks();

   return m_ptrTable;
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
//DoitTRSeedTrackProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< TRSeedTrack > >;

PROXY_BIND_METHOD( DoitTRSeedTrackProxy )
//-----------------------------------------------


