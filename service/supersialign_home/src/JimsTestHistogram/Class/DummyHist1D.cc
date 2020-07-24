// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyHist1D
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 23:05:44 EST 1999
// $Id: DummyHist1D.cc,v 1.3 2000/06/19 18:46:56 bkh Exp $
//
// Revision history
//
// $Log: DummyHist1D.cc,v $
// Revision 1.3  2000/06/19 18:46:56  bkh
// Implement new virtual fcns
//
// Revision 1.2  1999/12/23 23:30:08  cdj
// updated to new histogram interface
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

// user include files
#include "Experiment/report.h"
#include "DummyHistogram/DummyHist1D.h"

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

static const char* const kFacilityString = "DummyHistogram.DummyHist1D" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DummyHist1D.cc,v 1.3 2000/06/19 18:46:56 bkh Exp $";
static const char* const kTagString = "$Name:  $";

inline DH1D_sqr( double x ) { return x * x; }

//
// static data member definitions
//

//
// constructors and destructor
//
DummyHist1D::DummyHist1D( unsigned int  iId,
			  const string& iTitle,
			  unsigned int  iNChannels,
			  float         iLowEdge,
			  float         iHighEdge,
			  float         iBitsPerChannel) :
   HIHist1D( iId,
	     iTitle,
	     iNChannels,
	     iLowEdge,
	     iHighEdge,
	     iBitsPerChannel),
   m_underflow( 0. ),
   m_overflow( 0. )
{
   // I'm going to ignore iBitsPerChannel. They're all doubles.
   for ( int i = 0;  i < iNChannels;  i++ ) {
      m_data.push_back( 0. );
      m_errors.push_back( 0. );
   }
}

// DummyHist1D::DummyHist1D( const DummyHist1D& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DummyHist1D::~DummyHist1D()
{
}

//
// assignment operators
//
// const DummyHist1D& DummyHist1D::operator=( const DummyHist1D& rhs )
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
// Fill a 1D histogram
void DummyHist1D::fill( float iEntry,
			float iWeight )
{
   fillChannelValue( (unsigned int)( floor(
      ( iEntry - lowEdge() ) / ( highEdge() - lowEdge() )
      * double( nChannels() ) ) ),
		     iWeight );
}

// Scale
void DummyHist1D::scale( const float iScale )
{
   for ( int i = 0;  i < nChannels();  i++ ) {
      m_data[i] *= iScale;
      m_errors[i] *= iScale;
   }
}

// Add two 1D histograms
void DummyHist1D::add( const HIHist1D& iHist1,
		       const HIHist1D& iHist2,
		       const float iScale1,
		       const float iScale2 )
{
   if ( domainsMatch( iHist1, iHist2 ) ) {
      m_data.clear();
      m_errors.clear();
      m_overflow = 0.;
      m_underflow = 0.;

      for ( int i = 1;  i <= nChannels();  i++ ) {
	 m_data.push_back( iScale1*iHist1.channelValue(i) + iScale2*iHist2.channelValue(i) );
	 m_errors.push_back( sqrt( DH1D_sqr( iScale1*iHist1.channelError(i) ) +
				   DH1D_sqr( iScale2*iHist2.channelError(i) ) ) );
      } // end loop over channels
   } // end if domains match
}

// Subtract two 1D histograms
void DummyHist1D::sub( const HIHist1D& iHist1,
		       const HIHist1D& iHist2,
		       const float iScale1,
		       const float iScale2 )
{
   if ( domainsMatch( iHist1, iHist2 ) ) {
      m_data.clear();
      m_errors.clear();
      m_overflow = 0.;
      m_underflow = 0.;

      for ( int i = 1;  i <= nChannels();  i++ ) {
	 m_data.push_back( iScale1*iHist1.channelValue(i) - iScale2*iHist2.channelValue(i) );
	 m_errors.push_back( sqrt( DH1D_sqr( iScale1*iHist1.channelError(i) ) +
				   DH1D_sqr( iScale2*iHist2.channelError(i) ) ) );
      } // end loop over channels
   } // end if domains match
}

// Multiply two 1D histograms
void DummyHist1D::mult( const HIHist1D& iHist1,
			const HIHist1D& iHist2,
			const float iScale1,
			const float iScale2 )
{
   if ( domainsMatch( iHist1, iHist2 ) ) {
      m_data.clear();
      m_errors.clear();
      m_overflow = 0.;
      m_underflow = 0.;

      for ( int i = 1;  i <= nChannels();  i++ ) {
	 m_data.push_back( iScale1*iHist1.channelValue(i) * iScale2*iHist2.channelValue(i) );
	 if ( iHist1.channelValue(i) != 0.  &&  iHist2.channelValue(i) != 0. )
	    m_errors.push_back(
	       m_data[i] * sqrt( DH1D_sqr( iHist1.channelError(i) / iHist1.channelValue(i) ) +
				 DH1D_sqr( iHist2.channelError(i) / iHist2.channelValue(i) )   ) );
	 else
	    m_errors.push_back( 0. );
      } // end loop over channels
   } // end if domains match
}

// Divide two 1D histograms
void DummyHist1D::div( const HIHist1D& iHist1,
		       const HIHist1D& iHist2,
		       const float iScale1,
		       const float iScale2 )
{
   if ( domainsMatch( iHist1, iHist2 ) ) {
      m_data.clear();
      m_errors.clear();
      m_overflow = 0.;
      m_underflow = 0.;

      for ( int i = 1;  i <= nChannels();  i++ ) {
	 if ( iScale2*iHist2.channelValue(i) != 0. )
	    m_data.push_back( iScale1*iHist1.channelValue(i) / iScale2*iHist2.channelValue(i) );
	 else
	    m_data.push_back( 0. );

	 if ( iHist1.channelValue(i) != 0.  &&  iHist2.channelValue(i) != 0. )
	    m_errors.push_back(
	       m_data[i] * sqrt( DH1D_sqr( iHist1.channelError(i) / iHist1.channelValue(i) ) +
				 DH1D_sqr( iHist2.channelError(i) / iHist2.channelValue(i) )   ) );
	 else
	    m_errors.push_back( 0. );
      } // end loop over channels
   } // end if domains match
}

void DummyHist1D::copy( const HIHist1D& rOriginal )
{
   m_data.clear();
   m_errors.clear();
   m_overflow = rOriginal.overflow();
   m_underflow = rOriginal.underflow();
   
   for ( int i = 1;  i <= nChannels();  i++ ) {
      m_data.push_back( rOriginal.channelValue(i) );
      m_errors.push_back( rOriginal.channelError(i) );
   }
}

void DummyHist1D::reset()
{
   m_overflow = 0.;
   m_underflow = 0.;
   for ( int i = 1;  i <= nChannels();  i++ ) {
      m_data[i-1] = 0.;
      m_errors[i-1] = 0.;
   }
}

void DummyHist1D::fillChannelValue( unsigned int iChannel, float iWeight )
{
   if ( iChannel < 1 )
      m_underflow += iWeight;
   else if ( iChannel > nChannels() )
      m_overflow += iWeight;
   else {
      m_data[iChannel-1] += iWeight;
      m_errors[iChannel-1] = sqrt( m_data[iChannel] );
   }
}

void 
DummyHist1D::setErrors( const float* errors )
{
   for ( int i = 1;  i <= nChannels();  i++ )
      m_errors[i-1] = errors[i-1];
}
//
// const member functions
//
float DummyHist1D::channelValue( unsigned int iChannel ) const
{
   return float( m_data[iChannel-1] );
}

float DummyHist1D::channelError( unsigned int iChannel ) const
{
   return float( m_errors[iChannel-1] );
}

DABoolean DummyHist1D::domainsMatch( const HIHist1D& first,
				     const HIHist1D& second )
{
   if ( first.nChannels() != nChannels()     ||
	second.nChannels() != nChannels()    ||
	first.lowEdge() != lowEdge()         ||
	second.lowEdge() != lowEdge()        ||
	first.highEdge() != highEdge()       ||
	second.highEdge() != highEdge()         ) {
      report( ERROR, kFacilityString )
	 << "I (" << title() << " (" << id() << ")) cannot add the two histograms "
	 << first.title() << " (" << first.id() << ") and "
	 << second.title() << " (" << second.id() << ") because their bounds are:" << endl;
      report( ERROR, kFacilityString )
	 << "    " << first.lowEdge() << " to " << first.highEdge()
	 << " in " << first.nChannels() << " steps and" << endl;
      report( ERROR, kFacilityString )
	 << "    " << second.lowEdge() << " to " << second.highEdge()
	 << " in " << second.nChannels() << " steps and my bounds are:" << endl;
      report( ERROR, kFacilityString )
	 << "    " << lowEdge() << " to " << highEdge()
	 << " in " << nChannels() << " steps" << endl;
      return false;
   } else return true;
}

float 
DummyHist1D::sumWithinLimits() const 
{
   double sum( 0. );
   for ( int i = 1;  i <= nChannels();  i++ )
      sum += channelValue(i);
   return float( sum );
}

float 
DummyHist1D::maxValue()        const
{
   if ( nChannels() == 0 ) return 0.;

   double max( channelValue(1) );
   for ( int i = 2;  i <= nChannels();  i++ )
      if ( max < channelValue(i) ) max = channelValue(i);

   return max;
}

float 
DummyHist1D::minValue()        const
{
   if ( nChannels() == 0 ) return 0.;

   double min( channelValue(1) );
   for ( int i = 2;  i <= nChannels();  i++ )
      if ( min > channelValue(i) ) min = channelValue(i);

   return min;
}

float 
DummyHist1D::underflow() const
{
   return float( m_underflow );
}

float 
DummyHist1D::overflow()  const
{
   return float( m_overflow );
}

//
// static member functions
//
