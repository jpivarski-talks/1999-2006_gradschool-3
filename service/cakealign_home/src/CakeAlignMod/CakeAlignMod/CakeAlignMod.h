// -*- C++ -*-
#if !defined(CAKEALIGNMOD_CAKEALIGNMOD_H)
#define CAKEALIGNMOD_CAKEALIGNMOD_H
//
// Package:     CakeAlignMod
// Module:      CakeAlignMod
//
/**\class CakeAlignMod CakeAlignMod.h CakeAlignMod/CakeAlignMod.h
 
 Description: Suez Module that allows you to control the "event" loop

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Oct  6 13:30:51 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "CommandPattern/Module.h"
#include "CommandPattern/Parameter.h"
#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
#include "ToolBox/HistogramPackage.h"
#include "Utility/Severity.h"

#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "FrameAccess/FAConstants.h"
#include "BDLDRclient/DBDRWeight2LayerDriftEntAng.hh"
#include "BDLZDclient/DBZDWeight2LayerDriftEntAng.hh"
#include "BDLDRclient/DBDRGeomAlignment.hh"

// forward declarations

class HVPWindow;
class QApplication;

class CakeAlignMod : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum {
	 kDRFWs = 1414,
	 kRings = 8,
	 kFiterations = 9
      };

      // ---------- Constructors and destructor ----------------
      CakeAlignMod();
      virtual ~CakeAlignMod();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      ///override this function to do the actual iterations
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );

      int cake_ring(int layer) const;
      double julian_day() const;
      double sun_position(double j) const;
      double moon_position(double j, double ls) const;
      double moon_phase() const;
      HIHistoManager* book_histograms(const FIFrameIterator& iBegin);
      void read_files();
      void init_moves();
      void setup_fittingweights(int test_cake);
      void gather_eventlist(FIFrameIterator itFrame);
      DABoolean not_bhabha(FIFrameIterator itFrame);
      DABoolean not_mupair(FIFrameIterator itFrame);
      DABoolean not_on_eventlist(FIFrameIterator itFrame);
      void make_checker_plots(int fiteration, FIFrameIterator itFrame);
      void make_aligner_plots(int fiteration, FIFrameIterator itFrame);
      void make_shifter_plots(int fiteration);
      void print_status(int fiteration, Severity severity);
      void update_geometry(int fiteration);
      void print_geomfile(int fiteration);
      void print_kumac(int fiteration);

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      CakeAlignMod( const CakeAlignMod& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const CakeAlignMod& operator=( const CakeAlignMod& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      Parameter<string> m_drfwName;
      Parameter<string> m_geometryName;
      Parameter<unsigned int> m_numEvents;
      Parameter<double> m_momentumBound;
      Parameter<double> m_d0missBound;
      Parameter<double> m_residBound;
      Parameter<DABoolean> m_useMuPair;
      Parameter<DABoolean> m_dualConstraint;
      Parameter<double> m_halfCrossAngleRadians;
      Parameter<DABoolean> m_showPlots;
      Parameter<unsigned int> m_printoutFrequency;

      CLEOConstantsModifiable<DBDRWeight2LayerDriftEntAng> m_drfw;
      CLEOConstantsModifiable<DBDRWeight2LayerDriftEntAng> m_drfwbackup;
      CLEOConstantsModifiable<DBDRGeomAlignment> m_geometry;

      FIHolder<CLEOConstants<DBDRWeight2LayerDriftEntAng> > m_drfw_holder;
      FIHolder<CLEOConstants<DBDRGeomAlignment> > m_geometry_holder;

      pair<unsigned int, unsigned int>* m_eventlist;
      unsigned int m_eventlist_pointer;

      vector<HIHist1D*> m_hist1d[kFiterations];
      vector<HIHistProf*> m_histprof[kFiterations];

      HIHist1D*   m_posP[kFiterations];
      HIHist1D*   m_negP[kFiterations];
      HIHist1D*   m_d0miss[kFiterations];
      HIHistProf* m_posP_vphi0[kFiterations];
      HIHistProf* m_negP_vphi0[kFiterations];
      HIHistProf* m_d0miss_vphi0[kFiterations];
      HIHistProf* m_posP_vcT[kFiterations];
      HIHistProf* m_negP_vcT[kFiterations];
      HIHistProf* m_d0miss_vcT[kFiterations];
      HIHistProf* m_posP_vphi0_ctm4[kFiterations];
      HIHistProf* m_negP_vphi0_ctm4[kFiterations];
      HIHistProf* m_d0miss_vphi0_ctm4[kFiterations];
      HIHistProf* m_posP_vphi0_ctm3[kFiterations];
      HIHistProf* m_negP_vphi0_ctm3[kFiterations];
      HIHistProf* m_d0miss_vphi0_ctm3[kFiterations];
      HIHistProf* m_posP_vphi0_ctm2[kFiterations];
      HIHistProf* m_negP_vphi0_ctm2[kFiterations];
      HIHistProf* m_d0miss_vphi0_ctm2[kFiterations];
      HIHistProf* m_posP_vphi0_ctm1[kFiterations];
      HIHistProf* m_negP_vphi0_ctm1[kFiterations];
      HIHistProf* m_d0miss_vphi0_ctm1[kFiterations];
      HIHistProf* m_posP_vphi0_ctp1[kFiterations];
      HIHistProf* m_negP_vphi0_ctp1[kFiterations];
      HIHistProf* m_d0miss_vphi0_ctp1[kFiterations];
      HIHistProf* m_posP_vphi0_ctp2[kFiterations];
      HIHistProf* m_negP_vphi0_ctp2[kFiterations];
      HIHistProf* m_d0miss_vphi0_ctp2[kFiterations];
      HIHistProf* m_posP_vphi0_ctp3[kFiterations];
      HIHistProf* m_negP_vphi0_ctp3[kFiterations];
      HIHistProf* m_d0miss_vphi0_ctp3[kFiterations];
      HIHistProf* m_posP_vphi0_ctp4[kFiterations];
      HIHistProf* m_negP_vphi0_ctp4[kFiterations];
      HIHistProf* m_d0miss_vphi0_ctp4[kFiterations];

      HIHist1D*   m_just_resid[kFiterations];
      HIHistProf* m_east_resid[kFiterations];
      HIHistProf* m_east_cosresid[kFiterations];
      HIHistProf* m_east_sinresid[kFiterations];
      HIHistProf* m_west_resid[kFiterations];
      HIHistProf* m_west_cosresid[kFiterations];
      HIHistProf* m_west_sinresid[kFiterations];

      HIHist1D*   m_east_rotz[kFiterations];
      HIHist1D*   m_east_x[kFiterations];
      HIHist1D*   m_east_y[kFiterations];
      HIHist1D*   m_west_rotz[kFiterations];
      HIHist1D*   m_west_x[kFiterations];
      HIHist1D*   m_west_y[kFiterations];
      HIHist1D*   m_east_rotz_significance[kFiterations];
      HIHist1D*   m_east_x_significance[kFiterations];
      HIHist1D*   m_east_y_significance[kFiterations];
      HIHist1D*   m_west_rotz_significance[kFiterations];
      HIHist1D*   m_west_x_significance[kFiterations];
      HIHist1D*   m_west_y_significance[kFiterations];

      double      m_west_rotz_move[kFiterations][kRings];
      double      m_east_rotz_move[kFiterations][kRings];
      double      m_west_x_move[kFiterations][kRings];
      double      m_east_x_move[kFiterations][kRings];
      double      m_west_y_move[kFiterations][kRings];
      double      m_east_y_move[kFiterations][kRings];
      double      m_west_rotz_err[kFiterations][kRings];
      double      m_east_rotz_err[kFiterations][kRings];
      double      m_west_x_err[kFiterations][kRings];
      double      m_east_x_err[kFiterations][kRings];
      double      m_west_y_err[kFiterations][kRings];
      double      m_east_y_err[kFiterations][kRings];

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "CakeAlignMod/Template/CakeAlignMod.cc"
//#endif

#endif /* CAKEALIGNMOD_CAKEALIGNMOD_H */
