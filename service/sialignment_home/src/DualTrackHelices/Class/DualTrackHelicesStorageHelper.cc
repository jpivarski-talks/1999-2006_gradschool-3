// -*- C++ -*-
//
// Package:     DualTrackHelices
// Module:      DualTrackHelicesStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: DualTrackHelicesStorageHelper.cc,v 1.1.1.1 2001/01/30 23:57:19 llh14 Exp $
//
// Revision history
//
// $Log: DualTrackHelicesStorageHelper.cc,v $
// Revision 1.1.1.1  2001/01/30 23:57:19  llh14
// imported DualTrackHelices sources
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
#include "DualTrackHelices/DualTrackHelicesStorageHelper.h"
#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"
#include "StorageManagement/SMFillContents.h"
#include "StorageManagement/SMContentsBuiltIn.h"
#include "StorageManagement/SMPackInfo.h" //defines sm_pack*
#include <iterator>

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

static const char* const kFacilityString = "DualTrackHelices.DualTrackHelicesStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackHelicesStorageHelper.cc,v 1.1.1.1 2001/01/30 23:57:19 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackHelicesStorageHelper::DualTrackHelicesStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &DualTrackHelicesStorageHelper::deliverV1 );
}

// DualTrackHelicesStorageHelper::DualTrackHelicesStorageHelper( const DualTrackHelicesStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//DualTrackHelicesStorageHelper::~DualTrackHelicesStorageHelper()
//{
//}

//
// assignment operators
//
// const DualTrackHelicesStorageHelper& DualTrackHelicesStorageHelper::operator=( const DualTrackHelicesStorageHelper& rhs )
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
DualTrackHelicesStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const DualTrackHelices& iData ) 
{

   const STL_VECTOR(double) errVector(iData.storeErrMatrix());

   iSink 
      << SM_VAR( iData, identifier )   
      << SM_VAR( iData, curvature )   
      << SM_VAR( iData, phi0 )   
      << SM_VAR( iData, d0 )   
      << SM_VAR( iData, cotTheta )   
      << SM_VAR( iData, z0 )   
      << sm_field( "storeErrMatrix", sm_contents_builtin( errVector.begin(), 
							errVector.end()) )   
      ;
}

DualTrackHelices* 
DualTrackHelicesStorageHelper::deliverV1( SMSourceStream& iSource )
{
   
   DualTrackHelices::Identifier identifier; 
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
      >> z0; 

   STL_VECTOR(double) errVector; 
   back_insert_iterator<STL_VECTOR(double)> insertIterator(errVector);
   iSource >> sm_fill_contents(insertIterator);
   
   return make( DualTrackHelices( 
      identifier,  
      curvature,  
      phi0,  
      d0,  
      cotTheta,  
      z0,  
      errVector ) ); 
}

//
// const member functions
//
const DualTrackHelices& 
DualTrackHelicesStorageHelper::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   static const DualTrackHelices temp(
      DualTrackHelices::Identifier ( 1 ), // identifier 
      double ( 2 ), // curvature 
      double ( 3 ), // phi0 
      double ( 4 ), // d0 
      double ( 5 ), // cotTheta 
      double ( 6 ), // z0 
      STL_VECTOR(double) ( ) ); // errVector 
   return temp;
}

DABoolean 
DualTrackHelicesStorageHelper::compare( 
   const DualTrackHelices* iNewData, 
   const DualTrackHelices& iOldData ) const
{
//write a comparison operation if operator== doesn't exist for DualTrackHelices
   return (*iNewData == iOldData ) ;
}

//
// static member functions
//

//DualTrackHelices*
//SMStorageHelper<DualTrackHelices>::make( const DualTrackHelices& iTempData ) const
//{
   //change this line if DualTrackHelices does not have a copy constructor
//   return new DualTrackHelices( iTempData );
//}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( DualTrackHelicesStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( DualTrackHelicesStorageHelper, DualTrackHelices )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(DualTrackHelices)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<DualTrackHelices>;

//------------------------------------------------------
