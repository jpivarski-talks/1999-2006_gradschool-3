// -*- C++ -*-
//
// Package:     CheckSimpleThing
// Module:      CheckSimpleThing
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 15:47:02 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "CheckSimpleThing/CheckSimpleThing.h"
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
static const char* const kFacilityString = "Processor.CheckSimpleThing" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
CheckSimpleThing::CheckSimpleThing( void )               // anal1
   : Processor( "CheckSimpleThing" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &CheckSimpleThing::event,    Stream::kEvent );
   //bind( &CheckSimpleThing::beginRun, Stream::kBeginRun );
   //bind( &CheckSimpleThing::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

CheckSimpleThing::~CheckSimpleThing()                    // anal5
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
CheckSimpleThing::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CheckSimpleThing::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
CheckSimpleThing::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   const char* label[kNumEntries] = {
      "curv", "cotTh", "chisq", "dof", "pperp", "cresid"};
   m_ntuple = iHistoManager.ntuple(
      1, "vars", kNumEntries, 262144, label);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
CheckSimpleThing::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks, "TrkmanApproved");
   FATable<NavTrack>::const_iterator track_iter = tracks.begin();
   FATable<NavTrack>::const_iterator track_end = tracks.end();

   FAItem<ExitPionFitCathodeHitLattice> cathlat;
   extract(iFrame.record(Stream::kEvent), cathlat);

   for (; track_iter != track_end;  ++track_iter) {
      DABoolean positive = (track_iter->pionHelix()->curvature() > 0.);
      double cotTheta = track_iter->pionHelix()->cotTheta();
      double chiSquare = track_iter->pionQuality()->chiSquare();
      double dof = double(track_iter->pionQuality()->degreesOfFreedom());
      double pperp = track_iter->pionFit()->momentum().perp();

      const ExitPionFitCathodeHitLattice::Links& cathlinks = cathlat->linksGivenLeft(track_iter->identifier());

      if (cathlinks.size() > 1) {
	 report(EMERGENCY, kFacilityString)
	    << "More than one cathode link per track?!?" << endl;
      }
      else if (cathlinks.size() == 1) {
	 float v[kNumEntries];
	 v[kCurvature] = track_iter->pionHelix()->curvature();
	 v[kCotTheta] = track_iter->pionHelix()->cotTheta();
	 v[kChiSquare] = track_iter->pionQuality()->chiSquare();
	 v[kDOF] = track_iter->pionQuality()->degreesOfFreedom();
	 v[kPPerp] = track_iter->pionFit()->momentum().perp();
	 v[kResidual] = (**(cathlinks.begin())).linkData().residual();

	 m_ntuple->fill(v);
      }

   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
CheckSimpleThing::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
CheckSimpleThing::endRun( Frame& iFrame )         // anal4 equiv.
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
