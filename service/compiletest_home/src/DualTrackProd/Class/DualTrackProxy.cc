// -*- C++ -*-
//
// Package:     <DualTrackProd>
// Module:      DualTrackProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 09:29:10 EST 2003
// $Id: DualTrackProxy.cc,v 1.3 2003/12/16 22:16:22 mccann Exp $
//
// Revision history
//
// $Log: DualTrackProxy.cc,v $
// Revision 1.3  2003/12/16 22:16:22  mccann
// Turned on ZD lattices and fixed a Linux compilation bug.
//
// Revision 1.2  2003/12/15 17:53:11  mccann
// adding hit lattices for dual-constrained residuals
//
// Revision 1.1.1.1  2003/12/11 17:14:28  mccann
// imported DualTrackProd sources
//
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

#include "DualTrackProd/DualTrackProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "DualTrackProd/DualTrackProxy.h"
#include "Navigation/NavTrack.h"
#include "MagField/MagneticField.h"

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
static const char* const kFacilityString = "DualTrackProd.DualTrackProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackProxy.cc,v 1.3 2003/12/16 22:16:22 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackProxy::DualTrackProxy(DBCandidate::Hypo massHypothesis,
                               HepVector3D pVirtualPhoton,
                               DABoolean pointConstraint,
                               DABoolean ptConstraint,
                               DABoolean pzConstraint,
                               DABoolean printOutP)
   : m_massHypothesis(massHypothesis)
   , m_pVirtualPhoton(pVirtualPhoton)
   , m_pointConstraint(pointConstraint)
   , m_ptConstraint(ptConstraint)
   , m_pzConstraint(pzConstraint)
   , m_printOutP(printOutP)
   , m_DualTrack(NULL)
{
}

// DualTrackProxy::DualTrackProxy( const DualTrackProxy& iproxy )
// {
//   *this = iproxy;
// }

DualTrackProxy::~DualTrackProxy()
{
   delete m_DualTrack ;
}

//
// assignment operators
//
// const DualTrackProxy& DualTrackProxy::operator=( const DualTrackProxy& iproxy )
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
DualTrackProxy::invalidateCache()
{
   delete m_DualTrack;
   m_DualTrack = 0;
}

const DualTrackProxy::value_type*
DualTrackProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
   report(DEBUG,kFacilityString) << "entering DualTrack faultHandler" << endl;
   assert(m_DualTrack == NULL);

   auto_ptr<DualTrack> p_DualTrack;

   FATable<NavTrack> tracks;
   extract(iRecord, tracks);
   if (!tracks.valid()) {
      report(WARNING, kFacilityString)
         << "The FATable<NavTrack> is not valid.  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }
   if (tracks.size() != 2) {
      report(WARNING, kFacilityString)
         << "This event has " << tracks.size() << " track(s).  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }

   FATable<NavTrack>::const_iterator iter = tracks.begin();
   const KTHelix* one = &*(iter)->helixFit(m_massHypothesis);
   unsigned int oneid = iter->helixFit(m_massHypothesis)->identifier();
   const KTHelix* two = &*(++iter)->helixFit(m_massHypothesis);
   unsigned int twoid = iter->helixFit(m_massHypothesis)->identifier();

   if (one->curvature() * two->curvature() > 0.) {
      report(WARNING, kFacilityString)
         << "Both tracks in this event have the same charge.  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }
   if (one->curvature() == 0.  ||  two->curvature() == 0.) {
      report(WARNING, kFacilityString)
         << "A track in this event has zero curvature.  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }
   const KTHelix *positive, *negative;
   unsigned int posid, negid;
   if (one->curvature() > 0.)
   {
      positive = one;  negative = two;
      posid = oneid;   negid = twoid;
   }
   else {
      negative = one;  positive = two;
      negid = oneid;   posid = twoid;
   }

   if (positive->hasNullErrorMatrix()) {
      report(WARNING, kFacilityString)
	 << "Positive track has a null error matrix.  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }
   if (negative->hasNullErrorMatrix()) {
      report(WARNING, kFacilityString)
	 << "Negative track has a null error matrix.  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }
   if (tracks.find(posid)->quality(m_massHypothesis)->fitAbort()) {
      report(WARNING, kFacilityString)
	 << "Positive track fit failed.  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }
   if (tracks.find(negid)->quality(m_massHypothesis)->fitAbort()) {
      report(WARNING, kFacilityString)
	 << "Negative track fit failed.  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }

   FAItem<MagneticField> magneticField;
   extract((iRecord.frame()).record(Stream::kStartRun), magneticField);
   if (!magneticField.valid()) {
      report(WARNING, kFacilityString)
         << "The FAItem<MagneticField> is not valid.  No DualTrack constraint performed." << endl;
      p_DualTrack.reset(new DualTrack());
      return m_DualTrack = p_DualTrack.release();
   }

   if (m_printOutP) {
      HepVector3D total_momentum = positive->momentum(magneticField->BField()) +
	 negative->momentum(magneticField->BField());

      report(INFO, kFacilityString)
	 << "PxPyPz " << total_momentum.x() << " " << total_momentum.y() << " " << total_momentum.z() << endl;
   }

   p_DualTrack.reset(new DualTrack(posid,
				   positive,
				   negid,
				   negative,
				   magneticField->BField().z(),
				   m_pVirtualPhoton,
				   m_pointConstraint,
				   m_ptConstraint,
				   m_pzConstraint));
   report(DEBUG,kFacilityString) << "leaving DualTrack faultHandler" << endl;
   return m_DualTrack = p_DualTrack.release();
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
//DualTrackProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate<DualTrack>;

PROXY_BIND_METHOD( DualTrackProxy )
//-----------------------------------------------

