// -*- C++ -*-
//
// Package:     ComparePionKaon
// Module:      ComparePionKaon
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Dec 18 12:44:23 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "ComparePionKaon/ComparePionKaon.h"
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
#include "EventProperties/EventProperties.h"

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
static const char* const kFacilityString = "Processor.ComparePionKaon" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_01 $";

//
// static data member definitions
//



//
// constructors and destructor
//
ComparePionKaon::ComparePionKaon( void )               // anal1
   : Processor( "ComparePionKaon" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &ComparePionKaon::event,    Stream::kEvent );
   //bind( &ComparePionKaon::beginRun, Stream::kBeginRun );
   //bind( &ComparePionKaon::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

ComparePionKaon::~ComparePionKaon()                    // anal5
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
ComparePionKaon::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
ComparePionKaon::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
ComparePionKaon::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_pion_kaon = iHistoManager.histogram(
      100, "pion err trace / kaon err trace", 1000, 0., 1e6);
   m_kaon_pion = iHistoManager.histogram(
      200, "kaon err trace / pion err trace", 1000, 0., 1e6);
   m_exitPion_kaon = iHistoManager.histogram(
      300, "exitPion err trace / kaon err trace", 1000, 0., 1e6);
   m_exitPion_pion = iHistoManager.histogram(
      400, "exitPion err trace / pion err trace", 1000, 0., 1e6);
   m_exitKaon_kaon = iHistoManager.histogram(
      500, "exitKaon err trace / kaon err trace", 1000, 0., 1e6);
   m_exitKaon_pion = iHistoManager.histogram(
      600, "exitKaon err trace / pion err trace", 1000, 0., 1e6);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
ComparePionKaon::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable< NavTrack > tracks;
   extract( iFrame.record( Stream::kEvent ), tracks );
   FATable< NavTrack >::const_iterator track_iter;
   FATable< NavTrack >::const_iterator tracks_begin = tracks.begin();
   FATable< NavTrack >::const_iterator tracks_end = tracks.end();

   for ( track_iter = tracks_begin;  track_iter != tracks_end;  track_iter++ ) {
      int i(0), j(0);
      
      double pionErr(0.);
      double kaonErr(0.);
      double exitPionErr(0.);
      double exitKaonErr(0.);

      for ( i = 1;  i <= 5;  i++ )
	 pionErr += abs(track_iter->pionHelix()->errorMatrix()(i,i));

      for ( i = 1;  i <= 5;  i++ )
	 kaonErr += abs(track_iter->kaonHelix()->errorMatrix()(i,i));

      for ( i = 1;  i <= 5;  i++ )
	 exitPionErr += abs(track_iter->exitPionHelix()->errorMatrix()(i,i));

      for ( i = 1;  i <= 5;  i++ )
	 exitKaonErr += abs(track_iter->exitKaonHelix()->errorMatrix()(i,i));

      m_pion_kaon->fill(pionErr/kaonErr);
      m_kaon_pion->fill(kaonErr/pionErr);
      m_exitPion_kaon->fill(exitPionErr/kaonErr);
      m_exitPion_pion->fill(exitPionErr/pionErr);
      m_exitKaon_kaon->fill(exitKaonErr/kaonErr);
      m_exitKaon_pion->fill(exitKaonErr/pionErr);

//        report(INFO, kFacilityString)
//  	 << "exitPionErr/pionErr = " << exitPionErr/pionErr
//  	 << " exitPionErr/kaonErr = " << exitPionErr/kaonErr << endl;

//        report(INFO, kFacilityString)
//  	 << " phi0 = " << track_iter->seedTrack()->phi0()
//  	 << " cotTheta = " << track_iter->seedTrack()->cotTheta() << endl
//  	 << " pionErr = " << pionErr << " kaonErr = " << kaonErr
//  	 << " exitPionErr = " << exitPionErr << endl;
//        report(INFO, kFacilityString)
//  	 << " pionErr = " << (track_iter->pionHelix()->hasNullErrorMatrix() ? "NULL" : "okay")
//  	 << " kaonErr = " << (track_iter->kaonHelix()->hasNullErrorMatrix() ? "NULL" : "okay")
//  	 << " exitPionErr = " << (track_iter->exitPionHelix()->hasNullErrorMatrix() ? "NULL" : "okay")
//  	 << endl;

//        report(INFO, kFacilityString)
//  	 << track_iter->pionHelix()->errorMatrix() << endl;

//        report(INFO, kFacilityString)
//  	 << track_iter->kaonHelix()->errorMatrix() << endl;

//        report(INFO, kFacilityString)
//  	 << track_iter->exitPionHelix()->errorMatrix() << endl;

   } // end loop over tracks

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
ComparePionKaon::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
ComparePionKaon::endRun( Frame& iFrame )         // anal4 equiv.
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
