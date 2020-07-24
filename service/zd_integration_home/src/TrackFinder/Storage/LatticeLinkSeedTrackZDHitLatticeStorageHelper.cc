// -*- C++ -*-
//
// Package:     <package>
// Module:      SeedTrackZDHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:45:44 EDT 1999
// $Id: LatticeLinkSeedTrackZDHitLatticeStorageHelper.cc,v 1.1 2002/09/27 21:02:59 bkh Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackZDHitLatticeStorageHelper.cc,v $
// Revision 1.1  2002/09/27 21:02:59  bkh
// Add ZD capability
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <iterator>
#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "TrackFinder/LatticeLinkSeedTrackZDHitLatticeStorageHelper.h"

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

static const char* const kFacilityString = "package.LatticeLinkSeedTrackZDHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: LatticeLinkSeedTrackZDHitLatticeStorageHelper.cc,v 1.1 2002/09/27 21:02:59 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//Simplify some names
typedef SeedTrackZDHitLattice::VectorLeftID LeftIDs;
typedef SeedTrackZDHitLattice::VectorRightID RightIDs;

//
// static data member definitions
//

//
// constructors and destructor
//
LatticeLinkSeedTrackZDHitLatticeStorageHelper::LatticeLinkSeedTrackZDHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &LatticeLinkSeedTrackZDHitLatticeStorageHelper::deliverV1 );
}

// LatticeLinkSeedTrackZDHitLatticeStorageHelper::LatticeLinkSeedTrackZDHitLatticeStorageHelper( const LatticeLinkSeedTrackZDHitLatticeStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

LatticeLinkSeedTrackZDHitLatticeStorageHelper::~LatticeLinkSeedTrackZDHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkSeedTrackZDHitLatticeStorageHelper& LatticeLinkSeedTrackZDHitLatticeStorageHelper::operator=( const LatticeLinkSeedTrackZDHitLatticeStorageHelper& rhs )
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
LatticeLinkSeedTrackZDHitLatticeStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase< SeedTrackZDHitLattice >& iData ) 
{
   const LeftIDs& leftIDs = iData.vLeftID();
   const RightIDs& rightIDs = iData.vRightID();

   assert(leftIDs.size() == 1);
   assert(rightIDs.size() == 1);

   //store the left and right ids
   iSink << sm_pack(
             sm_field( "trackID", leftIDs[0]),
	     0, 100, 1)

	 << sm_pack(
             sm_field( "hitID", rightIDs[0]),
	     0, 9797, 1)

      //store info from link data
	 << sm_pack( SM_VAR( iData, linkData().signedDriftDistance ),
		     -1.0, 1.0, 5.0e-7 )

	 << sm_pack( SM_VAR( iData, linkData().pull ),
		     -0.3, 0.3, 5.0e-7 )

	 << sm_pack( sm_field( "fitFlag", int( iData.linkData().fitFlag() ) ),
		     0, 2, 1 )
// 	 << sm_pack( SM_VAR( iData, linkData().fitFlag ),
// 		     0, 2, 1)
      ;
}

SMLatticeLinkInfoBase< SeedTrackZDHitLattice >* 
LatticeLinkSeedTrackZDHitLatticeStorageHelper::deliverV1( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< SeedTrackZDHitLattice >* link = 
      new SMLatticeLinkInfoIn< SeedTrackZDHitLattice>();

   //first read the left and right IDs
   LeftIDs::value_type leftID;
   RightIDs::value_type rightID;

   iSource >> leftID
	   >> rightID;

   SeedTrackZDHitLattice::VectorLeftID leftIDs(1,leftID);
   SeedTrackZDHitLattice::VectorRightID rightIDs(1,rightID);
   
   //now read data for the LinkData
   float signedDriftDistance; 
   float pull; 
   int fitFlag;

   iSource 
      >> signedDriftDistance 
      >> pull 
      >> fitFlag
      ;
   SeedTrackZDHitLattice::LinkData linkData = 
      SeedTrackZDHitLattice::LinkData( 
	 signedDriftDistance,   
	 pull, (SeedZDHitLink::fitFlagEnum)fitFlag  
	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

//
// const member functions
//
const SMLatticeLinkInfoBase< SeedTrackZDHitLattice >& 
LatticeLinkSeedTrackZDHitLatticeStorageHelper::getDefault() const
{
   static const SMLatticeLinkInfoDefault< SeedTrackZDHitLattice > 
      temp(  LeftIDs(1,LeftIDs::value_type(1)),
	     SeedTrackZDHitLattice::LinkData(
		float ( 1 ), // signedDriftDistance 
		float ( 2 ),  // pull 
		SeedZDHitLink::kFITTABLE
		),
	     RightIDs(1,RightIDs::value_type(1)) );
   return temp;
}

DABoolean 
LatticeLinkSeedTrackZDHitLatticeStorageHelper::compare( 
   const SMLatticeLinkInfoBase< SeedTrackZDHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< SeedTrackZDHitLattice >& iOldData ) const
{
   //need to compare contents
   return ( 
	               (iNewData->linkData() == iOldData.linkData() ) &&
// comment out the line above and uncomment the lines below if operator==
// does not exist for the link data
		       //	   (iNewData->linkData().signedDriftDistance() == iOldData.linkData().signedDriftDistance() ) && 
		       //	   (iNewData->linkData().pull() == iOldData.linkData().pull() ) && 
	   (iNewData->vLeftID() == iOldData.vLeftID() ) &&
	   (iNewData->vRightID() == iOldData.vRightID() ) );
}

//
// static member functions
//

//------------------------------------------------------------------------
// DO NOT DELETE
SM_REGISTER_STORAGE( LatticeLinkSeedTrackZDHitLatticeStorageHelper );

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
typedef SMLatticeLinkInfoBase< SeedTrackZDHitLattice > SeedTrackZDHitLatticeLink;
INSTANTIATE_ONE( SeedTrackZDHitLatticeLink )
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( LatticeLinkSeedTrackZDHitLatticeStorageHelper, SMLatticeLinkInfoBase< SeedTrackZDHitLattice > )

typedef SeedTrackZDHitLattice _lattice_element_;
#include "StorageManagement/instantiate_latticelinkhelper.h"



