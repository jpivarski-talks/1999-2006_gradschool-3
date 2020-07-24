// -*- C++ -*-
//
// Package:     <EnderSiAlignmentMod>
// Module:      EnderSiAlignmentMod
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri Sep 14 12:59:14 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "Experiment/report.h"
#include "EnderSiAlignmentMod/EnderSiAlignmentMod.h"
#include "DataHandler/Stream.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "DataHandler/Frame.h"

#include "/home/mccann/sialignment/src/DualTrackHelices/DualTrackHelices/DualTrackHelices.h"

#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HICompositeSurface.h"
#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

// I'm not going to bother with this yet.
// #include "BinaryDelivery/ByteSwapping.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

#include <stdio.h>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "EnderSiAlignmentMod.EnderSiAlignmentMod" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule.cc,v 1.2 2000/12/04 19:11:05 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
EnderSiAlignmentMod::EnderSiAlignmentMod()
   : FrameIteratorModuleBase( "EnderSiAlignmentMod", "This is my module" )
   , m_target_hits( "target_hits", this, 100 )
   , m_file_global( "global_init", this, "unmoved.svalignmentglobal" )
   , m_file_layer(  "layer_init",  this, "unmoved.svalignmentlayer" )
   , m_file_ladder( "ladder_init", this, "unmoved.svalignmentladder" )
   , m_file_sensor( "sensor_init", this, "unmoved.svalignmentsensor" )
   , m_file_output( "file_output", this, "output.log" )
   , m_device( "device", this, "global" )
   , m_number( "number", this, 0 )
   , m_skim_in( "skim_in", this, "" )
   , m_rphi_norm( "rphi_norm", this, 0.000060 )
   , m_z_norm( "z_norm", this, 0.0012 )
   , m_rphi_cut( "rphi_cut", this, 0.001 )
   , m_z_cut( "z_cut", this, 0.003 )
   , m_deltaX_step( "deltaX_step", this, 0.000050 )
   , m_deltaY_step( "deltaY_step", this, 0.000050 )
   , m_deltaZ_step( "deltaZ_step", this, 0.002500 )
   , m_phiX_step( "phiX_step", this, 0.002000 )
   , m_phiY_step( "phiY_step", this, 0.005000 )
   , m_phiZ_step( "phiZ_step", this, 0.001000 )
   , m_deltaX_limit( "deltaX_limit", this, 0.001000 )
   , m_deltaY_limit( "deltaY_limit", this, 0.001000 )
   , m_deltaZ_limit( "deltaZ_limit", this, 0.010000 )
   , m_phiX_limit( "phiX_limit", this, 0.050000 )
   , m_phiY_limit( "phiY_limit", this, 0.050000 )
   , m_phiZ_limit( "phiZ_limit", this, 0.050000 )
   , m_const_global( new DBSVAlignmentGlobal[1], 1 )
   , m_const_layer(  new DBSVAlignmentLayer[4], 4 )
   , m_const_ladder( new DBSVAlignmentLadder[61], 61 )
   , m_const_sensor( new DBSVAlignmentSensor[447], 447 )
   , m_holder_global( &m_const_global )
   , m_holder_layer(  &m_const_layer )
   , m_holder_ladder( &m_const_ladder )
   , m_holder_sensor( &m_const_sensor )
   , m_comp_dev( kGlobal )
   , m_comp_num( 0 )
   , m_output( NULL )
   , m_tracks( NULL )
   , m_svr_hits( NULL )
   , m_svz_hits( NULL )
   , m_svr_lengths( NULL )
   , m_svz_lengths( NULL )
   , m_num_tracks( 0 )
   , m_num_svr_hits( 0 )
   , m_num_svz_hits( 0 )
   , m_begin( NULL )
   , m_minuit_iteration( 0 )
{
   //register the Proxy Factory created by the FIHolder

   registerProxyFactory(
      Stream::kSVAlignment, m_holder_global.makeFactory(), UsageTag() );

   registerProxyFactory(
      Stream::kSVAlignment, m_holder_layer.makeFactory(),  UsageTag() );

   registerProxyFactory(
      Stream::kSVAlignment, m_holder_ladder.makeFactory(), UsageTag() );

   registerProxyFactory(
      Stream::kSVAlignment, m_holder_sensor.makeFactory(), UsageTag() );

   //You must set what streams you which to iterate over
   //  that is, what events should the Frames be stopped on
   iterateOver( Stream::kEvent );

}

// EnderSiAlignmentMod::EnderSiAlignmentMod( const EnderSiAlignmentMod& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

EnderSiAlignmentMod::~EnderSiAlignmentMod()
{
}

//
// assignment operators
//
// const EnderSiAlignmentMod& EnderSiAlignmentMod::operator=( const EnderSiAlignmentMod& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//
void
EnderSiAlignmentMod::iterate( const FIFrameIterator& iBegin,
			      const FIFrameIterator& iEnd )
{
   if( ! read_init_constants() )
      return;

   if ( strcmp( m_device.value().c_str(), "global" ) == 0 )
      m_comp_dev = kGlobal;
   else if ( strcmp( m_device.value().c_str(), "layer" ) == 0 )
      m_comp_dev = kLayer;
   else if ( strcmp( m_device.value().c_str(), "ladder" ) == 0 )
      m_comp_dev = kLadder;
   else if ( strcmp( m_device.value().c_str(), "sensor" ) == 0 )
      m_comp_dev = kSensor;
   else
   {
      report( ERROR, kFacilityString )
	 << "param device must be \"global\", \"layer\", \"ladder\" or \"sensor\"."
	 << endl << "Reset this parameter and try again." << endl;
      return;
   }
   
   switch ( m_comp_dev )
   {
      case kGlobal:
	 if ( m_number.value() != 1 )
	 {
	    report( ERROR, kFacilityString )
	       << "param number can only be one for \"global\" device."
	       << endl;
	    return;
	 }
	 break;
      case kLayer:
	 if ( m_number.value() < 1  ||  m_number.value() > 4 )
	 {
	    report( ERROR, kFacilityString )
	       << "param number must be in range [1,4] for \"layer\" device."
	       << endl;
	    return;
	 }
	 break;
      case kLadder:
	 if ( m_number.value() < 1  ||  m_number.value() > 61 )
	 {
	    report( ERROR, kFacilityString )
	       << "param number must be in range [1,61] for \"ladder\" device."
	       << endl;
	    return;
	 }
	 break;
      case kSensor:
	 if ( m_number.value() < 1  ||  m_number.value() > 447 )
	 {
	    report( ERROR, kFacilityString )
	       << "param number must be in range [1,447] for \"sensor\" device."
	       << endl;
	    return;
	 }
	 break;
   }
   m_comp_num = m_number.value() - 1;

   addInitialParameter( "deltaX", get_deltaX(),	m_deltaX_step.value(),
			get_deltaX() - m_deltaX_limit.value(),
			get_deltaX() + m_deltaX_limit.value() );
   addInitialParameter( "deltaY", get_deltaY(),	m_deltaY_step.value(),
			get_deltaY() - m_deltaY_limit.value(),
			get_deltaY() + m_deltaY_limit.value() );
   addInitialParameter( "deltaZ", get_deltaZ(),	m_deltaZ_step.value(),
			get_deltaZ() - m_deltaZ_limit.value(),
			get_deltaZ() + m_deltaZ_limit.value() );
   addInitialParameter( "phiX", get_phiX(), m_phiX_step.value(),
			get_phiX() - m_phiX_limit.value(),
			get_phiX() + m_phiX_limit.value() );
   addInitialParameter( "phiY", get_phiY(), m_phiY_step.value(),
			get_phiY() - m_phiY_limit.value(),
			get_phiY() + m_phiY_limit.value() );
   addInitialParameter( "phiZ", get_phiZ(), m_phiZ_step.value(),
			get_phiZ() - m_phiZ_limit.value(),
			get_phiZ() + m_phiZ_limit.value() );

   MinuitInterface* mi = MinuitInterface::instance();
   mi->loadFcn( *this );
   mi->setDiagLevel( MinuitInterface::kMax );

//     mi->fixParameter( kDeltaX );
//     mi->fixParameter( kDeltaY );
//     mi->fixParameter( kDeltaZ );
//     mi->fixParameter( kPhiX );
//     mi->fixParameter( kPhiY );
//     mi->fixParameter( kPhiZ );

   init_hits();

   if ( strcmp( m_skim_in.value().c_str(), "" ) == 0 )
      get_hits( iBegin, iEnd );
   else
      if ( ! read_hits() ) return;

   m_begin = &iBegin;

   report( NOTICE, kFacilityString )
      << "Logging progress to file "
      << m_file_output.value().c_str() << endl;
   m_output = fopen( m_file_output.value().c_str(), "w" );
   if ( m_output == NULL )
   {
      report( EMERGENCY, kFacilityString )
	 << "Can't write to output file "
	 << m_file_output.value().c_str() << "!" << endl;
      return;
   }
      
   fprintf( m_output, "ABS:   %3s | %12s %12s %12s %12s %12s %12s | %12s | %12s %12s, %12s %12s\n",
	    "itr", "deltaX", "deltaY", "deltaZ", "phiX", "phiY", "phiZ",
	    "chisq", "rphi mean", "rphi rms", "z mean", "z rms" );
   fprintf( m_output, "ABS:   =========================================================================================================================================================\n" );
   
   fprintf( m_output, "DIFF:  %3s | %12s %12s %12s %12s %12s %12s | %12s | %12s %12s, %12s %12s\n",
	    "itr", "deltaX", "deltaY", "deltaZ", "phiX", "phiY", "phiZ",
	    "chisq", "rphi mean", "rphi rms", "z mean", "z rms" );
   fprintf( m_output, "DIFF:  =========================================================================================================================================================\n" );
   
   fprintf( m_output, "MOVE:  %3s | %12s %12s %12s %12s %12s %12s | %12s | %12s %12s, %12s %12s\n",
	    "itr", "deltaX", "deltaY", "deltaZ", "phiX", "phiY", "phiZ",
	    "chisq", "rphi mean", "rphi rms", "z mean", "z rms" );
   fprintf( m_output, "MOVE:  =========================================================================================================================================================\n" );
   
   fflush( m_output );

   JobControl* jc = JobControl::instance();
   HIHistoManager* hm = jc->histogramManagerP();

   m_rphi_resid_before = hm->histogram(
      100, "rphi residual before", 100, 0. - m_rphi_cut.value(), m_rphi_cut.value() );
   m_z_resid_before = hm->histogram(
      200, "z residual before", 100, 0. - m_z_cut.value(), m_z_cut.value() );

   m_rphi_resid_after = hm->histogram(
      150, "rphi residual after", 100, 0. - m_rphi_cut.value(), m_rphi_cut.value() );
   m_z_resid_after = hm->histogram(
      250, "z residual after", 100, 0. - m_z_cut.value(), m_z_cut.value() );

   double values[ kNumParams ]
      = { get_deltaX(), get_deltaY(), get_deltaZ(),
	  get_phiX(), get_phiY(), get_phiZ() };
   m_plot_before = true;
   m_plot_after = false;
   iterate( values );
   m_plot_before = false;
   m_plot_after = false;

   m_minuit_iteration = 0;
   mi->runMigrad();

   values[0] = get_deltaX();  values[1] = get_deltaY();  values[2] = get_deltaZ();
   values[3] = get_phiX();    values[4] = get_phiY();    values[5] = get_phiZ();
   m_plot_before = false;
   m_plot_after = true;
   iterate( values );
   m_plot_before = false;
   m_plot_after = false;
   
   fprintf( m_output, "CONST: FIN | %d %g %g %g %g %g %g\n",
	    m_comp_num + 1,
	    get_deltaX(),
	    get_deltaY(),
	    get_deltaZ(),
	    get_phiX(),
	    get_phiY(),
	    get_phiZ() );
   
   fflush( m_output );
   
   fprintf( m_output, "================================================================================================================================================================\n\n" );
   fprintf( m_output, "Final values of parameters:\n\n" );
   fprintf( m_output, "    DeltaX = %12.9f +/- %12.9f\n",
	    mi->parameter( kDeltaX ).value(),
	    mi->parameter( kDeltaX ).parabolicErr() );
   fprintf( m_output, "    DeltaY = %12.9f +/- %12.9f\n",
	    mi->parameter( kDeltaY ).value(),
	    mi->parameter( kDeltaY ).parabolicErr() );
   fprintf( m_output, "    DeltaZ = %12.9f +/- %12.9f\n",
	    mi->parameter( kDeltaZ ).value(),
	    mi->parameter( kDeltaZ ).parabolicErr() );
   fprintf( m_output, "    PhiX = %12.9f +/- %12.9f\n",
	    mi->parameter( kPhiX ).value(),
	    mi->parameter( kPhiX ).parabolicErr() );
   fprintf( m_output, "    PhiY = %12.9f +/- %12.9f\n",
	    mi->parameter( kPhiY ).value(),
	    mi->parameter( kPhiY ).parabolicErr() );
   fprintf( m_output, "    PhiZ = %12.9f +/- %12.9f\n\n",
	    mi->parameter( kPhiZ ).value(),
	    mi->parameter( kPhiZ ).parabolicErr() );
   fprintf( m_output, "================================================================================================================================================================\n" );
      
   fflush( m_output );

}

DABoolean
EnderSiAlignmentMod::read_init_constants()
{
   ifstream global_file( m_file_global.value().c_str() );
   if ( ! m_const_global.readFromStream( global_file ) )
   {
      report( ERROR, kFacilityString )
	 << "Couldn't read from file " << m_file_global.value().c_str()
	 << "! Reset and try again." << endl;
      return false;
   }

   ifstream layer_file( m_file_layer.value().c_str() );
   if ( ! m_const_layer.readFromStream( layer_file ) )
   {
      report( ERROR, kFacilityString )
	 << "Couldn't read from file " << m_file_layer.value().c_str()
	 << "! Reset and try again." << endl;
      return false;
   }

   ifstream ladder_file( m_file_ladder.value().c_str() );
   if ( ! m_const_ladder.readFromStream( ladder_file ) )
   {
      report( ERROR, kFacilityString )
	 << "Couldn't read from file " << m_file_ladder.value().c_str()
	 << "! Reset and try again." << endl;
      return false;
   }

   ifstream sensor_file( m_file_sensor.value().c_str() );
   if ( ! m_const_sensor.readFromStream( sensor_file ) )
   {
      report( ERROR, kFacilityString )
	 << "Couldn't read from file " << m_file_sensor.value().c_str()
	 << "! Reset and try again." << endl;
      return false;
   }

   return true;
}

void
EnderSiAlignmentMod::init_hits()
{
   report( NOTICE, kFacilityString )
      << "Testing to see if we have enough memory to allocate "
      << m_target_hits.value() << " hits." << endl;

   // I'm never going to run this twice, so I don't care about trying
   // to delete formerly-existant structures

   const uint target_hits = m_target_hits.value();
   m_tracks = new HelixPtr[ 2 * target_hits ];
   m_svr_hits = new SVRPtrPtr[ 2 * target_hits ];
   m_svz_hits = new SVZPtrPtr[ 2 * target_hits ];
   m_svr_lengths = new uint[ 2 * target_hits ];
   m_svz_lengths = new uint[ 2 * target_hits ];

//     // when they come from a file, I actually don't know how many
//     // tracks I'll need to get all the rphi hits I need. This is a hack
//     // for ladder and sensor only. (which have small target_hits)
//     const uint target_hits = m_target_hits.value();
//     m_tracks = new HelixPtr[ 250 * target_hits ];
//     m_svr_hits = new SVRPtrPtr[ 250 * target_hits ];
//     m_svz_hits = new SVZPtrPtr[ 250 * target_hits ];
//     m_svr_lengths = new uint[ 250 * target_hits ];
//     m_svz_lengths = new uint[ 250 * target_hits ];

   uint i;
//     for ( i = 0;  i < 2 * target_hits;  i++ )
//        m_tracks[i] = new HIHelix();

//     for ( i = 0;  i < target_hits;  i++ )
//     {
//        m_svr_hits[i] = new SVRPtr;
//        m_svz_hits[i] = new SVZPtr;

//        (* m_svr_hits[i]) = new CalibratedSVRphiHit(
//  	 1, 1, 1, 1, 1, 1, 1.0, 1.0, 1.0, 1.0, 1, true );
//        (* m_svz_hits[i]) = new CalibratedSVZHit(
//  	 1, 1, 1, 1, 1, 1, 1.0, 1.0, 1.0, 1.0, 1, true );
//     }

//     report( NOTICE, kFacilityString )
//        << "Successfully allocated " << m_target_hits.value()
//        << " tracks, svrhits and svzhits!" << endl;

//     for ( i = 0;  i < target_hits;  i++ )
//     {
//        delete (* m_svr_hits[i]);
//        delete (* m_svz_hits[i]);

//        delete m_svr_hits[i];
//        delete m_svz_hits[i];
//     }

   for ( i = 0;  i < 2 * target_hits;  i++ )
   {
//      delete m_tracks[i];
      m_tracks[i] = NULL;

      m_svr_hits[i] = NULL;
      m_svz_hits[i] = NULL;

      m_svr_lengths[i] = 0;
      m_svz_lengths[i] = 0;
   }

   m_num_tracks = 0;
   m_num_svr_hits = 0;
   m_num_svz_hits = 0;

   report( NOTICE, kFacilityString )
      << "Successfully deleted them. Ok, let's get to work." << endl;
}

void
EnderSiAlignmentMod::get_hits( const FIFrameIterator& iBegin,
			       const FIFrameIterator& iEnd )
{
   uint comp_num_one = m_comp_num + 1;

   report( NOTICE, kFacilityString )
      << "Starting to get " << m_target_hits.value() << " ";
   switch ( m_comp_dev )
   {
      case kGlobal:
	 report( NOTICE, kFacilityString ) << "hits." << endl;
	 break;
      case kLayer:
	 report( NOTICE, kFacilityString )
	    << "layer " << comp_num_one << " hits." << endl;
	 break;
      case kLadder:
	 report( NOTICE, kFacilityString )
	    << "ladder " << comp_num_one << " hits." << endl;
	 break;
      case kSensor:
	 report( NOTICE, kFacilityString )
	    << "sensor " << comp_num_one << " hits." << endl;
	 break;
   }

   for ( FIFrameIterator itFrame = iBegin;
	 ( ( m_num_svr_hits < m_target_hits.value()  ||
	     m_num_svz_hits < m_target_hits.value()     )  &&
	   itFrame != iEnd );
	 ++itFrame )
   {
      report( INFO, kFacilityString )
	 << "So far, found " << m_num_svr_hits
	 << " rphi hits and " << m_num_svz_hits << " z hits in "
	 << m_num_tracks << " tracks." << endl;

      report( DEBUG, kFacilityString ) << "Starting event." << endl;

      FAItem< ASiStore > store;
      extract( itFrame->record( Stream::kSVAlignment ), store );
      assert( store.valid() );

      report( DEBUG, kFacilityString ) << "Got sistore." << endl;

      FATable< NavTrack > navtracks;
      extract( itFrame->record( Stream::kEvent ), navtracks );
      FATable< NavTrack >::const_iterator nav_iter;
      FATable< NavTrack >::const_iterator nav_begin = navtracks.begin();
      FATable< NavTrack >::const_iterator nav_end = navtracks.end();

      report( DEBUG, kFacilityString )
	 << "Got navtracks (num tracks = " << navtracks.size() << ")." << endl;

      // be sure that we have exactly two good tracks
      if ( navtracks.size() != 2 ) continue;

      report( DEBUG, kFacilityString )
	 << "( nav_iter = nav_begin )->electronQuality()->fitAbort() = "
	 << ( nav_iter = nav_begin )->electronQuality()->fitAbort() << endl
	 << "abs( nav_iter->electronHelix()->cotTheta() ) = "
	 << abs( nav_iter->electronHelix()->cotTheta() ) << endl
	 << "nav_iter->seedSVRHits()->size() = "
	 << nav_iter->seedSVRHits()->size() << endl
	 << "nav_iter->seedSVZHits()->size() = "
	 << nav_iter->seedSVZHits()->size() << endl
	 << "abs( nav_iter->electronHelix()->d0() ) = "
	 << abs( nav_iter->electronHelix()->d0() ) << endl
	 << "abs( nav_iter->electronHelix()->z0() ) = "
	 << abs( nav_iter->electronHelix()->z0() ) << endl
	 << "abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) = "
	 << abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) << endl
	 << endl
	 << "( ++nav_iter )->electronQuality()->fitAbort() = "
	 << ( ++nav_iter )->electronQuality()->fitAbort() << endl
	 << "abs( nav_iter->electronHelix()->cotTheta() ) = "
	 << abs( nav_iter->electronHelix()->cotTheta() ) << endl
	 << "nav_iter->seedSVRHits()->size() = "
	 << nav_iter->seedSVRHits()->size() << endl
	 << "nav_iter->seedSVZHits()->size() = "
	 << nav_iter->seedSVZHits()->size() << endl
	 << "abs( nav_iter->electronHelix()->d0() ) = "
	 << abs( nav_iter->electronHelix()->d0() ) << endl
	 << "abs( nav_iter->electronHelix()->z0() ) = "
	 << abs( nav_iter->electronHelix()->z0() ) << endl
	 << "abs( nav_iter->electronFit()->momentum().mag() - 4.71671 = "
	 << abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) << endl;

      if ( ( nav_iter = nav_begin )->electronQuality()->fitAbort() ) continue;
      if ( abs( nav_iter->electronHelix()->cotTheta() ) > 1.25 ) continue;
      if ( nav_iter->seedSVRHits()->size() < 1  &&  nav_iter->seedSVZHits()->size() < 1 ) continue;
      if ( abs( nav_iter->electronHelix()->d0() ) > 0.0032 ) continue;
      if ( abs( nav_iter->electronHelix()->z0() ) > 0.04 ) continue;
//      if ( abs( nav_iter->electronFit()->momentum().mag() - 5.25 ) > 1.25 ) continue;
      if ( abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) > 1.25 ) continue;

      report( DEBUG, kFacilityString ) << "First track is fine." << endl;

      if ( ( ++nav_iter )->electronQuality()->fitAbort() ) continue;
      if ( abs( nav_iter->electronHelix()->cotTheta() ) > 1.25 ) continue;
      if ( nav_iter->seedSVRHits()->size() < 1  &&  nav_iter->seedSVZHits()->size() < 1 ) continue;
      if ( abs( nav_iter->electronHelix()->d0() ) > 0.0032 ) continue;
      if ( abs( nav_iter->electronHelix()->z0() ) > 0.04 ) continue;
//      if ( abs( nav_iter->electronFit()->momentum().mag() - 5.25 ) > 1.25 ) continue;
      if ( abs( nav_iter->electronFit()->momentum().mag() - 4.71671 ) > 1.25 ) continue;

      report( DEBUG, kFacilityString ) << "Second track is fine." << endl;

      // with no final-state radiation (well, very little... this is
      // one of those infrared divergences, isn't it?
      HepVector3D event_momentum =
	 ( ( nav_iter = nav_begin )->electronFit()->momentum() +
	   ( ++nav_iter )->electronFit()->momentum() );
      double xdiff2 = sqr( event_momentum.x() + 0.0261 );
      double ydiff2 = sqr( event_momentum.y() + 0.0003 );
      if ( ( xdiff2 + ydiff2 ) > 0.300 ) continue;
//        if ( abs( ( nav_iter = nav_begin )->electronHelix()->curvature() +
//  		( ++nav_iter )->electronHelix()->curvature()             )
//  	   > 4E-3 ) continue;

      // This is getting a little rediculous...
      if ( ( ( nav_iter = nav_begin )->electronHelix()->curvature() ) *
	   ( ( ++ nav_iter )->electronHelix()->curvature() ) > 0. ) continue;

      FATable< DualTrackHelices > dualtracks;
      extract( itFrame->record( Stream::kEvent ), dualtracks );
      FATable< DualTrackHelices >::const_iterator dualtracks_iter;

      report( DEBUG, kFacilityString ) << "Two tracks: loaded dualtracks." << endl;

      for ( nav_iter = nav_begin;  nav_iter != nav_end;  nav_iter++ )
      {
	 report( DEBUG, kFacilityString ) << "Starting track." << endl;

	 const NavTrack::SVRHitTable* nav_rphi_hits = nav_iter->seedSVRHits();
	 NavTrack::SVRHitTable::const_iterator nav_rphi_hits_iter;
	 NavTrack::SVRHitTable::const_iterator nav_rphi_hits_begin
	    = nav_rphi_hits->begin();
	 NavTrack::SVRHitTable::const_iterator nav_rphi_hits_end
	    = nav_rphi_hits->end();

	 uint relevant_rphi = 0;
	 for ( nav_rphi_hits_iter = nav_rphi_hits_begin;
	       nav_rphi_hits_iter != nav_rphi_hits_end;
	       nav_rphi_hits_iter++ )
	    switch ( m_comp_dev )
	    {
	       case kGlobal:
		  relevant_rphi++;
		  break;
	       case kLayer:
	       {
		  int layer = (* nav_rphi_hits_iter)->layer();
		  if ( layer == 1  ||  layer == 2 )
		     layer = store->clamshellForHybrid(
			(* nav_rphi_hits_iter)->hybrid() );
		  if ( layer == comp_num_one ) relevant_rphi++;
	       }
		  break;
	       case kLadder:
		  if ( store->ladderForSensor(
		     (* nav_rphi_hits_iter)->sensor() ) == comp_num_one )
		     relevant_rphi++;
		  break;
	       case kSensor:
		  if ( (* nav_rphi_hits_iter)->sensor() == comp_num_one )
		     relevant_rphi++;
		  break;
	    }

	 const NavTrack::SVZHitTable* nav_z_hits = nav_iter->seedSVZHits();
	 NavTrack::SVZHitTable::const_iterator nav_z_hits_iter;
	 NavTrack::SVZHitTable::const_iterator nav_z_hits_begin
	    = nav_z_hits->begin();
	 NavTrack::SVZHitTable::const_iterator nav_z_hits_end
	    = nav_z_hits->end();

	 uint relevant_z = 0;
	 for ( nav_z_hits_iter = nav_z_hits_begin;
	       nav_z_hits_iter != nav_z_hits_end;
	       nav_z_hits_iter++ )
	    switch ( m_comp_dev )
	    {
	       case kGlobal:
		  relevant_z++;
		  break;
	       case kLayer:
	       {
		  int layer = (* nav_z_hits_iter)->layer();
		  if ( layer == 1  ||  layer == 2 )
		     layer = store->clamshellForHybrid(
			(* nav_z_hits_iter)->hybrid() );
		  if ( layer == comp_num_one ) relevant_z++;
	       }
		  break;
	       case kLadder:
		  if ( store->ladderForSensor(
		     (* nav_z_hits_iter)->sensor() ) == comp_num_one )
		     relevant_z++;
		  break;
	       case kSensor:
		  if ( (* nav_z_hits_iter)->sensor() == comp_num_one )
		     relevant_z++;
		  break;
	    }

	 report( DEBUG, kFacilityString )
	    << "There are " << relevant_rphi << " (relevant) rphi hits and "
	    << relevant_z << " (relevant) z hits." << endl;

	 if ( relevant_rphi == 0  &&  relevant_z == 0 ) continue;
	 
	 dualtracks_iter = dualtracks.find( nav_iter->identifier() );
	 
	 // Toggle comments for single/dual track fits
//  	 m_tracks[ m_num_tracks ] = new HIHelix(
//  	    (* nav_iter->electronHelix() ),
//  	    DBCandidate::mass( DBCandidate::Hypo( DBCandidate::kElectron ) ) );
	 m_tracks[ m_num_tracks ] = new HIHelix( dualtracks_iter->helix() );

	 report( DEBUG, kFacilityString )
	    << "Storing the following helix: curv = "
	    << m_tracks[ m_num_tracks ]->curvature()
	    << " cotTheta = " << m_tracks[ m_num_tracks ]->cotTheta()
	    << " d0 = " << m_tracks[ m_num_tracks ]->d0()
	    << " z0 = " << m_tracks[ m_num_tracks ]->z0()
	    << " phi0 = " << m_tracks[ m_num_tracks ]->phi0() << endl;

	 uint which_hit;

	 m_svr_hits[ m_num_tracks ] = new SVRPtr[ relevant_rphi ];
	 m_svr_lengths[ m_num_tracks ] = relevant_rphi;

	 report( DEBUG, kFacilityString ) << "   SVRphi hits: ";
	 which_hit = 0;
	 for ( nav_rphi_hits_iter = nav_rphi_hits_begin;
	       nav_rphi_hits_iter != nav_rphi_hits_end;
	       nav_rphi_hits_iter++ )
	 {
	    if ( which_hit >= relevant_rphi ) break;
	    
	    int layer = (* nav_rphi_hits_iter)->layer();
	    if ( layer == 1  ||  layer == 2 )
	       layer = store->clamshellForHybrid(
		  (* nav_rphi_hits_iter)->hybrid() );
	    
	    if ( m_comp_dev == kLayer  &&  layer != comp_num_one )
	       continue;
	    
	    if ( m_comp_dev == kLadder  &&
		 store->ladderForSensor( (* nav_rphi_hits_iter)->sensor() )
		 != comp_num_one )
	       continue;
	    
	    if ( m_comp_dev == kSensor  &&
		 (* nav_rphi_hits_iter)->sensor() != comp_num_one )
	       continue;
	    
	    m_svr_hits[ m_num_tracks ][ which_hit ]
	       = new CalibratedSVRphiHit( (** nav_rphi_hits_iter ) );
	    report( DEBUG, kFacilityString )
	       << "(" << m_svr_hits[ m_num_tracks ][ which_hit ]->layer()
	       << ", " << store->ladderForSensor(
		  m_svr_hits[ m_num_tracks ][ which_hit ]->sensor() )
	       << ", " << m_svr_hits[ m_num_tracks ][ which_hit ]->sensor()
	       << ") ";
	    which_hit++;
	    m_num_svr_hits++;
	 } // end of svr hit loop

	 assert( which_hit == relevant_rphi );
	 report( DEBUG, kFacilityString ) << endl;

	 m_svz_hits[ m_num_tracks ] = new SVZPtr[ relevant_z ];
	 m_svz_lengths[ m_num_tracks ] = relevant_z;

	 report( DEBUG, kFacilityString ) << "   SVZ hits: ";
	 which_hit = 0;
	 for ( nav_z_hits_iter = nav_z_hits_begin;
	       nav_z_hits_iter != nav_z_hits_end;
	       nav_z_hits_iter++ )
	 {
	    if ( which_hit >= relevant_z ) break;
	    
	    int layer = (* nav_z_hits_iter)->layer();
	    if ( layer == 1  ||  layer == 2 )
	       layer = store->clamshellForHybrid(
		  (* nav_z_hits_iter)->hybrid() );
	    if ( m_comp_dev == kLayer  &&  layer != comp_num_one )
	       continue;
	    
	    if ( m_comp_dev == kLadder  &&
		 store->ladderForSensor( (* nav_z_hits_iter)->sensor() )
		 != comp_num_one )
	       continue;
	    
	    if ( m_comp_dev == kSensor  &&
		 (* nav_z_hits_iter)->sensor() != comp_num_one )
	       continue;
	    
	    m_svz_hits[ m_num_tracks ][ which_hit ]
	       = new CalibratedSVZHit( (** nav_z_hits_iter ) );
	    report( DEBUG, kFacilityString )
	       << "(" << m_svz_hits[ m_num_tracks ][ which_hit ]->layer()
	       << ", " << store->ladderForSensor(
		  m_svz_hits[ m_num_tracks ][ which_hit ]->sensor() )
	       << ", " << m_svz_hits[ m_num_tracks ][ which_hit ]->sensor()
	       << ") ";
	    which_hit++;
	    m_num_svz_hits++;
	 } // end of svz hit loop

	 assert( which_hit == relevant_z );
	 report( DEBUG, kFacilityString ) << endl;

	 m_num_tracks++;
      } // end of track loop (within event)
   } // end loop over frames

   report( NOTICE, kFacilityString )
      << "Finished gathering hits. Found " << m_num_svr_hits
      << " rphi hits and " << m_num_svz_hits << " z hits." << endl;
}

DABoolean
EnderSiAlignmentMod::read_hits()
{
   FILE* hitfile = fopen( m_skim_in.value().c_str(), "rb" );
   if ( hitfile == NULL )
   {
      report( EMERGENCY, kFacilityString )
	 << "Couldn't read from file " << m_skim_in.value().c_str() << endl;
      return false;
   }

   report( NOTICE, kFacilityString )
      << "Opened the file " << m_skim_in.value().c_str()
      << " for hits." << endl;

   m_num_tracks = -1;
   
   NextInFile next_in_file;
   if ( fread( &next_in_file, sizeof( NextInFile ), 1, hitfile ) != 1 )
   {
      report( EMERGENCY, kFacilityString )
	 << "Couldn't read NextInFile record from file "
	 << m_skim_in.value().c_str() << endl;
      return false;
   }

//     // let it leak... it's better than having to deal with destructors
//     HIHelix* garbage_helix = new HIHelix( 1., 1., 1., 1., 1. );
//     CalibratedSVRphiHit* garbage_svr = new CalibratedSVRphiHit(
//        1, 1, 1, 1, 1, 1, 1.0, 1.0, 1.0, 1.0, 1, true );
//     CalibratedSVZHit* garbage_svz = new CalibratedSVZHit(
//        1, 1, 1, 1, 1, 1, 1.0, 1.0, 1.0, 1.0, 1, true );

   while ( next_in_file != kNothing )
   {
      switch ( next_in_file )
      {
	 case kTrack:
//  	    if ( m_num_svr_hits < m_target_hits.value()  ||
//  		 m_num_svz_hits < m_target_hits.value()     )
	    {
	       m_num_tracks++;
	       m_tracks[ m_num_tracks ] = new HIHelix( 1., 1., 1., 1., 1. );
	       if ( fread( m_tracks[ m_num_tracks ], sizeof( HIHelix ), 1,
			   hitfile ) != 1 )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Couldn't read track from file "
		     << m_skim_in.value().c_str() << endl;
		  return false;
	       }
	       report( DEBUG, kFacilityString )
		  << "Read the following helix: curv = "
		  << m_tracks[ m_num_tracks ]->curvature()
		  << " cotTheta = " << m_tracks[ m_num_tracks ]->cotTheta()
		  << " d0 = " << m_tracks[ m_num_tracks ]->d0()
		  << " z0 = " << m_tracks[ m_num_tracks ]->z0()
		  << " phi0 = " << m_tracks[ m_num_tracks ]->phi0() << endl;
	    }
//  	    else
//  	    {
//  	       if ( fread( garbage_helix, sizeof( HIHelix ), 1, hitfile ) != 1 )
//  	       {
//  		  report( EMERGENCY, kFacilityString )
//  		     << "Couldn't read track from file "
//  		     << m_skim_in.value().c_str() << endl;
//  		  return false;
//  	       }
//  	    }
	    break;

	 case kSVRphiHit:
//  	    if ( m_num_svr_hits < m_target_hits.value() )
	    {
	       m_svr_lengths[ m_num_tracks ]++;
	       SVRPtrPtr new_svr_list =
		  new SVRPtr[ m_svr_lengths[ m_num_tracks ] ];
	       for ( int i = 0;  i < m_svr_lengths[ m_num_tracks ] - 1;  i++ )
		  new_svr_list[i] = m_svr_hits[ m_num_tracks ][i];
	       if ( m_svr_lengths[ m_num_tracks ] - 1 > 0 )
		  delete[] m_svr_hits[ m_num_tracks ];
	       m_svr_hits[ m_num_tracks ] = new_svr_list;
	       
	       m_svr_hits[ m_num_tracks ][ m_svr_lengths[ m_num_tracks ] - 1 ]
		  = new CalibratedSVRphiHit(
		     1, 1, 1, 1, 1, 1, 1.0, 1.0, 1.0, 1.0, 1, true );
	       if ( fread(
		  m_svr_hits[ m_num_tracks ][ m_svr_lengths[ m_num_tracks ] - 1 ],
		  sizeof( CalibratedSVRphiHit ), 1,
		  hitfile ) != 1 )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Couldn't read rphi hit from file "
		     << m_skim_in.value().c_str() << endl;
		  return false;
	       }
	       report( DEBUG, kFacilityString )
		  << "    Read the following rphi hit: sensor = "
     << m_svr_hits[ m_num_tracks ][ m_svr_lengths[ m_num_tracks ] - 1 ]->sensor()
		  << endl;

	       m_num_svr_hits++;
	    }
//  	    else
//  	    {
//  	       if ( fread( garbage_svr, sizeof( CalibratedSVRphiHit ), 1,
//  			   hitfile ) != 1 )
//  	       {
//  		  report( EMERGENCY, kFacilityString )
//  		     << "Couldn't read rphi hit from file "
//  		     << m_skim_in.value().c_str() << endl;
//  		  return false;
//  	       }
//  	    }
	    break;

	 case kSVZHit:
//  	    if ( m_num_svz_hits < m_target_hits.value() )
	    {
	       m_svz_lengths[ m_num_tracks ]++;
	       SVZPtrPtr new_svz_list =
		  new SVZPtr[ m_svz_lengths[ m_num_tracks ] ];
	       for ( int i = 0;  i < m_svz_lengths[ m_num_tracks ] - 1;  i++ )
		  new_svz_list[i] = m_svz_hits[ m_num_tracks ][i];
	       if ( m_svz_lengths[ m_num_tracks ] - 1 > 0 )
		  delete[] m_svz_hits[ m_num_tracks ];
	       m_svz_hits[ m_num_tracks ] = new_svz_list;
	       
	       m_svz_hits[ m_num_tracks ][ m_svz_lengths[ m_num_tracks ] - 1 ]
		  = new CalibratedSVZHit(
		     1, 1, 1, 1, 1, 1, 1.0, 1.0, 1.0, 1.0, 1, true );
	       if ( fread(
		  m_svz_hits[ m_num_tracks ][ m_svz_lengths[ m_num_tracks ] - 1 ],
		  sizeof( CalibratedSVZHit ), 1,
		  hitfile ) != 1 )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Couldn't read z hit from file "
		     << m_skim_in.value().c_str() << endl;
		  return false;
	       }
	       report( DEBUG, kFacilityString )
		  << "    Read the following z hit: sensor = "
     << m_svz_hits[ m_num_tracks ][ m_svz_lengths[ m_num_tracks ] - 1 ]->sensor()
		  << endl;
	       
	       m_num_svz_hits++;
	    }
//  	    else
//  	    {
//  	       if ( fread( garbage_svz, sizeof( CalibratedSVZHit ), 1,
//  			   hitfile ) != 1 )
//  	       {
//  		  report( EMERGENCY, kFacilityString )
//  		     << "Couldn't read z hit from file "
//  		     << m_skim_in.value().c_str() << endl;
//  		  return false;
//  	       }
//  	    }
	    
	    break;
      }

      if ( fread( &next_in_file, sizeof( NextInFile ), 1, hitfile ) != 1 )
      {
	 report( EMERGENCY, kFacilityString )
	    << "Couldn't read NextInFile record from file "
	    << m_skim_in.value().c_str() << endl;
	 return false;
      }
   } // end file entry loop
   m_num_tracks++;

   report( NOTICE, kFacilityString )
      << "Finished reading tracks and hits. (" << m_num_tracks
      << " tracks, " << m_num_svr_hits << " svr hits and " << m_num_svz_hits
      << " svz hits.)" << endl;

//     for ( int which_track = 0;  which_track < m_num_tracks;  which_track++ )
//     {
//        report( DEBUG, kFacilityString )
//  	 << "Read the following helix: curv = "
//  	 << m_tracks[ which_track ]->curvature()
//  	 << " cotTheta = " << m_tracks[ which_track ]->cotTheta()
//  	 << " d0 = " << m_tracks[ which_track ]->d0()
//  	 << " z0 = " << m_tracks[ which_track ]->z0()
//  	 << " phi0 = " << m_tracks[ which_track ]->phi0() << endl;
      
//        for ( int which_svr = 0;
//  	    which_svr < m_svr_lengths[ which_track ];
//  	    which_svr++ )
//  	 report( DEBUG, kFacilityString )
//  	    << "    Read the following rphi hit: sensor = "
//  	    << m_svr_hits[ which_track ][ which_svr ]->sensor() << endl;

//        for ( int which_svz = 0;
//  	    which_svz < m_svz_lengths[ which_track ];
//  	    which_svz++ )
//  	 report( DEBUG, kFacilityString )
//  	    << "    Read the following z hit: sensor = "
//  	    << m_svz_hits[ which_track ][ which_svz ]->sensor() << endl;
//     }

   return true;
}

double
EnderSiAlignmentMod::iterate( double* values )
{
   set_deltaX( values[ kDeltaX ] );
   set_deltaY( values[ kDeltaY ] );
   set_deltaZ( values[ kDeltaZ ] );
   set_phiX( values[ kPhiX ] );
   set_phiY( values[ kPhiY ] );
   set_phiZ( values[ kPhiZ ] );

   uint loaded_rphi = 0;
   uint loaded_z = 0;

   double chisq = 0.;
   double rphi_sum = 0.;
   double rphi_squaresum = 0.;
   double z_sum = 0.;
   double z_squaresum = 0.;
   uint rphi_hits_used = 0;
   uint z_hits_used = 0;

   report( NOTICE, kFacilityString )
      << "Starting Minuit iteration. Parameters are: ( "
      << get_deltaX() << ", "
      << get_deltaY() << ", "
      << get_deltaZ() << ", "
      << get_phiX() << ", "
      << get_phiY() << ", "
      << get_phiZ() << " )" << endl;

   // assignment of FIFrameIterators calls resetToBeginning(), which
   // resets proxies
   FIFrameIterator recomputeFrame = *m_begin;

   FAItem< ASiStore > siStore;
   extract( recomputeFrame->record( Stream::kSVAlignment ), siStore );
   assert( siStore.valid() );
   report( DEBUG, kFacilityString ) << "extracted geometry siStore" << endl;

//     FAItem< DGDetectorStore > detStore;
//     extract( recomputeFrame->record( Stream::kSVAlignment ), detStore );
//     assert( detStore.valid() );
//     report( DEBUG, kFacilityString ) << "extracted detStore" << endl;

   HISiSurfaceFactory siSurfaceFactory( (* recomputeFrame),
					NULL, NULL, false );

   report( DEBUG, kFacilityString ) << "created surface factory" << endl;

   for ( uint which_track = 0;
	 which_track < m_num_tracks;
	 which_track++ )
   {
      if ( m_svr_lengths[ which_track ] == 0  &&
	   m_svz_lengths[ which_track ] == 0     )
	 continue;

      // Error matricies don't survive writing to files (yet)
      // but it doesn't look like I'll be needing them.
//        HIHelix copy_track( *m_tracks[ which_track ] );
      HIHelix* p = m_tracks[ which_track ];
      HIHelix copy_track(
	 p->curvature(), p->phi0(), p->d0(), p->cotTheta(), p->z0(),
	 HepSymMatrix( 5, 1 ), p->referencePoint(), p->mass(),
	 p->reportFitResultsAtInitRefPt(), p->lastArcLength(),
	 p->identifier() );

      report( DEBUG, kFacilityString )
	 << "curv = " << copy_track.curvature()
	 << " cotTheta = " << copy_track.cotTheta()
	 << " d0 = " << copy_track.d0()
	 << " z0 = " << copy_track.z0()
	 << " phi0 = " << copy_track.phi0() << endl;

      uint which_hit;
      vector< const CalibratedSVRphiHit* > rphi_hits;
      for ( which_hit = 0;
//  	    ( which_hit < m_svr_lengths[ which_track ]  &&
//  	      loaded_rphi < m_target_hits.value()           );
	    which_hit < m_svr_lengths[ which_track ];
	    which_hit++ )
      {
//  	 CalibratedSVRphiHit* copy_hit = new
//  	    CalibratedSVRphiHit( *m_svr_hits[ which_track ][ which_hit ] );

	 CalibratedSVRphiHit* pr = m_svr_hits[ which_track ][ which_hit ];
	 CalibratedSVRphiHit* copy_hit = new
	    CalibratedSVRphiHit( pr->identifier(), pr->parent(), pr->hybrid(),
				 pr->layer(), pr->sensor(), pr->width(),
				 pr->charge(), pr->charge_error(),
				 pr->coord(), pr->coord_error(),
				 pr->quality(), pr->saturated() );
	 rphi_hits.push_back( copy_hit );

//    	 rphi_hits.push_back( m_svr_hits[ which_track ][ which_hit ] );
  	 loaded_rphi++;
      }

      vector< const CalibratedSVZHit* > z_hits;
      for ( which_hit = 0;
//  	    ( which_hit < m_svz_lengths[ which_track ]  &&
//  	      loaded_z < m_target_hits.value()              );
	    which_hit < m_svz_lengths[ which_track ];
	    which_hit++ )
      {
//  	 CalibratedSVZHit* copy_hit = new
//  	    CalibratedSVZHit( *m_svz_hits[ which_track ][ which_hit ] );

	 CalibratedSVZHit* pz = m_svz_hits[ which_track ][ which_hit ];
	 CalibratedSVZHit* copy_hit = new
	    CalibratedSVZHit( pz->identifier(), pz->parent(), pz->hybrid(),
			      pz->layer(), pz->sensor(), pz->width(),
			      pz->charge(), pz->charge_error(),
			      pz->coord(), pz->coord_error(),
			      pz->quality(), pz->saturated() );
	 z_hits.push_back( copy_hit );

//    	 z_hits.push_back( m_svz_hits[ which_track ][ which_hit ] );
	 loaded_z++;
      }

      if ( rphi_hits.size() == 0  &&  z_hits.size() == 0 ) continue;

      siSurfaceFactory.generateCenterSurfacesWithHits(
	 rphi_hits, z_hits, 0 );
      report( DEBUG, kFacilityString ) << "generated surfaces" << endl;

      vector< HIIntersectionSurface* > surfaces;
      siSurfaceFactory.transferSurfacesTo( surfaces, false );
      report( DEBUG, kFacilityString ) << "transferred surfaces" << endl;
      
      vector< HIIntersectionSurface* >::const_iterator surfaces_iter;
      vector< HIIntersectionSurface* >::const_iterator surfaces_begin
	 = surfaces.begin();
      vector< HIIntersectionSurface* >::const_iterator surfaces_end
	 = surfaces.end();

      vector< HIIntersectionSurface* > composite_surface;
      composite_surface.push_back( (* surfaces_begin) );
      vector< HIIntersectionSurface* >::const_iterator
	 composite_surface_begin = composite_surface.begin();
      report( DEBUG, kFacilityString )
	 << "pushed first surface onto composite" << endl;
      for ( surfaces_iter = surfaces_begin, surfaces_iter++;
	    surfaces_iter != surfaces_end;
	    surfaces_iter++ )
      {
	 HICompositeSurface* tmp_surface
	    = ( (HICompositeSurface*) (* surfaces_iter) );
	 ( (HICompositeSurface*) (* composite_surface_begin) )
	    ->addChildren( *tmp_surface );
      }
      report( DEBUG, kFacilityString ) << "made composite surface" << endl;

      HIHelixIntersector intersector( composite_surface,
				      HIHelixIntersector::kIncreasingRadius,
				      &copy_track );
      report( DEBUG, kFacilityString ) << "created intersector" << endl;

      for ( HIHelixIntersector::IntersectionStatus status =
	       intersector.swimToCurrentSurface(
		  KTMoveControl::kDirectionForward );
	    status == HIHelixIntersector::kIntersectionOK;
	    status = intersector.swimToNextIntersection(
	       KTMoveControl::kDirectionForward ) )
      {
	 HIIntersectionSurface* surface = intersector.currentSurface();
	 report( DEBUG, kFacilityString )
	    << "  sub-surface in the composite surface" << endl;
	 
	 do { 
	    report( DEBUG, kFacilityString )
	       << "    hit in the sub-surface" << endl;
	    double phi = copy_track.phi0();
	    double z = copy_track.position().z();

	    const CalibratedHit* hit = surface->calibratedHit();
	    int sensor = ( (CalibratedSVHit*) (hit) )->sensor();
	    int ladder = siStore->ladderForSensor( sensor );
	    int layer = siStore->layerForSensor( sensor );
	    int layerOrClam = layer;
	    if ( layer == 1  ||  layer == 2 )
	       layerOrClam = siStore->clamshellForHybrid(
		  ( (CalibratedSVHit*) (hit) )->hybrid() );

	    report( DEBUG, kFacilityString )
	       << "    layerOrClam = " << layerOrClam
	       << ", ladder = " << ladder
	       << ", sensor = " << sensor << endl;
	    
	    double residual = surface->dca( copy_track );
	    double norm_residual = residual;
	    DABoolean passes_cut = false;
	    
	    if ( hit->deviceType() == CalibratedHit::SVR )
	    {
	       norm_residual /= m_rphi_norm.value();
//  	       if ( abs( norm_residual ) < 5. )
	       if ( abs( residual ) < m_rphi_cut.value() )
	       {
		  passes_cut = true;
		  report( DEBUG, kFacilityString )
		     << "    rphi hit (" << layerOrClam
		     << ", " << ladder << ", " << sensor
		     << ") residual = " << residual
		     << " (norm resid = " << norm_residual << ")"
		     << " at phi = " << phi << ", z = " << z << endl;
		  rphi_sum += residual;
		  rphi_squaresum += sqr( residual );
		  rphi_hits_used++;

		  if ( m_plot_before )
		     m_rphi_resid_before->fill( residual );
		  if ( m_plot_after )
		     m_rphi_resid_after->fill( residual );
	       }
	    }
	    else if ( hit->deviceType() == CalibratedHit::SVZ )
	    {
	       norm_residual /= m_z_norm.value();
//  	       if ( abs( norm_residual ) < 5. )
	       if ( abs( residual ) < m_z_cut.value() )
	       {
		  passes_cut = true;
		  report( DEBUG, kFacilityString )
		     << "    z hit (" << layerOrClam
		     << ", " << ladder << ", " << sensor
		     << ") residual = " << residual
		     << " (norm resid = " << norm_residual << ")"
		     << " at phi = " << phi << ", z = " << z << endl;
		  z_sum += residual;
		  z_squaresum += sqr( residual );
		  z_hits_used++;

		  if ( m_plot_before )
		     m_z_resid_before->fill( residual );
		  if ( m_plot_after )
		     m_z_resid_after->fill( residual );
	       }
	    }

//  	    if ( abs( norm_residual ) < 5. )
	    if ( passes_cut )
	       chisq += sqr( norm_residual );
	       
	 } while ( surface->advanceToNextCalibratedHit( copy_track ) );
      } // end loop over surfaces
      
      report( DEBUG, kFacilityString ) << "deleting surfaces" << endl;
      for ( surfaces_iter = surfaces_begin;
	    surfaces_iter != surfaces_end;
	    surfaces_iter++ )
      {
	 (* surfaces_iter)->deleteCondition();
	 delete (* surfaces_iter );
      }

      for ( which_hit = 0;  which_hit < rphi_hits.size();  which_hit++ )
	 delete rphi_hits[ which_hit ];

      for ( which_hit = 0;  which_hit < z_hits.size();  which_hit++ )
	 delete z_hits[ which_hit ];

      report( DEBUG, kFacilityString ) << "done with track!" << endl;
   } // end track loop  

   m_minuit_iteration++;

   double rphi_mean = rphi_sum / rphi_hits_used;
   double rphi_spread = sqrt( rphi_squaresum / rphi_hits_used
			      - sqr( rphi_mean ) );
   double z_mean = z_sum / z_hits_used;
   double z_spread = sqrt( z_squaresum / z_hits_used - sqr( z_mean ) );

   report( NOTICE, kFacilityString )
      << "Finished Minuit iteration! chisq = " << chisq
      << ", rphi hits used = " << rphi_hits_used
      << ", z hits used = " << z_hits_used << endl;

   fprintf( m_output, "ABS:   %03d | %12.9f %12.9f %12.9f %12.9f %12.9f %12.9f | %12.1f | %12.9f %12.9f, %12.9f %12.9f\n",
	   m_minuit_iteration,
	   get_deltaX(),
	   get_deltaY(),
	   get_deltaZ(),
	   get_phiX(),
	   get_phiY(),
	   get_phiZ(),
	   chisq,
	   rphi_mean,
	   rphi_spread,
	   z_mean,
	   z_spread );

   fflush( m_output );

   if ( m_minuit_iteration > 1 )
   {
      fprintf( m_output, "DIFF:  %03d | %12.9f %12.9f %12.9f %12.9f %12.9f %12.9f | %12.1f | %12.9f %12.9f, %12.9f %12.9f\n",
	      m_minuit_iteration,
	      get_deltaX() - m_last_params[ kDeltaX ],
	      get_deltaY() - m_last_params[ kDeltaY ],
	      get_deltaZ() - m_last_params[ kDeltaZ ],
	      get_phiX() - m_last_params[ kPhiX ],
	      get_phiY() - m_last_params[ kPhiY ],
	      get_phiZ() - m_last_params[ kPhiZ ],
	      chisq - m_last_chisq,
	      rphi_mean - m_last_rphi_mean,
	      rphi_spread - m_last_rphi_spread,
	      z_mean - m_last_z_mean,
	      z_spread - m_last_z_spread );

      fflush( m_output );

      fprintf( m_output, "MOVE:  %03d | %12.9f %12.9f %12.9f %12.9f %12.9f %12.9f | %12.1f | %12.9f %12.9f, %12.9f %12.9f\n",
	      m_minuit_iteration,
	      get_deltaX() - m_first_params[ kDeltaX ],
	      get_deltaY() - m_first_params[ kDeltaY ],
	      get_deltaZ() - m_first_params[ kDeltaZ ],
	      get_phiX() - m_first_params[ kPhiX ],
	      get_phiY() - m_first_params[ kPhiY ],
	      get_phiZ() - m_first_params[ kPhiZ ],
	      chisq - m_first_chisq,
	      rphi_mean - m_first_rphi_mean,
	      rphi_spread - m_first_rphi_spread,
	      z_mean - m_first_z_mean,
	      z_spread - m_first_z_spread );

      fflush( m_output );
   }

   fprintf( m_output, "CONST: %03d | %d %g %g %g %g %g %g\n",
	   m_minuit_iteration,
	   m_comp_num + 1,
	   get_deltaX(),
	   get_deltaY(),
	   get_deltaZ(),
	   get_phiX(),
	   get_phiY(),
	   get_phiZ() );

   fprintf( m_output, "\n" );
   fflush( m_output );

   if ( m_minuit_iteration == 1 )
   {
      m_first_params[ kDeltaX ] = get_deltaX();
      m_first_params[ kDeltaY ] = get_deltaY();
      m_first_params[ kDeltaZ ] = get_deltaZ();
      m_first_params[ kPhiX ] = get_phiX();
      m_first_params[ kPhiY ] = get_phiY();
      m_first_params[ kPhiZ ] = get_phiZ();
      m_first_chisq = chisq;
      m_first_rphi_mean = rphi_mean;
      m_first_rphi_spread = rphi_spread;
      m_first_z_mean = z_mean;
      m_first_z_spread = z_spread;
   }

   m_last_params[ kDeltaX ] = get_deltaX();
   m_last_params[ kDeltaY ] = get_deltaY();
   m_last_params[ kDeltaZ ] = get_deltaZ();
   m_last_params[ kPhiX ] = get_phiX();
   m_last_params[ kPhiY ] = get_phiY();
   m_last_params[ kPhiZ ] = get_phiZ();
   m_last_chisq = chisq;
   m_last_rphi_mean = rphi_mean;
   m_last_rphi_spread = rphi_spread;
   m_last_z_mean = z_mean;
   m_last_z_spread = z_spread;

   if ( chisq < 5. )
      return 10000000. + ( sqr(get_deltaX()) + sqr(get_deltaY()) + sqr(get_deltaZ()) +
			   sqr(get_phiX()) + sqr(get_phiY()) + sqr(get_phiZ()) );
   else
      return chisq;
}

double
EnderSiAlignmentMod::get_deltaX()
{
   switch ( m_comp_dev )
   {
      case kGlobal: return m_const_global[ m_comp_num ].get_deltaX();
      case kLayer:  return m_const_layer[  m_comp_num ].get_deltaX();
      case kLadder: return m_const_ladder[ m_comp_num ].get_deltaX();
      case kSensor: return m_const_sensor[ m_comp_num ].get_deltaX();
   }
}

double
EnderSiAlignmentMod::get_deltaY()
{
   switch ( m_comp_dev )
   {
      case kGlobal: return m_const_global[ m_comp_num ].get_deltaY();
      case kLayer:  return m_const_layer[  m_comp_num ].get_deltaY();
      case kLadder: return m_const_ladder[ m_comp_num ].get_deltaY();
      case kSensor: return m_const_sensor[ m_comp_num ].get_deltaY();
   }
}

double
EnderSiAlignmentMod::get_deltaZ()
{
   switch ( m_comp_dev )
   {
      case kGlobal: return m_const_global[ m_comp_num ].get_deltaZ();
      case kLayer:  return m_const_layer[  m_comp_num ].get_deltaZ();
      case kLadder: return m_const_ladder[ m_comp_num ].get_deltaZ();
      case kSensor: return m_const_sensor[ m_comp_num ].get_deltaZ();
   }
}

double
EnderSiAlignmentMod::get_phiX()
{
   switch ( m_comp_dev )
   {
      case kGlobal: return m_const_global[ m_comp_num ].get_phiX();
      case kLayer:  return m_const_layer[  m_comp_num ].get_phiX();
      case kLadder: return m_const_ladder[ m_comp_num ].get_phiX();
      case kSensor: return m_const_sensor[ m_comp_num ].get_phiX();
   }
}

double
EnderSiAlignmentMod::get_phiY()
{
   switch ( m_comp_dev )
   {
      case kGlobal: return m_const_global[ m_comp_num ].get_phiY();
      case kLayer:  return m_const_layer[  m_comp_num ].get_phiY();
      case kLadder: return m_const_ladder[ m_comp_num ].get_phiY();
      case kSensor: return m_const_sensor[ m_comp_num ].get_phiY();
   }
}

double
EnderSiAlignmentMod::get_phiZ()
{
   switch ( m_comp_dev )
   {
      case kGlobal: return m_const_global[ m_comp_num ].get_phiZ();
      case kLayer:  return m_const_layer[  m_comp_num ].get_phiZ();
      case kLadder: return m_const_ladder[ m_comp_num ].get_phiZ();
      case kSensor: return m_const_sensor[ m_comp_num ].get_phiZ();
   }
}

void
EnderSiAlignmentMod::set_deltaX( double value )
{
   switch ( m_comp_dev )
   {
      case kGlobal: m_const_global[ m_comp_num ].set_deltaX( value );
	 break;
      case kLayer:  m_const_layer[  m_comp_num ].set_deltaX( value );
	 break;
      case kLadder: m_const_ladder[ m_comp_num ].set_deltaX( value );
	 break;
      case kSensor: m_const_sensor[ m_comp_num ].set_deltaX( value );
	 break;
   }
}

void
EnderSiAlignmentMod::set_deltaY( double value )
{
   switch ( m_comp_dev )
   {
      case kGlobal: m_const_global[ m_comp_num ].set_deltaY( value );
	 break;
      case kLayer:  m_const_layer[  m_comp_num ].set_deltaY( value );
	 break;
      case kLadder: m_const_ladder[ m_comp_num ].set_deltaY( value );
	 break;
      case kSensor: m_const_sensor[ m_comp_num ].set_deltaY( value );
	 break;
   }
}

void
EnderSiAlignmentMod::set_deltaZ( double value )
{
   switch ( m_comp_dev )
   {
      case kGlobal: m_const_global[ m_comp_num ].set_deltaZ( value );
	 break;
      case kLayer:  m_const_layer[  m_comp_num ].set_deltaZ( value );
	 break;
      case kLadder: m_const_ladder[ m_comp_num ].set_deltaZ( value );
	 break;
      case kSensor: m_const_sensor[ m_comp_num ].set_deltaZ( value );
	 break;
   }
}

void
EnderSiAlignmentMod::set_phiX( double value )
{
   switch ( m_comp_dev )
   {
      case kGlobal: m_const_global[ m_comp_num ].set_phiX( value );
	 break;
      case kLayer:  m_const_layer[  m_comp_num ].set_phiX( value );
	 break;
      case kLadder: m_const_ladder[ m_comp_num ].set_phiX( value );
	 break;
      case kSensor: m_const_sensor[ m_comp_num ].set_phiX( value );
	 break;
   }
}

void
EnderSiAlignmentMod::set_phiY( double value )
{
   switch ( m_comp_dev )
   {
      case kGlobal: m_const_global[ m_comp_num ].set_phiY( value );
	 break;
      case kLayer:  m_const_layer[  m_comp_num ].set_phiY( value );
	 break;
      case kLadder: m_const_ladder[ m_comp_num ].set_phiY( value );
	 break;
      case kSensor: m_const_sensor[ m_comp_num ].set_phiY( value );
	 break;
   }
}

void
EnderSiAlignmentMod::set_phiZ( double value )
{
   switch ( m_comp_dev )
   {
      case kGlobal: m_const_global[ m_comp_num ].set_phiZ( value );
	 break;
      case kLayer:  m_const_layer[  m_comp_num ].set_phiZ( value );
	 break;
      case kLadder: m_const_ladder[ m_comp_num ].set_phiZ( value );
	 break;
      case kSensor: m_const_sensor[ m_comp_num ].set_phiZ( value );
	 break;
   }
}

//
// static member functions
//
