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

#include "/home/mccann/sialignment/src/DualTrackHelices/DualTrackHelices/DualTrackHelices.h"

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
   , m_target_hits( "target_hits", this, 100 )
   , m_skim_dir( "skim_dir", this, "." )
   , m_device( "device", this, "global" )
   , m_start( "start", this, 1 )
   , m_end( "end", this, 447 )
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

   m_files_right = true;

   if ( strcmp( m_device.value().c_str(), "layer" ) == 0 )
   {
      m_comp_dev = kLayer;
      char num[2];
  
      for ( int n = m_start.value();  n <= 4  &&  n <= m_end.value();  n++ )
      {
	 sprintf( num, "%1d", n );
	 string fullpath( m_skim_dir.value() );
	 fullpath += string( "/layer" );
	 fullpath += string( num );
	 fullpath += string( ".eskim" );

	 if ( ( m_layer[n-1] = fopen( fullpath.c_str(), "wb" ) ) == NULL )
	 {
	    report( EMERGENCY, kFacilityString )
	       << "Couldn't open layer file number " << n
	       << " which is called " << fullpath << endl;
	    m_files_right = false;
	 }

	 m_rphi_layer[n-1] = 0;
	 m_z_layer[n-1] = 0;
      }
   }
   else if ( strcmp( m_device.value().c_str(), "ladder" ) == 0 )
   {
      m_comp_dev = kLadder;
      char num[3];
  
      for ( int n = m_start.value();  n <= 61  &&  n <= m_end.value();  n++ )
      {
	 sprintf( num, "%02d", n );
	 string fullpath( m_skim_dir.value() );
	 fullpath += string( "/ladder" );
	 fullpath += string( num );
	 fullpath += string( ".eskim" );

	 if ( ( m_ladder[n-1] = fopen( fullpath.c_str(), "wb" ) ) == NULL )
	 {
	    report( EMERGENCY, kFacilityString )
	       << "Couldn't open ladder file number " << n
	       << " which is called " << fullpath << endl;
	    m_files_right = false;
	 }
	 
	 m_rphi_ladder[n-1] = 0;
	 m_z_ladder[n-1] = 0;
      }
   }
   else if ( strcmp( m_device.value().c_str(), "sensor" ) == 0 )
   {
      m_comp_dev = kSensor;
      char num[4];
  
      for ( int n = m_start.value();  n <= 447  &&  n <= m_end.value();  n++ )
      {
	 sprintf( num, "%03d", n );
	 string fullpath( m_skim_dir.value() );
	 fullpath += string( "/sensor" );
	 fullpath += string( num );
	 fullpath += string( ".eskim" );

	 if ( ( m_sensor[n-1] = fopen( fullpath.c_str(), "wb" ) ) == NULL )
	 {
	    report( EMERGENCY, kFacilityString )
	       << "Couldn't open sensor file number " << n
	       << " which is called " << fullpath << endl;
	    m_files_right = false;
	 }

	 m_rphi_sensor[n-1] = 0;
	 m_z_sensor[n-1] = 0;
      }
   }
   else
   {
      m_comp_dev = kBad;
      report( ERROR, kFacilityString )
	 << "param device must be \"layer\", \"ladder\" or \"sensor\"."
	 << endl;
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

   switch ( m_comp_dev )
   {
      case kLayer:
	 for ( n = m_start.value();  n <= 4  &&  n <= m_end.value();  n++ )
	 {
	    if ( fwrite( &next_in_file, sizeof( NextInFile ), 1,
			 m_layer[n-1] ) != 1 )
	       report( EMERGENCY, kFacilityString )
		  << "File write failed on the last token! Layer number "
		  << n << endl;
	    fclose( m_layer[n-1] );
	    report( NOTICE, kFacilityString )
	       << "Wrote out " << m_rphi_layer[n-1] << " rphi hits and "
	       << m_z_layer[n-1] << " z hits for layer " << n << "." << endl;
	 }
	 break;

      case kLadder:
	 for ( n = m_start.value();  n <= 61  &&  n <= m_end.value();  n++ )
	 {
	    if ( fwrite( &next_in_file, sizeof( NextInFile ), 1,
			 m_ladder[n-1] ) != 1 )
	       report( EMERGENCY, kFacilityString )
		  << "File write failed on the last token! Ladder number "
		  << n << endl;
	    fclose( m_ladder[n-1] );
	    report( NOTICE, kFacilityString )
	       << "Wrote out " << m_rphi_ladder[n-1] << " rphi hits and "
	       << m_z_ladder[n-1] << " z hits for ladder " << n << "." << endl;
	 }
	 break;

      case kSensor:
	 for ( n = m_start.value();  n <= 447  &&  n <= m_end.value();  n++ )
	 {
	    if ( fwrite( &next_in_file, sizeof( NextInFile ), 1,
			 m_sensor[n-1] ) != 1 )
	       report( EMERGENCY, kFacilityString )
		  << "File write failed on the last token! Sensor number "
		  << n << endl;
	    fclose( m_sensor[n-1] );
	    report( NOTICE, kFacilityString )
	       << "Wrote out " << m_rphi_sensor[n-1] << " rphi hits and "
	       << m_z_sensor[n-1] << " z hits for sensor " << n << "." << endl;
	 }
	 break;
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

   if ( m_comp_dev == kBad  ||  m_files_right == false )
      return ActionBase::kFailed;

   FAItem< ASiStore > store;
   extract( iFrame.record( Stream::kSVAlignment ), store );
   assert( store.valid() );
   
   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator nav_iter;
   FATable< NavTrack >::const_iterator nav_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator nav_end = navtracks.end();
   FATable< DualTrackHelices > dualtracks;
   extract( iFrame.record( Stream::kEvent ), dualtracks );
   FATable< DualTrackHelices >::const_iterator dualtracks_iter;
   
   // be sure that we have exactly two good tracks
   if ( navtracks.size() != 2 ) return ActionBase::kFailed;
   if ( ( nav_iter = nav_begin )->muonQuality()->fitAbort() )
      return ActionBase::kFailed;
   if ( abs( nav_iter->muonHelix()->cotTheta() ) > 1.25 )
      return ActionBase::kFailed;
   if ( nav_iter->seedSVRHits()->size() <= 1 ) return ActionBase::kFailed;
   if ( nav_iter->seedSVZHits()->size() <= 1 ) return ActionBase::kFailed;
   if ( ( ++nav_iter )->muonQuality()->fitAbort() )
      return ActionBase::kFailed;
   if ( abs( nav_iter->muonHelix()->cotTheta() ) > 1.25 )
      return ActionBase::kFailed;
   if ( nav_iter->seedSVRHits()->size() <= 1 ) return ActionBase::kFailed;
   if ( nav_iter->seedSVZHits()->size() <= 1 ) return ActionBase::kFailed;
   
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
   
   for ( nav_iter = nav_begin;  nav_iter != nav_end;  nav_iter++ )
   {
      dualtracks_iter = dualtracks.find( nav_iter->identifier() );

      // Toggle comments for single/dual track fits
//        HIHelix copy_helix( (* nav_iter->muonHelix() ),
//  			  DBCandidate::mass(
//  			     DBCandidate::Hypo( DBCandidate::kMuon ) ) );
      HIHelix copy_helix( dualtracks_iter->helix() );

      report( DEBUG, kFacilityString )
	 << "Track has: curv = " << copy_helix.curvature()
	 << " cotTheta = " << copy_helix.cotTheta()
	 << " d0 = " << copy_helix.d0()
	 << " z0 = " << copy_helix.z0()
	 << " phi0 = " << copy_helix.phi0() << endl;

      DABoolean wrote_track[447];
      for ( uint i = 0;  i < 447;  i++ )
	 wrote_track[i] = false;

      const NavTrack::SVRHitTable* nav_rphi_hits = nav_iter->seedSVRHits();
      NavTrack::SVRHitTable::const_iterator nav_rphi_hits_iter;
      NavTrack::SVRHitTable::const_iterator nav_rphi_hits_begin
	 = nav_rphi_hits->begin();
      NavTrack::SVRHitTable::const_iterator nav_rphi_hits_end
	 = nav_rphi_hits->end();

      for ( nav_rphi_hits_iter = nav_rphi_hits_begin;
	    nav_rphi_hits_iter != nav_rphi_hits_end;
	    nav_rphi_hits_iter++ )
      {
	 CalibratedSVRphiHit copy_hit( **nav_rphi_hits_iter );

	 int layer = copy_hit.layer();
	 if ( layer == 1  ||  layer == 2 )
	    layer = store->clamshellForHybrid( copy_hit.hybrid() );
	 int ladder = store->ladderForSensor( copy_hit.sensor() );
	 int sensor = copy_hit.sensor();
	       
	 report( DEBUG, kFacilityString )
	    << "(" << layer << ", " << ladder << ", " << sensor << ") ";

	 switch ( m_comp_dev )
	 {
	    case kLayer:
	       if ( layer < m_start.value()  ||  layer > m_end.value() )
		  continue;

	       if ( m_rphi_layer[ layer-1 ] > m_target_hits.value()  &&
		    m_z_layer[ layer-1 ] > m_target_hits.value() )
		  continue;

	       if ( ! wrote_track[ layer-1 ] )
	       {
		  if ( ! write_track( &copy_helix, m_layer[ layer-1 ] ) )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "Error trying to write track to layer file number "
			<< layer << endl;
		     m_files_right = false;
		     return ActionBase::kFailed;
		  }
		  wrote_track[ layer-1 ] = true;
	       }

	       if ( ! write_rphi_hit( &copy_hit, m_layer[ layer-1 ] ) )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Error trying to write rphi hit to layer file number "
		     << layer << endl;
		  m_files_right = false;
		  return ActionBase::kFailed;
	       }

	       m_rphi_layer[ layer-1 ]++;

	       break;
	    case kLadder:
	       if ( ladder < m_start.value()  ||  ladder > m_end.value() )
		  continue;

	       if ( m_rphi_ladder[ ladder-1 ] > m_target_hits.value()  &&
		    m_z_ladder[ ladder-1 ] > m_target_hits.value() )
		  continue;

	       if ( ! wrote_track[ ladder-1 ] )
	       {
		  if ( ! write_track( &copy_helix, m_ladder[ ladder-1 ] ) )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "Error trying to write track to ladder file number "
			<< ladder << endl;
		     m_files_right = false;
		     return ActionBase::kFailed;
		  }
		  wrote_track[ ladder-1 ] = true;
	       }

	       if ( ! write_rphi_hit( &copy_hit, m_ladder[ ladder-1 ] ) )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Error trying to write rphi hit to ladder file number "
		     << ladder << endl;
		  m_files_right = false;
		  return ActionBase::kFailed;
	       }

	       m_rphi_ladder[ ladder-1 ]++;

	       break;
	    case kSensor:
	       if ( sensor < m_start.value()  ||  sensor > m_end.value() )
		  continue;

	       if ( m_rphi_sensor[ sensor-1 ] > m_target_hits.value()  &&
		    m_z_sensor[ sensor-1 ] > m_target_hits.value() )
		  continue;

	       if ( ! wrote_track[ sensor-1 ] )
	       {
		  if ( ! write_track( &copy_helix, m_sensor[ sensor-1 ] ) )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "Error trying to write track to sensor file number "
			<< sensor << endl;
		     m_files_right = false;
		     return ActionBase::kFailed;
		  }
		  wrote_track[ sensor-1 ] = true;
	       }

	       if ( ! write_rphi_hit( &copy_hit, m_sensor[ sensor-1 ] ) )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Error trying to write rphi hit to sensor file number "
		     << sensor << endl;
		  m_files_right = false;
		  return ActionBase::kFailed;
	       }

	       m_rphi_sensor[ sensor-1 ]++;

	       break;
	 } // end switch device
      } // end foreach rphi hit
      report( DEBUG, kFacilityString ) << endl;

      const NavTrack::SVZHitTable* nav_z_hits = nav_iter->seedSVZHits();
      NavTrack::SVZHitTable::const_iterator nav_z_hits_iter;
      NavTrack::SVZHitTable::const_iterator nav_z_hits_begin
	 = nav_z_hits->begin();
      NavTrack::SVZHitTable::const_iterator nav_z_hits_end
	 = nav_z_hits->end();

      for ( nav_z_hits_iter = nav_z_hits_begin;
	    nav_z_hits_iter != nav_z_hits_end;
	    nav_z_hits_iter++ )
      {
	 CalibratedSVZHit copy_hit( **nav_z_hits_iter );

	 int layer = copy_hit.layer();
	 if ( layer == 1  ||  layer == 2 )
	    layer = store->clamshellForHybrid( copy_hit.hybrid() );
	 int ladder = store->ladderForSensor( copy_hit.sensor() );
	 int sensor = copy_hit.sensor();
	       
	 report( DEBUG, kFacilityString )
	    << "(" << layer << ", " << ladder << ", " << sensor << ") ";

	 switch ( m_comp_dev )
	 {
	    case kLayer:
	       if ( layer < m_start.value()  ||  layer > m_end.value() )
		  continue;

	       if ( m_rphi_layer[ layer-1 ] > m_target_hits.value()  &&
		    m_z_layer[ layer-1 ] > m_target_hits.value() )
		  continue;

	       if ( ! wrote_track[ layer-1 ] )
	       {
		  if ( ! write_track( &copy_helix, m_layer[ layer-1 ] ) )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "Error trying to write track to layer file number "
			<< layer << endl;
		     m_files_right = false;
		     return ActionBase::kFailed;
		  }
		  wrote_track[ layer-1 ] = true;
	       }

	       if ( ! write_z_hit( &copy_hit, m_layer[ layer-1 ] ) )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Error trying to write z hit to layer file number "
		     << layer << endl;
		  m_files_right = false;
		  return ActionBase::kFailed;
	       }

	       m_z_layer[ layer-1 ]++;

	       break;
	    case kLadder:
	       if ( ladder < m_start.value()  ||  ladder > m_end.value() )
		  continue;

	       if ( m_z_ladder[ ladder-1 ] > m_target_hits.value()  &&
		    m_z_ladder[ ladder-1 ] > m_target_hits.value() )
		  continue;

	       if ( ! wrote_track[ ladder-1 ] )
	       {
		  if ( ! write_track( &copy_helix, m_ladder[ ladder-1 ] ) )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "Error trying to write track to ladder file number "
			<< ladder << endl;
		     m_files_right = false;
		     return ActionBase::kFailed;
		  }
		  wrote_track[ ladder-1 ] = true;
	       }

	       if ( ! write_z_hit( &copy_hit, m_ladder[ ladder-1 ] ) )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Error trying to write z hit to ladder file number "
		     << ladder << endl;
		  m_files_right = false;
		  return ActionBase::kFailed;
	       }

	       m_z_ladder[ ladder-1 ]++;

	       break;
	    case kSensor:
	       if ( sensor < m_start.value()  ||  sensor > m_end.value() )
		  continue;

	       if ( m_z_sensor[ sensor-1 ] > m_target_hits.value()  &&
		    m_z_sensor[ sensor-1 ] > m_target_hits.value() )
		  continue;

	       if ( ! wrote_track[ sensor-1 ] )
	       {
		  if ( ! write_track( &copy_helix, m_sensor[ sensor-1 ] ) )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "Error trying to write track to sensor file number "
			<< sensor << endl;
		     m_files_right = false;
		     return ActionBase::kFailed;
		  }
		  wrote_track[ sensor-1 ] = true;
	       }

	       if ( ! write_z_hit( &copy_hit, m_sensor[ sensor-1 ] ) )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Error trying to write z hit to sensor file number "
		     << sensor << endl;
		  m_files_right = false;
		  return ActionBase::kFailed;
	       }

	       m_z_sensor[ sensor-1 ]++;

	       break;
	 } // end switch device
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

   if ( fwrite( helixPtr, sizeof( HIHelix ), 1, filePtr ) != 1 )
      return false;

   return true;
}

DABoolean
EnderSkim::write_rphi_hit( CalibratedSVRphiHit* hitPtr, FILE* filePtr )
{
   NextInFile next_in_file = kSVRphiHit;

   if ( fwrite( &next_in_file, sizeof( NextInFile ), 1, filePtr ) != 1 )
      return false;

   if ( fwrite( hitPtr, sizeof( CalibratedSVRphiHit ), 1, filePtr ) != 1 )
      return false;

   return true;
}

DABoolean
EnderSkim::write_z_hit( CalibratedSVZHit* hitPtr, FILE* filePtr )
{
   NextInFile next_in_file = kSVZHit;

   if ( fwrite( &next_in_file, sizeof( NextInFile ), 1, filePtr ) != 1 )
      return false;

   if ( fwrite( hitPtr, sizeof( CalibratedSVZHit ), 1, filePtr ) != 1 )
      return false;

   return true;
}

