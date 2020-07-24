// -*- C++ -*-
//
// Package:     <PivarskiProd>
// Module:      PivarskiProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 02:22:32 EDT 1999
// $Id: PivarskiProd.cc,v 1.15 2003/04/17 17:05:05 bkh Exp $
//
// Revision history
//
// $Log: PivarskiProd.cc,v $
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
// Transfered from PivarskiFilter
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
#include "PivarskiProd/PivarskiProd.h"
#include "PivarskiProd/PivarskiTrackFitterProxy.h"
#include "PivarskiProd/PivarskiHelixFitProxy.h"
#include "PivarskiProd/PivarskiFitDRHitLatticeProxy.h"
#include "PivarskiProd/PivarskiFitZDHitLatticeProxy.h"
#include "PivarskiProd/PivarskiFitCathodeHitLatticeProxy.h"
#include "PivarskiProd/PivarskiFitSVRHitLatticeProxy.h"
#include "PivarskiProd/PivarskiFitSVZHitLatticeProxy.h"
#include "PivarskiProd/PivarskiCandidateQualityProxy.h"
#include "PivarskiProd/PivarskiBaseQualityProxy.h"
#include "PivarskiProd/PivarskiDBTrackerValuesProxy.h"

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
static const char* const kFacilityString = "PivarskiProd.PivarskiProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: PivarskiProd.cc,v 1.15 2003/04/17 17:05:05 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//
const STL_VECTOR( string ) PivarskiProd::AllHypos ;

//
// constructors and destructor
//
PivarskiProd::PivarskiProd( void )               // anal1
   : Producer(                 "PivarskiProd" ),
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
      new ProxyFactory< PivarskiDBTrackerValuesProxy >, 
      UsageTag("Pivarski") );

//    registerProxyFactory(
//       Stream::kEvent, 
//       new ProxyFactory< PivarskiTrackFitterProxy >, 
//       UsageTag("Pivarski") );

   registerProxyFactory( 
      Stream::kEvent, 
      new ProxyFactoryFunction< PivarskiProd, PivarskiTrackFitterProxy >(
      *this, 
      &PivarskiProd::makePivarskiTrackFitterProxy ),
      UsageTag() );    // or UsageTag( "MyUsage" )

   // Register proxies for TRHelixCandidateFits.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kElectron > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kMuon > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kChargedPion > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kChargedKaon > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kProton > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitElectron > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitMuon > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitPion > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitKaon > >, 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitProton > >, 
      UsageTag("Pivarski") );

   // Register proxies for track/DR hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<PivarskiFitDRHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<PivarskiFitDRHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitDRHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitDRHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("Pivarski") );


   // Register proxies for track/ZD hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<PivarskiFitZDHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<PivarskiFitZDHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitZDHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitZDHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("Pivarski") );


   // Register proxies for track/Cathode hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy< DBCandidate::kElectron > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy<DBCandidate::kChargedPion> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitPion > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitCathodeHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("Pivarski") );


   // Register proxies for track/SVR hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitSVRHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitSVRHitLatticeProxy<DBCandidate::kChargedKaon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<PivarskiFitSVRHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("Pivarski") );

   // Register proxies for track/SVZ hit Lattices.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kElectron> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitSVZHitLatticeProxy<DBCandidate::kChargedPion > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitSVZHitLatticeProxy<DBCandidate::kChargedKaon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kProton > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitElectron > > , 
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitMuon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitPion> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitKaon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<PivarskiFitSVZHitLatticeProxy<DBCandidate::kExitProton> >,
      UsageTag("Pivarski") );

   // Register proxies for Pivarski quality objects.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiCandidateQualityProxy<DBCandidate::kElectron > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiCandidateQualityProxy<
      DBCandidate::kChargedPion> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiCandidateQualityProxy<
      DBCandidate::kChargedKaon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kProton > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<
      PivarskiCandidateQualityProxy<DBCandidate::kExitElectron > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiCandidateQualityProxy<DBCandidate::kExitMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiCandidateQualityProxy<DBCandidate::kExitPion > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiCandidateQualityProxy<DBCandidate::kExitKaon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<PivarskiCandidateQualityProxy<DBCandidate::kExitProton> >,
      UsageTag("Pivarski") );

   // Register proxies for base quality objects.
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy<DBCandidate::kElectron > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kChargedPion> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kChargedKaon> >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kProton > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy<DBCandidate::kExitElectron > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy<DBCandidate::kExitMuon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy<DBCandidate::kExitPion > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory< PivarskiBaseQualityProxy<DBCandidate::kExitKaon > >,
      UsageTag("Pivarski") );
   registerProxyFactory(
      Stream::kEvent,
      new ProxyFactory<PivarskiBaseQualityProxy<DBCandidate::kExitProton> >,
      UsageTag("Pivarski") );

   // or
   //    register non-default-ctor proxy 
   //       (don't forget to write "PivarskiProd::makeMyProxy()")
#if 0
   registerProxyFactory( 
      Stream::kEvent, 
      new ProxyFactoryFunction< PivarskiProd, MyProxy >(
	 *this, 
	 &PivarskiProd::makeMyProxy )
      UsageTag("Pivarski") );    // or UsageTag( "MyUsage" )
#endif
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
//template class ProxyFactory< PivarskiTrackFitterProxy >;
template class ProxyFactory< PivarskiDBTrackerValuesProxy >;

template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kMuon > > ;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kProton > >;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< PivarskiHelixFitProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< PivarskiFitDRHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< PivarskiFitZDHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< PivarskiFitCathodeHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< PivarskiFitSVRHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kProton > >;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< PivarskiFitSVZHitLatticeProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kProton > >;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< PivarskiCandidateQualityProxy< DBCandidate::kExitProton > >;

template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kElectron > > ;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kMuon > >;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kChargedPion > >;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kChargedKaon > >;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kProton > >;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kExitElectron > > ;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kExitMuon > >;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kExitPion > >;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kExitKaon > >;
template class
ProxyFactory< PivarskiBaseQualityProxy< DBCandidate::kExitProton > >;


#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< PivarskiProd, PivarskiTrackFitterProxy >;

// non-trivial proxyfactory producer method used above
// (if you want to use it, don't forget to declare this method 
//  in "producer.h"!)

ProxyBase*
PivarskiProd::makePivarskiTrackFitterProxy()
{ 
   return new PivarskiTrackFitterProxy( this ) ;
}


// PivarskiProd::PivarskiProd( const PivarskiProd& iPivarskiProd )
// {
//    *this = iPivarskiProd;
// }

PivarskiProd::~PivarskiProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const PivarskiProd& PivarskiProd::operator=( const PivarskiProd& iPivarskiProd )
// {
//   if( this != &iPivarskiProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iPivarskiProd );
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
PivarskiProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
PivarskiProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
PivarskiProd::hist_book( TBHistoManager& )
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
