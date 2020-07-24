// -*- C++ -*-
//
// Package:     <MuTracking>
// Module:      MomentumFitFunc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Sat Oct 20 22:08:51 EDT 2001
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
//#include "Experiment/report.h"
#include "MuTracking/MomentumFitFunc.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "MuTracking.MomentumFitFunc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
MomentumFitFunc::MomentumFitFunc()
{
}

MomentumFitFunc::MomentumFitFunc(
   HIHist* histogram, HistogramType type,
   double* paramsStart, double* paramsStep,
   DABoolean hasLimits, double* paramsLimitsLow, double* paramsLimitsHigh,
   DABoolean useMinos )
   : HIHistFitFunc( histogram, type, kNumParams, paramsStart, paramsStep,
		    hasLimits, paramsLimitsLow, paramsLimitsHigh, useMinos )
{
}

// MomentumFitFunc::MomentumFitFunc( const MomentumFitFunc& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MomentumFitFunc::~MomentumFitFunc()
{
}

//
// assignment operators
//
// const MomentumFitFunc& MomentumFitFunc::operator=( const MomentumFitFunc& rhs )
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
MomentumFitFunc::func( double x, double* params )
{
   assert( numParams() == kNumParams );
   const double area1 = params[kArea1];
   const double mean1 = params[kMean1];
   const double sigma1 = params[kSigma1];
   const double area2 = params[kArea2];
   const double mean2 = params[kMean2];
   const double sigma2 = params[kSigma2];

   if ( sigma1 == 0.  ||  sigma2 == 0. ) return 0.;

   return ( area1 * exp( -sqr( ( x - mean1 ) / sigma1 ) / 2. )
	    / sqrt( 2. * 3.1415926 ) / sigma1 ) +
      (  area2 * exp( -sqr( ( x - mean2 ) / sigma2 ) / 2. )
	 / sqrt( 2. * 3.1415926 ) / sigma2 );
}

void
MomentumFitFunc::print( ostream& os )
{
   os << "P0 * exp( -( x - P1 )^2 / P2^2 / 2 ) / sqrt( 2 pi ) / P2 +" << endl
      << "P3 * exp( -( x - P4 )^2 / P5^2 / 2 ) / sqrt( 2 pi ) / P5" << endl;

   HIHistFitFunc::print( os );
}

//
// const member functions
//

//
// static member functions
//
