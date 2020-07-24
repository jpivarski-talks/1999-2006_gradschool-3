// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqCandidateQualityProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 16:17:26 EDT 1999
// $Id: ChisqCandidateQualityProxy.cc,v 1.12 2003/07/25 23:27:48 mccann Exp $
//
// Revision history
//
// $Log: ChisqCandidateQualityProxy.cc,v $
// Revision 1.12  2003/07/25 23:27:48  mccann
// Added the ZD to track fitting
//
// Revision 1.11  2002/08/09 14:29:21  cdj
// replaced C style casts with static_cast
//
// Revision 1.10  2002/08/08 13:39:14  cleo3
// removed invalid use of reinterpret_cast
//
// Revision 1.9  2002/06/12 19:26:39  cleo3
// added missing typename
//
// Revision 1.8  2001/01/05 05:41:44  lyon
// Added layer map
//
// Revision 1.7  2000/12/22 07:03:33  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.6  2000/12/14 20:25:03  wsun
// Added #hits, avg residuals, and rms residuals by subdetector to fit quality.
//
// Revision 1.5  2000/09/05 21:02:18  wsun
// Catch exception if no TRSeedTrackQuality -- needed for FastMC.
//
// Revision 1.4  2000/08/16 21:05:58  wsun
// Updated to new TRSeedTrackQuality.
//
// Revision 1.3  1999/08/30 22:14:57  wsun
// More accurate hit counting.
//
// Revision 1.2  1999/07/17 20:28:30  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:45:22  wsun
// First submission.
//
// Revision 1.1.1.1  1999/06/16 02:48:28  wsun
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

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"

#include "ChisqFitProducer/ChisqCandidateQualityProxy.h"
#include "ChisqFitter/ChisqTrackFitter.h"

#include "SiHits/CalibratedSVHit.h"

#include "TrackRoot/TRSeedTrackQuality.h"

#include "DAException/DANoDataException.h"

#include "DetectorConfiguration/DetectorConfiguration.h"

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
static const char* const kFacilityString = "ChisqFitProducer.ChisqCandidateQualityProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqCandidateQualityProxy.cc,v 1.12 2003/07/25 23:27:48 mccann Exp $";
static const char* const kTagString = "$Name:  $";

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
ChisqCandidateQualityProxy< H >::ChisqCandidateQualityProxy()
   : m_ptrTable( new value_type )
{
   if ( 0 == m_ptrTable ) {
      report( EMERGENCY, kFacilityString )
         << "Unable to allocate memory"
         << endl ;
      ::exit( 1 ) ;
   }

}

// ChisqCandidateQualityProxy::ChisqCandidateQualityProxy( const ChisqCandidateQualityProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
ChisqCandidateQualityProxy< H >::~ChisqCandidateQualityProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const ChisqCandidateQualityProxy& ChisqCandidateQualityProxy::operator=( const ChisqCandidateQualityProxy& iproxy )
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
ChisqCandidateQualityProxy< H >::invalidateCache()
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
const typename ChisqCandidateQualityProxy< H >::value_type*
ChisqCandidateQualityProxy< H >::faultHandler( const Record& iRecord,
					       const DataKey& iKey )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // ---------- fill FAPtrTable with new items ----------------

   const Frame& theFrame = iRecord.frame();
   const Record& baseGeometryRecord = theFrame.record(Stream::kBaseGeometry);
   FAItem< DetectorConfiguration > detectorConfig;
   extract(baseGeometryRecord, detectorConfig);

   TRSubdetectorLists::Detectors detectorGeneration(
      detectorConfig->useZDnotSI() ? TRSubdetectorLists::kCLEOc : TRSubdetectorLists::kCLEOIII);

   // Get the ChisqTrackFitter, which will have had its event()
   // function called.
   FAItem< ChisqTrackFitter > fitter;
   extract( iRecord, fitter );

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

   // Get the vector of ChisqHelix pointers for this hypothesis.
   STL_VECTOR( ChisqHelix* )* chisqHelices = fitter->fittedHelices() ;

   // Get the ADRSenseWireStore.
   FAItem< ADRSenseWireStore > wireStore ;
   extract( iRecord.frame().record( Stream::kDRAlignment ), wireStore ) ;

   // Link the fitted helices with their hits -- the TRHelixCandidateFit and
   // the corresponding ChisqHelix objects have the same identifier.
   STL_VECTOR( ChisqHelix* )::iterator helixItr = chisqHelices->begin() ;
   STL_VECTOR( ChisqHelix* )::iterator helixEnd = chisqHelices->end() ;

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
      
      // Loop over hits
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
		 layerMap.setLayerHit(detectorGeneration,
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
				    detectorGeneration,
				    TRSubdetectorLists::kDR3Axial );

		 layerMap.setLayerHit(detectorGeneration,
			      TRSubdetectorLists::kDR3Stereo, stereoLayer);

		  ++numberHits[ TRSubdetectorLists::kDR3Stereo ] ;
		  avgResiduals[ TRSubdetectorLists::kDR3Stereo ] += residual ;
		  rmsResiduals[ TRSubdetectorLists::kDR3Stereo ] +=
		     sqr( residual ) ;
	       }
	    }
	    else if( type == CalibratedHit::Cathode )
	    {
	       layerMap.setLayerHit(detectorGeneration,
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

	       layerMap.setLayerHit(detectorGeneration,
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

	       layerMap.setLayerHit(detectorGeneration,
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
      STL_VECTOR( double ) avgResidualsVector ;
      STL_VECTOR( double ) rmsResidualsVector ;

      for( int j = 0 ; j < TRSubdetectorLists::kNumberOfHitTypes ; ++j )
      {
	 if( TRSubdetectorLists::subdetectorValid(
	    detectorGeneration,
	    TRSubdetectorLists::HitTypes( j ) ) )
	 {
	    numberHitsVector.push_back( numberHits[ j ] ) ;
	    avgResidualsVector.push_back( avgResiduals[ j ] ) ;
	    rmsResidualsVector.push_back( rmsResiduals[ j ] ) ;
	 }
      }

      ChisqCandidateQuality< H >* tmp = new ChisqCandidateQuality< H >(
                  ( *helixItr )->identifier(),
                  ( *helixItr )->chisq(),
                  ndof,
                  ( *helixItr )->numberOfHitsInFit(),    // # hits
		  nHitsExp,
                  ( *helixItr )->hitsOnTrack().size(),    // # hits input
		  ( *helixItr )->numberHitsDropped(),
		  ( *helixItr )->numberIterations(),
                  true,        // fit
                  fitAbort,    // fit abort
		  detectorGeneration,
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



