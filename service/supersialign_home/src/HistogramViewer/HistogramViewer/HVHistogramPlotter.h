#if !defined(HISTOGRAMVIEWER_HVHISTOGRAMPLOTTER_H)
#define HISTOGRAMVIEWER_HVHISTOGRAMPLOTTER_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogramPlotter
// 
/**\class HVHistogramPlotter HVHistogramPlotter.h HistogramViewer/HVHistogramPlotter.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Sat Jul 14 14:46:16 EDT 2001
// $Id: HVHistogramPlotter.h,v 1.2 2001/09/19 16:01:01 cdj Exp $
//
// Revision history
//
// $Log: HVHistogramPlotter.h,v $
// Revision 1.2  2001/09/19 16:01:01  cdj
// generalized to allow more than 1D histograms
//
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include "qwt_plot.h"

// forward declarations

class HVHistogramPlotter : public QwtPlot
{
      // ---------- friend classes and functions ---------------
      Q_OBJECT

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVHistogramPlotter(const char* iHistogramName,
			 QWidget* iParent=0,
			 const char* iName="" );
      virtual ~HVHistogramPlotter();

      // ---------- member functions ---------------------------
      virtual void retrieveNewValues() = 0;

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      
      // ---------- protected const member functions -----------
      long curveId() const { return m_curveId; }

   private:
      // ---------- Constructors and destructor ----------------
      HVHistogramPlotter( const HVHistogramPlotter& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVHistogramPlotter& operator=( const HVHistogramPlotter& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      long m_curveId;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVHistogramPlotter.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVHISTOGRAMPLOTTER_H */
