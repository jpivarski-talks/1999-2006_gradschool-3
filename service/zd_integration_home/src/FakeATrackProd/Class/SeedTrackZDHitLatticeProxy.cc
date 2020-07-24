// -*- C++ -*-
//
// Package:     <FakeATrackProd>
// Module:      SeedTrackZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jul 11 11:46:22 EDT 2003
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

#include "FakeATrackProd/SeedTrackZDHitLatticeProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "TrackRoot/TRSeedTrack.h"
#include "KinematicTrajectory/KTHelix.h"
#include "AZDGeom/AZDSenseWireStore.h"
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
static const char* const kFacilityString = "FakeATrackProd.SeedTrackZDHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: proxyone.cc,v 1.12 2000/12/04 19:11:16 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
SeedTrackZDHitLatticeProxy::SeedTrackZDHitLatticeProxy()
   : m_SeedTrackZDHitLattice( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &SeedTrackZDHitLatticeProxy::boundMethod, Stream::kBeginRun );
}

// SeedTrackZDHitLatticeProxy::SeedTrackZDHitLatticeProxy( const SeedTrackZDHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

SeedTrackZDHitLatticeProxy::~SeedTrackZDHitLatticeProxy()
{
   delete m_SeedTrackZDHitLattice ;
}

//
// assignment operators
//
// const SeedTrackZDHitLatticeProxy& SeedTrackZDHitLatticeProxy::operator=( const SeedTrackZDHitLatticeProxy& iproxy )
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
SeedTrackZDHitLatticeProxy::invalidateCache()
{
   delete m_SeedTrackZDHitLattice ;
   m_SeedTrackZDHitLattice = 0 ;
}

const SeedTrackZDHitLatticeProxy::value_type*
SeedTrackZDHitLatticeProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{

   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_SeedTrackZDHitLattice );

   //if an exception is thrown during this function, p_SeedTrackZDHitLattice will
   // automatically delete the object it is refering to
   auto_ptr<SeedTrackZDHitLattice> p_SeedTrackZDHitLattice;


   SeedTrackZDHitLattice* theLattice;
   p_SeedTrackZDHitLattice.reset(theLattice = new SeedTrackZDHitLattice(SeedTrackZDHitLattice::LNodeMulti));

   FATable<TRSeedTrack> seedtrack;
   extract(iRecord, seedtrack, "fake-o");
   assert(seedtrack.valid()  &&  seedtrack.size() == 1);
   FATable<TRSeedTrack>::const_iterator seed_iter(seedtrack.begin());
   KTHelix track(*(seed_iter));

   HepPoint3D center(-(track.d0() + 1./2./track.curvature()) * sin(track.phi0()),
		     (track.d0() + 1./2./track.curvature()) * cos(track.phi0()),
		     track.z0());

   FATable<CalibratedZDHit> zdhit;
   extract(iRecord, zdhit, "fake-o");
   assert(zdhit.valid()  &&  zdhit.size() > 0);

   const Frame& theFrame = iRecord.frame();
   const Record& zdAlignmentRecord = theFrame.record(Stream::kZDAlignment);
   FAItem<AZDSenseWireStore> zdstore;
   extract(zdAlignmentRecord, zdstore);
   assert(zdstore.valid());

   for (FATable<CalibratedZDHit>::const_iterator zdhit_iter = zdhit.begin();
	zdhit_iter != zdhit.end();
	++zdhit_iter) {
      double distance_moved(0.);
      track.moveToRadius(zdstore->radiusNominal(zdhit_iter->layer()), distance_moved);

      double dca(
	 (zdstore->wire(zdhit_iter->layer(), zdhit_iter->wireInLyr()).wirePoint(track.referencePoint().z())
	  - track.referencePoint()).mag());
      
      HepPoint3D wirepoint(zdstore->wire(zdhit_iter->layer(), zdhit_iter->wireInLyr()).wirePoint(track.referencePoint().z()));
      HepPoint3D pca(track.referencePoint());

      report(DEBUG, kFacilityString) << "ZOINK wirepoint " << wirepoint << " pca " << pca << endl;

      if ((wirepoint - center).mag() < (pca - center).mag()) {
	 dca *= -1.;
      } else {
	 dca *= 1.;
      }

      double residual(fabs(dca) - zdhit_iter->distance());
      double pull(residual / sqrt(zdhit_iter->weight()));

      theLattice->connect(seedtrack.begin(), zdhit_iter,
			  SeedZDHitLink(residual, pull, SeedZDHitLink::kFITTABLE));
   }



   
   //Since no exception happened, tell p_SeedTrackZDHitLattice to give its pointer to
   // m_SeedTrackZDHitLattice
   return ( m_SeedTrackZDHitLattice = p_SeedTrackZDHitLattice.release() ) ;
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
//SeedTrackZDHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< SeedTrackZDHitLattice >;

PROXY_BIND_METHOD( SeedTrackZDHitLatticeProxy )
//-----------------------------------------------

