// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:41:33 EST 2003
// $Id: DualTrackZDHitLatticeProxy.cc,v 1.1 2003/12/15 17:53:12 mccann Exp $
//
// Revision history
//
// $Log: DualTrackZDHitLatticeProxy.cc,v $
// Revision 1.1  2003/12/15 17:53:12  mccann
// adding hit lattices for dual-constrained residuals
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#include <memory>  //for auto_ptr

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "DualTrackProd/DualTrackZDHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

#include "DualTrackProd/DualTrack.h"
#include "Navigation/NavTrack.h"
#include "MagField/MagneticField.h"
#include "ZDHitCorrectorProd/ZDHitCorrector.h"
#include "AZDGeom/AZDSenseWireStore.h"
#include "DetectorGeometry/DGMaterialStore.h"
#include "MaterialDelivery/material_identifiers.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HISingleWireCylinder.h"

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DualTrackProd.DualTrackZDHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackZDHitLatticeProxy.cc,v 1.1 2003/12/15 17:53:12 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackZDHitLatticeProxy::DualTrackZDHitLatticeProxy(DBCandidate::Hypo hypo)
   : m_DualTrackZDHitLattice( 0 )
   , m_hypo(hypo)
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DualTrackZDHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DualTrackZDHitLatticeProxy::DualTrackZDHitLatticeProxy( const DualTrackZDHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DualTrackZDHitLatticeProxy::~DualTrackZDHitLatticeProxy()
{
   delete m_DualTrackZDHitLattice ;
}

//
// assignment operators
//
// const DualTrackZDHitLatticeProxy& DualTrackZDHitLatticeProxy::operator=( const DualTrackZDHitLatticeProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//
void
DualTrackZDHitLatticeProxy::invalidateCache()
{
   delete m_DualTrackZDHitLattice ;
   m_DualTrackZDHitLattice = 0 ;
}

const DualTrackZDHitLatticeProxy::value_type*
DualTrackZDHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_DualTrackZDHitLattice == 0);
   auto_ptr<DualTrackZDHitLattice> p_DualTrackZDHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<DualTrackFitHelix> tracks;
   extract(iRecord, tracks);

   FATable<CalibratedZDHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_DualTrackZDHitLattice.reset(new DualTrackZDHitLattice(
				       tracks, hits, DualTrackZDHitLattice::LNodeMulti));
      return m_DualTrackZDHitLattice = p_DualTrackZDHitLattice.release();
   }

   FATable<NavTrack> navtracks;
   extract(iRecord, navtracks);

   FAItem<MagneticField> magneticField;
   extract((iRecord.frame()).record(Stream::kStartRun), magneticField);

   FAItem<ZDHitCorrector> corrector;
   extract((iRecord.frame()).record(Stream::kStartRun), corrector);

   FAItem<AZDSenseWireStore> store;
   extract((iRecord.frame()).record(Stream::kZDAlignment), store);

   DGMaterialStore materials;
   extract((iRecord.frame()).record(Stream::kMaterial), materials);
   const DGConstMaterialPtr& zdGas = materials[kZDGasWireVaporMatID];

   p_DualTrackZDHitLattice.reset(new DualTrackZDHitLattice(
				    tracks, hits, DualTrackZDHitLattice::LNodeMulti));

   for (FATable<DualTrackFitHelix>::const_iterator track_iter = tracks.begin();
	track_iter != tracks.end();
	++track_iter) {
      const NavTrack::ZDHitLinkTable* old_links = navtracks.find(track_iter->identifier())->zdHitLinks(m_hypo);
      NavTrack::ZDHitLinkTable::const_iterator old_link_iter = old_links->begin();
      NavTrack::ZDHitLinkTable::const_iterator old_link_end = old_links->end();

      double total_charge = 0.;
      int charge_hits = 0;
      for (;  old_link_iter != old_link_end;  ++old_link_iter) {
	 const TrackFitZDHitLink& old_linkData = (**old_link_iter).linkData();
	 assert((**old_link_iter).rightID() != NULL);
	 FATable<CalibratedZDHit>::const_iterator hit_iter = hits.find(*(**old_link_iter).rightID());

	 if (old_linkData.used()) { total_charge += hit_iter->charge();  charge_hits++; }
      }
      double average_charge = 0.;
      if (charge_hits > 0) average_charge = total_charge / double(charge_hits);

      for (old_link_iter = old_links->begin();  old_link_iter != old_link_end;  ++old_link_iter) {
	 const TrackFitZDHitLink& old_linkData = (**old_link_iter).linkData();
	 FATable<CalibratedZDHit>::const_iterator hit_iter = hits.find(*(**old_link_iter).rightID());

	 HIHelix helix(*track_iter);

	 // HIHelixIntersector doesn't have much overhead: go ahead
	 // and make a new one for each hit
	 vector<HIIntersectionSurface*> surface;
	 surface.push_back(new HISingleWireCylinder(zdGas, zdGas,
						    store->layer(hit_iter->layer()),
						    hit_iter->layer(),
						    DGTransform::Identity,
						    NULL, NULL,
						    &*hit_iter,
						    old_linkData.signedDriftDistance(),
						    &*corrector,
						    false,
						    average_charge));
	 helix.moveToRadius(store->layer(hit_iter->layer()).radius());
	 HIHelixIntersector intersector(surface, HIHelixIntersector::kOrderAsGiven, &helix);
	 intersector.swimToCurrentSurface(KTMoveControl::kDirectionEither);
	 intersector.currentSurface()->hiCalibratedHit()->applyTrackCorrections(helix);

	 HepPoint3D referencePoint = helix.position();
	 double dca = intersector.currentSurface()->dca(helix);
	 HepVector derivs(5, 0);
	 intersector.currentSurface()->derivatives(helix, derivs);
	 double dcaError2 = helix.errorMatrix().similarity(derivs);
	 double residual = old_linkData.signedDriftDistance() - dca;
	 double residualError2 = 1./old_linkData.fittingWeight(); // + dcaError2;   default chi2fitter doesn't include this
	 HepVector3D momentum = helix.momentum(magneticField->BField());
	 double sinAlpha = sin(momentum.phi() - referencePoint.phi());

	 DualTrackZDHitLink* new_linkData = new DualTrackZDHitLink(old_linkData.used(),
								   residual,
								   sqrt(residualError2),
								   momentum.mag(),
								   referencePoint,
								   dca,
								   sqrt(dcaError2),
								   sinAlpha,
								   old_linkData.driftTime(),
								   old_linkData.fittingWeight(),
								   old_linkData.signedDriftDistance(),

								   old_linkData.residual(),
								   old_linkData.residualError(),
								   old_linkData.momentum(),
								   old_linkData.trackRefPt(),
								   old_linkData.signedDcaToWire(),
								   old_linkData.signedDcaError(),
								   old_linkData.sinTrackToRadial());
	 p_DualTrackZDHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_DualTrackZDHitLattice = p_DualTrackZDHitLattice.release();
}

//
// const member functions
//

//
// static member functions
//

//
// 'bound' functions
//

//void
//DualTrackZDHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< DualTrackZDHitLattice >;

PROXY_BIND_METHOD( DualTrackZDHitLatticeProxy )
//-----------------------------------------------

