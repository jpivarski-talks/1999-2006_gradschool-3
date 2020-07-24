// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavigationProd
// 
// Description: Producer providing proxies for navigation objects.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:20:22 EDT 1998
// $Id: NavigationProd.cc,v 1.9 2001/09/10 18:46:05 ajm36 Exp $
//
// Revision history
//
// $Log: NavigationProd.cc,v $
// Revision 1.9  2001/09/10 18:46:05  ajm36
// register NavLambda proxy
//
// Revision 1.8  2001/05/10 19:56:40  bkh
// Addition of etaPrime classes
//
// Revision 1.7  2001/03/26 20:15:14  urner
// added Nav Ks
//
// Revision 1.6  2000/10/03 15:47:29  lkg
// tweaks for cxx compatibility
//
// Revision 1.5  2000/09/26 21:28:33  lkg
// An partial implementation of specification of seed track and fit track
// usage and production tags.  For track-only use, the implementation is
// complete.  No effort has been made to enforce the same track tags
// for clusters/rich/muons/etc (or to allow specification of
// prod/usage tag information for that information).
// * add parameters to NavigationProd for specification of the finder and
// fitter usage and production tags
// * add accessor functions to NavigationProd to access the tags
// * pass a pointer to (the calling) NavigationProd to the NavTrackProxy
// constructor, so it may access the tag information
// * create the NavTrack master block using the prod/usage tag information
//
// Revision 1.4  2000/07/27 01:35:19  ts
// NavRich added
//
// Revision 1.3  1999/08/09 16:27:21  bkh
// Renaming and addition of pi0, eta proxies
//
// Revision 1.2  1999/08/05 20:29:02  bkh
// new eta proxy and renamed pi0 proxy
//
// Revision 1.1.1.1  1999/07/19 16:47:46  bkh
// imported NavigationProd sources
//
// Revision 1.8  1999/06/29 21:14:55  bkh
// Add Proxies for high level shower object, connected regions, matches
//
// Revision 1.7  1999/05/17 22:24:23  wsun
// Removed all registering and instantiation of quality object proxies.
//
// Revision 1.6  1999/05/07 19:08:14  marsh
// Added remaining outgoing hypotheses.
//
// Revision 1.5  1998/12/31 18:12:23  marsh
// NavTrack and related files have been moved to new library Navigation.
//
// Revision 1.4  1998/12/19 21:38:31  marsh
// Fixed statements for new proxy system.
//
// Revision 1.3  1998/12/19 21:23:53  marsh
// Removed registerProxies() and moved proxy creation to ctor() using
// ProxyFactory<> system.  Added proxies to convert fitter-specific
// track quality objects to generic TRTrackCandidateFitQuality objects.
//
// Revision 1.2  1998/10/21 14:55:13  marsh
// * changed to new proxy system
// * added typename for egcs
//
// Revision 1.1.1.1  1998/07/13 23:26:20  marsh
// New library for user-level analysis objects
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "NavigationProd/NavigationProd.h"
#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

#include "Navigation/NavTrack.h"
#include "NavigationProd/NavTrackProxy.h"
#include "NavigationProd/NavShowerProxy.h"
#include "NavigationProd/NavPi0ToGGProxy.h"
#include "NavigationProd/NavEtaToGGProxy.h"
#include "NavigationProd/NavEtaPrimeToGGProxy.h"
#include "NavigationProd/NavKsProxy.h"
#include "NavigationProd/NavLambdaProxy.h"
#include "NavigationProd/NavConRegProxy.h"
#include "NavigationProd/NavTkShMatchProxy.h"
// #include "NavigationProd/AETrackCandidateFitQualityProxy.h"
#include "NavigationProd/NavRichProxy.h"

// STL classes

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Producer.NavigationProd" ;

//
// static data member definitions
//

//
// constructors and destructor
//
NavigationProd::NavigationProd( void )               // anal1
   : Producer( "NavigationProd" ),
    m_fitterUsageTag("fitterUsageTag", this, UsageTag().value() ),
    m_fitterProductionTag("fitterProductionTag", this, ProductionTag().value() ),
    m_finderUsageTag("finderUsageTag", this, UsageTag().value() ),
    m_finderProductionTag("finderProductionTag", this, ProductionTag().value() )
{
   registerProxyFactory( Stream::kEvent,
                         new ProxyFactoryFunction< NavigationProd, NavTrackProxy >( *this, &NavigationProd::makeNavTrackProxy ),
                         UsageTag() );			    
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavShowerProxy    >,
                         UsageTag() );			    
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavKsProxy   >,
                         UsageTag() );			    
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavLambdaProxy   >,
                         UsageTag() );			    
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavPi0ToGGProxy   >,
                         UsageTag() );			    
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavEtaToGGProxy   >,
                         UsageTag() );			    
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavEtaPrimeToGGProxy   >,
                         UsageTag() );			    
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavConRegProxy    >,
                         UsageTag() );			    
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavTkShMatchProxy >,
                         UsageTag() );
   registerProxyFactory( Stream::kEvent,		    
                         new ProxyFactory< NavRichProxy    >,
                         UsageTag() );			    
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kChargedPion > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kChargedKaon > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kElectron > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kMuon > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kProton > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitElectron > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitMuon > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitPion > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitKaon > >,
//       UsageTag() );
//    registerProxyFactory(
//       Stream::kEvent,
//       new ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitProton > >,
//       UsageTag() );
}

// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< NavigationProd, NavTrackProxy     > ;
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory< NavShowerProxy    > ;
template class ProxyFactory< NavPi0ToGGProxy   > ;
template class ProxyFactory< NavEtaToGGProxy   > ;
template class ProxyFactory< NavEtaPrimeToGGProxy   > ;
template class ProxyFactory< NavKsProxy   > ;
template class ProxyFactory< NavLambdaProxy   > ;
template class ProxyFactory< NavConRegProxy    > ;
template class ProxyFactory< NavTkShMatchProxy > ;
template class ProxyFactory< NavRichProxy    > ;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kChargedPion > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kChargedKaon > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kElectron > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kMuon > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kProton > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitElectron > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitMuon > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitPion > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitKaon > >;
// template class
// ProxyFactory< AETrackCandidateFitQualityProxy< DBCandidate::kExitProton > >;

// non-trivial proxyfactory producer method used above
ProxyBase*
NavigationProd::makeNavTrackProxy() {
   return new NavTrackProxy( this );
}

NavigationProd::~NavigationProd()                    // anal5
{
}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
NavigationProd::init( void )          // anal1 "Interactive"
{
}

// -------------------- terminate method ----------------------------
void
NavigationProd::terminate( void )     // anal5 "Interactive"
{
}

/*
// ---------------- standard place to book histograms ---------------
void
NavigationProd::hist_book( TBHistoManager& )
{
   report( INFO, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}
*/

// ------------ const member functions ----------------------
TDTrackMaster::TagPair 
NavigationProd::fitterTags() const {

  return TDTrackMaster::TagPair( UsageTag(m_fitterUsageTag.value().c_str()),
				 ProductionTag(m_fitterProductionTag.value().c_str()) );
}

TDTrackMaster::TagPair 
NavigationProd::finderTags() const {

  return TDTrackMaster::TagPair( UsageTag(m_finderUsageTag.value().c_str()),
				 ProductionTag(m_finderProductionTag.value().c_str()) );
}

