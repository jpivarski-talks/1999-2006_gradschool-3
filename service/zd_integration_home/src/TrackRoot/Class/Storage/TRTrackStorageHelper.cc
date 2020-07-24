// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Wed Nov 3 17:00:02 EDT 1999
// $Id: TRTrackStorageHelper.cc,v 1.1 2000/01/05 18:46:15 cdj Exp $
//
// Revision history
//
// $Log: TRTrackStorageHelper.cc,v $
// Revision 1.1  2000/01/05 18:46:15  cdj
// first submission
//
// Revision 1.1  1999/11/04 14:44:54  mkl
// added storage helper for TRTrack
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
#include "TrackRoot/Storage/TRTrackStorageHelper.h"
#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"

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

static const char* const kFacilityString = "TrackRoot.TRTrackStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRTrackStorageHelper.cc,v 1.1 2000/01/05 18:46:15 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
TRTrackStorageHelper::TRTrackStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &TRTrackStorageHelper::deliverV1 );
}

// TRTrackStorageHelper::TRTrackStorageHelper( const TRTrackStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//TRTrackStorageHelper::~TRTrackStorageHelper()
//{
//}

//
// assignment operators
//
// const TRTrackStorageHelper& TRTrackStorageHelper::operator=( const TRTrackStorageHelper& rhs )
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
TRTrackStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const TRTrack& iData ) 
{
   iSink 
      << SM_VAR( iData, identifier )   
      ;
}

TRTrack* 
TRTrackStorageHelper::deliverV1( SMSourceStream& iSource )
{
   TRTrack::Identifier id; 

   iSource 
      >> id 
	 ;
   
   return make( TRTrack( id ) ); 
}

//
// const member functions
//
const TRTrack& 
TRTrackStorageHelper::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const TRTrack temp(
      TRTrack::Identifier ( 1 ) ); // id 
   return temp;
}

DABoolean 
TRTrackStorageHelper::compare( 
   const TRTrack* iNewData, 
   const TRTrack& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for TRTrack
   return ((*iNewData).identifier() == iOldData.identifier() );
}

//
// static member functions
//

//TRTrack*
//SMStorageHelper<TRTrack>::make( const TRTrack& iTempData ) const
//{
//   //change this line if TRTrack does not have a copy constructor
//   return new TRTrack( iTempData.id() );
//}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( TRTrackStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( TRTrackStorageHelper, TRTrack )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(TRTrack)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<TRTrack>;

//------------------------------------------------------
