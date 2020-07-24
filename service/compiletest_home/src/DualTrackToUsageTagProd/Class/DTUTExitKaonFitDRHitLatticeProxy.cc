// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitKaonFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitKaonFitDRHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:57 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitKaonFitDRHitLatticeProxy.cc,v $
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

#include "DualTrackToUsageTagProd/DTUTExitKaonFitDRHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixExitKaonFit.h"
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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTExitKaonFitDRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DTUTExitKaonFitDRHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:57 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTExitKaonFitDRHitLatticeProxy::DTUTExitKaonFitDRHitLatticeProxy()
   : m_ExitKaonFitDRHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTExitKaonFitDRHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTExitKaonFitDRHitLatticeProxy::DTUTExitKaonFitDRHitLatticeProxy( const DTUTExitKaonFitDRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTExitKaonFitDRHitLatticeProxy::~DTUTExitKaonFitDRHitLatticeProxy()
{
   delete m_ExitKaonFitDRHitLattice ;
}

//
// assignment operators
//
// const DTUTExitKaonFitDRHitLatticeProxy& DTUTExitKaonFitDRHitLatticeProxy::operator=( const DTUTExitKaonFitDRHitLatticeProxy& iproxy )
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
DTUTExitKaonFitDRHitLatticeProxy::invalidateCache()
{
   delete m_ExitKaonFitDRHitLattice ;
   m_ExitKaonFitDRHitLattice = 0 ;
}

const DTUTExitKaonFitDRHitLatticeProxy::value_type*
DTUTExitKaonFitDRHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ExitKaonFitDRHitLattice == 0);
   auto_ptr<ExitKaonFitDRHitLattice> p_ExitKaonFitDRHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixExitKaonFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedDRHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_ExitKaonFitDRHitLattice.reset(new ExitKaonFitDRHitLattice(
				       tracks, hits, ExitKaonFitDRHitLattice::LNodeMulti));
      return m_ExitKaonFitDRHitLattice = p_ExitKaonFitDRHitLattice.release();
   }

   FAItem<DualTrackDRHitLattice> lattice;
   extract(iRecord, lattice);

   p_ExitKaonFitDRHitLattice.reset(new ExitKaonFitDRHitLattice(
				    tracks, hits, ExitKaonFitDRHitLattice::LNodeMulti));

   for (FATable<TRHelixExitKaonFit>::const_iterator track_iter = tracks.begin();
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

	 p_ExitKaonFitDRHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_ExitKaonFitDRHitLattice = p_ExitKaonFitDRHitLattice.release();
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
//DTUTExitKaonFitDRHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ExitKaonFitDRHitLattice >;

PROXY_BIND_METHOD( DTUTExitKaonFitDRHitLatticeProxy )
//-----------------------------------------------

