// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTMuonFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTMuonFitDRHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $
//
// Revision history
//
// $Log: DTUTMuonFitDRHitLatticeProxy.cc,v $
// Revision 1.1.1.1  2003/12/17 21:00:56  mccann
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

#include "DualTrackToUsageTagProd/DTUTMuonFitDRHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixMuonFit.h"
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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTMuonFitDRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DTUTMuonFitDRHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTMuonFitDRHitLatticeProxy::DTUTMuonFitDRHitLatticeProxy()
   : m_MuonFitDRHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTMuonFitDRHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTMuonFitDRHitLatticeProxy::DTUTMuonFitDRHitLatticeProxy( const DTUTMuonFitDRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTMuonFitDRHitLatticeProxy::~DTUTMuonFitDRHitLatticeProxy()
{
   delete m_MuonFitDRHitLattice ;
}

//
// assignment operators
//
// const DTUTMuonFitDRHitLatticeProxy& DTUTMuonFitDRHitLatticeProxy::operator=( const DTUTMuonFitDRHitLatticeProxy& iproxy )
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
DTUTMuonFitDRHitLatticeProxy::invalidateCache()
{
   delete m_MuonFitDRHitLattice ;
   m_MuonFitDRHitLattice = 0 ;
}

const DTUTMuonFitDRHitLatticeProxy::value_type*
DTUTMuonFitDRHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_MuonFitDRHitLattice == 0);
   auto_ptr<MuonFitDRHitLattice> p_MuonFitDRHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixMuonFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedDRHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_MuonFitDRHitLattice.reset(new MuonFitDRHitLattice(
				       tracks, hits, MuonFitDRHitLattice::LNodeMulti));
      return m_MuonFitDRHitLattice = p_MuonFitDRHitLattice.release();
   }

   FAItem<DualTrackDRHitLattice> lattice;
   extract(iRecord, lattice);

   p_MuonFitDRHitLattice.reset(new MuonFitDRHitLattice(
				    tracks, hits, MuonFitDRHitLattice::LNodeMulti));

   for (FATable<TRHelixMuonFit>::const_iterator track_iter = tracks.begin();
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

	 p_MuonFitDRHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_MuonFitDRHitLattice = p_MuonFitDRHitLattice.release();
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
//DTUTMuonFitDRHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< MuonFitDRHitLattice >;

PROXY_BIND_METHOD( DTUTMuonFitDRHitLatticeProxy )
//-----------------------------------------------

