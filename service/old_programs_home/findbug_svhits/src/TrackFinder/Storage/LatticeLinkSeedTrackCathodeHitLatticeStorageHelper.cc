// -*- C++ -*-
//
// Package:     <package>
// Module:      SeedTrackCathodeHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:45:44 EDT 1999
// $Id: LatticeLinkSeedTrackCathodeHitLatticeStorageHelper.cc,v 1.2 2000/05/03 22:08:57 lyon Exp $
//
// Revision history
//
// $Log: LatticeLinkSeedTrackCathodeHitLatticeStorageHelper.cc,v $
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

// user include files
#include "Experiment/report.h"
#include "TrackFinder/LatticeLinkSeedTrackCathodeHitLatticeStorageHelper.h"

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

static const char* const kFacilityString = "package.LatticeLinkSeedTrackCathodeHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: LatticeLinkSeedTrackCathodeHitLatticeStorageHelper.cc,v 1.2 2000/05/03 22:08:57 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//Simplify some names
typedef SeedTrackCathodeHitLattice::VectorLeftID LeftIDs;
typedef SeedTrackCathodeHitLattice::VectorRightID RightIDs;

//
// static data member definitions
//

//
// constructors and destructor
//
LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::LatticeLinkSeedTrackCathodeHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::deliverV1 );
}

// LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::LatticeLinkSeedTrackCathodeHitLatticeStorageHelper( const LatticeLinkSeedTrackCathodeHitLatticeStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::~LatticeLinkSeedTrackCathodeHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkSeedTrackCathodeHitLatticeStorageHelper& LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::operator=( const LatticeLinkSeedTrackCathodeHitLatticeStorageHelper& rhs )
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
LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice >& iData ) 
{
   const LeftIDs& leftIDs = iData.vLeftID();
   const RightIDs& rightIDs = iData.vRightID();

   //store the left and right ids
   iSink << SMPackInfoFixedTemplate<LeftIDs::value_type>(
	    sm_field( "leftIDs", sm_contents_builtin( leftIDs.begin(), 
						      leftIDs.end() )),
	    0, 100, 1)

	 << SMPackInfoFixedTemplate<RightIDs::value_type>(
	    sm_field( "rightIDs", sm_contents_builtin( rightIDs.begin(), 
						       rightIDs.end() ) ),
	    0, 1600, 1)
      //store info from link data

      ;
}

SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice >* 
LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::deliverV1( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice >* link = 
      new SMLatticeLinkInfoIn< SeedTrackCathodeHitLattice>();

   //first read the left and right IDs
   SeedTrackCathodeHitLattice::VectorLeftID leftIDs;
   SeedTrackCathodeHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<LeftIDs> leftInsertIterator( leftIDs );
   back_insert_iterator<RightIDs> rightInsertIterator( rightIDs );

   iSource >> sm_fill_contents( leftInsertIterator )
	   >> sm_fill_contents( rightInsertIterator ) ;

   //now read data for the LinkData


   iSource 

      ;
   SeedTrackCathodeHitLattice::LinkData linkData = 
      SeedTrackCathodeHitLattice::LinkData( 


	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

//
// const member functions
//
const SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice >& 
LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::getDefault() const
{
   static const SMLatticeLinkInfoDefault< SeedTrackCathodeHitLattice > 
      temp(  LeftIDs(1,LeftIDs::value_type(1)),
	     SeedTrackCathodeHitLattice::LinkData(


		),
	     RightIDs(1,RightIDs::value_type(1)) );
   return temp;
}

DABoolean 
LatticeLinkSeedTrackCathodeHitLatticeStorageHelper::compare( 
   const SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice >& iOldData ) const
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
SM_REGISTER_STORAGE( LatticeLinkSeedTrackCathodeHitLatticeStorageHelper );

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
typedef SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice > SeedTrackCathodeHitLatticeLink;
INSTANTIATE_ONE( SeedTrackCathodeHitLatticeLink )
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( LatticeLinkSeedTrackCathodeHitLatticeStorageHelper, SMLatticeLinkInfoBase< SeedTrackCathodeHitLattice > )

typedef SeedTrackCathodeHitLattice _lattice_element_;
#include "StorageManagement/instantiate_latticelinkhelper.h"



