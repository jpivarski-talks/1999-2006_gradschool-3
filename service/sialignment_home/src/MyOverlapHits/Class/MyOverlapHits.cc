// -*- C++ -*-
//
// Package:     MyOverlapHits
// Module:      MyOverlapHits
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Sun Nov 25 16:04:06 EST 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "MyOverlapHits/MyOverlapHits.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"


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
static const char* const kFacilityString = "Processor.MyOverlapHits" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.24 2001/11/14 15:22:17 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_02_01 $";

//
// static data member definitions
//


//
// constructors and destructor
//
MyOverlapHits::MyOverlapHits( void )               // anal1
   : Processor( "MyOverlapHits" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &MyOverlapHits::event,    Stream::kEvent );
   //bind( &MyOverlapHits::beginRun, Stream::kBeginRun );
   //bind( &MyOverlapHits::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

MyOverlapHits::~MyOverlapHits()                    // anal5
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
MyOverlapHits::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
MyOverlapHits::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
MyOverlapHits::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const char* lables[ kNumVars ] = {
      "layer",
      "ladle", "senle", "ladgr", "sengr",
      "locZle", "locZgr",
      "resle", "resgr", "resdiff",
      "worldXle", "worldYle", "worldZle",
      "worldXgr", "worldYgr", "worldZgr" };

   m_z = iHistoManager.ntuple(
      2, "z", kNumVars, kLargeRecord, lables );

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
MyOverlapHits::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );
   unsigned int run = header->run();
//   unsigned int event = header->number();
   if ( m_run != run )
   {
      report( NOTICE, kFacilityString ) << "New run " << run << endl;
      m_run = run;
   }

   FAItem< ASiStore > sistore;
   extract( iFrame.record( Stream::kSVAlignment ), sistore );

   FATable< CalibratedSVZHit > calibrated_svz_hits;
   extract( iFrame.record( Stream::kEvent ), calibrated_svz_hits );

   FATable< TRHelixMuonFit > muonfit_tracks;
   extract( iFrame.record( Stream::kEvent ), muonfit_tracks );
   if ( muonfit_tracks.size() != 2 ) return ActionBase::kFailed;

   FATable< TRTrackMuonFitQuality > muonquality_tracks;
   extract( iFrame.record( Stream::kEvent ), muonquality_tracks );
   DABoolean both_succeeded = true;
   for ( FATable< TRTrackMuonFitQuality >::const_iterator muonquality_iter
            = muonquality_tracks.begin();
         muonquality_iter != muonquality_tracks.end();
         muonquality_iter++ )
      if ( muonquality_iter->fitAbort() ) both_succeeded = false;
   if ( ! both_succeeded ) return ActionBase::kFailed;

   FAItem< MuonFitSVZHitLattice > muonfit_svz_lattice;
   extract( iFrame.record( Stream::kEvent ), muonfit_svz_lattice );

   for ( FATable< TRHelixMuonFit >::const_iterator muonfit_iter
            = muonfit_tracks.begin();
         muonfit_iter != muonfit_tracks.end();
         muonfit_iter++ )
   {
      const MuonFitSVZHitLattice::Links svz_links
         = muonfit_svz_lattice->linksGivenLeft( muonfit_iter->identifier() );

      unsigned int ladder, ladlesser, i;
      MuonFitSVZHitLattice::Links::const_iterator hit_on_ladder[61];
      for ( ladder = 1;  ladder <= 61;  ladder++ )
	 hit_on_ladder[ladder-1] = svz_links.end();

      vector< MuonFitSVZHitLattice::Links::const_iterator > overlap_lesser;
      vector< MuonFitSVZHitLattice::Links::const_iterator > overlap_greater;

      for ( MuonFitSVZHitLattice::Links::const_iterator svz_link_iter
               = svz_links.begin();
            svz_link_iter != svz_links.end();
            svz_link_iter++ )
      {
         const CalibratedSVZHit& svz_hit
            = calibrated_svz_hits[ (* svz_link_iter)->vRightID()[ 0 ] ];

	 hit_on_ladder[ sistore->ladderForSensor( svz_hit.sensor() ) - 1 ]
	    = svz_link_iter;
      } // end loop over z hits (searching for overlaps)
	 
      // layer 1
      for ( ladlesser = 1;  ladlesser <= 7;  ladlesser++ )
      {
	 unsigned int ladgreater = ladlesser + 1;
	 if ( ladgreater == 8 ) ladgreater = 1;

	 if ( hit_on_ladder[ladlesser-1] != svz_links.end()  &&
	      hit_on_ladder[ladgreater-1] != svz_links.end()    )
	 {
	    overlap_lesser.push_back( hit_on_ladder[ladlesser-1] );
	    overlap_greater.push_back( hit_on_ladder[ladgreater-1] );
	 } // end if found overlap pair
      } // end loop over layer 1 ladders

      // layer 2
      for ( ladlesser = 8;  ladlesser <= 17;  ladlesser++ )
      {
	 unsigned int ladgreater = ladlesser + 1;
	 if ( ladgreater == 18 ) ladgreater = 8;

	 if ( hit_on_ladder[ladlesser-1] != svz_links.end()  &&
	      hit_on_ladder[ladgreater-1] != svz_links.end()    )
	 {
	    overlap_lesser.push_back( hit_on_ladder[ladlesser-1] );
	    overlap_greater.push_back( hit_on_ladder[ladgreater-1] );
	 } // end if found overlap pair
      } // end loop over layer 2 ladders

      // layer 3
      for ( ladlesser = 18;  ladlesser <= 35;  ladlesser++ )
      {
	 unsigned int ladgreater = ladlesser + 1;
	 if ( ladgreater == 36 ) ladgreater = 18;

	 if ( hit_on_ladder[ladlesser-1] != svz_links.end()  &&
	      hit_on_ladder[ladgreater-1] != svz_links.end()    )
	 {
	    overlap_lesser.push_back( hit_on_ladder[ladlesser-1] );
	    overlap_greater.push_back( hit_on_ladder[ladgreater-1] );
	 } // end if found overlap pair
      } // end loop over layer 3 ladders

      // layer 4
      for ( ladlesser = 36;  ladlesser <= 61;  ladlesser++ )
      {
	 unsigned int ladgreater = ladlesser + 1;
	 if ( ladgreater == 62 ) ladgreater = 36;

	 if ( hit_on_ladder[ladlesser-1] != svz_links.end()  &&
	      hit_on_ladder[ladgreater-1] != svz_links.end()    )
	 {
	    overlap_lesser.push_back( hit_on_ladder[ladlesser-1] );
	    overlap_greater.push_back( hit_on_ladder[ladgreater-1] );
	 } // end if found overlap pair
      } // end loop over layer 4 ladders

      for ( i = 0;  i < overlap_lesser.size();  i++ )
      {
	 const CalibratedSVZHit& lesser_hit
            = calibrated_svz_hits[ (* overlap_lesser[i])->vRightID()[ 0 ] ];
	 const CalibratedSVZHit& greater_hit
            = calibrated_svz_hits[ (* overlap_greater[i])->vRightID()[ 0 ] ];

	 float vars[ kNumVars ];
	 for ( unsigned int j = 0;  j <= kNumVars;  j++ ) vars[j] = 0.;

	 vars[ kLayer ] = lesser_hit.layer();
	 vars[ kLadLe ] = sistore->ladderForSensor( lesser_hit.sensor() );
	 vars[ kSenLe ] = lesser_hit.sensor();
	 vars[ kLadGr ] = sistore->ladderForSensor( greater_hit.sensor() );
	 vars[ kSenGr ] = greater_hit.sensor();
	 vars[ kLocZLe ] = lesser_hit.localcoord().z();
	 vars[ kLocZGr ] = greater_hit.localcoord().z();
	 vars[ kResLe ] = (* overlap_lesser[i])->linkData().residual();
	 vars[ kResGr ] = (* overlap_greater[i])->linkData().residual();
	 vars[ kResDiff ] = ( (* overlap_lesser[i])->linkData().residual() -
			      (* overlap_greater[i])->linkData().residual()  );
	 vars[ kWorldXLe ] = lesser_hit.worldcoord( *sistore ).x();
	 vars[ kWorldYLe ] = lesser_hit.worldcoord( *sistore ).y();
	 vars[ kWorldZLe ] = lesser_hit.worldcoord( *sistore ).z();
	 vars[ kWorldXGr ] = greater_hit.worldcoord( *sistore ).x();
	 vars[ kWorldYGr ] = greater_hit.worldcoord( *sistore ).y();
	 vars[ kWorldZGr ] = greater_hit.worldcoord( *sistore ).z();

	 m_z->fill( vars );
      } // end loop over all found overlaps

   } // end loop over tracks

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
MyOverlapHits::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
MyOverlapHits::endRun( Frame& iFrame )         // anal4 equiv.
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
