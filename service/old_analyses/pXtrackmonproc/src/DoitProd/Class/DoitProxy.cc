// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 14:43:57 EDT 1999
// $Id: DoitProxy.cc,v 1.9 2001/09/13 20:54:22 cdj Exp $
//
// Revision history
//
// $Log: DoitProxy.cc,v $
// Revision 1.9  2001/09/13 20:54:22  cdj
// use new SVHybridEfficiency stream
//
// Revision 1.8  2001/04/05 02:02:54  lyon
// Add new stop on kSVEfficiency stream
//
// Revision 1.7  2000/10/11 20:02:32  lyon
// Set the doit Mode number before begin run
//
// Revision 1.6  2000/06/09 00:43:55  lkg
// pass useSilicon status from parameter to DoitTrackFinder
//
// Revision 1.5  2000/02/08 18:37:36  lyon
// Added DoitModeNumber parameter
//
// Revision 1.4  2000/01/15 20:18:13  lyon
// include set
//
// Revision 1.3  2000/01/14 02:54:32  lyon
// Add parameters to continue if hits are missing
//
// Revision 1.2  1999/10/31 16:11:11  lyon
// Fixes to call clearCache
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
#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "C3sv/SiStream.h"

#include "DoitProd/DoitProd.h"

#include "DoitProd/DoitProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD


// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DoitProd.DoitProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitProxy.cc,v 1.9 2001/09/13 20:54:22 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitProxy::DoitProxy( DoitProd* aDoitProd )
   : m_doitProd( aDoitProd ),
     m_doitTrackFinder( 0 ),   // will be a pointer to the below
     m_theDoitTrackFinder() 
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   bind( &DoitProxy::beginRun, Stream::kBeginRun );
   bind( &DoitProxy::svEfficiencyConstants, SiStream::kSVHybridEfficiency );
}

// DoitProxy::DoitProxy( const DoitProxy& iproxy )
// {
//   *this = iproxy;
// }

DoitProxy::~DoitProxy()
{
  m_doitTrackFinder = 0;
  // m_theDoitTrackFinder is automatically deleted
}

//
// assignment operators
//
// const DoitProxy& DoitProxy::operator=( const DoitProxy& iproxy )
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
DoitProxy::invalidateCache()
{
  // when m_doitTrackFinder == 0, the fault handler will run and
  // Doit will be called
  
  // DO NOT DELETE m_doitTrackFinder -- just zero the pointer
  m_doitTrackFinder = 0 ;

  // Tell Doit to clear its cache
  // !!! All Doit data is deleted here by this proxy. This is done instead
  // of the individual data proxies so that all data is deleted, regardless
  // of whether or not the proxy was called. Otherwise, you'll have a big
  // honkin memory leak.
  m_theDoitTrackFinder.clearCache();
  
}

const DoitProxy::value_type*
DoitProxy::faultHandler( const Record& iRecord,
			 const DataKey& iKey )
{
  // ---------------- Sample implementation -------------------
  
  // be assertive: there should be no cached data at this point, else
  // it's a programming error
  assert( 0 == m_doitTrackFinder );

  // ---------- create the new object --------------------
  //report(DEBUG, kFacilityString) << "Running DoitProxy Fault Handler" << endl;

  int diagLevel = 0;
  // Get the parameter information
  if ( m_doitProd )
  {
    diagLevel = m_doitProd->diagLevel();

    m_theDoitTrackFinder.setModeNumber(m_doitProd->modeNumber());

    m_theDoitTrackFinder.setExceptionContinuationFlags(
		     m_doitProd->continueIfHitsMissing(),
		     m_doitProd->continueIfAnyException() );
    
    m_theDoitTrackFinder.setSiliconUsage( m_doitProd->useSilicon() );
  }

  // Call the event loop with the diagnostics level
  DABoolean success = m_theDoitTrackFinder.event(iRecord.frame(), 
						 diagLevel);

  if ( true == success )
  {
    // Get the track information out of Fortran
    m_theDoitTrackFinder.getTracksFromFortran(iRecord.frame(), diagLevel);
    
    // m_theDoitTrackFinder holds good values - send it back to the
    // user
    m_doitTrackFinder = &m_theDoitTrackFinder;
  }
  else
  {
    report(WARNING, kFacilityString) << "DoitTrackFinder failed" << endl;
    m_doitTrackFinder = 0;
  }

  return m_doitTrackFinder;
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

void DoitProxy::beginRun( const Record& iRecord )
{
  report( DEBUG, kFacilityString) <<"BoundMethod beginrun: " 
				  << iRecord.stream().value() << endl;

  // Set the mode number
  m_theDoitTrackFinder.setModeNumber(m_doitProd->modeNumber());

  // Call beginRun stuff on the doitTrackFinder
  m_theDoitTrackFinder.beginRun(iRecord.frame());

}

void DoitProxy::svEfficiencyConstants(const Record& iRecord )
{
  m_theDoitTrackFinder.fillSVEfficiencyConstants(iRecord);
}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< DoitTrackFinder  >;

PROXY_BIND_METHOD( DoitProxy )
//-----------------------------------------------

