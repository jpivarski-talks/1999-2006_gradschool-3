// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTProtonFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTProtonFitDRHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $
//
// Revision history
//
// $Log: DTUTProtonFitDRHitLatticeProxy.cc,v $
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

#include "DualTrackToUsageTagProd/DTUTProtonFitDRHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixProtonFit.h"
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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTProtonFitDRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DTUTProtonFitDRHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTProtonFitDRHitLatticeProxy::DTUTProtonFitDRHitLatticeProxy()
   : m_ProtonFitDRHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTProtonFitDRHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTProtonFitDRHitLatticeProxy::DTUTProtonFitDRHitLatticeProxy( const DTUTProtonFitDRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTProtonFitDRHitLatticeProxy::~DTUTProtonFitDRHitLatticeProxy()
{
   delete m_ProtonFitDRHitLattice ;
}

//
// assignment operators
//
// const DTUTProtonFitDRHitLatticeProxy& DTUTProtonFitDRHitLatticeProxy::operator=( const DTUTProtonFitDRHitLatticeProxy& iproxy )
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
DTUTProtonFitDRHitLatticeProxy::invalidateCache()
{
   delete m_ProtonFitDRHitLattice ;
   m_ProtonFitDRHitLattice = 0 ;
}

const DTUTProtonFitDRHitLatticeProxy::value_type*
DTUTProtonFitDRHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ProtonFitDRHitLattice == 0);
   auto_ptr<ProtonFitDRHitLattice> p_ProtonFitDRHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixProtonFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedDRHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_ProtonFitDRHitLattice.reset(new ProtonFitDRHitLattice(
				       tracks, hits, ProtonFitDRHitLattice::LNodeMulti));
      return m_ProtonFitDRHitLattice = p_ProtonFitDRHitLattice.release();
   }

   FAItem<DualTrackDRHitLattice> lattice;
   extract(iRecord, lattice);

   p_ProtonFitDRHitLattice.reset(new ProtonFitDRHitLattice(
				    tracks, hits, ProtonFitDRHitLattice::LNodeMulti));

   for (FATable<TRHelixProtonFit>::const_iterator track_iter = tracks.begin();
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

	 p_ProtonFitDRHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_ProtonFitDRHitLattice = p_ProtonFitDRHitLattice.release();
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
//DTUTProtonFitDRHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ProtonFitDRHitLattice >;

PROXY_BIND_METHOD( DTUTProtonFitDRHitLatticeProxy )
//-----------------------------------------------

