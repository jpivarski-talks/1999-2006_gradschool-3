// -*- C++ -*-
//
// Package:    <MinuitInterface>
// Module:     ExampleFcn
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Mon Nov 22 13:43:57 EST 1999
// $Id: ExampleFcn.cc,v 1.1 1999/12/14 05:15:49 lyon Exp $
//
// Revision history
//
// $Log: ExampleFcn.cc,v $
// Revision 1.1  1999/12/14 05:15:49  lyon
// Renamed LeastSquaresFcn to ExampleFcn
//
// Revision 1.3  1999/12/12 21:12:32  lyon
// Added extensive Doxygen documentation
//
// Revision 1.2  1999/12/12 04:25:35  lyon
// Simplified adding parameters
//
// Revision 1.1.1.1  1999/12/09 15:34:51  lyon
// Imported MinuitInterface sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "MinuitInterface/ExampleFcn.h"


// Include this for the diagLevel enum symbols
#include "MinuitInterface/MinuitInterface.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "MinuitInterface.ExampleFcn";

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ExampleFcn.cc,v 1.1 1999/12/14 05:15:49 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
// Main Constructor - take in the list of points and create the parameter
//                    list (note how it calls the constructor for MIFcn).
ExampleFcn::ExampleFcn( int npts, float* x, float* y ) :
  MIFcn(),     // Must call MIFcn's constructor
  m_npts(npts),
  m_x(x),
  m_y(y)
{
  // Make initial parameters
  
  addInitialParameter("constant", 0.0, 0.0001);   // Parameter for kConstant

  addInitialParameter("slope",    0.0, 0.0001);   // Parameter for kSlope
}

//D'tor
ExampleFcn::~ExampleFcn()
{}

//
// member functions
//

// This is the function that does the work. Return the chi^2
double ExampleFcn::iterate(double* values)
{
  // 'values' is an array of doubles of the values of the parameters that 
  // Minuit is trying
  double constant = values[kConstant];
  double slope    = values[kSlope];

  // Calculate the chi squared -- loop over points
  double chisq = 0.0;

  for ( unsigned int i = 0; i < m_npts; i++ )
  {
    double dif = m_y[i] - slope*m_x[i] - constant;
    chisq += dif*dif;
  }

  // Print diagnostics?
  if ( diagLevel() >= MinuitInterface::kIntermediate )
  {
    report(INFO, kFacilityString) << "constant = " << constant << "   "
				  << "slope = " << slope << "   "
				  << "chisq = " << chisq << endl;
  }

  return chisq;
}

//
// const member functions
//

//
// static member functions
//
