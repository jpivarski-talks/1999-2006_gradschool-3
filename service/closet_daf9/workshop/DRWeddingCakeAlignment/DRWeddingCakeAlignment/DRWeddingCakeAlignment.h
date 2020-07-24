// -*- C++ -*-
#if !defined(DRWEDDINGCAKEALIGNMENT_DRWEDDINGCAKEALIGNMENT_H)
#define DRWEDDINGCAKEALIGNMENT_DRWEDDINGCAKEALIGNMENT_H
//
// Package:     DRWeddingCakeAlignment
// Module:      DRWeddingCakeAlignment
//
/**\class DRWeddingCakeAlignment DRWeddingCakeAlignment.h DRWeddingCakeAlignment/DRWeddingCakeAlignment.h
 
 Description: Suez Module that allows you to control the "event" loop

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Tue Jul 17 10:31:50 EDT 2001
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
#include "FrameIterate/FIHolder.h"
#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "BDLDRclient/DBDRGeomAlignment.hh"
#include "CommandPattern/Parameter.h"
#include "MinuitInterface/MinuitInterface.h"
#include "MinuitInterface/MIFcn.h"

#include "CLHEP/Vector/ThreeVector.h"

#include <fstream.h>
#include <iostream.h>
#include <exception.h>
#include <stdexcept>

// external functions
extern "C" float prob_( const float&, const int& );

// forward declarations

class DRWeddingCakeAlignment : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DRWeddingCakeAlignment();
      virtual ~DRWeddingCakeAlignment();

      // ---------- member functions ---------------------------

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
      DRWeddingCakeAlignment( const DRWeddingCakeAlignment& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DRWeddingCakeAlignment& operator=( const DRWeddingCakeAlignment& ); // stop default

      // ---------- private member functions -------------------

      void print_change( ostream &os, MinuitInterface* mi );
      void print_fancy_errors( ostream &os, MinuitInterface* mi );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      Parameter< unsigned int > m_cake;
      Parameter< unsigned int > m_minhits;
      Parameter< string > m_inputConst;

      CLEOConstantsModifiable< DBDRGeomAlignment > m_geomalignment;
      FIHolder< CLEOConstants< DBDRGeomAlignment > > m_geomalignmentHolder;

      double m_start_east_deltaX;
      double m_start_east_deltaY;
      double m_start_east_rotZ;
      double m_start_west_deltaX;
      double m_start_west_deltaY;
      double m_start_west_rotZ;

      // ---------- static data members ------------------------

      // ---------- nested classes -----------------------------

   public:
      class histogramBundle
      {
	 public:
	    histogramBundle( HIHistoManager* hm, unsigned int iteration );

	    HIHist1D* dca;
	    HIHist1D* e_dca;
	    HIHist1D* drift;
	    HIHist1D* e_drift;
	    HIHist1D* residual;
	    HIHist1D* e_residual;
	    HIHist1D* norm_resid;

//  	    HIHist2D* resid_v_layer;
      };

      class residualAlignment : public MIFcn
      {
	 public:
	    enum {
	       kWestDeltaX,
	       kWestDeltaY,
	       kWestRotZ,
	       kEastDeltaX,
	       kEastDeltaY,
	       kEastRotZ,
	       kNumParams,

	       khistBundles = 20
	    };

	    residualAlignment( const FIFrameIterator* begin,
			       const FIFrameIterator* end,
			       unsigned int cake,
			       unsigned int minhits,
			       CLEOConstantsModifiable< DBDRGeomAlignment >* geomConst,
			       HIHistoManager* hm );
	    ~residualAlignment();

	    double iterate( double* values );
	    double last_time();

	 protected:
	    residualAlignment() { ; }
	    
	 private:
	    const FIFrameIterator* mm_begin;
	    const FIFrameIterator* mm_end;
	    unsigned int mm_cake;
	    unsigned int mm_minhits;
	    CLEOConstantsModifiable< DBDRGeomAlignment >* mm_geomConst;
	    DABoolean mm_draw_last_histograms;
	    DABoolean mm_draw_any_histograms;

	    vector< DRWeddingCakeAlignment::histogramBundle* > mm_hists;
      };
};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DRWeddingCakeAlignment/Template/DRWeddingCakeAlignment.cc"
//#endif

#endif /* DRWEDDINGCAKEALIGNMENT_DRWEDDINGCAKEALIGNMENT_H */
