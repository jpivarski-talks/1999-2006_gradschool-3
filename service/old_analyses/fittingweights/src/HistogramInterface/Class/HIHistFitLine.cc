// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistFitLine
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
#include "HistogramInterface/HIHistFitLine.h"


//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HIHistFitLine" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: skeleton.cc,v 1.6 2001/01/03 16:25:23 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIHistFitLine::HIHistFitLine()
{
}

HIHistFitLine::HIHistFitLine(
   HIHist* histogram, HistogramType type,
   double* paramsStart, double* paramsStep,
   DABoolean hasLimits, double* paramsLimitsLow, double* paramsLimitsHigh,
   DABoolean useMinos )
   : HIHistFitFunc( histogram, type, kNumParams, paramsStart, paramsStep,
		    hasLimits, paramsLimitsLow, paramsLimitsHigh, useMinos )
{
}

// HIHistFitLine::HIHistFitLine( const HIHistFitLine& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIHistFitLine::~HIHistFitLine()
{
}

//
// assignment operators
//
// const HIHistFitLine& HIHistFitLine::operator=( const HIHistFitLine& rhs )
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
HIHistFitLine::constant()
{
   return parameter( kConstant ).value();
}

double
HIHistFitLine::slope()
{
   return parameter( kSlope ).value();
}

double
HIHistFitLine::func( double x, double* params )
{
   assert( numParams() == kNumParams );
   const double constant = params[kConstant];
   const double slope = params[kSlope];

   return constant + slope * x;
}

//
// const member functions
//

//
// static member functions
//
