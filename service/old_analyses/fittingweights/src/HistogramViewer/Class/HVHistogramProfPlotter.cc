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
#include "HistogramInterface/HIHistFitFunc.h"

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
   m_fitId( insertCurve( "fit" ) ),
   m_histogram(iHistogram),
   m_numberOfChannels( iHistogram->nChannels() ),
   m_errorbar_curves()
{
   setCurveStyle(curveId(), QwtCurve::Dots);
   setCurvePen(curveId(), black );
   setCurveSymbol(curveId(), QwtSymbol(QwtSymbol::Ellipse,
				       QBrush( black ), QPen(),
				       QSize( 5, 5 ) ) );

   setCurveStyle( fitId(), QwtCurve::Lines );

   setAxisAutoScale(true);

   m_errorbar_curves = new long[m_numberOfChannels];

   for ( int i = 0;  i < m_numberOfChannels;  i++ )
   {
      m_errorbar_curves[i] = insertCurve( "" );
      setCurveStyle( m_errorbar_curves[i], QwtCurve::Lines );
      setCurvePen( m_errorbar_curves[i], black );
   }

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
   double* fitx = new double[m_numberOfChannels];
   double* fity = new double[m_numberOfChannels];

   for ( unsigned int index = 1;  index <= m_numberOfChannels;  ++index ) {
      x[index-1] = m_histogram->channelCenter(index);
      y[index-1] = m_histogram->channelValue(index);
      double error = m_histogram->channelError(index);

      double errorbarx[2];
      double errorbary[2];
      errorbarx[0] = errorbarx[1] = x[index-1];
      errorbary[0] = y[index-1] - error;
      errorbary[1] = y[index-1] + error;

      setCurveData( m_errorbar_curves[index-1], errorbarx, errorbary, 2 );
   }

   HIHistFitFunc* fitFunc = m_histogram->fitFunc();
   if ( fitFunc != NULL  &&
        fitFunc->histogramType() == HIHistFitFunc::kHistProf  &&
        fitFunc->fit() )
   {
      setCurvePen( fitId(), red );
      unsigned int index, start, end;
      start = fitFunc->domainIndexStart();
      end = fitFunc->domainIndexEnd();

      for( index = 1; index < start; ++index) {
	 fitx[index-1] = m_histogram->channelCenter( start );
	 fity[index-1] = fitFunc->func( fitx[index-1] );
      }
      for( index = start; index < end; ++index) {
	 fitx[index-1] = m_histogram->channelCenter( index );
	 fity[index-1] = fitFunc->func( fitx[index-1] );
      }
      for( index = end; index <= m_numberOfChannels; ++index) {
	 fitx[index-1] = m_histogram->channelCenter( end );
	 fity[index-1] = fitFunc->func( fitx[index-1] );
      }

   }
   else
   {
      setCurvePen( fitId(), black );
      for( unsigned int index = 1; index <= m_numberOfChannels; ++index) {
         fitx[index-1] = m_histogram->channelLowEdge(1);
         fity[index-1] = m_histogram->channelValue(1);
      }
   }

   setCurveData( curveId(), x, y, m_numberOfChannels );
   setCurveData( fitId(), fitx, fity, m_numberOfChannels );
   replot();

   delete [] x;
   delete [] y;
   delete [] fitx;
   delete [] fity;
}

//
// const member functions
//

//
// static member functions
//
