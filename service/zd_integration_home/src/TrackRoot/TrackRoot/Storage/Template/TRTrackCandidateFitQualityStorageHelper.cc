// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackCandidateFitQualityStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Feb 24 00:12:46 EST 2000
// $Id: TRTrackCandidateFitQualityStorageHelper.cc,v 1.12 2003/06/18 17:37:51 cdj Exp $
//
// Revision history
//
// $Log: TRTrackCandidateFitQualityStorageHelper.cc,v $
// Revision 1.12  2003/06/18 17:37:51  cdj
// updated storage helper to readback same type as written out
//
// Revision 1.11  2002/04/02 15:04:55  cdj
// minor speed optimization for reading back data from storage
//
// Revision 1.10  2001/01/05 05:27:39  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.9  2000/12/22 06:56:15  wsun
// New hit and residual functions that can be used with CLEO II, II.V, or III.
//
// Revision 1.8  2000/12/14 16:01:31  wsun
// Added #hits, average residuals, and rms residuals by subdetector.
//
// Revision 1.7  2000/08/06 00:42:49  wsun
// Typo in sm_pack for DABoolean fit().
//
// Revision 1.6  2000/08/01 23:29:40  wsun
// Updated to new TRTrackCandidateFitQuality.
//
// Revision 1.5  2000/07/13 18:45:50  wsun
// Fixed compression hint: ndof can be -5.
//
// Revision 1.4  2000/05/04 22:48:07  wsun
// Force boolean fields to be 0 or 1 for storage.
//
// Revision 1.3  2000/05/03 23:11:30  wsun
// More reasonable packing hints.
//
// Revision 1.2  2000/04/28 22:07:37  wsun
// Added compression hints.
//
// Revision 1.1  2000/02/25 01:08:56  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"
#include <assert.h>

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
#include "Experiment/report.h"
#include "TrackRoot/Storage/TRTrackCandidateFitQualityStorageHelper.h"
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

static const char* const kFacilityString = "TrackRoot.TRTrackCandidateFitQualityStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRTrackCandidateFitQualityStorageHelper.cc,v 1.12 2003/06/18 17:37:51 cdj Exp $";
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
TRTrackCandidateFitQualityStorageHelper<H>::TRTrackCandidateFitQualityStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &TRTrackCandidateFitQualityStorageHelper::deliverV1 );
   registerVersion( &TRTrackCandidateFitQualityStorageHelper::deliverV2 );
   registerVersion( &TRTrackCandidateFitQualityStorageHelper::deliverV3 );
}

// TRTrackCandidateFitQualityStorageHelper::TRTrackCandidateFitQualityStorageHelper( const TRTrackCandidateFitQualityStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//TRTrackCandidateFitQualityStorageHelper::~TRTrackCandidateFitQualityStorageHelper()
//{
//}

//
// assignment operators
//
// const TRTrackCandidateFitQualityStorageHelper& TRTrackCandidateFitQualityStorageHelper::operator=( const TRTrackCandidateFitQualityStorageHelper& rhs )
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
TRTrackCandidateFitQualityStorageHelper<H>::implementStore( 
   SMSinkStream& iSink,
   const TRTrackCandidateFitQuality<H>& iData ) 
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
TRTrackCandidateFitQuality<H>* 
TRTrackCandidateFitQualityStorageHelper<H>::deliverV1( SMSourceStream& iSource)
{
   
   TRTrackCandidateFitQuality<H>::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   char zEndpointUsed; 
   char originUsed; 
   int numberHitsInput; 
   char fit; 
   char fitAbort; 

   iSource 
      >> identifier 
      >> chiSquare 
      >> degreesOfFreedom 
      >> numberHits 
      >> numberHitsExpected 
      >> zEndpointUsed 
      >> originUsed 
      >> numberHitsInput 
      >> fit 
      >> fitAbort 
         ;
   
   return make( TRTrackCandidateFitQuality<H>( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberHitsInput,  
      0, // number hits dropped
      0, // number iterations
      fit,  
      fitAbort,
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
      TRLayerMap() ) );
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
TRTrackCandidateFitQuality<H>* 
TRTrackCandidateFitQualityStorageHelper<H>::deliverV2( SMSourceStream& iSource)
{
   
   TRTrackCandidateFitQuality<H>::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberHitsInput; 
   int numberHitsDropped;
   int numberIterations;
   char fit; 
   char fitAbort; 

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
	 ;
   
   return make( TRTrackCandidateFitQuality<H>( 
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
      TRLayerMap() ) );
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
TRTrackCandidateFitQuality<H>* 
TRTrackCandidateFitQualityStorageHelper<H>::deliverV3( SMSourceStream& iSource)
{
   TRTrackCandidateFitQuality<H>::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   unsigned int numberHitsInput; 
   unsigned int numberHitsDropped;
   unsigned int numberIterations;
   char fit; 
   char fitAbort; 
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
      >> detector
      ;
   
   static STL_VECTOR( int ) numberHitsVec(
      TRSubdetectorLists::kNumberOfHitTypes, int(0) );
   numberHitsVec.erase(numberHitsVec.begin(), numberHitsVec.end());

   static STL_VECTOR( double ) avgResidualsVec(
      TRSubdetectorLists::kNumberOfHitTypes, double(0));
   avgResidualsVec.erase(avgResidualsVec.begin(), avgResidualsVec.end());

   static STL_VECTOR( double ) rmsResidualsVec(
      TRSubdetectorLists::kNumberOfHitTypes, double(0));
   rmsResidualsVec.erase(rmsResidualsVec.begin(), rmsResidualsVec.end());

   back_insert_iterator<STL_VECTOR(int)>
      numberHitsInsertItr( numberHitsVec ) ;
   back_insert_iterator<STL_VECTOR(double)>
      avgResidualsInsertItr( avgResidualsVec ) ;
   back_insert_iterator<STL_VECTOR(double)>
      rmsResidualsInsertItr( rmsResidualsVec ) ;

   iSource >> sm_fill_contents( numberHitsInsertItr )
	   >> sm_fill_contents( avgResidualsInsertItr )
	   >> sm_fill_contents( rmsResidualsInsertItr );

   // Check that things haven't changed
   
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

  // Make the Quality object
  return new TRTrackCandidateFitQuality<H>( 
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
      TRSubdetectorLists::Detectors( detector ),
      numberHitsVec,
      avgResidualsVec,
      rmsResidualsVec,
      layerMap ) ;
}

//
// const member functions
//
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const TRTrackCandidateFitQuality<H>& 
TRTrackCandidateFitQualityStorageHelper<H>::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const TRTrackCandidateFitQuality<H> temp(
      TRTrackCandidateFitQuality<H>::Identifier ( 1 ), // identifier 
      double ( 2 ), // chiSquare 
      int ( 3 ), // degreesOfFreedom 
      int ( 4 ), // numberHits 
      int ( 5 ), // numberHitsExpected 
      int ( 6 ), // numberHitsInput 
      int ( 7 ), // numberHitsDropped
      int ( 8 ), // numberIterations
      DABoolean ( true ), // fit 
      DABoolean ( false ),// fitAbort 
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
      TRLayerMap() );

   return temp;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
DABoolean 
TRTrackCandidateFitQualityStorageHelper<H>::compare( 
   const TRTrackCandidateFitQuality<H>* iNewData, 
   const TRTrackCandidateFitQuality<H>& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for TRTrackCandidateFitQuality
   return ( iNewData->identifier() == iOldData.identifier() ) ;
}

//
// static member functions
//

//TRTrackCandidateFitQuality*
//SMStorageHelper<TRTrackCandidateFitQuality>::make( const TRTrackCandidateFitQuality& iTempData ) const
//{
   //change this line if TRTrackCandidateFitQuality does not have a copy constructor
//   return new TRTrackCandidateFitQuality( iTempData );
//}

// //------------------------------------------------------
// // DO NOT DELETE 
// SM_REGISTER_STORAGE( TRTrackCandidateFitQualityStorageHelper );

// #include "StorageManagement/SMDeliverer.h"
// SM_REGISTER_VERSION_DEF( TRTrackCandidateFitQualityStorageHelper, TRTrackCandidateFitQuality )

// //this is done to create a TypeTag and its name
// #include "FrameAccess/instantiate_macros.h"
// INSTANTIATE_ONE(TRTrackCandidateFitQuality)
// #include "StorageManagement/instantiate_one_typetag.h"

// #include "StorageManagement/Template/SMStorageHelper.cc"
// template class SMStorageHelper<TRTrackCandidateFitQuality>;

// //------------------------------------------------------
