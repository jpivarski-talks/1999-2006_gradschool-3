#if !defined(HISTOGRAMVIEWER_HVHISTOGRAM2DPLOTTER_H)
#define HISTOGRAMVIEWER_HVHISTOGRAM2DPLOTTER_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogram2DPlotter
// 
/**\class HVHistogram2DPlotter HVHistogram2DPlotter.h HistogramViewer/HVHistogram2DPlotter.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Sep 19 10:59:23 EDT 2001
// $Id: HVHistogram2DPlotter.h,v 1.2 2002/05/06 17:51:40 mccann Exp $
//
// Revision history
//
// $Log: HVHistogram2DPlotter.h,v $
// Revision 1.2  2002/05/06 17:51:40  mccann
// improved 2d histogram plotting--- now it works like the box option in PAW
//
// Revision 1.1  2001/09/19 16:01:44  cdj
// first submission
//

// system include files

// user include files
#include "HistogramViewer/HVHistogramPlotter.h"

// forward declarations
class HIHist2D;

class HVHistogram2DPlotter : public HVHistogramPlotter
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVHistogram2DPlotter(const HIHist2D*,
			   QWidget* iParent=0,
			   const char* iName = "" );
      virtual ~HVHistogram2DPlotter();

      // ---------- member functions ---------------------------
      void retrieveNewValues();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVHistogram2DPlotter( const HVHistogram2DPlotter& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVHistogram2DPlotter& operator=( const HVHistogram2DPlotter& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const HIHist2D* m_histogram;

      long* m_drawBin;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVHistogram2DPlotter.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVHISTOGRAM2DPLOTTER_H */
