// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIInitialParameter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Thu Nov 18 16:13:40 EST 1999
// $Id: MIInitialParameter.cc,v 1.1.1.1 1999/12/09 15:34:51 lyon Exp $
//
// Revision history
//
// $Log: MIInitialParameter.cc,v $
// Revision 1.1.1.1  1999/12/09 15:34:51  lyon
// Imported MinuitInterface sources
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
#include "MinuitInterface/MIInitialParameter.h"

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

static const char* const kFacilityString = "MinuitInterface.MIInitialParameter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MIInitialParameter.cc,v 1.1.1.1 1999/12/09 15:34:51 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

// Default constructor
MIInitialParameter::MIInitialParameter() :
  m_name(0),
  m_start(0.0),
  m_step(0.0),
  m_lowLimit(0.0),
  m_highLimit(0.0)
{}

// Main constructor
MIInitialParameter::MIInitialParameter(string name, double start, 
				       double step, double low, double high) :
  m_name(name),
  m_start(start),
  m_step(step),
  m_lowLimit(low),
  m_highLimit(high)
{}

// Constructor with no limits
MIInitialParameter::MIInitialParameter(string name, double start, 
				       double step) :
  m_name(name),
  m_start(start),
  m_step(step),
  m_lowLimit(0.0),
  m_highLimit(0.0)
{}


// MIInitialParameter::MIInitialParameter( const MIInitialParameter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MIInitialParameter::~MIInitialParameter()
{
}

//
// assignment operators
//
// const MIInitialParameter& MIInitialParameter::operator=( const MIInitialParameter& rhs )
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

//
// static member functions
//
