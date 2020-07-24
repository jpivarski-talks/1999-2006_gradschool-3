// -*- C++ -*-
//
// Package:     ConfLev
// Module:      ConfLev
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Wed Feb 20 19:00:25 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "ConfLev/ConfLev.h"
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

#include "CLHEP/Vector/ThreeVector.h"
#include "VXFit/VXFitVeeKShort.h"

#include "CleoDB/DBEventHeader.h"

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

// external functions
extern "C" float prob_(const float&, const int&);

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Processor.ConfLev" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.25 2002/01/25 20:42:06 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_03_01 $";

//
// static data member definitions
//


//
// constructors and destructor
//
ConfLev::ConfLev( void )               // anal1
   : Processor( "ConfLev" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &ConfLev::event,    Stream::kEvent );
   //bind( &ConfLev::beginRun, Stream::kBeginRun );
   //bind( &ConfLev::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

ConfLev::~ConfLev()                    // anal5
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
ConfLev::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
ConfLev::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
ConfLev::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const int bins = 100;

   h_track_chi2 = iHistoManager.histogram(
      100, "Track Chi^2!", bins, 0., 6. );
   h_track_chi2dof = iHistoManager.histogram(
      200, "Track Chi^2! per d.o.f", bins, 0., 6. );
   h_track_confLevel = iHistoManager.histogram(
      300, "Track Confidence Level", bins, 0., 1. );

   h_ks_chi2 = iHistoManager.histogram(
      1210, "K0-short Chi^2!", bins, 0., 20. );
   h_ks_chi2dof = iHistoManager.histogram(
      1220, "K0-short Chi^2! per d.o.f", bins, 0., 6. );
   h_ks_confLevel = iHistoManager.histogram(
      1240, "K0-short Confidence Level", bins, 0., 1. );
   h_ks_mass = iHistoManager.histogram(
      1250, "K0-short Mass", bins, 0.475, 0.525 );
   
   h_ks_vertex_xy = iHistoManager.histogram(
      1260, "K0-short vertex Y vs X", bins, -0.03, 0.03, bins, -0.03, 0.03 );
   h_ks_vertex_z = iHistoManager.histogram(
      1270, "K0-short vertex Z", bins, -0.06, 0.06 );
   
   h_ks_confLevel_non_ks = iHistoManager.histogram(
      1500, "K0-short Confidence Level OUTSIDE sharp mass cut", bins, 0., 1. );
   h_ks_confLevel_ks = iHistoManager.histogram(
      1510, "K0-short Confidence Level WITHIN sharp mass cut", bins, 0., 1. );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
ConfLev::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );
   unsigned int run = header->run();
   unsigned int event = header->number();

//   if ( run == 123717  &&  event >= 62065 ) return ActionBase::kFailed;

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

      double d0( pionHelix->d0() );
      double z0( pionHelix->z0() );
      double hit_fraction( (*pionQuality).ratioNumberHitsToExpected() );
      double chi2( pionQuality->chiSquare() );
      int dof( pionQuality->degreesOfFreedom() );

      if ( true                           &&
	   ! pionQuality->fitAbort()      &&
	   ! seedQuality->originUsed()    &&
	   abs( d0 ) < 5.*k_mm            &&
	   abs( z0 ) < 5.*k_cm            &&
	   0.5 < hit_fraction             &&
	   hit_fraction < 1.2             &&
	   dof > 0                        &&
	   true ) {

	 report( DEBUG, kFacilityString ) << "good track!!" << endl;

	 h_track_chi2->fill( d0 );
	 h_track_chi2dof->fill( chi2 / double( dof ) );
	 h_track_confLevel->fill( prob_( chi2, dof ) );

      } // end if passes cuts
   } // end track loop

   FATable< VXFitVeeKShort > kshorts;
   extract( iFrame.record( Stream::kEvent ), kshorts );
   FATable< VXFitVeeKShort >::const_iterator ks_iter;
   FATable< VXFitVeeKShort >::const_iterator ks_begin = kshorts.begin();
   FATable< VXFitVeeKShort >::const_iterator ks_end = kshorts.end();

   for( ks_iter = ks_begin; ks_iter != ks_end; ks_iter++ )
   {
      FATable< NavTrack >::const_iterator piplus =
	 trackTable.find( ks_iter->piPlusId() );
      FATable< NavTrack >::const_iterator piminus =
	 trackTable.find( ks_iter->piMinusId() );
      if ( true                                                       &&
	   ! piplus->pionQuality()->fitAbort()          	      &&
	   ! piplus->seedQuality()->originUsed()        	      &&
	   abs( piplus->pionHelix()->d0() ) < 5.*k_mm   	      &&
	   abs( piplus->pionHelix()->z0() ) < 5.*k_cm   	      &&
	   0.5 < piplus->pionQuality()->ratioNumberHitsToExpected()   &&
	   piplus->pionQuality()->ratioNumberHitsToExpected() < 1.2   &&
	   ! piminus->pionQuality()->fitAbort()          	      &&
	   ! piminus->seedQuality()->originUsed()        	      &&
	   abs( piminus->pionHelix()->d0() ) < 5.*k_mm   	      &&
	   abs( piminus->pionHelix()->z0() ) < 5.*k_cm   	      &&
	   0.5 < piminus->pionQuality()->ratioNumberHitsToExpected()   &&
	   piminus->pionQuality()->ratioNumberHitsToExpected() < 1.2   &&
	   true ) {

	 h_ks_chi2->fill( ks_iter->fitChiSquare() );
	 h_ks_chi2dof->fill( ks_iter->fitChiSquare() / 1. );
	 h_ks_confLevel->fill( prob_( ks_iter->fitChiSquare(), 1 ) );
	 h_ks_mass->fill( ks_iter->mass() );
	 h_ks_vertex_xy->fill( ks_iter->guessVertex().x(),
			       ks_iter->guessVertex().y() );
	 h_ks_vertex_z->fill( ks_iter->guessVertex().z() );

	 if ( abs( ks_iter->mass() - 0.4994 ) < 0.01228 )
	    h_ks_confLevel_ks->fill( prob_( ks_iter->fitChiSquare(), 1 ) );
	 else
	    h_ks_confLevel_non_ks->fill( prob_( ks_iter->fitChiSquare(), 1 ) );

      } // end if pi+ and pi- pass cuts
   } // end loop over K-shorts

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
ConfLev::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
ConfLev::endRun( Frame& iFrame )         // anal4 equiv.
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
