#if !defined(HISTOGRAMVIEWERPROC_HVPWINDOW_H)
#define HISTOGRAMVIEWERPROC_HVPWINDOW_H
// -*- C++ -*-
//
// Package:     <HistogramViewerProc>
// Module:      HVPWindow
// 
/**\class HVPWindow HVPWindow.h HistogramViewerProc/HVPWindow.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Fri Jul 20 09:27:20 EDT 2001
// $Id: HVPWindow.h,v 1.1.1.1 2001/07/30 14:11:45 cdj Exp $
//
// Revision history
//
// $Log: HVPWindow.h,v $
// Revision 1.1.1.1  2001/07/30 14:11:45  cdj
// importing
//

// system include files

// user include files
#include "qwidget.h"

// forward declarations
class HIHistoManager;
class HVHistogramsListView;
class HVHistogramsViewer;
class QPushButton;
class QPopupMenu;

class HVPWindow : public QWidget
{
      // ---------- friend classes and functions ---------------
      Q_OBJECT

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVPWindow(HIHistoManager* iManager,
		QWidget* iParent=0, const char* iName ="");
      virtual ~HVPWindow();

      // ---------- member functions ---------------------------
      void updateInfo();

      // ---------- const member functions ---------------------
      DABoolean interactionRequested() const {
	 return m_interactionRequested; }

      DABoolean continueRequested() const {
	 return m_continueRequested; }

      unsigned int frequency() const { 
         return m_frequency; 
      }
      // ---------- static member functions --------------------

   public slots:
      void slotInteract();
      void slotContinue();
      void slotFrequencyText(const QString&);
      void slotPlotsMenuAboutToShow();
      void slotPlotsMenuActivated(int);

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVPWindow( const HVPWindow& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVPWindow& operator=( const HVPWindow& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      HVHistogramsListView* m_listView;
      HVHistogramsViewer* m_viewer;

      QPushButton* m_interactButton;
      QPushButton* m_continueButton;
      QPopupMenu* m_plotsMenu;

      DABoolean m_interactionRequested;
      DABoolean m_continueRequested;
      
      unsigned int m_frequency;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewerProc/Template/HVPWindow.cc"
//#endif

#endif /* HISTOGRAMVIEWERPROC_HVPWINDOW_H */
