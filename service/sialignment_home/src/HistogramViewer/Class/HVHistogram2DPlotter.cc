// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogram2DPlotter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Sep 19 11:02:06 EDT 2001
// $Id: HVHistogram2DPlotter.cc,v 1.3 2001/09/19 18:02:20 cdj Exp $
//
// Revision history
//
// $Log: HVHistogram2DPlotter.cc,v $
// Revision 1.3  2001/09/19 18:02:20  cdj
// improved color choice
//
// Revision 1.2  2001/09/19 17:36:54  cdj
// set axes limits
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
#include "HistogramViewer/HVHistogram2DPlotter.h"
#include "HistogramInterface/HIHist2D.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVHistogram2DPlotter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVHistogram2DPlotter.cc,v 1.3 2001/09/19 18:02:20 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVHistogram2DPlotter::HVHistogram2DPlotter(const HIHist2D* iHistogram,
					   QWidget* iParent,
					   const char* iName ) :
   HVHistogramPlotter(iHistogram->title().c_str(), iParent, iName ),
   m_histogram(iHistogram)
{
   setCurveStyle(curveId(), QwtCurve::Dots);
   setAxisScale(yLeft, m_histogram->lowEdgeY(), m_histogram->highEdgeY());
   setAxisScale(xBottom, m_histogram->lowEdgeX(), m_histogram->highEdgeX());
   setCurvePen(curveId(), black );

   long line = insertCurve( "" );
   setCurveStyle( line, QwtCurve::Lines );  setCurvePen( line, black );
   double x[2], y[2];
   x[0] = m_histogram->lowEdgeX();  x[1] = m_histogram->highEdgeX();
   y[0] = y[1] = 0.;
   setCurveData( line, x, y, 2 );

   for( int sen = 0;  sen < 447;  sen++ )
   {
      m_sensor_curve_left[sen] = insertCurve( "sensor" );
      setCurveStyle( m_sensor_curve_left[sen], QwtCurve::Lines );
      setCurvePen( m_sensor_curve_left[sen], green );

      m_sensor_curve_right[sen] = insertCurve( "sensor" );
      setCurveStyle( m_sensor_curve_right[sen], QwtCurve::Lines );
      setCurvePen( m_sensor_curve_right[sen], green );
   }

   retrieveNewValues();
}

// HVHistogram2DPlotter::HVHistogram2DPlotter( const HVHistogram2DPlotter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVHistogram2DPlotter::~HVHistogram2DPlotter()
{
}

//
// assignment operators
//
// const HVHistogram2DPlotter& HVHistogram2DPlotter::operator=( const HVHistogram2DPlotter& rhs )
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
HVHistogram2DPlotter::retrieveNewValues()
{
   const unsigned int numberOfChannelsX = m_histogram->nChannelsX();
   const unsigned int numberOfChannelsY = m_histogram->nChannelsY();
   double* x = new double[numberOfChannelsX*numberOfChannelsY];
   double* y = new double[numberOfChannelsX*numberOfChannelsY];

   double y2[2];
   y2[0] = m_histogram->lowEdgeY();  y2[1] = m_histogram->highEdgeY();
   double nully[2];  nully[0] = nully[1] = 0.;
   
   unsigned int runningIndex = 0;
   for( unsigned int indexX = 1; indexX <= numberOfChannelsX; ++indexX) {
      for( unsigned int indexY = 1; indexY <= numberOfChannelsY; ++indexY) {
	 if( 0.0 < m_histogram->channelValue(indexX, indexY ) ) {
	    pair<float,float> center = m_histogram->channelCenter(
	       indexX, indexY );
	    x[runningIndex] = center.first;
	    y[runningIndex] = center.second;
	    ++runningIndex;
	 }
      }
   }

   setCurveData( curveId(), x,y,runningIndex);

   double xleft[2];
   double xright[2];
   for ( int sensor = 1;  sensor <= 447;  sensor++ )
   {
      get_edges( sensor, xleft[0], xleft[1], xright[0], xright[1] );
      if ( ( xleft[0] == 0  &&  xright[0] == 0 )  ||  m_histogram->id() == 300 )
      {
	 setCurveData( m_sensor_curve_right[sensor-1], xleft, nully, 2 );
	 setCurveData( m_sensor_curve_right[sensor-1], xright, nully, 2 );
      }
      else
      {
	 setCurveData( m_sensor_curve_right[sensor-1], xleft, y2, 2 );
	 setCurveData( m_sensor_curve_right[sensor-1], xright, y2, 2 );
      }
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
