// -*- C++ -*-
//
// Package:     <DRFittingWeights>
// Module:      DRFittingWeights
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Thu Oct 18 10:45:01 EDT 2001
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
#include "DRFittingWeights/DRFittingWeights.h"
#include "DataHandler/Stream.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "DataHandler/Frame.h"

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
static const char* const kFacilityString = "DRFittingWeights.DRFittingWeights" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule.cc,v 1.2 2000/12/04 19:11:05 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DRFittingWeights::DRFittingWeights()
   : FrameIteratorModuleBase( "DRFittingWeights", "This is my module" )
   , m_file_layer( "layer_init", this, "unit.drweightlayer" )
   , m_file_drift( "drift_init", this, "unit.drweightdrift" )
   , m_file_theta( "theta_init", this, "unit.drweighttheta" )
//     , m_file_phi(   "phi_init",   this, "unit.drweightphi" )
//     , m_file_alpha( "alpha_init", this, "unit.drweightalpha" )
   , m_target_hits( "target_hits", this, 1000 )
   , m_iterations( "iterations", this, 3 )
   , m_particle( "particle", this, "muon" )
   , m_kumacName( "kumac", this, "check_fits.kumac" )
   , m_kumac( NULL )
   , m_const_layer( new DBDRWeightLayer[47], 47 )
   , m_const_drift( new DBDRWeightDrift[707], 707 )
   , m_const_theta( new DBDRWeightTheta[942], 942 )
//     , m_const_phi(   new DBDRWeightPhi[], )
//     , m_const_alpha( new DBDRWeightAlpha[], )
   , m_holder_layer( &m_const_layer )
   , m_holder_drift( &m_const_drift )
   , m_holder_theta( &m_const_theta )
//     , m_holder_phi(   &m_const_phi )
//     , m_holder_alpha( &m_const_alpha )
   , m_layer_info_valid( false )
   , m_drift_info_valid( false )
   , m_theta_info_valid( false )
   , m_layer_geometry_valid( false )
{
   registerProxyFactory(
      DrStream::kWeightLayer, m_holder_layer.makeFactory(), UsageTag() );
   registerProxyFactory(
      DrStream::kWeightDrift, m_holder_drift.makeFactory(), UsageTag() );
   registerProxyFactory(
      DrStream::kWeightTheta, m_holder_theta.makeFactory(), UsageTag() );
//     registerProxyFactory(
//        DrStream::kWeightPhi, m_holder_phi.makeFactory(), UsageTag() );
//     registerProxyFactory(
//        DrStream::kWeightAlpha, m_holder_alpha.makeFactory(), UsageTag() );

   //You must set what streams you which to iterate over
   //  that is, what events should the Frames be stopped on
   iterateOver( Stream::kEvent );

}

// DRFittingWeights::DRFittingWeights( const DRFittingWeights& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DRFittingWeights::~DRFittingWeights()
{
   for ( unsigned int layer = 1;  layer <= 47;  layer++ )
   {
      m_drift_full[layer-1].clear();
      m_drift_factor[layer-1].clear();
      m_drift_hits[layer-1].clear();

      m_theta_full[layer-1].clear();
      m_theta_factor[layer-1].clear();
      m_theta_hits[layer-1].clear();

//        m_phi_full[layer-1].clear();
//        m_phi_factor[layer-1].clear();
//        m_phi_hits[layer-1].clear();

//        m_alpha_full[layer-1].clear();
//        m_alpha_factor[layer-1].clear();
//        m_alpha_hits[layer-1].clear();
   }
}

//
// assignment operators
//
// const DRFittingWeights& DRFittingWeights::operator=( const DRFittingWeights& rhs )
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

double
DRFittingWeights::my_edrift( unsigned int layer, double drift, double theta,
			     double phi, double alpha )
{
   return 0.000123 * get_layer_full( layer )
                   * get_drift_full( layer, drift )
		   * get_theta_full( layer, theta );
//                     * get_phi_full( layer, phi )
//                     * get_alpha_full( layer, alpha );
}

double
DRFittingWeights::my_factor( unsigned int layer, double drift, double theta,
			     double phi, double alpha )
{
   return get_layer_factor( layer )
        * get_drift_factor( layer, drift )
	* get_theta_factor( layer, theta );
//          * get_phi_factor( layer, phi )
//          * get_alpha_factor( layer, alpha );
}

//
// const member functions
//
void
DRFittingWeights::iterate( const FIFrameIterator& iBegin,
			   const FIFrameIterator& iEnd )
{
   report( NOTICE, kFacilityString ) << "Initializing..." << endl;

   m_jc = JobControl::instance();
   m_hm = m_jc->histogramManagerP();

   report( NOTICE, kFacilityString )
      << "Opening kumac file for writing... ";
   if ( m_kumac == NULL )
      m_kumac = new ofstream( m_kumacName.value().c_str() );
   if ( m_kumac == NULL )
   {
      report( EMERGENCY, kFacilityString )
	 << "Couldn't open kumac file for writing. Giving up." << endl;
      return;
   }
   else
      report( NOTICE, kFacilityString ) << "success!" << endl;	 

   if ( ! setup_histograms() )
   {
      report( EMERGENCY, kFacilityString )
	 << "Couldn't set up histograms. Giving up." << endl;
      return;
   }

   if ( ! read_init_constants() )
   {
      report( EMERGENCY, kFacilityString )
	 << "Couldn't read input fittingweights. Giving up." << endl;
      return;
   }

   setup_bins();

   if ( ! read_layer_info( iBegin ) )
   {
      report( EMERGENCY, kFacilityString )
	 << "Couldn't get layer information from suez. Giving up." << endl;
      return;
   }
   report( NOTICE, kFacilityString ) << "Finished initialization." << endl;

   for ( unsigned int iteration = 1;
	 iteration <= m_iterations.value();
	 iteration++ )
   {
      report( NOTICE, kFacilityString )
	 << "Starting iteration " << iteration << "!" << endl;
      report( NOTICE, kFacilityString ) << "Resetting bins..." << endl;
      reset_bins();

      report( NOTICE, kFacilityString )
	 << "Resetting factors to 1.0..." << endl;
      reset_factors();

      report( NOTICE, kFacilityString ) << "Gathering data..." << endl;
      unsigned int hits = gather_data( iBegin, iEnd );
      report( NOTICE, kFacilityString )
	 << "Gathered " << hits << " hits." << endl;

      report( NOTICE, kFacilityString )
	 << "Optimizing (getting new factors)..." << endl;
      optimize( iteration );
      report( NOTICE, kFacilityString ) << "Finished optimizing!" << endl;
      
      report( NOTICE, kFacilityString )
	 << "Recalculating full lambdas from new factors..." << endl;
      recalc_fulls();

      report( NOTICE, kFacilityString )
	 << "Writing out updated constants..." << endl;
      char iter[3];
      sprintf( iter, "%02d", iteration );
      if ( ! write_state( string( "iter" ) + string( iter ) ) )
      {
	 report( EMERGENCY, kFacilityString )
	    << "Couldn't write out updated constants. Giving up." << endl;
	 return;
      }

      report( NOTICE, kFacilityString )
	 << "Overall full lambda is " << overall_lambda() << endl;

      report( NOTICE, kFacilityString )
	 << "Updating CLEOConstantsModifiable..." << endl;
      if ( ! update_constants() )
      {
	 report( EMERGENCY, kFacilityString )
	    << "Couldn't update constants internally. Giving up." << endl;
	 return;
      }

      report( NOTICE, kFacilityString )
	 << "Finished with iteration " << iteration << "!" << endl;
   }
   report( NOTICE, kFacilityString ) << "All done!" << endl;
}

DABoolean
DRFittingWeights::setup_histograms()
{
   m_edrift_diff = m_hm->histogram(
      101, "edrift diff", 100, -0.0001, 0.0001 );
   m_edrift_ratio = m_hm->histogram(
      102, "edrift ratio", 100, 0.5, 2. );
   m_edrift_normdiff = m_hm->histogram(
      103, "edrift norm diff", 100, -0.5, 0.5 );
   m_edrift_diff_vs_layer = m_hm->histogram(
      104, "edrift diff VS layer", 47, 0.5, 47.5, 100, -0.0001, 0.0001 );
   m_edrift_diff_vs_absDrift = m_hm->histogram(
      105, "edrift diff VS absDrift", 15, 0., 1.5, 100, -0.0001, 0.0001 );
   m_edrift_diff_vs_cosTheta = m_hm->histogram(
      106, "edrift diff VS cosTheta", 20, -1., 1., 100, -0.0001, 0.0001 );
   m_edrift_vs_layer = m_hm->histogram(
      107, "edrift VS layer", 47, 0.5, 47.5, 100, 0., 0.001 );
   m_edrift_vs_absDrift = m_hm->histogram(
      108, "edrift VS absDrift", 60, 0., 1.5, 100, 0., 0.001 );
   m_edrift_vs_cosTheta = m_hm->histogram(
      109, "edrift VS cosTheta", 80, -1., 1., 100, 0., 0.001 );
   m_fw_vs_layer = m_hm->histogram(
      110, "fw VS layer", 47, 0.5, 47.5, 100, 0., 0.001 );
   m_fw_vs_absDrift = m_hm->histogram(
      111, "fw VS absDrift", 60, 0., 1.5, 100, 0., 0.001 );
   m_fw_vs_cosTheta = m_hm->histogram(
      112, "fw VS cosTheta", 80, -1., 1., 100, 0., 0.001 );
   m_track_edrift_miss = m_hm->histogram(
      113, "track edrift miss", 500, -0.0047, 0.0047 );

   m_track_edrift_miss_close = m_hm->histogram(
      114, "track edrift miss (close)", 100, -0.0003, 0.0003 );
   double edriftmissStart[] = { 20., 0., 0.0001 };
   double edriftmissStep[] = { 1., 0.00001, 0.00005 };
   m_track_edrift_miss_close->attachFitFunc(
      new HIHistFitGauss( m_track_edrift_miss_close, HIHistFitFunc::kHist1D,
			  edriftmissStart, edriftmissStep ) );
//   m_track_edrift_miss_close->fitFunc()->setDomain( -0.0001, 0.0001 );

   m_residual = m_hm->histogram(
      200, "residual", 200, -0.000600, 0.000600 );
   double residStart[] = { 20., 0., 0.000123 };
   double residStep[] = { 1., 0.000001, 0.000020 };
   m_residual->attachFitFunc(
      new HIHistFitGauss( m_residual, HIHistFitFunc::kHist1D,
			  residStart, residStep ) );
   m_norm_residual = m_hm->histogram(
      201, "norm residual", 200, -6., 6. );
   double normresidStart[] = { 20., 0., 1. };
   double normresidStep[] = { 1., 0.001, 0.01 };
   m_norm_residual->attachFitFunc(
      new HIHistFitGauss( m_norm_residual, HIHistFitFunc::kHist1D,
			  normresidStart, normresidStep ) );

   return true;
}

DABoolean
DRFittingWeights::read_init_constants()
{
   ifstream layer_file( m_file_layer.value().c_str() );
   if ( ! m_const_layer.readFromStream( layer_file ) )
      return false;
   ifstream drift_file( m_file_drift.value().c_str() );
   if ( ! m_const_drift.readFromStream( drift_file ) )
      return false;
   ifstream theta_file( m_file_theta.value().c_str() );
   if ( ! m_const_theta.readFromStream( theta_file ) )
      return false;
//     ifstream phi_file(   m_file_phi.value().c_str() );
//     if ( ! m_const_phi.readFromStream(   phi_file ) )
//        return false;
//     ifstream alpha_file( m_file_alpha.value().c_str() );
//     if ( ! m_const_alpha.readFromStream( alpha_file ) )
//        return false;

   int layer, bin;
   for ( layer = 1;  layer <= 47;  layer++ )
   {
      DBDRWeightLayer this_layer = m_const_layer[layer-1];
      if ( this_layer.get_layer() != layer ) return false;
      m_layer_full[layer-1] = this_layer.get_fittingweight();
      m_layer_factor[layer-1] = 1.;
      m_layer_hits[layer-1] = 0;
   }
   m_layer_info_valid = true;

   DBDRWeightDrift this_drift = m_const_drift[0];
   if ( this_drift.get_layer() != 48 ) return false;
   if ( this_drift.get_bin() != 0 ) return false;
   m_drift_min = this_drift.get_fittingweight();
   this_drift = m_const_drift[1];
   if ( this_drift.get_layer() != 0 ) return false;
   m_drift_num_bins = this_drift.get_bin();
   m_drift_max = this_drift.get_fittingweight();
   for ( layer = 1;  layer <= 47;  layer++ )
   {
      m_drift_full[layer-1].clear();
      m_drift_factor[layer-1].clear();
      m_drift_hits[layer-1].clear();
      for ( bin = 1;  bin <= m_drift_num_bins;  bin++ )
      {
	 this_drift = m_const_drift[ (layer-1) * m_drift_num_bins + (bin-1)
				   + 2 ];
	 if ( this_drift.get_layer() != layer ) return false;
	 if ( this_drift.get_bin() != bin ) return false;
	 m_drift_full[layer-1].push_back( this_drift.get_fittingweight() );
	 m_drift_factor[layer-1].push_back( 1. );
	 m_drift_hits[layer-1].push_back( 0 );
      }
   }
   m_drift_info_valid = true;

   DBDRWeightTheta this_theta = m_const_theta[0];
   if ( this_theta.get_layer() != 48 ) return false;
   if ( this_theta.get_bin() != 0 ) return false;
   m_theta_min = this_theta.get_fittingweight();
   this_theta = m_const_theta[1];
   if ( this_theta.get_layer() != 0 ) return false;
   m_theta_num_bins = this_theta.get_bin();
   m_theta_max = this_theta.get_fittingweight();
   for ( layer = 1;  layer <= 47;  layer++ )
   {
      m_theta_full[layer-1].clear();
      m_theta_factor[layer-1].clear();
      m_theta_hits[layer-1].clear();
      for ( bin = 1;  bin <= m_theta_num_bins;  bin++ )
      {
	 this_theta = m_const_theta[ (layer-1) * m_theta_num_bins + (bin-1)
				   + 2 ];
	 if ( this_theta.get_layer() != layer ) return false;
	 if ( this_theta.get_bin() != bin ) return false;
	 m_theta_full[layer-1].push_back( this_theta.get_fittingweight() );
	 m_theta_factor[layer-1].push_back( 1. );
	 m_theta_hits[layer-1].push_back( 0. );
      }
   }
   m_theta_info_valid = true;

//     DBDRWeightPhi this_phi = m_const_phi[0];
//     if ( this_phi.get_layer() != 48 ) return false;
//     if ( this_phi.get_bin() != 0 ) return false;
//     m_phi_min = this_phi.get_fittingweight();
//     this_phi = m_const_phi[1];
//     if ( this_phi.get_layer() != 0 ) return false;
//     m_phi_num_bins = this_phi.get_bin();
//     m_phi_max = this_phi.get_fittingweight();
//     for ( layer = 1;  layer <= 47;  layer++ )
//     {
//        m_phi_full[layer-1].clear();
//        m_phi_factor[layer-1].clear();
//        m_phi_hits[layer-1].clear();
//        for ( bin = 1;  bin <= m_phi_num_bins;  bin++ )
//        {
//  	 this_phi = m_const_phi[ (layer-1) * m_phi_num_bins + (bin-1)
//  				   + 2 ];
//  	 if ( this_phi.get_layer() != layer ) return false;
//  	 if ( this_phi.get_bin() != bin ) return false;
//  	 m_phi_full[layer-1].push_back( this_phi.get_fittingweight() );
//  	 m_phi_factor[layer-1].push_back( 1. );
//       m_phi_hits[layer-1].push_back( 0. );
//        }
//     }
//     m_phi_info_valid = true;

//     DBDRWeightAlpha this_alpha = m_const_alpha[0];
//     if ( this_alpha.get_layer() != 48 ) return false;
//     if ( this_alpha.get_bin() != 0 ) return false;
//     m_alpha_min = this_alpha.get_fittingweight();
//     this_alpha = m_const_alpha[1];
//     if ( this_alpha.get_layer() != 0 ) return false;
//     m_alpha_num_bins = this_alpha.get_bin();
//     m_alpha_max = this_alpha.get_fittingweight();
//     for ( layer = 1;  layer <= 47;  layer++ )
//     {
//        m_alpha_full[layer-1].clear();
//        m_alpha_factor[layer-1].clear();
//        m_alpha_hits[layer-1].clear();
//        for ( bin = 1;  bin <= m_alpha_num_bins;  bin++ )
//        {
//  	 this_alpha = m_const_alpha[ (layer-1) * m_alpha_num_bins + (bin-1)
//  				   + 2 ];
//  	 if ( this_alpha == m_const_alpha.end() ) return false;
//  	 if ( this_alpha.get_layer() != layer ) return false;
//  	 if ( this_alpha.get_bin() != bin ) return false;
//  	 m_alpha_full[layer-1].push_back( this_alpha.get_fittingweight() );
//  	 m_alpha_factor[layer-1].push_back( 1. );
//       m_alpha_hits[layer-1].push_back( 0. );
//        }
//     }
//     m_alpha_info_valid = true;

   return true;
}

DABoolean
DRFittingWeights::read_layer_info( const FIFrameIterator& iBegin )
{
   // To call resetToBeginning()
   FIFrameIterator itFrame = iBegin;

   FAItem< ADRSenseWireStore > adrWireStore;
   extract( itFrame->record( Stream::kBaseGeometry ), adrWireStore );
   if ( ! adrWireStore.valid() ) return false;

   for ( int layer = 1;  layer <= 47;  layer++ )
   {
      m_layer_num_cells[layer-1] = adrWireStore->numberOfWiresLyr( layer );
      m_layer_radius[layer-1] = adrWireStore->radius( layer );
   }
   m_layer_geometry_valid = true;

   return true;
}

DABoolean
DRFittingWeights::write_state( string tag )
{
   unsigned int layer, bin, index;
   DABoolean success = true;
   char line[128];

   ofstream layer_file( ( tag + string(".drweightlayer") ).c_str() );
   success = success && layer_file;
   success = success && layer_file << "1 0 0 0 0 0 0 0 DEFAULT" << endl;
   success = success && layer_file << "47" << endl;
   sprintf( line, "%s.drweightlayer %5s %15s %15s %10s",
	    tag.c_str(), "index", "full", "factor", "hits" );
   cout << line << endl;

   index = 1;
   for ( layer = 1;  layer <= 47;  layer++ )
   {
      success = success && layer_file << index << " "
				      << m_layer_full[layer-1] << endl;

      sprintf( line, "%s.drweightlayer %5d %15.10f %15.10f %10d", tag.c_str(),
	       index, m_layer_full[layer-1], m_layer_factor[layer-1],
	       m_layer_hits[layer-1] );
      cout << line << endl;

      index++;
   }

   ofstream drift_file( ( tag + string(".drweightdrift") ).c_str() );
   success = success && drift_file;
   success = success && drift_file << "1 0 0 0 0 0 0 0 DEFAULT" << endl;
   success = success && drift_file << ( 47 * m_drift_num_bins + 2 ) << endl;
   success = success && drift_file << "1 48 0 " << m_drift_min << endl;
   success = success && drift_file << "2 0 " << m_drift_num_bins << " "
				   << m_drift_max << endl;

   sprintf( line, "%s.drweightdrift %5s %5s %5s %15s %15s %10s",
	    tag.c_str(), "index", "layer", "bin", "full", "factor", "hits" );
   cout << line << endl;

   index = 3;
   for ( layer = 1;  layer <= 47;  layer++ )
      for ( bin = 1;  bin <= m_drift_num_bins;  bin++ )
      {
	 success = success &&
	    drift_file << index << " " << layer << " " << bin << " "
		       << m_drift_full[layer-1][bin-1] << endl;

	 sprintf( line, "%s.drweightdrift %5d %5d %5d %15.10f %15.10f %10d",
		  tag.c_str(), index, layer, bin,
		  m_drift_full[layer-1][bin-1],
		  m_drift_factor[layer-1][bin-1],
		  int( m_drift_hits[layer-1][bin-1] ) );
	 cout << line << endl;

	 index++;
      }

   ofstream theta_file( ( tag + string(".drweighttheta") ).c_str() );
   success = success && theta_file;
   success = success && theta_file << "1 0 0 0 0 0 0 0 DEFAULT" << endl;
   success = success && theta_file << ( 47 * m_theta_num_bins + 2 ) << endl;
   success = success && theta_file << "1 48 0 " << m_theta_min << endl;
   success = success && theta_file << "2 0 " << m_theta_num_bins << " "
				   << m_theta_max << endl;

   sprintf( line, "%s.drweighttheta %5s %5s %5s %15s %15s %10s",
	    tag.c_str(), "index", "layer", "bin", "full", "factor", "hits" );
   cout << line << endl;

   index = 3;
   for ( layer = 1;  layer <= 47;  layer++ )
      for ( bin = 1;  bin <= m_theta_num_bins;  bin++ )
      {
	 success = success &&
	    theta_file << index << " " << layer << " " << bin << " "
		       << m_theta_full[layer-1][bin-1] << endl;

	 sprintf( line, "%s.drweighttheta %5d %5d %5d %15.10f %15.10f %10d",
		  tag.c_str(), index, layer, bin,
		  m_theta_full[layer-1][bin-1],
		  m_theta_factor[layer-1][bin-1],
		  int( m_theta_hits[layer-1][bin-1] ) );
	 cout << line << endl;

	 index++;
      }

//     ofstream phi_file( ( tag + string(".drweightphi") ).c_str() );
//     success = success && phi_file;
//     success = success && phi_file << "1 0 0 0 0 0 0 0 DEFAULT" << endl;
//     success = success && phi_file << ( 47 * m_phi_num_bins + 2 ) << endl;
//     success = success && phi_file << "1 48 0 " << m_phi_min << endl;
//     success = success && phi_file << "2 0 " << m_phi_num_bins << " "
//  				   << m_phi_max << endl;

//     sprintf( line, "%s.drweightphi %5s %5s %5s %15s %15s %10s",
//  	    tag.c_str(), "index", "layer", "bin", "full", "factor", "hits" );
//     cout << line << endl;

//     index = 3;
//     for ( layer = 1;  layer <= 47;  layer++ )
//        for ( bin = 1;  bin <= m_phi_num_bins;  bin++ )
//        {
//  	 success = success &&
//  	    phi_file << index << " " << layer << " " << bin << " "
//  		       << m_phi_full[layer-1][bin-1] << endl;

//  	 sprintf( line, "%s.drweightphi %5d %5d %5d %15.10f %15.10f %10d",
//  		  tag.c_str(), index, layer, bin,
//  		  m_phi_full[layer-1][bin-1],
//  		  m_phi_factor[layer-1][bin-1],
//                int( m_phi_hits[layer-1][bin-1] ) );
//  	 cout << line << endl;

//  	 index++;
//        }

//     ofstream alpha_file( ( tag + string(".drweightalpha") ).c_str() );
//     success = success && alpha_file;
//     success = success && alpha_file << "1 0 0 0 0 0 0 0 DEFAULT" << endl;
//     success = success && alpha_file << ( 47 * m_alpha_num_bins + 2 ) << endl;
//     success = success && alpha_file << "1 48 0 " << m_alpha_min << endl;
//     success = success && alpha_file << "2 0 " << m_alpha_num_bins << " "
//  				   << m_alpha_max << endl;

//     sprintf( line, "%s.drweightalpha %5s %5s %5s %15s %15s %10s",
//  	    tag.c_str(), "index", "layer", "bin", "full", "factor", "hits" );
//     cout << line << endl;

//     index = 3;
//     for ( layer = 1;  layer <= 47;  layer++ )
//        for ( bin = 1;  bin <= m_alpha_num_bins;  bin++ )
//        {
//  	 success = success &&
//  	    alpha_file << index << " " << layer << " " << bin << " "
//  		       << m_alpha_full[layer-1][bin-1] << endl;

//  	 sprintf( line, "%s.drweightalpha %5d %5d %5d %15.10f %15.10f %10d",
//  		  tag.c_str(), index, layer, bin,
//  		  m_alpha_full[layer-1][bin-1],
//  		  m_alpha_factor[layer-1][bin-1],
//                int( m_alpha_hits[layer-1][bin-1] ) );
//  	 cout << line << endl;

//  	 index++;
//        }

   return success;
}

DABoolean
DRFittingWeights::update_constants()
{
   unsigned int layer, bin;
   
   for ( layer = 1;  layer <= 47;  layer++ )
      m_const_layer[layer-1].set_fittingweight( m_layer_full[layer-1] );

   for ( layer = 1;  layer <= 47;  layer++ )
      for ( bin = 1;  bin <= m_drift_num_bins;  bin++ )
	 m_const_drift[ (layer-1) * m_drift_num_bins + (bin-1) + 2 ]
	    .set_fittingweight( m_drift_full[layer-1][bin-1] );
      
   for ( layer = 1;  layer <= 47;  layer++ )
      for ( bin = 1;  bin <= m_theta_num_bins;  bin++ )
	 m_const_theta[ (layer-1) * m_theta_num_bins + (bin-1) + 2 ]
	    .set_fittingweight( m_theta_full[layer-1][bin-1] );
      
//     for ( layer = 1;  layer <= 47;  layer++ )
//        for ( bin = 1;  bin <= m_phi_num_bins;  bin++ )
//  	 m_const_phi[ (layer-1) * m_phi_num_bins + (bin-1) + 3 ]
//  	    .set_fittingweight( m_phi_full[layer-1][bin-1] );
      
//     for ( layer = 1;  layer <= 47;  layer++ )
//        for ( bin = 1;  bin <= m_alpha_num_bins;  bin++ )
//  	 m_const_alpha[ (layer-1) * m_alpha_num_bins + (bin-1) + 3 ]
//  	    .set_fittingweight( m_alpha_full[layer-1][bin-1] );

   return true;
}

void
DRFittingWeights::reset_factors()
{
   int layer, bin;
   for ( layer = 1;  layer <= 47;  layer++ )
   {
      m_layer_factor[layer-1] = 1.;

      m_drift_factor[layer-1].clear();
      for ( bin = 1;  bin <= m_drift_num_bins;  bin++ )
	 m_drift_factor[layer-1].push_back( 1. );

      m_theta_factor[layer-1].clear();
      for ( bin = 1;  bin <= m_theta_num_bins;  bin++ )
	 m_theta_factor[layer-1].push_back( 1. );

//        m_phi_factor[layer-1].clear();
//        for ( bin = 1;  bin <= m_phi_num_bins;  bin++ )
//  	     m_phi_factor[layer-1].push_back( 1. );

//        m_alpha_factor[layer-1].clear();
//        for ( bin = 1;  bin <= m_alpha_num_bins;  bin++ )
//  	     m_alpha_factor[layer-1].push_back( 1. );
   }
}

void
DRFittingWeights::recalc_fulls()
{
   int layer, bin;
   for ( layer = 1;  layer <= 47;  layer++ )
   {
      m_layer_full[layer-1] *= m_layer_factor[layer-1];

      for ( bin = 1;  bin <= m_drift_num_bins;  bin++ )
	 m_drift_full[layer-1][bin] *= m_drift_factor[layer-1][bin];

      for ( bin = 1;  bin <= m_theta_num_bins;  bin++ )
	 m_theta_full[layer-1][bin] *= m_theta_factor[layer-1][bin];

//        for ( bin = 1;  bin <= m_phi_num_bins;  bin++ )
//	     m_phi_full[layer-1][bin] *= m_phi_factor[layer-1][bin];

//        for ( bin = 1;  bin <= m_alpha_num_bins;  bin++ )
//	     m_alpha_full[layer-1][bin] *= m_alpha_factor[layer-1][bin];
   }
}

double
DRFittingWeights::overall_lambda()
{
   unsigned int layer, bin;

   double layer_sum = 0.;
   double layer_n = 0.;
   for ( layer = 1;  layer <= 47;  layer++ )
   {
      layer_sum += ( m_layer_hits[layer-1] *
		     m_layer_full[layer-1]   );
      layer_n += m_layer_hits[layer-1];
   }
   double layer_lambda = layer_sum / layer_n;

   double drift_sum = 0.;
   double drift_n = 0.;
   for ( layer = 1;  layer <= 47;  layer++ )
      for ( bin = 1;  bin <= m_drift_num_bins;  bin++ )
      {
	 drift_sum += ( m_drift_hits[layer-1][bin-1] *
			m_drift_full[layer-1][bin-1]   );
	 drift_n += m_drift_hits[layer-1][bin-1];
      }
   double drift_lambda = drift_sum / drift_n;

   double theta_sum = 0.;
   double theta_n = 0.;
   for ( layer = 1;  layer <= 47;  layer++ )
      for ( bin = 1;  bin <= m_theta_num_bins;  bin++ )
      {
	 theta_sum += ( m_theta_hits[layer-1][bin-1] *
			m_theta_full[layer-1][bin-1]   );
	 theta_n += m_theta_hits[layer-1][bin-1];
      }
   double theta_lambda = theta_sum / theta_n;

//     double phi_sum = 0.;
//     double phi_n = 0.;
//     for ( layer = 1;  layer <= 47;  layer++ )
//        for ( bin = 1;  bin <= m_phi_num_bins;  bin++ )
//        {
//  	 phi_sum += ( m_phi_hits[layer-1][bin-1] *
//  			m_phi_full[layer-1][bin-1]   );
//  	 phi_n += m_phi_hits[layer-1][bin-1];
//        }
//     double phi_lambda = phi_sum / phi_n;

//     double alpha_sum = 0.;
//     double alpha_n = 0.;
//     for ( layer = 1;  layer <= 47;  layer++ )
//        for ( bin = 1;  bin <= m_alpha_num_bins;  bin++ )
//        {
//  	 alpha_sum += ( m_alpha_hits[layer-1][bin-1] *
//  			m_alpha_full[layer-1][bin-1]   );
//  	 alpha_n += m_alpha_hits[layer-1][bin-1];
//        }
//     double alpha_lambda = alpha_sum / alpha_n;

   cout << "layer ave = " << layer_lambda << ", drift ave = "
	<< drift_lambda << ", theta ave = " << theta_lambda << endl;
   return layer_lambda * drift_lambda * theta_lambda;
//	  * phi_lambda * alpha_lambda;
}

unsigned int
DRFittingWeights::get_bin( double value, unsigned int num_bins,
			   double min, double max )
{
   return (unsigned int)( floor( ( ( value - min ) / ( max - min ) )
				 * double( num_bins ) ) );
}

double
DRFittingWeights::get_bin_center( unsigned int bin, unsigned int num_bins,
				  double min, double max )
{
   return ( double( bin ) + 0.5 ) * ( max - min ) / double( num_bins ) + min;
}

double
DRFittingWeights::get_bin_top( unsigned int bin, unsigned int num_bins,
			       double min, double max )
{
   double halfwidth = ( max - min ) / double( num_bins ) / 2.;
   return get_bin_center( bin, num_bins, min, max ) + halfwidth;
}

double
DRFittingWeights::get_bin_bottom( unsigned int bin, unsigned int num_bins,
				  double min, double max )
{
   double halfwidth = ( max - min ) / double( num_bins ) / 2.;
   return get_bin_center( bin, num_bins, min, max ) - halfwidth;
}

double
DRFittingWeights::interp( double value, unsigned int num_bins,
			  double min, double max, vector< double >& data )
{
   // extrapolate if you are to the left of the center of the first bin
   if ( ( value - min ) < ( ( max - min ) / 2. / double( num_bins ) ) )
   {
      double y1 = data[0];
      double y2 = data[1];
      double x1 = get_bin_center( 0, num_bins, min, max );
      double x2 = get_bin_center( 1, num_bins, min, max );

      return ( (y2 - y1)/(x2 - x1) * ( value - x1 ) + y1 );
   }
   // extrapolate if you are to the right of the center of the last bin
   else if ( ( max - value ) < ( ( max - min ) / 2. / double( num_bins ) ) )
   {
      double y1 = data[ num_bins-2 ];
      double y2 = data[ num_bins-1 ];
      double x1 = get_bin_center( num_bins-2, num_bins, min, max );
      double x2 = get_bin_center( num_bins-1, num_bins, min, max );

      return ( (y2 - y1)/(x2 - x1) * ( value - x1 ) + y1 );
   }
   // interpolate if you are in between bin centers
   else
   {
      double soft_bin = ( ( value - min ) / ( max - min ) *
			  double( num_bins ) - 0.5 );
      double left_lambda = data[ (unsigned int)(floor( soft_bin )) ];
      double right_lambda = data[ (unsigned int)(ceil( soft_bin )) ];
      
      return ( ( right_lambda - left_lambda )
	       * ( soft_bin - double( floor( soft_bin ) ) ) + left_lambda );
   }
}

double
DRFittingWeights::get_absDrift( unsigned int layer, double drift )
{
   assert( m_layer_geometry_valid );

   return ( abs( drift ) * double( m_layer_num_cells[layer-1] ) /
	    ( 3.1415926 * m_layer_radius[layer-1] ) );
}

double
DRFittingWeights::get_layer_factor( unsigned int layer )
{
   return m_layer_factor[layer-1];
}

double
DRFittingWeights::get_drift_factor( unsigned int layer, double drift )
{
   return interp( get_absDrift( layer, drift ),
		  m_drift_num_bins, m_drift_min, m_drift_max,
		  m_drift_factor[layer-1] );
}

double
DRFittingWeights::get_theta_factor( unsigned int layer, double theta )
{
   return interp( theta,
		  m_theta_num_bins, m_theta_min, m_theta_max,
		  m_theta_factor[layer-1] );
}

//  double
//  DRFittingWeights::get_phi_factor( unsigned int layer, double phi )
//  {
//     return interp( phi,
//  		  m_phi_num_bins, m_phi_min, m_phi_max,
//  		  m_phi_factor[layer-1] );
//  }

//  double
//  DRFittingWeights::get_alpha_factor( unsigned int layer, double alpha )
//  {
//     return interp( alpha,
//  		  m_alpha_num_bins, m_alpha_min, m_alpha_max,
//  		  m_alpha_factor[layer-1] );
//  }

double
DRFittingWeights::get_layer_full( unsigned int layer )
{
   return m_layer_full[layer-1];
}

double
DRFittingWeights::get_drift_full( unsigned int layer, double drift )
{
   return interp( get_absDrift( layer, drift ),
		  m_drift_num_bins, m_drift_min, m_drift_max,
		  m_drift_full[layer-1] );
}

double
DRFittingWeights::get_theta_full( unsigned int layer, double theta )
{
   return interp( theta,
		  m_theta_num_bins, m_theta_min, m_theta_max,
		  m_theta_full[layer-1] );
}

//  double
//  DRFittingWeights::get_phi_full( unsigned int layer, double phi )
//  {
//     return interp( phi,
//  		  m_phi_num_bins, m_phi_min, m_phi_max,
//  		  m_phi_full[layer-1] );
//  }

//  double
//  DRFittingWeights::get_alpha_full( unsigned int layer, double alpha )
//  {
//     return interp( alpha,
//  		  m_alpha_num_bins, m_alpha_min, m_alpha_max,
//  		  m_alpha_full[layer-1] );
//  }

unsigned int
DRFittingWeights::gather_data( const FIFrameIterator& iBegin,
			       const FIFrameIterator& iEnd )
{
   DBCandidate::Hypo particle;
   if ( strcmp( m_particle.value().c_str(), "electron" ) == 0 )
      particle = DBCandidate::kElectron;
   else if ( strcmp( m_particle.value().c_str(), "muon" ) == 0 )
      particle = DBCandidate::kMuon;
   
   FIFrameIterator itFrame = iBegin;
   FAItem< ADRSenseWireStore > adrWireStore;
   extract( itFrame->record( Stream::kBaseGeometry ), adrWireStore );
   if ( ! adrWireStore.valid() ) return false;

   unsigned int hit_count = 0;
   for( ;  itFrame != iEnd  &&  ! all_filled();  ++itFrame )
   {
      FATable< NavTrack > navtracks;
      extract( itFrame->record( Stream::kEvent ), navtracks );
      FATable< NavTrack >::const_iterator navtracks_iterator;
      FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
      FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();

////  the cuts  ////////////////////////////////////////////////////////
      if ( navtracks.size() != 2 ) continue;
      
      double d0miss = 0.;
      double z0miss = 0.;
      double curvSignProduct = 1.;
      DABoolean bad_track = false;
      for( navtracks_iterator = navtracks_begin;
	   navtracks_iterator != navtracks_end;
	   navtracks_iterator++ )
      {
	 FAItem< TDKinematicFit > trackFit =
	    navtracks_iterator->kinematicFit( particle );
	 FAItem< TRHelixFit > trackHelix =
	    navtracks_iterator->helixFit( particle );
	 FAItem< TRTrackFitQuality > trackQuality =
	    navtracks_iterator->quality( particle );
	 
	 double curvSign = ( trackHelix->curvature() > 0. ? 1. : -1. );
	 d0miss += trackHelix->d0();
	 z0miss += trackHelix->z0() * curvSign;
	 curvSignProduct *= curvSign;

	 HepVector3D momentum = trackFit->momentum();
	 double chiSquare = trackQuality->chiSquare();
	 int degreesOfFreedom = trackQuality->degreesOfFreedom();
	 int numberHits = trackQuality->numberHits();
	 int numberHitsExpected = trackQuality->numberHitsExpected();
	 double d0 = trackHelix->d0();
	 double z0 = trackHelix->z0();
	 double cotTheta_err =
	    sqrt( abs( trackHelix->errorMatrix()(
	       KTHelix::kCotTheta, KTHelix::kCotTheta ) ) );
	 double z0_err =
	    sqrt( abs( trackHelix->errorMatrix()(
	       KTHelix::kZ0, KTHelix::kZ0 ) ) );

	 if ( trackQuality->fitAbort() ) bad_track = true;
	 if ( momentum.mag() < 4.5  ||  momentum.mag() > 6.0 )
	    bad_track = true;
	 if ( degreesOfFreedom == 0  ||
	      chiSquare / double( degreesOfFreedom ) > 50.  ||
	      chiSquare / double( degreesOfFreedom ) < 0.      )
	    bad_track = true;
	 if ( numberHitsExpected == 0  ||
	      double( numberHits ) / double( numberHitsExpected ) < 0.5 )
	    bad_track = true;
	 if ( abs( d0 ) > 0.03  ||  abs( z0 ) > 0.150 ) bad_track = true;
	 if ( cotTheta_err > 0.50  ||  z0_err > 0.25 ) bad_track = true;

      } // end loop over tracks (first check sweep)
      if ( curvSignProduct > 0. ) continue;
      if ( bad_track ) continue;

////  the real thing  //////////////////////////////////////////////////
      for( navtracks_iterator = navtracks_begin;
	   navtracks_iterator != navtracks_end;
	   navtracks_iterator++ )
      {
	 FAItem< TDKinematicFit > trackFit =
	    navtracks_iterator->kinematicFit( particle );
	 FAItem< TRHelixFit > trackHelix =
	    navtracks_iterator->helixFit( particle );
	 TRHelixFit swimmable_helix = (* trackHelix );

	 double cotTheta = trackHelix->cotTheta();
	 double cosTheta = cotTheta / sqrt( 1 + sqr( cotTheta ) );
	 
	 const NavTrack::DRHitTable* p_trackDRHits =
	    navtracks_iterator->drHits( particle );
	 if ( p_trackDRHits == NULL ) continue;
	 NavTrack::DRHitTable trackDRHits = (* p_trackDRHits );

	 NavTrack::DRHitTable::const_iterator trackDRHits_iterator;
	 NavTrack::DRHitTable::const_iterator trackDRHits_begin =
	    trackDRHits.begin();
	 NavTrack::DRHitTable::const_iterator trackDRHits_end =
	    trackDRHits.end();

	 const NavTrack::DRHitLinkTable* trackDRLinks =
	    navtracks_iterator->drHitLinks( particle );
	 if ( trackDRLinks == NULL ) continue;
	 NavTrack::DRHitLinkTable::const_iterator trackDRLinks_iterator;
	 NavTrack::DRHitLinkTable::const_iterator trackDRLinks_begin =
	    trackDRLinks->begin();
	 NavTrack::DRHitLinkTable::const_iterator trackDRLinks_end =
	    trackDRLinks->end();

	 double track_edrift_miss = 0.;
	 for( trackDRHits_iterator = trackDRHits_begin;
	      trackDRHits_iterator != trackDRHits_end;
	      trackDRHits_iterator++ )
	 {
	    for( trackDRLinks_iterator = trackDRLinks_begin;
		 trackDRLinks_iterator != trackDRLinks_end
		    &&  (     (* trackDRLinks_iterator )->rightID() == 0
			      ||  *(* trackDRLinks_iterator )->rightID() !=
			      ( *trackDRHits_iterator )->identifier()
		       );
		 trackDRLinks_iterator++ );
	    if ( trackDRLinks_iterator == trackDRLinks_end       ||
		 (* trackDRLinks_iterator )->rightID() == 0 ) continue;
	    const TrackFitDRHitLink& trackDRHitLink =
	       (* trackDRLinks_iterator )->linkData();
	    
	    if ( trackDRHitLink.used() )
	    {
	       unsigned int layer = (* trackDRHits_iterator )->layer();
	       double residual = trackDRHitLink.residual();
	       double edca = trackDRHitLink.signedDcaError();
	       double drift = trackDRHitLink.signedDriftDistance();
	       double edrift = trackDRHitLink.signedDriftError();
	       double sinAlpha = trackDRHitLink.sinTrackToRadial();

	       double distance_swim;
	       double z = swimmable_helix.moveToRadius(
		  adrWireStore->radius( layer ), distance_swim );
	       unsigned int wire_number =
		  (* trackDRHits_iterator )->wireInLyr();
	       double phi = adrWireStore->wire( layer, wire_number )
		  .zPoint( z ).first.phi();

	       double fw_edrift = my_edrift( layer, drift, cosTheta,
					     phi, sinAlpha );
	       double edrift_miss = edrift - fw_edrift;
	       track_edrift_miss += edrift_miss;
	       double absDrift = get_absDrift( layer, drift );

	       m_edrift_diff->fill( edrift_miss );
	       m_edrift_ratio->fill( edrift / fw_edrift );
	       m_edrift_normdiff->fill( edrift_miss / fw_edrift );

	       m_edrift_diff_vs_layer->fill( layer, edrift_miss );
	       m_edrift_diff_vs_absDrift->fill( absDrift, edrift_miss );
	       m_edrift_diff_vs_cosTheta->fill( cosTheta, edrift_miss );

	       m_edrift_vs_layer->fill( layer, edrift );
	       m_edrift_vs_absDrift->fill( absDrift, edrift );
	       m_edrift_vs_cosTheta->fill( cosTheta, edrift );

	       m_fw_vs_layer->fill( layer, fw_edrift );
	       m_fw_vs_absDrift->fill( absDrift, fw_edrift );
	       m_fw_vs_cosTheta->fill( cosTheta, fw_edrift );

	       m_residual->fill( residual );
	       m_norm_residual->fill( residual
				      / sqrt( sqr( edrift ) + sqr( edca ) ) );
	       
	       take( residual, sqr( edrift ), sqr( edca ),
		     layer, drift, cosTheta, phi, sinAlpha );
	       
	       hit_count++;
	    } // end if hit used in the fit
	 } // end loop over DR hits
	 
	 m_track_edrift_miss->fill( track_edrift_miss );
	 m_track_edrift_miss_close->fill( track_edrift_miss );
	 
      } // end loop over tracks (the real thing)
   } // end loop over frames
   return hit_count;
}

void
DRFittingWeights::setup_bins()
{
   m_everything = new DRFBin( m_hm, 1000,
			      string( "everything" ), 1, 0., 0.,
			      m_target_hits.value(),
			      m_iterations.value() );
}

void
DRFittingWeights::reset_bins()
{
   m_everything->reset();
}

DABoolean
DRFittingWeights::all_filled()
{
   return m_everything->state() == DRFBin::kFilled;
}

void
DRFittingWeights::take( double residual, double edrift2, double edca2,
			unsigned int layer, double drift, double cosTheta,
			double phi, double sinAlpha )
{
   m_everything->take( residual, edrift2, edca2,
		       layer, drift, cosTheta, phi, sinAlpha );

   m_layer_hits[layer-1]++;
   m_drift_hits[layer-1][
      get_bin( get_absDrift( layer, drift ),
	       m_drift_num_bins, m_drift_min, m_drift_max )-1 ]++;
   m_theta_hits[layer-1][
      get_bin( cosTheta, m_theta_num_bins, m_theta_min, m_theta_max )-1 ]++;
//     m_phi_hits[layer-1][
//        get_bin( phi, m_phi_num_bins, m_phi_min, m_phi_max )-1 ]++;
//     m_alpha_hits[layer-1][
//        get_bin( sinAlpha, m_alpha_num_bins, m_alpha_min, m_alpha_max )-1 ]++;

}

void
DRFittingWeights::optimize( unsigned int iteration )
{
   if ( m_everything->state() == DRFBin::kFilled )
   {
      m_everything->update( this );
      if ( m_everything->optimize() == DRFBin::kOptimized )
      {
	 m_everything->copyHist( iteration, *m_kumac );

	 double lambda = m_everything->lambda();
	 for ( unsigned int i = 0;  i < 47;  i++ )
	    m_layer_factor[i] *= lambda;
      } // end if optimization is successful
      else
	 report( NOTICE, kFacilityString )
	    << "Optimization of only bin failed." << endl;
   } // end if filling was successful
   else
      report( NOTICE, kFacilityString )
	 << "Filling of only bin failed." << endl;
}

//
// static member functions
//
