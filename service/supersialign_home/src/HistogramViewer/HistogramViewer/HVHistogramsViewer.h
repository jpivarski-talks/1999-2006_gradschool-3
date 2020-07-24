#if !defined(HISTOGRAMVIEWER_HVHISTOGRAMSVIEWER_H)
#define HISTOGRAMVIEWER_HVHISTOGRAMSVIEWER_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogramsViewer
// 
/**\class HVHistogramsViewer HVHistogramsViewer.h HistogramViewer/HVHistogramsViewer.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Sat Jul 14 14:34:37 EDT 2001
// $Id: HVHistogramsViewer.h,v 1.3 2001/09/21 03:24:21 mccann Exp $
//
// Revision history
//
// $Log: HVHistogramsViewer.h,v $
// Revision 1.3  2001/09/21 03:24:21  mccann
// Implemented profile histograms
//
// Revision 1.2  2001/09/19 16:01:02  cdj
// generalized to allow more than 1D histograms
//
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include <qworkspace.h>

// forward declarations
class HIHist1D;
class HIHist2D;
class HIHistProf;

class HVHistogramsViewer : public QWorkspace
{
      // ---------- friend classes and functions ---------------

      Q_OBJECT
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVHistogramsViewer(QWidget* iParent=0, const char* iName ="");
      virtual ~HVHistogramsViewer();

      // ---------- member functions ---------------------------
 public slots:
      void createPlot(const HIHist1D* iHist);
      void createPlot(const HIHist2D* iHist);
      void createPlot(const HIHistProf* iHist);
      void updateHistograms();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVHistogramsViewer( const HVHistogramsViewer& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVHistogramsViewer& operator=( const HVHistogramsViewer& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVHistogramsViewer.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVHISTOGRAMSVIEWER_H */
