// -*- C++ -*-
//
// Package:     PointToError
// Module:      PointToError
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 26 17:01:42 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "PointToError/PointToError.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
#include "TrackRoot/TRHelixFit.h"
#include "TrackRoot/TRTrackFitQuality.h"
#include "TrackRoot/TRSeedTrackQuality.h"
#include "TrackDelivery/TDKinematicFit.h"
#include "CleoDB/DBEventHeader.h"
#include "EventProperties/EventProperties.h"
#include "Doit/DoitSeedQuality.h"
#include "Navigation/NavRich.h"

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
static const char* const kFacilityString = "Processor.PointToError" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
PointToError::PointToError( void )               // anal1
   : Processor( "PointToError" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &PointToError::event,    Stream::kEvent );
   //bind( &PointToError::beginRun, Stream::kBeginRun );
   //bind( &PointToError::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

PointToError::~PointToError()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
PointToError::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
PointToError::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
PointToError::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const char* label[kEntries] = {
      "curv", "phi0", "d0", "cottheta", "z0",
      "xcurv", "xphi0", "xd0", "xcottheta", "xz0",
      "ecurv", "ephi0", "ed0", "ecottheta", "ez0",
      "chisquare", "dof", "hitsexp", "hits", "fit", "fitabort",
      "xchisquare", "xdof", "xhitsexp", "xhits", "xfit", "xfitabort",
      "svrave", "svrrms", 
      "svzave", "svzrms", 
      "axave", "axrms", 
      "stave", "strms", 
      "cathave", "cathrms",
      "richanal"
   };
   m_ntuple = iHistoManager.ntuple(
      1, "bigerr", kEntries, 262144, label );

   char rad[6];
   for ( int j = 0;  j < 1500;  j++ ) {
      sprintf(rad, "%5.1f", float(j)/10.);
      m_healthy_z[j] = iHistoManager.histogram(
	 10000 + j, (string("healthy z dist at R = ") + string(rad) + string(" mm")).c_str(),
	 500, -0.5, 0.5 );
      m_sick_z[j] = iHistoManager.histogram(
	 20000 + j, (string("sick z dist at R = ") + string(rad) + string(" mm")).c_str(),
	 500, -0.5, 0.5 );
   }

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
PointToError::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable< NavTrack > tracks;
   extract( iFrame.record( Stream::kEvent ), tracks );
   FATable< NavTrack >::const_iterator track_iter;
   FATable< NavTrack >::const_iterator tracks_begin = tracks.begin();
   FATable< NavTrack >::const_iterator tracks_end = tracks.end();

   report( DEBUG, kFacilityString ) << "extracted tracks" << endl;

//    FATable< DoitSeedQuality > seedquals;
//    extract( iFrame.record( Stream::kEvent ), seedquals );
//    FATable< DoitSeedQuality >::const_iterator seedqual;

//    report(INFO, kFacilityString ) << "Number of tracks is " << tracks.size() << endl;

   for ( track_iter = tracks_begin;  track_iter != tracks_end;  track_iter++ ) {
      float v[kEntries];

      FAItem<NavRich> rich = track_iter->richInfo();

      report( DEBUG, kFacilityString ) << "let's begin now" << endl;
   
      v[kCurv] = track_iter->pionHelix()->curvature();
      v[kPhi0] = track_iter->pionHelix()->phi0();
      v[kD0] = track_iter->pionHelix()->d0();
      v[kCotTheta] = track_iter->pionHelix()->cotTheta();
      v[kZ0] = track_iter->pionHelix()->z0();
      
      report( DEBUG, kFacilityString )
	 << "one "
	 << track_iter->exitPionHelix().valid() << " "
	 << endl;
   
      if ( track_iter->exitPionHelix().valid() ) {
	 v[kXCurv] = track_iter->exitPionHelix()->curvature();
	 v[kXPhi0] = track_iter->exitPionHelix()->phi0();
	 v[kXD0] = track_iter->exitPionHelix()->d0();
	 v[kXCotTheta] = track_iter->exitPionHelix()->cotTheta();
	 v[kXZ0] = track_iter->exitPionHelix()->z0();
      }
      else {
	 v[kXCurv] = 1000.;
	 v[kXPhi0] = 1000.;
	 v[kXD0] = 1000.;
	 v[kXCotTheta] = 1000.;
	 v[kXZ0] = 1000.;
      }
      
      report( DEBUG, kFacilityString ) << "two" << endl;

      if ( track_iter->exitPionHelix().valid() ) {
	 v[kECurv] = track_iter->exitPionHelix()->errorMatrix()(KTHelix::kCurvature,KTHelix::kCurvature);
	 v[kEPhi0] = track_iter->exitPionHelix()->errorMatrix()(KTHelix::kPhi0,KTHelix::kPhi0);
	 v[kED0] = track_iter->exitPionHelix()->errorMatrix()(KTHelix::kD0,KTHelix::kD0);
	 v[kECotTheta] = track_iter->exitPionHelix()->errorMatrix()(KTHelix::kCotTheta,KTHelix::kCotTheta);
	 v[kEZ0] = track_iter->exitPionHelix()->errorMatrix()(KTHelix::kZ0,KTHelix::kZ0);
      }
      else {
	 v[kECurv] = 1000;
	 v[kEPhi0] = 1000;
	 v[kED0] = 1000;
	 v[kECotTheta] = 1000;
	 v[kEZ0] = 1000;
      }
      
      report( DEBUG, kFacilityString ) << "three" << endl;
	 
      v[kChiSquare] = track_iter->pionQuality()->chiSquare();
      v[kDOF] = float( track_iter->pionQuality()->degreesOfFreedom() );
      v[kHitsExp] = float( track_iter->pionQuality()->numberHitsExpected() );
      v[kHits] = track_iter->pionQuality()->numberHits();
      v[kFit] = ( track_iter->pionQuality()->fit() ? 1. : 0. );
      v[kFitAbort] = ( track_iter->pionQuality()->fitAbort() ? 1. : 0. );
      
      report( DEBUG, kFacilityString ) << "four" << endl;

      if ( track_iter->exitPionQuality().valid() ) {
	 v[kXChiSquare] = track_iter->exitPionQuality()->chiSquare();
	 v[kXDOF] = float( track_iter->exitPionQuality()->degreesOfFreedom() );
	 v[kXHitsExp] = float( track_iter->exitPionQuality()->numberHitsExpected() );
	 v[kXHits] = track_iter->exitPionQuality()->numberHits();
	 v[kXFit] = ( track_iter->exitPionQuality()->fit() ? 1. : 0. );
	 v[kXFitAbort] = ( track_iter->exitPionQuality()->fitAbort() ? 1. : 0. );
      }
      else {
	 v[kXChiSquare] = 1000;
	 v[kXDOF] = 1000;
	 v[kXHitsExp] = 1000;
	 v[kXHits] = 1000;
	 v[kXFit] = 1000;
	 v[kXFitAbort] = 1000;
      }
      
      report( DEBUG, kFacilityString ) << "five" << endl;

      v[kSVRave] = track_iter->pionQuality()->averageResidual(TRSubdetectorLists::kSVR);
      v[kSVRrms] = track_iter->pionQuality()->rmsResidual(TRSubdetectorLists::kSVR);
      
      report( DEBUG, kFacilityString ) << "six" << endl;

      v[kSVZave] = track_iter->pionQuality()->averageResidual(TRSubdetectorLists::kSVZ);
      v[kSVZrms] = track_iter->pionQuality()->rmsResidual(TRSubdetectorLists::kSVZ);
      
      report( DEBUG, kFacilityString ) << "seven" << endl;

      v[kAXave] = track_iter->pionQuality()->averageResidual(TRSubdetectorLists::kDR3Axial);
      v[kAXrms] = track_iter->pionQuality()->rmsResidual(TRSubdetectorLists::kDR3Axial);
      
      report( DEBUG, kFacilityString ) << "eight" << endl;

      v[kSTave] = track_iter->pionQuality()->averageResidual(TRSubdetectorLists::kDR3Stereo);
      v[kSTrms] = track_iter->pionQuality()->rmsResidual(TRSubdetectorLists::kDR3Stereo);
      
      report( DEBUG, kFacilityString ) << "nine" << endl;

      v[kCATHave] = track_iter->pionQuality()->averageResidual(TRSubdetectorLists::kDRCathode);
      v[kCATHrms] = track_iter->pionQuality()->rmsResidual(TRSubdetectorLists::kDRCathode);
      
      report( DEBUG, kFacilityString ) << "ten" << endl;

      if ( rich.valid() ) {
	 v[kRICHanal] = ( rich->pionHypWasAnalyzed() ? 1. : 0. );
      }
      else {	
	 v[kRICHanal] = -1.;
      }

      m_ntuple->fill(v);
      
      report( DEBUG, kFacilityString ) << "flush!" << endl;


//       seedqual = seedquals.find( track_iter->identifier() );

//       int i(0);
//       double pionErr(0.);
//       double exitPionErr(0.);
//       DABoolean sick(false);

//       for ( i = 1;  i <= 5;  i++ )
//  	 pionErr += abs(track_iter->pionHelix()->errorMatrix()(i,i));

//       for ( i = 1;  i <= 5;  i++ )
//  	 exitPionErr += abs(track_iter->exitPionHelix()->errorMatrix()(i,i));

//       report(NOTICE, kFacilityString)
// 	 << "OINK track " << track_iter->identifier()
// 	 << " of " << tracks.size() << " z0_z0error "
// 	 << track_iter->exitPionHelix()->errorMatrix()(KTHelix::kZ0, KTHelix::kZ0)
// 	 << " fitabort " << track_iter->pionQuality()->fitAbort()
// 	 << " fit " << track_iter->pionQuality()->fit()
// 	 << " numberHitsInput " << track_iter->pionQuality()->numberHitsInput()
// 	 << " numberHitsDropped " << track_iter->pionQuality()->numberHitsDropped()
// 	 << " numberIterations " << track_iter->pionQuality()->numberIterations()
// 	 << " chiSquare " << track_iter->pionQuality()->chiSquare()
// 	 << " dof " << track_iter->pionQuality()->degreesOfFreedom()
// 	 << " svzhits " << track_iter->pionQuality()->numberSubdetectorHitLayers(TRSubdetectorLists::kSVZ)
// 	 << " dr3hits " << track_iter->pionQuality()->numberSubdetectorHitLayers(TRSubdetectorLists::kDR3Stereo)
// 	 << " cathodehits " << track_iter->pionQuality()->numberSubdetectorHitLayers(TRSubdetectorLists::kDRCathode)
// 	 << " goodZ " << seedqual->goodZ()
// 	 << " inwardDoitCurler " << seedqual->inwardDoitCurler() << endl;

//       if ( track_iter->pionHelix()->cotTheta() < 0.  &&
// 	   track_iter->pionHelix()->z0() < 0.        &&
// 	   pionErr > 0  &&  exitPionErr / pionErr > 1e4 )
//       {
// 	 cout << "Track " << track_iter->identifier() << " at "
// 	      << track_iter->pionHelix()->curvature() << " "
// 	      << track_iter->pionHelix()->phi0() << " "
// 	      << track_iter->pionHelix()->d0() << " "
// 	      << track_iter->pionHelix()->cotTheta() << " "
// 	      << track_iter->pionHelix()->z0() << " "
// 	      << "has error matrices of "
// 	      << track_iter->pionHelix()->errorMatrix()
// 	      << " and "
// 	      << track_iter->exitPionHelix()->errorMatrix()
// 	      << endl;
//       }

//      if ( pionErr > 0  &&  exitPionErr / pionErr > 1e3 ) {
// 	 sick = true;

// 	 float v[kEntries];
// 	 v[kCurvature] = track_iter->pionHelix()->curvature();
// 	 v[kPhi0] = track_iter->pionHelix()->phi0();
// 	 v[kD0] = track_iter->pionHelix()->d0();
// 	 v[kCotTheta] = track_iter->pionHelix()->cotTheta();
// 	 v[kZ0] = track_iter->pionHelix()->z0();
// 	 v[kErrRatio] = exitPionErr / pionErr;

// 	 m_ntuple->fill(v);
//       } // end if sick

//       KTHelix helix(*track_iter->pionHelix());
//       double zat2cm(0.);
//       for ( int j = 0;  j < 1500;  j++ ) {
// 	 double dummy;
// 	 helix.moveToRadius(float(j)/10000., dummy);
// 	 double z = helix.referencePoint().z();
// 	 if ( sick ) m_sick_z[j]->fill(z);
// 	 else m_healthy_z[j]->fill(z);
// 	 if( j == 200 ) zat2cm = z;
//       } // end loop over radii

//	 report(SYSTEM, kFacilityString)
// 	 << "NTUPLE id " << track_iter->identifier()
// 	 << " zat2cm " << zat2cm
//	    << " sigz2 " << track_iter->exitPionHelix()->errorMatrix()(KTHelix::kZ0, KTHelix::kZ0)
//	    << endl;

   } // end loop over tracks

   report( DEBUG, kFacilityString ) << "not my problem anymore" << endl;
   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
PointToError::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
PointToError::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

//
// const member functions
//

//
// static member functions
//
