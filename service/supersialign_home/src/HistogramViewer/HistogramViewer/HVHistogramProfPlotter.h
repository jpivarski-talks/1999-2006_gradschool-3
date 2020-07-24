#if !defined(HISTOGRAMVIEWER_HVHISTOGRAMPROFPLOTTER_H)
#define HISTOGRAMVIEWER_HVHISTOGRAMPROFPLOTTER_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogramProfPlotter
// 
/**\class HVHistogramProfPlotter HVHistogramProfPlotter.h HistogramViewer/HVHistogramProfPlotter.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Sep 19 10:59:23 EDT 2001
// $Id: HVHistogramProfPlotter.h,v 1.1 2001/09/21 17:12:36 mccann Exp $
//
// Revision history
//
// $Log: HVHistogramProfPlotter.h,v $
// Revision 1.1  2001/09/21 17:12:36  mccann
// added HVHistogramProfilePlotter with cvs add
//
// Revision 1.1  2001/09/19 16:01:43  cdj
// first submission
//

// system include files

// user include files
#include "HistogramViewer/HVHistogramPlotter.h"
#include "MinuitInterface/MinuitInterface.h"
#include "MinuitInterface/MIFcn.h"

// forward declarations
class HIHistProf;

class HVHistogramProfPlotter : public HVHistogramPlotter, public MIFcn
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVHistogramProfPlotter(const HIHistProf*,
			   QWidget* iParent=0,
			   const char* iName = "" );
      virtual ~HVHistogramProfPlotter();

      // ---------- member functions ---------------------------
      void retrieveNewValues();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      double iterate( double* values );

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVHistogramProfPlotter( const HVHistogramProfPlotter& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVHistogramProfPlotter& operator=( const HVHistogramProfPlotter& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const HIHistProf* m_histogram;
      const unsigned int m_numberOfChannels;
      long* m_errorbar_curves;

      MinuitInterface* m_mi;
      long m_fit;
      long m_oldcurve;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVHistogramProfPlotter.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVHISTOGRAMPROFPLOTTER_H */
