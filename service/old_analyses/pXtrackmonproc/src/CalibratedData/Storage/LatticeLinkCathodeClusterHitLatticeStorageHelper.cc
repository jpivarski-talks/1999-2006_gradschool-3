// -*- C++ -*-
//
// Package:     <package>
// Module:      CathodeClusterHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:45:44 EDT 1999
// $Id: LatticeLinkCathodeClusterHitLatticeStorageHelper.cc,v 1.2 2000/05/03 22:07:22 lyon Exp $
//
// Revision history
//
// $Log: LatticeLinkCathodeClusterHitLatticeStorageHelper.cc,v $
// Revision 1.2  2000/05/03 22:07:22  lyon
// Added compression hints
//
// Revision 1.1  2000/03/03 15:12:55  lyon
// Storage helpers for CalibratedData
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
#include "CalibratedData/LatticeLinkCathodeClusterHitLatticeStorageHelper.h"

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

static const char* const kFacilityString = "package.LatticeLinkCathodeClusterHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: LatticeLinkCathodeClusterHitLatticeStorageHelper.cc,v 1.2 2000/05/03 22:07:22 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//Simplify some names
typedef CathodeClusterHitLattice::VectorLeftID LeftIDs;
typedef CathodeClusterHitLattice::VectorRightID RightIDs;

//
// static data member definitions
//

//
// constructors and destructor
//
LatticeLinkCathodeClusterHitLatticeStorageHelper::LatticeLinkCathodeClusterHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &LatticeLinkCathodeClusterHitLatticeStorageHelper::deliverV1 );
}

// LatticeLinkCathodeClusterHitLatticeStorageHelper::LatticeLinkCathodeClusterHitLatticeStorageHelper( const LatticeLinkCathodeClusterHitLatticeStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

LatticeLinkCathodeClusterHitLatticeStorageHelper::~LatticeLinkCathodeClusterHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkCathodeClusterHitLatticeStorageHelper& LatticeLinkCathodeClusterHitLatticeStorageHelper::operator=( const LatticeLinkCathodeClusterHitLatticeStorageHelper& rhs )
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
LatticeLinkCathodeClusterHitLatticeStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase< CathodeClusterHitLattice >& iData ) 
{
   const LeftIDs& leftIDs = iData.vLeftID();
   const RightIDs& rightIDs = iData.vRightID();

   //store the left and right ids
   iSink << SMPackInfoFixedTemplate<LeftIDs::value_type>(
                   sm_field( "leftIDs", sm_contents_builtin( leftIDs.begin(), 
							     leftIDs.end() )),
		   0, 1600, 1)
     
	 << SMPackInfoFixedTemplate<RightIDs::value_type>(
		   sm_field( "rightIDs", sm_contents_builtin( rightIDs.begin(), 
							    rightIDs.end() ) ),
		   0, 1600, 1)
      //store info from link data

      ;
}

SMLatticeLinkInfoBase< CathodeClusterHitLattice >* 
LatticeLinkCathodeClusterHitLatticeStorageHelper::deliverV1( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< CathodeClusterHitLattice >* link = 
      new SMLatticeLinkInfoIn< CathodeClusterHitLattice>();

   //first read the left and right IDs
   CathodeClusterHitLattice::VectorLeftID leftIDs;
   CathodeClusterHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<LeftIDs> leftInsertIterator( leftIDs );
   back_insert_iterator<RightIDs> rightInsertIterator( rightIDs );

   iSource >> sm_fill_contents( leftInsertIterator )
	   >> sm_fill_contents( rightInsertIterator ) ;

   //now read data for the LinkData


   iSource 

      ;
   CathodeClusterHitLattice::LinkData linkData = 
      CathodeClusterHitLattice::LinkData( 


	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

//
// const member functions
//
const SMLatticeLinkInfoBase< CathodeClusterHitLattice >& 
LatticeLinkCathodeClusterHitLatticeStorageHelper::getDefault() const
{
   static const SMLatticeLinkInfoDefault< CathodeClusterHitLattice > 
      temp(  LeftIDs(1,LeftIDs::value_type(1)),
	     CathodeClusterHitLattice::LinkData(


		),
	     RightIDs(1,RightIDs::value_type(1)) );
   return temp;
}

DABoolean 
LatticeLinkCathodeClusterHitLatticeStorageHelper::compare( 
   const SMLatticeLinkInfoBase< CathodeClusterHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< CathodeClusterHitLattice >& iOldData ) const
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
SM_REGISTER_STORAGE( LatticeLinkCathodeClusterHitLatticeStorageHelper );

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
typedef SMLatticeLinkInfoBase< CathodeClusterHitLattice > CathodeClusterHitLatticeLink;
INSTANTIATE_ONE( CathodeClusterHitLatticeLink )
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( LatticeLinkCathodeClusterHitLatticeStorageHelper, SMLatticeLinkInfoBase< CathodeClusterHitLattice > )

typedef CathodeClusterHitLattice _lattice_element_;
#include "StorageManagement/instantiate_latticelinkhelper.h"



