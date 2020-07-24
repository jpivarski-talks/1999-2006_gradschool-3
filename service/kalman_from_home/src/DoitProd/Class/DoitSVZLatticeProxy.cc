// -*- C++ -*-
//
// DoitProd:     <DoitProd>
// Module:      DoitSVRLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Peter Gaidarev
// Created:     Tue May 30 11:06:12 EDT 2000
// $Id: DoitSVZLatticeProxy.cc,v 1.1 2000/05/30 20:45:19 pg Exp $
//
// Revision history
//
// $Log: DoitSVZLatticeProxy.cc,v $
// Revision 1.1  2000/05/30 20:45:19  pg
// Added proxies for Si lattices
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

#include "DoitProd/DoitSVZLatticeProxy.h"
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
static const char* const kFacilityString = "DoitProd.DoitSVZLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitSVZLatticeProxy.cc,v 1.1 2000/05/30 20:45:19 pg Exp $";
static const char* const kTagString = "$Name: v01_10_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitSVZLatticeProxy::DoitSVZLatticeProxy()
   : m_DoitSVZLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DoitSVZLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DoitSVZLatticeProxy::DoitSVZLatticeProxy( const DoitSVZLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DoitSVZLatticeProxy::~DoitSVZLatticeProxy()
{
  // Do not delete the lattice, DoitProxy does this

}

//
// assignment operators
//
// const DoitSVZLatticeProxy& DoitSVZLatticeProxy::operator=( const DoitSVZLatticeProxy& iproxy )
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
DoitSVZLatticeProxy::invalidateCache()
{
   m_DoitSVZLattice = 0 ;
}

const DoitSVZLatticeProxy::value_type*
DoitSVZLatticeProxy::faultHandler( const Record& iRecord,
				   const DataKey& iKey )
{
// ---------------- Sample implementation -------------------

   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_DoitSVZLattice );

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
   m_DoitSVZLattice = trackFinder->getSVZLattice();
    
   return ( m_DoitSVZLattice ) ;
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
//DoitSVZLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< SeedTrackSVZHitLattice >;

PROXY_BIND_METHOD( DoitSVZLatticeProxy )
//-----------------------------------------------
