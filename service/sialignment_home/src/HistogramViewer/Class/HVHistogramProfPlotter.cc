// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVHistogramProfPlotter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Sep 19 11:02:06 EDT 2001
// $Id: HVHistogramProfPlotter.cc,v 1.1 2001/09/21 17:12:34 mccann Exp $
//
// Revision history
//
// $Log: HVHistogramProfPlotter.cc,v $
// Revision 1.1  2001/09/21 17:12:34  mccann
// added HVHistogramProfilePlotter with cvs add
//
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
#include "HistogramViewer/HVHistogramProfPlotter.h"
#include "HistogramInterface/HIHistProf.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramViewer.HVHistogramProfPlotter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HVHistogramProfPlotter.cc,v 1.1 2001/09/21 17:12:34 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HVHistogramProfPlotter::HVHistogramProfPlotter(const HIHistProf* iHistogram,
					   QWidget* iParent,
					   const char* iName ) :
   HVHistogramPlotter(iHistogram->title().c_str(), iParent, iName ),
   m_histogram(iHistogram),
   m_numberOfChannels( iHistogram->nChannels() ),
   m_errorbar_curves(),
   m_last_curve( insertCurve( "last" ) ),
   m_linear_fit( insertCurve( "fit" ) )
{
   setCurveStyle(curveId(), QwtCurve::Dots);
   setCurvePen(curveId(), black );
   setCurveSymbol(curveId(), QwtSymbol(QwtSymbol::Ellipse,
				       QBrush( black ), QPen(),
				       QSize( 5, 5 ) ) );

   setCurveStyle( m_last_curve, QwtCurve::Dots );
   setCurvePen( m_last_curve, red );
   setCurveSymbol( m_last_curve, QwtSymbol( QwtSymbol::Ellipse,
					    QBrush( red ), QPen( red ),
					    QSize( 3, 3 ) ) );

   if ( iHistogram->id() == 500 )
      setAxisScale( yLeft, -0.000100, 0.000100 );
   else if ( iHistogram->id() == 501 )
      setAxisAutoScale( true );
   else
      setAxisScale( yLeft, -0.000030, 0.000030 );
   setAxisScale( xBottom, m_histogram->lowEdgeX(), m_histogram->highEdgeX() );

   long line1 = insertCurve( "" );
   long line2 = insertCurve( "" );
   setCurveStyle( line1, QwtCurve::Lines );  setCurvePen( line1, black );
   setCurveStyle( line2, QwtCurve::Lines );  setCurvePen( line2, black );

   double x[2];
   double y[2];
   x[0] = m_histogram->lowEdgeX();  x[1] = m_histogram->highEdgeX();
   if ( iHistogram->id() == 500 )
   {
      y[0] = y[1] = -0.000015;
      setCurveData( line1, x, y, 2 );
      y[0] = y[1] =  0.000015;
      setCurveData( line2, x, y, 2 );
   }
   else
   {
      y[0] = y[1] = -0.000007;
      setCurveData( line1, x, y, 2 );
      y[0] = y[1] =  0.000007;
      setCurveData( line2, x, y, 2 );
   }

   m_errorbar_curves = new long[m_numberOfChannels];

   for ( int i = 0;  i < m_numberOfChannels;  i++ )
   {
      m_errorbar_curves[i] = insertCurve( "" );
      setCurveStyle( m_errorbar_curves[i], QwtCurve::Lines );
      setCurvePen( m_errorbar_curves[i], black );
   }

   for( int sen = 0;  sen < 447;  sen++ )
   {
      m_sensor_curve_left[sen] = insertCurve( "sensor" );
      setCurveStyle( m_sensor_curve_left[sen], QwtCurve::Lines );
      setCurvePen( m_sensor_curve_left[sen], green );

      m_sensor_curve_right[sen] = insertCurve( "sensor" );
      setCurveStyle( m_sensor_curve_right[sen], QwtCurve::Lines );
      setCurvePen( m_sensor_curve_right[sen], green );
   }

   setCurveStyle( m_linear_fit, QwtCurve::Lines );
   setCurvePen( m_linear_fit, black );

   addInitialParameter( "intercept", 0., 0.000010 );
   addInitialParameter( "slope", 0., 0.000002 );
   m_mi = MinuitInterface::instance();
   m_mi->setDiagLevel( MinuitInterface::kMute );

   retrieveNewValues();
}

// HVHistogramProfPlotter::HVHistogramProfPlotter( const HVHistogramProfPlotter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HVHistogramProfPlotter::~HVHistogramProfPlotter()
{
   delete [] m_errorbar_curves;
}

//
// assignment operators
//
// const HVHistogramProfPlotter& HVHistogramProfPlotter::operator=( const HVHistogramProfPlotter& rhs )
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
HVHistogramProfPlotter::retrieveNewValues()
{
//     unsigned int numberOfChannels = m_histogram->nChannels();
//     double* x = new double[numberOfChannels];
//     double* y = new double[numberOfChannels];

//     for( unsigned int index = 1; index <= numberOfChannels; ++index) {
//        x[index-1] = m_histogram->channelLowEdge(index);
//        y[index-1] = m_histogram->channelValue(index);
//     }

//     setCurveData( curveId(), x,y,numberOfChannels);
//     replot();

//     delete [] x;
//     delete [] y;

   double* x = new double[m_numberOfChannels];
   double* y = new double[m_numberOfChannels];

   double y2[2];
   if ( m_histogram->id() == 500 )
   { y2[0] = -0.000100;  y2[1] = 0.000100; }
   else if ( m_histogram->id() == 501 )
   { y2[0] = 0.;  y2[1] = 0.; }
   else
   { y2[0] = -0.000030;  y2[1] = 0.000030; }
   double nully[2];  nully[0] = nully[1] = 0.;

//     double xlower = -1000.;
//     double xupper = -1000.;
   double xlower = m_histogram->channelCenter(1);
   double xupper = m_histogram->channelCenter(m_numberOfChannels);

   for ( unsigned int index = 1;  index <= m_numberOfChannels;  ++index ) {
      x[index-1] = m_histogram->channelCenter(index);
      y[index-1] = m_histogram->channelValue(index);
      double error = m_histogram->channelError(index);

//        if ( abs( error ) > 0.000003 )
//        {
//  	 if ( xlower < -999. )
//  	    xlower = x[index-1];
//  	 xupper = x[index-1];
//        }

      double errorbarx[2];
      double errorbary[2];
      errorbarx[0] = errorbarx[1] = x[index-1];
      errorbary[0] = y[index-1] - error;
      errorbary[1] = y[index-1] + error;

      setCurveData( m_errorbar_curves[index-1], errorbarx, errorbary, 2 );

      if ( errorbary[0] < y2[0] ) y2[0] = errorbary[0];
      if ( errorbary[1] > y2[1] ) y2[1] = errorbary[1];
   }

   setCurveData( curveId(), x, y, m_numberOfChannels );

   double xleft[2];
   double xright[2];
   for ( int sensor = 1;  sensor <= 447;  sensor++ )
   {
      get_edges( sensor, xleft[0], xleft[1], xright[0], xright[1] );
      if ( ( xleft[0] == 0  &&  xright[0] == 0 )
	   ||  m_histogram->id() == 300  ||  m_histogram->id() == 310  ||  m_histogram->id() == 320 )
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

   m_mi->loadFcn( *this );
   m_mi->changeParameterStart( 0, 0. );
   m_mi->changeParameterStart( 1, 0. );
   m_mi->changeParameterStep( 0, 0.000010 );
   m_mi->changeParameterStep( 1, 0.000002 );
   m_mi->runMigrad();
   double fitx[2];  fitx[0] = xlower;  fitx[1] = xupper;
   double fity[2];
   fity[0] = m_mi->parameter(0).value() + m_mi->parameter(1).value() * fitx[0];
   fity[1] = m_mi->parameter(0).value() + m_mi->parameter(1).value() * fitx[1];
   setCurveData( m_linear_fit, fitx, fity, 2 );
   cout << m_histogram->title()
	<< " intercept = " << m_mi->parameter(0).value()
	<< " slope = " << m_mi->parameter(1).value() << endl;

   replot();

   setCurveData( m_last_curve, x, y, m_numberOfChannels );

   delete [] x;
   delete [] y;
}

double
HVHistogramProfPlotter::iterate( double* values )
{
   double intercept = values[0];
   double slope = values[1];

   double chi2 = 0.;
   for ( unsigned int index = 1;  index <= m_numberOfChannels;  ++index )
   {
      double x = m_histogram->channelCenter(index);
      double y = m_histogram->channelValue(index);
      double error = m_histogram->channelError(index);

      double calcy = intercept + slope * x;

      if ( abs( error ) > 0.000002 )
	 chi2 += sqr( ( calcy - y ) / error );
   }
   return chi2;
}

//
// const member functions
//

//
// static member functions
//
