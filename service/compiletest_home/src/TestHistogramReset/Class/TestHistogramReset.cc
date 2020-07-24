// -*- C++ -*-
//
// Package:     TestHistogramReset
// Module:      TestHistogramReset
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Fri Oct 31 15:30:25 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TestHistogramReset/TestHistogramReset.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"




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
static const char* const kFacilityString = "Processor.TestHistogramReset" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
TestHistogramReset::TestHistogramReset( void )               // anal1
   : Processor( "TestHistogramReset" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &TestHistogramReset::event,    Stream::kEvent );
   //bind( &TestHistogramReset::beginRun, Stream::kBeginRun );
   //bind( &TestHistogramReset::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

TestHistogramReset::~TestHistogramReset()                    // anal5
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
TestHistogramReset::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
TestHistogramReset::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
TestHistogramReset::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   m_hist1d = iHistoManager.histogram(
      100, "hist1d", 100, 0., 1.);

   m_histprof = iHistoManager.profile(
      200, "histprof", 100, 0., 1., -5., 5., HIHistProf::kErrorOnMean);

   m_hist2d = iHistoManager.histogram(
      300, "hist2d", 100, 0., 1., 100, -5., 5.);
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
TestHistogramReset::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   report(SYSTEM, kFacilityString) << "about to m_hist1d->reset();" << endl;
   m_hist1d->reset();
   report(SYSTEM, kFacilityString) << "about to m_histprof->reset();" << endl;
   m_histprof->reset();
   report(SYSTEM, kFacilityString) << "about to m_hist2d->reset();" << endl;
   m_hist2d->reset();

   report(SYSTEM, kFacilityString) << "about to m_hist1d->fill();" << endl;
   m_hist1d->fill(0.5);
   report(SYSTEM, kFacilityString) << "about to m_histprof->fill();" << endl;
   m_histprof->fill(0.5, 1.);
   report(SYSTEM, kFacilityString) << "about to m_histprof->fill();" << endl;
   m_histprof->fill(0.5, 2.);
   report(SYSTEM, kFacilityString) << "about to m_hist2d->fill();" << endl;
   m_hist2d->fill(0.5, 1.);
   report(SYSTEM, kFacilityString) << "about to m_hist2d->fill();" << endl;
   m_hist2d->fill(0.5, 2.);

   report(SYSTEM, kFacilityString) << "all is good." << endl;

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
TestHistogramReset::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
TestHistogramReset::endRun( Frame& iFrame )         // anal4 equiv.
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
