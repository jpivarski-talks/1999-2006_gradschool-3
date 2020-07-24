// -*- C++ -*-
//
// Package:     DRAlignmentBhabhaFilterProc
// Module:      DRAlignmentBhabhaFilterProc
// 
// Description: Selects bhabha events for DRAlignmentMod
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Dec  5 12:08:39 EST 2003
// $Id: DRAlignmentBhabhaFilterProc.cc,v 1.1.1.1 2003/12/05 17:54:58 mccann Exp $
//
// Revision history
//
// $Log: DRAlignmentBhabhaFilterProc.cc,v $
// Revision 1.1.1.1  2003/12/05 17:54:58  mccann
// imported DRAlignmentBhabhaFilterProc sources
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "DRAlignmentBhabhaFilterProc/DRAlignmentBhabhaFilterProc.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "BeamEnergy/BeamEnergy.h"
#include "Navigation/NavTrack.h"

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
static const char* const kFacilityString = "Processor.DRAlignmentBhabhaFilterProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DRAlignmentBhabhaFilterProc.cc,v 1.1.1.1 2003/12/05 17:54:58 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//



//
// constructors and destructor
//
DRAlignmentBhabhaFilterProc::DRAlignmentBhabhaFilterProc( void )               // anal1
   : Processor( "DRAlignmentBhabhaFilterProc" )
   , m_crossingangle_x("crossingAngleX", this, 0.015)  // appropriate for early cleo-c runs
   , m_crossingangle_y("crossingAngleY", this, 0.)
   , m_asymmetric_beamz("asymmetricBeamZ", this, 0.)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &DRAlignmentBhabhaFilterProc::event,    Stream::kEvent );
   //bind( &DRAlignmentBhabhaFilterProc::beginRun, Stream::kBeginRun );
   //bind( &DRAlignmentBhabhaFilterProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

DRAlignmentBhabhaFilterProc::~DRAlignmentBhabhaFilterProc()                    // anal5
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
DRAlignmentBhabhaFilterProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DRAlignmentBhabhaFilterProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
DRAlignmentBhabhaFilterProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   m_good_tracks = iHistoManager.histogram(
      "good tracks", 7, -0.5, 6.5);
   m_bad_tracks = iHistoManager.histogram(
      "bad tracks", 7, -0.5, 6.5);
   m_charge = iHistoManager.histogram(
      "charge", 7, -3.5, 3.5);
   m_d0miss = iHistoManager.histogram(
      "d0?pos! + d0?neg!", 100, -0.001, 0.001);
   m_z0miss = iHistoManager.histogram(
      "z0?pos! - z0?neg!", 100, -0.03, 0.03);
   m_tracks_momentum_phi = iHistoManager.histogram(
      "tracks momentum phi", 64, -M_PI, M_PI);
   m_tracks_momentum_perp = iHistoManager.histogram(
      "tracks momentum perp", 100, 0., 0.05);
   m_tracks_momentum_z = iHistoManager.histogram(
      "tracks momentum z", 100, -0.05, 0.05);

   m_pertrack_fit = iHistoManager.histogram(
      "(pertrack) fit succeeded", 2, -0.5, 1.5);
   m_pertrack_reduced_chi2 = iHistoManager.histogram(
      "(pertrack) [h]^2! / dof", 100, 0., 20.);
   m_pertrack_hitfraction = iHistoManager.histogram(
      "(pertrack) layers hit / expected", 150, 0., 1.5);
   m_pertrack_d0 = iHistoManager.histogram(
      "(pertrack) d0", 100, -0.01, 0.01);
   m_pertrack_z0 = iHistoManager.histogram(
      "(pertrack) z0", 100, -0.1, 0.1);
   m_pertrack_p = iHistoManager.histogram(
      "(pertrack) pmag", 150, 0., 1.5);
   m_pertrack_cotTheta = iHistoManager.histogram(
      "(pertrack) cotTheta", 100, -3., 3.);
   m_pertrack_err_cT = iHistoManager.histogram(
      "(pertrack) err?cotTheta!", 100, 0., 1.);
   m_pertrack_err_z0 = iHistoManager.histogram(
      "(pertrack) err?z0!", 100, 0., 0.5);
   m_pertrack_numdrhits = iHistoManager.histogram(
      "(pertrack) \"f# DR hits", 17, -0.5, 16.5);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
DRAlignmentBhabhaFilterProc::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<BeamEnergy> beam_energy;
   extract(iFrame.record(Stream::kStartRun), beam_energy);

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   FATable<NavTrack>::const_iterator track_iter = tracks.begin();
   FATable<NavTrack>::const_iterator track_end = tracks.end();

   int good_tracks = 0;
   int bad_tracks = 0;
   int charge = 0;
   double d0miss = 0.;
   double z0miss = 0.;
   HepVector3D tracks_momentum(m_crossingangle_x.value(),
			       m_crossingangle_y.value(),
			       m_asymmetric_beamz.value());
   for (track_iter = tracks.begin();  track_iter != track_end;  ++track_iter) {
      bool fit = (track_iter->pionQuality()->fit() &&
		  !track_iter->pionQuality()->fitAbort());
      double dof = track_iter->pionQuality()->degreesOfFreedom();
      double chi2 = track_iter->pionQuality()->chiSquare();
      double nexpect = track_iter->seedQuality()->numberHitsExpected();
      double nlayers = track_iter->seedQuality()->numberLayers();
      int q = (track_iter->pionHelix()->curvature() > 0. ? 1 : -1);
      double d0 = track_iter->pionHelix()->d0();
      double z0 = track_iter->pionHelix()->z0();
      HepVector3D p = track_iter->pionFit()->momentum();
      double cotTheta = track_iter->pionHelix()->cotTheta();
      double err_cT = sqrt(fabs(track_iter->pionHelix()->errorMatrix()
				(KTHelix::kCotTheta, KTHelix::kCotTheta)));
      double err_z0 = sqrt(fabs(track_iter->pionHelix()->errorMatrix()
				(KTHelix::kZ0, KTHelix::kZ0)));
      int numdrhits = track_iter->pionQuality()->numberSubdetectorHitLayers(TRSubdetectorLists::kDR3Axial);

      if (fit                                   &&
	  dof > 0                               &&
	  chi2 > 0                              &&
	  chi2/dof < 10.                        &&
	  nexpect > 0                           &&
 	  nlayers/nexpect > 0.5                 &&
 	  nlayers/nexpect < 1.2                 &&
	  fabs(d0) < 0.005                      &&
	  fabs(z0) < 0.05                       &&
	  p.mag() > 0.9 * beam_energy->value()  &&
	  p.mag() < 1.1 * beam_energy->value()  &&
 	  fabs(cotTheta) < 2.4                  &&
	  err_cT < 0.5                          &&
	  err_z0 < 0.25                         &&
	  numdrhits > 8                         &&
	 true) good_tracks++;
      else bad_tracks++;

      charge += q;
      d0miss += d0;
      z0miss += z0 * double(q);
      tracks_momentum += p;

      m_pertrack_fit->fill(fit);
      if (dof > 0) m_pertrack_reduced_chi2->fill(chi2/dof);
      if (nexpect > 0) m_pertrack_hitfraction->fill(nlayers/nexpect);
      m_pertrack_d0->fill(d0);
      m_pertrack_z0->fill(z0);
      m_pertrack_p->fill(p.mag() / beam_energy->value());
      m_pertrack_cotTheta->fill(cotTheta);
      m_pertrack_err_cT->fill(err_cT);
      m_pertrack_err_z0->fill(err_z0);
      m_pertrack_numdrhits->fill(numdrhits);
   }

   bool pass_good_tracks = (good_tracks == 2);
   bool pass_bad_tracks = (bad_tracks == 0);
   bool pass_charge = (charge == 0);
   bool pass_d0miss = (fabs(d0miss) < 0.0005);
   bool pass_z0miss = (fabs(z0miss) < 0.015);
   bool pass_tracks_momentum_perp = (tracks_momentum.perp() / 2. / beam_energy->value() < 0.02);
   bool pass_tracks_momentum_z = (fabs(tracks_momentum.z()) / 2. / beam_energy->value() < 0.02);

   if (pass_charge            	   &&
       pass_d0miss            	   &&
       pass_z0miss            	   &&
       pass_tracks_momentum_perp   &&
       pass_tracks_momentum_z      &&
       true) {
      m_good_tracks->fill(good_tracks);
      m_bad_tracks->fill(bad_tracks);
   }

   if (pass_good_tracks       	   &&
       pass_bad_tracks        	   &&
       pass_d0miss            	   &&
       pass_z0miss            	   &&
       pass_tracks_momentum_perp   &&
       pass_tracks_momentum_z      &&
       true) m_charge->fill(charge);

   if (pass_good_tracks       	   &&
       pass_bad_tracks        	   &&
       pass_charge            	   &&
       pass_z0miss            	   &&
       pass_tracks_momentum_perp   &&
       pass_tracks_momentum_z      &&
       true) m_d0miss->fill(d0miss);

   if (pass_good_tracks       	   &&
       pass_bad_tracks        	   &&
       pass_charge            	   &&
       pass_d0miss            	   &&
       pass_tracks_momentum_perp   &&
       pass_tracks_momentum_z      &&
       true) m_z0miss->fill(z0miss);

   if (pass_good_tracks       	   &&
       pass_bad_tracks        	   &&
       pass_charge            	   &&
       pass_d0miss            	   &&
       pass_z0miss            	   &&
       pass_tracks_momentum_z      &&
       true) {
      m_tracks_momentum_phi->fill(tracks_momentum.phi());
      m_tracks_momentum_perp->fill(tracks_momentum.perp() / 2. / beam_energy->value());
   }

   if (pass_good_tracks       	   &&
       pass_bad_tracks        	   &&
       pass_charge            	   &&
       pass_d0miss            	   &&
       pass_z0miss            	   &&
       pass_tracks_momentum_perp   &&
       true) m_tracks_momentum_z->fill(tracks_momentum.z() / 2. / beam_energy->value());

   // The Final Cut
   if (pass_good_tracks       	   &&
       pass_bad_tracks        	   &&
       pass_charge            	   &&
       pass_d0miss            	   &&
       pass_z0miss            	   &&
       pass_tracks_momentum_perp   &&
       pass_tracks_momentum_z      &&
       true) return ActionBase::kPassed;
   else return ActionBase::kFailed;
}

/*
ActionBase::ActionResult
DRAlignmentBhabhaFilterProc::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
DRAlignmentBhabhaFilterProc::endRun( Frame& iFrame )         // anal4 equiv.
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
