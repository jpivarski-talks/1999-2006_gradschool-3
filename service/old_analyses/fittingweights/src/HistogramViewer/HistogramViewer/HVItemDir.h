#if !defined(HISTOGRAMVIEWER_HVITEMDIR_H)
#define HISTOGRAMVIEWER_HVITEMDIR_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemDir
// 
/**\class HVItemDir HVItemDir.h HistogramViewer/HVItemDir.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 13:11:46 EDT 2001
// $Id: HVItemDir.h,v 1.1.1.1 2001/07/30 14:11:32 cdj Exp $
//
// Revision history
//
// $Log: HVItemDir.h,v $
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include "HistogramViewer/HVItemBase.h"

// forward declarations
class HIHistDir; 
class HVHistogramsListView;

class HVItemDir : public HVItemBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVItemDir(QListViewItem* iParent, const HIHistDir* iDir );
      HVItemDir(QListView* iParent, const HIHistDir* iDir );
      //virtual ~HVItemDir();

      // ---------- member functions ---------------------------
      virtual void expanded();
      virtual void collapsed();
      virtual void updateInfo();

      // ---------- const member functions ---------------------
      Types type() const { return kDir;}

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVItemDir( const HVItemDir& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVItemDir& operator=( const HVItemDir& ); // stop default

      // ---------- private member functions -------------------
      void makeChildren();

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const HIHistDir* m_dir;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVItemDir.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVITEMDIR_H */
