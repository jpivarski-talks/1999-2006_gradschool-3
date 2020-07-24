// -*- C++ -*-
//
// Package:     CakeDiag
// Module:      CakeDiag
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Thu May  2 10:42:29 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "CakeDiag/CakeDiag.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
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
static const char* const kFacilityString = "Processor.CakeDiag" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.30 2002/04/02 22:04:09 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_05_01 $";

//
// static data member definitions
//



//
// constructors and destructor
//
CakeDiag::CakeDiag( void )               // anal1
   : Processor( "CakeDiag" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &CakeDiag::event,    Stream::kEvent );
   //bind( &CakeDiag::beginRun, Stream::kBeginRun );
   //bind( &CakeDiag::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

CakeDiag::~CakeDiag()                    // anal5
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
CakeDiag::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CakeDiag::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
CakeDiag::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const char* labels[10] = {
      "px", "py", "pz", "pmag", "pcor",
      "phi0", "curv", "d0", "z0", "cotTh"
   };

   m_ntuple = iHistoManager.ntuple(
      1, "tracks", 10, 262144, labels );

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
CakeDiag::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator nav_iter;
   FATable< NavTrack >::const_iterator nav_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator nav_end = navtracks.end();
   
   // be sure that we have exactly two good tracks
   if ( navtracks.size() != 2 ) return ActionBase::kPassed; // ActionBase::kFailed;
   if ( ( nav_iter = nav_begin )->muonQuality()->fitAbort() )
      return ActionBase::kPassed; // ActionBase::kFailed;
//     if ( nav_iter->seedSVRHits()->size() < 1 ) return ActionBase::kPassed; // ActionBase::kFailed;
   if ( ( ++nav_iter )->muonQuality()->fitAbort() )
      return ActionBase::kPassed; // ActionBase::kFailed;
//     if ( nav_iter->seedSVRHits()->size() < 1 ) return ActionBase::kPassed; // ActionBase::kFailed;
   
   // with no final-state radiation (well, very little... this is
   // one of those infrared divergences, isn't it?
   HepVector3D event_momentum =
      ( ( nav_iter = nav_begin )->muonFit()->momentum() +
	( ++nav_iter )->muonFit()->momentum() );
   double xdiff2 = sqr( event_momentum.x() + 0.0261 );
   double ydiff2 = sqr( event_momentum.y() + 0.0003 );
   if ( ( xdiff2 + ydiff2 ) > 0.300 ) return ActionBase::kPassed; // ActionBase::kFailed;
//     if ( abs( ( nav_iter = nav_begin )->muonHelix()->curvature() +
//  	     ( ++nav_iter )->muonHelix()->curvature()             )
//  	> 4E-3 ) return ActionBase::kPassed; // ActionBase::kFailed;
   
   // This is getting a little ridiculous...
   if ( ( ( nav_iter = nav_begin )->muonHelix()->curvature() ) *
	( ( ++ nav_iter )->muonHelix()->curvature() ) > 0. )
      return ActionBase::kPassed; // ActionBase::kFailed;

   FATable< DualTrackHelices > dualtracks;
   extract( iFrame.record( Stream::kEvent ), dualtracks );
   FATable< DualTrackHelices >::const_iterator dualtracks_iter;

   for ( nav_iter = nav_begin;  nav_iter != nav_end;  nav_iter++ )
   {
      dualtracks_iter = dualtracks.find( nav_iter->identifier() );
      double cotTheta = dualtracks_iter->helix().cotTheta();
      HIHelix helix( dualtracks_iter->helix() );

      Hep3Vector p( nav_iter->pionFit()->momentum() );

      float ntuple[10] = {
	 p.x(), p.y(), p.z(), p.mag(), ( p + HepVector3D( 0.013, 0., 0. ) ).mag(),
	 helix.phi0(), helix.curvature(), helix.d0(), helix.z0(), helix.cotTheta() };

      m_ntuple->fill( ntuple );

   } // end loop over tracks

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
CakeDiag::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
CakeDiag::endRun( Frame& iFrame )         // anal4 equiv.
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
