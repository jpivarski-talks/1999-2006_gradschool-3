// -*- C++ -*-
//
// Package:     <TrackFitter>
// Module:      TrackFitZDHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Feb  6 16:35:10 EST 2000
// $Id: LatticeLinkTrackFitZDHitLatticeStorageHelper.cc,v 1.1 2003/04/17 17:03:37 bkh Exp $
//
// Revision history
//
// $Log: LatticeLinkTrackFitZDHitLatticeStorageHelper.cc,v $
// Revision 1.1  2003/04/17 17:03:37  bkh
// Add ZD Lattice Object & Instantiations
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <iterator>

// user include files
#include "Experiment/report.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAItem.h"

#include "TrackFitter/Storage/LatticeLinkTrackFitZDHitLatticeStorageHelper.h"
#include "TrackFinder/SeedZDHitLink.h"

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

static const char* const kSMFacilityString = "TrackFitter.LatticeLinkTrackFitZDHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kSMIdString  = "$Id: LatticeLinkTrackFitZDHitLatticeStorageHelper.cc,v 1.1 2003/04/17 17:03:37 bkh Exp $";
static const char* const kSMTagString = "$Name:  $";

// //Simplify some names
// typedef TrackFitZDHitLattice::VectorLeftID LeftIDs;
// typedef TrackFitZDHitLattice::VectorRightID RightIDs;

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
LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::
LatticeLinkTrackFitZDHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion(
      &LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::deliverV1 );
}

// LatticeLinkTrackFitZDHitLatticeStorageHelper::LatticeLinkTrackFitZDHitLatticeStorageHelper( const LatticeLinkTrackFitZDHitLatticeStorageHelper& rhs )
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
LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::
~LatticeLinkTrackFitZDHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkTrackFitZDHitLatticeStorageHelper& LatticeLinkTrackFitZDHitLatticeStorageHelper::operator=( const LatticeLinkTrackFitZDHitLatticeStorageHelper& rhs )
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
LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase<
      typename LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::TrackFitZDHitLattice >&
      iData ) 
{
   const TrackFitZDHitLattice::VectorLeftID& leftIDs = iData.vLeftID();
   const TrackFitZDHitLattice::VectorRightID& rightIDs = iData.vRightID();

   //store the left and right ids
   iSink
      << sm_pack( sm_field( "leftIDs", leftIDs[0] ),
		  0, 100, 1 )
      << sm_pack( sm_field( "rightIDs", rightIDs[0] ),
		  0, 9797, 1 )
      //store info from link data
      << sm_pack( sm_field( "used",
			    char( iData.linkData().used() ? 1 : 0 ) ),
		  0, 1, 1 )
      << sm_pack( SM_VAR( iData, linkData().residual ),
		  -0.02, 0.02, 5.e-7 )
      << sm_pack( SM_VAR( iData, linkData().residualError ),
		  0., 0.1, 5.e-7 )
      << sm_pack_fraction( SM_VAR( iData, linkData().momentum ),
			   1.e-3, 25., 1.e-5 )
      << sm_pack( SM_VAR( iData, linkData().trackRefPt().x ),
		  -2., 2., 1.e-6 )
      << sm_pack( SM_VAR( iData, linkData().trackRefPt().y ),
		  -2., 2., 1.e-6 )
      << sm_pack( SM_VAR( iData, linkData().trackRefPt().z ),
		  -2., 2., 1.e-6 )
      << sm_pack( SM_VAR( iData, linkData().signedDcaToWire ),
		  -0.02, 0.02, 5.e-7 )
      << sm_pack( SM_VAR( iData, linkData().signedDcaError ),
		  0., 0.1, 5.e-7 )
      << sm_pack( SM_VAR( iData, linkData().sinTrackToRadial ),
		  -1., 1., 1.e-6 )
      << sm_pack( SM_VAR( iData, linkData().driftTime ),
		  0, 1.0e7, 100.0 )
      << SM_VAR( iData, linkData().fittingWeight )
      << sm_pack( SM_VAR( iData, linkData().signedDriftDistance ),
		  -0.02, 0.02, 5.e-7 )
      ;
}


#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
SMLatticeLinkInfoBase<
   typename LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::TrackFitZDHitLattice >* 
LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::deliverV1(
   SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< TrackFitZDHitLattice >* link = 
      new SMLatticeLinkInfoIn< TrackFitZDHitLattice >();

   //first read the left and right IDs
   TrackFitZDHitLattice::VectorLeftID leftIDs;
   TrackFitZDHitLattice::VectorRightID rightIDs;
   
   TrackFitZDHitLattice::VectorLeftID::value_type leftID ;
   TrackFitZDHitLattice::VectorRightID::value_type rightID ;

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
   double refx, refy, refz ;
   Meters signedDcaToWire; 
   Meters signedDcaError; 
   double sinTrackToRadial;
   CalibratedTimeHit::PicoSec driftTime ;
   double fittingWeight ;
   Meters signedDriftDistance; 

   iSource 
      >> used 
      >> residual 
      >> residualError 
      >> momentum 
      >> refx >> refy >> refz
      >> signedDcaToWire 
      >> signedDcaError 
      >> sinTrackToRadial 
      >> driftTime
      >> fittingWeight
      >> signedDriftDistance
      ;

   TrackFitZDHitLattice::LinkData linkData = 
      TrackFitZDHitLattice::LinkData( 
	 used,
	 residual,   
	 residualError,   
	 momentum,   
	 HepPoint3D( refx, refy, refz ),
	 signedDcaToWire,   
	 signedDcaError,   
	 sinTrackToRadial,
	 driftTime,
	 fittingWeight,
	 signedDriftDistance
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
   typename LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::TrackFitZDHitLattice >& 
LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::getDefault() const
{
   static const SMLatticeLinkInfoDefault< TrackFitZDHitLattice > 
      temp( TrackFitZDHitLattice::VectorLeftID(
	 1,TrackFitZDHitLattice::VectorLeftID::value_type(1)),
	     TrackFitZDHitLattice::LinkData(
		DABoolean ( true ), // used 
		double ( 2 ), // residual 
		double ( 3 ), // residualError 
//		double ( 4 ), // curvature 
		double ( 5 ), // momentum 
		HepPoint3D( 0., 0., 0. ),
		Meters ( 7 ), // signedDcaToWire 
		Meters ( 8 ), // signedDcaError 
		double ( 9 ), // sinTrackToRadial 
		CalibratedTimeHit::PicoSec( 10 ), // drifttime
		double ( 11 ), // fitting weight
		Meters ( 12 ) // signedDriftDistance
		),
	     TrackFitZDHitLattice::VectorRightID(
		1,TrackFitZDHitLattice::VectorRightID::value_type(1)) );
   return temp;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
DABoolean 
LatticeLinkTrackFitZDHitLatticeStorageHelper<H>::compare( 
   const SMLatticeLinkInfoBase< TrackFitZDHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< TrackFitZDHitLattice >& iOldData ) const
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

// SM_REGISTER_STORAGE( LatticeLinkTrackFitZDHitLatticeStorageHelper );

// //this is done to create a TypeTag and its name
// #include "FrameAccess/instantiate_macros.h"
// typedef SMLatticeLinkInfoBase< TrackFitZDHitLattice > TrackFitZDHitLatticeLink;
// INSTANTIATE_ONE( TrackFitZDHitLatticeLink )
// #include "StorageManagement/instantiate_one_typetag.h"

// #include "StorageManagement/SMDeliverer.h"
// SM_REGISTER_VERSION_DEF( LatticeLinkTrackFitZDHitLatticeStorageHelper, SMLatticeLinkInfoBase< TrackFitZDHitLattice > )

// typedef TrackFitZDHitLattice _lattice_element_;
// #include "StorageManagement/instantiate_latticelinkhelper.h"
