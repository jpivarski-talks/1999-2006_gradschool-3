// -*- C++ -*-
//
// Package:     KalmanFilter
// Module:      KalmanCandidateQualityStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Aug  4 02:47:36 EDT 2000
// $Id: KalmanCandidateQualityStorageHelper.cc,v 1.6 2001/08/17 21:45:12 wsun Exp $
//
// Revision history
//
// $Log: KalmanCandidateQualityStorageHelper.cc,v $
// Revision 1.6  2001/08/17 21:45:12  wsun
// Fixed bug in packing for energy loss and arc length.
//
// Revision 1.5  2001/01/05 05:33:11  lyon
// Added LayerMap to quality object
//
// Revision 1.4  2000/12/22 07:02:30  wsun
// Updated to new TRTrackFitQuality.
//
// Revision 1.3  2000/12/14 16:19:15  wsun
// Added #hits, avg residuals, and rms residuals by subdetector to fit quality.
//
// Revision 1.2  2000/12/02 01:33:12  wsun
// Added moreBadHits and pullOfNextWorstHit to KalmanCandidateQuality.
//
// Revision 1.1  2000/08/16 20:07:48  wsun
// First submission.
//
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "KalmanFilter/Storage/KalmanCandidateQualityStorageHelper.h"
#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"
#include "StorageManagement/SMPackInfo.h" //defines sm_pack*
#include "StorageManagement/SMContents.h"
#include "StorageManagement/SMContentsBuiltIn.h"
#include "StorageManagement/SMFillContents.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "KalmanFilter.KalmanCandidateQualityStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanCandidateQualityStorageHelper.cc,v 1.6 2001/08/17 21:45:12 wsun Exp $";
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
KalmanCandidateQualityStorageHelper<H>::KalmanCandidateQualityStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &KalmanCandidateQualityStorageHelper::deliverV1 );
   registerVersion( &KalmanCandidateQualityStorageHelper::deliverV2 );
   registerVersion( &KalmanCandidateQualityStorageHelper::deliverV3 );
}

// KalmanCandidateQualityStorageHelper::KalmanCandidateQualityStorageHelper( const KalmanCandidateQualityStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//KalmanCandidateQualityStorageHelper::~KalmanCandidateQualityStorageHelper()
//{
//}

//
// assignment operators
//
// const KalmanCandidateQualityStorageHelper& KalmanCandidateQualityStorageHelper::operator=( const KalmanCandidateQualityStorageHelper& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
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
KalmanCandidateQualityStorageHelper<H>::implementStore( 
   SMSinkStream& iSink,
   const KalmanCandidateQuality<H>& iData ) 
{
  // Check that things haven't changed
  // If this assertion fails, then someone has changed the TRLayerMap object
  // to such an extent that this storage helper must be reworked!
  assert( TRSubdetectorLists::kMaxLayerMapSize == 3 );

  // Get the layer map array itself
  const UInt32* layerMap = iData.layerMap().begin();
   const STL_VECTOR( int ) numberHitsVec = iData.numberHitsVector() ;
   const STL_VECTOR( double ) avgResidualsVec = iData.averageResidualsVector();
   const STL_VECTOR( double ) rmsResidualsVec = iData.rmsResidualsVector() ;

   iSink 
      << sm_pack( SM_VAR( iData, identifier ), 1, 1000, 1 )
      << sm_pack( SM_VAR( iData, chiSquare ), 0., 5000., 1e-3 )
      << sm_pack( SM_VAR( iData, degreesOfFreedom ), -5, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberHits ), 0, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberHitsExpected ), 0, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberHitsInput ), 0, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberHitsDropped ), 0, 50, 1 )
      << sm_pack( SM_VAR( iData, numberIterations ), 0, 50, 1 )
      << sm_pack( sm_field( "fit", char( iData.fit() ? 1 : 0 ) ),
		  0, 1, 1 )
      << sm_pack( sm_field( "fitAbort", char( iData.fitAbort() ? 1 : 0 ) ),
		  0, 1, 1 )
      << sm_pack( SM_VAR( iData, energyLoss ), -0.1, 0.1, 0.00001 )
      << sm_pack( SM_VAR( iData, arcSwum ), -5., 5., 1.e-6 )
      << sm_pack( sm_field( "moreBadHits", char( iData.moreBadHits() ? 1:0 ) ),
		  0, 1, 1 )
      << sm_pack( SM_VAR( iData, pullOfNextWorstHit ), -1000., 1000., 0.1 )
      << sm_pack( sm_field( "detector", int( iData.detector() ) ),
		  0, TRSubdetectorLists::kNumberOfDetectors, 1 )
      << SMPackInfoFixedTemplate<int>(
	 sm_field( "numberHitsVector",
		   sm_contents_builtin( numberHitsVec.begin(), 
					numberHitsVec.end() ) ),
	 0, 1000, 1 )
      << SMPackInfoFixedTemplate<double>(
	 sm_field( "averageResidualsVector",
		   sm_contents_builtin( avgResidualsVec.begin(), 
					avgResidualsVec.end() ) ),
	 -0.01, 0.01, 1e-6 )
      << SMPackInfoFixedTemplate<double>(
	 sm_field( "rmsResidualsVector",
		   sm_contents_builtin( rmsResidualsVec.begin(), 
					rmsResidualsVec.end() ) ),
	 0.0, 0.01, 1e-6 )
      << sm_field( "layerMap1", layerMap[0] )

      << sm_field( "layerMap2", layerMap[1] )

      << sm_pack( sm_field( "layerMap3", layerMap[2] ),
		  0, 1<<7 - 1, 1)  // 1<<7-1 = 2^7-1 = 127
      ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanCandidateQuality<H>* 
KalmanCandidateQualityStorageHelper<H>::deliverV1( SMSourceStream& iSource)
{
   
   KalmanCandidateQuality<H>::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberHitsInput; 
   int numberHitsDropped;
   int numberIterations;
   char fit; 
   char fitAbort; 
   float energyLoss;
   float arcSwum;

   iSource 
      >> identifier 
      >> chiSquare 
      >> degreesOfFreedom 
      >> numberHits 
      >> numberHitsExpected 
      >> numberHitsInput 
      >> numberHitsDropped
      >> numberIterations
      >> fit 
      >> fitAbort 
      >> energyLoss
      >> arcSwum
	 ;
   
   return make( KalmanCandidateQuality<H>( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberHitsInput,  
      numberHitsDropped,
      numberIterations,
      fit,  
      fitAbort,
      energyLoss,
      arcSwum,
      false, // more bad hits
      0.,// pull of next worst hit
      TRSubdetectorLists::Detectors( TRSubdetectorLists::kCLEOIII ),// detector
      STL_VECTOR( int )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0 ),
      STL_VECTOR( double )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0. ),
      STL_VECTOR( double )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0. ),
      TRLayerMap() ) ) ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanCandidateQuality<H>* 
KalmanCandidateQualityStorageHelper<H>::deliverV2( SMSourceStream& iSource)
{
   
   KalmanCandidateQuality<H>::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberHitsInput; 
   int numberHitsDropped;
   int numberIterations;
   char fit; 
   char fitAbort; 
   float energyLoss;
   float arcSwum;
   char moreBadHits;
   float pullOfNextWorstHit;
   

   iSource 
      >> identifier 
      >> chiSquare 
      >> degreesOfFreedom 
      >> numberHits 
      >> numberHitsExpected 
      >> numberHitsInput 
      >> numberHitsDropped
      >> numberIterations
      >> fit 
      >> fitAbort 
      >> energyLoss
      >> arcSwum
      >> moreBadHits
      >> pullOfNextWorstHit
	 ;
   
   return make( KalmanCandidateQuality<H>( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberHitsInput,  
      numberHitsDropped,
      numberIterations,
      fit,  
      fitAbort,
      energyLoss,
      arcSwum,
      moreBadHits,
      pullOfNextWorstHit,
      TRSubdetectorLists::Detectors( TRSubdetectorLists::kCLEOIII ),// detector
      STL_VECTOR( int )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0 ),
      STL_VECTOR( double )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0. ),
      STL_VECTOR( double )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0. ),
      TRLayerMap() ) ) ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanCandidateQuality<H>* 
KalmanCandidateQualityStorageHelper<H>::deliverV3( SMSourceStream& iSource)
{
   
   KalmanCandidateQuality<H>::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberHitsInput; 
   int numberHitsDropped;
   int numberIterations;
   char fit; 
   char fitAbort; 
   float energyLoss;
   float arcSwum;
   char moreBadHits;
   float pullOfNextWorstHit;
   int detector;

   UInt32 layerMapWord1;
   UInt32 layerMapWord2;
   UInt32 layerMapWord3;

   iSource 
      >> identifier 
      >> chiSquare 
      >> degreesOfFreedom 
      >> numberHits 
      >> numberHitsExpected 
      >> numberHitsInput 
      >> numberHitsDropped
      >> numberIterations
      >> fit 
      >> fitAbort 
      >> energyLoss
      >> arcSwum
      >> moreBadHits
      >> pullOfNextWorstHit
      >> detector
      ;
   
   STL_VECTOR( int ) numberHitsVec ;
   STL_VECTOR( double ) avgResidualsVec ;
   STL_VECTOR( double ) rmsResidualsVec ;

   back_insert_iterator<STL_VECTOR(int)>
      numberHitsInsertItr( numberHitsVec ) ;
   back_insert_iterator<STL_VECTOR(double)>
      avgResidualsInsertItr( avgResidualsVec ) ;
   back_insert_iterator<STL_VECTOR(double)>
      rmsResidualsInsertItr( rmsResidualsVec ) ;

   iSource >> sm_fill_contents( numberHitsInsertItr )
	   >> sm_fill_contents( avgResidualsInsertItr )
	   >> sm_fill_contents( rmsResidualsInsertItr ) ;

   // Get the layer map information
   iSource >> layerMapWord1
	   >> layerMapWord2
	   >> layerMapWord3
     ;

  // If this assertion fails, then someone has changed the TRLayerMap object
  // to such an extent that this storage helper must be reworked!
  assert( TRSubdetectorLists::kMaxLayerMapSize == 3 );

  // Make a TRLayerMap object
  TRLayerMap layerMap(layerMapWord1, layerMapWord2, layerMapWord3);

   return make( KalmanCandidateQuality<H>( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberHitsInput,  
      numberHitsDropped,
      numberIterations,
      fit,  
      fitAbort,
      energyLoss,
      arcSwum,
      moreBadHits,
      pullOfNextWorstHit,
      TRSubdetectorLists::Detectors( detector ),
      numberHitsVec,
      avgResidualsVec,
      rmsResidualsVec,
      layerMap) ); 
}

//
// const member functions
//
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const KalmanCandidateQuality<H>& 
KalmanCandidateQualityStorageHelper<H>::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const KalmanCandidateQuality<H> temp(
      KalmanCandidateQuality<H>::Identifier ( 1 ), // identifier 
      double ( 2 ), // chiSquare 
      int ( 3 ), // degreesOfFreedom 
      int ( 4 ), // numberHits 
      int ( 5 ), // numberHitsExpected 
      int ( 6 ), // numberHitsInput 
      int ( 7 ), // numberHitsDropped
      int ( 8 ), // numberIterations
      DABoolean ( true ), // fit 
      DABoolean ( false ), // fitAbort 
      float( 9 ), // energyLoss
      float( 10 ), // arcSwum
      DABoolean( false ), // moreBadHits
      float( 0 ), // pullOfNextWorstHit
      TRSubdetectorLists::Detectors( TRSubdetectorLists::kCLEOIII ),// detector
      STL_VECTOR( int )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0 ),
      STL_VECTOR( double )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0.),
      STL_VECTOR( double )(
	 TRSubdetectorLists::numberOfHitTypes( TRSubdetectorLists::kCLEOIII ),
	 0. ),
      TRLayerMap() ) ;

   return temp;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
DABoolean 
KalmanCandidateQualityStorageHelper<H>::compare( 
   const KalmanCandidateQuality<H>* iNewData, 
   const KalmanCandidateQuality<H>& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for KalmanCandidateQuality
   return ( iNewData->identifier() == iOldData.identifier() ) ;
}

//
// static member functions
//

//KalmanCandidateQuality*
//SMStorageHelper<KalmanCandidateQuality>::make( const KalmanCandidateQuality& iTempData ) const
//{
   //change this line if KalmanCandidateQuality does not have a copy constructor
//   return new KalmanCandidateQuality( iTempData );
//}

// //------------------------------------------------------
// // DO NOT DELETE 
// SM_REGISTER_STORAGE( KalmanCandidateQualityStorageHelper );

// #include "StorageManagement/SMDeliverer.h"
// SM_REGISTER_VERSION_DEF( KalmanCandidateQualityStorageHelper, KalmanCandidateQuality )

// //this is done to create a TypeTag and its name
// #include "FrameAccess/instantiate_macros.h"
// INSTANTIATE_ONE(KalmanCandidateQuality)
// #include "StorageManagement/instantiate_one_typetag.h"

// #include "StorageManagement/Template/SMStorageHelper.cc"
// template class SMStorageHelper<KalmanCandidateQuality>;

// //------------------------------------------------------
