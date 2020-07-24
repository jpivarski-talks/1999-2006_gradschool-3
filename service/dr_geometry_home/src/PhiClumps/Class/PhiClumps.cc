// -*- C++ -*-
//
// Package:     PhiClumps
// Module:      PhiClumps
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Sep  3 15:07:59 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "PhiClumps/PhiClumps.h"
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

#include "ADRGeom/ADRSenseWireStore.h"

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
static const char* const kFacilityString = "Processor.PhiClumps" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
PhiClumps::PhiClumps( void )               // anal1
   : Processor( "PhiClumps" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &PhiClumps::event,    Stream::kEvent );
   //bind( &PhiClumps::beginRun, Stream::kBeginRun );
   //bind( &PhiClumps::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

PhiClumps::~PhiClumps()                    // anal5
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
PhiClumps::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
PhiClumps::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
PhiClumps::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   const char* label[3] = {"x", "y", "z"};
   m_ntuple = iHistoManager.ntuple(1, "vars", 3, 262144, label );
   
   m_wirenumber = iHistoManager.histogram(100, "layer VS wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_wirenumber_dca = iHistoManager.histogram(101, "mean DCA at layer,wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_wirenumber_dca2 = iHistoManager.histogram(102, "mean sqr(DCA) at layer,wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_wirenumber_drift = iHistoManager.histogram(103, "mean drift at layer,wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_wirenumber_drift2 = iHistoManager.histogram(104, "mean sqr(drift) at layer,wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_wirenumber_sresid = iHistoManager.histogram(105, "mean spacelike resid at layer,wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_wirenumber_sresid2 = iHistoManager.histogram(106, "mean sqr(spacelike resid) at layer,wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_wirenumber_tresid = iHistoManager.histogram(107, "mean timelike resid at layer,wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_wirenumber_tresid2 = iHistoManager.histogram(108, "mean sqr(timelike resid) at layer,wireInLyr",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_xy = iHistoManager.histogram(200, "Y VS X of hit wire (at track Z)",
				  200, -1., 1.,
				  200, -1., 1.);
   m_xy_dca = iHistoManager.histogram(201, "mean DCA at XY",
				  200, -1., 1.,
				  200, -1., 1.);
   m_xy_dca2 = iHistoManager.histogram(202, "mean sqr(DCA) at XY",
				       200, -1., 1.,
				       200, -1., 1.);
   m_xy_drift = iHistoManager.histogram(203, "mean drift at XY",
					200, -1., 1.,
					200, -1., 1.);
   m_xy_drift2 = iHistoManager.histogram(204, "mean sqr(drift) at XY",
					 200, -1., 1.,
					 200, -1., 1.);
   m_xy_sresid = iHistoManager.histogram(205, "mean spacelike resid at XY",
					 200, -1., 1.,
					 200, -1., 1.);
   m_xy_sresid2 = iHistoManager.histogram(206, "mean sqr(spacelike resid) at XY",
					  200, -1., 1.,
					  200, -1., 1.);
   m_xy_tresid = iHistoManager.histogram(207, "mean timelike resid at XY",
					 200, -1., 1.,
					 200, -1., 1.);
   m_xy_tresid2 = iHistoManager.histogram(208, "mean sqr(timelike resid) at XY",
					  200, -1., 1.,
					  200, -1., 1.);
   m_trackphi0 = iHistoManager.histogram(300, "track phi at origin",
					 30, 0., 2.*M_PI);
   m_trackphi = iHistoManager.histogram(400, "track phi at hit",
					30, 0., 2.*M_PI);
   m_trackphis = iHistoManager.histogram(500, "track phi at origin VS at hit",
					 30, 0., 2.*M_PI,
					 30, 0., 2.*M_PI);

   m_notrack_wirenumber = iHistoManager.histogram(1100, "layer VS wireInLyr (notrack)",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_notrack_wirenumber_drift = iHistoManager.histogram(1103, "mean drift at layer,wireInLyr (notrack)",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);
   m_notrack_wirenumber_drift2 = iHistoManager.histogram(1104, "mean sqr(drift) at layer,wireInLyr (notrack)",
					  348, -0.5, 347.5,
					  31, 16.5, 47.5);

   m_dca_vphi_s1 = iHistoManager.profile(601, "dca VS phi (superlayer 1)",
					 30, 0., 2.*M_PI,
					 -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_dca_vphi_s2 = iHistoManager.profile(602, "dca VS phi (superlayer 2)",
					 30, 0., 2.*M_PI,
					 -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_dca_vphi_s3 = iHistoManager.profile(603, "dca VS phi (superlayer 3)",
					 30, 0., 2.*M_PI,
					 -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_dca_vphi_s4 = iHistoManager.profile(604, "dca VS phi (superlayer 4)",
					 30, 0., 2.*M_PI,
					 -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_dca_vphi_l1 = iHistoManager.profile(611, "dca VS phi (first layers)",
					 30, 0., 2.*M_PI,
					 -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_dca_vphi_l2 = iHistoManager.profile(612, "dca VS phi (second layers)",
					 30, 0., 2.*M_PI,
					 -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_dca_vphi_l3 = iHistoManager.profile(613, "dca VS phi (third layers)",
					 30, 0., 2.*M_PI,
					 -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_dca_vphi_l4 = iHistoManager.profile(614, "dca VS phi (fourth layers)",
					 30, 0., 2.*M_PI,
					 -0.02, 0.02, HIHistProf::kErrorOnMean);

   m_drift_vphi_s1 = iHistoManager.profile(701, "drift VS phi (superlayer 1)",
					   30, 0., 2.*M_PI,
					   -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_drift_vphi_s2 = iHistoManager.profile(702, "drift VS phi (superlayer 2)",
					   30, 0., 2.*M_PI,
					   -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_drift_vphi_s3 = iHistoManager.profile(703, "drift VS phi (superlayer 3)",
					   30, 0., 2.*M_PI,
					   -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_drift_vphi_s4 = iHistoManager.profile(704, "drift VS phi (superlayer 4)",
					   30, 0., 2.*M_PI,
					   -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_drift_vphi_l1 = iHistoManager.profile(711, "drift VS phi (first layers)",
					   30, 0., 2.*M_PI,
					   -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_drift_vphi_l2 = iHistoManager.profile(712, "drift VS phi (second layers)",
					   30, 0., 2.*M_PI,
					   -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_drift_vphi_l3 = iHistoManager.profile(713, "drift VS phi (third layers)",
					   30, 0., 2.*M_PI,
					   -0.02, 0.02, HIHistProf::kErrorOnMean);
   m_drift_vphi_l4 = iHistoManager.profile(714, "drift VS phi (fourth layers)",
					   30, 0., 2.*M_PI,
					   -0.02, 0.02, HIHistProf::kErrorOnMean);

   m_sresid_vphi_s1 = iHistoManager.profile(801, "spaceresid VS phi (superlayer 1)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_sresid_vphi_s2 = iHistoManager.profile(802, "spaceresid VS phi (superlayer 2)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_sresid_vphi_s3 = iHistoManager.profile(803, "spaceresid VS phi (superlayer 3)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_sresid_vphi_s4 = iHistoManager.profile(804, "spaceresid VS phi (superlayer 4)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_sresid_vphi_l1 = iHistoManager.profile(811, "spaceresid VS phi (first layers)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_sresid_vphi_l2 = iHistoManager.profile(812, "spaceresid VS phi (second layers)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_sresid_vphi_l3 = iHistoManager.profile(813, "spaceresid VS phi (third layers)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_sresid_vphi_l4 = iHistoManager.profile(814, "spaceresid VS phi (fourth layers)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);

   m_tresid_vphi_s1 = iHistoManager.profile(901, "timeresid VS phi (superlayer 1)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_tresid_vphi_s2 = iHistoManager.profile(902, "timeresid VS phi (superlayer 2)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_tresid_vphi_s3 = iHistoManager.profile(903, "timeresid VS phi (superlayer 3)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_tresid_vphi_s4 = iHistoManager.profile(904, "timeresid VS phi (superlayer 4)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_tresid_vphi_l1 = iHistoManager.profile(911, "timeresid VS phi (first layers)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_tresid_vphi_l2 = iHistoManager.profile(912, "timeresid VS phi (second layers)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_tresid_vphi_l3 = iHistoManager.profile(913, "timeresid VS phi (third layers)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);
   m_tresid_vphi_l4 = iHistoManager.profile(914, "timeresid VS phi (fourth layers)",
					    30, 0., 2.*M_PI,
					    -0.004, 0.004, HIHistProf::kErrorOnMean);

   m_dca_vphi_s1_spread = iHistoManager.profile(1601, "dca VS phi (superlayer 1)",
						30, 0., 2.*M_PI,
						-0.02, 0.02, HIHistProf::kSpread);
   m_dca_vphi_s2_spread = iHistoManager.profile(1602, "dca VS phi (superlayer 2)",
						30, 0., 2.*M_PI,
						-0.02, 0.02, HIHistProf::kSpread);
   m_dca_vphi_s3_spread = iHistoManager.profile(1603, "dca VS phi (superlayer 3)",
						30, 0., 2.*M_PI,
						-0.02, 0.02, HIHistProf::kSpread);
   m_dca_vphi_s4_spread = iHistoManager.profile(1604, "dca VS phi (superlayer 4)",
						30, 0., 2.*M_PI,
						-0.02, 0.02, HIHistProf::kSpread);
   m_dca_vphi_l1_spread = iHistoManager.profile(1611, "dca VS phi (first layers)",
						30, 0., 2.*M_PI,
						-0.02, 0.02, HIHistProf::kSpread);
   m_dca_vphi_l2_spread = iHistoManager.profile(1612, "dca VS phi (second layers)",
						30, 0., 2.*M_PI,
						-0.02, 0.02, HIHistProf::kSpread);
   m_dca_vphi_l3_spread = iHistoManager.profile(1613, "dca VS phi (third layers)",
						30, 0., 2.*M_PI,
						-0.02, 0.02, HIHistProf::kSpread);
   m_dca_vphi_l4_spread = iHistoManager.profile(1614, "dca VS phi (fourth layers)",
						30, 0., 2.*M_PI,
						-0.02, 0.02, HIHistProf::kSpread);

   m_drift_vphi_s1_spread = iHistoManager.profile(1701, "drift VS phi (superlayer 1)",
						  30, 0., 2.*M_PI,
						  -0.02, 0.02, HIHistProf::kSpread);
   m_drift_vphi_s2_spread = iHistoManager.profile(1702, "drift VS phi (superlayer 2)",
						  30, 0., 2.*M_PI,
						  -0.02, 0.02, HIHistProf::kSpread);
   m_drift_vphi_s3_spread = iHistoManager.profile(1703, "drift VS phi (superlayer 3)",
						  30, 0., 2.*M_PI,
						  -0.02, 0.02, HIHistProf::kSpread);
   m_drift_vphi_s4_spread = iHistoManager.profile(1704, "drift VS phi (superlayer 4)",
						  30, 0., 2.*M_PI,
						  -0.02, 0.02, HIHistProf::kSpread);
   m_drift_vphi_l1_spread = iHistoManager.profile(1711, "drift VS phi (first layers)",
						  30, 0., 2.*M_PI,
						  -0.02, 0.02, HIHistProf::kSpread);
   m_drift_vphi_l2_spread = iHistoManager.profile(1712, "drift VS phi (second layers)",
						  30, 0., 2.*M_PI,
						  -0.02, 0.02, HIHistProf::kSpread);
   m_drift_vphi_l3_spread = iHistoManager.profile(1713, "drift VS phi (third layers)",
						  30, 0., 2.*M_PI,
						  -0.02, 0.02, HIHistProf::kSpread);
   m_drift_vphi_l4_spread = iHistoManager.profile(1714, "drift VS phi (fourth layers)",
						  30, 0., 2.*M_PI,
						  -0.02, 0.02, HIHistProf::kSpread);

   m_sresid_vphi_s1_spread = iHistoManager.profile(1801, "spaceresid VS phi (superlayer 1)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_sresid_vphi_s2_spread = iHistoManager.profile(1802, "spaceresid VS phi (superlayer 2)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_sresid_vphi_s3_spread = iHistoManager.profile(1803, "spaceresid VS phi (superlayer 3)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_sresid_vphi_s4_spread = iHistoManager.profile(1804, "spaceresid VS phi (superlayer 4)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_sresid_vphi_l1_spread = iHistoManager.profile(1811, "spaceresid VS phi (first layers)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_sresid_vphi_l2_spread = iHistoManager.profile(1812, "spaceresid VS phi (second layers)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_sresid_vphi_l3_spread = iHistoManager.profile(1813, "spaceresid VS phi (third layers)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_sresid_vphi_l4_spread = iHistoManager.profile(1814, "spaceresid VS phi (fourth layers)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);

   m_tresid_vphi_s1_spread = iHistoManager.profile(1901, "timeresid VS phi (superlayer 1)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_tresid_vphi_s2_spread = iHistoManager.profile(1902, "timeresid VS phi (superlayer 2)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_tresid_vphi_s3_spread = iHistoManager.profile(1903, "timeresid VS phi (superlayer 3)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_tresid_vphi_s4_spread = iHistoManager.profile(1904, "timeresid VS phi (superlayer 4)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_tresid_vphi_l1_spread = iHistoManager.profile(1911, "timeresid VS phi (first layers)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_tresid_vphi_l2_spread = iHistoManager.profile(1912, "timeresid VS phi (second layers)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_tresid_vphi_l3_spread = iHistoManager.profile(1913, "timeresid VS phi (third layers)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);
   m_tresid_vphi_l4_spread = iHistoManager.profile(1914, "timeresid VS phi (fourth layers)",
						   30, 0., 2.*M_PI,
						   -0.004, 0.004, HIHistProf::kSpread);

   m_dca_hiphi_s1 = iHistoManager.histogram(2601, "dca for cos4phi.gt.0 (superlayer 1)",
					    90, -0.015, 0.015);
   m_dca_hiphi_s2 = iHistoManager.histogram(2602, "dca for cos4phi.gt.0 (superlayer 2)",
					    90, -0.015, 0.015);
   m_dca_hiphi_s3 = iHistoManager.histogram(2603, "dca for cos4phi.gt.0 (superlayer 3)",
					    90, -0.015, 0.015);
   m_dca_hiphi_s4 = iHistoManager.histogram(2604, "dca for cos4phi.gt.0 (superlayer 4)",
					    90, -0.015, 0.015);
   m_dca_hiphi_l1 = iHistoManager.histogram(2611, "dca for cos4phi.gt.0 (first layers)",
					    90, -0.015, 0.015);
   m_dca_hiphi_l2 = iHistoManager.histogram(2612, "dca for cos4phi.gt.0 (second layers)",
					    90, -0.015, 0.015);
   m_dca_hiphi_l3 = iHistoManager.histogram(2613, "dca for cos4phi.gt.0 (third layers)",
					    90, -0.015, 0.015);
   m_dca_hiphi_l4 = iHistoManager.histogram(2614, "dca for cos4phi.gt.0 (fourth layers)",
					    90, -0.015, 0.015);

   m_drift_hiphi_s1 = iHistoManager.histogram(2701, "drift for cos4phi.gt.0 (superlayer 1)",
					      90, -0.015, 0.015);
   m_drift_hiphi_s2 = iHistoManager.histogram(2702, "drift for cos4phi.gt.0 (superlayer 2)",
					      90, -0.015, 0.015);
   m_drift_hiphi_s3 = iHistoManager.histogram(2703, "drift for cos4phi.gt.0 (superlayer 3)",
					      90, -0.015, 0.015);
   m_drift_hiphi_s4 = iHistoManager.histogram(2704, "drift for cos4phi.gt.0 (superlayer 4)",
					      90, -0.015, 0.015);
   m_drift_hiphi_l1 = iHistoManager.histogram(2711, "drift for cos4phi.gt.0 (first layers)",
					      90, -0.015, 0.015);
   m_drift_hiphi_l2 = iHistoManager.histogram(2712, "drift for cos4phi.gt.0 (second layers)",
					      90, -0.015, 0.015);
   m_drift_hiphi_l3 = iHistoManager.histogram(2713, "drift for cos4phi.gt.0 (third layers)",
					      90, -0.015, 0.015);
   m_drift_hiphi_l4 = iHistoManager.histogram(2714, "drift for cos4phi.gt.0 (fourth layers)",
					      90, -0.015, 0.015);

   m_sresid_hiphi_s1 = iHistoManager.histogram(2801, "spaceresid for cos4phi.gt.0 (superlayer 1)",
					       90, -0.0003, 0.0003);
   m_sresid_hiphi_s2 = iHistoManager.histogram(2802, "spaceresid for cos4phi.gt.0 (superlayer 2)",
					       90, -0.0003, 0.0003);
   m_sresid_hiphi_s3 = iHistoManager.histogram(2803, "spaceresid for cos4phi.gt.0 (superlayer 3)",
					       90, -0.0003, 0.0003);
   m_sresid_hiphi_s4 = iHistoManager.histogram(2804, "spaceresid for cos4phi.gt.0 (superlayer 4)",
					       90, -0.0003, 0.0003);
   m_sresid_hiphi_l1 = iHistoManager.histogram(2811, "spaceresid for cos4phi.gt.0 (first layers)",
					       90, -0.0003, 0.0003);
   m_sresid_hiphi_l2 = iHistoManager.histogram(2812, "spaceresid for cos4phi.gt.0 (second layers)",
					       90, -0.0003, 0.0003);
   m_sresid_hiphi_l3 = iHistoManager.histogram(2813, "spaceresid for cos4phi.gt.0 (third layers)",
					       90, -0.0003, 0.0003);
   m_sresid_hiphi_l4 = iHistoManager.histogram(2814, "spaceresid for cos4phi.gt.0 (fourth layers)",
					       90, -0.0003, 0.0003);

   m_tresid_hiphi_s1 = iHistoManager.histogram(2901, "timeresid for cos4phi.gt.0 (superlayer 1)",
					       90, -0.0003, 0.0003);
   m_tresid_hiphi_s2 = iHistoManager.histogram(2902, "timeresid for cos4phi.gt.0 (superlayer 2)",
					       90, -0.0003, 0.0003);
   m_tresid_hiphi_s3 = iHistoManager.histogram(2903, "timeresid for cos4phi.gt.0 (superlayer 3)",
					       90, -0.0003, 0.0003);
   m_tresid_hiphi_s4 = iHistoManager.histogram(2904, "timeresid for cos4phi.gt.0 (superlayer 4)",
					       90, -0.0003, 0.0003);
   m_tresid_hiphi_l1 = iHistoManager.histogram(2911, "timeresid for cos4phi.gt.0 (first layers)",
					       90, -0.0003, 0.0003);
   m_tresid_hiphi_l2 = iHistoManager.histogram(2912, "timeresid for cos4phi.gt.0 (second layers)",
					       90, -0.0003, 0.0003);
   m_tresid_hiphi_l3 = iHistoManager.histogram(2913, "timeresid for cos4phi.gt.0 (third layers)",
					       90, -0.0003, 0.0003);
   m_tresid_hiphi_l4 = iHistoManager.histogram(2914, "timeresid for cos4phi.gt.0 (fourth layers)",
					       90, -0.0003, 0.0003);

   m_dca_lophi_s1 = iHistoManager.histogram(3601, "dca for cos4phi.lt.0 (superlayer 1)",
					    90, -0.015, 0.015);
   m_dca_lophi_s2 = iHistoManager.histogram(3602, "dca for cos4phi.lt.0 (superlayer 2)",
					    90, -0.015, 0.015);
   m_dca_lophi_s3 = iHistoManager.histogram(3603, "dca for cos4phi.lt.0 (superlayer 3)",
					    90, -0.015, 0.015);
   m_dca_lophi_s4 = iHistoManager.histogram(3604, "dca for cos4phi.lt.0 (superlayer 4)",
					    90, -0.015, 0.015);
   m_dca_lophi_l1 = iHistoManager.histogram(3611, "dca for cos4phi.lt.0 (first layers)",
					    90, -0.015, 0.015);
   m_dca_lophi_l2 = iHistoManager.histogram(3612, "dca for cos4phi.lt.0 (second layers)",
					    90, -0.015, 0.015);
   m_dca_lophi_l3 = iHistoManager.histogram(3613, "dca for cos4phi.lt.0 (third layers)",
					    90, -0.015, 0.015);
   m_dca_lophi_l4 = iHistoManager.histogram(3614, "dca for cos4phi.lt.0 (fourth layers)",
					    90, -0.015, 0.015);

   m_drift_lophi_s1 = iHistoManager.histogram(3701, "drift for cos4phi.lt.0 (superlayer 1)",
					      90, -0.015, 0.015);
   m_drift_lophi_s2 = iHistoManager.histogram(3702, "drift for cos4phi.lt.0 (superlayer 2)",
					      90, -0.015, 0.015);
   m_drift_lophi_s3 = iHistoManager.histogram(3703, "drift for cos4phi.lt.0 (superlayer 3)",
					      90, -0.015, 0.015);
   m_drift_lophi_s4 = iHistoManager.histogram(3704, "drift for cos4phi.lt.0 (superlayer 4)",
					      90, -0.015, 0.015);
   m_drift_lophi_l1 = iHistoManager.histogram(3711, "drift for cos4phi.lt.0 (first layers)",
					      90, -0.015, 0.015);
   m_drift_lophi_l2 = iHistoManager.histogram(3712, "drift for cos4phi.lt.0 (second layers)",
					      90, -0.015, 0.015);
   m_drift_lophi_l3 = iHistoManager.histogram(3713, "drift for cos4phi.lt.0 (third layers)",
					      90, -0.015, 0.015);
   m_drift_lophi_l4 = iHistoManager.histogram(3714, "drift for cos4phi.lt.0 (fourth layers)",
					      90, -0.015, 0.015);

   m_sresid_lophi_s1 = iHistoManager.histogram(3801, "spaceresid for cos4phi.lt.0 (superlayer 1)",
					       90, -0.0003, 0.0003);
   m_sresid_lophi_s2 = iHistoManager.histogram(3802, "spaceresid for cos4phi.lt.0 (superlayer 2)",
					       90, -0.0003, 0.0003);
   m_sresid_lophi_s3 = iHistoManager.histogram(3803, "spaceresid for cos4phi.lt.0 (superlayer 3)",
					       90, -0.0003, 0.0003);
   m_sresid_lophi_s4 = iHistoManager.histogram(3804, "spaceresid for cos4phi.lt.0 (superlayer 4)",
					       90, -0.0003, 0.0003);
   m_sresid_lophi_l1 = iHistoManager.histogram(3811, "spaceresid for cos4phi.lt.0 (first layers)",
					       90, -0.0003, 0.0003);
   m_sresid_lophi_l2 = iHistoManager.histogram(3812, "spaceresid for cos4phi.lt.0 (second layers)",
					       90, -0.0003, 0.0003);
   m_sresid_lophi_l3 = iHistoManager.histogram(3813, "spaceresid for cos4phi.lt.0 (third layers)",
					       90, -0.0003, 0.0003);
   m_sresid_lophi_l4 = iHistoManager.histogram(3814, "spaceresid for cos4phi.lt.0 (fourth layers)",
					       90, -0.0003, 0.0003);

   m_tresid_lophi_s1 = iHistoManager.histogram(3901, "timeresid for cos4phi.lt.0 (superlayer 1)",
					       90, -0.0003, 0.0003);
   m_tresid_lophi_s2 = iHistoManager.histogram(3902, "timeresid for cos4phi.lt.0 (superlayer 2)",
					       90, -0.0003, 0.0003);
   m_tresid_lophi_s3 = iHistoManager.histogram(3903, "timeresid for cos4phi.lt.0 (superlayer 3)",
					       90, -0.0003, 0.0003);
   m_tresid_lophi_s4 = iHistoManager.histogram(3904, "timeresid for cos4phi.lt.0 (superlayer 4)",
					       90, -0.0003, 0.0003);
   m_tresid_lophi_l1 = iHistoManager.histogram(3911, "timeresid for cos4phi.lt.0 (first layers)",
					       90, -0.0003, 0.0003);
   m_tresid_lophi_l2 = iHistoManager.histogram(3912, "timeresid for cos4phi.lt.0 (second layers)",
					       90, -0.0003, 0.0003);
   m_tresid_lophi_l3 = iHistoManager.histogram(3913, "timeresid for cos4phi.lt.0 (third layers)",
					       90, -0.0003, 0.0003);
   m_tresid_lophi_l4 = iHistoManager.histogram(3914, "timeresid for cos4phi.lt.0 (fourth layers)",
					       90, -0.0003, 0.0003);

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
PhiClumps::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);

   if ( tracks.size() != 2 ) return ActionBase::kFailed;
   
   FATable<NavTrack>::const_iterator first = tracks.begin();
   FATable<NavTrack>::const_iterator second = first;  ++second;

  if ( fabs(first->seedTrack()->cotTheta()) < 1.8 ) return ActionBase::kFailed;
  if ( fabs(second->seedTrack()->cotTheta()) < 1.8 ) return ActionBase::kFailed;

//    if ( !first->pionQuality()->fit()  ||  first->pionQuality()->fitAbort() ) return ActionBase::kFailed;
//    if ( !second->pionQuality()->fit()  ||  second->pionQuality()->fitAbort() ) return ActionBase::kFailed;

   if ( first->pionFit()->momentum().mag() < 4. ) return ActionBase::kFailed;
   if ( second->pionFit()->momentum().mag() < 4. ) return ActionBase::kFailed;

   if ( fabs(first->pionHelix()->d0()) > 0.01 ) return ActionBase::kFailed;
   if ( fabs(second->pionHelix()->d0()) > 0.01 ) return ActionBase::kFailed;

   if ( fabs(first->pionHelix()->z0()) > 0.10 ) return ActionBase::kFailed;
   if ( fabs(second->pionHelix()->z0()) > 0.10 ) return ActionBase::kFailed;

   FAItem<ADRSenseWireStore> store;
   extract(iFrame.record(Stream::kBaseGeometry), store);

   FATable<CalibratedDRHit> hits;
   extract(iFrame.record(Stream::kEvent), hits);
   FATable<CalibratedDRHit>::const_iterator hits_end = hits.end();

   DABoolean steep(fabs(first->pionHelix()->cotTheta()) > 1.8  &&
		   fabs(second->pionHelix()->cotTheta()) > 1.8   );

   if (steep) {
      for (FATable<CalibratedDRHit>::const_iterator hit_iter = hits.begin();
	   hit_iter != hits_end;
	   ++hit_iter) {
	 int layer = hit_iter->layer();
	 int wireInLyr = hit_iter->wireInLyr();
	 double drift = hit_iter->distance();
	 
	 m_notrack_wirenumber->fill(wireInLyr, layer);
	 m_notrack_wirenumber_drift->fill(wireInLyr, layer, drift);
	 m_notrack_wirenumber_drift2->fill(wireInLyr, layer, sqr(drift));
      } // end loop over all hits (not just those associated with tracks)
   } // end if two steep tracks

   for (FATable<NavTrack>::const_iterator track_iter = tracks.begin();
	track_iter != tracks.end();
	++track_iter) {

      KTHelix helix(*track_iter->pionHelix());

      const NavTrack::DRHitLinkTable* links = track_iter->pionDRHitLinks();
      NavTrack::DRHitLinkTable::const_iterator links_end = links->end();
      for (NavTrack::DRHitLinkTable::const_iterator link_iter = links->begin();
	   link_iter != links_end;
	   ++link_iter) {
	 
	 FATable<CalibratedDRHit>::const_iterator hit = hits.find(*(*link_iter)->rightID());
	 int layer = hit->layer();
	 int wireInLyr = hit->wireInLyr();

	 double dummy;
	 helix.moveToRadius(store->radius(layer), dummy);
	 HepPoint3D wire_location(store->wire(layer, wireInLyr).wirePoint(helix.referencePoint().z()));

// 	 float xyz[3];
// 	 xyz[0] = wire_location.x();
// 	 xyz[1] = wire_location.y();
// 	 xyz[2] = helix.referencePoint().z();
// 	 if (fabs(xyz[0]) < 1.  &&  fabs(xyz[1]) < 1.  &&  fabs(xyz[2]) < 2. ) {
// 	    m_ntuple->fill(xyz);
// 	 }

	 if (steep) {
	    m_wirenumber->fill(wireInLyr, layer);
	    m_xy->fill(wire_location.x(), wire_location.y());
	    m_trackphi0->fill(track_iter->pionHelix()->phi0());
	    m_trackphi->fill(helix.phi0());
	    m_trackphis->fill(helix.phi0(), track_iter->pionHelix()->phi0());

	    double dca = (*link_iter)->linkData().signedDcaToWire();
	    double drift = (*link_iter)->linkData().signedDriftDistance();
	    double sresid = drift - dca;
	    double tresid = fabs(drift) - fabs(dca);

	    m_wirenumber_dca->fill(wireInLyr, layer, dca);
	    m_wirenumber_dca2->fill(wireInLyr, layer, sqr(dca));
	    m_wirenumber_drift->fill(wireInLyr, layer, drift);
	    m_wirenumber_drift2->fill(wireInLyr, layer, sqr(drift));
	    m_wirenumber_sresid->fill(wireInLyr, layer, sresid);
	    m_wirenumber_sresid2->fill(wireInLyr, layer, sqr(sresid));
	    m_wirenumber_tresid->fill(wireInLyr, layer, tresid);
	    m_wirenumber_tresid2->fill(wireInLyr, layer, sqr(tresid));

	    m_xy_dca->fill(wire_location.x(), wire_location.y(), dca);
	    m_xy_dca2->fill(wire_location.x(), wire_location.y(), sqr(dca));
	    m_xy_drift->fill(wire_location.x(), wire_location.y(), drift);
	    m_xy_drift2->fill(wire_location.x(), wire_location.y(), sqr(drift));
	    m_xy_sresid->fill(wire_location.x(), wire_location.y(), sresid);
	    m_xy_sresid2->fill(wire_location.x(), wire_location.y(), sqr(sresid));
	    m_xy_tresid->fill(wire_location.x(), wire_location.y(), tresid);
	    m_xy_tresid2->fill(wire_location.x(), wire_location.y(), sqr(tresid));

	    switch (layer) {
	       case 17: case 18: case 19: case 20:
		  m_dca_vphi_s1->fill(helix.phi0(), dca);
		  m_dca_vphi_s1_spread->fill(helix.phi0(), dca);
		  m_drift_vphi_s1->fill(helix.phi0(), drift);
		  m_drift_vphi_s1_spread->fill(helix.phi0(), drift);
		  m_sresid_vphi_s1->fill(helix.phi0(), sresid);
		  m_sresid_vphi_s1_spread->fill(helix.phi0(), sresid);
		  m_tresid_vphi_s1->fill(helix.phi0(), tresid);
		  m_tresid_vphi_s1_spread->fill(helix.phi0(), tresid);

		  if (cos(4.*helix.phi0()) > 0) {
		     m_dca_hiphi_s1->fill(dca);
		     m_drift_hiphi_s1->fill(drift);
		     m_sresid_hiphi_s1->fill(sresid);
		     m_tresid_hiphi_s1->fill(tresid);
		  } else {
		     m_dca_lophi_s1->fill(dca);
		     m_drift_lophi_s1->fill(drift);
		     m_sresid_lophi_s1->fill(sresid);
		     m_tresid_lophi_s1->fill(tresid);
		  }

		  break;
	       case 21: case 22: case 23: case 24:
		  m_dca_vphi_s2->fill(helix.phi0(), dca);
		  m_dca_vphi_s2_spread->fill(helix.phi0(), dca);
		  m_drift_vphi_s2->fill(helix.phi0(), drift);
		  m_drift_vphi_s2_spread->fill(helix.phi0(), drift);
		  m_sresid_vphi_s2->fill(helix.phi0(), sresid);
		  m_sresid_vphi_s2_spread->fill(helix.phi0(), sresid);
		  m_tresid_vphi_s2->fill(helix.phi0(), tresid);
		  m_tresid_vphi_s2_spread->fill(helix.phi0(), tresid);

		  if (cos(4.*helix.phi0()) > 0) {
		     m_dca_hiphi_s2->fill(dca);
		     m_drift_hiphi_s2->fill(drift);
		     m_sresid_hiphi_s2->fill(sresid);
		     m_tresid_hiphi_s2->fill(tresid);
		  } else {
		     m_dca_lophi_s2->fill(dca);
		     m_drift_lophi_s2->fill(drift);
		     m_sresid_lophi_s2->fill(sresid);
		     m_tresid_lophi_s2->fill(tresid);
		  }

		  break;
	       case 25: case 26: case 27: case 28:
		  m_dca_vphi_s3->fill(helix.phi0(), dca);
		  m_dca_vphi_s3_spread->fill(helix.phi0(), dca);
		  m_drift_vphi_s3->fill(helix.phi0(), drift);
		  m_drift_vphi_s3_spread->fill(helix.phi0(), drift);
		  m_sresid_vphi_s3->fill(helix.phi0(), sresid);
		  m_sresid_vphi_s3_spread->fill(helix.phi0(), sresid);
		  m_tresid_vphi_s3->fill(helix.phi0(), tresid);
		  m_tresid_vphi_s3_spread->fill(helix.phi0(), tresid);
	       
		  if (cos(4.*helix.phi0()) > 0) {
		     m_dca_hiphi_s3->fill(dca);
		     m_drift_hiphi_s3->fill(drift);
		     m_sresid_hiphi_s3->fill(sresid);
		     m_tresid_hiphi_s3->fill(tresid);
		  } else {
		     m_dca_lophi_s3->fill(dca);
		     m_drift_lophi_s3->fill(drift);
		     m_sresid_lophi_s3->fill(sresid);
		     m_tresid_lophi_s3->fill(tresid);
		  }

		  break;
	       case 29: case 30: case 31: case 32:
		  m_dca_vphi_s4->fill(helix.phi0(), dca);
		  m_dca_vphi_s4_spread->fill(helix.phi0(), dca);
		  m_drift_vphi_s4->fill(helix.phi0(), drift);
		  m_drift_vphi_s4_spread->fill(helix.phi0(), drift);
		  m_sresid_vphi_s4->fill(helix.phi0(), sresid);
		  m_sresid_vphi_s4_spread->fill(helix.phi0(), sresid);
		  m_tresid_vphi_s4->fill(helix.phi0(), tresid);
		  m_tresid_vphi_s4_spread->fill(helix.phi0(), tresid);

		  if (cos(4.*helix.phi0()) > 0) {
		     m_dca_hiphi_s4->fill(dca);
		     m_drift_hiphi_s4->fill(drift);
		     m_sresid_hiphi_s4->fill(sresid);
		     m_tresid_hiphi_s4->fill(tresid);
		  } else {
		     m_dca_lophi_s4->fill(dca);
		     m_drift_lophi_s4->fill(drift);
		     m_sresid_lophi_s4->fill(sresid);
		     m_tresid_lophi_s4->fill(tresid);
		  }

		  break;
	    } // end switch superlayers

	    switch (layer) {
	       case 17: case 21: case 25: case 29:
		  m_dca_vphi_l1->fill(helix.phi0(), dca);
		  m_dca_vphi_l1_spread->fill(helix.phi0(), dca);
		  m_drift_vphi_l1->fill(helix.phi0(), drift);
		  m_drift_vphi_l1_spread->fill(helix.phi0(), drift);
		  m_sresid_vphi_l1->fill(helix.phi0(), sresid);
		  m_sresid_vphi_l1_spread->fill(helix.phi0(), sresid);
		  m_tresid_vphi_l1->fill(helix.phi0(), tresid);
		  m_tresid_vphi_l1_spread->fill(helix.phi0(), tresid);

		  if (cos(4.*helix.phi0()) > 0) {
		     m_dca_hiphi_l1->fill(dca);
		     m_drift_hiphi_l1->fill(drift);
		     m_sresid_hiphi_l1->fill(sresid);
		     m_tresid_hiphi_l1->fill(tresid);
		  } else {
		     m_dca_lophi_l1->fill(dca);
		     m_drift_lophi_l1->fill(drift);
		     m_sresid_lophi_l1->fill(sresid);
		     m_tresid_lophi_l1->fill(tresid);
		  }

		  break;
	       case 18: case 22: case 26: case 30:
		  m_dca_vphi_l2->fill(helix.phi0(), dca);
		  m_dca_vphi_l2_spread->fill(helix.phi0(), dca);
		  m_drift_vphi_l2->fill(helix.phi0(), drift);
		  m_drift_vphi_l2_spread->fill(helix.phi0(), drift);
		  m_sresid_vphi_l2->fill(helix.phi0(), sresid);
		  m_sresid_vphi_l2_spread->fill(helix.phi0(), sresid);
		  m_tresid_vphi_l2->fill(helix.phi0(), tresid);
		  m_tresid_vphi_l2_spread->fill(helix.phi0(), tresid);

		  if (cos(4.*helix.phi0()) > 0) {
		     m_dca_hiphi_l2->fill(dca);
		     m_drift_hiphi_l2->fill(drift);
		     m_sresid_hiphi_l2->fill(sresid);
		     m_tresid_hiphi_l2->fill(tresid);
		  } else {
		     m_dca_lophi_l2->fill(dca);
		     m_drift_lophi_l2->fill(drift);
		     m_sresid_lophi_l2->fill(sresid);
		     m_tresid_lophi_l2->fill(tresid);
		  }

		  break;
	       case 19: case 23: case 27: case 31:
		  m_dca_vphi_l3->fill(helix.phi0(), dca);
		  m_dca_vphi_l3_spread->fill(helix.phi0(), dca);
		  m_drift_vphi_l3->fill(helix.phi0(), drift);
		  m_drift_vphi_l3_spread->fill(helix.phi0(), drift);
		  m_sresid_vphi_l3->fill(helix.phi0(), sresid);
		  m_sresid_vphi_l3_spread->fill(helix.phi0(), sresid);
		  m_tresid_vphi_l3->fill(helix.phi0(), tresid);
		  m_tresid_vphi_l3_spread->fill(helix.phi0(), tresid);
	       
		  if (cos(4.*helix.phi0()) > 0) {
		     m_dca_hiphi_l3->fill(dca);
		     m_drift_hiphi_l3->fill(drift);
		     m_sresid_hiphi_l3->fill(sresid);
		     m_tresid_hiphi_l3->fill(tresid);
		  } else {
		     m_dca_lophi_l3->fill(dca);
		     m_drift_lophi_l3->fill(drift);
		     m_sresid_lophi_l3->fill(sresid);
		     m_tresid_lophi_l3->fill(tresid);
		  }

		  break;
	       case 20: case 24: case 28: case 32:
		  m_dca_vphi_l4->fill(helix.phi0(), dca);
		  m_dca_vphi_l4_spread->fill(helix.phi0(), dca);
		  m_drift_vphi_l4->fill(helix.phi0(), drift);
		  m_drift_vphi_l4_spread->fill(helix.phi0(), drift);
		  m_sresid_vphi_l4->fill(helix.phi0(), sresid);
		  m_sresid_vphi_l4_spread->fill(helix.phi0(), sresid);
		  m_tresid_vphi_l4->fill(helix.phi0(), tresid);
		  m_tresid_vphi_l4_spread->fill(helix.phi0(), tresid);

		  if (cos(4.*helix.phi0()) > 0) {
		     m_dca_hiphi_l4->fill(dca);
		     m_drift_hiphi_l4->fill(drift);
		     m_sresid_hiphi_l4->fill(sresid);
		     m_tresid_hiphi_l4->fill(tresid);
		  } else {
		     m_dca_lophi_l4->fill(dca);
		     m_drift_lophi_l4->fill(drift);
		     m_sresid_lophi_l4->fill(sresid);
		     m_tresid_lophi_l4->fill(tresid);
		  }

		  break;
	    } // end switch superlayers

	 } // end if two steep tracks
      } // end loop over links

   } // end loop over tracks

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
PhiClumps::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
PhiClumps::endRun( Frame& iFrame )         // anal4 equiv.
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
