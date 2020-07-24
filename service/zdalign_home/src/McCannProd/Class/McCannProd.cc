// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 02:22:32 EDT 1999
// $Id: McCannProd.cc,v 1.15 2003/04/17 17:05:05 bkh Exp $
//
// Revision history
//
// $Log: McCannProd.cc,v $
// Revision 1.15  2003/04/17 17:05:05  bkh
// Add ZD Lattice Proxy & throw exceptions if wrong config
//
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
// Transfered from McCannFilter
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
#include "McCannProd/McCannProd.h"
#include "McCannProd/McCannTrackFitterProxy.h"
#include "McCannProd/McCannHelixFitProxy.h"
#include "McCannProd/McCannTRSeedTrackProxy.h"
#include "McCannProd/McCannFitDRHitLatticeProxy.h"
#include "McCannProd/McCannFitZDHitLatticeProxy.h"
#include "McCannProd/McCannFitCathodeHitLatticeProxy.h"
#include "McCannProd/McCannFitSVRHitLatticeProxy.h"
#include "McCannProd/McCannFitSVZHitLatticeProxy.h"
#include "McCannProd/McCannCandidateQualityProxy.h"
#include "McCannProd/McCannBaseQualityProxy.h"
#include "McCannProd/McCannDBTrackerValuesProxy.h"

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
static const char* const kFacilityString = "McCannProd.McCannProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: McCannProd.cc,v 1.15 2003/04/17 17:05:05 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//
const STL_VECTOR( string ) McCannProd::AllHypos ;

//
// constructors and destructor
//
McCannProd::McCannProd( void )               // anal1
   : Producer(                 "McCannProd" ),
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
     m_drHitProdTag(           "DRRHitProductionTag", this, string( "" ) ),
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
      new ProxyFactory< McCannDBTrackerValuesProxy >, 
      UsageTag("McCann") );

//    registerProxyFactory(
//       Stream::kEvent, 
//       new ProxyFactory< McCannTrackFitterProxy >, 
//       UsageTag("McCann") );

   registerProxyFactory( 
      Stream::kEvent, 
      new ProxyFactoryFunction< McCannProd, McCannTrackFitterProxy >(
      *this, 
      &McCannProd::makeMcCannTrackFitterProxy ),
      UsageTag() );    // or UsageTag( "MyUsage" )

   // Register proxies for TRHelixCandidateFits.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kElectron > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kMuon > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kChargedPion > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kChargedKaon > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kProton > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitElectron > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitMuon > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitPion > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitKaon > >, 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitProton > >, 
      UsageTag("McCann") );

   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannTRSeedTrackProxy >,
      UsageTag("McCann") );

   // Register proxies for track/DR hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<McCannFitDRHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<McCannFitDRHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitDRHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitDRHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("McCann") );


   // Register proxies for track/ZD hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<McCannFitZDHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<McCannFitZDHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitZDHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitZDHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("McCann") );


   // Register proxies for track/Cathode hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitCathodeHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("McCann") );


   // Register proxies for track/SVR hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitSVRHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitSVRHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitSVRHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<McCannFitSVRHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("McCann") );

   // Register proxies for track/SVZ hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitSVZHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitSVZHitLatticeProxy<DBCandidate::kChargedKaon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannFitSVZHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<McCannFitSVZHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("McCann") );

   // Register proxies for McCann quality objects.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannCandidateQualityProxy<DBCandidate::kElectron > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannCandidateQualityProxy<
      DBCandidate::kChargedPion> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannCandidateQualityProxy<
      DBCandidate::kChargedKaon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kProton > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      McCannCandidateQualityProxy<DBCandidate::kExitElectron > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannCandidateQualityProxy<DBCandidate::kExitMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannCandidateQualityProxy<DBCandidate::kExitPion > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannCandidateQualityProxy<DBCandidate::kExitKaon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<McCannCandidateQualityProxy<DBCandidate::kExitProton> >,
      UsageTag("McCann") );

   // Register proxies for base quality objects.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy<DBCandidate::kElectron > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy< DBCandidate::kMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy< DBCandidate::kChargedPion> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy< DBCandidate::kChargedKaon> >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy< DBCandidate::kProton > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy<DBCandidate::kExitElectron > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy<DBCandidate::kExitMuon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy<DBCandidate::kExitPion > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< McCannBaseQualityProxy<DBCandidate::kExitKaon > >,
      UsageTag("McCann") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<McCannBaseQualityProxy<DBCandidate::kExitProton> >,
      UsageTag("McCann") );

   // or
   //    register non-default-ctor proxy 
   //       (don't forget to write "McCannProd::makeMyProxy()")
#if 0
   registerProxyFactory( 
      Stream::kEvent, 
      new ProxyFactoryFunction< McCannProd, MyProxy >(
	 *this, 
	 &McCannProd::makeMyProxy )
      UsageTag("McCann") );    // or UsageTag( "MyUsage" )
#endif
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
//template class ProxyFactory< McCannTrackFitterProxy >;
template class ProxyFactory< McCannDBTrackerValuesProxy >;

template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kMuon > > ;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kProton > >;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< McCannHelixFitProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< McCannTRSeedTrackProxy >;

template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< McCannFitDRHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< McCannFitZDHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< McCannFitCathodeHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< McCannFitSVRHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< McCannFitSVZHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kProton > >;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< McCannCandidateQualityProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kProton > >;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< McCannBaseQualityProxy< DBCandidate::kExitProton > >;


#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< McCannProd, McCannTrackFitterProxy >;

// non-trivial proxyfactory producer method used above
// (if you want to use it, don't forget to declare this method 
//  in "producer.h"!)

ProxyBase*
McCannProd::makeMcCannTrackFitterProxy()
{ 
   return new McCannTrackFitterProxy( this ) ;
}


// McCannProd::McCannProd( const McCannProd& iMcCannProd )
// {
//    *this = iMcCannProd;
// }

McCannProd::~McCannProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const McCannProd& McCannProd::operator=( const McCannProd& iMcCannProd )
// {
//   if( this != &iMcCannProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iMcCannProd );
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
McCannProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
McCannProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
McCannProd::hist_book( TBHistoManager& )
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
