#if !defined(HISTOGRAMVIEWER_HVHISTOGRAMSLISTVIEW_H)
#define HISTOGRAMVIEWER_HVHISTOGRAMSLISTVIEW_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogramsListView
// 
/**\class HVHistogramsListView HVHistogramsListView.h HistogramViewer/HVHistogramsListView.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 08:31:26 EDT 2001
// $Id: HVHistogramsListView.h,v 1.3 2001/09/21 03:24:20 mccann Exp $
//
// Revision history
//
// $Log: HVHistogramsListView.h,v $
// Revision 1.3  2001/09/21 03:24:20  mccann
// Implemented profile histograms
//
// Revision 1.2  2001/09/19 16:01:01  cdj
// generalized to allow more than 1D histograms
//
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include <qlistview.h>

// forward declarations
class HIHistoManager;
class HIHist1D;
class HIHist2D;
class HIHistProf;

class HVHistogramsListView : public QListView
{
      // ---------- friend classes and functions ---------------
      Q_OBJECT
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVHistogramsListView(const HIHistoManager* iManager, 
			   QWidget* iParent, const char* iName ="");
      virtual ~HVHistogramsListView();

      // ---------- member functions ---------------------------
      void updateHistogramList();

 public slots:
      void slotDoubleClicked( QListViewItem* );
      void slotReturnPressed( QListViewItem* );

      signals:
        void doubleClicked( const HIHist1D* );
        void returnPressed( const HIHist1D* );

        void doubleClicked( const HIHist2D* );
        void returnPressed( const HIHist2D* );

        void doubleClicked( const HIHistProf* );
        void returnPressed( const HIHistProf* );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVHistogramsListView( const HVHistogramsListView& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVHistogramsListView& operator=( const HVHistogramsListView& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVHistogramsListView.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVHISTOGRAMSLISTVIEW_H */
