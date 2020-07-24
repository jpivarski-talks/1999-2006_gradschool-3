#if !defined(HISTOGRAMVIEWER_HVITEMBASE_H)
#define HISTOGRAMVIEWER_HVITEMBASE_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVItemBase
// 
/**\class HVItemBase HVItemBase.h HistogramViewer/HVItemBase.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 12:51:39 EDT 2001
// $Id: HVItemBase.h,v 1.1.1.1 2001/07/30 14:11:32 cdj Exp $
//
// Revision history
//
// $Log: HVItemBase.h,v $
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include "qlistview.h"

// forward declarations

class HVItemBase : public QListViewItem
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      enum Types {kDir, kHist1D, kHist2D, kProfile, kNtuple, kNTypes};

      // ---------- Constructors and destructor ----------------
      virtual ~HVItemBase();

      // ---------- member functions ---------------------------
      virtual void expanded();
      virtual void collapsed();
      virtual void updateInfo();

      // ---------- const member functions ---------------------
      virtual Types type() const = 0;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      HVItemBase(QListViewItem* iParent,
		 const QString& iName, 
		 const QString& iType, 
		 unsigned int iIdentifier);

      HVItemBase(QListView* iParent,
		 const QString& iName, 
		 const QString& iType, 
		 unsigned int iIdentifier);

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVItemBase( const HVItemBase& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVItemBase& operator=( const HVItemBase& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVItemBase.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVITEMBASE_H */
