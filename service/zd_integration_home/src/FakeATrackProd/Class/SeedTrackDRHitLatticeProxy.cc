// -*- C++ -*-
//
// Package:     <FakeATrackProd>
// Module:      SeedTrackDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Jul  1 16:39:52 EDT 2003
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

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "FakeATrackProd/SeedTrackDRHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "TrackRoot/TRSeedTrack.h"
#include "KinematicTrajectory/KTHelix.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "ActiveElements/AEWireInfo.h"

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
static const char* const kFacilityString = "FakeATrackProd.SeedTrackDRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: proxyone.cc,v 1.12 2000/12/04 19:11:16 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
SeedTrackDRHitLatticeProxy::SeedTrackDRHitLatticeProxy()
   : m_SeedTrackDRHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &SeedTrackDRHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// SeedTrackDRHitLatticeProxy::SeedTrackDRHitLatticeProxy( const SeedTrackDRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

SeedTrackDRHitLatticeProxy::~SeedTrackDRHitLatticeProxy()
{
   delete m_SeedTrackDRHitLattice ;
}

//
// assignment operators
//
// const SeedTrackDRHitLatticeProxy& SeedTrackDRHitLatticeProxy::operator=( const SeedTrackDRHitLatticeProxy& iproxy )
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
SeedTrackDRHitLatticeProxy::invalidateCache()
{
   delete m_SeedTrackDRHitLattice ;
   m_SeedTrackDRHitLattice = 0 ;
}

const SeedTrackDRHitLatticeProxy::value_type*
SeedTrackDRHitLatticeProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{

   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_SeedTrackDRHitLattice );

   //if an exception is thrown during this function, p_SeedTrackDRHitLattice will
   // automatically delete the object it is refering to
   auto_ptr<SeedTrackDRHitLattice> p_SeedTrackDRHitLattice;

   SeedTrackDRHitLattice* theLattice;
   p_SeedTrackDRHitLattice.reset(theLattice = new SeedTrackDRHitLattice(SeedTrackDRHitLattice::LNodeMulti));

   FATable<TRSeedTrack> seedtrack;
   extract(iRecord, seedtrack, "fake-o");
   assert(seedtrack.valid()  &&  seedtrack.size() == 1);
   FATable<TRSeedTrack>::const_iterator seed_iter(seedtrack.begin());
   KTHelix track(*(seed_iter));

   HepPoint3D center(-(track.d0() + 1./2./track.curvature()) * sin(track.phi0()),
		     (track.d0() + 1./2./track.curvature()) * cos(track.phi0()),
		     track.z0());

   FATable<CalibratedDRHit> drhit;
   extract(iRecord, drhit, "fake-o");
   assert(drhit.valid()  &&  drhit.size() > 0);

   const Frame& theFrame = iRecord.frame();
   const Record& drAlignmentRecord = theFrame.record(Stream::kDRAlignment);
   FAItem<ADRSenseWireStore> drstore;
   extract(drAlignmentRecord, drstore);
   assert(drstore.valid());

   for (FATable<CalibratedDRHit>::const_iterator drhit_iter = drhit.begin();
	drhit_iter != drhit.end();
	++drhit_iter) {
      double distance_moved(0.);
      track.moveToRadius(drstore->radiusNominal(drhit_iter->layer()), distance_moved);

      double dca(
	 (drstore->wire(drhit_iter->layer(), drhit_iter->wireInLyr()).wirePoint(track.referencePoint().z())
	  - track.referencePoint()).mag());
      
      HepPoint3D wirepoint(drstore->wire(drhit_iter->layer(), drhit_iter->wireInLyr()).wirePoint(track.referencePoint().z()));
      HepPoint3D pca(track.referencePoint());

      report(DEBUG, kFacilityString) << "ZOINK wirepoint " << wirepoint << " pca " << pca << endl;

      if ((wirepoint - center).mag() < (pca - center).mag()) {
	 dca *= -1.;
      } else {
	 dca *= 1.;
      }

      double residual(fabs(dca) - drhit_iter->distance());
      double pull(residual / sqrt(drhit_iter->weight()));

      theLattice->connect(seedtrack.begin(), drhit_iter,
			  SeedDRHitLink(residual, pull, SeedDRHitLink::kFITTABLE));
   }
   //Since no exception happened, tell p_SeedTrackDRHitLattice to give its pointer to
   // m_SeedTrackDRHitLattice
   return ( m_SeedTrackDRHitLattice = p_SeedTrackDRHitLattice.release() ) ;
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
//SeedTrackDRHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< SeedTrackDRHitLattice >;

PROXY_BIND_METHOD( SeedTrackDRHitLatticeProxy )
//-----------------------------------------------

