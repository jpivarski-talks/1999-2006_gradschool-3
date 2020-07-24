// -*- C++ -*-
//
// Package:     AskForFittingWeights
// Module:      AskForFittingWeights
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Oct  1 14:36:31 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "AskForFittingWeights/AskForFittingWeights.h"
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

#include "ADRGeom/ADRSenseWireStore.h"

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
static const char* const kFacilityString = "Processor.AskForFittingWeights" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
AskForFittingWeights::AskForFittingWeights( void )               // anal1
   : Processor( "AskForFittingWeights" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &AskForFittingWeights::event,    Stream::kEvent );
   //bind( &AskForFittingWeights::beginRun, Stream::kBeginRun );
   //bind( &AskForFittingWeights::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

AskForFittingWeights::~AskForFittingWeights()                    // anal5
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
AskForFittingWeights::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
AskForFittingWeights::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
AskForFittingWeights::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here
   const char* label[kNumEntries] = {
      "layer", "drift", "sinalpha", "cosTheta", "fitweight"};
   m_ntuple = iHistoManager.ntuple(
      1, "vars", kNumEntries, 262144, label);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
AskForFittingWeights::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<ADRSenseWireStore> store;
   extract(iFrame.record(Stream::kBaseGeometry), store);

   FATable<CalibratedDRHit> drhits;
   extract(iFrame.record(Stream::kEvent), drhits);

   // Create a table of tracks and fill it.
   FATable< NavTrack > trackTable;
   extract( iFrame.record( Stream::kEvent ) , trackTable );

   FATable< NavTrack >::const_iterator trackTableBegin = trackTable.begin();
   FATable< NavTrack >::const_iterator trackTableEnd = trackTable.end();
   // Loop over tracks.
   for ( FATable< NavTrack >::const_iterator trackItr = trackTableBegin;
         trackItr != trackTableEnd ;
         ++trackItr )
   {
      double cosTheta =
	 sqrt(1. - 1./(1. + sqr(trackItr->pionHelix()->cotTheta())))
	 * (trackItr->pionHelix()->cotTheta() > 0. ? 1. : -1.);

      const NavTrack::DRHitLinkTable* links = trackItr->pionDRHitLinks();
      NavTrack::DRHitLinkTable::const_iterator linkItr = links->begin();
      NavTrack::DRHitLinkTable::const_iterator linksEnd = links->end();
      for( ; linkItr != linksEnd;  ++linkItr) {
	 FATable<CalibratedDRHit>::const_iterator hit = drhits.find(*(*linkItr)->rightID());
	 int layer = hit->layer();

	 TrackFitDRHitLink ld = (**linkItr).linkData();

	 const float layerCells = float(store->numberOfWiresLyr(layer));
	 const float layerRadius = store->radiusNominal(layer);
	 float absDrift(fabs(ld.signedDriftDistance()) * layerCells / (M_PI * layerRadius));

	 float v[kNumEntries];
	 v[kLayer] = float(layer);
	 v[kDrift] = absDrift;
	 v[kSinAlpha] = ld.sinTrackToRadial();
	 v[kCosTheta] = cosTheta;
	 v[kFitWeight] = ld.signedDriftError();
	 m_ntuple->fill(v);
      }
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
AskForFittingWeights::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
AskForFittingWeights::endRun( Frame& iFrame )         // anal4 equiv.
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
