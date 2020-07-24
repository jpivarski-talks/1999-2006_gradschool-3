#if !defined(DRFITTINGWEIGHTS_DRFBIN_H)
#define DRFITTINGWEIGHTS_DRFBIN_H
// -*- C++ -*-
//
// Package:     <DRFittingWeights>
// Module:      DRFBin
// 
/**\class DRFBin DRFBin.h DRFittingWeights/DRFBin.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Wed Oct 24 16:34:37 EDT 2001
// $Id$
//
// Revision history
//
// $Log$

// system include files

// user include files
#include "DRFittingWeights/DRFittingWeights.h"
#include "HistogramInterface/HIHistFitGauss.h"

// forward declarations

#define DRF_HISTBOUND 7.
#define DRF_HISTBINS 200

class DRFittingWeights;

class DRFBin
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef enum {
	 kEmpty,
	 kFilling,
	 kFilled,
	 kUpdated,
	 kOptimized,
	 kFailed
      } State;

      // ---------- Constructors and destructor ----------------
      DRFBin();
      DRFBin( HIHistoManager* hm, unsigned int id,
	      string& nameBase, unsigned int bin, double bottom, double top,
	      unsigned int target, unsigned int iterations );
      virtual ~DRFBin();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      string name();
      unsigned int hits();
      State state();
      double lambda();
      double chi2ndf();

      void reset();
      void take( double residual, double edrift2, double edca2,
		 unsigned int layer, double drift, double cosTheta,
		 double phi, double sinAlpha );
      void update( DRFittingWeights* that ); // to get m_fw_already
      State optimize();
      void copyHist( unsigned int iteration, ostream& kumac );

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DRFBin( const DRFBin& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DRFBin& operator=( const DRFBin& ); // stop default

      // ---------- private member functions -------------------

      // each of these is called by the next:
      //   resets and fills workingHist
      void fillHist( double lambda );
      //   fits and returns mean^2 + sigma^2 or -1. for fit failure
      double residwidth( double lambda );
      //   returns the best value between left and right (recursive)
      //   or -2. for below range, -1. for above range
      pair< double, double > midpoint( pair< double, double > left,
				       pair< double, double > right );
      //   tries lambda = zero, two, four... until it finds a sign
      //   change or twenty (in which case it fails with value -1).
      double scan();

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      string m_name;
      unsigned int m_id;
      HIHist1D* m_workingHist;
      vector< HIHist1D* > m_histLog;
      unsigned int m_target;
      unsigned int m_iterations;
      unsigned int m_hits;
      State m_state;
      double m_lambda;
      double m_chi2ndf;
      HIHistFitGauss* m_fitfunc;

      vector< double > m_fw2_already;
      vector< double > m_residual;
      vector< double > m_edrift2;
      vector< double > m_edca2;
      vector< unsigned int > m_layer;
      vector< double > m_drift;
      vector< double > m_cosTheta;
//        vector< double > m_phi;
//        vector< double > m_sinAlpha;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DRFittingWeights/Template/DRFBin.cc"
//#endif

#endif /* DRFITTINGWEIGHTS_DRFBIN_H */
