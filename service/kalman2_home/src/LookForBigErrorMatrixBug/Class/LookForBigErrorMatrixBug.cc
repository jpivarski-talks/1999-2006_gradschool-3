// -*- C++ -*-
//
// Package:     LookForBigErrorMatrixBug
// Module:      LookForBigErrorMatrixBug
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Mon May 12 13:44:54 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "LookForBigErrorMatrixBug/LookForBigErrorMatrixBug.h"
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
static const char* const kFacilityString = "Processor.LookForBigErrorMatrixBug" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
LookForBigErrorMatrixBug::LookForBigErrorMatrixBug( void )               // anal1
   : Processor( "LookForBigErrorMatrixBug" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &LookForBigErrorMatrixBug::event,    Stream::kEvent );
   //bind( &LookForBigErrorMatrixBug::beginRun, Stream::kBeginRun );
   //bind( &LookForBigErrorMatrixBug::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

LookForBigErrorMatrixBug::~LookForBigErrorMatrixBug()                    // anal5
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
LookForBigErrorMatrixBug::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

   m_run = 0;
}

// -------------------- terminate method ----------------------------
void
LookForBigErrorMatrixBug::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
   if ( m_run != 0  &&  m_total > 0  &&  m_total2 > 0 ) {
      report(SYSTEM, kFacilityString)
	 << "RUN " << m_run << " "
	 << double(m_bad)/double(m_total) << " " << double(m_bad)/double(m_real_total) << " "
	 << double(m_bad2)/double(m_total2) << " " << double(m_bad2)/double(m_real_total) << " "
	 << endl;
   }
}


// ---------------- standard place to book histograms ---------------
void
LookForBigErrorMatrixBug::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
LookForBigErrorMatrixBug::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<DBEventHeader> header;
   extract(iFrame.record(Stream::kEvent), header);
   
   if ( header->run() != m_run ) {
      if ( m_run != 0  &&  m_total > 0  &&  m_total2 > 0 ) {
	 report(SYSTEM, kFacilityString)
	    << "RUN " << m_run << " "
	    << double(m_bad)/double(m_total) << " " << double(m_bad)/double(m_real_total) << " "
	    << double(m_bad2)/double(m_total2) << " " << double(m_bad2)/double(m_real_total) << " "
	    << endl;
      }
      m_total = 0;
      m_total2 = 0;
      m_real_total = 0;
      m_bad = 0;
      m_bad2 = 0;
      m_run = header->run();
   }

   // Create a table of tracks and fill it.
   FATable< NavTrack > trackTable;
   extract( iFrame.record( Stream::kEvent ) , trackTable );

   FATable< NavTrack >::const_iterator trackTableBegin = trackTable.begin();
   FATable< NavTrack >::const_iterator trackTableEnd = trackTable.end();
   // Loop over tracks.
   for ( FATable< NavTrack >::const_iterator trackItr = trackTableBegin;
         trackItr != trackTableEnd ;
         ++trackItr )
   {
      if ( fabs(trackItr->pionHelix()->d0()) < 0.02  &&  trackItr->pionQuality()->degreesOfFreedom() > 6 ) {
	 KTHelix helix(*trackItr->pionHelix());
	 double dummy;
	 helix.moveToRadius(0.02, dummy);
	 if ( helix.referencePoint().z() < 0. ) {
	    if (trackItr->exitPionQuality()->degreesOfFreedom() < 0 ) {
	       m_bad++;
	    }
	    m_total++;
	 }
      }

      if ( fabs(trackItr->exitPionHelix()->cotTheta()) < 0.1 ) {
	 if ( trackItr->exitPionFit()->position().x() * trackItr->exitPionFit()->momentum().x() +
	      trackItr->exitPionFit()->position().y() * trackItr->exitPionFit()->momentum().y()   < 0. ) {
	    m_bad2++;
	 }
	 m_total2++;
      }

      m_real_total++;
   }



   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
LookForBigErrorMatrixBug::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
LookForBigErrorMatrixBug::endRun( Frame& iFrame )         // anal4 equiv.
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
