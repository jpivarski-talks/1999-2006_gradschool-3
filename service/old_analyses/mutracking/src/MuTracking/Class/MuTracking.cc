// -*- C++ -*-
//
// Package:     MuTracking
// Module:      MuTracking
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri Oct 26 13:25:53 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "MuTracking/MuTracking.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "CleoDB/DBEventHeader.h"
#include "EventType/EventType.h"

#include "Navigation/NavTrack.h"
#include "CLHEP/Vector/ThreeVector.h"

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
static const char* const kFacilityString = "Processor.MuTracking" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.21 2001/09/01 22:44:37 llh14 Exp $";
static const char* const kTagString = "$Name: v06_01_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
MuTracking::MuTracking( void )               // anal1
   : Processor( "MuTracking" )
   , m_logName( "log", this, "peakfits.log" )
   , m_kumacName( "kumac", this, "peakfits.kumac" )
   , m_hm( NULL )
   , m_working( NULL )
   , m_log( NULL )
   , m_kumac( NULL )
   , m_current_run( 0 )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &MuTracking::event,    Stream::kEvent );
   //bind( &MuTracking::beginRun, Stream::kBeginRun );
   //bind( &MuTracking::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

MuTracking::~MuTracking()                    // anal5
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
MuTracking::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
MuTracking::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
   
   finish_run( m_current_run, *m_log, *m_kumac );
}

// ---------------- standard place to book histograms ---------------
void
MuTracking::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_hm = &iHistoManager;

   m_working = iHistoManager.histogram(
      100, "Mupair Momentum", 200, 4.5, 6.0 );

   double paramsStart[] = { 1000., 5.2, 0.05, 300., 5.3, 0.05 };
   double paramsStep[] = { 5., 0.05, 0.01, 3., 0.05, 0.01 };
   m_working->attachFitFunc( new MomentumFitFunc(
      m_working, HIHistFitFunc::kHist1D, paramsStart, paramsStep ) );
   m_working->fitFunc()->setDomain( 5.0, 5.5 );
   
   m_log = new ofstream( m_logName.value().c_str() );
   m_kumac = new ofstream( m_kumacName.value().c_str() );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
MuTracking::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );
   unsigned int run = header->run();

   if ( m_current_run == 0 )
      m_current_run = run;

   if ( run != m_current_run )
   {
      finish_run( m_current_run, *m_log, *m_kumac );
      m_current_run = run;
   }

   FAItem< EventType > type;
   extract( iFrame.record( Stream::kEvent ), type );
   if ( type->unknown()  ||  ! type->muPair() ) return ActionBase::kFailed;

   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator navtracks_iterator;
   FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();
//   DBCandidate::Hypo particle = DBCandidate::kElectron;
   DBCandidate::Hypo particle = DBCandidate::kMuon;

   if ( navtracks.size() != 2 ) return ActionBase::kFailed;
   if ( ( navtracks_iterator =
	  navtracks_begin )->quality( particle )->fitAbort()  ||
	( ++navtracks_iterator )->quality( particle )->fitAbort() )
      return ActionBase::kFailed;

   for( navtracks_iterator = navtracks_begin;
	navtracks_iterator != navtracks_end;
	navtracks_iterator++ )
   {
      // Get the Kalman-fitted track
      FAItem< TDKinematicFit > trackFit =
	 navtracks_iterator->kinematicFit( particle );

      FAItem< TRHelixFit > trackHelix =
	 navtracks_iterator->helixFit( particle );

      TRHelixFit loc_trackHelix = (* trackHelix);
      Meters movelength;
      KTHelix::MoveStatus status = loc_trackHelix.moveToReferencePoint(
	 HepPoint3D( -0.00160, 0., 0. ), movelength );

      FAItem< TRTrackFitQuality > trackQuality =
	 navtracks_iterator->quality( particle );

      if ( ! trackQuality->fitAbort()                 &&
	   abs( loc_trackHelix.d0() ) < 0.0015        &&
	   abs( loc_trackHelix.z0() ) < 0.04              )
      {
	 m_working->fill( trackFit->momentum().mag() );
      }
      else
	 return ActionBase::kFailed;

   } // end foreach track

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
MuTracking::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
MuTracking::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

void
MuTracking::finish_run( unsigned int run, ofstream& log, ofstream& kumac )
{
   // do one last fit
   MomentumFitFunc* fitfunc = (MomentumFitFunc*)(m_working->fitFunc());
   fitfunc->fit();

   // record its parameters

   cout << "Run " << run << ":" << endl;
   fitfunc->print( cout );  cout << endl;

   log << "Run " << run << ":" << endl;
   fitfunc->print( log );  log << endl;

   // copy the histogram and clear the working hist

   char runNum[7];
   sprintf( runNum, "%06d", run );

   HIHist1D* copyHist = m_hm->histogram(
      run, ( string( "Mupair Momentum for " ) + string( runNum ) ).c_str(),
	     200, 4.5, 6.0 );
   
   copyHist->copy( *m_working );
   m_working->reset();

   // write the appropriate commands to the kumac file

   kumac << "h/plot " << run << endl;

   double area1 = fitfunc->parameter( MomentumFitFunc::kArea1 ).value();
   double mean1 = fitfunc->parameter( MomentumFitFunc::kMean1 ).value();
   double sigma1 =
      abs( fitfunc->parameter( MomentumFitFunc::kSigma1 ).value() );
   double area2 = fitfunc->parameter( MomentumFitFunc::kArea2 ).value();
   double mean2 = fitfunc->parameter( MomentumFitFunc::kMean2 ).value();
   double sigma2 =
      abs( fitfunc->parameter( MomentumFitFunc::kSigma2 ).value() );

   char comm[512];
   sprintf( comm, "%.1f*exp(-(x-(%.4f))**2/%.4f**2/2.)/sqrt(2.*3.1415926)/%.4f+%.1f*exp(-(x-(%.4f))**2/%.4f**2/2.)/sqrt(2.*3.1415926)/%.4f\n",
	    area1, mean1, sigma1, sigma1,
	    area2, mean2, sigma2, sigma2,
	    fitfunc->domainXStart(),
	    fitfunc->domainXEnd() );
   kumac << comm << endl;
}

//
// const member functions
//

//
// static member functions
//
