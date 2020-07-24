// -*- C++ -*-
//
// Package:     SquishChecker
// Module:      SquishChecker
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Feb  4 10:10:07 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "SquishChecker/SquishChecker.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "CleoDB/DBEventHeader.h"

#include "Navigation/NavTrack.h"
#include "TrackRoot/TRHelixFit.h"
#include "TrackRoot/TRTrackFitQuality.h"
#include "TrackRoot/TRSeedTrackQuality.h"
#include "TrackDelivery/TDKinematicFit.h"

#include "Navigation/NavShower.h"
#include "C3cc/CcShowerAttributes.h"

#include "TrackFitter/TrackFitDRHitLink.h"
#include "TrackFitter/TrackFitSVRHitLink.h"
#include "TrackFitter/TrackFitSVZHitLink.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

#include "TrackFitter/TrackFitCathodeHitLink.h"
#include "TrackFitter/TrackFitSVZHitLink.h"

#include "BeamSpot/BeamSpot.h"

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
static const char* const kFacilityString = "Processor.SquishChecker" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
SquishChecker::SquishChecker( void )               // anal1
   : Processor("SquishChecker")
   , m_crossingAngle("crossingAngle", this, 0.026)
   , m_requireZInfo("requireZInfo", this, false)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &SquishChecker::event,    Stream::kEvent );
   //bind( &SquishChecker::beginRun, Stream::kBeginRun );
   //bind( &SquishChecker::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

SquishChecker::~SquishChecker()                    // anal5
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
SquishChecker::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
SquishChecker::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
SquishChecker::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   report(NOTICE, kFacilityString) << "hist_book() one" << endl;

   m_curvmiss = iHistoManager.histogram(
      98, "1/2/curv1 + 1/2/curv2 for abs(cotTheta).in.(1.0, 1.4)", 60000, -0.6, 0.6);
   m_z0miss = iHistoManager.histogram(
      99, "z0 miss", 1000, -.1, .1);

   m_all_intersections = iHistoManager.histogram(
      50, "intersections of all tracks", 500, -0.01, 0.01, 500, -0.01, 0.01);
   m_positives_intersections = iHistoManager.histogram(
      51, "intersections of positive tracks", 500, -0.01, 0.01, 500, -0.01, 0.01);
   m_negatives_intersections = iHistoManager.histogram(
      52, "intersections of negative tracks", 500, -0.01, 0.01, 500, -0.01, 0.01);

   m_d0miss = iHistoManager.histogram(
      100, "d0 miss vs posphi0", 157, 0., 2*M_PI, 100, -1e-2, 1e-2);
   m_d0miss_prof = iHistoManager.profile(
      150, "d0 miss vs posphi0", 157, 0., 2*M_PI, -1e-2, 1e-2,
      HIHistProf::kErrorOnMean);
   m_d0miss_midtheta = iHistoManager.histogram(
      102, "d0 miss vs posphi0 abs(cotTheta).lt.0.5", 157, 0., 2*M_PI, 100, -1e-2, 1e-2);
   m_d0miss_midtheta_prof = iHistoManager.profile(
      152, "d0 miss vs posphi0 abs(cotTheta).lt.0.5", 157, 0., 2*M_PI, -1e-2, 1e-2,
      HIHistProf::kErrorOnMean);
   m_d0miss_fartheta = iHistoManager.histogram(
      103, "d0 miss vs posphi0 abs(cotTheta).gt.2", 157, 0., 2*M_PI, 100, -1e-2, 1e-2);
   m_d0miss_fartheta_prof = iHistoManager.profile(
      153, "d0 miss vs posphi0 abs(cotTheta).gt.2", 157, 0., 2*M_PI, -1e-2, 1e-2,
      HIHistProf::kErrorOnMean);
   m_d0miss1 = iHistoManager.histogram(
      101, "d0 miss vs 1phi0", 157, 0., 2*M_PI, 100, -1e-2, 1e-2);
   m_d0miss1_prof = iHistoManager.profile(
      151, "d0 miss vs 1phi0", 157, 0., 2*M_PI, -1e-2, 1e-2,
      HIHistProf::kErrorOnMean);

   m_pmag = iHistoManager.histogram(
      200, "pmag vs posphi0", 157, 0., 2*M_PI, 100, 0., 2.);
   m_pmag_prof = iHistoManager.profile(
      250, "pmag vs posphi0", 157, 0., 2*M_PI, 0., 2.,
      HIHistProf::kErrorOnMean);
   m_pmag1 = iHistoManager.histogram(
      201, "pmag vs 1phi0", 157, 0., 2*M_PI, 100, 0., 2.);
   m_pmag1_prof = iHistoManager.profile(
      251, "pmag vs 1phi0", 157, 0., 2*M_PI, 0., 2.,
      HIHistProf::kErrorOnMean);

   m_phi0miss = iHistoManager.histogram(
      300, "sin(posphi0 - negphi0) vs posphi0", 157, 0., 2*M_PI, 100, -1e-1, 1e-1);
   m_phi0miss_prof = iHistoManager.profile(
      350, "sin(posphi0 - negphi0) vs posphi0", 157, 0., 2*M_PI, -1e-1, 1e-1,
      HIHistProf::kErrorOnMean);
   m_phi0miss1 = iHistoManager.histogram(
      301, "sin(posphi0 - negphi0) vs 1phi0", 157, 0., 2*M_PI, 100, -1e-1, 1e-1);
   m_phi0miss1_prof = iHistoManager.profile(
      351, "sin(posphi0 - negphi0) vs 1phi0", 157, 0., 2*M_PI, -1e-1, 1e-1,
      HIHistProf::kErrorOnMean);

   report(NOTICE, kFacilityString) << "hist_book() two" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() three" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() four" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() five" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() six" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() seven" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() eight" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() nine" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() ten" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() eleven" << endl;

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

   report(NOTICE, kFacilityString) << "hist_book() twelve" << endl;

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
SquishChecker::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> track;
   extract(iFrame.record(Stream::kEvent), track);
   if ( track.size() != 2 ) return ActionBase::kFailed;

   FATable<NavShower> shower;
   extract(iFrame.record(Stream::kEvent), shower);
   int num_neutrals(0);
   FATable<NavShower>::const_iterator shower_end = shower.end();
   for ( FATable<NavShower>::const_iterator shower_iter = shower.begin();
	 shower_iter != shower_end;
	 ++shower_iter )
      if ( !shower_iter->attributes().hot()             &&
	   shower_iter->attributes().energy() > 0.030   &&
	   shower_iter->noTrackMatch()                  &&
	   shower_iter->noTrackMatchConReg()               )
	 num_neutrals++;
   if ( num_neutrals > 0 ) return ActionBase::kFailed;
   
   FATable<NavTrack>::const_iterator track_first = track.begin();
   FATable<NavTrack>::const_iterator track_second = track.begin();  ++track_second;

   if ( track_first->pionHelix()->curvature() *
	track_second->pionHelix()->curvature()  > 0. ) return ActionBase::kFailed;

   if ( track_first->pionFit()->momentum().mag() < 4.  ||
	track_second->pionFit()->momentum().mag() < 4.    ) return ActionBase::kFailed;

   if ( m_requireZInfo.value() ) {
      FAItem<PionFitCathodeHitLattice> cathode_lattice;
      extract(iFrame.record(Stream::kEvent), cathode_lattice);
      FAItem<PionFitSVZHitLattice> svz_lattice;
      extract(iFrame.record(Stream::kEvent), svz_lattice);

      const PionFitCathodeHitLattice::VectorRightID* track_first_cathode =
	 cathode_lattice->vRightGivenLeft(track_first->pionFit()->identifier());
      const PionFitCathodeHitLattice::VectorRightID* track_second_cathode =
	 cathode_lattice->vRightGivenLeft(track_second->pionFit()->identifier());

      const PionFitSVZHitLattice::VectorRightID* track_first_svz =
	 svz_lattice->vRightGivenLeft(track_first->pionFit()->identifier());
      const PionFitSVZHitLattice::VectorRightID* track_second_svz =
	 svz_lattice->vRightGivenLeft(track_second->pionFit()->identifier());

      if ( track_first_cathode == 0  	    ||  track_second_cathode == 0         ||
	   track_first_svz == 0      	    ||  track_second_svz == 0             ||
	   track_first_cathode->size() < 1  ||  track_second_cathode->size() < 1  ||
	   track_first_svz->size() < 2      ||  track_second_svz->size() < 2         )
	 return ActionBase::kFailed;
   }

   report(INFO, kFacilityString) << "cotTheta_1 = " << track_first->pionHelix()->cotTheta()
				 << " cotTheta_2 = " << track_second->pionHelix()->cotTheta() << endl;
   if ( abs(track_first->pionHelix()->cotTheta()) > 1.   &&  abs(track_first->pionHelix()->cotTheta()) < 1.4   &&
	abs(track_second->pionHelix()->cotTheta()) > 1.  &&  abs(track_second->pionHelix()->cotTheta()) < 1.4     )
      m_curvmiss->fill(1./2./track_first->pionHelix()->curvature() +
		       1./2./track_second->pionHelix()->curvature()   );
   m_z0miss->fill(track_first->pionHelix()->z0() + track_second->pionHelix()->z0());

   if ( abs(track_first->pionHelix()->z0() + track_second->pionHelix()->z0()) > 0.10 )
      return ActionBase::kFailed;

//    FAItem<BeamSpot> beamspot;
//    extract(iFrame.record(Stream::kBeginRun), beamspot);

   KTHelix nobs_first(*track_first->pionHelix());
   KTHelix nobs_second(*track_second->pionHelix());

   if ( m_positives.size() < 5010 )
      m_positives.push_back(nobs_first.curvature() > 0. ? nobs_first : nobs_second);
   if ( m_negatives.size() < 5010 )
      m_negatives.push_back(nobs_first.curvature() < 0. ? nobs_first : nobs_second);

   if ( m_positives.size() == 5000 ) {
      report(SYSTEM, kFacilityString)
	 << "5000th positive track: calculating intersections..." << endl;
      for ( int i = 0;  i < 5000;  i++ )
	 for ( int j = i + 1;  j < 5000;  j++ ) {
	    double tx(-1000.), ty(-1000.);
	    if ( abs(sin(m_positives[i].phi0() - m_positives[j].phi0())) > 0.05  &&
		 calc_intersection(tx, ty, m_positives[i], m_positives[j])          ) {
	       m_positives_intersections->fill(tx, ty);
	       m_all_intersections->fill(tx, ty);
	    } // end if calc_intersection is okay
	 } // end j loop
      report(SYSTEM, kFacilityString)
	 << "done calculating intersections!" << endl;
   } // end if we're on the 500th positive track

   if ( m_negatives.size() == 5000 ) {
      report(SYSTEM, kFacilityString)
	 << "5000th negative track: calculating intersections..." << endl;
      for ( int i = 0;  i < 5000;  i++ )
	 for ( int j = i + 1;  j < 5000;  j++ ) {
	    double tx(-1000.), ty(-1000.);
	    if ( abs(sin(m_negatives[i].phi0() - m_negatives[j].phi0())) > 0.05  &&
		 calc_intersection(tx, ty, m_negatives[i], m_negatives[j])          ) {
	       m_negatives_intersections->fill(tx, ty);
	       m_all_intersections->fill(tx, ty);
	    } // end if calc_intersection is okay
	 } // end j loop
      report(SYSTEM, kFacilityString)
	 << "done calculating intersections!" << endl;
   } // end if we're on the 500th negative track

   if ( m_positives.size() == 5000  &&  m_negatives.size() == 5000 ) {
      report(SYSTEM, kFacilityString)
	 << "10000th track: calculating intersections..." << endl;
      for ( int i = 0;  i < 5000;  i++ )
	 for ( int j = 0;  j < 5000;  j++ ) {
	    double tx(-1000.), ty(-1000.);
	    if ( abs(sin(m_positives[i].phi0() - m_negatives[j].phi0())) > 0.05  &&
		 calc_intersection(tx, ty, m_positives[i], m_negatives[j])          )
	       m_all_intersections->fill(tx, ty);
	 } // end j loop
      report(SYSTEM, kFacilityString)
	 << "done calculating intersections!" << endl;
   } // end if we're on the 1000th track

   double distanceMoved;
   nobs_first.moveToReferencePoint(HepPoint3D(-0.00006,0.0008959,0.), distanceMoved);
   nobs_second.moveToReferencePoint(HepPoint3D(-0.00006,0.0008959,0.), distanceMoved);

//    // This is a loop over the upper triangle of the helix-helix
//    // matrix.
//    int i, j;
//    int nhelix( m_helix.size() );
//    for ( i = 0;  i < nhelix;  i++ )
//       for ( j = i + 1;  j < nhelix;  j++ ) {
// 	 double tx(-1000.), ty(-1000.);
	 
// 	 // Don't bother with any failed intersections
// 	 if ( calc_intersection( tx, ty, m_helix[i], m_helix[j] ) ) {
// 	    report(SYSTEM, kFacilityString) << tx << " " << ty << endl;
// 	 } // end if intersection succeeded

//       } // end loop over pairs of helices

   double posPhi0(track_first->pionHelix()->curvature() > 0. ?
		  track_first->pionHelix()->phi0() :
		  track_second->pionHelix()->phi0());

   double d0miss(nobs_first.d0() +
		 nobs_second.d0()  );

   double pmag((track_first->pionFit()->momentum() +
		track_second->pionFit()->momentum() +
		Hep3Vector(m_crossingAngle.value(), 0., 0.)
		  ).mag());

   double phi0miss((track_first->pionHelix()->curvature() > 0. ? 1. : -1. ) *
		   sin(track_first->pionHelix()->phi0() -
		       track_second->pionHelix()->phi0()));

   m_d0miss->fill(posPhi0, d0miss);
   m_d0miss_prof->fill(posPhi0, d0miss);
   if ( abs(track_first->pionHelix()->cotTheta()) < 0.5  &&
	abs(track_second->pionHelix()->cotTheta()) < 0.5    ) {
      m_d0miss_midtheta->fill(posPhi0, d0miss);
      m_d0miss_midtheta_prof->fill(posPhi0, d0miss);
   }
   else if ( abs(track_first->pionHelix()->cotTheta()) > 2.  &&
	     abs(track_second->pionHelix()->cotTheta()) > 2.    ) {
      m_d0miss_fartheta->fill(posPhi0, d0miss);
      m_d0miss_fartheta_prof->fill(posPhi0, d0miss);
   }
   m_d0miss1->fill(track_first->pionHelix()->phi0(), d0miss);
   m_d0miss1_prof->fill(track_first->pionHelix()->phi0(), d0miss);

   m_pmag->fill(posPhi0, pmag);
   m_pmag_prof->fill(posPhi0, pmag);
   m_pmag1->fill(track_first->pionHelix()->phi0(), pmag);
   m_pmag1_prof->fill(track_first->pionHelix()->phi0(), pmag);

   m_phi0miss->fill(posPhi0, phi0miss);
   m_phi0miss_prof->fill(posPhi0, phi0miss);
   m_phi0miss1->fill(track_first->pionHelix()->phi0(), phi0miss);
   m_phi0miss1_prof->fill(track_first->pionHelix()->phi0(), phi0miss);

   report( DEBUG, kFacilityString ) << "doing what DRWiggle used to do..." << endl;

   FAItem<DBEventHeader> header;
   extract(iFrame.record(Stream::kEvent), header);
   report( DEBUG, kFacilityString ) << "extracted DBEventHeader" << endl;

   FAItem<ASiStore> store;
   extract(iFrame.record(Stream::kSVAlignment), store);
   report( DEBUG, kFacilityString ) << "extracted ASiStore" << endl;

   HISiSurfaceFactory siSurfaceFactory(iFrame);
   report( DEBUG, kFacilityString ) << "created HISiSurfaceFactory" << endl;

   FAItem<PionFitDRHitLattice> drlattice;
   extract(iFrame.record(Stream::kEvent), drlattice);
   report( DEBUG, kFacilityString ) << "extracted PionFitDRHitLattice" << endl;

   FATable<CalibratedDRHit> drhits;
   extract(iFrame.record(Stream::kEvent), drhits);
   report( DEBUG, kFacilityString ) << "extracted CalibratedDRHits" << endl;

   FATable<NavTrack>::const_iterator track_end = track.end();
   for ( FATable<NavTrack>::const_iterator track_iter = track.begin();
	 track_iter != track_end;
	 ++track_iter ) {
      report( DEBUG, kFacilityString ) << "for each track" << endl;

      HIHelix* helix(new HIHelix(*(track_iter->pionHelix())));

      double cotTheta(helix->cotTheta());
      double costh(cotTheta / sqrt(sqr(cotTheta) + 1.));
      DABoolean positive(helix->curvature() > 0.);
      report( DEBUG, kFacilityString ) << "  copied the helix and extracted cotTheta, curvature" << endl;

      const NavTrack::DRHitLinkTable* drhitlinks(track_iter->pionDRHitLinks());
      NavTrack::DRHitLinkTable::const_iterator drhitlink_end = drhitlinks->end();
      for ( NavTrack::DRHitLinkTable::const_iterator drhitlink_iter = drhitlinks->begin();
	    drhitlink_iter != drhitlink_end;
	    ++drhitlink_iter ) {
	 double dr_residual((*drhitlink_iter)->linkData().signedDcaToWire());

	 if ( positive ) {
	    switch ( (drhits.find(*((*drhitlink_iter)->vRightID().begin())))->layer() ) {
	       case 17:  m_dr17_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 18:  m_dr18_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 19:  m_dr19_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 20:  m_dr20_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 21:  m_dr21_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 22:  m_dr22_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 23:  m_dr23_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 24:  m_dr24_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 25:  m_dr25_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 26:  m_dr26_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 27:  m_dr27_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 28:  m_dr28_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 29:  m_dr29_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 30:  m_dr30_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 31:  m_dr31_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 32:  m_dr32_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 33:  m_dr33_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 34:  m_dr34_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 35:  m_dr35_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 36:  m_dr36_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 37:  m_dr37_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 38:  m_dr38_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 39:  m_dr39_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 40:  m_dr40_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 41:  m_dr41_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 42:  m_dr42_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 43:  m_dr43_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 44:  m_dr44_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 45:  m_dr45_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 46:  m_dr46_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	       case 47:  m_dr47_vcotTh_pos->fill(cotTheta, dr_residual);  break;
	    } // end switch
	 } else { // negative
	    switch ( (drhits.find(*((*drhitlink_iter)->vRightID().begin())))->layer() ) {
	       case 17:  m_dr17_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 18:  m_dr18_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 19:  m_dr19_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 20:  m_dr20_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 21:  m_dr21_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 22:  m_dr22_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 23:  m_dr23_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 24:  m_dr24_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 25:  m_dr25_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 26:  m_dr26_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 27:  m_dr27_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 28:  m_dr28_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 29:  m_dr29_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 30:  m_dr30_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 31:  m_dr31_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 32:  m_dr32_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 33:  m_dr33_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 34:  m_dr34_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 35:  m_dr35_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 36:  m_dr36_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 37:  m_dr37_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 38:  m_dr38_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 39:  m_dr39_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 40:  m_dr40_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 41:  m_dr41_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 42:  m_dr42_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 43:  m_dr43_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 44:  m_dr44_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 45:  m_dr45_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 46:  m_dr46_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	       case 47:  m_dr47_vcotTh_neg->fill(cotTheta, dr_residual);  break;
	    } // end switch
	 } // end if positive/negative
     } // end loop over DR hits

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

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
SquishChecker::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
SquishChecker::endRun( Frame& iFrame )         // anal4 equiv.
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

DABoolean SquishChecker::calc_intersection( double& x, double& y, KTHelix& a, KTHelix& b )
{
   x = y = 1000.;  // To more easily find unchecked failures

   // Express the two helices in terms of (x - xa)^2 + (y - ya)^2 == ra^2
   double ra( 1./2./a.curvature() );
   double xa( -(a.d0() + ra) * sin(a.phi0()) );
   double ya( (a.d0() + ra) * cos(a.phi0()) );

   double rb( 1./2./b.curvature() );
   double xb( -(b.d0() + rb) * sin(b.phi0()) );
   double yb( (b.d0() + rb) * cos(b.phi0()) );

   // Zero or infinitely many solutions
   // I don't want to deal with either case
   if ( xa == xb  &&  ya == yb ) return false;

   double disc( (pow(4.*sqr(ra)*(xa - xb) - 
		     4.*(sqr(rb)*(xa - xb) + 
			(xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)))
		     ,2.) - 16.*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb))*
		 (pow(xa,4.) - 2.*sqr(xa)*sqr(xb) + pow(xb,4.) + 
		  2.*sqr(xb)*(sqr(ra) - sqr(rb) + sqr(ya - yb)) + 
		  2.*sqr(xa)*(-sqr(ra) + sqr(rb) + sqr(ya - yb)) + 
		  pow(-sqr(ra) + sqr(rb) + sqr(ya - yb),2.) - 
		  4.*sqr(rb)*sqr(ya - yb)))/
		(64.*pow(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb),2.)) );
   // no real solutions
   if ( disc < 0 ) return false;

   double pre( (sqr(rb)*(xa - xb) + sqr(ra)*(-xa + xb) + 
		(xa + xb)*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb)))/
	       (2.*(sqr(xa) - 2.*xa*xb + sqr(xb) + sqr(ya - yb))) );

   // The two x solutions
   double xsol1( pre + sqrt( disc ) );
   double xsol2( pre - sqrt( disc ) );

   // Two possibilities for the y solution to each x solution
   double ysol1a( yb + sqrt( sqr(rb) - sqr(xsol1 - xb) ) );
   double ysol1b( yb - sqrt( sqr(rb) - sqr(xsol1 - xb) ) );
   double ysol1;
   if ( abs( sqr(xsol1 - xa) + sqr(ysol1a - ya) - sqr(ra) ) <
	abs( sqr(xsol1 - xa) + sqr(ysol1b - ya) - sqr(ra) )   )
      ysol1 = ysol1a;
   else
      ysol1 = ysol1b;

   double ysol2a( yb + sqrt( sqr(rb) - sqr(xsol2 - xb) ) );
   double ysol2b( yb - sqrt( sqr(rb) - sqr(xsol2 - xb) ) );
   double ysol2;
   if ( abs( sqr(xsol2 - xa) + sqr(ysol2a - ya) - sqr(ra) ) <
	abs( sqr(xsol2 - xa) + sqr(ysol2b - ya) - sqr(ra) )   )
      ysol2 = ysol2a;
   else
      ysol2 = ysol2b;

   // Calculate the arclength of each solution along the two tracks.
   // Throw out a solution if the particle needs to travel (too far)
   // backward to get to it. If both solutions are okay, take the one
   // closest to the origin.
   DABoolean arclength1_ok( arclength_test( xsol1, ysol1, a, b ) );
   DABoolean arclength2_ok( arclength_test( xsol2, ysol2, a, b ) );
   int which_num;
   char which_let;
   
   if ( !arclength1_ok  &&  !arclength2_ok ) return false;

   else if ( arclength1_ok  &&  !arclength2_ok ) {
      x = xsol1;  y = ysol1;  which_num = 1;
      if ( abs(ysol1 - ysol1a) < abs(ysol1 - ysol1b) )
	 which_let = 'a';  else  which_let = 'b';
   }

   else if ( !arclength1_ok  &&  arclength2_ok ) {
      x = xsol2;  y = ysol2;  which_num = 2;
      if ( abs(ysol2 - ysol2a) < abs(ysol2 - ysol2b) )
	 which_let = 'a';  else  which_let = 'b';
   }      

   else if ( sqr(xsol1) + sqr(ysol1) < sqr(xsol2) + sqr(ysol2) ) {
      x = xsol1;  y = ysol1;  which_num = 1;
      if ( abs(ysol1 - ysol1a) < abs(ysol1 - ysol1b) )
	 which_let = 'a';  else  which_let = 'b';
   }

   else {
      x = xsol2;  y = ysol2;  which_num = 2;
      if ( abs(ysol2 - ysol2a) < abs(ysol2 - ysol2b) )
	 which_let = 'a';  else  which_let = 'b';
   }      

   return true;
}

DABoolean SquishChecker::arclength_test( double x, double y, KTHelix& a, KTHelix& b )
{
   double asinphi1( 2. * a.curvature() * (x * cos(a.phi0()) + y * sin(a.phi0())) );
   double acosphi1( 2. * a.curvature() * (x * sin(a.phi0()) - y * cos(a.phi0())
					  + a.d0()) + 1. );
   double aarclength( atan2(asinphi1, acosphi1) / 2. / a.curvature() );

   double bsinphi1( 2. * b.curvature() * (x * cos(b.phi0()) + y * sin(b.phi0())) );
   double bcosphi1( 2. * b.curvature() * (x * sin(b.phi0()) - y * cos(b.phi0())
					  + b.d0()) + 1. );
   double barclength( atan2(bsinphi1, bcosphi1) / 2. / b.curvature() );

//    return ( aarclength > -0.005  &&  aarclength < M_PI/2./abs(a.curvature()) )  &&
//       ( barclength > -0.005  &&  barclength < M_PI/2./abs(b.curvature()) );
   return ( aarclength > 0.  &&  aarclength < M_PI/2./abs(a.curvature()) )  &&
      ( barclength > 0.  &&  barclength < M_PI/2./abs(b.curvature()) );
}
