// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqTrackFitterProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 14:59:33 EDT 1999
// $Id: ChisqTrackFitterProxy.cc,v 1.10 2003/07/25 23:27:51 mccann Exp $
//
// Revision history
//
// $Log: ChisqTrackFitterProxy.cc,v $
// Revision 1.10  2003/07/25 23:27:51  mccann
// Added the ZD to track fitting
//
// Revision 1.9  2001/03/11 14:20:21  cdj
// now get MagneticField from StartRun
//
// Revision 1.8  2000/11/18 00:21:39  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.7  2000/09/05 20:43:22  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.6  2000/08/16 21:06:21  wsun
// Added usage and production tags for TRSeedTracks.
//
// Revision 1.5  2000/06/10 06:27:45  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.4  2000/01/11 20:18:39  wsun
// Replaced control flags with a pointer to ChisqFitProducer.
//
// Revision 1.3  1999/07/07 16:45:18  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.2  1999/06/24 22:56:35  wsun
// Added use of HIMagField.
//
// Revision 1.1.1.1  1999/06/16 02:48:27  wsun
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
#include "ChisqFitProducer/ChisqFitProducer.h"
#include "ChisqFitProducer/ChisqTrackFitterProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

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
static const char* const kFacilityString = "ChisqFitProducer.ChisqTrackFitterProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqTrackFitterProxy.cc,v 1.10 2003/07/25 23:27:51 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

// ChisqTrackFitterProxy::ChisqTrackFitterProxy( const ChisqTrackFitterProxy& iproxy )
// {
//   *this = iproxy;
// }

ChisqTrackFitterProxy::ChisqTrackFitterProxy(
   ChisqFitProducer* aChisqFitProducer )
   : m_ChisqTrackFitter( 0 ),
     m_chisqFitProducer( aChisqFitProducer )
{
   if( m_chisqFitProducer != 0 &&
       m_chisqFitProducer->bfieldMagnitude() != 0. ) // default value from ChisqFitProducer
   {
      // This will override the MagneticField object in the Frame.
      HIMagField::instance()->setBfield(
	 m_chisqFitProducer->bfieldMagnitude() ) ;
   }

   bind( &ChisqTrackFitterProxy::updateMagField, Stream::kStartRun ) ;
}

ChisqTrackFitterProxy::~ChisqTrackFitterProxy()
{
   HIMagField::instance()->destroy() ;
   delete m_ChisqTrackFitter ;
}

//
// assignment operators
//
// const ChisqTrackFitterProxy& ChisqTrackFitterProxy::operator=( const ChisqTrackFitterProxy& iproxy )
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
ChisqTrackFitterProxy::invalidateCache()
{
//    delete m_ChisqTrackFitter ;
//    m_ChisqTrackFitter = 0 ;

   if( m_ChisqTrackFitter != 0 )
   {
      m_ChisqTrackFitter->clearCache() ;
   }
}

const ChisqTrackFitterProxy::value_type*
ChisqTrackFitterProxy::faultHandler( const Record& iRecord,
				     const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
//   assert( 0 == m_ChisqTrackFitter );

   if( 0 == m_ChisqTrackFitter )
   {
      // ---------- create the new object --------------------
      m_ChisqTrackFitter = new ChisqTrackFitter(
	 false,  // include initial error matrix?
	 m_chisqFitProducer->maxDeltaChisqNdof(),
	 m_chisqFitProducer->maxIterations(),
	 m_chisqFitProducer->maxHitChisq() ) ;
   }

   m_ChisqTrackFitter->setPrintDiagnostics(
      m_chisqFitProducer->printDiagnostics() ) ;

   m_ChisqTrackFitter->setUseDRAxialHits(
      m_chisqFitProducer->useDRAxialHits() ) ;
   m_ChisqTrackFitter->setUseDRStereoUHits(
      m_chisqFitProducer->useDRStereoUHits() ) ;
   m_ChisqTrackFitter->setUseDRStereoVHits(
      m_chisqFitProducer->useDRStereoVHits() ) ;
   m_ChisqTrackFitter->setCorrectDRHits(
      m_chisqFitProducer->correctDRHits() ) ;

   m_ChisqTrackFitter->setUseZDStereoUHits(
      m_chisqFitProducer->useZDStereoUHits() ) ;
   m_ChisqTrackFitter->setUseZDStereoVHits(
      m_chisqFitProducer->useZDStereoVHits() ) ;
   m_ChisqTrackFitter->setCorrectZDHits(
      m_chisqFitProducer->correctZDHits() ) ;

   m_ChisqTrackFitter->setUseDRCathodeHits(
      m_chisqFitProducer->useDRCathodeHits() ) ;
   m_ChisqTrackFitter->setCorrectDRCathodeHits(
      m_chisqFitProducer->correctDRCathodeHits() ) ;

   m_ChisqTrackFitter->setUseSVRPhiHits(
      m_chisqFitProducer->useSVRPhiHits() ) ;
   m_ChisqTrackFitter->setUseSVZHits( m_chisqFitProducer->useSVZHits() ) ;
   m_ChisqTrackFitter->setCorrectSVRHits(
      m_chisqFitProducer->correctSVRHits() ) ;
   m_ChisqTrackFitter->setCorrectSVZHits(
      m_chisqFitProducer->correctSVZHits() ) ;

   if( m_chisqFitProducer->fixCurvature() ) m_ChisqTrackFitter->fixCurvature();
   if( m_chisqFitProducer->fixPhi0() )  m_ChisqTrackFitter->fixPhi0() ;
   if( m_chisqFitProducer->fixD0() )    m_ChisqTrackFitter->fixD0() ;
   if( m_chisqFitProducer->fixCotTheta() ) m_ChisqTrackFitter->fixCotTheta() ;
   if( m_chisqFitProducer->fixZ0() )    m_ChisqTrackFitter->fixZ0() ;

   m_ChisqTrackFitter->setContinueIfHitsMissing(
      m_chisqFitProducer->continueIfHitsMissing() ) ;
   m_ChisqTrackFitter->setContinueIfAnyException(
      m_chisqFitProducer->continueIfAnyException() ) ;

   m_ChisqTrackFitter->setDRHitUsageTag(
      m_chisqFitProducer->drHitUsageTag() ) ;
   m_ChisqTrackFitter->setDRHitProductionTag(
      m_chisqFitProducer->drHitProductionTag() ) ;
   m_ChisqTrackFitter->setZDHitUsageTag(
      m_chisqFitProducer->zdHitUsageTag() ) ;
   m_ChisqTrackFitter->setZDHitProductionTag(
      m_chisqFitProducer->zdHitProductionTag() ) ;
   m_ChisqTrackFitter->setCathodeHitUsageTag(
      m_chisqFitProducer->cathodeHitUsageTag() ) ;
   m_ChisqTrackFitter->setCathodeHitProductionTag(
      m_chisqFitProducer->cathodeHitProductionTag() ) ;
   m_ChisqTrackFitter->setSVRHitUsageTag(
      m_chisqFitProducer->svrHitUsageTag() ) ;
   m_ChisqTrackFitter->setSVRHitProductionTag(
      m_chisqFitProducer->svrHitProductionTag() ) ;
   m_ChisqTrackFitter->setSVZHitUsageTag(
      m_chisqFitProducer->svzHitUsageTag() ) ;
   m_ChisqTrackFitter->setSVZHitProductionTag(
      m_chisqFitProducer->svzHitProductionTag() ) ;

   m_ChisqTrackFitter->setSeedTrackUsageTag(
      m_chisqFitProducer->seedTrackUsageTag() ) ;
   m_ChisqTrackFitter->setSeedTrackProductionTag(
      m_chisqFitProducer->seedTrackProductionTag() ) ;

   if( m_chisqFitProducer->forceDRFittingWeight() )
   {
      m_ChisqTrackFitter->setDRFittingWeight(
	 m_chisqFitProducer->drFittingWeight() ) ;
   }

   if( m_chisqFitProducer->forceZDFittingWeight() )
   {
      m_ChisqTrackFitter->setZDFittingWeight(
	 m_chisqFitProducer->zdFittingWeight() ) ;
   }

   if( m_chisqFitProducer->forceSVRFittingWeight() )
   {
      m_ChisqTrackFitter->setSVRFittingWeight(
	 m_chisqFitProducer->svrFittingWeight() ) ;
   }

   if( m_chisqFitProducer->forceSVZFittingWeight() )
   {
      m_ChisqTrackFitter->setSVZFittingWeight(
	 m_chisqFitProducer->svzFittingWeight() ) ;
   }

   if( m_chisqFitProducer->forceCathodeFittingWeight() )
   {
      m_ChisqTrackFitter->setCathodeFittingWeight(
	 m_chisqFitProducer->cathodeFittingWeight() ) ;
   }

   if( m_chisqFitProducer->scaleCathodeResolution() )
   {
      m_ChisqTrackFitter->setCathodeFittingWeightScale(
	 1. / sqr( m_chisqFitProducer->cathodeResolutionScale() ) ) ;
   }

   // The function event() extracts the seed tracks from the Frame, filters
   // them, and fits each track.  The resultant ChisqHelix
   // objects are kept as data members of ChisqTrackFitter.

   m_ChisqTrackFitter->event( ( Frame& ) iRecord.frame(),
			       DBCandidate::kChargedPion ) ;

   return ( m_ChisqTrackFitter ) ;
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
ChisqTrackFitterProxy::updateMagField( const Record& iRecord )
{
   HIMagField::instance()->setBfield( iRecord ) ;
}

//void
//ChisqTrackFitterProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ChisqTrackFitter >;

PROXY_BIND_METHOD( ChisqTrackFitterProxy )
//-----------------------------------------------

