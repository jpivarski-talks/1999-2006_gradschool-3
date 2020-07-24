// -*- C++ -*-
//
// Package:     HitLevel
// Module:      HitLevel
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri Jun 15 16:05:16 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "HitLevel/HitLevel.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "ADRGeom/ADRSenseWireStore.h"
#include "Navigation/NavTrack.h"
#include "Navigation/NavShower.h"
#include "C3cc/CcShowerAttributes.h"
#include "Navigation/NavConReg.h"
#include "KinematicTrajectory/KTKinematicData.h"
#include "Navigation/NavTkShMatch.h"
#include "C3cc/CcAssignedEnergyHit.h"
#include "TrackFitter/TrackFitDRHitLink.h"
// #include "EventType/EventType.h"
#include "C3cc/CcQedEvent.h"

#include "CLHEP/Vector/ThreeVector.h"

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
static const char* const kFacilityString = "Processor.HitLevel" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.17 2000/12/04 19:11:11 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
HitLevel::HitLevel( void )               // anal1
   : Processor( "HitLevel" )
   , m_num_hits( 0 )
   , m_done( false )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &HitLevel::event,    Stream::kEvent );
   //bind( &HitLevel::beginRun, Stream::kBeginRun );
   //bind( &HitLevel::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

HitLevel::~HitLevel()                    // anal5
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
HitLevel::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
HitLevel::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
HitLevel::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

//     const char* labels[ 14 ] = 
//     { "layer", "radius", "cells", "ndca", "ndrift", "sinalpha", "dca", "drift",
//       "edca", "edrift", "resid", "normres", "cottheta", "pperp" };
//     m_hitvars = iHistoManager.ntuple( 1, "Hit-level vars", 14, 200000, labels );

   const char* labels[ 4 ] =
   { "layer", "resid", "phi", "cottheta" };
   m_hitvars = iHistoManager.ntuple( 1, "Some hit-level vars", 4, 200000, labels );

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
HitLevel::event( Frame& iFrame )          // anal3 equiv.
{
   report( INFO, kFacilityString ) << "here in event()" << endl;

   if ( m_done )
      return ActionBase::kPassed;

   report( INFO, kFacilityString ) << "not yet done..." << endl;

//     FAItem< EventType > event_type;
//     extract( iFrame.record( Stream::kEvent ), event_type );
//     if ( ! event_type->bhabhaTight() ) return ActionBase::kFailed;

//     FAItem< CcQedEvent > event_type;
//     extract( iFrame.record( Stream::kEvent ), event_type );
//     if ( ! event_type->bhabhaBarrel()  &&  ! event_type->bhabhaEndcap() )
//        return ActionBase::kFailed;

   FATable< NavShower > navshowers;
   extract( iFrame.record( Stream::kEvent ), navshowers );
   FATable< NavShower >::const_iterator navshowers_iterator;
   FATable< NavShower >::const_iterator navshowers_begin = navshowers.begin();
   FATable< NavShower >::const_iterator navshowers_end = navshowers.end();

   int good_showers = 0;
   for ( navshowers_iterator = navshowers_begin;
 	 navshowers_iterator != navshowers_end;
 	 navshowers_iterator++ )
   {
      const CcShowerAttributes& atts ( navshowers_iterator->attributes() );
      if ( atts.goodBarrel()  &&  atts.energy() > 1.50 )
	 good_showers++;
   }

   if ( good_showers != 2 )
   {
      report( INFO, kFacilityString )
 	 << "I saw " << good_showers << " good shower(s) (!= 2)." << endl;
      return ActionBase::kFailed;
   }

   report( INFO, kFacilityString )
      << "I saw two good showers, so I'll reconstruct tracks now." << endl;

   int matched_showers = 0;
   for ( navshowers_iterator = navshowers_begin;
 	 navshowers_iterator != navshowers_end;
 	 navshowers_iterator++ )
   {
      const CcShowerAttributes& atts ( navshowers_iterator->attributes() );
      const CcGeV energy ( atts.energy() );
      if ( atts.goodBarrel()  &&  energy > 0.150
	   &&  ! navshowers_iterator->noTrackMatch() )
	 matched_showers++;
   }   

   if ( matched_showers != 2 )
   {
      report( INFO, kFacilityString )
 	 << "The two good showers weren't both matched to tracks." << endl;
      return ActionBase::kFailed;
   }

   report( INFO, kFacilityString )
      << "I saw two good showers, matched to tracks, so I'll get to work!" << endl;

   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator navtracks_iterator;
   FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();
   report( DEBUG, kFacilityString ) << "gatherData: got NavTracks" << endl;

   int numtracks = navtracks.size();
   // First cut: electron pairs must have exactly two tracks
   if ( numtracks != 2 )
   {
      // Report to NOTICE if failed (later INFO or DEBUG)
      report( NOTICE, kFacilityString )
	 << "Event doesn't pass first quality cut: number of tracks is "
	 << numtracks << "." << endl;
      return ActionBase::kFailed;
   }
   report( INFO, kFacilityString ) << "gatherData: have two tracks" << endl;

   HepVector3D track_momentum_sum( 0., 0., 0. );
   // now for the bhabha kinematic cuts
   for( navtracks_iterator = navtracks_begin;
 	navtracks_iterator != navtracks_end;
 	navtracks_iterator++ )
   {
      // Get the Kalman-fitted track
      FAItem< TDKinematicFit > trackFit =
 	 navtracks_iterator->electronFit();
      if ( trackFit == NULL )
      {
 	 report( EMERGENCY, kFacilityString )
 	    << "electronFit is missing from the NavTrack!" << endl;
 	 return ActionBase::kFailed;
      }

      if ( abs( trackFit->momentum().mag() - 5.179 ) > 1.25 )
	 return ActionBase::kFailed;

      FAItem< TRHelixFit > trackHelix =
	 navtracks_iterator->muonHelix();
      if ( trackHelix == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "muonHelix is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }

      if ( abs( trackHelix->d0() ) > 0.0032 )
	 return ActionBase::kFailed;

      if ( abs( trackHelix->z0() ) > 0.04 )
	 return ActionBase::kFailed;

      track_momentum_sum += trackFit->momentum();
   }

   report( DEBUG, kFacilityString )
      << "total_momentum = " << track_momentum_sum << endl;

   double xdiff2 = sqr( track_momentum_sum.x() + 0.0261 );
   double ydiff2 = sqr( track_momentum_sum.y() + 0.0003 );
   if ( ( xdiff2 + ydiff2 ) > 0.300 ) return ActionBase::kFailed;

   FAItem< ADRSenseWireStore > adrWireStore;
   extract( iFrame.record( Stream::kBaseGeometry ), adrWireStore );
   if ( ! adrWireStore.valid() )
   {
      report( EMERGENCY, kFacilityString )
	 << "ADRSenseWireStore is not valid!" << endl;
      return ActionBase::kFailed;
   }

   // Loop over NavTracks
   for( navtracks_iterator = navtracks_begin;
	navtracks_iterator != navtracks_end;
	navtracks_iterator++ )
   {
      // Get the Kalman-fitted track
      FAItem< TDKinematicFit > trackFit =
	 navtracks_iterator->muonFit();
      if ( trackFit == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "muonFit is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }

      // and the Kalman-fitted helix
      FAItem< TRHelixFit > trackHelix =
	 navtracks_iterator->muonHelix();
      if ( trackHelix == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "muonHelix is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }
      TRHelixFit loc_trackHelix = (* trackHelix );

//        // Move the track's reference point to the origin so that d0 and
//        // z0 will be right (later, this will be to the beamspot)
//        report( INFO, kFacilityString )
//  	 << "Attempting to move helix." << endl;
//        Meters movelength;
//        KTHelix::MoveStatus status =
//  	 loc_trackHelix.moveToReferencePoint( HepPoint3D( -0.00210, 0., 0. ),
//  					       movelength );
//        if ( status != KTMoveControl::kMoveOK )
//        {
//  	 report( EMERGENCY, kFacilityString )
//  	    << "trackHelix.moveToReferencePoint returned "
//  	    << status << " (not kMoveOK, as it should)." << endl;
//  	 return ActionBase::kFailed;
//        }
//        report( INFO, kFacilityString )
//  	 << "Moved helix sucessfully." << endl;

      // Get its quality object
      FAItem< TRTrackFitQuality > trackQuality =
	 navtracks_iterator->muonQuality();
      if ( trackQuality == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "electronQuality is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }

      // Get the DR hits associated with it
      const vector< const CalibratedDRHit* >* p_trackDRHits =
	 navtracks_iterator->muonDRHits();
      if ( p_trackDRHits == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "muonDRHits is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }
      vector< const CalibratedDRHit* > trackDRHits = (* p_trackDRHits );

      vector< const CalibratedDRHit* >::const_iterator trackDRHits_iterator;
      vector< const CalibratedDRHit* >::const_iterator trackDRHits_begin =
	 trackDRHits.begin();
      vector< const CalibratedDRHit* >::const_iterator trackDRHits_end =
	 trackDRHits.end();

      // Get the variables to cut on
      HepVector3D momentum = trackFit->momentum();
      double cotTheta = loc_trackHelix.cotTheta();
      double curvature = loc_trackHelix.curvature();
      double cosTheta = cotTheta / sqrt( 1 + cotTheta * cotTheta );
      double chiSquare = trackQuality->chiSquare();
      int degreesOfFreedom = trackQuality->degreesOfFreedom();
      int numberHits = trackQuality->numberHits();
      int numberHitsExpected = trackQuality->numberHitsExpected();
      double d0 = loc_trackHelix.d0();
      double z0 = loc_trackHelix.z0();
      double cotTheta_err =
	 sqrt( abs( loc_trackHelix.errorMatrix()( KTHelix::kCotTheta,
						  KTHelix::kCotTheta ) ) );
      double z0_err =
	 sqrt( abs( loc_trackHelix.errorMatrix()( KTHelix::kZ0,
						  KTHelix::kZ0 ) ) );

      // Cut on them (REJECT if any of the below are TRUE).
      if ( trackQuality->fitAbort()                                    ||

//  // 	   momentum.mag() < 4.5                                        ||
//  //	   momentum.mag() > 6.0                                        ||

//  // 	   momentum.mag() < 0.1                                        ||
//  // 	   momentum.mag() > 7.0                                        ||

//    	   momentum.mag() < 0.0                                        ||
//    	   momentum.mag() > 4.0                                        ||

	   abs( cosTheta ) > 0.95                                      ||
  	   degreesOfFreedom == 0                                       ||
 	   chiSquare / double( degreesOfFreedom ) > 10.                ||
//  	   chiSquare / double( degreesOfFreedom ) > 50.                ||
//  	   chiSquare / double( degreesOfFreedom ) <= 0.0               ||
  	   numberHitsExpected == 0                                     ||
  	   double( numberHits ) / double( numberHitsExpected ) < 0.5   ||
 	   double( numberHits ) / double( numberHitsExpected ) > 1.0   ||
//  	   abs( d0 ) > 0.03                                            ||
//  	   abs( z0 ) > 0.150                                           ||
  	   cotTheta_err > 0.500                                        ||
  	   z0_err > 0.25                                                   )
      {
	 // Report to NOTICE if failed (later INFO or DEBUG)
	 report( NOTICE, kFacilityString )
	    << "Track doesn't pass quality cuts:" << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "Track criteria:" << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    momentum = " << momentum.mag() << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    chiSquare = " << chiSquare
//  	    << ", degreesOfFreedom = " << degreesOfFreedom << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    numberHits = " << numberHits
//  	    << ", numberHitsExpected = " << numberHitsExpected << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    curvature = " << loc_trackHelix.curvature() << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    cotTheta = " << cotTheta << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    d0 = " << d0 << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    z0 = " << z0 << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    phi0 = " << loc_trackHelix.phi0() << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    cotTheta_err = " << cotTheta_err << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "    z0_err = " << z0_err << endl;
      }
      else // passes cuts
      {

	 for( trackDRHits_iterator = trackDRHits_begin;
	      trackDRHits_iterator != trackDRHits_end;
	      trackDRHits_iterator++ )
	 {
	    // get the link between this track and this hit, and its linkdata
	    report( DEBUG, kFacilityString )
	       << "Trying to get hit link..." << endl;
	    const NavTrack::DRHitLinkTable* trackDRLinks =
	       navtracks_iterator->muonDRHitLinks();
	    if ( trackDRLinks == NULL )
	    {
	       report( EMERGENCY, kFacilityString )
		  << "NavTrack is missing the muon hit links!" << endl;
	       return ActionBase::kFailed;
	    }
	    NavTrack::DRHitLinkTable::const_iterator trackDRLinks_iterator;
	    NavTrack::DRHitLinkTable::const_iterator trackDRLinks_begin =
	       trackDRLinks->begin();
	    NavTrack::DRHitLinkTable::const_iterator trackDRLinks_end =
	       trackDRLinks->end();
	    
	    for( trackDRLinks_iterator = trackDRLinks_begin;
		 trackDRLinks_iterator != trackDRLinks_end
		    &&  (     (* trackDRLinks_iterator )->rightID() == 0
			      ||  *(* trackDRLinks_iterator )->rightID() !=
			      ( *trackDRHits_iterator )->identifier()
		       );
		 trackDRLinks_iterator++ );
	    
	    if ( trackDRLinks_iterator == trackDRLinks_end       ||
		 (* trackDRLinks_iterator )->rightID() == 0 )
	    {
	       report( EMERGENCY, kFacilityString )
		  << "Couldn't get the link between this track ("
		  << navtracks_iterator->identifier() << ") "
		  << "and this hit ( "
		  << (* trackDRHits_iterator )->identifier()
		  << "), even avoiding Lattice." << endl;
	       return ActionBase::kFailed;
	    }
	    const TrackFitDRHitLink& trackDRHitLink =
	       (* trackDRLinks_iterator )->linkData();

	    if ( trackDRHitLink.used() )
	    {
	       // variables for calculating chi( lambda )
	       Meters dca = trackDRHitLink.signedDcaToWire();
	       Meters e_dca = trackDRHitLink.signedDcaError();
	       Meters drift = trackDRHitLink.signedDriftDistance();
	       Meters e_drift = trackDRHitLink.signedDriftError();
	       
	       int layer = (* trackDRHits_iterator )->layer();
	       double layerRadius = adrWireStore->radius( layer );
	       int layerCells = adrWireStore->numberOfWiresLyr( layer );
	       double normdca = ( dca * double( layerCells ) /
				  ( 3.1415926 * layerRadius ) );
	       double normdrift = ( drift * double( layerCells ) /
				    ( 3.1415926 * layerRadius ) );
	       double sinalpha = layerRadius * curvature;

	       double residual = drift - dca;
	       double normresidual = 0.;
	       if ( e_dca > 0.  &&  e_drift > 0. )
		  normresidual = residual / sqrt( e_dca * e_dca +
						  e_drift * e_drift );

	       double distance_swim;
	       double z = loc_trackHelix.moveToRadius( layerRadius, distance_swim );
	       int wire_number = (* trackDRHits_iterator )->wireInLyr();
	       double phi = adrWireStore->wire( layer, wire_number ).zPoint( z ).first.phi();

//             This filled in the wrong order for the following:
//                 hadron_hitstudy.rzn
//                 hadron_nogarfield_hitstudy.rzn
//                 radbhabha_hitstudy.rzn
//  	       float hitvars[ 12 ] = {
//  		  float( layer ), layerRadius, normdrift, sinalpha,
//  		  dca, e_dca, drift, e_drift, residual, normresidual,
//  		  cotTheta, momentum.perp() };
//  	       float hitvars[ 14 ] = {
//  		  float( layer ), layerRadius, float( layerCells ), normdca,
//  		  normdrift, sinalpha, dca, drift, e_dca, e_drift,
//  		  residual, normresidual, cotTheta, momentum.perp() };

	       float hitvars[ 4 ] = { float( layer ), residual, phi, cotTheta };

	       m_hitvars->fill( hitvars );
	       m_num_hits++;

//  	       if ( m_num_hits > 300000 )
//  		  m_done = true;
	    } // end if used
	 } // end loop over tracks
      } // end track passes cuts
   } // end loop over tracks

   report( INFO, kFacilityString ) << "everything worked!" << endl;

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
HitLevel::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
HitLevel::endRun( Frame& iFrame )         // anal4 equiv.
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
