// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTProtonFitZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTProtonFitZDHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $
//
// Revision history
//
// $Log: DTUTProtonFitZDHitLatticeProxy.cc,v $
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

#include "DualTrackToUsageTagProd/DTUTProtonFitZDHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixProtonFit.h"
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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTProtonFitZDHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DTUTProtonFitZDHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTProtonFitZDHitLatticeProxy::DTUTProtonFitZDHitLatticeProxy()
   : m_ProtonFitZDHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTProtonFitZDHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTProtonFitZDHitLatticeProxy::DTUTProtonFitZDHitLatticeProxy( const DTUTProtonFitZDHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTProtonFitZDHitLatticeProxy::~DTUTProtonFitZDHitLatticeProxy()
{
   delete m_ProtonFitZDHitLattice ;
}

//
// assignment operators
//
// const DTUTProtonFitZDHitLatticeProxy& DTUTProtonFitZDHitLatticeProxy::operator=( const DTUTProtonFitZDHitLatticeProxy& iproxy )
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
DTUTProtonFitZDHitLatticeProxy::invalidateCache()
{
   delete m_ProtonFitZDHitLattice ;
   m_ProtonFitZDHitLattice = 0 ;
}

const DTUTProtonFitZDHitLatticeProxy::value_type*
DTUTProtonFitZDHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ProtonFitZDHitLattice == 0);
   auto_ptr<ProtonFitZDHitLattice> p_ProtonFitZDHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixProtonFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedZDHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_ProtonFitZDHitLattice.reset(new ProtonFitZDHitLattice(
				       tracks, hits, ProtonFitZDHitLattice::LNodeMulti));
      return m_ProtonFitZDHitLattice = p_ProtonFitZDHitLattice.release();
   }

   FAItem<DualTrackZDHitLattice> lattice;
   extract(iRecord, lattice);

   p_ProtonFitZDHitLattice.reset(new ProtonFitZDHitLattice(
				    tracks, hits, ProtonFitZDHitLattice::LNodeMulti));

   for (FATable<TRHelixProtonFit>::const_iterator track_iter = tracks.begin();
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

	 p_ProtonFitZDHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_ProtonFitZDHitLattice = p_ProtonFitZDHitLattice.release();
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
//DTUTProtonFitZDHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ProtonFitZDHitLattice >;

PROXY_BIND_METHOD( DTUTProtonFitZDHitLatticeProxy )
//-----------------------------------------------

