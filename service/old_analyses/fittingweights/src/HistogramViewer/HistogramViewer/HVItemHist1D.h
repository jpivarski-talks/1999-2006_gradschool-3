#if !defined(HISTOGRAMVIEWER_HVITEMHIST1D_H)
#define HISTOGRAMVIEWER_HVITEMHIST1D_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemHist1D
// 
/**\class HVItemHist1D HVItemHist1D.h HistogramViewer/HVItemHist1D.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 13:11:56 EDT 2001
// $Id: HVItemHist1D.h,v 1.1.1.1 2001/07/30 14:11:32 cdj Exp $
//
// Revision history
//
// $Log: HVItemHist1D.h,v $
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include "HistogramViewer/HVItemBase.h"

// forward declarations
class HIHist1D;

class HVItemHist1D : public HVItemBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVItemHist1D(QListViewItem* iParent, const HIHist1D* );

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      Types type() const { return kHist1D; }
      const HIHist1D* histogram() const { return m_hist;}

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVItemHist1D( const HVItemHist1D& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVItemHist1D& operator=( const HVItemHist1D& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const HIHist1D* m_hist;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVItemHist1D.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVITEMHIST1D_H */
