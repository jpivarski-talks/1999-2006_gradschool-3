// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogramPlotter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Sat Jul 14 15:08:04 EDT 2001
// $Id: HVHistogramPlotter.cc,v 1.2 2001/09/19 16:00:58 cdj Exp $
//
// Revision history
//
// $Log: HVHistogramPlotter.cc,v $
// Revision 1.2  2001/09/19 16:00:58  cdj
// generalized to allow more than 1D histograms
//
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
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
#include "HistogramViewer/HVHistogramPlotter.h"
#include "HistogramInterface/HIHist1D.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVHistogramPlotter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVHistogramPlotter.cc,v 1.2 2001/09/19 16:00:58 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVHistogramPlotter::HVHistogramPlotter(const char* iHistogramName,
				       QWidget* iParent,
				       const char* iName):
   QwtPlot(iParent, iName),
   m_curveId( insertCurve(iHistogramName ) )
{
}

// HVHistogramPlotter::HVHistogramPlotter( const HVHistogramPlotter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVHistogramPlotter::~HVHistogramPlotter()
{
}

//
// assignment operators
//
// const HVHistogramPlotter& HVHistogramPlotter::operator=( const HVHistogramPlotter& rhs )
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

//
// const member functions
//

//
// static member functions
//

#include "Class/HVHistogramPlotter.moc"
