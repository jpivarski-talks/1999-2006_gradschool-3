// -*- C++ -*-
//
// Package:     <TrackFitter>
// Module:      TrackFitCathodeHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Feb  6 16:35:10 EST 2000
// $Id: LatticeLinkTrackFitCathodeHitLatticeStorageHelper.cc,v 1.6 2000/08/16 19:34:16 wsun Exp $
//
// Revision history
//
// $Log: LatticeLinkTrackFitCathodeHitLatticeStorageHelper.cc,v $
// Revision 1.6  2000/08/16 19:34:16  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.5  2000/08/16 19:34:11  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.4  2000/08/16 19:34:01  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.3  2000/06/27 23:35:38  wsun
// Fixed bugs for OSF1 compilation.
//
// Revision 1.2  2000/06/26 18:17:41  wsun
// Added compression hints.
//
// Revision 1.1  2000/02/24 17:26:24  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <iterator>

// user include files
#include "Experiment/report.h"
#include "TrackFitter/Storage/LatticeLinkTrackFitCathodeHitLatticeStorageHelper.h"

#include "StorageManagement/SMLatticeLinkInfoIn.h"
#include "StorageManagement/SMLatticeLinkInfoOut.h"
#include "StorageManagement/SMLatticeLinkInfoDefault.h"

#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"
#include "StorageManagement/SMContents.h"
#include "StorageManagement/SMContentsBuiltIn.h"
#include "StorageManagement/SMFillContents.h"

#include "StorageManagement/SMPackInfo.h" //defines sm_pack*

#include "StorageManagement/SMDeliverer.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kSMFacilityString = "TrackFitter.LatticeLinkTrackFitCathodeHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kSMIdString  = "$Id: LatticeLinkTrackFitCathodeHitLatticeStorageHelper.cc,v 1.6 2000/08/16 19:34:16 wsun Exp $";
static const char* const kSMTagString = "$Name:  $";

// //Simplify some names
// typedef TrackFitCathodeHitLattice::VectorLeftID LeftIDs;
// typedef TrackFitCathodeHitLattice::VectorRightID RightIDs;

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
LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::
LatticeLinkTrackFitCathodeHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion(
      &LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::deliverV1 );
   registerVersion(
      &LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::deliverV2 );
}

// LatticeLinkTrackFitCathodeHitLatticeStorageHelper::LatticeLinkTrackFitCathodeHitLatticeStorageHelper( const LatticeLinkTrackFitCathodeHitLatticeStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::
~LatticeLinkTrackFitCathodeHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkTrackFitCathodeHitLatticeStorageHelper& LatticeLinkTrackFitCathodeHitLatticeStorageHelper::operator=( const LatticeLinkTrackFitCathodeHitLatticeStorageHelper& rhs )
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
LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase<
      LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::TrackFitCathodeHitLattice >&
      iData ) 
{
   const TrackFitCathodeHitLattice::VectorLeftID& leftIDs = iData.vLeftID();
   const TrackFitCathodeHitLattice::VectorRightID& rightIDs = iData.vRightID();

   //store the left and right ids
   iSink
      << sm_pack( sm_field( "leftIDs", leftIDs[0] ),
		  0, 100, 1 )
      << sm_pack( sm_field( "rightIDs", rightIDs[0] ),
		  0, 1600, 1 )
      //store info from link data
      << sm_pack( sm_field( "used", char( iData.linkData().used() ? 1 : 0 ) ),
		  0, 1, 1 )
      << sm_pack( SM_VAR( iData, linkData().residual ),
		  -0.02, 0.02, 5.e-7 )
      << sm_pack( SM_VAR( iData, linkData().residualError ),
		  0., 0.1, 5.e-7 )
      << sm_pack_fraction( SM_VAR( iData, linkData().momentum ),
			   1.e-3, 25., 1.e-5 )
      ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
SMLatticeLinkInfoBase<
   LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::TrackFitCathodeHitLattice >* 
LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::deliverV1(
   SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< TrackFitCathodeHitLattice >* link = 
      new SMLatticeLinkInfoIn< TrackFitCathodeHitLattice >();

   //first read the left and right IDs
   TrackFitCathodeHitLattice::VectorLeftID leftIDs;
   TrackFitCathodeHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<TrackFitCathodeHitLattice::VectorLeftID>
      leftInsertIterator( leftIDs );
   back_insert_iterator<TrackFitCathodeHitLattice::VectorRightID>
      rightInsertIterator( rightIDs );

   iSource >> sm_fill_contents( leftInsertIterator )
	   >> sm_fill_contents( rightInsertIterator ) ;

   //now read data for the LinkData
   char used ; // really a DABoolean
   double residual; 
   double residualError; 
   double curvature; 
   double momentum; 
   double refx, refy, refz ;
   Meters signedDcaToWire; 
   Meters signedDcaError; 
   double sinTrackToRadial;

   iSource 
      >> used 
      >> residual 
      >> residualError 
      >> curvature 
      >> momentum 
      ;
   TrackFitCathodeHitLattice::LinkData linkData = 
      TrackFitCathodeHitLattice::LinkData( 
	 used,
	 residual,   
	 residualError,   
//	 curvature,   
	 momentum
	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
SMLatticeLinkInfoBase<
   LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::TrackFitCathodeHitLattice >* 
LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::deliverV2(
   SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< TrackFitCathodeHitLattice >* link = 
      new SMLatticeLinkInfoIn< TrackFitCathodeHitLattice >();

   //first read the left and right IDs
   TrackFitCathodeHitLattice::VectorLeftID leftIDs;
   TrackFitCathodeHitLattice::VectorRightID rightIDs;

   TrackFitCathodeHitLattice::VectorLeftID::value_type leftID ;
   TrackFitCathodeHitLattice::VectorRightID::value_type rightID ;

   iSource
      >> leftID
      >> rightID
      ;

   leftIDs.push_back( leftID ) ;
   rightIDs.push_back( rightID ) ;


   //now read data for the LinkData
   char used ; // really a DABoolean
   double residual; 
   double residualError; 
   double momentum; 

   iSource 
      >> used 
      >> residual 
      >> residualError 
      >> momentum 
      ;
   TrackFitCathodeHitLattice::LinkData linkData = 
      TrackFitCathodeHitLattice::LinkData( 
	 used,
	 residual,   
	 residualError,   
	 momentum
	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

//
// const member functions
//
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const SMLatticeLinkInfoBase<
   LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::TrackFitCathodeHitLattice >& 
LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::getDefault() const
{
   static const SMLatticeLinkInfoDefault< TrackFitCathodeHitLattice > 
      temp( TrackFitCathodeHitLattice::VectorLeftID(
	 1,TrackFitCathodeHitLattice::VectorLeftID::value_type(1)),
	     TrackFitCathodeHitLattice::LinkData(
		DABoolean ( true ), // used 
		double ( 2 ), // residual 
		double ( 3 ), // residualError 
//		double ( 4 ), // curvature 
		double ( 5 ) // momentum 
		),
	     TrackFitCathodeHitLattice::VectorRightID(
		1,TrackFitCathodeHitLattice::VectorRightID::value_type(1)) );
   return temp;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
DABoolean 
LatticeLinkTrackFitCathodeHitLatticeStorageHelper<H>::compare( 
   const SMLatticeLinkInfoBase< TrackFitCathodeHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< TrackFitCathodeHitLattice >& iOldData ) const
{
   //need to compare contents
   return ( 
            (iNewData->linkData() == iOldData.linkData() ) &&
// comment out the line above and uncomment the lines below if operator==
// does not exist for the link data
      //    (iNewData->linkData().used() == iOldData.linkData().used() ) && 
      //    (iNewData->linkData().residual() == iOldData.linkData().residual() ) && 
      //    (iNewData->linkData().residualError() == iOldData.linkData().residualError() ) && 
      //    (iNewData->linkData().curvature() == iOldData.linkData().curvature() ) && 
      //    (iNewData->linkData().momentum() == iOldData.linkData().momentum() ) && 
      //    (iNewData->linkData().trackRefPt() == iOldData.linkData().trackRefPt() ) && 
      //    (iNewData->linkData().signedDcaToWire() == iOldData.linkData().signedDcaToWire() ) && 
      //    (iNewData->linkData().signedDcaError() == iOldData.linkData().signedDcaError() ) && 
      //    (iNewData->linkData().sinTrackToRadial() == iOldData.linkData().sinTrackToRadial() ) && 
	    (iNewData->vLeftID() == iOldData.vLeftID() ) &&
	    (iNewData->vRightID() == iOldData.vRightID() ) );
}

//
// static member functions
//

//------------------------------------------------------------------------
// DO NOT DELETE

// Do instantiations in Instantiate files.

// SM_REGISTER_STORAGE( LatticeLinkTrackFitCathodeHitLatticeStorageHelper );

// //this is done to create a TypeTag and its name
// #include "FrameAccess/instantiate_macros.h"
// typedef SMLatticeLinkInfoBase< TrackFitCathodeHitLattice > TrackFitCathodeHitLatticeLink;
// INSTANTIATE_ONE( TrackFitCathodeHitLatticeLink )
// #include "StorageManagement/instantiate_one_typetag.h"

// #include "StorageManagement/SMDeliverer.h"
// SM_REGISTER_VERSION_DEF( LatticeLinkTrackFitCathodeHitLatticeStorageHelper, SMLatticeLinkInfoBase< TrackFitCathodeHitLattice > )

// typedef TrackFitCathodeHitLattice _lattice_element_;
// #include "StorageManagement/instantiate_latticelinkhelper.h"
