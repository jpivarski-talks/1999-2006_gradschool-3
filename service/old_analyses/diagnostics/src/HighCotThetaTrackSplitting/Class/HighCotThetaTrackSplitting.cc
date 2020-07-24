// -*- C++ -*-
//
// Package:     HighCotThetaTrackSplitting
// Module:      HighCotThetaTrackSplitting
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Mon Feb 26 10:28:15 EST 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "HighCotThetaTrackSplitting/HighCotThetaTrackSplitting.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "TrackRoot/TRSeedTrack.h"
#include "MagField/MagneticField.h"
//#include "DualTrackHelices/DualTrackHelices.h"

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
static const char* const kFacilityString = "Processor.HighCotThetaTrackSplitting" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.17 2000/12/04 19:11:11 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
HighCotThetaTrackSplitting::HighCotThetaTrackSplitting( void )               // anal1
   : Processor( "HighCotThetaTrackSplitting" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &HighCotThetaTrackSplitting::event,    Stream::kEvent );
   //bind( &HighCotThetaTrackSplitting::beginRun, Stream::kBeginRun );
   bind( &HighCotThetaTrackSplitting::startRun, Stream::kStartRun );
   //bind( &HighCotThetaTrackSplitting::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

HighCotThetaTrackSplitting::~HighCotThetaTrackSplitting()                    // anal5
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
HighCotThetaTrackSplitting::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
HighCotThetaTrackSplitting::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
HighCotThetaTrackSplitting::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_track_finished = false;
   m_tracks_seen = 0;

   const char* nt_track_labels[ 22 ] =
   { "ntracks",
     "cotTh", "curv", "d0", "phi0", "z0",
     "e_cotTh", "e_curv", "e_d0", "e_phi0", "e_z0",
     "px", "py", "pz", "e_px", "e_py", "e_pz",
     "chi2", "dof", "conflev", "nhits", "exphits" };

   m_trackvars = iHistoManager.ntuple(
      10, "track", 22, 262144, nt_track_labels );

   const char* nt_event_labels[ 28 ] =
   { "d0p", "d0n", "e_d0p", "e_d0n",
     "z0p", "z0n", "e_z0p", "e_z0n",
     "ctThp", "ctThn", "e_ctThp", "e_ctThn", 
     "phi0p", "phi0n", "e_phi0p", "e_phi0n",
     "momp", "momn", "ptx", "pty", "ptz",
     "c2dofp", "c2dofn", "confp", "confn", "hitrp", "hitrn",
     "photon" };

   m_eventvars = iHistoManager.ntuple(
      20, "event", 28, 262144, nt_event_labels );
}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
HighCotThetaTrackSplitting::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   if ( m_track_finished )
   {
      report( INFO, kFacilityString )
	 << "Finished filling ntuple. Trying to exit nicely..." << endl;
      return ActionBase::kPassed;
   }

//     FAItem< CcEventSummary > eventSummary;
//     extract( iFrame.record( Stream::kEvent ), eventSummary );
//     report( DEBUG, kFacilityString ) << "event: got CcEventSummary" << endl;

//   int qednum = 0;
//    if ( eventSummary->qedEvent().muPair() )
//       qednum += 1;
//    if ( eventSummary->qedEvent().radMuPair() )
//       qednum += 2;
// //    if ( eventSummary->qedEvent().mmZMiss() )
//       qednum += 4;

//    // The biggest this number could ever get is 131071
//    if ( eventSummary->qedEvent().eeZMiss() )
//       qednum += 1;		 
//    if ( eventSummary->qedEvent().bhabhaBarrel() )
//       qednum += 2;		 
//    if ( eventSummary->qedEvent().bhabhaEndcap() )
//       qednum += 4;		 
//    if ( eventSummary->qedEvent().radBhaGamBarrel() )
//       qednum += 8;		 
//    if ( eventSummary->qedEvent().radBhaGamEndcap() )
//       qednum += 16;		 
//    if ( eventSummary->qedEvent().muPair() )
//       qednum += 32;		 
//    if ( eventSummary->qedEvent().radMuPair() )
//       qednum += 64;		 
//    if ( eventSummary->qedEvent().gamGamBarrel() )
//       qednum += 128;		 
//    if ( eventSummary->qedEvent().gamGamEndcap() )
//       qednum += 256;		 
//    if ( eventSummary->qedEvent().bhaGamLoose() )
//       qednum += 512;
//    if ( eventSummary->qedEvent().bhaGamTight() )
//       qednum += 1024;		 
//    if ( eventSummary->qedEvent().barrelBhabhaInnerPrescale() )
//       qednum += 2048;		 
//    if ( eventSummary->qedEvent().barrelBhabhaMiddlePrescale() )
//       qednum += 4096;		 
//    if ( eventSummary->qedEvent().barrelBhabhaOuterPrescale() )
//       qednum += 8192;		 
//    if ( eventSummary->qedEvent().barrelBhabhaWholePrescale() )
//       qednum += 16384;
//    if ( eventSummary->qedEvent().endcapBhabhaInnerPrescale() )
//       qednum += 32768;		 
//    if ( eventSummary->qedEvent().endcapBhabhaOuterPrescale() )
//       qednum += 65536;		 


//    if ( qednum == 0 )
//    {
//       report( INFO, kFacilityString )
// 	 << "Not a CC-selected event... skipping it." << endl;
//       return ActionBase::kFailed;
//    }

//     if ( !eventSummary->qedEvent().bhabhaBarrel()   &&
//  	!eventSummary->qedEvent().bhabhaEndcap()      )
//     {
//        report( INFO, kFacilityString )
//  	 << "This is not a bhabha. Exiting..." << endl;
//        return ActionBase::kFailed;
//     }

//     if ( !eventSummary->qedEvent().radBhaGamBarrel()   &&
//  	!eventSummary->qedEvent().radBhaGamEndcap()      )
//     {
//        report( INFO, kFacilityString )
//  	 << "This is not a radiative bhabha. Exiting..." << endl;
//        return ActionBase::kFailed;
//     }

//     if ( ! eventSummary->qedEvent().muPair() )
//     {
//        report( INFO, kFacilityString )
//   	 << "This is not a mupair. Exiting..." << endl;
//        return ActionBase::kFailed;
//     }

   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator navtracks_iterator;
   FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();
   report( DEBUG, kFacilityString ) << "event: got NavTracks" << endl;

//   DBCandidate::Hypo particle = DBCandidate::kElectron;
   DBCandidate::Hypo particle = DBCandidate::kMuon;

   int numtracks = navtracks.size();

   float pos_trackvars[ 12 ];
   float neg_trackvars[ 12 ];
   HepVector3D momp, momn;
   DABoolean pos_valid = false;
   DABoolean neg_valid = false;

   if ( navtracks.size() != 2 )
      return ActionBase::kFailed;

//     FATable< DualTrackHelices > dualtracks;
//     extract( iFrame.record( Stream::kEvent ), dualtracks );
//     FATable< DualTrackHelices >::const_iterator dualtracks_iterator;
//     FATable< DualTrackHelices >::const_iterator dualtracks_begin = dualtracks.begin();
//     FATable< DualTrackHelices >::const_iterator dualtracks_end = dualtracks.end();

   // loop over mupairs (or electrons)
//     for( navtracks_iterator = navtracks_begin, dualtracks_iterator = dualtracks_begin;
//  	navtracks_iterator != navtracks_end;
//  	navtracks_iterator++, dualtracks_iterator++ )
   for( navtracks_iterator = navtracks_begin;
	navtracks_iterator != navtracks_end;
	navtracks_iterator++ )
   {
      // Get the seedtrack
      FAItem< TRSeedTrack > seedTrack =
	 navtracks_iterator->seedTrack();
      if ( seedTrack == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "seedTrack is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }
      
      // Get the Kalman-fitted track
      FAItem< TDKinematicFit > trackFit =
	 navtracks_iterator->kinematicFit( particle );
      if ( trackFit == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "kinematicFit is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }

      report( DEBUG, kFacilityString )
	 << "Haven't asked for momentum yet." << endl;
      // This is accounting for crossing angle
//        Hep3Vector true_momentum =
//   	 trackFit->momentum() - Hep3Vector( -0.013, 0., 0. );
      Hep3Vector true_momentum = trackFit->momentum();
//        Hep3Vector true_momentum =
//  	 seedTrack->momentum( m_bField ) - Hep3Vector( -0.013, 0., 0. );
      report( DEBUG, kFacilityString )
	 << "Got momentum!" << endl;

      // and the Kalman-fitted helix
      FAItem< TRHelixFit > trackHelix =
	 navtracks_iterator->helixFit( particle );
      if ( trackHelix == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "helixFit is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }
      KTHelix loc_trackHelix = (* trackHelix );
//    KTHelix loc_trackHelix = dualtracks_iterator->helix();
//    KTHelix loc_trackHelix = (* seedTrack );
   
//     // Move the track's reference point to the origin so that d0 and
//     // z0 will be right (later, this will be to the beamspot)
//        report( INFO, kFacilityString )
//  	 << "Attempting to move helix." << endl;
//        Meters movelength;
//        KTHelix::MoveStatus status =
//  	 loc_trackHelix.moveToReferencePoint(
//  //  //	    HepPoint3D( -0.001709835, 0.000080827, 0. ), movelength );
//  //  	    HepPoint3D( -0.14298E-02, 0.13478E-04, 0. ), movelength );
//  //  // After the above (second one), I still see a sin + cos of this magnitude:
//  //  //     1      P1       0.25594E-03   0.10014E-04   0.10309E-04   -714.40    
//  //  //     2      P2      -0.19023E-03   0.98585E-05   0.10149E-04    532.41    
//  	    HepPoint3D( -0.00210, 0., 0. ), movelength );

//        if ( status != KTMoveControl::kMoveOK )
//        {
//  	 report( EMERGENCY, kFacilityString )
//  	    << "trackHelix.moveToReferencePoint returned "
//  	    << status << " (not kMoveOK, as it should)." << endl;
//  	 return ActionBase::kFailed;
//        }
//        report( INFO, kFacilityString )
//  	 << "Moved helix sucessfully." << endl;
      
      // Get its quality object
      FAItem< TRTrackFitQuality > trackQuality =
	 navtracks_iterator->quality( particle );
      if ( trackQuality == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "quality object is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }

      // some trackquality variables
      float nhits = trackQuality->numberHits();
      float exphits = trackQuality->numberHitsExpected();
      
      report( INFO, kFacilityString )
	 << "nhits = " << nhits << ", exphits = " << exphits << endl;

      // Don't even cut on this, put it in a bit of numtracks
      if ( trackQuality->fitAbort() )
	 numtracks *= -1;

      // The helix variables
      float cotTh = loc_trackHelix.cotTheta();
      float curv = loc_trackHelix.curvature();
      float d0 = loc_trackHelix.d0();
      float phi0 = loc_trackHelix.phi0();
      float z0 = loc_trackHelix.z0();

      float e_cotTh = loc_trackHelix.errorMatrix()(
	 KTHelix::kCotTheta, KTHelix::kCotTheta );
      float e_curv = loc_trackHelix.errorMatrix()(
	 KTHelix::kCurvature, KTHelix::kCurvature );
      float e_d0 = loc_trackHelix.errorMatrix()(
	 KTHelix::kD0, KTHelix::kD0 );
      float e_phi0 = loc_trackHelix.errorMatrix()(
	 KTHelix::kPhi0, KTHelix::kPhi0 );
      float e_z0 = loc_trackHelix.errorMatrix()(
	 KTHelix::kZ0, KTHelix::kZ0 );

      report( INFO, kFacilityString )
	 << "cotTh = " << cotTh << " +/- " << e_cotTh << endl;
      report( INFO, kFacilityString )
	 << "curv = " << curv << " +/- " << e_curv << endl;
      report( INFO, kFacilityString )
	 << "d0 = " << d0 << " +/- " << e_d0 << endl;
      report( INFO, kFacilityString )
	 << "phi0 = " << phi0 << " +/- " << e_phi0 << endl;
      report( INFO, kFacilityString )
	 << "z0 = " << z0 << " +/- " << e_z0 << endl;

      // The kinematicfit variables
      float px = true_momentum.x();
      float py = true_momentum.y();
      float pz = true_momentum.z();
      float pmag = true_momentum.mag();

      float e_px = trackFit->errorMatrix()(
	 KTKinematicData::kPx, KTKinematicData::kPx );
      float e_py = trackFit->errorMatrix()(
	 KTKinematicData::kPy, KTKinematicData::kPy );
      float e_pz = trackFit->errorMatrix()(
	 KTKinematicData::kPz, KTKinematicData::kPz );

      report( INFO, kFacilityString )
	 << "px = " << px << " +/- " << e_px << ", "
	 << "py = " << py << " +/- " << e_py << ", "
	 << "pz = " << pz << " +/- " << e_pz << endl;

      // The (other) trackQuality variables
      float chi2 = trackQuality->chiSquare();
      float dof = float( trackQuality->degreesOfFreedom() );
      float conflev = prob_( chi2, trackQuality->degreesOfFreedom() );

      report( INFO, kFacilityString )
	 << "chi2 = " << chi2 << ", "
	 << "dof = " << dof << ", "
	 << "conflev = " << conflev << endl;

//       // Additional cuts based on looking at first ntuple (acceptance
//       // is wider than the values I chose when I looked at the ntuple
//       // by a factor of two)
//       if ( abs(cotTh) < 2.*1.6                                         &&
// 	   abs(curv) < 2.*0.1                                          &&
// 	   (pow((x+0.045E-2),2.)+pow((y+0.005E-2),2.)) < 4.*0.49E-6    &&
// 	   abs(z) < 2.*0.04                                            &&
// 	   (pow(px,2.)+pow(py,2.)) < 4.*(36.)                          &&
// 	   (pow(px,2.)+pow(py,2.)) > 4.*(1.)                           &&
// 	   abs(pz) < 2.*4.5                                            &&
// 	   0.4E-4/2. < e_cotTh < 2.*0.07E-3                            &&
// 	   0.25E-6/2. < e_curv < 2.*0.043E-5                           &&
// 	   0.1E-7/2. < e_d0 < 2.*0.17E-7                               &&
// 	   0.17E-4/2. < e_z0 < 2.*0.022E-3                             &&
// 	   0.02E-5/2. < e_phi0 < 2.*0.012E-4                           &&
// 	   0. < e_x < 2.*0.015E-6                                      &&
// 	   0. < e_y < 2.*0.016E-6                                      &&
// 	   0.16E-4/2. < e_z < 2.*0.22E-4                               &&
// 	   0. < e_px < 2.*0.003                                        &&
// 	   0. < e_py < 2.*0.004                                        &&
// 	   0.0015/2. < e_pz < 2.*0.0025                                &&
// 	   0.0017/2. < e_en < 2.*0.0055                                &&
// 	   // this one is the same
// 	   exphits > 0.                                                &&
// 	   0.74 < (nhits/exphits) < 1.01                               &&
// 	   // and this one has been expanded significantly
// 	   0. < (chi2/dof) < 10.                                          )
//       {
	 report( INFO, kFacilityString )
	    << "  Taking this track." << endl;

	 report( INFO, kFacilityString )
	    << "  We have seen " << m_tracks_seen << " tracks." << endl;
//	 if ( m_tracks_seen > 50000 )
//	    m_track_finished = true;
//	 else
//	 {
	    report( INFO, kFacilityString )
	       << "    This track is being added to the ntuple." << endl;
	    
	    float trackvars[ 22 ] =
	    { float(numtracks),
	      cotTh, curv, d0, phi0, z0,
	      e_cotTh, e_curv, e_d0, e_phi0, e_z0,
	      px, py, pz, e_px, e_py, e_pz,
	      chi2, dof, conflev, nhits, exphits };
	    
	    m_trackvars->fill( trackvars );
	    m_tracks_seen++;

	    if ( numtracks == 2  &&  dof != 0.  &&  exphits != 0. )
	       // numtracks == 2 implies that fit didn't abort (by being positive)
	    {
	       if ( curv > 0.  &&  pos_valid == false )
	       {
		  pos_trackvars[ 0] = d0;
		  pos_trackvars[ 1] = e_d0;
		  pos_trackvars[ 2] = z0;
		  pos_trackvars[ 3] = e_z0;
		  pos_trackvars[ 4] = cotTh;
		  pos_trackvars[ 5] = e_cotTh;
		  pos_trackvars[ 6] = phi0;
		  pos_trackvars[ 7] = e_phi0;
		  pos_trackvars[ 8] = pmag;
		  pos_trackvars[ 9] = (chi2/dof);
		  pos_trackvars[10] = conflev;
		  pos_trackvars[11] = (nhits/exphits);
		  momp = true_momentum;

		  pos_valid = true;
	       }
	       else if ( curv < 0.  &&  neg_valid == false )
	       {
		  neg_trackvars[ 0] = d0;
		  neg_trackvars[ 1] = e_d0;
		  neg_trackvars[ 2] = z0;
		  neg_trackvars[ 3] = e_z0;
		  neg_trackvars[ 4] = cotTh;
		  neg_trackvars[ 5] = e_cotTh;
		  neg_trackvars[ 6] = phi0;
		  neg_trackvars[ 7] = e_phi0;
		  neg_trackvars[ 8] = pmag;
		  neg_trackvars[ 9] = (chi2/dof);
		  neg_trackvars[10] = conflev;
		  neg_trackvars[11] = (nhits/exphits);
		  momn = true_momentum;

		  neg_valid = true;
	       }
	    }

//	 } // end if still below fill limit

//       } // end if passes second cut
//       else
// 	 report( INFO, kFacilityString )
// 	    << "  Not taking this track." << endl;

   } // end foreach track

//     // look for a photon (the largest unmatched shower)
//     FATable< NavShower > navShower;
//     extract( iFrame.record( Stream::kEvent ), navShower );
//     FATable< NavShower >::const_iterator shower_iter;
//     FATable< NavShower >::const_iterator shower_begin = navShower.begin();
//     FATable< NavShower >::const_iterator shower_end = navShower.end();
//     int num_significant_showers = 0;
//     float photon_energy = 0.;
//     for ( shower_iter = shower_begin;  shower_iter != shower_end;  shower_iter++ )
//        if ( shower_iter->noTrackMatch() )
//        {
//  	 const CcShowerAttributes& atts ( shower_iter->attributes() );
//  	 if ( atts.goodBarrel() )
//  	 {
//  	    if ( photon_energy < atts.energy() )
//  	       photon_energy = atts.energy();

//  	    if ( photon_energy > 0.100 )
//  	       num_significant_showers++;
//  	 }
//        }

   float photon_energy = 0.;
   int num_significant_showers = 1;

   if ( pos_valid  &&  neg_valid  &&  num_significant_showers <= 1 )
   {
      HepVector3D mom_total = momp + momn;

      float eventvars[ 28 ] =
      { pos_trackvars[ 0], neg_trackvars[ 0], pos_trackvars[ 1], neg_trackvars[ 1],
	pos_trackvars[ 2], neg_trackvars[ 2], pos_trackvars[ 3], neg_trackvars[ 3],
	pos_trackvars[ 4], neg_trackvars[ 4], pos_trackvars[ 5], neg_trackvars[ 5],
	pos_trackvars[ 6], neg_trackvars[ 6], pos_trackvars[ 7], neg_trackvars[ 7],
	pos_trackvars[ 8], neg_trackvars[ 8], mom_total.x(), mom_total.y(), mom_total.z(), 
	pos_trackvars[ 9], neg_trackvars[ 9], pos_trackvars[10], neg_trackvars[10],
	pos_trackvars[11], neg_trackvars[11], photon_energy };

      m_eventvars->fill( eventvars );
   }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
HighCotThetaTrackSplitting::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

ActionBase::ActionResult
HighCotThetaTrackSplitting::startRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in startRun()" << endl;

   FAItem< MagneticField > field;
   extract( iFrame.record( Stream::kStartRun ), field );

   m_bField = field->BField();

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
HighCotThetaTrackSplitting::endRun( Frame& iFrame )         // anal4 equiv.
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
