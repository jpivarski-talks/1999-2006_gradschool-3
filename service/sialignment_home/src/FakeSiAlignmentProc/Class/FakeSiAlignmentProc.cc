// -*- C++ -*-
//
// Package:     FakeSiAlignmentProc
// Module:      FakeSiAlignmentProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri Feb 22 09:13:05 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "FakeSiAlignmentProc/FakeSiAlignmentProc.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "/home/mccann/sialignment/src/DualTrackHelices/DualTrackHelices/DualTrackHelices.h"

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
static const char* const kFacilityString = "Processor.FakeSiAlignmentProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.25 2002/01/25 20:42:06 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_03_01 $";

//
// static data member definitions
//


//
// constructors and destructor
//
FakeSiAlignmentProc::FakeSiAlignmentProc( void )               // anal1
   : Processor( "FakeSiAlignmentProc" )
   , m_tracks( 0 )
   , m_rphi( 0 )
   , m_z( 0 )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &FakeSiAlignmentProc::event,    Stream::kEvent );
   //bind( &FakeSiAlignmentProc::beginRun, Stream::kBeginRun );
   //bind( &FakeSiAlignmentProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

FakeSiAlignmentProc::~FakeSiAlignmentProc()                    // anal5
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
FakeSiAlignmentProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
FakeSiAlignmentProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
FakeSiAlignmentProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
FakeSiAlignmentProc::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   // Create a table of tracks and fill it.
   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ) , navtracks );
   FATable< NavTrack >::const_iterator nav_iter;

   report( INFO, kFacilityString )
      << "Got navtracks (num tracks = " << navtracks.size() << ")." << endl;

   // be sure that we have exactly two good tracks
   if ( navtracks.size() != 2 ) return ActionBase::kFailed;

   report( INFO, kFacilityString )
      << "( nav_iter = navtracks.begin() )->electronQuality()->fitAbort() = "
      << ( nav_iter = navtracks.begin() )->electronQuality()->fitAbort() << endl
      << "abs( nav_iter->electronHelix()->cotTheta() ) = "
      << abs( nav_iter->electronHelix()->cotTheta() ) << endl
      << "nav_iter->seedSVRHits()->size() = "
      << nav_iter->seedSVRHits()->size() << endl
      << "nav_iter->seedSVZHits()->size() = "
      << nav_iter->seedSVZHits()->size() << endl
      << "abs( nav_iter->electronHelix()->d0() ) = "
      << abs( nav_iter->electronHelix()->d0() ) << endl
      << "abs( nav_iter->electronHelix()->z0() ) = "
      << abs( nav_iter->electronHelix()->z0() ) << endl
      << "abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) = "
      << abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) << endl
      << endl
      << "( ++nav_iter )->electronQuality()->fitAbort() = "
      << ( ++nav_iter )->electronQuality()->fitAbort() << endl
      << "abs( nav_iter->electronHelix()->cotTheta() ) = "
      << abs( nav_iter->electronHelix()->cotTheta() ) << endl
      << "nav_iter->seedSVRHits()->size() = "
      << nav_iter->seedSVRHits()->size() << endl
      << "nav_iter->seedSVZHits()->size() = "
      << nav_iter->seedSVZHits()->size() << endl
      << "abs( nav_iter->electronHelix()->d0() ) = "
      << abs( nav_iter->electronHelix()->d0() ) << endl
      << "abs( nav_iter->electronHelix()->z0() ) = "
      << abs( nav_iter->electronHelix()->z0() ) << endl
      << "abs( nav_iter->electronFit()->momentum().mag() - 4.71671 = "
      << abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) << endl << endl;

   if ( ( nav_iter = navtracks.begin() )->electronQuality()->fitAbort() ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived fitAbort() check" << endl;
//   if ( nav_iter->seedQuality()->originUsed() ) return ActionBase::kFailed;
   if ( my_abs( nav_iter->electronHelix()->cotTheta() ) > 1.25 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived cotTheta check" << endl;
   if ( nav_iter->seedSVRHits()->size() < 1  &&  nav_iter->seedSVZHits()->size() < 1 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived seedHits check" << endl;
   if ( my_abs( nav_iter->electronHelix()->d0() ) > 0.0032 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived d0 check" << endl;
   if ( my_abs( nav_iter->electronHelix()->z0() ) > 0.04 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived z0 check" << endl;
   if ( my_abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) > 1.25 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived momentum check" << endl;

   report( INFO, kFacilityString ) << "First track is fine." << endl;

   if ( ( ++nav_iter )->electronQuality()->fitAbort() ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived fitAbort() check" << endl;
//   if ( nav_iter->seedQuality()->originUsed() ) return ActionBase::kFailed;
   if ( my_abs( nav_iter->electronHelix()->cotTheta() ) > 1.25 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived cotTheta check" << endl;
   if ( nav_iter->seedSVRHits()->size() < 1  &&  nav_iter->seedSVZHits()->size() < 1 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived seedHits check" << endl;
   if ( my_abs( nav_iter->electronHelix()->d0() ) > 0.0032 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived d0 check" << endl;
   if ( my_abs( nav_iter->electronHelix()->z0() ) > 0.04 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived z0 check" << endl;
   if ( my_abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) > 1.25 ) return ActionBase::kFailed;
   report( INFO, kFacilityString ) << "survived momentum check" << endl;
   
   report( INFO, kFacilityString ) << "Second track is fine." << endl;

   HepVector3D event_momentum = 
      ( ( nav_iter = navtracks.begin() )->electronFit()->momentum() +
	( ++nav_iter )->electronFit()->momentum() );
   double xdiff2 = sqr( event_momentum.x() + 0.0261 );
   double ydiff2 = sqr( event_momentum.y() + 0.0003 );
   if ( ( xdiff2 + ydiff2 ) > 0.300 ) return ActionBase::kFailed;

   if ( ( ( nav_iter = navtracks.begin() )->electronHelix()->curvature() ) *
	( ( ++nav_iter )->electronHelix()->curvature() ) > 0. ) return ActionBase::kFailed;

   report( INFO, kFacilityString ) << "Tracks are sufficiently back-to-back." << endl;

   for ( nav_iter = navtracks.begin();
	 nav_iter != navtracks.end();
	 nav_iter++ ) {
      
      const NavTrack::SVRHitTable* nav_rphi_hits = nav_iter->seedSVRHits();
      NavTrack::SVRHitTable::const_iterator nav_rphi_hits_iter;
      NavTrack::SVRHitTable::const_iterator nav_rphi_hits_begin
	 = nav_rphi_hits->begin();
      NavTrack::SVRHitTable::const_iterator nav_rphi_hits_end
	 = nav_rphi_hits->end();

      const NavTrack::SVZHitTable* nav_z_hits = nav_iter->seedSVZHits();
      NavTrack::SVZHitTable::const_iterator nav_z_hits_iter;
      NavTrack::SVZHitTable::const_iterator nav_z_hits_begin
	 = nav_z_hits->begin();
      NavTrack::SVZHitTable::const_iterator nav_z_hits_end
	 = nav_z_hits->end();

      int relevant_rphi( 0 );
      int relevant_z( 0 );

      for ( nav_rphi_hits_iter = nav_rphi_hits_begin;
	    nav_rphi_hits_iter != nav_rphi_hits_end;
	    nav_rphi_hits_iter++ )
	 if ( (* nav_rphi_hits_iter )->layer() == 4 )
	    relevant_rphi++;

      for ( nav_z_hits_iter = nav_z_hits_begin;
	    nav_z_hits_iter != nav_z_hits_end;
	    nav_z_hits_iter++ )
	 if ( (* nav_z_hits_iter )->layer() == 4 )
	    relevant_z++;

      m_tracks++;
      m_rphi += relevant_rphi;
      m_z += relevant_z;
	 
      report( NOTICE, kFacilityString )
	 << "Now have " << m_tracks << " with "
	 << m_rphi << " rphi hits and " << m_z << " z hits." << endl;


   } // end loop over tracks

   return ActionBase::kPassed;
}

double
FakeSiAlignmentProc::my_abs( double x )
{
   return ( x < 0. ? -x : x );
}

/*
ActionBase::ActionResult
FakeSiAlignmentProc::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
FakeSiAlignmentProc::endRun( Frame& iFrame )         // anal4 equiv.
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
