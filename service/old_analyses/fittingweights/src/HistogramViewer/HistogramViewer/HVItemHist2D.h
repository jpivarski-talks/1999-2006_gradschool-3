#if !defined(HISTOGRAMVIEWER_HVITEMHIST2D_H)
#define HISTOGRAMVIEWER_HVITEMHIST2D_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemHist2D
// 
/**\class HVItemHist2D HVItemHist2D.h HistogramViewer/HVItemHist2D.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 13:11:56 EDT 2001
// $Id: HVItemHist2D.h,v 1.1 2001/09/19 16:01:44 cdj Exp $
//
// Revision history
//
// $Log: HVItemHist2D.h,v $
// Revision 1.1  2001/09/19 16:01:44  cdj
// first submission
//
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include "HistogramViewer/HVItemBase.h"

// forward declarations
class HIHist2D;

class HVItemHist2D : public HVItemBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVItemHist2D(QListViewItem* iParent, const HIHist2D* );

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      Types type() const { return kHist2D; }
      const HIHist2D* histogram() const { return m_hist;}

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVItemHist2D( const HVItemHist2D& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVItemHist2D& operator=( const HVItemHist2D& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const HIHist2D* m_hist;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVItemHist2D.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVITEMHIST2D_H */
