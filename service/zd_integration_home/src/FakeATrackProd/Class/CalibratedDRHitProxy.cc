// -*- C++ -*-
//
// Package:     <FakeATrackProd>
// Module:      CalibratedDRHitProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 14:46:10 EDT 2003
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

#include "FakeATrackProd/CalibratedDRHitProxy.h"
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
static const char* const kFacilityString = "FakeATrackProd.CalibratedDRHitProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: proxymany.cc,v 1.14 2000/12/04 19:11:16 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedDRHitProxy::CalibratedDRHitProxy()
   : m_ptrTable( new value_type )
{

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &CalibratedDRHitProxy::boundMethod, Stream::kBeginRun );
}

// CalibratedDRHitProxy::CalibratedDRHitProxy( const CalibratedDRHitProxy& iproxy )
// {
//   *this = iproxy;
// }

CalibratedDRHitProxy::~CalibratedDRHitProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const CalibratedDRHitProxy& CalibratedDRHitProxy::operator=( const CalibratedDRHitProxy& iproxy )
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
CalibratedDRHitProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

const CalibratedDRHitProxy::value_type*
CalibratedDRHitProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{

   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   //If an exception is thrown in this routine, m_ptrTable will be
   // automatically emptied (which is the proper behavior).
   // Use ptrTable just like you would use m_ptrTable
   FAAutoEraseAllPtr<CalibratedDRHit> ptrTable( m_ptrTable );

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
   const Record& drAlignmentRecord = theFrame.record(Stream::kDRAlignment);
   FAItem<ADRSenseWireStore> drstore;
   extract(drAlignmentRecord, drstore);
   assert(drstore.valid());
   int id(1);

   for (int layer = 1;  layer <= 5;  layer++) {
      if (layer == 3) continue;

      double distance_moved(0.);
      track.moveToRadius(drstore->radiusNominal(layer), distance_moved);
      
      double bestDistance(10000.);
      int bestWire(0), bestCell(0);
      DABoolean inZ(false);
      for (int wire = 0;  wire < drstore->numberOfWiresLyr(layer);  wire++) {
	 double distance(
	    (drstore->wire(layer, wire).wirePoint(track.referencePoint().z())
	     - track.referencePoint()).mag());
	 double wire_z(drstore->wire(layer, wire).wirePoint(track.referencePoint().z()).z());

	 if (distance < bestDistance) {
	    bestDistance = distance;
	    bestWire = wire;
	    bestCell = drstore->cellID(layer, wire);
	    inZ = (drstore->zMin(layer) < wire_z  &&  wire_z < drstore->zMax(layer));
	 }
      } // end loop over wires
	 
      report(INFO, kFacilityString)
	 << "adding DR hit " << layer << " track = " << track.referencePoint()
	 << ", wire = " << drstore->wire(layer, bestWire).wirePoint(track.referencePoint().z()) << endl;

      if (inZ) {
	 double time(4e6/0.067*bestDistance - 1e8*bestDistance*bestDistance);
	 ptrTable->insert(new CalibratedDRHit(bestCell, layer, bestWire, -1., time,
					      Tseries(bestDistance,0,0), 300815., 136,
					      true, id++));
      } // end if inZ
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
//CalibratedDRHitProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< CalibratedDRHit > >;

PROXY_BIND_METHOD( CalibratedDRHitProxy )
//-----------------------------------------------


