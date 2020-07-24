// -*- C++ -*-
//
// Package:     <CakeAlignMod>
// Module:      CakeAlignMod
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Oct  6 13:30:51 EDT 2003
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
#include "CakeAlignMod/CakeAlignMod.h"
#include "DataHandler/Stream.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/FANonSeqConstants.h"
#include "FrameAccess/extract.h"
#include "DataHandler/Frame.h"
#include "JobControl/JobControl.h"

#include "C3dr/DrStream.h"
#include "CleoDB/DBEventHeader.h"
#include "Navigation/NavTrack.h"
#include "BeamEnergy/BeamEnergy.h"
#include "MagField/MagneticField.h"
#include "C3cc/CcShowerAttributes.h"
#include "TrackletBunchFinder/TrackletBunchDecision.h"
#include "HelixIntersection/HIDualTrackConstraint.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIDRSurfaceFactory.h"
#include "HelixIntersection/HICalibratedWireHit.h"
#include "HelixIntersection/HIFitHelix.h"
#include "qapplication.h"
#include "qtimer.h"
#include "HistogramViewerProc/HVPWindow.h"

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
#include <fstream.h>
#include <iostream.h>
#include <time.h>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "CakeAlignMod.CakeAlignMod" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule.cc,v 1.2 2000/12/04 19:11:05 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
CakeAlignMod::CakeAlignMod()
   : FrameIteratorModuleBase( "CakeAlignMod", "This is the awesomest module" )
     , m_drfwName("drfw", this, "normal.drweight2layerdriftentang")
     , m_geometryName("geom", this, "initial.drgeomalignment")
     , m_numEvents("events", this, 50000)
     , m_momentumBound("momentumBound", this, 1.)
     , m_d0missBound("d0missBound", this, 0.01)
     , m_residBound("residBound", this, 0.002)
     , m_useMuPair("useMuPair", this, false)
     , m_dualConstraint("dualConstraint", this, true)
     , m_halfCrossAngleRadians("halfCrossingAngleRadians", this, 0.0025)
     , m_showPlots("showPlots", this, true)
     , m_printoutFrequency("printoutFrequency", this, 100)
   // yes, the bin spacings are compiled in
     , m_drfw(new DBDRWeight2LayerDriftEntAng[kDRFWs], kDRFWs)
     , m_drfwbackup(new DBDRWeight2LayerDriftEntAng[kDRFWs], kDRFWs)
     , m_geometry(new DBDRGeomAlignment[18], 18)
     , m_drfw_holder(&m_drfw)
     , m_geometry_holder(&m_geometry)
     , m_eventlist(NULL)
{
   //register the Proxy Factory created by the FIHolder
   registerProxyFactory( DrStream::kWeight2LayerDriftEntAng,
			 m_drfw_holder.makeFactory(),
			 UsageTag() );

   registerProxyFactory( Stream::kDRAlignment,
			 m_geometry_holder.makeFactory(),
			 UsageTag() );

   //You must set what streams you which to iterate over
   //  that is, what events should the Frames be stopped on
   iterateOver( Stream::kEvent );

}

// CakeAlignMod::CakeAlignMod( const CakeAlignMod& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

CakeAlignMod::~CakeAlignMod()
{
}

//
// assignment operators
//
// const CakeAlignMod& CakeAlignMod::operator=( const CakeAlignMod& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// const member functions
//

int
CakeAlignMod::cake_ring(int layer) const
{
   // layer number: 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
   // cake ring:       1     2     3     4     5     6     7     8
   return (layer + 1) / 2;
}

double
CakeAlignMod::julian_day() const
{
   time_t right_now = std::time(NULL);
   std::tm now = *std::gmtime(&right_now);
   int year = now.tm_year + 1900;
   int month = now.tm_mon + 1;
   int day = now.tm_mday;

   int a, b, c, e;
   if (month < 3) {
      year--;
      month += 12;
   }
   if (year > 1582  ||  (year == 1582 && month > 10)  ||
       (year == 1582 && month==10 && day > 15)) {
      a = year / 100;
      b = 2 - a + a/4;
   }
   c = 365.25 * year;
   e = 30.6001 * (month+1);

   return b + c + e + day + 1720994.5;
}

double 
CakeAlignMod::sun_position(double j) const
{
   double n, x, e, l, dl, v;
   double m2;
   int i;

   n = 360 / 365.2422 * j;
   i = n / 360;
   n = n - i * 360.0;
   x = n - 3.762863;
   if (x < 0) x += 360;
   x *= M_PI/180.;
   e = x;
   do {
      dl = e - 0.016718 * sin(e) - x;
      e = e - dl / (1 - 0.016718 * cos(e));
   } while (fabs(dl) >= 1e-12);
   v = 360 / M_PI * atan(1.01686011182 * tan(e/2));
   l = v + 282.596403;
   i = l - i * 360.0;
   return l;
}

double
CakeAlignMod::moon_position(double j, double ls) const
{
   double ms, l, mm, n, ev, sms, z, x, lm, bm, ae, ec;
   double d;
   double ds, as, dm;
   int i;
   
   ms = 0.985647332099 * j - 3.762863;
   if (ms < 0) ms += 360.0;
   l = 13.176396 * j + 64.975464;
   i = l / 360;
   l = l - i * 360.0;
   if (l < 0) l += 360.0;
   mm = l - 0.1114041 * j - 349.383063;
   i = mm / 360;
   mm -= i * 360.0;
   n = 151.950429 - 0.0529539 * j;
   i = n / 360;
   n -= i * 360.0;
   ev = 1.2739 * sin((2 * (l-ls) - mm) * M_PI/180.);
   sms = sin(ms * M_PI/180.);
   ae = 0.1858 * sms;
   mm += ev - ae - 0.37 * sms;
   ec = 6.2886 * sin(mm * M_PI/180.);
   l += ev + ec - ae + 0.214 * sin(2 * mm * M_PI/180.);
   l = 0.6583 * sin(2 * (l-ls) * M_PI/180.) + l;
   return l;
}

double
CakeAlignMod::moon_phase() const
{
   double j = julian_day();
   double ls = sun_position(j);
   double lm = moon_position(j, ls);
   
   double t = lm - ls;
   if (t < 0) t += 360;
   return (1.0 - cos((lm - ls) * M_PI/180.)) / 2;
}

//
// member functions
//

static int argc = 1;
static char* argv[] = {"HistogramViewer"};

void
CakeAlignMod::iterate( const FIFrameIterator& iBegin,
		       const FIFrameIterator& iEnd )
{
   read_files();

   QApplication* qapplication;
   HVPWindow* hvp_window;
   
   if (m_showPlots.value()) {
      qapplication = new QApplication(argc, &(argv[0]));
      hvp_window = new HVPWindow(book_histograms(iBegin));
   }
   else {
      book_histograms(iBegin);
   }

   m_eventlist = new pair<unsigned int, unsigned int>[m_numEvents.value()];
   assert(m_eventlist);

   init_moves();

   for (int fiteration = kFiterations;  fiteration > 0;  fiteration--) {
      report(SYSTEM, kFacilityString) << "=============== Starting fiteration " << fiteration << endl;
      if (fiteration-1 > 0) {
	 report(SYSTEM, kFacilityString)
	    << "                (Focus on cake ring " << fiteration-1 << ")" << endl;
      }
      else {
	 report(NOTICE, kFacilityString)
	    << "                (Last residual check)" << endl;
      }
      setup_fittingweights(fiteration);

      m_eventlist_pointer = 0;
      unsigned int since_last_update = 0;
      for( FIFrameIterator itFrame = iBegin;  itFrame != iEnd;  ++itFrame ) {
	 if (fiteration == kFiterations) {
	    if (m_useMuPair.value()) {
	       if (not_mupair(itFrame)) continue;
	    }
	    else {
	       if (not_bhabha(itFrame)) continue;
	    }
	    gather_eventlist(itFrame);
	 }
	 else {
	    if (not_on_eventlist(itFrame)) continue;
	 }
	 if (m_eventlist_pointer >= m_numEvents.value()) break; // before you break

	 make_checker_plots(fiteration, itFrame);
	 make_aligner_plots(fiteration, itFrame);
	 make_shifter_plots(fiteration);
	 if (m_eventlist_pointer % m_printoutFrequency.value() == 0) print_status(fiteration, NOTICE);

	 if (m_showPlots.value()) {
	    if (qapplication->mainWidget() == NULL) {
	       qapplication->setMainWidget(hvp_window);
	       hvp_window->show();
//	       QTimer::singleShot(5000, qapplication, SLOT(quit()));
	       qapplication->exec();
	    }
	    else {
	       if (++since_last_update >= hvp_window->frequency()) {
		  since_last_update = 0;
		  hvp_window->updateInfo();
		  QTimer::singleShot(0, qapplication, SLOT(quit()));
		  qapplication->exec();

		  if (hvp_window->interactionRequested()) {
		     qapplication->exec();
		  } // end if someone pressed "Interact"
	       } // end if it's time to update
	    } // end if the window is all right
	 } // end if we're showing plots
	 
      } // end loop over events
      
      print_status(fiteration, SYSTEM);
      update_geometry(fiteration);
      print_geomfile(fiteration);
      print_kumac(fiteration);

   } // end loop over rings (fiterations)

   delete [] m_eventlist;
}

//
// static member functions
//

HIHistoManager*
CakeAlignMod::book_histograms(const FIFrameIterator& iBegin)
{
   FIFrameIterator itFrame = iBegin;
   FAItem<BeamEnergy> beam_energy;
   extract(itFrame->record(Stream::kStartRun), beam_energy);

   JobControl* job_control = JobControl::instance();
   HIHistoManager* histman = job_control->histogramManagerP();

   for (int f = 0;  f < kFiterations;  f++) {
      m_hist1d[f].clear();
      m_histprof[f].clear();

      char directory[30];  // padded for safety
      sprintf(directory, "Fiteration%d", f+1);
      char preamble[10];  // padded for safety
      sprintf(preamble, "(F%d) ", f+1);

      HIHistDir* iterdir = histman->mkdir(directory);
      HIHistDir* diagnostics = iterdir->mkdir("Diagnostics");
      HIHistDir* phi = diagnostics->mkdir("Phi");
      HIHistDir* theta = diagnostics->mkdir("Theta");
      HIHistDir* both = diagnostics->mkdir("Both");
      HIHistDir* residuals = iterdir->mkdir("Residuals");
      HIHistDir* next_iteration = iterdir->mkdir("NextIteration");

      int id = 0;

      m_hist1d[f].push_back(m_posP[f] = diagnostics->histogram(
	 ++id, (string(preamble) + string("pos momentum")).c_str(),
	 100, -m_momentumBound.value(), m_momentumBound.value()));
      m_hist1d[f].push_back(m_negP[f] = diagnostics->histogram(
	 ++id, (string(preamble) + string("neg momentum")).c_str(),
	 100, -m_momentumBound.value(), m_momentumBound.value()));
      m_hist1d[f].push_back(m_d0miss[f] = diagnostics->histogram(
	 ++id, (string(preamble) + string("d0miss")).c_str(),
	 100, -m_d0missBound.value(), m_d0missBound.value()));

      m_hist1d[f].push_back(m_just_resid[f] = residuals->histogram(
	 ++id, (string(preamble) + string("residuals")).c_str(),
	 50, -m_residBound.value(), m_residBound.value()));
      
      m_histprof[f].push_back(m_posP_vphi0[f] = phi->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0[f] = phi->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0[f] = phi->profile(
	 ++id, (string(preamble) + string("d0miss V phi0")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      
      m_histprof[f].push_back(m_posP_vcT[f] = theta->profile(
	 ++id, (string(preamble) + string("pos momentum V cotTheta")).c_str(),
	 30, -1.6, 1.6, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vcT[f] = theta->profile(
	 ++id, (string(preamble) + string("neg momentum V cotTheta")).c_str(),
	 30, -1.6, 1.6, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vcT[f] = theta->profile(
	 ++id, (string(preamble) + string("d0miss V cotTheta")).c_str(),
	 30, -1.6, 1.6, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      
      m_histprof[f].push_back(m_posP_vphi0_ctm4[f] = both->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0 with cotTheta in (-2, -1.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0_ctm4[f] = both->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0 with cotTheta in (-2, -1.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0_ctm4[f] = both->profile(
	 ++id, (string(preamble) + string("d0miss V phi0 with cotTheta in (-2, -1.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_posP_vphi0_ctm3[f] = both->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0 with cotTheta in (-1.5, -1)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0_ctm3[f] = both->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0 with cotTheta in (-1.5, -1)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0_ctm3[f] = both->profile(
	 ++id, (string(preamble) + string("d0miss V phi0 with cotTheta in (-1.5, -1)")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_posP_vphi0_ctm2[f] = both->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0 with cotTheta in (-1, -0.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0_ctm2[f] = both->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0 with cotTheta in (-1, -0.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0_ctm2[f] = both->profile(
	 ++id, (string(preamble) + string("d0miss V phi0 with cotTheta in (-1, -0.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_posP_vphi0_ctm1[f] = both->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0 with cotTheta in (-0.5, 0)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0_ctm1[f] = both->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0 with cotTheta in (-0.5, 0)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0_ctm1[f] = both->profile(
	 ++id, (string(preamble) + string("d0miss V phi0 with cotTheta in (-0.5, 0)")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_posP_vphi0_ctp1[f] = both->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0 with cotTheta in (0, 0.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0_ctp1[f] = both->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0 with cotTheta in (0, 0.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0_ctp1[f] = both->profile(
	 ++id, (string(preamble) + string("d0miss V phi0 with cotTheta in (0, 0.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_posP_vphi0_ctp2[f] = both->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0 with cotTheta in (0.5, 1)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0_ctp2[f] = both->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0 with cotTheta in (0.5, 1)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0_ctp2[f] = both->profile(
	 ++id, (string(preamble) + string("d0miss V phi0 with cotTheta in (0.5, 1)")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_posP_vphi0_ctp3[f] = both->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0 with cotTheta in (1, 1.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0_ctp3[f] = both->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0 with cotTheta in (1, 1.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0_ctp3[f] = both->profile(
	 ++id, (string(preamble) + string("d0miss V phi0 with cotTheta in (1, 1.5)")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_posP_vphi0_ctp4[f] = both->profile(
	 ++id, (string(preamble) + string("pos momentum V phi0 with cotTheta in (1.5, 2)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_negP_vphi0_ctp4[f] = both->profile(
	 ++id, (string(preamble) + string("neg momentum V phi0 with cotTheta in (1.5, 2)")).c_str(),
	 30, 0., 2.*M_PI, -m_momentumBound.value(), m_momentumBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_d0miss_vphi0_ctp4[f] = both->profile(
	 ++id, (string(preamble) + string("d0miss V phi0 with cotTheta in (1.5, 2)")).c_str(),
	 30, 0., 2.*M_PI, -m_d0missBound.value(), m_d0missBound.value(), HIHistProf::kErrorOnMean));
      
      m_histprof[f].push_back(m_east_resid[f] = residuals->profile(
	 ++id, (string(preamble) + string("east (-1.265, -0.8) resid")).c_str(),
	 8, 0.5, 8.5, -m_residBound.value(), m_residBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_east_cosresid[f] = residuals->profile(
	 ++id, (string(preamble) + string("east (-1.265, -0.8) cos(phi) * resid (|sin(phi)| < 0.4)")).c_str(),
	 8, 0.5, 8.5, -m_residBound.value(), m_residBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_east_sinresid[f] = residuals->profile(
	 ++id, (string(preamble) + string("east (-1.265, -0.8) -sin(phi) * resid (|cos(phi)| < 0.4)")).c_str(),
	 8, 0.5, 8.5, -m_residBound.value(), m_residBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_west_resid[f] = residuals->profile(
	 ++id, (string(preamble) + string("west (0.8, 1.265) resid")).c_str(),
	 8, 0.5, 8.5, -m_residBound.value(), m_residBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_west_cosresid[f] = residuals->profile(
	 ++id, (string(preamble) + string("west (0.8, 1.265) cos(phi) * resid (|sin(phi)| < 0.4)")).c_str(), 
	 8, 0.5, 8.5, -m_residBound.value(), m_residBound.value(), HIHistProf::kErrorOnMean));
      m_histprof[f].push_back(m_west_sinresid[f] = residuals->profile(
	 ++id, (string(preamble) + string("west (0.8, 1.265) -sin(phi) * resid (|cos(phi)| < 0.4)")).c_str(), 
	 8, 0.5, 8.5, -m_residBound.value(), m_residBound.value(), HIHistProf::kErrorOnMean));
      
      m_east_rotz[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("microns east rotation")).c_str(), 8, 0.5, 8.5);
      m_east_x[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("microns east x translation")).c_str(), 8, 0.5, 8.5);
      m_east_y[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("microns east y translation")).c_str(), 8, 0.5, 8.5);
      m_west_rotz[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("microns west rotation")).c_str(), 8, 0.5, 8.5);
      m_west_x[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("microns west x translation")).c_str(), 8, 0.5, 8.5);
      m_west_y[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("microns west y translation")).c_str(), 8, 0.5, 8.5);

      m_east_rotz_significance[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("sigmas east rotation")).c_str(), 8, 0.5, 8.5);
      m_east_x_significance[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("sigmas east x translation")).c_str(), 8, 0.5, 8.5);
      m_east_y_significance[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("sigmas east y translation")).c_str(), 8, 0.5, 8.5);
      m_west_rotz_significance[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("sigmas west rotation")).c_str(), 8, 0.5, 8.5);
      m_west_x_significance[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("sigmas west x translation")).c_str(), 8, 0.5, 8.5);
      m_west_y_significance[f] = next_iteration->histogram(
	 ++id, (string(preamble) + string("sigmas west y translation")).c_str(), 8, 0.5, 8.5);
   } // end loop over fiterations

   return histman;
}

void
CakeAlignMod::read_files()
{
   if (true) { // be sure the files are closed by making the ifstream go out of scope
      ifstream drfw_file(m_drfwName.value().c_str());
      assert(m_drfw.readFromStream(drfw_file));

      for (int i = 0;  i < kDRFWs;  i++) {
	 if (m_drfw[i].get_layer() == 0)
	    report(VERIFY, kFacilityString)
	       << "drfw header " << m_drfw[i].get_driftbin() << " "
	       << " " << m_drfw[i].get_entangbin() << " "
	       << " " << m_drfw[i].get_resolution() << endl;
      }
   }

   ifstream drfw_file(m_drfwName.value().c_str());
   assert(m_drfwbackup.readFromStream(drfw_file));

   for (int i = 0;  i < kDRFWs;  i++) {
      if (m_drfwbackup[i].get_layer() == 0)
	 report(VERIFY, kFacilityString)
	    << "drfwbackup header " << m_drfwbackup[i].get_driftbin() << " "
	    << " " << m_drfwbackup[i].get_entangbin() << " "
	    << " " << m_drfwbackup[i].get_resolution() << endl;
   }

   ifstream geometry_file(m_geometryName.value().c_str());
   assert(m_geometry.readFromStream(geometry_file));
}

void
CakeAlignMod::init_moves()
{
   for (int f = 0;  f < kFiterations;  f++) {
      for (int r = 0;  r < kRings;  r++) {
	 m_west_rotz_move[f][r] = 0.;
	 m_east_rotz_move[f][r] = 0.;
	 m_west_x_move[f][r] = 0.;
	 m_east_x_move[f][r] = 0.;
	 m_west_y_move[f][r] = 0.;
	 m_east_y_move[f][r] = 0.;
	 m_west_rotz_err[f][r] = 0.;
	 m_east_rotz_err[f][r] = 0.;
	 m_west_x_err[f][r] = 0.;
	 m_east_x_err[f][r] = 0.;
	 m_west_y_err[f][r] = 0.;
	 m_east_y_err[f][r] = 0.;
      }
   }
}

void
CakeAlignMod::setup_fittingweights(int fiteration)
{
   for (int i = 0;  i < kDRFWs;  i++) {
      if (m_drfw[i].get_layer() != 0) {
	 if (cake_ring(m_drfw[i].get_layer()) < fiteration) {
	    m_drfw[i].set_resolution(1000.);
	 }
	 else {
	    m_drfw[i].set_resolution(m_drfwbackup[i].get_resolution());
	 }
      }
   }
   if (moon_phase() > 1. - 1./28.) {  // i.e. full
      switch (fiteration) {
	 case 9: report(SYSTEM, kFacilityString)
	    << "Three Rings for the Elvin-kings under the sky," << endl;  break;
	 case 8: report(SYSTEM, kFacilityString)
	    << "Seven for the Dwarf-lords in the halls of stone," << endl;  break;
	 case 7: report(SYSTEM, kFacilityString)
	    << "Nine for Mortal Men doomed to die," << endl;  break;
	 case 6: report(SYSTEM, kFacilityString)
	    << "One for the Dark Lord on his dark throne." << endl;  break;
	 case 5: report(SYSTEM, kFacilityString)
	    << "In the Land of Mordor where the Shadows lie," << endl;  break;
	 case 4: report(SYSTEM, kFacilityString)
	    << "One Ring to rule them all, One Ring to find them," << endl;  break;
	 case 3: report(SYSTEM, kFacilityString)
	    << "One Ring to bring them all" << endl;  break;
	 case 2: report(SYSTEM, kFacilityString)
	    << "And in the Darkness bind them," << endl;  break;
	 case 1: report(SYSTEM, kFacilityString)
	    << "In the Land of Mordor where the Shadows lie." << endl;  break;
      }
   }
}

void
CakeAlignMod::gather_eventlist(FIFrameIterator itFrame)
{
   FAItem<DBEventHeader> event_header;
   extract(itFrame->record(Stream::kEvent), event_header);

   m_eventlist[m_eventlist_pointer].first = event_header->run();
   m_eventlist[m_eventlist_pointer].second = event_header->number();

   m_eventlist_pointer++;
}

DABoolean
CakeAlignMod::not_bhabha(FIFrameIterator itFrame)
{
   FAItem<TrackletBunchDecision> bunch_decision;
   extract(iFrame.record(Stream::kEvent), bunch_decision);

   if (bunch_decision->numberOfTestedBunches() < 2) return true;
   double bestbunch = bunch_decision->bestBunchInfo().bweight();
   if (bestbunch <= 0.) return true;
   double nextbunch = bunch_decision->bunchInfo(1).bweight();
   if ((bestbunch-nextbunch)/bestbunch < 0.4) return true;

   FAItem<BeamEnergy> beam_energy;
   extract(itFrame->record(Stream::kStartRun), beam_energy);

   FAItem<MagneticField> magnetic_field;
   extract(itFrame->record(Stream::kStartRun), magnetic_field);

   // This extraction fails on the second time for some reason...
   FATable<CcShowerAttributes> showers;
   extract(itFrame->record(Stream::kEvent), showers);
   FATable<CcShowerAttributes>::const_iterator shower_iter = showers.begin();
   FATable<CcShowerAttributes>::const_iterator shower_end = showers.end();

   double biggest_energy = 0.;
   double nextbig_energy = 0.;
   FATable<CcShowerAttributes>::const_iterator biggest;
   FATable<CcShowerAttributes>::const_iterator nextbig;

   for (; shower_iter != shower_end;  ++shower_iter) {
      if (shower_iter->hot()) continue;

      if (shower_iter->energy() > biggest_energy) {
	 biggest_energy = shower_iter->energy();
	 biggest = shower_iter;
      }
      else if (shower_iter->energy() > nextbig_energy) {
	 nextbig_energy = shower_iter->energy();
	 nextbig = shower_iter;
      }
   }

   if (nextbig_energy < 0.5 * beam_energy->value()) return true;

   double lowcut  = 2.3 * ((5.29/beam_energy->value()) * (fabs(magnetic_field->BField().z())/15.)) * M_PI/180.;
   double highcut = 15. * ((5.29/beam_energy->value()) * (fabs(magnetic_field->BField().z())/15.)) * M_PI/180.;
   double colinearity = fabs(M_PI - fabs(biggest->phi() - nextbig->phi()));

   if (colinearity < lowcut  ||  colinearity > highcut) return true;

   FATable<TRSeedTrack> seeds;
   extract(itFrame->record(Stream::kEvent), seeds);
   if (seeds.size() != 2 ) return true;

   FAItem<SeedTrackDRHitLattice> drlattice;
   extract(itFrame->record(Stream::kEvent), drlattice);
   
   for (FATable<TRSeedTrack>::const_iterator seed_iter = seeds.begin();
	seed_iter != seeds.end();
	++seed_iter) {
      int drhits = 0;
      const SeedTrackDRHitLattice::VectorRightID* vect_drhits =
	 drlattice->vRightGivenLeft(seed_iter->identifier());
      if (vect_drhits == NULL  ||  vect_drhits->size() < 42) return true;
   }

   FATable<NavTrack> tracks;
   extract(itFrame->record(Stream::kEvent), tracks);

   FATable<NavTrack>::const_iterator first = tracks.begin();
   FATable<NavTrack>::const_iterator second = first;  ++second;

   if (first->pionHelix()->curvature() * second->pionHelix()->curvature() > 0.) return true;

   if (!first->pionQuality()->fit()  ||  first->pionQuality()->fitAbort()) return true;
   if (!second->pionQuality()->fit()  ||  second->pionQuality()->fitAbort()) return true;

   if (first->pionFit()->momentum().mag() < 0.8 * beam_energy->value()) return true;
   if (second->pionFit()->momentum().mag() < 0.8 * beam_energy->value()) return true;

   if (fabs(first->pionHelix()->d0()) > 0.01) return true;
   if (fabs(second->pionHelix()->d0()) > 0.01) return true;

   if (fabs(first->pionHelix()->z0()) > 0.10) return true;
   if (fabs(second->pionHelix()->z0()) > 0.10) return true;

   if ((first->pionFit()->momentum() + second->pionFit()->momentum() +
	HepVector3D(0.013 * beam_energy->value()/5.29, 0, 0)).mag()
       > 0.548 * beam_energy->value()/5.29) return true;

   return false;
}

DABoolean
CakeAlignMod::not_mupair(FIFrameIterator itFrame)
{
   FAItem<TrackletBunchDecision> bunch_decision;
   extract(iFrame.record(Stream::kEvent), bunch_decision);

   if (bunch_decision->numberOfTestedBunches() < 2) return true;
   double bestbunch = bunch_decision->bestBunchInfo().bweight();
   if (bestbunch <= 0.) return true;
   double nextbunch = bunch_decision->bunchInfo(1).bweight();
   if ((bestbunch-nextbunch)/bestbunch < 0.4) return true;

   FAItem<BeamEnergy> beam_energy;
   extract(itFrame->record(Stream::kStartRun), beam_energy);

   FAItem<MagneticField> magnetic_field;
   extract(itFrame->record(Stream::kStartRun), magnetic_field);

   FATable<CcShowerAttributes> showers;
   extract(itFrame->record(Stream::kEvent), showers);
   FATable<CcShowerAttributes>::const_iterator shower_iter = showers.begin();
   FATable<CcShowerAttributes>::const_iterator shower_end = showers.end();

   double biggest_energy = 0.;
   double nextbig_energy = 0.;
   FATable<CcShowerAttributes>::const_iterator biggest;
   FATable<CcShowerAttributes>::const_iterator nextbig;

   for (; shower_iter != shower_end;  ++shower_iter) {
      if (shower_iter->hot()) continue;
      if (shower_iter->energy() > biggest_energy) {
	 biggest_energy = shower_iter->energy();
	 biggest = shower_iter;
      }
      else if (shower_iter->energy() > nextbig_energy) {
	 nextbig_energy = shower_iter->energy();
	 nextbig = shower_iter;
      }
   }

   if (biggest_energy < 0.075  ||  biggest_energy > 0.7  ||
       nextbig_energy < 0.075  ||  nextbig_energy > 0.7    ) return true;
   
   double lowcut  = 2.3 * ((5.29/beam_energy->value()) * (fabs(magnetic_field->BField().z())/15.)) * M_PI/180.;
   double highcut = 15. * ((5.29/beam_energy->value()) * (fabs(magnetic_field->BField().z())/15.)) * M_PI/180.;
   double colinearity = fabs(M_PI - fabs(biggest->phi() - nextbig->phi()));

   if (colinearity < lowcut  ||  colinearity > highcut) return true;

   FATable<TRSeedTrack> seeds;
   extract(itFrame->record(Stream::kEvent), seeds);
   if (seeds.size() != 2 ) return true;

   FAItem<SeedTrackDRHitLattice> drlattice;
   extract(itFrame->record(Stream::kEvent), drlattice);
   
   for (FATable<TRSeedTrack>::const_iterator seed_iter = seeds.begin();
	seed_iter != seeds.end();
	++seed_iter) {
      int drhits = 0;
      const SeedTrackDRHitLattice::VectorRightID* vect_drhits =
	 drlattice->vRightGivenLeft(seed_iter->identifier());
      if (vect_drhits == NULL  ||  vect_drhits->size() < 42) return true;
   }

   FATable<NavTrack> tracks;
   extract(itFrame->record(Stream::kEvent), tracks);

   FATable<NavTrack>::const_iterator first = tracks.begin();
   FATable<NavTrack>::const_iterator second = first;  ++second;

   if (first->pionHelix()->curvature() * second->pionHelix()->curvature() > 0.) return true;

   if (!first->pionQuality()->fit()  ||  first->pionQuality()->fitAbort()) return true;
   if (!second->pionQuality()->fit()  ||  second->pionQuality()->fitAbort()) return true;

   if (first->pionFit()->momentum().mag() < 0.8 * beam_energy->value()) return true;
   if (second->pionFit()->momentum().mag() < 0.8 * beam_energy->value()) return true;

   if (first->pionFit()->momentum().mag() > 1.2 * beam_energy->value()) return true;
   if (second->pionFit()->momentum().mag() > 1.2 * beam_energy->value()) return true;

   if (fabs(first->pionHelix()->d0()) > 0.01) return true;
   if (fabs(second->pionHelix()->d0()) > 0.01) return true;

   if (fabs(first->pionHelix()->z0()) > 0.10) return true;
   if (fabs(second->pionHelix()->z0()) > 0.10) return true;

   if ((first->pionFit()->momentum() + second->pionFit()->momentum() +
	HepVector3D(0.013 * beam_energy->value()/5.29, 0, 0)).mag()
       > 0.548 * beam_energy->value()/5.29) return true;

   return false;
}

DABoolean
CakeAlignMod::not_on_eventlist(FIFrameIterator itFrame)
{
   FAItem<DBEventHeader> event_header;
   extract(itFrame->record(Stream::kEvent), event_header);

   if ( m_eventlist[m_eventlist_pointer].first != event_header->run()     ||
	m_eventlist[m_eventlist_pointer].second != event_header->number()   ) return true;

   m_eventlist_pointer++;
   return false;
}

void
CakeAlignMod::make_checker_plots(int fiteration, FIFrameIterator itFrame)
{
   FATable<NavTrack> tracks;
   extract(itFrame->record(Stream::kEvent), tracks);

   FAItem<BeamEnergy> beam_energy;
   extract(itFrame->record(Stream::kStartRun), beam_energy);

   FATable<NavTrack>::const_iterator first = tracks.begin();
   FATable<NavTrack>::const_iterator second = first;  ++second;
   FATable<NavTrack>::const_iterator positive, negative;
   if (first->pionHelix()->curvature() > 0.) {
      positive = first;
      negative = second;
   }
   else {
      positive = second;
      negative = first;
   }

   double phi0 = positive->pionHelix()->phi0();
   double cotTheta = positive->pionHelix()->cotTheta();

   double posP = (positive->pionFit()->momentum() +
		  HepVector3D(0.013 * beam_energy->value()/5.29, 0, 0)).mag() - beam_energy->value();
   double negP = (negative->pionFit()->momentum() +
		  HepVector3D(0.013 * beam_energy->value()/5.29, 0, 0)).mag() - beam_energy->value();

   double d0miss = positive->pionHelix()->d0() + negative->pionHelix()->d0();

   m_posP[fiteration-1]->fill(posP);
   m_negP[fiteration-1]->fill(negP);
   m_d0miss[fiteration-1]->fill(d0miss);

   m_posP_vphi0[fiteration-1]->fill(phi0, posP);
   m_negP_vphi0[fiteration-1]->fill(phi0, negP);
   m_d0miss_vphi0[fiteration-1]->fill(phi0, d0miss);
   m_posP_vcT[fiteration-1]->fill(cotTheta, posP);
   m_negP_vcT[fiteration-1]->fill(cotTheta, negP);
   m_d0miss_vcT[fiteration-1]->fill(cotTheta, d0miss);

   if (-2.0 < cotTheta  &&  cotTheta <= -1.5) {
      m_posP_vphi0_ctm4[fiteration-1]->fill(phi0, posP);
      m_negP_vphi0_ctm4[fiteration-1]->fill(phi0, negP);
      m_d0miss_vphi0_ctm4[fiteration-1]->fill(phi0, d0miss);
   }
   else if (-1.5 < cotTheta  &&  cotTheta <= -1.0) {
      m_posP_vphi0_ctm3[fiteration-1]->fill(phi0, posP);
      m_negP_vphi0_ctm3[fiteration-1]->fill(phi0, negP);
      m_d0miss_vphi0_ctm3[fiteration-1]->fill(phi0, d0miss);
   }
   else if (-1.0 < cotTheta  &&  cotTheta <= -0.5) {
      m_posP_vphi0_ctm2[fiteration-1]->fill(phi0, posP);
      m_negP_vphi0_ctm2[fiteration-1]->fill(phi0, negP);
      m_d0miss_vphi0_ctm2[fiteration-1]->fill(phi0, d0miss);
   }
   else if (-0.5 < cotTheta  &&  cotTheta <= 0.0) {
      m_posP_vphi0_ctm1[fiteration-1]->fill(phi0, posP);
      m_negP_vphi0_ctm1[fiteration-1]->fill(phi0, negP);
      m_d0miss_vphi0_ctm1[fiteration-1]->fill(phi0, d0miss);
   }
   else if (0.0 < cotTheta  &&  cotTheta <= 0.5) {
      m_posP_vphi0_ctp1[fiteration-1]->fill(phi0, posP);
      m_negP_vphi0_ctp1[fiteration-1]->fill(phi0, negP);
      m_d0miss_vphi0_ctp1[fiteration-1]->fill(phi0, d0miss);
   }
   else if (0.5 < cotTheta  &&  cotTheta <= 1.0) {
      m_posP_vphi0_ctp2[fiteration-1]->fill(phi0, posP);
      m_negP_vphi0_ctp2[fiteration-1]->fill(phi0, negP);
      m_d0miss_vphi0_ctp2[fiteration-1]->fill(phi0, d0miss);
   }
   else if (1.0 < cotTheta  &&  cotTheta <= 1.5) {
      m_posP_vphi0_ctp3[fiteration-1]->fill(phi0, posP);
      m_negP_vphi0_ctp3[fiteration-1]->fill(phi0, negP);
      m_d0miss_vphi0_ctp3[fiteration-1]->fill(phi0, d0miss);
   }
   else if (1.5 < cotTheta  &&  cotTheta <= 2.0) {
      m_posP_vphi0_ctp4[fiteration-1]->fill(phi0, posP);
      m_negP_vphi0_ctp4[fiteration-1]->fill(phi0, negP);
      m_d0miss_vphi0_ctp4[fiteration-1]->fill(phi0, d0miss);
   }
}

void
CakeAlignMod::make_aligner_plots(int fiteration, FIFrameIterator itFrame)
{
   FATable<NavTrack> tracks;
   extract(itFrame->record(Stream::kEvent), tracks);
   FATable<NavTrack>::const_iterator first = tracks.begin();
   FATable<NavTrack>::const_iterator second = first;  ++second;

   FAItem<BeamEnergy> beam_energy;
   extract(itFrame->record(Stream::kStartRun), beam_energy);

   HIFitHelix helix_first(*(first->pionHelix()));    helix_first.setIdentifier(first->identifier());
   HIFitHelix helix_second(*(second->pionHelix()));  helix_second.setIdentifier(second->identifier());

   vector<HIFitHelix*> helix_vector;
   helix_vector.push_back(&helix_first);
   helix_vector.push_back(&helix_second);

   if (m_dualConstraint.value()) {
      double particleMass = DBCandidate::mass(DBCandidate::kElectron);
      if (m_useMuPair.value()) particleMass = DBCandidate::mass(DBCandidate::kChargedPion);

      HIDualTrackConstraint constraint(m_halfCrossAngleRadians.value(),
				       beam_energy->value(),
				       HepVector3D(-1, 0, 0),
				       true,                   // DO constrain pT
				       particleMass);

      HIFitConstraint::Results results = constraint.applyConstraint(helix_vector);

   } // end if use dual constraint

   for (vector<HIFitHelix*>::const_iterator helix_iter = helix_vector.begin();
	helix_iter != helix_vector.end();
	++helix_iter) {

      DABoolean east = (-1.265 < (**helix_iter).cotTheta()  &&  (**helix_iter).cotTheta() < -0.8);
      DABoolean west = ( 0.8   < (**helix_iter).cotTheta()  &&  (**helix_iter).cotTheta() <  1.265);
      if (!east && !west) continue;

      HIHelix helix(**helix_iter);
			  
      // Generate DR surfaces (we're going to calculate residuals
      // ourselves, so that they will be relative to the dual tracks,
      // if you decide to use dual tracks)
      vector<HIIntersectionSurface*> surfaces;
      vector<HIDRSurfaceFactory::DRHitAndDriftDistance> drHitInfos;
      HIDRSurfaceFactory drFactory(*itFrame, true, true, true);
      if (! drFactory.geometryAvailable()) {
	 report(ERROR, kFacilityString)
	    << "HIDRSurfaceFactory::geonetryAvailable() failed: skipping event" << endl;
	 return;
      }
      drFactory.getDRHits(&(*(tracks.find(helix.identifier())->seedTrack())),
			  drHitInfos,
			  UsageTag(""),
			  ProductionTag(""),
			  true);              // fittable hits only
      if (drHitInfos.empty()) {
	 report(ERROR, kFacilityString)
	    << "HIDRSurfaceFactory::getDRHits returned no hits: skipping track" << endl;
	 continue;
      }
      drFactory.generateSingleWireSurfaces(surfaces, drHitInfos, NULL, false);

      // Now intersect those surfaces
      HIHelixIntersector intersector(surfaces,
				     HIHelixIntersector::kOrderAsGiven,
				     &helix);
      for (HIHelixIntersector::IntersectionStatus status =
	      intersector.swimToCurrentSurface(KTMoveControl::kDirectionEither);
	   status == HIHelixIntersector::kIntersectionOK;
	   status = intersector.swimToNextIntersection(KTMoveControl::kDirectionEither)) {

	 HIIntersectionSurface* surface = intersector.currentSurface();
	 int layer = ((CalibratedDRHit*) surface->calibratedHit())->layer();
	 if (layer > 16) continue;

	 double phi = helix.phi0();
	 double z = helix.position().z();
	 double drift = ((HICalibratedWireHit*) surface->hiCalibratedHit())->signedDriftDistance();
	 double dca = surface->dca(helix);
	 double residual = drift - dca;

	 m_just_resid[fiteration-1]->fill(residual);
	 if (east) {
	    m_east_resid[fiteration-1]->fill(cake_ring(layer), residual);
	    if (fabs(sin(phi)) < 0.4) {
	       m_east_cosresid[fiteration-1]->fill(cake_ring(layer),  cos(phi) * residual);
	    }
	    if (fabs(cos(phi)) < 0.4) {
	       m_east_sinresid[fiteration-1]->fill(cake_ring(layer), -sin(phi) * residual);
	    }
	 }
	 else if (west) {
	    m_west_resid[fiteration-1]->fill(cake_ring(layer), residual);
	    if (fabs(sin(phi)) < 0.4) {
	       m_west_cosresid[fiteration-1]->fill(cake_ring(layer),  cos(phi) * residual);
	    }
	    if (fabs(cos(phi)) < 0.4) {
	       m_west_sinresid[fiteration-1]->fill(cake_ring(layer), -sin(phi) * residual);
	    }
	 }
      } // end loop over surfaces

   } // end loop over tracks
}

void
CakeAlignMod::make_shifter_plots(int fiteration)
{
   const int f = fiteration - 1;
   const int focus_ring = fiteration - 1;
   const double tomicrons = 1e6;

   double biggest_err = 0.;
   for (int ring = 1;  ring <= focus_ring;  ring++) {
      const int r = ring - 1;

      m_west_rotz_move[f][r] = 0.7*m_east_resid[f]->channelValue(ring) - 1.7*m_west_resid[f]->channelValue(ring);
      m_east_rotz_move[f][r] = 0.7*m_west_resid[f]->channelValue(ring) - 1.7*m_east_resid[f]->channelValue(ring);
      m_west_x_move[f][r] = 0.7*m_east_sinresid[f]->channelValue(ring) - 1.7*m_west_sinresid[f]->channelValue(ring);
      m_east_x_move[f][r] = 0.7*m_west_sinresid[f]->channelValue(ring) - 1.7*m_east_sinresid[f]->channelValue(ring);
      m_west_y_move[f][r] = 0.7*m_east_cosresid[f]->channelValue(ring) - 1.7*m_west_cosresid[f]->channelValue(ring);
      m_east_y_move[f][r] = 0.7*m_west_cosresid[f]->channelValue(ring) - 1.7*m_east_cosresid[f]->channelValue(ring);

      m_west_rotz_err[f][r] = sqrt(sqr(0.7*m_east_resid[f]->channelError(ring)) + sqr(1.7*m_west_resid[f]->channelError(ring)));
      m_east_rotz_err[f][r] = sqrt(sqr(0.7*m_west_resid[f]->channelError(ring)) + sqr(1.7*m_east_resid[f]->channelError(ring)));
      m_west_x_err[f][r] = sqrt(sqr(0.7*m_east_sinresid[f]->channelError(ring)) + sqr(1.7*m_west_sinresid[f]->channelError(ring)));
      m_east_x_err[f][r] = sqrt(sqr(0.7*m_west_sinresid[f]->channelError(ring)) + sqr(1.7*m_east_sinresid[f]->channelError(ring)));
      m_west_y_err[f][r] = sqrt(sqr(0.7*m_east_cosresid[f]->channelError(ring)) + sqr(1.7*m_west_cosresid[f]->channelError(ring)));
      m_east_y_err[f][r] = sqrt(sqr(0.7*m_west_cosresid[f]->channelError(ring)) + sqr(1.7*m_east_cosresid[f]->channelError(ring)));

      if (m_west_rotz_err[f][r] > biggest_err) biggest_err = m_west_rotz_err[f][r];
      if (m_east_rotz_err[f][r] > biggest_err) biggest_err = m_east_rotz_err[f][r];
      if (m_west_x_err[f][r] > biggest_err) biggest_err = m_west_x_err[f][r];
      if (m_east_x_err[f][r] > biggest_err) biggest_err = m_east_x_err[f][r];
      if (m_west_y_err[f][r] > biggest_err) biggest_err = m_west_y_err[f][r];
      if (m_east_y_err[f][r] > biggest_err) biggest_err = m_east_y_err[f][r];

      m_east_rotz[f]->fill(ring, m_east_rotz_move[f][r]*tomicrons - m_east_rotz[f]->channelValue(ring));
      m_east_x[f]->fill(ring, m_east_x_move[f][r]*tomicrons - m_east_x[f]->channelValue(ring));
      m_east_y[f]->fill(ring, m_east_y_move[f][r]*tomicrons - m_east_y[f]->channelValue(ring));
      m_west_rotz[f]->fill(ring, m_west_rotz_move[f][r]*tomicrons - m_west_rotz[f]->channelValue(ring));
      m_west_x[f]->fill(ring, m_west_x_move[f][r]*tomicrons - m_west_x[f]->channelValue(ring));
      m_west_y[f]->fill(ring, m_west_y_move[f][r]*tomicrons - m_west_y[f]->channelValue(ring));

      if (m_east_rotz_move[f][r] != 0.)
	 m_east_rotz_significance[f]->fill(ring, m_east_rotz_err[f][r]/m_east_rotz_move[f][r] - m_east_rotz_significance[f]->channelValue(ring));
      if (m_east_x_move[f][r] != 0.)
	 m_east_x_significance[f]->fill(ring, m_east_x_err[f][r]/m_east_x_move[f][r] - m_east_x_significance[f]->channelValue(ring));
      if (m_east_y_move[f][r] != 0.)
	 m_east_y_significance[f]->fill(ring, m_east_y_err[f][r]/m_east_y_move[f][r] - m_east_y_significance[f]->channelValue(ring));
      if (m_west_rotz_move[f][r] != 0.)
	 m_west_rotz_significance[f]->fill(ring, m_west_rotz_err[f][r]/m_west_rotz_move[f][r] - m_west_rotz_significance[f]->channelValue(ring));
      if (m_west_x_move[f][r] != 0.)
	 m_west_x_significance[f]->fill(ring, m_west_x_err[f][r]/m_west_x_move[f][r] - m_west_x_significance[f]->channelValue(ring));
      if (m_west_y_move[f][r] != 0.)
	 m_west_y_significance[f]->fill(ring, m_west_y_err[f][r]/m_west_y_move[f][r] - m_west_y_significance[f]->channelValue(ring));
   }
}

void
CakeAlignMod::print_status(int fiteration, Severity severity)
{
   const double tomicrons = 1e6;
   const double toinches = 39.3700787402;

   if (fiteration == 1) return;

   const int f = fiteration - 1;
   const int focus_ring = fiteration - 1;

   report(severity, kFacilityString) << "1 0 0 0 0 0 1 5 DEFAULT" << endl;
   report(severity, kFacilityString) << "18" << endl;
   
   for (int i = 1;  i <= 2;  i++) {
      assert(m_geometry[i-1].get_id() == i);
      report(severity, kFacilityString) << i << " "
				      << m_geometry[i-1].get_deltaX() << " "
				      << m_geometry[i-1].get_deltaY() << " "
				      << m_geometry[i-1].get_deltaZ() << " "
				      << m_geometry[i-1].get_rotX() << " "
				      << m_geometry[i-1].get_rotY() << " "
				      << m_geometry[i-1].get_rotZ() << endl;
   }
   for (int i = 3;  i <= 10;  i++) {
      assert(m_geometry[i-1].get_id() == i);
      report(severity, kFacilityString) << i << " "
				      << m_geometry[i-1].get_deltaX() + m_east_x_move[f][i-3]*toinches << " "
				      << m_geometry[i-1].get_deltaY() + m_east_y_move[f][i-3]*toinches << " "
				      << m_geometry[i-1].get_deltaZ() << " "
				      << m_geometry[i-1].get_rotX() << " "
				      << m_geometry[i-1].get_rotY() << " "
				      << m_geometry[i-1].get_rotZ() + m_east_rotz_move[f][i-3]*toinches << endl;
   }
   for (int i = 11;  i <= 18;  i++) {
      assert(m_geometry[i-1].get_id() == i);
      report(severity, kFacilityString) << i << " "
				      << m_geometry[i-1].get_deltaX() + m_west_x_move[f][i-11]*toinches << " "
				      << m_geometry[i-1].get_deltaY() + m_west_y_move[f][i-11]*toinches << " "
				      << m_geometry[i-1].get_deltaZ() << " "
				      << m_geometry[i-1].get_rotX() << " "
				      << m_geometry[i-1].get_rotY() << " "
				      << m_geometry[i-1].get_rotZ() + m_west_rotz_move[f][i-11]*toinches << endl;
   }

   report(severity, kFacilityString)
      << "Shift, Sensitivity to cake ring " << focus_ring << " EAST_X is "
      << m_east_x_move[f][focus_ring-1]*tomicrons << ", " << m_east_x_err[f][focus_ring-1]*tomicrons << " microns after "
      << m_eventlist_pointer << " hits." << endl << "    We need "
      << floor(m_eventlist_pointer * sqr(m_east_x_err[f][focus_ring-1]/0.000100)) << ", "
      << floor(m_eventlist_pointer * sqr(m_east_x_err[f][focus_ring-1]/0.000010)) << ", "
      << floor(m_eventlist_pointer * sqr(m_east_x_err[f][focus_ring-1]/0.000001)) << " events for 100, 10, 1 micron sensitivity." << endl;

   report(severity, kFacilityString)
      << "Shift, Sensitivity to cake ring " << focus_ring << " EAST_Y is "
      << m_east_y_move[f][focus_ring-1]*tomicrons << ", " << m_east_y_err[f][focus_ring-1]*tomicrons << " microns after "
      << m_eventlist_pointer << " hits." << endl << "    We need "
      << floor(m_eventlist_pointer * sqr(m_east_y_err[f][focus_ring-1]/0.000100)) << ", "
      << floor(m_eventlist_pointer * sqr(m_east_y_err[f][focus_ring-1]/0.000010)) << ", "
      << floor(m_eventlist_pointer * sqr(m_east_y_err[f][focus_ring-1]/0.000001)) << " events for 100, 10, 1 micron sensitivity." << endl;

   report(severity, kFacilityString)
      << "Shift, Sensitivity to cake ring " << focus_ring << " EAST_ROTZ is "
      << m_east_rotz_move[f][focus_ring-1]*tomicrons << ", " << m_east_rotz_err[f][focus_ring-1]*tomicrons << " microns after "
      << m_eventlist_pointer << " hits." << endl << "    We need "
      << floor(m_eventlist_pointer * sqr(m_east_rotz_err[f][focus_ring-1]/0.000100)) << ", "
      << floor(m_eventlist_pointer * sqr(m_east_rotz_err[f][focus_ring-1]/0.000010)) << ", "
      << floor(m_eventlist_pointer * sqr(m_east_rotz_err[f][focus_ring-1]/0.000001)) << " events for 100, 10, 1 micron sensitivity." << endl;

   report(severity, kFacilityString)
      << "Shift, Sensitivity to cake ring " << focus_ring << " WEST_X is "
      << m_west_x_move[f][focus_ring-1]*tomicrons << ", " << m_west_x_err[f][focus_ring-1]*tomicrons << " microns after "
      << m_eventlist_pointer << " hits." << endl << "    We need "
      << floor(m_eventlist_pointer * sqr(m_west_x_err[f][focus_ring-1]/0.000100)) << ", "
      << floor(m_eventlist_pointer * sqr(m_west_x_err[f][focus_ring-1]/0.000010)) << ", "
      << floor(m_eventlist_pointer * sqr(m_west_x_err[f][focus_ring-1]/0.000001)) << " events for 100, 10, 1 micron sensitivity." << endl;

   report(severity, kFacilityString)
      << "Shift, Sensitivity to cake ring " << focus_ring << " WEST_Y is "
      << m_west_y_move[f][focus_ring-1]*tomicrons << ", " << m_west_y_err[f][focus_ring-1]*tomicrons << " microns after "
      << m_eventlist_pointer << " hits." << endl << "    We need "
      << floor(m_eventlist_pointer * sqr(m_west_y_err[f][focus_ring-1]/0.000100)) << ", "
      << floor(m_eventlist_pointer * sqr(m_west_y_err[f][focus_ring-1]/0.000010)) << ", "
      << floor(m_eventlist_pointer * sqr(m_west_y_err[f][focus_ring-1]/0.000001)) << " events for 100, 10, 1 micron sensitivity." << endl;

   report(severity, kFacilityString)
      << "Shift, Sensitivity to cake ring " << focus_ring << " WEST_ROTZ is "
      << m_west_rotz_move[f][focus_ring-1]*tomicrons << ", " << m_west_rotz_err[f][focus_ring-1]*tomicrons << " microns after "
      << m_eventlist_pointer << " hits." << endl << "    We need "
      << floor(m_eventlist_pointer * sqr(m_west_rotz_err[f][focus_ring-1]/0.000100)) << ", "
      << floor(m_eventlist_pointer * sqr(m_west_rotz_err[f][focus_ring-1]/0.000010)) << ", "
      << floor(m_eventlist_pointer * sqr(m_west_rotz_err[f][focus_ring-1]/0.000001)) << " events for 100, 10, 1 micron sensitivity." << endl;
}

void
CakeAlignMod::update_geometry(int fiteration)
{
   const int f = fiteration - 1;
   const int focus_ring = fiteration - 1;
   const double tomicrons = 1e6;
   const double toinches = 39.3700787402;
   const double inchtomicron = 25400.;

// +-----------+-----------------+-----------------+-----------------+
// | CAKE RING |        X        |        Y        |      ROT Z      |
// +-----------+-----------------+-----------------+-----------------+
// | 1234 EAST | 123456789012345 | 123456789012345 | 123456789012345 |
// |    `----> | 123456789012345 | 123456789012345 | 123456789012345 |
// |  (diff?)  | 123456789012345 | 123456789012345 | 123456789012345 |
// +-----------+-----------------+-----------------+-----------------+
   char line[300]; // padded for safety

   report(SYSTEM, kFacilityString)
      << "+-----------+-----------------+-----------------+-----------------+" << endl;
   report(SYSTEM, kFacilityString)
      << "| CAKE RING |        X        |        Y        |      ROT Z      |" << endl;
   report(SYSTEM, kFacilityString)
      << "+-----------+-----------------+-----------------+-----------------+" << endl;

   for (int ring = 1;  ring <= kRings;  ring++) {
      const int east = ring - 1 +  3 - 1;
      const int west = ring - 1 + 11 - 1;
      assert(m_geometry[east].get_id() == east+1);
      assert(m_geometry[west].get_id() == west+1);

      const int r = ring - 1;

      sprintf(line, "| %4d EAST | %15.3f | %15.3f | %15.3f |", ring,
	      m_geometry[east].get_deltaX()*inchtomicron, m_geometry[east].get_deltaY()*inchtomicron, m_geometry[east].get_rotZ()*inchtomicron);
      report(SYSTEM, kFacilityString) << line << endl;

      m_geometry[east].set_deltaX(m_geometry[east].get_deltaX() + m_east_x_move[f][r]*toinches);
      m_geometry[east].set_deltaY(m_geometry[east].get_deltaY() + m_east_y_move[f][r]*toinches);
      m_geometry[east].set_rotZ(  m_geometry[east].get_rotZ()   + m_east_rotz_move[f][r]*toinches);

      if (ring > focus_ring) {
	 report(SYSTEM, kFacilityString)
	    << "|           |                 |                 |                 |" << endl
	    << "|           |                 |                 |                 |" << endl;
      }
      else {
	 sprintf(line, "|    `----> | %15.3f | %15.3f | %15.3f |", 
		 m_geometry[east].get_deltaX()*inchtomicron, m_geometry[east].get_deltaY()*inchtomicron, m_geometry[east].get_rotZ()*inchtomicron);
	 report(SYSTEM, kFacilityString) << line << endl;

	 sprintf(line, "|  (diff?)  | %15.3f | %15.3f | %15.3f |", 
		 m_east_x_move[f][r]*tomicrons, m_east_y_move[f][r]*tomicrons, m_east_rotz_move[f][r]*tomicrons);
	 report(SYSTEM, kFacilityString) << line << endl;
      }
      report(SYSTEM, kFacilityString)
	 << "+-----------+-----------------+-----------------+-----------------+" << endl;

      sprintf(line, "| %4d WEST | %15.3f | %15.3f | %15.3f |", ring,
	      m_geometry[west].get_deltaX()*inchtomicron, m_geometry[west].get_deltaY()*inchtomicron, m_geometry[west].get_rotZ()*inchtomicron);
      report(SYSTEM, kFacilityString) << line << endl;

      m_geometry[west].set_deltaX(m_geometry[west].get_deltaX() + m_west_x_move[f][r]*toinches);
      m_geometry[west].set_deltaY(m_geometry[west].get_deltaY() + m_west_y_move[f][r]*toinches);
      m_geometry[west].set_rotZ(  m_geometry[west].get_rotZ()   + m_west_rotz_move[f][r]*toinches);

      if (ring > focus_ring) {
	 report(SYSTEM, kFacilityString)
	    << "|           |                 |                 |                 |" << endl
	    << "|           |                 |                 |                 |" << endl;
      }
      else {
	 sprintf(line, "|    `----> | %15.3f | %15.3f | %15.3f |", 
		 m_geometry[west].get_deltaX()*inchtomicron, m_geometry[west].get_deltaY()*inchtomicron, m_geometry[west].get_rotZ()*inchtomicron);
	 report(SYSTEM, kFacilityString) << line << endl;

	 sprintf(line, "|  (diff?)  | %15.3f | %15.3f | %15.3f |", 
		 m_west_x_move[f][r]*tomicrons, m_west_y_move[f][r]*tomicrons, m_west_rotz_move[f][r]*tomicrons);
	 report(SYSTEM, kFacilityString) << line << endl;
      }
      report(SYSTEM, kFacilityString)
	 << "+-----------+-----------------+-----------------+-----------------+" << endl;
   }
}

void
CakeAlignMod::print_geomfile(int fiteration)
{
   char name[30];  // padded for safety
   sprintf(name, "fiter%d.drgeomalignment", fiteration);
   ofstream geom(name);

   geom << "1 0 0 0 0 0 1 5 DEFAULT" << endl;
   geom << "18" << endl;
   
   for (int i = 0;  i < 18;  i++) {
      assert(m_geometry[i].get_id() == i+1);
      geom << i+1 << " "
	   << m_geometry[i].get_deltaX() << " "
	   << m_geometry[i].get_deltaY() << " "
	   << m_geometry[i].get_deltaZ() << " "
	   << m_geometry[i].get_rotX() << " "
	   << m_geometry[i].get_rotY() << " "
	   << m_geometry[i].get_rotZ() << endl;
   }
}

void
CakeAlignMod::print_kumac(int fiteration)
{
   const int f = fiteration-1;

   char name[30];  // padded for safety
   sprintf(name, "fiter%d.kumac", fiteration);
   ofstream kumac(name);
   
   int id = 0;
   for(vector<HIHist1D*>::const_iterator iter1d = m_hist1d[f].begin();
       iter1d != m_hist1d[f].end();
       ++iter1d) {
      assert(*iter1d);
      id++;

      int channels = (**iter1d).nChannels();
      int i;
      kumac << "h/cr/1d " << id << " \'" << (**iter1d).title() << "\' "
	    << channels << " " << (**iter1d).lowEdge() << " " << (**iter1d).highEdge() << endl;
      kumac << "ve/cr v(" << channels << ")" << endl;
      kumac << "ve/cr e(" << channels << ")" << endl;
      i = 1;
      do {
	 int end = i + 9;
	 kumac << "ve/inp v(" << i << ":" << (end < channels ? end : channels) << ") ";
	 for (;  i <= end  &&  i <= channels;  i++) kumac << (**iter1d).channelValue(i) << " ";
	 if (i <= channels) kumac << endl;
      } while (i <= channels);
      kumac << endl;
      i = 1;
      do {
	 int end = i + 9;
	 kumac << "ve/inp e(" << i << ":" << (end < channels ? end : channels) << ") ";
	 for (;  i <= end  &&  i <= channels;  i++) kumac << (**iter1d).channelError(i) << " ";
	 if (i <= channels) kumac << endl;
      } while (i <= channels);
      kumac << endl;
      kumac << "h/put_vect/contents " << id << " v" << endl;
      kumac << "h/put_vect/errors " << id << " e" << endl;
      kumac << endl;
   }

   for(vector<HIHistProf*>::const_iterator iterprof = m_histprof[f].begin();
       iterprof != m_histprof[f].end();
       ++iterprof) {
      assert(*iterprof);
      id++;

      int channels = (**iterprof).nChannels();
      int i;
      kumac << "h/cr/1d " << id << " \'" << (**iterprof).title() << "\' "
	    << channels << " " << (**iterprof).lowEdgeX() << " " << (**iterprof).highEdgeX() << endl;
      kumac << "ve/cr v(" << channels << ")" << endl;
      kumac << "ve/cr e(" << channels << ")" << endl;
      i = 1;
      do {
	 int end = i + 9;
	 kumac << "ve/inp v(" << i << ":" << (end < channels ? end : channels) << ") ";
	 for (;  i <= end  &&  i <= channels;  i++) kumac << (**iterprof).channelValue(i) << " ";
	 if (i <= channels) kumac << endl;
      } while (i <= channels);
      kumac << endl;
      i = 1;
      do {
	 int end = i + 9;
	 kumac << "ve/inp e(" << i << ":" << (end < channels ? end : channels) << ") ";
	 for (;  i <= end  &&  i <= channels;  i++) kumac << (**iterprof).channelError(i) << " ";
	 if (i <= channels) kumac << endl;
      } while (i <= channels);
      kumac << endl;
      kumac << "h/put_vect/contents " << id << " v" << endl;
      kumac << "h/put_vect/errors " << id << " e" << endl;
      kumac << endl;
   }
}
