// -*- C++ -*-
//
// Package:     Doit
// Module:      DoitSeedQualityStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Aug 15 23:47:22 EDT 2000
// $Id: DoitSeedQualityStorageHelper.cc,v 1.7 2001/12/05 16:47:35 lyon Exp $
//
// Revision history
//
// $Log: DoitSeedQualityStorageHelper.cc,v $
// Revision 1.7  2001/12/05 16:47:35  lyon
// Add more bits to the DoitStatusInfo object
//
// Revision 1.6  2001/01/05 06:30:27  lyon
// Added assert to check for bad SeedType; Actually register V3 storage helper this time
//
// Revision 1.5  2001/01/05 05:37:20  lyon
// Added Doit status words
//
// Revision 1.4  2000/11/19 15:06:02  cdj
// use better default for old stored quantities
//
// Revision 1.3  2000/11/19 03:24:26  cdj
// added missing version for DoitSeedQuality storage helper
//
// Revision 1.2  2000/11/01 01:23:27  lyon
// Add Fittable flag and new members to quality object
//
// Revision 1.1  2000/08/16 19:41:55  wsun
// First submission.
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
#include "Experiment/report.h"
#include "Doit/Storage/DoitSeedQualityStorageHelper.h"
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

static const char* const kFacilityString = "Doit.DoitSeedQualityStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitSeedQualityStorageHelper.cc,v 1.7 2001/12/05 16:47:35 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitSeedQualityStorageHelper::DoitSeedQualityStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &DoitSeedQualityStorageHelper::deliverV1 );
   registerVersion( &DoitSeedQualityStorageHelper::deliverV2 );
   registerVersion( &DoitSeedQualityStorageHelper::deliverV3 );
   registerVersion( &DoitSeedQualityStorageHelper::deliverV4 );
}

// DoitSeedQualityStorageHelper::DoitSeedQualityStorageHelper( const DoitSeedQualityStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//DoitSeedQualityStorageHelper::~DoitSeedQualityStorageHelper()
//{
//}

//
// assignment operators
//
// const DoitSeedQualityStorageHelper& DoitSeedQualityStorageHelper::operator=( const DoitSeedQualityStorageHelper& rhs )
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
DoitSeedQualityStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const DoitSeedQuality& iData ) 
{

  // Get the quality words for the DoitStatusObject
  
  // If the assert below fails, that means that the number of words
  // in the status object has changed. This storage helper will need
  // to be reworked.
  assert ( DoitStatusInfo::knWords == 4 );

  // Get the array of status words -- right now we only need to save 
  // three of them
  const UInt32* doitStati = iData.doitStatus().words();

   iSink 
      << sm_pack( SM_VAR( iData, identifier ), 1, 1000, 1 ) 
      << sm_pack( SM_VAR( iData, chiSquare ), 0., 5000., 1e-3 )
      << sm_pack( SM_VAR( iData, degreesOfFreedom ), -5, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberHits ), 0, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberHitsExpected ), 0, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberLayers ), 0, 1000, 1 )
      << sm_pack( SM_VAR( iData, numberMultipleHits), 0, 1000, 1)
      << sm_pack( sm_field( "zEndpointUsed",
                            char( iData.zEndpointUsed() ? 1 : 0 ) ),
                  0, 1, 1 )
      << sm_pack( sm_field( "originUsed",
                            char( iData.originUsed() ? 1 : 0 ) ),
                  0, 1, 1 )
      << sm_pack( sm_field( "useForExtrapolation",
                            char( iData.useForExtrapolation() ? 1 : 0 ) ),
                  0, 1, 1 )     
      << sm_pack( sm_field( "goodZ",
			    char( iData.goodZ() ? 1 : 0 ) ),
                  0, 1, 1 )
      << sm_pack( sm_field( "inwardDoitCurler",
                            char( iData.inwardDoitCurler() ? 1 : 0 ) ),
                  0, 1, 1 )
      << sm_pack( SM_VAR( iData, chiSquareZ ), 0., 5000., 1e-3 )
      << sm_pack( SM_VAR( iData, chiSquare3 ), 0., 5000., 1e-3 )
      << sm_pack( SM_VAR( iData, resicd ), 0., 0.1, 1e-3 )
      << sm_field("DoitStatusWord1", UInt32( doitStati[0] ) )
      << sm_field("DoitStatusWord2", UInt32( doitStati[1] ) )
      << sm_field("DoitStatusWord3", UInt32( doitStati[2] ) )
      ;
}

DoitSeedQuality* 
DoitSeedQualityStorageHelper::deliverV1( SMSourceStream& iSource )
{
   
   DoitSeedQuality ::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   char zEndpointUsed; 
   char originUsed; 
   char goodZ; 
   char inwardDoitCurler; 
   float chiSquareZ; 
   float chiSquare3; 
   float resicd; 

   iSource 
      >> identifier 
      >> chiSquare 
      >> degreesOfFreedom 
      >> numberHits 
      >> numberHitsExpected 
      >> zEndpointUsed 
      >> originUsed 
      >> goodZ 
      >> inwardDoitCurler 
      >> chiSquareZ 
      >> chiSquare3 
      >> resicd 
	 ;
   
   //previously, only allowed one hit per layer
   int numberLayers =numberHits;
   int numberMultipleHits=0;

   //previously, all hits were to be used for extrapolation
   char useForExtrapolation = true;

   return make( DoitSeedQuality( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberLayers,
      numberMultipleHits,
      zEndpointUsed,  
      originUsed,  
      useForExtrapolation,
      goodZ,  
      inwardDoitCurler,  
      chiSquareZ,  
      chiSquare3,  
      resicd,
      DoitStatusInfo(0, 0, 0, 0) ) ); 
}

DoitSeedQuality* 
DoitSeedQualityStorageHelper::deliverV2( SMSourceStream& iSource )
{
   
   DoitSeedQuality ::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberLayers;
   int numberMultipleHits;
   char zEndpointUsed; 
   char originUsed; 
   char goodZ; 
   char useForExtrapolation;
   char inwardDoitCurler; 
   float chiSquareZ; 
   float chiSquare3; 
   float resicd; 

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
      >> goodZ 
      >> inwardDoitCurler 
      >> chiSquareZ 
      >> chiSquare3 
      >> resicd 
	 ;
   
   return make( DoitSeedQuality( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberLayers,
      numberMultipleHits,
      zEndpointUsed,  
      originUsed,  
      useForExtrapolation,
      goodZ,  
      inwardDoitCurler,  
      chiSquareZ,  
      chiSquare3,  
      resicd,
      DoitStatusInfo(0, 0, 0, 0)) ); 
}

DoitSeedQuality* 
DoitSeedQualityStorageHelper::deliverV3( SMSourceStream& iSource )
{
   
   DoitSeedQuality ::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberLayers;
   int numberMultipleHits;
   char zEndpointUsed; 
   char originUsed; 
   char goodZ; 
   char useForExtrapolation;
   char inwardDoitCurler; 
   float chiSquareZ; 
   float chiSquare3; 
   float resicd; 
   UInt32 doitStatusWord1;
   UInt32 doitStatusWord2;

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
      >> goodZ 
      >> inwardDoitCurler 
      >> chiSquareZ 
      >> chiSquare3 
      >> resicd 
      >> doitStatusWord1
      >> doitStatusWord2
	 ;
   
   // Make the status object
   // If the assert below fails, that means that the number of words
   // in the status object has changed. This storage helper will need
   // to be reworked.
   assert ( DoitStatusInfo::knWords == 4 );
   
   DoitStatusInfo doitStatus( doitStatusWord1, doitStatusWord2, 0, 0);

   return make( DoitSeedQuality( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberLayers,
      numberMultipleHits,
      zEndpointUsed,  
      originUsed,  
      useForExtrapolation,
      goodZ,  
      inwardDoitCurler,  
      chiSquareZ,  
      chiSquare3,  
      resicd,
      doitStatus) ); 
}

DoitSeedQuality* 
DoitSeedQualityStorageHelper::deliverV4( SMSourceStream& iSource )
{
   
   DoitSeedQuality ::Identifier identifier; 
   double chiSquare; 
   int degreesOfFreedom; 
   int numberHits; 
   int numberHitsExpected; 
   int numberLayers;
   int numberMultipleHits;
   char zEndpointUsed; 
   char originUsed; 
   char goodZ; 
   char useForExtrapolation;
   char inwardDoitCurler; 
   float chiSquareZ; 
   float chiSquare3; 
   float resicd; 
   UInt32 doitStatusWord1;
   UInt32 doitStatusWord2;
   UInt32 doitStatusWord3;

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
      >> goodZ 
      >> inwardDoitCurler 
      >> chiSquareZ 
      >> chiSquare3 
      >> resicd 
      >> doitStatusWord1
      >> doitStatusWord2
      >> doitStatusWord3
	 ;
   
   // Make the status object
   // If the assert below fails, that means that the number of words
   // in the status object has changed. This storage helper will need
   // to be reworked.
   assert ( DoitStatusInfo::knWords == 4 );
   
   DoitStatusInfo doitStatus( doitStatusWord1, doitStatusWord2, 
			      doitStatusWord3, 0);

   return make( DoitSeedQuality( 
      identifier,  
      chiSquare,  
      degreesOfFreedom,  
      numberHits,  
      numberHitsExpected,  
      numberLayers,
      numberMultipleHits,
      zEndpointUsed,  
      originUsed,  
      useForExtrapolation,
      goodZ,  
      inwardDoitCurler,  
      chiSquareZ,  
      chiSquare3,  
      resicd,
      doitStatus) ); 
}

//
// const member functions
//
const DoitSeedQuality& 
DoitSeedQualityStorageHelper::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const DoitSeedQuality temp(
      DoitSeedQuality ::Identifier ( 1 ), // identifier 
      double ( 2 ), // chiSquare 
      int ( 3 ), // degreesOfFreedom 
      int ( 4 ), // numberHits 
      int ( 5 ), // numberHitsExpected 
      int ( 20 ), // numberLayers
      int ( 21 ), // numberMultipleHits
      DABoolean ( 6 ), // zEndpointUsed 
      DABoolean ( 7 ), // originUsed 
      DABoolean ( 22 ), // useForExtrapolation
      DABoolean ( 8 ), // goodZ 
      DABoolean ( 9 ), // inwardDoitCurler 
      float ( 10 ), // chiSquareZ 
      float ( 11 ), // chiSquare3 
      float ( 12 ),
      DoitStatusInfo(0, 0, 0, 0)); // resicd 
   return temp;
}

DABoolean 
DoitSeedQualityStorageHelper::compare( 
   const DoitSeedQuality* iNewData, 
   const DoitSeedQuality& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for DoitSeedQuality
   return (*iNewData == iOldData ) ;
}

//
// static member functions
//

//DoitSeedQuality*
//SMStorageHelper<DoitSeedQuality>::make( const DoitSeedQuality& iTempData ) const
//{
   //change this line if DoitSeedQuality does not have a copy constructor
//   return new DoitSeedQuality( iTempData );
//}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( DoitSeedQualityStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( DoitSeedQualityStorageHelper, DoitSeedQuality )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(DoitSeedQuality)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<DoitSeedQuality>;

//------------------------------------------------------
