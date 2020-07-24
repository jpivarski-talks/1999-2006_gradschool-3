// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTElectronFitZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTElectronFitZDHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $
//
// Revision history
//
// $Log: DTUTElectronFitZDHitLatticeProxy.cc,v $
// Revision 1.1.1.1  2003/12/17 21:00:55  mccann
// imported DualTrackToUsageTagProd sources
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
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "DualTrackToUsageTagProd/DTUTElectronFitZDHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixElectronFit.h"
#include "DualTrackProd/DualTrackZDHitLink.h"

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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTElectronFitZDHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DTUTElectronFitZDHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:55 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTElectronFitZDHitLatticeProxy::DTUTElectronFitZDHitLatticeProxy()
   : m_ElectronFitZDHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTElectronFitZDHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTElectronFitZDHitLatticeProxy::DTUTElectronFitZDHitLatticeProxy( const DTUTElectronFitZDHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTElectronFitZDHitLatticeProxy::~DTUTElectronFitZDHitLatticeProxy()
{
   delete m_ElectronFitZDHitLattice ;
}

//
// assignment operators
//
// const DTUTElectronFitZDHitLatticeProxy& DTUTElectronFitZDHitLatticeProxy::operator=( const DTUTElectronFitZDHitLatticeProxy& iproxy )
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
DTUTElectronFitZDHitLatticeProxy::invalidateCache()
{
   delete m_ElectronFitZDHitLattice ;
   m_ElectronFitZDHitLattice = 0 ;
}

const DTUTElectronFitZDHitLatticeProxy::value_type*
DTUTElectronFitZDHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ElectronFitZDHitLattice == 0);
   auto_ptr<ElectronFitZDHitLattice> p_ElectronFitZDHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixElectronFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedZDHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_ElectronFitZDHitLattice.reset(new ElectronFitZDHitLattice(
				       tracks, hits, ElectronFitZDHitLattice::LNodeMulti));
      return m_ElectronFitZDHitLattice = p_ElectronFitZDHitLattice.release();
   }

   FAItem<DualTrackZDHitLattice> lattice;
   extract(iRecord, lattice);

   p_ElectronFitZDHitLattice.reset(new ElectronFitZDHitLattice(
				    tracks, hits, ElectronFitZDHitLattice::LNodeMulti));

   for (FATable<TRHelixElectronFit>::const_iterator track_iter = tracks.begin();
	track_iter != tracks.end();
	++track_iter) {
      const DualTrackZDHitLattice::Links& links = lattice->linksGivenLeft(track_iter->identifier());
      DualTrackZDHitLattice::Links::const_iterator iter = links.begin();
      DualTrackZDHitLattice::Links::const_iterator end = links.end();

      for (; iter != end;  ++iter) {
	 assert((**iter).rightID() != NULL);
	 FATable<CalibratedZDHit>::const_iterator hit_iter = hits.find(*(**iter).rightID());
	 const DualTrackZDHitLink& linkData = (**iter).linkData();

	 TrackFitZDHitLink* new_linkData = new TrackFitZDHitLink(linkData.used(),
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

	 p_ElectronFitZDHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_ElectronFitZDHitLattice = p_ElectronFitZDHitLattice.release();
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
//DTUTElectronFitZDHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ElectronFitZDHitLattice >;

PROXY_BIND_METHOD( DTUTElectronFitZDHitLatticeProxy )
//-----------------------------------------------

