// -*- C++ -*-
//
// Package:     TestDualTrackProc
// Module:      TestDualTrackProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 17:15:20 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TestDualTrackProc/TestDualTrackProc.h"
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

#include "DualTrackProd/DualTrackZDHitLink.h"


#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixPionFit.h"
#include "TrackFitter/TrackFitZDHitLink.h"

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
static const char* const kFacilityString = "Processor.TestDualTrackProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
TestDualTrackProc::TestDualTrackProc( void )               // anal1
   : Processor( "TestDualTrackProc" )
   , m_id(0)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &TestDualTrackProc::event,    Stream::kEvent );
   //bind( &TestDualTrackProc::beginRun, Stream::kBeginRun );
   //bind( &TestDualTrackProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

TestDualTrackProc::~TestDualTrackProc()                    // anal5
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
TestDualTrackProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
TestDualTrackProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
TestDualTrackProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const char* label[kNumEntries] = {
      "id", "oldcurv", "oldphi0", "oldd0", "oldcotth", "oldz0",
      "curv", "phi0", "d0", "cotth", "z0",
      "resid", "reserr", "p", "pointx", "pointy", "pointz",
      "dca", "dcaerr", "sinalph", "time", "fw", "drift",
      "oldresid", "oldreserr", "oldp", "oldptx", "oldpty", "oldptz",
      "olddca", "olddcaerr", "oldsinalph"};
   m_ntuple = iHistoManager.ntuple(
      1, "vars", kNumEntries, 262144, label);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
TestDualTrackProc::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<TRHelixPionFit> normal_tracks;
   extract(iFrame.record(Stream::kEvent), normal_tracks);

   FATable<TRHelixPionFit> dual_tracks;
   extract(iFrame.record(Stream::kEvent), dual_tracks, "DualTrack");

   FAItem<PionFitZDHitLattice> normal_lattice;
   extract(iFrame.record(Stream::kEvent), normal_lattice);

   FAItem<PionFitZDHitLattice> dual_lattice;
   extract(iFrame.record(Stream::kEvent), dual_lattice, "DualTrack");

   FATable<CalibratedZDHit> hits;
   extract(iFrame.record(Stream::kEvent), hits);

   FAItem<DualTrack> dual;
   extract(iFrame.record(Stream::kEvent), dual);
   if (!dual->results().fitSuccessful()) return ActionBase::kFailed;

   for (FATable<TRHelixPionFit>::const_iterator normaltrack_iter = normal_tracks.begin();
	normaltrack_iter != normal_tracks.end();
	++normaltrack_iter) {
      FATable<TRHelixPionFit>::const_iterator dualtrack_iter = dual_tracks.find(normaltrack_iter->identifier());
      
      cout << "NORMAL "
	   << normaltrack_iter->curvature() << " "
	   << normaltrack_iter->phi0() << " "
	   << normaltrack_iter->d0() << " "
	   << normaltrack_iter->cotTheta() << " "
	   << normaltrack_iter->z0() << endl;
      cout << "DUAL   "
	   << dualtrack_iter->curvature() << " "
	   << dualtrack_iter->phi0() << " "
	   << dualtrack_iter->d0() << " "
	   << dualtrack_iter->cotTheta() << " "
	   << dualtrack_iter->z0() << endl;

      cout << "NORMAL ";
      const PionFitZDHitLattice::Links& normal_links = normal_lattice->linksGivenLeft(normaltrack_iter->identifier());
      PionFitZDHitLattice::Links::const_iterator normal_iter = normal_links.begin();
      PionFitZDHitLattice::Links::const_iterator normal_end = normal_links.end();
      for (; normal_iter != normal_end;  ++normal_iter) {
	 assert((**normal_iter).rightID() != NULL);
	 FATable<CalibratedZDHit>::const_iterator hit_iter = hits.find(*(**normal_iter).rightID());
	 const TrackFitZDHitLink& normal_linkData = (**normal_iter).linkData();

	 cout << hit_iter->wire() << ". " << normal_linkData.residual() << " ";
      }
      cout << endl;

      cout << "DUAL   ";
      const PionFitZDHitLattice::Links& dual_links = dual_lattice->linksGivenLeft(dualtrack_iter->identifier());
      PionFitZDHitLattice::Links::const_iterator dual_iter = dual_links.begin();
      PionFitZDHitLattice::Links::const_iterator dual_end = dual_links.end();
      for (; dual_iter != dual_end;  ++dual_iter) {
	 assert((**dual_iter).rightID() != NULL);
	 FATable<CalibratedZDHit>::const_iterator hit_iter = hits.find(*(**dual_iter).rightID());
	 const TrackFitZDHitLink& dual_linkData = (**dual_iter).linkData();

	 cout << hit_iter->wire() << ". " << dual_linkData.residual() << " ";
      }
      cout << endl;

      cout << endl;
   } // end of track loop

   return ActionBase::kPassed;

   /////////////////////////////////////////////////////

   FATable<NavTrack> navtracks;
   extract(iFrame.record(Stream::kEvent), navtracks);

   if (navtracks.size() != 2) return ActionBase::kFailed;

   FATable<DualTrackFitHelix> dualtracks;
   extract(iFrame.record(Stream::kEvent), dualtracks);

   FAItem<DualTrackZDHitLattice> newlattice;
   extract(iFrame.record(Stream::kEvent), newlattice);
   
   m_id++;
   for (FATable<DualTrackFitHelix>::const_iterator dual_iter = dualtracks.begin();
	dual_iter != dualtracks.end();
	++dual_iter) {
      float v[kNumEntries];

      FATable<NavTrack>::const_iterator nav_iter = navtracks.find(dual_iter->identifier());
      v[kId] = float(m_id) * (nav_iter->pionHelix()->curvature() > 0. ? 1. : -1.);
      v[kOldCurv] = nav_iter->pionHelix()->curvature();
      v[kOldPhi0] = nav_iter->pionHelix()->phi0();
      v[kOldD0] = nav_iter->pionHelix()->d0();
      v[kOldCotTh] = nav_iter->pionHelix()->cotTheta();
      v[kOldZ0] = nav_iter->pionHelix()->z0();

      v[kCurv] = dual_iter->curvature();
      v[kPhi0] = dual_iter->phi0();
      v[kD0] = dual_iter->d0();
      v[kCotTh] = dual_iter->cotTheta();
      v[kZ0] = dual_iter->z0();
      
      const DualTrackZDHitLattice::Links& newlinks = newlattice->linksGivenLeft(dual_iter->identifier());
      DualTrackZDHitLattice::Links::const_iterator newlink_iter = newlinks.begin();
      DualTrackZDHitLattice::Links::const_iterator newlink_end = newlinks.end();
      for (; newlink_iter != newlink_end;  ++newlink_iter) {
	 DualTrackZDHitLink& linkData = (**newlink_iter).linkData();
	 assert((**newlink_iter).rightID() != NULL);
	 if (linkData.used()) {
	    v[kResid] = linkData.residual();
	    v[kResidErr] = linkData.residualError();
	    v[kP] = linkData.momentum();
	    v[kPointX] = linkData.trackRefPt().x();
	    v[kPointY] = linkData.trackRefPt().y();
	    v[kPointZ] = linkData.trackRefPt().z();
	    v[kDca] = linkData.signedDcaToWire();
	    v[kDcaErr] = linkData.signedDcaError();
	    v[kSinAlph] = linkData.sinTrackToRadial();
	    v[kTime] = linkData.driftTime();
	    v[kFW] = linkData.fittingWeight();
	    v[kDrift] = linkData.signedDriftDistance();
	    v[kOldResid] = linkData.singleTrack_residual();
	    v[kOldResidErr] = linkData.singleTrack_residualError();
	    v[kOldP] = linkData.singleTrack_momentum();
	    v[kOldPointX] = linkData.singleTrack_trackRefPt().x();
	    v[kOldPointY] = linkData.singleTrack_trackRefPt().y();
	    v[kOldPointZ] = linkData.singleTrack_trackRefPt().z();
	    v[kOldDca] = linkData.singleTrack_signedDcaToWire();
	    v[kOldDcaErr] = linkData.singleTrack_signedDcaError();
	    v[kOldSinAlph] = linkData.singleTrack_sinTrackToRadial();

	    m_ntuple->fill(v);
	 } // end if used
      } // end link loop
   } // end track loop

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
TestDualTrackProc::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
TestDualTrackProc::endRun( Frame& iFrame )         // anal4 equiv.
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
