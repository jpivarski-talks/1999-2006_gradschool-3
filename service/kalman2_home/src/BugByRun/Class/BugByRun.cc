// -*- C++ -*-
//
// Package:     BugByRun
// Module:      BugByRun
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Jan 27 11:50:31 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "BugByRun/BugByRun.h"
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

//RICH example 
//Dedx example
//Event Shape example


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
static const char* const kFacilityString = "Processor.BugByRun" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_01 $";

//
// static data member definitions
//



//
// constructors and destructor
//
BugByRun::BugByRun( void )               // anal1
   : Processor( "BugByRun" )
   , run(0)
   , buggies(0)
   , tracks(0)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &BugByRun::event,    Stream::kEvent );
   //bind( &BugByRun::beginRun, Stream::kBeginRun );
   //bind( &BugByRun::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

BugByRun::~BugByRun()                    // anal5
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
BugByRun::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
BugByRun::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
   double buggie_fraction( double(buggies) / double(tracks) );
   DABoolean buggy( buggie_fraction > 0.05 );
   report( SYSTEM, kFacilityString )
      << "RUN " << run << " is " << ( buggy ? "buggy" : "unbugged" ) << endl
      << "RUN " << run << " details: " << buggies << " / " << tracks
      << " = " << buggie_fraction << " (threshold is 5%)" << endl;
}


// ---------------- standard place to book histograms ---------------
void
BugByRun::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
BugByRun::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );
   
   FATable< NavTrack > trackTable;
   extract( iFrame.record( Stream::kEvent ) , trackTable );

   if ( header.valid()  &&  trackTable.valid() ) {

      if ( header->run() != run ) {
	 if ( run != 0 ) {
	    double buggie_fraction( double(buggies) / double(tracks) );
	    DABoolean buggy( buggie_fraction > 0.05 );
	    report( SYSTEM, kFacilityString )
	       << "RUN " << run << " is " << ( buggy ? "buggy" : "unbugged" ) << endl
	       << "RUN " << run << " details: " << buggies << " / " << tracks
	       << " = " << buggie_fraction << " (threshold is 5%)" << endl;
	 }
	 run = header->run();
      }

      FATable< NavTrack >::const_iterator trackTableBegin = trackTable.begin();
      FATable< NavTrack >::const_iterator trackTableEnd = trackTable.end();
      // Loop over tracks.
      for ( FATable< NavTrack >::const_iterator trackItr = trackTableBegin;
	    trackItr != trackTableEnd ;
	    ++trackItr ) {
	 if ( trackItr->exitPionQuality().valid() ) {
	    if ( trackItr->exitPionQuality()->numberHits() < 3 ) buggies++;
	    tracks++;
	 }
      }

   }

   report( DEBUG, kFacilityString ) << "leaving event()" << endl;
   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
BugByRun::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
BugByRun::endRun( Frame& iFrame )         // anal4 equiv.
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
