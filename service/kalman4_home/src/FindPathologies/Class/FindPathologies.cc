// -*- C++ -*-
//
// Package:     FindPathologies
// Module:      FindPathologies
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Apr 16 16:40:23 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "FindPathologies/FindPathologies.h"
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

#include "TrackFitter/TrackFitCathodeHitLink.h"

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
static const char* const kFacilityString = "Processor.FindPathologies" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
FindPathologies::FindPathologies( void )               // anal1
   : Processor( "FindPathologies" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &FindPathologies::event,    Stream::kEvent );
   //bind( &FindPathologies::beginRun, Stream::kBeginRun );
   //bind( &FindPathologies::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

FindPathologies::~FindPathologies()                    // anal5
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
FindPathologies::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
FindPathologies::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
FindPathologies::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const char* label[kNumEntries] = {
      "curv", "phi0", "d0", "cotTheta", "z0",
      "exitdot", "exitzref",
      "chits", "cresid", "cused"};
   m_ntuple = iHistoManager.ntuple(
      1, "vars", kNumEntries, 262144, label);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
FindPathologies::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   FATable<NavTrack>::const_iterator tracks_end = tracks.end();

   FAItem<ExitPionFitCathodeHitLattice> cathodeLattice;
   extract( iFrame.record(Stream::kEvent), cathodeLattice);

   for ( FATable<NavTrack>::const_iterator track_iter = tracks.begin();
	 track_iter != tracks_end;
	 ++track_iter ) {
      int i;
      float v[kNumEntries];  for ( i = 0;  i < kNumEntries;  i++ ) v[i] = 1000.;
      
      v[kCurv] = track_iter->pionHelix()->curvature();
      v[kPhi0] = track_iter->pionHelix()->phi0();
      v[kD0] = track_iter->pionHelix()->d0();
      v[kCotTheta] = track_iter->pionHelix()->cotTheta();
      v[kZ0] = track_iter->pionHelix()->z0();

      v[kExitDot] = ((track_iter->exitPionHelix()->referencePoint().x() *
		      cos(track_iter->exitPionHelix()->phi0())            ) +
		     (track_iter->exitPionHelix()->referencePoint().y() *
		      sin(track_iter->exitPionHelix()->phi0())            )   );

      v[kExitZRef] = track_iter->exitPionHelix()->referencePoint().z();


      v[kCathodeHits] = 0.;
      v[kCathodeResidual] = 0.;
      v[kCathodeUsed] = 0.;
      const PionFitCathodeHitLattice::VectorRightID* vectCathodeHitIDs =
	 cathodeLattice->vRightGivenLeft( track_iter->identifier() );
      if ( vectCathodeHitIDs != 0 ) {
	 v[kCathodeHits] = float(vectCathodeHitIDs->size());

	 if ( vectCathodeHitIDs->size() > 0 ) {
	    v[kCathodeResidual] = float(cathodeLattice->linkGivenLeft( track_iter->identifier() )->linkData().residual());
	    v[kCathodeUsed] = float(cathodeLattice->linkGivenLeft( track_iter->identifier() )->linkData().used());
	 }
      }

      m_ntuple->fill(v);
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
FindPathologies::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
FindPathologies::endRun( Frame& iFrame )         // anal4 equiv.
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
