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
   m_oldcurve( insertCurve( "last" ) ),
   m_fit( insertCurve( "fit" ) )
{
   setCurveStyle(curveId(), QwtCurve::Dots);
   setCurvePen(curveId(), black );
   setCurveSymbol(curveId(), QwtSymbol(QwtSymbol::Ellipse,
				       QBrush( black ), QPen( black ),
				       QSize( 5, 5 ) ) );

   setCurveStyle( m_oldcurve, QwtCurve::Dots );
   setCurvePen( m_oldcurve, red );
   setCurveSymbol( m_oldcurve, QwtSymbol( QwtSymbol::Ellipse,
					  QBrush( red ), QPen( red ),
					  QSize( 3, 3 ) ) );

   setCurveStyle( m_fit, QwtCurve::Lines );
   setCurvePen( m_fit, blue );

   setAxisAutoScale(true);

   long guide_up, guide_middle, guide_down;
   switch ( m_histogram->id() ) {
      case 100: case 200: case 150: case 250:
      case 400: case 402: case 452: 
//  	 setAxisScale( yLeft, -0.000030, 0.000030 );

	 guide_up = insertLineMarker( "", yLeft );
	 guide_middle = insertLineMarker( "", yLeft );
	 guide_down = insertLineMarker( "", yLeft );
	 setMarkerYPos( guide_up, 0.000007 );
	 setMarkerYPos( guide_middle, 0. );
	 setMarkerYPos( guide_down, -0.000007 );
	 setMarkerLinePen( guide_up, QPen( darkGreen, 1, DashLine ) );
	 setMarkerLinePen( guide_middle, QPen( darkGreen, 1, SolidLine ) );
	 setMarkerLinePen( guide_down, QPen( darkGreen, 1, DashLine ) );
	 break;

      case 500: case 502: case 550:
//  	 setAxisScale( yLeft, -0.000100, 0.000100 );

	 guide_up = insertLineMarker( "", yLeft );
	 guide_middle = insertLineMarker( "", yLeft );
	 guide_down = insertLineMarker( "", yLeft );
	 setMarkerYPos( guide_up, 0.000015 );
	 setMarkerYPos( guide_middle, 0. );
	 setMarkerYPos( guide_down, -0.000015 );
	 setMarkerLinePen( guide_up, QPen( darkGreen, 1, DashLine ) );
	 setMarkerLinePen( guide_middle, QPen( darkGreen, 1, SolidLine ) );
	 setMarkerLinePen( guide_down, QPen( darkGreen, 1, DashLine ) );
	 break;

      case 601: case 602: case 603: case 604:
      case 651: case 652: case 653: case 654:
//  	 setAxisScale( yLeft, -0.000300, 0.000300 );

	 guide_up = insertLineMarker( "", yLeft );
	 guide_middle = insertLineMarker( "", yLeft );
	 guide_down = insertLineMarker( "", yLeft );
	 setMarkerYPos( guide_up, 0.000015 );
	 setMarkerYPos( guide_middle, 0. );
	 setMarkerYPos( guide_down, -0.000015 );
	 setMarkerLinePen( guide_up, QPen( darkGreen, 1, DashLine ) );
	 setMarkerLinePen( guide_middle, QPen( darkGreen, 1, SolidLine ) );
	 setMarkerLinePen( guide_down, QPen( darkGreen, 1, DashLine ) );
	 break;
   }

   m_errorbar_curves = new long[m_numberOfChannels];

   for ( int i = 0;  i < m_numberOfChannels;  i++ )
   {
      m_errorbar_curves[i] = insertCurve( "" );
      setCurveStyle( m_errorbar_curves[i], QwtCurve::Lines );
      setCurvePen( m_errorbar_curves[i], black );
   }

   addInitialParameter( "constant",  0., 1e-5 );
   addInitialParameter( "linear",    0., 1e-3 );
   addInitialParameter( "quadratic", 0., 1e-1 );
   addInitialParameter( "cubic",     0., 1e-1 );
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
   double* x = new double[m_numberOfChannels];
   double* y = new double[m_numberOfChannels];

   vector< long > unset_bars;
   double last_good_x( 0. );
   double last_good_y_low( 0. );
   double last_good_y_high( 0. );
   double min_x( 0. );
   double max_x( 0. );

   unsigned int index, running;
   for ( index = 1, running = 0;
	 index <= m_numberOfChannels;
	 index++ )
      if ( m_histogram->channelValue(index) != 0. ) {
	 x[running] = m_histogram->channelCenter(index);
	 y[running] = m_histogram->channelValue(index);
	 double error = m_histogram->channelError(index);

	 double errorbarx[2];
	 double errorbary[2];
	 last_good_x      = errorbarx[0] = errorbarx[1] = x[running];
	 last_good_y_low  = errorbary[0] = y[running] - error;
	 last_good_y_high = errorbary[1] = y[running] + error;

	 setCurveData( m_errorbar_curves[index-1], errorbarx, errorbary, 2 );

	 if ( running == 0 )
	    min_x = max_x = x[running];
	 else {
	    if ( x[running] < min_x ) min_x = x[running];
	    if ( x[running] > max_x ) max_x = x[running];
	 }

	 running++;
      }
      else
	 unset_bars.push_back( m_errorbar_curves[index-1] );

   setCurveData( curveId(), x, y, running );

   // some error bars haven't been updated yet. update them
   for ( vector< long >::const_iterator bar = unset_bars.begin();
	 bar != unset_bars.end();
	 bar++ ) {
      double errorbarx[2];
      double errorbary[2];
      errorbarx[0] = errorbarx[1] = last_good_x;
      errorbary[0] = last_good_y_low;
      errorbary[1] = last_good_y_high;

      setCurveData( *bar, errorbarx, errorbary, 2 );
   }

// This plots all channels, even zeroed ones:

//     double* x = new double[m_numberOfChannels];
//     double* y = new double[m_numberOfChannels];

//     for ( unsigned int index = 1;  index <= m_numberOfChannels;  ++index ) {
//        x[index-1] = m_histogram->channelCenter(index);
//        y[index-1] = m_histogram->channelValue(index);
//        double error = m_histogram->channelError(index);

//        double errorbarx[2];
//        double errorbary[2];
//        errorbarx[0] = errorbarx[1] = x[index-1];
//        errorbary[0] = y[index-1] - error;
//        errorbary[1] = y[index-1] + error;

//        setCurveData( m_errorbar_curves[index-1], errorbarx, errorbary, 2 );
//     }

//     setCurveData( curveId(), x, y, m_numberOfChannels );

   switch ( m_histogram->id() ) {
      case 100: case 200: case 150: case 250:
      case 400: case 402: case 452: 
      case 500: case 502: case 550:
      case 601: case 602: case 603: case 604:
      case 651: case 652: case 653: case 654:

	 const unsigned int nfitpoints = 100;
	 double fitx[nfitpoints];
	 double fity[nfitpoints];

	 m_mi->loadFcn( *this );
	 m_mi->changeParameterStart( 0, 0. );
	 m_mi->changeParameterStart( 1, 0. );
	 m_mi->changeParameterStart( 2, 0. );
	 m_mi->changeParameterStart( 3, 0. );
	 m_mi->changeParameterStep( 0, 1e-5 );
	 m_mi->changeParameterStep( 1, 1e-3 );
	 m_mi->changeParameterStep( 2, 1e-1 );
	 m_mi->changeParameterStep( 3, 1e-1 );
	 m_mi->runMigrad();

	 char val0[13], val1[13], val2[13], val3[13];
	 sprintf( val0, "%12g", m_mi->parameter(0).value() );
	 sprintf( val1, "%12g", m_mi->parameter(1).value() );
	 sprintf( val2, "%12g", m_mi->parameter(2).value() );
	 sprintf( val3, "%12g", m_mi->parameter(3).value() );

	 switch ( m_histogram->id() ) {
	    case 100: case 150:
	       cout << "set pos_constant " << val0
		    << " ; set pos_linear " << val1
		    << " ; set pos_quadratic " << val2
		    << " ; set pos_cubic " << val3 << endl;
	       break;
	    case 200: case 250:
	       cout << "set neg_constant " << val0
		    << " ; set neg_linear " << val1
		    << " ; set neg_quadratic " << val2
		    << " ; set neg_cubic " << val3 << endl;
	       break;
	 }

	 double step( ( max_x - min_x ) / double(nfitpoints-1) );
	 double constant( m_mi->parameter(0).value() );
	 double linear( m_mi->parameter(1).value() );
	 double quadratic( m_mi->parameter(2).value() );
	 double cubic( m_mi->parameter(3).value() );

	 for ( int i = 0;  i < nfitpoints;  i++ ) {
	    fitx[i] = double(i) * step + min_x;
	    fity[i] = ( constant +
			linear    * fitx[i] +
			quadratic * fitx[i]*fitx[i] +
			cubic     * fitx[i]*fitx[i]*fitx[i] );
	 }
	 setCurveData( m_fit, fitx, fity, nfitpoints );
   } // end if this is a fittable histogram
   
   replot();

   setCurveData( m_oldcurve, x, y, running );

   delete [] x;
   delete [] y;
}

double
HVHistogramProfPlotter::iterate( double* values )
{
   double chi2 = 0.;
   for ( unsigned int index = 1;  index <= m_numberOfChannels;  ++index ) {
      double x = m_histogram->channelCenter(index);
      double y = m_histogram->channelValue(index);
      double error = m_histogram->channelError(index);

      double calcy = ( values[0] +
		       values[1] * x +
		       values[2] * x*x +
		       values[3] * x*x*x );

      if ( error > 0. )
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
