// -*- C++ -*-
//
// Package:     <DRAlignmentMod>
// Module:      DRAlignmentMod
// 
// Description: Allows global minimization of the DR via minuit "set par" commands, or automatically via "mini"
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Mon Dec  8 11:51:47 EST 2003
// $Id: 
//
// Revision history
//

#include "Experiment/Experiment.h"

// system include files
#include <stdio.h>
#include <fstream.h>
#include <iostream.h>

// user include files
#include "Experiment/report.h"
#include "DRAlignmentMod/DRAlignmentMod.h"
#include "DRAlignmentMod/DRAlignmentFcn.h"
#include "DataHandler/Stream.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "DataHandler/Frame.h"
#include "Navigation/NavTrack.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "MagField/MagneticField.h"
#include "DRAlignmentMod/DRAlignmentDualTrackConstraint.h"
#include "HelixIntersection/HIFitHelix.h"

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
static const char* const kFacilityString = "DRAlignmentMod.DRAlignmentMod" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRAlignmentMod.cc,v 1.2 2003/12/15 18:42:41 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DRAlignmentMod::DRAlignmentMod()
   : FrameIteratorModuleBase( "DRAlignmentMod", "This is the awesomest module." )
   , m_geometry(new DBDRGeomAlignment[18], 18)
   , m_geometry_holder(&m_geometry)
   , m_first_time(true)
   , m_geomName("geom", this, "best.zdgeomalignment")
   , m_ring("ring", this, 1)
   , m_minDrift("minDrift", this, 1.5)  // these are in mm, of course
   , m_maxDrift("maxDrift", this, 3.8)
   , m_showPlots("showPlots", this, true)
   , m_interactive("interactive", this, true)
   , m_fakeDriftFunction("fakeDriftFunction", this, false)
   , m_crossingangle_x("crossingAngleX", this, 0.015)  // appropriate for early cleo-c runs
   , m_crossingangle_y("crossingAngleY", this, 0.)
   , m_asymmetric_beamz("asymmetricBeamZ", this, 0.)
   , m_fcn(&m_geometry)
   , m_minuit(MinuitInterface::instance())
{
   registerProxyFactory(Stream::kDRAlignment,
			m_geometry_holder.makeFactory(),
			UsageTag());

   //You must set what streams you which to iterate over
   //  that is, what events should the Frames be stopped on
   iterateOver(Stream::kEvent);

   m_minuit->loadFcn(m_fcn);

   if (full_moon()) {
      report(SYSTEM, kFacilityString)
	 << "Three Rings for the Elvin-kings under the sky," << endl
	 << "Seven for the Dwarf-lords in the halls of stone," << endl
	 << "Nine for Mortal Men doomed to die," << endl
	 << "One for the Dark Lord on his dark throne." << endl
	 << "In the Land of Mordor where the Shadows lie," << endl
	 << "One Ring to rule them all, One Ring to find them," << endl
	 << "One Ring to bring them all" << endl
	 << "And in the Darkness bind them," << endl
	 << "In the Land of Mordor where the Shadows lie." << endl;
   }
}

// DRAlignmentMod::DRAlignmentMod( const DRAlignmentMod& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DRAlignmentMod::~DRAlignmentMod()
{
}

//
// assignment operators
//
// const DRAlignmentMod& DRAlignmentMod::operator=( const DRAlignmentMod& rhs )
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

void
DRAlignmentMod::iterate( const FIFrameIterator& iBegin,
			 const FIFrameIterator& iEnd )
{
   if (m_first_time) {
      ifstream geometry_file(m_geomName.value().c_str());
      if (! m_geometry.readFromStream(geometry_file)) assert(false);

      const unsigned int eastindex = m_ring.value() + 2;
      const unsigned int westindex = m_ring.value() + 10;

      m_minuit->changeParameterStart(DRAlignmentFcn::kXEast, m_geometry[eastindex-1].get_deltaX()*25.4);
      m_minuit->changeParameterStart(DRAlignmentFcn::kYEast, m_geometry[eastindex-1].get_deltaY()*25.4);
      m_minuit->changeParameterStart(DRAlignmentFcn::kPhiEast, m_geometry[eastindex-1].get_rotZ()*25.4);
      m_minuit->changeParameterStart(DRAlignmentFcn::kXWest, m_geometry[westindex-1].get_deltaX()*25.4);
      m_minuit->changeParameterStart(DRAlignmentFcn::kYWest, m_geometry[westindex-1].get_deltaY()*25.4);
      m_minuit->changeParameterStart(DRAlignmentFcn::kPhiWest, m_geometry[westindex-1].get_rotZ()*25.4);

      m_first_time = false;
   }

   m_fcn.reset(iBegin);

   for(FIFrameIterator itFrame = iBegin;  itFrame != iEnd;  ++itFrame) {
      m_fcn.book(m_showPlots.value(), m_fakeDriftFunction.value(), m_ring.value());

      FATable<NavTrack> tracks;
      extract(itFrame->record(Stream::kEvent), tracks);

      FATable<CalibratedDRHit> allhits;
      extract(itFrame->record(Stream::kEvent), allhits);

      FAItem<MagneticField> magnetic_field;
      extract(itFrame->record(Stream::kStartRun), magnetic_field);

      FATable<NavTrack>::const_iterator one = tracks.begin();
      FATable<NavTrack>::const_iterator two = tracks.begin();  ++two;
      HIFitHelix dualone(*one->pionHelix());
      HIFitHelix dualtwo(*two->pionHelix());
      dualone.setIdentifier(one->identifier());
      dualtwo.setIdentifier(two->identifier());
      vector<HIFitHelix*> dualtracks;
      dualtracks.push_back(&dualone);
      dualtracks.push_back(&dualtwo);

      DRAlignmentDualTrackConstraint constraint(true,
						true,
						magnetic_field->BField().z(),
						HepVector3D(m_crossingangle_x.value(),
							    m_crossingangle_y.value(),
							    m_asymmetric_beamz.value()));
      HIFitConstraint::Results results = constraint.applyConstraint(dualtracks);

      if (results.chisq()/results.ndof() > 20.) continue;
      if (fabs(dualtracks[0]->cotTheta()) > 2.) continue;
      if (fabs(dualtracks[1]->cotTheta()) > 2.) continue;

      for (int i = 0;  i < 2;  i++) {
	 const KTHelix& helix = *dualtracks[i];
	 FATable<NavTrack>::const_iterator navtrack_iter = tracks.find(dualtracks[i]->identifier());
	 const NavTrack::SeedDRHitLinkTable* links = navtrack_iter->seedDRHitLinks();
	 NavTrack::SeedDRHitLinkTable::const_iterator link_end = links->end();

	 double charge_sum = 0.;
	 unsigned int charge_N = 0;
	 vector<const CalibratedDRHit*> hits;
	 vector<double> drifts;

	 for (NavTrack::SeedDRHitLinkTable::const_iterator link_iter = links->begin();
	      link_iter != link_end;
	      ++link_iter) {

	    if ((**link_iter).linkData().fitFlag() == SeedDRHitLink::kFITTABLE) {
	       assert(*(**link_iter).rightID());
	       FATable<CalibratedDRHit>::const_iterator hit_iter = allhits.find(*(**link_iter).rightID());

	       charge_sum += hit_iter->charge();  charge_N++;

	       if (cake_ring(hit_iter->layer()) == m_ring.value()         &&
		   fabs(hit_iter->distance()) > m_minDrift.value()/1000.  &&
		   fabs(hit_iter->distance()) < m_maxDrift.value()/1000.    ) {
		  hits.push_back(&*hit_iter);
		  drifts.push_back((**link_iter).linkData().signedDriftDistance());
	       }

	    } // end if hit is kFITTABLE
	 } // end loop over hit links

	 if (!hits.empty()) m_fcn.fill(helix, charge_sum/double(charge_N), hits, drifts);
      } // end loop over tracks
   } // end loop over events

   if (m_interactive.value()) {
      m_minuit->setDiagLevel(MinuitInterface::kMute);
      for (int i = DRAlignmentFcn::kT0;  i < DRAlignmentFcn::kNumParams;  i++) m_minuit->fixParameter(i);
      m_minuit->setDiagLevel(MinuitInterface::kNormal);
      m_minuit->interact();
   }
   else {
      m_minuit->setDiagLevel(MinuitInterface::kMax);
      for (int i = DRAlignmentFcn::kT0;  i < DRAlignmentFcn::kNumParams;  i++) m_minuit->fixParameter(i);
      m_minuit->runMigrad();
   }
}

//
// const member functions
//

int
DRAlignmentMod::cake_ring(int layer) const
{
   // layer number: 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
   // cake ring:       1     2     3     4     5     6     7     8
   return (layer + 1) / 2;
}

bool
DRAlignmentMod::full_moon() const
{
   const double synodic = 29.53058867;  // the synodic period in days
   const int new_moon = 914016960;      // Fri Dec 18 16:36:00 EST 1998
   int now = time(NULL);

   double phase = fabs(double(now - new_moon)) / (synodic * 24.*60.*60.) * 100.;
   while (phase > 100.) phase -= 100.;

   return (phase >= 47.5  &&  phase <= 52.49);  // between two new moons (50%)
}

//
// static member functions
//



// for comparing fitter's residuals with mine
// 	 const NavTrack::DRHitLinkTable* pionLinks = tracks[i].pionDRHitLinks();
// 	 NavTrack::DRHitLinkTable::const_iterator pionLink_end = pionLinks->end();
// 	 for (NavTrack::DRHitLinkTable::const_iterator pionLink_iter = pionLinks->begin();
// 	      pionLink_iter != pionLink_end;
// 	      ++pionLink_iter) {
// 	    cout <<  "FITTER SAYS " << (**pionLink_iter).linkData().signedDriftDistance() << " "
// 		 << (**pionLink_iter).linkData().signedDcaToWire() << " "
// 		 << (**pionLink_iter).linkData().residual() << endl;
// 	 }
