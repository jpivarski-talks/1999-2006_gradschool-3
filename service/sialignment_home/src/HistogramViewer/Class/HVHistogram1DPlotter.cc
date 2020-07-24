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
   m_current_curve( insertCurve( "current" ) ),
//   m_peak_line( insertCurve( "peak" ) ),
   m_ave_line( insertCurve( "average" ) )
{
   setCurveStyle(curveId(), QwtCurve::Steps);
   setCurvePen(curveId(), red );
   setAxisAutoScale(true);

   setCurveStyle( m_current_curve, QwtCurve::Steps );
   setCurvePen( m_current_curve, black );

//     setCurveStyle( m_peak_line, QwtCurve::Lines );
//     setCurvePen( m_peak_line, green );

   setCurveStyle( m_ave_line, QwtCurve::Lines );
   setCurvePen( m_ave_line, blue );

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

   double peakx;
   double peaky = 0.;
   double ave = 0.;
   int points = 0;

   for( unsigned int index = 1; index <= numberOfChannels; ++index) {
      x[index-1] = m_histogram->channelLowEdge(index);
      y[index-1] = m_histogram->channelValue(index);

      if ( y[index-1] > peaky )
      {
	 peakx = x[index-1];
	 peaky = y[index-1];
      }

      ave += m_histogram->channelCenter(index) * y[index-1];
      points += y[index-1];
   }
   if ( points != 0 )
      ave /= double( points );

   setCurveData( m_current_curve, x, y, numberOfChannels );
   
   double x2[2], y2[2];
   y2[0] = 0.;  y2[1] = peaky;
//     x2[0] = x2[1] = peakx;
//     setCurveData( m_peak_line, x2, y2, 2 );
   x2[0] = x2[1] = ave;
   setCurveData( m_ave_line, x2, y2, 2 );

   replot();
   setCurveData( curveId(), x,y,numberOfChannels);

   delete [] x;
   delete [] y;
}

//
// const member functions
//

//
// static member functions
//
