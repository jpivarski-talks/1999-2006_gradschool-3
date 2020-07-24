// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      CalibratedCathodeHitStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: CalibratedCathodeHitStorageHelper.cc,v 1.2 2000/05/03 22:07:21 lyon Exp $
//
// Revision history
//
// $Log: CalibratedCathodeHitStorageHelper.cc,v $
// Revision 1.2  2000/05/03 22:07:21  lyon
// Added compression hints
//
// Revision 1.1  2000/03/03 15:12:53  lyon
// Storage helpers for CalibratedData
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
#include "Experiment/cleo_math.h"

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
#include "CalibratedData/CalibratedCathodeHitStorageHelper.h"
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

static const char* const kFacilityString = "CalibratedData.CalibratedCathodeHitStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CalibratedCathodeHitStorageHelper.cc,v 1.2 2000/05/03 22:07:21 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedCathodeHitStorageHelper::CalibratedCathodeHitStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &CalibratedCathodeHitStorageHelper::deliverV1 );
}

// CalibratedCathodeHitStorageHelper::CalibratedCathodeHitStorageHelper( const CalibratedCathodeHitStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//CalibratedCathodeHitStorageHelper::~CalibratedCathodeHitStorageHelper()
//{
//}

//
// assignment operators
//
// const CalibratedCathodeHitStorageHelper& CalibratedCathodeHitStorageHelper::operator=( const CalibratedCathodeHitStorageHelper& rhs )
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
CalibratedCathodeHitStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const CalibratedCathodeHit& iData ) 
{
   iSink 
     << sm_pack( SM_VAR( iData, peakPad ), 0, 10000, 1)
     << sm_pack( SM_VAR( iData, numberOfPads ), 0, 10000, 1)
     << sm_pack_fraction( SM_VAR( iData, totalCharge ), 1.0e-5, 1.0e6, 0.001)
     << sm_pack_fraction( SM_VAR( iData, peakCharge ), 1.0e-5, 1.0e6, 0.001)
     << sm_pack( SM_VAR( iData, identifier ), 0, 10000, 1)
     << sm_pack( SM_VAR( iData, z ), -2.0, 2.0, 1.0e-5)
     << sm_pack( SM_VAR( iData, phi ), 0.0, 2.0*M_PI, 1.0e-5)
     << SM_VAR( iData, weight )
     << sm_pack( SM_VAR( iData, width ), 0.0, 2.0, 1.0e-5)
     << SM_VAR( iData, qualHit )   
      ;
}

CalibratedCathodeHit* 
CalibratedCathodeHitStorageHelper::deliverV1( SMSourceStream& iSource )
{
   
   UInt16 peakPad; 
   UInt16 numberOfPads; 
   CalibratedCathodeHit::PicoCoul totalCharge; 
   CalibratedCathodeHit::PicoCoul peakCharge; 
   CalibratedHit::Identifier id; 
   Meters z;
   double phi;
   double weight;
   double width;
   CalibratedCathodeHit::QualHit qualHit;

   iSource 
      >> peakPad 
      >> numberOfPads 
      >> totalCharge 
      >> peakCharge 
      >> id 
      >> z 
      >> phi 
      >> weight
      >> width
      >> qualHit
	 ;
   
   return make( CalibratedCathodeHit( 
      peakPad,  
      numberOfPads,  
      totalCharge,  
      peakCharge,  
      id,  
      z,  
      phi,
      weight,  
      width,  
      qualHit ) ); 
}

//
// const member functions
//
const CalibratedCathodeHit& 
CalibratedCathodeHitStorageHelper::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const CalibratedCathodeHit temp(
      UInt16 ( 1 ), // peakPad 
      UInt16 ( 1 ), // numberOfPads 
      CalibratedCathodeHit::PicoCoul ( 1.0 ), // totalCharge 
      CalibratedCathodeHit::PicoCoul ( 1.0 ), // peakCharge 
      CalibratedHit::Identifier ( 1 ), // id 
      CalibratedCathodeHit::Meters ( 1.0 ), // z
      double ( 1.0 ), // z
      double ( 1.0 ), // phi 
      double  ( 1.0 ), // weight 
      CalibratedCathodeHit::QualHit ( 1 ) ); // qual
   return temp;
}

DABoolean 
CalibratedCathodeHitStorageHelper::compare( 
   const CalibratedCathodeHit* iNewData, 
   const CalibratedCathodeHit& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for CalibratedCathodeHit
   return (*iNewData == iOldData ) ;
}

//
// static member functions
//

//CalibratedCathodeHit*
//SMStorageHelper<CalibratedCathodeHit>::make( const CalibratedCathodeHit& iTempData ) const
//{
   //change this line if CalibratedCathodeHit does not have a copy constructor
//   return new CalibratedCathodeHit( iTempData );
//}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( CalibratedCathodeHitStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( CalibratedCathodeHitStorageHelper, CalibratedCathodeHit )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(CalibratedCathodeHit)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<CalibratedCathodeHit>;

//------------------------------------------------------
