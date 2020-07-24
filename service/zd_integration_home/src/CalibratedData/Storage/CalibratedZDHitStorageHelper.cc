// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      CalibratedZDHitStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: CalibratedZDHitStorageHelper.cc,v 1.1 2002/09/27 20:30:20 bkh Exp $
//
// Revision history
//
// $Log: CalibratedZDHitStorageHelper.cc,v $
// Revision 1.1  2002/09/27 20:30:20  bkh
// Add ZD
//
// Revision 1.3  2001/11/29 22:12:30  bkh
// Add raw charge and time to object
// Add raw charge and time to storage helper
// Fix operator== object to not just use identifier
//
// Revision 1.2  2000/05/03 22:07:22  lyon
// Added compression hints
//
// Revision 1.1  2000/03/03 15:12:54  lyon
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
#include "CalibratedData/CalibratedZDHitStorageHelper.h"
#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"
#include "StorageManagement/SMFillContents.h"
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

static const char* const kFacilityString = "CalibratedData.CalibratedZDHitStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CalibratedZDHitStorageHelper.cc,v 1.1 2002/09/27 20:30:20 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedZDHitStorageHelper::CalibratedZDHitStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &CalibratedZDHitStorageHelper::deliverV1 );
}

// CalibratedZDHitStorageHelper::CalibratedZDHitStorageHelper( const CalibratedZDHitStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//CalibratedZDHitStorageHelper::~CalibratedZDHitStorageHelper()
//{
//}

//
// assignment operators
//
// const CalibratedZDHitStorageHelper& CalibratedZDHitStorageHelper::operator=( const CalibratedZDHitStorageHelper& rhs )
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
CalibratedZDHitStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const CalibratedZDHit& iData ) 
{
   iSink 
     << sm_pack( SM_VAR( iData, wire ), 1, 9797, 1)
     << sm_pack( SM_VAR( iData, layer ), 1, 48, 1)
     << sm_pack( SM_VAR( iData, wireInLyr ), 0, 500, 1)
     << SM_VAR( iData, charge )
     << sm_pack( SM_VAR( iData, time ), 0, 1.0e7, 100.0)
     << sm_pack( SM_VAR( iData, tSeries().distance ), -0.3, 0.3, 5.0e-7)
     << SM_VAR( iData, tSeries().velocity )   
     << SM_VAR( iData, tSeries().acceleration )   
     << SM_VAR( iData, weight )
     << SM_VAR( iData, qualHit )   
     << sm_pack( SM_VAR( iData, nTDC ), 0, 20, 1)
     << SM_VAR( iData, earlyGap )
     << SM_VAR( iData, lateGap )
     << sm_pack( SM_VAR( iData, whichTDC ), 0, 20, 1)
     << sm_pack( SM_VAR( iData, identifier ), 0, 10000, 1)
     << sm_pack( SM_VAR( iData, rawCharge ), 0, 65535, 1 )
     << sm_pack( SM_VAR( iData, rawTime ), 0, 65535, 1 )
     ;
}

CalibratedZDHit* 
CalibratedZDHitStorageHelper::deliverV1( SMSourceStream& iSource )
{
   
   UInt16 wire; 
   UInt16 layer; 
   UInt16 wireInLyr; 
   CalibratedZDHit::PicoCoul charge; 
   CalibratedZDHit::PicoSec time; 
   Tseries tSeries;
   double d, v, a;
   double weight; 
   CalibratedZDHit::QualHit qualHit; 
   UInt16 nTDC; 
   CalibratedZDHit::PicoSec earlyGap; 
   CalibratedZDHit::PicoSec lateGap; 
   CalibratedZDHit ::Identifier identifier;
   UInt32 whichTDC;

   iSource 
      >> wire 
      >> layer 
      >> wireInLyr 
      >> charge 
      >> time 
      >> d >> v >> a
      >> weight 
      >> qualHit 
      >> nTDC 
      >> earlyGap 
      >> lateGap 
      >> whichTDC
      >> identifier
	 ;

   CalibratedZDHit::RawChargeTime rawChargeTime ;

   iSource 
      >> rawChargeTime.first
      >> rawChargeTime.second
	 ;
   
   CalibratedZDHit* hit ( make( CalibratedZDHit( 
      wire,  
      layer,  
      wireInLyr,  
      charge,  
      time,  
      Tseries(d, v, a),
      weight,  
      qualHit,  
      nTDC,  
      earlyGap,  
      lateGap,  
      whichTDC,
      identifier ) ) ); 

   hit->setRawChargeTime( rawChargeTime ) ;
   
   return hit ; 
}

//
// const member functions
//
const CalibratedZDHit& 
CalibratedZDHitStorageHelper::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const CalibratedZDHit temp(
      UInt16 ( 1 ), // wire 
      UInt16 ( 1 ), // layer 
      UInt16 ( 1 ), // wireInLyr 
      CalibratedZDHit::PicoCoul ( 1.0 ), // charge 
      CalibratedZDHit::PicoSec ( 1.0 ), // time 
      Tseries (  ), // distanceSeries 
      double ( 0 ), // weight 
      CalibratedZDHit::QualHit ( 0 ), // qualHit 
      UInt16 ( 0 ), // nTDC 
      CalibratedZDHit::PicoSec ( 1.0 ), // earlyGap 
      CalibratedZDHit::PicoSec ( 1.0 ), // lateGap 
      UInt16( 0 ), // whichTDC
      CalibratedZDHit ::Identifier ( 0 ) ); // id 

   return temp;
}

DABoolean 
CalibratedZDHitStorageHelper::compare( 
   const CalibratedZDHit* iNewData, 
   const CalibratedZDHit& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for CalibratedZDHit
   return (*iNewData == iOldData ) ;
}

//
// static member functions
//

//CalibratedZDHit*
//SMStorageHelper<CalibratedZDHit>::make( const CalibratedZDHit& iTempData ) const
//{
   //change this line if CalibratedZDHit does not have a copy constructor
//   return new CalibratedZDHit( iTempData );
//}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( CalibratedZDHitStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( CalibratedZDHitStorageHelper, CalibratedZDHit )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(CalibratedZDHit)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<CalibratedZDHit>;

//------------------------------------------------------
