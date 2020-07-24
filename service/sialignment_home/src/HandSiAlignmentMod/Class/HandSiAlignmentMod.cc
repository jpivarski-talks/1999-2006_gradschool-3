// -*- C++ -*-
//
// Package:     <HandSiAlignmentMod>
// Module:      HandSiAlignmentMod
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
#include "HandSiAlignmentMod/HandSiAlignmentMod.h"
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

#include "qapplication.h"
#include "HistogramViewerProc/HVPWindow.h"
#include "JobControl/JobControl.h"
#include "ToolBox/HistogramPackage.h"
#include "ASiStorePro/ASiSensorInfo.h"

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
static const char* const kFacilityString = "HandSiAlignmentMod.HandSiAlignmentMod" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule.cc,v 1.2 2000/12/04 19:11:05 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

HandSiAlignmentMod* HandSiAlignmentMod::m_instance = NULL;

//
// constructors and destructor
//
HandSiAlignmentMod::HandSiAlignmentMod()
   : FrameIteratorModuleBase( "HandSiAlignmentMod", "This is my module" )
   , m_command( "const", this )
   , m_file_global( "global_init", this, "unmoved.svalignmentglobal" )
   , m_file_layer(  "layer_init",  this, "unmoved.svalignmentlayer" )
   , m_file_ladder( "ladder_init", this, "unmoved.svalignmentladder" )
   , m_file_sensor( "sensor_init", this, "unmoved.svalignmentsensor" )
   , m_rphi_cut( "rphi_cut", this, 0.0002 )
   , m_z_cut( "z_cut", this, 0.0015 )
   , m_device( "device", this, "global" )
   , m_number( "number", this, 0 )
   , m_skim_dir( "skim_dir", this, "/cdat/dafe/mccann/check_pds/skims" )
   , m_skim_in( "" )
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
   , m_first_time( true )
   , m_const_loaded( false )
   , m_tracks( NULL )
   , m_svr_hits( NULL )
   , m_svz_hits( NULL )
   , m_svr_lengths( NULL )
   , m_svz_lengths( NULL )
   , m_num_tracks( 0 )
   , m_num_svr_hits( 0 )
   , m_num_svz_hits( 0 )
   , m_begin( NULL )
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

// HandSiAlignmentMod::HandSiAlignmentMod( const HandSiAlignmentMod& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HandSiAlignmentMod::~HandSiAlignmentMod()
{
}

//
// assignment operators
//
// const HandSiAlignmentMod& HandSiAlignmentMod::operator=( const HandSiAlignmentMod& rhs )
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

double
HandSiAlignmentMod::get_constant( SubComponent comp, uint which, Direction dir )
{
   if ( ! m_const_loaded )
   {
      m_const_loaded = true;
      read_init_constants();
   }

   switch ( comp )
   {
      case kGlobal:
	 switch ( dir )
	 {
	    case kDeltaX: return m_const_global[ which - 1 ].get_deltaX();
	    case kDeltaY: return m_const_global[ which - 1 ].get_deltaY();
	    case kDeltaZ: return m_const_global[ which - 1 ].get_deltaZ();
	    case kPhiX:   return m_const_global[ which - 1 ].get_phiX();
	    case kPhiY:   return m_const_global[ which - 1 ].get_phiY();
	    case kPhiZ:   return m_const_global[ which - 1 ].get_phiZ();
	 }
      case kLayer:
	 switch ( dir )
	 {
	    case kDeltaX: return m_const_layer[ which - 1 ].get_deltaX();
	    case kDeltaY: return m_const_layer[ which - 1 ].get_deltaY();
	    case kDeltaZ: return m_const_layer[ which - 1 ].get_deltaZ();
	    case kPhiX:   return m_const_layer[ which - 1 ].get_phiX();
	    case kPhiY:   return m_const_layer[ which - 1 ].get_phiY();
	    case kPhiZ:   return m_const_layer[ which - 1 ].get_phiZ();
	 }
      case kLadder:
	 switch ( dir )
	 {
	    case kDeltaX: return m_const_ladder[ which - 1 ].get_deltaX();
	    case kDeltaY: return m_const_ladder[ which - 1 ].get_deltaY();
	    case kDeltaZ: return m_const_ladder[ which - 1 ].get_deltaZ();
	    case kPhiX:   return m_const_ladder[ which - 1 ].get_phiX();
	    case kPhiY:   return m_const_ladder[ which - 1 ].get_phiY();
	    case kPhiZ:   return m_const_ladder[ which - 1 ].get_phiZ();
	 }
      case kSensor:
	 switch ( dir )
	 {
	    case kDeltaX: return m_const_sensor[ which - 1 ].get_deltaX();
	    case kDeltaY: return m_const_sensor[ which - 1 ].get_deltaY();
	    case kDeltaZ: return m_const_sensor[ which - 1 ].get_deltaZ();
	    case kPhiX:   return m_const_sensor[ which - 1 ].get_phiX();
	    case kPhiY:   return m_const_sensor[ which - 1 ].get_phiY();
	    case kPhiZ:   return m_const_sensor[ which - 1 ].get_phiZ();
	 }
   }
}

void
HandSiAlignmentMod::set_constant( SubComponent comp, uint which, Direction dir, double value )
{
   if ( ! m_const_loaded )
   {
      m_const_loaded = true;
      read_init_constants();
   }

   switch ( comp )
   {
      case kGlobal:
	 switch ( dir )
	 {
	    case kDeltaX: m_const_global[ which - 1 ].set_deltaX( value ); break;
	    case kDeltaY: m_const_global[ which - 1 ].set_deltaY( value ); break;
	    case kDeltaZ: m_const_global[ which - 1 ].set_deltaZ( value ); break;
	    case kPhiX:   m_const_global[ which - 1 ].set_phiX( value ); break;
	    case kPhiY:   m_const_global[ which - 1 ].set_phiY( value ); break;
	    case kPhiZ:   m_const_global[ which - 1 ].set_phiZ( value ); break;
	 }
	 break;
      case kLayer:	  
	 switch ( dir )	  
	 {		  
	    case kDeltaX: m_const_layer[ which - 1 ].set_deltaX( value ); break;
	    case kDeltaY: m_const_layer[ which - 1 ].set_deltaY( value ); break;
	    case kDeltaZ: m_const_layer[ which - 1 ].set_deltaZ( value ); break;
	    case kPhiX:   m_const_layer[ which - 1 ].set_phiX( value ); break;
	    case kPhiY:   m_const_layer[ which - 1 ].set_phiY( value ); break;
	    case kPhiZ:   m_const_layer[ which - 1 ].set_phiZ( value ); break;
	 }
	 break;
      case kLadder:	  
	 switch ( dir )	  
	 {		  
	    case kDeltaX: m_const_ladder[ which - 1 ].set_deltaX( value ); break;
	    case kDeltaY: m_const_ladder[ which - 1 ].set_deltaY( value ); break;
	    case kDeltaZ: m_const_ladder[ which - 1 ].set_deltaZ( value ); break;
	    case kPhiX:   m_const_ladder[ which - 1 ].set_phiX( value ); break;
	    case kPhiY:   m_const_ladder[ which - 1 ].set_phiY( value ); break;
	    case kPhiZ:   m_const_ladder[ which - 1 ].set_phiZ( value ); break;
	 }
	 break;
      case kSensor:	  
	 switch ( dir )	  
	 {		  
	    case kDeltaX: m_const_sensor[ which - 1 ].set_deltaX( value ); break;
	    case kDeltaY: m_const_sensor[ which - 1 ].set_deltaY( value ); break;
	    case kDeltaZ: m_const_sensor[ which - 1 ].set_deltaZ( value ); break;
	    case kPhiX:   m_const_sensor[ which - 1 ].set_phiX( value ); break;
	    case kPhiY:   m_const_sensor[ which - 1 ].set_phiY( value ); break;
	    case kPhiZ:   m_const_sensor[ which - 1 ].set_phiZ( value ); break;
	 }
	 break;
   }  
}

static int argc=1;
static char* argv[] ={"HistogramViewer"};

void
HandSiAlignmentMod::switch_files()
{
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

   init_hits();

   m_skim_in = m_skim_dir.value();
   m_skim_in += "/ladder";
   char num[3];
   sprintf( num, "%02d", m_number.value() );
   m_skim_in += num;
   m_skim_in += ".eskim";

   if ( ! read_hits() ) return;
}

void
HandSiAlignmentMod::iterate( const FIFrameIterator& iBegin,
			     const FIFrameIterator& iEnd )
{
   if ( ! m_const_loaded )
   {
      m_const_loaded = true;
      if( ! read_init_constants() )
	 return;
   }
  
   if ( m_first_time )
   {
      m_first_time = false;
      m_begin = &iBegin;
      m_instance = this;
      
      m_jc = JobControl::instance();
      m_hm = m_jc->histogramManagerP();
      m_rphi_resid = m_hm->histogram( 100, "rphi residual",
				      100, -m_rphi_cut.value(), m_rphi_cut.value() );
      m_z_resid = m_hm->histogram( 200, "z residual",
				   100, -m_z_cut.value(), m_z_cut.value() );
      m_rphires_v_x = m_hm->profile(
	 300, "rphi residual VS. X",
	 50, -0.016, 0.016, -m_rphi_cut.value(), m_rphi_cut.value(),
	 HIHistProf::kErrorOnMean );
      m_rphires_v_x_negz = m_hm->profile(
	 310, "negative Z rphi residual VS. X",
	 50, -0.016, 0.016, -m_rphi_cut.value(), m_rphi_cut.value(),
	 HIHistProf::kErrorOnMean );
      m_rphires_v_x_posz = m_hm->profile(
	 320, "positive Z rphi residual VS. X",
	 50, -0.016, 0.016, -m_rphi_cut.value(), m_rphi_cut.value(),
	 HIHistProf::kErrorOnMean );
      m_rphires_v_z = m_hm->profile(
	 400, "rphi residual VS. Z", 50, -0.2, 0.2, -m_rphi_cut.value(), m_rphi_cut.value(),
	 HIHistProf::kErrorOnMean );
      m_zres_v_z = m_hm->profile(
	 500, "z residual VS. Z", 50, -0.2, 0.2, -m_z_cut.value(), m_z_cut.value(),
      	 HIHistProf::kErrorOnMean );
      m_zres_v_z2 = m_hm->profile(
	 501, "z residual VS. Z", 100, -0.2, 0.2, -m_z_cut.value(), m_z_cut.value(),
      	 HIHistProf::kErrorOnMean );
      m_scat_rphires_v_x = m_hm->histogram(
	 350, "rphi residual VS. X (scatter)", 500, -0.016, 0.016,
	 500, -m_rphi_cut.value(), m_rphi_cut.value() );
      m_scat_rphires_v_z = m_hm->histogram(
	 450, "rphi residual VS. Z (scatter)", 500, -0.2, 0.2,
	 500, -m_rphi_cut.value(), m_rphi_cut.value() );
      m_scat_zres_v_z = m_hm->histogram(
	 550, "z residual VS. Z (scatter)", 500, -0.2, 0.2,
	 500, -m_z_cut.value(), m_z_cut.value() );

      m_app = new QApplication( argc, &(argv[0]) );
      m_window = new HVPWindow( m_hm );
      
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

      switch_files();
   } // end if first time
   else if ( m_number.value() != m_comp_num + 1 )
      switch_files();

   // someone may close it. who knows?
   if ( m_app->mainWidget() == NULL )
   {
      m_app->setMainWidget( m_window );
      m_window->show();
   }      

   plot();

   m_window->updateInfo();
   m_app->exec();
   print_out();
}

DABoolean
HandSiAlignmentMod::read_init_constants()
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
HandSiAlignmentMod::init_hits()
{
   // when they come from a file, I actually don't know how many
   // tracks I'll need to get all the rphi hits I need. This is a hack
   // for ladder and sensor only. (which have small target_hits)
   const uint target_hits = 100000;

//     if ( m_tracks != NULL )
//     {
//        for ( uint j = 0;  j < target_hits;  j++ )
//        {
//  	 delete m_tracks[j];

//  	 uint i;
//  	 for ( i = 0;  i < m_svr_lengths[j];  i++ )
//  	    delete m_svr_hits[j][i];
//  	 for ( i = 0;  i < m_svz_lengths[j];  i++ )
//  	    delete m_svz_hits[j][i];

//  	 delete[] m_svr_hits[j];
//  	 delete[] m_svz_hits[j];
//        }
//        delete[] m_tracks;
//        delete[] m_svr_hits;
//        delete[] m_svz_hits;
//     }

   if ( m_tracks == NULL )
   {
      m_tracks = new HelixPtr[ target_hits ];
      m_svr_hits = new SVRPtrPtr[ target_hits ];
      m_svz_hits = new SVZPtrPtr[ target_hits ];
      m_svr_lengths = new uint[ target_hits ];
      m_svz_lengths = new uint[ target_hits ];
   }

   for ( uint i = 0;  i < target_hits;  i++ )
   {
      m_tracks[i] = NULL;

      m_svr_hits[i] = NULL;
      m_svz_hits[i] = NULL;

      m_svr_lengths[i] = 0;
      m_svz_lengths[i] = 0;
   }

   m_num_tracks = 0;
   m_num_svr_hits = 0;
   m_num_svz_hits = 0;
}

void
HandSiAlignmentMod::get_hits( const FIFrameIterator& iBegin,
			      const FIFrameIterator& iEnd )
{
   uint comp_num_one = m_comp_num + 1;

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
	 itFrame != iEnd;
	 ++itFrame )
   {
      FAItem< ASiStore > store;
      extract( itFrame->record( Stream::kSVAlignment ), store );
      assert( store.valid() );

      FATable< NavTrack > navtracks;
      extract( itFrame->record( Stream::kEvent ), navtracks );
      FATable< NavTrack >::const_iterator nav_iter;
      FATable< NavTrack >::const_iterator nav_begin = navtracks.begin();
      FATable< NavTrack >::const_iterator nav_end = navtracks.end();
      FATable< DualTrackHelices > dualtracks;
      extract( itFrame->record( Stream::kEvent ), dualtracks );
      FATable< DualTrackHelices >::const_iterator dualtracks_iter;

      // be sure that we have exactly two good tracks
      if ( navtracks.size() != 2 ) continue;
      if ( ( nav_iter = nav_begin )->muonQuality()->fitAbort() ) continue;
      if ( abs( nav_iter->muonHelix()->cotTheta() ) > 1.25 ) continue;
      if ( nav_iter->seedSVRHits()->size() <= 1 ) continue;
      if ( nav_iter->seedSVZHits()->size() <= 1 ) continue;
      if ( ( ++nav_iter )->muonQuality()->fitAbort() ) continue;
      if ( abs( nav_iter->muonHelix()->cotTheta() ) > 1.25 ) continue;
      if ( nav_iter->seedSVRHits()->size() <= 1 ) continue;
      if ( nav_iter->seedSVZHits()->size() <= 1 ) continue;

      // with no final-state radiation (well, very little... this is
      // one of those infrared divergences, isn't it?
      HepVector3D event_momentum =
	 ( ( nav_iter = nav_begin )->muonFit()->momentum() +
	   ( ++nav_iter )->muonFit()->momentum() );
      double xdiff2 = sqr( event_momentum.x() + 0.0261 );
      double ydiff2 = sqr( event_momentum.y() + 0.0003 );
      if ( ( xdiff2 + ydiff2 ) > 0.300 ) continue;
//        if ( abs( ( nav_iter = nav_begin )->muonHelix()->curvature() +
//  		( ++nav_iter )->muonHelix()->curvature()             )
//  	   > 4E-3 ) continue;

      // This is getting a little rediculous...
      if ( ( ( nav_iter = nav_begin )->muonHelix()->curvature() ) *
	   ( ( ++ nav_iter )->muonHelix()->curvature() ) > 0. ) continue;

//        report( DEBUG, kFacilityString ) << "this is a good event." << endl;

      for ( nav_iter = nav_begin;  nav_iter != nav_end;  nav_iter++ )
      {
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

	 if ( relevant_rphi == 0  &&  relevant_z == 0 ) continue;
	 
	 dualtracks_iter = dualtracks.find( nav_iter->identifier() );
	 
	 // Toggle comments for single/dual track fits
//  	 m_tracks[ m_num_tracks ] = new HIHelix(
//  	    (* nav_iter->muonHelix() ),
//  	    DBCandidate::mass( DBCandidate::Hypo( DBCandidate::kMuon ) ) );
	 m_tracks[ m_num_tracks ] = new HIHelix( dualtracks_iter->helix() );

//  	 report( DEBUG, kFacilityString )
//  	    << "Storing the following helix: curv = "
//  	    << m_tracks[ m_num_tracks ]->curvature()
//  	    << " cotTheta = " << m_tracks[ m_num_tracks ]->cotTheta()
//  	    << " d0 = " << m_tracks[ m_num_tracks ]->d0()
//  	    << " z0 = " << m_tracks[ m_num_tracks ]->z0()
//  	    << " phi0 = " << m_tracks[ m_num_tracks ]->phi0() << endl;

  	 uint which_hit;

	 m_svr_hits[ m_num_tracks ] = new SVRPtr[ relevant_rphi ];
	 m_svr_lengths[ m_num_tracks ] = relevant_rphi;

//  	 report( DEBUG, kFacilityString ) << "   SVRphi hits: ";
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
//  	    report( DEBUG, kFacilityString )
//  	       << "(" << m_svr_hits[ m_num_tracks ][ which_hit ]->layer()
//  	       << ", " << store->ladderForSensor(
//  		  m_svr_hits[ m_num_tracks ][ which_hit ]->sensor() )
//  	       << ", " << m_svr_hits[ m_num_tracks ][ which_hit ]->sensor()
//  	       << ") ";
	    which_hit++;
	    m_num_svr_hits++;
	 } // end of svr hit loop

	 assert( which_hit == relevant_rphi );
//  	 report( DEBUG, kFacilityString ) << endl;

	 m_svz_hits[ m_num_tracks ] = new SVZPtr[ relevant_z ];
	 m_svz_lengths[ m_num_tracks ] = relevant_z;

//  	 report( DEBUG, kFacilityString ) << "   SVZ hits: ";
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
//  	    report( DEBUG, kFacilityString )
//  	       << "(" << m_svz_hits[ m_num_tracks ][ which_hit ]->layer()
//  	       << ", " << store->ladderForSensor(
//  		  m_svz_hits[ m_num_tracks ][ which_hit ]->sensor() )
//  	       << ", " << m_svz_hits[ m_num_tracks ][ which_hit ]->sensor()
//  	       << ") ";
	    which_hit++;
	    m_num_svz_hits++;
	 } // end of svz hit loop

	 assert( which_hit == relevant_z );
//  	 report( DEBUG, kFacilityString ) << endl;

	 m_num_tracks++;
      } // end of track loop (within event)
   } // end loop over frames

   report( NOTICE, kFacilityString )
      << "Finished gathering hits. Found " << m_num_svr_hits
      << " rphi hits and " << m_num_svz_hits << " z hits." << endl;
}

DABoolean
HandSiAlignmentMod::read_hits()
{
   FILE* hitfile = fopen( m_skim_in.c_str(), "rb" );
   if ( hitfile == NULL )
   {
      report( EMERGENCY, kFacilityString )
	 << "Couldn't read from file " << m_skim_in.c_str() << endl;
      return false;
   }

   report( NOTICE, kFacilityString )
      << "Opened the file " << m_skim_in.c_str()
      << " for hits." << endl;

   m_num_tracks = -1;
   
   NextInFile next_in_file;
   if ( fread( &next_in_file, sizeof( NextInFile ), 1, hitfile ) != 1 )
   {
      report( EMERGENCY, kFacilityString )
	 << "Couldn't read NextInFile record from file "
	 << m_skim_in.c_str() << endl;
      return false;
   }

   while ( next_in_file != kNothing )
   {
      uint i;
      SVRPtrPtr new_svr_list;
      SVZPtrPtr new_svz_list;
      switch ( next_in_file )
      {
	 case kTrack:
	    m_num_tracks++;
	    m_tracks[ m_num_tracks ] = new HIHelix( 1., 1., 1., 1., 1. );
	    if ( fread( m_tracks[ m_num_tracks ], sizeof( HIHelix ), 1,
			hitfile ) != 1 )
	    {
	       report( EMERGENCY, kFacilityString )
		  << "Couldn't read track from file "
		  << m_skim_in.c_str() << endl;
	       return false;
	    }
//    	    report( DEBUG, kFacilityString )
//    	       << "Read the following helix: curv = "
//    	       << m_tracks[ m_num_tracks ]->curvature()
//    	       << " cotTheta = " << m_tracks[ m_num_tracks ]->cotTheta()
//    	       << " d0 = " << m_tracks[ m_num_tracks ]->d0()
//    	       << " z0 = " << m_tracks[ m_num_tracks ]->z0()
//    	       << " phi0 = " << m_tracks[ m_num_tracks ]->phi0() << endl;
	    break;

	 case kSVRphiHit:
	    m_svr_lengths[ m_num_tracks ]++;
	    new_svr_list = new SVRPtr[ m_svr_lengths[ m_num_tracks ] ];
	    for ( i = 0;  i < m_svr_lengths[ m_num_tracks ] - 1;  i++ )
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
		  << m_skim_in.c_str() << endl;
	       return false;
	    }
//    	    report( DEBUG, kFacilityString )
//    	       << "    Read the following rphi hit: sensor = "
//  	       << m_svr_hits[ m_num_tracks ][ m_svr_lengths[ m_num_tracks ] - 1 ]->sensor()
//  	       << " coord = "
//  	       << m_svr_hits[ m_num_tracks ][ m_svr_lengths[ m_num_tracks ] - 1 ]->coord()
//  	       << " coord_error = "
//  	       << m_svr_hits[ m_num_tracks ][ m_svr_lengths[ m_num_tracks ] - 1 ]->coord_error()
//    	       << endl;

	    m_num_svr_hits++;
	    break;

	 case kSVZHit:
	    m_svz_lengths[ m_num_tracks ]++;
	    new_svz_list = new SVZPtr[ m_svz_lengths[ m_num_tracks ] ];
	    for ( i = 0;  i < m_svz_lengths[ m_num_tracks ] - 1;  i++ )
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
		  << m_skim_in.c_str() << endl;
	       return false;
	    }
//    	    report( DEBUG, kFacilityString )
//    	       << "    Read the following z hit: sensor = "
//  	       << m_svz_hits[ m_num_tracks ][ m_svz_lengths[ m_num_tracks ] - 1 ]->sensor()
//  	       << " coord = "
//  	       << m_svz_hits[ m_num_tracks ][ m_svz_lengths[ m_num_tracks ] - 1 ]->coord()
//  	       << " coord_error = "
//  	       << m_svz_hits[ m_num_tracks ][ m_svz_lengths[ m_num_tracks ] - 1 ]->coord_error()
//    	       << endl;
	    
	    m_num_svz_hits++;
	    break;
      }

      if ( fread( &next_in_file, sizeof( NextInFile ), 1, hitfile ) != 1 )
      {
	 report( EMERGENCY, kFacilityString )
	    << "Couldn't read NextInFile record from file "
	    << m_skim_in.c_str() << endl;
	 return false;
      }
   } // end file entry loop
   m_num_tracks++;

   report( NOTICE, kFacilityString )
      << "Read in " << m_num_tracks
      << " tracks, " << m_num_svr_hits << " svr hits and " << m_num_svz_hits
      << " svz hits." << endl;

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

void
HandSiAlignmentMod::plot()
{
   double rphi_sum = 0.;
   double rphi_squaresum = 0.;
   double z_sum = 0.;
   double z_squaresum = 0.;
   uint rphi_hits_used = 0;
   uint z_hits_used = 0;

   report( INFO, kFacilityString )
      << m_rphi_resid << " " << m_z_resid << " " << m_rphires_v_x << " " << m_rphires_v_x_negz << " "
      << m_rphires_v_x_posz << " " << " " << m_rphires_v_z << " "
      << m_zres_v_z << " " << m_zres_v_z2 << " " << m_scat_rphires_v_x << " " << m_scat_rphires_v_z
      << " " << m_scat_zres_v_z << endl;

   double sug_deltax = 0.;
   double sug_deltaz = 0.;
   int sug_n_rphi = 0;
   int sug_n_z = 0;
   double sug_sen_deltaz[447];
   int sug_n_sen_z[447];

   int seen_sensor[447];
   double sum_rphi_resid[447];
   double squaresum_rphi_resid[447];
   uint n_rphi_resid[447];
   double sum_z_resid[447];
   double squaresum_z_resid[447];
   uint n_z_resid[447];
   int sen;
   for ( sen = 0;  sen < 447;  sen++ )
   {
      seen_sensor[sen] = 0;
      sum_rphi_resid[sen] = 0.;
      squaresum_rphi_resid[sen] = 0.;
      n_rphi_resid[sen] = 0;
      sum_z_resid[sen] = 0.;
      squaresum_z_resid[sen] = 0.;
      n_z_resid[sen] = 0;

      sug_sen_deltaz[sen] = 0.;
      sug_n_sen_z[sen] = 0;

      m_sensor_cotTheta[sen] = 0.;
   }

   m_sensors_seen.clear();

   report( INFO, kFacilityString )
      << m_rphi_resid << " " << m_z_resid << " " << m_rphires_v_x << " " << m_rphires_v_x_negz << " "
      << m_rphires_v_x_posz << " " << m_rphires_v_z << " "
      << m_zres_v_z << " " << m_zres_v_z2 << " " << m_scat_rphires_v_x << " " << m_scat_rphires_v_z
      << " " << m_scat_zres_v_z << endl;

   m_rphi_resid->reset();
   m_z_resid->reset();
   m_rphires_v_x->reset();
   m_rphires_v_x_negz->reset();
   m_rphires_v_x_posz->reset();
   m_rphires_v_z->reset();
   m_zres_v_z->reset();
   m_zres_v_z2->reset();
   m_scat_rphires_v_x->reset();
   m_scat_rphires_v_z->reset();
   m_scat_zres_v_z->reset();

   report( NOTICE, kFacilityString ) << "Starting to plot ";
   switch ( m_comp_dev )
   {
      case kGlobal:
	 report( NOTICE, kFacilityString )
	    << "global " << " at:" << endl
	    << "1 "
	    << m_const_global[0].get_deltaX() << " "
	    << m_const_global[0].get_deltaY() << " "
	    << m_const_global[0].get_deltaZ() << " "
	    << m_const_global[0].get_phiX() << " "
	    << m_const_global[0].get_phiY() << " "
	    << m_const_global[0].get_phiZ() << endl;
	 break;
      case kLayer:
	 report( NOTICE, kFacilityString )
	    << "layer " << m_comp_num + 1 << " at:" << endl
	    << m_comp_num + 1 << " "
	    << m_const_layer[m_comp_num].get_deltaX() << " "
	    << m_const_layer[m_comp_num].get_deltaY() << " "
	    << m_const_layer[m_comp_num].get_deltaZ() << " "
	    << m_const_layer[m_comp_num].get_phiX() << " "
	    << m_const_layer[m_comp_num].get_phiY() << " "
	    << m_const_layer[m_comp_num].get_phiZ() << endl;
	 break;
      case kLadder:
	 report( NOTICE, kFacilityString )
	    << "ladder " << m_comp_num + 1 << " at:" << endl
	    << m_comp_num + 1 << " "
	    << m_const_ladder[m_comp_num].get_deltaX() << " "
	    << m_const_ladder[m_comp_num].get_deltaY() << " "
	    << m_const_ladder[m_comp_num].get_deltaZ() << " "
	    << m_const_ladder[m_comp_num].get_phiX() << " "
	    << m_const_ladder[m_comp_num].get_phiY() << " "
	    << m_const_ladder[m_comp_num].get_phiZ() << endl;
	 break;
      case kSensor:
	 report( NOTICE, kFacilityString )
	    << "sensor " << m_comp_num + 1 << " at:" << endl
	    << m_comp_num + 1 << " "
	    << m_const_sensor[m_comp_num].get_deltaX() << " "
	    << m_const_sensor[m_comp_num].get_deltaY() << " "
	    << m_const_sensor[m_comp_num].get_deltaZ() << " "
	    << m_const_sensor[m_comp_num].get_phiX() << " "
	    << m_const_sensor[m_comp_num].get_phiY() << " "
	    << m_const_sensor[m_comp_num].get_phiZ() << endl;
	 break;
   }

   // assignment of FIFrameIterators calls resetToBeginning(), which
   // resets proxies
   FIFrameIterator recomputeFrame = *m_begin;

   FAItem< ASiStore > siStore;
   extract( recomputeFrame->record( Stream::kSVAlignment ), siStore );
   assert( siStore.valid() );
   m_siStore = &( *siStore );
//     report( DEBUG, kFacilityString ) << "extracted geometry siStore" << endl;

//     FAItem< DGDetectorStore > detStore;
//     extract( recomputeFrame->record( Stream::kSVAlignment ), detStore );
//     assert( detStore.valid() );
//     report( DEBUG, kFacilityString ) << "extracted detStore" << endl;

   HISiSurfaceFactory siSurfaceFactory( (* recomputeFrame),
					NULL, NULL, false );

//     report( DEBUG, kFacilityString ) << "created surface factory" << endl;

   for ( uint which_track = 0;
	 which_track < m_num_tracks;
	 which_track++ )
   {
      if ( m_svr_lengths[ which_track ] == 0  &&
	   m_svz_lengths[ which_track ] == 0     )
	 continue;

      // Error matricies don't survive writing to files (yet)
      // but it doesn't look like I'll be needing them.
//      HIHelix copy_track( *m_tracks[ which_track ] );
      HIHelix* p = m_tracks[ which_track ];
      HIHelix copy_track(
	 p->curvature(), p->phi0(), p->d0(), p->cotTheta(), p->z0(),
	 HepSymMatrix( 5, 1 ), p->referencePoint(), p->mass(),
	 p->reportFitResultsAtInitRefPt(), p->lastArcLength(),
	 p->identifier() );
      double cotTheta = p->cotTheta();

//        report( DEBUG, kFacilityString )
//  	 << "curv = " << copy_track.curvature()
//  	 << " cotTheta = " << copy_track.cotTheta()
//  	 << " d0 = " << copy_track.d0()
//  	 << " z0 = " << copy_track.z0()
//  	 << " phi0 = " << copy_track.phi0() << endl;

      uint which_hit;
      vector< const CalibratedSVRphiHit* > rphi_hits;
      for ( which_hit = 0;
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
//  	 rphi_hits.push_back( m_svr_hits[ which_track ][ which_hit ] );
      }

      vector< const CalibratedSVZHit* > z_hits;
      for ( which_hit = 0;
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
//  	 z_hits.push_back( m_svz_hits[ which_track ][ which_hit ] );
      }

      if ( rphi_hits.size() == 0  &&  z_hits.size() == 0 ) continue;

      siSurfaceFactory.generateCenterSurfacesWithHits(
	 rphi_hits, z_hits, 0 );
//        report( DEBUG, kFacilityString ) << "generated surfaces" << endl;

      vector< HIIntersectionSurface* > surfaces;
      siSurfaceFactory.transferSurfacesTo( surfaces, false );
//        report( DEBUG, kFacilityString ) << "transferred surfaces" << endl;
      
      vector< HIIntersectionSurface* >::const_iterator surfaces_iter;
      vector< HIIntersectionSurface* >::const_iterator surfaces_begin
	 = surfaces.begin();
      vector< HIIntersectionSurface* >::const_iterator surfaces_end
	 = surfaces.end();

      vector< HIIntersectionSurface* > composite_surface;
      composite_surface.push_back( (* surfaces_begin) );
      vector< HIIntersectionSurface* >::const_iterator
	 composite_surface_begin = composite_surface.begin();
//        report( DEBUG, kFacilityString )
//  	 << "pushed first surface onto composite" << endl;
      for ( surfaces_iter = surfaces_begin, surfaces_iter++;
	    surfaces_iter != surfaces_end;
	    surfaces_iter++ )
      {
	 HICompositeSurface* tmp_surface
	    = ( (HICompositeSurface*) (* surfaces_iter) );
	 ( (HICompositeSurface*) (* composite_surface_begin) )
	    ->addChildren( *tmp_surface );
      }
//        report( DEBUG, kFacilityString ) << "made composite surface" << endl;

      HIHelixIntersector intersector( composite_surface,
				      HIHelixIntersector::kIncreasingRadius,
				      &copy_track );
//        report( DEBUG, kFacilityString ) << "created intersector" << endl;

      for ( HIHelixIntersector::IntersectionStatus status =
	       intersector.swimToCurrentSurface(
		  KTMoveControl::kDirectionForward );
	    status == HIHelixIntersector::kIntersectionOK;
	    status = intersector.swimToNextIntersection(
	       KTMoveControl::kDirectionForward ) )
      {
	 HIIntersectionSurface* surface = intersector.currentSurface();
//  	 report( DEBUG, kFacilityString )
//  	    << "  sub-surface in the composite surface" << endl;
	 
	 do { 
//  	    report( DEBUG, kFacilityString )
//  	       << "    hit in the sub-surface" << endl;
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

//  	    report( DEBUG, kFacilityString )
//  	       << "    layerOrClam = " << layerOrClam
//  	       << ", ladder = " << ladder
//  	       << ", sensor = " << sensor << endl;
	    
	    double residual = surface->dca( copy_track );

	    if ( hit->deviceType() == CalibratedHit::SVR  &&  abs( residual ) < m_rphi_cut.value() )
	    {
//  	       report( DEBUG, kFacilityString )
//  		  << "    rphi hit (" << layerOrClam
//  		  << ", " << ladder << ", " << sensor
//  		  << ") residual = " << residual
//  		  << " at phi = " << phi << ", z = " << z << endl;
	       m_rphi_resid->fill( residual );
	       m_rphires_v_x->fill( ( (CalibratedSVRphiHit*) (hit) )->coord(), residual );
	       if ( z > 0. )
		  m_rphires_v_x_posz->fill( ( (CalibratedSVRphiHit*) (hit) )->coord(), residual );
	       else
		  m_rphires_v_x_negz->fill( ( (CalibratedSVRphiHit*) (hit) )->coord(), residual );
	       m_rphires_v_z->fill( z, residual );
	       m_scat_rphires_v_x->fill( ( (CalibratedSVRphiHit*) (hit) )->coord(), residual );
	       m_scat_rphires_v_z->fill( z, residual );

	       sug_deltax -= residual;
	       sug_n_rphi++;

	       seen_sensor[sensor-1]++;
	       sum_rphi_resid[sensor-1] += residual;
	       squaresum_rphi_resid[sensor-1] += sqr( residual );
	       n_rphi_resid[sensor-1]++;

	       rphi_sum += residual;
	       rphi_squaresum += sqr( residual );
	       rphi_hits_used++;

	       report( INFO, kFacilityString )
//		  << "phi = " << phi
		  << " rphires = " << residual
		  << " x = " << ( (CalibratedSVRphiHit*) (hit) )->coord()
		  << " z = " << z << endl;
	    }
	    else if ( hit->deviceType() == CalibratedHit::SVZ  &&  abs( residual ) < m_z_cut.value() )
	    {
//  	       report( DEBUG, kFacilityString )
//  		  << "    z hit (" << layerOrClam
//  		  << ", " << ladder << ", " << sensor
//  		  << ") residual = " << residual
//  		  << " at phi = " << phi << ", z = " << z << endl;

//	       double real_z = ( (CalibratedSVZHit*) (hit) )->worldcoord( *siStore ).z();
	       double real_z = z;

	       m_z_resid->fill( residual );
	       m_zres_v_z->fill( real_z, residual );
	       m_zres_v_z2->fill( real_z, residual );
	       m_scat_zres_v_z->fill( real_z, residual );

	       sug_deltaz += residual;
	       sug_n_z++;
	       sug_sen_deltaz[sensor-1] -= residual;
	       sug_n_sen_z[sensor-1]++;

	       seen_sensor[sensor-1]++;
	       sum_z_resid[sensor-1] += residual;
	       squaresum_z_resid[sensor-1] += sqr( residual );
	       n_z_resid[sensor-1]++;

	       m_sensor_cotTheta[sensor-1] += cotTheta;

	       z_sum += residual;
	       z_squaresum += sqr( residual );
	       z_hits_used++;

	       report( INFO, kFacilityString )
//		  << "phi = " << phi
		  << " zres = " << residual
		  << " z = " << z << endl;
	    }

	 } while ( surface->advanceToNextCalibratedHit( copy_track ) );
      } // end loop over surfaces
      
//        report( DEBUG, kFacilityString ) << "deleting surfaces" << endl;
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

//        report( DEBUG, kFacilityString ) << "done with track!" << endl;
   } // end track loop  

   for ( sen = 0;  sen < 447;  sen++ )
      if ( seen_sensor[sen] != 0 )
	 m_sensor_cotTheta[sen] /= double( seen_sensor[sen] );

   double rphi_mean = rphi_sum / rphi_hits_used;
   double rphi_spread = sqrt( rphi_squaresum / rphi_hits_used
			      - sqr( rphi_mean ) );
   double z_mean = z_sum / z_hits_used;
   double z_spread = sqrt( z_squaresum / z_hits_used - sqr( z_mean ) );

   if ( sug_n_rphi != 0 )
   { sug_deltax /= double( sug_n_rphi );   sug_deltax *= 1000000.;    sug_deltax = floor( sug_deltax ); }
   if ( sug_n_z != 0 )
   { sug_deltaz /= double( sug_n_z );      sug_deltaz *= 1000000.;    sug_deltaz = floor( sug_deltaz ); }

   cout << "iterator go" << endl;
   cout << "const ladder " << m_comp_num + 1 << " phix [ expr int( -1069163. * ( posZrphivzslope + negZrphivzslope ) ) ]" << endl;
   cout << "const ladder " << m_comp_num + 1 << " y [ expr int( 64476. * ( posZrphivzsloep - negZrphivzslope ) ) ]" << endl;
   cout << "const ladder " << m_comp_num + 1 << " phiy [ expr int( -1000000. * rphivzslope ) ]" << endl;
   cout << "const ladder " << m_comp_num + 1 << " x " << sug_deltax << endl;
   cout << "const ladder " << m_comp_num + 1 << " z " << sug_deltaz << endl;

//     report( NOTICE, kFacilityString )
//        << "Finished plotting! rphi hits used = " << rphi_hits_used
//        << ", z hits used = " << z_hits_used << endl;

//     cout << endl;
//     char line[150];
//     sprintf( line, "       RPHI mean                RPHI sigma                  Z  mean                  Z  sigma" );
//     cout << line << endl;
//     sprintf( line, "%9.6f +/- %8.6f   %9.6f +/- %8.6f    %9.6f +/- %8.6f   %9.6f +/- %8.6f",
//  	    rphi_mean, rphi_spread / sqrt( double( rphi_hits_used ) ),
//  	    rphi_spread, rphi_spread / sqrt( 2. * double( rphi_hits_used ) ),
//  	    z_mean, z_spread / sqrt( double( z_hits_used ) ),
//  	    z_spread, z_spread / sqrt( 2. * double( z_hits_used ) ) );
//     cout << line << endl;

   vector< unsigned int > the_sensors;
   vector< unsigned int > the_population;
   vector< double > the_low_z;
   vector< double > the_high_z;
   vector< double > the_rphi;
   vector< double > the_rphi_err;
   vector< double > the_z;
   vector< double > the_z_err;

   for ( sen = 0;  sen < 447;  sen++ )
   {
      if ( seen_sensor[sen] > 20 )
      {
	 the_sensors.push_back( sen + 1 );
	 the_population.push_back( seen_sensor[sen] );
	 m_sensors_seen.push_back( sen + 1 );

	 const ASiSensorInfo& sensinfo = siStore->sensor( sen + 1 );
	 the_low_z.push_back( sensinfo.coorInSi().z() - sensinfo.halfLengthZ() );
	 the_high_z.push_back( sensinfo.coorInSi().z() + sensinfo.halfLengthZ() );

	 if ( n_z_resid[sen] != 0 )
	 {
	    sug_sen_deltaz[sen] = floor( sug_sen_deltaz[sen] / double( sug_n_sen_z[sen] ) * 1000000. );

	    cout << "const sensor " << sen + 1 << " z ";

	    double what = -0.005;
	    if ( siStore->layerForSensor(sen+1) == 1  ||
		 siStore->layerForSensor(sen+1) == 3     )
	       if ( siStore->ladderForSensor(sen+1) % 2 == 0 )
		  what *= -1.;

	    if ( sensinfo.coorInSi().z() > what ) cout << sug_sen_deltaz[sen];
	    else cout << ( 0. - sug_sen_deltaz[sen] );
	    cout << endl;
	 }

	 if ( n_rphi_resid[sen] == 0 ) the_rphi.push_back( 0. );
	 else the_rphi.push_back( sum_rphi_resid[sen] / double( n_rphi_resid[sen] ) );

	 if ( n_rphi_resid[sen] == 0 ) the_rphi_err.push_back( 0. );
	 else the_rphi_err.push_back( sqrt( ( squaresum_rphi_resid[sen] / double( n_rphi_resid[sen] )
					      - sqr( squaresum_rphi_resid[sen] / double( n_rphi_resid[sen] ) ) )
					    / double( n_rphi_resid[sen] ) ) );

	 if ( n_z_resid[sen] == 0 ) the_z.push_back( 0. );
	 else the_z.push_back( sum_z_resid[sen] / double( n_z_resid[sen] ) );

	 if ( n_z_resid[sen] == 0 ) the_z_err.push_back( 0. );
	 else the_z_err.push_back( sqrt( ( squaresum_z_resid[sen] / double( n_z_resid[sen] )
					   - sqr( squaresum_z_resid[sen] / double( n_z_resid[sen] ) ) )
					 / double( n_z_resid[sen] ) ) );
      }
   }

   while ( the_sensors.size() > 0 )
   {
      uint end, alpha;
      char chunk[15];

      cout << endl << "    S";
      for ( alpha = 1;  alpha <= 8;  alpha++ )
      {
	 end = the_sensors.size() - 1;
	 sprintf( chunk, " |     %03d    ", the_sensors[end] ); cout << chunk;
	 the_sensors.pop_back();
	 if ( the_sensors.size() < 1 ) break;
      }
      char ocompnum[4];
      sprintf( ocompnum, "%02d ", m_comp_num + 1 );
      cout << endl << ocompnum << "  ";
      for ( alpha = 1;  alpha <= 8;  alpha++ )
      {
	 end = the_low_z.size() - 1;
	 sprintf( chunk, " | %4d - %4d",
		  int( 1000. * the_low_z[end] ),
		  int( 1000. * the_high_z[end] ) ); cout << chunk;
	 the_low_z.pop_back();
	 the_high_z.pop_back();
	 if ( the_low_z.size() < 1 ) break;
	 if ( the_high_z.size() < 1 ) break;
      }
//        cout << endl << "   R";
//        for ( alpha = 1;  alpha <= 8;  alpha++ )
//        {
//  	 end = the_rphi.size() - 1;
//  	 if ( the_rphi[end] == 0. ) cout << " |            ";
//  	 else { sprintf( chunk, " |  %9.6f ", the_rphi[end] ); cout << chunk; }
//  	 the_rphi.pop_back();
//  	 if ( the_rphi.size() < 1 ) break;
//        }
//        cout << endl << "   +";
//        for ( alpha = 1;  alpha <= 8;  alpha++ )
//        {
//  	 end = the_rphi_err.size() - 1;
//  	 if ( the_rphi_err[end] == 0. ) cout << " |            ";
//  	 else { sprintf( chunk, " |  (%8.6f)", the_rphi_err[end] ); cout << chunk; }
//  	 the_rphi_err.pop_back();
//  	 if ( the_rphi_err.size() < 1 ) break;
//        }
//        cout << endl << "   Z";
//        for ( alpha = 1;  alpha <= 8;  alpha++ )
//        {
//  	 end = the_z.size() - 1;
//  	 if ( the_z[end] == 0. ) cout << " |            ";
//  	 else { sprintf( chunk, " |  %9.6f ", the_z[end] ); cout << chunk; }
//  	 the_z.pop_back();
//  	 if ( the_z.size() < 1 ) break;
//        }
//        cout << endl << "   -";
//        for ( alpha = 1;  alpha <= 8;  alpha++ )
//        {
//  	 end = the_z_err.size() - 1;
//  	 if ( the_z_err[end] == 0. ) cout << " |            ";
//  	 else { sprintf( chunk, " |  (%8.6f)", the_z_err[end] ); cout << chunk; }
//  	 the_z_err.pop_back();
//  	 if ( the_z_err.size() < 1 ) break;
//        }
      cout << endl << "    N";
      for ( alpha = 1;  alpha <= 8;  alpha++ )
      {
	 end = the_population.size() - 1;
	 sprintf( chunk, " |   %6d   ", the_population[end] ); cout << chunk;
	 the_population.pop_back();
	 if ( the_population.size() < 1 ) break;
      }
      cout << endl;
   }

   // it should look like this:
//  S |     438     |     439     |     450     |     451     |     452     |     453     |     453     |     453     |     453     |     453
//    | -000 - -000 | -000 - -000 | -000 - -000 | -000 - -000 | -000 - -000 | -000 - -000 | -000 - -000 | -000 - -000 | -000 - -000 | -000 - -000
//  R |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000
//  + |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000
//  Z |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000  |  -0.000000
//  - |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000  |   0.000000

}

void
HandSiAlignmentMod::print_out()
{
   uint which;

   ofstream glob( "state.svalignmentglobal" );
   glob << "1 0 0 0 0 0 1 10 DEFAULT" << endl
	<< "1" << endl
	<< "1 " << m_const_global[0].get_deltaX() << " "
	<< m_const_global[0].get_deltaY() << " "
	<< m_const_global[0].get_deltaZ() << " "
	<< m_const_global[0].get_phiX() << " "
	<< m_const_global[0].get_phiY() << " "
	<< m_const_global[0].get_phiZ() << endl;

   ofstream lay( "state.svalignmentlayer" );
   lay << "1 0 0 0 0 0 1 11 DEFAULT" << endl
       << "4" << endl;
   for ( which = 1;  which <= 4;  which++ )
      lay << which << " " << m_const_layer[ which - 1 ].get_deltaX() << " "
	  << m_const_layer[ which - 1 ].get_deltaY() << " "
	  << m_const_layer[ which - 1 ].get_deltaZ() << " "
	  << m_const_layer[ which - 1 ].get_phiX() << " "
	  << m_const_layer[ which - 1 ].get_phiY() << " "
	  << m_const_layer[ which - 1 ].get_phiZ() << endl;

   ofstream lad( "state.svalignmentladder" );
   lad << "1 0 0 0 0 0 1 12 DEFAULT" << endl
       << "61" << endl;
   for ( which = 1;  which <= 61;  which++ )
      lad << which << " " << m_const_ladder[ which - 1 ].get_deltaX() << " "
	  << m_const_ladder[ which - 1 ].get_deltaY() << " "
	  << m_const_ladder[ which - 1 ].get_deltaZ() << " "
	  << m_const_ladder[ which - 1 ].get_phiX() << " "
	  << m_const_ladder[ which - 1 ].get_phiY() << " "
	  << m_const_ladder[ which - 1 ].get_phiZ() << endl;

   ofstream sen( "state.svalignmentsensor" );
   sen << "1 0 0 0 0 0 1 4 DEFAULT" << endl
       << "447" << endl;
   for ( which = 1;  which <= 447;  which++ )
      sen << which << " " << m_const_sensor[ which - 1 ].get_deltaX() << " "
	  << m_const_sensor[ which - 1 ].get_deltaY() << " "
	  << m_const_sensor[ which - 1 ].get_deltaZ() << " "
	  << m_const_sensor[ which - 1 ].get_phiX() << " "
	  << m_const_sensor[ which - 1 ].get_phiY() << " "
	  << m_const_sensor[ which - 1 ].get_phiZ() << endl;
}

double
HandSiAlignmentMod::get_cotTheta( int sensor )
{
   if ( sensor < 1  ||  sensor > 447 )
      return 0.;
   return m_sensor_cotTheta[sensor-1];
}

int
HandSiAlignmentMod::get_showing()
{
   return m_comp_num + 1;
}

void
HandSiAlignmentMod::get_edges( int sensor, double& lower1, double& lower2,
			                   double& upper1, double& upper2 )
{
   vector< uint >::const_iterator iter;
   vector< uint >::const_iterator begin = m_sensors_seen.begin();
   vector< uint >::const_iterator end = m_sensors_seen.end();

   for ( iter = begin;  iter != end;  iter++ )
      if ( *iter == sensor )
      {
	 const ASiSensorInfo& sensinfo = m_siStore->sensor( sensor );
	 lower1 = lower2 = sensinfo.coorInSi().z() - sensinfo.halfLengthZ();
	 upper1 = upper2 = sensinfo.coorInSi().z() + sensinfo.halfLengthZ();
	 return;
      }

   lower1 = lower2 = upper1 = upper2 = 0.;
}

void get_edges( int sensor, double& lower1, double& lower2,
		            double& upper1, double& upper2 )
{
   HandSiAlignmentMod::instance()->get_edges( sensor, lower1, lower2, upper1, upper2 );
}

void
HandSiAlignmentMod::replot()
{
   m_window->updateInfo();
   m_app->exec();
}

HandSiAlignmentMod*
HandSiAlignmentMod::instance()
{
   return m_instance;
}

//
// static member functions
//
