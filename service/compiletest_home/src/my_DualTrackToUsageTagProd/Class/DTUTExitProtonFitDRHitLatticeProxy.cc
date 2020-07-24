// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitProtonFitDRHitLatticeProxy
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

#include "DualTrackToUsageTagProd/DTUTExitProtonFitDRHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixExitProtonFit.h"
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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTExitProtonFitDRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: proxyone.cc,v 1.12 2000/12/04 19:11:16 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTExitProtonFitDRHitLatticeProxy::DTUTExitProtonFitDRHitLatticeProxy()
   : m_ExitProtonFitDRHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTExitProtonFitDRHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTExitProtonFitDRHitLatticeProxy::DTUTExitProtonFitDRHitLatticeProxy( const DTUTExitProtonFitDRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTExitProtonFitDRHitLatticeProxy::~DTUTExitProtonFitDRHitLatticeProxy()
{
   delete m_ExitProtonFitDRHitLattice ;
}

//
// assignment operators
//
// const DTUTExitProtonFitDRHitLatticeProxy& DTUTExitProtonFitDRHitLatticeProxy::operator=( const DTUTExitProtonFitDRHitLatticeProxy& iproxy )
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
DTUTExitProtonFitDRHitLatticeProxy::invalidateCache()
{
   delete m_ExitProtonFitDRHitLattice ;
   m_ExitProtonFitDRHitLattice = 0 ;
}

const DTUTExitProtonFitDRHitLatticeProxy::value_type*
DTUTExitProtonFitDRHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ExitProtonFitDRHitLattice == 0);
   auto_ptr<ExitProtonFitDRHitLattice> p_ExitProtonFitDRHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixExitProtonFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedDRHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_ExitProtonFitDRHitLattice.reset(new ExitProtonFitDRHitLattice(
				       tracks, hits, ExitProtonFitDRHitLattice::LNodeMulti));
      return m_ExitProtonFitDRHitLattice = p_ExitProtonFitDRHitLattice.release();
   }

   FAItem<DualTrackDRHitLattice> lattice;
   extract(iRecord, lattice);

   p_ExitProtonFitDRHitLattice.reset(new ExitProtonFitDRHitLattice(
				    tracks, hits, ExitProtonFitDRHitLattice::LNodeMulti));

   for (FATable<TRHelixExitProtonFit>::const_iterator track_iter = tracks.begin();
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

	 p_ExitProtonFitDRHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_ExitProtonFitDRHitLattice = p_ExitProtonFitDRHitLattice.release();
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
//DTUTExitProtonFitDRHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ExitProtonFitDRHitLattice >;

PROXY_BIND_METHOD( DTUTExitProtonFitDRHitLatticeProxy )
//-----------------------------------------------

