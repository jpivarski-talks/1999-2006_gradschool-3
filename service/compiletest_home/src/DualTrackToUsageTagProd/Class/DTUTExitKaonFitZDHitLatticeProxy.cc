// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitKaonFitZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitKaonFitZDHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:57 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitKaonFitZDHitLatticeProxy.cc,v $
// Revision 1.1.1.1  2003/12/17 21:00:57  mccann
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

#include "DualTrackToUsageTagProd/DTUTExitKaonFitZDHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixExitKaonFit.h"
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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTExitKaonFitZDHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DTUTExitKaonFitZDHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:57 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTExitKaonFitZDHitLatticeProxy::DTUTExitKaonFitZDHitLatticeProxy()
   : m_ExitKaonFitZDHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTExitKaonFitZDHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTExitKaonFitZDHitLatticeProxy::DTUTExitKaonFitZDHitLatticeProxy( const DTUTExitKaonFitZDHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTExitKaonFitZDHitLatticeProxy::~DTUTExitKaonFitZDHitLatticeProxy()
{
   delete m_ExitKaonFitZDHitLattice ;
}

//
// assignment operators
//
// const DTUTExitKaonFitZDHitLatticeProxy& DTUTExitKaonFitZDHitLatticeProxy::operator=( const DTUTExitKaonFitZDHitLatticeProxy& iproxy )
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
DTUTExitKaonFitZDHitLatticeProxy::invalidateCache()
{
   delete m_ExitKaonFitZDHitLattice ;
   m_ExitKaonFitZDHitLattice = 0 ;
}

const DTUTExitKaonFitZDHitLatticeProxy::value_type*
DTUTExitKaonFitZDHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ExitKaonFitZDHitLattice == 0);
   auto_ptr<ExitKaonFitZDHitLattice> p_ExitKaonFitZDHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixExitKaonFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedZDHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_ExitKaonFitZDHitLattice.reset(new ExitKaonFitZDHitLattice(
				       tracks, hits, ExitKaonFitZDHitLattice::LNodeMulti));
      return m_ExitKaonFitZDHitLattice = p_ExitKaonFitZDHitLattice.release();
   }

   FAItem<DualTrackZDHitLattice> lattice;
   extract(iRecord, lattice);

   p_ExitKaonFitZDHitLattice.reset(new ExitKaonFitZDHitLattice(
				    tracks, hits, ExitKaonFitZDHitLattice::LNodeMulti));

   for (FATable<TRHelixExitKaonFit>::const_iterator track_iter = tracks.begin();
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

	 p_ExitKaonFitZDHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_ExitKaonFitZDHitLattice = p_ExitKaonFitZDHitLattice.release();
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
//DTUTExitKaonFitZDHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ExitKaonFitZDHitLattice >;

PROXY_BIND_METHOD( DTUTExitKaonFitZDHitLatticeProxy )
//-----------------------------------------------

