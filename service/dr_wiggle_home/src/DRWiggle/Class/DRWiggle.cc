// -*- C++ -*-
//
// Package:     DRWiggle
// Module:      DRWiggle
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Nov 19 16:18:57 EST 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DRWiggle/DRWiggle.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "CleoDB/DBEventHeader.h"
#include "Navigation/NavTrack.h"
#include "DualTrackHelices/DualTrackHelices.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "TrackRoot/TRHelixPionFit.h"

#include "TrackRoot/TRTrack.h"
#include "TrackRoot/TRHelixPionFit.h"
#include "TrackDelivery/TDKinematicPionFit.h"
#include "TrackRoot/TRTrackPionFitQuality.h"
#include "TrackFitter/TrackFitDRHitLink.h"
#include "TrackFitter/TrackFitSVRHitLink.h"
#include "TrackFitter/TrackFitSVZHitLink.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
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
static const char* const kFacilityString = "Processor.DRWiggle" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
DRWiggle::DRWiggle( void )               // anal1
   : Processor( "DRWiggle" )
   , m_trackUse( "trackUse", this, "dual" )
   , m_crossingAngle( "crossingAngle", this, 0.026 )
   , m_getDuals( "getDuals", this, true )
   , m_getResiduals( "getResiduals", this, true )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &DRWiggle::event,    Stream::kEvent );
   //bind( &DRWiggle::beginRun, Stream::kBeginRun );
   //bind( &DRWiggle::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

DRWiggle::~DRWiggle()                    // anal5
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
DRWiggle::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DRWiggle::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
DRWiggle::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_d0 = iHistoManager.histogram(
      10, "d0", 100, -0.01, 0.01 );
   m_d0_v_phi0 = iHistoManager.profile(
      20, "d0 VS phi0", 79, 0., 6.28318530718, -0.01, 0.01, HIHistProf::kErrorOnMean );
   m_z0 = iHistoManager.histogram(
      30, "z0", 100, -0.05, 0.05 );

   m_curvSum = iHistoManager.histogram(
      110, "curvature sum", 100, -0.005, 0.005 );
   m_sinPhi0Diff = iHistoManager.histogram(
      120, "sin(phi0 diff)", 100, -0.02, 0.02 );
   m_d0Sum = iHistoManager.histogram(
      130, "d0 sum", 100, -0.0005, 0.0005 );
   m_cotThetaSum = iHistoManager.histogram(
      140, "cotTheta sum", 100, -0.05, 0.05 );
   m_z0Diff = iHistoManager.histogram(
      150, "z0 diff", 100, -0.05, 0.05 );

   m_dual_curvSum = iHistoManager.histogram(
      210, "dual curvature sum", 100, -0.005, 0.005 );
   m_dual_sinPhi0Diff = iHistoManager.histogram(
      220, "dual sin(phi0 diff)", 100, -0.02, 0.02 );
   m_dual_d0Sum = iHistoManager.histogram(
      230, "dual d0 sum", 100, -0.0005, 0.0005 );
   m_dual_cotThetaSum = iHistoManager.histogram(
      240, "dual cotTheta sum", 100, -0.05, 0.05 );
   m_dual_z0Diff = iHistoManager.histogram(
      250, "dual z0 diff", 100, -0.05, 0.05 );

   m_d0missVSphi0 = iHistoManager.profile(
      300, "d0miss VS phi0", 157, 0., 6.28318530718, -0.001, 0.001, HIHistProf::kErrorOnMean);
   m_momentumXY = iHistoManager.histogram(
      310, "event momentum XY", 100, -.2, .2, 100, -.2, .2);
   m_momentumMagVSphi0 = iHistoManager.profile(
      320, "event momentum mag VS phi0", 157, 0., 6.28318530718, 0., 4., HIHistProf::kErrorOnMean);
   m_momentumAngVSphi0 = iHistoManager.profile(
      330, "sin(event momentum ang - phi0) VS phi0", 157, 0., 6.28318530718, -1., 1., HIHistProf::kErrorOnMean);
   m_momentumAngVSphi0_2d = iHistoManager.histogram(
      340, "event momentum ang VS phi0", 157, 0., 6.28318530718, 157, -3.1415926, 3.1415926);

   m_rphi_vphi_bac_end_pos = iHistoManager.profile(
      1100, "rphi resid VS phi, cos(theta) .lt. -0.83, pos", 628, 0., .28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_bac_mid_pos = iHistoManager.profile(
      1110, "rphi resid VS phi, -0.83 .lt. cos(theta) .lt. -0.77, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_bac_barrel_pos = iHistoManager.profile(
      1120, "rphi resid VS phi, -0.77 .lt. cos(theta) .lt. -0.68, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_for_barrel_pos = iHistoManager.profile(
      1130, "rphi resid VS phi, 0.68 .lt. cos(theta) .lt. 0.77, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_for_mid_pos = iHistoManager.profile(
      1140, "rphi resid VS phi, 0.77 .lt. cos(theta) .lt. 0.83, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_for_end_pos = iHistoManager.profile(
      1150, "rphi resid VS phi, 0.83 .lt. cos(theta), pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vphi_bac_end_pos = iHistoManager.profile(
      1200, "z resid VS phi, cos(theta) .lt. -0.83, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_bac_mid_pos = iHistoManager.profile(
      1210, "z resid VS phi, -0.83 .lt. cos(theta) .lt. -0.77, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_bac_barrel_pos = iHistoManager.profile(
      1220, "z resid VS phi, -0.77 .lt. cos(theta) .lt. -0.68, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_for_barrel_pos = iHistoManager.profile(
      1230, "z resid VS phi, 0.68 .lt. cos(theta) .lt. 0.77, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_for_mid_pos = iHistoManager.profile(
      1240, "z resid VS phi, 0.77 .lt. cos(theta) .lt. 0.83, pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_for_end_pos = iHistoManager.profile(
      1250, "z resid VS phi, 0.83 .lt. cos(theta), pos", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_pos = iHistoManager.profile(
      1300, "rphi resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_pos_left = iHistoManager.profile(
      1301, "rphi resid VS cotTheta, pos, left", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_pos_top = iHistoManager.profile(
      1302, "rphi resid VS cotTheta, pos, top", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_pos_right = iHistoManager.profile(
      1303, "rphi resid VS cotTheta, pos, right", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_pos_bottom = iHistoManager.profile(
      1304, "rphi resid VS cotTheta, pos, bottom", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_pos = iHistoManager.profile(
      1400, "z resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_pos_left = iHistoManager.profile(
      1401, "z resid VS cotTheta, pos, left", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_pos_top = iHistoManager.profile(
      1402, "z resid VS cotTheta, pos, top", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_pos_right = iHistoManager.profile(
      1403, "z resid VS cotTheta, pos, right", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_pos_bottom = iHistoManager.profile(
      1404, "z resid VS cotTheta, pos, bottom", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vphi_bac_end_neg = iHistoManager.profile(
      2100, "rphi resid VS phi, cos(theta) .lt. -0.83, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_bac_mid_neg = iHistoManager.profile(
      2110, "rphi resid VS phi, -0.83 .lt. cos(theta) .lt. -0.77, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_bac_barrel_neg = iHistoManager.profile(
      2120, "rphi resid VS phi, -0.77 .lt. cos(theta) .lt. -0.68, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_for_barrel_neg = iHistoManager.profile(
      2130, "rphi resid VS phi, 0.68 .lt. cos(theta) .lt. 0.77, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_for_mid_neg = iHistoManager.profile(
      2140, "rphi resid VS phi, 0.77 .lt. cos(theta) .lt. 0.83, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_vphi_for_end_neg = iHistoManager.profile(
      2150, "rphi resid VS phi, 0.83 .lt. cos(theta), neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vphi_bac_end_neg = iHistoManager.profile(
      2200, "z resid VS phi, cos(theta) .lt. -0.83, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_bac_mid_neg = iHistoManager.profile(
      2210, "z resid VS phi, -0.83 .lt. cos(theta) .lt. -0.77, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_bac_barrel_neg = iHistoManager.profile(
      2220, "z resid VS phi, -0.77 .lt. cos(theta) .lt. -0.68, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_for_barrel_neg = iHistoManager.profile(
      2230, "z resid VS phi, 0.68 .lt. cos(theta) .lt. 0.77, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_for_mid_neg = iHistoManager.profile(
      2240, "z resid VS phi, 0.77 .lt. cos(theta) .lt. 0.83, neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_z_vphi_for_end_neg = iHistoManager.profile(
      2250, "z resid VS phi, 0.83 .lt. cos(theta), neg", 628, 0., 6.28318530718,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_neg = iHistoManager.profile(
      2300, "rphi resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_neg_left = iHistoManager.profile(
      2301, "rphi resid VS cotTheta, neg, left", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_neg_top = iHistoManager.profile(
      2302, "rphi resid VS cotTheta, neg, top", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_neg_right = iHistoManager.profile(
      2303, "rphi resid VS cotTheta, neg, right", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_vcotTh_neg_bottom = iHistoManager.profile(
      2304, "rphi resid VS cotTheta, neg, bottom", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_neg = iHistoManager.profile(
      2400, "z resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_neg_left = iHistoManager.profile(
      2401, "z resid VS cotTheta, neg, left", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_neg_top = iHistoManager.profile(
      2402, "z resid VS cotTheta, neg, top", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_neg_right = iHistoManager.profile(
      2403, "z resid VS cotTheta, neg, right", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_z_vcotTh_neg_bottom = iHistoManager.profile(
      2404, "z resid VS cotTheta, neg, bottom", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_dr17_vcotTh_pos = iHistoManager.profile(
      3017, "DR layer 17 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr18_vcotTh_pos = iHistoManager.profile(
      3018, "DR layer 18 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr19_vcotTh_pos = iHistoManager.profile(
      3019, "DR layer 19 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr20_vcotTh_pos = iHistoManager.profile(
      3020, "DR layer 20 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr21_vcotTh_pos = iHistoManager.profile(
      3021, "DR layer 21 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr22_vcotTh_pos = iHistoManager.profile(
      3022, "DR layer 22 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr23_vcotTh_pos = iHistoManager.profile(
      3023, "DR layer 23 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr24_vcotTh_pos = iHistoManager.profile(
      3024, "DR layer 24 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr25_vcotTh_pos = iHistoManager.profile(
      3025, "DR layer 25 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr26_vcotTh_pos = iHistoManager.profile(
      3026, "DR layer 26 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr27_vcotTh_pos = iHistoManager.profile(
      3027, "DR layer 27 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr28_vcotTh_pos = iHistoManager.profile(
      3028, "DR layer 28 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr29_vcotTh_pos = iHistoManager.profile(
      3029, "DR layer 29 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr30_vcotTh_pos = iHistoManager.profile(
      3030, "DR layer 30 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr31_vcotTh_pos = iHistoManager.profile(
      3031, "DR layer 31 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr32_vcotTh_pos = iHistoManager.profile(
      3032, "DR layer 32 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr33_vcotTh_pos = iHistoManager.profile(
      3033, "DR layer 33 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr34_vcotTh_pos = iHistoManager.profile(
      3034, "DR layer 34 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr35_vcotTh_pos = iHistoManager.profile(
      3035, "DR layer 35 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr36_vcotTh_pos = iHistoManager.profile(
      3036, "DR layer 36 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr37_vcotTh_pos = iHistoManager.profile(
      3037, "DR layer 37 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr38_vcotTh_pos = iHistoManager.profile(
      3038, "DR layer 38 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr39_vcotTh_pos = iHistoManager.profile(
      3039, "DR layer 39 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr40_vcotTh_pos = iHistoManager.profile(
      3040, "DR layer 40 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr41_vcotTh_pos = iHistoManager.profile(
      3041, "DR layer 41 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr42_vcotTh_pos = iHistoManager.profile(
      3042, "DR layer 42 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr43_vcotTh_pos = iHistoManager.profile(
      3043, "DR layer 43 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr44_vcotTh_pos = iHistoManager.profile(
      3044, "DR layer 44 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr45_vcotTh_pos = iHistoManager.profile(
      3045, "DR layer 45 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr46_vcotTh_pos = iHistoManager.profile(
      3046, "DR layer 46 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr47_vcotTh_pos = iHistoManager.profile(
      3047, "DR layer 47 resid VS cotTheta, pos", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_dr17_vcotTh_neg = iHistoManager.profile(
      4017, "DR layer 17 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr18_vcotTh_neg = iHistoManager.profile(
      4018, "DR layer 18 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr19_vcotTh_neg = iHistoManager.profile(
      4019, "DR layer 19 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr20_vcotTh_neg = iHistoManager.profile(
      4020, "DR layer 20 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr21_vcotTh_neg = iHistoManager.profile(
      4021, "DR layer 21 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr22_vcotTh_neg = iHistoManager.profile(
      4022, "DR layer 22 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr23_vcotTh_neg = iHistoManager.profile(
      4023, "DR layer 23 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr24_vcotTh_neg = iHistoManager.profile(
      4024, "DR layer 24 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr25_vcotTh_neg = iHistoManager.profile(
      4025, "DR layer 25 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr26_vcotTh_neg = iHistoManager.profile(
      4026, "DR layer 26 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr27_vcotTh_neg = iHistoManager.profile(
      4027, "DR layer 27 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr28_vcotTh_neg = iHistoManager.profile(
      4028, "DR layer 28 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr29_vcotTh_neg = iHistoManager.profile(
      4029, "DR layer 29 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr30_vcotTh_neg = iHistoManager.profile(
      4030, "DR layer 30 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr31_vcotTh_neg = iHistoManager.profile(
      4031, "DR layer 31 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr32_vcotTh_neg = iHistoManager.profile(
      4032, "DR layer 32 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr33_vcotTh_neg = iHistoManager.profile(
      4033, "DR layer 33 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr34_vcotTh_neg = iHistoManager.profile(
      4034, "DR layer 34 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr35_vcotTh_neg = iHistoManager.profile(
      4035, "DR layer 35 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr36_vcotTh_neg = iHistoManager.profile(
      4036, "DR layer 36 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr37_vcotTh_neg = iHistoManager.profile(
      4037, "DR layer 37 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr38_vcotTh_neg = iHistoManager.profile(
      4038, "DR layer 38 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr39_vcotTh_neg = iHistoManager.profile(
      4039, "DR layer 39 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr40_vcotTh_neg = iHistoManager.profile(
      4040, "DR layer 40 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr41_vcotTh_neg = iHistoManager.profile(
      4041, "DR layer 41 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr42_vcotTh_neg = iHistoManager.profile(
      4042, "DR layer 42 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr43_vcotTh_neg = iHistoManager.profile(
      4043, "DR layer 43 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr44_vcotTh_neg = iHistoManager.profile(
      4044, "DR layer 44 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr45_vcotTh_neg = iHistoManager.profile(
      4045, "DR layer 45 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr46_vcotTh_neg = iHistoManager.profile(
      4046, "DR layer 46 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_dr47_vcotTh_neg = iHistoManager.profile(
      4047, "DR layer 47 resid VS cotTheta, neg", 700, -3.5, 3.5,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
DRWiggle::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<DBEventHeader> header;
   extract(iFrame.record(Stream::kEvent), header);
   report( DEBUG, kFacilityString ) << "extracted DBEventHeader" << endl;

   FAItem<ASiStore> store;
   if ( ! m_getResiduals.value() ) {
      extract(iFrame.record(Stream::kSVAlignment), store);
      report( DEBUG, kFacilityString ) << "extracted ASiStore" << endl;
   }

   HISiSurfaceFactory siSurfaceFactory(iFrame);
   report( DEBUG, kFacilityString ) << "created HISiSurfaceFactory" << endl;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   report( DEBUG, kFacilityString ) << "extracted NavTrack" << endl;

   FATable<DualTrackHelices> duals;
   if ( m_getDuals.value() ) {
      extract(iFrame.record(Stream::kEvent), duals);
      report( DEBUG, kFacilityString ) << "extracted DualTrackHelices" << endl;
   }

//     FAItem<PionFitDRHitLattice> drlattice;
//     extract(iFrame.record(Stream::kEvent), drlattice);
//     report( DEBUG, kFacilityString ) << "extracted PionFitDRHitLattice" << endl;

//    FATable<CalibratedDRHit> drhits;
//    extract(iFrame.record(Stream::kEvent), drhits);
//    report( DEBUG, kFacilityString ) << "extracted CalibratedDRHits" << endl;

   report( DEBUG, kFacilityString ) << "ready to make fako-tracks" << endl;

   if ( tracks.size() != 2 ) return ActionBase::kFailed;

   FATable<NavTrack>::const_iterator track_first = tracks.begin();
   FATable<NavTrack>::const_iterator track_second = tracks.begin();
   ++track_second;
   FATable<DualTrackHelices>::const_iterator dual_first;
   FATable<DualTrackHelices>::const_iterator dual_second;
   if ( m_getDuals.value() ) {
      dual_first = duals.begin();
      dual_second = duals.begin();
      ++dual_second;
   }

   report( DEBUG, kFacilityString )
      << "momenta: " << track_first->pionHelix()->momentum(Hep3Vector(0.,0.,14.944)).mag() << endl
      << track_second->pionHelix()->momentum(Hep3Vector(0.,0.,14.944)).mag() << endl;

   if ( track_first->pionHelix()->momentum(Hep3Vector(0.,0.,14.944)).mag() < 4.  ||
	track_second->pionHelix()->momentum(Hep3Vector(0.,0.,14.944)).mag() < 4.    )
      return ActionBase::kFailed;

   report( DEBUG, kFacilityString ) << "ready to fill fako-track histograms" << endl;

   m_curvSum->fill( track_first->pionHelix()->curvature() +
		    track_second->pionHelix()->curvature() );
   m_sinPhi0Diff->fill( sin( track_first->pionHelix()->phi0() -
			     track_second->pionHelix()->phi0() ) );
   m_d0Sum->fill( track_first->pionHelix()->d0() +
		  track_second->pionHelix()->d0() );
   m_cotThetaSum->fill( track_first->pionHelix()->cotTheta() +
			track_second->pionHelix()->cotTheta() );
   m_z0Diff->fill( track_first->pionHelix()->z0() -
		   track_second->pionHelix()->z0() );

   report( DEBUG, kFacilityString ) << "done filling fako-track histograms" << endl;

   if ( m_getDuals.value() ) {
      m_dual_curvSum->fill( dual_first->helix().curvature() +
			    dual_second->helix().curvature() );
      m_dual_sinPhi0Diff->fill( sin( dual_first->helix().phi0() -
				     dual_second->helix().phi0() ) );
      m_dual_d0Sum->fill( dual_first->helix().d0() +
			  dual_second->helix().d0() );
      m_dual_cotThetaSum->fill( dual_first->helix().cotTheta() +
				dual_second->helix().cotTheta() );
      m_dual_z0Diff->fill( dual_first->helix().z0() -
			   dual_second->helix().z0() );

      report( DEBUG, kFacilityString ) << "filled dual constraint monitor histograms" << endl;
   }

   report( DEBUG, kFacilityString ) << "calculating posPhi0" << endl;

   double posPhi0(track_first->pionHelix()->curvature() > 0. ?
		  track_first->pionHelix()->phi0() :
		  track_second->pionHelix()->phi0());

   report( DEBUG, kFacilityString ) << "filling d0missVSphi0" << endl;

   m_d0missVSphi0->fill(posPhi0,
			track_first->pionHelix()->d0() +
			track_second->pionHelix()->d0()  );
   
   report( DEBUG, kFacilityString ) << "calculating total momentum" << endl;

   Hep3Vector totalMomentum(track_first->pionHelix()->momentum(Hep3Vector(0.,0.,14.944)) +
			    track_second->pionHelix()->momentum(Hep3Vector(0.,0.,14.944)) +
			    Hep3Vector(m_crossingAngle.value(),0.,0.));

   report( DEBUG, kFacilityString ) << "filling total momentum histograms" << endl;

   m_momentumXY->fill(totalMomentum.x(), totalMomentum.y());
   m_momentumMagVSphi0->fill(posPhi0, totalMomentum.mag());
   m_momentumAngVSphi0->fill(posPhi0, sin(totalMomentum.phi() - posPhi0));
   m_momentumAngVSphi0_2d->fill(posPhi0, totalMomentum.phi());

   report( DEBUG, kFacilityString ) << "time for MC jobs to skip out" << endl;

   if ( ! m_getResiduals.value() ) return ActionBase::kPassed;

   FATable<DualTrackHelices>::const_iterator dual_iter;
   for ( FATable<NavTrack>::const_iterator track_iter = tracks.begin();
	 track_iter != tracks.end();
	 ++track_iter ) {
      report( DEBUG, kFacilityString ) << "for each track" << endl;
      if ( m_getDuals.value() ) {
	 dual_iter = duals.find(track_iter->identifier());
	 report( DEBUG, kFacilityString ) << "found dual_iter" << endl;
      }

      HIHelix* helix( NULL );
      if ( m_trackUse.value() == string("dual") )
	 helix = new HIHelix(dual_iter->helix());
      else if ( m_trackUse.value() == string("seed") )
	 helix = new HIHelix((KTHelix)(*(track_iter->seedTrack())));
      else if ( m_trackUse.value() == string("pion") )
	 helix = new HIHelix(*(track_iter->pionHelix()));
      else {
	 report(EMERGENCY, kFacilityString) << "What is a " << m_trackUse.value() << " track?" << endl;
	 assert(false);
      }

      // Should we move to the beamspot?  No!
//        double distanceMoved;
//        helix->moveToReferencePoint(
//  	 HepPoint3D( -1.3651e-3, 6.0302e-4, 0. ), distanceMoved );
      double cotTheta(helix->cotTheta());
      double costh(cotTheta / sqrt(sqr(cotTheta) + 1.));
      DABoolean positive(helix->curvature() > 0.);
      report( DEBUG, kFacilityString ) << "  copied the helix and extracted cotTheta, curvature" << endl;

      m_d0->fill(helix->d0());
      m_d0_v_phi0->fill(helix->phi0(), helix->d0());
      m_z0->fill(helix->z0());

//       const NavTrack::DRHitLinkTable* drhitlinks(track_iter->pionDRHitLinks());
//       NavTrack::DRHitLinkTable::const_iterator drhitlink_end = drhitlinks->end();
//       for ( NavTrack::DRHitLinkTable::const_iterator drhitlink_iter = drhitlinks->begin();
// 	    drhitlink_iter != drhitlink_end;
// 	    ++drhitlink_iter ) {
// 	 double dr_residual((*drhitlink_iter)->linkData().signedDcaToWire());

// 	 if ( positive ) {
// 	    switch ( (drhits.find(*((*drhitlink_iter)->vRightID().begin())))->layer() ) {
// 	       case 17:  m_dr17_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 18:  m_dr18_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 19:  m_dr19_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 20:  m_dr20_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 21:  m_dr21_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 22:  m_dr22_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 23:  m_dr23_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 24:  m_dr24_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 25:  m_dr25_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 26:  m_dr26_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 27:  m_dr27_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 28:  m_dr28_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 29:  m_dr29_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 30:  m_dr30_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 31:  m_dr31_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 32:  m_dr32_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 33:  m_dr33_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 34:  m_dr34_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 35:  m_dr35_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 36:  m_dr36_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 37:  m_dr37_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 38:  m_dr38_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 39:  m_dr39_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 40:  m_dr40_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 41:  m_dr41_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 42:  m_dr42_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 43:  m_dr43_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 44:  m_dr44_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 45:  m_dr45_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 46:  m_dr46_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	       case 47:  m_dr47_vcotTh_pos->fill(cotTheta, dr_residual);  break;
// 	    } // end switch
// 	 } else { // negative
// 	    switch ( (drhits.find(*((*drhitlink_iter)->vRightID().begin())))->layer() ) {
// 	       case 17:  m_dr17_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 18:  m_dr18_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 19:  m_dr19_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 20:  m_dr20_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 21:  m_dr21_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 22:  m_dr22_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 23:  m_dr23_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 24:  m_dr24_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 25:  m_dr25_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 26:  m_dr26_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 27:  m_dr27_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 28:  m_dr28_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 29:  m_dr29_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 30:  m_dr30_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 31:  m_dr31_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 32:  m_dr32_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 33:  m_dr33_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 34:  m_dr34_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 35:  m_dr35_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 36:  m_dr36_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 37:  m_dr37_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 38:  m_dr38_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 39:  m_dr39_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 40:  m_dr40_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 41:  m_dr41_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 42:  m_dr42_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 43:  m_dr43_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 44:  m_dr44_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 45:  m_dr45_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 46:  m_dr46_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	       case 47:  m_dr47_vcotTh_neg->fill(cotTheta, dr_residual);  break;
// 	    } // end switch
// 	 } // end if positive/negative
//       } // end loop over DR hits

      report( DEBUG, kFacilityString ) << "  filled d0, d0 vs phi0, and z0 histograms" << endl;

      vector< const CalibratedSVRphiHit* > rphi_hits( *track_iter->pionSVRHits() );
      vector< const CalibratedSVZHit* > z_hits( *track_iter->pionSVZHits() );
      if ( rphi_hits.size() == 0  ||  z_hits.size() == 0 ) break;

      report( DEBUG, kFacilityString ) << "  re-referenced rphi_hits and z_hits vectors" << endl;

      report( DEBUG, kFacilityString )
	 << "  rphi_hits: " << rphi_hits.size() << endl;
      for ( vector< const CalibratedSVRphiHit* >::const_iterator tmp1 = rphi_hits.begin();
	    tmp1 != rphi_hits.end();
	    ++tmp1 )
	 report( DEBUG, kFacilityString )
	    << "    " << (* tmp1)->sensor() << endl;

      report( DEBUG, kFacilityString )
	 << "  z_hits: " << z_hits.size() << endl;
      for ( vector< const CalibratedSVZHit* >::const_iterator tmp2 = z_hits.begin();
	    tmp2 != z_hits.end();
	    ++tmp2 )
	 report( DEBUG, kFacilityString )
	    << "    " << (* tmp2)->sensor() << endl;
      report( DEBUG, kFacilityString )
	 << "hey hey hey!" << endl;

      siSurfaceFactory.generateCenterSurfacesWithHits( rphi_hits, z_hits, NULL );

      report( DEBUG, kFacilityString ) << "  generated center surfaces with hits" << endl;

      vector< HIIntersectionSurface* > surfaces;
      vector< HIIntersectionSurface* >::const_iterator surfaces_iter;
      siSurfaceFactory.transferSurfacesTo( surfaces, false );

      vector< HIIntersectionSurface* > composite_surface;
      composite_surface.push_back( (* surfaces.begin()) );
      for ( surfaces_iter = surfaces.begin(), surfaces_iter++;
 	    surfaces_iter != surfaces.end();
 	    surfaces_iter++ ) {
 	 HICompositeSurface* tmp_surface
 	    = ( (HICompositeSurface*) (* surfaces_iter) );
 	 ( (HICompositeSurface*) (* composite_surface.begin()) )
 	    ->addChildren( *tmp_surface );
      } // end loop over surfaces

      report( DEBUG, kFacilityString ) << "  did surface composite stuff" << endl;

      HIHelixIntersector intersector( composite_surface,
 				      HIHelixIntersector::kIncreasingRadius,
 				      helix );

      report( DEBUG, kFacilityString ) << "  set up intersector" << endl;

      for ( HIHelixIntersector::IntersectionStatus status =
	       intersector.swimToCurrentSurface(
		  KTMoveControl::kDirectionForward );
	    status == HIHelixIntersector::kIntersectionOK;
	    status = intersector.swimToNextIntersection(
	       KTMoveControl::kDirectionForward ) )
      {
	 HIIntersectionSurface* surface = intersector.currentSurface();
	 do { 
 	    report( DEBUG, kFacilityString ) << "    for each hit..." << endl;

	    double phi( helix->phi0() );
//  	    double z( helix->position().z() );
//  	    HepSymMatrix errorMatrix( helix->errorMatrix() );

	    report( DEBUG, kFacilityString ) << "    phi = " << helix->phi0() << endl;

	    const CalibratedHit* hit = surface->calibratedHit();
	    int sensor( ( (CalibratedSVHit*) (hit) )->sensor() );
	    int layer( store->layerForSensor( sensor ) );

	    report( DEBUG, kFacilityString ) << "    sensor, layer = " << sensor << ", " << layer << endl;

	    double residual( surface->dca( *helix ) );

	    report( DEBUG, kFacilityString ) << "    residual = " << residual << endl;

//  	    // The rest of this is needed to calculate the residual derivative
//  	    const ASiSensorInfo& sensorInfo( store->sensor( sensor ) );
//  	    HepVector3D measurement_direction;
//  	    if ( hit->deviceType() == CalibratedHit::SVR )
//  	       measurement_direction = -sensorInfo.unitVectorXInSi();
//  	    else if ( hit->deviceType() == CalibratedHit::SVZ )
//  	       measurement_direction = sensorInfo.unitVectorZInSi();
//  	    else assert( 0 );

//  	    HepVector3D X( helix->position() - helix->referencePoint() );
//  	    HepVector3D P( helix->momentumUnit() );
//  	    HepVector3D N( sensorInfo.unitVectorYInSi() );
//  	    double nDotPInv = 1. / N.dot( P );
//  	    double pt = P.perp();
//  	    HepVector3D zCrossPUnit( -P.y() / pt, P.x() / pt, 0. );

//  	    HepVector derivs( HIHelix::kNTrackParameters, 0 );

//  	    derivs( HIHelix::kD0 ) =
//  	       measurement_direction.dot(
//  		  zCrossPUnit - ( N.x() * zCrossPUnit.x() +
//  				  N.y() * zCrossPUnit.y() ) * nDotPInv * P );

//  	    derivs( HIHelix::kZ0 ) =
//  	       measurement_direction.z() - N.z() * nDotPInv *
//  	       measurement_direction.dot( P );

//  	    double residual_err2( 0. );
//  	    HepVector VD( HIHelix::kNTrackParameters, 0 );
//  	    for ( int m = 1;  m <= HIHelix::kNTrackParameters;  m++ )
//  	       for ( int n = 1;  n <= HIHelix::kNTrackParameters;  n++ )
//  		  VD( m ) += errorMatrix( m, n ) * derivs( n );
//  	    for ( int i = 1;  i <= HIHelix::kNTrackParameters;  i++ ) {
//  	       double derivi = derivs( i );
//  	       residual_err2 += sqr( derivi ) * errorMatrix.fast( i, i );

//  	       for ( int j = i + 1;  j <= HIHelix::kNTrackParameters;  j++ )
//  		  residual_err2 += 2. * derivi * derivs( j ) * errorMatrix.fast( j, i );
//  	    }
	    
	    ////////////////////////// if this is an RPHI hit
	    if ( hit->deviceType() == CalibratedHit::SVR ) {
	       const CalibratedSVRphiHit* rphi_hit = ((CalibratedSVRphiHit*) hit);
	       
	       report( DEBUG, kFacilityString ) << "    hit is rphi.  Filling histograms." << endl;
	       report(INFO, kFacilityString)
		  << "RPHI " << header->run() << "." << header->number() << "."
		  << track_iter->identifier() << "." << hit->identifier() << " "
		  << residual << " " << phi << " " << costh << endl;

	       if ( positive ) {

		  if ( costh < -0.83 ) {
		     m_rphi_vphi_bac_end_pos->fill(phi, residual);
		  } else if ( costh < -0.77 ) {
		     m_rphi_vphi_bac_mid_pos->fill(phi, residual);
		  } else if ( costh < -0.68 ) {
		     m_rphi_vphi_bac_barrel_pos->fill(phi, residual);
		  } else if ( costh < 0.68 ) {
		     // in-between region. Do nothing.
		  } else if ( costh < 0.77 ) {
		     m_rphi_vphi_for_barrel_pos->fill(phi, residual);
		  } else if ( costh < 0.83 ) {
		     m_rphi_vphi_for_mid_pos->fill(phi, residual);
		  } else {
		     m_rphi_vphi_for_end_pos->fill(phi, residual);
		  } // end switch costh region
		  m_rphi_vcotTh_pos->fill(cotTheta, residual);
		  if ( abs(sin(phi)) < sqrt(0.5) ) {  // left or right
		     if ( cos(phi) > 0. )
			m_rphi_vcotTh_pos_right->fill(cotTheta, residual);
		     else
			m_rphi_vcotTh_pos_left->fill(cotTheta, residual);
		  } // end if left or right
		  else { // top or bottom
		     if ( sin(phi) > 0. )
			m_rphi_vcotTh_pos_top->fill(cotTheta, residual);
		     else
			m_rphi_vcotTh_pos_bottom->fill(cotTheta, residual);
		  } // end if top or bottom

	       } else { // (negative)

		  if ( costh < -0.83 ) {
		     m_rphi_vphi_bac_end_neg->fill(phi, residual);
		  } else if ( costh < -0.77 ) {
		     m_rphi_vphi_bac_mid_neg->fill(phi, residual);
		  } else if ( costh < -0.68 ) {
		     m_rphi_vphi_bac_barrel_neg->fill(phi, residual);
		  } else if ( costh < 0.68 ) {
		     // in-between region. Do nothing.
		  } else if ( costh < 0.77 ) {
		     m_rphi_vphi_for_barrel_neg->fill(phi, residual);
		  } else if ( costh < 0.83 ) {
		     m_rphi_vphi_for_mid_neg->fill(phi, residual);
		  } else {
		     m_rphi_vphi_for_end_neg->fill(phi, residual);
		  } // end switch costh region
		  m_rphi_vcotTh_neg->fill(cotTheta, residual);
		  if ( abs(sin(phi)) < sqrt(0.5) ) {  // left or right
		     if ( cos(phi) > 0. )
			m_rphi_vcotTh_neg_right->fill(cotTheta, residual);
		     else
			m_rphi_vcotTh_neg_left->fill(cotTheta, residual);
		  } // end if left or right
		  else { // top or bottom
		     if ( sin(phi) > 0. )
			m_rphi_vcotTh_neg_top->fill(cotTheta, residual);
		     else
			m_rphi_vcotTh_neg_bottom->fill(cotTheta, residual);
		  } // end if top or bottom

	       } // end if positive/negative

	    } // end if this is an RPHI hit

	    ////////////////////////// if this is a Z hit
	    if ( hit->deviceType() == CalibratedHit::SVZ ) {
	       const CalibratedSVZHit* z_hit = ((CalibratedSVZHit*) hit);

	       report( DEBUG, kFacilityString ) << "    hit is z.  Filling histograms." << endl;
	       report(INFO, kFacilityString)
		  << "Z " << header->run() << "." << header->number() << "."
		  << track_iter->identifier() << "." << hit->identifier() << " "
		  << residual << " " << phi << " " << costh << endl;

	       if ( positive ) {

		  if ( costh < -0.83 ) {
		     m_z_vphi_bac_end_pos->fill(phi, residual);
		  } else if ( costh < -0.77 ) {
		     m_z_vphi_bac_mid_pos->fill(phi, residual);
		  } else if ( costh < -0.68 ) {
		     m_z_vphi_bac_barrel_pos->fill(phi, residual);
		  } else if ( costh < 0.68 ) {
		     // in-between region. Do nothing.
		  } else if ( costh < 0.77 ) {
		     m_z_vphi_for_barrel_pos->fill(phi, residual);
		  } else if ( costh < 0.83 ) {
		     m_z_vphi_for_mid_pos->fill(phi, residual);
		  } else {
		     m_z_vphi_for_end_pos->fill(phi, residual);
		  } // end switch costh region
		  m_z_vcotTh_pos->fill(cotTheta, residual);
		  if ( abs(sin(phi)) < sqrt(0.5) ) {  // left or right
		     if ( cos(phi) > 0. )
			m_z_vcotTh_pos_right->fill(cotTheta, residual);
		     else
			m_z_vcotTh_pos_left->fill(cotTheta, residual);
		  } // end if left or right
		  else { // top or bottom
		     if ( sin(phi) > 0. )
			m_z_vcotTh_pos_top->fill(cotTheta, residual);
		     else
			m_z_vcotTh_pos_bottom->fill(cotTheta, residual);
		  } // end if top or bottom

	       } else { // (negative)

		  if ( costh < -0.83 ) {
		     m_z_vphi_bac_end_neg->fill(phi, residual);
		  } else if ( costh < -0.77 ) {
		     m_z_vphi_bac_mid_neg->fill(phi, residual);
		  } else if ( costh < -0.68 ) {
		     m_z_vphi_bac_barrel_neg->fill(phi, residual);
		  } else if ( costh < 0.68 ) {
		     // in-between region. Do nothing.
		  } else if ( costh < 0.77 ) {
		     m_z_vphi_for_barrel_neg->fill(phi, residual);
		  } else if ( costh < 0.83 ) {
		     m_z_vphi_for_mid_neg->fill(phi, residual);
		  } else {
		     m_z_vphi_for_end_neg->fill(phi, residual);
		  } // end switch costh region
		  m_z_vcotTh_neg->fill(cotTheta, residual);
		  if ( abs(sin(phi)) < sqrt(0.5) ) {  // left or right
		     if ( cos(phi) > 0. )
			m_z_vcotTh_neg_right->fill(cotTheta, residual);
		     else
			m_z_vcotTh_neg_left->fill(cotTheta, residual);
		  } // end if left or right
		  else { // top or bottom
		     if ( sin(phi) > 0. )
			m_z_vcotTh_neg_top->fill(cotTheta, residual);
		     else
			m_z_vcotTh_neg_bottom->fill(cotTheta, residual);
		  } // end if top or bottom

	       } // end if positive/negative

	    } // end if this is a z hit
	 } while ( surface->advanceToNextCalibratedHit( *helix ) );  // end do-while
      } // end (for) loop over surfaces

      report( DEBUG, kFacilityString ) << "  Finished with all hits." << endl;

      vector< HIIntersectionSurface* >::const_iterator surfaces_end = surfaces.end();
      for ( surfaces_iter = surfaces.begin();
	    surfaces_iter != surfaces_end;
	    surfaces_iter++ ) {
	 report( DEBUG, kFacilityString ) << "  Ready to delete surfaces_iter..." << endl;
	 (* surfaces_iter)->deleteCondition();
	 report( DEBUG, kFacilityString ) << "    sent deleteCondition()" << endl;
	 delete (* surfaces_iter );
	 report( DEBUG, kFacilityString ) << "    deleted surface" << endl;
      }
      report( DEBUG, kFacilityString ) << "  Finished deleting surfaces!" << endl;
      delete helix;
      report( DEBUG, kFacilityString ) << "  Deleted helix!" << endl;
   } // end loop over tracks

   
   
   




   report( DEBUG, kFacilityString ) << "Finished everything.  Let's head for home." << endl;
   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
DRWiggle::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
DRWiggle::endRun( Frame& iFrame )         // anal4 equiv.
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


