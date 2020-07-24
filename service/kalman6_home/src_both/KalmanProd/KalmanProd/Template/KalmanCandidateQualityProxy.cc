// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanCandidateQualityProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 01:58:33 EDT 1999
// $Id: KalmanCandidateQualityProxy.cc,v 1.10 2002/08/09 17:38:36 cdj Exp $
//
// Revision history
//
// $Log: KalmanCandidateQualityProxy.cc,v $
// Revision 1.10  2002/08/09 17:38:36  cdj
// replaced incorrect usage of reinterpret_cast with static_cast
//
// Revision 1.9  2002/06/12 20:31:09  cleo3
// added missing typename
//
// Revision 1.8  2001/03/27 06:43:02  wsun
// Extract Kalman objects with same production tag as KalmanProd.
//
// Revision 1.7  2001/01/05 05:44:49  lyon
// Added layer map
//
// Revision 1.6  2000/12/22 07:02:52  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.5  2000/12/14 16:25:28  wsun
// Added #hits, avg residuals, and rms residuals by subdetector to fit quality.
//
// Revision 1.4  2000/12/02 01:34:49  wsun
// Added moreBadHits and pullOfNextWorstHit to KalmanCandidateQuality.
//
// Revision 1.3  2000/09/05 20:47:35  wsun
// Catch exception if no TRSeedTrackQuality -- needed for FastMC.
//
// Revision 1.2  2000/08/16 20:19:39  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from KalmanFilter
//
// Revision 1.4  1999/08/30 21:50:56  wsun
// More accurate hit counts.
//
// Revision 1.3  1999/07/15 23:01:33  cdj
// now compiles with CC optimization on
//
// Revision 1.2  1999/06/18 19:27:47  wsun
// More improvements to low momentum fitting.
//
// Revision 1.1  1999/05/14 01:12:09  wsun
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
#include "FrameAccess/FAPtrTable.h"

#include "HelixIntersection/HIMagField.h"

#include "KalmanProd/KalmanCandidateQualityProxy.h"
#include "KalmanFilter/KalmanTrackFitter.h"

#include "SiHits/CalibratedSVHit.h"

#include "TrackRoot/TRSeedTrackQuality.h"

#include "DAException/DANoDataException.h"

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
static const char* const kFacilityString = "KalmanCandidateQualityProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanCandidateQualityProxy.cc,v 1.10 2002/08/09 17:38:36 cdj Exp $";
static const char* const kTagString = "$Name: v02_03_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanCandidateQualityProxy< H >::KalmanCandidateQualityProxy()
   : m_ptrTable( new value_type )
{
   if ( 0 == m_ptrTable ) {
      report( EMERGENCY, kFacilityString )
         << "Unable to allocate memory"
         << endl ;
      ::exit( 1 ) ;
   }

}

// KalmanCandidateQualityProxy::KalmanCandidateQualityProxy( const KalmanCandidateQualityProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanCandidateQualityProxy< H >::~KalmanCandidateQualityProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const KalmanCandidateQualityProxy& KalmanCandidateQualityProxy::operator=( const KalmanCandidateQualityProxy& iproxy )
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
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
void
KalmanCandidateQualityProxy< H >::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename KalmanCandidateQualityProxy< H >::value_type*
KalmanCandidateQualityProxy< H >::faultHandler( const Record& iRecord,
						const DataKey& iKey )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // ---------- fill FAPtrTable with new items ----------------

   // Get the KalmanTrackFitter, and call event().
   FAItem< KalmanTrackFitter > fitter;
   extract( iRecord, fitter, "", iKey.production().value() );

   // Get the seed track qualities so # hits expected can be copied.
   FATable< TRSeedTrackQuality > seedQualities ;
   DABoolean seedQualAvailable = true ;

   try
   {
      extract( iRecord, seedQualities ) ;
   }
   catch( NO_TABLE_EXCEPTION( TRSeedTrackQuality )& aException )
   {
      seedQualAvailable = false ;
   }

   // Fit the tracks under the hypothesis H.
   fitter->event( ( Frame& ) iRecord.frame(), ( TrackFitter::HypoType ) H ) ;

   // Get the vector of KalmanHelix pointers for this hypothesis.
   STL_VECTOR( KalmanHelix* )* kalmanHelices =
      fitter->fittedHelices( ( TrackFitter::HypoType ) H ) ;

   // Get the ADRSenseWireStore.
   FAItem< ADRSenseWireStore > wireStore ;
   extract( iRecord.frame().record( Stream::kDRAlignment ), wireStore ) ;

   // Link the fitted helices with their hits -- the TRHelixCandidateFit and
   // the corresponding KalmanHelix objects have the same identifier.
   STL_VECTOR( KalmanHelix* )::iterator helixItr = kalmanHelices->begin() ;
   STL_VECTOR( KalmanHelix* )::iterator helixEnd = kalmanHelices->end() ;

   for( ; helixItr != helixEnd ; ++helixItr )
   {
      int ndof = ( *helixItr )->ndof() ;
      DABoolean fitAbort = ndof <= 0 || ( *helixItr )->fitFailed() ;

      // Get the number of hits expected from the seed track quality.
      FAItem< TRSeedTrackQuality > seedQual ;

      if( seedQualAvailable )
      {
	 seedQual = seedQualities.find( ( *helixItr )->identifier() ) ;
      }

      int nHitsExp = 0 ;
      if( seedQual.valid() )
      {
	 nHitsExp = seedQual->numberHitsExpected() ;
      }

      int nIterations = ( *helixItr )->numberHitsDropped() +
	 ( *helixItr )->numberHitsFlipped() + 1 ;

      // Calculate average and rms residuals.
      int numberHits[ TRSubdetectorLists::kNumberOfHitTypes ] ;
      double avgResiduals[ TRSubdetectorLists::kNumberOfHitTypes ] ;
      double rmsResiduals[ TRSubdetectorLists::kNumberOfHitTypes ] ;

      for( int i = 0 ; i < TRSubdetectorLists::kNumberOfHitTypes ; ++i )
      {
	 numberHits[ i ] = 0 ;
	 avgResiduals[ i ] = 0. ;
	 rmsResiduals[ i ] = 0. ;
      }

      // Make a blank layer map
      TRLayerMap layerMap;

      STL_VECTOR( HIFitHelix::HitAndLinkData )::const_iterator hitItr =
	 ( *helixItr )->hitsOnTrack().begin() ;
      STL_VECTOR( HIFitHelix::HitAndLinkData )::const_iterator hitEnd =
	 ( *helixItr )->hitsOnTrack().end() ;

      for( ; hitItr != hitEnd ; ++hitItr )
      {
	 const TrackFitHitLink* linkData = hitItr->linkData() ;

	 if( linkData->used() )
	 {
	    double residual = linkData->residual() ;

	    ++numberHits[ TRSubdetectorLists::kEntireDetector ] ;
	    avgResiduals[ TRSubdetectorLists::kEntireDetector ] += residual ;
	    rmsResiduals[ TRSubdetectorLists::kEntireDetector ] +=
	       sqr( residual ) ;

	    // Find subdetector.
	    CalibratedHit::DeviceType type =
	       hitItr->calibratedHit()->deviceType() ;

	    if( type == CalibratedHit::DR )
	    {
	       const CalibratedDRHit* drHit =
		 static_cast<const CalibratedDRHit*>(
						 hitItr->calibratedHit()) ;

	       const AEWireLayerCylindrical& layer =
		  wireStore->layer( drHit->layer() ) ;

	       if( layer.isAxial() )
	       {
		 layerMap.setLayerHit(TRSubdetectorLists::kCLEOIII,
			  TRSubdetectorLists::kDR3Axial, drHit->layer());

		  ++numberHits[ TRSubdetectorLists::kDR3Axial ] ;
		  avgResiduals[ TRSubdetectorLists::kDR3Axial ] += residual ;
		  rmsResiduals[ TRSubdetectorLists::kDR3Axial ] +=
		     sqr( residual ) ;
	       }
	       else if( layer.isStereo() )
	       {

		 unsigned int stereoLayer = drHit->layer() - 
		   TRSubdetectorLists::layerMapNLayers(
				    TRSubdetectorLists::kCLEOIII,
				    TRSubdetectorLists::kDR3Axial );

		 layerMap.setLayerHit(TRSubdetectorLists::kCLEOIII,
			      TRSubdetectorLists::kDR3Stereo, stereoLayer);

		  ++numberHits[ TRSubdetectorLists::kDR3Stereo ] ;
		  avgResiduals[ TRSubdetectorLists::kDR3Stereo ] += residual ;
		  rmsResiduals[ TRSubdetectorLists::kDR3Stereo ] +=
		     sqr( residual ) ;
	       }
	    }
	    else if( type == CalibratedHit::Cathode )
	    {
	       layerMap.setLayerHit(TRSubdetectorLists::kCLEOIII,
				    TRSubdetectorLists::kDRCathode, 1);

	       ++numberHits[ TRSubdetectorLists::kDRCathode ] ;
	       avgResiduals[ TRSubdetectorLists::kDRCathode ] += residual ;
	       rmsResiduals[ TRSubdetectorLists::kDRCathode ] +=
		  sqr( residual ) ;
	    }
	    else if( type == CalibratedHit::SVR )
	    {
	      const CalibratedSVHit* svHit =
		  static_cast<const CalibratedSVHit*> (
					      hitItr->calibratedHit()) ;

	       layerMap.setLayerHit(TRSubdetectorLists::kCLEOIII,
				    TRSubdetectorLists::kSVR, svHit->layer() );

	       ++numberHits[ TRSubdetectorLists::kSVR ] ;
	       avgResiduals[ TRSubdetectorLists::kSVR ] += residual ;
	       rmsResiduals[ TRSubdetectorLists::kSVR ] +=
		  sqr( residual ) ;
	    }
	    else if( type == CalibratedHit::SVZ )
	    {
	      const CalibratedSVHit* svHit =
		  static_cast<const CalibratedSVHit*> (
					      hitItr->calibratedHit()) ;

	       layerMap.setLayerHit(TRSubdetectorLists::kCLEOIII,
				    TRSubdetectorLists::kSVZ, svHit->layer() );

	       ++numberHits[ TRSubdetectorLists::kSVZ ] ;
	       avgResiduals[ TRSubdetectorLists::kSVZ ] += residual ;
	       rmsResiduals[ TRSubdetectorLists::kSVZ ] +=
		  sqr( residual ) ;
	    }
	 }
      }

      if( numberHits[ TRSubdetectorLists::kEntireDetector ] > 0 )
      {
	 avgResiduals[ TRSubdetectorLists::kEntireDetector ] /=
	    numberHits[ TRSubdetectorLists::kEntireDetector ] ;

	 if( numberHits[ TRSubdetectorLists::kEntireDetector ] > 1 )
	 {
	    rmsResiduals[ TRSubdetectorLists::kEntireDetector ] =
	       sqrt( rmsResiduals[ TRSubdetectorLists::kEntireDetector ]/
		     numberHits[ TRSubdetectorLists::kEntireDetector ] -
		     sqr( avgResiduals[ TRSubdetectorLists::kEntireDetector ] )
		  ) ;
	 }
	 else
	 {
	    rmsResiduals[ TRSubdetectorLists::kEntireDetector ] = 0. ;
	 }
      }

      if( numberHits[ TRSubdetectorLists::kDR3Axial ] > 0 )
      {
	 avgResiduals[ TRSubdetectorLists::kDR3Axial ] /=
	    numberHits[ TRSubdetectorLists::kDR3Axial ] ;

	 if( numberHits[ TRSubdetectorLists::kDR3Axial ] > 1 )
	 {
	    rmsResiduals[ TRSubdetectorLists::kDR3Axial ] =
	       sqrt( rmsResiduals[ TRSubdetectorLists::kDR3Axial ] /
		     numberHits[ TRSubdetectorLists::kDR3Axial ] -
		     sqr( avgResiduals[ TRSubdetectorLists::kDR3Axial ] ) ) ;
	 }
	 else
	 {
	    rmsResiduals[ TRSubdetectorLists::kDR3Axial ] = 0. ;
	 }
      }

      if( numberHits[ TRSubdetectorLists::kDR3Stereo ] > 0 )
      {
	 avgResiduals[ TRSubdetectorLists::kDR3Stereo ] /=
	    numberHits[ TRSubdetectorLists::kDR3Stereo ] ;

	 if( numberHits[ TRSubdetectorLists::kDR3Stereo ] > 1 )
	 {
	    rmsResiduals[ TRSubdetectorLists::kDR3Stereo ] =
	       sqrt( rmsResiduals[ TRSubdetectorLists::kDR3Stereo ] /
		     numberHits[ TRSubdetectorLists::kDR3Stereo ] -
		     sqr( avgResiduals[ TRSubdetectorLists::kDR3Stereo ] ) ) ;
	 }
	 else
	 {
	    rmsResiduals[ TRSubdetectorLists::kDR3Stereo ] = 0. ;
	 }
      }

      if( numberHits[ TRSubdetectorLists::kDRCathode ] > 0 )
      {
	 avgResiduals[ TRSubdetectorLists::kDRCathode ] /=
	    numberHits[ TRSubdetectorLists::kDRCathode ] ;

	 if( numberHits[ TRSubdetectorLists::kDRCathode ] > 1 )
	 {
	    rmsResiduals[ TRSubdetectorLists::kDRCathode ] =
	       sqrt( rmsResiduals[ TRSubdetectorLists::kDRCathode ] /
		     numberHits[ TRSubdetectorLists::kDRCathode ] -
		     sqr( avgResiduals[ TRSubdetectorLists::kDRCathode ] ) ) ;
	 }
	 else
	 {
	    rmsResiduals[ TRSubdetectorLists::kDRCathode ] = 0. ;
	 }
      }

      if( numberHits[ TRSubdetectorLists::kSVR ] > 0 )
      {
	 avgResiduals[ TRSubdetectorLists::kSVR ] /=
	    numberHits[ TRSubdetectorLists::kSVR ] ;

	 if( numberHits[ TRSubdetectorLists::kSVR ] > 1 )
	 {
	    rmsResiduals[ TRSubdetectorLists::kSVR ] =
	       sqrt( rmsResiduals[ TRSubdetectorLists::kSVR ] /
		     numberHits[ TRSubdetectorLists::kSVR ] -
		     sqr( avgResiduals[ TRSubdetectorLists::kSVR ] ) ) ;
	 }
	 else
	 {
	    rmsResiduals[ TRSubdetectorLists::kSVR ] = 0. ;
	 }
      }

      if( numberHits[ TRSubdetectorLists::kSVZ ] > 0 )
      {
	 avgResiduals[ TRSubdetectorLists::kSVZ ] /=
	    numberHits[ TRSubdetectorLists::kSVZ ] ;

	 if( numberHits[ TRSubdetectorLists::kSVZ ] > 1 )
	 {
	    rmsResiduals[ TRSubdetectorLists::kSVZ ] =
	       sqrt( rmsResiduals[ TRSubdetectorLists::kSVZ ] /
		     numberHits[ TRSubdetectorLists::kSVZ ] -
		     sqr( avgResiduals[ TRSubdetectorLists::kSVZ ] ) ) ;
	 }
	 else
	 {
	    rmsResiduals[ TRSubdetectorLists::kSVZ ] = 0. ;
	 }
      }

      // Fill vectors to be given to the quality object ctor.
      STL_VECTOR( int ) numberHitsVector ;
      numberHitsVector.reserve(TRSubdetectorLists::kNumberOfHitTypes);

      STL_VECTOR( double ) avgResidualsVector ;
      avgResidualsVector.reserve(TRSubdetectorLists::kNumberOfHitTypes);

      STL_VECTOR( double ) rmsResidualsVector ;
      rmsResidualsVector.reserve(TRSubdetectorLists::kNumberOfHitTypes);


      for( int j = 0 ; j < TRSubdetectorLists::kNumberOfHitTypes ; ++j )
      {
	 if( TRSubdetectorLists::subdetectorValid(
	    TRSubdetectorLists::kCLEOIII,
	    TRSubdetectorLists::HitTypes( j ) ) )
	 {
	    numberHitsVector.push_back( numberHits[ j ] ) ;
	    avgResidualsVector.push_back( avgResiduals[ j ] ) ;
	    rmsResidualsVector.push_back( rmsResiduals[ j ] ) ;
	 }
      }

      KalmanCandidateQuality< H >* tmp = new KalmanCandidateQuality< H >(
                  ( *helixItr )->identifier(),
                  ( *helixItr )->chisq(),
                  ndof,
                  ( *helixItr )->numberOfHitsInFit(),   // # hits
		  nHitsExp,                             // # hits expected
                  ( *helixItr )->hitsOnTrack().size(),  // # hits input
		  ( *helixItr )->numberHitsDropped(),   // # hits dropped
		  nIterations,                          // # iterations
                  true,                                 // fit
                  fitAbort,                             // fit abort
                  ( *helixItr )->totalDeltaEnergy(),    // energy loss
                  ( *helixItr )->totalArcLength(),      // arc swum
		  ( *helixItr )->moreBadHits(),
		  ( *helixItr )->pullOfNextWorstHit(),
		  TRSubdetectorLists::kCLEOIII,
		  numberHitsVector,
		  avgResidualsVector,
		  rmsResidualsVector,
		  layerMap
         ) ;

      if ( 0 == tmp ) 
      {
         report( EMERGENCY, kFacilityString )
            << "No memory to allocate another object"
            << endl ;
         ::exit( 1 ) ;
      }
      
      ( *m_ptrTable ).insert( tmp ) ;
   }

   return ( m_ptrTable ) ;
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
//KalmanCandidateQualityProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}



