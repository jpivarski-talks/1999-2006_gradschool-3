// -*- C++ -*-
//
// Package:     <FakeATrackProd>
// Module:      FakeATrackProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jul 11 16:44:13 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "FakeATrackProd/FakeATrackProd.h"
#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

#include "TrackRoot/TRSeedTrack.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "TrackFinder/SeedZDHitLink.h"

#include "FakeATrackProd/TRSeedTrackProxy.h"
#include "FakeATrackProd/CalibratedDRHitProxy.h"
#include "FakeATrackProd/CalibratedZDHitProxy.h"
#include "FakeATrackProd/SeedTrackDRHitLatticeProxy.h"
#include "FakeATrackProd/SeedTrackZDHitLatticeProxy.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "FakeATrackProd.FakeATrackProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: producer.cc,v 1.19 2000/12/04 19:11:13 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
FakeATrackProd::FakeATrackProd( void )               // anal1
   : Producer( "FakeATrackProd" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   registerProxyFactory(Stream::kEvent,
			new ProxyFactory<TRSeedTrackProxy>,
			UsageTag("fake-o"));
   registerProxyFactory(Stream::kEvent,
			new ProxyFactory<CalibratedDRHitProxy>,
			UsageTag("fake-o"));
   registerProxyFactory(Stream::kEvent,
			new ProxyFactory<CalibratedZDHitProxy>,
			UsageTag("fake-o"));
   registerProxyFactory(Stream::kEvent,
			new ProxyFactory<SeedTrackDRHitLatticeProxy>,
			UsageTag("fake-o"));
   registerProxyFactory(Stream::kEvent,
			new ProxyFactory<SeedTrackZDHitLatticeProxy>,
			UsageTag("fake-o"));


//    registerProxyFactory( Stream::kEvent, 
// 			 new ProxyFactory< MyProxy >, 
// 			 UsageTag() );    // or UsageTag( "MyUsage" )
//    registerProxyFactory( 
//       Stream::kEvent, 
//       new ProxyFactoryFunction< FakeATrackProd, MyProxy >( *this, 
// 						     &FakeATrackProd::makeMyProxy ),
//       UsageTag() );    // or UsageTag( "MyUsage" )

}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory<TRSeedTrackProxy>;
template class ProxyFactory<CalibratedDRHitProxy>;
template class ProxyFactory<CalibratedZDHitProxy>;
template class ProxyFactory<SeedTrackDRHitLatticeProxy>;
template class ProxyFactory<SeedTrackZDHitLatticeProxy>;
#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction<FakeATrackProd, TRSeedTrackProxy>;
template class ProxyFactoryFunction<FakeATrackProd, CalibratedDRHitProxy>;
template class ProxyFactoryFunction<FakeATrackProd, CalibratedZDHitProxy>;
template class ProxyFactoryFunction<FakeATrackProd, SeedTrackDRHitLatticeProxy>;
template class ProxyFactoryFunction<FakeATrackProd, SeedTrackZDHitLatticeProxy>;

// non-trivial proxyfactory producer method used above
// (if you want to use it, don't forget to declare this method 
//  in "producer.h"!)
#if 0
ProxyBase*
FakeATrackProd::makeMyProxy()
{ 
   return new MyProxy( "pass me something" );
}
#endif

// FakeATrackProd::FakeATrackProd( const FakeATrackProd& iFakeATrackProd )
// {
//    *this = iFakeATrackProd;
// }

FakeATrackProd::~FakeATrackProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const FakeATrackProd& FakeATrackProd::operator=( const FakeATrackProd& iFakeATrackProd )
// {
//   if( this != &iFakeATrackProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iFakeATrackProd );
//   }
//
//   return *this;
// }

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
FakeATrackProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
FakeATrackProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
FakeATrackProd::hist_book( HIHistoManager& )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

//
// const member functions
//

//
// static member functions
//
