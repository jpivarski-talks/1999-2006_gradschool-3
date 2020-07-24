// -*- C++ -*-
//
// Package:     AlignmentPlots
// Module:      AlignmentPlots
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri Mar  8 13:42:56 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "AlignmentPlots/AlignmentPlots.h"
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

#include "DualTrackHelices/DualTrackHelices.h"

#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HICompositeSurface.h"
#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiSensorInfo.h"

#include "HelixIntersection/HIMagField.h"

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
static const char* const kFacilityString = "Processor.AlignmentPlots" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.25 2002/01/25 20:42:06 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_03_01 $";

static const double rphi_typical_err = 60.*k_um;
static const double z_typical_err = 1.2*k_mm;

//
// static data member definitions
//


//
// constructors and destructor
//
AlignmentPlots::AlignmentPlots( void )               // anal1
   : Processor( "AlignmentPlots" )
   , m_useKalmanResids( "useKalmanResids", this, true )
   , m_useMuPairs( "useMuPairs", this, false )
   , m_useDualTracks( "useDualTracks", this, true )
   , m_beamEnergy( "beamEnergy", this, 5.26874 )
   , m_weightHits( "weightHits", this, false )
   , m_rphiCut( "rphiCut", this, 200.*k_um )
   , m_zCut( "zCut", this, 1.5*k_mm )
   , m_requiredRphiLayers( "requiredRphiLayers", this, 1 )
   , m_requiredZLayers( "requiredZLayers", this, 1 )
   , m_fillSuperNtuple( "fillSuperNtuple", this, false )
   , m_fillRphiHitNtuple( "fillRphiHitNtuple", this, false )
   , m_fillZHitNtuple( "fillZHitNtuple", this, false )
   , m_fillMatchedNtuple( "fillMatchedNtuple", this, false )
   , m_fillRphiOverlapNtuple( "fillRphiOverlapNtuple", this, false )
   , m_fillZOverlapNtuple( "fillZOverlapNtuple", this, false )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &AlignmentPlots::event,    Stream::kEvent );
   bind( &AlignmentPlots::startRun, Stream::kStartRun );
   //bind( &AlignmentPlots::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

AlignmentPlots::~AlignmentPlots()                    // anal5
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
AlignmentPlots::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
AlignmentPlots::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
AlignmentPlots::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   const char* super_ntuple_labels[] =
   { "d0p", "z0p", "ctp", "phi0p", "momp",
     "d0n", "z0n", "ctn", "phi0n", "momn",
     "r1pexist", "r1plad", "r1pwaf", "r1px", "r1pfres", "r1pfrese", "r1phres", "r1pphi", "r1ptz",
     "r2pexist", "r2plad", "r2pwaf", "r2px", "r2pfres", "r2pfrese", "r2phres", "r2pphi", "r2ptz",
     "r3pexist", "r3plad", "r3pwaf", "r3px", "r3pfres", "r3pfrese", "r3phres", "r3pphi", "r3ptz",
     "r4pexist", "r4plad", "r4pwaf", "r4px", "r4pfres", "r4pfrese", "r4phres", "r4pphi", "r4ptz",
     "r1nexist", "r1nlad", "r1nwaf", "r1nx", "r1nfres", "r1nfrese", "r1nhres", "r1nphi", "r1ntz",
     "r2nexist", "r2nlad", "r2nwaf", "r2nx", "r2nfres", "r2nfrese", "r2nhres", "r2nphi", "r2ntz",
     "r3nexist", "r3nlad", "r3nwaf", "r3nx", "r3nfres", "r3nfrese", "r3nhres", "r3nphi", "r3ntz",
     "r4nexist", "r4nlad", "r4nwaf", "r4nx", "r4nfres", "r4nfrese", "r4nhres", "r4nphi", "r4ntz",
     "z1pexist", "z1plad", "z1pwaf", "z1pz", "z1pfres", "z1pfrese", "z1phres", "z1pphi", "z1ptz",
     "z2pexist", "z2plad", "z2pwaf", "z2pz", "z2pfres", "z2pfrese", "z2phres", "z2pphi", "z2ptz",
     "z3pexist", "z3plad", "z3pwaf", "z3pz", "z3pfres", "z3pfrese", "z3phres", "z3pphi", "z3ptz",
     "z4pexist", "z4plad", "z4pwaf", "z4pz", "z4pfres", "z4pfrese", "z4phres", "z4pphi", "z4ptz",
     "z1nexist", "z1nlad", "z1nwaf", "z1nz", "z1nfres", "z1nfrese", "z1nhres", "z1nphi", "z1ntz",
     "z2nexist", "z2nlad", "z2nwaf", "z2nz", "z2nfres", "z2nfrese", "z2nhres", "z2nphi", "z2ntz",
     "z3nexist", "z3nlad", "z3nwaf", "z3nz", "z3nfres", "z3nfrese", "z3nhres", "z3nphi", "z3ntz",
     "z4nexist", "z4nlad", "z4nwaf", "z4nz", "z4nfres", "z4nfrese", "z4nhres", "z4nphi", "z4ntz" };

   m_super_ntuple = iHistoManager.ntuple( 1, "ntuple", kSuperLength, 262144, super_ntuple_labels );

   m_rphi_hits_vs_phi_l1 = iHistoManager.histogram(
      111, "silicon rphi hits vs phi (layer 1)", 63, 0., 2*3.1415926 );
   m_rphi_hits_vs_phi_l2 = iHistoManager.histogram(
      112, "silicon rphi hits vs phi (layer 2)", 63, 0., 2*3.1415926 );
   m_rphi_hits_vs_phi_l3 = iHistoManager.histogram(
      113, "silicon rphi hits vs phi (layer 3)", 63, 0., 2*3.1415926 );
   m_rphi_hits_vs_phi_l4 = iHistoManager.histogram(
      114, "silicon rphi hits vs phi (layer 4)", 63, 0., 2*3.1415926 );

   m_z_hits_vs_phi_l1 = iHistoManager.histogram(
      121, "silicon z hits vs phi (layer 1)", 63, 0., 2*3.1415926 );
   m_z_hits_vs_phi_l2 = iHistoManager.histogram(
      122, "silicon z hits vs phi (layer 2)", 63, 0., 2*3.1415926 );
   m_z_hits_vs_phi_l3 = iHistoManager.histogram(
      123, "silicon z hits vs phi (layer 3)", 63, 0., 2*3.1415926 );
   m_z_hits_vs_phi_l4 = iHistoManager.histogram(
      124, "silicon z hits vs phi (layer 4)", 63, 0., 2*3.1415926 );

   double rphiCut_eff( m_rphiCut.value() );
   double zCut_eff( m_zCut.value() );
   if ( m_weightHits.value() ) {
      rphiCut_eff /= sqr( rphi_typical_err );
      zCut_eff /= sqr( z_typical_err );
   }

   for ( int ladder = 1;  ladder <= 61;  ladder++ )
   {
      char name[3];
      sprintf( name, "%02d", ladder );
      string nameStr( name );

      int layer;
      if ( ladder <= 7 ) layer = 1;
      else if ( ladder <= 17 ) layer = 2;
      else if ( ladder <= 35 ) layer = 3;
      else layer = 4;

      double rphiWidth( 1.6*k_mm );
      double zWidth;
      switch ( layer ) {
	 case 1: zWidth = 7.*k_cm; break;
	 case 2: zWidth = 9.*k_cm; break;
	 case 3: zWidth = 15.*k_cm; break;
	 case 4: zWidth = 20.*k_cm; break;
      } // end switch

      m_rphires_v_x_negz[ladder-1] = iHistoManager.profile(
	 1100 + ladder,
	 ( nameStr + string( " rphi residual VS. X for NEGAITVE Z" ) ).c_str(),
	 50, -rphiWidth, rphiWidth, -rphiCut_eff, rphiCut_eff,
	 HIHistProf::kErrorOnMean );

      m_rphires_v_x_negz_scat[ladder-1] = iHistoManager.histogram(
	 2100 + ladder,
	 ( nameStr + string( " rphi residual VS. X for NEGAITVE Z" ) ).c_str(),
	 50, -rphiWidth, rphiWidth, 50, -rphiCut_eff, rphiCut_eff );

      m_rphires_v_x_posz[ladder-1] = iHistoManager.profile(
	 1200 + ladder, 
	 ( nameStr + string( " rphi residual VS. X for POSITIVE Z" ) ).c_str(),
	 50, -rphiWidth, rphiWidth, -rphiCut_eff, rphiCut_eff,
	 HIHistProf::kErrorOnMean );

      m_rphires_v_x_posz_scat[ladder-1] = iHistoManager.histogram(
	 2200 + ladder, 
	 ( nameStr + string( " rphi residual VS. X for POSITIVE Z" ) ).c_str(),
	 50, -rphiWidth, rphiWidth, 50, -rphiCut_eff, rphiCut_eff );

      m_rphires_v_trackz[ladder-1] = iHistoManager.profile(
	 1300 + ladder, 
	 ( nameStr + string( " rphi residual VS. track Z" ) ).c_str(),
	 50, -zWidth, zWidth, -rphiCut_eff, rphiCut_eff,
	 HIHistProf::kErrorOnMean );

      m_rphires_v_trackz_scat[ladder-1] = iHistoManager.histogram(
	 2300 + ladder, 
	 ( nameStr + string( " rphi residual VS. track Z" ) ).c_str(),
	 50, -zWidth, zWidth, 50, -rphiCut_eff, rphiCut_eff );

      m_zres_v_trackz[ladder-1] = iHistoManager.profile(
	 1400 + ladder, 
	 ( nameStr + string( " z residual VS. track Z" ) ).c_str(),
	 100, -zWidth, zWidth, -zCut_eff, zCut_eff,
	 HIHistProf::kErrorOnMean );

      m_zres_v_trackz_scat[ladder-1] = iHistoManager.histogram(
	 2400 + ladder, 
	 ( nameStr + string( " z residual VS. track Z" ) ).c_str(),
	 100, -zWidth, zWidth, 50, -zCut_eff, zCut_eff );
      
      m_zres_v_hitz[ladder-1] = iHistoManager.profile(
	 1500 + ladder, 
	 ( nameStr + string( " z residual VS. hit Z" ) ).c_str(),
	 100, -zWidth, zWidth, -zCut_eff, zCut_eff,
	 HIHistProf::kErrorOnMean );

      m_zres_v_hitz_scat[ladder-1] = iHistoManager.histogram(
	 2500 + ladder, 
	 ( nameStr + string( " z residual VS. hit Z" ) ).c_str(),
	 100, -zWidth, zWidth, 50, -zCut_eff, zCut_eff );
      
      m_zres_v_trackz_matched[ladder-1] = iHistoManager.profile(
	 1600 + ladder, 
	 ( nameStr + string( " z residual VS. track Z MATCHED" ) ).c_str(),
	 100, -zWidth, zWidth, -zCut_eff, zCut_eff,
	 HIHistProf::kErrorOnMean );

      m_zres_v_trackz_matched_scat[ladder-1] = iHistoManager.histogram(
	 2600 + ladder, 
	 ( nameStr + string( " z residual VS. track Z MATCHED" ) ).c_str(),
	 100, -zWidth, zWidth, 50, -zCut_eff, zCut_eff );
      
      m_zres_v_hitz_matched[ladder-1] = iHistoManager.profile(
	 1700 + ladder, 
	 ( nameStr + string( " z residual VS. hit Z MATCHED" ) ).c_str(),
	 100, -zWidth, zWidth, -zCut_eff, zCut_eff,
	 HIHistProf::kErrorOnMean );

      m_zres_v_hitz_matched_scat[ladder-1] = iHistoManager.histogram(
	 2700 + ladder, 
	 ( nameStr + string( " z residual VS. hit Z MATCHED" ) ).c_str(),
	 100, -zWidth, zWidth, 50, -zCut_eff, zCut_eff );
      
      m_zres_v_hitz_matched[ladder-1] = iHistoManager.profile(
	 1800 + ladder, 
	 ( nameStr + string( " z residual VS. MATCHED X" ) ).c_str(),
	 100, -rphiWidth, rphiWidth, -zCut_eff, zCut_eff,
	 HIHistProf::kErrorOnMean );

      m_zres_v_hitz_matched_scat[ladder-1] = iHistoManager.histogram(
	 2800 + ladder, 
	 ( nameStr + string( " z residual VS. MATCHED X" ) ).c_str(),
	 100, -rphiWidth, rphiWidth, 50, -zCut_eff, zCut_eff );

      m_rphi_overlap[ladder-1] = iHistoManager.histogram(
	 3000 + ladder,
	 ( nameStr + string( " rphi overlap" ) ).c_str(),
	 100, -200.*k_um, 200.*k_um );
      
      m_z_overlap[ladder-1] = iHistoManager.histogram(
	 3100 + ladder,
	 ( nameStr + string( " z overlap" ) ).c_str(),
	 100, -500.*k_um, 500.*k_um );
      
   } // end loop over ladders

   const char* rphi_hit_labels[] =
   { "phi", "trackz", "sensor", "ladder", "layer", "clam",
     "q", "qerr", "width", "pitch",
     "rphires", "rphierr", "hitx", "xerr" };

   const char* z_hit_labels[] =
   { "phi", "trackz", "sensor", "ladder", "layer", "clam",
     "q", "qerr", "width", "pitch",
     "zres", "zerr", "hitz", "zerr" };

   const char* matched_labels[] =
   { "phi", "trackz", "sensor", "ladder", "layer", "clam",
     "xq", "xqerr", "zq", "zqerr", "xwidth", "xpitch", "zwidth", "zpitch",
     "rphires", "rphierr", "zres", "zerr",
     "hitx", "xerr", "hitz", "zerr" };

   const char* rphi_overlap_labels[] =
   { "l_phi", "l_trackz", "l_sensor", "l_ladder",
     "g_phi", "g_trackz", "g_sensor", "g_ladder",
     "layer", "clam", "rphidiff", "rphidiff_err",
     "l_hitx", "g_hitx" };

   const char* z_overlap_labels[] =
   { "l_phi", "l_trackz", "l_sensor", "l_ladder",
     "g_phi", "g_trackz", "g_sensor", "g_ladder",
     "layer", "clam", "zdiff", "zdiff_err",
     "l_hitz", "g_hitz" };

   m_rphi_hit_ntuple = iHistoManager.ntuple(
      4001, "rphi hit", 14, 262144, rphi_hit_labels );

   m_z_hit_ntuple = iHistoManager.ntuple(
      4002, "z hit", 14, 262144, z_hit_labels );

   m_matched_ntuple = iHistoManager.ntuple(
      4003, "matched", 22, 262144, matched_labels );

   m_rphi_overlap_ntuple = iHistoManager.ntuple(
      4004, "rphi overlap", 14, 262144, rphi_overlap_labels );

   m_z_overlap_ntuple = iHistoManager.ntuple(
      4005, "z overlap", 14, 262144, z_overlap_labels );

   m_layer1left1 = iHistoManager.profile(
      5111, "layer1left1", 20, -0.6, 0.4, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer1left1_scat = iHistoManager.histogram(
      5211, "layer1left1", 20, -0.6, 0.4, 35, -0.002, 0.002 );
   m_layer1middle = iHistoManager.profile(
      5112, "layer1middle", 20, -0.6, 0.4, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer1middle_scat = iHistoManager.histogram(
      5212, "layer1middle", 20, -0.6, 0.4, 35, -0.002, 0.002 );
   m_layer1right1 = iHistoManager.profile(
      5113, "layer1right1", 20, -0.6, 0.4, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer1right1_scat = iHistoManager.histogram(
      5213, "layer1right1", 20, -0.6, 0.4, 35, -0.002, 0.002 );

   m_layer2left2 = iHistoManager.profile(
      5121, "layer2left2", 24, -0.6, 0.2, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer2left2_scat = iHistoManager.histogram(
      5221, "layer2left2", 24, -0.6, 0.2, 35, -0.002, 0.002 );
   m_layer2left1 = iHistoManager.profile(
      5122, "layer2left1", 24, -0.6, 0.2, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer2left1_scat = iHistoManager.histogram(
      5222, "layer2left1", 24, -0.6, 0.2, 35, -0.002, 0.002 );
   m_layer2right1 = iHistoManager.profile(
      5123, "layer2right1", 24, -0.6, 0.2, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer2right1_scat = iHistoManager.histogram(
      5223, "layer2right1", 24, -0.6, 0.2, 35, -0.002, 0.002 );
   m_layer2right2 = iHistoManager.profile(
      5124, "layer2right2", 24, -0.6, 0.2, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer2right2_scat = iHistoManager.histogram(
      5224, "layer2right2", 24, -0.6, 0.2, 35, -0.002, 0.002 );

   m_layer3left3 = iHistoManager.profile(
      5131, "layer3left3", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3left3_scat = iHistoManager.histogram(
      5231, "layer3left3", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3left2 = iHistoManager.profile(
      5132, "layer3left2", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3left2_scat = iHistoManager.histogram(
      5232, "layer3left2", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3left1 = iHistoManager.profile(
      5133, "layer3left1", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3left1_scat = iHistoManager.histogram(
      5233, "layer3left1", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3middle = iHistoManager.profile(
      5134, "layer3middle", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3middle_scat = iHistoManager.histogram(
      5234, "layer3middle", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3right1 = iHistoManager.profile(
      5135, "layer3right1", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3right1_scat = iHistoManager.histogram(
      5235, "layer3right1", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3right2 = iHistoManager.profile(
      5136, "layer3right2", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3right2_scat = iHistoManager.histogram(
      5236, "layer3right2", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3right3 = iHistoManager.profile(
      5137, "layer3right3", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3right3_scat = iHistoManager.histogram(
      5237, "layer3right3", 25, -0.4, 0.1, 35, -0.002, 0.002 );

   m_layer4left5 = iHistoManager.profile(
      5141, "layer4left5", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left5_scat = iHistoManager.histogram(
      5241, "layer4left5", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4left4 = iHistoManager.profile(
      5142, "layer4left4", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left4_scat = iHistoManager.histogram(
      5242, "layer4left4", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4left3 = iHistoManager.profile(
      5143, "layer4left3", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left3_scat = iHistoManager.histogram(
      5243, "layer4left3", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4left2 = iHistoManager.profile(
      5144, "layer4left2", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left2_scat = iHistoManager.histogram(
      5244, "layer4left2", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4left1 = iHistoManager.profile(
      5145, "layer4left1", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left1_scat = iHistoManager.histogram(
      5245, "layer4left1", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right1 = iHistoManager.profile(
      5146, "layer4right1", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right1_scat = iHistoManager.histogram(
      5246, "layer4right1", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right2 = iHistoManager.profile(
      5147, "layer4right2", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right2_scat = iHistoManager.histogram(
      5247, "layer4right2", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right3 = iHistoManager.profile(
      5148, "layer4right3", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right3_scat = iHistoManager.histogram(
      5248, "layer4right3", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right4 = iHistoManager.profile(
      5149, "layer4right4", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right4_scat = iHistoManager.histogram(
      5249, "layer4right4", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right5 = iHistoManager.profile(
      5140, "layer4right5", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right5_scat = iHistoManager.histogram(
      5240, "layer4right5", 28, -0.26, 0.02, 35, -0.002, 0.002 );

   m_layer1left1_matched = iHistoManager.profile(
      5311, "layer1left1 MATCHED", 20, -0.6, 0.4, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer1left1_matched_scat = iHistoManager.histogram(
      5411, "layer1left1 MATCHED", 20, -0.6, 0.4, 35, -0.002, 0.002 );
   m_layer1middle_matched = iHistoManager.profile(
      5312, "layer1middle MATCHED", 20, -0.6, 0.4, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer1middle_matched_scat = iHistoManager.histogram(
      5412, "layer1middle MATCHED", 20, -0.6, 0.4, 35, -0.002, 0.002 );
   m_layer1right1_matched = iHistoManager.profile(
      5313, "layer1right1 MATCHED", 20, -0.6, 0.4, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer1right1_matched_scat = iHistoManager.histogram(
      5413, "layer1right1 MATCHED", 20, -0.6, 0.4, 35, -0.002, 0.002 );

   m_layer2left2_matched = iHistoManager.profile(
      5321, "layer2left2 MATCHED", 24, -0.6, 0.2, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer2left2_matched_scat = iHistoManager.histogram(
      5421, "layer2left2 MATCHED", 24, -0.6, 0.2, 35, -0.002, 0.002 );
   m_layer2left1_matched = iHistoManager.profile(
      5322, "layer2left1 MATCHED", 24, -0.6, 0.2, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer2left1_matched_scat = iHistoManager.histogram(
      5422, "layer2left1 MATCHED", 24, -0.6, 0.2, 35, -0.002, 0.002 );
   m_layer2right1_matched = iHistoManager.profile(
      5323, "layer2right1 MATCHED", 24, -0.6, 0.2, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer2right1_matched_scat = iHistoManager.histogram(
      5423, "layer2right1 MATCHED", 24, -0.6, 0.2, 35, -0.002, 0.002 );
   m_layer2right2_matched = iHistoManager.profile(
      5324, "layer2right2 MATCHED", 24, -0.6, 0.2, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer2right2_matched_scat = iHistoManager.histogram(
      5424, "layer2right2 MATCHED", 24, -0.6, 0.2, 35, -0.002, 0.002 );

   m_layer3left3_matched = iHistoManager.profile(
      5331, "layer3left3 MATCHED", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3left3_matched_scat = iHistoManager.histogram(
      5431, "layer3left3 MATCHED", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3left2_matched = iHistoManager.profile(
      5332, "layer3left2 MATCHED", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3left2_matched_scat = iHistoManager.histogram(
      5432, "layer3left2 MATCHED", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3left1_matched = iHistoManager.profile(
      5333, "layer3left1 MATCHED", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3left1_matched_scat = iHistoManager.histogram(
      5433, "layer3left1 MATCHED", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3middle_matched = iHistoManager.profile(
      5334, "layer3middle MATCHED", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3middle_matched_scat = iHistoManager.histogram(
      5434, "layer3middle MATCHED", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3right1_matched = iHistoManager.profile(
      5335, "layer3right1 MATCHED", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3right1_matched_scat = iHistoManager.histogram(
      5435, "layer3right1 MATCHED", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3right2_matched = iHistoManager.profile(
      5336, "layer3right2 MATCHED", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3right2_matched_scat = iHistoManager.histogram(
      5436, "layer3right2 MATCHED", 25, -0.4, 0.1, 35, -0.002, 0.002 );
   m_layer3right3_matched = iHistoManager.profile(
      5337, "layer3right3 MATCHED", 25, -0.4, 0.1, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer3right3_matched_scat = iHistoManager.histogram(
      5437, "layer3right3 MATCHED", 25, -0.4, 0.1, 35, -0.002, 0.002 );

   m_layer4left5_matched = iHistoManager.profile(
      5341, "layer4left5 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left5_matched_scat = iHistoManager.histogram(
      5441, "layer4left5 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4left4_matched = iHistoManager.profile(
      5342, "layer4left4 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left4_matched_scat = iHistoManager.histogram(
      5442, "layer4left4 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4left3_matched = iHistoManager.profile(
      5343, "layer4left3 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left3_matched_scat = iHistoManager.histogram(
      5443, "layer4left3 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4left2_matched = iHistoManager.profile(
      5344, "layer4left2 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left2_matched_scat = iHistoManager.histogram(
      5444, "layer4left2 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4left1_matched = iHistoManager.profile(
      5345, "layer4left1 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4left1_matched_scat = iHistoManager.histogram(
      5445, "layer4left1 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right1_matched = iHistoManager.profile(
      5346, "layer4right1 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right1_matched_scat = iHistoManager.histogram(
      5446, "layer4right1 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right2_matched = iHistoManager.profile(
      5347, "layer4right2 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right2_matched_scat = iHistoManager.histogram(
      5447, "layer4right2 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right3_matched = iHistoManager.profile(
      5348, "layer4right3 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right3_matched_scat = iHistoManager.histogram(
      5448, "layer4right3 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right4_matched = iHistoManager.profile(
      5349, "layer4right4 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right4_matched_scat = iHistoManager.histogram(
      5449, "layer4right4 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
   m_layer4right5_matched = iHistoManager.profile(
      5340, "layer4right5 MATCHED", 28, -0.26, 0.02, -0.005, 0.005,
      HIHistProf::kErrorOnMean );
   m_layer4right5_matched_scat = iHistoManager.histogram(
      5440, "layer4right5 MATCHED", 28, -0.26, 0.02, 35, -0.002, 0.002 );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
AlignmentPlots::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "enter the dragon" << endl;

//     FAItem< DBEventHeader > header;
//     extract( iFrame.record( Stream::kEvent ), header );
//     int run = header->run();
//     int event = header->event();

   float superNtuple[kSuperLength];
   for ( int k = 0;  k < kSuperLength;  k++ ) superNtuple[k] = 0.;

   DBCandidate::Hypo particle;
   if ( m_useMuPairs.value() ) particle = DBCandidate::kMuon;
   else particle = DBCandidate::kElectron;
   
   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator navtracks_iter;
   FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();

   report( DEBUG, kFacilityString ) << "got navtracks" << endl;

   // the standard cuts
   if ( navtracks.size() != 2 ) return ActionBase::kFailed;

   if ( ( navtracks_iter = navtracks.begin() )->electronQuality()->fitAbort() )
      return ActionBase::kFailed;
   if ( abs( navtracks_iter->helixFit( particle )->cotTheta() ) > 1.25 )
      return ActionBase::kFailed;
   if ( navtracks_iter->seedSVRHits()->size() < 1  &&
	navtracks_iter->seedSVZHits()->size() < 1 )
      return ActionBase::kFailed;
   if ( abs( navtracks_iter->helixFit( particle )->d0() ) > 0.0032 )
      return ActionBase::kFailed;
   if ( abs( navtracks_iter->helixFit( particle )->z0() ) > 0.04 )
      return ActionBase::kFailed;
   if ( abs( navtracks_iter->kinematicFit( particle )->momentum().mag() -
	     m_beamEnergy.value() ) > 1.25 ) return ActionBase::kFailed;

   if ( ( ++navtracks_iter )->electronQuality()->fitAbort() )
      return ActionBase::kFailed;
   if ( abs( navtracks_iter->helixFit( particle )->cotTheta() ) > 1.25 )
      return ActionBase::kFailed;
   if ( navtracks_iter->seedSVRHits()->size() < 1  &&
	navtracks_iter->seedSVZHits()->size() < 1 )
      return ActionBase::kFailed;
   if ( abs( navtracks_iter->helixFit( particle )->d0() ) > 0.0032 )
      return ActionBase::kFailed;
   if ( abs( navtracks_iter->helixFit( particle )->z0() ) > 0.04 )
      return ActionBase::kFailed;
   if ( abs( navtracks_iter->kinematicFit( particle )->momentum().mag() -
		m_beamEnergy.value() ) > 1.25 ) return ActionBase::kFailed;

   HepVector3D event_momentum = 
      ( ( navtracks_iter = navtracks.begin() )->kinematicFit( particle )->momentum() +
	( ++navtracks_iter )->kinematicFit( particle )->momentum() );
   double xdiff2 = sqr( event_momentum.x() + 0.0261 );
   double ydiff2 = sqr( event_momentum.y() + 0.0003 );
   if ( ( xdiff2 + ydiff2 ) > 0.300 ) return ActionBase::kFailed;

   if ( ( ( navtracks_iter = navtracks.begin() )->helixFit( particle )->curvature() ) *
	( ( ++navtracks_iter )->helixFit( particle )->curvature() ) > 0. )
      return ActionBase::kFailed;

   report( DEBUG, kFacilityString ) << "passed cuts" << endl;

   // get some more things
   HISiSurfaceFactory siSurfaceFactory( iFrame, NULL, NULL, false );

   FAItem< ASiStore > siStore;
   extract( iFrame.record( Stream::kSVAlignment ), siStore );

   FATable< DualTrackHelices > dualtracks;
   FATable< DualTrackHelices >::const_iterator dualtracks_iter;
   if ( m_useDualTracks.value() )
      extract( iFrame.record( Stream::kEvent ), dualtracks );
   
   report( DEBUG, kFacilityString ) << "got other things" << endl;

   // the track loop
   for( navtracks_iter = navtracks_begin;
	navtracks_iter != navtracks_end;
	navtracks_iter++ )
   {
      report( DEBUG, kFacilityString ) << "foreach track" << endl;

      HIHelix track_helix;
      if ( m_useDualTracks.value() ) {
	 dualtracks_iter = dualtracks.find( navtracks_iter->identifier() );
	 track_helix = HIHelix( dualtracks_iter->helix() );
      } else
	 track_helix = HIHelix( * navtracks_iter->helixFit( particle ) );

      if ( track_helix.curvature() > 0. ) {
	 superNtuple[kD0p] = track_helix.d0();
	 superNtuple[kZ0p] = track_helix.z0();
	 superNtuple[kCtp] = track_helix.cotTheta();
	 superNtuple[kPhi0p] = track_helix.phi0();
	 superNtuple[kMomp] = track_helix.momentum().mag();
      }
      else {
	 superNtuple[kD0n] = track_helix.d0();
	 superNtuple[kZ0n] = track_helix.z0();
	 superNtuple[kCtn] = track_helix.cotTheta();
	 superNtuple[kPhi0n] = track_helix.phi0();
	 superNtuple[kMomn] = track_helix.momentum().mag();
      }

      const NavTrack::SVRHitTable* rphi_hits = navtracks_iter->seedSVRHits();
      NavTrack::SVRHitTable::const_iterator rphi_hits_iter;
      NavTrack::SVRHitTable::const_iterator rphi_hits_begin = rphi_hits->begin();
      NavTrack::SVRHitTable::const_iterator rphi_hits_end = rphi_hits->end();

      const NavTrack::SVZHitTable* z_hits = navtracks_iter->seedSVZHits();
      NavTrack::SVZHitTable::const_iterator z_hits_iter;
      NavTrack::SVZHitTable::const_iterator z_hits_begin = z_hits->begin();
      NavTrack::SVZHitTable::const_iterator z_hits_end = z_hits->end();

      // protect surfacefactory and save some time
      if ( rphi_hits->size() == 0  &&  z_hits->size() == 0 )
	 continue;

      const NavTrack::SVRHitLinkTable* rphi_links = navtracks_iter->svrHitLinks( particle );
      NavTrack::SVRHitLinkTable::const_iterator rphi_links_iter;
      NavTrack::SVRHitLinkTable::const_iterator rphi_links_begin = rphi_links->begin();
      NavTrack::SVRHitLinkTable::const_iterator rphi_links_end = rphi_links->end();

      const NavTrack::SVZHitLinkTable* z_links = navtracks_iter->svzHitLinks( particle );
      NavTrack::SVZHitLinkTable::const_iterator z_links_iter;
      NavTrack::SVZHitLinkTable::const_iterator z_links_begin = z_links->begin();
      NavTrack::SVZHitLinkTable::const_iterator z_links_end = z_links->end();

      // make a map of where the other hits are
      AP_rphiHit* rphimap[61];
      AP_zHit* zmap[61];
      for ( int i = 0;  i < 61;  i++ ) {
	 rphimap[i] = NULL;
	 zmap[i] = NULL;
      }
      vector< AP_rphiHit* > rphihits;
      vector< AP_rphiHit* >::const_iterator rphihits_iter;
      vector< AP_zHit* > zhits;
      vector< AP_zHit* >::const_iterator zhits_iter;

      DABoolean rphiHitLayer[4];
      DABoolean zHitLayer[4];

      report( DEBUG, kFacilityString ) << "made maps" << endl;

      // calculate my own residuals
      siSurfaceFactory.generateCenterSurfacesWithHits( *rphi_hits, *z_hits, NULL );

      vector< HIIntersectionSurface* > surfaces;
      siSurfaceFactory.transferSurfacesTo( surfaces, false );

      vector< HIIntersectionSurface* >::const_iterator surfaces_iter;
      vector< HIIntersectionSurface* >::const_iterator surfaces_begin = surfaces.begin();
      vector< HIIntersectionSurface* >::const_iterator surfaces_end = surfaces.end();

      vector< HIIntersectionSurface* > composite_surface;
      composite_surface.push_back( (* surfaces_begin) );
      vector< HIIntersectionSurface* >::const_iterator
	 composite_surface_begin = composite_surface.begin();

      for ( surfaces_iter = surfaces_begin, surfaces_iter++;
	    surfaces_iter != surfaces_end;
	    surfaces_iter++ ) {
	 HICompositeSurface* tmp_surface
	    = ( (HICompositeSurface*) (* surfaces_iter) );
	 ( (HICompositeSurface*) (* composite_surface_begin) )
	    ->addChildren( *tmp_surface );
      } // end loop over surfaces

      HIHelixIntersector intersector( composite_surface,
				      HIHelixIntersector::kIncreasingRadius,
				      &track_helix );

      report( DEBUG, kFacilityString ) << "set up residual calculating stuff" << endl;

      for ( HIHelixIntersector::IntersectionStatus status =
	       intersector.swimToCurrentSurface(
		  KTMoveControl::kDirectionForward );
	    status == HIHelixIntersector::kIntersectionOK;
	    status = intersector.swimToNextIntersection(
	       KTMoveControl::kDirectionForward ) )
      {
	 HIIntersectionSurface* surface = intersector.currentSurface();
	 do { 
	    report( DEBUG, kFacilityString ) << "foreach hit" << endl;

	    double phi = track_helix.phi0();
	    double trackz = track_helix.position().z();

	    const CalibratedHit* hit = surface->calibratedHit();
	    int sensor = ( (CalibratedSVHit*) (hit) )->sensor();
	    int ladder = siStore->ladderForSensor( sensor );
	    int layer = siStore->layerForSensor( sensor );
	    int layerOrClam = layer;
	    if ( layer == 1  ||  layer == 2 )
	       layerOrClam = siStore->clamshellForHybrid(
		  ( (CalibratedSVHit*) (hit) )->hybrid() );

	    // if this is an RPHI hit
	    if ( hit->deviceType() == CalibratedHit::SVR ) {
	       report( DEBUG, kFacilityString ) << "rphi hit" << endl;

	       rphiHitLayer[layer-1] = true;

	       // get the hit link
	       DABoolean found_link( false );
	       for ( rphi_links_iter = rphi_links_begin;
		     rphi_links_iter != rphi_links_end;
		     rphi_links_iter++ )
		  if ( (* rphi_links_iter )->vRightID()[0] == hit->identifier() ) {
		     found_link = true;
		     break;
		  }
	       TrackFitSVRHitLink linkData(
		  true, 1., 1., navtracks_iter->kinematicFit( particle )->momentum().mag() );
	       if ( ! found_link )
		  report( INFO, kFacilityString )
		     << "Couldn't get the link between track " << navtracks_iter->identifier()
		     << " and rphi hit " << hit->identifier() << endl;
	       else
		  linkData = (* rphi_links_iter )->linkData();

	       report( DEBUG, kFacilityString ) << "got hitlink" << endl;

	       // standard cuts
	       if ( ! linkData.used() ) continue;

	       report( DEBUG, kFacilityString ) << "passed hit cuts" << endl;

	       // get quantities to fill histograms with
	       double rphires;
	       double rphires_err;
	       if ( m_useKalmanResids.value() ) {
		  rphires = linkData.residual();
		  rphires_err = linkData.residualError();
	       } else {
		  rphires = surface->dca( track_helix );
		  rphires_err = rphi_typical_err;
	       }
	       double weighted_rphires( rphires / sqr( rphires_err ) );
	       double hitx( ( ( (CalibratedSVRphiHit*) (hit) )->coord() ) );
	       double hitx_err( ( ( (CalibratedSVRphiHit*) (hit) )->coord_error() ) );
	       double charge( ( (CalibratedSVRphiHit*) (hit) )->charge() );
	       double charge_err( ( (CalibratedSVRphiHit*) (hit) )->charge_error() );
	       double width( ( (CalibratedSVRphiHit*) (hit) )->width() );
	       double pitch( ( ( (CalibratedSVRphiHit*) (hit) )->pitch() ) );

	       switch ( layer ) {
		  case 1:
		     if ( track_helix.curvature() > 0. ) {
			superNtuple[kR1pexist] = 1.;
			superNtuple[kR1plad] = ladder;
			superNtuple[kR1pwaf] = sensor;
			superNtuple[kR1px] = hitx;
			superNtuple[kR1pfres] = linkData.residual();
			superNtuple[kR1pfrese] = linkData.residualError();
			superNtuple[kR1phres] = surface->dca( track_helix );
			superNtuple[kR1pphi] = phi;
			superNtuple[kR1ptz] = trackz;
		     } else {
			superNtuple[kR1nexist] = 1.;
			superNtuple[kR1nlad] = ladder;
			superNtuple[kR1nwaf] = sensor;
			superNtuple[kR1nx] = hitx;
			superNtuple[kR1nfres] = linkData.residual();
			superNtuple[kR1nfrese] = linkData.residualError();
			superNtuple[kR1nhres] = surface->dca( track_helix );
			superNtuple[kR1nphi] = phi;
			superNtuple[kR1ntz] = trackz;
		     }
		     break;
		  case 2:
		     if ( track_helix.curvature() > 0. ) {
			superNtuple[kR2pexist] = 1.;
			superNtuple[kR2plad] = ladder;
			superNtuple[kR2pwaf] = sensor;
			superNtuple[kR2px] = hitx;
			superNtuple[kR2pfres] = linkData.residual();
			superNtuple[kR2pfrese] = linkData.residualError();
			superNtuple[kR2phres] = surface->dca( track_helix );
			superNtuple[kR2pphi] = phi;
			superNtuple[kR2ptz] = trackz;
		     } else {
			superNtuple[kR2nexist] = 1.;
			superNtuple[kR2nlad] = ladder;
			superNtuple[kR2nwaf] = sensor;
			superNtuple[kR2nx] = hitx;
			superNtuple[kR2nfres] = linkData.residual();
			superNtuple[kR2nfrese] = linkData.residualError();
			superNtuple[kR2nhres] = surface->dca( track_helix );
			superNtuple[kR2nphi] = phi;
			superNtuple[kR2ntz] = trackz;
		     }
		     break;
		  case 3:
		     if ( track_helix.curvature() > 0. ) {
			superNtuple[kR3pexist] = 1.;
			superNtuple[kR3plad] = ladder;
			superNtuple[kR3pwaf] = sensor;
			superNtuple[kR3px] = hitx;
			superNtuple[kR3pfres] = linkData.residual();
			superNtuple[kR3pfrese] = linkData.residualError();
			superNtuple[kR3phres] = surface->dca( track_helix );
			superNtuple[kR3pphi] = phi;
			superNtuple[kR3ptz] = trackz;
		     } else {
			superNtuple[kR3nexist] = 1.;
			superNtuple[kR3nlad] = ladder;
			superNtuple[kR3nwaf] = sensor;
			superNtuple[kR3nx] = hitx;
			superNtuple[kR3nfres] = linkData.residual();
			superNtuple[kR3nfrese] = linkData.residualError();
			superNtuple[kR3nhres] = surface->dca( track_helix );
			superNtuple[kR3nphi] = phi;
			superNtuple[kR3ntz] = trackz;
		     }
		     break;
		  case 4:
		     if ( track_helix.curvature() > 0. ) {
			superNtuple[kR4pexist] = 1.;
			superNtuple[kR4plad] = ladder;
			superNtuple[kR4pwaf] = sensor;
			superNtuple[kR4px] = hitx;
			superNtuple[kR4pfres] = linkData.residual();
			superNtuple[kR4pfrese] = linkData.residualError();
			superNtuple[kR4phres] = surface->dca( track_helix );
			superNtuple[kR4pphi] = phi;
			superNtuple[kR4ptz] = trackz;
		     } else {
			superNtuple[kR4nexist] = 1.;
			superNtuple[kR4nlad] = ladder;
			superNtuple[kR4nwaf] = sensor;
			superNtuple[kR4nx] = hitx;
			superNtuple[kR4nfres] = linkData.residual();
			superNtuple[kR4nfrese] = linkData.residualError();
			superNtuple[kR4nhres] = surface->dca( track_helix );
			superNtuple[kR4nphi] = phi;
			superNtuple[kR4ntz] = trackz;
		     }
		     break;
	       } // end switch layer

	       AP_rphiHit* hitinfo = new AP_rphiHit();
	       hitinfo->phi = phi;
	       hitinfo->trackz = trackz;
	       hitinfo->sensor = sensor;
	       hitinfo->ladder = ladder;
	       hitinfo->layer = layer;
	       hitinfo->layerOrClam = layerOrClam;
	       hitinfo->rphires = rphires;
	       hitinfo->rphires_err = rphires_err;
	       hitinfo->weighted_rphires = weighted_rphires;
	       hitinfo->hitx = hitx;
	       hitinfo->hitx_err = hitx_err;
	       hitinfo->charge = charge;
	       hitinfo->charge_err = charge_err;
	       hitinfo->width = width;
	       hitinfo->pitch = pitch;

	       rphihits.push_back( hitinfo );
	       rphimap[ladder-1] = hitinfo;

	       report( DEBUG, kFacilityString ) << "filled hit information" << endl;
	    } // end if hit is rphi

	    // if this is a z hit
	    else if ( hit->deviceType() == CalibratedHit::SVZ ) {
	       report( DEBUG, kFacilityString ) << "z hit" << endl;

	       zHitLayer[layer-1] = true;

	       // get the hit link
	       DABoolean found_link( false );
	       for ( z_links_iter = z_links_begin;
		     z_links_iter != z_links_end;
		     z_links_iter++ )
		  if ( (* z_links_iter )->vRightID()[0] == hit->identifier() ) {
		     found_link = true;
		     break;
		  }
	       TrackFitSVZHitLink linkData(
		  true, 1., 1., navtracks_iter->kinematicFit( particle )->momentum().mag() );
	       if ( ! found_link )
		  report( INFO, kFacilityString )
		     << "Couldn't get the link between track " << navtracks_iter->identifier()
		     << " and z hit " << hit->identifier() << endl;
	       else
		  linkData = (* z_links_iter )->linkData();

	       report( DEBUG, kFacilityString ) << "got hitlink" << endl;

	       // standard cuts
	       if ( ! linkData.used() ) continue;

	       report( DEBUG, kFacilityString ) << "passed hit cuts" << endl;

	       // get quantities to fill histograms with
	       double zres;
	       double zres_err;
	       if ( m_useKalmanResids.value() ) {
		  zres = linkData.residual();
		  zres_err = linkData.residualError();
	       } else {
		  zres = surface->dca( track_helix );
		  zres_err = z_typical_err;
	       }
	       double weighted_zres( zres / sqr( zres_err ) );
	       double hitz( ( (CalibratedSVZHit*) (hit) )->coord() );
	       double hitz_err( ( (CalibratedSVZHit*) (hit) )->coord_error() );
	       double charge( ( (CalibratedSVZHit*) (hit) )->charge() );
	       double charge_err( ( (CalibratedSVZHit*) (hit) )->charge_error() );
	       double width( ( (CalibratedSVZHit*) (hit) )->width() );
	       double pitch( ( (CalibratedSVZHit*) (hit) )->pitch() );

	       switch ( layer ) {
		  case 1:
		     if ( track_helix.curvature() > 0. ) {
			superNtuple[kZ1pexist] = 1.;
			superNtuple[kZ1plad] = ladder;
			superNtuple[kZ1pwaf] = sensor;
			superNtuple[kZ1pz] = hitz;
			superNtuple[kZ1pfres] = linkData.residual();
			superNtuple[kZ1pfrese] = linkData.residualError();
			superNtuple[kZ1phres] = surface->dca( track_helix );
			superNtuple[kZ1pphi] = phi;
			superNtuple[kZ1ptz] = trackz;
		     } else {
			superNtuple[kZ1nexist] = 1.;
			superNtuple[kZ1nlad] = ladder;
			superNtuple[kZ1nwaf] = sensor;
			superNtuple[kZ1nz] = hitz;
			superNtuple[kZ1nfres] = linkData.residual();
			superNtuple[kZ1nfrese] = linkData.residualError();
			superNtuple[kZ1nhres] = surface->dca( track_helix );
			superNtuple[kZ1nphi] = phi;
			superNtuple[kZ1ntz] = trackz;
		     }
		     break;
		  case 2:
		     if ( track_helix.curvature() > 0. ) {
			superNtuple[kZ2pexist] = 1.;
			superNtuple[kZ2plad] = ladder;
			superNtuple[kZ2pwaf] = sensor;
			superNtuple[kZ2pz] = hitz;
			superNtuple[kZ2pfres] = linkData.residual();
			superNtuple[kZ2pfrese] = linkData.residualError();
			superNtuple[kZ2phres] = surface->dca( track_helix );
			superNtuple[kZ2pphi] = phi;
			superNtuple[kZ2ptz] = trackz;
		     } else {
			superNtuple[kZ2nexist] = 1.;
			superNtuple[kZ2nlad] = ladder;
			superNtuple[kZ2nwaf] = sensor;
			superNtuple[kZ2nz] = hitz;
			superNtuple[kZ2nfres] = linkData.residual();
			superNtuple[kZ2nfrese] = linkData.residualError();
			superNtuple[kZ2nhres] = surface->dca( track_helix );
			superNtuple[kZ2nphi] = phi;
			superNtuple[kZ2ntz] = trackz;
		     }
		     break;
		  case 3:
		     if ( track_helix.curvature() > 0. ) {
			superNtuple[kZ3pexist] = 1.;
			superNtuple[kZ3plad] = ladder;
			superNtuple[kZ3pwaf] = sensor;
			superNtuple[kZ3pz] = hitz;
			superNtuple[kZ3pfres] = linkData.residual();
			superNtuple[kZ3pfrese] = linkData.residualError();
			superNtuple[kZ3phres] = surface->dca( track_helix );
			superNtuple[kZ3pphi] = phi;
			superNtuple[kZ3ptz] = trackz;
		     } else {
			superNtuple[kZ3nexist] = 1.;
			superNtuple[kZ3nlad] = ladder;
			superNtuple[kZ3nwaf] = sensor;
			superNtuple[kZ3nz] = hitz;
			superNtuple[kZ3nfres] = linkData.residual();
			superNtuple[kZ3nfrese] = linkData.residualError();
			superNtuple[kZ3nhres] = surface->dca( track_helix );
			superNtuple[kZ3nphi] = phi;
			superNtuple[kZ3ntz] = trackz;
		     }
		     break;
		  case 4:
		     if ( track_helix.curvature() > 0. ) {
			superNtuple[kZ4pexist] = 1.;
			superNtuple[kZ4plad] = ladder;
			superNtuple[kZ4pwaf] = sensor;
			superNtuple[kZ4pz] = hitz;
			superNtuple[kZ4pfres] = linkData.residual();
			superNtuple[kZ4pfrese] = linkData.residualError();
			superNtuple[kZ4phres] = surface->dca( track_helix );
			superNtuple[kZ4pphi] = phi;
			superNtuple[kZ4ptz] = trackz;
		     } else {
			superNtuple[kZ4nexist] = 1.;
			superNtuple[kZ4nlad] = ladder;
			superNtuple[kZ4nwaf] = sensor;
			superNtuple[kZ4nz] = hitz;
			superNtuple[kZ4nfres] = linkData.residual();
			superNtuple[kZ4nfrese] = linkData.residualError();
			superNtuple[kZ4nhres] = surface->dca( track_helix );
			superNtuple[kZ4nphi] = phi;
			superNtuple[kZ4ntz] = trackz;
		     }
		     break;
	       } // end switch layer

	       AP_zHit* hitinfo = new AP_zHit();
	       hitinfo->phi = phi;
	       hitinfo->trackz = trackz;
	       hitinfo->sensor = sensor;
	       hitinfo->ladder = ladder;
	       hitinfo->layer = layer;
	       hitinfo->layerOrClam = layerOrClam;
	       hitinfo->zres = zres;
	       hitinfo->zres_err = zres_err;
	       hitinfo->weighted_zres = weighted_zres;
	       hitinfo->hitz = hitz;
	       hitinfo->hitz_err = hitz_err;
	       hitinfo->charge = charge;
	       hitinfo->charge_err = charge_err;
	       hitinfo->width = width;
	       hitinfo->pitch = pitch;

	       zhits.push_back( hitinfo );
	       zmap[ladder-1] = hitinfo;

	       report( DEBUG, kFacilityString ) << "filled hit information" << endl;
	    } // end switch deviceType()
      
	 } while ( surface->advanceToNextCalibratedHit( track_helix ) );
      } // end loop over surfaces

      // fill the histograms

      report( DEBUG, kFacilityString ) << "ready to fill histograms" << endl;

      if ( m_fillSuperNtuple.value() ) m_super_ntuple->fill( superNtuple );

      int rphiLayersHit( 0 );
      int zLayersHit( 0 );
      for ( int j = 0;  j < 4;  j++ ) {
	 if ( rphiHitLayer[j] ) rphiLayersHit++;
	 if ( zHitLayer[j] ) zLayersHit++;
      }

      vector< AP_rphiHit* >::const_iterator rphihits_begin = rphihits.begin();
      vector< AP_rphiHit* >::const_iterator rphihits_end = rphihits.end();
      if ( rphiLayersHit > m_requiredRphiLayers.value() ) {
	 for ( rphihits_iter = rphihits_begin;
	       rphihits_iter != rphihits_end;
	       rphihits_iter++ ) {
	    report( DEBUG, kFacilityString ) << "foreach rphi hit" << endl;

	    AP_rphiHit* hi( * rphihits_iter );
	    int ladder = hi->ladder;
	    int prevlad = ladder - 1;
	    if (      hi->layer == 1  &&  prevlad == 0 ) prevlad = 7;
	    else if ( hi->layer == 2  &&  prevlad == 7 ) prevlad = 17;
	    else if ( hi->layer == 3  &&  prevlad == 17 ) prevlad = 35;
	    else if ( hi->layer == 4  &&  prevlad == 35 ) prevlad = 61;

	    if ( hi->layer == 1 ) m_rphi_hits_vs_phi_l1->fill( hi->phi );
	    if ( hi->layer == 2 ) m_rphi_hits_vs_phi_l2->fill( hi->phi );
	    if ( hi->layer == 3 ) m_rphi_hits_vs_phi_l3->fill( hi->phi );
	    if ( hi->layer == 4 ) m_rphi_hits_vs_phi_l4->fill( hi->phi );

	    double rphires_use = hi->rphires;
	    if ( m_weightHits.value() ) rphires_use = hi->weighted_rphires;

	    if ( hi->trackz < 0 ) {
	       m_rphires_v_x_negz[ladder-1]->fill( hi->hitx, rphires_use );
	       m_rphires_v_x_negz_scat[ladder-1]->fill( hi->hitx, rphires_use );
	    }
	    if ( hi->trackz > 0 ) {
	       m_rphires_v_x_posz[ladder-1]->fill( hi->hitx, rphires_use );
	       m_rphires_v_x_posz_scat[ladder-1]->fill( hi->hitx, rphires_use );
	    }

	    m_rphires_v_trackz[ladder-1]->fill( hi->trackz, rphires_use );
	    m_rphires_v_trackz_scat[ladder-1]->fill( hi->trackz, rphires_use );

	    if ( m_fillRphiHitNtuple.value() ) {
	       report( DEBUG, kFacilityString ) << "fill rphi ntuple" << endl;

	       float values[] = {
		  hi->phi, hi->trackz,
		  (float) (hi->sensor), (float) (hi->ladder),
		  (float) (hi->layer), (float) (hi->layerOrClam),
		  hi->charge, hi->charge_err, hi->width, hi->pitch,
		  hi->rphires, hi->rphires_err, hi->hitx, hi->hitx_err
	       };
	       m_rphi_hit_ntuple->fill( values );
	    }

	    if ( zmap[ladder-1] != NULL ) {
	       report( DEBUG, kFacilityString ) << "hit is matched" << endl;
	       // Matched hits will be handled in the loop over z hits.
	       // If I did it in both places, I would double-count all hits.
	    } // end if matched

	    if ( rphimap[prevlad-1] != NULL ) {
	       report( DEBUG, kFacilityString ) << "hit has an overlap" << endl;

	       AP_rphiHit* pl( rphimap[prevlad-1] );

	       // it wouldn't make sense to use a weighted rphires here
	       m_rphi_overlap[ladder-1]->fill( hi->rphires - pl->rphires );

	       if ( m_fillRphiOverlapNtuple.value() ) {
		  report( DEBUG, kFacilityString ) << "fill overlap ntuple" << endl;

		  float values[] = {
		     (float) (pl->phi), (float) (pl->trackz),
		     (float) (pl->sensor), (float) (pl->ladder),
		     (float) (hi->phi), (float) (hi->trackz),
		     (float) (hi->sensor), (float) (hi->ladder),
		     (float) (hi->layer), (float) (hi->layerOrClam),
		     hi->rphires - pl->rphires, sqrt( sqr( hi->rphires ) + sqr( pl->rphires ) ),
		     pl->hitx, hi->hitx
		  };
		  m_rphi_overlap_ntuple->fill( values );
	       }
	    } // end if there is an overlap hit

	 } // end foreach rphi hit
      } // end if there are enough layers

      vector< AP_zHit* >::const_iterator zhits_begin = zhits.begin();
      vector< AP_zHit* >::const_iterator zhits_end = zhits.end();
      if ( zLayersHit > m_requiredZLayers.value() ) {
	 for ( zhits_iter = zhits_begin;
	       zhits_iter != zhits_end;
	       zhits_iter++ ) {
	    report( DEBUG, kFacilityString ) << "foreach z hit" << endl;

	    AP_zHit* hi( * zhits_iter );
	    int ladder = hi->ladder;
	    int prevlad = ladder - 1;
	    if (      hi->layer == 1  &&  prevlad == 0 ) prevlad = 7;
	    else if ( hi->layer == 2  &&  prevlad == 7 ) prevlad = 17;
	    else if ( hi->layer == 3  &&  prevlad == 17 ) prevlad = 35;
	    else if ( hi->layer == 4  &&  prevlad == 35 ) prevlad = 61;

	    if ( hi->layer == 1 ) m_z_hits_vs_phi_l1->fill( hi->phi );
	    if ( hi->layer == 2 ) m_z_hits_vs_phi_l2->fill( hi->phi );
	    if ( hi->layer == 3 ) m_z_hits_vs_phi_l3->fill( hi->phi );
	    if ( hi->layer == 4 ) m_z_hits_vs_phi_l4->fill( hi->phi );

	    double zres_use = hi->zres;
	    if ( m_weightHits.value() ) zres_use = hi->weighted_zres;

	    m_zres_v_trackz[ladder-1]->fill( hi->trackz, zres_use );
	    m_zres_v_trackz_scat[ladder-1]->fill( hi->trackz, zres_use );

	    m_zres_v_hitz[ladder-1]->fill( hi->hitz, zres_use );
	    m_zres_v_hitz_scat[ladder-1]->fill( hi->hitz, zres_use );

	    if ( m_fillZHitNtuple.value() ) {
	       report( DEBUG, kFacilityString ) << "fill z ntuple" << endl;

	       float values[] = {
		  hi->phi, hi->trackz,
		  (float) (hi->sensor), (float) (hi->ladder),
		  (float) (hi->layer), (float) (hi->layerOrClam),
		  hi->charge, hi->charge_err, hi->width, hi->pitch,
		  hi->zres, hi->zres_err, hi->hitz, hi->hitz_err
	       };
	       m_z_hit_ntuple->fill( values );
	    }

	    if ( rphimap[ladder-1] != NULL ) {
	       report( DEBUG, kFacilityString ) << "hit is matched" << endl;

	       AP_rphiHit* mh( rphimap[ladder-1] );

	       m_zres_v_trackz_matched[ladder-1]->fill( hi->trackz, zres_use );
	       m_zres_v_trackz_matched_scat[ladder-1]->fill( hi->trackz, zres_use );

	       m_zres_v_hitz_matched[ladder-1]->fill( hi->hitz, zres_use );
	       m_zres_v_hitz_matched_scat[ladder-1]->fill( hi->hitz, zres_use );

	       if ( m_fillMatchedNtuple.value() ) {
		  report( DEBUG, kFacilityString ) << "fill matched ntuple" << endl;

		  float values[] = {
		     ( hi->phi + mh->phi ) / 2., ( hi->trackz + mh->trackz ) / 2.,
		     (float) (hi->sensor), (float) (hi->ladder),
		     (float) (hi->layer), (float) (hi->layerOrClam),
		     mh->charge, mh->charge_err, hi->charge, hi->charge_err,
		     mh->width, mh->pitch, hi->width, hi->pitch,
		     mh->rphires, mh->rphires_err, hi->zres, hi->zres_err,
		     mh->hitx, mh->hitx_err, hi->hitz, hi->hitz_err
		  };
		  m_matched_ntuple->fill( values );
	       } // end if filling matched ntuple
	    } // end if matched
	 
	    if ( zmap[prevlad-1] != NULL ) {
	       report( DEBUG, kFacilityString ) << "hit has overlap" << endl;

	       AP_zHit* pl( zmap[prevlad-1] );

	       // it wouldn't make sense to use a weighted zres here
	       m_z_overlap[ladder-1]->fill( hi->zres - pl->zres );

	       if ( m_fillZOverlapNtuple.value() ) {
		  report( DEBUG, kFacilityString ) << "fill overlap ntuple" << endl;

		  float values[] = {
		     (float) (pl->phi), (float) (pl->trackz),
		     (float) (pl->sensor), (float) (pl->ladder),
		     (float) (hi->phi), (float) (hi->trackz),
		     (float) (hi->sensor), (float) (hi->ladder),
		     (float) (hi->layer), (float) (hi->layerOrClam),
		     hi->zres - pl->zres, sqrt( sqr( hi->zres ) + sqr( pl->zres ) ),
		     pl->hitz, hi->hitz
		  };
		  m_z_overlap_ntuple->fill( values );
	       }
	    } // end if there is an overlap hit

	    // smiles! horribly coded, but fast!
	    double l1phidiv( 2 * 3.1415926 / 7 );
	    double l2phidiv( 2 * 3.1415926 / 10 );
	    double l3phidiv( 2 * 3.1415926 / 18 );
	    double l4phidiv( 2 * 3.1415926 / 26 );

	    double smilephi( hi->phi );
	    switch ( hi->ladder ) {
	       case 6: case 15: case 31: case 32: case 55: case 56:
		  smilephi = ( smilephi > 3.1415926 ? smilephi - 2. * 3.1415926 : smilephi );
		  break;
	    } // end switch sensor for smilephi

	    // this was "switch ( hi->layer )" in all the plots you
	    // generated... explains some things, doesn't it?
	    switch ( hi->ladder ) {
	       case 1: smilephi -= 2 * l1phidiv; break;
	       case 2: smilephi -= 3 * l1phidiv; break;
	       case 3: smilephi -= 4 * l1phidiv; break;
	       case 4: smilephi -= 5 * l1phidiv; break;
	       case 5: smilephi -= 6 * l1phidiv; break;
	       case 6: smilephi -= 0 * l1phidiv; break;
	       case 7: smilephi -= 1 * l1phidiv; break;

	       case 8: smilephi -= 3 * l2phidiv; break;
	       case 9: smilephi -= 4 * l2phidiv; break;
	       case 10: smilephi -= 5 * l2phidiv; break;
	       case 11: smilephi -= 6 * l2phidiv; break;
	       case 12: smilephi -= 7 * l2phidiv; break;
	       case 13: smilephi -= 8 * l2phidiv; break;
	       case 14: smilephi -= 9 * l2phidiv; break;
	       case 15: smilephi -= 0 * l2phidiv; break;
	       case 16: smilephi -= 1 * l2phidiv; break;
	       case 17: smilephi -= 2 * l2phidiv; break;

	       case 18: smilephi -= 5 * l3phidiv; break;
	       case 19: smilephi -= 6 * l3phidiv; break;
	       case 20: smilephi -= 7 * l3phidiv; break;
	       case 21: smilephi -= 8 * l3phidiv; break;
	       case 22: smilephi -= 9 * l3phidiv; break;
	       case 23: smilephi -= 10 * l3phidiv; break;
	       case 24: smilephi -= 11 * l3phidiv; break;
	       case 25: smilephi -= 12 * l3phidiv; break;
	       case 26: smilephi -= 13 * l3phidiv; break;
	       case 27: smilephi -= 14 * l3phidiv; break;
	       case 28: smilephi -= 15 * l3phidiv; break;
	       case 29: smilephi -= 16 * l3phidiv; break;
	       case 30: smilephi -= 17 * l3phidiv; break;
	       case 31: smilephi -= 0 * l3phidiv; break;
	       case 32: smilephi -= 1 * l3phidiv; break;
	       case 33: smilephi -= 2 * l3phidiv; break;
	       case 34: smilephi -= 3 * l3phidiv; break;
	       case 35: smilephi -= 4 * l3phidiv; break;

	       case 36: smilephi -= 7 * l4phidiv; break;
	       case 37: smilephi -= 8 * l4phidiv; break;
	       case 38: smilephi -= 9 * l4phidiv; break;
	       case 39: smilephi -= 10 * l4phidiv; break;
	       case 40: smilephi -= 11 * l4phidiv; break;
	       case 41: smilephi -= 12 * l4phidiv; break;
	       case 42: smilephi -= 13 * l4phidiv; break;
	       case 43: smilephi -= 14 * l4phidiv; break;
	       case 44: smilephi -= 15 * l4phidiv; break;
	       case 45: smilephi -= 16 * l4phidiv; break;
	       case 46: smilephi -= 17 * l4phidiv; break;
	       case 47: smilephi -= 18 * l4phidiv; break;
	       case 48: smilephi -= 19 * l4phidiv; break;
	       case 49: smilephi -= 20 * l4phidiv; break;
	       case 50: smilephi -= 21 * l4phidiv; break;
	       case 51: smilephi -= 22 * l4phidiv; break;
	       case 52: smilephi -= 23 * l4phidiv; break;
	       case 53: smilephi -= 24 * l4phidiv; break;
	       case 54: smilephi -= 25 * l4phidiv; break;
	       case 55: smilephi -= 0 * l4phidiv; break;
	       case 56: smilephi -= 1 * l4phidiv; break;
	       case 57: smilephi -= 2 * l4phidiv; break;
	       case 58: smilephi -= 3 * l4phidiv; break;
	       case 59: smilephi -= 4 * l4phidiv; break;
	       case 60: smilephi -= 5 * l4phidiv; break;
	       case 61: smilephi -= 6 * l4phidiv; break;
	    } // end switch ladder for overlapping phi

	    switch ( hi->sensor ) {
	       // layer1left1
	       case 1: case 4: case 7: case 10: case 13: case 16: case 19:
		  m_layer1left1->fill( smilephi, hi->zres );
		  m_layer1left1_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer1left1_matched->fill( smilephi, hi->zres );
		     m_layer1left1_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer1middle
	       case 2: case 5: case 8: case 11: case 14: case 17: case 20:
		  m_layer1middle->fill( smilephi, hi->zres );
		  m_layer1middle_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer1middle_matched->fill( smilephi, hi->zres );
		     m_layer1middle_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer1right1
	       case 3: case 6: case 9: case 12: case 15: case 18: case 21:
		  m_layer1right1->fill( smilephi, hi->zres );
		  m_layer1right1_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer1right1_matched->fill( smilephi, hi->zres );
		     m_layer1right1_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;

	    // layer2left2
	       case 22: case 26: case 30: case 34: case 38:
	       case 42: case 46: case 50: case 54: case 58:
		  m_layer2left2->fill( smilephi, hi->zres );
		  m_layer2left2_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer2left2_matched->fill( smilephi, hi->zres );
		     m_layer2left2_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer2left1
	       case 23: case 27: case 31: case 35: case 39:
	       case 43: case 47: case 51: case 55: case 59:
		  m_layer2left1->fill( smilephi, hi->zres );
		  m_layer2left1_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer2left1_matched->fill( smilephi, hi->zres );
		     m_layer2left1_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer2right1
	       case 24: case 28: case 32: case 36: case 40:
	       case 44: case 48: case 52: case 56: case 60:
		  m_layer2right1->fill( smilephi, hi->zres );
		  m_layer2right1_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer2right1_matched->fill( smilephi, hi->zres );
		     m_layer2right1_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer2right2
	       case 25: case 29: case 33: case 37: case 41:
	       case 45: case 49: case 53: case 57: case 61:
		  m_layer2right2->fill( smilephi, hi->zres );
		  m_layer2right2_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer2right2_matched->fill( smilephi, hi->zres );
		     m_layer2right2_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;

	    // layer3left3
	       case 62: case 69: case 76: case 83: case 90: case 97: case 104:
	       case 111: case 118: case 125: case 132: case 139: case 146: case 153:
	       case 160: case 167: case 174: case 181:
		  m_layer3left3->fill( smilephi, hi->zres );
		  m_layer3left3_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer3left3_matched->fill( smilephi, hi->zres );
		     m_layer3left3_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer3left2
	       case 63: case 70: case 77: case 84: case 91: case 98: case 105:
	       case 112: case 119: case 126: case 133: case 140: case 147: case 154:
	       case 161: case 168: case 175: case 182:
		  m_layer3left2->fill( smilephi, hi->zres );
		  m_layer3left2_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer3left2_matched->fill( smilephi, hi->zres );
		     m_layer3left2_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer3left1
	       case 64: case 71: case 78: case 85: case 92: case 99: case 106:
	       case 113: case 120: case 127: case 134: case 141: case 148: case 155:
	       case 162: case 169: case 176: case 183:
		  m_layer3left1->fill( smilephi, hi->zres );
		  m_layer3left1_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer3left1_matched->fill( smilephi, hi->zres );
		     m_layer3left1_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer3middle
	       case 65: case 72: case 79: case 86: case 93: case 100: case 107:
	       case 114: case 121: case 128: case 135: case 142: case 149: case 156:
	       case 163: case 170: case 177: case 184:
		  m_layer3middle->fill( smilephi, hi->zres );
		  m_layer3middle_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer3middle_matched->fill( smilephi, hi->zres );
		     m_layer3middle_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer3right1
	       case 66: case 73: case 80: case 87: case 94: case 101: case 108:
	       case 115: case 122: case 129: case 136: case 143: case 150: case 157:
	       case 164: case 171: case 178: case 185:
		  m_layer3right1->fill( smilephi, hi->zres );
		  m_layer3right1_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer3right1_matched->fill( smilephi, hi->zres );
		     m_layer3right1_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer3right2
	       case 67: case 74: case 81: case 88: case 95: case 102: case 109:
	       case 116: case 123: case 130: case 137: case 144: case 151: case 158:
	       case 165: case 172: case 179: case 186:
		  m_layer3right2->fill( smilephi, hi->zres );
		  m_layer3right2_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer3right2_matched->fill( smilephi, hi->zres );
		     m_layer3right2_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer3right3
	       case 68: case 75: case 82: case 89: case 96: case 103: case 110:
	       case 117: case 124: case 131: case 138: case 145: case 152: case 159:
	       case 166: case 173: case 180: case 187:
		  m_layer3right3->fill( smilephi, hi->zres );
		  m_layer3right3_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer3right3_matched->fill( smilephi, hi->zres );
		     m_layer3right3_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;

	    // layer4left5
	       case 188: case 198: case 208: case 218: case 228: case 238: case 248:
	       case 258: case 268: case 278: case 288: case 298: case 308: case 318:
	       case 328: case 338: case 348: case 358: case 368: case 378: case 388:
	       case 398: case 408: case 418: case 428: case 438:
		  m_layer4left5->fill( smilephi, hi->zres );
		  m_layer4left5_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4left5_matched->fill( smilephi, hi->zres );
		     m_layer4left5_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4left4
	       case 189: case 199: case 209: case 219: case 229: case 239: case 249:
	       case 259: case 269: case 279: case 289: case 299: case 309: case 319:
	       case 329: case 339: case 349: case 359: case 369: case 379: case 389:
	       case 399: case 409: case 419: case 429: case 439:
		  m_layer4left4->fill( smilephi, hi->zres );
		  m_layer4left4_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4left4_matched->fill( smilephi, hi->zres );
		     m_layer4left4_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4left3
	       case 190: case 200: case 210: case 220: case 230: case 240: case 250:
	       case 260: case 270: case 280: case 290: case 300: case 310: case 320:
	       case 330: case 340: case 350: case 360: case 370: case 380: case 390:
	       case 400: case 410: case 420: case 430: case 440:
		  m_layer4left3->fill( smilephi, hi->zres );
		  m_layer4left3_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4left3_matched->fill( smilephi, hi->zres );
		     m_layer4left3_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4left2
	       case 191: case 201: case 211: case 221: case 231: case 241: case 251:
	       case 261: case 271: case 281: case 291: case 301: case 311: case 321:
	       case 331: case 341: case 351: case 361: case 371: case 381: case 391:
	       case 401: case 411: case 421: case 431: case 441:
		  m_layer4left2->fill( smilephi, hi->zres );
		  m_layer4left2_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4left2_matched->fill( smilephi, hi->zres );
		     m_layer4left2_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4left1
	       case 192: case 202: case 212: case 222: case 232: case 242: case 252:
	       case 262: case 272: case 282: case 292: case 302: case 312: case 322:
	       case 332: case 342: case 352: case 362: case 372: case 382: case 392:
	       case 402: case 412: case 422: case 432: case 442:
		  m_layer4left1->fill( smilephi, hi->zres );
		  m_layer4left1_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4left1_matched->fill( smilephi, hi->zres );
		     m_layer4left1_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4right1
	       case 193: case 203: case 213: case 223: case 233: case 243: case 253:
	       case 263: case 273: case 283: case 293: case 303: case 313: case 323:
	       case 333: case 343: case 353: case 363: case 373: case 383: case 393:
	       case 403: case 413: case 423: case 433: case 443:
		  m_layer4right1->fill( smilephi, hi->zres );
		  m_layer4right1_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4right1_matched->fill( smilephi, hi->zres );
		     m_layer4right1_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4right2
	       case 194: case 204: case 214: case 224: case 234: case 244: case 254:
	       case 264: case 274: case 284: case 294: case 304: case 314: case 324:
	       case 334: case 344: case 354: case 364: case 374: case 384: case 394:
	       case 404: case 414: case 424: case 434: case 444:
		  m_layer4right2->fill( smilephi, hi->zres );
		  m_layer4right2_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4right2_matched->fill( smilephi, hi->zres );
		     m_layer4right2_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4right3
	       case 195: case 205: case 215: case 225: case 235: case 245: case 255:
	       case 265: case 275: case 285: case 295: case 305: case 315: case 325:
	       case 335: case 345: case 355: case 365: case 375: case 385: case 395:
	       case 405: case 415: case 425: case 435: case 445:
		  m_layer4right3->fill( smilephi, hi->zres );
		  m_layer4right3_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4right3_matched->fill( smilephi, hi->zres );
		     m_layer4right3_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4right4
	       case 196: case 206: case 216: case 226: case 236: case 246: case 256:
	       case 266: case 276: case 286: case 296: case 306: case 316: case 326:
	       case 336: case 346: case 356: case 366: case 376: case 386: case 396:
	       case 406: case 416: case 426: case 436: case 446:
		  m_layer4right4->fill( smilephi, hi->zres );
		  m_layer4right4_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4right4_matched->fill( smilephi, hi->zres );
		     m_layer4right4_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;
		  // layer4right5
	       case 197: case 207: case 217: case 227: case 237: case 247: case 257:
	       case 267: case 277: case 287: case 297: case 307: case 317: case 327:
	       case 337: case 347: case 357: case 367: case 377: case 387: case 397:
	       case 407: case 417: case 427: case 437: case 447:
		  m_layer4right5->fill( smilephi, hi->zres );
		  m_layer4right5_scat->fill( smilephi, hi->zres );
		  if ( rphimap[ladder-1] != NULL ) {
		     m_layer4right5_matched->fill( smilephi, hi->zres );
		     m_layer4right5_matched_scat->fill( smilephi, hi->zres );
		  }
		  break;

	    } // end switch sensor for which histogram
	 
	 } // end foreach z hit
      } // end if there are enough z layers

      report( DEBUG, kFacilityString ) << "begin cleanup" << endl;

      // clean up
      for ( surfaces_iter = surfaces_begin;
	    surfaces_iter != surfaces_end;
	    surfaces_iter++ ) {
	 (* surfaces_iter )->deleteCondition();
	 delete (* surfaces_iter );
      }

      for ( rphihits_iter = rphihits_begin;
	    rphihits_iter != rphihits_end;
	    rphihits_iter++ )
	 delete (* rphihits_iter );

      for ( zhits_iter = zhits_begin;
	    zhits_iter != zhits_end;
	    zhits_iter++ )
	 delete (* zhits_iter );

   } // end track loop

   report( DEBUG, kFacilityString ) << "all done!" << endl;
   return ActionBase::kPassed;
}

ActionBase::ActionResult
AlignmentPlots::startRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   HIMagField::instance()->setBfield( iFrame.record( Stream::kStartRun ) );

   return ActionBase::kPassed;
}


/*
ActionBase::ActionResult
AlignmentPlots::endRun( Frame& iFrame )         // anal4 equiv.
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

AP_rphiHit::AP_rphiHit()
   : phi( 0. )
   , trackz( 0. )
   , sensor( 0 )
   , ladder( 0 )
   , layer( 0 )
   , layerOrClam( 0 )
   , rphires( 0. )
   , rphires_err( 0. )
   , weighted_rphires( 0. )
   , hitx( 0. )
{ }

AP_zHit::AP_zHit()
   : phi( 0. )
   , trackz( 0. )
   , sensor( 0 )
   , ladder( 0 )
   , layer( 0 )
   , layerOrClam( 0 )
   , zres( 0. )
   , zres_err( 0. )
   , weighted_zres( 0. )
   , hitz( 0. )
{ }
