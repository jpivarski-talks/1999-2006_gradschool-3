// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DualTrackToUsageTagProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Dec 15 14:41:35 EST 2003
// $Id: DualTrackToUsageTagProd.cc,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $
//
// Revision history
//
// $Log: DualTrackToUsageTagProd.cc,v $
// Revision 1.1.1.1  2003/12/17 21:00:55  mccann
// imported DualTrackToUsageTagProd sources
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DualTrackToUsageTagProd/DualTrackToUsageTagProd.h"
#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

#include "DualTrackToUsageTagProd/DTUTTRHelixElectronFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixMuonFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixPionFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixKaonFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixProtonFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixExitElectronFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixExitMuonFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixExitPionFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixExitKaonFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTTRHelixExitProtonFitProxy.h"
#include "DualTrackToUsageTagProd/DTUTElectronFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTMuonFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTPionFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTKaonFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTProtonFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitElectronFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitMuonFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitPionFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitKaonFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitProtonFitDRHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTElectronFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTMuonFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTPionFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTKaonFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTProtonFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitElectronFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitMuonFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitPionFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitKaonFitZDHitLatticeProxy.h"
#include "DualTrackToUsageTagProd/DTUTExitProtonFitZDHitLatticeProxy.h"

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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DualTrackToUsageTagProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackToUsageTagProd.cc,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackToUsageTagProd::DualTrackToUsageTagProd( void )               // anal1
   : Producer( "DualTrackToUsageTagProd" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // Proxy Registration Step 1:
   // either
   // register default-ctor proxy
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixElectronFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixMuonFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixPionFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixKaonFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixProtonFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixExitElectronFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixExitMuonFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixExitPionFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixExitKaonFitProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTTRHelixExitProtonFitProxy>, UsageTag("DualTrack"));

   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTElectronFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTMuonFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTPionFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTKaonFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTProtonFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitElectronFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitMuonFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitPionFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitKaonFitDRHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitProtonFitDRHitLatticeProxy>, UsageTag("DualTrack"));

   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTElectronFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTMuonFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTPionFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTKaonFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTProtonFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitElectronFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitMuonFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitPionFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitKaonFitZDHitLatticeProxy>, UsageTag("DualTrack"));
   registerProxyFactory(Stream::kEvent, new ProxyFactory<DTUTExitProtonFitZDHitLatticeProxy>, UsageTag("DualTrack"));
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"

template class ProxyFactory<DTUTTRHelixElectronFitProxy>;
template class ProxyFactory<DTUTTRHelixMuonFitProxy>;
template class ProxyFactory<DTUTTRHelixPionFitProxy>;
template class ProxyFactory<DTUTTRHelixKaonFitProxy>;
template class ProxyFactory<DTUTTRHelixProtonFitProxy>;
template class ProxyFactory<DTUTTRHelixExitElectronFitProxy>;
template class ProxyFactory<DTUTTRHelixExitMuonFitProxy>;
template class ProxyFactory<DTUTTRHelixExitPionFitProxy>;
template class ProxyFactory<DTUTTRHelixExitKaonFitProxy>;
template class ProxyFactory<DTUTTRHelixExitProtonFitProxy>;

template class ProxyFactory<DTUTElectronFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTMuonFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTPionFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTKaonFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTProtonFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTExitElectronFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTExitMuonFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTExitPionFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTExitKaonFitDRHitLatticeProxy>;
template class ProxyFactory<DTUTExitProtonFitDRHitLatticeProxy>;

template class ProxyFactory<DTUTElectronFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTMuonFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTPionFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTKaonFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTProtonFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTExitElectronFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTExitMuonFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTExitPionFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTExitKaonFitZDHitLatticeProxy>;
template class ProxyFactory<DTUTExitProtonFitZDHitLatticeProxy>;

// DualTrackToUsageTagProd::DualTrackToUsageTagProd( const DualTrackToUsageTagProd& iDualTrackToUsageTagProd )
// {
//    *this = iDualTrackToUsageTagProd;
// }

DualTrackToUsageTagProd::~DualTrackToUsageTagProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const DualTrackToUsageTagProd& DualTrackToUsageTagProd::operator=( const DualTrackToUsageTagProd& iDualTrackToUsageTagProd )
// {
//   if( this != &iDualTrackToUsageTagProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iDualTrackToUsageTagProd );
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
DualTrackToUsageTagProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DualTrackToUsageTagProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)

}

// ---------------- standard place to book histograms ---------------
void
DualTrackToUsageTagProd::hist_book( HIHistoManager& )
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
