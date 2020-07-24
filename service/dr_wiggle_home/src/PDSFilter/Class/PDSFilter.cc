// -*- C++ -*-
//
// Package:     PDSFilter
// Module:      PDSFilter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Nov 20 13:35:09 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "PDSFilter/PDSFilter.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "TrackRoot/TRTrack.h"
#include "TrackRoot/TRHelixPionFit.h"
#include "TrackDelivery/TDKinematicPionFit.h"
#include "TrackRoot/TRTrackPionFitQuality.h"
#include "TrackFitter/TrackFitDRHitLink.h"
#include "TrackFitter/TrackFitSVRHitLink.h"
#include "TrackFitter/TrackFitSVZHitLink.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
#include "DualTrackHelices/DualTrackHelices.h"

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
static const char* const kFacilityString = "Processor.PDSFilter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
PDSFilter::PDSFilter( void )               // anal1
   : Processor( "PDSFilter" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &PDSFilter::event,    Stream::kEvent );
   //bind( &PDSFilter::beginRun, Stream::kBeginRun );
   //bind( &PDSFilter::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

PDSFilter::~PDSFilter()                    // anal5
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
PDSFilter::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
PDSFilter::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
PDSFilter::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
PDSFilter::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<TRTrack> fa_TRTrack;
   FATable<TRHelixPionFit> fa_TRHelixPionFit;
   FATable<TDKinematicPionFit> fa_TDKinematicPionFit;
   FATable<TRTrackPionFitQuality> fa_TRTrackPionFitQuality;

   extract(iFrame.record(Stream::kEvent), fa_TRTrack);
   extract(iFrame.record(Stream::kEvent), fa_TRHelixPionFit);
   extract(iFrame.record(Stream::kEvent), fa_TDKinematicPionFit);
   extract(iFrame.record(Stream::kEvent), fa_TRTrackPionFitQuality);

   if ( fa_TRTrack.size() != 2 ) return ActionBase::kFailed;
   if ( fa_TDKinematicPionFit.begin()->momentum().mag() < 4.       ||
	( ++fa_TDKinematicPionFit.begin() )->momentum().mag() < 4.    )
      return ActionBase::kFailed;
   if ( fa_TRTrackPionFitQuality.begin()->fitAbort()       ||
	( ++fa_TRTrackPionFitQuality.begin() )->fitAbort()    )
      return ActionBase::kFailed;

   FAItem<PionFitDRHitLattice> fa_PionFitDRHitLattice;
   FAItem<PionFitSVRHitLattice> fa_PionFitSVRHitLattice;
   FAItem<PionFitSVZHitLattice> fa_PionFitSVZHitLattice;
   FATable<CalibratedSVRphiHit> fa_CalibratedSVRphiHit;
   FATable<CalibratedSVZHit> fa_CalibratedSVZHit;
   FATable<DualTrackHelices> fa_DualTrackHelices;

   extract(iFrame.record(Stream::kEvent), fa_PionFitDRHitLattice);
   extract(iFrame.record(Stream::kEvent), fa_PionFitSVRHitLattice);
   extract(iFrame.record(Stream::kEvent), fa_PionFitSVZHitLattice);
   extract(iFrame.record(Stream::kEvent), fa_CalibratedSVRphiHit);
   extract(iFrame.record(Stream::kEvent), fa_CalibratedSVZHit);
   extract(iFrame.record(Stream::kEvent), fa_DualTrackHelices);

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
PDSFilter::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
PDSFilter::endRun( Frame& iFrame )         // anal4 equiv.
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
