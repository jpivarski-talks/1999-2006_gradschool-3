// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistProf
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Tue Feb  2 11:10:39 EST 1999
// $Id: HIHistProf.cc,v 1.3 2001/08/03 17:26:00 bkh Exp $
//
// Revision history
//
// $Log: HIHistProf.cc,v $
// Revision 1.3  2001/08/03 17:26:00  bkh
// Implement channelLoEdge and channelCenter
//
// Revision 1.2  1999/09/07 18:53:21  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:05  cdj
// first submission

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
#include "HistogramInterface/HIHistProf.h"
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

static const char* const kFacilityString = "HistogramInterface.HIHistProf" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIHistProf.cc,v 1.3 2001/08/03 17:26:00 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIHistProf::HIHistProf( Count         iId,
			const         string& iTitle,
			unsigned int  iNChannels,
			float         iLowEdgeX,
			float         iHighEdgeX,
			float         iLowEdgeY,
			float         iHighEdgeY,
			DisplayErrorsAs iDisplayErrorsAs ) :
  HIHist( iId,
	  iTitle ) ,
  m_nchannels( iNChannels ) ,
  m_lowedgeX( iLowEdgeX ) ,
  m_highedgeX( iHighEdgeX ) ,
  m_lowedgeY( iLowEdgeY ) ,
  m_highedgeY( iHighEdgeY ) ,
  m_errordisplay( iDisplayErrorsAs )
{
}

// HIHistProf::HIHistProf( const HIHistProf& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIHistProf::~HIHistProf()
{
}

//
// assignment operators
//
// const HIHistProf& HIHistProf::operator=( const HIHistProf& rhs )
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

//
// const member functions
//
unsigned int HIHistProf::nChannels ( void ) const
{
  return ( m_nchannels ) ;
}
//
float HIHistProf::lowEdgeX ( void ) const
{
  return ( m_lowedgeX ) ;
}
//
float HIHistProf::highEdgeX ( void ) const
{
  return ( m_highedgeX ) ;
}
//
float HIHistProf::lowEdgeY ( void ) const
{
  return ( m_lowedgeY ) ;
}
//
float HIHistProf::highEdgeY ( void ) const
{
  return ( m_highedgeY ) ;
}
//
HIHistProf::DisplayErrorsAs
HIHistProf::errorDisplay ( void ) const
{
  return ( m_errordisplay ) ;
}

float 
HIHistProf::channelLowEdge( unsigned int iChannel ) const
{
  return ( 0 < m_nchannels ? 
	   ( m_lowedgeX +
	     ( iChannel - 1 )*( m_highedgeX - m_lowedgeX )/(1.*m_nchannels) ) :
	   m_lowedgeX ) ;
}

float 
HIHistProf::channelCenter( unsigned int iChannel ) const
{
  return ( 0 < m_nchannels ? 
	   ( m_lowedgeX +
	     ( iChannel - 0.5 )*( m_highedgeX-m_lowedgeX )/(1.*m_nchannels) ) :
	   m_lowedgeX ) ;
}

//
// static member functions
//
