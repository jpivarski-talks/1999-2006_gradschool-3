// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHist2D
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Tue Feb  2 11:10:28 EST 1999
// $Id: HIHist2D.cc,v 1.5 2000/06/20 20:29:38 bkh Exp $
//
// Revision history
//
// $Log: HIHist2D.cc,v $
// Revision 1.5  2000/06/20 20:29:38  bkh
// Add new virtual fcns
//
// Revision 1.4  2000/02/15 16:16:35  cdj
// arithmetic operations now use expression templates
//
// Revision 1.3  1999/12/23 23:25:23  cdj
// added arithmetic operations on histograms
//
// Revision 1.2  1999/09/07 18:53:20  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:05  cdj
// first submission
//
#include "Experiment/Experiment.h"
#include "Experiment/types.h"

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
#include "HistogramInterface/HIHist2D.h"
// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HIHist2D" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIHist2D.cc,v 1.5 2000/06/20 20:29:38 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

HIHist2D::HIHist2D( Count         iId,
		    const string& iTitle,
		    unsigned int  iNChannelsX,
		    float         iLowEdgeX,
		    float         iHighEdgeX,
		    unsigned int  iNChannelsY,
		    float         iLowEdgeY,
		    float         iHighEdgeY,
		    float         iBitsPerChannel ) :
  HIHist( iId,
	  iTitle ) ,
  m_nchannelsX( iNChannelsX ) ,
  m_lowedgeX( iLowEdgeX ) ,
  m_highedgeX(iHighEdgeX ) ,
  m_nchannelsY(iNChannelsY ) ,
  m_lowedgeY( iLowEdgeY ) ,
  m_highedgeY( iHighEdgeY ) ,
  m_bitsperchannel( iBitsPerChannel ) 

{
}

// HIHist2D::HIHist2D( const HIHist2D& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }


HIHist2D::~HIHist2D()
{
}

//
// assignment operators
//
// const HIHist2D& HIHist2D::operator=( const HIHist2D& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

const HIHist2D&
HIHist2D::operator=( const HIHistHolder<HIHist2D>& rHistHolder )
{
   // Note that if error handling is ever added for 2D, the copy function
   // may not be sufficient.  See HIHist1D.cc for the operator= method that 
   // will definitely work.

   copy( rHistHolder.histogram() );

   scale( rHistHolder.scalar() );

   return *this;
}


//
// member functions
//

//
// const member functions
//

unsigned int HIHist2D::nChannelsX ( void ) const
{
  return ( m_nchannelsX ) ;
}
//

float HIHist2D::lowEdgeX ( void ) const
{
  return ( m_lowedgeX ) ;
}
//

float HIHist2D::highEdgeX ( void ) const 
{
  return ( m_highedgeX ) ;
}
//

unsigned int HIHist2D::nChannelsY ( void ) const 
{
  return ( m_nchannelsY ) ;
}
//

float HIHist2D::lowEdgeY ( void ) const
{
  return ( m_lowedgeY ) ;
}
//

float HIHist2D::highEdgeY ( void ) const
{
  return ( m_highedgeY ) ;
}
//

float HIHist2D::bitsPerChannel ( void ) const
{
  return ( m_bitsperchannel ) ;
}


pair<float,float> 
HIHist2D::channelLoEdge( unsigned int iXChannel ,
			 unsigned int iYChannel  )  const
{
   const double x ( 0 < m_nchannelsX ?
		    ( m_lowedgeX + ( iXChannel - 1 )*
		      ( m_highedgeX - m_lowedgeX )/(1.*m_nchannelsX ) ) :
		    m_lowedgeX ) ;

   const double y ( 0 < m_nchannelsY ?
		    ( m_lowedgeY + ( iYChannel - 1 )*
		      ( m_highedgeY - m_lowedgeY )/(1.*m_nchannelsY ) ) :
		    m_lowedgeY ) ;

   return pair<float,float> (x,y) ;
}

pair<float,float> 
HIHist2D::channelCenter( unsigned int iXChannel ,
			 unsigned int iYChannel  ) const 
{
   const double x ( 0 < m_nchannelsX ?
		    ( m_lowedgeX + ( iXChannel - 0.5 )*
		      ( m_highedgeX - m_lowedgeX )/(1.*m_nchannelsX ) ) :
		    m_lowedgeX ) ;

   const double y ( 0 < m_nchannelsY ?
		    ( m_lowedgeY + ( iYChannel - 0.5 )*
		      ( m_highedgeY - m_lowedgeY )/(1.*m_nchannelsY ) ) :
		    m_lowedgeY ) ;

   return pair<float,float> (x,y) ;
}


//
// static member functions
//



