// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistFitGauss
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
#include "HistogramInterface/HIHistFitGauss.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HIHistFitGauss" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIHistFitGauss::HIHistFitGauss()
{
}

HIHistFitGauss::HIHistFitGauss(
   HIHist* histogram, HistogramType type,
   double* paramsStart, double* paramsStep,
   DABoolean hasLimits, double* paramsLimitsLow, double* paramsLimitsHigh,
   DABoolean useMinos )
   : HIHistFitFunc( histogram, type, kNumParams, paramsStart, paramsStep,
		    hasLimits, paramsLimitsLow, paramsLimitsHigh, useMinos )
{
}

// HIHistFitGauss::HIHistFitGauss( const HIHistFitGauss& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIHistFitGauss::~HIHistFitGauss()
{
}

//
// assignment operators
//
// const HIHistFitGauss& HIHistFitGauss::operator=( const HIHistFitGauss& rhs )
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
HIHistFitGauss::area()
{
   return parameter( kArea ).value();
}

double
HIHistFitGauss::mean()
{
   return parameter( kMean ).value();
}

double
HIHistFitGauss::sigma()
{
   return parameter( kSigma ).value();
}

double
HIHistFitGauss::func( double x, double* params )
{
   assert( numParams() == kNumParams );
   const double area = params[kArea];
   const double mean = params[kMean];
   const double sigma = params[kSigma];

   if ( sigma == 0. ) return 0.;

   return area * exp( -sqr( ( x - mean ) / sigma ) / 2. )
      / sqrt( 2. * 3.1415926 ) / sigma;
}

//
// const member functions
//

//
// static member functions
//
