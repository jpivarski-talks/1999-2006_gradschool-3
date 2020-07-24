// -*- C++ -*-
//
// Package:     TestWeightedProf
// Module:      TestWeightedProf
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri May  3 11:30:52 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TestWeightedProf/TestWeightedProf.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
#include "HelixIntersection/HIHelix.h"

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
static const char* const kFacilityString = "Processor.TestWeightedProf" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.30 2002/04/02 22:04:09 ajm36 Exp $";
static const char* const kTagString = "$Name: v06_05_01 $";

//
// static data member definitions
//



//
// constructors and destructor
//
TestWeightedProf::TestWeightedProf( void )               // anal1
   : Processor( "TestWeightedProf" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &TestWeightedProf::event,    Stream::kEvent );
   //bind( &TestWeightedProf::beginRun, Stream::kBeginRun );
   //bind( &TestWeightedProf::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

TestWeightedProf::~TestWeightedProf()                    // anal5
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
TestWeightedProf::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
TestWeightedProf::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)

}


// ---------------- standard place to book histograms ---------------
void
TestWeightedProf::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_unweighted_err = iHistoManager.profile(
      1, "unweighted err", 2, 0.5, 2.5, -100., 100., HIHistProf::kErrorOnMean );
   m_unweighted_spread = iHistoManager.profile(
      2, "unweighted spread", 2, 0.5, 2.5, -100., 100., HIHistProf::kSpread );
   m_weighted_err = iHistoManager.profile(
      3, "weighted err", 2, 0.5, 2.5, -100., 100., HIHistProf::kErrorOnMean );
   m_weighted_spread = iHistoManager.profile(
      4, "weighted spread", 2, 0.5, 2.5, -100., 100., HIHistProf::kSpread );
   m_weighted = iHistoManager.profile(
      5, "weighted", 2, 0.5, 2.5, -100., 100., HIHistProf::kWeightedError );
   m_oned = iHistoManager.histogram(
      6, "oned", 2, 0.5, 2.5 );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
TestWeightedProf::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   static int n;
   static double sum;
   static double sqrsum;
   static double numerator;
   static double denominator;

   FATable< NavTrack > trackTable;
   extract( iFrame.record( Stream::kEvent ) , trackTable );
   FATable< NavTrack >::const_iterator trackTableBegin = trackTable.begin();
   FATable< NavTrack >::const_iterator trackTableEnd = trackTable.end();
   for ( FATable< NavTrack >::const_iterator trackItr = trackTableBegin;
         trackItr != trackTableEnd ;
         ++trackItr ) {
      double value = trackItr->pionHelix()->phi0();
      double error2 = trackItr->pionHelix()->errorMatrix()[HIHelix::kPhi0][HIHelix::kPhi0];

      m_unweighted_err->fill( 1., value );
      m_unweighted_spread->fill( 1., value );
      m_weighted_err->fill( 1., value, 1./error2 );
      m_weighted_spread->fill( 1., value, 1./error2 );
      m_weighted->fill( 1., value, 1./error2 );
      m_oned->fill( 1. );

      float errorArray[2] = { 1., 1. };
      m_oned->setErrors( errorArray );

      n++;
      sum += value;
      sqrsum += value * value;
      numerator += value / error2;
      denominator += 1 / error2;

      double unweighted_err_value( m_unweighted_err->channelValue(1) );
      double unweighted_err_error( m_unweighted_err->channelError(1) );
      double unweighted_spread_value( m_unweighted_spread->channelValue(1) );
      double unweighted_spread_error( m_unweighted_spread->channelError(1) );
      double weighted_err_value( m_weighted_err->channelValue(1) );
      double weighted_err_error( m_weighted_err->channelError(1) );
      double weighted_spread_value( m_weighted_spread->channelValue(1) );
      double weighted_spread_error( m_weighted_spread->channelError(1) );
      double weighted_value( m_weighted->channelValue(1) );
      double weighted_error( m_weighted->channelError(1) );
      double weighted_value_2( m_weighted->channelValue(2) );
      double weighted_error_2( m_weighted->channelError(2) );
      double oned_value( m_oned->channelValue(1) );
      double oned_error( m_oned->channelError(1) );

      cout << endl;

      cout << "official: "
	   << unweighted_err_value << " +/- " << unweighted_err_error << "\t"
	   << unweighted_spread_value << " +/- " << unweighted_spread_error << "\t"
	   << weighted_err_value << " +/- " << weighted_err_error << "\t"
	   << weighted_spread_value << " +/- " << weighted_spread_error << "\t"
	   << weighted_value << " +/- " << weighted_error << "\t"
	   << weighted_value_2 << " +/- " << weighted_error_2 << "\t"
	   << oned_value << " +/- " << oned_error << endl;

      double mean( sum / double(n) );
      double rms( sqrt( sqrsum / double(n) - mean * mean ) );
      double wmean( numerator / denominator );
      double werr( sqrt( 1 / denominator ) );

      cout << "predictions: "
	   << mean << " +/- " << rms / sqrt( double(n) ) << "\t"
	   << mean << " +/- " << rms << "\t"
	   << wmean << " +/- ???\t"
	   << wmean << " +/- ???\t"
	   << wmean << " +/- " << werr << "\t"
	   << "0 +/- 0\t" << n << " +/- 1" << endl;

      cout << endl;

      if ( n == 100 ) {
	 n = 0;
	 sum = 0.;
	 sqrsum = 0.;
	 numerator = 0.;
	 denominator = 0.;
	 
	 float unweighted_err_sum( m_unweighted_err->sumWithinLimits() );
	 float unweighted_err_max( m_unweighted_err->maxValue() );
	 float unweighted_err_min( m_unweighted_err->minValue() );
	 float unweighted_spread_sum( m_unweighted_spread->sumWithinLimits() );
	 float unweighted_spread_max( m_unweighted_spread->maxValue() );
	 float unweighted_spread_min( m_unweighted_spread->minValue() );
	 float weighted_err_sum( m_weighted_err->sumWithinLimits() );
	 float weighted_err_max( m_weighted_err->maxValue() );
	 float weighted_err_min( m_weighted_err->minValue() );
	 float weighted_spread_sum( m_weighted_spread->sumWithinLimits() );
	 float weighted_spread_max( m_weighted_spread->maxValue() );
	 float weighted_spread_min( m_weighted_spread->minValue() );
	 float weighted_sum( m_weighted->sumWithinLimits() );
	 float weighted_max( m_weighted->maxValue() );
	 float weighted_min( m_weighted->minValue() );
	 float oned_sum( m_oned->sumWithinLimits() );
	 float oned_max( m_oned->maxValue() );
	 float oned_min( m_oned->minValue() );
	 float oned_over( m_oned->overflow() );
	 float oned_under( m_oned->underflow() );

	 cout << unweighted_err_sum << " " << unweighted_err_max << " " << unweighted_err_min << endl;
	 cout << unweighted_spread_sum << " " << unweighted_spread_max << " " << unweighted_spread_min << endl;
	 cout << weighted_err_sum << " " << weighted_err_max << " " << weighted_err_min << endl;
	 cout << weighted_spread_sum << " " << weighted_spread_max << " " << weighted_spread_min << endl;
	 cout << weighted_sum << " " << weighted_max << " " << weighted_min << endl;
	 cout << oned_sum << " " << oned_max << " " << oned_min << " "
	      << oned_over << " " << oned_under << endl;

	 m_unweighted_err->reset();
	 m_unweighted_spread->reset();
	 m_weighted_err->reset();
	 m_weighted_spread->reset();
	 m_weighted->reset();
	 m_oned->reset();
      }

   } // end loop over tracks

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
TestWeightedProf::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
TestWeightedProf::endRun( Frame& iFrame )         // anal4 equiv.
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
