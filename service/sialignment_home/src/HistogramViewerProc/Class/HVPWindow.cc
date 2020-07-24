// -*- C++ -*-
//
// Package:     <HistogramViewerProc>
// Module:      HVPWindow
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri Jul 20 09:51:38 EDT 2001
// $Id: HVPWindow.cc,v 1.3 2001/09/21 03:25:05 mccann Exp $
//
// Revision history
//
// $Log: HVPWindow.cc,v $
// Revision 1.3  2001/09/21 03:25:05  mccann
// Implemented profile histograms
//
// Revision 1.2  2001/09/19 16:02:13  cdj
// added 2D histogram capability
//
// Revision 1.1.1.1  2001/07/30 14:11:45  cdj
// importing
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
#include "qsplitter.h"
#include "qmenubar.h"
#include "qhbox.h"
#include "qvbox.h"
#include "qlayout.h"
#include "qpushbutton.h"
#include "qapplication.h"
#include "qlabel.h"
#include "qlineedit.h" 
#include "qvalidator.h"
#include "qtextview.h"

//#include "Experiment/report.h"
#include "HistogramViewerProc/HVPWindow.h"

#include "HistogramViewer/HVHistogramsListView.h"
#include "HistogramViewer/HVHistogramsViewer.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewerProc.HVPWindow" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVPWindow.cc,v 1.3 2001/09/21 03:25:05 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVPWindow::HVPWindow(HIHistoManager* iManager,
		     QWidget* iParent,
		     const char* iName) :
   QWidget(iParent, iName),
   m_listView(0),
   m_viewer(0),
   m_interactButton(0),
   m_continueButton(0),
   m_interactionRequested(false),
   m_continueRequested(false),
   m_frequency(1)
{
   QVBoxLayout* layout = new QVBoxLayout( this );

   QHBox* box = new QHBox(this);
   box->setFrameStyle( QFrame::Box | QFrame::Sunken );
   layout->addWidget( box );

   QLabel* frequencyLabel = new QLabel("Update Frequency:",box);
   QLineEdit* frequencyEditor= new QLineEdit(QString::number(m_frequency), box);
   QValidator* validator = new QIntValidator(1, 100000000, frequencyEditor);
   frequencyEditor->setValidator(validator);
   connect(frequencyEditor, SIGNAL(textChanged( const QString&)),
           this, SLOT(slotFrequencyText(const QString&)));
   
   QVBox* buttonBox = new QVBox(box);
   m_interactButton = new QPushButton(buttonBox);
   m_interactButton->setText("Interact");
   m_continueButton = new QPushButton(buttonBox);
   m_continueButton->setText("Continue");
   m_interactButton->setEnabled(false);

   connect(m_interactButton, SIGNAL(clicked()),
	   this, SLOT(slotInteract()));

   connect(m_continueButton, SIGNAL(clicked()),
	   this, SLOT(slotContinue()));

   QSplitter* splitter = new QSplitter(this);
   layout->addWidget(splitter);

   QMenuBar* menuBar = new QMenuBar(this);
   layout->setMenuBar(menuBar);

   QPopupMenu* fileMenu = new QPopupMenu(menuBar);
   fileMenu->insertItem("Continue", this, SLOT(slotContinue()));
   menuBar->insertItem("File", fileMenu);

   m_plotsMenu = new QPopupMenu(menuBar);
   QObject::connect(m_plotsMenu, SIGNAL(aboutToShow()),
                    this, SLOT(slotPlotsMenuAboutToShow()) );
   menuBar->insertItem("Plots", m_plotsMenu);
   
   m_listView = new HVHistogramsListView(iManager,
							     splitter);

   m_viewer = new HVHistogramsViewer(splitter);

   //add help window
   QLabel* helpWindow = new QLabel(m_viewer);
   helpWindow->setCaption("Instructions");
   QString instructions =
      QString("1) Select histograms to display by double clicking \n  on the histogram in the directory listing to the left.\n")+
      QString("2) Press \"Continue\" button to watch histograms fill.\n")+
      QString("3) Press \"Interact\" button while histograms are\n   filling to be able to modify histograms.\n")+
      QString("4) If histogram filling is to slow, increase the \"Update Frequency\".");
   helpWindow->setText(instructions);
   QObject::connect(m_listView, SIGNAL(doubleClicked(const HIHist1D*)),
		     m_viewer, SLOT(createPlot(const HIHist1D*)) );
   QObject::connect(m_listView, SIGNAL(doubleClicked(const HIHist2D*)),
		     m_viewer, SLOT(createPlot(const HIHist2D*)) );
   QObject::connect(m_listView, SIGNAL(doubleClicked(const HIHistProf*)),
		     m_viewer, SLOT(createPlot(const HIHistProf*)) );

}

// HVPWindow::HVPWindow( const HVPWindow& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVPWindow::~HVPWindow()
{
}

//
// assignment operators
//
// const HVPWindow& HVPWindow::operator=( const HVPWindow& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//
void
HVPWindow::updateInfo()
{
   m_viewer->updateHistograms();
}

void
HVPWindow::slotInteract() {
   m_interactionRequested=true;
   m_continueRequested=false;
   m_interactButton->setEnabled(false);
   m_continueButton->setEnabled(true);
}

void
HVPWindow::slotContinue() {
   m_continueRequested=true;
   m_interactionRequested=false;

   m_interactButton->setEnabled(true);
   m_continueButton->setEnabled(false);
   qApp->processEvents();

   qApp->quit();
}

void
HVPWindow::slotFrequencyText(const QString& iValue)
{
   m_frequency = iValue.toUInt();
}

void
HVPWindow::slotPlotsMenuAboutToShow()
{
     m_plotsMenu->clear();
    int tileId = m_plotsMenu->insertItem("&Tile", m_viewer, SLOT(tile() ) )
;
    int cascadeId = m_plotsMenu->insertItem("&Cascade", m_viewer, SLOT(cascade() ) );
    if ( m_viewer->windowList().isEmpty() ) {
        m_plotsMenu->setItemEnabled( cascadeId, FALSE );
        m_plotsMenu->setItemEnabled( tileId, FALSE );
    }
    m_plotsMenu->insertSeparator();
    QWidgetList windows = m_viewer->windowList();
    for ( int i = 0; i < int(windows.count()); ++i ) {
       //We do not want to  show the Instruction window in the list of plots
       if( ! windows.at(i)->inherits("QLabel") ) {
	  int id = m_plotsMenu->insertItem(windows.at(i)->caption(),
					   this, 
					   SLOT( slotPlotsMenuActivated( int )
					      ) );
	  m_plotsMenu->setItemParameter( id, i );
	  m_plotsMenu->setItemChecked( id, m_viewer->activeWindow() == windows.at(i) );
       }
    }

}

void 
HVPWindow::slotPlotsMenuActivated( int id )
{
    QWidget* w = m_viewer->windowList().at( id );
    if ( w ) { w->setFocus(); }
}

//
// const member functions
//

//
// static member functions
//
#include "Class/HVPWindow.moc"
