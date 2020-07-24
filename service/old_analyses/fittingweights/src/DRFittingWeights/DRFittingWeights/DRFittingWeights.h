// -*- C++ -*-
#if !defined(DRFITTINGWEIGHTS_DRFITTINGWEIGHTS_H)
#define DRFITTINGWEIGHTS_DRFITTINGWEIGHTS_H
//
// Package:     DRFittingWeights
// Module:      DRFittingWeights
//
/**\class DRFittingWeights DRFittingWeights.h DRFittingWeights/DRFittingWeights.h
 
 Description: Suez Module that allows you to control the "event" loop

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Thu Oct 18 10:45:01 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "CommandPattern/Module.h"
#include "FrameIterate/FrameIteratorModuleBase.h"
#include "DRFittingWeights/DRFBin.h"
#include "FrameIterate/FIHolder.h"
#include "JobControl/JobControl.h"
#include "ToolBox/HistogramPackage.h"
#include "HistogramInterface/HIHistFitLine.h"
#include "HistogramInterface/HIHistFitGauss.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "Navigation/NavTrack.h"
#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "FrameAccess/FAConstants.h"
#include "C3dr/DrStream.h"
#include "BDLDRclient/DBDRWeightLayer.hh"
#include "BDLDRclient/DBDRWeightDrift.hh"
#include "BDLDRclient/DBDRWeightTheta.hh"
//  #include "BDLDRclient/DBDRWeightPhi.hh"
//  #include "BDLDRclient/DBDRWeightAlpha.hh"
#include "CommandPattern/Parameter.h"
#include "CLHEP/Vector/ThreeVector.h"
#include<fstream>
#include<vector>

// forward declarations

class DRFBin;

class DRFittingWeights : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DRFittingWeights();
      virtual ~DRFittingWeights();

      // ---------- member functions ---------------------------

      double my_edrift( unsigned int layer, double drift, double theta,
			double phi, double alpha );
      double my_factor( unsigned int layer, double drift, double theta,
			double phi, double alpha );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      ///override this function to do the actual iterations
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DRFittingWeights( const DRFittingWeights& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DRFittingWeights& operator=( const DRFittingWeights& ); // stop default

      // ---------- private member functions -------------------

      DABoolean setup_histograms();
      DABoolean read_init_constants();
      DABoolean read_layer_info( const FIFrameIterator& iBegin );
      DABoolean write_state( string tag );
      DABoolean update_constants();
      void reset_factors();
      void recalc_fulls();
      double overall_lambda();
      unsigned int get_bin( double value, unsigned int num_bins,
			    double min, double max );
      double get_bin_center( unsigned int bin, unsigned int num_bins,
			     double min, double max );
      double get_bin_top( unsigned int bin, unsigned int num_bins,
			  double min, double max );
      double get_bin_bottom( unsigned int bin, unsigned int num_bins,
			     double min, double max );
      double interp( double value, unsigned int num_bins,
		     double min, double max, vector< double >& data );
      double get_absDrift( unsigned int layer, double drift );
      double get_layer_factor( unsigned int layer );
      double get_drift_factor( unsigned int layer, double drift );
      double get_theta_factor( unsigned int layer, double theta );
//        double phi_factor( unsigned int layer, double phi );
//        double alpha_factor( unsigned int layer, double alpha );
      double get_layer_full( unsigned int layer );
      double get_drift_full( unsigned int layer, double drift );
      double get_theta_full( unsigned int layer, double theta );
//        double phi_full( unsigned int layer, double phi );
//        double alpha_full( unsigned int layer, double alpha );
      unsigned int gather_data( const FIFrameIterator& iBegin,
				const FIFrameIterator& iEnd );

      // these depend on bin method
      void setup_bins();
      void reset_bins();
      DABoolean all_filled();
      void take( double residual, double edrift2, double edca2,
		 unsigned int layer, double drift, double cosTheta,
		 double phi, double sinAlpha );
      // this changes lambda factors (but not fulls, call recalc_fulls)
      void optimize( unsigned int iteration );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      Parameter< string > m_file_layer;
      Parameter< string > m_file_drift;
      Parameter< string > m_file_theta;
//        Parameter< string > m_file_phi;
//        Parameter< string > m_file_alpha;
      Parameter< unsigned int > m_target_hits;
      Parameter< unsigned int > m_iterations;
      Parameter< string > m_particle;
      Parameter< string > m_kumacName;
      ofstream* m_kumac;

      CLEOConstantsModifiable< DBDRWeightLayer > m_const_layer;
      CLEOConstantsModifiable< DBDRWeightDrift > m_const_drift;
      CLEOConstantsModifiable< DBDRWeightTheta > m_const_theta;
//       CLEOConstantsModifiable< DBDRWeightPhi >   m_const_phi;
//       CLEOConstantsModifiable< DBDRWeightAlpha > m_const_alpha;
      FIHolder< CLEOConstants< DBDRWeightLayer > > m_holder_layer;
      FIHolder< CLEOConstants< DBDRWeightDrift > > m_holder_drift;
      FIHolder< CLEOConstants< DBDRWeightTheta > > m_holder_theta;
//        FIHolder< CLEOConstants< DBDRWeightPhi   > > m_holder_phi;
//        FIHolder< CLEOConstants< DBDRWeightAlpha > > m_holder_alpha;

      JobControl* m_jc;
      HIHistoManager* m_hm;
      HIHist1D* m_edrift_diff;
      HIHist1D* m_edrift_ratio;
      HIHist1D* m_edrift_normdiff;
      HIHist2D* m_edrift_diff_vs_layer;
      HIHist2D* m_edrift_diff_vs_absDrift;
      HIHist2D* m_edrift_diff_vs_cosTheta;
      HIHist2D* m_edrift_vs_layer;
      HIHist2D* m_edrift_vs_absDrift;
      HIHist2D* m_edrift_vs_cosTheta;
      HIHist2D* m_fw_vs_layer;
      HIHist2D* m_fw_vs_absDrift;
      HIHist2D* m_fw_vs_cosTheta;
      HIHist1D* m_track_edrift_miss;
      HIHist1D* m_track_edrift_miss_close;
      HIHist1D* m_residual;
      HIHist1D* m_norm_residual;

      DABoolean m_layer_info_valid;
      double m_layer_full[47];
      double m_layer_factor[47];
      unsigned int m_layer_hits[47];

      DABoolean m_drift_info_valid;
      unsigned int m_drift_num_bins;
      double m_drift_min;
      double m_drift_max;
      vector< double > m_drift_full[47];
      vector< double > m_drift_factor[47];
      vector< double > m_drift_hits[47];

      DABoolean m_theta_info_valid;
      unsigned int m_theta_num_bins;
      double m_theta_min;
      double m_theta_max;
      vector< double > m_theta_full[47];
      vector< double > m_theta_factor[47];
      vector< double > m_theta_hits[47];

//        DABoolean m_phi_info_valid;
//        unsigned int m_phi_num_bins;
//        double m_phi_min;
//        double m_phi_max;
//        vector< double > m_phi_full[47];
//        vector< double > m_phi_factor[47];
//        vector< double > m_phi_hits[47];

//        DABoolean m_alpha_info_valid;
//        unsigned int m_alpha_num_bins;
//        double m_alpha_min;
//        double m_alpha_max;
//        vector< double > m_alpha_full[47];
//        vector< double > m_alpha_factor[47];
//        vector< double > m_alpha_hits[47];

      DABoolean m_layer_geometry_valid;
      unsigned int m_layer_num_cells[47];
      double m_layer_radius[47];

      DRFBin* m_everything;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DRFittingWeights/Template/DRFittingWeights.cc"
//#endif

#endif /* DRFITTINGWEIGHTS_DRFITTINGWEIGHTS_H */
