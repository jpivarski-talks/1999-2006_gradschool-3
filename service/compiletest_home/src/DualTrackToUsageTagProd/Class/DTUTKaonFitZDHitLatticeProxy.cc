// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTKaonFitZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTKaonFitZDHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $
//
// Revision history
//
// $Log: DTUTKaonFitZDHitLatticeProxy.cc,v $
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

#include "DualTrackToUsageTagProd/DTUTKaonFitZDHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixKaonFit.h"
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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTKaonFitZDHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DTUTKaonFitZDHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTKaonFitZDHitLatticeProxy::DTUTKaonFitZDHitLatticeProxy()
   : m_KaonFitZDHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTKaonFitZDHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTKaonFitZDHitLatticeProxy::DTUTKaonFitZDHitLatticeProxy( const DTUTKaonFitZDHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTKaonFitZDHitLatticeProxy::~DTUTKaonFitZDHitLatticeProxy()
{
   delete m_KaonFitZDHitLattice ;
}

//
// assignment operators
//
// const DTUTKaonFitZDHitLatticeProxy& DTUTKaonFitZDHitLatticeProxy::operator=( const DTUTKaonFitZDHitLatticeProxy& iproxy )
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
DTUTKaonFitZDHitLatticeProxy::invalidateCache()
{
   delete m_KaonFitZDHitLattice ;
   m_KaonFitZDHitLattice = 0 ;
}

const DTUTKaonFitZDHitLatticeProxy::value_type*
DTUTKaonFitZDHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_KaonFitZDHitLattice == 0);
   auto_ptr<KaonFitZDHitLattice> p_KaonFitZDHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixKaonFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedZDHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_KaonFitZDHitLattice.reset(new KaonFitZDHitLattice(
				       tracks, hits, KaonFitZDHitLattice::LNodeMulti));
      return m_KaonFitZDHitLattice = p_KaonFitZDHitLattice.release();
   }

   FAItem<DualTrackZDHitLattice> lattice;
   extract(iRecord, lattice);

   p_KaonFitZDHitLattice.reset(new KaonFitZDHitLattice(
				    tracks, hits, KaonFitZDHitLattice::LNodeMulti));

   for (FATable<TRHelixKaonFit>::const_iterator track_iter = tracks.begin();
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

	 p_KaonFitZDHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_KaonFitZDHitLattice = p_KaonFitZDHitLattice.release();
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
//DTUTKaonFitZDHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< KaonFitZDHitLattice >;

PROXY_BIND_METHOD( DTUTKaonFitZDHitLatticeProxy )
//-----------------------------------------------

