// -*- C++ -*-
//
// Package:     SquishFilter
// Module:      SquishFilter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Feb  4 10:29:04 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "SquishFilter/SquishFilter.h"
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

#include "Navigation/NavShower.h"
#include "C3cc/CcShowerAttributes.h"

#include "BunchFinder/BunchDecision.h"

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
static const char* const kFacilityString = "Processor.SquishFilter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
SquishFilter::SquishFilter( void )               // anal1
   : Processor( "SquishFilter" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &SquishFilter::event,    Stream::kEvent );
   //bind( &SquishFilter::beginRun, Stream::kBeginRun );
   //bind( &SquishFilter::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

SquishFilter::~SquishFilter()                    // anal5
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
SquishFilter::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
SquishFilter::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
SquishFilter::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_chiSquare_first = iHistoManager.histogram(
      101, "chisquare track 1", 100, 0., 100.);
   m_chiSquare_second = iHistoManager.histogram(
      102, "chisquare track 2", 100, 0., 100.);

   m_d0_first = iHistoManager.histogram(
      201, "d0 track 1", 100, -0.30, 0.30);
   m_d0_second = iHistoManager.histogram(
      202, "d0 track 2", 100, -0.30, 0.30);

   m_z0Diff = iHistoManager.histogram(
      300, "z0 difference", 100, -0.05, 0.05);
   m_cotThetaSum = iHistoManager.histogram(
      350, "cotTheta sum", 100, -2., 2.);

   m_bunchTimeBefore = iHistoManager.histogram(
      400, "bunchtime", 100, 0., 1000.);
   m_bunchTimeAfter = iHistoManager.histogram(
      450, "bunchtime after other cuts", 100, 0., 1000.);

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
SquishFilter::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> track;
   extract(iFrame.record(Stream::kEvent), track);
   if ( track.size() != 2 ) return ActionBase::kFailed;

   FATable<NavShower> shower;
   extract(iFrame.record(Stream::kEvent), shower);
   int num_neutrals(0);
   FATable<NavShower>::const_iterator shower_end = shower.end();
   for ( FATable<NavShower>::const_iterator shower_iter = shower.begin();
	 shower_iter != shower_end;
	 ++shower_iter )
      if ( !shower_iter->attributes().hot()             &&
	   shower_iter->attributes().energy() > 0.030   &&
	   shower_iter->noTrackMatch()                  &&
	   shower_iter->noTrackMatchConReg()               )
	 num_neutrals++;
   if ( num_neutrals > 0 ) return ActionBase::kFailed;
   
   FATable<NavTrack>::const_iterator track_first = track.begin();
   FATable<NavTrack>::const_iterator track_second = track.begin();  ++track_second;

   if ( track_first->pionQuality()->degreesOfFreedom() > 0 )
      m_chiSquare_first->fill(track_first->pionQuality()->chiSquare() /
			      double(track_first->pionQuality()->degreesOfFreedom()));
   if ( track_second->pionQuality()->degreesOfFreedom() > 0 )
      m_chiSquare_second->fill(track_second->pionQuality()->chiSquare() /
			       double(track_second->pionQuality()->degreesOfFreedom()));

   m_d0_first->fill(track_first->pionHelix()->d0());
   m_d0_second->fill(track_second->pionHelix()->d0());

   m_z0Diff->fill(track_first->pionHelix()->z0() - track_second->pionHelix()->z0());
   m_cotThetaSum->fill(track_first->pionHelix()->cotTheta() +
		       track_second->pionHelix()->cotTheta());

   FAItem<BunchDecision> bunch;
   extract(iFrame.record(Stream::kEvent), bunch);
   m_bunchTimeBefore->fill(bunch->bestBunchTime());

   if ( track_first->pionQuality()->degreesOfFreedom() <= 0 ) return ActionBase::kFailed;
   if ( track_second->pionQuality()->degreesOfFreedom() <= 0 ) return ActionBase::kFailed;

   if ( track_first->pionQuality()->chiSquare() /
	track_first->pionQuality()->degreesOfFreedom() > 8. ) return ActionBase::kFailed;
   if ( track_second->pionQuality()->chiSquare() /
	track_second->pionQuality()->degreesOfFreedom() > 8. ) return ActionBase::kFailed;

   if ( track_first->pionHelix()->d0() > 0.05 ) return ActionBase::kFailed;
   if ( track_second->pionHelix()->d0() > 0.05 ) return ActionBase::kFailed;

   if ( track_first->pionHelix()->z0() - track_second->pionHelix()->z0() < 0.01 )
      return ActionBase::kFailed;

   if ( track_first->pionHelix()->cotTheta() - track_second->pionHelix()->cotTheta() < 0.5 )
      return ActionBase::kFailed;

   m_bunchTimeAfter->fill(bunch->bestBunchTime());

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
SquishFilter::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
SquishFilter::endRun( Frame& iFrame )         // anal4 equiv.
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
