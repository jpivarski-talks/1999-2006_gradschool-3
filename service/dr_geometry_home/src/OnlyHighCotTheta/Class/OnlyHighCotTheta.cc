// -*- C++ -*-
//
// Package:     OnlyHighCotTheta
// Module:      OnlyHighCotTheta
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Sep  9 11:54:08 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "OnlyHighCotTheta/OnlyHighCotTheta.h"
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
static const char* const kFacilityString = "Processor.OnlyHighCotTheta" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
OnlyHighCotTheta::OnlyHighCotTheta( void )               // anal1
   : Processor( "OnlyHighCotTheta" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &OnlyHighCotTheta::event,    Stream::kEvent );
   //bind( &OnlyHighCotTheta::beginRun, Stream::kBeginRun );
   //bind( &OnlyHighCotTheta::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

OnlyHighCotTheta::~OnlyHighCotTheta()                    // anal5
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
OnlyHighCotTheta::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
OnlyHighCotTheta::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
OnlyHighCotTheta::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
OnlyHighCotTheta::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);

   if ( tracks.size() != 2 ) return ActionBase::kFailed;

   FATable<NavTrack>::const_iterator first = tracks.begin();
   FATable<NavTrack>::const_iterator second = first;  ++second;

   if ( fabs(first->seedTrack()->cotTheta()) < 1.8 ) return ActionBase::kFailed;
   if ( fabs(second->seedTrack()->cotTheta()) < 1.8 ) return ActionBase::kFailed;

   if ( !first->pionQuality()->fit()  ||  first->pionQuality()->fitAbort() ) return ActionBase::kFailed;
   if ( !second->pionQuality()->fit()  ||  second->pionQuality()->fitAbort() ) return ActionBase::kFailed;

   if ( first->pionFit()->momentum().mag() < 4. ) return ActionBase::kFailed;
   if ( second->pionFit()->momentum().mag() < 4. ) return ActionBase::kFailed;

   if ( fabs(first->pionHelix()->d0()) > 0.01 ) return ActionBase::kFailed;
   if ( fabs(second->pionHelix()->d0()) > 0.01 ) return ActionBase::kFailed;

   if ( fabs(first->pionHelix()->z0()) > 0.10 ) return ActionBase::kFailed;
   if ( fabs(second->pionHelix()->z0()) > 0.10 ) return ActionBase::kFailed;

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
OnlyHighCotTheta::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
OnlyHighCotTheta::endRun( Frame& iFrame )         // anal4 equiv.
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
