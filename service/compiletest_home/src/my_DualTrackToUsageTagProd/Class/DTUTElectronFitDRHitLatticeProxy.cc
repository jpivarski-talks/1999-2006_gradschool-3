// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTElectronFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#include <memory>  //for auto_ptr

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "DualTrackToUsageTagProd/DTUTElectronFitDRHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixElectronFit.h"
#include "DualTrackProd/DualTrackDRHitLink.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTElectronFitDRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: proxyone.cc,v 1.12 2000/12/04 19:11:16 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTElectronFitDRHitLatticeProxy::DTUTElectronFitDRHitLatticeProxy()
   : m_ElectronFitDRHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTElectronFitDRHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTElectronFitDRHitLatticeProxy::DTUTElectronFitDRHitLatticeProxy( const DTUTElectronFitDRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTElectronFitDRHitLatticeProxy::~DTUTElectronFitDRHitLatticeProxy()
{
   delete m_ElectronFitDRHitLattice ;
}

//
// assignment operators
//
// const DTUTElectronFitDRHitLatticeProxy& DTUTElectronFitDRHitLatticeProxy::operator=( const DTUTElectronFitDRHitLatticeProxy& iproxy )
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
DTUTElectronFitDRHitLatticeProxy::invalidateCache()
{
   delete m_ElectronFitDRHitLattice ;
   m_ElectronFitDRHitLattice = 0 ;
}

const DTUTElectronFitDRHitLatticeProxy::value_type*
DTUTElectronFitDRHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ElectronFitDRHitLattice == 0);
   auto_ptr<ElectronFitDRHitLattice> p_ElectronFitDRHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixElectronFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedDRHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_ElectronFitDRHitLattice.reset(new ElectronFitDRHitLattice(
				       tracks, hits, ElectronFitDRHitLattice::LNodeMulti));
      return m_ElectronFitDRHitLattice = p_ElectronFitDRHitLattice.release();
   }

   FAItem<DualTrackDRHitLattice> lattice;
   extract(iRecord, lattice);

   p_ElectronFitDRHitLattice.reset(new ElectronFitDRHitLattice(
				    tracks, hits, ElectronFitDRHitLattice::LNodeMulti));

   for (FATable<TRHelixElectronFit>::const_iterator track_iter = tracks.begin();
	track_iter != tracks.end();
	++track_iter) {
      const DualTrackDRHitLattice::Links& links = lattice->linksGivenLeft(track_iter->identifier());
      DualTrackDRHitLattice::Links::const_iterator iter = links.begin();
      DualTrackDRHitLattice::Links::const_iterator end = links.end();

      for (; iter != end;  ++iter) {
	 assert((**iter).rightID() != NULL);
	 FATable<CalibratedDRHit>::const_iterator hit_iter = hits.find(*(**iter).rightID());
	 const DualTrackDRHitLink& linkData = (**iter).linkData();

	 TrackFitDRHitLink* new_linkData = new TrackFitDRHitLink(linkData.used(),
								 linkData.residual(),
								 linkData.residualError(),
								 linkData.momentum(),
								 linkData.trackRefPt(),
								 linkData.signedDcaToWire(),
								 linkData.signedDcaError(),
								 linkData.sinTrackToRadial(),
								 linkData.driftTime(),
								 linkData.fittingWeight(),
								 linkData.signedDriftDistance());

	 p_ElectronFitDRHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_ElectronFitDRHitLattice = p_ElectronFitDRHitLattice.release();
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
//DTUTElectronFitDRHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ElectronFitDRHitLattice >;

PROXY_BIND_METHOD( DTUTElectronFitDRHitLatticeProxy )
//-----------------------------------------------

