// -*- C++ -*-
//
// Package:     CheckBunchBug
// Module:      CheckBunchBug
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri May  7 13:33:14 EDT 2004
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "CheckBunchBug/CheckBunchBug.h"
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

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Processor.CheckBunchBug" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
CheckBunchBug::CheckBunchBug( void )               // anal1
   : Processor( "CheckBunchBug" )
   , m_run(0)
   , m_sumRedChi(0.)
   , m_sumcutRedChi(0.)
   , m_tailRedChi(0)
   , m_totalRedChi(0)
   , m_sumErrCotth(0.)
   , m_sumcutErrCotth(0.)
   , m_tailErrCotth(0)
   , m_totalErrCotth(0)
   , m_sumErrZ0(0.)
   , m_sumcutErrZ0(0.)
   , m_tailErrZ0(0)
   , m_totalErrZ0(0)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &CheckBunchBug::event,    Stream::kEvent );
   //bind( &CheckBunchBug::beginRun, Stream::kBeginRun );
   //bind( &CheckBunchBug::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

CheckBunchBug::~CheckBunchBug()                    // anal5
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
CheckBunchBug::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CheckBunchBug::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)

   summarize();
}


// ---------------- standard place to book histograms ---------------
void
CheckBunchBug::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   m_RedChi = iHistoManager.histogram("reduced chi2", 1000, 0., 200.);
   m_ErrCotth = iHistoManager.histogram("err cotTheta", 1000, 0., 1.0);
   m_ErrZ0 = iHistoManager.histogram("err Z0", 1000, 0., 0.5);
   m_RedChi2 = iHistoManager.histogram("reduced chi2", 1000, 0., 1000.);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
CheckBunchBug::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<DBEventHeader> head;
   extract(iFrame.record(Stream::kEvent), head);
   if (head->run() != m_run) {
      if (m_run != 0) summarize();
      m_run = head->run();
   }

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   FATable<NavTrack>::const_iterator tracks_end = tracks.end();

   for (FATable<NavTrack>::const_iterator track_iter = tracks.begin();
	track_iter != tracks_end;
	++track_iter) {
      const double dof = track_iter->pionQuality()->degreesOfFreedom();
      const double chi2 = track_iter->pionQuality()->chiSquare();
      double reducedChi2 = (dof > 0 ? chi2/dof : 0.);

      const double errCotTheta = sqrt(fabs(track_iter->pionHelix()->errorMatrix()(
					      KTHelix::kCotTheta, KTHelix::kCotTheta)));
      const double errZ0 = sqrt(fabs(track_iter->pionHelix()->errorMatrix()(
					KTHelix::kZ0, KTHelix::kZ0)));

      m_RedChi->fill(reducedChi2);
      m_ErrCotth->fill(errCotTheta);
      m_ErrZ0->fill(errZ0);
      m_RedChi2->fill(reducedChi2);

      m_sumRedChi += reducedChi2;
      if (reducedChi2 < 100.) m_sumcutRedChi += reducedChi2;
      if (reducedChi2 > 100.) m_tailRedChi++;
      m_totalRedChi++;

      m_sumErrCotth += errCotTheta;
      if (errCotTheta < 0.5) m_sumcutErrCotth += errCotTheta;
      if (errCotTheta > 0.5) m_tailErrCotth++;
      m_totalErrCotth++;

      m_sumErrZ0 += errZ0;
      if (errZ0 < 0.25) m_sumcutErrZ0 += errZ0;
      if (errZ0 > 0.25) m_tailErrZ0++;
      m_totalErrZ0++;

   }

   return ActionBase::kPassed;
}

void CheckBunchBug::summarize()
{
   if (m_totalRedChi == 0  ||  m_totalErrCotth == 0  ||  m_totalErrZ0 == 0) {
      report(SYSTEM, kFacilityString)
	 << "ACK!  Run " << m_run << " had no tracks?!?" << endl;
   }
   else {
      double meanRedChi = m_sumRedChi / double(m_totalRedChi);
      double meancutRedChi = m_sumcutRedChi / double(m_totalRedChi);
      double fracRedChi = double(m_tailRedChi) / double(m_totalRedChi);
      double meanErrCotth = m_sumErrCotth / double(m_totalErrCotth);
      double meancutErrCotth = m_sumcutErrCotth / double(m_totalErrCotth);
      double fracErrCotth = double(m_tailErrCotth) / double(m_totalErrCotth);
      double meanErrZ0 = m_sumErrZ0 / double(m_totalErrZ0);
      double meancutErrZ0 = m_sumcutErrZ0 / double(m_totalErrZ0);
      double fracErrZ0 = double(m_tailErrZ0) / double(m_totalErrZ0);

      report(SYSTEM, kFacilityString)
	 << "Run " << m_run << " meanRedChi " << meanRedChi << " meancutRedChi " << meancutRedChi << " fracRedChi " << fracRedChi
	 << " meanErrCotth " << meanErrCotth << " meancutErrCotth " << meancutErrCotth << " fracErrCotth " << fracErrCotth
	 << " meanErrZ0 " << meanErrZ0 << " meancutErrZ0 " << meancutErrZ0 << " fracErrZ0 " << fracErrZ0 << endl;
   }

   m_sumRedChi = 0.;
   m_sumcutRedChi = 0.;
   m_tailRedChi = 0;
   m_totalRedChi = 0;
   m_sumErrCotth = 0.;
   m_sumcutErrCotth = 0.;
   m_tailErrCotth = 0;
   m_totalErrCotth = 0;
   m_sumErrZ0 = 0.;
   m_sumcutErrZ0 = 0.;
   m_tailErrZ0 = 0;
   m_totalErrZ0 = 0;
}

/*
ActionBase::ActionResult
CheckBunchBug::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
CheckBunchBug::endRun( Frame& iFrame )         // anal4 equiv.
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
