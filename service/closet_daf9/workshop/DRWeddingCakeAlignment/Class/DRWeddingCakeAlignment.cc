// -*- C++ -*-
//
// Package:     <DRWeddingCakeAlignment>
// Module:      DRWeddingCakeAlignment
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Tue Jul 17 10:31:50 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "Experiment/report.h"
#include "DRWeddingCakeAlignment/DRWeddingCakeAlignment.h"
#include "DataHandler/Stream.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "DataHandler/Frame.h"
#include "DAException/DAException.h"

#include "Navigation/NavTrack.h"
#include "/home/mccann/workshop/DualTrackHelices/DualTrackHelices/DualTrackHelices.h"
#include "KinematicTrajectory/KTKinematicData.h"
#include "JobControl/JobControl.h"
#include "ToolBox/HistogramPackage.h"
#include "CleoDB/DBEventHeader.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "HelixIntersection/HIDRSurfaceFactory.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "CleoDB/DBCandidate.h"
#include "CalibratedData/CalibratedDRHit.h"

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
static const char* const kFacilityString = "DRWeddingCakeAlignment.DRWeddingCakeAlignment" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule.cc,v 1.2 2000/12/04 19:11:05 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DRWeddingCakeAlignment::DRWeddingCakeAlignment()
   : FrameIteratorModuleBase( "DRWeddingCakeAlignment",
			      "For aligning the wedding cake with MINUIT" )
   , m_cake( "cake", this, 1 )
   , m_minhits( "minhits", this, 10000 )
   , m_inputConst( "inputConst", this, "unaligned.drgeomalignment" )
   , m_geomalignment( new DBDRGeomAlignment[18], 18 )
   , m_geomalignmentHolder( &m_geomalignment )
{
   // Lauren does this two times also
   ifstream starting_alignment( m_inputConst.value().c_str() );
   if ( ! m_geomalignment.readFromStream( starting_alignment ) )
   {
      report( EMERGENCY, kFacilityString )
	 << "Could not load input constants! (from file \""
	 << m_inputConst.value().c_str() << "\")" << endl;
   }

   registerProxyFactory( Stream::kDRAlignment,
			 m_geomalignmentHolder.makeFactory(),
			 UsageTag() );
   //You must set what streams you which to iterate over
   //  that is, what events should the Frames be stopped on
//   iterateOver( Stream::kBeginRun );
   iterateOver( Stream::kEvent );
}

// DRWeddingCakeAlignment::DRWeddingCakeAlignment( const DRWeddingCakeAlignment& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DRWeddingCakeAlignment::~DRWeddingCakeAlignment()
{
}

//
// assignment operators
//
// const DRWeddingCakeAlignment& DRWeddingCakeAlignment::operator=( const DRWeddingCakeAlignment& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//
void
DRWeddingCakeAlignment::iterate( const FIFrameIterator& iBegin,
				 const FIFrameIterator& iEnd )
{
   report( DEBUG, kFacilityString )
      << "enter DRWeddingCakeAlignment::iterate " << endl;

   // Lauren does this two times also
   ifstream starting_alignment( m_inputConst.value().c_str() );
   if ( ! m_geomalignment.readFromStream( starting_alignment ) )
   {
      report( EMERGENCY, kFacilityString )
	 << "Could not load input constants! (from file \""
	 << m_inputConst.value().c_str() << "\")" << endl;
      return;
   }

   JobControl* jc = JobControl::instance();
   HIHistoManager* hm = jc->histogramManagerP();

   unsigned int i_west = 1 + m_cake.value();
   unsigned int i_east = 9 + m_cake.value();

   m_start_west_deltaX = m_geomalignment[ i_west ].get_deltaX();
   m_start_west_deltaY = m_geomalignment[ i_west ].get_deltaY();
   m_start_west_rotZ   = m_geomalignment[ i_west ].get_rotZ();
   m_start_east_deltaX = m_geomalignment[ i_east ].get_deltaX();
   m_start_east_deltaY = m_geomalignment[ i_east ].get_deltaY();
   m_start_east_rotZ   = m_geomalignment[ i_east ].get_rotZ();

   report( NOTICE, kFacilityString )
      << "Aligning cake " << m_cake.value()
      << " with at lwest " << m_minhits.value() << " hits..." << endl;

   residualAlignment resAlign( &iBegin, &iEnd, m_cake.value(), m_minhits.value(),
			       &m_geomalignment, hm );

   MinuitInterface* mi = MinuitInterface::instance();
   mi->loadFcn( resAlign );
   mi->setDiagLevel( MinuitInterface::kMax );
//     mi->fixParameter( residualAlignment::kWestDeltaX );
//     mi->fixParameter( residualAlignment::kWestDeltaY );
//     mi->fixParameter( residualAlignment::kWestRotZ );
//     mi->fixParameter( residualAlignment::kEastDeltaX );
//     mi->fixParameter( residualAlignment::kEastDeltaY );
//     mi->fixParameter( residualAlignment::kEastRotZ );
   mi->runMigrad();

   print_change( report( NOTICE, kFacilityString ), mi );

   char outfilename[60];
   sprintf( outfilename, "results_cake%1d.txt",
	    m_cake.value() );
   ofstream outfile( outfilename );
   if ( outfile )
      print_change( outfile, mi );

   double chi2 = resAlign.last_time();

   print_change( report( NOTICE, kFacilityString ), mi );
   report( NOTICE, kFacilityString )
      << "    with a chi^2 of " << chi2 << endl;
   if ( outfile )
      outfile << "    with a chi^2 of " << chi2 << endl;

   mi->runMinos();

   print_fancy_errors( report( NOTICE, kFacilityString ), mi );
   if ( outfile )
      print_fancy_errors( outfile, mi );
}

void DRWeddingCakeAlignment::print_change( ostream &os, MinuitInterface* mi )
{
   MIParameter west_deltaX = mi->parameter( residualAlignment::kWestDeltaX );
   MIParameter west_deltaY = mi->parameter( residualAlignment::kWestDeltaY );
   MIParameter west_rotZ = mi->parameter( residualAlignment::kWestRotZ );
   MIParameter east_deltaX = mi->parameter( residualAlignment::kEastDeltaX );
   MIParameter east_deltaY = mi->parameter( residualAlignment::kEastDeltaY );
   MIParameter east_rotZ = mi->parameter( residualAlignment::kEastRotZ );

   os.precision( 15 );

   os << "Cake " << m_cake.value() << " final results:" << endl;

   os << "    west deltaX = " << west_deltaX.value()
      << " +/- " << west_deltaX.parabolicErr() << endl;
   os << "      (started at " << m_start_west_deltaX
      << ", significance of change is "
      << ( ( west_deltaX.value() - m_start_west_deltaX ) / west_deltaX.parabolicErr() )
      << " standard deviations." << endl;

   os << "    west deltaY = " << west_deltaY.value()
      << " +/- " << west_deltaY.parabolicErr() << endl;
   os << "      (started at " << m_start_west_deltaY
      << ", significance of change is "
      << ( ( west_deltaY.value() - m_start_west_deltaY ) / west_deltaY.parabolicErr() )
      << " standard deviations." << endl;

   os << "    west rotZ = " << west_rotZ.value()
      << " +/- " << west_rotZ.parabolicErr() << endl;
   os << "      (started at " << m_start_west_rotZ
      << ", significance of change is "
      << ( ( west_rotZ.value() - m_start_west_rotZ ) / west_rotZ.parabolicErr() )
      << " standard deviations." << endl;

   os << "    east deltaX = " << east_deltaX.value()
      << " +/- " << east_deltaX.parabolicErr() << endl;
   os << "      (started at " << m_start_east_deltaX
      << ", significance of change is "
      << ( ( east_deltaX.value() - m_start_east_deltaX ) / east_deltaX.parabolicErr() )
      << " standard deviations." << endl;

   os << "    east deltaY = " << east_deltaY.value()
      << " +/- " << east_deltaY.parabolicErr() << endl;
   os << "      (started at " << m_start_east_deltaY
      << ", significance of change is "
      << ( ( east_deltaY.value() - m_start_east_deltaY ) / east_deltaY.parabolicErr() )
      << " standard deviations." << endl;

   os << "    east rotZ = " << east_rotZ.value()
      << " +/- " << east_rotZ.parabolicErr() << endl;
   os << "      (started at " << m_start_east_rotZ
      << ", significance of change is "
      << ( ( east_rotZ.value() - m_start_east_rotZ ) / east_rotZ.parabolicErr() )
      << " standard deviations." << endl;

   os << "FOR_CONST_FILE: " << ( m_cake.value() + 2 ) << " "
      << west_deltaX.value() << " " << west_deltaY.value()
      << " 0 0 0 " << west_rotZ.value() << endl;
   os << "FOR_CONST_FILE: " << ( m_cake.value() + 10 ) << " "
      << east_deltaX.value() << " " << east_deltaY.value()
      << " 0 0 0 " << east_rotZ.value() << endl;
}

void DRWeddingCakeAlignment::print_fancy_errors( ostream &os, MinuitInterface* mi )
{
   MIParameter west_deltaX = mi->parameter( residualAlignment::kWestDeltaX );
   MIParameter west_deltaY = mi->parameter( residualAlignment::kWestDeltaY );
   MIParameter west_rotZ = mi->parameter( residualAlignment::kWestRotZ );
   MIParameter east_deltaX = mi->parameter( residualAlignment::kEastDeltaX );
   MIParameter east_deltaY = mi->parameter( residualAlignment::kEastDeltaY );
   MIParameter east_rotZ = mi->parameter( residualAlignment::kEastRotZ );

   os.precision( 15 );

   os << "Cake " << m_cake.value() << " final, final results:" << endl;

   os << "    west deltaX = " << west_deltaX.value()
      << " + " << west_deltaX.posErr() << " or - " << west_deltaX.negErr() << endl;
   os << "    west deltaY = " << west_deltaY.value()
      << " + " << west_deltaY.posErr() << " or - " << west_deltaY.negErr() << endl;
   os << "    west rotZ = " << west_rotZ.value()
      << " + " << west_rotZ.posErr() << " or - " << west_rotZ.negErr() << endl;
   os << "    east deltaX = " << east_deltaX.value()
      << " + " << east_deltaX.posErr() << " or - " << east_deltaX.negErr() << endl;
   os << "    east deltaY = " << east_deltaY.value()
      << " + " << east_deltaY.posErr() << " or - " << east_deltaY.negErr() << endl;
   os << "    east rotZ = " << east_rotZ.value()
      << " + " << east_rotZ.posErr() << " or - " << east_rotZ.negErr() << endl;
}

//
// static member functions
//

//
// nested classes
//

DRWeddingCakeAlignment::histogramBundle::histogramBundle(
   HIHistoManager* hm, unsigned int iteration )
{
   dca = hm->histogram(
      ( iteration * 100 + 1 ), "DCA from track to wire", 100, -0.01, 0.01 );
   e_dca = hm->histogram(
      ( iteration * 100 + 2 ), "error in DCA", 100, 0., 0.001 );
   drift = hm->histogram(
      ( iteration * 100 + 3 ), "measured drift distance", 100, -0.01, 0.01 );
   e_drift = hm->histogram(
      ( iteration * 100 + 4 ), "error in drift", 100, 0., 0.001 );
   residual = hm->histogram(
      ( iteration * 100 + 5 ), "residual", 100, -0.002, 0.002 );
   e_residual = hm->histogram(
      ( iteration * 100 + 6 ), "error in residual", 100, 0., 0.001 );
   norm_resid = hm->histogram(
      ( iteration * 100 + 7 ), "normalized residual", 100, -4., 4. );

//     resid_v_layer = hm->histogram(
//        ( iteration * 100 + 7 ), "residual VS layer", 16, 0.5, 16.5, 100, -0.005, 0.005 );
}

DRWeddingCakeAlignment::residualAlignment::residualAlignment(
   const FIFrameIterator* begin, const FIFrameIterator* end,
   unsigned int cake, unsigned int minhits,
   CLEOConstantsModifiable< DBDRGeomAlignment >* geomConst,
   HIHistoManager* hm ) :
   mm_begin( begin ), mm_end( end ), mm_cake( cake ), mm_minhits( minhits ),
   mm_geomConst( geomConst ),
   mm_draw_last_histograms( false ),
   mm_draw_any_histograms( true )
{
   unsigned int i_west = 1 + mm_cake;
   unsigned int i_east = 9 + mm_cake;

   // Must be in the order given by the enum in residualAlignment class declaration
   addInitialParameter( "west X",    (* mm_geomConst)[ i_west ].get_deltaX(), 0.001 );
   addInitialParameter( "west Y",    (* mm_geomConst)[ i_west ].get_deltaY(), 0.001 );
   addInitialParameter( "west rotZ", (* mm_geomConst)[ i_west ].get_rotZ(),   0.001 );
   addInitialParameter( "east X",    (* mm_geomConst)[ i_east ].get_deltaX(), 0.001 );
   addInitialParameter( "east Y",    (* mm_geomConst)[ i_east ].get_deltaY(), 0.001 );
   addInitialParameter( "east rotZ", (* mm_geomConst)[ i_east ].get_rotZ(),   0.001 );

   for ( int i = 0;  i < khistBundles;  i++ )
   {
      DRWeddingCakeAlignment::histogramBundle* hb
	 = new DRWeddingCakeAlignment::histogramBundle( hm, ( i + 1 ) );
      mm_hists.push_back( hb );
   }
}

DRWeddingCakeAlignment::residualAlignment::~residualAlignment()
{
   for ( int i = 0;  i < mm_hists.size();  i++ )
      delete mm_hists[ i ];
}

double DRWeddingCakeAlignment::residualAlignment::iterate( double* values )
{
   static unsigned int iteration;
   iteration++;
   static DABoolean run_out_of_histograms;

   unsigned int i_west = 1 + mm_cake;
   unsigned int i_east = 9 + mm_cake;

   (* mm_geomConst)[ i_west ].set_deltaX( values[ kWestDeltaX ] );
   (* mm_geomConst)[ i_west ].set_deltaY( values[ kWestDeltaY ] );
   (* mm_geomConst)[ i_west ].set_rotZ( values[ kWestRotZ ] );
   (* mm_geomConst)[ i_east ].set_deltaX( values[ kEastDeltaX ] );
   (* mm_geomConst)[ i_east ].set_deltaY( values[ kEastDeltaY ] );
   (* mm_geomConst)[ i_east ].set_rotZ( values[ kEastRotZ ] );

   double sum_resid = 0.;
   double squaresum_resid = 0.;
   double sum_norm_resid = 0.;
   double squaresum_norm_resid = 0.;
   unsigned int num_hits = 0;
   unsigned int negative_errors = 0;

   report( NOTICE, kFacilityString )
      << "Iteration " << iteration << ": calculating cake "
      << mm_cake << " residuals with" << endl;
   report( NOTICE, kFacilityString )
      << "    west (X, Y, ROTZ) = ("
      << (* mm_geomConst)[ i_west ].get_deltaX() << ", "
      << (* mm_geomConst)[ i_west ].get_deltaY() << ", "
      << (* mm_geomConst)[ i_west ].get_rotZ() << ")" << endl;
   report( NOTICE, kFacilityString )
      << "    east (X, Y, ROTZ) = ("
      << (* mm_geomConst)[ i_east ].get_deltaX() << ", "
      << (* mm_geomConst)[ i_east ].get_deltaY() << ", "
      << (* mm_geomConst)[ i_east ].get_rotZ() << ")" << endl;

   // assignment of a FIFrameIterator calls resetToBeginning() which
   // clears the proxies and forces the ADRSenseWireStore object to be
   // recalculated using the new constants
//     unsigned int event_checksum = 0;
   for( FIFrameIterator itFrame = (* mm_begin);
	( num_hits < mm_minhits )  &&  ( itFrame != (* mm_end) );
	++itFrame )
   {
//        if ( num_hits % 10000 < 5 )
//  	 report( NOTICE, kFacilityString )
//  	    << "I've seen " << num_hits << " hits so far..." << endl;

      FAItem< DBEventHeader > header;
      extract( itFrame->record( Stream::kEvent ), header );
      unsigned int run = header->run();
      unsigned int event = header->number();
//        try {
//  	 event_checksum += event;
//        }
//        catch( overflow_error& thisException )
//        {
//  	 report( INFO, kFacilityString )
//  	    << "checksum rolled over: " << event_checksum << endl;
//        }

      // get a new geometry store using the new alignment constants
      FAItem< ADRSenseWireStore > aStore;
      extract( itFrame->record( Stream::kDRAlignment ), aStore );

      // a surface factory turns HitAndDriftDistance objects into surfaces
      HIDRSurfaceFactory drFactory( (* itFrame), true, true, true, true, &(* aStore) );
      STL_VECTOR( HIIntersectionSurface* ) surfaces;
      STL_VECTOR( HIDRSurfaceFactory::DRHitAndDriftDistance ) drhitinfos;

      // loop over tracks
      FATable< NavTrack > navtracks;
      extract( itFrame->record( Stream::kEvent ), navtracks );
      if ( navtracks.size() != 2 )
	 continue;
      FATable< NavTrack >::const_iterator navtracks_iter;
      FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
      FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();

      FATable< DualTrackHelices > dualtracks;
      extract( itFrame->record( Stream::kEvent ), dualtracks );
      FATable< DualTrackHelices >::const_iterator dualtracks_iter;
      FATable< DualTrackHelices >::const_iterator dualtracks_begin = dualtracks.begin();
      FATable< DualTrackHelices >::const_iterator dualtracks_end = dualtracks.end();

      double phi01, phi02, d01, d02, z01, z02;
      double chi2ndf, conf;
      int which = 1;

//        for ( navtracks_iter = navtracks_begin;
//   	    navtracks_iter != navtracks_end;
//   	    navtracks_iter++ )
      for ( navtracks_iter = navtracks_begin,    dualtracks_iter = dualtracks_begin;
	    navtracks_iter != navtracks_end  &&  dualtracks_iter != dualtracks_end;
	    navtracks_iter++, dualtracks_iter++ )
      {
	 // we don't want to use bad fits
	 if ( navtracks_iter->muonQuality()->fitAbort() )
	    continue;

//  	 // This is a single helix, without dual track constraints
//  	 HIHelix helix( (* navtracks_iter->muonHelix() ),
//  			DBCandidate::mass( DBCandidate::Hypo( DBCandidate::kMuon ) ) );

	 // This is a dual-track constrained helix
	 HIHelix helix( dualtracks_iter->helix() );

	 if ( which == 1 )
	 {
	    phi01 = helix.phi0();
	    d01 = helix.d0();
	    z01 = helix.z0();
	    chi2ndf = dualtracks_iter->chisq() / double( dualtracks_iter->ndof() );
	    conf = prob_( dualtracks_iter->chisq(), dualtracks_iter->ndof() );
	    which++;
	 }
	 else
	 {
	    phi02 = helix.phi0();
	    d02 = helix.d0();
	    z02 = helix.z0();
	 }

	 // this hard cut on confidence level cuts out final state radiation
	 // (the confidence level distribution is basically flat after 11%)
	 if ( prob_( dualtracks_iter->chisq(), dualtracks_iter->ndof() ) < 0.11  ||
	      prob_( dualtracks_iter->chisq(), dualtracks_iter->ndof() ) > 1. )
	    continue;
	 
//  	 FATable< CalibratedDRHit > alldrhits;
//  	 extract( itFrame->record( Stream::kEvent ), alldrhits );

	 // loop over hits
	 const NavTrack::DRHitTable* hittable = navtracks_iter->seedDRHits();
	 NavTrack::DRHitTable::const_iterator hit_iter;
	 NavTrack::DRHitTable::const_iterator hit_begin = hittable->begin();
	 NavTrack::DRHitTable::const_iterator hit_end = hittable->end();
	 for ( hit_iter = hit_begin;
	       hit_iter != hit_end;
	       hit_iter++ )
	 {
	    const CalibratedDRHit* drhit = (* hit_iter);
	    unsigned int which_cake = ( drhit->layer() + 1 ) / 2;
	    // get it from the right cake *and* select good hits
	    // before computing surfaces
	    if ( which_cake == mm_cake )
	    {
	       if ( abs( drhit->distance() ) < 0.0035 )
	       {
		  drhitinfos.push_back(
		     HIDRSurfaceFactory::DRHitAndDriftDistance(
			drhit, drhit->distance() ) );
		  report( DEBUG, kFacilityString )
		     << "Looking at hit on layer " << drhit->layer() << endl;
	       } // end if this is a good hit
	    } // end if this is a hit on our cake
	 } // end loop over hits

	 drFactory.generateMultiWireSurfaces( surfaces, drhitinfos, NULL );
	 if ( surfaces.empty() )
	    continue;

	 HIHelixIntersector intersector(
	    surfaces, HIHelixIntersector::kIncreasingRadius, &helix );
	 HIHelixIntersector::IntersectionStatus status =
	    intersector.swimToCurrentSurface( KTMoveControl::kDirectionForward );
	 do {
	    HIIntersectionSurface* surface = intersector.currentSurface();
	    do {
	       Meters dca = surface->dca( helix );
	       Meters drift = surface->measuredDca();
	       if ( dca < 0. ) drift *= -1.;
	       Meters residual = dca - drift;

	       if ( dca == 0.  ||  drift == 0. )
		  continue;

	       if ( abs( dca - drift ) > 0.001 )
		  continue;

	       double e_drift2 = 1. / surface->fittingWeight();

	       HepVector derivs( HIHelix::kNTrackParameters );
	       surface->derivatives( helix, derivs );
	       double e_dca2 = helix.errorMatrix().similarity( derivs );

	       if ( e_drift2 <= 0.  ||  e_dca2 <= 0. )
	       {
		  negative_errors++;
		  continue;
	       }

	       double e_residual2 = e_drift2 + e_dca2;

	       if ( sqr( residual ) / e_residual2 > 25. )
		  continue;

	       report( DEBUG, kFacilityString )
		  << "dca = " << dca << " +/- " << sqrt( e_dca2 )
		  << ", drift = " << drift << " +/- " << sqrt( e_drift2 )
		  << " ==> residual = " << residual << " +/- "
		  << sqrt( e_drift2 + e_dca2 ) << endl;

	       if ( mm_draw_any_histograms  &&
		    ( iteration <= khistBundles - 1  ||  mm_draw_last_histograms ) )
	       {
		  int index = iteration - 1;
		  if ( mm_draw_last_histograms  &&  index > khistBundles - 1 )
		     index = khistBundles - 1;

		  mm_hists[ index ]->dca->fill( dca );
		  mm_hists[ index ]->e_dca->fill( sqrt( e_dca2 ) );
		  mm_hists[ index ]->drift->fill( drift );
		  mm_hists[ index ]->e_drift->fill( sqrt( e_drift2 ) );
		  mm_hists[ index ]->residual->fill( residual );
		  mm_hists[ index ]->e_residual->fill( sqrt( e_residual2 ) );
		  mm_hists[ index ]->norm_resid->fill(
		     residual / sqrt( e_residual2 ) );

//  		     mm_hists[ index ]->resid_v_layer->fill(
//  			alldrhits.find( surface->calibratedHit()->identifier() )->layer(),
//  			residual );
	       }
	       else
	       {
		  if ( ! run_out_of_histograms )
		     report( WARNING, kFacilityString )
			<< "I've run out of pre-booked histograms to fill! "
			<< "No more plotting!" << endl;
		  run_out_of_histograms = true;
	       }
		  
	       sum_resid += residual;
	       squaresum_resid += sqr( residual );

	       sum_norm_resid += residual / sqrt( e_residual2 );
	       squaresum_norm_resid += sqr( residual ) / e_residual2;
	       num_hits++;

	    } while ( surface->advanceToNextCalibratedHit( helix ) );
	 } while ( intersector.swimToNextIntersection( KTMoveControl::kDirectionForward )
		   == HIHelixIntersector::kIntersectionOK );

	 drhitinfos.clear();
	 for ( int i = 0;  i < surfaces.size();  i++ )
	    delete surfaces[ i ];
	 surfaces.clear();
	 
      } // end loop over tracks
      
      report( DEBUG, kFacilityString )
 	 << "phi0_1 = " << phi01 << ", phi0_diff = "
 	 << ( abs( phi01 - phi02 ) - 3.1415926 )
 	 << ", d0_diff = " << ( d01 + d02 )
 	 << ", z0_diff = " << ( z01 - z02 )
 	 << ", chi2/ndf = " << chi2ndf << ", conf = " << conf << endl;
      
   } // end loop over frames
   
   double mean_resid = sum_resid / double( num_hits );
   double mean_square_resid = squaresum_resid / double( num_hits );
   double sigma_resid = sqrt( mean_square_resid - sqr( mean_resid ) );
   double error_in_mean_resid = sigma_resid / sqrt( double( num_hits ) );
   double error_in_sigma_resid = sigma_resid / sqrt( double( 2 * num_hits ) );

   double mean_norm_resid = sum_norm_resid / double( num_hits );
   double mean_square_norm_resid = squaresum_norm_resid / double( num_hits );
   double sigma_norm_resid = sqrt( mean_square_norm_resid - sqr( mean_norm_resid ) );
   double error_in_mean_norm_resid = sigma_norm_resid / sqrt( double( num_hits ) );
   double error_in_sigma_norm_resid = sigma_norm_resid / sqrt( double( 2 * num_hits ) );

   double chi2 = squaresum_norm_resid;

   report( NOTICE, kFacilityString )
      << "Finished iteration: mean resid = " << mean_resid
      << " +/- " << error_in_mean_resid << endl;
   report( NOTICE, kFacilityString )
      << "                    sigma resid = " << sigma_resid
      << " +/- " << error_in_sigma_resid << endl;
   report( NOTICE, kFacilityString )
      << "                    mean norm resid = " << mean_norm_resid
      << " +/- " << error_in_mean_norm_resid << endl;
   report( NOTICE, kFacilityString )
      << "                    sigma norm resid = " << sigma_norm_resid
      << " +/- " << error_in_sigma_norm_resid << endl;
   report( NOTICE, kFacilityString )
      << "                    chi2 / dof = " << chi2 << " / "
      << ( num_hits - 6 ) << " = " << ( chi2 / double( num_hits - 6 ) ) << endl;
   report( NOTICE, kFacilityString )
      << "                    confidence level = "
      << prob_( chi2, ( num_hits - 6 ) ) << endl;
   report( NOTICE, kFacilityString )
      << "                    number of negative error^2 hits / total = "
      << negative_errors << " / " << num_hits << " = "
      << double( negative_errors ) / double( num_hits ) << endl;
//     report( NOTICE, kFacilityString )
//        << "(event checksum is " << event_checksum << ")" << endl;

   return chi2;
}

double DRWeddingCakeAlignment::residualAlignment::last_time()
{
   mm_draw_last_histograms = true;
   
   unsigned int i_west = 1 + mm_cake;
   unsigned int i_east = 9 + mm_cake;

   double values[ kNumParams ] = { (* mm_geomConst)[ i_west ].get_deltaX(),
				   (* mm_geomConst)[ i_west ].get_deltaY(),
				   (* mm_geomConst)[ i_west ].get_rotZ(),
				   (* mm_geomConst)[ i_east ].get_deltaX(),
				   (* mm_geomConst)[ i_east ].get_deltaY(),
				   (* mm_geomConst)[ i_east ].get_rotZ() };

   double chi2 = iterate( values );
   mm_draw_last_histograms = false;
   mm_draw_any_histograms = false;
   return chi2;
}
