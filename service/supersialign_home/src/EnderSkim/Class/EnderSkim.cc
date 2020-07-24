// -*- C++ -*-
//
// Package:     EnderSkim
// Module:      EnderSkim
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Mon Sep 17 15:42:10 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "EnderSkim/EnderSkim.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

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
static const char* const kFacilityString = "Processor.EnderSkim" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.21 2001/09/01 22:44:37 llh14 Exp $";
static const char* const kTagString = "$Name: v06_00_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
EnderSkim::EnderSkim( void )               // anal1
   : Processor( "EnderSkim" )
   , m_skim_dir( "skim_dir", this, "." )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &EnderSkim::event,    Stream::kEvent );
   //bind( &EnderSkim::beginRun, Stream::kBeginRun );
   //bind( &EnderSkim::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

EnderSkim::~EnderSkim()                    // anal5
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
EnderSkim::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

   char num[3];
  
   int n;
   for ( n = 1;  n <= 61;  n++ ) {
      sprintf( num, "%02d", n );
      string fullpath( m_skim_dir.value() );
      fullpath += string( "/ladder" );
      fullpath += string( num );
      fullpath += string( ".eskim" );

      if ( ( m_ladder[n-1] = fopen( fullpath.c_str(), "wb" ) ) == NULL ) {
	 report( EMERGENCY, kFacilityString )
	    << "Couldn't open ladder file number " << n
	    << " which is called " << fullpath << endl;
	 assert( 0 );
      }
   }
	 
   for ( n = 1;  n <= 61;  n++ ) {
      sprintf( num, "%02d", n );
      string fullpath( m_skim_dir.value() );
      fullpath += string( "/ladder_noz" );
      fullpath += string( num );
      fullpath += string( ".eskim" );

      if ( ( m_ladder_noz[n-1] = fopen( fullpath.c_str(), "wb" ) ) == NULL ) {
	 report( EMERGENCY, kFacilityString )
	    << "Couldn't open ladder_noz file number " << n
	    << " which is called " << fullpath << endl;
	 assert( 0 );
      }
	 
      m_tracks = 0;
      m_rphi_ladder[n-1] = 0;
      m_z_ladder[n-1] = 0;
   }
}

// -------------------- terminate method ----------------------------
void
EnderSkim::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)

   uint n;
   NextInFile next_in_file = kNothing;

   for ( n = 1;  n <= 61;  n++ ) {
      if ( fwrite( &next_in_file, sizeof( NextInFile ), 1,
		   m_ladder[n-1] ) != 1 )
	 report( EMERGENCY, kFacilityString )
	    << "File write failed on the last token! Ladder number "
	    << n << endl;
      fclose( m_ladder[n-1] );

      if ( fwrite( &next_in_file, sizeof( NextInFile ), 1,
		   m_ladder_noz[n-1] ) != 1 )
	 report( EMERGENCY, kFacilityString )
	    << "File write failed on the last token! Ladder_noz number "
	    << n << endl;
      fclose( m_ladder_noz[n-1] );

      report( NOTICE, kFacilityString )
	 << "Wrote out " << m_rphi_ladder[n-1] << " rphi hits and "
	 << m_z_ladder[n-1] << " z hits for ladder " << n << "." << endl;
   }
}

// ---------------- standard place to book histograms ---------------
void
EnderSkim::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
EnderSkim::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< ASiStore > store;
   extract( iFrame.record( Stream::kSVAlignment ), store );
   assert( store.valid() );
   
   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator nav_iter;
   FATable< NavTrack >::const_iterator nav_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator nav_end = navtracks.end();
   
   // be sure that we have exactly two good tracks
   if ( navtracks.size() != 2 ) return ActionBase::kFailed;
   if ( ( nav_iter = nav_begin )->muonQuality()->fitAbort() )
      return ActionBase::kFailed;
   if ( abs( nav_iter->muonHelix()->cotTheta() ) > 1.265 )
      return ActionBase::kFailed;
   if ( nav_iter->seedSVRHits()->size() < 1 ) return ActionBase::kFailed;
   if ( ( ++nav_iter )->muonQuality()->fitAbort() )
      return ActionBase::kFailed;
   if ( abs( nav_iter->muonHelix()->cotTheta() ) > 1.265 )
      return ActionBase::kFailed;
   if ( nav_iter->seedSVRHits()->size() < 1 ) return ActionBase::kFailed;
   
   // with no final-state radiation (well, very little... this is
   // one of those infrared divergences, isn't it?
   HepVector3D event_momentum =
      ( ( nav_iter = nav_begin )->muonFit()->momentum() +
	( ++nav_iter )->muonFit()->momentum() );
   double xdiff2 = sqr( event_momentum.x() + 0.0261 );
   double ydiff2 = sqr( event_momentum.y() + 0.0003 );
   if ( ( xdiff2 + ydiff2 ) > 0.300 ) return ActionBase::kFailed;
//     if ( abs( ( nav_iter = nav_begin )->muonHelix()->curvature() +
//  	     ( ++nav_iter )->muonHelix()->curvature()             )
//  	> 4E-3 ) return ActionBase::kFailed;
   
   // This is getting a little ridiculous...
   if ( ( ( nav_iter = nav_begin )->muonHelix()->curvature() ) *
	( ( ++ nav_iter )->muonHelix()->curvature() ) > 0. )
      return ActionBase::kFailed;
   
   report( DEBUG, kFacilityString ) << "this is a good event." << endl;
   
   FATable< DualTrackHelices > dualtracks;
   extract( iFrame.record( Stream::kEvent ), dualtracks );
   FATable< DualTrackHelices >::const_iterator dualtracks_iter;

   for ( nav_iter = nav_begin;  nav_iter != nav_end;  nav_iter++ )
   {
      dualtracks_iter = dualtracks.find( nav_iter->identifier() );

      // Toggle comments for single/dual track fits
//        HIHelix copy_helix( (* nav_iter->muonHelix() ),
//  			  DBCandidate::mass(
//  			     DBCandidate::Hypo( DBCandidate::kMuon ) ) );
      HIHelix copy_helix( dualtracks_iter->helix() );

      m_tracks++;

      report( DEBUG, kFacilityString )
	 << "Track has: curv = " << copy_helix.curvature()
	 << " cotTheta = " << copy_helix.cotTheta()
	 << " d0 = " << copy_helix.d0()
	 << " z0 = " << copy_helix.z0()
	 << " phi0 = " << copy_helix.phi0() << endl;

      DABoolean wrote_track[61];
      for ( uint i = 0;  i < 61;  i++ )
	 wrote_track[i] = false;

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

      for ( nav_rphi_hits_iter = nav_rphi_hits_begin;
	    nav_rphi_hits_iter != nav_rphi_hits_end;
	    nav_rphi_hits_iter++ )
      {
	 CalibratedSVRphiHit copy_hit( **nav_rphi_hits_iter );

	 int layer = copy_hit.layer();
	 int ladder = store->ladderForSensor( copy_hit.sensor() );
	       
	 report( DEBUG, kFacilityString )
	    << "(" << layer << ", " << ladder << ") ";

	 if ( ! wrote_track[ ladder-1 ] ) {
	    if ( ! write_track( &copy_helix, m_ladder[ ladder-1 ] ) ) {
	       report( EMERGENCY, kFacilityString )
		  << "Error trying to write track to ladder file number "
		  << ladder << endl;
	       assert( 0 );
	    }

	    if ( ! write_track( &copy_helix, m_ladder_noz[ ladder-1 ] ) ) {
	       report( EMERGENCY, kFacilityString )
		  << "Error trying to write track to ladder_noz file number "
		  << ladder << endl;
	       assert( 0 );
	    }

	    wrote_track[ ladder-1 ] = true;
	 }

	 if ( ! write_rphi_hit( &copy_hit, m_ladder[ ladder-1 ] ) ) {
	    report( EMERGENCY, kFacilityString )
	       << "Error trying to write rphi hit to ladder file number "
	       << ladder << endl;
	    assert( 0 );
	 }

	 if ( ! write_rphi_hit( &copy_hit, m_ladder_noz[ ladder-1 ] ) ) {
	    report( EMERGENCY, kFacilityString )
	       << "Error trying to write rphi hit to ladder_noz file number "
	       << ladder << endl;
	    assert( 0 );
	 }

	 m_rphi_ladder[ ladder-1 ]++;

      } // end foreach rphi hit
      report( DEBUG, kFacilityString ) << endl;

      for ( nav_z_hits_iter = nav_z_hits_begin;
	    nav_z_hits_iter != nav_z_hits_end;
	    nav_z_hits_iter++ )
      {
	 CalibratedSVZHit copy_hit( **nav_z_hits_iter );

	 int layer = copy_hit.layer();
	 int ladder = store->ladderForSensor( copy_hit.sensor() );
	       
	 report( DEBUG, kFacilityString )
	    << "(" << layer << ", " << ladder << ") ";

	 if ( ! wrote_track[ ladder-1 ] ) {
	    if ( ! write_track( &copy_helix, m_ladder[ ladder-1 ] ) ) {
	       report( EMERGENCY, kFacilityString )
		  << "Error trying to write track to ladder file number "
		  << ladder << endl;
	       assert( 0 );
	    }
	    wrote_track[ ladder-1 ] = true;
	 }

	 if ( ! write_z_hit( &copy_hit, m_ladder[ ladder-1 ] ) ) {
	    report( EMERGENCY, kFacilityString )
	       << "Error trying to write z hit to ladder file number "
	       << ladder << endl;
	    assert( 0 );
	 }

	 m_z_ladder[ ladder-1 ]++;

      } // end foreach z hit
      report( DEBUG, kFacilityString ) << endl;

   } // end foreach track
   
   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
EnderSkim::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
EnderSkim::endRun( Frame& iFrame )         // anal4 equiv.
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

DABoolean
EnderSkim::write_track( HIHelix* helixPtr, FILE* filePtr )
{
   NextInFile next_in_file = kTrack;

   if ( fwrite( &next_in_file, sizeof( NextInFile ), 1, filePtr ) != 1 )
      return false;

   report( NOTICE, kFacilityString )
      << "track: (" << helixPtr->curvature() << " " << helixPtr->phi0() << " "
      << helixPtr->d0() << " " << helixPtr->cotTheta() << " " << helixPtr->z0() << endl;
   report( NOTICE, kFacilityString )
      << helixPtr->errorMatrix() << endl;   

   double curvature( helixPtr->curvature() );
   double phi0( helixPtr->phi0() );
   double d0( helixPtr->d0() );
   double cotTheta( helixPtr->cotTheta() );
   double z0( helixPtr->z0() );

   DABoolean errorMatrixOK( helixPtr->errorMatrixOK() );
   double mat_cc( helixPtr->errorMatrix()( KTHelix::kCurvature, KTHelix::kCurvature ) );
   double mat_cp( helixPtr->errorMatrix()( KTHelix::kCurvature, KTHelix::kPhi0 ) );
   double mat_cd( helixPtr->errorMatrix()( KTHelix::kCurvature, KTHelix::kD0 ) );
   double mat_ct( helixPtr->errorMatrix()( KTHelix::kCurvature, KTHelix::kCotTheta ) );
   double mat_cz( helixPtr->errorMatrix()( KTHelix::kCurvature, KTHelix::kZ0 ) );
   double mat_pp( helixPtr->errorMatrix()( KTHelix::kPhi0, KTHelix::kPhi0 ) );
   double mat_pd( helixPtr->errorMatrix()( KTHelix::kPhi0, KTHelix::kD0 ) );
   double mat_pt( helixPtr->errorMatrix()( KTHelix::kPhi0, KTHelix::kCotTheta ) );
   double mat_pz( helixPtr->errorMatrix()( KTHelix::kPhi0, KTHelix::kZ0 ) );
   double mat_dd( helixPtr->errorMatrix()( KTHelix::kD0, KTHelix::kD0 ) );
   double mat_dt( helixPtr->errorMatrix()( KTHelix::kD0, KTHelix::kCotTheta ) );
   double mat_dz( helixPtr->errorMatrix()( KTHelix::kD0, KTHelix::kZ0 ) );
   double mat_tt( helixPtr->errorMatrix()( KTHelix::kCotTheta, KTHelix::kCotTheta ) );
   double mat_tz( helixPtr->errorMatrix()( KTHelix::kCotTheta, KTHelix::kZ0 ) );
   double mat_zz( helixPtr->errorMatrix()( KTHelix::kZ0, KTHelix::kZ0 ) );

   if ( fwrite( &curvature, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &phi0, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &d0, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &cotTheta, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &z0, sizeof( double ), 1, filePtr ) != 1 )
      return false;

   if ( fwrite( &errorMatrixOK, sizeof( DABoolean ), 1, filePtr ) != 1 )
      return false;

   if ( fwrite( &mat_cc, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_cp, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_cd, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_ct, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_cz, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_pp, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_pd, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_pt, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_pz, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_dd, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_dt, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_dz, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_tt, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_tz, sizeof( double ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &mat_zz, sizeof( double ), 1, filePtr ) != 1 )
      return false;

   return true;
}

DABoolean
EnderSkim::write_rphi_hit( CalibratedSVRphiHit* hitPtr, FILE* filePtr )
{
   NextInFile next_in_file = kSVRphiHit;

   if ( fwrite( &next_in_file, sizeof( NextInFile ), 1, filePtr ) != 1 )
      return false;

   report( NOTICE, kFacilityString ) << "hit: ";
   hitPtr->printCSVH( report( NOTICE, kFacilityString ) );
   report( NOTICE, kFacilityString ) << endl;

   CalibratedSVRphiHit::Identifier identifier( hitPtr->identifier() );
   SmallCount parent( hitPtr->parent() );
   SmallCount hybrid( hitPtr->hybrid() );
   SmallCount layer( hitPtr->layer() );
   SmallCount sensor( hitPtr->sensor() );
   SmallCount width( hitPtr->width() );
   CalibratedSVRphiHit::PicoCoul charge( hitPtr->charge() );
   CalibratedSVRphiHit::PicoCoul charge_error( hitPtr->charge_error() );
   float coord( hitPtr->coord() );
   float coord_error( hitPtr->coord_error() );
   CalibratedSVRphiHit::QualHit quality( hitPtr->quality() );
   DABoolean saturated( hitPtr->saturated() );

   if ( fwrite( &identifier, sizeof( CalibratedSVRphiHit::Identifier ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &parent, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &hybrid, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &layer, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &sensor, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &width, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &charge, sizeof( CalibratedSVRphiHit::PicoCoul ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &charge_error, sizeof( CalibratedSVRphiHit::PicoCoul ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &coord, sizeof( float ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &coord_error, sizeof( float ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &quality, sizeof( CalibratedSVRphiHit::QualHit ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &saturated, sizeof( DABoolean ), 1, filePtr ) != 1 )
      return false;

   return true;
}

DABoolean
EnderSkim::write_z_hit( CalibratedSVZHit* hitPtr, FILE* filePtr )
{
   NextInFile next_in_file = kSVZHit;

   if ( fwrite( &next_in_file, sizeof( NextInFile ), 1, filePtr ) != 1 )
      return false;

   report( NOTICE, kFacilityString ) << "hit: ";
   hitPtr->printCSVH( report( NOTICE, kFacilityString ) );
   report( NOTICE, kFacilityString ) << endl;

   CalibratedSVZHit::Identifier identifier( hitPtr->identifier() );
   SmallCount parent( hitPtr->parent() );
   SmallCount hybrid( hitPtr->hybrid() );
   SmallCount layer( hitPtr->layer() );
   SmallCount sensor( hitPtr->sensor() );
   SmallCount width( hitPtr->width() );
   CalibratedSVZHit::PicoCoul charge( hitPtr->charge() );
   CalibratedSVZHit::PicoCoul charge_error( hitPtr->charge_error() );
   float coord( hitPtr->coord() );
   float coord_error( hitPtr->coord_error() );
   CalibratedSVZHit::QualHit quality( hitPtr->quality() );
   DABoolean saturated( hitPtr->saturated() );

   if ( fwrite( &identifier, sizeof( CalibratedSVZHit::Identifier ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &parent, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &hybrid, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &layer, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &sensor, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &width, sizeof( SmallCount ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &charge, sizeof( CalibratedSVZHit::PicoCoul ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &charge_error, sizeof( CalibratedSVZHit::PicoCoul ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &coord, sizeof( float ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &coord_error, sizeof( float ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &quality, sizeof( CalibratedSVZHit::QualHit ), 1, filePtr ) != 1 )
      return false;
   if ( fwrite( &saturated, sizeof( DABoolean ), 1, filePtr ) != 1 )
      return false;

   return true;
}

