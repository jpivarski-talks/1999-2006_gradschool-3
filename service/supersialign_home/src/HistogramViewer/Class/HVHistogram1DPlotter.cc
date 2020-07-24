// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogram1DPlotter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Sep 19 11:02:06 EDT 2001
// $Id: HVHistogram1DPlotter.cc,v 1.2 2001/09/19 18:02:19 cdj Exp $
//
// Revision history
//
// $Log: HVHistogram1DPlotter.cc,v $
// Revision 1.2  2001/09/19 18:02:19  cdj
// improved color choice
//
// Revision 1.1  2001/09/19 16:01:41  cdj
// first submission
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
#include "HistogramViewer/HVHistogram1DPlotter.h"
#include "HistogramInterface/HIHist1D.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVHistogram1DPlotter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVHistogram1DPlotter.cc,v 1.2 2001/09/19 18:02:19 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVHistogram1DPlotter::HVHistogram1DPlotter(const HIHist1D* iHistogram,
					   QWidget* iParent,
					   const char* iName ) :
   HVHistogramPlotter(iHistogram->title().c_str(), iParent, iName ),
   m_histogram(iHistogram),
   m_hv()
{
   setCurveStyle(curveId(), QwtCurve::Steps);
   setCurvePen(curveId(), black );
   setAxisAutoScale(true);

   if ( m_histogram->id() == 300 ) {
      m_pos_thresh_mark = insertLineMarker( "", xBottom );
      m_neg_thresh_mark = insertLineMarker( "", xBottom );
      setMarkerXPos( m_pos_thresh_mark, m_hv.pos_thresh );
      setMarkerXPos( m_neg_thresh_mark, m_hv.neg_thresh );
      setMarkerPen( m_pos_thresh_mark, QPen( blue, 1, SolidLine ) );
      setMarkerPen( m_neg_thresh_mark, QPen( blue, 1, SolidLine ) );
   }

   retrieveNewValues();
}

// HVHistogram1DPlotter::HVHistogram1DPlotter( const HVHistogram1DPlotter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVHistogram1DPlotter::~HVHistogram1DPlotter()
{
}

//
// assignment operators
//
// const HVHistogram1DPlotter& HVHistogram1DPlotter::operator=( const HVHistogram1DPlotter& rhs )
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
HVHistogram1DPlotter::retrieveNewValues()
{
   unsigned int numberOfChannels = m_histogram->nChannels();
   double* x = new double[numberOfChannels];
   double* y = new double[numberOfChannels];

   for( unsigned int index = 1; index <= numberOfChannels; ++index) {
      x[index-1] = m_histogram->channelLowEdge(index);
      y[index-1] = m_histogram->channelValue(index);
   }

   setCurveData( curveId(), x,y,numberOfChannels);

   if ( m_histogram->id() == 300 ) {
      setMarkerXPos( m_pos_thresh_mark, m_hv.pos_thresh );
      setMarkerXPos( m_neg_thresh_mark, m_hv.neg_thresh );
   }

   replot();

   delete [] x;
   delete [] y;
}

//
// const member functions
//

//
// static member functions
//
