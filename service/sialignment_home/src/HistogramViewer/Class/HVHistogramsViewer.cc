// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogramsViewer
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Thu Jul 19 14:42:44 EDT 2001
// $Id: HVHistogramsViewer.cc,v 1.5 2001/09/21 03:24:17 mccann Exp $
//
// Revision history
//
// $Log: HVHistogramsViewer.cc,v $
// Revision 1.5  2001/09/21 03:24:17  mccann
// Implemented profile histograms
//
// Revision 1.4  2001/09/19 18:02:20  cdj
// improved color choice
//
// Revision 1.3  2001/09/19 17:36:35  cdj
// removed debug printout
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
#include "HistogramViewer/HVHistogramsViewer.h"
#include "HistogramViewer/HVHistogram1DPlotter.h"
#include "HistogramInterface/HIHist1D.h"

#include "HistogramViewer/HVHistogram2DPlotter.h"
#include "HistogramInterface/HIHist2D.h"

#include "HistogramViewer/HVHistogramProfPlotter.h"
#include "HistogramInterface/HIHistProf.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVHistogramsViewer" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVHistogramsViewer.cc,v 1.5 2001/09/21 03:24:17 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVHistogramsViewer::HVHistogramsViewer(QWidget* iParent, const char* iName):
   QWorkspace(iParent, iName)
{
}

// HVHistogramsViewer::HVHistogramsViewer( const HVHistogramsViewer& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVHistogramsViewer::~HVHistogramsViewer()
{
}

//
// assignment operators
//
// const HVHistogramsViewer& HVHistogramsViewer::operator=( const HVHistogramsViewer& rhs )
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
HVHistogramsViewer::createPlot(const HIHist1D* iHist)
{
   HVHistogramPlotter* plotter = new HVHistogram1DPlotter(iHist, this);
   plotter->setTitle( iHist->title().c_str() );
   plotter->setCaption(iHist->title().c_str());
   plotter->setPlotBackground( white );
   plotter->setGridPen( QPen(lightGray, 0, DotLine) );
   plotter->show();
}

void
HVHistogramsViewer::createPlot(const HIHist2D* iHist)
{
   HVHistogramPlotter* plotter = new HVHistogram2DPlotter(iHist, this);
   plotter->setTitle( iHist->title().c_str() );
   plotter->setCaption(iHist->title().c_str());
   plotter->setPlotBackground( white );
   plotter->setGridPen( QPen(lightGray, 0, DotLine) );
   plotter->show();
}

void
HVHistogramsViewer::createPlot(const HIHistProf* iHist)
{
   HVHistogramPlotter* plotter = new HVHistogramProfPlotter(iHist, this);
   plotter->setTitle( iHist->title().c_str() );
   plotter->setCaption(iHist->title().c_str());
   plotter->setPlotBackground( white );
   plotter->setGridPen( QPen(lightGray, 0, DotLine) );
   plotter->show();
}

void
HVHistogramsViewer::updateHistograms()
{
   QWidgetList list = windowList();
   QListIterator<QWidget> it( list );

   for(; it.current(); ++it) {
      if( (*it)->inherits("HVHistogramPlotter")) {
	 reinterpret_cast<HVHistogramPlotter*>( *it )->retrieveNewValues();
      }
   }
}
//
// const member functions
//

//
// static member functions
//

#include "Class/HVHistogramsViewer.moc"
