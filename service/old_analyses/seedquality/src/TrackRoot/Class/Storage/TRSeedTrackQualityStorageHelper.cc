// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRSeedTrackQualityStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: TRSeedTrackQualityStorageHelper.cc,v 1.4 2000/10/31 21:43:29 ajm36 Exp $
//
// Revision history
//
// $Log: TRSeedTrackQualityStorageHelper.cc,v $
// Revision 1.4  2000/10/31 21:43:29  ajm36
// use better defaults for old qualities
//
// Revision 1.3  2000/10/30 17:35:27  ajm36
// added version of delivery for old qualities
//
// Revision 1.2  2000/10/28 20:34:46  ajm36
// added numberLayers, numberMultipleHits, useForExtrapolation to seed qualities
//
// Revision 1.2  2000/01/21 21:07:37  cdj
// helper skeletons now include SMPackInfo.h
//
// Revision 1.1  1999/10/18 18:13:04  cdj
// first submission
//
// Revision 1.2  1999/09/30 22:38:33  cdj
// no longer need default constructor
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
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
#include "TrackRoot/Storage/TRSeedTrackQualityStorageHelper.h"
#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"
#include "StorageManagement/SMPackInfo.h" //defines sm_pack*

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

static const char* const kFacilityString = "TrackRoot.TRSeedTrackQualityStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRSeedTrackQualityStorageHelper.cc,v 1.4 2000/10/31 21:43:29 ajm36 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TRSeedTrackQualityStorageHelper::TRSeedTrackQualityStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &TRSeedTrackQualityStorageHelper::deliverV1 );

   //v2 has numberLayers, numberMultipleHits, useForExtrapolation added
   registerVersion( &TRSeedTrackQualityStorageHelper::deliverV2 );
}

// TRSeedTrackQualityStorageHelper::TRSeedTrackQualityStorageHelper( const TRSeedTrackQualityStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//TRSeedTrackQualityStorageHelper::~TRSeedTrackQualityStorageHelper()
//{
//}

//
// assignment operators
//
// const TRSeedTrackQualityStorageHelper& TRSeedTrackQualityStorageHelper::operator=( const TRSeedTrackQualityStorageHelper& rhs )
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
void 
TRSeedTrackQualityStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const TRSeedTrackQuality& iData ) 
{
   iSink 
      << sm_pack( SM_VAR( iData, identifier ), 1, 1000, 1 )
      << sm_pack( SM_VAR( iData, chiSquare ), 0., 5000., 1e-3 )
      << sm_pack( SM_VAR( iData, degreesOfFreedom ), -5, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberHits ), 0, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberHitsExpected ), 0, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberLayers ), 0, 60, 1 )
      << sm_pack( SM_VAR( iData, numberMultipleHits ), 0, 1000, 1 )
      << sm_pack( sm_field( "zEndpointUsed",
                            char( iData.zEndpointUsed() ? 1 : 0 ) ),
                  0, 1, 1 )
      << sm_pack( sm_field( "originUsed",
                            char( iData.originUsed() ? 1 : 0 ) ),
                  0, 1, 1 )
      << sm_pack( sm_field( "useForExtrapolation",
                            char( iData.useForExtrapolation() ? 1 : 0 ) ),
                  0, 1, 1 )
      ;
}

TRSeedTrackQuality* 
TRSeedTrackQualityStorageHelper::deliverV2( SMSourceStream& iSource )
{
   
   TRSeedTrackQuality ::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberLayers; 
   int numberMultipleHits; 
   char zEndpointUsed; 
   char originUsed; 
   char useForExtrapolation; 

   iSource 
      >> identifier 
      >> chiSquare 
      >> degreesOfFreedom 
      >> numberHits 
      >> numberHitsExpected 
      >> numberLayers 
      >> numberMultipleHits 
      >> zEndpointUsed 
      >> originUsed 
      >> useForExtrapolation 
	 ;
   
   return make( TRSeedTrackQuality( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberLayers,  
      numberMultipleHits,  
      zEndpointUsed,  
      originUsed,  
      useForExtrapolation ) ); 
}

TRSeedTrackQuality* 
TRSeedTrackQualityStorageHelper::deliverV1( SMSourceStream& iSource )
{
   //This is the version before numberLayers, numberMultipleHits,
   //useForExtrapolation were added.  Fill with "default" values.
   TRSeedTrackQuality ::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberLayers; 
   int numberMultipleHits; 
   char zEndpointUsed; 
   char originUsed; 
   char useForExtrapolation; 

   iSource 
      >> identifier 
      >> chiSquare 
      >> degreesOfFreedom 
      >> numberHits 
      >> numberHitsExpected 
      >> zEndpointUsed 
      >> originUsed 
	 ;

   numberLayers = numberHits;
   numberMultipleHits = 0;
   useForExtrapolation = true;
   
   return make( TRSeedTrackQuality( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberLayers,  
      numberMultipleHits,  
      zEndpointUsed,  
      originUsed,  
      useForExtrapolation ) ); 
}

//
// const member functions
//
const TRSeedTrackQuality& 
TRSeedTrackQualityStorageHelper::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const TRSeedTrackQuality temp(
      TRSeedTrackQuality ::Identifier ( 1 ), // identifier 
      double ( 2 ), // chiSquare 
      int ( 3 ), // degreesOfFreedom 
      int ( 4 ), // numberHits 
      int ( 5 ), // numberHitsExpected 
      int ( 6 ), // numberLayers 
      int ( 7 ), // numberMultipleHits 
      DABoolean ( false ), // zEndpointUsed 
      DABoolean ( false ), // originUsed 
      DABoolean ( false ) ); // useForExtrapolation 
   return temp;
}

DABoolean 
TRSeedTrackQualityStorageHelper::compare( 
   const TRSeedTrackQuality* iNewData, 
   const TRSeedTrackQuality& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for TRSeedTrackQuality
   return (*iNewData == iOldData ) ;
}

//
// static member functions
//

//TRSeedTrackQuality*
//SMStorageHelper<TRSeedTrackQuality>::make( const TRSeedTrackQuality& iTempData ) const
//{
   //change this line if TRSeedTrackQuality does not have a copy constructor
//   return new TRSeedTrackQuality( iTempData );
//}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( TRSeedTrackQualityStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( TRSeedTrackQualityStorageHelper, TRSeedTrackQuality )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(TRSeedTrackQuality)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<TRSeedTrackQuality>;

//------------------------------------------------------
