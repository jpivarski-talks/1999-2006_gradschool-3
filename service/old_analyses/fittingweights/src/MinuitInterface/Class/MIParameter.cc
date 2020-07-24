// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIParameter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Thu Nov 18 16:13:46 EST 1999
// $Id: MIParameter.cc,v 1.1.1.1 1999/12/09 15:34:51 lyon Exp $
//
// Revision history
//
// $Log: MIParameter.cc,v $
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
#include "MinuitInterface/MIParameter.h"

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

static const char* const kFacilityString = "MinuitInterface.MIParameter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MIParameter.cc,v 1.1.1.1 1999/12/09 15:34:51 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

// default
MIParameter::MIParameter() :
  m_name(),
  m_start(0.0),
  m_step(0.0),
  m_lowLimit(0.0),
  m_highLimit(0.0),
  m_value(0.0),
  m_posErr(0.0),
  m_negErr(0.0),
  m_parabolicErr(0.0),
  m_globalCorrelation(0.0)
{}

MIParameter::MIParameter(const MIInitialParameter& i) :
  m_name(i.name()),
  m_start(i.start()),
  m_step(i.step()),
  m_lowLimit(i.lowLimit()),
  m_highLimit(i.highLimit()),
  m_value(0.0),
  m_posErr(0.0),
  m_negErr(0.0),
  m_parabolicErr(0.0),
  m_globalCorrelation(0.0)
{}
  

// MIParameter::MIParameter( const MIParameter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MIParameter::~MIParameter()
{}

//
// assignment operators
//
// const MIParameter& MIParameter::operator=( const MIParameter& rhs )
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
void MIParameter::setStart(double start)
{
  m_start = start;
}

void MIParameter::setStep(double step)
{
  m_step = step;
}

void MIParameter::setLimits(double low, double high)
{
  m_lowLimit = low;
  m_highLimit = high;
}

void MIParameter::putResults(double value, double posErr, double negErr,
			     double parabolicErr, double globalCorrelation)
{
  m_value = value;
  m_posErr = posErr;
  m_negErr = negErr;
  m_parabolicErr = parabolicErr;
  m_globalCorrelation = globalCorrelation;
}


//
// const member functions
//

void MIParameter::dump(ostream& out) const
{
  out << "Parameter " << name() << ": " << value() << " +/- " 
      << parabolicErr() << " (parabolic);  [" << posErr()
      << ", " << negErr() << "] (minos)";
}



//
// static member functions
//

//
// friend member functions
//
ostream& operator<<( ostream& s, const MIParameter& aParam ) {

   aParam.dump( s );
   return s;
}
