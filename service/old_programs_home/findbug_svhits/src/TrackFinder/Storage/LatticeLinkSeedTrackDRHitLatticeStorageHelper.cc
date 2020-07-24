// -*- C++ -*-
//
// Package:     <package>
// Module:      SeedTrackDRHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:45:44 EDT 1999
// $Id: LatticeLinkSeedTrackDRHitLatticeStorageHelper.cc,v 1.7 2001/10/23 19:12:54 cdj Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackDRHitLatticeStorageHelper.cc,v $
// Revision 1.7  2001/10/23 19:12:54  cdj
// modified StorageHelpers so they store individual identifiers rather than lists
//
// Revision 1.6  2000/10/29 02:43:07  wsun
// kFITABLE->kFITTABLE and cast fitFlag() into int for sm_pack.
//
// Revision 1.5  2000/10/25 20:08:23  lyon
// Add DeliverV2 and make DeliverV1 backwards compatable
//
// Revision 1.4  2000/10/25 18:35:05  lyon
// Remove fittingweights from SeedDRHitLink and its storage helper
//
// Revision 1.3  2000/10/25 15:07:19  lyon
// Added new fitFlag to SeedDRHitLink link data
//
// Revision 1.2  2000/05/03 22:08:57  lyon
// Added compression hints
//
// Revision 1.1  2000/02/08 18:29:08  lyon
// Storage helpers
//
// Revision 1.3  2000/01/21 21:07:34  cdj
// helper skeletons now include SMPackInfo.h
//
// Revision 1.2  2000/01/08 19:18:08  cdj
// updated Lattice code to work with new cxx compiler
//
// Revision 1.1  1999/10/18 21:30:18  cdj
// renamed files
//
// Revision 1.2  1999/10/18 18:16:45  cdj
// easier to use and maintain
//
// Revision 1.1  1999/10/11 22:00:36  cdj
// added Lattice storage code
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
#include "TrackFinder/LatticeLinkSeedTrackDRHitLatticeStorageHelper.h"

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

static const char* const kFacilityString = "package.LatticeLinkSeedTrackDRHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: LatticeLinkSeedTrackDRHitLatticeStorageHelper.cc,v 1.7 2001/10/23 19:12:54 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//Simplify some names
typedef SeedTrackDRHitLattice::VectorLeftID LeftIDs;
typedef SeedTrackDRHitLattice::VectorRightID RightIDs;

//
// static data member definitions
//

//
// constructors and destructor
//
LatticeLinkSeedTrackDRHitLatticeStorageHelper::LatticeLinkSeedTrackDRHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &LatticeLinkSeedTrackDRHitLatticeStorageHelper::deliverV1 );
   
   // Version 2 adds the fitFlag
   registerVersion( &LatticeLinkSeedTrackDRHitLatticeStorageHelper::deliverV2 );

   // Version 3 stores the individual IDs rather than lists
   registerVersion( &LatticeLinkSeedTrackDRHitLatticeStorageHelper::deliverV3 );
}

// LatticeLinkSeedTrackDRHitLatticeStorageHelper::LatticeLinkSeedTrackDRHitLatticeStorageHelper( const LatticeLinkSeedTrackDRHitLatticeStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

LatticeLinkSeedTrackDRHitLatticeStorageHelper::~LatticeLinkSeedTrackDRHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkSeedTrackDRHitLatticeStorageHelper& LatticeLinkSeedTrackDRHitLatticeStorageHelper::operator=( const LatticeLinkSeedTrackDRHitLatticeStorageHelper& rhs )
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
LatticeLinkSeedTrackDRHitLatticeStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase< SeedTrackDRHitLattice >& iData ) 
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

SMLatticeLinkInfoBase< SeedTrackDRHitLattice >* 
LatticeLinkSeedTrackDRHitLatticeStorageHelper::deliverV1( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< SeedTrackDRHitLattice >* link = 
      new SMLatticeLinkInfoIn< SeedTrackDRHitLattice>();

   //first read the left and right IDs
   SeedTrackDRHitLattice::VectorLeftID leftIDs;
   SeedTrackDRHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<LeftIDs> leftInsertIterator( leftIDs );
   back_insert_iterator<RightIDs> rightInsertIterator( rightIDs );

   iSource >> sm_fill_contents( leftInsertIterator )
	   >> sm_fill_contents( rightInsertIterator ) ;

   //now read data for the LinkData
   float signedDriftDistance; 
   float pull; 

   iSource 
      >> signedDriftDistance 
      >> pull 
      ;

   SeedTrackDRHitLattice::LinkData linkData = 
      SeedTrackDRHitLattice::LinkData( 
	 signedDriftDistance,   
	 pull
	 );

   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

//
// const member functions
//

SMLatticeLinkInfoBase< SeedTrackDRHitLattice >* 
LatticeLinkSeedTrackDRHitLatticeStorageHelper::deliverV2( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< SeedTrackDRHitLattice >* link = 
      new SMLatticeLinkInfoIn< SeedTrackDRHitLattice>();

   //first read the left and right IDs
   SeedTrackDRHitLattice::VectorLeftID leftIDs;
   SeedTrackDRHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<LeftIDs> leftInsertIterator( leftIDs );
   back_insert_iterator<RightIDs> rightInsertIterator( rightIDs );

   iSource >> sm_fill_contents( leftInsertIterator )
	   >> sm_fill_contents( rightInsertIterator ) ;

   //now read data for the LinkData
   float signedDriftDistance; 
   float pull; 
   int fitFlag;

   iSource 
      >> signedDriftDistance 
      >> pull 
      >> fitFlag
      ;
   SeedTrackDRHitLattice::LinkData linkData = 
      SeedTrackDRHitLattice::LinkData( 
	 signedDriftDistance,   
	 pull, (SeedDRHitLink::fitFlagEnum)fitFlag  
	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

SMLatticeLinkInfoBase< SeedTrackDRHitLattice >* 
LatticeLinkSeedTrackDRHitLatticeStorageHelper::deliverV3( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< SeedTrackDRHitLattice >* link = 
      new SMLatticeLinkInfoIn< SeedTrackDRHitLattice>();

   //first read the left and right IDs
   LeftIDs::value_type leftID;
   RightIDs::value_type rightID;

   iSource >> leftID
	   >> rightID;

   SeedTrackDRHitLattice::VectorLeftID leftIDs(1,leftID);
   SeedTrackDRHitLattice::VectorRightID rightIDs(1,rightID);
   
   //now read data for the LinkData
   float signedDriftDistance; 
   float pull; 
   int fitFlag;

   iSource 
      >> signedDriftDistance 
      >> pull 
      >> fitFlag
      ;
   SeedTrackDRHitLattice::LinkData linkData = 
      SeedTrackDRHitLattice::LinkData( 
	 signedDriftDistance,   
	 pull, (SeedDRHitLink::fitFlagEnum)fitFlag  
	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

//
// const member functions
//
const SMLatticeLinkInfoBase< SeedTrackDRHitLattice >& 
LatticeLinkSeedTrackDRHitLatticeStorageHelper::getDefault() const
{
   static const SMLatticeLinkInfoDefault< SeedTrackDRHitLattice > 
      temp(  LeftIDs(1,LeftIDs::value_type(1)),
	     SeedTrackDRHitLattice::LinkData(
		float ( 1 ), // signedDriftDistance 
		float ( 2 ),  // pull 
		SeedDRHitLink::kFITTABLE
		),
	     RightIDs(1,RightIDs::value_type(1)) );
   return temp;
}

DABoolean 
LatticeLinkSeedTrackDRHitLatticeStorageHelper::compare( 
   const SMLatticeLinkInfoBase< SeedTrackDRHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< SeedTrackDRHitLattice >& iOldData ) const
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
SM_REGISTER_STORAGE( LatticeLinkSeedTrackDRHitLatticeStorageHelper );

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
typedef SMLatticeLinkInfoBase< SeedTrackDRHitLattice > SeedTrackDRHitLatticeLink;
INSTANTIATE_ONE( SeedTrackDRHitLatticeLink )
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( LatticeLinkSeedTrackDRHitLatticeStorageHelper, SMLatticeLinkInfoBase< SeedTrackDRHitLattice > )

typedef SeedTrackDRHitLattice _lattice_element_;
#include "StorageManagement/instantiate_latticelinkhelper.h"



