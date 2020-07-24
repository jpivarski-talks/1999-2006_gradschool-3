// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistFitFunc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Fri Oct 19 15:55:03 EDT 2001
// $Id$
//
// Revision history
//
// $Log$

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
#include "Experiment/report.h"
#include "HistogramInterface/HIHistFitFunc.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HIHistFitFunc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIHistFitFunc::HIHistFitFunc()
   : m_histogram( NULL )
{
}

HIHistFitFunc::HIHistFitFunc( HIHist* histogram, HistogramType type,
			      unsigned int numParams,
			      double* paramsStart, double* paramsStep,
			      DABoolean hasLimits,
			      double* paramsLimitsLow,
			      double* paramsLimitsHigh,
			      DABoolean useMinos )
   : m_histogram( histogram )
   , m_type( type )
   , m_numParams( numParams )
   , m_hasLimits( hasLimits )
   , m_paramsStart( NULL )
   , m_paramsStep( NULL )
   , m_paramsLimitsLow( NULL )
   , m_paramsLimitsHigh( NULL )
   , m_useMinos( useMinos )
   , m_numPointsUsed( 0 )
{
   assert( m_type < kNumAllowedTypes );
   switch ( m_type )
   {
      case kHist1D:
	 m_numberOfChannels = ((HIHist1D*) (m_histogram))->nChannels();
	 break;

      case kHistProf:
	 m_numberOfChannels = ((HIHistProf*) (m_histogram))->nChannels();
	 break;
   }
   m_domainStart = 1;
   m_domainEnd = m_numberOfChannels;

   m_paramsStart = new double[m_numParams];
   m_paramsStep = new double[m_numParams];
   if ( m_hasLimits )
   {
      m_paramsLimitsLow = new double[m_numParams];
      m_paramsLimitsHigh = new double[m_numParams];
   }

   char parName[512];
   for ( int i = 0;  i < m_numParams;  i++ )
   {
      m_paramsStart[i] = paramsStart[i];
      m_paramsStep[i] = paramsStep[i];
      if ( m_hasLimits )
      {
	 m_paramsLimitsLow[i] = paramsLimitsLow[i];
	 m_paramsLimitsHigh[i] = paramsLimitsHigh[i];
      }

      sprintf( parName, "P%d", i );
      if ( m_hasLimits )
	 addInitialParameter( parName, m_paramsStart[i], m_paramsStep[i],
			      m_paramsLimitsLow[i], m_paramsLimitsHigh[i] );
      else
	 addInitialParameter( parName, m_paramsStart[i], m_paramsStep[i] );
   }
}

// HIHistFitFunc::HIHistFitFunc( const HIHistFitFunc& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIHistFitFunc::~HIHistFitFunc()
{
   delete [] m_paramsStart;
   delete [] m_paramsStep;
   if ( m_hasLimits )
   {
      delete [] m_paramsLimitsLow;
      delete [] m_paramsLimitsHigh;
   }
}

//
// assignment operators
//
// const HIHistFitFunc& HIHistFitFunc::operator=( const HIHistFitFunc& rhs )
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

double
HIHistFitFunc::func( double x )
{
   double* params = new double[m_numParams];
   for ( int i = 0;  i < m_numParams;  i++ )
      params[i] = parameter(i).value();
   
   double retVal = func( x, params );

   delete [] params;
   return retVal;
}

void
HIHistFitFunc::print( ostream& os )
{
   for ( unsigned int i = 0;  i < m_numParams;  i++ )
   {
      MIParameter param = parameter( i );
      os << "    " << param.name() << " = " << param.value() << " +/- ";
      if ( m_useMinos )
	 os << param.posErr() << " above and " << param.negErr()
	    << " below." << endl;
      else
	 os << param.parabolicErr() << "." << endl;
   }

   os << "chi^2 / ndof = " << chisq() << " / " << degreesOfFreedom();
   if ( degreesOfFreedom() == 0 )
      os << " = Inf" << endl;
   else
      os << " = " << ( chisq() / double( degreesOfFreedom() ) ) << endl;
}

DABoolean
HIHistFitFunc::fit()
{
   if ( histogram_empty() ) return false;

   MinuitInterface* mi = MinuitInterface::instance();
   mi->setDiagLevel( MinuitInterface::kMute );
   mi->loadFcn( *this );

   for ( int i = 0;  i < m_numParams;  i++ )
   {
      mi->changeParameterStart( i, m_paramsStart[i] );
      mi->changeParameterStep( i, m_paramsStep[i] );
     if ( m_hasLimits )
	 mi->changeParameterLimits(
	    i, m_paramsLimitsLow[i], m_paramsLimitsHigh[i] );
   }

   mi->runMigrad();
   if ( m_useMinos )
      mi->runMinos();

   report( INFO, kFacilityString )
      << "Histogram " << m_histogram->id() << " fit:" << endl;
   print( report( INFO, kFacilityString ) );

   MIStats::errstat status = mi->minuitStats().errorMatrixStatus();
//     return ( status == MIStats::kAccurate  ||
//  	    status == MIStats::kDiagonalApproximation );
   return status == MIStats::kAccurate;
}

HIHist*
HIHistFitFunc::histogram()
{
   return m_histogram;
}

HIHistFitFunc::HistogramType
HIHistFitFunc::histogramType()
{
   return m_type;
}

DABoolean
HIHistFitFunc::histogram_empty()
{
   for ( unsigned int index = m_domainStart;  index <= m_domainEnd;  index++ )
   {
      switch ( m_type )
      {
	 case kHist1D:
	    if ( ((HIHist1D*) (m_histogram))->channelValue( index ) != 0. )
	       return false;
	    break;

	 case kHistProf:
	    if ( ((HIHistProf*) (m_histogram))->channelValue( index ) )
	       return false;
	    break;
      } // end switch between allowed histogram types
   }
   return true;
}

unsigned int
HIHistFitFunc::numParams()
{
   return m_numParams;
}

MIParameter
HIHistFitFunc::parameter( unsigned int paramNumber )
{
   MinuitInterface* mi = MinuitInterface::instance();
   if ( mi->fcn() != this )
      report( EMERGENCY, kFacilityString )
	 << "returning parameter for some other fit function!"
	 << endl;
   assert( paramNumber >= 0  &&  paramNumber < m_numParams );
   return mi->parameter( paramNumber );
}

void
HIHistFitFunc::setParamStart( double* start )
{
   delete [] m_paramsStart;
   m_paramsStart = new double[m_numParams];

   for ( int i = 0;  i < m_numParams;  i++ )
      m_paramsStart[i] = start[i];
}

void
HIHistFitFunc::setParamStartToFitted()
{
   delete [] m_paramsStart;
   m_paramsStart = new double[m_numParams];

   for ( int i = 0;  i < m_numParams;  i++ )
      m_paramsStart[i] = parameter( i ).value();
}

void
HIHistFitFunc::setParamStep( double* step )
{
   delete [] m_paramsStep;
   m_paramsStep = new double[m_numParams];

   for ( int i = 0;  i < m_numParams;  i++ )
      m_paramsStep[i] = step[i];
}

DABoolean
HIHistFitFunc::hasLimits()
{
   return m_hasLimits;
}

void
HIHistFitFunc::setParamLimits( unsigned int paramNumber,
			       double low, double high )
{
   assert( paramNumber > 0  &&  paramNumber < m_numParams );
   assert( m_hasLimits );

   m_paramsLimitsLow[paramNumber] = low;
   m_paramsLimitsHigh[paramNumber] = high;
}

unsigned int
HIHistFitFunc::domainIndexStart()
{
   return m_domainStart;
}

unsigned int
HIHistFitFunc::domainIndexEnd()
{
   return m_domainEnd;
}

double
HIHistFitFunc::domainXStart()
{
   switch ( m_type )
   {
      case kHist1D:
	 return ((HIHist1D*) (m_histogram))->channelLowEdge( m_domainStart );
      case kHistProf:
	 return ((HIHistProf*) (m_histogram))->channelLowEdge( m_domainStart );
   }
}

double
HIHistFitFunc::domainXEnd()
{
   double low, center;
   switch ( m_type )
   {
      case kHist1D:
	 low = ((HIHist1D*) (m_histogram))->channelLowEdge( m_domainEnd );
	 center = ((HIHist1D*) (m_histogram))->channelCenter( m_domainEnd );
	 break;
      case kHistProf:
	 low = ((HIHistProf*) (m_histogram))->channelLowEdge( m_domainEnd );
	 center = ((HIHistProf*) (m_histogram))->channelCenter( m_domainEnd );
	 break;
   }
   return center + ( center - low );
}

void
HIHistFitFunc::setDomainFull()
{
   m_domainStart = 1;
   m_domainEnd = m_numberOfChannels;
}

void
HIHistFitFunc::setDomain( int indexStart, int indexEnd )
{
   assert( indexStart != 0  &&  indexEnd != 0 );

   if ( indexStart > 0 )
      m_domainStart = indexStart;
   else
      m_domainStart = m_numberOfChannels + 1 + indexStart;

   if ( indexEnd > 0 )
      m_domainEnd = indexEnd;
   else
      m_domainEnd = m_numberOfChannels + 1 + indexEnd;

   assert( m_domainStart <= m_domainEnd );
}

void
HIHistFitFunc::setDomain( double xStart, double xEnd )
{
   m_domainStart = m_numberOfChannels;
   m_domainEnd = m_numberOfChannels;
   assert( xStart < domainXStart() );

   m_domainStart = 1;
   m_domainEnd = 1;
   assert( xEnd > domainXEnd() );

   setDomainFull();

   while ( domainXStart() < xStart  ||  domainXEnd() > xEnd )
   {
      if ( domainXStart() < xStart )
	 m_domainStart++;

      if ( domainXEnd() > xEnd )
	 m_domainEnd--;
   }   

   if ( m_domainStart != 1 )
      m_domainStart--;

   if ( m_domainEnd != m_numberOfChannels )
      m_domainEnd++;

   assert( m_domainStart < m_domainEnd );
}

DABoolean
HIHistFitFunc::useMinos()
{
   return m_useMinos;
}

void
HIHistFitFunc::setUseMinos( DABoolean useMinos )
{
   m_useMinos = useMinos;
}

HepMatrix
HIHistFitFunc::covarianceMatrix()
{
   MinuitInterface* mi = MinuitInterface::instance();
   if ( mi->fcn() != this )
      report( EMERGENCY, kFacilityString )
	 << "returning covarianceMatrix for some other fit function!"
	 << endl;
   return mi->covarianceMatrix();
}

MIStats
HIHistFitFunc::minuitStats()
{
   MinuitInterface* mi = MinuitInterface::instance();
   if ( mi->fcn() != this )
      report( EMERGENCY, kFacilityString )
	 << "returning minuitStats for some other fit function!"
	 << endl;
   return mi->minuitStats();
}

double
HIHistFitFunc::chisq()
{
   MinuitInterface* mi = MinuitInterface::instance();
   if ( mi->fcn() != this )
      report( EMERGENCY, kFacilityString )
	 << "returning chisq for some other fit function!"
	 << endl;
   return mi->minuitStats().functionMinimum();
}

unsigned int
HIHistFitFunc::numPointsUsed()
{
   return m_numPointsUsed;
}

unsigned int
HIHistFitFunc::degreesOfFreedom()
{
   int dof = m_numPointsUsed;
   dof -= m_numParams;
   if ( dof > 0 )
      return dof;
   else
      return 0.;
}

double
HIHistFitFunc::iterate( double* values )
{
   double chisq = 0.;
   m_numPointsUsed = 0;

   for ( unsigned int index = m_domainStart;  index <= m_domainEnd;  index++ )
   {
      double x, y, error;
      switch ( m_type )
      {
	 case kHist1D:
	    x = ((HIHist1D*) (m_histogram))->channelCenter( index );
	    y = ((HIHist1D*) (m_histogram))->channelValue( index );
	    error = sqrt( y );
	    break;

	 case kHistProf:
	    x = ((HIHistProf*) (m_histogram))->channelCenter( index );
	    y = ((HIHistProf*) (m_histogram))->channelValue( index );
	    error = ((HIHistProf*) (m_histogram))->channelError( index );
	    break;
      } // end switch between allowed histogram types

      if ( error != 0. )
      {
	 double funcVal = func( x, values );
	 chisq += sqr( funcVal - y ) / error;
	 m_numPointsUsed++;
      }
   } // end loop over histogram index
   return chisq;
}

//
// const member functions
//

//
// static member functions
//
