// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyHistProf
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 23:05:53 EST 1999
// $Id: DummyHistProf.cc,v 1.3 2001/08/03 17:39:16 bkh Exp $
//
// Revision history
//
// $Log: DummyHistProf.cc,v $
// Revision 1.3  2001/08/03 17:39:16  bkh
// Implement pure virtual base class function with dummy
//
// Revision 1.2  2000/06/19 18:46:56  bkh
// Implement new virtual fcns
//
// Revision 1.1.1.1  1999/05/12 00:10:09  billm
// imported DummyHistogram sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "math.h"

// user include files
#include "Experiment/report.h"
#include "DummyHistogram/DummyHistProf.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DummyHistogram.DummyHistProf" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DummyHistProf.cc,v 1.3 2001/08/03 17:39:16 bkh Exp $";
static const char* const kTagString = "$Name:  $";

inline DHP_sqr( double x ) { return x * x; }

//
// static data member definitions
//

//
// constructors and destructor
//
DummyHistProf::DummyHistProf( unsigned int   iId,
			      const string&  iTitle,
			      unsigned int   iNChannels,
			      float          iLowEdgeX,
			      float          iHighEdgeX,
			      float          iLowEdgeY,
			      float          iHighEdgeY,
			      HIHistProf::DisplayErrorsAs iDisplayErrorsAs ) :
  HIHistProf(
	      iId,
	      iTitle,
	      iNChannels,
	      iLowEdgeX,
	      iHighEdgeX,
	      iLowEdgeY,
	      iHighEdgeY,
	      iDisplayErrorsAs ),
  m_underflow( 0. ),
  m_overflow( 0. )
{
   for ( int i = 0;  i < iNChannels;  i++ ) {
      m_population.push_back( 0 );
      m_numerator.push_back( 0. );
      m_denominator.push_back( 0. );
   }
}

// DummyHistProf::DummyHistProf( const DummyHistProf& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DummyHistProf::~DummyHistProf()
{
}

//
// assignment operators
//
// const DummyHistProf& DummyHistProf::operator=( const DummyHistProf& rhs )
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
void DummyHistProf::fill ( float iXEntry,
			   float iYEntry,
			   float iWeight )
{
   if ( iYEntry >= lowEdgeY()  &&  iYEntry <= highEdgeY() )
      fillChannelValue( (unsigned int)( floor(
	 ( iXEntry - lowEdgeX() ) / ( highEdgeX() - lowEdgeX() )
	 * double( nChannels() ) ) ) + 1,
			iYEntry, iWeight );
}
// Add two profile plots
void DummyHistProf::add ( const HIHistProf& iHist1,
			  const HIHistProf& iHist2 )
{
   if ( domainsMatch( iHist1, iHist2 ) ) {
      m_population.clear();
      m_numerator.clear();
      m_denominator.clear();
      m_overflow = 0.;
      m_underflow = 0.;

      for ( int i = 1;  i <= nChannels();  i++ ) {
	 double value = iHist1.channelValue(i) + iHist2.channelValue(i);
	 double error2 = DHP_sqr( iHist1.channelError(i) ) + DHP_sqr( iHist2.channelError(i) );

	 m_population.push_back( 1. );
	 m_numerator.push_back( value / error2 );
	 m_denominator.push_back( 1. / error2 );
      } // end loop over channels
   } // end if domains match
}

// Subtract two profile plots
void DummyHistProf::sub ( const HIHistProf& iHist1,
			  const HIHistProf& iHist2 )
{
   if ( domainsMatch( iHist1, iHist2 ) ) {
      m_population.clear();
      m_numerator.clear();
      m_denominator.clear();
      m_overflow = 0.;
      m_underflow = 0.;

      for ( int i = 1;  i <= nChannels();  i++ ) {
	 double value( iHist1.channelValue(i) - iHist2.channelValue(i) );
	 double error2( DHP_sqr( iHist1.channelError(i) ) + DHP_sqr( iHist2.channelError(i) ) );

	 m_population.push_back( 1. );
	 m_numerator.push_back( value / error2 );
	 m_denominator.push_back( 1. / error2 );
      } // end loop over channels
   } // end if domains match
}

// Multiply two profile plots
void DummyHistProf::mult ( const HIHistProf& iHist1,
			   const HIHistProf& iHist2 )
{
   if ( domainsMatch( iHist1, iHist2 ) ) {
      m_population.clear();
      m_numerator.clear();
      m_denominator.clear();
      m_overflow = 0.;
      m_underflow = 0.;

      for ( int i = 1;  i <= nChannels();  i++ ) {
	 double value( iHist1.channelValue(i) * iHist2.channelValue(i) );
	 double error2;

	 if ( iHist1.channelValue(i) != 0.  &&  iHist2.channelValue(i) != 0. )
	    error2 = DHP_sqr( value ) * (
	       DHP_sqr( iHist1.channelError(i) / iHist1.channelValue(i) ) +
	       DHP_sqr( iHist2.channelError(i) / iHist2.channelValue(i) ) );
	 else
	    error2 = 0.;

	 m_population.push_back( 1. );
	 if ( error2 != 0. ) {
	    m_numerator.push_back( value / error2 );
	    m_denominator.push_back( 1. / error2 );
	 } else {
	    m_numerator.push_back( value );
	    m_denominator.push_back( 0. );
	 }
      } // end loop over channels
   } // end if domains match
}

// Divide two profile plots
void DummyHistProf::div ( const HIHistProf& iHist1,
			  const HIHistProf& iHist2 )
{
   if ( domainsMatch( iHist1, iHist2 ) ) {
      m_population.clear();
      m_numerator.clear();
      m_denominator.clear();
      m_overflow = 0.;
      m_underflow = 0.;

      for ( int i = 1;  i <= nChannels();  i++ ) {
	 double value;
	 double error2;

	 if ( iHist2.channelValue(i) != 0. )
	    value = iHist1.channelValue(i) / iHist2.channelValue(i);
	 else
	    value = 0.;

	 if ( iHist1.channelValue(i) != 0.  &&  iHist2.channelValue(i) != 0. )
	    error2 = DHP_sqr( value ) * (
	       DHP_sqr( iHist1.channelError(i) / iHist1.channelValue(i) ) +
	       DHP_sqr( iHist2.channelError(i) / iHist2.channelValue(i) ) );
	 else
	    error2 = 0.;

	 m_population.push_back( 1. );
	 if ( error2 != 0. ) {
	    m_numerator.push_back( value / error2 );
	    m_denominator.push_back( 1. / error2 );
	 } else {
	    m_numerator.push_back( value );
	    m_denominator.push_back( 0. );
	 }
      } // end loop over channels
   } // end if domains match
}

void DummyHistProf::reset()
{
   m_overflow = 0.;
   m_underflow = 0.;
   for ( int i = 1;  i <= nChannels();  i++ ) {
      m_population[i-1] = 0.;
      m_numerator[i-1] = 0.;
      m_denominator[i-1] = 0.;
   }
}

//
// const member functions
//

float 
DummyHistProf::sumWithinLimits() const
{
   double sum( 0. );
   for ( int i = 1;  i <= nChannels();  i++ )
      sum += channelValue(i);

   return sum;
}

float 
DummyHistProf::maxValue()        const
{
   if ( nChannels() == 0 ) return 0.;

   double max( channelValue(1) );
   for ( int i = 2;  i <= nChannels();  i++ )
      if ( max < channelValue(i) ) max = channelValue(i);

   return max;
}

float 
DummyHistProf::minValue()        const
{
   if ( nChannels() == 0 ) return 0.;

   double min( channelValue(1) );
   for ( int i = 2;  i <= nChannels();  i++ )
      if ( min > channelValue(i) ) min = channelValue(i);

   return min;
}

float 
DummyHistProf::channelValue( unsigned int iChan )        const
{
   if ( m_denominator[iChan-1] != 0. )
      return m_numerator[iChan-1] / m_denominator[iChan-1];
   else
      return m_numerator[iChan-1];
}

float 
DummyHistProf::channelError( unsigned int iChan )        const
{
   if ( m_denominator[iChan-1] != 0. ) {
      if ( errorDisplay() == kErrorOnMean )
	 return sqrt( 1. / m_denominator[iChan-1] );
      else
	 return sqrt( m_population[iChan-1] / m_denominator[iChan-1] );
   } else
      return 0.;
}

void
DummyHistProf::fillChannelValue( unsigned int iChannel, float iYEntry, float iWeight )
{
   if ( iChannel < 1 )
      m_underflow += iWeight;
   else if ( iChannel > nChannels() )
      m_overflow += iWeight;
   else {
      m_population[iChannel-1]++;
      m_numerator[iChannel-1] += iYEntry * iWeight;
      m_denominator[iChannel-1] += iWeight;
   }
}

DABoolean
DummyHistProf::domainsMatch( const HIHistProf& first,
			     const HIHistProf& second )
{
   if ( first.nChannels() != nChannels()     ||
	second.nChannels() != nChannels()    ||
	first.lowEdgeX() != lowEdgeX()         ||
	second.lowEdgeX() != lowEdgeX()        ||
	first.highEdgeX() != highEdgeX()       ||
	second.highEdgeX() != highEdgeX()         ) {
      report( ERROR, kFacilityString )
	 << "I (" << title() << " (" << id() << ")) cannot add the two histograms "
	 << first.title() << " (" << first.id() << ") and "
	 << second.title() << " (" << second.id() << ") because their bounds are:" << endl;
      report( ERROR, kFacilityString )
	 << "    " << first.lowEdgeX() << " to " << first.highEdgeX()
	 << " in " << first.nChannels() << " steps and" << endl;
      report( ERROR, kFacilityString )
	 << "    " << second.lowEdgeX() << " to " << second.highEdgeX()
	 << " in " << second.nChannels() << " steps and my bounds are:" << endl;
      report( ERROR, kFacilityString )
	 << "    " << lowEdgeX() << " to " << highEdgeX()
	 << " in " << nChannels() << " steps" << endl;
      return false;
   } else return true;
}

//
// static member functions
//
