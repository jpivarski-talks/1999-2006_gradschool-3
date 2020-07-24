// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHist1D
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Thu Jan 28 10:33:42 EST 1999
// $Id: HIHist1D.cc,v 1.5 2000/06/20 20:29:37 bkh Exp $
//
// Revision history
//
// $Log: HIHist1D.cc,v $
// Revision 1.5  2000/06/20 20:29:37  bkh
// Add new virtual fcns
//
// Revision 1.4  2000/02/15 16:16:35  cdj
// arithmetic operations now use expression templates
//
// Revision 1.3  1999/12/23 23:25:22  cdj
// added arithmetic operations on histograms
//
// Revision 1.2  1999/09/07 18:53:19  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:04  cdj
// first submission
//
#include "Experiment/Experiment.h"
#include "Experiment/types.h"

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
#include "HistogramInterface/HIHist1D.h"

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

static const char* const kFacilityString = "HistogramInterface.HIHist1D" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIHist1D.cc,v 1.5 2000/06/20 20:29:37 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIHist1D::HIHist1D( 
   Count         iId,
   const string& iTitle,
   unsigned int  iNChannels,
   float         iLowEdge,
   float         iHighEdge,
   float         iBitsPerChannel ) :
    HIHist( iId,
	    iTitle ),
  m_nchannels( iNChannels ),
  m_lowedge( iLowEdge ) ,
  m_highedge( iHighEdge ) , 
  m_bitsperchannel( iBitsPerChannel )

{
}

// Modifiers
// 
//
// Identifiers
//
// HIHist1D::HIHist1D( const HIHist1D& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }


HIHist1D::~HIHist1D()
{
}

//
// assignment operators
//
//const HIHist1D& HIHist1D::operator=( const HIHist1D& rhs )
//{
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
//}

const HIHist1D&
HIHist1D::operator=( const HIHistHolder<HIHist1D>& rHistHolder )
{
   float scaleValue = rHistHolder.scalar();

   for ( int channel = 1; 
	 channel <= nChannels(); 
	 channel++ )
   { 
      fillChannelValue( channel, 
			scaleValue*rHistHolder.execute( channel ) ); 
   }
   float* errorArray = new float[nChannels()];
   
   for ( int channelError = 1; 
	 channelError <= nChannels(); 
	 channelError++ )
   { 
      errorArray[channelError-1] = 
	 scaleValue*rHistHolder.executeError( channelError );
   }
   
   setErrors( errorArray );
   
   delete [] errorArray; 

   return *this;
}

//
// member functions
//

//
// const member functions
//

unsigned int HIHist1D::nChannels ( void ) const 
{
  return ( m_nchannels ) ;
}
//

float HIHist1D::lowEdge ( void ) const
{
  return ( m_lowedge ) ;
}
//

float HIHist1D::highEdge ( void ) const
{
  return ( m_highedge ) ;
}
//

float HIHist1D::bitsPerChannel ( void ) const
{
  return ( m_bitsperchannel ) ;
}


float HIHist1D::channelLowEdge( unsigned int iChannel ) const
{
  return ( 0 < m_nchannels ? 
	   ( m_lowedge +
	     ( iChannel - 1 )*( m_highedge - m_lowedge )/(1.*m_nchannels) ) :
	   m_lowedge ) ;
}

float HIHist1D::channelCenter( unsigned int iChannel ) const
{
  return ( 0 < m_nchannels ? 
	   ( m_lowedge +
	     ( iChannel - 0.5 )*( m_highedge - m_lowedge )/(1.*m_nchannels) ) :
	   m_lowedge ) ;
}

//
// static member functions
//








