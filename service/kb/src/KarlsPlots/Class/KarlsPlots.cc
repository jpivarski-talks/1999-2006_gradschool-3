// -*- C++ -*-
//
// Package:     KarlsPlots
// Module:      KarlsPlots
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Tue Jan 28 14:30:53 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "KarlsPlots/KarlsPlots.h"
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

#include "Navigation/NavShower.h"
#include "C3cc/CcShowerAttributes.h"
#include "Navigation/NavTkShMatch.h"

#include "BeamEnergy/BeamEnergy.h"

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
static const char* const kFacilityString = "Processor.KarlsPlots" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
KarlsPlots::KarlsPlots( void )               // anal1
   : Processor( "KarlsPlots" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &KarlsPlots::event,    Stream::kEvent );
   //bind( &KarlsPlots::beginRun, Stream::kBeginRun );
   //bind( &KarlsPlots::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

KarlsPlots::~KarlsPlots()                    // anal5
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
KarlsPlots::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
KarlsPlots::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
KarlsPlots::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book histograms

   // 1D histogram: id, title, bins, low, high
   m_p = iHistoManager.histogram(
      100, "momentum of charged tracks", 100, 0., 6.);

   m_pz = iHistoManager.histogram(
      101, "Z momentum of charged tracks", 100, -6., 6.);

   m_pperp = iHistoManager.histogram(
      102, "Perp momentum of charged tracks", 100, 0., 6.);

   m_pcosth = iHistoManager.histogram(
      110, "cos(theta) of charged tracks", 100, -1., 1.);

   // 2D histogram: id, title, xbins, xlow, xhigh, ybins, ylow, yhigh
   m_p_pcosth = iHistoManager.histogram(
      120, "momentum VS cos(theta) of charged tracks", 100, -1., 1., 100, 0., 6.);

   // profile: id, title, xbins, xlow, xhigh, ylow, yhigh, kErrorOnMean or kSpread
   m_p_pcosth_prof = iHistoManager.profile(
      130, "momentum VS cos(theta) of charged tracks", 100, -1., 1., 0., 6.,
      HIHistProf::kErrorOnMean);

   m_e = iHistoManager.histogram(
      200, "energy of neutral particles", 100, 0., 6.);

   m_e_ecosth = iHistoManager.histogram(
      210, "energy VS cos(theta) of neutral particles", 100, -1., 1., 100, 0., 6.);

   m_e_ecosth_prof = iHistoManager.profile(
      220, "energy VS cos(theta) of neutral particles", 100, -1., 1., 0., 6.,
      HIHistProf::kErrorOnMean);

   m_total_mult = iHistoManager.histogram(
      300, "total multiplicity", 100, -0.5, 99.5);

   m_charged_mult = iHistoManager.histogram(
      310, "charged track multiplicity", 50, -0.5, 49.5);

   m_neutral_mult = iHistoManager.histogram(
      320, "neutral shower multiplicity", 50, -0.5, 49.5);

   m_total_energy = iHistoManager.histogram(
      400, "total energy", 60, 0., 12.);

   m_charged_energy = iHistoManager.histogram(
      410, "charged energy", 60, 0., 12.);

   m_neutral_energy = iHistoManager.histogram(
      420, "neutral energy", 60, 0., 12.);

   m_big_hemisphere = iHistoManager.histogram(
      450, "energy in hemisphere with the most energy", 60, 0., 12.);

   m_little_hemisphere = iHistoManager.histogram(
      460, "energy in hemisphere with the least energy", 60, 0., 12.);

   m_total_scalarsum_pt = iHistoManager.histogram(
      500, "scalar sum of particle pt", 60, 0., 12.);

   m_charged_scalarsum_pt = iHistoManager.histogram(
      510, "scalar sum of charged particle pt", 60, 0., 12.);

   m_neutral_scalarsum_pt = iHistoManager.histogram(
      520, "scalar sum of neutral particle pt", 60, 0., 12.);

   m_track_pmag = iHistoManager.histogram(
      600, "charged track pmag", 40, 0., 4.);

   m_track_pt = iHistoManager.histogram(
      610, "charged track pt", 40, 0., 4.);

   m_track_pz = iHistoManager.histogram(
      620, "charged track pz", 80, -4., 4.);

   m_shower_pmag = iHistoManager.histogram(
      700, "neutral shower E", 40, 0., 4.);

   m_shower_pt = iHistoManager.histogram(
      710, "neutral shower track Et", 40, 0., 4.);

   m_shower_pz = iHistoManager.histogram(
      720, "neutral shower track Ez", 80, -4., 4.);

} // end hist_book() subroutine

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
KarlsPlots::event( Frame& iFrame )          // anal3 equiv.
{
   report(DEBUG, kFacilityString) << "here in event()" << endl;

   FAItem<BeamEnergy> eBeam;
   extract(iFrame.record(Stream::kStartRun), eBeam);

   // extract tracks from the Frame
   FATable<NavTrack> tracks;
   extract(iFrame.record(Stream::kEvent), tracks);
   FATable<NavTrack>::const_iterator tracks_begin = tracks.begin();
   FATable<NavTrack>::const_iterator tracks_end = tracks.end();

   int charged_mult = 0;
   double charged_energy = 0.;
   double west_energy = 0.;
   double east_energy = 0.;
   double charged_scalarsum_pt = 0.;

   // loop over tracks
   for ( FATable< NavTrack >::const_iterator track_iter = tracks_begin;
         track_iter != tracks_end;
         ++track_iter )
   {
      double dof = track_iter->pionQuality()->degreesOfFreedom();
      double hits_expected = track_iter->seedQuality()->numberHitsExpected();

      // quality checks
      if ( !track_iter->pionQuality()->fitAbort()                           &&
	   track_iter->pionQuality()->fit()                                 &&
	   dof > 0.                                                         &&
	   track_iter->pionQuality()->chiSquare() / dof < 100.              &&
	   hits_expected > 0.                                               &&
	   track_iter->seedQuality()->numberLayers() / hits_expected > 0.5  &&
	   track_iter->seedQuality()->numberLayers() / hits_expected < 1.2  &&
	   fabs(track_iter->pionHelix()->d0()) < 0.03                       &&
	   fabs(track_iter->pionHelix()->position().z()) < 0.18             &&
	   track_iter->pionFit()->momentum().mag() > 0.01*eBeam->value()    &&
	   track_iter->pionFit()->momentum().mag() < 1.500*eBeam->value()   &&
	   fabs(track_iter->pionHelix()->cotTheta()) < 3.0424               &&  // <=> |cosTh| < 0.95
	   sqrt(track_iter->pionHelix()->errorMatrix()(
		   KTHelix::kCotTheta, KTHelix::kCotTheta)) < 0.5           &&
	   sqrt(track_iter->pionHelix()->errorMatrix()(
		   KTHelix::kZ0, KTHelix::kZ0)) < 0.25                         ) {

	 // Possible completions of track_iter->seedQuality()->FILLINHERE are in
	 // http://www.lns.cornell.edu/~cleo3/current/doc/doxygen/html/classTRSeedTrackQuality.html
	 //
	 // Possible completions of track_iter->pionQuality()->FILLINHERE are in
	 // http://www.lns.cornell.edu/~cleo3/current/doc/doxygen/html/classTRTrackFitQuality.html
	 //
	 // Possible completions of track_iter->pionFit()->FILLINHERE are in
	 // http://www.lns.cornell.edu/~cleo3/current/doc/doxygen/html/classKTKinematicData.html
	 //
	 // Possible completions of track_iter->pionHelix()->FILLINHERE are in
	 // http://www.lns.cornell.edu/~cleo3/current/doc/doxygen/html/classKTHelix.html
	 //
	 // ("pion" refers to the pion hypothesis, which was used to
	 // fit the track.  You can substitute with "proton" or "kaon".)
	 
	 charged_mult++;
	 charged_energy += track_iter->pionFit()->energy();
	 if ( track_iter->pionHelix()->cotTheta() > 0. )
	    west_energy += track_iter->pionFit()->energy();
	 else
	    east_energy += track_iter->pionFit()->energy();
	 charged_scalarsum_pt += track_iter->pionFit()->momentum().perp();

	 // declare/define variables
	 double p = track_iter->pionFit()->momentum().mag();
	 double pcosth = track_iter->pionFit()->momentum().cosTheta();

	 // fill histograms
	 m_p->fill(p);
	 m_pz->fill(track_iter->pionFit()->momentum().z());
	 m_pperp->fill(track_iter->pionFit()->momentum().perp());
	 m_pcosth->fill(pcosth);
	 m_p_pcosth->fill(pcosth, p);
	 m_p_pcosth_prof->fill(pcosth, p);

	 m_track_pmag->fill(track_iter->pionFit()->momentum().mag());
	 m_track_pt->fill(track_iter->pionFit()->momentum().perp());
	 m_track_pz->fill(track_iter->pionFit()->momentum().z());

      } // end if
   } // end for

   int neutral_mult = 0;
   double neutral_energy = 0.;
   double neutral_scalarsum_pt = 0.;

   // extract showers from the Frame
   FATable<NavShower> showers;
   extract(iFrame.record(Stream::kEvent), showers);
   FATable< NavShower >::const_iterator shower_begin = showers.begin();
   FATable< NavShower >::const_iterator shower_end = showers.end();

   // loop over showers
   for (FATable< NavShower >::const_iterator shower_iter = shower_begin; 
        shower_iter != shower_end;
	++shower_iter ) {
      // quality checks
      if ( shower_iter->attributes().status()                         &&
	   !shower_iter->attributes().hot()                           &&
	   shower_iter->attributes().energy() > 0.01*eBeam->value()   &&
	   shower_iter->noTrackMatch()                                &&
	   shower_iter->noTrackMatchConReg()                          &&
	   ( !shower_iter->bremTrack().valid() ||
	     ( shower_iter->bremTrack()->trackShowerMatch().valid()       &&
	       shower_iter->bremTrack()->trackShowerMatch()->eOverP()<0.5    )
	      )                                                          ) {

	 // Possible completions of shower_iter->FILLINHERE are in
	 // http://www.lns.cornell.edu/~cleo3/current/doc/doxygen/html/classNavShower.html
	 //
	 // Possible completions of shower_iter->attributes().FILLINHERE are in
	 // http://www.lns.cornell.edu/~cleo3/current/doc/doxygen/html/classCcShowerAttributes.html

	 neutral_mult++;
	 neutral_energy += shower_iter->attributes().energy();
	 if ( shower_iter->attributes().theta() > 0. )
	    west_energy += shower_iter->attributes().energy();
	 else
	    east_energy += shower_iter->attributes().energy();
	 neutral_scalarsum_pt += shower_iter->attributes().momentum().perp();

	 // declare/define variables
	 double e = shower_iter->attributes().energy();
	 double ecosth = cos(shower_iter->attributes().theta());

	 // fill histograms
	 m_e->fill(e);
	 m_e_ecosth->fill(ecosth, e);
	 m_e_ecosth_prof->fill(ecosth, e);

	 m_shower_pmag->fill(shower_iter->attributes().energy());
	 m_shower_pt->fill(shower_iter->attributes().energy()*fabs(sin(shower_iter->attributes().theta())));
	 m_shower_pz->fill(shower_iter->attributes().energy()*cos(shower_iter->attributes().theta()));

      } // end if
   } // end for

   m_total_mult->fill(charged_mult + neutral_mult);
   m_charged_mult->fill(charged_mult);
   m_neutral_mult->fill(neutral_mult);

   m_total_energy->fill(charged_energy + neutral_energy);
   m_charged_energy->fill(charged_energy);
   m_neutral_energy->fill(neutral_energy);

   if ( west_energy > east_energy ) {
      m_big_hemisphere->fill(west_energy);
      m_little_hemisphere->fill(east_energy);
   }
   else {
      m_big_hemisphere->fill(east_energy);
      m_little_hemisphere->fill(west_energy);
   }

   m_total_scalarsum_pt->fill(charged_scalarsum_pt + neutral_scalarsum_pt);
   m_charged_scalarsum_pt->fill(charged_scalarsum_pt);
   m_neutral_scalarsum_pt->fill(neutral_scalarsum_pt);

   report(DEBUG, kFacilityString) << "not my problem anymore" << endl;
   return ActionBase::kPassed;
} // end event() subroutine

/*
ActionBase::ActionResult
KarlsPlots::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
KarlsPlots::endRun( Frame& iFrame )         // anal4 equiv.
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
