// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanTrackFitterProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 17:59:13 EDT 1999
// $Id: KalmanTrackFitterProxy.cc,v 1.16 2002/02/23 06:51:01 wsun Exp $
//
// Revision history
//
// $Log: KalmanTrackFitterProxy.cc,v $
// Revision 1.16  2002/02/23 06:51:01  wsun
// Added flag for applying B field correction.
//
// Revision 1.15  2002/02/22 03:33:57  wsun
// Change outwardFitIsInwardSeed to inwardFitIsOutwardSeed.
//
// Revision 1.14  2001/02/26 16:49:16  cdj
// now get MagneticField from StartRun
//
// Revision 1.13  2000/12/13 07:04:16  wsun
// Added option to not have inward fits propagated to the origin.
//
// Revision 1.12  2000/11/18 00:18:34  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.11  2000/09/05 20:48:16  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.10  2000/08/16 20:19:09  wsun
// Added usage and production tags for seed tracks.
//
// Revision 1.9  2000/07/27 23:30:00  wsun
// Added parameter for specifying usage and and production tags for hits.
//
// Revision 1.8  2000/06/10 05:56:45  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.7  2000/06/09 00:56:15  wsun
// Added parameters for selecting hypotheses to be fit and which
// hypothesis determines the hit list.
//
// Revision 1.6  2000/06/06 22:17:18  wsun
// Added parameter CorrectDRHits.
//
// Revision 1.5  2000/02/03 18:54:14  wsun
// Added parameters MaxHitsToDelete and MaxHitResidualPull.
//
// Revision 1.4  2000/01/11 20:39:43  wsun
// Replaced control flags with a pointer to KalmanProd.
//
// Revision 1.3  1999/10/09 00:00:23  wsun
// Better handling of optimize parameters.
//
// Revision 1.2  1999/10/08 23:23:48  wsun
// * Added param to optimize inward+outward fitting for a single mass hypo.
// * Added param to optimize fitting of all tracks to all hypotheses.
//
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from KalmanFilter
//
// Revision 1.8  1999/08/30 21:51:52  wsun
// New constructor with parameters for dE/dx straggling.
//
// Revision 1.7  1999/08/03 22:34:29  wsun
// Bind updateSiGeom to base geometry stream instead of begin run.
//
// Revision 1.6  1999/07/07 16:46:43  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.5  1999/06/30 23:52:34  wsun
// Added updateSiGeom() function bound to beginrun.
//
// Revision 1.4  1999/06/24 22:44:27  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.3  1999/06/18 19:28:01  wsun
// More improvements to low momentum fitting.
//
// Revision 1.2  1999/06/02 23:07:44  wsun
// Improved low-momentum fitting.
//
// Revision 1.1  1999/05/14 01:13:00  wsun
// First submission.
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

#include "HelixIntersection/HIMagField.h"
#include "KalmanProd/KalmanTrackFitterProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "KalmanProd/KalmanProd.h"

#if 0
#include "TrackDelivery/TDKinematicPionFit.h" 
#endif

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
static const char* const kFacilityString = "KalmanTrackFitterProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanTrackFitterProxy.cc,v 1.16 2002/02/23 06:51:01 wsun Exp $";
static const char* const kTagString = "$Name: v02_03_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//

KalmanTrackFitterProxy::KalmanTrackFitterProxy( KalmanProd* aKalmanProd )
   : m_kalmanTrackFitter( 0 ),
     m_kalmanProd( aKalmanProd )
{
   if( m_kalmanProd != 0 &&
       m_kalmanProd->bfieldMagnitude() != 0. ) // default value from KalmanProd
   {
      // This will override the MagneticField object in the Frame.
      HIMagField::instance()->setBfield( m_kalmanProd->bfieldMagnitude() ) ;
   }

   bind( &KalmanTrackFitterProxy::updateMagField, Stream::kStartRun ) ;
   bind( &KalmanTrackFitterProxy::updateSiGeom,   Stream::kSVAlignment ) ;
   bind( &KalmanTrackFitterProxy::updateIRGeom,   Stream::kBaseGeometry ) ;
   bind( &KalmanTrackFitterProxy::updateDRGeom,   Stream::kDRAlignment ) ;
}

// KalmanTrackFitterProxy::KalmanTrackFitterProxy( const KalmanTrackFitterProxy& iproxy )
// {
//   *this = iproxy;
// }

KalmanTrackFitterProxy::~KalmanTrackFitterProxy()
{
   HIMagField::instance()->destroy() ;
   delete m_kalmanTrackFitter ;
}

//
// assignment operators
//
// const KalmanTrackFitterProxy& KalmanTrackFitterProxy::operator=( const KalmanTrackFitterProxy& iproxy )
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
KalmanTrackFitterProxy::invalidateCache()
{
   if( m_kalmanTrackFitter != 0 )
   {
      m_kalmanTrackFitter->clearCache() ;
   }
}

const KalmanTrackFitterProxy::value_type*
KalmanTrackFitterProxy::faultHandler( const Record& iRecord,
				      const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
//   assert( 0 == m_kalmanTrackFitter );

   if( 0 == m_kalmanTrackFitter )
   {
      // ---------- create the new object --------------------
      m_kalmanTrackFitter = new KalmanTrackFitter ;
   }

   m_kalmanTrackFitter->setPrintDiagnostics(
      m_kalmanProd->printDiagnostics() ) ;
   m_kalmanTrackFitter->setUseDRAxialHits(
      m_kalmanProd->useDRAxialHits() ) ;
   m_kalmanTrackFitter->setUseDRStereoUHits(
      m_kalmanProd->useDRStereoUHits() ) ;
   m_kalmanTrackFitter->setUseDRStereoVHits(
      m_kalmanProd->useDRStereoVHits() ) ;
   m_kalmanTrackFitter->setCorrectDRHits(
      m_kalmanProd->correctDRHits() ) ;

   m_kalmanTrackFitter->setUseDRCathodeHits(
      m_kalmanProd->useDRCathodeHits() ) ;
   m_kalmanTrackFitter->setCorrectDRCathodeHits(
      m_kalmanProd->correctDRCathodeHits() ) ;

   m_kalmanTrackFitter->setUseSVRPhiHits( m_kalmanProd->useSVRPhiHits() ) ;
   m_kalmanTrackFitter->setUseSVZHits( m_kalmanProd->useSVZHits() ) ;
   m_kalmanTrackFitter->setCorrectSVRHits(
      m_kalmanProd->correctSVRHits() ) ;
   m_kalmanTrackFitter->setCorrectSVZHits(
      m_kalmanProd->correctSVZHits() ) ;

   m_kalmanTrackFitter->setApplyDedx( m_kalmanProd->applyDedx() ) ;
   m_kalmanTrackFitter->setApplyStraggling(
      m_kalmanProd->applyStraggling() ) ;
   m_kalmanTrackFitter->setStragglingCoeff(
      m_kalmanProd->stragglingCoeff() ) ;
   m_kalmanTrackFitter->setApplyMultScat( m_kalmanProd->applyMultScat() ) ;
   m_kalmanTrackFitter->setApplyBfieldCorrections(
      m_kalmanProd->applyBfieldCorrections() ) ;

   m_kalmanTrackFitter->setHitChisqCut( m_kalmanProd->maxHitChisq() ) ;
   m_kalmanTrackFitter->setUpdateErr2Z0( m_kalmanProd->updateErr2Z0() ) ;
   m_kalmanTrackFitter->setUpdateErr2CotTheta(
      m_kalmanProd->updateErr2CotTheta() ) ;
   m_kalmanTrackFitter->setUpdateDFracCotTheta(
      m_kalmanProd->updateDFracCotTheta() ) ;
   m_kalmanTrackFitter->setMaxFractionalDeltaP(
      m_kalmanProd->maxFracDeltaP() ) ;

   m_kalmanTrackFitter->setInwardFitIsOutwardSeed(
      m_kalmanProd->inwardFitIsOutwardSeed() ||
      m_kalmanProd->optimizeForAllHypos() );

   m_kalmanTrackFitter->setMaxHitsToDelete( m_kalmanProd->maxHitsToDelete() ) ;
   m_kalmanTrackFitter->setMaxHitResidualPull(
      m_kalmanProd->maxHitResidualPull() ) ;

   if( m_kalmanProd->fixCurvature() )  m_kalmanTrackFitter->fixCurvature() ;
   if( m_kalmanProd->fixPhi0() )       m_kalmanTrackFitter->fixPhi0() ;
   if( m_kalmanProd->fixD0() )         m_kalmanTrackFitter->fixD0() ;
   if( m_kalmanProd->fixCotTheta() )   m_kalmanTrackFitter->fixCotTheta() ;
   if( m_kalmanProd->fixZ0() )         m_kalmanTrackFitter->fixZ0() ;

   m_kalmanTrackFitter->setContinueIfHitsMissing(
      m_kalmanProd->continueIfHitsMissing() ) ;
   m_kalmanTrackFitter->setContinueIfAnyException(
      m_kalmanProd->continueIfAnyException() ) ;      

   m_kalmanTrackFitter->setDRHitUsageTag(
      m_kalmanProd->drHitUsageTag() ) ;
   m_kalmanTrackFitter->setDRHitProductionTag(
      m_kalmanProd->drHitProductionTag() ) ;
   m_kalmanTrackFitter->setCathodeHitUsageTag(
      m_kalmanProd->cathodeHitUsageTag() ) ;
   m_kalmanTrackFitter->setCathodeHitProductionTag(
      m_kalmanProd->cathodeHitProductionTag() ) ;
   m_kalmanTrackFitter->setSVRHitUsageTag(
      m_kalmanProd->svrHitUsageTag() ) ;
   m_kalmanTrackFitter->setSVRHitProductionTag(
      m_kalmanProd->svrHitProductionTag() ) ;
   m_kalmanTrackFitter->setSVZHitUsageTag(
      m_kalmanProd->svzHitUsageTag() ) ;
   m_kalmanTrackFitter->setSVZHitProductionTag(
      m_kalmanProd->svzHitProductionTag() ) ;

   m_kalmanTrackFitter->setSeedTrackUsageTag(
      m_kalmanProd->seedTrackUsageTag() ) ;
   m_kalmanTrackFitter->setSeedTrackProductionTag(
      m_kalmanProd->seedTrackProductionTag() ) ;

   if( m_kalmanProd->forceDRFittingWeight() )
   {
      m_kalmanTrackFitter->setDRFittingWeight(
	 m_kalmanProd->drFittingWeight() ) ;
   }

   if( m_kalmanProd->forceSVRFittingWeight() )
   {
      m_kalmanTrackFitter->setSVRFittingWeight(
	 m_kalmanProd->svrFittingWeight() ) ;
   }

   if( m_kalmanProd->forceSVZFittingWeight() )
   {
      m_kalmanTrackFitter->setSVZFittingWeight(
	 m_kalmanProd->svzFittingWeight() ) ;
   }

   if( m_kalmanProd->forceCathodeFittingWeight() )
   {
      m_kalmanTrackFitter->setCathodeFittingWeight(
	 m_kalmanProd->cathodeFittingWeight() ) ;
   }

   if( m_kalmanProd->scaleCathodeResolution() )
   {
      m_kalmanTrackFitter->setCathodeFittingWeightScale(
	 1. / sqr( m_kalmanProd->cathodeResolutionScale() ) ) ;
   }

   m_kalmanTrackFitter->setInwardFitsAtLastHit(
      m_kalmanProd->inwardFitsAtLastHit() ) ;

   // Set hypotheses to be fitted.
   STL_VECTOR( DBCandidate::Hypo ) hyposToFit ;

   // If the vector from KalmanProd is empty, then fit all hypos.
   if( m_kalmanProd->hyposToFit().size() == 0 )
   {
      hyposToFit.push_back( DBCandidate::kElectron ) ;
      hyposToFit.push_back( DBCandidate::kMuon ) ;
      hyposToFit.push_back( DBCandidate::kChargedPion ) ;
      hyposToFit.push_back( DBCandidate::kChargedKaon ) ;
      hyposToFit.push_back( DBCandidate::kProton ) ;
   }
   else
   {
      STL_VECTOR( string )::const_iterator fitHypoItr =
	 m_kalmanProd->hyposToFit().begin() ;
      STL_VECTOR( string )::const_iterator fitHypoEnd =
	 m_kalmanProd->hyposToFit().end() ;

      for( ; fitHypoItr != fitHypoEnd ; ++fitHypoItr )
      {
	 if( *fitHypoItr == "electron" )
	 {
	    hyposToFit.push_back( DBCandidate::kElectron ) ;
	 }
	 else if( *fitHypoItr == "muon" )
	 {
	    hyposToFit.push_back( DBCandidate::kMuon ) ;
	 }
	 else if( *fitHypoItr == "pion" )
	 {
	    hyposToFit.push_back( DBCandidate::kChargedPion ) ;
	 }
	 else if( *fitHypoItr == "kaon" )
	 {
	    hyposToFit.push_back( DBCandidate::kChargedKaon ) ;
	 }
	 else if( *fitHypoItr == "proton" )
	 {
	    hyposToFit.push_back( DBCandidate::kProton ) ;
	 }
      }
   }

   m_kalmanTrackFitter->setHyposToFit( hyposToFit ) ;


   // Determine which (if any) hypo sets the hit list.
   if( m_kalmanProd->hitListFromOneHypo() )
   {
      m_kalmanTrackFitter->setHitListFromOneHypo( true ) ;

      if( m_kalmanProd->hitListHypo() == "electron" )
      {
	 m_kalmanTrackFitter->setHitListHypo( DBCandidate::kElectron ) ;
      }
      else if( m_kalmanProd->hitListHypo() == "muon" )
      {
	 m_kalmanTrackFitter->setHitListHypo( DBCandidate::kMuon ) ;
      }
      else if( m_kalmanProd->hitListHypo() == "pion" )
      {
	 m_kalmanTrackFitter->setHitListHypo( DBCandidate::kChargedPion ) ;
      }
      else if( m_kalmanProd->hitListHypo() == "kaon" )
      {
	 m_kalmanTrackFitter->setHitListHypo( DBCandidate::kChargedKaon ) ;
      }
      else if( m_kalmanProd->hitListHypo() == "proton" )
      {
	 m_kalmanTrackFitter->setHitListHypo( DBCandidate::kProton ) ;
      }
   }
   else
   {
      m_kalmanTrackFitter->setHitListFromOneHypo( false ) ;
   }
   
   // wsun 5-13-99: Don't call event( Frame& ), which fits all the tracks
   // under all hypotheses.  Instead, let each templated proxy call
   // event( Frame&, DBCandidate::Hypo ) which only fits the tracks under
   // the given hypothesis.  If the fits for the given hypothesis have already
   // been performed for this event, then nothing happens.

   if( m_kalmanProd->optimizeForAllHypos() )
   {
      // The function event() extracts the seed tracks from the Frame, filters
      // them, and fits each track under each hypothesis.  These KalmanHelix
      // objects are kept as data members of KalmanTrackFitter.  This
      // function should be called for pass2.

      // If the user only wants tracks fitted with one (or a few) hypothesis,
      // this function is not called because fitting all the other hypotheses
      // is a waste of time.  Instead, the track and lattice proxies for
      // each hypothesis calls a different event function that takes a
      // hypothesis as an argument.

      m_kalmanTrackFitter->event( ( Frame& ) iRecord.frame() ) ;
   }

   return ( m_kalmanTrackFitter ) ;
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
void
KalmanTrackFitterProxy::updateMagField( const Record& iRecord )
{
   HIMagField::instance()->setBfield( iRecord ) ;
}

void
KalmanTrackFitterProxy::updateSiGeom( const Record& iRecord )
{
   if( m_kalmanTrackFitter != 0 )
   {
      m_kalmanTrackFitter->updateSiSurfaceCache() ;
   }
}

void
KalmanTrackFitterProxy::updateIRGeom( const Record& iRecord )
{
   if( m_kalmanTrackFitter != 0 )
   {
      m_kalmanTrackFitter->updateIRSurfaceCache() ;
   }
}

void
KalmanTrackFitterProxy::updateDRGeom( const Record& iRecord )
{
   if( m_kalmanTrackFitter != 0 )
   {
      m_kalmanTrackFitter->updateDRSurfaceCache() ;
   }
}

//void
//KalmanTrackFitterProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
PROXY_BIND_METHOD( KalmanTrackFitterProxy )
//-----------------------------------------------

