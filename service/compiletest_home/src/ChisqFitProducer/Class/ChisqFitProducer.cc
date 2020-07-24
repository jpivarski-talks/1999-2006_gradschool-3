// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitProducer
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 00:06:11 EDT 1999
// $Id: ChisqFitProducer.cc,v 1.11 2003/04/17 17:00:48 bkh Exp $
//
// Revision history
//
// $Log: ChisqFitProducer.cc,v $
// Revision 1.11  2003/04/17 17:00:48  bkh
// Add ZD options & throw exceptions for wrong config
//
// Revision 1.10  2000/11/18 00:21:38  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.9  2000/09/05 20:43:21  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.8  2000/08/16 21:06:20  wsun
// Added usage and production tags for TRSeedTracks.
//
// Revision 1.7  2000/06/10 06:27:44  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.6  2000/01/11 20:18:12  wsun
// Added parameters to control exception handling.
//
// Revision 1.5  1999/09/11 19:34:17  wsun
// Added DR cathodes.
//
// Revision 1.4  1999/07/07 16:45:16  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.3  1999/06/24 22:56:34  wsun
// Added use of HIMagField.
//
// Revision 1.2  1999/06/20 22:45:46  wsun
// Added proxies for all ten fit hypotheses.
//
// Revision 1.1.1.1  1999/06/16 02:48:27  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ChisqFitProducer/ChisqFitProducer.h"
#include "ChisqFitProducer/ChisqTrackFitterProxy.h"
#include "ChisqFitProducer/ChisqHelixFitProxy.h"
#include "ChisqFitProducer/ChisqFitDRHitLatticeProxy.h"
#include "ChisqFitProducer/ChisqFitZDHitLatticeProxy.h"
#include "ChisqFitProducer/ChisqFitCathodeHitLatticeProxy.h"
#include "ChisqFitProducer/ChisqFitSVRHitLatticeProxy.h"
#include "ChisqFitProducer/ChisqFitSVZHitLatticeProxy.h"
#include "ChisqFitProducer/ChisqCandidateQualityProxy.h"
#include "ChisqFitProducer/ChisqBaseQualityProxy.h"
#include "ChisqFitProducer/ChisqDBTrackerValuesProxy.h"

#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ChisqFitProducer.ChisqFitProducer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqFitProducer.cc,v 1.11 2003/04/17 17:00:48 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChisqFitProducer::ChisqFitProducer( void )               // anal1
   : Producer( "ChisqFitProducer" ),
     m_printDiagnostics(       "PrintDiagnostics", this, false ),
     m_useDRAxialHits(         "UseDRAxial", this, true ),
     m_useDRStereoUHits(       "UseDRStereoU", this, true ),
     m_useDRStereoVHits(       "UseDRStereoV", this, true ),
     m_correctDRHits(          "CorrectDRHits", this, true ),
     m_useZDStereoUHits(       "UseZDStereoU", this, true ),
     m_useZDStereoVHits(       "UseZDStereoV", this, true ),
     m_correctZDHits(          "CorrectZDHits", this, true ),
     m_useDRCathodeHits(       "UseDRCathode", this, true ),
     m_correctDRCathodeHits(   "CorrectDRCathodeHits", this, true ),
     m_useSVRPhiHits(          "UseSVRPhi", this, true ),
     m_useSVZHits(             "UseSVZ", this, true ),
     m_correctSVRHits(         "CorrectSVRHits", this, true ),
     m_correctSVZHits(         "CorrectSVZHits", this, true ),
     m_maxHitChisq(            "MaxHitChisq", this, 50. ),
     m_maxDeltaChisqNdof(      "MaxDeltaChisqNdof", this, 0.01 ),
     m_maxIterations(          "MaxIterations", this, 10 ),
     m_bfieldMagnitude(        "BfieldMagnitudeKGauss", this, 0. ),
     m_fixCurv(                "FixCurvature", this, false ),
     m_fixPhi0(                "FixPhi0", this, false ),
     m_fixD0(                  "FixD0", this, false ),
     m_fixCotTh(               "FixCotTheta", this, false ),
     m_fixZ0(                  "FixZ0", this, false ),
     m_continueIfHitsMissing(  "ContinueIfHitsMissing", this, false ),
     m_continueIfAnyException( "ContinueIfAnyException", this, false ),
     m_drHitUsageTag(          "DRHitUsageTag", this, string( "" ) ),
     m_drHitProdTag(           "DRHitProductionTag", this, string( "" ) ),
     m_zdHitUsageTag(          "ZDHitUsageTag", this, string( "" ) ),
     m_zdHitProdTag(           "ZDHitProductionTag", this, string( "" ) ),
     m_cathodeHitUsageTag(     "CathodeHitUsageTag", this, string( "" ) ),
     m_cathodeHitProdTag(      "CathodeHitProductionTag", this, string( "" ) ),
     m_svrHitUsageTag(         "SVRHitUsageTag", this, string( "" ) ),
     m_svrHitProdTag(          "SVRHitProductionTag", this, string( "" ) ),
     m_svzHitUsageTag(         "SVZHitUsageTag", this, string( "" ) ),
     m_svzHitProdTag(          "SVZHitProductionTag", this, string( "" ) ),
     m_seedTrackUsageTag(      "SeedTrackUsageTag", this, string( "" ) ),
     m_seedTrackProdTag(       "SeedTrackProductionTag", this, string( "" ) ),
     m_forceDRFittingWeight(   "ForceDRFittingWeight", this, false ),
     m_drFittingWeight(        "DRFittingWeight", this, 0. ),
     m_forceZDFittingWeight(   "ForceZDFittingWeight", this, false ),
     m_zdFittingWeight(        "ZDFittingWeight", this, 0. ),
     m_forceSVRFittingWeight(  "ForceSVRFittingWeight", this, false ),
     m_svrFittingWeight(       "SVRFittingWeight", this, 0. ),
     m_forceSVZFittingWeight(  "ForceSVZFittingWeight", this, false ),
     m_svzFittingWeight(       "SVZFittingWeight", this, 0. ),
     m_forceCathodeFittingWeight( "ForceCathodeFittingWeight", this, false ),
     m_cathodeFittingWeight(      "CathodeFittingWeight", this, 0. ),
     m_scaleCathodeResolution( "ScaleCathodeResolution", this, false ),
     m_cathodeResolutionScale( "CathodeResolutionScale", this, 1. )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ----- register proxy factories ------------------
   // (Please see the "ProxyExample" library for examples!)
   //
   // Two steps: 1) use "registerProxyFactory" to register
   //            2) instantiate ProxyFactory/Function
   //               (if you're using the standard templated factories;
   //                you may of course roll your own!)

   // Proxy Registration Step 1:
   // either
   //    register default-ctor proxy
   registerProxyFactory(
      Stream::kEvent, 
      new ProxyFactory< ChisqDBTrackerValuesProxy >, 
      UsageTag() );

   registerProxyFactory( 
      Stream::kEvent, 
      new ProxyFactoryFunction< ChisqFitProducer, ChisqTrackFitterProxy >(
      *this, 
      &ChisqFitProducer::makeChisqTrackFitterProxy ),
      UsageTag() );    // or UsageTag( "MyUsage" )

   // Register proxies for TRHelixCandidateFits.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kElectron > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kMuon > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kChargedPion > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kChargedKaon > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kProton > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitElectron > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitMuon > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitPion > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitKaon > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitProton > >, 
      UsageTag() );

   // Register proxies for track/DR hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<ChisqFitDRHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<ChisqFitDRHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitDRHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitDRHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for track/ZD hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<ChisqFitZDHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<ChisqFitZDHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitZDHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitZDHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for track/cathode hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitCathodeHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for track/SVR hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitSVRHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitSVRHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitSVRHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<ChisqFitSVRHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for track/SVZ hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitSVZHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitSVZHitLatticeProxy<DBCandidate::kChargedKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqFitSVZHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<ChisqFitSVZHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for Chisq quality objects.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqCandidateQualityProxy<DBCandidate::kElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqCandidateQualityProxy<
      DBCandidate::kChargedPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqCandidateQualityProxy<
      DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      ChisqCandidateQualityProxy<DBCandidate::kExitElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqCandidateQualityProxy<DBCandidate::kExitMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqCandidateQualityProxy<DBCandidate::kExitPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqCandidateQualityProxy<DBCandidate::kExitKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<ChisqCandidateQualityProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for base quality objects.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy<DBCandidate::kElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kChargedPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy<DBCandidate::kExitElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy<DBCandidate::kExitMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy<DBCandidate::kExitPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< ChisqBaseQualityProxy<DBCandidate::kExitKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<ChisqBaseQualityProxy<DBCandidate::kExitProton> >,
      UsageTag() );
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory< ChisqDBTrackerValuesProxy >;

template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kMuon > > ;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kProton > >;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< ChisqHelixFitProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< ChisqFitDRHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< ChisqFitZDHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< ChisqFitCathodeHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< ChisqFitSVRHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< ChisqFitSVZHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kProton > >;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< ChisqCandidateQualityProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kProton > >;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< ChisqBaseQualityProxy< DBCandidate::kExitProton > >;

#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< ChisqFitProducer, ChisqTrackFitterProxy >;


// non-trivial proxyfactory producer method used above
// (if you want to use it, don't forget to declare this method 
//  in "producer.h"!)

ProxyBase*
ChisqFitProducer::makeChisqTrackFitterProxy()
{ 
   return new ChisqTrackFitterProxy( this ) ;
}

// ChisqFitProducer::ChisqFitProducer( const ChisqFitProducer& iChisqFitProducer )
// {
//    *this = iChisqFitProducer;
// }

ChisqFitProducer::~ChisqFitProducer()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const ChisqFitProducer& ChisqFitProducer::operator=( const ChisqFitProducer& iChisqFitProducer )
// {
//   if( this != &iChisqFitProducer ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iChisqFitProducer );
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
ChisqFitProducer::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
ChisqFitProducer::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
ChisqFitProducer::hist_book( TBHistoManager& )
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
