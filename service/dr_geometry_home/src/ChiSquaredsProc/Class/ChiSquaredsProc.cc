// -*- C++ -*-
//
// Package:     ChiSquaredsProc
// Module:      ChiSquaredsProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon Oct 20 10:40:38 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "ChiSquaredsProc/ChiSquaredsProc.h"
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

#include "CleoDB/DBEventHeader.h"

// STL classes
// You may have to uncomment some of these or other stl headers 
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

extern "C" float prob_(const float&, const int&);

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Processor.ChiSquaredsProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
ChiSquaredsProc::ChiSquaredsProc( void )               // anal1
   : Processor( "ChiSquaredsProc" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &ChiSquaredsProc::event,    Stream::kEvent );
   //bind( &ChiSquaredsProc::beginRun, Stream::kBeginRun );
   //bind( &ChiSquaredsProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

ChiSquaredsProc::~ChiSquaredsProc()                    // anal5
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
ChiSquaredsProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
ChiSquaredsProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
ChiSquaredsProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here
   m_chi2pos = iHistoManager.histogram(2001, "[v]^2! of positive tracks", 420, -10., 200.);
   m_chi2neg = iHistoManager.histogram(3001, "[v]^2! of negative tracks", 420, -10., 200.);
   m_chi2pos_vcotTh = iHistoManager.profile(2002, "[v]^2!/dof of positive tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kErrorOnMean);
   m_chi2neg_vcotTh = iHistoManager.profile(3002, "[v]^2!/dof of negative tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kErrorOnMean);
   m_chi2pos_vcotTh_spread = iHistoManager.profile(2003, "[v]^2!/dof of positive tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kSpread);
   m_chi2neg_vcotTh_spread = iHistoManager.profile(3003, "[v]^2!/dof of negative tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kSpread);
   m_probpos = iHistoManager.histogram(2004, "[v]^2! probability for positive tracks", 100, 0., 1.);
   m_probneg = iHistoManager.histogram(3004, "[v]^2! probability for negative tracks", 100, 0., 1.);

   m_pperppos = iHistoManager.histogram(2005, "pperp of positive tracks", 420, -10., 200.);
   m_pperpneg = iHistoManager.histogram(3005, "pperp of negative tracks", 420, -10., 200.);
   m_pperppos_vcotTh = iHistoManager.profile(2006, "pperp of positive tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kErrorOnMean);
   m_pperpneg_vcotTh = iHistoManager.profile(3006, "pperp of negative tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kErrorOnMean);
   m_pperppos_vcotTh_spread = iHistoManager.profile(2007, "pperp of positive tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kSpread);
   m_pperpneg_vcotTh_spread = iHistoManager.profile(3007, "pperp of negative tracks VS cotTheta", 60, -3., 3., 0., 4., HIHistProf::kSpread);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
ChiSquaredsProc::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks, "TrkmanApproved");
   FATable<NavTrack>::const_iterator track_iter = tracks.begin();
   FATable<NavTrack>::const_iterator track_end = tracks.end();

   FAItem<DBEventHeader> header;
   extract(iFrame.record(Stream::kEvent), header);

   for (; track_iter != track_end;  ++track_iter) {
      DABoolean positive = (track_iter->pionHelix()->curvature() > 0.);
      double cotTheta = track_iter->pionHelix()->cotTheta();
      double chiSquare = track_iter->pionQuality()->chiSquare();
      double dof = double(track_iter->pionQuality()->degreesOfFreedom());
      double pperp = track_iter->pionFit()->momentum().perp();

      cout << "FORPPERP " << header->run() << "." << header->number() << "." << track_iter->identifier() << " " << pperp << endl;

      if (dof > 0.) {
	 if (positive) {
	    cout << "FORREDCHIP " << header->run() << "." << header->number() << "." << track_iter->identifier() << " " << chiSquare/dof << endl;

	    m_chi2pos->fill(chiSquare);
	    m_chi2pos_vcotTh->fill(cotTheta, chiSquare/dof);
	    m_chi2pos_vcotTh_spread->fill(cotTheta, chiSquare/dof);
	    m_probpos->fill(prob_(chiSquare, dof));
	    m_pperppos->fill(pperp);
	    m_pperppos_vcotTh->fill(cotTheta, pperp);
	    m_pperppos_vcotTh_spread->fill(cotTheta, pperp);
	 }
	 else {
	    cout << "FORREDCHIM " << header->run() << "." << header->number() << "." << track_iter->identifier() << " " << chiSquare/dof << endl;

	    m_chi2neg->fill(chiSquare);
	    m_chi2neg_vcotTh->fill(cotTheta, chiSquare/dof);
	    m_chi2neg_vcotTh_spread->fill(cotTheta, chiSquare/dof);
	    m_probneg->fill(prob_(chiSquare, dof));
	    m_pperpneg->fill(pperp);
	    m_pperpneg_vcotTh->fill(cotTheta, pperp);
	    m_pperpneg_vcotTh_spread->fill(cotTheta, pperp);
	 }
      }
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
ChiSquaredsProc::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
ChiSquaredsProc::endRun( Frame& iFrame )         // anal4 equiv.
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
