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
// $Id: HVHistogram2DPlotter.cc,v 1.4 2002/05/06 17:51:37 mccann Exp $
//
// Revision history
//
// $Log: HVHistogram2DPlotter.cc,v $
// Revision 1.4  2002/05/06 17:51:37  mccann
// improved 2d histogram plotting--- now it works like the box option in PAW
//
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
static const char* const kIdString  = "$Id: HVHistogram2DPlotter.cc,v 1.4 2002/05/06 17:51:37 mccann Exp $";
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
   m_histogram(iHistogram),
   m_drawBin( NULL )
{
   setAxisScale(yLeft, m_histogram->lowEdgeY(), m_histogram->highEdgeY());
   setAxisScale(xBottom, m_histogram->lowEdgeX(), m_histogram->highEdgeX());
//     enableGridX( false );
//     enableGridY( false );

   // this curve is not used
   setCurveStyle(curveId(), QwtCurve::Dots);
   setCurvePen(curveId(), white );

   const unsigned int numberOfChannelsX = iHistogram->nChannelsX();
   const unsigned int numberOfChannelsY = iHistogram->nChannelsY();
   unsigned int indexX;
   unsigned int indexY;
   double x[1];
   double y[1];

   m_drawBin = new long[ numberOfChannelsX * numberOfChannelsY ];

   for ( indexX = 1;  indexX <= numberOfChannelsX;  indexX++ )
      for ( indexY = 1;  indexY <= numberOfChannelsY;  indexY++ ) {
	 pair< float, float > center = m_histogram->channelCenter( indexX, indexY );
	 m_drawBin[(indexX-1)*numberOfChannelsY+(indexY-1)] = insertCurve( "" );
	 x[0] = center.first;
	 y[0] = center.second;
	 setCurveData( m_drawBin[(indexX-1)*numberOfChannelsY+(indexY-1)], x, y, 1 );
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
   if ( m_drawBin != NULL )
      delete m_drawBin;
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
   unsigned int indexX;
   unsigned int indexY;
   
   const double max_width( floor( ( contentsRect().width() - 150 ) / numberOfChannelsX ) );
   const double max_height( floor( ( contentsRect().height() - 112 ) / numberOfChannelsY ) );

   double min( 0. );
   double max( 0. );
   for ( indexX = 1;  indexX <= numberOfChannelsX;  indexX++ )
      for ( indexY = 1;  indexY <= numberOfChannelsY;  indexY++ ) {

	 double value( m_histogram->channelValue( indexX, indexY ) );
	 if ( indexX == 1  &&  indexY == 1 ) min = max = value;

	 if ( min > value ) min = value;
	 if ( max < value ) max = value;
      }
   if ( min > 0. ) min = 0.;

   double translate_width( max_width / ( max - min ) );
   double translate_height( max_height / ( max - min ) );
   for ( indexX = 1;  indexX <= numberOfChannelsX;  indexX++ )
      for ( indexY = 1;  indexY <= numberOfChannelsY;  indexY++ ) {

	 double value( m_histogram->channelValue( indexX, indexY ) );

	 if ( value > 0. ) {
	    int width( rint( translate_width * ( value - min ) ) );
	    int height( rint( translate_height * ( value - min ) ) );

	    setCurveSymbol( m_drawBin[(indexX-1)*numberOfChannelsY+(indexY-1)],
			    QwtSymbol( QwtSymbol::Rect, QBrush( white, SolidPattern ),
				       QPen( black ), QSize(width,height) ) );
	 } else {
	    setCurveSymbol( m_drawBin[(indexX-1)*numberOfChannelsY+(indexY-1)],
			    QwtSymbol( QwtSymbol::Rect, QBrush( white, SolidPattern ),
				       QPen( white ), QSize(0,0) ) );
	 }

      } // end for loops

   replot();
}

//
// const member functions
//

//
// static member functions
//
