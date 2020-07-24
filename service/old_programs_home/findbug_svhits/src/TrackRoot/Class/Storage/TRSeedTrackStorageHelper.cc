// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRSeedTrackStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: TRSeedTrackStorageHelper.cc,v 1.1 2000/01/05 18:46:14 cdj Exp $
//
// Revision history
//
// $Log: TRSeedTrackStorageHelper.cc,v $
// Revision 1.1  2000/01/05 18:46:14  cdj
// first submission
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

#include <math.h>
// user include files
//#include "Experiment/report.h"
#include "TrackRoot/Storage/TRSeedTrackStorageHelper.h"
#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"
#include "StorageManagement/SMPackInfo.h"

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

static const char* const kFacilityString = "TrackRoot.TRSeedTrackStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRSeedTrackStorageHelper.cc,v 1.1 2000/01/05 18:46:14 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TRSeedTrackStorageHelper::TRSeedTrackStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &TRSeedTrackStorageHelper::deliverV1 );
}

// TRSeedTrackStorageHelper::TRSeedTrackStorageHelper( const TRSeedTrackStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//TRSeedTrackStorageHelper::~TRSeedTrackStorageHelper()
//{
//}

//
// assignment operators
//
// const TRSeedTrackStorageHelper& TRSeedTrackStorageHelper::operator=( const TRSeedTrackStorageHelper& rhs )
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
TRSeedTrackStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const TRSeedTrack& iData ) 
{
  // using compression values same as TRHelixCandidateFit
   iSink 
     << sm_pack( SM_VAR( iData, identifier ) , 1 , 50000, 1)           
      << sm_pack_fraction( SM_VAR( iData, curvature )  , 0.01, 25, 1e-5)
      << sm_pack( SM_VAR( iData, phi0 )                , 0 , 2*M_PI , 1e-5)
      << sm_pack( SM_VAR( iData, d0 )                  , -.5 , .5 , 1e-6)
      << sm_pack( SM_VAR( iData, cotTheta )            , -10. , 10. , 1e-5)
      << sm_pack( SM_VAR( iData, z0 )                  , -1. , 1. , 1e-6)
     ;
}

TRSeedTrack* 
TRSeedTrackStorageHelper::deliverV1( SMSourceStream& iSource )
{
   
   TRSeedTrack ::Identifier identifier; 
   double curvature; 
   double phi0; 
   double d0; 
   double cotTheta; 
   double z0; 

   iSource 
      >> identifier 
      >> curvature 
      >> phi0 
      >> d0 
      >> cotTheta 
      >> z0 
	 ;
   
   return make( TRSeedTrack( 
      identifier,  
      curvature,  
      phi0,  
      d0,  
      cotTheta,  
      z0 ) ); 
}

//
// const member functions
//
const TRSeedTrack& 
TRSeedTrackStorageHelper::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const TRSeedTrack temp(
      TRSeedTrack ::Identifier ( 1 ), // identifier 
      double ( 2 ), // curvature 
      double ( 3 ), // phi0 
      double ( 4 ), // d0 
      double ( 5 ), // cotTheta 
      double ( 6 ) ); // z0 
   return temp;
}

DABoolean 
TRSeedTrackStorageHelper::compare( 
   const TRSeedTrack* iNewData, 
   const TRSeedTrack& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for TRSeedTrack
   return (*iNewData == iOldData ) ;
}

//
// static member functions
//

//TRSeedTrack*
//SMStorageHelper<TRSeedTrack>::make( const TRSeedTrack& iTempData ) const
//{
   //change this line if TRSeedTrack does not have a copy constructor
//   return new TRSeedTrack( iTempData );
//}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( TRSeedTrackStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( TRSeedTrackStorageHelper, TRSeedTrack )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(TRSeedTrack)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<TRSeedTrack>;

//------------------------------------------------------
