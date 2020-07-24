// -*- C++ -*-
//
// Package:     CompareDRandZD
// Module:      CompareDRandZD
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 10:45:19 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "CompareDRandZD/CompareDRandZD.h"
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

#include "TrackRoot/TRSeedTrack.h"
#include "TrackRoot/TRHelixPionFit.h"
#include "McCannFilter/McCannPionQuality.h"
#include "BeamEnergy/BeamEnergy.h"
#include "MagField/MagneticField.h"
#include "C3cc/CcShowerAttributes.h"

// too many, but who cares?
#include "C3dr/DrStream.h"
#include "CleoDB/DBEventHeader.h"
#include "Navigation/NavTrack.h"
#include "BeamEnergy/BeamEnergy.h"
#include "MagField/MagneticField.h"
#include "C3cc/CcShowerAttributes.h"
#include "HelixIntersection/HIDualTrackConstraint.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIZDSurfaceFactory.h"
#include "HelixIntersection/HICalibratedWireHit.h"
#include "HelixIntersection/HIFitHelix.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "TrackFitter/TrackFitZDHitLink.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "AZDGeom/AZDSenseWireStore.h"

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
static const char* const kFacilityString = "Processor.CompareDRandZD" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
CompareDRandZD::CompareDRandZD( void )               // anal1
   : Processor( "CompareDRandZD" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &CompareDRandZD::event,    Stream::kEvent );
   //bind( &CompareDRandZD::beginRun, Stream::kBeginRun );
   //bind( &CompareDRandZD::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

CompareDRandZD::~CompareDRandZD()                    // anal5
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
CompareDRandZD::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CompareDRandZD::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
CompareDRandZD::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

//    const char* label[kNumEntries] = {
//       "curv", "d", "phi", "ct", "z",
//       "deld", "delphi", "delct", "delz"};
//    m_ntuple = iHistoManager.ntuple(
//       1, "vars", kNumEntries, 262144, label);

//    const char* labelhit[kNumHitEntries] = {
//       "layer", "wire", "z", "phi", "drift", "dca", "resid"};
//    m_hitntuple = iHistoManager.ntuple(
//       2, "vars", kNumHitEntries, 262144, labelhit);

   m_delphi0 = iHistoManager.histogram(
      100, "[D]phi?0!", 100, -0.3, 0.3);
   m_deld0_vphi0 = iHistoManager.profile(
      200, "[D]d?0! VS phi?0!", 30, 0., 2.*M_PI, -0.05, 0.05, HIHistProf::kErrorOnMean);
   m_deld0 = iHistoManager.histogram(
      201, "[D]d?0!", 100, -0.05, 0.05);
   m_delz0 = iHistoManager.histogram(
      300, "[D]z?0!", 100, -0.2, 0.2);
   m_delct_vphi0 = iHistoManager.profile(
      400, "[D]cotTheta VS phi?0!", 30, 0., 2.*M_PI, -0.5, 0.5, HIHistProf::kErrorOnMean);
   m_delct = iHistoManager.histogram(
      401, "[D]cotTheta", 100, -0.5, 0.5);

   m_drhits = iHistoManager.histogram(
      500, "DR Hits on bhabhas", 61, -0.5, 60.5);
   m_zdhits = iHistoManager.histogram(
      501, "ZD Hits on bhabhas", 11, -0.5, 10.5);

   m_resid_v_phi_1 = iHistoManager.profile(
      601, "resid V phi L1", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_resid_v_phi_2 = iHistoManager.profile(
      602, "resid V phi L2", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_resid_v_phi_3 = iHistoManager.profile(
      603, "resid V phi L3", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_resid_v_phi_4 = iHistoManager.profile(
      604, "resid V phi L4", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_resid_v_phi_5 = iHistoManager.profile(
      605, "resid V phi L5", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_resid_v_phi_6 = iHistoManager.profile(
      606, "resid V phi L6", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);

   m_resid_v_phi_uplus = iHistoManager.profile(
      701, "resid V phi UPLUS", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_resid_v_phi_vplus = iHistoManager.profile(
      702, "resid V phi VPLUS", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_resid_v_phi_uminus = iHistoManager.profile(
      703, "resid V phi UMINUS", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);
   m_resid_v_phi_vminus = iHistoManager.profile(
      704, "resid V phi VMINUS", 15, -M_PI, M_PI, -0.01, 0.01, HIHistProf::kErrorOnMean);

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
CompareDRandZD::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   if (notbhabha(iFrame)) return ActionBase::kFailed;

   FATable<TRHelixPionFit> mtracks;
   extract(iFrame.record(Stream::kEvent), mtracks, "McCann");
   FATable<TRHelixPionFit>::const_iterator mtrack_iter = mtracks.begin();
   FATable<TRHelixPionFit>::const_iterator mtrack_end = mtracks.end();

   FATable<TRHelixPionFit> ptracks;
   extract(iFrame.record(Stream::kEvent), ptracks, "Pivarski");
   FATable<TRHelixPionFit>::const_iterator ptrack_end = ptracks.end();

   FAItem<SeedTrackDRHitLattice> drlattice;
   extract(iFrame.record(Stream::kEvent), drlattice);

   FAItem<SeedTrackZDHitLattice> zdlattice;
   extract(iFrame.record(Stream::kEvent), zdlattice);

   FAItem<PionFitZDHitLattice> zdpionlattice;
   extract(iFrame.record(Stream::kEvent), zdpionlattice, "McCann");

   FATable<CalibratedZDHit> calibratedzdhits;
   extract(iFrame.record(Stream::kEvent), calibratedzdhits);

   FAItem<AZDSenseWireStore> store;
   extract(iFrame.record(Stream::kZDAlignment), store);

   for (; mtrack_iter != mtrack_end;  ++mtrack_iter) {
      FATable<TRHelixPionFit>::const_iterator ptrack_iter = ptracks.find(mtrack_iter->identifier());
      if (ptrack_iter == ptrack_end) continue;

      int drhits = 0;
      int zdhits = 0;
      const SeedTrackDRHitLattice::VectorRightID* vect_drhits =
	 drlattice->vRightGivenLeft(mtrack_iter->identifier());
      const SeedTrackZDHitLattice::VectorRightID* vect_zdhits =
	 zdlattice->vRightGivenLeft(mtrack_iter->identifier());
      if (vect_drhits != NULL) drhits = vect_drhits->size();
      if (vect_zdhits != NULL) zdhits = vect_zdhits->size();
      m_drhits->fill(drhits);
      m_zdhits->fill(zdhits);

      if (zdhits >= 5) {
	 float v[kNumEntries];
	 v[kCurv] = mtrack_iter->curvature();
	 v[kD] = mtrack_iter->d0();
	 v[kPhi] = mtrack_iter->phi0();
	 v[kCt] = mtrack_iter->cotTheta();
	 v[kZ] = mtrack_iter->z0();
	 v[kDelD] = ptrack_iter->d0() - mtrack_iter->d0();
	 v[kDelPhi] = ptrack_iter->phi0() - mtrack_iter->phi0();
	 if (v[kDelPhi] > M_PI) v[kDelPhi] -= 2.*M_PI;
	 v[kDelCt] = ptrack_iter->cotTheta() - mtrack_iter->cotTheta();
	 v[kDelZ] = ptrack_iter->z0() - mtrack_iter->z0();
//	 m_ntuple->fill(v);

	 m_delphi0->fill(v[kDelPhi]);
	 m_deld0_vphi0->fill(v[kPhi], v[kDelD]);
	 m_deld0->fill(v[kDelD]);
	 m_delz0->fill(v[kDelZ]);
	 m_delct_vphi0->fill(v[kPhi], v[kDelCt]);
	 m_delct->fill(v[kDelCt]);
      }

      const PionFitZDHitLattice::Links& zdlinks = zdpionlattice->linksGivenLeft(mtrack_iter->identifier());
      PionFitZDHitLattice::Links::const_iterator zdlinks_iter = zdlinks.begin();
      PionFitZDHitLattice::Links::const_iterator zdlinks_end = zdlinks.end();
      for (; zdlinks_iter != zdlinks_end;  ++zdlinks_iter) {
	 TrackFitZDHitLink& zdlink = (**zdlinks_iter).linkData();
	 const PionFitZDHitLattice::VectorRightID& hitids = (**zdlinks_iter).vRightID();
	 assert(hitids.size() == 1);
	 FATable<CalibratedZDHit>::const_iterator zdhit = calibratedzdhits.find(*hitids.begin());

	 KTHelix helix(*mtrack_iter);
	 double dummy;
	 helix.moveToRadius(store->layer(zdhit->layer()).radiusNominal(), dummy,
			    KTMoveControl::kDirectionEither);

	 float u[kNumHitEntries];
	 u[kLayer] = zdhit->layer();
	 u[kWire] = zdhit->wireInLyr();
 	 u[kZHit] = helix.position().z();
 	 u[kPhiHit] = helix.position().phi();
 	 u[kDrift] = zdlink.signedDriftDistance();
 	 u[kDCA] = zdlink.signedDcaToWire();
	 u[kResidual] = zdlink.residual();

// 	 m_hitntuple->fill(u);

	 if (u[kLayer] == 1) m_resid_v_phi_1->fill(u[kPhiHit], u[kResidual]);
	 if (u[kLayer] == 2) m_resid_v_phi_2->fill(u[kPhiHit], u[kResidual]);
	 if (u[kLayer] == 3) m_resid_v_phi_3->fill(u[kPhiHit], u[kResidual]);
	 if (u[kLayer] == 4) m_resid_v_phi_4->fill(u[kPhiHit], u[kResidual]);
	 if (u[kLayer] == 5) m_resid_v_phi_5->fill(u[kPhiHit], u[kResidual]);
	 if (u[kLayer] == 6) m_resid_v_phi_6->fill(u[kPhiHit], u[kResidual]);

	 if (u[kLayer] <= 3) {
	    if (u[kZHit] < 0.) m_resid_v_phi_uminus->fill(u[kPhiHit], u[kResidual]);
	    else m_resid_v_phi_uplus->fill(u[kPhiHit], u[kResidual]);
	 }
	 else {
	    if (u[kZHit] < 0.) m_resid_v_phi_vminus->fill(u[kPhiHit], u[kResidual]);
	    else m_resid_v_phi_vplus->fill(u[kPhiHit], u[kResidual]);
	 }

      } // end loop over zd hit links

   } // end loop over tracks

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
CompareDRandZD::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
CompareDRandZD::endRun( Frame& iFrame )         // anal4 equiv.
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

DABoolean
CompareDRandZD::notbhabha(Frame& iFrame)
{
   FAItem<BeamEnergy> beam_energy;
   extract(iFrame.record(Stream::kStartRun), beam_energy);

   FAItem<MagneticField> magnetic_field;
   extract(iFrame.record(Stream::kStartRun), magnetic_field);

   FATable<CcShowerAttributes> showers;
   extract(iFrame.record(Stream::kEvent), showers);
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
   extract(iFrame.record(Stream::kEvent), seeds);
   if (seeds.size() != 2 ) return true;

   FAItem<SeedTrackDRHitLattice> drlattice;
   extract(iFrame.record(Stream::kEvent), drlattice);
   
   for (FATable<TRSeedTrack>::const_iterator seed_iter = seeds.begin();
	seed_iter != seeds.end();
	++seed_iter) {
      int drhits = 0;
      const SeedTrackDRHitLattice::VectorRightID* vect_drhits =
	 drlattice->vRightGivenLeft(seed_iter->identifier());
      if (vect_drhits == NULL  ||  vect_drhits->size() < 42) return true;
   }

   FATable<TRHelixPionFit> tracks;
   extract(iFrame.record(Stream::kEvent), tracks, "McCann");
   FATable<TRHelixPionFit>::const_iterator firstHelix = tracks.begin();
   FATable<TRHelixPionFit>::const_iterator secondHelix = firstHelix;  ++secondHelix;

   FATable<McCannPionQuality> quality;
   extract(iFrame.record(Stream::kEvent), quality, "McCann");
   FATable<McCannPionQuality>::const_iterator firstQuality = quality.find(firstHelix->identifier());
   FATable<McCannPionQuality>::const_iterator secondQuality = quality.find(secondHelix->identifier());

   if (firstHelix->curvature() * secondHelix->curvature() > 0.) return true;

   if (!firstQuality->fit()  ||  firstQuality->fitAbort()) return true;
   if (!secondQuality->fit()  ||  secondQuality->fitAbort()) return true;

   if (firstHelix->momentum(magnetic_field->BField()).mag() < 0.8 * beam_energy->value()) return true;
   if (secondHelix->momentum(magnetic_field->BField()).mag() < 0.8 * beam_energy->value()) return true;

   if (fabs(firstHelix->d0()) > 0.01) return true;
   if (fabs(secondHelix->d0()) > 0.01) return true;

   if (fabs(firstHelix->z0()) > 0.10) return true;
   if (fabs(secondHelix->z0()) > 0.10) return true;

   if ((firstHelix->momentum(magnetic_field->BField()) + secondHelix->momentum(magnetic_field->BField()) +
	HepVector3D(0.013 * beam_energy->value()/5.29, 0, 0)).mag()
       > 0.548 * beam_energy->value()/5.29) return true;


   return false;
}
