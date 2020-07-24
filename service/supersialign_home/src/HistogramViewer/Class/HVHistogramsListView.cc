// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogramsListView
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Jul 18 14:58:49 EDT 2001
// $Id: HVHistogramsListView.cc,v 1.3 2001/09/21 03:24:17 mccann Exp $
//
// Revision history
//
// $Log: HVHistogramsListView.cc,v $
// Revision 1.3  2001/09/21 03:24:17  mccann
// Implemented profile histograms
//
// Revision 1.2  2001/09/19 16:00:58  cdj
// generalized to allow more than 1D histograms
//
// Revision 1.1.1.1  2001/07/30 14:11:33  cdj
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
//#include "Experiment/report.h"
#include "HistogramViewer/HVHistogramsListView.h"
#include "HistogramViewer/HVItemDir.h"
#include "HistogramViewer/HVItemHist1D.h"
#include "HistogramViewer/HVItemHist2D.h"
#include "HistogramViewer/HVItemHistProf.h"
#include "HistogramInterface/HIHistoManager.h"
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVHistogramsListView" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVHistogramsListView.cc,v 1.3 2001/09/21 03:24:17 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVHistogramsListView::HVHistogramsListView(const HIHistoManager* iManager,
					   QWidget* iParent,
					   const char* iName) :
   QListView(iParent, iName)
{
   addColumn("Name");
   addColumn("Type");
   addColumn("Id");
   setRootIsDecorated(true);
   insertItem( new HVItemDir( this, iManager->rootDir() ) );

   connect( this, SIGNAL(returnPressed(QListViewItem*)),
	    this, SLOT(slotReturnPressed(QListViewItem*)) );
   connect( this, SIGNAL(doubleClicked(QListViewItem*)),
	    this, SLOT(slotDoubleClicked(QListViewItem*)));

}

// HVHistogramsListView::HVHistogramsListView( const HVHistogramsListView& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVHistogramsListView::~HVHistogramsListView()
{
}

//
// assignment operators
//
// const HVHistogramsListView& HVHistogramsListView::operator=( const HVHistogramsListView& rhs )
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
HVHistogramsListView::slotDoubleClicked(QListViewItem* iItem)
{
   if( HVItemBase::kHist1D == reinterpret_cast<HVItemBase*>(iItem)->type() ) {
      emit doubleClicked(reinterpret_cast<HVItemHist1D*>(iItem)->histogram() );
   }
   if( HVItemBase::kHist2D == reinterpret_cast<HVItemBase*>(iItem)->type() ) {
      emit doubleClicked(reinterpret_cast<HVItemHist2D*>(iItem)->histogram() );
   }
   if( HVItemBase::kProfile == reinterpret_cast<HVItemBase*>(iItem)->type() ) {
      emit doubleClicked(reinterpret_cast<HVItemHistProf*>(iItem)->histogram() );
   }
}

void
HVHistogramsListView::slotReturnPressed(QListViewItem* iItem)
{
   if( HVItemBase::kHist1D == reinterpret_cast<HVItemBase*>(iItem)->type() ) {
      emit returnPressed(reinterpret_cast<HVItemHist1D*>(iItem)->histogram() );
   }
   if( HVItemBase::kHist2D == reinterpret_cast<HVItemBase*>(iItem)->type() ) {
      emit returnPressed(reinterpret_cast<HVItemHist2D*>(iItem)->histogram() );
   }
   if( HVItemBase::kProfile == reinterpret_cast<HVItemBase*>(iItem)->type() ) {
      emit returnPressed(reinterpret_cast<HVItemHistProf*>(iItem)->histogram() );
   }
}
//
// const member functions
//

//
// static member functions
//
#include "HVHistogramsListView.moc"
