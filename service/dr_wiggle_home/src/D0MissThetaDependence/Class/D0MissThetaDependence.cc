// -*- C++ -*-
//
// Package:     D0MissThetaDependence
// Module:      D0MissThetaDependence
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Mar 21 15:52:01 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "D0MissThetaDependence/D0MissThetaDependence.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
#include "TrackRoot/TRHelixFit.h"
#include "TrackRoot/TRTrackFitQuality.h"
#include "TrackRoot/TRSeedTrackQuality.h"
#include "TrackDelivery/TDKinematicFit.h"
#include "TrackRoot/TRHelixPionFit.h"

#include "MagField/MagneticField.h"
#include "BeamEnergy/BeamEnergy.h"
#include "BeamSpot/BeamSpot.h"

//RICH example 
//Dedx example
//Event Shape example


// STL classes
// You may have to uncomment some of these or other stl headers 
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Processor.D0MissThetaDependence" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_01 $";

//
// static data member definitions
//



//
// constructors and destructor
//
D0MissThetaDependence::D0MissThetaDependence( void )               // anal1
   : Processor( "D0MissThetaDependence" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &D0MissThetaDependence::event,    Stream::kEvent );
   //bind( &D0MissThetaDependence::beginRun, Stream::kBeginRun );
   //bind( &D0MissThetaDependence::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

D0MissThetaDependence::~D0MissThetaDependence()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
D0MissThetaDependence::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
D0MissThetaDependence::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
D0MissThetaDependence::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here
   const char* label[kNumEntries] = {
      "plpperp", "plpz", "pld0", "plphi0", "plcotTh", "plz0",
      "mipperp", "mipz", "mid0", "miphi0", "micotTh", "miz0",
      "px", "py", "pperp", "pz", "d0", "phi0", "cotTh", "z0" };
   m_ntuple = iHistoManager.ntuple(
      1, "vars", kNumEntries, 262144, label);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
D0MissThetaDependence::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<MagneticField> cleo3BField;
   extract(iFrame.record(Stream::kStartRun), cleo3BField);

   FAItem<BeamEnergy> beamEnergy;
   extract(iFrame.record(Stream::kStartRun), beamEnergy);

   FAItem<BeamSpot> beamSpot;
   extract(iFrame.record(Stream::kBeginRun), beamSpot);

   FATable<TRHelixPionFit> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);

   if ( tracks.size() != 2 ) return ActionBase::kFailed;
   FATable<TRHelixPionFit>::const_iterator one = tracks.begin();
   FATable<TRHelixPionFit>::const_iterator two = one;  ++two;
   if ( one->curvature() * two->curvature() > 0. ) return ActionBase::kFailed;

   FATable<TRHelixPionFit>::const_iterator plus = (one->curvature() > 0. ? one : two);
   FATable<TRHelixPionFit>::const_iterator minus = (one->curvature() > 0. ? two : one);

   Hep3Vector plusMomentum(plus->momentum(cleo3BField->BField(plus->position())));
   Hep3Vector minusMomentum(minus->momentum(cleo3BField->BField(minus->position())));

   KTHelix plusHelix(*plus);
   KTHelix minusHelix(*minus);
   double distanceMoved;
   if ( plusHelix.moveToReferencePoint(beamSpot->center(), distanceMoved)
	!= KTMoveControl::kMoveOK ) {
      report(ERROR, kFacilityString) << "Move failure (plus)!!!" << endl;
      return ActionBase::kFailed;
   }
   if ( minusHelix.moveToReferencePoint(beamSpot->center(), distanceMoved)
	!= KTMoveControl::kMoveOK ) {
      report(ERROR, kFacilityString) << "Move failure (minus)!!!" << endl;
      return ActionBase::kFailed;
   }
	 
   float v[kNumEntries];  for ( int i = 0;  i < kNumEntries;  i++ ) v[i] = -1000.;

   v[kPlusPPerp] = plusMomentum.perp();
   v[kPlusPZ] = plusMomentum.z();
   v[kPlusD0] = plusHelix.d0();
   v[kPlusPhi0] = plusHelix.phi0();
   v[kPlusCotTheta] = plusHelix.cotTheta();
   v[kPlusZ0] = plusHelix.z0();

   v[kMinusPPerp] = minusMomentum.perp();
   v[kMinusPZ] = minusMomentum.z();
   v[kMinusD0] = minusHelix.d0();
   v[kMinusPhi0] = minusHelix.phi0();
   v[kMinusCotTheta] = minusHelix.cotTheta();
   v[kMinusZ0] = minusHelix.z0();
   
   Hep3Vector eventMomentum(plusMomentum + minusMomentum + 
			    Hep3Vector(0.026*beamEnergy->value()/5.3, 0., 0.));
   v[kPX] = eventMomentum.x();
   v[kPY] = eventMomentum.y();
   v[kPPerp] = eventMomentum.perp();
   v[kPZ] = eventMomentum.z();
   v[kD0] = (plusHelix.d0() + minusHelix.d0());
   v[kPhi0] = sin(plusHelix.phi0() - minusHelix.phi0() -
		  0.026/5.3 * sqrt(1 + sqr(plusHelix.cotTheta())) * sin(plusHelix.phi0()) - M_PI);
   v[kCotTheta] = (plusHelix.cotTheta() + minusHelix.cotTheta());
   v[kZ0] = (plusHelix.z0() - minusHelix.z0());

   m_ntuple->fill(v);

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
D0MissThetaDependence::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
D0MissThetaDependence::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

//
// const member functions
//

//
// static member functions
//
