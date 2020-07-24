// -*- C++ -*-
//
// Package:     FindBug
// Module:      FindBug
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
#include "FindBug/FindBug.h"
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
static const char* const kFacilityString = "Processor.FindBug" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.23 2001/10/31 19:48:05 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_02_00 $";

//
// static data member definitions
//


//
// constructors and destructor
//
FindBug::FindBug( void )               // anal1
   : Processor( "FindBug" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &FindBug::event,    Stream::kEvent );
   //bind( &FindBug::beginRun, Stream::kBeginRun );
   //bind( &FindBug::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

FindBug::~FindBug()                    // anal5
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
FindBug::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
FindBug::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
FindBug::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_both_rphi_residual = iHistoManager.histogram(
      10, "rphi residual (fitter VS. me)", 500, -0.001, 0.001, 500, -0.001, 0.001 );
   m_both_z_residual = iHistoManager.histogram(
      20, "z residual (fitter VS. me)", 500, -0.005, 0.005, 500, -0.005, 0.005 );
   m_rphi_difference = iHistoManager.histogram(
      110, "rphi residual difference (fitter - me)", 500, -0.001, 0.001 );
   m_z_difference = iHistoManager.histogram(
      120, "z residual difference (fitter - me)", 500, -0.005, 0.005 );
   m_rphi_ratio = iHistoManager.histogram(
      130, "rphi residual ratio log( fitter / me )", 500, -1., 2. );
   m_z_ratio = iHistoManager.histogram(
      140, "z residual ratio log( fitter / me )", 500, -1., 2. );
   m_rphi_diff_v_phi = iHistoManager.profile(
      210, "rphi residual difference (fitter - me) VS. phi",
      157, 0., 2.*3.1415926, -0.001, 0.001, HIHistProf::kErrorOnMean );
   m_z_diff_v_phi = iHistoManager.profile(
      220, "z residual difference (fitter - me) VS. phi",
      157, 0., 2.*3.1415926, -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_diff_v_phi0 = iHistoManager.profile(
      230, "rphi residual difference (fitter - me) VS. phi0",
      157, 0., 2.*3.1415926, -0.001, 0.001, HIHistProf::kErrorOnMean );
   m_z_diff_v_phi0 = iHistoManager.profile(
      240, "z residual difference (fitter - me) VS. phi0",
      157, 0., 2.*3.1415926, -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_rphi_diff_v_cotTheta = iHistoManager.profile(
      250, "rphi residual difference (fitter - me) VS. cotTheta",
      150, -1.5, 1.5, -0.001, 0.001, HIHistProf::kErrorOnMean );
   m_z_diff_v_cotTheta = iHistoManager.profile(
      260, "z residual difference (fitter - me) VS. cotTheta",
      150, -1.5, 1.5, -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_rphi_ratio_v_cotTheta = iHistoManager.profile(
      310, "rphi residual ratio log( fitter / me ) VS. cotTheta",
      150, -1.5, 1.5, -1., 2., HIHistProf::kErrorOnMean );
   m_z_ratio_v_cotTheta = iHistoManager.profile(
      320, "z residual ratio log( fitter / me ) VS. cotTheta",
      150, -1.5, 1.5, -1., 2., HIHistProf::kErrorOnMean );
   m_rphi_ratio_v_z0 = iHistoManager.profile(
      330, "rphi residual ratio log( fitter / me ) VS. z0",
      150, -0.02, 0.02, -1., 2., HIHistProf::kErrorOnMean );
   m_z_ratio_v_z0 = iHistoManager.profile(
      340, "z residual ratio log( fitter / me ) VS. z0",
      150, -0.02, 0.02, -1., 2., HIHistProf::kErrorOnMean );
   m_rphi_ratio_v_z = iHistoManager.profile(
      350, "rphi residual ratio log( fitter / me ) VS. z",
      150, -0.20, 0.20, -1., 2., HIHistProf::kErrorOnMean );
   m_rphi_ratio_v_z_layer1 = iHistoManager.profile(
      351, "rphi residual ratio log( fitter / me ) VS. z layer1",
      150, -0.075, 0.075, -1., 2., HIHistProf::kErrorOnMean );
   m_rphi_ratio_v_z_layer2 = iHistoManager.profile(
      352, "rphi residual ratio log( fitter / me ) VS. z layer2",
      150, -0.10, 0.10, -1., 2., HIHistProf::kErrorOnMean );
   m_rphi_ratio_v_z_layer3 = iHistoManager.profile(
      353, "rphi residual ratio log( fitter / me ) VS. z layer3",
      150, -0.125, 0.125, -1., 2., HIHistProf::kErrorOnMean );
   m_rphi_ratio_v_z_layer4 = iHistoManager.profile(
      354, "rphi residual ratio log( fitter / me ) VS. z layer4",
      150, -0.20, 0.20, -1., 2., HIHistProf::kErrorOnMean );
   m_z_ratio_v_z = iHistoManager.profile(
      360, "z residual ratio log( fitter / me ) VS. z",
      150, -0.20, 0.20, -1., 2., HIHistProf::kErrorOnMean );
   m_z_ratio_v_z_layer1 = iHistoManager.profile(
      361, "z residual ratio log( fitter / me ) VS. z layer1",
      150, -0.075, 0.075, -1., 2., HIHistProf::kErrorOnMean );
   m_z_ratio_v_z_layer2 = iHistoManager.profile(
      362, "z residual ratio log( fitter / me ) VS. z layer2",
      150, -0.10, 0.10, -1., 2., HIHistProf::kErrorOnMean );
   m_z_ratio_v_z_layer3 = iHistoManager.profile(
      363, "z residual ratio log( fitter / me ) VS. z layer3",
      150, -0.125, 0.125, -1., 2., HIHistProf::kErrorOnMean );
   m_z_ratio_v_z_layer4 = iHistoManager.profile(
      364, "z residual ratio log( fitter / me ) VS. z layer4",
      150, -0.20, 0.20, -1., 2., HIHistProf::kErrorOnMean );

   m_z_atan_ratio = iHistoManager.histogram(
      410, "z residual atan( fitter / me )",
      314, -3.1415926/2., 3.1415926/2. );
   m_z_atan_ratio_v_z = iHistoManager.profile(
      420, "z residual abs( atan( fitter / me ) ) VS. z",
      150, -0.20, 0.20, -3.1415926/2., 3.1415926/2.,
      HIHistProf::kErrorOnMean );
   m_z_atan_ratio_v_z_layer1 = iHistoManager.profile(
      421, "z residual abs( atan( fitter / me ) ) VS. z layer1",
      150, -0.075, 0.075, -3.1415926/2., 3.1415926/2.,
      HIHistProf::kErrorOnMean );
   m_z_atan_ratio_v_z_layer2 = iHistoManager.profile(
      422, "z residual abs( atan( fitter / me ) ) VS. z layer2",
      150, -0.10, 0.10, -3.1415926/2., 3.1415926/2.,
      HIHistProf::kErrorOnMean );
   m_z_atan_ratio_v_z_layer3 = iHistoManager.profile(
      423, "z residual abs( atan( fitter / me ) ) VS. z layer3",
      150, -0.125, 0.125, -3.1415926/2., 3.1415926/2.,
      HIHistProf::kErrorOnMean );
   m_z_atan_ratio_v_z_layer4 = iHistoManager.profile(
      424, "z residual abs( atan( fitter / me ) ) VS. z layer4",
      150, -0.20, 0.20, -3.1415926/2., 3.1415926/2.,
      HIHistProf::kErrorOnMean );

   m_fitter_rphi_residual = iHistoManager.histogram(
      1010, "fitter rphi residual", 500, -0.001, 0.001 );
   m_fitter_z_residual = iHistoManager.histogram(
      1020, "fitter z residual", 500, -0.005, 0.005 );

   m_fitter_layer1right1 = iHistoManager.profile(
      1100, "fitter layer1right1", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1right1_1 = iHistoManager.profile(
      1110, "fitter layer1right1 (-0.075.lt.z.lt.-0.0625)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1right1_2 = iHistoManager.profile(
      1120, "fitter layer1right1 (-0.0625.lt.z.lt.-0.05)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1right1_3 = iHistoManager.profile(
      1130, "fitter layer1right1 (-0.05.lt.z.lt.-0.0375)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1right1_4 = iHistoManager.profile(
      1140, "fitter layer1right1 (-0.0375.lt.z.lt.-0.025)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_fitter_layer1middle = iHistoManager.profile(
      1200, "fitter layer1middle", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1middle_1 = iHistoManager.profile(
      1210, "fitter layer1middle (-0.025.lt.-0.0125)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1middle_2 = iHistoManager.profile(
      1220, "fitter layer1middle (-0.0125.lt.0.)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1middle_3 = iHistoManager.profile(
      1230, "fitter layer1middle (0.lt.0.0125)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1middle_4 = iHistoManager.profile(
      1240, "fitter layer1middle (0.0125.lt.0.025)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_fitter_layer1left1 = iHistoManager.profile(
      1300, "fitter layer1left1", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1left1_1 = iHistoManager.profile(
      1310, "fitter layer1left1 (0.025.lt.z.lt.0.0375)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1left1_2 = iHistoManager.profile(
      1320, "fitter layer1left1 (0.0375.lt.z.lt.0.05)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1left1_3 = iHistoManager.profile(
      1330, "fitter layer1left1 (0.05.lt.z.lt.0.0625)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer1left1_4 = iHistoManager.profile(
      1340, "fitter layer1left1 (0.0625.lt.z.lt.0.075)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_fitter_layer2left2 = iHistoManager.profile(
      1410, "fitter layer2left2", 24, -0.6, 0.2,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer2left1 = iHistoManager.profile(
      1420, "fitter layer2left1", 24, -0.6, 0.2,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer2right1 = iHistoManager.profile(
      1430, "fitter layer2right1", 24, -0.6, 0.2,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer2right2 = iHistoManager.profile(
      1440, "fitter layer2right2", 24, -0.6, 0.2,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_fitter_layer3left2 = iHistoManager.profile(
      1510, "fitter layer3left2", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer3left1 = iHistoManager.profile(
      1520, "fitter layer3left1", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer3middle = iHistoManager.profile(
      1530, "fitter layer3middle", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer3right1 = iHistoManager.profile(
      1540, "fitter layer3right1", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer3right2 = iHistoManager.profile(
      1550, "fitter layer3right2", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_fitter_layer4left4 = iHistoManager.profile(
      1610, "fitter layer4left4", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer4left3 = iHistoManager.profile(
      1620, "fitter layer4left3", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer4left2 = iHistoManager.profile(
      1630, "fitter layer4left2", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer4left1 = iHistoManager.profile(
      1640, "fitter layer4left1", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer4right1 = iHistoManager.profile(
      1650, "fitter layer4right1", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer4right2 = iHistoManager.profile(
      1660, "fitter layer4right2", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer4right3 = iHistoManager.profile(
      1670, "fitter layer4right3", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_fitter_layer4right4 = iHistoManager.profile(
      1680, "fitter layer4right4", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_my_rphi_residual = iHistoManager.histogram(
      2010, "my rphi residual", 500, -0.001, 0.001 );
   m_my_z_residual = iHistoManager.histogram(
      2020, "my z residual", 500, -0.005, 0.005 );

   m_my_layer1right1 = iHistoManager.profile(
      2100, "my layer1right1", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1right1_1 = iHistoManager.profile(
      2110, "my layer1right1 (-0.075.lt.z.lt.-0.0625)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1right1_2 = iHistoManager.profile(
      2120, "my layer1right1 (-0.0625.lt.z.lt.-0.05)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1right1_3 = iHistoManager.profile(
      2130, "my layer1right1 (-0.05.lt.z.lt.-0.0375)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1right1_4 = iHistoManager.profile(
      2140, "my layer1right1 (-0.0375.lt.z.lt.-0.025)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_my_layer1middle = iHistoManager.profile(
      2200, "my layer1middle", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1middle_1 = iHistoManager.profile(
      2210, "my layer1middle (-0.025.lt.-0.0125)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1middle_2 = iHistoManager.profile(
      2220, "my layer1middle (-0.0125.lt.0.)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1middle_3 = iHistoManager.profile(
      2230, "my layer1middle (0.lt.0.0125)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1middle_4 = iHistoManager.profile(
      2240, "my layer1middle (0.0125.lt.0.025)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_my_layer1left1 = iHistoManager.profile(
      2300, "my layer1left1", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1left1_1 = iHistoManager.profile(
      2310, "my layer1left1 (0.025.lt.z.lt.0.0375)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1left1_2 = iHistoManager.profile(
      2320, "my layer1left1 (0.0375.lt.z.lt.0.05)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1left1_3 = iHistoManager.profile(
      2330, "my layer1left1 (0.05.lt.z.lt.0.0625)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer1left1_4 = iHistoManager.profile(
      2340, "my layer1left1 (0.0625.lt.z.lt.0.075)", 20, -0.6, 0.4,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_my_layer2left2 = iHistoManager.profile(
      2410, "my layer2left2", 24, -0.6, 0.2,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer2left1 = iHistoManager.profile(
      2420, "my layer2left1", 24, -0.6, 0.2,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer2right1 = iHistoManager.profile(
      2430, "my layer2right1", 24, -0.6, 0.2,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer2right2 = iHistoManager.profile(
      2440, "my layer2right2", 24, -0.6, 0.2,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_my_layer3left2 = iHistoManager.profile(
      2510, "my layer3left2", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer3left1 = iHistoManager.profile(
      2520, "my layer3left1", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer3middle = iHistoManager.profile(
      2530, "my layer3middle", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer3right1 = iHistoManager.profile(
      2540, "my layer3right1", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer3right2 = iHistoManager.profile(
      2550, "my layer3right2", 25, -0.4, 0.1,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

   m_my_layer4left4 = iHistoManager.profile(
      2610, "my layer4left4", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer4left3 = iHistoManager.profile(
      2620, "my layer4left3", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer4left2 = iHistoManager.profile(
      2630, "my layer4left2", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer4left1 = iHistoManager.profile(
      2640, "my layer4left1", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer4right1 = iHistoManager.profile(
      2650, "my layer4right1", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer4right2 = iHistoManager.profile(
      2660, "my layer4right2", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer4right3 = iHistoManager.profile(
      2670, "my layer4right3", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );
   m_my_layer4right4 = iHistoManager.profile(
      2680, "my layer4right4", 28, -0.26, 0.02,
      -0.005, 0.005, HIHistProf::kErrorOnMean );

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
FindBug::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< ASiStore > sistore;
   extract( iFrame.record( Stream::kSVAlignment ), sistore );

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

//////////////////////////////////////////////////////////// track finder part
   report( NOTICE, kFacilityString ) << "Calling track finder:" << endl;
   FATable< TRSeedTrack > seed_tracks;
   extract( iFrame.record( Stream::kEvent ), seed_tracks );
   report( NOTICE, kFacilityString )
      << "    Number of tracks = " << seed_tracks.size() << endl;
   if ( seed_tracks.size() != 2 ) return ActionBase::kFailed;

   FAItem< SeedTrackSVRHitLattice > seed_svr_lattice;
   extract( iFrame.record( Stream::kEvent ), seed_svr_lattice );
   FAItem< SeedTrackSVZHitLattice > seed_svz_lattice;
   extract( iFrame.record( Stream::kEvent ), seed_svz_lattice );

//////////////////////////////////////////////////////////// loop over seed tracks
   for ( FATable< TRSeedTrack >::const_iterator seed_track_iter
	    = seed_tracks.begin();
	 seed_track_iter != seed_tracks.end();
	 seed_track_iter++ )
   {
      HIHelix copy_helix = *seed_track_iter;
      HIHelix copy_helix2 = *seed_track_iter;
      report( NOTICE, kFacilityString )
	 << "    ==== Seed track " << seed_track_iter->identifier()
	 << "=======================================" << endl;
      report( NOTICE, kFacilityString )
	 << "    curvature = " << copy_helix.curvature()
	 << " phi0 = " << copy_helix.phi0()
	 << " d0 = " << copy_helix.d0()
	 << " cotTheta = " << copy_helix.cotTheta()
	 << " z0 = " << copy_helix.z0() << endl;

//////////////////////////////////////////////////////////// loop over rphi hits
      const SeedTrackSVRHitLattice::Links svr_links
	 = seed_svr_lattice->linksGivenLeft( seed_track_iter->identifier() );

      report( NOTICE, kFacilityString )
	 << "    Number of rphi hits on seed track #"
	 << seed_track_iter->identifier() << " = "
	 << svr_links.size() << endl;

      for ( SeedTrackSVRHitLattice::Links::const_iterator svr_link_iter
	       = svr_links.begin();
	    svr_link_iter != svr_links.end();
	    svr_link_iter++ )
      {
	 const CalibratedSVRphiHit& svr_hit
	    = calibrated_svr_hits[ (* svr_link_iter)->vRightID()[ 0 ] ];

	 report( NOTICE, kFacilityString )
	    << "        Rphi hit #" << svr_hit.identifier()
	    << " layer " << svr_hit.layer()
	    << " ladder " << sistore->ladderForSensor( svr_hit.sensor() )
	    << " sensor " << svr_hit.sensor()
	    << " hybrid " << svr_hit.hybrid() << endl;
	 report( NOTICE, kFacilityString )
	    << "        rphi coord = " << svr_hit.coord()
	    << " +/- " << svr_hit.coord_error() << endl;
	 report( NOTICE, kFacilityString )
	    << "        Local coordinates: "
	    << svr_hit.localcoord() << endl;
	 report( NOTICE, kFacilityString )
	    << "        World coordinates: "
	    << svr_hit.worldcoord( *sistore ) << endl;

	 report( NOTICE, kFacilityString )
	    << "        Unit vectors: \\hat{x} = "
	    << sistore->sensor( svr_hit.sensor() ).unitVectorXInSi() << endl;
	 report( NOTICE, kFacilityString )
	    << "                      \\hat{y} = "
	    << sistore->sensor( svr_hit.sensor() ).unitVectorYInSi() << endl;
	 report( NOTICE, kFacilityString )
	    << "                      \\hat{z} = "
	    << sistore->sensor( svr_hit.sensor() ).unitVectorZInSi() << endl;

	 copy_helix.moveToRadius( svr_hit.worldcoord( *sistore ).perp() );
	 report( NOTICE, kFacilityString )
	    << "        Track at radius of hit: position = "
	    << copy_helix.referencePoint()
	    << " phi = " << copy_helix.phi0() << endl;

//  	 HepPoint3D pcaBend = copy_helix2.pointOfClosestApproachBend(
//  	    svr_hit.worldcoord( *sistore ) );
//  	 double dcaBend = copy_helix2.distanceOfClosestApproachBend(
//  	    svr_hit.worldcoord( *sistore ) );
//  	 report( NOTICE, kFacilityString )
//  	    << "        pointOfClosestApproachBend = " << pcaBend << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "        distanceOfClosestApproachBend = " << dcaBend << endl;

	 if ( false )
	 {
	    char in_char;
	    NextType next_type = kError;
	    do {
	       report( NOTICE, kFacilityString )
		  << "Enter \"h\", \"t\", \"e\" or \"q\""
		  << " (to step to the next hit, track, event or to quit)" << endl;
	       cin >> in_char;
	       if ( in_char == 'h' ) next_type = kHit;
	       else if ( in_char == 't' ) next_type = kTrack;
	       else if ( in_char == 'e' ) next_type = kEvent;
	       else if ( in_char == 'q' ) next_type = kEnd;
	    } while ( next_type == kError );

	    if ( next_type == kHit ) ;
	    if ( next_type == kTrack ) break;
	    if ( next_type == kEvent ) return ActionBase::kPassed;
	    if ( next_type == kEnd ) assert( false );
	 } // endif we stop here
	 report( NOTICE, kFacilityString ) << endl;
	 
      } // end loop over rphi hit links (seedtrack)

//////////////////////////////////////////////////////////// loop over z hits
      const SeedTrackSVZHitLattice::Links svz_links
	 = seed_svz_lattice->linksGivenLeft( seed_track_iter->identifier() );

      report( NOTICE, kFacilityString )
	 << "    Number of z hits on seed track #"
	 << seed_track_iter->identifier() << " = "
	 << svz_links.size() << endl;

      for ( SeedTrackSVZHitLattice::Links::const_iterator svz_link_iter
	       = svz_links.begin();
	    svz_link_iter != svz_links.end();
	    svz_link_iter++ )
      {
	 const CalibratedSVZHit& svz_hit
	    = calibrated_svz_hits[ (* svz_link_iter)->vRightID()[ 0 ] ];

	 report( NOTICE, kFacilityString )
	    << "        Z hit #" << svz_hit.identifier()
	    << " layer " << svz_hit.layer()
	    << " ladder " << sistore->ladderForSensor( svz_hit.sensor() )
	    << " sensor " << svz_hit.sensor()
	    << " hybrid " << svz_hit.hybrid() << endl;
	 report( NOTICE, kFacilityString )
	    << "        z coord = " << svz_hit.coord()
	    << " +/- " << svz_hit.coord_error() << endl;
	 report( NOTICE, kFacilityString )
	    << "        Local coordinates: "
	    << svz_hit.localcoord() << endl;
	 report( NOTICE, kFacilityString )
	    << "        World coordinates: "
	    << svz_hit.worldcoord( *sistore ) << endl;

	 report( NOTICE, kFacilityString )
	    << "        Unit vectors: \\hat{x} = "
	    << sistore->sensor( svz_hit.sensor() ).unitVectorXInSi() << endl;
	 report( NOTICE, kFacilityString )
	    << "                      \\hat{y} = "
	    << sistore->sensor( svz_hit.sensor() ).unitVectorYInSi() << endl;
	 report( NOTICE, kFacilityString )
	    << "                      \\hat{z} = "
	    << sistore->sensor( svz_hit.sensor() ).unitVectorZInSi() << endl;

	 copy_helix.moveToRadius( svz_hit.worldcoord( *sistore ).perp() );
	 report( NOTICE, kFacilityString )
	    << "        Track at radius of hit: position = "
	    << copy_helix.referencePoint()
	    << " phi = " << copy_helix.phi0() << endl;

//  	 HepPoint3D pcaBend = copy_helix2.pointOfClosestApproachBend(
//  	    svz_hit.worldcoord( *sistore ) );
//  	 double dcaBend = copy_helix2.distanceOfClosestApproachBend(
//  	    svz_hit.worldcoord( *sistore ) );
//  	 report( NOTICE, kFacilityString )
//  	    << "        pointOfClosestApproachBend = " << pcaBend << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "        distanceOfClosestApproachBend = " << dcaBend << endl;

	 if ( false )
	 {
	    char in_char;
	    NextType next_type = kError;
	    do {
	       report( NOTICE, kFacilityString )
		  << "Enter \"h\", \"t\", \"e\" or \"q\""
		  << " (to step to the next hit, track, event or to quit)" << endl;
	       cin >> in_char;
	       if ( in_char == 'h' ) next_type = kHit;
	       else if ( in_char == 't' ) next_type = kTrack;
	       else if ( in_char == 'e' ) next_type = kEvent;
	       else if ( in_char == 'q' ) next_type = kEnd;
	    } while ( next_type == kError );

	    if ( next_type == kHit ) ;
	    if ( next_type == kTrack ) break;
	    if ( next_type == kEvent ) return ActionBase::kPassed;
	    if ( next_type == kEnd ) assert( false );
	 } // endif we stop here
	 report( NOTICE, kFacilityString ) << endl;
	 
      } // end loop over z hit links (seedtrack)

      report( NOTICE, kFacilityString ) << endl;
   } // end loop over seed tracks

//////////////////////////////////////////////////////////// track fitter part
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

//////////////////////////////////////////////////////////// loop over fitted tracks
   for ( FATable< TRHelixMuonFit >::const_iterator muonfit_iter
	    = muonfit_tracks.begin();
	 muonfit_iter != muonfit_tracks.end();
	 muonfit_iter++ )
   {
      HIHelix copy_helix = *muonfit_iter;
//        HIHelix copy_helix2 = *muonfit_iter;
      HIHelix swimming_track = *muonfit_iter;
      report( NOTICE, kFacilityString )
	 << "    ==== Fitted track " << muonfit_iter->identifier()
	 << "=====================================" << endl;

      double phi0 = copy_helix.phi0();
      double cotTheta = copy_helix.cotTheta();
      double z0 = copy_helix.z0();
      report( NOTICE, kFacilityString )
	 << "    curvature = " << copy_helix.curvature()
	 << " phi0 = " << phi0
	 << " d0 = " << copy_helix.d0()
	 << " cotTheta = " << cotTheta
	 << " z0 = " << copy_helix.z0() << endl;

      const MuonFitSVRHitLattice::Links svr_links
	 = muonfit_svr_lattice->linksGivenLeft( muonfit_iter->identifier() );

      const MuonFitSVZHitLattice::Links svz_links
	 = muonfit_svz_lattice->linksGivenLeft( muonfit_iter->identifier() );

//////////////////////////////////////////////////////////// surfaces loop

      unsigned int surf_howmany = 0;
      vector< int > surf_identifier;
      vector< DABoolean > surf_isRphi;
      vector< double > surf_track_phi;
      vector< HepPoint3D > surf_track_position;
      vector< double > surf_residual;

      surf_identifier.clear();  surf_isRphi.clear();
      surf_track_phi.clear();  surf_track_position.clear();  surf_residual.clear();

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

      vector< HIIntersectionSurface* > surfaces;
      surfaces.clear();
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

	    surf_howmany++;
	    surf_identifier.push_back( surface->calibratedHit()->identifier() );
	    surf_isRphi.push_back(
	       surface->calibratedHit()->deviceType() == CalibratedHit::SVR );
	    surf_track_phi.push_back( swimming_track.phi0() );
	    surf_track_position.push_back( swimming_track.position() );
	    surf_residual.push_back( 0. - surface->dca( swimming_track ) );

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

//////////////////////////////////////////////////////////// loop over rphi hits
      report( NOTICE, kFacilityString )
	 << "    Number of rphi hits on fitted track #"
	 << muonfit_iter->identifier() << " = "
	 << svr_links.size() << endl;

      for ( MuonFitSVRHitLattice::Links::const_iterator svr_link_iter
	       = svr_links.begin();
	    svr_link_iter != svr_links.end();
	    svr_link_iter++ )
      {
	 const CalibratedSVRphiHit& svr_hit
	    = calibrated_svr_hits[ (* svr_link_iter)->vRightID()[ 0 ] ];

	 report( NOTICE, kFacilityString )
	    << "        Rphi hit #" << svr_hit.identifier()
	    << " layer " << svr_hit.layer()
	    << " ladder " << sistore->ladderForSensor( svr_hit.sensor() )
	    << " sensor " << svr_hit.sensor()
	    << " hybrid " << svr_hit.hybrid() << endl;
	 report( NOTICE, kFacilityString )
	    << "        rphi coord = " << svr_hit.coord()
	    << " +/- " << svr_hit.coord_error() << endl;
	 report( NOTICE, kFacilityString )
	    << "        Local coordinates: "
	    << svr_hit.localcoord() << endl;
	 report( NOTICE, kFacilityString )
	    << "        World coordinates: "
	    << svr_hit.worldcoord( *sistore ) << endl;

	 report( NOTICE, kFacilityString )
	    << "        Unit vectors: \\hat{x} = "
	    << sistore->sensor( svr_hit.sensor() ).unitVectorXInSi() << endl;
	 report( NOTICE, kFacilityString )
	    << "                      \\hat{y} = "
	    << sistore->sensor( svr_hit.sensor() ).unitVectorYInSi() << endl;
	 report( NOTICE, kFacilityString )
	    << "                      \\hat{z} = "
	    << sistore->sensor( svr_hit.sensor() ).unitVectorZInSi() << endl;

	 copy_helix.moveToRadius( svr_hit.worldcoord( *sistore ).perp() );
	 report( NOTICE, kFacilityString )
	    << "        Track at radius of hit: position = "
	    << copy_helix.referencePoint()
	    << " phi = " << copy_helix.phi0() << endl;

	 double fitter_rphires = (* svr_link_iter)->linkData().residual();
	 report( NOTICE, kFacilityString )
	    << "        Rphi residual of hit to track (from fitter) = "
	    << fitter_rphires << " +/- "
	    << (* svr_link_iter)->linkData().residualError() << endl;
	 m_fitter_rphi_residual->fill( (* svr_link_iter)->linkData().residual() );

//  	 HepPoint3D pcaBend = copy_helix2.pointOfClosestApproachBend(
//  	    svr_hit.worldcoord( *sistore ) );
//  	 double dcaBend = copy_helix2.distanceOfClosestApproachBend(
//  	    svr_hit.worldcoord( *sistore ) );
//  	 report( NOTICE, kFacilityString )
//  	    << "        pointOfClosestApproachBend = " << pcaBend << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "        distanceOfClosestApproachBend = " << dcaBend << endl;

	 for ( int i = 0;  i < surf_howmany;  i++ )
	    if ( surf_isRphi[i]  &&  surf_identifier[i] == svr_hit.identifier() )
	    {
	       report( NOTICE, kFacilityString )
		  << "        My surface-intersector calculation of residual = "
		  << surf_residual[i] << endl;
	       report( NOTICE, kFacilityString )
		  << "            with track position = " << surf_track_position[i]
		  << " and phi = " << surf_track_phi[i] << endl;

	       double my_rphires = surf_residual[i];
	       m_my_rphi_residual->fill( my_rphires );
	       m_both_rphi_residual->fill( my_rphires, fitter_rphires );
	       double diff_rphires = fitter_rphires - my_rphires;
	       m_rphi_difference->fill( diff_rphires );
	       if ( abs( fitter_rphires ) > 1.e-10  &&
		    abs( my_rphires ) > 1.e-10      &&
		    fitter_rphires * my_rphires > 0.  )
	       {
		  double ratio = log10( fitter_rphires / my_rphires );
		  m_rphi_ratio->fill( ratio );
		  m_rphi_ratio_v_cotTheta->fill( cotTheta, ratio );
		  m_rphi_ratio_v_z0->fill( z0, ratio );
		  double z = surf_track_position[i].z();
		  m_rphi_ratio_v_z->fill( z, ratio );
		  switch ( svr_hit.layer() )
		  {
		     case 1:
			m_rphi_ratio_v_z_layer1->fill( z, ratio );
			break;

		     case 2:
			m_rphi_ratio_v_z_layer2->fill( z, ratio );
			break;

		     case 3:
			m_rphi_ratio_v_z_layer3->fill( z, ratio );
			break;

		     case 4:
			m_rphi_ratio_v_z_layer4->fill( z, ratio );
			break;

		  } // end switch layer
	       } // end no extreme residuals

	       m_rphi_diff_v_phi->fill( surf_track_phi[i], diff_rphires );
	       m_rphi_diff_v_phi0->fill( phi0, diff_rphires );
	       m_rphi_diff_v_cotTheta->fill( cotTheta, diff_rphires );

	       break;
	    }

	 if ( false )
	 {
	    char in_char;
	    NextType next_type = kError;
	    do {
	       report( NOTICE, kFacilityString )
		  << "Enter \"h\", \"t\", \"e\" or \"q\""
		  << " (to step to the next hit, track, event or to quit)" << endl;
	       cin >> in_char;
	       if ( in_char == 'h' ) next_type = kHit;
	       else if ( in_char == 't' ) next_type = kTrack;
	       else if ( in_char == 'e' ) next_type = kEvent;
	       else if ( in_char == 'q' ) next_type = kEnd;
	    } while ( next_type == kError );

	    if ( next_type == kHit ) ;
	    if ( next_type == kTrack ) break;
	    if ( next_type == kEvent ) return ActionBase::kPassed;
	    if ( next_type == kEnd ) assert( false );
	 } // endif we stop here
	 report( NOTICE, kFacilityString ) << endl;
	 
      } // end loop over rphi hit links (seedtrack)

//////////////////////////////////////////////////////////// loop over z hits
      report( NOTICE, kFacilityString )
	 << "    Number of z hits on fitted track #"
	 << muonfit_iter->identifier() << " = "
	 << svz_links.size() << endl;

      for ( MuonFitSVZHitLattice::Links::const_iterator svz_link_iter
	       = svz_links.begin();
	    svz_link_iter != svz_links.end();
	    svz_link_iter++ )
      {
	 const CalibratedSVZHit& svz_hit
	    = calibrated_svz_hits[ (* svz_link_iter)->vRightID()[ 0 ] ];

	 report( NOTICE, kFacilityString )
	    << "        Z hit #" << svz_hit.identifier()
	    << " layer " << svz_hit.layer()
	    << " ladder " << sistore->ladderForSensor( svz_hit.sensor() )
	    << " sensor " << svz_hit.sensor()
	    << " hybrid " << svz_hit.hybrid() << endl;
	 report( NOTICE, kFacilityString )
	    << "        z coord = " << svz_hit.coord()
	    << " +/- " << svz_hit.coord_error() << endl;
	 report( NOTICE, kFacilityString )
	    << "        Local coordinates: "
	    << svz_hit.localcoord() << endl;
	 report( NOTICE, kFacilityString )
	    << "        World coordinates: "
	    << svz_hit.worldcoord( *sistore ) << endl;

	 report( NOTICE, kFacilityString )
	    << "        Unit vectors: \\hat{x} = "
	    << sistore->sensor( svz_hit.sensor() ).unitVectorXInSi() << endl;
	 report( NOTICE, kFacilityString )
	    << "                      \\hat{y} = "
	    << sistore->sensor( svz_hit.sensor() ).unitVectorYInSi() << endl;
	 report( NOTICE, kFacilityString )
	    << "                      \\hat{z} = "
	    << sistore->sensor( svz_hit.sensor() ).unitVectorZInSi() << endl;

	 copy_helix.moveToRadius( svz_hit.worldcoord( *sistore ).perp() );
	 report( NOTICE, kFacilityString )
	    << "        Track at radius of hit: position = "
	    << copy_helix.referencePoint()
	    << " phi = " << copy_helix.phi0() << endl;

	 double fitter_zres = (* svz_link_iter)->linkData().residual();
	 report( NOTICE, kFacilityString )
	    << "        Z residual of hit to track (from fitter) = "
	    << fitter_zres << " +/- "
	    << (* svz_link_iter)->linkData().residualError() << endl;
	 m_fitter_z_residual->fill( fitter_zres );

//  	 HepPoint3D pcaBend = copy_helix2.pointOfClosestApproachBend(
//  	    svz_hit.worldcoord( *sistore ) );
//  	 double dcaBend = copy_helix2.distanceOfClosestApproachBend(
//  	    svz_hit.worldcoord( *sistore ) );
//  	 report( NOTICE, kFacilityString )
//  	    << "        pointOfClosestApproachBend = " << pcaBend << endl;
//  	 report( NOTICE, kFacilityString )
//  	    << "        distanceOfClosestApproachBend = " << dcaBend << endl;

	 DABoolean hasSurf = false;
	 double z, phi, my_zres;
	 unsigned int layer = svz_hit.layer();
	 unsigned int ladder = sistore->ladderForSensor( svz_hit.sensor() );
	 unsigned int sensor = svz_hit.sensor();
	 for ( int i = 0;  i < surf_howmany;  i++ )
	    if ( ! surf_isRphi[i]  &&  surf_identifier[i] == svz_hit.identifier() )
	    {
	       report( NOTICE, kFacilityString )
		  << "        My surface-intersector calculation of residual = "
		  << surf_residual[i] << endl;
	       report( NOTICE, kFacilityString )
		  << "            with track position = " << surf_track_position[i]
		  << " and phi = " << surf_track_phi[i] << endl;

	       my_zres = surf_residual[i];
	       m_my_z_residual->fill( my_zres );
	       m_both_z_residual->fill( my_zres, fitter_zres );
	       hasSurf = true;
	       z = surf_track_position[i].z();
	       phi = surf_track_phi[i];

	       double diff_zres = fitter_zres - my_zres;
	       m_z_difference->fill( diff_zres );
	       if ( abs( fitter_zres ) > 1.e-10  &&
		    abs( my_zres ) > 1.e-10      &&
		    fitter_zres * my_zres > 0.      )
	       {
		  double ratio = log10( fitter_zres / my_zres );
		  m_z_ratio->fill( ratio );
		  m_z_ratio_v_cotTheta->fill( cotTheta, ratio );
		  m_z_ratio_v_z0->fill( z0, ratio );
		  m_z_ratio_v_z->fill( z, ratio );
		  switch ( layer )
		  {
		     case 1:
			m_z_ratio_v_z_layer1->fill( z, ratio );
			break;

		     case 2:
			m_z_ratio_v_z_layer2->fill( z, ratio );
			break;

		     case 3:
			m_z_ratio_v_z_layer3->fill( z, ratio );
			break;

		     case 4:
			m_z_ratio_v_z_layer4->fill( z, ratio );
			break;

		  } // end switch layer
	       } // end no extreme residuals

	       if ( fitter_zres != 0.  &&  my_zres != 0. )
	       {
		  double atan_ratio = atan2( fitter_zres, my_zres );
		  m_z_atan_ratio->fill( atan_ratio );
		  atan_ratio = abs( atan_ratio );
		  m_z_atan_ratio_v_z->fill( z, atan_ratio );
		  switch ( layer )
		  {
		     case 1:  m_z_atan_ratio_v_z_layer1->fill( z, atan_ratio );
			break;
		     case 2:  m_z_atan_ratio_v_z_layer2->fill( z, atan_ratio );
			break;
		     case 3:  m_z_atan_ratio_v_z_layer3->fill( z, atan_ratio );
			break;
		     case 4:  m_z_atan_ratio_v_z_layer4->fill( z, atan_ratio );
			break;
		  } // end switch layer
	       } // end residuals not zero

	       m_z_diff_v_phi->fill( surf_track_phi[i], diff_zres );
	       m_z_diff_v_phi0->fill( phi0, diff_zres );
	       m_z_diff_v_cotTheta->fill( cotTheta, diff_zres );

	       break;
	    }

	 if ( hasSurf ) // I don't really expect this not to happen
	 {
	    const double l1phidiv = 2. * 3.1415926 / 7.;
	    const double l2phidiv = 2. * 3.1415926 / 10.;
	    const double l3phidiv = 2. * 3.1415926 / 18.;
	    const double l4phidiv = 2. * 3.1415926 / 26.;

	    switch ( sensor )
	    {
	       case 16: case 17: case 18: case 50: case 51: case 52: case 53:
	       case 153: case 154: case 155: case 156: case 157: case 158:
	       case 159: case 160: case 161: case 162: case 163: case 164:
	       case 165: case 166: case 378: case 379: case 380: case 381:
	       case 382: case 383: case 384: case 385: case 386: case 387:
	       case 388: case 389: case 390: case 391: case 392: case 393:
	       case 394: case 395: case 396: case 397:
		  if ( phi > 3.1415926 ) phi -= 2. * 3.1415926;
	    } // end if this is an antiphi sensor

	    if ( layer == 1 )
	    {
	       switch ( ladder )
	       {
		  case 1:  phi -= 2. * l1phidiv;  break;
		  case 2:  phi -= 3. * l1phidiv;  break;
		  case 3:  phi -= 4. * l1phidiv;  break;
		  case 4:  phi -= 5. * l1phidiv;  break;
		  case 5:  phi -= 6. * l1phidiv;  break;
		  case 6:  phi -= 0. * l1phidiv;  break;
		  case 7:  phi -= 1. * l1phidiv;  break;
	       } // end switch ladder

	       switch ( sensor )
	       {
		  case 3: case 6: case 9: case 12: case 15: case 18: case 21:
		     m_my_layer1right1->fill( phi, fitter_zres );
		     m_fitter_layer1right1->fill( phi, fitter_zres );

		     if ( z > -0.075  &&  z <= -0.0625 )
		     {
			m_my_layer1right1_1->fill( phi, my_zres );
			m_fitter_layer1right1_1->fill( phi, fitter_zres );
		     }
		     else if ( z > -0.0625  &&  z <= -0.05 )
		     {
			m_my_layer1right1_2->fill( phi, my_zres );
			m_fitter_layer1right1_2->fill( phi, fitter_zres );
		     }
		     else if ( z > -0.05  &&  z <= -0.0375 )
		     {
			m_my_layer1right1_3->fill( phi, my_zres );
			m_fitter_layer1right1_3->fill( phi, fitter_zres );
		     }
		     else if ( z > -0.0375  &&  z <= -0.025 )
		     {
			m_my_layer1right1_4->fill( phi, my_zres );
			m_fitter_layer1right1_4->fill( phi, fitter_zres );
		     } // end elsif chain for z position
		     break;

		  case 2: case 5: case 8: case 11: case 14: case 17: case 20:
		     m_my_layer1middle->fill( phi, fitter_zres );
		     m_fitter_layer1middle->fill( phi, fitter_zres );

		     if ( z > -0.025  &&  z <= -0.0125 )
		     {
			m_my_layer1middle_1->fill( phi, my_zres );
			m_fitter_layer1middle_1->fill( phi, fitter_zres );
		     }
		     else if ( z > -0.0125  &&  z <= 0. )
		     {
			m_my_layer1middle_2->fill( phi, my_zres );
			m_fitter_layer1middle_2->fill( phi, fitter_zres );
		     }
		     else if ( z > 0.  &&  z <= 0.0125 )
		     {
			m_my_layer1middle_3->fill( phi, my_zres );
			m_fitter_layer1middle_3->fill( phi, fitter_zres );
		     }
		     else if ( z > 0.0125  &&  z <= 0.025 )
		     {
			m_my_layer1middle_4->fill( phi, my_zres );
			m_fitter_layer1middle_4->fill( phi, fitter_zres );
		     } // end elsif chain for z position
		     break;

		  case 1: case 4: case 7: case 10: case 13: case 16: case 19:
		     m_my_layer1left1->fill( phi, my_zres );
		     m_fitter_layer1left1->fill( phi, fitter_zres );

		     if ( z > 0.025  &&  z <= 0.0375 )
		     {
			m_my_layer1left1_1->fill( phi, my_zres );
			m_fitter_layer1left1_1->fill( phi, fitter_zres );
		     }
		     else if ( z > 0.0375  &&  z <= 0.05 )
		     {
			m_my_layer1left1_2->fill( phi, my_zres );
			m_fitter_layer1left1_2->fill( phi, fitter_zres );
		     }
		     else if ( z > 0.05  &&  z <= 0.0625 )
		     {
			m_my_layer1left1_3->fill( phi, my_zres );
			m_fitter_layer1left1_3->fill( phi, fitter_zres );
		     }
		     else if ( z > 0.0625  &&  z <= 0.075 )
		     {
			m_my_layer1left1_4->fill( phi, my_zres );
			m_fitter_layer1left1_4->fill( phi, fitter_zres );
		     } // end elsif chain for z position
		     break;

	       } // end switch sensor
	    }
	    else if ( layer == 2 )
	    {
	       switch ( ladder )
	       {
		  case 8:  phi -= 3. * l2phidiv;  break;
		  case 9:  phi -= 4. * l2phidiv;  break;
		  case 10:  phi -= 5. * l2phidiv;  break;
		  case 11:  phi -= 6. * l2phidiv;  break;
		  case 12:  phi -= 7. * l2phidiv;  break;
		  case 13:  phi -= 8. * l2phidiv;  break;
		  case 14:  phi -= 9. * l2phidiv;  break;
		  case 15:  phi -= 0. * l2phidiv;  break;
		  case 16:  phi -= 1. * l2phidiv;  break;
		  case 17:  phi -= 2. * l2phidiv;  break;
	       } // end switch ladder

	       switch ( sensor )
	       {
		  case 22: case 26: case 30: case 34: case 38:
		  case 42: case 46: case 50: case 54: case 58:
		     m_my_layer2left2->fill( phi, my_zres );
		     m_fitter_layer2left2->fill( phi, fitter_zres );
		     break;

		  case 23: case 27: case 31: case 35: case 39:
		  case 43: case 47: case 51: case 55: case 59:
		     m_my_layer2left1->fill( phi, my_zres );
		     m_fitter_layer2left1->fill( phi, fitter_zres );
		     break;

		  case 24: case 28: case 32: case 36: case 40:
		  case 44: case 48: case 52: case 56: case 60:
		     m_my_layer2right1->fill( phi, my_zres );
		     m_fitter_layer2right1->fill( phi, fitter_zres );
		     break;

		  case 25: case 29: case 33: case 37: case 41:
		  case 45: case 49: case 53: case 57: case 61:
		     m_my_layer2right2->fill( phi, my_zres );
		     m_fitter_layer2right2->fill( phi, fitter_zres );
		     break;

	       } // end switch sensor
	    }
	    else if ( layer == 3 )
	    {
	       switch ( ladder )
	       {
		  case 18:  phi -= 5. * l3phidiv;  break;
		  case 19:  phi -= 6. * l3phidiv;  break;
		  case 20:  phi -= 7. * l3phidiv;  break;
		  case 21:  phi -= 8. * l3phidiv;  break;
		  case 22:  phi -= 9. * l3phidiv;  break;
		  case 23:  phi -= 10. * l3phidiv;  break;
		  case 24:  phi -= 11. * l3phidiv;  break;
		  case 25:  phi -= 12. * l3phidiv;  break;
		  case 26:  phi -= 13. * l3phidiv;  break;
		  case 27:  phi -= 14. * l3phidiv;  break;
		  case 28:  phi -= 15. * l3phidiv;  break;
		  case 29:  phi -= 16. * l3phidiv;  break;
		  case 30:  phi -= 17. * l3phidiv;  break;
		  case 31:  phi -= 0. * l3phidiv;  break;
		  case 32:  phi -= 1. * l3phidiv;  break;
		  case 33:  phi -= 2. * l3phidiv;  break;
		  case 34:  phi -= 3. * l3phidiv;  break;
		  case 35:  phi -= 4. * l3phidiv;  break;
	       } // end switch ladder

	       switch ( sensor )
	       {
		  case 63: case 70: case 77: case 84: case 91: case 98:
		  case 105: case 112: case 119: case 126: case 133: case 140:
		  case 147: case 154: case 161: case 168: case 175: case 182:
		     m_my_layer3left2->fill( phi, my_zres );
		     m_fitter_layer3left2->fill( phi, fitter_zres );
		     break;

		  case 64: case 71: case 78: case 85: case 92: case 99:
		  case 106: case 113: case 120: case 127: case 134: case 141:
		  case 148: case 155: case 162: case 169: case 176: case 183:
		     m_my_layer3left1->fill( phi, my_zres );
		     m_fitter_layer3left1->fill( phi, fitter_zres );
		     break;

		  case 65: case 72: case 79: case 86: case 93: case 100:
		  case 107: case 114: case 121: case 128: case 135: case 142:
		  case 149: case 156: case 163: case 170: case 177: case 184:
		     m_my_layer3middle->fill( phi, my_zres );
		     m_fitter_layer3middle->fill( phi, fitter_zres );
		     break;

		  case 66: case 73: case 80: case 87: case 94: case 101:
		  case 108: case 115: case 122: case 129: case 136: case 143:
		  case 150: case 157: case 164: case 171: case 178: case 185:
		     m_my_layer3right1->fill( phi, my_zres );
		     m_fitter_layer3right1->fill( phi, fitter_zres );
		     break;

		  case 67: case 74: case 81: case 88: case 95: case 102:
		  case 109: case 116: case 123: case 130: case 137: case 144:
		  case 151: case 158: case 165: case 172: case 179: case 186:
		     m_my_layer3right2->fill( phi, my_zres );
		     m_fitter_layer3right2->fill( phi, fitter_zres );
		     break;

	       } // end switch sensor
	    }
	    else if ( layer == 4 )
	    {
	       switch ( ladder )
	       {
		  case 36:  phi -= 7. * l4phidiv;  break;
		  case 37:  phi -= 8. * l4phidiv;  break;
		  case 38:  phi -= 9. * l4phidiv;  break;
		  case 39:  phi -= 10. * l4phidiv;  break;
		  case 40:  phi -= 11. * l4phidiv;  break;
		  case 41:  phi -= 12. * l4phidiv;  break;
		  case 42:  phi -= 13. * l4phidiv;  break;
		  case 43:  phi -= 14. * l4phidiv;  break;
		  case 44:  phi -= 15. * l4phidiv;  break;
		  case 45:  phi -= 16. * l4phidiv;  break;
		  case 46:  phi -= 17. * l4phidiv;  break;
		  case 47:  phi -= 18. * l4phidiv;  break;
		  case 48:  phi -= 19. * l4phidiv;  break;
		  case 49:  phi -= 20. * l4phidiv;  break;
		  case 50:  phi -= 21. * l4phidiv;  break;
		  case 51:  phi -= 22. * l4phidiv;  break;
		  case 52:  phi -= 23. * l4phidiv;  break;
		  case 53:  phi -= 24. * l4phidiv;  break;
		  case 54:  phi -= 25. * l4phidiv;  break;
		  case 55:  phi -= 0. * l4phidiv;  break;
		  case 56:  phi -= 1. * l4phidiv;  break;
		  case 57:  phi -= 2. * l4phidiv;  break;
		  case 58:  phi -= 3. * l4phidiv;  break;
		  case 59:  phi -= 4. * l4phidiv;  break;
		  case 60:  phi -= 5. * l4phidiv;  break;
		  case 61:  phi -= 6. * l4phidiv;  break;
	       } // end switch ladder

	       switch ( sensor )
	       {
		  case 189: case 199: case 209: case 219: case 229: case 239:
		  case 249: case 259: case 269: case 279: case 289: case 299:
		  case 309: case 319: case 329: case 339: case 349: case 359:
		  case 369: case 379: case 389: case 399: case 409: case 419:
		  case 429: case 439:
		     m_my_layer4left4->fill( phi, my_zres );
		     m_fitter_layer4left4->fill( phi, fitter_zres );
		     break;

		  case 190: case 200: case 210: case 220: case 230: case 240:
		  case 250: case 260: case 270: case 280: case 290: case 300:
		  case 310: case 320: case 330: case 340: case 350: case 360:
		  case 370: case 380: case 390: case 400: case 410: case 420:
		  case 430: case 440:
		     m_my_layer4left3->fill( phi, my_zres );
		     m_fitter_layer4left3->fill( phi, fitter_zres );
		     break;

		  case 191: case 201: case 211: case 221: case 231: case 241:
		  case 251: case 261: case 271: case 281: case 291: case 301:
		  case 311: case 321: case 331: case 341: case 351: case 361:
		  case 371: case 381: case 391: case 401: case 411: case 421:
		  case 431: case 441:
		     m_my_layer4left2->fill( phi, my_zres );
		     m_fitter_layer4left2->fill( phi, fitter_zres );
		     break;

		  case 192: case 202: case 212: case 222: case 232: case 242:
		  case 252: case 262: case 272: case 282: case 292: case 302:
		  case 312: case 322: case 332: case 342: case 352: case 362:
		  case 372: case 382: case 392: case 402: case 412: case 422:
		  case 432: case 442:
		     m_my_layer4left1->fill( phi, my_zres );
		     m_fitter_layer4left1->fill( phi, fitter_zres );
		     break;

		  case 193: case 203: case 213: case 223: case 233: case 243:
		  case 253: case 263: case 273: case 283: case 293: case 303:
		  case 313: case 323: case 333: case 343: case 353: case 363:
		  case 373: case 383: case 393: case 403: case 413: case 423:
		  case 433: case 443:
		     m_my_layer4right1->fill( phi, my_zres );
		     m_fitter_layer4right1->fill( phi, fitter_zres );
		     break;

		  case 194: case 204: case 214: case 224: case 234: case 244:
		  case 254: case 264: case 274: case 284: case 294: case 304:
		  case 314: case 324: case 334: case 344: case 354: case 364:
		  case 374: case 384: case 394: case 404: case 414: case 424:
		  case 434: case 444:
		     m_my_layer4right2->fill( phi, my_zres );
		     m_fitter_layer4right2->fill( phi, fitter_zres );
		     break;

		  case 195: case 205: case 215: case 225: case 235: case 245:
		  case 255: case 265: case 275: case 285: case 295: case 305:
		  case 315: case 325: case 335: case 345: case 355: case 365:
		  case 375: case 385: case 395: case 405: case 415: case 425:
		  case 435: case 445:
		     m_my_layer4right3->fill( phi, my_zres );
		     m_fitter_layer4right3->fill( phi, fitter_zres );
		     break;

		  case 196: case 206: case 216: case 226: case 236: case 246:
		  case 256: case 266: case 276: case 286: case 296: case 306:
		  case 316: case 326: case 336: case 346: case 356: case 366:
		  case 376: case 386: case 396: case 406: case 416: case 426:
		  case 436: case 446:
		     m_my_layer4right4->fill( phi, my_zres );
		     m_fitter_layer4right4->fill( phi, fitter_zres );
		     break;

	       } // end switch sensor
	    } // end elsif chain for layer

	 } // end if we have a phi and can do thousands of histograms

	 if ( false )
	 {
	    char in_char;
	    NextType next_type = kError;
	    do {
	       report( NOTICE, kFacilityString )
		  << "Enter \"h\", \"t\", \"e\" or \"q\""
		  << " (to step to the next hit, track, event or to quit)" << endl;
	       cin >> in_char;
	       if ( in_char == 'h' ) next_type = kHit;
	       else if ( in_char == 't' ) next_type = kTrack;
	       else if ( in_char == 'e' ) next_type = kEvent;
	       else if ( in_char == 'q' ) next_type = kEnd;
	    } while ( next_type == kError );

	    if ( next_type == kHit ) ;
	    if ( next_type == kTrack ) break;
	    if ( next_type == kEvent ) return ActionBase::kPassed;
	    if ( next_type == kEnd ) assert( false );
	 } // endif we stop here
	 report( NOTICE, kFacilityString ) << endl;
	 
      } // end loop over z hit links (fitted tracks)

      report( NOTICE, kFacilityString ) << endl;
   } // end loop over fitted tracks

   if ( false )
   {
      char in_char;
      NextType next_type = kError;
      do {
	 report( NOTICE, kFacilityString )
	    << "Enter \"e\" or \"q\""
	    << " (to step to the event or to quit)" << endl;
	 cin >> in_char;
	 if ( in_char == 'e' ) next_type = kEvent;
	 else if ( in_char == 'q' ) next_type = kEnd;
      } while ( next_type == kError );

      if ( next_type == kEvent ) return ActionBase::kPassed;
      if ( next_type == kEnd ) assert( false );
   } // endif we stop here

   report( NOTICE, kFacilityString ) << endl;
   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
FindBug::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
FindBug::endRun( Frame& iFrame )         // anal4 equiv.
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
