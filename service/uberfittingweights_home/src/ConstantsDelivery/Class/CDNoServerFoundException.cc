// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      CDNoServerFoundException
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Mon Jun 12 14:25:53 EDT 2000
// $Id: CDNoServerFoundException.cc,v 1.1 2000/06/17 19:21:49 cdj Exp $
//
// Revision history
//
// $Log: CDNoServerFoundException.cc,v $
// Revision 1.1  2000/06/17 19:21:49  cdj
// first submission
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
#include "ConstantsDelivery/CDNoServerFoundException.h"

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

static const char* const kFacilityString = "ConstantsDelivery.CDNoServerFoundException" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CDNoServerFoundException.cc,v 1.1 2000/06/17 19:21:49 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
//CDNoServerFoundException::CDNoServerFoundException()
//{
//}

// CDNoServerFoundException::CDNoServerFoundException( const CDNoServerFoundException& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//CDNoServerFoundException::~CDNoServerFoundException()
//{
//}

//
// assignment operators
//
// const CDNoServerFoundException& CDNoServerFoundException::operator=( const CDNoServerFoundException& rhs )
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
const char*
CDNoServerFoundException::what() const
{
   return m_message.c_str();
}
//
// static member functions
//
