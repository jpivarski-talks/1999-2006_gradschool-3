// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HINumber
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Fri Sep  3 13:31:57 EDT 1999
// $Id: HINumber.cc,v 1.2 1999/10/07 18:05:01 hufnagel Exp $
//
// Revision history
//
// $Log: HINumber.cc,v $
// Revision 1.2  1999/10/07 18:05:01  hufnagel
// Numbers now use float instead of double
//
// Revision 1.1  1999/09/07 18:53:23  mkl
// new histogram interface
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HistogramInterface/HINumber.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HINumber" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HINumber.cc,v 1.2 1999/10/07 18:05:01 hufnagel Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HINumber::HINumber( const string& iName,
		    float         iValue )
   : m_name( iName ),
     m_value( iValue ),
     m_isInitialized( true )
{
}

// HINumber::HINumber( const HINumber& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HINumber::~HINumber()
{
}

//
// assignment operators
//
// const HINumber& HINumber::operator=( const HINumber& rhs )
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
void
HINumber::update( float iEntry )
{
   m_value = iEntry;
   m_isInitialized = true;
}

void 
HINumber::reset()
{
   m_value = 0;
   m_isInitialized = false;
}

//
// const member functions
//

//
// static member functions
//
