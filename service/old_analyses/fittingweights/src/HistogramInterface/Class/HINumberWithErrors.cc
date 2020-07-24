// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HINumberWithErrors
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Fri Sep  3 13:32:01 EDT 1999
// $Id: HINumberWithErrors.cc,v 1.2 1999/10/07 18:05:02 hufnagel Exp $
//
// Revision history
//
// $Log: HINumberWithErrors.cc,v $
// Revision 1.2  1999/10/07 18:05:02  hufnagel
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
#include "HistogramInterface/HINumberWithErrors.h"

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

static const char* const kFacilityString = "HistogramInterface.HINumberWithErrors" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HINumberWithErrors.cc,v 1.2 1999/10/07 18:05:02 hufnagel Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HINumberWithErrors::HINumberWithErrors( const string&  iName,
					float          iValue,
					float          iPositiveError,
					float          iNegativeError )
   : m_name( iName ),
     m_value( iValue ),
     m_positiveError( iPositiveError ),
     m_negativeError( iNegativeError ),
     m_isInitialized( true )
{
}

// HINumberWithErrors::HINumberWithErrors( const HINumberWithErrors& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HINumberWithErrors::~HINumberWithErrors()
{
}

//
// assignment operators
//
// const HINumberWithErrors& HINumberWithErrors::operator=( const HINumberWithErrors& rhs )
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
HINumberWithErrors::update( float iEntry,
			    float iPosErr,
			    float iNegErr )
{
   m_value         = iEntry;
   m_positiveError = iPosErr;
   m_negativeError = iNegErr;
   m_isInitialized = true;
}

void
HINumberWithErrors::reset()
{
   m_value         = 0;
   m_positiveError = 0;
   m_negativeError = 0;
   m_isInitialized = 0;
}

//
// const member functions
//

//
// static member functions
//





