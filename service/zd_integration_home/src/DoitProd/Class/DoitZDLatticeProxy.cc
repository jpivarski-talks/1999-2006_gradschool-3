// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitZDLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Mon Aug 23 16:17:07 EDT 1999
// $Id: DoitZDLatticeProxy.cc,v 1.2 2003/04/16 18:12:38 bkh Exp $
//
// Revision history
//
// $Log: DoitZDLatticeProxy.cc,v $
// Revision 1.2  2003/04/16 18:12:38  bkh
// Throw exceptions for ZD if SI-mode and vice versa
//
// Revision 1.1  2002/09/27 21:04:16  bkh
// For ZD capability
//
// Revision 1.4  2000/02/25 16:23:09  ajm36
// changed extract of trackFinder to allow use of production tags
//
// Revision 1.3  1999/10/31 16:11:09  lyon
// Fixes to call clearCache
//
// Revision 1.2  1999/10/07 17:44:58  lyon
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
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "DoitProd/DoitZDLatticeProxy.h"
#include "Doit/DoitTrackFinder.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD


#include "DetectorConfiguration/DetectorConfiguration.h"
#include "DAException/DANoDataException.h"

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
static const char* const kFacilityString = "DoitProd.DoitZDLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitZDLatticeProxy.cc,v 1.2 2003/04/16 18:12:38 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitZDLatticeProxy::DoitZDLatticeProxy()
   : m_DoitZDLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DoitZDLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DoitZDLatticeProxy::DoitZDLatticeProxy( const DoitZDLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DoitZDLatticeProxy::~DoitZDLatticeProxy()
{
  // Do not delete the lattice - DoitProxy does this
}

//
// assignment operators
//
// const DoitZDLatticeProxy& DoitZDLatticeProxy::operator=( const DoitZDLatticeProxy& iproxy )
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
DoitZDLatticeProxy::invalidateCache()
{
  // Do not delete the lattice - DoitProxy does this
  m_DoitZDLattice = 0 ;
}

const DoitZDLatticeProxy::value_type*
DoitZDLatticeProxy::faultHandler( const Record& iRecord,
				  const DataKey& iKey )
{
   FAItem<DetectorConfiguration>                              iConfig   ;
   extract( iRecord.frame().record( Stream::kBaseGeometry ) , iConfig ) ;
   if( iConfig->useSInotZD() )
   {
      throw DANoDataException< value_type >( iRecord.stream(),iKey ) ;
   }
// ---------------- Sample implementation -------------------
  // be assertive: there should be no cached data at this point, else
  // it's a programming error
  assert( 0 == m_DoitZDLattice );
  
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
  m_DoitZDLattice = trackFinder->getZDLattice();
  
  return ( m_DoitZDLattice ) ;
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
//DoitZDLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< SeedTrackZDHitLattice >;

PROXY_BIND_METHOD( DoitZDLatticeProxy )
//-----------------------------------------------

