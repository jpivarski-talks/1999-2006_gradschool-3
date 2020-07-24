// -*- C++ -*-
//
// Package:     FindFitAborts
// Module:      FindFitAborts
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Tue Oct 16 15:56:24 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "FindFitAborts/FindFitAborts.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"

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
static const char* const kFacilityString = "Processor.FindFitAborts" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.21 2001/09/01 22:44:37 llh14 Exp $";
static const char* const kTagString = "$Name: v06_01_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
FindFitAborts::FindFitAborts( void )               // anal1
   : Processor( "FindFitAborts" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &FindFitAborts::event,    Stream::kEvent );
   //bind( &FindFitAborts::beginRun, Stream::kBeginRun );
   //bind( &FindFitAborts::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

FindFitAborts::~FindFitAborts()                    // anal5
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
FindFitAborts::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
FindFitAborts::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
FindFitAborts::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
FindFitAborts::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator navtracks_iterator;
   FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();
   DBCandidate::Hypo particle = DBCandidate::kElectron;
//   DBCandidate::Hypo particle = DBCandidate::kMuon;

   vector< TRTrack::Identifier > identifiers;
   vector< DABoolean > fitaborts;
   DABoolean any_fitaborts = false;
   Count numtracks = 0;

   for( navtracks_iterator = navtracks_begin;
        navtracks_iterator != navtracks_end;
        navtracks_iterator++ )
   {
      FAItem< TRTrackFitQuality > trackQuality =
         navtracks_iterator->quality( particle );
      if ( trackQuality == NULL )
      {
         report( EMERGENCY, kFacilityString )
            << "quality object is missing from the NavTrack!" << endl;
         return ActionBase::kFailed;
      }
      
      identifiers.push_back( navtracks_iterator->identifier() );
      fitaborts.push_back( trackQuality->fitAbort() );
      numtracks++;

      if ( trackQuality->fitAbort() )
	 any_fitaborts = true;
   }

   if ( any_fitaborts )
   {
      report( NOTICE, kFacilityString )
	 << "fit aborts (id,abort?): ";
      for ( Count i = 0;  i < numtracks;  i++ )
      {
	 report( NOTICE, kFacilityString )
	    << "( " << identifiers[i] << ", "
	    << ( fitaborts[i] ? "yes" : "no" )
	    << " ) ";
      }
      report( NOTICE, kFacilityString ) << endl;
      return ActionBase::kPassed;
   }

   report( DEBUG, kFacilityString )
      << "There were no fitaborts." << endl;
   return ActionBase::kFailed;
}

/*
ActionBase::ActionResult
FindFitAborts::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
FindFitAborts::endRun( Frame& iFrame )         // anal4 equiv.
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
