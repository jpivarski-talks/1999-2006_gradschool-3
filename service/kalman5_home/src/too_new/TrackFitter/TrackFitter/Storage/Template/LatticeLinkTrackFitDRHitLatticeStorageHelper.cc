// -*- C++ -*-
//
// Package:     <TrackFitter>
// Module:      TrackFitDRHitLatticeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sun Feb  6 16:35:10 EST 2000
// $Id: LatticeLinkTrackFitDRHitLatticeStorageHelper.cc,v 1.11 2002/02/14 20:07:03 cleo3 Exp $
//
// Revision history
//
// $Log: LatticeLinkTrackFitDRHitLatticeStorageHelper.cc,v $
// Revision 1.11  2002/02/14 20:07:03  cleo3
// changed needed for Solaris8
//
// Revision 1.10  2001/09/26 08:01:26  wsun
// Added signed drift distance as data member.
//
// Revision 1.9  2000/10/11 06:31:28  wsun
// Added fittingWeight() to TrackFitDRHitLink.
//
// Revision 1.8  2000/08/16 19:34:17  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.7  2000/08/16 19:34:12  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.6  2000/08/16 19:34:02  wsun
// Removed curvature from TrackFitHitLink.
//
// Revision 1.5  2000/06/27 23:35:39  wsun
// Fixed bugs for OSF1 compilation.
//
// Revision 1.4  2000/06/26 18:17:18  wsun
// Added deliverV2 and added compression hints.
//
// Revision 1.3  2000/06/16 19:30:24  marsh
// Purged tool box typedefs.
//
// Revision 1.2  2000/06/09 00:20:42  wsun
// Updated to new TrackFitDRHitLink.
//
// Revision 1.1  2000/02/24 17:26:25  wsun
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
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAItem.h"

#include "TrackFitter/Storage/LatticeLinkTrackFitDRHitLatticeStorageHelper.h"
#include "TrackFinder/SeedDRHitLink.h"

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

static const char* const kSMFacilityString = "TrackFitter.LatticeLinkTrackFitDRHitLatticeStorageHelper" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kSMIdString  = "$Id: LatticeLinkTrackFitDRHitLatticeStorageHelper.cc,v 1.11 2002/02/14 20:07:03 cleo3 Exp $";
static const char* const kSMTagString = "$Name:  $";

// //Simplify some names
// typedef TrackFitDRHitLattice::VectorLeftID LeftIDs;
// typedef TrackFitDRHitLattice::VectorRightID RightIDs;

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
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::
LatticeLinkTrackFitDRHitLatticeStorageHelper()
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion(
      &LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV1 );
   registerVersion(
      &LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV2 );
   registerVersion(
      &LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV3 );
   registerVersion(
      &LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV4 );
   registerVersion(
      &LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV5 );
}

// LatticeLinkTrackFitDRHitLatticeStorageHelper::LatticeLinkTrackFitDRHitLatticeStorageHelper( const LatticeLinkTrackFitDRHitLatticeStorageHelper& rhs )
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
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::
~LatticeLinkTrackFitDRHitLatticeStorageHelper()
{
}

//
// assignment operators
//
// const LatticeLinkTrackFitDRHitLatticeStorageHelper& LatticeLinkTrackFitDRHitLatticeStorageHelper::operator=( const LatticeLinkTrackFitDRHitLatticeStorageHelper& rhs )
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
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::implementStore( 
   SMSinkStream& iSink,
   const SMLatticeLinkInfoBase<
      typename LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::TrackFitDRHitLattice >&
      iData ) 
{
   const TrackFitDRHitLattice::VectorLeftID& leftIDs = iData.vLeftID();
   const TrackFitDRHitLattice::VectorRightID& rightIDs = iData.vRightID();

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

// Between deliverV1 and deliverV2, the signedDriftDistance (corrected by
// the fitter) data member was added to TrackFitDRHitLink.  For backwards
// compatibility, deliverV1 initializes the TrackFitDRHitLink with the
// signed drift as determined by the track FINDER, not the track FITTER.

// wsun 08-08-00: removed TrackFitHitLink::curvature(), so deliverV1 and
// deliverV2 read in curvature from the database, but the value is ignored
// when instantiating the TrackFitDRHitLink.

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
SMLatticeLinkInfoBase<
   typename LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::TrackFitDRHitLattice >* 
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV1(
   SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< TrackFitDRHitLattice >* link = 
      new SMLatticeLinkInfoIn< TrackFitDRHitLattice >();

   //first read the left and right IDs
   TrackFitDRHitLattice::VectorLeftID leftIDs;
   TrackFitDRHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<TrackFitDRHitLattice::VectorLeftID>
      leftInsertIterator( leftIDs );
   back_insert_iterator<TrackFitDRHitLattice::VectorRightID>
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
      >> refx >> refy >> refz
      >> signedDcaToWire 
      >> signedDcaError 
      >> sinTrackToRadial 
      ;

   // Extract SeedTrackDRHitLattice and get the signed drift distance
   // for this hit.
   FAItem< SeedTrackDRHitLattice > lattice ;
   extract( iSource.record(), lattice ) ;

   // Given the topology of the SeedTrackDRHitLattice (each DR hit can have
   // only one link), we can ask for a pointer to the single link for this
   // hit.
   const SeedTrackDRHitLattice::Link* seedLink =
      lattice->linkGivenRight( rightIDs.begin() );

   static int warningCount = 0 ;
   Meters signedDriftDistance ;
   if( seedLink != 0 )
   {
      signedDriftDistance = seedLink->linkData().signedDriftDistance() ;

      if( warningCount == 0 )
      {
	 ++warningCount ;
	 report( WARNING, kSMFacilityString )
	    << "Copying signed drift dist from SeedDRHitLink into "
	    << "TrackFitDRHitLink." << endl ;
      }
   }
   else
   {
      signedDriftDistance = 0. ;

      report( WARNING, kSMFacilityString )
	 << "Signed drift dist in TrackFitDRHitLink set to zero because "
	 << "of topology error in SeedTrackDRHitLattice." << endl ;
   }

   // Extract CalibratedDRHits and get the fitting weight for this hit.
   FATable< CalibratedDRHit > drHits ;
   extract( iSource.record(), drHits ) ;
   double weight = drHits.find( *( rightIDs.begin() ) )->weight() ;

   TrackFitDRHitLattice::LinkData linkData = 
      TrackFitDRHitLattice::LinkData( 
	 used,
	 residual,   
	 residualError,
	 momentum,   
	 HepPoint3D( refx, refy, refz ),
	 signedDcaToWire,   
	 signedDcaError,   
	 sinTrackToRadial,
	 signedDriftDistance,
	 weight,
	 residual + signedDcaToWire
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
   typename LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::TrackFitDRHitLattice >* 
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV2(
   SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< TrackFitDRHitLattice >* link = 
      new SMLatticeLinkInfoIn< TrackFitDRHitLattice >();

   //first read the left and right IDs
   TrackFitDRHitLattice::VectorLeftID leftIDs;
   TrackFitDRHitLattice::VectorRightID rightIDs;
   
   back_insert_iterator<TrackFitDRHitLattice::VectorLeftID>
      leftInsertIterator( leftIDs );
   back_insert_iterator<TrackFitDRHitLattice::VectorRightID>
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
   CalibratedTimeHit::PicoSec driftTime ;

   iSource 
      >> used 
      >> residual 
      >> residualError 
      >> curvature 
      >> momentum 
      >> refx >> refy >> refz
      >> signedDcaToWire 
      >> signedDcaError 
      >> sinTrackToRadial 
      >> driftTime
      ;

   // Extract CalibratedDRHits and get the fitting weight for this hit.
   FATable< CalibratedDRHit > drHits ;
   extract( iSource.record(), drHits ) ;
   double weight = drHits.find( *( rightIDs.begin() ) )->weight() ;

   TrackFitDRHitLattice::LinkData linkData = 
      TrackFitDRHitLattice::LinkData( 
	 used,
	 residual,   
	 residualError,   
	 momentum,   
	 HepPoint3D( refx, refy, refz ),
	 signedDcaToWire,   
	 signedDcaError,   
	 sinTrackToRadial,
	 driftTime,
	 weight,
	 residual + signedDcaToWire
	 );
   //Add all the information to the link
   link->set( leftIDs, linkData, rightIDs );
   return link;
}

// Between deliverV2 and deliverV3, the lists of leftIDs and rightIDs were
// replaced by leftIDs[0] and rightIDs[0] -- each link connects only one
// track and only one hit.
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
SMLatticeLinkInfoBase<
   typename LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::TrackFitDRHitLattice >* 
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV3(
   SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< TrackFitDRHitLattice >* link = 
      new SMLatticeLinkInfoIn< TrackFitDRHitLattice >();

   //first read the left and right IDs
   TrackFitDRHitLattice::VectorLeftID leftIDs;
   TrackFitDRHitLattice::VectorRightID rightIDs;
   
   TrackFitDRHitLattice::VectorLeftID::value_type leftID ;
   TrackFitDRHitLattice::VectorRightID::value_type rightID ;

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
      ;

   // Extract CalibratedDRHits and get the fitting weight for this hit.
   FATable< CalibratedDRHit > drHits ;
   extract( iSource.record(), drHits ) ;
   double weight = drHits.find( *( rightIDs.begin() ) )->weight() ;

   TrackFitDRHitLattice::LinkData linkData = 
      TrackFitDRHitLattice::LinkData( 
	 used,
	 residual,   
	 residualError,   
	 momentum,   
	 HepPoint3D( refx, refy, refz ),
	 signedDcaToWire,   
	 signedDcaError,   
	 sinTrackToRadial,
	 driftTime,
	 weight,
	 residual + signedDcaToWire
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
   typename LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::TrackFitDRHitLattice >* 
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV4(
   SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< TrackFitDRHitLattice >* link = 
      new SMLatticeLinkInfoIn< TrackFitDRHitLattice >();

   //first read the left and right IDs
   TrackFitDRHitLattice::VectorLeftID leftIDs;
   TrackFitDRHitLattice::VectorRightID rightIDs;
   
   TrackFitDRHitLattice::VectorLeftID::value_type leftID ;
   TrackFitDRHitLattice::VectorRightID::value_type rightID ;

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
      ;

   TrackFitDRHitLattice::LinkData linkData = 
      TrackFitDRHitLattice::LinkData( 
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
	 residual + signedDcaToWire
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
   typename LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::TrackFitDRHitLattice >* 
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::deliverV5(
   SMSourceStream& iSource )
{
   SMLatticeLinkInfoBase< TrackFitDRHitLattice >* link = 
      new SMLatticeLinkInfoIn< TrackFitDRHitLattice >();

   //first read the left and right IDs
   TrackFitDRHitLattice::VectorLeftID leftIDs;
   TrackFitDRHitLattice::VectorRightID rightIDs;
   
   TrackFitDRHitLattice::VectorLeftID::value_type leftID ;
   TrackFitDRHitLattice::VectorRightID::value_type rightID ;

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

   TrackFitDRHitLattice::LinkData linkData = 
      TrackFitDRHitLattice::LinkData( 
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
   typename LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::TrackFitDRHitLattice >& 
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::getDefault() const
{
   static const SMLatticeLinkInfoDefault< TrackFitDRHitLattice > 
      temp( TrackFitDRHitLattice::VectorLeftID(
	 1,TrackFitDRHitLattice::VectorLeftID::value_type(1)),
	     TrackFitDRHitLattice::LinkData(
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
	     TrackFitDRHitLattice::VectorRightID(
		1,TrackFitDRHitLattice::VectorRightID::value_type(1)) );
   return temp;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
DABoolean 
LatticeLinkTrackFitDRHitLatticeStorageHelper<H>::compare( 
   const SMLatticeLinkInfoBase< TrackFitDRHitLattice >* iNewData, 
   const SMLatticeLinkInfoBase< TrackFitDRHitLattice >& iOldData ) const
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

// SM_REGISTER_STORAGE( LatticeLinkTrackFitDRHitLatticeStorageHelper );

// //this is done to create a TypeTag and its name
// #include "FrameAccess/instantiate_macros.h"
// typedef SMLatticeLinkInfoBase< TrackFitDRHitLattice > TrackFitDRHitLatticeLink;
// INSTANTIATE_ONE( TrackFitDRHitLatticeLink )
// #include "StorageManagement/instantiate_one_typetag.h"

// #include "StorageManagement/SMDeliverer.h"
// SM_REGISTER_VERSION_DEF( LatticeLinkTrackFitDRHitLatticeStorageHelper, SMLatticeLinkInfoBase< TrackFitDRHitLattice > )

// typedef TrackFitDRHitLattice _lattice_element_;
// #include "StorageManagement/instantiate_latticelinkhelper.h"
