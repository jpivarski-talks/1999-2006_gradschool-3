// -*- C++ -*-
//
// Package:     TestTracking
// Module:      TestTracking
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Jul 21 11:47:51 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TestTracking/TestTracking.h"
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
#include "CalibratedData/CalibratedZDHit.h"
#include "TrackFinder/SeedZDHitLink.h"
#include "TrackFitter/TrackFitZDHitLink.h"

#include "MCInfo/MCDecayTree/MCDecayTree.h"
#include "MCInfo/MCDecayTree/MCParticle.h"
#include "MCInfo/MCParticleProperty/MCParticleProperty.h"
#include "MCInfo/MCParticleProperty/MCParticlePropertyStore.h"

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
static const char* const kFacilityString = "Processor.TestTracking" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
TestTracking::TestTracking( void )               // anal1
   : Processor( "TestTracking" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &TestTracking::event,    Stream::kEvent );
   //bind( &TestTracking::beginRun, Stream::kBeginRun );
   //bind( &TestTracking::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

TestTracking::~TestTracking()                    // anal5
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
TestTracking::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
TestTracking::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
TestTracking::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   const char* label[kNumEntries] = {
      "seedcu", "seedph", "seedd0", "seedct", "seedz0",
      "pioncu", "pionph", "piond0", "pionct", "pionz0",
      "px", "py", "pz",
      "chi2", "dof", "hitexp", "hitrat", "hitin", "fabort", "hitdrop", "iters",
      "l1wire", "l1q", "l1drift", "l1dre", "l1dca", "l1dcae", "l1sina", "l1used", "l1seeddr",
      "l2wire", "l2q", "l2drift", "l2dre", "l2dca", "l2dcae", "l2sina", "l2used", "l2seeddr",
      "l3wire", "l3q", "l3drift", "l3dre", "l3dca", "l3dcae", "l3sina", "l3used", "l3seeddr",
      "l4wire", "l4q", "l4drift", "l4dre", "l4dca", "l4dcae", "l4sina", "l4used", "l4seeddr",
      "l5wire", "l5q", "l5drift", "l5dre", "l5dca", "l5dcae", "l5sina", "l5used", "l5seeddr",
      "l6wire", "l6q", "l6drift", "l6dre", "l6dca", "l6dcae", "l6sina", "l6used", "l6seeddr",
      "mcx", "mcy", "mcz"
   };
   m_ntuple = iHistoManager.ntuple(
      1, "vars", kNumEntries, 262144, label);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
TestTracking::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> navtrack;
   extract(iFrame.record(Stream::kEvent), navtrack);
   FATable<NavTrack>::const_iterator nt_end(navtrack.end());

   FATable<CalibratedZDHit> calibratedzdhit;
   extract(iFrame.record(Stream::kEvent), calibratedzdhit);

   FAItem<SeedTrackZDHitLattice> seedtrackzdhitlattice;
   extract(iFrame.record(Stream::kEvent), seedtrackzdhitlattice);
   
   FAItem<PionFitZDHitLattice> pionfitzdhitlattice;
   extract(iFrame.record(Stream::kEvent), pionfitzdhitlattice);

   for (FATable<NavTrack>::const_iterator nt_iter(navtrack.begin());
	nt_iter != nt_end;  ++nt_iter) {
      float v[kNumEntries];
      for (int i(0);  i < kNumEntries;  i++) v[i] = -1000.;

      v[kSeedCurvature] = nt_iter->seedTrack()->curvature();
      v[kSeedPhi0] = nt_iter->seedTrack()->phi0();
      v[kSeedD0] = nt_iter->seedTrack()->d0();
      v[kSeedCotTheta] = nt_iter->seedTrack()->cotTheta();
      v[kSeedZ0] = nt_iter->seedTrack()->z0();

      v[kPionCurvature] = nt_iter->pionHelix()->curvature();
      v[kPionPhi0] = nt_iter->pionHelix()->phi0();
      v[kPionD0] = nt_iter->pionHelix()->d0();
      v[kPionCotTheta] = nt_iter->pionHelix()->cotTheta();
      v[kPionZ0] = nt_iter->pionHelix()->z0();

      v[kPionMomentumX] = nt_iter->pionFit()->momentum().x();
      v[kPionMomentumY] = nt_iter->pionFit()->momentum().y();
      v[kPionMomentumZ] = nt_iter->pionFit()->momentum().z();

      v[kChiSquare] = nt_iter->pionQuality()->chiSquare();
      v[kDOF] = double(nt_iter->pionQuality()->degreesOfFreedom());
      v[kHitsExpected] = double(nt_iter->pionQuality()->numberHitsExpected());
      v[kHitRatio] = nt_iter->pionQuality()->ratioNumberHitsToExpected();
      v[kHitsInput] = double(nt_iter->pionQuality()->numberHitsInput());
      v[kFitAbort] = double(nt_iter->pionQuality()->fitAbort());
      v[kHitsDropped] = double(nt_iter->pionQuality()->numberHitsDropped());
      v[kIterations] = double(nt_iter->pionQuality()->numberIterations());

      int bestWire[6];
      double bestResid[6];
      for (int j(0);  j < 6;  j++) {
	 bestWire[j] = 0;
	 bestResid[j] = 1000.;
      }

      const PionFitZDHitLattice::VectorRightID* zdhitsonpion(
	 pionfitzdhitlattice->vRightGivenLeft(nt_iter->identifier()));
      PionFitZDHitLattice::RightItr zdpid_end(zdhitsonpion->end());
      for (PionFitZDHitLattice::RightItr zdpid_iter(zdhitsonpion->begin());
	   zdpid_iter != zdpid_end;  ++zdpid_iter) {
	 const CalibratedZDHit& hit = calibratedzdhit[*zdpid_iter];
	 const PionFitZDHitLattice::Links& zdslink(
	    pionfitzdhitlattice->linksGivenRight(zdpid_iter));
	 const TrackFitZDHitLink& link((*zdslink.begin())->linkData());
	 
	 double resid(fabs(link.signedDriftDistance() -
			   link.signedDcaToWire()));
	 if (resid < bestResid[hit.layer() - 1]) {
	    bestWire[hit.layer() - 1] = hit.wireInLyr();
	    bestResid[hit.layer() - 1] = resid;

	    switch (hit.layer()) {
	       case 1:
		  v[kL1Wire] = hit.wireInLyr();
		  v[kL1Charge] = hit.charge();
		  v[kL1Drift] = link.signedDriftDistance();
		  v[kL1DriftError] = link.signedDriftError();
		  v[kL1DCA] = link.signedDcaToWire();
		  v[kL1DCAError] = link.signedDcaError();
		  v[kL1SinAlpha] = link.sinTrackToRadial();
		  v[kL1Used] = link.used() ? 1. : 0.;
		  break;
	       case 2:
		  v[kL2Wire] = hit.wireInLyr();
		  v[kL2Charge] = hit.charge();
		  v[kL2Drift] = link.signedDriftDistance();
		  v[kL2DriftError] = link.signedDriftError();
		  v[kL2DCA] = link.signedDcaToWire();
		  v[kL2DCAError] = link.signedDcaError();
		  v[kL2SinAlpha] = link.sinTrackToRadial();
		  v[kL2Used] = link.used() ? 1. : 0.;
		  break;
	       case 3:
		  v[kL3Wire] = hit.wireInLyr();
		  v[kL3Charge] = hit.charge();
		  v[kL3Drift] = link.signedDriftDistance();
		  v[kL3DriftError] = link.signedDriftError();
		  v[kL3DCA] = link.signedDcaToWire();
		  v[kL3DCAError] = link.signedDcaError();
		  v[kL3SinAlpha] = link.sinTrackToRadial();
		  v[kL3Used] = link.used() ? 1. : 0.;
		  break;
	       case 4:
		  v[kL4Wire] = hit.wireInLyr();
		  v[kL4Charge] = hit.charge();
		  v[kL4Drift] = link.signedDriftDistance();
		  v[kL4DriftError] = link.signedDriftError();
		  v[kL4DCA] = link.signedDcaToWire();
		  v[kL4DCAError] = link.signedDcaError();
		  v[kL4SinAlpha] = link.sinTrackToRadial();
		  v[kL4Used] = link.used() ? 1. : 0.;
		  break;
	       case 5:
		  v[kL5Wire] = hit.wireInLyr();
		  v[kL5Charge] = hit.charge();
		  v[kL5Drift] = link.signedDriftDistance();
		  v[kL5DriftError] = link.signedDriftError();
		  v[kL5DCA] = link.signedDcaToWire();
		  v[kL5DCAError] = link.signedDcaError();
		  v[kL5SinAlpha] = link.sinTrackToRadial();
		  v[kL5Used] = link.used() ? 1. : 0.;
		  break;
	       case 6:
		  v[kL6Wire] = hit.wireInLyr();
		  v[kL6Charge] = hit.charge();
		  v[kL6Drift] = link.signedDriftDistance();
		  v[kL6DriftError] = link.signedDriftError();
		  v[kL6DCA] = link.signedDcaToWire();
		  v[kL6DCAError] = link.signedDcaError();
		  v[kL6SinAlpha] = link.sinTrackToRadial();
		  v[kL6Used] = link.used() ? 1. : 0.;
		  break;
	    } // end switch layer
	 } // end if we haven't seen a better hit
      } // end loop over pionfit zd hits

      const SeedTrackZDHitLattice::VectorRightID* zdhitsonseed(
	 seedtrackzdhitlattice->vRightGivenLeft(nt_iter->identifier()));
      SeedTrackZDHitLattice::RightItr zdsid_end(zdhitsonseed->end());
      for (SeedTrackZDHitLattice::RightItr zdsid_iter(zdhitsonseed->begin());
	   zdsid_iter != zdsid_end;  ++zdsid_iter) {
	 const CalibratedZDHit& hit = calibratedzdhit[*zdsid_iter];
	 const SeedTrackZDHitLattice::Links& zdslink(
	    seedtrackzdhitlattice->linksGivenRight(zdsid_iter));
	 const SeedZDHitLink& link((*zdslink.begin())->linkData());
	 
	 if (fabs(bestResid[hit.layer() - 1] - 1000.) < 1.  ||
	     bestWire[hit.layer() - 1] == hit.wireInLyr()     ) {
	    switch (hit.layer()) {
	       case 1:
		  v[kL1Used] += (link.fitFlag() == SeedDRHitLink::kFITTABLE ? 2. : 0.);
		  v[kL1SeedDrift] = link.signedDriftDistance();
		  break;
	       case 2:
		  v[kL2Used] += (link.fitFlag() == SeedDRHitLink::kFITTABLE ? 2. : 0.);
		  v[kL2SeedDrift] = link.signedDriftDistance();
		  break;
	       case 3:
		  v[kL3Used] += (link.fitFlag() == SeedDRHitLink::kFITTABLE ? 2. : 0.);
		  v[kL3SeedDrift] = link.signedDriftDistance();
		  break;
	       case 4:
		  v[kL4Used] += (link.fitFlag() == SeedDRHitLink::kFITTABLE ? 2. : 0.);
		  v[kL4SeedDrift] = link.signedDriftDistance();
		  break;
	       case 5:
		  v[kL5Used] += (link.fitFlag() == SeedDRHitLink::kFITTABLE ? 2. : 0.);
		  v[kL5SeedDrift] = link.signedDriftDistance();
		  break;
	       case 6:
		  v[kL6Used] += (link.fitFlag() == SeedDRHitLink::kFITTABLE ? 2. : 0.);
		  v[kL6SeedDrift] = link.signedDriftDistance();
		  break;
	    } // end switch layer
	 } // end if this is the best hit (from before)
      } // end loop over seedtrack zd hits

      if (nt_iter->mcTag().valid()) {
	 v[kMCMomentumX] = nt_iter->mcTag()->momentum().x();
	 v[kMCMomentumY] = nt_iter->mcTag()->momentum().y();
	 v[kMCMomentumZ] = nt_iter->mcTag()->momentum().z();
      } // end if track has an MC tag
   } // end loop over tracks

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
TestTracking::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
TestTracking::endRun( Frame& iFrame )         // anal4 equiv.
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
