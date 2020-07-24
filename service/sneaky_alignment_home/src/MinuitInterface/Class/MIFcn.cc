// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIFcn
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Thu Nov 18 16:13:51 EST 1999
// $Id: MIFcn.cc,v 1.2 1999/12/12 04:25:36 lyon Exp $
//
// Revision history
//
// $Log: MIFcn.cc,v $
// Revision 1.2  1999/12/12 04:25:36  lyon
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
//#include "Experiment/report.h"
#include "MinuitInterface/MIFcn.h"

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

static const char* const kFacilityString = "MinuitInterface.MIFcn" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MIFcn.cc,v 1.2 1999/12/12 04:25:36 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
MIFcn::MIFcn() :
  m_initialParameters( new STL_VECTOR(MIInitialParameter) )
{}

// MIFcn::MIFcn( const MIFcn& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MIFcn::~MIFcn()
{
  delete m_initialParameters;
  m_initialParameters = 0;
}

//
// assignment operators
//
// const MIFcn& MIFcn::operator=( const MIFcn& rhs )
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

void MIFcn::addInitialParameter(string name, double start, double step)
{
  m_initialParameters->push_back(MIInitialParameter(name, start, step));
}

void MIFcn::addInitialParameter(string name, double start, double step,
				double lowBound, double highBound)
{
  m_initialParameters->push_back(MIInitialParameter(name, start, step,
						    lowBound, highBound));
}


void MIFcn::initialize()
{}

void MIFcn::finalize()
{}

void MIFcn::derivatives(double* values, double* derivatives)
{
  // Don't do anything

  // (you would fill values of derivatives based on values)
  return;
}

void MIFcn::setDiagLevel(int level)
{
  m_diagLevel = level;
}

//
// const member functions
//
const STL_VECTOR(MIInitialParameter)* MIFcn::initialParameters() const
{
  return m_initialParameters;
}

//
// static member functions
//
