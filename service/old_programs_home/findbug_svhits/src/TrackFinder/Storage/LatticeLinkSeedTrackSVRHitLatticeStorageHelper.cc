// -*- C++ -*-
//
// Package:     <package>
// Module:      SeedTrackSVRHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:45:44 EDT 1999
// $Id: LatticeLinkSeedTrackSVRHitLatticeStorageHelper.cc,v 1.3 2001/10/23 19:12:55 cdj Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackSVRHitLatticeStorageHelper.cc,v $
// Revision 1.3  2001/10/23 19:12:55  cdj
// modified StorageHelpers so they store individual identifiers rather than lists
//
// Revision 1.2  2000/05/03 22:08:57  lyon
// Added compression hints
//
// Revision 1.1  2000/02/08 18:29:09  lyon
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
#include "TrackFinder/LatticeLinkSeedTrackSVRHitLatticeStorageHelper.h"

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

static const char* const kFacilityString = "package.LatticeLinkSeedTrackSVRHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: LatticeLinkSeedTrackSVRHitLatticeStorageHelper.cc,v 1.3 2001/10/23 19:12:55 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//Simplify some names
typedef SeedTrackSVRHitLattice::VectorLeftID LeftIDs;
typedef SeedTrackSVRHitLattice::VectorRightID RightIDs;

//
// static data member definitions
//

//
// constructors and destructor
//
LatticeLinkSeedTrackSVRHitLatticeStorageHelper::LatticeLinkSeedTrackSVRHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &LatticeLinkSeedTrackSVRHitLatticeStorageHelper::deliverV1 );
   registerVersion( &LatticeLinkSeedTrackSVRHitLatticeStorageHelper::deliverV2 );
}

// LatticeLinkSeedTrackSVRHitLatticeStorageHelper::LatticeLinkSeedTrackSVRHitLatticeStorageHelper( const LatticeLinkSeedTrackSVRHitLatticeStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

LatticeLinkSeedTrackSVRHitLatticeStorageHelper::~LatticeLinkSeedTrackSVRHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkSeedTrackSVRHitLatticeStorageHelper& LatticeLinkSeedTrackSVRHitLatticeStorageHelper::operator=( const LatticeLinkSeedTrackSVRHitLatticeStorageHelper& rhs )
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
LatticeLinkSeedTrackSVRHitLatticeStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase< SeedTrackSVRHitLattice >& iData ) 
{
   const LeftIDs& leftIDs = iData.vLeftID();
   const RightIDs& rightIDs = iData.vRightID();

   assert( leftIDs.size() == 1);
   assert( rightIDs.size() == 1);
   //store the left and right ids
   iSink << sm_pack(
             sm_field( "trackID", leftIDs[0]),
	     0, 100, 1)

	 << sm_pack(
             sm_field( "hitID", rightIDs[0]),
	     0, 10000, 1)

      //store info from link data

      ;
}

SMLatticeLinkInfoBase< SeedTrackSVRHitLattice >* 
LatticeLinkSeedTrackSVRHitLatticeStorageHelper::deliverV1( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< SeedTrackSVRHitLattice >* link = 
      new SMLatticeLinkInfoIn< SeedTrackSVRHitLattice>();

   //first read the left and right IDs
   SeedTrackSVRHitLattice::VectorLeftID leftIDs;
   SeedTrackSVRHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<LeftIDs> leftInsertIterator( leftIDs );
   back_insert_iterator<RightIDs> rightInsertIterator( rightIDs );

   iSource >> sm_fill_contents( leftInsertIterator )
	   >> sm_fill_contents( rightInsertIterator ) ;

   //now read data for the LinkData


   iSource 

      ;
   SeedTrackSVRHitLattice::LinkData linkData = 
      SeedTrackSVRHitLattice::LinkData( 


	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

SMLatticeLinkInfoBase< SeedTrackSVRHitLattice >* 
LatticeLinkSeedTrackSVRHitLatticeStorageHelper::deliverV2( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< SeedTrackSVRHitLattice >* link = 
      new SMLatticeLinkInfoIn< SeedTrackSVRHitLattice>();

   //first read the left and right IDs
   
   LeftIDs::value_type leftID;
   RightIDs::value_type rightID;

   iSource >> leftID
	   >> rightID;

   SeedTrackSVRHitLattice::VectorLeftID leftIDs(1,leftID);
   SeedTrackSVRHitLattice::VectorRightID rightIDs(1,rightID);
   //now read data for the LinkData


   iSource 

      ;
   SeedTrackSVRHitLattice::LinkData linkData = 
      SeedTrackSVRHitLattice::LinkData( 


	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}
//
// const member functions
//
const SMLatticeLinkInfoBase< SeedTrackSVRHitLattice >& 
LatticeLinkSeedTrackSVRHitLatticeStorageHelper::getDefault() const
{
   static const SMLatticeLinkInfoDefault< SeedTrackSVRHitLattice > 
      temp(  LeftIDs(1,LeftIDs::value_type(1)),
	     SeedTrackSVRHitLattice::LinkData(


		),
	     RightIDs(1,RightIDs::value_type(1)) );
   return temp;
}

DABoolean 
LatticeLinkSeedTrackSVRHitLatticeStorageHelper::compare( 
   const SMLatticeLinkInfoBase< SeedTrackSVRHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< SeedTrackSVRHitLattice >& iOldData ) const
{
   //need to compare contents
   return ( 
	   //            (iNewData->linkData() == iOldData.linkData() ) &&
// comment out the line above and uncomment the lines below if operator==
// does not exist for the link data

	    (iNewData->vLeftID() == iOldData.vLeftID() ) &&
	    (iNewData->vRightID() == iOldData.vRightID() ) );
}

//
// static member functions
//

//------------------------------------------------------------------------
// DO NOT DELETE
SM_REGISTER_STORAGE( LatticeLinkSeedTrackSVRHitLatticeStorageHelper );

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
typedef SMLatticeLinkInfoBase< SeedTrackSVRHitLattice > SeedTrackSVRHitLatticeLink;
INSTANTIATE_ONE( SeedTrackSVRHitLatticeLink )
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( LatticeLinkSeedTrackSVRHitLatticeStorageHelper, SMLatticeLinkInfoBase< SeedTrackSVRHitLattice > )

typedef SeedTrackSVRHitLattice _lattice_element_;
#include "StorageManagement/instantiate_latticelinkhelper.h"



