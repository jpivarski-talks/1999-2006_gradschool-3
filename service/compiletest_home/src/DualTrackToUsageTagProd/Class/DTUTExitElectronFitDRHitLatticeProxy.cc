// -*- C++ -*-
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DTUTExitElectronFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Dec 16 13:40:12 EST 2003
// $Id: DTUTExitElectronFitDRHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $
//
// Revision history
//
// $Log: DTUTExitElectronFitDRHitLatticeProxy.cc,v $
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

#include "DualTrackToUsageTagProd/DTUTExitElectronFitDRHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrack.h"
#include "TrackRoot/TRHelixExitElectronFit.h"
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
static const char* const kFacilityString = "DualTrackToUsageTagProd.DTUTExitElectronFitDRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DTUTExitElectronFitDRHitLatticeProxy.cc,v 1.1.1.1 2003/12/17 21:00:56 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DTUTExitElectronFitDRHitLatticeProxy::DTUTExitElectronFitDRHitLatticeProxy()
   : m_ExitElectronFitDRHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DTUTExitElectronFitDRHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// DTUTExitElectronFitDRHitLatticeProxy::DTUTExitElectronFitDRHitLatticeProxy( const DTUTExitElectronFitDRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

DTUTExitElectronFitDRHitLatticeProxy::~DTUTExitElectronFitDRHitLatticeProxy()
{
   delete m_ExitElectronFitDRHitLattice ;
}

//
// assignment operators
//
// const DTUTExitElectronFitDRHitLatticeProxy& DTUTExitElectronFitDRHitLatticeProxy::operator=( const DTUTExitElectronFitDRHitLatticeProxy& iproxy )
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
DTUTExitElectronFitDRHitLatticeProxy::invalidateCache()
{
   delete m_ExitElectronFitDRHitLattice ;
   m_ExitElectronFitDRHitLattice = 0 ;
}

const DTUTExitElectronFitDRHitLatticeProxy::value_type*
DTUTExitElectronFitDRHitLatticeProxy::faultHandler(const Record& iRecord, const DataKey& iKey)
{
   assert(m_ExitElectronFitDRHitLattice == 0);
   auto_ptr<ExitElectronFitDRHitLattice> p_ExitElectronFitDRHitLattice;

   FAItem<DualTrack> dual;
   extract(iRecord, dual);

   FATable<TRHelixExitElectronFit> tracks;
   extract(iRecord, tracks, "DualTrack");

   FATable<CalibratedDRHit> hits;
   extract(iRecord, hits);

   if (!dual->results().fitSuccessful()) {
      p_ExitElectronFitDRHitLattice.reset(new ExitElectronFitDRHitLattice(
				       tracks, hits, ExitElectronFitDRHitLattice::LNodeMulti));
      return m_ExitElectronFitDRHitLattice = p_ExitElectronFitDRHitLattice.release();
   }

   FAItem<DualTrackDRHitLattice> lattice;
   extract(iRecord, lattice);

   p_ExitElectronFitDRHitLattice.reset(new ExitElectronFitDRHitLattice(
				    tracks, hits, ExitElectronFitDRHitLattice::LNodeMulti));

   for (FATable<TRHelixExitElectronFit>::const_iterator track_iter = tracks.begin();
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

	 p_ExitElectronFitDRHitLattice->connect(track_iter, hit_iter, *new_linkData);
      } // end loop over old links
   } // end track loop

   return m_ExitElectronFitDRHitLattice = p_ExitElectronFitDRHitLattice.release();
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
//DTUTExitElectronFitDRHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< ExitElectronFitDRHitLattice >;

PROXY_BIND_METHOD( DTUTExitElectronFitDRHitLatticeProxy )
//-----------------------------------------------

