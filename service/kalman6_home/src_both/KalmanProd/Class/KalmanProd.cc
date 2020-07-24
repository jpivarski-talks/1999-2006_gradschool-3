// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 02:22:32 EDT 1999
// $Id: KalmanProd.cc,v 1.14 2002/02/23 06:51:01 wsun Exp $
//
// Revision history
//
// $Log: KalmanProd.cc,v $
// Revision 1.14  2002/02/23 06:51:01  wsun
// Added flag for applying B field correction.
//
// Revision 1.13  2002/02/22 03:33:57  wsun
// Change outwardFitIsInwardSeed to inwardFitIsOutwardSeed.
//
// Revision 1.12  2000/12/13 07:04:15  wsun
// Added option to not have inward fits propagated to the origin.
//
// Revision 1.11  2000/11/18 00:18:33  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.10  2000/09/05 20:48:15  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.9  2000/08/16 20:19:08  wsun
// Added usage and production tags for seed tracks.
//
// Revision 1.8  2000/07/27 23:29:59  wsun
// Added parameter for specifying usage and and production tags for hits.
//
// Revision 1.7  2000/06/10 05:56:44  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.6  2000/06/09 00:56:14  wsun
// Added parameters for selecting hypotheses to be fit and which
// hypothesis determines the hit list.
//
// Revision 1.5  2000/06/06 22:17:17  wsun
// Added parameter CorrectDRHits.
//
// Revision 1.4  2000/02/03 18:54:14  wsun
// Added parameters MaxHitsToDelete and MaxHitResidualPull.
//
// Revision 1.3  2000/01/11 20:39:09  wsun
// Added parameters to control exception handling.
//
// Revision 1.2  1999/10/08 23:23:46  wsun
// * Added param to optimize inward+outward fitting for a single mass hypo.
// * Added param to optimize fitting of all tracks to all hypotheses.
//
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from KalmanFilter
//
// Revision 1.8  1999/08/30 21:49:31  wsun
// Added parameters for dE/dx straggling.
//
// Revision 1.7  1999/08/27 20:35:45  wsun
// Changed MaxFractionalDeltaP default to 1.
//
// Revision 1.6  1999/07/07 16:46:39  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.5  1999/06/30 23:53:13  wsun
// Changed default value for MaxFractionalDeltaP to 0.002.
//
// Revision 1.4  1999/06/24 22:44:25  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.3  1999/06/18 19:27:59  wsun
// More improvements to low momentum fitting.
//
// Revision 1.2  1999/06/02 23:07:41  wsun
// Improved low-momentum fitting.
//
// Revision 1.1  1999/05/14 01:12:59  wsun
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
#include "KalmanProd/KalmanProd.h"
#include "KalmanProd/KalmanTrackFitterProxy.h"
#include "KalmanProd/KalmanHelixFitProxy.h"
#include "KalmanProd/KalmanFitDRHitLatticeProxy.h"
#include "KalmanProd/KalmanFitCathodeHitLatticeProxy.h"
#include "KalmanProd/KalmanFitSVRHitLatticeProxy.h"
#include "KalmanProd/KalmanFitSVZHitLatticeProxy.h"
#include "KalmanProd/KalmanCandidateQualityProxy.h"
#include "KalmanProd/KalmanBaseQualityProxy.h"
#include "KalmanProd/KalmanDBTrackerValuesProxy.h"

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
static const char* const kFacilityString = "KalmanProd.KalmanProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanProd.cc,v 1.14 2002/02/23 06:51:01 wsun Exp $";
static const char* const kTagString = "$Name: v02_03_00 $";

//
// static data member definitions
//
const STL_VECTOR( string ) KalmanProd::AllHypos ;

//
// constructors and destructor
//
KalmanProd::KalmanProd( void )               // anal1
   : Producer(                 "KalmanProd" ),
     m_printDiagnostics(       "PrintDiagnostics", this, false ),
     m_useDRAxialHits(         "UseDRAxial", this, true ),
     m_useDRStereoUHits(       "UseDRStereoU", this, true ),
     m_useDRStereoVHits(       "UseDRStereoV", this, true ),
     m_correctDRHits(          "CorrectDRHits", this, true ),
     m_useDRCathodeHits(       "UseDRCathode", this, true ),
     m_correctDRCathodeHits(   "CorrectDRCathodeHits", this, true ),
     m_useSVRPhiHits(          "UseSVRPhi", this, true ),
     m_useSVZHits(             "UseSVZ", this, true ),
     m_correctSVRHits(         "CorrectSVRHits", this, true ),
     m_correctSVZHits(         "CorrectSVZHits", this, true ),
     m_applyDedx(              "ApplyDedx", this, true ),
     m_applyStraggling(        "ApplyStraggling", this, true ),
     m_stragglingCoeff(        "StragglingCoeff", this, 0.001 ),
     m_applyMultScat(          "ApplyMultScat", this, true ),
     m_applyBfieldCorrections( "ApplyBfieldCorrections", this, true ),
     m_maxHitChisq(            "MaxHitChisq", this, 15. ),
     m_updateErr2Z0(           "UpdateErr2Z0", this, 1.e-6 ),
     m_updateErr2CotTheta(     "UpdateErr2CotTheta", this, 0.01 ),
     m_updateDFracCotTheta(    "UpdateDFracCotTheta", this, 20. ),
     m_maxFracDeltaP(          "MaxFractionalDeltaP", this, 1.1 ),
     m_bfieldMagnitude(        "BfieldMagnitudeKGauss", this, 0. ),
     m_fixCurv(                "FixCurvature", this, false ),
     m_fixPhi0(                "FixPhi0", this, false ),
     m_fixD0(                  "FixD0", this, false ),
     m_fixCotTh(               "FixCotTheta", this, false ),
     m_fixZ0(                  "FixZ0", this, false ),
     m_optimizeForAllHypos(    "OptimizeForAllHypos", this, false ),
     m_inwardFitIsOutwardSeed( "InwardFitIsOutwardSeed", this, false ),
     m_continueIfHitsMissing(  "ContinueIfHitsMissing", this, false ),
     m_continueIfAnyException( "ContinueIfAnyException", this, false ),
     m_maxHitsToDelete(        "MaxHitsToDelete", this, 3 ),
     m_maxHitResidualPull(     "MaxHitResidualPull", this, 3.5 ),
     m_hyposToFit(             "HyposToFit", this, AllHypos ),
     m_hitListFromOneHypo(     "HitListFromOneHypo", this, true ),
     m_hitListHypo(            "HitListHypo" , this, string( "pion" ) ),
     m_drHitUsageTag(          "DRHitUsageTag", this, string( "" ) ),
     m_drHitProdTag(           "DRHitProductionTag", this, string( "" ) ),
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
     m_forceSVRFittingWeight(  "ForceSVRFittingWeight", this, false ),
     m_svrFittingWeight(       "SVRFittingWeight", this, 0. ),
     m_forceSVZFittingWeight(  "ForceSVZFittingWeight", this, false ),
     m_svzFittingWeight(       "SVZFittingWeight", this, 0. ),
     m_forceCathodeFittingWeight( "ForceCathodeFittingWeight", this, false ),
     m_cathodeFittingWeight(      "CathodeFittingWeight", this, 0. ),
     m_scaleCathodeResolution( "ScaleCathodeResolution", this, false ),
     m_cathodeResolutionScale( "CathodeResolutionScale", this, 1. ),
     m_inwardFitsAtLastHit(    "InwardFitsAtLastHit", this, false )
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
      new ProxyFactory< KalmanDBTrackerValuesProxy >, 
      UsageTag() );

//    registerProxyFactory(
//       Stream::kEvent, 
//       new ProxyFactory< KalmanTrackFitterProxy >, 
//       UsageTag() );

   registerProxyFactory( 
      Stream::kEvent, 
      new ProxyFactoryFunction< KalmanProd, KalmanTrackFitterProxy >(
      *this, 
      &KalmanProd::makeKalmanTrackFitterProxy ),
      UsageTag() );    // or UsageTag( "MyUsage" )

   // Register proxies for TRHelixCandidateFits.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kElectron > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kMuon > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kChargedPion > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kChargedKaon > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kProton > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitElectron > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitMuon > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitPion > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitKaon > >, 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitProton > >, 
      UsageTag() );

   // Register proxies for track/DR hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<KalmanFitDRHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<KalmanFitDRHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitDRHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitDRHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );


   // Register proxies for track/Cathode hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitCathodeHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );


   // Register proxies for track/SVR hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitSVRHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitSVRHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitSVRHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<KalmanFitSVRHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for track/SVZ hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitSVZHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitSVZHitLatticeProxy<DBCandidate::kChargedKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanFitSVZHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<KalmanFitSVZHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for Kalman quality objects.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanCandidateQualityProxy<DBCandidate::kElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanCandidateQualityProxy<
      DBCandidate::kChargedPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanCandidateQualityProxy<
      DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      KalmanCandidateQualityProxy<DBCandidate::kExitElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanCandidateQualityProxy<DBCandidate::kExitMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanCandidateQualityProxy<DBCandidate::kExitPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanCandidateQualityProxy<DBCandidate::kExitKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<KalmanCandidateQualityProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // Register proxies for base quality objects.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy<DBCandidate::kElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kChargedPion> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kChargedKaon> >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kProton > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy<DBCandidate::kExitElectron > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy<DBCandidate::kExitMuon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy<DBCandidate::kExitPion > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< KalmanBaseQualityProxy<DBCandidate::kExitKaon > >,
      UsageTag() );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<KalmanBaseQualityProxy<DBCandidate::kExitProton> >,
      UsageTag() );

   // or
   //    register non-default-ctor proxy 
   //       (don't forget to write "KalmanProd::makeMyProxy()")
#if 0
   registerProxyFactory( 
      Stream::kEvent, 
      new ProxyFactoryFunction< KalmanProd, MyProxy >(
	 *this, 
	 &KalmanProd::makeMyProxy )
      UsageTag() );    // or UsageTag( "MyUsage" )
#endif
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
//template class ProxyFactory< KalmanTrackFitterProxy >;
template class ProxyFactory< KalmanDBTrackerValuesProxy >;

template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kMuon > > ;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kProton > >;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< KalmanHelixFitProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< KalmanFitDRHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< KalmanFitCathodeHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< KalmanFitSVRHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< KalmanFitSVZHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kProton > >;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< KalmanCandidateQualityProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kProton > >;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< KalmanBaseQualityProxy< DBCandidate::kExitProton > >;


#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< KalmanProd, KalmanTrackFitterProxy >;

// non-trivial proxyfactory producer method used above
// (if you want to use it, don't forget to declare this method 
//  in "producer.h"!)

ProxyBase*
KalmanProd::makeKalmanTrackFitterProxy()
{ 
   return new KalmanTrackFitterProxy( this ) ;
}


// KalmanProd::KalmanProd( const KalmanProd& iKalmanProd )
// {
//    *this = iKalmanProd;
// }

KalmanProd::~KalmanProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const KalmanProd& KalmanProd::operator=( const KalmanProd& iKalmanProd )
// {
//   if( this != &iKalmanProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iKalmanProd );
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
KalmanProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
KalmanProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
KalmanProd::hist_book( TBHistoManager& )
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
