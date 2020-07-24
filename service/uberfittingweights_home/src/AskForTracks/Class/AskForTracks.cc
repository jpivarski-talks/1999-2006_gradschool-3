// -*- C++ -*-
//
// Package:     AskForTracks
// Module:      AskForTracks
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Sep 24 13:15:25 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "AskForTracks/AskForTracks.h"
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
static const char* const kFacilityString = "Processor.AskForTracks" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
AskForTracks::AskForTracks( void )               // anal1
   : Processor( "AskForTracks" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &AskForTracks::event,    Stream::kEvent );
   //bind( &AskForTracks::beginRun, Stream::kBeginRun );
   //bind( &AskForTracks::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

AskForTracks::~AskForTracks()                    // anal5
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
AskForTracks::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
AskForTracks::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
AskForTracks::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
AskForTracks::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;


   // Create a table of tracks and fill it.
   FATable< NavTrack > trackTable;
   extract( iFrame.record( Stream::kEvent ) , trackTable );

   FATable< NavTrack >::const_iterator trackTableBegin = trackTable.begin();
   FATable< NavTrack >::const_iterator trackTableEnd = trackTable.end();
   // Loop over tracks.
   for ( FATable< NavTrack >::const_iterator trackItr = trackTableBegin;
         trackItr != trackTableEnd ;
         ++trackItr )
   {
      // Print out the track's ID number.
      report( INFO, kFacilityString )
         << "track "
         << (*trackItr).identifier()
         << endl;

      // Ask for the pion fit and quality information.
      FAItem< TDKinematicFit > pionFit = (*trackItr).pionFit();
      FAItem< TRHelixFit > pionHelix = (*trackItr).pionHelix();
      FAItem< TRTrackFitQuality > pionQuality = (*trackItr).pionQuality();
      FAItem< TRSeedTrackQuality > seedQuality = (*trackItr).seedQuality();

      // Now apply some of the standard rare-B analysis cuts.
      if ( (*seedQuality).numberHitsExpected() == 0)
      {
         report(WARNING, kFacilityString)
            <<"Number of Hits expected is zero, skipping this track!" 
		  << endl;   
         continue;
      }

      float hit_fraction 
               = (*pionQuality).ratioNumberHitsToExpected();
      

      double cos_theta =
         sqrt( 1 - 1 / ( 1+sqr((*pionHelix).cotTheta()) ) );
      // You could also have:
      //    double cos_theta = (*pionFit).momentum().cosTheta();
      // but then you'd need to make sure that you take its absolute
      // value when using it, assuming that's what you want.

      if ( !(*pionQuality).fitAbort()          // not an aborted fit
           &&
           !(*seedQuality).originUsed()        // not a "z-escape"
           &&
           ( (*pionFit).pmag() <= 5.3*k_GeV )
           &&
           ( (*pionFit).pmag() > 100*k_MeV )
           &&
           ( hit_fraction >= 0.3 )
           &&
           ( cos_theta <= 0.9 ) )   // or abs(cos_theta)
      {
         report( INFO, kFacilityString )
            << "  momentum: "
            << (*pionFit).momentum()
            << endl;
      }
   }



   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
AskForTracks::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
AskForTracks::endRun( Frame& iFrame )         // anal4 equiv.
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
