#if !defined(HISTOGRAMVIEWER_HVHISTOGRAM1DPLOTTER_H)
#define HISTOGRAMVIEWER_HVHISTOGRAM1DPLOTTER_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogram1DPlotter
// 
/**\class HVHistogram1DPlotter HVHistogram1DPlotter.h HistogramViewer/HVHistogram1DPlotter.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Sep 19 10:59:23 EDT 2001
// $Id: HVHistogram1DPlotter.h,v 1.1 2001/09/19 16:01:43 cdj Exp $
//
// Revision history
//
// $Log: HVHistogram1DPlotter.h,v $
// Revision 1.1  2001/09/19 16:01:43  cdj
// first submission
//

// system include files

// user include files
#include "HistogramViewer/HVHistogramPlotter.h"

// forward declarations
class HIHist1D;

class HVHistogram1DPlotter : public HVHistogramPlotter
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVHistogram1DPlotter(const HIHist1D*,
			   QWidget* iParent=0,
			   const char* iName = "" );
      virtual ~HVHistogram1DPlotter();

      // ---------- member functions ---------------------------
      void retrieveNewValues();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      long fitId() const { return m_fitId; }

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVHistogram1DPlotter( const HVHistogram1DPlotter& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVHistogram1DPlotter& operator=( const HVHistogram1DPlotter& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const HIHist1D* m_histogram;
      long m_fitId;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVHistogram1DPlotter.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVHISTOGRAM1DPLOTTER_H */
