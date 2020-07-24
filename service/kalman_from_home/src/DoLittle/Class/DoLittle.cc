// -*- C++ -*-
//
// Package:     DoLittle
// Module:      DoLittle
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Oct 14 17:46:26 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DoLittle/DoLittle.h"
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
static const char* const kFacilityString = "Processor.DoLittle" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
DoLittle::DoLittle( void )               // anal1
   : Processor( "DoLittle" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &DoLittle::event,    Stream::kEvent );
   //bind( &DoLittle::beginRun, Stream::kBeginRun );
   //bind( &DoLittle::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

DoLittle::~DoLittle()                    // anal5
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
DoLittle::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DoLittle::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
DoLittle::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
DoLittle::event( Frame& iFrame )          // anal3 equiv.
{
   report( SYSTEM, kFacilityString ) << "NEW EVENT!!!" << endl;

   // Create a table of tracks and fill it.
   FATable< NavTrack > trackTable;
   extract( iFrame.record( Stream::kEvent ) , trackTable );

   report( SYSTEM, kFacilityString ) << "(it has " << trackTable.size() << " tracks)" << endl;

   for ( FATable< NavTrack >::const_iterator iter = trackTable.begin();
	 iter != trackTable.end();
	 iter++ ) {
      report( SYSTEM, kFacilityString )
	 << "    track params: " << iter->seedTrack()->curvature() << " -> "
	 << iter->pionHelix()->curvature() << endl
	 << "                  " << iter->seedTrack()->phi0() << " -> "
	 << iter->pionHelix()->phi0() << endl
	 << "                  " << iter->seedTrack()->d0() << " -> "
	 << iter->pionHelix()->d0() << endl
	 << "                  " << iter->seedTrack()->cotTheta() << " -> "
	 << iter->pionHelix()->cotTheta() << endl
	 << "                  " << iter->seedTrack()->z0() << " -> "
	 << iter->pionHelix()->z0() << endl << endl;
   }

   report( SYSTEM, kFacilityString ) << "end event!!!" << endl;

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
DoLittle::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
DoLittle::endRun( Frame& iFrame )         // anal4 equiv.
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
