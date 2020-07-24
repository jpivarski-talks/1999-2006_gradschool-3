// -*- C++ -*-
//
// Package:     <FakeATrackProd>
// Module:      CalibratedZDHitProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jul 11 10:23:36 EDT 2003
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

// user include files
#include "Experiment/report.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"
#include "FrameAccess/FAAutoEraseAllPtr.h"

#include "FakeATrackProd/CalibratedZDHitProxy.h"
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
static const char* const kFacilityString = "FakeATrackProd.CalibratedZDHitProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: proxymany.cc,v 1.14 2000/12/04 19:11:16 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedZDHitProxy::CalibratedZDHitProxy()
   : m_ptrTable( new value_type )
{

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &CalibratedZDHitProxy::boundMethod, Stream::kBeginRun );
}

// CalibratedZDHitProxy::CalibratedZDHitProxy( const CalibratedZDHitProxy& iproxy )
// {
//   *this = iproxy;
// }

CalibratedZDHitProxy::~CalibratedZDHitProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const CalibratedZDHitProxy& CalibratedZDHitProxy::operator=( const CalibratedZDHitProxy& iproxy )
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
CalibratedZDHitProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

const CalibratedZDHitProxy::value_type*
CalibratedZDHitProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{

   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   //If an exception is thrown in this routine, m_ptrTable will be
   // automatically emptied (which is the proper behavior).
   // Use ptrTable just like you would use m_ptrTable
   FAAutoEraseAllPtr<CalibratedZDHit> ptrTable( m_ptrTable );





   FATable<TRSeedTrack> seedtrack;
   extract(iRecord, seedtrack, "fake-o");
   assert(seedtrack.size() == 1);
   FATable<TRSeedTrack>::const_iterator seed_iter(seedtrack.begin());
   KTHelix track(*(seed_iter));

//    track.setCurvature(0.10001);
//    track.setPhi0(3.15);
//    track.setD0(0.0015);
//    track.setCotTheta(0.5001);
//    track.setZ0(0.015);

   const Frame& theFrame = iRecord.frame();
   const Record& zdAlignmentRecord = theFrame.record(Stream::kZDAlignment);
   FAItem<AZDSenseWireStore> zdstore;
   extract(zdAlignmentRecord, zdstore);
   assert(zdstore.valid());
   int id(1);

   for (int layer = 1;  layer <= AZDSenseWireStore::kNumberOfSenseLayers;  layer++) {
      if (layer == 4) continue;

      double bestDistance(10000.);
      int bestWire(0), bestCell(0);
      DABoolean inZ(false);
      for (int wire = 0;  wire < zdstore->numberOfWiresLyr(layer);  wire++) {
	 double distance_moved(0.);

	 double rad(zdstore->radiusNominal(layer));
	 double step(0.01);

	 do {
	    track.moveToRadius(rad, distance_moved);
	    double this_dist2(((zdstore->wire(layer, wire).wirePoint(track.referencePoint().z())) - track.referencePoint()).mag2());
	    track.moveToRadius(rad - step, distance_moved);
	    double neg_dist2(((zdstore->wire(layer, wire).wirePoint(track.referencePoint().z())) - track.referencePoint()).mag2());
	    track.moveToRadius(rad + step, distance_moved);
	    double pos_dist2(((zdstore->wire(layer, wire).wirePoint(track.referencePoint().z())) - track.referencePoint()).mag2());

	    if ( fabs(neg_dist2 + pos_dist2 - 2. * this_dist2) < 1e-12 ) break;
	    step = ((neg_dist2 - pos_dist2) * step) / (2. * (neg_dist2 + pos_dist2 - 2. * this_dist2));
	    rad += step;
	 } while (fabs(step) > 1e-12);
	    
	 track.moveToRadius(rad, distance_moved);
	 double distance(((zdstore->wire(layer, wire).wirePoint(track.referencePoint().z())) - track.referencePoint()).mag());
	 double wire_z(zdstore->wire(layer, wire).wirePoint(track.referencePoint().z()).z());

	 if (distance < bestDistance) {
	    bestDistance = distance;
	    bestWire = wire;
	    bestCell = zdstore->cellID(layer, wire);
	    inZ = (zdstore->zMin(layer) < wire_z  &&  wire_z < zdstore->zMax(layer));
	 }
      } // end loop over wires
	 
      report(INFO, kFacilityString)
	 << "adding ZD hit " << layer << " track = " << track.referencePoint()
	 << ", wire = " << zdstore->wire(layer, bestWire).wirePoint(track.referencePoint().z()) << endl;

      if (inZ) {
	 double time(4e6/0.067*bestDistance - 1e8*bestDistance*bestDistance);
	 ptrTable->insert(new CalibratedZDHit(bestCell, layer, bestWire, -1., time,
					      Tseries(bestDistance,0,0), 300815., 136,
					      true, id++));
      } // end if inZ

      if (layer == 3) {
	 double bestDistance2(10000.);
	 int bestWire2(0), bestCell2(0);
	 for (int wire = 0;  wire < zdstore->numberOfWiresLyr(layer);  wire++) {
	    double distance_moved(0.);

	    double rad(zdstore->radiusNominal(layer));
	    double step(0.01);

	    do {
	       track.moveToRadius(rad, distance_moved);
	       double this_dist2(((zdstore->wire(layer, wire).wirePoint(track.referencePoint().z())) - track.referencePoint()).mag2());
	       track.moveToRadius(rad - step, distance_moved);
	       double neg_dist2(((zdstore->wire(layer, wire).wirePoint(track.referencePoint().z())) - track.referencePoint()).mag2());
	       track.moveToRadius(rad + step, distance_moved);
	       double pos_dist2(((zdstore->wire(layer, wire).wirePoint(track.referencePoint().z())) - track.referencePoint()).mag2());

	       if ( fabs(neg_dist2 + pos_dist2 - 2. * this_dist2) < 1e-12 ) break;
	       step = ((neg_dist2 - pos_dist2) * step) / (2. * (neg_dist2 + pos_dist2 - 2. * this_dist2));
	       rad += step;
	    } while (fabs(step) > 1e-12);
	    
	    track.moveToRadius(rad, distance_moved);
	    double distance(((zdstore->wire(layer, wire).wirePoint(track.referencePoint().z())) - track.referencePoint()).mag());
	    double wire_z(zdstore->wire(layer, wire).wirePoint(track.referencePoint().z()).z());

	    if (fabs(distance - bestDistance) > 1e-6  &&  distance < bestDistance2) {
	       bestDistance2 = distance;
	       bestWire2 = wire;
	       bestCell2 = zdstore->cellID(layer, wire);
	    }
	 } // end loop over wires

	 double time(4e6/0.067*bestDistance2 - 1e8*bestDistance2*bestDistance2);
	 ptrTable->insert(new CalibratedZDHit(bestCell2, layer, bestWire2, -1., time,
					      Tseries(bestDistance2,0,0), 300815., 136,
					      true, id++));
      }
   } // end loop over layers





   //The release returns m_ptrTable and stops ptrTable from erasing the 
   // contents of m_ptrTable.
   return ( ptrTable.release() ) ;
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
//CalibratedZDHitProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< CalibratedZDHit > >;

PROXY_BIND_METHOD( CalibratedZDHitProxy )
//-----------------------------------------------


