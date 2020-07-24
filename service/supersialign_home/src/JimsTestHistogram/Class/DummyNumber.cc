// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyNumber
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Thu Sep  2 16:53:46 EDT 1999
// $Id: DummyNumber.cc,v 1.3 1999/10/07 18:53:54 hufnagel Exp $
//
// Revision history
//
// $Log: DummyNumber.cc,v $
// Revision 1.3  1999/10/07 18:53:54  hufnagel
// added name method for Numbers
//
// Revision 1.2  1999/10/07 18:06:27  hufnagel
// Numbers now use float instead of double
//
// Revision 1.1  1999/09/06 21:47:25  mkl
// move to new histogramming interface: dummy implementation
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
#include "DummyHistogram/DummyNumber.h"

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

static const char* const kFacilityString = "DummyHistogram.DummyNumber" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DummyNumber.cc,v 1.3 1999/10/07 18:53:54 hufnagel Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DummyNumber::DummyNumber( const string& iName,
			  float         iValue )
   : HINumber( iName, iValue )
{
}

// DummyNumber::DummyNumber( const DummyNumber& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DummyNumber::~DummyNumber()
{
}

//
// assignment operators
//
// const DummyNumber& DummyNumber::operator=( const DummyNumber& rhs )
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
