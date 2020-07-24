// -*- C++ -*-
//
// Package:     ExtractFakes
// Module:      ExtractFakes
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 13:42:34 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "ExtractFakes/ExtractFakes.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "TrackRoot/TRSeedTrack.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "TrackFinder/SeedZDHitLink.h"
#include "TrackRoot/TRHelixPionFit.h"
#include "TrackRoot/TRTrackPionFitQuality.h"

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
static const char* const kFacilityString = "Processor.ExtractFakes" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
ExtractFakes::ExtractFakes( void )               // anal1
   : Processor( "ExtractFakes" )
   , m_explicitExtraction("explicitExtraction", this, true)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &ExtractFakes::event,    Stream::kEvent );
   //bind( &ExtractFakes::beginRun, Stream::kBeginRun );
   //bind( &ExtractFakes::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

ExtractFakes::~ExtractFakes()                    // anal5
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
ExtractFakes::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
ExtractFakes::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
ExtractFakes::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
ExtractFakes::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   if (m_explicitExtraction.value()) {

      FATable<TRSeedTrack> seedtrack;
//       extract(iFrame.record(Stream::kEvent), seedtrack, "fake-o");
      extract(iFrame.record(Stream::kEvent), seedtrack);

      FATable<CalibratedDRHit> drhit;
//      extract(iFrame.record(Stream::kEvent), drhit, "fake-o");
      extract(iFrame.record(Stream::kEvent), drhit);
      report(INFO, kFacilityString)
	 << "number of DRhits: " << drhit.size() << endl;
      for (FATable<CalibratedDRHit>::const_iterator drhit_iter = drhit.begin();
	   drhit_iter != drhit.end();
	   ++drhit_iter) {
	 report(INFO, kFacilityString)
	    << "  "
	    << drhit_iter->identifier() << ". "
	    << drhit_iter->wire() << " "
	    << drhit_iter->layer() << " "
	    << drhit_iter->wireInLyr() << " "
	    << drhit_iter->charge() << " "
	    << drhit_iter->time() << " "
	    << drhit_iter->distance() << " "
	    << drhit_iter->tSeries().distance() << ","
	    << drhit_iter->tSeries().velocity() << ","
	    << drhit_iter->tSeries().acceleration() << "," << " "
	    << drhit_iter->weight() << " "
	    << drhit_iter->qualHit() << " "
	    << drhit_iter->nTDC() << " "
	    << drhit_iter->earlyGap() << " "
	    << drhit_iter->lateGap() << " "
	    << drhit_iter->whichTDC() << endl;
      }

      if (seedtrack.size() > 0) {
	 FAItem<SeedTrackDRHitLattice> drlattice;
//	 extract(iFrame.record(Stream::kEvent), drlattice, "fake-o");
	 extract(iFrame.record(Stream::kEvent), drlattice);

	 if (drlattice.valid()) {
	    const SeedTrackDRHitLattice::Links linksOn1 = drlattice->linksGivenLeft(seedtrack.begin()->identifier());
	    report(INFO, kFacilityString)
	       << "number of dr links on first track: " << linksOn1.size() << endl;
	    for (SeedTrackDRHitLattice::LinkItr linksOn1_iter = linksOn1.begin();
		 linksOn1_iter != linksOn1.end();
		 ++linksOn1_iter) {
	       report(INFO, kFacilityString)
		  << "  " << (**linksOn1_iter).vRightID()[0] << ". "
		  << "signedDriftDistance " << (**linksOn1_iter).linkData().signedDriftDistance() << " "
		  << "pull " << (**linksOn1_iter).linkData().pull() << " "
		  << "fitFlag " << (**linksOn1_iter).linkData().fitFlag() << endl;
	    } // end loop over links
	 } // endif drlattice is valid
      } // endif more than one seedtrack

      FATable<CalibratedZDHit> zdhit;
//      extract(iFrame.record(Stream::kEvent), zdhit, "fake-o");
      extract(iFrame.record(Stream::kEvent), zdhit);
      report(INFO, kFacilityString)
	 << "number of ZDhits: " << zdhit.size() << endl;
      for (FATable<CalibratedZDHit>::const_iterator zdhit_iter = zdhit.begin();
	   zdhit_iter != zdhit.end();
	   ++zdhit_iter) {
	 report(INFO, kFacilityString)
	    << "  "
	    << zdhit_iter->identifier() << ". "
	    << zdhit_iter->wire() << " "
	    << zdhit_iter->layer() << " "
	    << zdhit_iter->wireInLyr() << " "
	    << zdhit_iter->charge() << " "
	    << zdhit_iter->time() << " "
	    << zdhit_iter->distance() << " "
	    << zdhit_iter->tSeries().distance() << ","
	    << zdhit_iter->tSeries().velocity() << ","
	    << zdhit_iter->tSeries().acceleration() << "," << " "
	    << zdhit_iter->weight() << " "
	    << zdhit_iter->qualHit() << " "
	    << zdhit_iter->nTDC() << " "
	    << zdhit_iter->earlyGap() << " "
	    << zdhit_iter->lateGap() << " "
	    << zdhit_iter->whichTDC() << endl;
      }

      if (seedtrack.size() > 0) {
	 FAItem<SeedTrackZDHitLattice> zdlattice;
//	 extract(iFrame.record(Stream::kEvent), zdlattice, "fake-o");
	 extract(iFrame.record(Stream::kEvent), zdlattice);

	 if (zdlattice.valid()) {
	    const SeedTrackZDHitLattice::Links linksOn1 = zdlattice->linksGivenLeft(seedtrack.begin()->identifier());
	    report(INFO, kFacilityString)
	       << "number of zd links on first track: " << linksOn1.size() << endl;
	    for (SeedTrackZDHitLattice::LinkItr linksOn1_iter = linksOn1.begin();
		 linksOn1_iter != linksOn1.end();
		 ++linksOn1_iter) {
	       report(INFO, kFacilityString)
		  << "  " << (**linksOn1_iter).vRightID()[0] << ". "
		  << "signedDriftDistance " << (**linksOn1_iter).linkData().signedDriftDistance() << " "
		  << "pull " << (**linksOn1_iter).linkData().pull() << " "
		  << "fitFlag " << (**linksOn1_iter).linkData().fitFlag() << endl;
	    } // end loop over links
	 } // endif zdlattice is valid
      } // endif more than one seedtrack

      report(INFO, kFacilityString)
	 << "number of seedtracks: " << seedtrack.size() << endl;
      for (FATable<TRSeedTrack>::const_iterator seed_iter = seedtrack.begin();
	   seed_iter != seedtrack.end();
	   ++seed_iter) {
	 report(INFO, kFacilityString)
	    << "  "
	    << seed_iter->identifier() << ". "
	    << seed_iter->curvature() << " "
	    << seed_iter->phi0() << " "
	    << seed_iter->d0() << " "
	    << seed_iter->cotTheta() << " "
	    << seed_iter->z0() << endl;
      }
   } // end if explicitExtraction

   FATable<TRHelixPionFit> fittrack;
   extract(iFrame.record(Stream::kEvent), fittrack);

   report(INFO, kFacilityString)
      << "number of fittracks: " << fittrack.size() << endl;
   for (FATable<TRHelixPionFit>::const_iterator fit_iter = fittrack.begin();
	fit_iter != fittrack.end();
	++fit_iter) {
      report(INFO, kFacilityString)
	 << "  "
	 << fit_iter->identifier() << ". "
	 << fit_iter->curvature() << " "
	 << fit_iter->phi0() << " "
	 << fit_iter->d0() << " "
	 << fit_iter->cotTheta() << " "
	 << fit_iter->z0() << endl;
   }

   FATable<TRTrackPionFitQuality> fitquality;
   extract(iFrame.record(Stream::kEvent), fitquality);
   report(INFO, kFacilityString)
      << "number of fitquality objects: " << fitquality.size() << endl;
   for (FATable<TRTrackPionFitQuality>::const_iterator qual_iter = fitquality.begin();
	qual_iter != fitquality.end();
	++qual_iter) {
      report(INFO, kFacilityString)
	 << "  "
	 << qual_iter->identifier() << ". "
	 << qual_iter->chiSquare() << " "
	 << qual_iter->degreesOfFreedom() << " "
	 << qual_iter->numberHitsExpected() << " "
	 << qual_iter->ratioNumberHitsToExpected() << " "
	 << qual_iter->numberHitsInput() << " "
	 << qual_iter->fit() << " "
	 << qual_iter->fitAbort() << " "
	 << qual_iter->numberHitsDropped() << " "
	 << qual_iter->numberIterations() << " "
	 << qual_iter->numberHitsVector().size() << " "
	 << qual_iter->averageResidualsVector().size() << " "
	 << qual_iter->rmsResidualsVector().size() << endl;
      report(INFO, kFacilityString)
	 << qual_iter->numberSubdetectorHits(TRSubdetectorLists::kZD) << " "
	 << qual_iter->numberSubdetectorHitLayers(TRSubdetectorLists::kZD) << " "
	 << qual_iter->averageResidual(TRSubdetectorLists::kZD) << " "
	 << qual_iter->rmsResidual(TRSubdetectorLists::kZD) << " | "
	 << qual_iter->isLayerHit(TRSubdetectorLists::kZD, 1) << " "
	 << qual_iter->isLayerHit(TRSubdetectorLists::kZD, 2) << " "
	 << qual_iter->isLayerHit(TRSubdetectorLists::kZD, 3) << " "
	 << qual_iter->isLayerHit(TRSubdetectorLists::kZD, 4) << " "
	 << qual_iter->isLayerHit(TRSubdetectorLists::kZD, 5) << " "
	 << qual_iter->isLayerHit(TRSubdetectorLists::kZD, 6) << endl;
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
ExtractFakes::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
ExtractFakes::endRun( Frame& iFrame )         // anal4 equiv.
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
