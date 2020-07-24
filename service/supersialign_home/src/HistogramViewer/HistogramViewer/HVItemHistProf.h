#if !defined(HISTOGRAMVIEWER_HVITEMHISTPROF_H)
#define HISTOGRAMVIEWER_HVITEMHISTPROF_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemHistProf
// 
/**\class HVItemHistProf HVItemHistProf.h HistogramViewer/HVItemHistProf.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 13:11:56 EDT 2001
// $Id: HVItemHistProf.h,v 1.1 2001/09/25 19:38:37 mccann Exp $
//
// Revision history
//
// $Log: HVItemHistProf.h,v $
// Revision 1.1  2001/09/25 19:38:37  mccann
// forgot to put HVItemHistProf in the repository
//
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include "HistogramViewer/HVItemBase.h"

// forward declarations
class HIHistProf;

class HVItemHistProf : public HVItemBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVItemHistProf(QListViewItem* iParent, const HIHistProf* );

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      Types type() const { return kProfile; }
      const HIHistProf* histogram() const { return m_hist;}

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVItemHistProf( const HVItemHistProf& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVItemHistProf& operator=( const HVItemHistProf& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const HIHistProf* m_hist;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVItemHistProf.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVITEMHISTPROF_H */
