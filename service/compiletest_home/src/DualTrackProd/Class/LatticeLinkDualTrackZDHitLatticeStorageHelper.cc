// -*- C++ -*-
//
// Package:     <package>
// Module:      DualTrackZDHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Oct  6 10:45:44 EDT 1999
// $Id: LatticeLinkDualTrackZDHitLatticeStorageHelper.cc,v 1.1 2003/12/15 17:53:20 mccann Exp $
//
// Revision history
//
// $Log: LatticeLinkDualTrackZDHitLatticeStorageHelper.cc,v $
// Revision 1.1  2003/12/15 17:53:20  mccann
// adding hit lattices for dual-constrained residuals
//
// Revision 1.5  2003/04/04 21:34:44  cdj
// added compile time checking of data in deliver function matches type stored
//
// Revision 1.4  2001/01/31 22:16:03  cdj
// added reminder to users in storage routine to register new versions
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
#include "DualTrackProd/LatticeLinkDualTrackZDHitLatticeStorageHelper.h"

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

#include "StorageManagement/smTestReturnTypeMatchesVariable.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "package.LatticeLinkDualTrackZDHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: LatticeLinkDualTrackZDHitLatticeStorageHelper.cc,v 1.1 2003/12/15 17:53:20 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//Simplify some names
typedef DualTrackZDHitLattice::VectorLeftID LeftIDs;
typedef DualTrackZDHitLattice::VectorRightID RightIDs;

//
// static data member definitions
//

//
// constructors and destructor
//
LatticeLinkDualTrackZDHitLatticeStorageHelper::LatticeLinkDualTrackZDHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &LatticeLinkDualTrackZDHitLatticeStorageHelper::deliverV1 );
}

// LatticeLinkDualTrackZDHitLatticeStorageHelper::LatticeLinkDualTrackZDHitLatticeStorageHelper( const LatticeLinkDualTrackZDHitLatticeStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

LatticeLinkDualTrackZDHitLatticeStorageHelper::~LatticeLinkDualTrackZDHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkDualTrackZDHitLatticeStorageHelper& LatticeLinkDualTrackZDHitLatticeStorageHelper::operator=( const LatticeLinkDualTrackZDHitLatticeStorageHelper& rhs )
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
LatticeLinkDualTrackZDHitLatticeStorageHelper::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase< DualTrackZDHitLattice >& iData ) 
{
// NOTE: Any change (adding/removing variable OR packing info) to this routine
//    MUST be accompanied by registering a new version deliver routine in
//    this class's constructor.  
//    If only the packing info is changes, you can register the same deliver
//    routine a second time.

   const LeftIDs& leftIDs = iData.vLeftID();
   const RightIDs& rightIDs = iData.vRightID();

   //store the left and right ids
   iSink << sm_field( "leftIDs", sm_contents_builtin( leftIDs.begin(), 
						      leftIDs.end() ))
	 << sm_field( "rightIDs", sm_contents_builtin( rightIDs.begin(), 
						       rightIDs.end() ) )
      //store info from link data
	 << SM_VAR( iData, linkData().used )  
	 << SM_VAR( iData, linkData().residual )  
	 << SM_VAR( iData, linkData().residualError )  
	 << SM_VAR( iData, linkData().momentum )  
	 << SM_VAR( iData, linkData().trackRefPt().x )  
	 << SM_VAR( iData, linkData().trackRefPt().y )  
	 << SM_VAR( iData, linkData().trackRefPt().z )  
	 << SM_VAR( iData, linkData().signedDcaToWire )  
	 << SM_VAR( iData, linkData().signedDcaError )  
	 << SM_VAR( iData, linkData().sinTrackToRadial )  
	 << SM_VAR( iData, linkData().driftTime )  
	 << SM_VAR( iData, linkData().fittingWeight )  
	 << SM_VAR( iData, linkData().signedDriftDistance )  
	 << SM_VAR( iData, linkData().singleTrack_residual )  
	 << SM_VAR( iData, linkData().singleTrack_residualError )  
	 << SM_VAR( iData, linkData().singleTrack_momentum )  
	 << SM_VAR( iData, linkData().singleTrack_trackRefPt().x )  
	 << SM_VAR( iData, linkData().singleTrack_trackRefPt().y )  
	 << SM_VAR( iData, linkData().singleTrack_trackRefPt().z )  
	 << SM_VAR( iData, linkData().singleTrack_signedDcaToWire )  
	 << SM_VAR( iData, linkData().singleTrack_signedDcaError )  
	 << SM_VAR( iData, linkData().singleTrack_sinTrackToRadial )  
      ;
}

SMLatticeLinkInfoBase< DualTrackZDHitLattice >* 
LatticeLinkDualTrackZDHitLatticeStorageHelper::deliverV1( SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< DualTrackZDHitLattice >* link = 
      new SMLatticeLinkInfoIn< DualTrackZDHitLattice>();

   //first read the left and right IDs
   DualTrackZDHitLattice::VectorLeftID leftIDs;
   DualTrackZDHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<LeftIDs> leftInsertIterator( leftIDs );
   back_insert_iterator<RightIDs> rightInsertIterator( rightIDs );

   iSource >> sm_fill_contents( leftInsertIterator )
	   >> sm_fill_contents( rightInsertIterator ) ;

   //now read data for the LinkData
   //NOTE: variable type must match type used to store data
   DABoolean used; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,used); 
   double residual; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,residual); 
   double residualError; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,residualError); 
   double momentum; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,momentum); 
   HepPoint3D trackRefPt; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,trackRefPt); 
   Meters signedDcaToWire; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,signedDcaToWire); 
   Meters signedDcaError; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,signedDcaError); 
   double sinTrackToRadial; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,sinTrackToRadial); 
   CalibratedTimeHit::PicoSec driftTime; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,driftTime); 
   double fittingWeight; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,fittingWeight); 
   Meters signedDriftDistance; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,signedDriftDistance); 
   double singleTrack_residual; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,singleTrack_residual); 
   double singleTrack_residualError; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,singleTrack_residualError); 
   double singleTrack_momentum; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,singleTrack_momentum); 
   HepPoint3D singleTrack_trackRefPt; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,singleTrack_trackRefPt); 
   Meters singleTrack_signedDcaToWire; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,singleTrack_signedDcaToWire); 
   Meters singleTrack_signedDcaError; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,singleTrack_signedDcaError); 
   double singleTrack_sinTrackToRadial; TEST_RETURN_TYPE(DualTrackZDHitLattice::LinkData,singleTrack_sinTrackToRadial); 
   
   unsigned int tmp_used;
   double tmp_trackRefPt_x, tmp_trackRefPt_y, tmp_trackRefPt_z;
   double tmp_singleTrack_trackRefPt_x, tmp_singleTrack_trackRefPt_y, tmp_singleTrack_trackRefPt_z;

   iSource 
      >> tmp_used 
      >> residual 
      >> residualError 
      >> momentum 
      >> tmp_trackRefPt_x >> tmp_trackRefPt_y >> tmp_trackRefPt_z 
      >> signedDcaToWire 
      >> signedDcaError 
      >> sinTrackToRadial 
      >> driftTime 
      >> fittingWeight 
      >> signedDriftDistance 
      >> singleTrack_residual 
      >> singleTrack_residualError 
      >> singleTrack_momentum 
      >> tmp_singleTrack_trackRefPt_x >> tmp_singleTrack_trackRefPt_y >> tmp_singleTrack_trackRefPt_z 
      >> singleTrack_signedDcaToWire 
      >> singleTrack_signedDcaError 
      >> singleTrack_sinTrackToRadial 
      ;

   used = (tmp_used == 0 ? false : true);
   trackRefPt = HepPoint3D(tmp_trackRefPt_x, tmp_trackRefPt_y, tmp_trackRefPt_z);
   singleTrack_trackRefPt = HepPoint3D(tmp_singleTrack_trackRefPt_x, tmp_singleTrack_trackRefPt_y, tmp_singleTrack_trackRefPt_z);

   DualTrackZDHitLattice::LinkData linkData = 
      DualTrackZDHitLattice::LinkData( 
	 used,
	 residual,
	 residualError,
	 momentum,
	 trackRefPt,
	 signedDcaToWire,
	 signedDcaError,
	 sinTrackToRadial,
	 driftTime,
	 fittingWeight,
	 signedDriftDistance,
	 singleTrack_residual,
	 singleTrack_residualError,
	 singleTrack_momentum,
	 singleTrack_trackRefPt,
	 singleTrack_signedDcaToWire,
	 singleTrack_signedDcaError,
	 singleTrack_sinTrackToRadial
	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

//
// const member functions
//
const SMLatticeLinkInfoBase< DualTrackZDHitLattice >& 
LatticeLinkDualTrackZDHitLatticeStorageHelper::getDefault() const
{
   static const SMLatticeLinkInfoDefault< DualTrackZDHitLattice > 
      temp(  LeftIDs(1,LeftIDs::value_type(1)),
	     DualTrackZDHitLattice::LinkData(
		DABoolean ( 1 ), // iUsed 
		double ( 2 ), // iResidual 
		double ( 3 ), // iResidualError 
		double ( 4 ), // iMomentum 
		HepPoint3D ( 5 ), // iTrackRefPt 
		Meters ( 6 ), // iSignedDcaToWire 
		Meters ( 7 ), // iSignedDcaError 
		double ( 8 ), // iSinTrackToRadial 
		CalibratedTimeHit::PicoSec ( 9 ), // iDriftTime 
		double ( 10 ), // iFittingWeight 
		Meters ( 11 ), // iSignedDriftDistance 
		double ( 12 ), // iSingleTrackResidual 
		double ( 13 ), // iSingleTrackResidualError 
		double ( 14 ), // iSingleTrackMomentum 
		HepPoint3D ( 15 ), // iSingleTrackTrackRefPt 
		Meters ( 16 ), // iSingleTrackSignedDcaToWire 
		Meters ( 17 ), // iSingleTrackSignedDcaError 
		double ( 18 )  // iSingleTrackSinTrackToRadial 
		),
	     RightIDs(1,RightIDs::value_type(1)) );
   return temp;
}

DABoolean 
LatticeLinkDualTrackZDHitLatticeStorageHelper::compare( 
   const SMLatticeLinkInfoBase< DualTrackZDHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< DualTrackZDHitLattice >& iOldData ) const
{
   //need to compare contents
   return ( 
            (iNewData->linkData() == iOldData.linkData() ) &&
// comment out the line above and uncomment the lines below if operator==
// does not exist for the link data
      //    (iNewData->linkData().iUsed() == iOldData.linkData().iUsed() ) && 
      //    (iNewData->linkData().iResidual() == iOldData.linkData().iResidual() ) && 
      //    (iNewData->linkData().iResidualError() == iOldData.linkData().iResidualError() ) && 
      //    (iNewData->linkData().iMomentum() == iOldData.linkData().iMomentum() ) && 
      //    (iNewData->linkData().iTrackRefPt() == iOldData.linkData().iTrackRefPt() ) && 
      //    (iNewData->linkData().iSignedDcaToWire() == iOldData.linkData().iSignedDcaToWire() ) && 
      //    (iNewData->linkData().iSignedDcaError() == iOldData.linkData().iSignedDcaError() ) && 
      //    (iNewData->linkData().iSinTrackToRadial() == iOldData.linkData().iSinTrackToRadial() ) && 
      //    (iNewData->linkData().iDriftTime() == iOldData.linkData().iDriftTime() ) && 
      //    (iNewData->linkData().iFittingWeight() == iOldData.linkData().iFittingWeight() ) && 
      //    (iNewData->linkData().iSignedDriftDistance() == iOldData.linkData().iSignedDriftDistance() ) && 
      //    (iNewData->linkData().iSingleTrackResidual() == iOldData.linkData().iSingleTrackResidual() ) && 
      //    (iNewData->linkData().iSingleTrackResidualError() == iOldData.linkData().iSingleTrackResidualError() ) && 
      //    (iNewData->linkData().iSingleTrackMomentum() == iOldData.linkData().iSingleTrackMomentum() ) && 
      //    (iNewData->linkData().iSingleTrackTrackRefPt() == iOldData.linkData().iSingleTrackTrackRefPt() ) && 
      //    (iNewData->linkData().iSingleTrackSignedDcaToWire() == iOldData.linkData().iSingleTrackSignedDcaToWire() ) && 
      //    (iNewData->linkData().iSingleTrackSignedDcaError() == iOldData.linkData().iSingleTrackSignedDcaError() ) && 
      //    (iNewData->linkData().iSingleTrackSinTrackToRadial() == iOldData.linkData().iSingleTrackSinTrackToRadial() ) && 
	    (iNewData->vLeftID() == iOldData.vLeftID() ) &&
	    (iNewData->vRightID() == iOldData.vRightID() ) );
}

//
// static member functions
//

//------------------------------------------------------------------------
// DO NOT DELETE
SM_REGISTER_STORAGE( LatticeLinkDualTrackZDHitLatticeStorageHelper );

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
typedef SMLatticeLinkInfoBase< DualTrackZDHitLattice > DualTrackZDHitLatticeLink;
INSTANTIATE_ONE( DualTrackZDHitLatticeLink )
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( LatticeLinkDualTrackZDHitLatticeStorageHelper, SMLatticeLinkInfoBase< DualTrackZDHitLattice > )

typedef DualTrackZDHitLattice _lattice_element_;
#include "StorageManagement/instantiate_latticelinkhelper.h"



