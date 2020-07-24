// -*- C++ -*-
//
// Package:     FindBug2
// Module:      FindBug2
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Tue Nov  6 14:43:29 EST 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "FindBug2/FindBug2.h"
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
static const char* const kFacilityString = "Processor.FindBug2" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.23 2001/10/31 19:48:05 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_02_00 $";

//
// static data member definitions
//


//
// constructors and destructor
//
FindBug2::FindBug2( void )               // anal1
   : Processor( "FindBug2" )
   , m_world2local( NULL )
   , m_num_rphi( 0 )
   , m_num_z( 0 )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &FindBug2::event,    Stream::kEvent );
   //bind( &FindBug2::beginRun, Stream::kBeginRun );
   //bind( &FindBug2::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

FindBug2::~FindBug2()                    // anal5
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
FindBug2::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

   m_num_rphi = m_num_z = 0;
}

// -------------------- terminate method ----------------------------
void
FindBug2::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
FindBug2::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

      const char* m_lables[ kNumVars ] = {
	 "cotTh", "phi0", "z0",
	 "layer", "ladder", "sensor", "hybrid",
	 "locX", "locY", "locZ",
	 "tlocX", "tlocY", "tlocZ",
	 "worldX", "worldY", "worldZ",
	 "trackX", "trackY", "trackZ", "trackphi",
	 "residual"
      };
      
   m_rphi = iHistoManager.ntuple(
      1, "rphi", kNumVars, kLargeRecord, m_lables );

   m_z = iHistoManager.ntuple(
      2, "z", kNumVars, kLargeRecord, m_lables );

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
FindBug2::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< ASiStore > sistore;
   extract( iFrame.record( Stream::kSVAlignment ), sistore );

   if ( m_world2local == NULL )
   {
      m_world2local = new HepTransform3D[ kNumSensors ];
      for ( unsigned int i = 1;  i <= kNumSensors;  i++ )
	 m_world2local[ i-1 ] = sistore->sensor( i ).transToSi().inverse();
   } // end initialize m_world2local

   HISiSurfaceFactory siSurfaceFactory( iFrame, NULL, NULL, false );

   report( NOTICE, kFacilityString ) << "Extract hits:" << endl;
   FATable< CalibratedSVRphiHit > calibrated_svr_hits;
   extract( iFrame.record( Stream::kEvent ), calibrated_svr_hits );
   FATable< CalibratedSVZHit >    calibrated_svz_hits;
   extract( iFrame.record( Stream::kEvent ), calibrated_svz_hits );
   report( NOTICE, kFacilityString )
      << "    Number of rphi hits = " << calibrated_svr_hits.size() << endl;
   report( NOTICE, kFacilityString )
      << "    Number of z hits = " << calibrated_svz_hits.size() << endl;

   report( NOTICE, kFacilityString ) << "Calling track fitter:" << endl;
   FATable< TRHelixMuonFit > muonfit_tracks;
   extract( iFrame.record( Stream::kEvent ), muonfit_tracks );
   report( NOTICE, kFacilityString )
      << "    Number of (muon) tracks = " << muonfit_tracks.size() << endl;
   if ( muonfit_tracks.size() != 2 ) return ActionBase::kFailed;

   FATable< TRTrackMuonFitQuality > muonquality_tracks;
   extract( iFrame.record( Stream::kEvent ), muonquality_tracks );
   DABoolean both_succeeded = true;
   for ( FATable< TRTrackMuonFitQuality >::const_iterator muonquality_iter
	    = muonquality_tracks.begin();
	 muonquality_iter != muonquality_tracks.end();
	 muonquality_iter++ )
   {
      report( NOTICE, kFacilityString )
	 << "    Track " << muonquality_iter->identifier();
      if ( muonquality_iter->fitAbort() )
      {
	 report( NOTICE, kFacilityString ) << " failed to be fit properly." << endl;
	 both_succeeded = false;
      }
      else
	 report( NOTICE, kFacilityString ) << " was properly fit." << endl;
   }
   if ( ! both_succeeded ) return ActionBase::kFailed;

   FAItem< MuonFitSVRHitLattice > muonfit_svr_lattice;
   extract( iFrame.record( Stream::kEvent ), muonfit_svr_lattice );
   FAItem< MuonFitSVZHitLattice > muonfit_svz_lattice;
   extract( iFrame.record( Stream::kEvent ), muonfit_svz_lattice );

   for ( FATable< TRHelixMuonFit >::const_iterator muonfit_iter
	    = muonfit_tracks.begin();
	 muonfit_iter != muonfit_tracks.end();
	 muonfit_iter++ )
   {
      HIHelix swimming_track = *muonfit_iter;
      report( NOTICE, kFacilityString )
	 << "    ==== Fitted track " << muonfit_iter->identifier()
	 << "=====================================" << endl;

      double phi0 = swimming_track.phi0();
      double cotTheta = swimming_track.cotTheta();
      double z0 = swimming_track.z0();
      report( NOTICE, kFacilityString )
	 << "    curvature = " << swimming_track.curvature()
	 << " phi0 = " << phi0
	 << " d0 = " << swimming_track.d0()
	 << " cotTheta = " << cotTheta
	 << " z0 = " << swimming_track.z0() << endl;

      const MuonFitSVRHitLattice::Links svr_links
	 = muonfit_svr_lattice->linksGivenLeft( muonfit_iter->identifier() );

      const MuonFitSVZHitLattice::Links svz_links
	 = muonfit_svz_lattice->linksGivenLeft( muonfit_iter->identifier() );

      vector< const CalibratedSVRphiHit* > ptrs_rphi_hits;
      ptrs_rphi_hits.clear();
      for ( MuonFitSVRHitLattice::Links::const_iterator svr_link_copy_iter
	       = svr_links.begin();
	    svr_link_copy_iter != svr_links.end();
	    svr_link_copy_iter++ )
	 ptrs_rphi_hits.push_back(
	    &calibrated_svr_hits[ (* svr_link_copy_iter)->vRightID()[ 0 ] ] );

      vector< const CalibratedSVZHit* > ptrs_z_hits;
      ptrs_z_hits.clear();
      for ( MuonFitSVZHitLattice::Links::const_iterator svz_link_copy_iter
	       = svz_links.begin();
	    svz_link_copy_iter != svz_links.end();
	    svz_link_copy_iter++ )
	 ptrs_z_hits.push_back(
	    &calibrated_svz_hits[ (* svz_link_copy_iter)->vRightID()[ 0 ] ] );
      
      if ( ptrs_rphi_hits.size() == 0  ||  ptrs_z_hits.size() == 0 ) continue;

      siSurfaceFactory.generateCenterSurfacesWithHits(
	 ptrs_rphi_hits, ptrs_z_hits, 0 );

      vector< HIIntersectionSurface* > surfaces;  surfaces.clear();
      siSurfaceFactory.transferSurfacesTo( surfaces, false );
      vector< HIIntersectionSurface* > composite_surface;
      composite_surface.clear();
      composite_surface.push_back( (* surfaces.begin()) );
      for ( vector< HIIntersectionSurface* >::const_iterator surfaces_iter
	       = surfaces.begin();
	    surfaces_iter != surfaces.end();
	    surfaces_iter++ )
      {
	 HICompositeSurface* tmp_surface
	    = ( (HICompositeSurface*) (* surfaces_iter) );
	 ( (HICompositeSurface*) (* composite_surface.begin()) )
	    ->addChildren( *tmp_surface );
      }

      HIHelixIntersector intersector(
	 composite_surface, HIHelixIntersector::kIncreasingRadius, &swimming_track );

      for ( HIHelixIntersector::IntersectionStatus status =
	       intersector.swimToCurrentSurface(
		  KTMoveControl::kDirectionForward );
	    status == HIHelixIntersector::kIntersectionOK;
	    status = intersector.swimToNextIntersection(
	       KTMoveControl::kDirectionForward ) )
      {
	 HIIntersectionSurface* surface = intersector.currentSurface();
	 do {
	    float v[ kNumVars ];
	    v[ kCotTh ] = cotTheta;
	    v[ kPhi0 ] = phi0;
	    v[ kZ0 ] = z0;

	    CalibratedSVHit* hit = ( CalibratedSVHit* )( surface->calibratedHit() );
	    v[ kLayer ] = hit->layer();
	    v[ kLadder ] = sistore->ladderForSensor( hit->sensor() );
	    v[ kSensor ] = hit->sensor();
	    v[ kHybrid ] = hit->hybrid();

	    HepPoint3D track = swimming_track.position();
	    HepPoint3D track_local = m_world2local[ hit->sensor() ] * track;

	    HepPoint3D local = hit->localcoord();
	    v[ kLocX ] = local.x();
	    v[ kLocY ] = local.y();
	    v[ kLocZ ] = local.z();
	    v[ kTLocX ] = track_local.x();
	    v[ kTLocY ] = track_local.y();
	    v[ kTLocZ ] = track_local.z();

	    HepPoint3D world = hit->worldcoord( *sistore );
	    v[ kWorldX ] = world.x();
	    v[ kWorldY ] = world.y();
	    v[ kWorldZ ] = world.z();
	    v[ kTrackX ] = track.x();
	    v[ kTrackY ] = track.y();
	    v[ kTrackZ ] = track.z();
	    v[ kTrackPhi ] = swimming_track.phi0();
	    
	    v[ kResidual ] = 0. - surface->dca( swimming_track );

	    if ( hit->deviceType() == CalibratedHit::SVR )
	    {
	       m_rphi->fill( v );
	       m_num_rphi++;
	       report( INFO, kFacilityString )
		  << m_num_rphi << "th rphi hit:" << endl;
	    }
	    else if ( hit->deviceType() == CalibratedHit::SVZ )
	    {
	       m_z->fill( v );
	       m_num_z++;
	       report( INFO, kFacilityString )
		  << m_num_z << "th z hit:" << endl;
	    }

	    report( INFO, kFacilityString )
	       << "    v[ kCotTh ] = " << v[ kCotTh ]
	       << ", v[ kPhi0 ] = " << v[ kPhi0 ]
	       << ", v[ kZ0 ] = " << v[ kZ0 ] << endl
	       << "    v[ kLayer ] = " << v[ kLayer ]
	       << ", v[ kLadder ] = " << v[ kLadder ]
	       << ", v[ kSensor ] = " << v[ kSensor ]
	       << ", v[ kHybrid ] = " << v[ kHybrid ] << endl
	       << "    v[ kLocX ] = " << v[ kLocX ]
	       << ", v[ kLocY ] = " << v[ kLocY ]
	       << ", v[ kLocZ ] = " << v[ kLocZ ] << endl
	       << "    v[ kTLocX ] = " << v[ kTLocX ]
	       << ", v[ kTLocY ] = " << v[ kTLocY ]
	       << ", v[ kTLocZ ] = " << v[ kTLocZ ] << endl
	       << "    v[ kWorldX ] = " << v[ kWorldX ]
	       << ", v[ kWorldY ] = " << v[ kWorldY ]
	       << ", v[ kWorldZ ] = " << v[ kWorldZ ] << endl
	       << "    v[ kTrackX ] = " << v[ kTrackX ]
	       << ", v[ kTrackY ] = " << v[ kTrackY ]
	       << ", v[ kTrackZ ] = " << v[ kTrackZ ]
	       << ", v[ kTrackPhi ] = " << v[ kTrackPhi ] << endl
	       << "    v[ kResidual ] = " << v[ kResidual ] << endl;
	    
	 } while ( surface->advanceToNextCalibratedHit( swimming_track ) );
      } // end loop over surfaces

      for ( vector< HIIntersectionSurface* >::const_iterator surfaces_delete_iter
	       = surfaces.begin();
	    surfaces_delete_iter != surfaces.end();
	    surfaces_delete_iter++ )
      {
	 (* surfaces_delete_iter)->deleteCondition();
	 delete (* surfaces_delete_iter);
      }

   } // end loop over tracks

   report( NOTICE, kFacilityString ) << endl;
   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
FindBug2::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
FindBug2::endRun( Frame& iFrame )         // anal4 equiv.
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
