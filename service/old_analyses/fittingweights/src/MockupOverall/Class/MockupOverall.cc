// -*- C++ -*-
//
// Package:     MockupOverall
// Module:      MockupOverall
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Thu Feb 28 11:50:27 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "MockupOverall/MockupOverall.h"
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

#include "ADRGeom/ADRSenseWireStore.h"

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
static const char* const kFacilityString = "Processor.MockupOverall" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.25 2002/01/25 20:42:06 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_03_01 $";

//
// static data member definitions
//


//
// constructors and destructor
//
MockupOverall::MockupOverall( void )               // anal1
   : Processor( "MockupOverall" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &MockupOverall::event,    Stream::kEvent );
   //bind( &MockupOverall::beginRun, Stream::kBeginRun );
   //bind( &MockupOverall::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

   m_layer_info_valid = false;
   for ( int i = 0;  i < 47;  i++ ) {
      m_layer_num_cells[i] = 0;
      m_layer_radius[i] = 0;
   }

}

MockupOverall::~MockupOverall()                    // anal5
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
MockupOverall::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
MockupOverall::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
MockupOverall::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
MockupOverall::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   if ( ! m_layer_info_valid ) {
      FAItem< ADRSenseWireStore > adrWireStore;
      extract( iFrame.record( Stream::kBaseGeometry ), adrWireStore );
      for ( int layer = 1;  layer <= 47;  layer++ ) {
	 m_layer_num_cells[layer-1] = adrWireStore->numberOfWiresLyr( layer );
	 m_layer_radius[layer-1] = adrWireStore->radius( layer );
      } // end foreach layer
      m_layer_info_valid = true;
   } // end if we have never done this before

   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator navtracks_iterator;
   FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();

   DBCandidate::Hypo particle( DBCandidate::kElectron );

////  the cuts  ////////////////////////////////////////////////////////
   if ( navtracks.size() != 2 ) return ActionBase::kFailed;
      
   double d0miss = 0.;
   double z0miss = 0.;
   double curvSignProduct = 1.;
   DABoolean bad_track = false;
   for( navtracks_iterator = navtracks_begin;
	navtracks_iterator != navtracks_end;
	navtracks_iterator++ )
   {
      FAItem< TDKinematicFit > trackFit =
	 navtracks_iterator->kinematicFit( particle );
      FAItem< TRHelixFit > trackHelix =
	 navtracks_iterator->helixFit( particle );
      FAItem< TRTrackFitQuality > trackQuality =
	 navtracks_iterator->quality( particle );
	 
      double curvSign = ( trackHelix->curvature() > 0. ? 1. : -1. );
      d0miss += trackHelix->d0();
      z0miss += trackHelix->z0() * curvSign;
      curvSignProduct *= curvSign;

      HepVector3D momentum = trackFit->momentum();
      double chiSquare = trackQuality->chiSquare();
      int degreesOfFreedom = trackQuality->degreesOfFreedom();
      int numberHits = trackQuality->numberHits();
      int numberHitsExpected = trackQuality->numberHitsExpected();
      double d0 = trackHelix->d0();
      double z0 = trackHelix->z0();
      double cotTheta_err =
	 sqrt( abs( trackHelix->errorMatrix()(
	    KTHelix::kCotTheta, KTHelix::kCotTheta ) ) );
      double z0_err =
	 sqrt( abs( trackHelix->errorMatrix()(
	    KTHelix::kZ0, KTHelix::kZ0 ) ) );

      if ( false                                                             ||

	   ( trackQuality->fitAbort() )                                      ||

	   ( momentum.mag() < 4.5  ||  momentum.mag() > 6.0 )                ||

	   ( degreesOfFreedom == 0  ||
	     chiSquare / double( degreesOfFreedom ) > 50.  ||
	     chiSquare / double( degreesOfFreedom ) < 0.      )              ||

	   ( numberHitsExpected == 0  ||
	     double( numberHits ) / double( numberHitsExpected ) < 0.5 )     ||

	   ( abs( d0 ) > 0.03  ||  abs( z0 ) > 0.150 )                       ||

	   ( cotTheta_err > 0.50  ||  z0_err > 0.25 )                        ||

	   false ) bad_track = true;

   } // end loop over tracks (first check sweep)
   if ( curvSignProduct > 0. ) return ActionBase::kFailed;
   if ( bad_track ) return ActionBase::kFailed;

////  the real thing  //////////////////////////////////////////////////
   for( navtracks_iterator = navtracks_begin;
	navtracks_iterator != navtracks_end;
	navtracks_iterator++ )
   {
      FAItem< TDKinematicFit > trackFit =
	 navtracks_iterator->kinematicFit( particle );
      FAItem< TRHelixFit > trackHelix =
	 navtracks_iterator->helixFit( particle );
      TRHelixFit swimmable_helix = (* trackHelix );
      
      double cotTheta = trackHelix->cotTheta();
      double cosTheta = cotTheta / sqrt( 1 + sqr( cotTheta ) );
	 
      const NavTrack::DRHitTable* p_trackDRHits =
	 navtracks_iterator->drHits( particle );
      if ( p_trackDRHits == NULL ) continue;
      NavTrack::DRHitTable trackDRHits = (* p_trackDRHits );
      
      NavTrack::DRHitTable::const_iterator trackDRHits_iterator;
      NavTrack::DRHitTable::const_iterator trackDRHits_begin =
	 trackDRHits.begin();
      NavTrack::DRHitTable::const_iterator trackDRHits_end =
	 trackDRHits.end();
      
      const NavTrack::DRHitLinkTable* trackDRLinks =
	 navtracks_iterator->drHitLinks( particle );
      if ( trackDRLinks == NULL ) continue;
      NavTrack::DRHitLinkTable::const_iterator trackDRLinks_iterator;
      NavTrack::DRHitLinkTable::const_iterator trackDRLinks_begin =
	 trackDRLinks->begin();
      NavTrack::DRHitLinkTable::const_iterator trackDRLinks_end =
	 trackDRLinks->end();
      
      for( trackDRHits_iterator = trackDRHits_begin;
	   trackDRHits_iterator != trackDRHits_end;
	   trackDRHits_iterator++ )
      {
	 for( trackDRLinks_iterator = trackDRLinks_begin;
	      trackDRLinks_iterator != trackDRLinks_end
		 &&  (     (* trackDRLinks_iterator )->rightID() == 0
			   ||  *(* trackDRLinks_iterator )->rightID() !=
			   ( *trackDRHits_iterator )->identifier()
		    );
	      trackDRLinks_iterator++ );
	 if ( trackDRLinks_iterator == trackDRLinks_end       ||
	      (* trackDRLinks_iterator )->rightID() == 0 ) continue;
	 const TrackFitDRHitLink& trackDRHitLink =
	    (* trackDRLinks_iterator )->linkData();
	 
	 if ( trackDRHitLink.used() )
	 {
	    unsigned int layer = (* trackDRHits_iterator )->layer();
	    double residual = trackDRHitLink.residual();
	    double edca = trackDRHitLink.signedDcaError();
	    double drift = trackDRHitLink.signedDriftDistance();
	    double edrift = trackDRHitLink.signedDriftError();
//	    double sinAlpha = trackDRHitLink.sinTrackToRadial();
	    
	    double absDrift = get_absDrift( layer, drift );
	    
	    cout << "JIMMYNTUPLE "
		 << " " << residual
		 << " " << sqr( edrift ) << " " << sqr( edca )
		 << " " << layer << " " << absDrift << " " << cosTheta
		 << endl;
	       
	 } // end if hit used in the fit
      } // end loop over DR hits
	 
   } // end loop over tracks (the real thing)

   return ActionBase::kPassed;
}

double
MockupOverall::get_absDrift( unsigned int layer, double drift )
{
   assert( m_layer_info_valid );

   return ( abs( drift ) * double( m_layer_num_cells[layer-1] ) /
	    ( 3.1415926 * m_layer_radius[layer-1] ) );
}

/*
ActionBase::ActionResult
MockupOverall::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
MockupOverall::endRun( Frame& iFrame )         // anal4 equiv.
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
