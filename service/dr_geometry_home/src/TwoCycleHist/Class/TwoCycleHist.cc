// -*- C++ -*-
//
// Package:     TwoCycleHist
// Module:      TwoCycleHist
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri May 30 14:58:21 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TwoCycleHist/TwoCycleHist.h"
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

#include "BeamSpot/BeamSpot.h"

// STL classes
// You may have to uncomment some of these or other stl headers 
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

extern "C" float prob_(const float&, const int&);

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Processor.TwoCycleHist" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
TwoCycleHist::TwoCycleHist( void )               // anal1
   : Processor( "TwoCycleHist" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &TwoCycleHist::event,    Stream::kEvent );
   //bind( &TwoCycleHist::beginRun, Stream::kBeginRun );
   //bind( &TwoCycleHist::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

TwoCycleHist::~TwoCycleHist()                    // anal5
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
TwoCycleHist::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
TwoCycleHist::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
TwoCycleHist::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   m_cotTheta_m29 = iHistoManager.histogram(129, "cotTheta", 120, -3., 3.);
   m_d0miss_m29_2 = iHistoManager.histogram(1329, "d0miss (cT = -2.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m29   = iHistoManager.profile(329, "d0miss (cT = -2.9 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m29_2 = iHistoManager.histogram(1529, "z0miss (cT = -2.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m29   = iHistoManager.profile(529, "z0miss (cT = -2.9 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m27 = iHistoManager.histogram(127, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m27_2 = iHistoManager.histogram(1327, "d0miss (cT = -2.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m27	  = iHistoManager.profile(327, "d0miss (cT = -2.7 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m27_2 = iHistoManager.histogram(1527, "z0miss (cT = -2.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m27	  = iHistoManager.profile(527, "z0miss (cT = -2.7 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m25 = iHistoManager.histogram(125, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m25_2 = iHistoManager.histogram(1325, "d0miss (cT = -2.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m25	  = iHistoManager.profile(325, "d0miss (cT = -2.5 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m25_2 = iHistoManager.histogram(1525, "z0miss (cT = -2.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m25	  = iHistoManager.profile(525, "z0miss (cT = -2.5 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m23 = iHistoManager.histogram(123, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m23_2 = iHistoManager.histogram(1323, "d0miss (cT = -2.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m23	  = iHistoManager.profile(323, "d0miss (cT = -2.3 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m23_2 = iHistoManager.histogram(1523, "z0miss (cT = -2.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m23	  = iHistoManager.profile(523, "z0miss (cT = -2.3 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m21 = iHistoManager.histogram(121, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m21_2 = iHistoManager.histogram(1321, "d0miss (cT = -2.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m21	  = iHistoManager.profile(321, "d0miss (cT = -2.1 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m21_2 = iHistoManager.histogram(1521, "z0miss (cT = -2.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m21	  = iHistoManager.profile(521, "z0miss (cT = -2.1 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m19 = iHistoManager.histogram(119, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m19_2 = iHistoManager.histogram(1319, "d0miss (cT = -1.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m19	  = iHistoManager.profile(319, "d0miss (cT = -1.9 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m19_2 = iHistoManager.histogram(1519, "z0miss (cT = -1.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m19	  = iHistoManager.profile(519, "z0miss (cT = -1.9 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m17 = iHistoManager.histogram(117, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m17_2 = iHistoManager.histogram(1317, "d0miss (cT = -1.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m17	  = iHistoManager.profile(317, "d0miss (cT = -1.7 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m17_2 = iHistoManager.histogram(1517, "z0miss (cT = -1.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m17	  = iHistoManager.profile(517, "z0miss (cT = -1.7 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m15 = iHistoManager.histogram(115, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m15_2 = iHistoManager.histogram(1315, "d0miss (cT = -1.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m15	  = iHistoManager.profile(315, "d0miss (cT = -1.5 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m15_2 = iHistoManager.histogram(1515, "z0miss (cT = -1.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m15	  = iHistoManager.profile(515, "z0miss (cT = -1.5 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m13 = iHistoManager.histogram(113, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m13_2 = iHistoManager.histogram(1313, "d0miss (cT = -1.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m13	  = iHistoManager.profile(313, "d0miss (cT = -1.3 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m13_2 = iHistoManager.histogram(1513, "z0miss (cT = -1.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m13	  = iHistoManager.profile(513, "z0miss (cT = -1.3 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m11 = iHistoManager.histogram(111, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m11_2 = iHistoManager.histogram(1311, "d0miss (cT = -1.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m11	  = iHistoManager.profile(311, "d0miss (cT = -1.1 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m11_2 = iHistoManager.histogram(1511, "z0miss (cT = -1.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m11	  = iHistoManager.profile(511, "z0miss (cT = -1.1 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m9  = iHistoManager.histogram(19, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m9_2 = iHistoManager.histogram(139, "d0miss (cT = -.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m9	  = iHistoManager.profile(39, "d0miss (cT = -.9 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m9_2 = iHistoManager.histogram(159, "z0miss (cT = -.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m9	  = iHistoManager.profile(59, "z0miss (cT = -.9 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m7  = iHistoManager.histogram(17, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m7_2 = iHistoManager.histogram(137, "d0miss (cT = -.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m7	  = iHistoManager.profile(37, "d0miss (cT = -.7 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m7_2 = iHistoManager.histogram(157, "z0miss (cT = -.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m7	  = iHistoManager.profile(57, "z0miss (cT = -.7 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m5  = iHistoManager.histogram(15, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m5_2 = iHistoManager.histogram(135, "d0miss (cT = -.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m5	  = iHistoManager.profile(35, "d0miss (cT = -.5 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m5_2 = iHistoManager.histogram(155, "z0miss (cT = -.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m5	  = iHistoManager.profile(55, "z0miss (cT = -.5 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m3  = iHistoManager.histogram(13, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m3_2 = iHistoManager.histogram(133, "d0miss (cT = -.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m3	  = iHistoManager.profile(33, "d0miss (cT = -.3 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m3_2 = iHistoManager.histogram(153, "z0miss (cT = -.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m3	  = iHistoManager.profile(53, "z0miss (cT = -.3 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_m1  = iHistoManager.histogram(11, "cotTheta", 120, -3., 3.);							   
   m_d0miss_m1_2 = iHistoManager.histogram(131, "d0miss (cT = -.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_m1	  = iHistoManager.profile(31, "d0miss (cT = -.1 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_m1_2 = iHistoManager.histogram(151, "z0miss (cT = -.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_m1	  = iHistoManager.profile(51, "z0miss (cT = -.1 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p1  = iHistoManager.histogram(21, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p1_2 = iHistoManager.histogram(141, "d0miss (cT = +.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p1	  = iHistoManager.profile(41, "d0miss (cT = +.1 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p1_2 = iHistoManager.histogram(161, "z0miss (cT = +.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p1	  = iHistoManager.profile(61, "z0miss (cT = +.1 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p3  = iHistoManager.histogram(23, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p3_2 = iHistoManager.histogram(143, "d0miss (cT = +.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p3	  = iHistoManager.profile(43, "d0miss (cT = +.3 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p3_2 = iHistoManager.histogram(163, "z0miss (cT = +.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p3	  = iHistoManager.profile(63, "z0miss (cT = +.3 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p5  = iHistoManager.histogram(25, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p5_2 = iHistoManager.histogram(145, "d0miss (cT = +.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p5	  = iHistoManager.profile(45, "d0miss (cT = +.5 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p5_2 = iHistoManager.histogram(165, "z0miss (cT = +.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p5	  = iHistoManager.profile(65, "z0miss (cT = +.5 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p7  = iHistoManager.histogram(27, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p7_2 = iHistoManager.histogram(147, "d0miss (cT = +.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p7	  = iHistoManager.profile(47, "d0miss (cT = +.7 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p7_2 = iHistoManager.histogram(167, "z0miss (cT = +.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p7	  = iHistoManager.profile(67, "z0miss (cT = +.7 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p9  = iHistoManager.histogram(29, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p9_2 = iHistoManager.histogram(149, "d0miss (cT = +.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p9	  = iHistoManager.profile(49, "d0miss (cT = +.9 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p9_2 = iHistoManager.histogram(169, "z0miss (cT = +.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p9	  = iHistoManager.profile(69, "z0miss (cT = +.9 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p11 = iHistoManager.histogram(211, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p11_2 = iHistoManager.histogram(1411, "d0miss (cT = +1.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p11	  = iHistoManager.profile(411, "d0miss (cT = +1.1 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p11_2 = iHistoManager.histogram(1611, "z0miss (cT = +1.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p11	  = iHistoManager.profile(611, "z0miss (cT = +1.1 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p13 = iHistoManager.histogram(213, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p13_2 = iHistoManager.histogram(1413, "d0miss (cT = +1.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p13	  = iHistoManager.profile(413, "d0miss (cT = +1.3 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p13_2 = iHistoManager.histogram(1613, "z0miss (cT = +1.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p13	  = iHistoManager.profile(613, "z0miss (cT = +1.3 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p15 = iHistoManager.histogram(215, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p15_2 = iHistoManager.histogram(1415, "d0miss (cT = +1.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p15	  = iHistoManager.profile(415, "d0miss (cT = +1.5 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p15_2 = iHistoManager.histogram(1615, "z0miss (cT = +1.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p15	  = iHistoManager.profile(615, "z0miss (cT = +1.5 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p17 = iHistoManager.histogram(217, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p17_2 = iHistoManager.histogram(1417, "d0miss (cT = +1.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p17	  = iHistoManager.profile(417, "d0miss (cT = +1.7 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p17_2 = iHistoManager.histogram(1617, "z0miss (cT = +1.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p17	  = iHistoManager.profile(617, "z0miss (cT = +1.7 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p19 = iHistoManager.histogram(219, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p19_2 = iHistoManager.histogram(1419, "d0miss (cT = +1.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p19	  = iHistoManager.profile(419, "d0miss (cT = +1.9 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p19_2 = iHistoManager.histogram(1619, "z0miss (cT = +1.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p19	  = iHistoManager.profile(619, "z0miss (cT = +1.9 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p21 = iHistoManager.histogram(221, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p21_2 = iHistoManager.histogram(1421, "d0miss (cT = +2.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p21	  = iHistoManager.profile(421, "d0miss (cT = +2.1 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p21_2 = iHistoManager.histogram(1621, "z0miss (cT = +2.1 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p21	  = iHistoManager.profile(621, "z0miss (cT = +2.1 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p23 = iHistoManager.histogram(223, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p23_2 = iHistoManager.histogram(1423, "d0miss (cT = +2.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p23	  = iHistoManager.profile(423, "d0miss (cT = +2.3 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p23_2 = iHistoManager.histogram(1623, "z0miss (cT = +2.3 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p23	  = iHistoManager.profile(623, "z0miss (cT = +2.3 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p25 = iHistoManager.histogram(225, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p25_2 = iHistoManager.histogram(1425, "d0miss (cT = +2.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p25	  = iHistoManager.profile(425, "d0miss (cT = +2.5 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p25_2 = iHistoManager.histogram(1625, "z0miss (cT = +2.5 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p25	  = iHistoManager.profile(625, "z0miss (cT = +2.5 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p27 = iHistoManager.histogram(227, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p27_2 = iHistoManager.histogram(1427, "d0miss (cT = +2.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p27	  = iHistoManager.profile(427, "d0miss (cT = +2.7 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p27_2 = iHistoManager.histogram(1627, "z0miss (cT = +2.7 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p27	  = iHistoManager.profile(627, "z0miss (cT = +2.7 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_cotTheta_p29 = iHistoManager.histogram(229, "cotTheta", 120, -3., 3.);							   
   m_d0miss_p29_2 = iHistoManager.histogram(1429, "d0miss (cT = +2.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.01, 0.01);
   m_d0miss_p29	  = iHistoManager.profile(429, "d0miss (cT = +2.9 +- .1)", 30, 0., 2.*M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_z0miss_p29_2 = iHistoManager.histogram(1629, "z0miss (cT = +2.9 +- .1)", 30, 0., 2.*M_PI, 30, -0.05, 0.05);
   m_z0miss_p29	  = iHistoManager.profile(629, "z0miss (cT = +2.9 +- .1)", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);


   m_z0miss_cotTheta = iHistoManager.profile(999, "z0miss VS cotTheta", 60, -3., 3., -0.05, 0.05, HIHistProf::kErrorOnMean);

   m_posz0_cotTheta = iHistoManager.profile(998, "positive z0 VS cotTheta", 60, -3., 3., -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_negz0_cotTheta = iHistoManager.profile(997, "negative z0 VS cotTheta", 60, -3., 3., -0.05, 0.05, HIHistProf::kErrorOnMean);


   m_chi2pos = iHistoManager.histogram(2001, "[v]^2! of positive tracks", 420, -10., 200.);
   m_chi2neg = iHistoManager.histogram(3001, "[v]^2! of negative tracks", 420, -10., 200.);
   m_chi2pos_vcotTh = iHistoManager.profile(2002, "[v]^2!/dof of positive tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kErrorOnMean);
   m_chi2neg_vcotTh = iHistoManager.profile(3002, "[v]^2!/dof of negative tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kErrorOnMean);
   m_chi2pos_vcotTh_spread = iHistoManager.profile(2003, "[v]^2!/dof of positive tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kSpread);
   m_chi2neg_vcotTh_spread = iHistoManager.profile(3003, "[v]^2!/dof of negative tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kSpread);
   m_probpos = iHistoManager.histogram(2004, "[v]^2! probability for positive tracks", 100, 0., 1.);
   m_probneg = iHistoManager.histogram(3004, "[v]^2! probability for negative tracks", 100, 0., 1.);


}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
TwoCycleHist::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<BeamSpot> spot;
   extract(iFrame.record(Stream::kBeginRun), spot);

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);

   if ( tracks.size() != 2 ) return ActionBase::kFailed;

   FATable<NavTrack>::const_iterator first = tracks.begin();
   FATable<NavTrack>::const_iterator second = first;  ++second;

   if ( !first->pionQuality()->fit()  ||  first->pionQuality()->fitAbort() ) return ActionBase::kFailed;
   if ( !second->pionQuality()->fit()  ||  second->pionQuality()->fitAbort() ) return ActionBase::kFailed;

   if ( first->pionFit()->momentum().mag() < 4. ) return ActionBase::kFailed;
   if ( second->pionFit()->momentum().mag() < 4. ) return ActionBase::kFailed;

   if ( fabs(first->pionHelix()->d0()) > 0.01 ) return ActionBase::kFailed;
   if ( fabs(second->pionHelix()->d0()) > 0.01 ) return ActionBase::kFailed;

   if ( fabs(first->pionHelix()->z0()) > 0.10 ) return ActionBase::kFailed;
   if ( fabs(second->pionHelix()->z0()) > 0.10 ) return ActionBase::kFailed;

   if ( first->pionHelix()->curvature() * second->pionHelix()->curvature() > 0. ) return ActionBase::kFailed;

   double phi0(first->pionHelix()->curvature() > 0.
	       ? first->pionHelix()->phi0()
	       : second->pionHelix()->phi0());

   double cotTheta(first->pionHelix()->curvature() > 0.
		   ? first->pionHelix()->cotTheta()
		   : second->pionHelix()->cotTheta());

   double d0miss(first->pionHelix()->d0() + second->pionHelix()->d0());
   double z0miss(first->pionHelix()->z0() - second->pionHelix()->z0());
   if (first->pionHelix()->curvature() < 0.) z0miss *= -1;  // positive track - negative track

   if (first->pionHelix()->curvature() > 0.) {
      m_posz0_cotTheta->fill(first->pionHelix()->z0() - spot->center().z(), first->pionHelix()->cotTheta());
      m_negz0_cotTheta->fill(second->pionHelix()->z0() - spot->center().z(), first->pionHelix()->cotTheta());

      m_chi2pos->fill(first->pionQuality()->chiSquare());
      m_chi2neg->fill(second->pionQuality()->chiSquare());
      if (first->pionQuality()->degreesOfFreedom() > 0) {
	 m_chi2pos_vcotTh->fill(cotTheta, first->pionQuality()->chiSquare() / double(first->pionQuality()->degreesOfFreedom()));
	 m_chi2pos_vcotTh_spread->fill(cotTheta, first->pionQuality()->chiSquare() / double(first->pionQuality()->degreesOfFreedom()));
	 m_probpos->fill(prob_(first->pionQuality()->chiSquare(), double(first->pionQuality()->degreesOfFreedom())));
      }
      if (second->pionQuality()->degreesOfFreedom() > 0) {
	 m_chi2neg_vcotTh->fill(cotTheta, second->pionQuality()->chiSquare() / double(second->pionQuality()->degreesOfFreedom()));
	 m_chi2neg_vcotTh_spread->fill(cotTheta, second->pionQuality()->chiSquare() / double(second->pionQuality()->degreesOfFreedom()));
	 m_probneg->fill(prob_(second->pionQuality()->chiSquare(), double(second->pionQuality()->degreesOfFreedom())));
      }
   }
   else {
      m_posz0_cotTheta->fill(second->pionHelix()->z0() - spot->center().z(), second->pionHelix()->cotTheta());
      m_negz0_cotTheta->fill(first->pionHelix()->z0() - spot->center().z(), second->pionHelix()->cotTheta());

      m_chi2pos->fill(second->pionQuality()->chiSquare());
      m_chi2neg->fill(first->pionQuality()->chiSquare());
      if (second->pionQuality()->degreesOfFreedom() > 0) {
	 m_chi2pos_vcotTh->fill(cotTheta, second->pionQuality()->chiSquare() / double(second->pionQuality()->degreesOfFreedom()));
	 m_chi2pos_vcotTh_spread->fill(cotTheta, second->pionQuality()->chiSquare() / double(second->pionQuality()->degreesOfFreedom()));
	 m_probpos->fill(prob_(second->pionQuality()->chiSquare(), double(second->pionQuality()->degreesOfFreedom())));
      }
      if (first->pionQuality()->degreesOfFreedom() > 0) {
	 m_chi2neg_vcotTh->fill(cotTheta, first->pionQuality()->chiSquare() / double(first->pionQuality()->degreesOfFreedom()));
	 m_chi2neg_vcotTh_spread->fill(cotTheta, first->pionQuality()->chiSquare() / double(first->pionQuality()->degreesOfFreedom()));
	 m_probneg->fill(prob_(first->pionQuality()->chiSquare(), double(first->pionQuality()->degreesOfFreedom())));
      }
   }

   m_z0miss_cotTheta->fill(cotTheta, z0miss);

   if ( -3.0 < cotTheta  &&  cotTheta <= -2.8 ) {
      m_cotTheta_m29->fill(cotTheta);
      m_d0miss_m29_2->fill(phi0, d0miss);
      m_d0miss_m29->fill(phi0, d0miss);
      m_z0miss_m29_2->fill(phi0, z0miss);
      m_z0miss_m29->fill(phi0, z0miss);
   } else if ( -2.8 < cotTheta  &&  cotTheta <= -2.6 ) {
      m_cotTheta_m27->fill(cotTheta);
      m_d0miss_m27_2->fill(phi0, d0miss);
      m_d0miss_m27->fill(phi0, d0miss);
      m_z0miss_m27_2->fill(phi0, z0miss);
      m_z0miss_m27->fill(phi0, z0miss);
   } else if ( -2.6 < cotTheta  &&  cotTheta <= -2.4 ) {
      m_cotTheta_m25->fill(cotTheta);
      m_d0miss_m25_2->fill(phi0, d0miss);
      m_d0miss_m25->fill(phi0, d0miss);
      m_z0miss_m25_2->fill(phi0, z0miss);
      m_z0miss_m25->fill(phi0, z0miss);
   } else if ( -2.4 < cotTheta  &&  cotTheta <= -2.2 ) {
      m_cotTheta_m23->fill(cotTheta);
      m_d0miss_m23_2->fill(phi0, d0miss);
      m_d0miss_m23->fill(phi0, d0miss);
      m_z0miss_m23_2->fill(phi0, z0miss);
      m_z0miss_m23->fill(phi0, z0miss);
   } else if ( -2.2 < cotTheta  &&  cotTheta <= -2.0 ) {
      m_cotTheta_m21->fill(cotTheta);
      m_d0miss_m21_2->fill(phi0, d0miss);
      m_d0miss_m21->fill(phi0, d0miss);
      m_z0miss_m21_2->fill(phi0, z0miss);
      m_z0miss_m21->fill(phi0, z0miss);
   } else if ( -2.0 < cotTheta  &&  cotTheta <= -1.8 ) {
      m_cotTheta_m19->fill(cotTheta);
      m_d0miss_m19_2->fill(phi0, d0miss);
      m_d0miss_m19->fill(phi0, d0miss);
      m_z0miss_m19_2->fill(phi0, z0miss);
      m_z0miss_m19->fill(phi0, z0miss);
   } else if ( -1.8 < cotTheta  &&  cotTheta <= -1.6 ) {
      m_cotTheta_m17->fill(cotTheta);
      m_d0miss_m17_2->fill(phi0, d0miss);
      m_d0miss_m17->fill(phi0, d0miss);
      m_z0miss_m17_2->fill(phi0, z0miss);
      m_z0miss_m17->fill(phi0, z0miss);
   } else if ( -1.6 < cotTheta  &&  cotTheta <= -1.4 ) {
      m_cotTheta_m15->fill(cotTheta);
      m_d0miss_m15_2->fill(phi0, d0miss);
      m_d0miss_m15->fill(phi0, d0miss);
      m_z0miss_m15_2->fill(phi0, z0miss);
      m_z0miss_m15->fill(phi0, z0miss);
   } else if ( -1.4 < cotTheta  &&  cotTheta <= -1.2 ) {
      m_cotTheta_m13->fill(cotTheta);
      m_d0miss_m13_2->fill(phi0, d0miss);
      m_d0miss_m13->fill(phi0, d0miss);
      m_z0miss_m13_2->fill(phi0, z0miss);
      m_z0miss_m13->fill(phi0, z0miss);
   } else if ( -1.2 < cotTheta  &&  cotTheta <= -1.0 ) {
      m_cotTheta_m11->fill(cotTheta);
      m_d0miss_m11_2->fill(phi0, d0miss);
      m_d0miss_m11->fill(phi0, d0miss);
      m_z0miss_m11_2->fill(phi0, z0miss);
      m_z0miss_m11->fill(phi0, z0miss);
   } else if ( -1.0 < cotTheta  &&  cotTheta <= -.8 ) {
      m_cotTheta_m9->fill(cotTheta);
      m_d0miss_m9_2->fill(phi0, d0miss);
      m_d0miss_m9->fill(phi0, d0miss);
      m_z0miss_m9_2->fill(phi0, z0miss);
      m_z0miss_m9->fill(phi0, z0miss);
   } else if ( -.8 < cotTheta  &&  cotTheta <= -.6 ) {
      m_cotTheta_m7->fill(cotTheta);
      m_d0miss_m7_2->fill(phi0, d0miss);
      m_d0miss_m7->fill(phi0, d0miss);
      m_z0miss_m7_2->fill(phi0, z0miss);
      m_z0miss_m7->fill(phi0, z0miss);
   } else if ( -.6 < cotTheta  &&  cotTheta <= -.4 ) {
      m_cotTheta_m5->fill(cotTheta);
      m_d0miss_m5_2->fill(phi0, d0miss);
      m_d0miss_m5->fill(phi0, d0miss);
      m_z0miss_m5_2->fill(phi0, z0miss);
      m_z0miss_m5->fill(phi0, z0miss);
   } else if ( -.4 < cotTheta  &&  cotTheta <= -.2 ) {
      m_cotTheta_m3->fill(cotTheta);
      m_d0miss_m3_2->fill(phi0, d0miss);
      m_d0miss_m3->fill(phi0, d0miss);
      m_z0miss_m3_2->fill(phi0, z0miss);
      m_z0miss_m3->fill(phi0, z0miss);
   } else if ( -.2 < cotTheta  &&  cotTheta <= .0 ) {
      m_cotTheta_m1->fill(cotTheta);
      m_d0miss_m1_2->fill(phi0, d0miss);
      m_d0miss_m1->fill(phi0, d0miss);
      m_z0miss_m1_2->fill(phi0, z0miss);
      m_z0miss_m1->fill(phi0, z0miss);
   } else if ( .0 < cotTheta  &&  cotTheta <= .2 ) {
      m_cotTheta_p1->fill(cotTheta);
      m_d0miss_p1_2->fill(phi0, d0miss);
      m_d0miss_p1->fill(phi0, d0miss);
      m_z0miss_p1_2->fill(phi0, z0miss);
      m_z0miss_p1->fill(phi0, z0miss);
   } else if ( .2 < cotTheta  &&  cotTheta <= .4 ) {
      m_cotTheta_p3->fill(cotTheta);
      m_d0miss_p3_2->fill(phi0, d0miss);
      m_d0miss_p3->fill(phi0, d0miss);
      m_z0miss_p3_2->fill(phi0, z0miss);
      m_z0miss_p3->fill(phi0, z0miss);
   } else if ( .4 < cotTheta  &&  cotTheta <= .6 ) {
      m_cotTheta_p5->fill(cotTheta);
      m_d0miss_p5_2->fill(phi0, d0miss);
      m_d0miss_p5->fill(phi0, d0miss);
      m_z0miss_p5_2->fill(phi0, z0miss);
      m_z0miss_p5->fill(phi0, z0miss);
   } else if ( .6 < cotTheta  &&  cotTheta <= .8 ) {
      m_cotTheta_p7->fill(cotTheta);
      m_d0miss_p7_2->fill(phi0, d0miss);
      m_d0miss_p7->fill(phi0, d0miss);
      m_z0miss_p7_2->fill(phi0, z0miss);
      m_z0miss_p7->fill(phi0, z0miss);
   } else if ( .8 < cotTheta  &&  cotTheta <= 1.0 ) {
      m_cotTheta_p9->fill(cotTheta);
      m_d0miss_p9_2->fill(phi0, d0miss);
      m_d0miss_p9->fill(phi0, d0miss);
      m_z0miss_p9_2->fill(phi0, z0miss);
      m_z0miss_p9->fill(phi0, z0miss);
   } else if ( 1.0 < cotTheta  &&  cotTheta <= 1.2 ) {
      m_cotTheta_p11->fill(cotTheta);
      m_d0miss_p11_2->fill(phi0, d0miss);
      m_d0miss_p11->fill(phi0, d0miss);
      m_z0miss_p11_2->fill(phi0, z0miss);
      m_z0miss_p11->fill(phi0, z0miss);
   } else if ( 1.2 < cotTheta  &&  cotTheta <= 1.4 ) {
      m_cotTheta_p13->fill(cotTheta);
      m_d0miss_p13_2->fill(phi0, d0miss);
      m_d0miss_p13->fill(phi0, d0miss);
      m_z0miss_p13_2->fill(phi0, z0miss);
      m_z0miss_p13->fill(phi0, z0miss);
   } else if ( 1.4 < cotTheta  &&  cotTheta <= 1.6 ) {
      m_cotTheta_p15->fill(cotTheta);
      m_d0miss_p15_2->fill(phi0, d0miss);
      m_d0miss_p15->fill(phi0, d0miss);
      m_z0miss_p15_2->fill(phi0, z0miss);
      m_z0miss_p15->fill(phi0, z0miss);
   } else if ( 1.6 < cotTheta  &&  cotTheta <= 1.8 ) {
      m_cotTheta_p17->fill(cotTheta);
      m_d0miss_p17_2->fill(phi0, d0miss);
      m_d0miss_p17->fill(phi0, d0miss);
      m_z0miss_p17_2->fill(phi0, z0miss);
      m_z0miss_p17->fill(phi0, z0miss);
   } else if ( 1.8 < cotTheta  &&  cotTheta <= 2.0 ) {
      m_cotTheta_p19->fill(cotTheta);
      m_d0miss_p19_2->fill(phi0, d0miss);
      m_d0miss_p19->fill(phi0, d0miss);
      m_z0miss_p19_2->fill(phi0, z0miss);
      m_z0miss_p19->fill(phi0, z0miss);
   } else if ( 2.0 < cotTheta  &&  cotTheta <= 2.2 ) {
      m_cotTheta_p21->fill(cotTheta);
      m_d0miss_p21_2->fill(phi0, d0miss);
      m_d0miss_p21->fill(phi0, d0miss);
      m_z0miss_p21_2->fill(phi0, z0miss);
      m_z0miss_p21->fill(phi0, z0miss);
   } else if ( 2.2 < cotTheta  &&  cotTheta <= 2.4 ) {
      m_cotTheta_p23->fill(cotTheta);
      m_d0miss_p23_2->fill(phi0, d0miss);
      m_d0miss_p23->fill(phi0, d0miss);
      m_z0miss_p23_2->fill(phi0, z0miss);
      m_z0miss_p23->fill(phi0, z0miss);
   } else if ( 2.4 < cotTheta  &&  cotTheta <= 2.6 ) {
      m_cotTheta_p25->fill(cotTheta);
      m_d0miss_p25_2->fill(phi0, d0miss);
      m_d0miss_p25->fill(phi0, d0miss);
      m_z0miss_p25_2->fill(phi0, z0miss);
      m_z0miss_p25->fill(phi0, z0miss);
   } else if ( 2.6 < cotTheta  &&  cotTheta <= 2.8 ) {
      m_cotTheta_p27->fill(cotTheta);
      m_d0miss_p27_2->fill(phi0, d0miss);
      m_d0miss_p27->fill(phi0, d0miss);
      m_z0miss_p27_2->fill(phi0, z0miss);
      m_z0miss_p27->fill(phi0, z0miss);
   } else if ( 2.8 < cotTheta  &&  cotTheta <= 3.0 ) {
      m_cotTheta_p29->fill(cotTheta);
      m_d0miss_p29_2->fill(phi0, d0miss);
      m_d0miss_p29->fill(phi0, d0miss);
      m_z0miss_p29_2->fill(phi0, z0miss);
      m_z0miss_p29->fill(phi0, z0miss);
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
TwoCycleHist::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
TwoCycleHist::endRun( Frame& iFrame )         // anal4 equiv.
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
