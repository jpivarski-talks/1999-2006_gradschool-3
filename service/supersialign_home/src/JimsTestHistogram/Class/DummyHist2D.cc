// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyHist2D
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 23:05:48 EST 1999
// $Id: DummyHist2D.cc,v 1.3 2000/06/19 18:46:56 bkh Exp $
//
// Revision history
//
// $Log: DummyHist2D.cc,v $
// Revision 1.3  2000/06/19 18:46:56  bkh
// Implement new virtual fcns
//
// Revision 1.2  1999/12/23 23:30:09  cdj
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
//#include "Experiment/report.h"
#include "DummyHistogram/DummyHist2D.h"

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

static const char* const kFacilityString = "DummyHistogram.DummyHist2D" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DummyHist2D.cc,v 1.3 2000/06/19 18:46:56 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DummyHist2D::DummyHist2D( unsigned int  iId,
			  const string& iTitle,
			  unsigned int  iNChannelsX,
			  float         iLowEdgeX,
			  float         iHighEdgeX,
			  unsigned int  iNChannelsY,
			  float         iLowEdgeY,
			  float         iHighEdgeY,
			  float         iBitsPerChannel) :
  HIHist2D( iId,
	    iTitle,
	    iNChannelsX,
	    iLowEdgeX,
	    iHighEdgeX,
	    iNChannelsY,
	    iLowEdgeY,
	    iHighEdgeY,
	    iBitsPerChannel )
{
}

// DummyHist2D::DummyHist2D( const DummyHist2D& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DummyHist2D::~DummyHist2D()
{
}

//
// assignment operators
//
// const DummyHist2D& DummyHist2D::operator=( const DummyHist2D& rhs )
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
// Fill a 2D histogram
void DummyHist2D::fill( float iXEntry,
			float iYEntry,
			float iWeight )
{
}

//Scale 
void DummyHist2D::scale( const float iScale )
{
}

// Add two 2D histograms
void DummyHist2D::add( const HIHist2D&,
		       const HIHist2D&,
		       const float,
		       const float )
{
}

// Subtract two 2D histograms
void DummyHist2D::sub( const HIHist2D&,
		       const HIHist2D&,
		       const float,
		       const float )
{
}

// Multiply two 2D histograms
void DummyHist2D::mult( const HIHist2D&,
			const HIHist2D&,
			const float,
			const float )
{
}

// Divide two 2D histograms
void DummyHist2D::div( const HIHist2D&,
		       const HIHist2D&,
		       const float,
		       const float )
{
}

void DummyHist2D::copy( const HIHist2D& )
{
}

void DummyHist2D::reset()
{
}

void DummyHist2D::fillChannelValue( unsigned int iChannel, float iWeight )
{
}


//
// const member functions
//
float DummyHist2D::channelValue( unsigned int iChannel ) const
{
  return -1;
}

float 
DummyHist2D::sumWithinLimits() const
{
   return -1. ;
}

float 
DummyHist2D::maxValue()        const
{
   return -1. ;
}

float 
DummyHist2D::minValue()        const
{
   return -1. ;
}

float 
DummyHist2D::underXoverY()    const
{
   return -1. ;
}

float 
DummyHist2D::insideXoverY()   const
{
   return -1. ;
}

float 
DummyHist2D::overXoverY()     const
{
   return -1. ;
}

float 
DummyHist2D::underXinsideY()  const
{
   return -1. ;
}

float 
DummyHist2D::overXinsideY()   const
{
   return -1. ;
}

float 
DummyHist2D::underXunderY()   const
{
   return -1. ;
}

float 
DummyHist2D::insideXunderY()  const
{
   return -1. ;
}

float 
DummyHist2D::overXunderY()    const
{
   return -1. ;
}


//
// static member functions
//



