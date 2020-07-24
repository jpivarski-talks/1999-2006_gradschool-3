// -*- C++ -*-
//
// Package:     WheresTheBeamspot
// Module:      WheresTheBeamspot
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Mar 19 17:15:15 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "WheresTheBeamspot/WheresTheBeamspot.h"
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
static const char* const kFacilityString = "Processor.WheresTheBeamspot" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.31 2002/06/12 20:06:57 cleo3 Exp $";
static const char* const kTagString = "$Name: v06_06_01 $";

//
// static data member definitions
//



//
// constructors and destructor
//
WheresTheBeamspot::WheresTheBeamspot( void )               // anal1
   : Processor( "WheresTheBeamspot" )
   , m_run(0)
   , m_sum_z0(0.)
   , m_sum_shifted_z0(0.)
   , m_sqrsum_z0(0.)
   , m_sqrsum_shifted_z0(0.)
   , m_tracks(0)
   , m_shifted_tracks(0)
   , m_indiv_sum_z0(0.)
   , m_indiv_sum_shifted_z0(0.)
   , m_indiv_sqrsum_z0(0.)
   , m_indiv_sqrsum_shifted_z0(0.)
   , m_indiv_tracks(0)
   , m_indiv_shifted_tracks(0)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &WheresTheBeamspot::event,    Stream::kEvent );
   //bind( &WheresTheBeamspot::beginRun, Stream::kBeginRun );
   //bind( &WheresTheBeamspot::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

WheresTheBeamspot::~WheresTheBeamspot()                    // anal5
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
WheresTheBeamspot::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
WheresTheBeamspot::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
   calc_terms(m_run, m_indiv_d0VSphi, m_indiv_shifted_d0VSphi, 15);
   calc_terms(-1, m_d0VSphi_prof, m_shifted_d0VSphi_prof, 79);
}


// ---------------- standard place to book histograms ---------------
void
WheresTheBeamspot::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_d0VSphi = iHistoManager.histogram(
      100, "d0 vs phi", 314, 0., 2.*M_PI, 100, -0.005, 0.005);
   m_d0VSphi_prof = iHistoManager.profile(
      200, "d0 vs phi", 314, 0., 2.*M_PI, -0.005, 0.005, HIHistProf::kErrorOnMean);
   m_z0 = iHistoManager.histogram(
      300, "z0", 100, -0.05, 0.05);

   m_shifted_d0VSphi = iHistoManager.histogram(
      150, "shifted d0 vs phi", 314, 0., 2.*M_PI, 100, -0.005, 0.005);
   m_shifted_d0VSphi_prof = iHistoManager.profile(
      250, "shifted d0 vs phi", 314, 0., 2.*M_PI, -0.005, 0.005, HIHistProf::kErrorOnMean);
   m_shifted_z0 = iHistoManager.histogram(
      350, "shifted z0", 100, -0.05, 0.05);

   m_hm = &iHistoManager;
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
WheresTheBeamspot::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem<DBEventHeader> header;
   extract(iFrame.record(Stream::kEvent), header);

   if ( m_run != header->run() ) {
      if ( m_run != 0 ) calc_terms(m_run, m_indiv_d0VSphi, m_indiv_shifted_d0VSphi, 15);

      char runname[7];
      sprintf(runname, "%06d", header->run());
      string runname2(runname);

      m_indiv_d0VSphi = m_hm->profile(
	 header->run()*1000 + 200, (runname2 + string(" d0 vs phi")).c_str(),
	 31, 0., 2.*M_PI, -0.005, 0.005, HIHistProf::kErrorOnMean);
      m_indiv_shifted_d0VSphi = m_hm->profile(
	 header->run()*1000 + 250, (runname2 + string(" shifted d0 vs phi")).c_str(),
	 31, 0., 2.*M_PI, -0.005, 0.005, HIHistProf::kErrorOnMean);
      m_indiv_z0 = m_hm->histogram(
	 header->run()*1000 + 300, (runname2 + string(" z0")).c_str(),
	 100, -0.05, 0.05);
      m_indiv_shifted_z0 = m_hm->histogram(
	 header->run()*1000 + 350, (runname2 + string(" shifted z0")).c_str(),
	 100, -0.05, 0.05);

      m_indiv_sum_z0 = 0.;
      m_indiv_sum_shifted_z0 = 0.;
      m_indiv_sqrsum_z0 = 0.;
      m_indiv_sqrsum_shifted_z0 = 0.;
      m_indiv_tracks = 0;
      m_indiv_shifted_tracks = 0;

      m_run = header->run();
   }

   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   FATable<NavTrack>::const_iterator tracks_end = tracks.end();
   
   FAItem<BeamSpot> beamSpot;
   extract(iFrame.record(Stream::kBeginRun), beamSpot);

   for ( FATable<NavTrack>::const_iterator track_iter = tracks.begin();
	 track_iter != tracks_end;
	 ++track_iter ) {
      KTHelix myCopy(*(track_iter->pionHelix()));

      m_d0VSphi->fill(myCopy.phi0(), myCopy.d0());
      m_d0VSphi_prof->fill(myCopy.phi0(), myCopy.d0());
      m_indiv_d0VSphi->fill(myCopy.phi0(), myCopy.d0());
      m_z0->fill(myCopy.z0());
      m_indiv_z0->fill(myCopy.z0());

      m_sum_z0 += myCopy.z0();
      m_sqrsum_z0 += sqr(myCopy.z0());
      m_tracks++;

      m_indiv_sum_z0 += myCopy.z0();
      m_indiv_sqrsum_z0 += sqr(myCopy.z0());
      m_indiv_tracks++;

      double distanceMoved;
      if ( myCopy.moveToReferencePoint(beamSpot->center(), distanceMoved)
	   != KTMoveControl::kMoveOK )
	 report(ERROR, kFacilityString) << "Move failure!!!" << endl;
      else {
	 m_shifted_d0VSphi->fill(myCopy.phi0(), myCopy.d0());
	 m_shifted_d0VSphi_prof->fill(myCopy.phi0(), myCopy.d0());
	 m_indiv_shifted_d0VSphi->fill(myCopy.phi0(), myCopy.d0());
	 m_shifted_z0->fill(myCopy.z0());
	 m_indiv_shifted_z0->fill(myCopy.z0());

	 m_sum_shifted_z0 += myCopy.z0();
	 m_sqrsum_shifted_z0 += sqr(myCopy.z0());
	 m_shifted_tracks++;

	 m_indiv_sum_shifted_z0 += myCopy.z0();
	 m_indiv_sqrsum_shifted_z0 += sqr(myCopy.z0());
	 m_indiv_shifted_tracks++;
      }
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
WheresTheBeamspot::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
WheresTheBeamspot::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

void WheresTheBeamspot::calc_terms(int run, HIHistProf* normal, HIHistProf* shifted, int n)
{
   int i, j;
   double constterm(0.);
   double* sinterm = new double[n];
   double* costerm = new double[n];
   double err_constterm(0.);
   double* err_sinterm = new double[n];
   double* err_costerm = new double[n];
   double shifted_constterm(0.);
   double* shifted_sinterm = new double[n];
   double* shifted_costerm = new double[n];
   double shifted_err_constterm(0.);
   double* shifted_err_sinterm = new double[n];
   double* shifted_err_costerm = new double[n];

   for ( j = 0;  j < n;  j++ ) {
      sinterm[j] = 0.;
      costerm[j] = 0.;
      err_sinterm[j] = 0.;
      err_costerm[j] = 0.;
      shifted_sinterm[j] = 0.;
      shifted_costerm[j] = 0.;
      shifted_err_sinterm[j] = 0.;
      shifted_err_costerm[j] = 0.;
   }

   int nchannels(normal->nChannels());
   for ( i = 1;  i <= nchannels;  i++ ) {
      double x(normal->channelCenter(i));
      double y(normal->channelValue(i));
      double ey(normal->channelError(i));
	    
      constterm += y;
      err_constterm += sqr(ey);
      for ( j = 0;  j < n;  j++ ) {
	 sinterm[j] += y * sin(double(j+1) * x);
	 costerm[j] += y * cos(double(j+1) * x);
	 err_sinterm[j] += sqr(ey * sin(double(j+1) * x));
	 err_costerm[j] += sqr(ey * cos(double(j+1) * x));
      }
   }

   nchannels = shifted->nChannels();
   for ( i = 1;  i <= nchannels;  i++ ) {
      double x(shifted->channelCenter(i));
      double y(shifted->channelValue(i));
      double ey(shifted->channelError(i));
	    
      shifted_constterm += y;
      shifted_err_constterm += sqr(ey);
      for ( j = 0;  j < n;  j++ ) {
	 shifted_sinterm[j] += y * sin(double(j+1) * x);
	 shifted_costerm[j] += y * cos(double(j+1) * x);
	 shifted_err_sinterm[j] += sqr(ey * sin(double(j+1) * x));
	 shifted_err_costerm[j] += sqr(ey * cos(double(j+1) * x));
      }
   }

   constterm *= 2. / double(nchannels);
   err_constterm = sqrt(err_constterm) * 2. / double(nchannels);
   shifted_constterm *= 2. / double(nchannels);
   shifted_err_constterm = sqrt(shifted_err_constterm) * 2. / double(nchannels);
   report(SYSTEM, kFacilityString)
      << "CONST " << run << " before " << constterm << " +/- " << err_constterm
      << " after " << shifted_constterm << " +/- " << shifted_err_constterm << endl;
   for ( j = 0;  j < n;  j++ ) {
      sinterm[j] *= 2. / double(nchannels);
      costerm[j] *= 2. / double(nchannels);
      err_sinterm[j] = sqrt(err_sinterm[j]) * 2. / double(nchannels);
      err_costerm[j] = sqrt(err_costerm[j]) * 2. / double(nchannels);
      shifted_sinterm[j] *= 2. / double(nchannels);
      shifted_costerm[j] *= 2. / double(nchannels);
      shifted_err_sinterm[j] = sqrt(shifted_err_sinterm[j]) * 2. / double(nchannels);
      shifted_err_costerm[j] = sqrt(shifted_err_costerm[j]) * 2. / double(nchannels);
      report(SYSTEM, kFacilityString)
	 << "SIN" << (j+1) << "X " << run
	 << " before " << sinterm[j] << " +/- " << err_sinterm[j]
	 << " after " << shifted_sinterm[j] << " +/- " << shifted_err_sinterm[j] << endl;
      report(SYSTEM, kFacilityString)
	 << "COS" << (j+1) << "X " << run
	 << " before " << costerm[j] << " +/- " << err_costerm[j]
	 << " after " << shifted_costerm[j] << " +/- " << shifted_err_costerm[j] << endl;
   }

   if ( run == -1 )
      report(SYSTEM, kFacilityString)
	 << "MEANZ0 " << run << " before " << m_sum_z0/double(m_tracks)
	 << " +/- " << sqrt(m_sqrsum_z0/double(m_tracks) -
			    sqr(m_sum_z0/double(m_tracks)))/sqrt(double(m_tracks))
	 << " after " << m_sum_shifted_z0/double(m_shifted_tracks)
	 << " +/- " << (sqrt(m_sqrsum_shifted_z0/double(m_shifted_tracks) -
			     sqr(m_sum_shifted_z0/double(m_shifted_tracks)))
			/sqrt(double(m_shifted_tracks))) << endl;
   else
      report(SYSTEM, kFacilityString)
	 << "MEANZ0 " << run << " before " << m_indiv_sum_z0/double(m_indiv_tracks)
	 << " +/- " << sqrt(m_indiv_sqrsum_z0/double(m_indiv_tracks) -
			    sqr(m_indiv_sum_z0/double(m_indiv_tracks)))/sqrt(double(m_indiv_tracks))
	 << " after " << m_indiv_sum_shifted_z0/double(m_indiv_shifted_tracks)
	 << " +/- " << (sqrt(m_indiv_sqrsum_shifted_z0/double(m_indiv_shifted_tracks) -
			     sqr(m_indiv_sum_shifted_z0/double(m_indiv_shifted_tracks)))
			/sqrt(double(m_indiv_shifted_tracks))) << endl;

   delete [] sinterm;
   delete [] costerm;
   delete [] err_sinterm;
   delete [] err_costerm;
   delete [] shifted_sinterm;
   delete [] shifted_costerm;
   delete [] shifted_err_sinterm;
   delete [] shifted_err_costerm;
}

//
// const member functions
//

//
// static member functions
//
