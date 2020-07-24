// -*- C++ -*-
//
// Package:     CakeAlignFlat
// Module:      CakeAlignFlat
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Tue May 14 11:24:02 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "CakeAlignFlat/CakeAlignFlat.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "ADRGeom/ADRSenseWireStore.h"
#include "Navigation/NavTrack.h"
#include "DualTrackHelices/DualTrackHelices.h"
#include "CleoDB/DBEventHeader.h"

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
static const char* const kFacilityString = "Processor.CakeAlignFlat" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.29 2002/03/19 15:07:23 cdj Exp $";
static const char* const kTagString = "$Name: v06_05_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
CakeAlignFlat::CakeAlignFlat( void )               // anal1
   : Processor( "CakeAlignFlat" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &CakeAlignFlat::event,    Stream::kEvent );
   //bind( &CakeAlignFlat::beginRun, Stream::kBeginRun );
   //bind( &CakeAlignFlat::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

}

CakeAlignFlat::~CakeAlignFlat()                    // anal5
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
CakeAlignFlat::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CakeAlignFlat::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
CakeAlignFlat::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_west_all_1 = iHistoManager.histogram(
      101, "west all resid ring 1", 100, -1.*k_mm, 1.*k_mm );
   m_west_cosphi_1 = iHistoManager.histogram(
      102, "west cosphi*resid ring 1", 100, -1.*k_mm, 1.*k_mm );
   m_west_sinphi_1 = iHistoManager.histogram(
      103, "west -sinphi*resid ring 1", 100, -1.*k_mm, 1.*k_mm );
   m_east_all_1 = iHistoManager.histogram(
      104, "east all resid ring 1", 100, -1.*k_mm, 1.*k_mm );
   m_east_cosphi_1 = iHistoManager.histogram(
      105, "east cosphi*resid ring 1", 100, -1.*k_mm, 1.*k_mm );
   m_east_sinphi_1 = iHistoManager.histogram(
      106, "east -sinphi*resid ring 1", 100, -1.*k_mm, 1.*k_mm );

   m_west_all_2 = iHistoManager.histogram(
      201, "west all resid ring 2", 100, -1.*k_mm, 1.*k_mm );
   m_west_cosphi_2 = iHistoManager.histogram(
      202, "west cosphi*resid ring 2", 100, -1.*k_mm, 1.*k_mm );
   m_west_sinphi_2 = iHistoManager.histogram(
      203, "west -sinphi*resid ring 2", 100, -1.*k_mm, 1.*k_mm );
   m_east_all_2 = iHistoManager.histogram(
      204, "east all resid ring 2", 100, -1.*k_mm, 1.*k_mm );
   m_east_cosphi_2 = iHistoManager.histogram(
      205, "east cosphi*resid ring 2", 100, -1.*k_mm, 1.*k_mm );
   m_east_sinphi_2 = iHistoManager.histogram(
      206, "east -sinphi*resid ring 2", 100, -1.*k_mm, 1.*k_mm );

   m_west_all_3 = iHistoManager.histogram(
      301, "west all resid ring 3", 100, -1.*k_mm, 1.*k_mm );
   m_west_cosphi_3 = iHistoManager.histogram(
      302, "west cosphi*resid ring 3", 100, -1.*k_mm, 1.*k_mm );
   m_west_sinphi_3 = iHistoManager.histogram(
      303, "west -sinphi*resid ring 3", 100, -1.*k_mm, 1.*k_mm );
   m_east_all_3 = iHistoManager.histogram(
      304, "east all resid ring 3", 100, -1.*k_mm, 1.*k_mm );
   m_east_cosphi_3 = iHistoManager.histogram(
      305, "east cosphi*resid ring 3", 100, -1.*k_mm, 1.*k_mm );
   m_east_sinphi_3 = iHistoManager.histogram(
      306, "east -sinphi*resid ring 3", 100, -1.*k_mm, 1.*k_mm );

   m_west_all_4 = iHistoManager.histogram(
      401, "west all resid ring 4", 100, -1.*k_mm, 1.*k_mm );
   m_west_cosphi_4 = iHistoManager.histogram(
      402, "west cosphi*resid ring 4", 100, -1.*k_mm, 1.*k_mm );
   m_west_sinphi_4 = iHistoManager.histogram(
      403, "west -sinphi*resid ring 4", 100, -1.*k_mm, 1.*k_mm );
   m_east_all_4 = iHistoManager.histogram(
      404, "east all resid ring 4", 100, -1.*k_mm, 1.*k_mm );
   m_east_cosphi_4 = iHistoManager.histogram(
      405, "east cosphi*resid ring 4", 100, -1.*k_mm, 1.*k_mm );
   m_east_sinphi_4 = iHistoManager.histogram(
      406, "east -sinphi*resid ring 4", 100, -1.*k_mm, 1.*k_mm );

   m_west_all_5 = iHistoManager.histogram(
      501, "west all resid ring 5", 100, -1.*k_mm, 1.*k_mm );
   m_west_cosphi_5 = iHistoManager.histogram(
      502, "west cosphi*resid ring 5", 100, -1.*k_mm, 1.*k_mm );
   m_west_sinphi_5 = iHistoManager.histogram(
      503, "west -sinphi*resid ring 5", 100, -1.*k_mm, 1.*k_mm );
   m_east_all_5 = iHistoManager.histogram(
      504, "east all resid ring 5", 100, -1.*k_mm, 1.*k_mm );
   m_east_cosphi_5 = iHistoManager.histogram(
      505, "east cosphi*resid ring 5", 100, -1.*k_mm, 1.*k_mm );
   m_east_sinphi_5 = iHistoManager.histogram(
      506, "east -sinphi*resid ring 5", 100, -1.*k_mm, 1.*k_mm );

   m_west_all_6 = iHistoManager.histogram(
      601, "west all resid ring 6", 100, -1.*k_mm, 1.*k_mm );
   m_west_cosphi_6 = iHistoManager.histogram(
      602, "west cosphi*resid ring 6", 100, -1.*k_mm, 1.*k_mm );
   m_west_sinphi_6 = iHistoManager.histogram(
      603, "west -sinphi*resid ring 6", 100, -1.*k_mm, 1.*k_mm );
   m_east_all_6 = iHistoManager.histogram(
      604, "east all resid ring 6", 100, -1.*k_mm, 1.*k_mm );
   m_east_cosphi_6 = iHistoManager.histogram(
      605, "east cosphi*resid ring 6", 100, -1.*k_mm, 1.*k_mm );
   m_east_sinphi_6 = iHistoManager.histogram(
      606, "east -sinphi*resid ring 6", 100, -1.*k_mm, 1.*k_mm );

   m_west_all_7 = iHistoManager.histogram(
      701, "west all resid ring 7", 100, -1.*k_mm, 1.*k_mm );
   m_west_cosphi_7 = iHistoManager.histogram(
      702, "west cosphi*resid ring 7", 100, -1.*k_mm, 1.*k_mm );
   m_west_sinphi_7 = iHistoManager.histogram(
      703, "west -sinphi*resid ring 7", 100, -1.*k_mm, 1.*k_mm );
   m_east_all_7 = iHistoManager.histogram(
      704, "east all resid ring 7", 100, -1.*k_mm, 1.*k_mm );
   m_east_cosphi_7 = iHistoManager.histogram(
      705, "east cosphi*resid ring 7", 100, -1.*k_mm, 1.*k_mm );
   m_east_sinphi_7 = iHistoManager.histogram(
      706, "east -sinphi*resid ring 7", 100, -1.*k_mm, 1.*k_mm );

   m_west_all_8 = iHistoManager.histogram(
      801, "west all resid ring 8", 100, -1.*k_mm, 1.*k_mm );
   m_west_cosphi_8 = iHistoManager.histogram(
      802, "west cosphi*resid ring 8", 100, -1.*k_mm, 1.*k_mm );
   m_west_sinphi_8 = iHistoManager.histogram(
      803, "west -sinphi*resid ring 8", 100, -1.*k_mm, 1.*k_mm );
   m_east_all_8 = iHistoManager.histogram(
      804, "east all resid ring 8", 100, -1.*k_mm, 1.*k_mm );
   m_east_cosphi_8 = iHistoManager.histogram(
      805, "east cosphi*resid ring 8", 100, -1.*k_mm, 1.*k_mm );
   m_east_sinphi_8 = iHistoManager.histogram(
      806, "east -sinphi*resid ring 8", 100, -1.*k_mm, 1.*k_mm );

   m_momentum_vs_phi0_pos = iHistoManager.profile(
      900, "track momentum VS phi0 pos",
      79, 0., 6.283185307179586, 5.29*k_GeV - 0.1*k_GeV, 5.29*k_GeV + 0.1*k_GeV, 
      HIHistProf::kErrorOnMean );

   m_momentum_vs_phi0_pos_subtracted = iHistoManager.profile(
      910, "track momentum VS phi0 pos (xing-angle subtracted)",
      79, 0., 6.283185307179586, 5.29*k_GeV - 0.1*k_GeV, 5.29*k_GeV + 0.1*k_GeV, 
      HIHistProf::kErrorOnMean );

   m_momentum_vs_phi0_neg = iHistoManager.profile(
      905, "track momentum VS phi0 neg",
      79, 0., 6.283185307179586, 5.29*k_GeV - 0.1*k_GeV, 5.29*k_GeV + 0.1*k_GeV, 
      HIHistProf::kErrorOnMean );

   m_momentum_vs_phi0_neg_subtracted = iHistoManager.profile(
      915, "track momentum VS phi0 neg (xing-angle subtracted)",
      79, 0., 6.283185307179586, 5.29*k_GeV - 0.1*k_GeV, 5.29*k_GeV + 0.1*k_GeV, 
      HIHistProf::kErrorOnMean );

   m_poscurv_poscotTheta = iHistoManager.histogram(
      1001, "cotTheta for pos tracks", kFlatGranularity, 0.80, 1.265 );

   m_poscurv_negcotTheta = iHistoManager.histogram(
      1002, "cotTheta for pos tracks", kFlatGranularity, -1.265, -0.80 );

   m_negcurv_poscotTheta = iHistoManager.histogram(
      1003, "cotTheta for neg tracks", kFlatGranularity, 0.80, 1.265 );

   m_negcurv_negcotTheta = iHistoManager.histogram(
      1004, "cotTheta for neg tracks", kFlatGranularity, -1.265, -0.80 );

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
CakeAlignFlat::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );
//     cout << header->run() << " " << header->number() << " " << header->time() << endl;

   FATable< TRSeedTrack > seedtracks;
   extract( iFrame.record( Stream::kEvent ), seedtracks );

   if ( seedtracks.size() != 2 ) return ActionBase::kFailed;

   for ( FATable< TRSeedTrack >::const_iterator seed_iter = seedtracks.begin();
	 seed_iter != seedtracks.end();
	 seed_iter++ ) {

      double cotTheta( seed_iter->cotTheta() );
      DABoolean ctPos( cotTheta > 0. );
      DABoolean cvPos( seed_iter->curvature() > 0. );

      if ( 0.80 < cotTheta  &&  cotTheta < 1.265 ) ctPos = true;
      else if ( -1.265 < cotTheta  &&  cotTheta < -0.80 ) ctPos = false;
      else return ActionBase::kFailed;

//        cout << endl;
//        cout << "cotTheta = " << cotTheta
//  	   << ", ctPos = " << ctPos
//  	   << ", cvPos = " << cvPos << endl;

      if ( cvPos  &&  ! ctPos ) {
//  	 cout << "Simply filling poscurv_negcotTheta." << endl;
	 m_poscurv_negcotTheta->fill( cotTheta );
      }
      if ( cvPos  &&  ctPos ) {
	 int channel( ceil( kFlatGranularity * ( cotTheta - 0.80 ) / ( 1.265 - 0.80 ) ) );

//  	 cout << "poscurv_poscotTheta->channelValue( " << channel << " ) = "
//  	      << m_poscurv_poscotTheta->channelValue( channel ) << ", "
//  	      << "poscurv_negcotTheta->channelValue( " << kFlatGranularity - channel + 1 << " ) = "
//  	      << m_poscurv_negcotTheta->channelValue( kFlatGranularity - channel + 1 ) << endl;

	 if ( m_poscurv_poscotTheta->channelValue( channel )
	      < m_poscurv_negcotTheta->channelValue( kFlatGranularity - channel + 1 ) ) {
//  	    cout << "So I am filling poscurv_poscotTheta" << endl;
	    m_poscurv_poscotTheta->fill( cotTheta );
	 }
	 else {
//  	    cout << "So I skip this event." << endl;
	    return ActionBase::kFailed;
	 }
      }
      if ( ! cvPos  &&  ctPos ) {
//  	 cout << "Simply filling negcurv_poscotTheta." << endl;
	 m_negcurv_poscotTheta->fill( cotTheta );
      }
      if ( ! cvPos  &&  ! ctPos ) {
	 int channel( ceil( kFlatGranularity * ( cotTheta - -1.265 ) / ( -0.80 - -1.265 ) ) );

//  	 cout << "negcurv_negcotTheta->channelValue( " << channel << " ) = "
//  	      << m_negcurv_negcotTheta->channelValue( channel ) << ", "
//  	      << "negcurv_poscotTheta->channelValue( " << kFlatGranularity - channel + 1 << " ) = "
//  	      << m_negcurv_poscotTheta->channelValue( kFlatGranularity - channel + 1 ) << endl;

	 if ( m_negcurv_negcotTheta->channelValue( channel )
	      < m_negcurv_poscotTheta->channelValue( kFlatGranularity - channel + 1 ) ) {
//  	    cout << "So I am filling negcurv_negcotTheta" << endl;
	    m_negcurv_negcotTheta->fill( cotTheta );
	 }
	 else {
//  	    cout << "So I skip this event." << endl;
	    return ActionBase::kFailed;
	 }
      }
   } // end loop over seedtracks

   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator nav_iter;
   FATable< NavTrack >::const_iterator nav_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator nav_end = navtracks.end();
   
   // be sure that we have exactly two good tracks
   if ( navtracks.size() != 2 ) return ActionBase::kFailed;
   if ( ( nav_iter = nav_begin )->muonQuality()->fitAbort() )
      return ActionBase::kFailed;
//     if ( nav_iter->seedSVRHits()->size() < 1 ) return ActionBase::kFailed;
   if ( ( ++nav_iter )->muonQuality()->fitAbort() )
      return ActionBase::kFailed;
//     if ( nav_iter->seedSVRHits()->size() < 1 ) return ActionBase::kFailed;
   
   // with no final-state radiation (well, very little... this is
   // one of those infrared divergences, isn't it?
   HepVector3D event_momentum =
      ( ( nav_iter = nav_begin )->muonFit()->momentum() +
	( ++nav_iter )->muonFit()->momentum() );
   double xdiff2 = sqr( event_momentum.x() + 0.0261 );
   double ydiff2 = sqr( event_momentum.y() + 0.0003 );
   if ( ( xdiff2 + ydiff2 ) > 0.300 ) return ActionBase::kFailed;
//     if ( abs( ( nav_iter = nav_begin )->muonHelix()->curvature() +
//  	     ( ++nav_iter )->muonHelix()->curvature()             )
//  	> 4E-3 ) return ActionBase::kFailed;
   
   // This is getting a little ridiculous...
   if ( ( ( nav_iter = nav_begin )->muonHelix()->curvature() ) *
	( ( ++ nav_iter )->muonHelix()->curvature() ) > 0. )
      return ActionBase::kFailed;

   FAItem< ADRSenseWireStore > adrWireStore;
   extract( iFrame.record( Stream::kBaseGeometry ), adrWireStore );
   if ( ! adrWireStore.valid() ) {
      report( EMERGENCY, kFacilityString )
	 << "ADRSenseWireStore is not valid!" << endl;
      return ActionBase::kFailed;
   }

   report( DEBUG, kFacilityString ) << "this is a good event." << endl;
   
   FATable< DualTrackHelices > dualtracks;
   extract( iFrame.record( Stream::kEvent ), dualtracks );
   FATable< DualTrackHelices >::const_iterator dualtracks_iter;

   for ( nav_iter = nav_begin;  nav_iter != nav_end;  nav_iter++ )
   {
      dualtracks_iter = dualtracks.find( nav_iter->identifier() );
      double cotTheta = dualtracks_iter->helix().cotTheta();
      HIHelix local_helix( dualtracks_iter->helix() );

      DABoolean west;
      if ( 0.80 < cotTheta  &&  cotTheta < 1.265 ) west = true;
      else if ( -1.265 < cotTheta  &&  cotTheta < -0.80 ) west = false;
      else return ActionBase::kFailed;

      if ( local_helix.curvature() > 0. ) {
	 m_momentum_vs_phi0_pos->fill(
	    nav_iter->muonHelix()->phi0(),
	    nav_iter->muonFit()->momentum().mag() );
	 m_momentum_vs_phi0_pos_subtracted->fill(
	    nav_iter->muonHelix()->phi0(),
	    ( nav_iter->muonFit()->momentum() + HepVector3D( 0.013, 0., 0. ) ).mag() );
      }
      else {
	 m_momentum_vs_phi0_neg->fill(
	    nav_iter->muonHelix()->phi0(),
	    nav_iter->muonFit()->momentum().mag() );
	 m_momentum_vs_phi0_neg_subtracted->fill(
	    nav_iter->muonHelix()->phi0(),
	    ( nav_iter->muonFit()->momentum() + HepVector3D( 0.013, 0., 0. ) ).mag() );
      }

      const vector< const CalibratedDRHit* >* p_trackDRHits =
	 nav_iter->muonDRHits();
      if ( p_trackDRHits == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "muonDRHits is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }
      vector< const CalibratedDRHit* > trackDRHits = (* p_trackDRHits );

      vector< const CalibratedDRHit* >::const_iterator trackDRHits_iterator;
      vector< const CalibratedDRHit* >::const_iterator trackDRHits_begin =
	 trackDRHits.begin();
      vector< const CalibratedDRHit* >::const_iterator trackDRHits_end =
	 trackDRHits.end();

      report( DEBUG, kFacilityString )
	 << "extracted hit objects" << endl;

      for( trackDRHits_iterator = trackDRHits_begin;
	   trackDRHits_iterator != trackDRHits_end;
	   trackDRHits_iterator++ ) {
	 // get the link between this track and this hit, and its linkdata
	 report( DEBUG, kFacilityString )
	    << "Trying to get hit link..." << endl;
	 const NavTrack::DRHitLinkTable* trackDRLinks =
	    nav_iter->muonDRHitLinks();
	 if ( trackDRLinks == NULL ) {
	    report( EMERGENCY, kFacilityString )
	       << "NavTrack is missing the muon hit links!" << endl;
	    return ActionBase::kFailed;
	 }
	 NavTrack::DRHitLinkTable::const_iterator trackDRLinks_iterator;
	 NavTrack::DRHitLinkTable::const_iterator trackDRLinks_begin =
	    trackDRLinks->begin();
	 NavTrack::DRHitLinkTable::const_iterator trackDRLinks_end =
	    trackDRLinks->end();
	    
	 for( trackDRLinks_iterator = trackDRLinks_begin;
	      trackDRLinks_iterator != trackDRLinks_end
		 &&  (     (* trackDRLinks_iterator )->rightID() == 0
			   ||  *(* trackDRLinks_iterator )->rightID() !=
			   ( *trackDRHits_iterator )->identifier()
		    );
	      trackDRLinks_iterator++ );
	 
	 if ( trackDRLinks_iterator == trackDRLinks_end       ||
	      (* trackDRLinks_iterator )->rightID() == 0 ) {
	    report( EMERGENCY, kFacilityString )
	       << "Couldn't get the link between this track ("
	       << nav_iter->identifier() << ") "
	       << "and this hit ( "
	       << (* trackDRHits_iterator )->identifier()
	       << "), even avoiding Lattice." << endl;
	    return ActionBase::kFailed;
	 }
	 const TrackFitDRHitLink& trackDRHitLink =
	    (* trackDRLinks_iterator )->linkData();

	 if ( trackDRHitLink.used() )
	 {
	    int layer = (* trackDRHits_iterator )->layer();
	    double residual = trackDRHitLink.residual();

	    double layerRadius = adrWireStore->radius( layer );
	    double z = local_helix.moveToRadius( layerRadius );
	    int wire_number = (* trackDRHits_iterator )->wireInLyr();
	    double phi = adrWireStore->wire( layer, wire_number ).zPoint( z ).first.phi();

	    double sinphi = sin(phi);
	    double cosphi = cos(phi);

	    switch ( layer ) {
	       case 1: case 2:
		  if ( west ) {
		     m_west_all_1->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_west_cosphi_1->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_west_sinphi_1->fill( 0. - sinphi * residual );
		  } else {
		     m_east_all_1->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_east_cosphi_1->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_east_sinphi_1->fill( 0. - sinphi * residual );
		  }

	       case 3: case 4:
		  if ( west ) {
		     m_west_all_2->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_west_cosphi_2->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_west_sinphi_2->fill( 0. - sinphi * residual );
		  } else {
		     m_east_all_2->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_east_cosphi_2->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_east_sinphi_2->fill( 0. - sinphi * residual );
		  }

	       case 5: case 6:
		  if ( west ) {
		     m_west_all_3->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_west_cosphi_3->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_west_sinphi_3->fill( 0. - sinphi * residual );
		  } else {
		     m_east_all_3->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_east_cosphi_3->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_east_sinphi_3->fill( 0. - sinphi * residual );
		  }

	       case 7: case 8:
		  if ( west ) {
		     m_west_all_4->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_west_cosphi_4->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_west_sinphi_4->fill( 0. - sinphi * residual );
		  } else {
		     m_east_all_4->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_east_cosphi_4->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_east_sinphi_4->fill( 0. - sinphi * residual );
		  }

	       case 9: case 10:
		  if ( west ) {
		     m_west_all_5->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_west_cosphi_5->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_west_sinphi_5->fill( 0. - sinphi * residual );
		  } else {
		     m_east_all_5->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_east_cosphi_5->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_east_sinphi_5->fill( 0. - sinphi * residual );
		  }

	       case 11: case 12:
		  if ( west ) {
		     m_west_all_6->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_west_cosphi_6->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_west_sinphi_6->fill( 0. - sinphi * residual );
		  } else {
		     m_east_all_6->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_east_cosphi_6->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_east_sinphi_6->fill( 0. - sinphi * residual );
		  }

	       case 13: case 14:
		  if ( west ) {
		     m_west_all_7->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_west_cosphi_7->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_west_sinphi_7->fill( 0. - sinphi * residual );
		  } else {
		     m_east_all_7->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_east_cosphi_7->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_east_sinphi_7->fill( 0. - sinphi * residual );
		  }

	       case 15: case 16:
		  if ( west ) {
		     m_west_all_8->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_west_cosphi_8->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_west_sinphi_8->fill( 0. - sinphi * residual );
		  } else {
		     m_east_all_8->fill( residual );
		     if ( abs( sinphi ) < 0.4 )
			m_east_cosphi_8->fill( cosphi * residual );
		     if ( abs( cosphi ) < 0.4 )
			m_east_sinphi_8->fill( 0. - sinphi * residual );
		  }
	    } // end switch layer
	    report( DEBUG, kFacilityString ) << "after filling histograms." << endl;

	 } // end if link used
	 report( DEBUG, kFacilityString ) << "after hit cut." << endl;

      } // end loop over hits
      report( DEBUG, kFacilityString ) << "finished all loops over hits." << endl;
   } // end loop over tracks

   report( DEBUG, kFacilityString ) << "well, I'm done." << endl;

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
CakeAlignFlat::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
CakeAlignFlat::endRun( Frame& iFrame )         // anal4 equiv.
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
