// -*- C++ -*-
//
// Package:     DoTests
// Module:      DoTests
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Mar 18 11:50:28 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DoTests/DoTests.h"
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
//RICH example 
#include "Navigation/NavRich.h"


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
static const char* const kFacilityString = "Processor.DoTests" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
DoTests::DoTests( void )               // anal1
   : Processor( "DoTests" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &DoTests::event,    Stream::kEvent );
   //bind( &DoTests::beginRun, Stream::kBeginRun );
   //bind( &DoTests::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

DoTests::~DoTests()                    // anal5
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
DoTests::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DoTests::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
DoTests::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
DoTests::event( Frame& iFrame )          // anal3 equiv.
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

   FATable< NavShower > showerTable;
   extract( iFrame.record( Stream::kEvent ) , showerTable );

   FATable< NavShower >::const_iterator showerBegin = showerTable.begin();
   FATable< NavShower >::const_iterator showerEnd = showerTable.end();
   for (FATable< NavShower >::const_iterator showerItr = showerBegin; 
        showerItr != showerEnd ; ++showerItr )
   {
      report( INFO, kFacilityString )
         << "shower "
         << (*showerItr).identifier()
         << endl;
      report( INFO, kFacilityString )
         << "  energy: "
         << (*showerItr).attributes().energy()
         << endl;
   }

   //Loop over the NavTrack table to get rich info for each track
   for( FATable< NavTrack >::const_iterator navItrForRich = trackTableBegin; 
        navItrForRich != trackTableEnd; ++navItrForRich )
   {

      //Retrieve the rich info associated with the track
      FAItem<NavRich> richInfo = (*navItrForRich).richInfo();

      if( richInfo.valid() )
      {
         //Obtain the likelihood to be a Kaon 
         double kaonLLikelihood = (*richInfo).kaonLogLikelihood();
         //Obtain the likelihood to be a Pion
         double pionLLikelihood = (*richInfo).pionLogLikelihood();

         //if the difference is less than zero, then it is more
         //likely to be a kaon than a pion
         double likelihoodToBeKaon = 
           kaonLLikelihood - pionLLikelihood;
      }
    }


   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
DoTests::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
DoTests::endRun( Frame& iFrame )         // anal4 equiv.
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
